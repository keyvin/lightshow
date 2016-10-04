#include "program.h"
#include <cstring>

Program::Program()
{

}

int Program::getLength(){

    return (commands.length());
}



QStringList Program::getCommandStringsAt(int offset){
    QStringList command_strings;
    //sanity checks
    if (!commands.length())
        return command_strings;
    else if (offset < 0 || offset >= commands.length())
        return command_strings;

    command current;
    current = commands.at(offset);
    if (current.type == SET) {
        command_strings.append("SET");
    }
    else if (current.type == INCREMENT) {
        command_strings.append("INC");
    }
    else if (current.type == DECREMENT) {
        command_strings.append("DEC");
    }
    else if (current.type == NOOP) {
        command_strings.append("NOOP");
    }
    command_strings.append(QString::number(current.frames));
    command_strings.append(QString::number(current.R));
    command_strings.append(QString::number(current.G));
    command_strings.append(QString::number(current.B));
    return command_strings;

}

QString Program::getHexAt(int offset){
    char buffer[100];
    QString ret;

    //C   F     8F  8R    16R  G     24G  B
    //000 00000 000 00000 00   000000 0   0000000
    command c = commands.at(offset);

    quint8 hi = 0;
    quint8 frames = 0;
    quint8 tmp = 0;
    quint8 second = 0;
    quint8 third = 0;
    quint8 fourth = 0;

    switch (c.type) {
       case SET:
        hi = 0x00;
        break;
       case INCREMENT:
        hi = 0x01;
        break;
       case DECREMENT:
        hi = 0x02;
        break;
       case NOOP:
        hi = 0x03;
        break;
    }
    //left shift






    hi = (hi << 5) + (c.frames >> 3);
    //high byte is set
    second = (c.frames << 5) + (c.R >> 3);

    third = (c.R << 6) + (c.G >> 1);

    //high bit of  set
    fourth = (c.G << 7) + c.B;
    sprintf(buffer, "\\x%.2X\\x%.2X\\x%.2X\\x%.2X", hi, second, third, fourth);
    ret = buffer;
    return ret;


}

bool Program::addCommand(QStringList to_add){

    command working;
    working.type = SET;
    QString work_string;
    if (to_add.length() != 5)
        return false;

    if (to_add.at(0) == "SET")
        working.type = SET;
    else if (to_add.at(0) == "INC")
        working.type = INCREMENT;
    else if (to_add.at(0) == "DEC")
        working.type = DECREMENT;
    else if (to_add.at(0) == "NOOP")
        working.type = NOOP;
    else {

        return (false); // just skip this row...

    }

    work_string =  to_add.at(1);

    //wrap in a try catch!
    working.frames = (quint8 ) work_string.toInt();

    work_string = to_add.at(2);
    working.R = work_string.toInt();

    work_string = to_add.at(3);
    working.G = work_string.toInt();

    work_string = to_add.at(4);
    working.B = work_string.toInt();

    commands.push_back(working);

    return true;
}

void Program::clear(){
    //everything should be on the stack.
    QList<command> blank;
    commands.clear();
}

pixel Program::returnValueAtFrame(quint32 target_frame){
    quint32 frame_counter = 1;
    quint32 next_command = 1;
    int current_command = 0;
    pixel value;
    value.R = 0;
    value.G = 0;
    value.B = 0;

    if (commands.length() == 0)
        return (value);

    command current;

    //
    while (frame_counter < target_frame) {
        if (frame_counter == next_command){
            current = commands.at(current_command);
            next_command = frame_counter + current.frames;
            current_command++;
            if (current_command == commands.length())
                current_command = 0;
        }
        //execute current command
        if (current.type == SET){
            value.R = current.R;
            value.G = current.G;
            value.B = current.B;

        }
        else if (current.type == INCREMENT){
            value.R = value.R + current.R;
            value.G = value.G + current.G;
            value.B = value.B + current.B;
        }
        else if (current.type == DECREMENT){
            value.R = value.R - current.R;
            value.G = value.G - current.G;
            value.B = value.B - current.B;
        }
        else{
            //nothing

        }


        frame_counter++;
    }
    return value;

}






