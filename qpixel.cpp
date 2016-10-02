#include "qpixel.h"
#include <qpainter>
#include <qlabel>
#include <qcolor>
#include <qbrush>
#include <iostream>
#include <string>
#include <cstdio>

using namespace std;
QPixel::QPixel(const int& pixnum, QWidget* parent)
    : QLabel(parent)
{
    pixel_number = pixnum;
    image = new QImage(50, 50, QImage::Format_ARGB32_Premultiplied);
    image->fill(QColor::fromRgb(0,0,0));
    setPixmap(QPixmap::fromImage(*image));
    selected = false;
}

QPixel::~QPixel() {

    //free image object
    delete image;
}



void QPixel::mousePressEvent(QMouseEvent* event)
{

    QPainter brush;

    if (selected == false) {
        brush.begin(image);
        selected = true;
        brush.setPen(QColor(255,55,0));

        brush.drawLine(0,0,50,50);
        brush.end();
        this->setPixmap(QPixmap::fromImage(*image));
        this->repaint();
        printf("out %d\n", pixel_number);

    }
    else if (selected == true){
        image->fill(QColor::fromRgb(0,0,0));
        setPixmap(QPixmap::fromImage(*image));
        selected = false;
        this->repaint();
     //   cout >> "Was clicked when true";
    }

    emit clicked();
}
