#include "include/AST.h"

AST_T *init_ast(int type)
{
    AST_T *ast = new AST_T[1]();
    ast->type = (AST_T::ast_type)type;

    ast->scope = 0;

    // AST_VARIABLE_DEFINITION
    ast->variable_definition_variable_name = 0;
    ast->variable_definition_value = 0;

    // AST_FUNCTION_DEFINITION
    ast->function_definition_args_size = 0;
    ast->function_definition_body = 0;
    ast->function_definition_name = 0;
    ast->function_definition_args = 0;

    // AST_VARIABLE
    ast->variable_name = 0;

    // AST_FUNC_CALL
    ast->function_call_name = 0;
    ast->function_call_arguments = 0;
    ast->function_call_arguments_size = 0;

    // AST_STRING
    ast->string_value = 0;

    // AST_INTEGER
    ast->int_value = 0;

    // AST_FLOAT
    ast->float_value = 0;

    // AST_COMPOUND
    ast->compound_value = 0;
    ast->compound_size = 0;

    return ast;
}
