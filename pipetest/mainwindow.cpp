#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
	m_server.reset(new commserver);
}

MainWindow::~MainWindow()
{
}

