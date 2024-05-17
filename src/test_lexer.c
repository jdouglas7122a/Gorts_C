#include "test_lexer.h"
#include <stdio.h>

int main() {
    TestLexer lexer;
    lexer.pos = 10;
    printf("size_t is recognized and pos is %zu\n", lexer.pos);
    return 0;
}
