#ifndef STATEMENT_H
#define STATEMENT_H
#include <QString>
#include <expression.h>
#include <console.h>
enum StateType{REM,LET,PRINT,INPUT,GOTO,IF,END};

class statement
{
public:
    statement();                                                                          //生成语句
    virtual ~statement();
    virtual void execute(EvaluationContext &context,Console &console,int &line)=0;        //执行语句
    virtual StateType type()=0;
private:
    QString state;
};

class RemStmt:public statement{
public:
    RemStmt(QString comment);
    StateType type();
    void execute(EvaluationContext &context,Console &console,int &line);
private:
    QString comment;
};

class LetStmt:public statement{
public:
    LetStmt(Expression *expr);
    StateType type();
    void execute(EvaluationContext &context,Console &console,int &line);
private:
    Expression *expr;

};

class PrintStmt:public statement{
public:
    PrintStmt(Expression* expr);
    StateType type();
    void execute(EvaluationContext &context,Console &console,int &line);
private:
    Expression *expr;

};

class InputStmt:public statement{
public:
    InputStmt(QString var);
    StateType type();
    void execute(EvaluationContext &context,Console &console,int &line);
private:
    QString var;

};

class GoToStmt:public statement{
public:
    GoToStmt(int n);
    StateType type();
    void execute(EvaluationContext &context,Console &console,int &line);
private:
    int n;
};

class IfStmt:public statement{
public:
    IfStmt(Expression* expr1,QString* op,Expression* expr2,int n);
    StateType type();
    void execute(EvaluationContext &context,Console &console,int &line);

private:
    Expression* expr1;
    QString op;
    Expression* expr2;
    int n;
};

class EndStmt:public statement{
public:
    StateType type();
    void execute(EvaluationContext &context,Console &console,int &line);

};


#endif // STATEMENT_H
