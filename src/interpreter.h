#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

typedef enum {
    VAL_NUMBER,
    VAL_STRING,
    VAL_BOOL
} ValueType;

typedef struct {
    ValueType type;
    union {
        double number;
        char *string;
        int boolean;
    } value;
} Value;

void interpret(ASTNode *node);
void interpret_print(ASTNode *node);
Value evaluate_expression(ASTNode *node);
void free_ast(ASTNode *node);

#endif // INTERPRETER_H
