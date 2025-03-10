#ifndef EXPRESSION_H
#define EXPRESSION_H

enum operation { PLUS, MINUS, MULT, DIV, POW };

enum function { SIN, COS, LN, EXP };

struct operators {
    operation type;
    int priority;

    explicit operators(operation type); // для явного преобразования
};


class Expression {
};

#endif //EXPRESSION_H
