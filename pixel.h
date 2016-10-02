#ifndef PIXEL_H
#define PIXEL_H
#include <QString>


class Pixel {

public:
    quint8 r;
    quint8 g;
    quint8 b;
    //
    QString Hex();
    quint32 lowBits();

};

#endif // PIXEL_H
