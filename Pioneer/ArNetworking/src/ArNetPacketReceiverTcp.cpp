/*
Adept MobileRobots Robotics Interface for Applications (ARIA)
Copyright (C) 2004, 2005 ActivMedia Robotics LLC
Copyright (C) 2006, 2007, 2008, 2009, 2010 MobileRobots Inc.
Copyright (C) 2011, 2012, 2013 Adept Technology

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

If you wish to redistribute ARIA under different terms, contact 
Adept MobileRobots for information about a commercial version of ARIA at 
robots@mobilerobots.com or 
Adept MobileRobots, 10 Columbia Drive, Amherst, NH 03031; +1-603-881-7960
*/
#include "Aria.h"
#include "ArExport.h"
#include "ArNetPacketReceiverTcp.h"
#ifndef WIN32
#include "errno.h"
#endif

AREXPORT ArNetPacketReceiverTcp::ArNetPacketReceiverTcp() :

  myState(STATE_SYNC1),
  myProcessPacketCB(NULL),
  myQuiet(false),
  mySocket(NULL),
  myLastPacket(),
  myPacket(),

  myReadBuff(),
  myReadCount(0),
  myReadLength(0),
  myReadCommand(0),
  mySync1(0xF),
  mySync2(0xC),
  myLoggingPrefix("")
{
  memset(myReadBuff, 0, TOTAL_PACKET_LENGTH);
}

AREXPORT ArNetPacketReceiverTcp::~ArNetPacketReceiverTcp()
{

}

/**
   Sets the socket that this receiver will use, note that it does not
   transfer ownership of the socket.  

   @param socket the socket to use for receiving data
**/
AREXPORT void ArNetPacketReceiverTcp::setSocket(ArSocket *socket)
{
  mySocket = socket;
}

AREXPORT void ArNetPacketReceiverTcp::setLoggingPrefix(
	const char *loggingPrefix)
{
  if (loggingPrefix != NULL && loggingPrefix[0] != '\0')
    myLoggingPrefix = loggingPrefix;
  else
    myLoggingPrefix = "";
}

/**
   Gets the socket that the receiver is using, note that it does not
   have ownership of this socket and that whatever created it should.
**/
AREXPORT ArSocket *ArNetPacketReceiverTcp::getSocket(void)
{
  return mySocket;
}

/**
   This reads in the available data in the TCP port so that packets
   can be fetched with getPacket.  If an error occurs that is
   unrecoverable false will be returned which means that the socket
   owner who is calling this should close the socket and clean up.

   @return false on an error that should cause the socket to be closed,
   true with no errors
**/
AREXPORT bool ArNetPacketReceiverTcp::readData(void)
{
  Ret ret;

  while (1)
  {
    ret = readPacket(100);
    if (ret == RET_TIMED_OUT)
    {
      if (!myQuiet)
	ArLog::log(ArLog::Terse, "%sReadTcp timed out",
		   myLoggingPrefix.c_str());
      return true;
    }
    else if (ret == RET_CONN_CLOSED)
    {
      if (!myQuiet)
	ArLog::log(ArLog::Terse, "%sConnection to %s closed",
		   myLoggingPrefix.c_str(), mySocket->getIPString());
      return false;
    }
    else if (ret == RET_FAILED_READ)
    {
      // if neither of these true's trap it the false is returned cause it was some bad error
#ifdef WIN32
      if (WSAGetLastError() == WSAEWOULDBLOCK)
	return true;
      else if (!myQuiet)
	ArLog::log(ArLog::Terse, "%sFailed on read TCP, error %d", myLoggingPrefix.c_str(), WSAGetLastError());
#endif
#ifndef WIN32
      if (errno == EAGAIN)
	return true;
      else if (!myQuiet)
	ArLog::log(ArLog::Terse, "%sFailed on read TCP, error %d", myLoggingPrefix.c_str(), errno);
#endif
      if (!myQuiet)
	ArLog::log(ArLog::Terse, "%sFailed on the tcp read", myLoggingPrefix.c_str());
      return false;
    }
    else if (ret == RET_BAD_PACKET)
    {
      // well... there was a bad checksum, keep going
    }
    else if (ret == RET_GOT_PACKET)
    {
      if (myProcessPacketCB != NULL)
      {
	//printf("Got in a packet of %d\n", myPacket.getCommand());
	myPacket.setPacketSource(ArNetPacket::TCP);
	myProcessPacketCB->invoke(&myPacket);
      }
    }
    else
    {
      if (!myQuiet)
	ArLog::log(ArLog::Terse, "%sArNetPacketReceiverTcp: bad Ret value %d", myLoggingPrefix.c_str(), ret);
      return false;
    }
  }
}

AREXPORT ArNetPacketReceiverTcp::Ret ArNetPacketReceiverTcp::readPacket(int msWait)
{
  long timeToRunFor = -1;
  int numRead = 0;
  bool printing = true;
  int ret = 0;
  unsigned char c = 0;

  ArTime timeDone;
  timeDone.setToNow();
  timeDone.addMSec(msWait);

  //printf("Read packet!\n");
  do
  {
    timeToRunFor = timeDone.mSecTo();
    if (timeToRunFor < 0)
      timeToRunFor = 0;

    if (myState != STATE_ACQUIRE_DATA)
    {
      c = 0;
      if ((ret = mySocket->read((char *)&c, 1, 0/*timeToRunFor*/)) == -1) 
      {
        if (myState == STATE_SYNC1)
        {
          return RET_FAILED_READ;
        }
        else
        {
          //ArUtil::sleep(1);
          continue;
        }
      }
      else if (ret == 0)
      {
        return RET_CONN_CLOSED;
      }
    }

    /*
    if (myState != STATE_ACQUIRE_DATA)
    {
    printf("%d", myState);
    printf(" %d\n", c);
    }
    */
    //else
    //{
    //printf("\n");
    //}
    switch (myState) {
    case STATE_SYNC1:
      if (c == mySync1) // move on, resetting packet
      {
        myState = STATE_SYNC2;
        myPacket.empty();
        myPacket.setLength(0);
        myPacket.uByteToBuf(c);
      }
      else
      {
        if (printing)
          ArLog::log(ArLog::Verbose, "%sBad char in sync1 %d", myLoggingPrefix.c_str(), c);
        return RET_BAD_PACKET;
      }
      break;
    case STATE_SYNC2:
      if (c == mySync2) // move on, adding this byte
      {
        myState = STATE_LENGTH1;
        myPacket.uByteToBuf(c);
      }
      else // go back to beginning, packet hosed
      {
        if (printing)
          ArLog::log(ArLog::Verbose, "%sBad char in sync2 %d, returning to sync1",                              myLoggingPrefix.c_str(), c);
        myState = STATE_SYNC1;
        return RET_BAD_PACKET;
      }
      break;
    case STATE_LENGTH1:
      myState = STATE_LENGTH2;
      myReadLength = ((unsigned int)c & 0xff);
      myPacket.uByteToBuf(c);
      break;
    case STATE_LENGTH2:
      myState = STATE_ACQUIRE_DATA;
      myReadLength += ((unsigned int)c & 0xff) << 8;
      myPacket.uByteToBuf(c);
      myReadCount = 0;
      break;
    case STATE_ACQUIRE_DATA:
      if (myReadLength > ArNetPacket::MAX_LENGTH || 
        myReadLength < myPacket.getHeaderLength() + myPacket.getFooterLength())
      {
        if (!myQuiet)
          ArLog::log(ArLog::Normal, 
                     "%sArNetPacketReceiverTcp::readPacket: bad packet length, it is %d which is more than max length of %d bytes or less than the minimum %d", 
                     myLoggingPrefix.c_str(), myReadLength, 
                     ArNetPacket::MAX_LENGTH,
                     myPacket.getHeaderLength() + myPacket.getFooterLength());
        myState = STATE_SYNC1;
        return RET_BAD_PACKET;
      }

      // here we read until we get as much as we want, OR until
      // we go 100 ms without data... its arbitrary but it doesn't happen often
      // and it'll mean a bad packet anyways
      if (myReadCount < myReadLength - 4)
      {
        numRead = mySocket->read(myReadBuff + myReadCount, 
                                 myReadLength - myReadCount - 4);
        //printf("numRead %d myReadLength %d\n", numRead, myReadLength);
        // trap if it wasn't data
        if (numRead == 0)
          return RET_CONN_CLOSED;
        else if (numRead < 0)
          return RET_FAILED_READ;
        // if it was data add it to our number read
        myReadCount += numRead;
      }
      if (myReadCount > myReadLength - 4)
      {
        if (!myQuiet)
          ArLog::log(ArLog::Terse, 
          "%sRead is greater than it should be at %d > %d", 
          myLoggingPrefix.c_str(), myReadCount, myReadLength - 4);
      }
      if (myReadCount == myReadLength - 4)
      {
        myPacket.dataToBuf(myReadBuff, myReadCount);

        if (myPacket.verifyCheckSum()) 
        {
          myPacket.resetRead();
          // take off the footer from the packets length Variable
          /* put this in if you want to see the packets received
          //printf("Input ");
          myPacket.log();
          */
          // you can also do this next line if you only care about type
          //printf("Input %x\n", myPacket.getCommand());
          //myPacket.log();
          myState = STATE_SYNC1;
          return RET_GOT_PACKET;
        }
        else 
        {
          myPacket.resetRead();
          //if (!myQuiet)
            ArLog::log(ArLog::Normal, 
            "%sArNetPacketReceiverTcp::receivePacket: bad packet, bad checksum on packet %d", myLoggingPrefix.c_str(), myPacket.getCommand());
          myState = STATE_SYNC1;
          return RET_BAD_PACKET;
        }
      }
      break;
    default:
      break;
    }
  } while (timeDone.mSecTo() >= 0 || myState != STATE_SYNC1);

  return RET_TIMED_OUT;

}


/**
   @param functor the callback to use when a packet needs to be processed
**/
AREXPORT void ArNetPacketReceiverTcp::setProcessPacketCB(
	ArFunctor1<ArNetPacket *> *functor)
{
  myProcessPacketCB = functor;
}

/**
   @return the callback used when a packet needs to be processed
**/
AREXPORT ArFunctor1<ArNetPacket *> *ArNetPacketReceiverTcp::getProcessPacketCB(void)
{
  return myProcessPacketCB;
}


