#include <stdio.h>
#include "lexer.h"

int main() {
    const char* source =
    "let x = 10;\n"
    "let msg = \"hello world\";\n"
    "// comment\n"
    "x = x + 1;";
    Lexer lexer;
    initLexer(&lexer, source);

    Token token;
    do {
        token = nextToken(&lexer);
        printf("Type: %d, Text: %.*s\n", 
                token.type,
                token.length,
                token.start
        );
    } while (token.type != TOK_EOF);

    return 0;
}