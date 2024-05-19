#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Utility function to print AST nodes
void print_ast_node(ASTNode *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("ASTNode: type=%d, value=%f, name=%s\n", node->type, node->value, node->name ? node->name : "NULL");
    print_ast_node(node->left, depth + 1);
    print_ast_node(node->right, depth + 1);
    print_ast_node(node->condition, depth + 1);
    print_ast_node(node->body, depth + 1);
    print_ast_node(node->else_body, depth + 1);
    print_ast_node(node->next, depth + 1);
}

// Parse a factor (number, string, boolean, or parenthesized expression)
ASTNode* factor(Lexer *lexer) {
    Token token = lexer->current_token;
    if (token.type == TOKEN_NUMBER) {
        lexer_advance(lexer);
        return init_ast_node(TOKEN_NUMBER, token.value, NULL);
    } else if (token.type == TOKEN_STRING) {
        lexer_advance(lexer);
        return init_ast_node(TOKEN_STRING, 0, token.name); // Use token.name
    } else if (token.type == TOKEN_TRUE || token.type == TOKEN_FALSE) {
        lexer_advance(lexer);
        return init_ast_node(token.type, token.type == TOKEN_TRUE ? 1 : 0, NULL);
    } else if (token.type == TOKEN_LPAREN) {
        lexer_advance(lexer);
        ASTNode *node = parse_expression(lexer);
        if (lexer->current_token.type == TOKEN_RPAREN) {
            lexer_advance(lexer);
        } else {
            printf("Error: unmatched parenthesis\n");
            exit(1);
        }
        return node;
    } else if (token.type == TOKEN_MINUS || token.type == TOKEN_NOT) {
        lexer_advance(lexer);
        ASTNode *node = init_ast_node(token.type, 0, NULL);
        node->right = factor(lexer); // Handle unary operators
        return node;
    } else if (token.type == TOKEN_IDENTIFIER) {
        lexer_advance(lexer);
        return init_ast_node(TOKEN_IDENTIFIER, 0, token.name); // Use token.name
    }
    printf("Error: unknown factor: %d\n", token.type); // Debug: unknown factor
    exit(1);
    return NULL;
}

// Parse a term (multiplication and division)
ASTNode* term(Lexer *lexer) {
    ASTNode *node = factor(lexer);
    while (lexer->current_token.type == TOKEN_MUL || lexer->current_token.type == TOKEN_DIV) {
        Token token = lexer->current_token;
        lexer_advance(lexer);
        ASTNode *new_node = init_ast_node(token.type, 0, NULL);
        new_node->left = node;
        new_node->right = factor(lexer);
        node = new_node;
    }
    return node;
}

// Parse an arithmetic expression (addition, subtraction, and string concatenation)
ASTNode* arithmetic_expression(Lexer *lexer) {
    ASTNode *node = term(lexer);
    while (lexer->current_token.type == TOKEN_PLUS || lexer->current_token.type == TOKEN_MINUS) {
        Token token = lexer->current_token;
        lexer_advance(lexer);
        ASTNode *new_node = init_ast_node(token.type, 0, NULL);
        new_node->left = node;
        new_node->right = term(lexer);
        node = new_node;
    }
    return node;
}

// Parse a comparison expression
ASTNode* comparison(Lexer *lexer) {
    ASTNode *node = arithmetic_expression(lexer);
    while (lexer->current_token.type == TOKEN_LT || lexer->current_token.type == TOKEN_GT ||
           lexer->current_token.type == TOKEN_LTE || lexer->current_token.type == TOKEN_GTE) {
        Token token = lexer->current_token;
        lexer_advance(lexer);
        ASTNode *new_node = init_ast_node(token.type, 0, NULL);
        new_node->left = node;
        new_node->right = arithmetic_expression(lexer);
        node = new_node;
    }
    return node;
}

// Parse an equality expression
ASTNode* equality(Lexer *lexer) {
    ASTNode *node = comparison(lexer);
    while (lexer->current_token.type == TOKEN_EQ || lexer->current_token.type == TOKEN_NEQ) {
        Token token = lexer->current_token;
        lexer_advance(lexer);
        ASTNode *new_node = init_ast_node(token.type, 0, NULL);
        new_node->left = node;
        new_node->right = comparison(lexer);
        node = new_node;
    }
    return node;
}

// Parse a logical AND expression
ASTNode* logical_and(Lexer *lexer) {
    ASTNode *node = equality(lexer);
    while (lexer->current_token.type == TOKEN_AND) {
        Token token = lexer->current_token;
        lexer_advance(lexer);
        ASTNode *new_node = init_ast_node(token.type, 0, NULL);
        new_node->left = node;
        new_node->right = equality(lexer);
        node = new_node;
    }
    return node;
}

// Parse a logical OR expression
ASTNode* logical_or(Lexer *lexer) {
    ASTNode *node = logical_and(lexer);
    while (lexer->current_token.type == TOKEN_OR) {
        Token token = lexer->current_token;
        lexer_advance(lexer);
        ASTNode *new_node = init_ast_node(token.type, 0, NULL);
        new_node->left = node;
        new_node->right = logical_and(lexer);
        node = new_node;
    }
    return node;
}

// Parse an expression (logical OR)
ASTNode* parse_expression(Lexer *lexer) {
    return logical_or(lexer);
}

// Parse a block of statements
ASTNode* parse_block(Lexer *lexer) {
    ASTNode *block = init_ast_node(TOKEN_LBRACE, 0, NULL);
    lexer_advance(lexer); // Advance past '{'
    while (lexer->current_token.type != TOKEN_RBRACE && lexer->current_token.type != TOKEN_EOF) {
        ASTNode *stmt = parse_statement(lexer);
        if (stmt) {
            append_ast_node(block, stmt);
        }
        lexer_advance(lexer);
    }
    return block;
}

// Function to parse multiple statements
ASTNode* parse_statements(Lexer *lexer) {
    ASTNode *node = parse_statement(lexer);
    ASTNode *root = node;

    while (lexer->current_token.type != TOKEN_EOF) {
        ASTNode *next_node = parse_statement(lexer);
        if (!next_node) break;
        node->next = next_node;
        node = next_node;
    }

    return root;
}

// Parse a single statement
ASTNode* parse_statement(Lexer *lexer) {
    printf("Parsing statement: current token type = %d\n", lexer->current_token.type);
    if (lexer->current_token.type == TOKEN_PRINT) {
        return parse_print_statement(lexer);
    } else if (lexer->current_token.type == TOKEN_IDENTIFIER) {
        Token token = lexer->current_token;
        lexer_advance(lexer);
        if (lexer->current_token.type == TOKEN_ASSIGN) {
            lexer->pos -= 2; // Go back to the beginning of the identifier
            lexer->current_token = token; // Reset current token
            return parse_assignment_statement(lexer);
        } else {
            lexer->pos -= 1; // Reset lexer position
            lexer->current_token = token; // Reset current token
        }
    } 
    
    if (lexer->current_token.type == TOKEN_TRUE || lexer->current_token.type == TOKEN_FALSE || 
        lexer->current_token.type == TOKEN_LPAREN || lexer->current_token.type == TOKEN_NUMBER || 
        lexer->current_token.type == TOKEN_STRING || lexer->current_token.type == TOKEN_IDENTIFIER) {
        // Parse and return the expression as a statement
        ASTNode *expr = parse_expression(lexer);
        if (expr) {
            lexer_advance(lexer); // Ensure lexer advances after parsing the expression
            return expr;
        }
    }

    // If no valid statement is found, return NULL (or handle error)
    printf("Error: unknown statement\n");
    return NULL;
}

// Parse a print statement
ASTNode* parse_print_statement(Lexer *lexer) {
    printf("Parsing print statement\n");
    lexer_advance(lexer); // Advance past 'print'
    ASTNode *expr = parse_expression(lexer); // Parse the expression to print
    return init_ast_node_with_children(TOKEN_PRINT, expr);
}

// Parse an assignment statement
ASTNode* parse_assignment_statement(Lexer *lexer) {
    printf("Parsing assignment statement\n");
    Token token = lexer->current_token;
    lexer_advance(lexer); // Advance past identifier
    if (lexer->current_token.type != TOKEN_ASSIGN) {
        printf("Error: expected '='\n");
        return NULL;
    }
    lexer_advance(lexer); // Advance past '='
    ASTNode *expr = parse_expression(lexer);
    ASTNode *node = init_ast_node_with_children(TOKEN_ASSIGN, expr);
    node->name = strdup(token.name); // Store the variable name
    return node;
}

// Parse a while statement
ASTNode* parse_while_statement(Lexer *lexer) {
    printf("Parsing while statement\n");
    lexer_advance(lexer); // Advance past 'while'
    ASTNode *condition = parse_expression(lexer);
    ASTNode *body = parse_block(lexer);
    ASTNode *node = init_ast_node_with_children(TOKEN_WHILE, condition);
    node->body = body;
    return node;
}

// Initialize an AST node with children
ASTNode* init_ast_node_with_children(TokenType type, ASTNode *child) {
    ASTNode *node = init_ast_node(type, 0, NULL);
    node->left = child;
    return node;
}

// Initialize a basic AST node
ASTNode* init_ast_node(TokenType type, double value, char *name) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value;
    node->name = name ? strdup(name) : NULL;
    node->left = NULL;
    node->right = NULL;
    node->condition = NULL;
    node->body = NULL;
    node->else_body = NULL;
    node->next = NULL;
    return node;
}

// Append an AST node as a child to a parent node (for block statements)
void append_ast_node(ASTNode *parent, ASTNode *child) {
    if (!parent->left) {
        parent->left = child;
    } else {
        ASTNode *current = parent->left;
        while (current->right) {
            current = current->right;
        }
        current->right = child;
    }
}

// Entry point for parsing
ASTNode* parse(Lexer *lexer) {
    lexer_advance(lexer);
    printf("Starting parsing\n");
    ASTNode *root = parse_statements(lexer);
    printf("Finished parsing\n");
    return root;
}
