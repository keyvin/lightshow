#ifndef PROGRAM_H
#define PROGRAM_H

#include <qstring>
#include <qlist>
#include <qobject>
#include <qstringlist>
typedef enum MODE { SET, INCREMENT, DECREMENT } mode;

typedef struct PROGRAM {
    mode type;
    int frames;
    quint8 R;
    quint8 G;
    quint8 B;
} command;

class Program : QObject
{
    Q_OBJECT

private:
    QList<command> commands;

public:
    Program();
    //number of commands
    int getLength();

    //For the table widget
    QStringList getCommandStringsAt(int);

    //For generating the program. Returns 32 bits in hex
    QString getHexAt(int);

    bool addCommand(QStringList);

    void clear();

};

#endif // PROGRAM_H
