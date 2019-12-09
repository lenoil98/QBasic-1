#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <console.h>
#include <expression.h>
#include <program.h>
#include <statement.h>
#include <QStack>
#include <QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void newLine(QString);

private:
    Ui::MainWindow *ui;
    Console* console;
    Program program;
    EvaluationContext context,context2;                     //序号代码的和立即执行指令的分别的变量库
    int runningLine;                                        //目前运行到的行数
    void run();                                             //运行序号代码
    statement* read(QString code);                          //解析代码，生成语句对象
    Expression* parseExpression(QString code);              //解析一段string，生成成一个表达式
    void immediateRun(QString code);                        //立即运行指令

    static const int BASIC_WINDOW_WIDTH = 800;
    static const int BASIC_WINDOW_HEIGHT = 600;


    const QString help=
            "A simple BASIC interpreter.";
};

#endif // MAINWINDOW_H
