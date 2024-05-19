#include "interpreter.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define a structure for a variable
typedef struct {
    char *name;
    Value value;
} Variable;

#define MAX_GLOBALS 100 // Maximum number of global variables
Variable globals[MAX_GLOBALS];
int global_count = 0;

// Get the value of a variable
Value get_variable_value(const char *name) {
    for (int i = 0; i < global_count; i++) {
        if (strcmp(globals[i].name, name) == 0) {
            return globals[i].value;
        }
    }
    fprintf(stderr, "Undefined variable: %s\n", name);
    exit(1);
}

// Set the value of a variable
void set_variable_value(const char *name, Value value) {
    for (int i = 0; i < global_count; i++) {
        if (strcmp(globals[i].name, name) == 0) {
            globals[i].value = value;
            return;
        }
    }
    if (global_count >= MAX_GLOBALS) {
        fprintf(stderr, "Too many global variables\n");
        exit(1);
    }
    globals[global_count].name = strdup(name);
    globals[global_count].value = value;
    global_count++;
}

// Interpret an AST node
void interpret(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case TOKEN_PRINT:
            interpret_print(node);
            break;
        case TOKEN_ASSIGN:
            {
                Value value = evaluate_expression(node->left);
                set_variable_value(node->name, value);
            }
            break;
        case TOKEN_WHILE:
            {
                while (evaluate_expression(node->left).value.boolean) {
                    interpret(node->body);
                }
            }
            break;
        default:
            if (node->type == TOKEN_PLUS || node->type == TOKEN_MINUS ||
                node->type == TOKEN_MUL || node->type == TOKEN_DIV ||
                node->type == TOKEN_NUMBER || node->type == TOKEN_STRING ||
                node->type == TOKEN_LPAREN || node->type == TOKEN_EQ ||
                node->type == TOKEN_NEQ || node->type == TOKEN_LT ||
                node->type == TOKEN_GT || node->type == TOKEN_LTE ||
                node->type == TOKEN_GTE || node->type == TOKEN_AND ||
                node->type == TOKEN_OR || node->type == TOKEN_NOT ||
                node->type == TOKEN_TRUE || node->type == TOKEN_FALSE) {
                Value result = evaluate_expression(node);
                switch (result.type) {
                    case VAL_STRING:
                        printf("Result: %s\n", result.value.string);
                        break;
                    case VAL_BOOL:
                        printf("Result: %s\n", result.value.boolean ? "True" : "False");
                        break;
                    case VAL_NUMBER:
                    default:
                        printf("Result: %f\n", result.value.number);
                        break;
                }
            }
            break;
    }

    interpret(node->next); // Continue to the next statement in the sequence
}

// Interpret a print statement
void interpret_print(ASTNode *node) {
    if (node->left) {
        Value result = evaluate_expression(node->left);
        switch (result.type) {
            case VAL_STRING:
                printf("Print: %s\n", result.value.string);
                break;
            case VAL_BOOL:
                printf("Print: %s\n", result.value.boolean ? "True" : "False");
                break;
            case VAL_NUMBER:
            default:
                printf("Print: %f\n", result.value.number);
                break;
        }
    }
}

// Function to convert a number to a string
char* number_to_string(double number) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%f", number);
    return strdup(buffer);
}

// Function to convert a value to a string
char* value_to_string(Value value) {
    if (value.type == VAL_STRING) {
        return strdup(value.value.string);
    } else if (value.type == VAL_NUMBER) {
        return number_to_string(value.value.number);
    } else if (value.type == VAL_BOOL) {
        return strdup(value.value.boolean ? "True" : "False");
    }
    return strdup("");
}

// Evaluate an expression
Value evaluate_expression(ASTNode *node) {
    Value result;
    if (!node) {
        result.type = VAL_NUMBER;
        result.value.number = 0;
        return result;
    }
    printf("Evaluating node: type=%d, value=%f\n", node->type, node->value); // Debug: print node info

    Value left_result, right_result;
    char *left_str, *right_str;

    switch (node->type) {
        case TOKEN_NUMBER:
            result.type = VAL_NUMBER;
            result.value.number = node->value;
            return result;
        case TOKEN_STRING:
            result.type = VAL_STRING;
            result.value.string = node->name;
            return result;
        case TOKEN_TRUE:
            result.type = VAL_BOOL;
            result.value.boolean = 1;
            return result;
        case TOKEN_FALSE:
            result.type = VAL_BOOL;
            result.value.boolean = 0;
            return result;
        case TOKEN_IDENTIFIER:
            return get_variable_value(node->name); // Retrieve the value of a variable
        case TOKEN_PLUS:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            if (left_result.type == VAL_STRING || right_result.type == VAL_STRING) {
                left_str = value_to_string(left_result);
                right_str = value_to_string(right_result);
                char *result_str = malloc(strlen(left_str) + strlen(right_str) + 1);
                strcpy(result_str, left_str);
                strcat(result_str, right_str);
                free(left_str);
                free(right_str);
                result.type = VAL_STRING;
                result.value.string = result_str;
                return result;
            }
            result.type = VAL_NUMBER;
            result.value.number = left_result.value.number + right_result.value.number;
            return result;
        case TOKEN_MINUS:
            result.type = VAL_NUMBER;
            if (node->left) {
                result.value.number = evaluate_expression(node->left).value.number - evaluate_expression(node->right).value.number;
            } else {
                result.value.number = -evaluate_expression(node->right).value.number; // Handle unary negation
            }
            return result;
        case TOKEN_MUL:
            result.type = VAL_NUMBER;
            result.value.number = evaluate_expression(node->left).value.number * evaluate_expression(node->right).value.number;
            return result;
        case TOKEN_DIV:
            result.type = VAL_NUMBER;
            result.value.number = evaluate_expression(node->left).value.number / evaluate_expression(node->right).value.number;
            return result;
        case TOKEN_EQ:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            result.type = VAL_BOOL;
            if (left_result.type == VAL_STRING || right_result.type == VAL_STRING) {
                left_str = value_to_string(left_result);
                right_str = value_to_string(right_result);
                result.value.boolean = strcmp(left_str, right_str) == 0;
                free(left_str);
                free(right_str);
            } else {
                printf("Evaluating EQ: left=%f, right=%f\n", left_result.value.number, right_result.value.number);
                result.value.boolean = left_result.value.number == right_result.value.number;
            }
            return result;
        case TOKEN_NEQ:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            result.type = VAL_BOOL;
            if (left_result.type == VAL_STRING || right_result.type == VAL_STRING) {
                left_str = value_to_string(left_result);
                right_str = value_to_string(right_result);
                result.value.boolean = strcmp(left_str, right_str) != 0;
                free(left_str);
                free(right_str);
            } else {
                printf("Evaluating NEQ: left=%f, right=%f\n", left_result.value.number, right_result.value.number);
                result.value.boolean = left_result.value.number != right_result.value.number;
            }
            return result;
        case TOKEN_LT:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating LT: left=%f, right=%f\n", left_result.value.number, right_result.value.number);
            result.type = VAL_BOOL;
            result.value.boolean = left_result.value.number < right_result.value.number;
            return result;
        case TOKEN_GT:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating GT: left=%f, right=%f\n", left_result.value.number, right_result.value.number);
            result.type = VAL_BOOL;
            result.value.boolean = left_result.value.number > right_result.value.number;
            return result;
        case TOKEN_LTE:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating LTE: left=%f, right=%f\n", left_result.value.number, right_result.value.number);
            result.type = VAL_BOOL;
            result.value.boolean = left_result.value.number <= right_result.value.number;
            return result;
        case TOKEN_GTE:
            left_result = evaluate_expression(node->left);
            right_result = evaluate_expression(node->right);
            printf("Evaluating GTE: left=%f, right=%f\n", left_result.value.number, right_result.value.number);
            result.type = VAL_BOOL;
            result.value.boolean = left_result.value.number >= right_result.value.number;
            return result;
        case TOKEN_AND:
            left_result = evaluate_expression(node->left);
            printf("Evaluating AND: left=%f\n", left_result.value.number);
            if (!left_result.value.number) {
                result.type = VAL_BOOL;
                result.value.boolean = 0;
                return result; // Short-circuit evaluation
            }
            right_result = evaluate_expression(node->right);
            printf("Evaluating AND: right=%f\n", right_result.value.number);
            result.type = VAL_BOOL;
            result.value.boolean = left_result.value.number && right_result.value.number;
            return result;
        case TOKEN_OR:
            left_result = evaluate_expression(node->left);
            printf("Evaluating OR: left=%f\n", left_result.value.number);
            if (left_result.value.number) {
                result.type = VAL_BOOL;
                result.value.boolean = 1;
                return result; // Short-circuit evaluation
            }
            right_result = evaluate_expression(node->right);
            printf("Evaluating OR: right=%f\n", right_result.value.number);
            result.type = VAL_BOOL;
            result.value.boolean = left_result.value.number || right_result.value.number;
            return result;
        case TOKEN_NOT:
            right_result = evaluate_expression(node->right);
            printf("Evaluating NOT: right=%f\n", right_result.value.number);
            result.type = VAL_BOOL;
            result.value.boolean = !right_result.value.number;
            return result;
        default:
            printf("Unknown node type: %d\n", node->type); // Debug: unknown node type
            result.type = VAL_NUMBER;
            result.value.number = 0;
            return result;
    }
}

// Free the AST nodes
void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    if (node->type == TOKEN_STRING && node->name) {
        free(node->name);
    }
    free(node);
}
