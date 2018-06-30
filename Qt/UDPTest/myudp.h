#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QUdpSocket>
#include <QImage>

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QObject *parent = 0);



signals:


public slots:
    void readyRead();
    void broadcastLine();
    void SayHello();

private:
    QUdpSocket *socket;

};

#endif // MYUDP_H
