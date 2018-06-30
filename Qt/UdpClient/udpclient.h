#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QImage>

class UdpClient : public QObject
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = 0);

signals:
    void addText(QString);
    void drawImage(QImage);

public slots:
    void readyRead();

private:
    QUdpSocket *socket;

};

#endif // UDPCLIENT_H
