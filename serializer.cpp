#include "serializer.h"
#include <QFile>
#include <QXmlStreamWriter>
#include <QFileDialog>
#include <QFile>
#include "program.h"

//#include "qpixel.h"

Serializer::Serializer(QObject *parent) : QObject(parent)
{



}

bool Serializer::load(){ return true;}

bool Serializer::save(QList<QPixel *> &pixels, QMap<int, Program *> &program_map) {


    QString filename = "prog_save.xml";

    QFile file(filename);
    file.open( QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();


    //write out which program each pixel is assigned to.
    xmlWriter.writeStartElement("PIXELS");
    for (int counter = 0; counter < pixels.length(); counter++ ){
        QString pix_num = QString::number(counter);
        QString pix_group =  QString::number((pixels[counter]->group_number));
        xmlWriter.writeTextElement( pix_num, pix_group);
    }


    xmlWriter.writeEndElement();
    //write the programs out
    xmlWriter.writeStartElement("Programs");
    for(int counter = 0; counter < program_map.keys().length(); counter++) {
        Program *current;

        int curr_key = program_map.keys()[counter];
        QString prog_num =  QString::number((curr_key));
        current = program_map.value(curr_key);
        //begin the program number element
        xmlWriter.writeStartElement(prog_num);

        for( int curr_command = 0; curr_command < current->commands.length(); curr_command++ ) {
            QString num = QString::number(curr_command);
            //write the commands. What they do here doesn't matter.
            xmlWriter.writeStartElement(num);
            QString cmd = QString::number((int) current->commands[curr_command].type );
            QString frames = QString::number( current->commands[curr_command].frames );
            QString R = QString::number((current->commands[curr_command].R ));
            QString G = QString::number(current->commands[curr_command].G );
            QString B = QString::number(current->commands[curr_command].B );

            xmlWriter.writeTextElement("command", cmd);
            xmlWriter.writeTextElement("frames", frames);
            xmlWriter.writeTextElement("R", R);
            xmlWriter.writeTextElement("G", G);
            xmlWriter.writeTextElement("B", B);

            xmlWriter.writeEndElement();

        }

        xmlWriter.writeEndElement();
    }



    xmlWriter.writeEndElement();

    file.close();
    return true;
}
