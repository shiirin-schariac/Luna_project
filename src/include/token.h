#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
    enum token_type
    {
        TOKEN_ID, // identifier
        TOKEN_EQUALS,
        TOKEN_STRING,
        TOKEN_SEMI,   // 双引号
        TOKEN_LPAREN, // 左括号
        TOKEN_RPAREN, // 右括号
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_COMMA,
        TOKEN_ADD,
        TOKEN_SUB,
        TOKEN_MULTIPIE,
        TOKEN_DIV,
        TOKEN_MOD,
        TOKEN_LARGER,
        TOKEN_LESS,
        TOKEN_EQUAL,
        TOKEN_LESSEQUAL,
        TOKEN_LARGEEQUAL,
        TOKEN_INTEGER,
        TOKEN_FLOAT,
        TOKEN_INVALID,
        TOKEN_EOF,
    } type;

    char *value;
} token_T;

token_T *init_token(int type, char *value);

#endif