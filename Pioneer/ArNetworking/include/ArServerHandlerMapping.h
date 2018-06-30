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
#ifndef ARSERVERHANDLERMAPPING_H
#define ARSERVERHANDLERMAPPING_H

#include "Aria.h"
#include "ArServerBase.h"
#include "ArServerHandlerCommands.h"

/// Class that handles the starting, stopping, and status of mapping
class ArServerHandlerMapping
{
public:
  /// Constructor
  AREXPORT ArServerHandlerMapping(ArServerBase *server, ArRobot *robot,
				  ArLaser *laser, 
				  const char *baseDirectory = "",
				  const char *tempDirectory = "",
				  bool useReflectorValues = false,
				  ArLaser *laser2 = NULL, 
				  const char *sickSuffix = NULL,
				  const char *sick2Suffix = NULL);
  /// Deconstructor
  AREXPORT virtual ~ArServerHandlerMapping();
  /// Starts a new map unless ones already made
  AREXPORT void serverMappingStart(ArServerClient *client, 
				   ArNetPacket *packet);
  /// Ends a map
  AREXPORT void serverMappingEnd(ArServerClient *client, 
				 ArNetPacket *packet);
  /// Gets the mapping status
  AREXPORT void serverMappingStatus(ArServerClient *client, 
				    ArNetPacket *packet);
  /// Returns if we're mapping or not
  AREXPORT bool isMapping(void);

  /// Gets the filename we're using (make sure the robot's locked)
  AREXPORT const char *getFileName(void);
  /// Gets the mapname we're using (make sure the robot's locked)
  AREXPORT const char *getMapName(void);
  /// Adds the simple commands to the simple command handler passed in
  AREXPORT void addSimpleCommands(ArServerHandlerCommands *handlerCommands);
  /// Adds a loop start to the log
  AREXPORT void simpleLoopStart(ArArgumentBuilder *arg);
  /// Adds a loop end to the log
  AREXPORT void simpleLoopEnd(ArArgumentBuilder *arg);
  /// Adds a string for adding to the log when we start
  AREXPORT void addStringForStartOfLogs(
	  const char *str, ArListPos::Pos position = ArListPos::LAST);
  /// Removes a string for adding to the log when we start
  AREXPORT void remStringForStartOfLogs(const char *str);
  /// Adds a tag to the logg (has robot pose and stuff) (make sure the
  /// robot's locked)
  AREXPORT void addTagToLog(const char *str);
  /// Adds an info to the log (has no robot pose) (make sure the
  /// robot's locked)
  AREXPORT void addInfoToLog(const char *str);
  /// Adds a new type of location data to the logger
  AREXPORT bool addLocationData(
	  const char *name, 
	  ArRetFunctor2<int, ArTime, ArPose *> *functor);
  /// Adds a callback for the start of mapping
  AREXPORT void addMappingStartCallback(
	  ArFunctor *functor, ArListPos::Pos position = ArListPos::LAST);
  /// Removes a callback for the start of mapping
  AREXPORT void remMappingStartCallback(ArFunctor *functor);
  /// Adds a callback for the end of mapping
  AREXPORT void addMappingEndCallback(
	  ArFunctor *functor, ArListPos::Pos position = ArListPos::LAST);
  /// Removes a callback for the end of mapping
  AREXPORT void remMappingEndCallback(ArFunctor *functor);
  /// Adds a callback to be called before moving from temp dir to base dir
  AREXPORT void addPreMoveCallback(
	  ArFunctor *functor, ArListPos::Pos position = ArListPos::LAST);
  /// Removes a callback to be called before moving from temp dir to base dir
  AREXPORT void remPreMoveCallback(ArFunctor *functor);
  /// Adds a callback to be called after moving from temp dir to base dir
  AREXPORT void addPostMoveCallback(
	  ArFunctor *functor, ArListPos::Pos position = ArListPos::LAST);
  /// Removes a callback to be called after moving from temp dir to base dir
  AREXPORT void remPostMoveCallback(ArFunctor *functor);
  /// The packet handler for starting/stopping scans from the lcd
  AREXPORT bool packetHandler(ArRobotPacket *packet);
  /// Get location data map (mostly for internal things)
  AREXPORT const std::map<std::string, ArRetFunctor2<int, ArTime, ArPose *> *, 
		 ArStrCaseCmpOp> *getLocationDataMap(void);
  /// A method to add the strings for the start of a log just straight to an ArMap
  AREXPORT void addStringsForStartOfLogToMap(ArMap *arMap);
protected:
  ArServerBase *myServer;
  ArRobot *myRobot;
  ArLaser *myLaser;
  ArLaser *myLaser2;
  bool myUseReflectorValues;
  

  std::list<ArFunctor *> myMappingStartCallbacks;
  std::list<ArFunctor *> myMappingEndCallbacks;
  std::list<ArFunctor *> myPreMoveCallbacks;
  std::list<ArFunctor *> myPostMoveCallbacks;
  std::list<std::string> myStringsForStartOfLog;
  ArLaserLogger *myLaserLogger;
  ArLaserLogger *myLaserLogger2;
  std::string myMapName;
  std::string myFileName;
  std::string myFileName2;
  std::string myBaseDirectory;
  std::string myTempDirectory;
  
  std::string mySuffix;
  std::string mySuffix2;

  std::map<std::string, ArRetFunctor2<int, ArTime, ArPose *> *, 
	   ArStrCaseCmpOp> myLocationDataMap;

  ArFunctor2C<ArServerHandlerMapping, ArServerClient *, ArNetPacket *> myMappingStartCB;
  ArFunctor2C<ArServerHandlerMapping, ArServerClient *, ArNetPacket *> myMappingEndCB;
  ArFunctor2C<ArServerHandlerMapping, ArServerClient *, ArNetPacket *> myMappingStatusCB;
  ArServerHandlerCommands *myHandlerCommands;
  ArRetFunctor1C<bool, ArServerHandlerMapping, ArRobotPacket *> myPacketHandlerCB;
  ArFunctor1C<ArServerHandlerMapping, ArArgumentBuilder *> myLoopStartCB;
  ArFunctor1C<ArServerHandlerMapping, ArArgumentBuilder *> myLoopEndCB;
};



#endif // ARSERVERHANDLERMAPPING_H
