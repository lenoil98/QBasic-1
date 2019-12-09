#include "expression.h"
#include <QtMath>
#include "error.h"



//toString 还没有考虑

int Expression::getConstantValue(){return 0;}
QString Expression::getOperator(){return "";}
Expression* Expression::getLHS(){return nullptr;}
Expression* Expression::getRHS(){return nullptr;}
QString Expression::getName(){return "";}
Expression::Expression(){}
Expression::~Expression(){}

ConstantExp::ConstantExp(int x){
    value=x;
}

IdentifierExp::IdentifierExp(QString name){
    this->name=name;
}

CompoundExp::CompoundExp(QString op,Expression *lhs,Expression *rhs){
    this->op=op;
    this->lhs=lhs;
    this->rhs=rhs;
}

void EvaluationContext::setValue(QString var, int value){
    symbolTable[var]=value;
}

int EvaluationContext::getValue(QString var){
    return symbolTable[var];
}

bool EvaluationContext::isDefined(QString var){
    return symbolTable.contains(var);
}

void EvaluationContext::clear(){
    symbolTable.clear();
}

int ConstantExp::eval(EvaluationContext& context){
    return value;
}

QString IdentifierExp::getName(){return name;}

int IdentifierExp::eval(EvaluationContext& context){
    if (!context.isDefined(name)) throw error(name+" is undefined");
    return context.getValue(name);
}

QString CompoundExp::getOp(){return op;}
Expression* CompoundExp::getLHS(){return lhs;}
Expression* CompoundExp::getRHS(){return rhs;}

int CompoundExp::eval(EvaluationContext& context){
    int right=rhs->eval(context);
    if (op=="="){
        context.setValue(lhs->getName(),right);
        return right;
    }
    int left=lhs->eval(context);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
       if (right == 0) throw error("Division by 0");
          return left / right;
    }
    if (op=="**") return pow(left,right);
    throw error("Illegal operator in expression");
       return 0;
}

ExpressionType CompoundExp::type(){return COMPOUND;}
ExpressionType ConstantExp::type(){return CONSTANT;}
ExpressionType IdentifierExp::type(){return IDENTIFIER;}
