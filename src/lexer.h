#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN,
    TOKEN_PRINT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LTE,
    TOKEN_GTE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_SEMICOLON,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_INPUT,
    TOKEN_VAR
} TokenType;

typedef struct {
    TokenType type;
    double value;
    char *name;
} Token;

typedef struct {
    char *input;
    size_t pos;
    Token current_token;
} Lexer;

Lexer* init_lexer(char *input);
void advance(Lexer *lexer);
char current_char(Lexer *lexer);
void skip_whitespace(Lexer *lexer);
Token number(Lexer *lexer);
Token identifier_or_keyword(Lexer *lexer);
Token get_next_token(Lexer *lexer);
void lexer_advance(Lexer *lexer);

#endif // LEXER_H
