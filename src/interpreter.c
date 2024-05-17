#include "interpreter.h"
#include <stdio.h>

// Interpret an AST node
void interpret(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case TOKEN_PRINT:
            interpret_print(node);
            break;
        case TOKEN_ASSIGN:
            // Implement variable assignment interpretation here
            break;
        case TOKEN_WHILE:
            // Implement while loop interpretation here
            break;
        // Add other cases for different statement types
        default:
            // Evaluate and print the root expression
            if (node->type == TOKEN_PLUS || node->type == TOKEN_MINUS ||
                node->type == TOKEN_MUL || node->type == TOKEN_DIV ||
                node->type == TOKEN_NUMBER || node->type == TOKEN_LPAREN) {
                double result = evaluate_expression(node);
                printf("Result: %f\n", result);
            }
            break;
    }
}

// Interpret a print statement
void interpret_print(ASTNode *node) {
    if (node->left) {
        double result = evaluate_expression(node->left);
        printf("Print: %f\n", result); // Example output format
    }
}

// Evaluate an expression
double evaluate_expression(ASTNode *node) {
    if (!node) return 0;
    printf("Evaluating: type=%d, value=%f\n", node->type, node->value); // Debug: print node info

    switch (node->type) {
        case TOKEN_NUMBER:
            return node->value;
        case TOKEN_PLUS:
            return evaluate_expression(node->left) + evaluate_expression(node->right);
        case TOKEN_MINUS:
            if (node->left) {
                return evaluate_expression(node->left) - evaluate_expression(node->right);
            } else {
                return -evaluate_expression(node->right); // Handle unary negation
            }
        case TOKEN_MUL:
            return evaluate_expression(node->left) * evaluate_expression(node->right);
        case TOKEN_DIV:
            return evaluate_expression(node->left) / evaluate_expression(node->right);
        default:
            return 0;
    }
}

// Free the AST nodes
void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}
