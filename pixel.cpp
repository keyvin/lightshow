#include "pixel.h"

//Pixel::Pixel(){
//    r = 0;
//    g = 0;
//    b = 0;
//}

//Pixel::~Pixel(){
//}

QString Pixel::Hex(){
    QString hex = "#000000";
    return hex;
}

quint32 Pixel::lowBits(){
    quint32 result = 0;
    result = r;
    result = result << 7;
    result = result | g;
    result = result << 7;
    result = result | b;

    return result;

}
