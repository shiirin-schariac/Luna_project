#include "include/scope.h"
#include <string.h>

scope_T *init_scope()
{
    scope_T *scope = new SCOPE_STRUCT[1]();
    scope->function_definition = 0;
    scope->function_definition_size = 0;

    scope->variable_definition = 0;
    scope->variable_definition_size = 0;

    return scope;
}

AST_T *scope_add_function_definition(scope_T *scope, AST_T *fdef)
{
    scope->function_definition_size++;
    
    if (scope->function_definition == 0)
    {
        scope->function_definition = new AST_T *[1]();
    }
    else
    {
        scope->function_definition = (AST_T **)realloc(
            scope->function_definition, scope->function_definition_size * sizeof(AST_T **));
    }

    scope->function_definition[scope->function_definition_size - 1] = fdef;

    return fdef;
}

AST_T* scope_get_function_definition(scope_T* scope, const char* fname)
{
    for (int i = 0; i < scope->function_definition_size; i++)
    {
        AST_T* fdef = scope->function_definition[i];

        if (strcmp(fdef->function_definition_name, fname) == 0)
        {
            return fdef;
        }
    }

    return 0;
}

AST_T* scope_add_variable_definition(scope_T* scope, AST_T* vdef)
{
    if (scope->variable_definition == 0)
    {
        scope->variable_definition = new AST_T*[1]();
        scope->variable_definition[0] = vdef;
        scope->variable_definition_size += 1;
    }
    else
    {
        scope->variable_definition_size += 1;
        scope->variable_definition = (AST_T**)realloc(
            scope->variable_definition,
            scope->variable_definition_size * sizeof(struct AST_T*)  
        );
        scope->variable_definition[scope->variable_definition_size-1] = vdef;
    }

    return vdef;
}

AST_T* scope_get_variable_definition(scope_T* scope, const char* name)
{
    for (int i = 0; i < scope->variable_definition_size; i++)
    {
        AST_T* vdef = scope->variable_definition[i];

        if (strcmp(vdef->variable_definition_variable_name, name) == 0)
        {
            return vdef;
        }
    }

    return 0;
}