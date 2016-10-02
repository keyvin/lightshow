#ifndef QPIXEL_H
#define QPIXEL_H
#include <qstring>
#include <qwidget>
#include <qlabel>
#include <qimage>

class QPixel : public QLabel
{

     Q_OBJECT

public:
    explicit QPixel( const int &pixNum, QWidget* parent=0 );
    int pixel_number;
    bool selected;
    QImage *image;
    ~QPixel();
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // QPIXEL_H
