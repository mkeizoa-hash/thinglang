#include "lexer.h"
#include <ctype.h>
#include <string.h>

static int isAtEnd(Lexer* lexer) {
    return *lexer->current == '\0';
}

static char advance(Lexer* lexer) {
    lexer->current++;
    return lexer->current[-1];
}

static char peek(Lexer* lexer) {
    return *lexer->current;
}

static void skipWhitespace(Lexer* lexer) {
    for (;;) {
        char c = peek(lexer);
        switch(c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            case '\n':
                lexer->line++;
                advance(lexer);
                break;
            case '/':
                if (lexer->current[1] == '/') {
                    // consume until end of line
                    while (peek(lexer) != '\n' && !isAtEnd(lexer)) {
                        advance(lexer);
                    }
                } else {
                    return;
                }
                break;  

            default:
                return;
        }
    }
}

void initLexer(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

static Token makeToken(Lexer* lexer, TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;
    return token;
}

static Token number(Lexer* lexer) {
    while (isdigit(peek(lexer))) advance(lexer);
    
    return makeToken(lexer, TOK_INT);
}

static Token string(Lexer* lexer) {
    while (peek(lexer) != '"' && !isAtEnd(lexer)) {
        if (peek(lexer) == '"') lexer->line++;
        advance(lexer);
    }

    if (isAtEnd(lexer)) {
        return makeToken(lexer, TOK_INVALID);
    }

    advance(lexer);

    return makeToken(lexer, TOK_STR);
}

static TokenType identifierType(Lexer* lexer) {
    int length = lexer->current - lexer->start;

    switch (lexer->start[0]) {
        case 'v':
            if (length == 3 && strncmp(lexer->start, "var", 3) == 0)
                return TOK_VAR;
            break;

        case 'i':
            if (length == 2 && strncmp(lexer->start, "if", 2) == 0)
                return TOK_IF;
            break;

        case 'e':
            if (length == 4 && strncmp(lexer->start, "else", 4) == 0)
                return TOK_ELSE;
            break;

        case 'w':
            if (length == 5 && strncmp(lexer->start, "while", 5) == 0)
                return TOK_WHILE;
            break;

        case 'f':
            if (length == 2 && strncmp(lexer->start, "fn", 2) == 0)
                return TOK_FN;
            break;

        case 'r':
            if (length == 6 && strncmp(lexer->start, "return", 6) == 0)
                return TOK_RETURN;
            break;
    }
    
    return TOK_IDENT;

}

static Token identifier(Lexer* lexer) {
    while (isalnum(peek(lexer)) || peek(lexer) == '_')
        advance(lexer);

    return makeToken(lexer, identifierType(lexer));
}

static int match(Lexer* lexer, char expected) {
    if (isAtEnd(lexer)) return 0;
    if (*lexer->current != expected) return 0;

    lexer->current++;
    return 1;
}

Token nextToken(Lexer* lexer) {
    skipWhitespace(lexer);
    lexer->start = lexer->current;
    
    if (isAtEnd(lexer)) {
        return makeToken(lexer, TOK_EOF);
    }
    
    char c = advance(lexer);
    
    if (isdigit(c)) {
        return number(lexer);
    }
    
    if (isalpha(c) || c == '_') {
        return identifier(lexer);
    }
    
    switch (c) {
        case '+': return makeToken(lexer, TOK_PLUS);
        case '-': return makeToken(lexer, TOK_MINUS);
        case '*': return makeToken(lexer, TOK_STAR);
        case '/': return makeToken(lexer, TOK_SLASH);
        case '(': return makeToken(lexer, TOK_LPAREN);
        case ')': return makeToken(lexer, TOK_RPAREN);
        case ';': return makeToken(lexer, TOK_SEMICOLON);

        case '=':
            return makeToken(lexer,
                match(lexer, '=') ? TOK_EQUAL : TOK_ASSIGN);

        case '!':
            return makeToken(lexer,
                match(lexer, '=') ? TOK_NOT_EQUAL : TOK_INVALID);

        case '<':
            return makeToken(lexer,
                match(lexer, '=') ? TOK_LESS_EQUAL : TOK_LESS);

        case '>':
            return makeToken(lexer,
                match(lexer, '=') ? TOK_GREATER_EQUAL : TOK_GREATER);

        case '{': return makeToken(lexer, TOK_LBRACE);
        case '}': return makeToken(lexer, TOK_RBRACE);
        case ',': return makeToken(lexer, TOK_COMMA);

        case '"':
            return string(lexer);
        
        default:
            return makeToken(lexer, TOK_INVALID);
    }
}

