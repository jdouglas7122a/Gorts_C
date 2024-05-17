#include "lexer.h"
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

Lexer* init_lexer(char *input) {
    Lexer *lexer = (Lexer*)malloc(sizeof(Lexer));
    lexer->input = input;
    lexer->pos = 0;
    lexer->current_token = (Token){TOKEN_EOF, 0, NULL}; // Initialize name to NULL
    return lexer;
}

void advance(Lexer *lexer) {
    lexer->pos++;
}

char current_char(Lexer *lexer) {
    if (lexer->pos < (size_t)strlen(lexer->input)) { // Explicitly cast strlen to size_t
        return lexer->input[lexer->pos];
    }
    return '\0';
}

void skip_whitespace(Lexer *lexer) {
    while (isspace(current_char(lexer))) {
        advance(lexer);
    }
}

Token number(Lexer *lexer) {
    char buffer[64];
    int index = 0;
    while (isdigit(current_char(lexer)) || current_char(lexer) == '.') {
        buffer[index++] = current_char(lexer);
        advance(lexer);
    }
    buffer[index] = '\0';
    return (Token){TOKEN_NUMBER, atof(buffer), NULL}; // Initialize name to NULL
}

Token identifier_or_keyword(Lexer *lexer) {
    char buffer[64];
    int index = 0;
    while (isalpha(current_char(lexer)) || current_char(lexer) == '_') {
        buffer[index++] = current_char(lexer);
        advance(lexer);
    }
    buffer[index] = '\0';

    if (strcmp(buffer, "print") == 0) return (Token){TOKEN_PRINT, 0, NULL};
    if (strcmp(buffer, "if") == 0) return (Token){TOKEN_IF, 0, NULL};
    if (strcmp(buffer, "else") == 0) return (Token){TOKEN_ELSE, 0, NULL};
    if (strcmp(buffer, "while") == 0) return (Token){TOKEN_WHILE, 0, NULL};
    if (strcmp(buffer, "true") == 0) return (Token){TOKEN_TRUE, 1, NULL};
    if (strcmp(buffer, "false") == 0) return (Token){TOKEN_FALSE, 0, NULL};
    if (strcmp(buffer, "input") == 0) return (Token){TOKEN_INPUT, 0, NULL};
    if (strcmp(buffer, "var") == 0) return (Token){TOKEN_VAR, 0, NULL};

    return (Token){TOKEN_IDENTIFIER, 0, strdup(buffer)};
}

Token get_next_token(Lexer *lexer) {
    while (current_char(lexer) != '\0') {
        if (isspace(current_char(lexer))) {
            skip_whitespace(lexer);
            continue;
        }
        if (isdigit(current_char(lexer))) {
            return number(lexer);
        }
        if (isalpha(current_char(lexer)) || current_char(lexer) == '_') {
            return identifier_or_keyword(lexer);
        }
        if (current_char(lexer) == '+') {
            advance(lexer);
            return (Token){TOKEN_PLUS, 0, NULL};
        }
        if (current_char(lexer) == '-') {
            advance(lexer);
            return (Token){TOKEN_MINUS, 0, NULL};
        }
        if (current_char(lexer) == '*') {
            advance(lexer);
            return (Token){TOKEN_MUL, 0, NULL};
        }
        if (current_char(lexer) == '/') {
            advance(lexer);
            return (Token){TOKEN_DIV, 0, NULL};
        }
        if (current_char(lexer) == '(') {
            advance(lexer);
            return (Token){TOKEN_LPAREN, 0, NULL};
        }
        if (current_char(lexer) == ')') {
            advance(lexer);
            return (Token){TOKEN_RPAREN, 0, NULL};
        }
        if (current_char(lexer) == '=') {
            advance(lexer);
            if (current_char(lexer) == '=') {
                advance(lexer);
                return (Token){TOKEN_EQ, 0, NULL};
            }
            return (Token){TOKEN_ASSIGN, 0, NULL};
        }
        if (current_char(lexer) == '!') {
            advance(lexer);
            if (current_char(lexer) == '=') {
                advance(lexer);
                return (Token){TOKEN_NEQ, 0, NULL};
            }
            return (Token){TOKEN_NOT, 0, NULL};
        }
        if (current_char(lexer) == '<') {
            advance(lexer);
            if (current_char(lexer) == '=') {
                advance(lexer);
                return (Token){TOKEN_LTE, 0, NULL};
            }
            return (Token){TOKEN_LT, 0, NULL};
        }
        if (current_char(lexer) == '>') {
            advance(lexer);
            if (current_char(lexer) == '=') {
                advance(lexer);
                return (Token){TOKEN_GTE, 0, NULL};
            }
            return (Token){TOKEN_GT, 0, NULL};
        }
        if (current_char(lexer) == '&') {
            advance(lexer);
            if (current_char(lexer) == '&') {
                advance(lexer);
                return (Token){TOKEN_AND, 0, NULL};
            }
        }
        if (current_char(lexer) == '|') {
            advance(lexer);
            if (current_char(lexer) == '|') {
                advance(lexer);
                return (Token){TOKEN_OR, 0, NULL};
            }
        }
        if (current_char(lexer) == ';') {
            advance(lexer);
            return (Token){TOKEN_SEMICOLON, 0, NULL};
        }
        if (current_char(lexer) == '{') {
            advance(lexer);
            return (Token){TOKEN_LBRACE, 0, NULL};
        }
        if (current_char(lexer) == '}') {
            advance(lexer);
            return (Token){TOKEN_RBRACE, 0, NULL};
        }
        fprintf(stderr, "Unknown character: %c\n", current_char(lexer));
        exit(1);
    }
    return (Token){TOKEN_EOF, 0, NULL};
}

void lexer_advance(Lexer *lexer) {
    lexer->current_token = get_next_token(lexer);
}
