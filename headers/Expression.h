#ifndef EXPRESSION_H
#define EXPRESSION_H

enum operation {
    PLUS,
    MINUS,
    MULT,
    DIV,
    POW
};

enum function {
    SIN,
    COS,
    LN,
    EXP
};

struct operators {
    operation type;
    int priority;
    operators(operation type): type(type) { //explicit
        switch (type) {
            case PLUS: case MINUS: priority = 1; break;
            case MULT: case DIV: priority = 2; break;
            case POW: priority = 3;
        }
    }
};


class Expression {};

#endif //EXPRESSION_H
