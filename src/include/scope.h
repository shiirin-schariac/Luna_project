#ifndef SCOPE_H
#define SCOPE_H

#include "AST.h"

typedef struct SCOPE_STRUCT
{
    AST_T **function_definition;
    size_t function_definition_size;

    AST_T **variable_definition;
    size_t variable_definition_size;
} scope_T;

scope_T *init_scope();

AST_T *scope_add_function_definition(scope_T *scope, AST_T *fdef);

AST_T *scope_get_function_definition(scope_T *scope, const char *fname);

AST_T *scope_add_variable_definition(scope_T *scope, AST_T *vdef);

AST_T *scope_get_variable_definition(scope_T *scope, const char *name);

#endif