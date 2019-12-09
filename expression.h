#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <QString>
#include <QMap>
#include <console.h>
enum ExpressionType{CONSTANT,IDENTIFIER,COMPOUND};

class EvaluationContext {
                                                             //管理所有变量的变量库
public:

   void setValue(QString var, int value);
   int getValue(QString var);
   bool isDefined(QString var);
   void clear();
private:

   QMap<QString,int> symbolTable;

};


class Expression{
public:
    Expression();                                          //生成表达式
    virtual ~Expression();
    virtual int eval(EvaluationContext& context)=0;         //获得该表达式的值
    virtual ExpressionType type()=0;
    //virtual QString tostring();

    virtual int getConstantValue();
    virtual QString getName();
    virtual QString getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();

private:
    QString expr;
};

class ConstantExp:public Expression{
public:
    ConstantExp(int x);
    int eval(EvaluationContext& context);
    //QString tostring();
    ExpressionType type();
private:
    int value;

};

class IdentifierExp:public Expression{
public:
    IdentifierExp(QString name);
    int eval(EvaluationContext& context);
    //QString tostring();
    ExpressionType type();
    QString getName();
private:
    QString name;
};

class CompoundExp:public Expression{
public:
    CompoundExp(QString op,Expression *lhs,Expression *rhs);
    int eval(EvaluationContext& context);
    //QString tostring();
    ExpressionType type();
    QString getOp();
    Expression *getLHS();
    Expression *getRHS();

private:
    QString op;
    Expression *lhs,*rhs;
};




#endif // EXPRESSION_H
