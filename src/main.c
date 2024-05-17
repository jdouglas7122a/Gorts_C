#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = (char*)malloc(length + 1);
    if (!source) {
        perror("Failed to allocate memory");
        fclose(file);
        return 1;
    }

    if (fread(source, 1, length, file) != (size_t)length) {
        perror("Failed to read file");
        free(source);
        fclose(file);
        return 1;
    }
    source[length] = '\0';
    fclose(file);

    // Debug output to verify file reading
    printf("Source code:\n%s\n", source);

    Lexer *lexer = init_lexer(source);
    ASTNode *root = parse(lexer);

    interpret(root);

    free(source);
    free(lexer);
    free_ast(root);

    return 0;
}
