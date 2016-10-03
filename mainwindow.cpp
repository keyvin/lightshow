#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpixel.h"
#include <QStringList>
#include <QString>

//TODO (leave off) - pop up error that pixel is in other group
//TODO Lock unlock command table
//Create "program" class for groups.

#define COLUMNS 4

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QStringList table_header;
    ui->setupUi(this);
    QPixel *ptr = NULL;

    //initialize group counter - for naming TODO

    group_counter = 1;

    int counter = 0;
    for (counter = 0; counter < 30; counter++){
        ptr = new QPixel(counter);
        ui->mainLayout->addWidget(ptr, (int) (counter/4), counter % 4 );

        QObject::connect(ptr, SIGNAL(clicked(int)),
                             this, SLOT(pixelClicked(int)));
        pixels.append(ptr);
    }
    ui->commandTable->setColumnCount(5);
    ui->commandTable->setColumnWidth(0,70);
    ui->commandTable->setColumnWidth(1,50);
    ui->commandTable->setColumnWidth(2,30);
    ui->commandTable->setColumnWidth(3,30);
    ui->commandTable->setColumnWidth(4,30);

    ui->commandTable->setRowCount(10);

    table_header << "Command" << "Frames" << "R" << "G" << "B";
    ui->commandTable->setHorizontalHeaderLabels(table_header);
    ui->commandTable->setDisabled(true);

    //map click buttons
    QObject::connect(ui->createGroup, SIGNAL(clicked(bool)), this, SLOT(createGroup()));


    //Create default group. Make sure to free these if (when) they are deleted //todo
    active_group = new QList<int>;
    QString f("0");
    pixel_groups.insert(f, active_group);
    ui->group_list->insertItem(0, f);
    ui->group_list->setCurrentItem(0);
    QObject::connect(ui->group_list, SIGNAL(itemSelectionChanged()), this, SLOT(selectGroup()));
}

void MainWindow::pixelClicked(int pixel) {
    //is this already in the list (remove it)
    int counter = 0;
    for (counter = 0; counter < active_group->length(); counter ++){
        if (active_group->at(counter) == pixel){
            //is already in list. Remove it.
            active_group->removeAt(counter);
            updatePixelLabel();
            pixels.at(pixel)->changeGroup(-1);
            if (active_group->length() == 0)
                ui->commandTable->setDisabled(true);
            return;

            //if list is empty, deactivate command table. --TODO
        }
    }
    //Was not in the list.

    // TODO is it in another list? Ask to remove it OR force manual deselect.
    if (pixels.at(pixel)->group_number != -1){
        return;

    }
    //
    active_group->push_back(pixel);
    updatePixelLabel();
    ui->group_list->currentItem()->text().toInt();
    pixels.at(pixel)->changeGroup(ui->group_list->currentItem()->text().toInt());
    ui->commandTable->setDisabled(false);
}

void MainWindow::updatePixelLabel(){
    //generate string
    QString text = "Pixel(s) selected: ";
    int counter = 0;
    for (counter = 0; counter < active_group->length(); counter ++){
        text = text + QString::number(active_group->at(counter));
        text = text + ",";

    }
    ui->current_pixels->setText(text);
    ui->current_pixels->repaint();
    return;
}



void MainWindow::selectGroup(){
    QString selection;
    selection = ui->group_list->currentItem()->text();
    active_group = pixel_groups.value(selection);
    //TODO - load commands if they exist. Unlock
    updatePixelLabel();
    if (active_group->length() == 0)
        ui->commandTable->setDisabled(true);
    else
        ui->commandTable->setDisabled(false);
    return;
}

void MainWindow::createGroup(){
    QString text;
    text = QString::number(group_counter);
    ui->group_list->insertItem(0, text);
    group_counter++;
    active_group = new QList<int>();
    pixel_groups.insert(text, active_group);
    ui->commandTable->setDisabled(false);
    return;
}

void MainWindow::deleteGroup(){


    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}
