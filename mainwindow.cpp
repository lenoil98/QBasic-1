#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iterator"
#include "error.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("QBasic!");
    this->setFixedSize(BASIC_WINDOW_WIDTH, BASIC_WINDOW_HEIGHT);
    console=new Console(this);
    console->setGeometry(20,20,760,560);
    console->write( "welcome to my Basic interpreter!\n");
    connect(console,SIGNAL(newLineWritten(QString)),this,SLOT(newLine(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newLine(QString newline){

        if (newline[0].isNumber()){
            int s=0;
            while (newline[0].isNumber())
            {
                s=s*10+(newline.toStdString()[0]-'0');
                newline.remove(0,1);
            }
            if (newline=="")
                program.del(s);
            else program.insertCode(s,newline);
            return;
        }
        QString code=newline.remove(' ');

        try {
            if (code.startsWith("LET",Qt::CaseInsensitive)
                    ||code.startsWith("PRINT",Qt::CaseInsensitive)
                    ||code.startsWith("input",Qt::CaseInsensitive))
            {
                immediateRun(code);
                return;
            }
        }
        catch (error er) {
            console->write(er.message+'\n');
            context2.clear();
        }

        try {
            if (code.startsWith("RUN",Qt::CaseInsensitive))
                run();
            else if (code.startsWith("LIST",Qt::CaseInsensitive)){
                console->write(program.list());
            }
            else if (code.startsWith("CLEAR",Qt::CaseInsensitive)){
                program.clear();
                context.clear();
            }
            else if (code.startsWith("HELP",Qt::CaseInsensitive))
                console->write(help);
            else if (code.startsWith("QUIT",Qt::CaseInsensitive))
                exit(0);

            else throw error("Illegal statement");
        }
        catch (error er){
            console->write(er.message);
            context.clear();
        }



}

void MainWindow::run(){
    runningLine=0;
    while (true){
        while (!program.contains(runningLine)&&runningLine<=program.lastCode()){++runningLine;}
        if (runningLine>program.lastCode())
            throw error("The program has not an end");
        QString code=program.getCode(runningLine++);
        statement* state=read(code);
        if (state->type()==END){
            delete state;
            context.clear();
            return;
        }
        if (state->type()==IF){
            int tmp=runningLine;
            state->execute(context,*console,runningLine);
            if (runningLine!=tmp&&!program.contains(runningLine))
                throw error("Line "+QString::number(runningLine)+" does not exist");
        }
        else {
            state->execute(context,*console,runningLine);
            if (state->type()==GOTO&&!program.contains(runningLine))
                throw error("Line "+QString::number(runningLine)+" does not exist");
        }

        delete state;
    }
}


void MainWindow::immediateRun(QString code){
    statement* state= read(code);
    int x=-1;
    state->execute(context2,*console,x);
}

statement* MainWindow::read(QString code){
    statement* state;

    code.remove(' ');
    if (code.startsWith("REM",Qt::CaseInsensitive))
        state=new RemStmt(code.remove("REM",Qt::CaseInsensitive));
    else if (code.startsWith("LET",Qt::CaseInsensitive))
    {
        state=new LetStmt(parseExpression(code.remove("LET",Qt::CaseInsensitive)));
    }
    else if (code.startsWith("PRINT",Qt::CaseInsensitive)){
        state=new PrintStmt(parseExpression(code.remove("PRINT",Qt::CaseInsensitive)));
    }
    else if (code.startsWith("INPUT",Qt::CaseInsensitive)){
        Expression* ex=parseExpression(code.remove("INPUT",Qt::CaseInsensitive));
        if (ex->type()!=IDENTIFIER)
            throw error("Illegal statement");
        state=new InputStmt(ex->getName());
    }
    else if(code.startsWith("GOTO",Qt::CaseInsensitive)){
        Expression* ex=parseExpression(code.remove("GOTO",Qt::CaseInsensitive));
       if (ex->type()!=CONSTANT)
            throw error("Illegal statement");
        state=new GoToStmt(ex->eval(context));
    }
    else if(code.startsWith("IF",Qt::CaseInsensitive)){
        QString c,exp1,exp2,n,*op;
        c=code.remove("IF",Qt::CaseInsensitive);
        int x=c.indexOf('<');
        if (x==-1)
            x=c.indexOf('>');
        if (x==-1)
            x=c.indexOf('=');
        if (x==-1)
            throw error("Illegal statement");
        exp1=c.mid(0,x);
        op=new QString(c.mid(x,1));
        int y=c.indexOf("THEN",Qt::CaseInsensitive);
        if (y==-1)
            y=c.indexOf("then",Qt::CaseInsensitive);
        exp2=c.mid(x+1,y-x-1);
        n=c.mid(y+4);
        Expression* m=parseExpression(n);
        if (m->type()!=CONSTANT)
            throw error("Illegal statement");
        int p=m->eval(context);
        state=new IfStmt(parseExpression(exp1),op,parseExpression(exp2),p);
        delete op;
    }
    else if (code.startsWith("END",Qt::CaseInsensitive))
        state=new EndStmt;
    else throw error("Illegal statement");
    return state;
}

Expression* MainWindow::parseExpression(QString code){
    QStack<Expression*> operands;
    QStack<QString> ops;
    code.remove(' ');

    int i=0;
    while (!code[i].isNull()){
        if (code[i].isNumber()){
            int x=0;
            while (code[i].isNumber())
                x=x*10+(code.toStdString()[i++]-'0');
            Expression* ex=new ConstantExp(x);
            operands.push(ex);
        }
        else if (code[i].isLetterOrNumber()){
            QString var;
            while (code[i].isLetterOrNumber())
                var.push_back(code[i++]);
            Expression* ex=new IdentifierExp(var);
            operands.push(ex);
        }
        else if (code[i]=='='||code[i]=='+'||code[i]=='-'||code[i]=='*'||code[i]=='/'||code[i]=='('||code[i]==')'){
            if (code[i]=='=')
            {
                if (!ops.isEmpty()&&ops.top()!="=")
                    throw error("Illegal expression");
                ops.push("=");

            }
            else if (code[i]=='+'||code[i]=='-'){
                while (!ops.isEmpty()&&ops.top()!="("&&ops.top()!="="){
                    Expression* rhs=operands.pop();
                    Expression* lhs=operands.pop();
                    Expression* p=new CompoundExp(ops.pop(),lhs,rhs);
                    operands.push(p);
                }
                if (code[i]=='+') ops.push("+");
                else ops.push("-");
            }
            else if (code[i]=='*'||code[i]=='/'){
                if (code[i+1]=='*'){
                    ++i;
                    ops.push("**");
                }
                else {
                    while (!ops.isEmpty()&&ops.top()=="*"||ops.top()=="/"||ops.top()=="**"){
                        Expression* rhs=operands.pop();
                        Expression* lhs=operands.pop();
                        Expression* p=new CompoundExp(ops.pop(),lhs,rhs);
                        operands.push(p);
                    }
                    if (code[i]=='*')
                        ops.push("*");
                    else ops.push("/");
                }
            }
            else if (code[i]=='(')
                ops.push("(");
            else if (code[i]==')'){
                while (!ops.isEmpty()&&ops.top()!="("){
                    Expression* rhs=operands.pop();
                    Expression* lhs=operands.pop();
                    Expression* p=new CompoundExp(ops.pop(),lhs,rhs);
                    operands.push(p);
                }
                if (ops.isEmpty())
                    throw error("Illegal expression");
                ops.pop();
            }
            ++i;
        }
        else throw error("Illegal expression");
    }
    while (!ops.isEmpty()&&operands.size()>1) {
        Expression* rhs=operands.pop();
        Expression* lhs=operands.pop();
        Expression* p=new CompoundExp(ops.pop(),lhs,rhs);
        operands.push(p);
    }
    if (!ops.isEmpty()||operands.size()!=1)
        throw error("Illegal expression");
    return operands.pop();
}














