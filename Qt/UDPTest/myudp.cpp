#include "myudp.h"
#include <QDataStream>
#include <QTimer>
#include <QBuffer>

MyUDP::MyUDP(QObject *parent) :
    QObject(parent)
{

    socket = new QUdpSocket(this);

    QHostAddress address;
    address.setAddress("localhost");
    socket->bind(QHostAddress::LocalHost, 1235);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));




    QTimer *timer = new QTimer( this );
    timer->setInterval( 3000 );
    timer->start();

    //connect( timer, SIGNAL(timeout()), this, SLOT(SayHello()) );
}

void MyUDP::SayHello()
{

    QHostAddress address;
    address.setAddress("192.168.137.1");

    QByteArray Data;
    Data.append("Hello, UDP");
    socket->writeDatagram(Data,QHostAddress::LocalHost, 1235);


}


void MyUDP::broadcastLine()
{
/*
    QByteArray Data;
    Data.append("Hello, UDP");
    socket->writeDatagram(Data,QHostAddress::LocalHost, 1234);
    */

qDebug() << "Image";

    QImage *image = new QImage( "pic2.png" );
    if( image->isNull() )
      qFatal( "Failed to open test.png" );



    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    image->save(&buffer, "PNG");
    buffer.close();


}

void MyUDP::readyRead()
{
    QByteArray Buffer;
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(),Buffer.size(),&sender,&senderPort);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message : " << Buffer;
}
