#ifndef LETSTMT_H
#define LETSTMT_H

#include <statement.h>

class LetStmt:public statement
{
public:
    LetStmt();

private:
    IdentifierExp* var;
    Expression* right;
};

#endif // LETSTMT_H
