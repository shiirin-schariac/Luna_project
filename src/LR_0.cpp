#include "include/LR_0.h"
#include <string.h>
#include <unordered_map>

std::unordered_map<int, std::unordered_map<LR_0::lr_node_type, char *>> ParsingTable = {
    {0, {{LR_0::LR_NUMBER, (char *)"s4"}, {LR_0::LR_LPAREN, (char *)"s5"}, {LR_0::LR_END, (char *)"s16"}, {LR_0::LR_EXPR, (char *)"s1"}, {LR_0::LR_TERM, (char *)"s2"}, {LR_0::LR_FACTOR, (char *)"s3"}}},
    {1, {{LR_0::LR_ADD, (char *)"s6"}, {LR_0::LR_SUB, (char *)"s7"}, {LR_0::LR_END, (char *)"r1"}}},
    {2, {{LR_0::LR_ADD, (char *)"r4"}, {LR_0::LR_SUB, (char *)"r4"}, {LR_0::LR_MULTI, (char *)"s10"}, {LR_0::LR_DIV, (char *)"s11"}, {LR_0::LR_RPAREN, (char *)"r4"}, {LR_0::LR_END, (char *)"r4"}}},
    {3, {{LR_0::LR_ADD, (char *)"r7"}, {LR_0::LR_SUB, (char *)"r7"}, {LR_0::LR_MULTI, (char *)"r7"}, {LR_0::LR_DIV, (char *)"r7"}, {LR_0::LR_RPAREN, (char *)"r7"}, {LR_0::LR_END, (char *)"r7"}}},
    {4, {{LR_0::LR_ADD, (char *)"r8"}, {LR_0::LR_SUB, (char *)"r8"}, {LR_0::LR_MULTI, (char *)(char *)"r8"}, {LR_0::LR_DIV, (char *)"r8"}, {LR_0::LR_RPAREN, (char *)"r8"}, {LR_0::LR_END, (char *)"r8"}}},
    {5, {{LR_0::LR_NUMBER, (char *)"s4"}, {LR_0::LR_LPAREN, (char *)"s5"}, {LR_0::LR_EXPR, (char *)"s14"}, {LR_0::LR_TERM, (char *)"s2"}, {LR_0::LR_FACTOR, (char *)"s3"}}},
    {6, {{LR_0::LR_NUMBER, (char *)"s4"}, {LR_0::LR_LPAREN, (char *)"s5"}, {LR_0::LR_TERM, (char *)"s8"}, {LR_0::LR_FACTOR, (char *)"s3"}}},
    {7, {{LR_0::LR_NUMBER, (char *)"s4"}, {LR_0::LR_LPAREN, (char *)"s5"}, {LR_0::LR_TERM, (char *)"s9"}, {LR_0::LR_FACTOR, (char *)"s3"}}},
    {8, {{LR_0::LR_ADD, (char *)"r2"}, {LR_0::LR_SUB, (char *)"r2"}, {LR_0::LR_MULTI, (char *)"s10"}, {LR_0::LR_DIV, (char *)"s11"}, {LR_0::LR_RPAREN, (char *)"r2"}, {LR_0::LR_END, (char *)"r2"}}},
    {9, {{LR_0::LR_ADD, (char *)"r3"}, {LR_0::LR_SUB, (char *)"r3"}, {LR_0::LR_MULTI, (char *)"s10"}, {LR_0::LR_DIV, (char *)"s11"}, {LR_0::LR_RPAREN, (char *)"r3"}, {LR_0::LR_END, (char *)"r3"}}},
    {10, {{LR_0::LR_NUMBER, (char *)"s4"}, {LR_0::LR_LPAREN, (char *)"s5"}, {LR_0::LR_FACTOR, (char *)"s12"}}},
    {11, {{LR_0::LR_NUMBER, (char *)"s4"}, {LR_0::LR_LPAREN, (char *)"s5"}, {LR_0::LR_FACTOR, (char *)"s13"}}},
    {12, {{LR_0::LR_ADD, (char *)"r5"}, {LR_0::LR_SUB, (char *)"r5"}, {LR_0::LR_MULTI, (char *)"r5"}, {LR_0::LR_DIV, (char *)"r5"}, {LR_0::LR_RPAREN, (char *)"r5"}, {LR_0::LR_END, (char *)"r5"}}},
    {13, {{LR_0::LR_ADD, (char *)"r6"}, {LR_0::LR_SUB, (char *)"r6"}, {LR_0::LR_MULTI, (char *)"r6"}, {LR_0::LR_DIV, (char *)"r6"}, {LR_0::LR_RPAREN, (char *)"r6"}, {LR_0::LR_END, (char *)"r6"}}},
    {14, {{LR_0::LR_ADD, (char *)"s6"}, {LR_0::LR_SUB, (char *)"s7"}, {LR_0::LR_RPAREN, (char *)"s15"}}},
    {15, {{LR_0::LR_ADD, (char *)"r9"}, {LR_0::LR_SUB, (char *)"r9"}, {LR_0::LR_MULTI, (char *)"r9"}, {LR_0::LR_DIV, (char *)"r9"}, {LR_0::LR_RPAREN, (char *)"r9"}, {LR_0::LR_END, (char *)"r9"}}},
    {16, {{LR_0::LR_END, (char *)"acc"}}},
};

char *search_parsetable(int now_state, int next_type)
{
    for (const auto &entry : ParsingTable)
    {
        int state = entry.first;
        const auto &actions = entry.second;

        if (state == now_state)
        {
            for (const auto &action : actions)
            {
                if ((LR_0::lr_node_type)next_type == action.first)
                {
                    return action.second;
                }
            }
        }
    }

    printf("Error with unknown state change.\n");
    exit(1);
    return (char *)"\0";
}

int transfer_token_lr_type(int token_type)
{
    switch ((TOKEN_STRUCT::token_type)token_type)
    {
    case TOKEN_STRUCT::TOKEN_INTEGER:
        return LR_0::LR_NUMBER;
        break;

    case TOKEN_STRUCT::TOKEN_FLOAT:
        return LR_0::LR_NUMBER;
        break;

    case TOKEN_STRUCT::TOKEN_ID:
        return LR_0::LR_NUMBER;
        break;

    case TOKEN_STRUCT::TOKEN_ADD:
        return LR_0::LR_ADD;
        break;

    case TOKEN_STRUCT::TOKEN_SUB:
        return LR_0::LR_SUB;
        break;

    case TOKEN_STRUCT::TOKEN_MULTIPIE:
        return LR_0::LR_MULTI;
        break;

    case TOKEN_STRUCT::TOKEN_DIV:
        return LR_0::LR_DIV;
        break;

    case TOKEN_STRUCT::TOKEN_LPAREN:
        return LR_0::LR_LPAREN;
        break;

    case TOKEN_STRUCT::TOKEN_RPAREN:
        return LR_0::LR_RPAREN;
        break;

    case TOKEN_STRUCT::TOKEN_COMMA:
        return LR_0::LR_END;
        break;

    case TOKEN_STRUCT::TOKEN_SEMI:
        return LR_0::LR_END;
        break;

    default:
        printf("Unknown token in expression with type %d.\n",token_type);
        exit(0);
        break;
    }

    return 0;
}