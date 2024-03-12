#include "include/visitor.h"
#include "include/scope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

static AST_T *builtin_function_print(visitor_T *visitor, AST_T **args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T *visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
        case AST_T::AST_STRING:
            printf("%s\n", visited_ast->string_value);
            break;

        case AST_T::AST_INTEGER:
            printf("%d\n", visited_ast->int_value);
            break;

        case AST_T::AST_FLOAT:
            printf("%f\n", visited_ast->float_value);
            break;

        default:
            printf("%p\n", visited_ast);
            break;
        }
    }

    return init_ast(AST_T::AST_NOOP);
}

static AST_T *builtin_function_exit(visitor_T *visitor, AST_T **args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T *visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
        case AST_T::AST_NOOP:
        {
            printf("You exited.\n");
            exit(0);
            break;
        }

        default:
            printf("%p\n", visited_ast);
            break;
        }
    }

    return init_ast(AST_T::AST_NOOP);
}

static AST_T *builtin_function_clear(visitor_T *visitor, AST_T **args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T *visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
        case AST_T::AST_NOOP:
        {
            system("clear");
            break;
        }

        default:
            printf("%p\n", visited_ast);
            break;
        }
    }

    return init_ast(AST_T::AST_NOOP);
}

visitor_T *init_visitor()
{
    visitor_T *visitor = new VISITOR_STRUCT[1]();
    visitor->variable_definition = 0;
    visitor->variable_definition_size = 0;

    return visitor;
}

AST_T *visitor_visit(visitor_T *visitor, AST_T *node)
{
    switch (node->type)
    {
    case AST_T::AST_VARIABLE_DEFINITION:
    {
        return visitor_visit_variable_definition(visitor, node);
        break;
    }

    case AST_T::AST_VARIABLE:
    {
        return visitor_visit_variable(visitor, node);
        break;
    }

    case AST_T::AST_VARIABLE_ASSIGNMENT:
    {
        return visitor_visit_variable_assignment(visitor, node);
        break;
    }

    case AST_T::AST_FUNCTION_DEFINITION:
    {
        return visitor_visit_function_definition(visitor, node);
        break;
    }

    case AST_T::AST_FUNCTION_CALL:
    {
        return visitor_visit_function_call(visitor, node);
        break;
    }

    case AST_T::AST_EXPR:
    {
        return visitor_visit_expr(visitor, node);
        break;
    }

    case AST_T::AST_STRING:
    {
        return visitor_visit_string(visitor, node);
        break;
    }

    case AST_T::AST_INTEGER:
    {
        return visitor_visit_integer(visitor, node);
        break;
    }

    case AST_T::AST_FLOAT:
    {
        return visitor_visit_float(visitor, node);
        break;
    }

    case AST_T::AST_COMPOUND:
    {
        return visitor_visit_compound(visitor, node);
        break;
    }

    case AST_T::AST_NOOP:
    {
        return node;
        break;
    }

    default:
    {
        printf("Uncaught Statement of type %d.\n", node->type);
        exit(1);
        break;
    }
    }

    return init_ast(AST_T::AST_NOOP);
}

AST_T *visitor_visit_variable_definition(visitor_T *visitor, AST_T *node)
{
    // scope_add_variable_definition(
    //     node->scope,
    //     node);

    return visitor_visit_expr(visitor, node->variable_value);
}

AST_T *visitor_visit_variable(visitor_T *visitor, AST_T *node)
{
    AST_T *vdef = scope_get_variable_definition(
        node->scope,
        node->variable_name);

    if (vdef != 0)
        return visitor_visit(visitor, vdef->variable_value);

    printf("Used an undefined variable %s\n", node->variable_name);
    exit(1);
    return node;
}

AST_T *visitor_visit_variable_assignment(visitor_T *visitor, AST_T *node)
{
    AST_T *vdef = scope_get_variable_definition(
        node->scope,
        node->variable_assignment_variable_name);

    if (vdef != 0)
    {
        vdef->variable_value = visitor_visit(visitor, node->variable_assignment_value);
        return node;
    }

    printf("Assigned value to an undefined variable %s\n", node->variable_name);
    exit(1);
    return node;
}

AST_T *visitor_visit_function_definition(visitor_T *visitor, AST_T *node)
{
    // scope_add_function_definition(
    //     node->scope,
    //     node);

    return node;
}

AST_T *visitor_visit_function_call(visitor_T *visitor, AST_T *node)
{
    if (strcmp(node->function_call_name, "print") == 0)
    {
        return builtin_function_print(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }
    else if (strcmp(node->function_call_name, "exit") == 0)
    {
        return builtin_function_exit(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }
    else if (strcmp(node->function_call_name, "clear") == 0)
    {
        return builtin_function_clear(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }

    AST_T *fdef = scope_get_function_definition(
        node->scope,
        node->function_call_name);

    if (fdef == 0)
    {
        printf("Undefined method `%s`\n", node->function_call_name);
        exit(1);
    }

    // printf("Now this function has %d args.\n", node->function_call_arguments_size);

    for (int i = 0; i < (int)node->function_call_arguments_size; i++)
    {
        // grab the variable from the function definition arguments
        AST_T *ast_var = (AST_T *)fdef->function_definition_args[i];

        // grab the value from the function call arguments
        AST_T *ast_value = (AST_T *)node->function_call_arguments[i];

        // create a new variable definition with the value of the argument
        // in the function call.
        AST_T *ast_vardef = init_ast(AST_T::AST_VARIABLE_DEFINITION);
        ast_vardef->variable_value = ast_value;

        // copy the name from the function definition argument into the new
        // variable definition
        ast_vardef->variable_definition_variable_name = new char[strlen(ast_var->variable_name) + 1]();
        strcpy(ast_vardef->variable_definition_variable_name, ast_var->variable_name);

        // push our variable definition into the function body scope.
        scope_add_variable_definition(fdef->function_definition_body->scope, ast_vardef);
    }

    return visitor_visit(visitor, fdef->function_definition_body);
}

AST_T *visitor_visit_expr(visitor_T *visitor, AST_T *node)
{
    visitor_visit_calculate(visitor, node->expression_left);
    node->type = node->expression_left->type;

    if (node->expression_left->type == AST_T::AST_INTEGER)
    {
        node->int_value = node->expression_left->int_value;
    }
    else if (node->expression_left->type == AST_T::AST_FLOAT)
    {
        node->float_value = node->expression_left->float_value;
    }
    else if (node->expression_left->type == AST_T::AST_STRING)
    {
        node->string_value = node->expression_left->string_value;
    }
    else
    {
        printf("the type of the wrong is %d\n", node->expression_left->type);
    }

    return node;
}

void visitor_visit_calculate(visitor_T *visitor, AST_T *node)
{
    if (if_node_number(node))
    {
        return;
    }
    if (node->type == AST_T::AST_VARIABLE)
    {
        AST_T *var_node = visitor_visit_variable(visitor, node);
        node->type = var_node->type;
        if (node->type == AST_T::AST_INTEGER)
        {
            node->int_value = var_node->int_value;
        }
        else if (node->type == AST_T::AST_FLOAT)
        {
            node->float_value = var_node->float_value;
        }
        else if (node->type == AST_T::AST_STRING)
        {
            node->string_value = var_node->string_value;
        }
        else
        {
            printf("warning with UNSUPPORTED or UNDEFINED variable.\n");
        }
        return;
    }
    if (!(if_node_number(node->expression_left) && if_node_number(node->expression_right)))
    {
        visitor_visit_calculate(visitor, node->expression_left);
        visitor_visit_calculate(visitor, node->expression_right);
    }
    if (node->expression_left->type == AST_T::AST_INTEGER && node->expression_right->type == AST_T::AST_INTEGER)
    {
        switch (node->type)
        {
        case AST_T::AST_ADD:
            node->int_value = node->expression_left->int_value + node->expression_right->int_value;
            break;

        case AST_T::AST_SUB:
            node->int_value = node->expression_left->int_value - node->expression_right->int_value;
            break;

        case AST_T::AST_MUL:
            node->int_value = node->expression_left->int_value * node->expression_right->int_value;
            break;

        case AST_T::AST_DIV:
            node->int_value = node->expression_left->int_value / node->expression_right->int_value;
            break;

        default:
            printf("The operation type is UNSUPPORTED.\n");
            exit(1);
            break;
        }
        node->type = AST_T::AST_INTEGER;
        return;
    }
    else
    {
        switch (node->type)
        {
        case AST_T::AST_ADD:
        {
            double left = node->expression_left->int_value ? node->expression_left->int_value : node->expression_left->float_value;
            double right = node->expression_right->int_value ? node->expression_right->int_value : node->expression_right->float_value;
            node->float_value = left + right;
            break;
        }

        case AST_T::AST_SUB:
        {
            double left = node->expression_left->int_value ? node->expression_left->int_value : node->expression_left->float_value;
            double right = node->expression_right->int_value ? node->expression_right->int_value : node->expression_right->float_value;
            node->float_value = left - right;
            break;
        }

        case AST_T::AST_MUL:
        {
            double left = node->expression_left->int_value ? node->expression_left->int_value : node->expression_left->float_value;
            double right = node->expression_right->int_value ? node->expression_right->int_value : node->expression_right->float_value;
            node->float_value = left * right;
            break;
        }

        case AST_T::AST_DIV:
        {
            double left = node->expression_left->int_value ? node->expression_left->int_value : node->expression_left->float_value;
            double right = node->expression_right->int_value ? node->expression_right->int_value : node->expression_right->float_value;
            node->float_value = left / right;
            break;
        }

        default:
            printf("The operation type is UNSUPPORTED.\n");
            exit(1);
            break;
        }
        node->type = AST_T::AST_FLOAT;
        return;
    }
}

bool if_node_number(AST_T *node)
{
    return (node->type == AST_T::AST_INTEGER || node->type == AST_T::AST_FLOAT);
}

AST_T *visitor_visit_string(visitor_T *visitor, AST_T *node)
{
    return node;
}

AST_T *visitor_visit_integer(visitor_T *visitor, AST_T *node)
{
    return node;
}

AST_T *visitor_visit_float(visitor_T *visitor, AST_T *node)
{
    return node;
}

AST_T *visitor_visit_compound(visitor_T *visitor, AST_T *node)
{
    for (int i = 0; i < node->compound_size; i++)
    {
        // printf("%d\n", node->compound_value[i]->type);
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_T::AST_NOOP);
}