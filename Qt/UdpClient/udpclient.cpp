#include "udpclient.h"

UdpClient::UdpClient(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 1234);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

    QImage image( "pic2.png" );
    if( image.isNull() )
      qFatal( "Failed to open test.png" );

       emit(drawImage(image));



}


void UdpClient::readyRead()
{
    QByteArray Buffer;
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(),Buffer.size(),&sender,&senderPort);

    //emit(addText(Buffer.data()));

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message : " << Buffer;

}
