#include "../headers/Expression.h"

operators::operators(const operation type): type(type) {
    switch (type) {
        case PLUS: case MINUS: priority = 1; break;
        case MULT: case DIV: priority = 2; break;
        case POW: priority = 3;
    }
}