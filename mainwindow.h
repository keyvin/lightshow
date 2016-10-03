#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QList>
#include <QMap>
#include <QTimer>
#include "qpixel.h"
#include "program.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    quint32 frame;
    QList<int> *active_group;
    QList<QPixel *> pixels;
    void updatePixelLabel();
    QMap<QString, QList<int> *> pixel_groups;
    int group_counter;
    QMap<int, Program *> program_map; //maps groups to their program
    Program *current_program;
    void saveTable();
    void loadTable();
    QTimer play_timer;
    bool playing;
    void genProgramString();

public slots:
    void pixelClicked(int);
    void createGroup();
    void deleteGroup();
    void selectGroup();
    void onForward();
    void onBackward();
    void onPlay();
    void advanceFrame();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
