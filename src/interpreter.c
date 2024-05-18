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
        default:
            if (node->type == TOKEN_PLUS || node->type == TOKEN_MINUS ||
                node->type == TOKEN_MUL || node->type == TOKEN_DIV ||
                node->type == TOKEN_NUMBER || node->type == TOKEN_LPAREN ||
                node->type == TOKEN_EQ || node->type == TOKEN_NEQ ||
                node->type == TOKEN_LT || node->type == TOKEN_GT ||
                node->type == TOKEN_LTE || node->type == TOKEN_GTE ||
                node->type == TOKEN_AND || node->type == TOKEN_OR ||
                node->type == TOKEN_NOT || node->type == TOKEN_TRUE ||
                node->type == TOKEN_FALSE) {
                double result = evaluate_expression(node);
                if (node->type == TOKEN_TRUE || node->type == TOKEN_FALSE ||
                    node->type == TOKEN_EQ || node->type == TOKEN_NEQ ||
                    node->type == TOKEN_LT || node->type == TOKEN_GT ||
                    node->type == TOKEN_LTE || node->type == TOKEN_GTE ||
                    node->type == TOKEN_AND || node->type == TOKEN_OR ||
                    node->type == TOKEN_NOT) {
                    printf("Result: %s\n", result ? "True" : "False");
                } else {
                    printf("Result: %f\n", result);
                }
            }
            break;
    }
}

// Interpret a print statement
void interpret_print(ASTNode *node) {
    if (node->left) {
        double result = evaluate_expression(node->left);
        if (node->left->type == TOKEN_TRUE || node->left->type == TOKEN_FALSE ||
            node->left->type == TOKEN_EQ || node->left->type == TOKEN_NEQ ||
            node->left->type == TOKEN_LT || node->left->type == TOKEN_GT ||
            node->left->type == TOKEN_LTE || node->left->type == TOKEN_GTE ||
            node->left->type == TOKEN_AND || node->left->type == TOKEN_OR ||
            node->left->type == TOKEN_NOT) {
            printf("Print: %s\n", result ? "True" : "False");
        } else {
            printf("Print: %f\n", result);
        }
    }
}

// Evaluate an expression
double evaluate_expression(ASTNode *node) {
    if (!node) return 0;
    printf("Evaluating node: type=%d, value=%f\n", node->type, node->value); // Debug: print node info

    double left_result, right_result;

    switch (node->type) {
        case TOKEN_NUMBER:
            return node->value;
        case TOKEN_TRUE:
            return 1;
        case TOKEN_FALSE:
            return 0;
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
        case TOKEN_EQ:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating EQ: left=%f, right=%f\n", left_result, right_result);
            return left_result == right_result;
        case TOKEN_NEQ:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating NEQ: left=%f, right=%f\n", left_result, right_result);
            return left_result != right_result;
        case TOKEN_LT:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating LT: left=%f, right=%f\n", left_result, right_result);
            return left_result < right_result;
        case TOKEN_GT:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating GT: left=%f, right=%f\n", left_result, right_result);
            return left_result > right_result;
        case TOKEN_LTE:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating LTE: left=%f, right=%f\n", left_result, right_result);
            return left_result <= right_result;
        case TOKEN_GTE:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating GTE: left=%f, right=%f\n", left_result, right_result);
            return left_result >= right_result;
        case TOKEN_AND: {
            left_result = evaluate_expression(node->left);
            printf("Evaluating AND: left=%f\n", left_result);
            if (!left_result) return 0; // Short-circuit evaluation
            right_result = evaluate_expression(node->right);
            printf("Evaluating AND: right=%f\n", right_result);
            return left_result && right_result;
        }
        case TOKEN_OR: {
            left_result = evaluate_expression(node->left);
            printf("Evaluating OR: left=%f\n", left_result);
            if (left_result) return 1; // Short-circuit evaluation
            right_result = evaluate_expression(node->right);
            printf("Evaluating OR: right=%f\n", right_result);
            return left_result || right_result;
        }
        case TOKEN_NOT: {
            right_result = evaluate_expression(node->right);
            printf("Evaluating NOT: right=%f\n", right_result);
            return !right_result;
        }
        default:
            printf("Unknown node type: %d\n", node->type); // Debug: unknown node type
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