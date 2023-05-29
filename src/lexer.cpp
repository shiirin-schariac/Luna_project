#include "include/lexer.h"
#include "include/token.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include <vector>

lexer_T *init_lexer(char *contents)
{
    lexer_T *lexer = new LEXER_STURCT[1]();
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    return lexer;
}

void lexer_advance(lexer_T *lexer)
{
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
    {
        lexer->i++;
        lexer->c = lexer->contents[lexer->i];
    }
}

void lexer_skip_blank(lexer_T *lexer)
{
    while (lexer->c == ' ' || lexer->c == '\n')
    // 忽略换行与空格
    {
        lexer_advance(lexer);
    }
}

token_T *lexer_get_next_token(lexer_T *lexer)
{
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
    {
        if (lexer->c == ' ' || lexer->c == '\n')
        {
            lexer_skip_blank(lexer);
        }

        if (isalnum(lexer->c) || lexer->c == '_')
        {
            if (lexer->c >= '0' && lexer->c <= '9')
            {
                return lexer_collect_number(lexer);
            }
            else
                return lexer_collect_id(lexer);
        }

        if (lexer->c == '"')
        {
            return lexer_collect_string(lexer);
        }

        // if (lexer->c >= '0' && lexer->c <= '9')
        // {
        //     return lexer_collect_number(lexer);
        // }

        switch (lexer->c)
        {
        case '=':
        {
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
            break;
        }
        case ';':
        {
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_SEMI, lexer_get_current_char_as_string(lexer)));
            break;
        }
        case '(':
        {
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        }
        case ')':
        {
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        }
        case '{':
        {
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_LBRACE, lexer_get_current_char_as_string(lexer)));
            break;
        }
        case '}':
        {
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_RBRACE, lexer_get_current_char_as_string(lexer)));
            break;
        }
        case ',':
        {
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_COMMA, lexer_get_current_char_as_string(lexer)));
            break;
        }
        }
    }

    return init_token(TOKEN_STRUCT::TOKEN_EOF, "\0");
}

token_T *lexer_collect_string(lexer_T *lexer)
{
    lexer_advance(lexer);

    char *value = new char[1]();
    value[0] = '\0';

    while (lexer->c != '"')
    {
        char *s = lexer_get_current_char_as_string(lexer);
        // char *temp = new char[(strlen(value) + strlen(s) + 1)]();
        // std::copy(value, value + 1, temp);
        // delete[] value;
        // value = temp;
        // value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(TOKEN_STRUCT::TOKEN_STRING, value);
}

token_T *lexer_collect_id(lexer_T *lexer)
{
    char *value = new char[1]();
    value[0] = '\0';

    while (isalnum(lexer->c))
    {
        char *s = lexer_get_current_char_as_string(lexer);
        // char *temp = new char[(strlen(value) + strlen(s) + 1)]();
        // std::copy(value, value + 1, temp);
        // delete[] value;
        // value = temp;
        // value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    return init_token(TOKEN_STRUCT::TOKEN_ID, value);
}

token_T *lexer_collect_number(lexer_T *lexer)
{
    char *value = new char[1]();
    value[0] = '\0';

    while (isalnum(lexer->c) || lexer->c == '.')
    // 注意特判小数点
    {
        char *s = lexer_get_current_char_as_string(lexer);
        value = (char *)realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    regex_t regex;
    int ret;
    regmatch_t matches[2];

    const char *intPattern = "^[0-9]+$";
    const char *floatPattern = "^[0-9]+\\.[0-9]+$";
    const char *scientificNotationPattern = "^-?\\d+(\\.\\d+)?([eE][+-]?\\d+)?$";

    regcomp(&regex, intPattern, REG_EXTENDED);

    // 尝试匹配整型
    if (regexec(&regex, value, 2, matches, 0) == 0)
    {
        return init_token(TOKEN_STRUCT::TOKEN_INTEGER, value);
    }
    else
    {
        // 尝试匹配浮点型
        regcomp(&regex, floatPattern, REG_EXTENDED);
        if (regexec(&regex, value, 2, matches, 0) == 0)
        {
            return init_token(TOKEN_STRUCT::TOKEN_FLOAT, value);
        }
        // 尝试匹配科学记数法
        regcomp(&regex, scientificNotationPattern, REG_EXTENDED);
        if (regexec(&regex, value, 2, matches, 0) == 0)
        {
            return init_token(TOKEN_STRUCT::TOKEN_FLOAT, value);
        }
    }

    // 释放正则表达式
    regfree(&regex);

    return init_token(TOKEN_STRUCT::TOKEN_INVALID, value);
}

// TODO:to convert the number token from string into int/float
// maybe do it in parser

token_T *lexer_advance_with_token(lexer_T *lexer, token_T *token)
{
    lexer_advance(lexer);
    return token;
}

char *lexer_get_current_char_as_string(lexer_T *lexer)
{
    char *str = new char[2]();

    str[0] = lexer->c;
    str[1] = '\0';

    return str;
}