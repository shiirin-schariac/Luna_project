#ifndef AST_H
#define AST_H

#include <ctype.h>
#include <stdlib.h>

typedef struct AST_T
{
    enum ast_type
    {
        AST_VARIABLE_DEFINITION,
        AST_VARIABLE_ASSIGNMENT,
        AST_FUNCTION_DEFINITION,
        AST_VARIABLE,
        AST_EXPR,
        AST_ADD,
        AST_SUB,
        AST_DIV,
        AST_MUL,
        AST_FUNCTION_CALL,
        AST_STRING,
        AST_INTEGER,
        AST_FLOAT,
        AST_COMPOUND,
        AST_NOOP,
    } type;

    struct SCOPE_STRUCT *scope;

    // AST_VARIABLE_DEFINITION
    char *variable_definition_variable_name;
    struct AST_T *variable_value;

    // AST_VARIABLE_ASSIGNMENT
    char *variable_assignment_variable_name;
    struct AST_T *variable_assignment_value;

    // AST_FUNCTION_DEFINITION
    size_t function_definition_args_size;
    struct AST_T *function_definition_body;
    char *function_definition_name;
    struct AST_T **function_definition_args;

    // AST_VARIABLE
    char *variable_name;

    // AST_FUNC_CALL
    char *function_call_name;
    struct AST_T **function_call_arguments; 
    size_t function_call_arguments_size;

    // AST_EXPR
    struct AST_T *expression_left;
    struct AST_T *expression_right;
    int expression_state;
    int expression_node_type;

    // AST_STRING
    char *string_value;

    // AST_INTEGER
    int int_value;

    // AST_FLOAT
    float float_value;

    // AST_COMPOUND
    struct AST_T **compound_value;
    size_t compound_size;

} AST_T;

AST_T *init_ast(int type);

#endif