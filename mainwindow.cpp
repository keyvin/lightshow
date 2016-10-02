#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpixel.h"
#include <QString>

#define COLUMNS 4

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QLabel *ptr = NULL;
    int counter = 0;
    for (counter = 0; counter < 30; counter++){
        ptr = new QPixel(counter);
        ui->mainLayout->addWidget(ptr, (int) (counter/4), counter % 4 );
        ui->commandTable->setColumnCount(4);
        ui->commandTable->setColumnWidth(0,50);
        ui->commandTable->setColumnWidth(1,40);
        ui->commandTable->setColumnWidth(2,30);
        ui->commandTable->setRowCount(10);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
