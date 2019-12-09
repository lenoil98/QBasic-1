#include "program.h"
#include <QString>
Program::Program()
{

}

QString Program::getCode(int x){
    return  codes[x];
}
void Program::insertCode(int x,QString code){
    codes[x]=code;
}
QString Program::list(){
    QString ans;
    QMap<int, QString>::iterator i=codes.begin();
    while (i!=codes.constEnd()) {
        ans+=QString::number(i.key());
        ans+=i.value();
        ans+="\n";
        ++i;
    }
    return ans;
}

int Program::lastCode(){return codes.lastKey();}

void Program::clear(){
    codes.clear();
}

void Program::del(int x){
    if (codes.contains(x))
        codes.remove(x);
}

bool Program::contains(int x){
    return codes.contains(x);
}
