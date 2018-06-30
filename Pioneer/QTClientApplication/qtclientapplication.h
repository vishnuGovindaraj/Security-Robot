#ifndef QTCLIENTAPPLICATION_H
#define QTCLIENTAPPLICATION_H

#include <QtWidgets/QMainWindow>
#include "ui_qtclientapplication.h"

class QTClientApplication : public QMainWindow
{
	Q_OBJECT

public:
	QTClientApplication(QWidget *parent = 0);
	~QTClientApplication();

private:
	Ui::QTClientApplicationClass ui;

public:
	static void runClient();

public slots:
	void on_upButton_pressed();
	void on_upButton_released();
	void on_downButton_pressed();
	void on_downButton_released();
	void on_leftButton_pressed();
	void on_leftButton_released();
	void on_rightButton_pressed();
	void on_rightButton_released();

	void on_connectButton_clicked();
	void on_disconnectButton_clicked();

};




#endif // QTCLIENTAPPLICATION_H
