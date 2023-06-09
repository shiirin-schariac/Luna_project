#ifndef VISITOR_H
#define VISITOR_H

#include "AST.h"
#include "parser.h"

typedef struct VISITOR_STRUCT
{
    AST_T **variable_definition;
    size_t variable_definition_size;
} visitor_T;

visitor_T *init_visitor();

AST_T *visitor_visit(visitor_T *visitor, AST_T *node);

AST_T *visitor_visit_variable_definition(visitor_T *visitor, AST_T *node);

AST_T *visitor_visit_variable(visitor_T *visitor, AST_T *node);

AST_T *visitor_visit_variable_assignment(visitor_T *visitor, AST_T *node);

AST_T *visitor_visit_function_definition(visitor_T *visitor, AST_T *node);

AST_T *visitor_visit_function_call(visitor_T *visitor, AST_T *node);

AST_T *visitor_visit_expr(visitor_T *visitor, AST_T *node);

void visitor_visit_calculate(visitor_T *visitor, AST_T *node);

bool if_node_number(AST_T *node);

AST_T *visitor_visit_string(visitor_T *visitor, AST_T *node);

AST_T *visitor_visit_integer(visitor_T *visitor, AST_T *node);

AST_T *visitor_visit_float(visitor_T *visitor, AST_T *node);

AST_T *visitor_visit_compound(visitor_T *visitor, AST_T *node);

#endif