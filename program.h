#ifndef PROGRAM_H
#define PROGRAM_H

#include <qstring>
#include <qlist>
#include <qobject>
#include <qstringlist>
#include <qmap>

//Switched to full bytes. No more packing. Each command is now 40 bits/5bytes

//This class holds the program data displayed in the table, and also generates the program string
typedef enum MODE { SET, INCREMENT, DECREMENT, NOOP } mode;

//used for generating program hex string
typedef struct STRINGELEMENT{
    int pixel;
    int frame;
} pixel_element;

//Current Value of the PIXEL
typedef struct PIXEL {
    quint8 R;
    quint8 G;
    quint8 B;

} pixel;


//What frame is the next command fetched
typedef struct WHEN {
  int group_num;
  int frame;
} when_element;

typedef struct PROGRAM {
    mode type;
    quint8 frames;
    quint8 R;
    quint8 G;
    quint8 B;
} command;


class Program : QObject
{
    Q_OBJECT

private:


public:

    QList<command> commands;
    Program();
    //number of commands
    int getLength();

    //For the table widget
    QStringList getCommandStringsAt(int);

    //For generating the program. Returns 32 bits in hex
    QString getHexAt(int);

    bool addCommand(QStringList);
    pixel returnValueAtFrame(quint32 );
    void clear();

};

#endif // PROGRAM_H
