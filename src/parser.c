#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // Include for strdup

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

// Parse a factor (number or parenthesized expression)
ASTNode* factor(Lexer *lexer) {
    Token token = lexer->current_token;
    if (token.type == TOKEN_NUMBER) {
        lexer_advance(lexer);
        return init_ast_node(TOKEN_NUMBER, token.value);
    } else if (token.type == TOKEN_LPAREN) {
        lexer_advance(lexer);
        ASTNode *node = parse_expression(lexer);
        if (lexer->current_token.type == TOKEN_RPAREN) {
            lexer_advance(lexer);
        } else {
            // Error handling: unmatched parenthesis
            printf("Error: unmatched parenthesis\n");
            exit(1);
        }
        return node;
    } else if (token.type == TOKEN_MINUS) {
        lexer_advance(lexer);
        ASTNode *node = init_ast_node(TOKEN_MINUS, 0);
        node->right = factor(lexer); // Correctly set the right child for unary negation
        return node;
    }
    return NULL;
}

// Parse a term (multiplication and division)
ASTNode* term(Lexer *lexer) {
    ASTNode *node = factor(lexer);
    while (lexer->current_token.type == TOKEN_MUL || lexer->current_token.type == TOKEN_DIV) {
        Token token = lexer->current_token;
        lexer_advance(lexer);
        ASTNode *new_node = init_ast_node(token.type, 0);
        new_node->left = node;
        new_node->right = factor(lexer);
        node = new_node;
    }
    return node;
}

// Parse an expression (addition and subtraction)
ASTNode* parse_expression(Lexer *lexer) {
    ASTNode *node = term(lexer);
    while (lexer->current_token.type == TOKEN_PLUS || lexer->current_token.type == TOKEN_MINUS) {
        Token token = lexer->current_token;
        lexer_advance(lexer);
        ASTNode *new_node = init_ast_node(token.type, 0);
        new_node->left = node;
        new_node->right = term(lexer);
        node = new_node;
    }
    return node;
}

// Parse a block of statements
ASTNode* parse_block(Lexer *lexer) {
    ASTNode *block = init_ast_node(TOKEN_LBRACE, 0);
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

// Parse a single statement
ASTNode* parse_statement(Lexer *lexer) {
    if (lexer->current_token.type == TOKEN_PRINT) {
        return parse_print_statement(lexer);
    } else if (lexer->current_token.type == TOKEN_IDENTIFIER) {
        return parse_assignment_statement(lexer);
    } else if (lexer->current_token.type == TOKEN_WHILE) {
        return parse_while_statement(lexer);
    }
    return NULL;
}

// Parse a print statement
ASTNode* parse_print_statement(Lexer *lexer) {
    lexer_advance(lexer); // Advance past 'print'
    ASTNode *expr = parse_expression(lexer); // Parse the expression to print
    return init_ast_node_with_children(TOKEN_PRINT, expr);
}

// Parse an assignment statement
ASTNode* parse_assignment_statement(Lexer *lexer) {
    Token token = lexer->current_token;
    lexer_advance(lexer); // Advance past identifier
    if (lexer->current_token.type != TOKEN_ASSIGN) {
        return NULL; // Error: expected '='
    }
    lexer_advance(lexer); // Advance past '='
    ASTNode *expr = parse_expression(lexer);
    ASTNode *node = init_ast_node_with_children(TOKEN_ASSIGN, expr);
    node->name = strdup(token.name); // Store the variable name
    return node;
}

// Parse a while statement
ASTNode* parse_while_statement(Lexer *lexer) {
    lexer_advance(lexer); // Advance past 'while'
    ASTNode *condition = parse_expression(lexer);
    ASTNode *body = parse_block(lexer);
    ASTNode *node = init_ast_node_with_children(TOKEN_WHILE, condition);
    node->body = body;
    return node;
}

// Initialize an AST node with children
ASTNode* init_ast_node_with_children(TokenType type, ASTNode *child) {
    ASTNode *node = init_ast_node(type, 0);
    node->left = child;
    return node;
}

// Initialize a basic AST node
ASTNode* init_ast_node(TokenType type, double value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->name = NULL;
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
    // Determine whether we are parsing an expression or a block
    if (lexer->current_token.type == TOKEN_LBRACE) {
        ASTNode *root = parse_block(lexer);
        // Debug: Print the root node and its children
        printf("Parsed AST Root: type=%d\n", root->type);
        print_ast_node(root, 0); // Print the entire AST
        return root;
    } else {
        ASTNode *root = parse_expression(lexer);
        // Debug: Print the root node and its children
        printf("Parsed AST Root: type=%d\n", root->type);
        print_ast_node(root, 0); // Print the entire AST
        return root;
    }
}