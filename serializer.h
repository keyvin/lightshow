#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QObject>
#include <QWidget>
#include "qpixel.h"
#include "program.h"
class Serializer : public QObject
{
    Q_OBJECT

private:
    QString file_name;


public:
    explicit Serializer(QObject *parent = 0);

    bool save(QList<QPixel *> &, QMap<int, Program *> &);
    bool load();


signals:

public slots:

};

#endif // SERIALIZER_H
