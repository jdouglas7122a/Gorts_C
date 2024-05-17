#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

void interpret(ASTNode *node);
void interpret_print(ASTNode *node);
double evaluate_expression(ASTNode *node);
void free_ast(ASTNode *node);

#endif // INTERPRETER_H