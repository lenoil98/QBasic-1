#ifndef PROGRAM_H
#define PROGRAM_H

#include <QMap>
#include <QString>

class Program                                    //管理所有代码的代码库
{
public:
    Program();
    QString getCode(int x);
    void insertCode(int x,QString code);
    QString list();
    void clear();
    void del(int x);
    bool contains(int x);
    int lastCode();
private:
    QMap<int,QString> codes;
};

#endif // PROGRAM_H
