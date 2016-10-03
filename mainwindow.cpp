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

    current_program = new Program();
    //initial program set up
    program_map.insert(0, current_program);

    //bind forward button
    QObject::connect(ui->forwardButton, SIGNAL(clicked(bool)), this, SLOT(onForward()));
    //bind backwards button
    QObject::connect(ui->backwardButton, SIGNAL(clicked(bool)), this, SLOT(onBackward()));
    //bind play button
    QObject::connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(onPlay() ));
    playing = false;
    QObject::connect(&play_timer, SIGNAL(timeout()), this, SLOT(advanceFrame()));

    play_timer.start(34);

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
    saveTable();

    selection = ui->group_list->currentItem()->text();
    active_group = pixel_groups.value(selection);
    //TODO - load commands if they exist. Unlock
    updatePixelLabel();


    //TODO -- something here to do.


        ui->commandTable->setDisabled(false);
        current_program = program_map.value(selection.toInt());
        loadTable();


    return;
}

void MainWindow::loadTable() {
    QStringList row;
    int working;
    //clear all rows
    ui->commandTable->setRowCount(0);
    ui->commandTable->setRowCount(20);
    //get current group
    working = ui->group_list->currentItem()->text().toInt();

    current_program = program_map.value(working);

    //program is empty
    if (!current_program->getLength())
        return;

    for (int working = 0; working < current_program->getLength(); working++){
        row = current_program->getCommandStringsAt(working);
        ui->commandTable->insertRow(working);
        //funrolled loops lol - definitely not laziness
        ui->commandTable->setItem(working, 0, new QTableWidgetItem(row.at(0)));
        ui->commandTable->setItem(working, 1, new QTableWidgetItem(row.at(1)));
        ui->commandTable->setItem(working, 2, new QTableWidgetItem(row.at(2)));
        ui->commandTable->setItem(working, 3, new QTableWidgetItem(row.at(3)));
        ui->commandTable->setItem(working, 4, new QTableWidgetItem(row.at(4)));


   }
   return;
}

void MainWindow::saveTable() {
    QString working;
    QStringList list;
    int row_counter = 0;
    current_program->clear();
    //TODO Should be able to insert rows. Should be able to delete rows
    while (ui->commandTable->item(row_counter, 0) != NULL){
        working = ui->commandTable->item(row_counter, 0)->text();
        list.push_back(working);
        //get other table values
        for (int i = 1; i < 5; i++ ){
            list.push_back(ui->commandTable->item(row_counter, i)->text());

        }
        current_program->addCommand(list);
        list.clear();
        row_counter++;

    }
    if (row_counter > 0){
        current_program->getHexAt(0);
    }






}

void MainWindow::createGroup(){
    QString text;
    text = QString::number(group_counter);
    ui->group_list->insertItem(0, text);
    active_group = new QList<int>();
    pixel_groups.insert(text, active_group);
    ui->commandTable->setDisabled(false);
    current_program = new Program();
    program_map.insert(group_counter, current_program);
    group_counter++;
    return;
}

void MainWindow::deleteGroup(){


    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Roll back frame by 2, then call on forward
void MainWindow::onBackward() {
    if (frame == 1)
        return;
    else {
        frame = frame -2;
        onForward();
    }
}

void MainWindow::onForward(){

    //increment frame and redraw pixels
    pixel value;
    frame++;
    //calculate pixel color for each group
    Program *prog = NULL;
    QList<int> groups;
    groups = program_map.keys();
    for (int a = 0; a < groups.length(); a++){
        prog = program_map.value(groups.at(a));
        value = prog->returnValueAtFrame(frame);
        for (int b = 0; b < pixels.length();b++) {
            if (pixels.at(b)->group_number == a){
                pixels.at(b)->setColor(value);
            }

        }

    }
    ui->frameLabel->setText(QString::number(frame));


}

void MainWindow::onPlay(){
    if (playing == false){
        ui->playButton->setText("Stop");
        playing = true;
        play_timer.start(34);
    }
    else if (playing == true){
        ui->playButton->setText("Play");
        playing = false;
    }

}

void MainWindow::genProgramString() {

    QList<QString> groups;

    groups = pixel_groups.keys();

    //generate table





}


void MainWindow::advanceFrame(){
    if (playing == true){
        onForward();
    }

}

