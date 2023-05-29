#include "include/token.h"
#include <stdlib.h>

token_T *init_token(int type, char *value)
{
    token_T *token = new TOKEN_STRUCT[1]();
    token->type = (TOKEN_STRUCT::token_type)type;
    token->value = value;

    return token;
}