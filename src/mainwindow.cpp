#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "passwordlist.h"

#include "generate.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
	Generate* window = new Generate();
	window->show();
	this->hide();
}

void MainWindow::on_pushButton_clicked()
{
	PasswordList* window = new PasswordList();
	window->show();
	this->hide();
}
