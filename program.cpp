#include "program.h"

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

    command_strings.append(QString::number(current.frames));
    command_strings.append(QString::number(current.R));
    command_strings.append(QString::number(current.G));
    command_strings.append(QString::number(current.B));
    return command_strings;

}

QString Program::getHexAt(int offset){
    QString ret;
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

    work_string =  to_add.at(1);

    //wrap in a try catch!
    working.frames = work_string.toInt();

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


