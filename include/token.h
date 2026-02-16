#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOK_EOF,

    // literals
    TOK_INT,
    TOK_STR,
    TOK_IDENT,

    // keywords
    TOK_VAR,
    TOK_IF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_FN,
    TOK_RETURN,

    // operators
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_ASSIGN,

    TOK_EQUAL,
    TOK_NOT_EQUAL,
    TOK_GREATER,
    TOK_GREATER_EQUAL,
    TOK_LESS,
    TOK_LESS_EQUAL,

    // punctuation
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_SEMICOLON,
    TOK_COMMA,

    TOK_INVALID
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

#endif