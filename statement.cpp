#include "statement.h"
#include "error.h"
#include <QCoreApplication>

StateType LetStmt::type(){return LET;}
StateType PrintStmt::type(){return PRINT;}
StateType InputStmt::type(){return INPUT;}
StateType GoToStmt::type(){return GOTO;}
StateType RemStmt::type(){return REM;}
StateType IfStmt::type(){return IF;}
StateType EndStmt::type(){return END;}

statement::statement(){}
statement::~statement(){}

RemStmt::RemStmt(QString comment){
    this->comment=comment;
}

LetStmt::LetStmt(Expression *expr){
    this->expr=expr;
}

PrintStmt::PrintStmt(Expression* expr){
    this->expr=expr;
}

InputStmt::InputStmt(QString var){
    this->var=var;
}

GoToStmt::GoToStmt(int n){
    this->n=n;
}

IfStmt::IfStmt(Expression* expr1,QString* op,Expression* expr2,int n){
    this->expr1=expr1;
    this->expr2=expr2;
    this->op=*op;
    this->n=n;
}

void RemStmt::execute(EvaluationContext &context,Console &console,int &line){}

void LetStmt::execute(EvaluationContext &context,Console &console,int &line){
    expr->eval(context);
    delete expr;
}

void PrintStmt::execute(EvaluationContext &context,Console &console,int &line){
    console.write(QString::number(expr->eval(context)));
    delete expr;
}

void InputStmt::execute(EvaluationContext &context,Console &console,int &line){
    console.write(" ? ");
    console.InputEx=1;
    while (console.InputEx) {QCoreApplication::processEvents();}
    int x=1;
    if (console.tmpInput.startsWith("-")){
            x=-1;
            console.tmpInput.remove("-");
    }
    int s=0;
    while (console.tmpInput.toStdString()[0]>='0'&&console.tmpInput.toStdString()[0]<='9')
    {
        s=s*10+(console.tmpInput.toStdString()[0]-'0');
        console.tmpInput.remove(0,1);
    }
    if (!console.tmpInput.isEmpty())
        throw error("Illegal value");
    s*=x;
    context.setValue(var,s);
}

void GoToStmt::execute(EvaluationContext &context,Console &console,int &line){
    line=n;
}

void IfStmt::execute(EvaluationContext &context,Console &console,int &line){
    int left=expr1->eval(context);
    int right=expr2->eval(context);
    if (op=="="&&left==right)
        line=n;
    if (op=="<"&&left<right)
        line=n;
    if (op==">"&&left>right)
        line=n;
    delete expr1;
    delete expr2;
}

void EndStmt::execute(EvaluationContext &context,Console &console,int &line){}












