#ifndef PIXEL_H
#define PIXEL_H
#include <QString>


//This class represents an RGB value for a pixel, as well as the encoding.
//Hex ahould return the hexadecimal value as a string.
//not sure what lowbits does.

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
