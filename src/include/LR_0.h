#ifndef LR_0_H
#define LR_0_H

#include "token.h"
#include <unordered_map>

typedef struct LR_0
{
    enum lr_node_type
    {
        LR_NUMBER,
        LR_ADD,
        LR_SUB,
        LR_MULTI,
        LR_DIV,
        LR_LPAREN,
        LR_RPAREN,
        LR_START,
        LR_END,
        LR_EXPR,
        LR_TERM,
        LR_FACTOR
    } lr_type;
} lr_0;

char *search_parsetable(int now_state, int next_type);

int transfer_token_lr_type(int token_type);

#endif