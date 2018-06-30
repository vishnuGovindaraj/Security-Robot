#include <QCoreApplication>
#include "myudp.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyUDP Server;
    MyUDP Client;

    //Client.SayHello();

    return a.exec();
}
