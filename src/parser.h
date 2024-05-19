#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct ASTNode {
    TokenType type;
    double value;
    char *name;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *condition;
    struct ASTNode *body;
    struct ASTNode *else_body;
    struct ASTNode *next;
} ASTNode;

ASTNode* init_ast_node(TokenType type, double value, char *name);
ASTNode* init_ast_node_with_children(TokenType type, ASTNode *child);
ASTNode* parse(Lexer *lexer);
ASTNode* parse_block(Lexer *lexer);
ASTNode* parse_statement(Lexer *lexer);
ASTNode* parse_print_statement(Lexer *lexer);
ASTNode* parse_expression(Lexer *lexer);
ASTNode* parse_assignment_statement(Lexer *lexer);
ASTNode* parse_while_statement(Lexer *lexer);
void append_ast_node(ASTNode *parent, ASTNode *child);
void print_ast_node(ASTNode *node, int depth);

#endif // PARSER_H
