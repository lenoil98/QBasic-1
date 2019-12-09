#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>
#include <QString>

class Console : public QTextEdit
{
    Q_OBJECT
public:
    bool InputEx=0;
    QString tmpInput;
    explicit Console(QWidget *parent = nullptr);
signals:
    void newLineWritten(QString newline);

public slots:
    void write(QString msg);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // CONSOLE_H
