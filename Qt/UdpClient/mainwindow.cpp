#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "udpclient.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UdpClient *uClient = new UdpClient();
    connect(uClient, SIGNAL(addText(QString)), this, SLOT(addText(QString) ));
    connect(uClient, SIGNAL(drawImage(const QImage)), this, SLOT(drawImage(const QImage)));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addText(QString text)
{
   ui->textEdit->append(text);

}

void MainWindow::drawImage(const QImage image)
{
    ui->label->setPixmap(QPixmap::fromImage(image));
}



