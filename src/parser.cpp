#include "include/parser.h"
#include "include/scope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sstream>

parser_T *init_parser(lexer_T *lexer)
{
    parser_T *parser = new PARSER_STRUCT[1]();
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;

    parser->scope = init_scope();

    return parser;
}

void parser_eat(parser_T *parser, int token_type)
{
    if (parser->current_token->type == (TOKEN_STRUCT::token_type)token_type)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        printf("Unexpected token %s , with type %d", parser->current_token->value, parser->current_token->type);
        exit(1);
    }
}

AST_T *parser_parse(parser_T *parser, scope_T *scope)
{
    return parser_parse_statements(parser, scope);
}

AST_T *parser_parse_statement(parser_T *parser, scope_T *scope)
{
    switch (parser->current_token->type)
    {
    case TOKEN_STRUCT::TOKEN_ID:
    {
        // printf("%s\n", parser->current_token->value);
        return parser_parse_id(parser, scope);
    }
    case TOKEN_STRUCT::TOKEN_EOF:
    {
        return 0;
    }

    default:
    {
        return init_ast(AST_T::AST_NOOP);
        break;
    }
    }
    return init_ast(AST_T::AST_NOOP);
}

AST_T *parser_parse_statements(parser_T *parser, scope_T *scope)
{
    AST_T *compound = init_ast(AST_T::AST_COMPOUND);
    // compound->compound_value = calloc(1, sizeof(struct AST_T *));
    compound->scope = scope;
    compound->compound_value = new AST_T *[1]();

    AST_T *ast_statement = parser_parse_statement(parser, scope);
    ast_statement->scope = scope;
    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    while (parser->current_token->type == TOKEN_STRUCT::TOKEN_SEMI)
    {
        // printf("%s\n",parser->current_token->value);
        parser_eat(parser, TOKEN_STRUCT::TOKEN_SEMI);

        AST_T *ast_statement = parser_parse_statement(parser, scope);
        // add the next sentence as a new node to the AST

        if (ast_statement)
        {
            compound->compound_size += 1;
            // put the new sentence into list
            compound->compound_value = (AST_T **)realloc(compound->compound_value, compound->compound_size * sizeof(struct AST_T *));
            // realloc memory

            compound->compound_value[compound->compound_size - 1] = ast_statement;
        }
    }

    return compound;
}

AST_T *parser_parse_express(parser_T *parser, scope_T *scope)
{
    switch (parser->current_token->type)
    {
    case TOKEN_STRUCT::TOKEN_STRING:
        return parser_parse_string(parser, scope);
    case TOKEN_STRUCT::TOKEN_ID:
        return parser_parse_id(parser, scope);
    case TOKEN_STRUCT::TOKEN_INTEGER:
        return parser_parse_int(parser, scope);
    case TOKEN_STRUCT::TOKEN_FLOAT:
        return parser_parse_float(parser, scope);

    default:
    {
        return init_ast(AST_T::AST_NOOP);
        break;
    }
    }
    return init_ast(AST_T::AST_NOOP);
}

// AST_T *parser_parse_factor(parser_T *parser, scope_T *scope)
// {
// }

// AST_T *parser_parse_term(parser_T *parser, scope_T *scope)
// {
// }

AST_T *parser_parse_function_call(parser_T *parser, scope_T *scope)
{
    AST_T *function_call = init_ast(AST_T::AST_FUNCTION_CALL);

    function_call->function_call_name = parser->prev_token->value;
    parser_eat(parser, TOKEN_STRUCT::TOKEN_LPAREN);

    if (parser->current_token->type != TOKEN_STRUCT::TOKEN_RPAREN)
    {
        function_call->function_call_arguments = new AST_T *[1]();

        AST_T *ast_expr = parser_parse_express(parser, scope);
        function_call->function_call_arguments[0] = ast_expr;
        function_call->function_call_arguments_size += 1;

        while (parser->current_token->type == TOKEN_STRUCT::TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_STRUCT::TOKEN_COMMA);

            AST_T *ast_expr = parser_parse_express(parser, scope);
            // add the next sentence as a new node to the AST

            function_call->function_call_arguments_size += 1;
            // put the new sentence into list
            function_call->function_call_arguments = (AST_T **)realloc(function_call->function_call_arguments, function_call->function_call_arguments_size * sizeof(struct AST_T *));
            // realloc memory

            function_call->function_call_arguments[function_call->function_call_arguments_size - 1] = ast_expr;
        }
    }

    parser_eat(parser, TOKEN_STRUCT::TOKEN_RPAREN);

    function_call->scope = scope;

    return function_call;
}

AST_T *parser_parse_function_definition(parser_T *parser, scope_T *scope)
{
    AST_T *function_def = init_ast(AST_T::AST_FUNCTION_DEFINITION);
    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID);

    char *function_name = parser->current_token->value;
    function_def->function_definition_name = new char[strlen(function_name) + 1]();
    function_def->function_definition_name = function_name;

    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID);
    parser_eat(parser, TOKEN_STRUCT::TOKEN_LPAREN);

    if (parser->current_token->type == TOKEN_STRUCT::TOKEN_RPAREN)
    {
        function_def->function_call_arguments_size = 0;
        function_def->function_definition_args_size = 0;
    }
    else
    {
        function_def->function_definition_args = new AST_T *[1]();

        AST_T *arg = parser_parse_variable(parser, scope);
        function_def->function_definition_args_size += 1;
        function_def->function_definition_args[function_def->function_definition_args_size - 1] = arg;

        while (parser->current_token->type == TOKEN_STRUCT::TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_STRUCT::TOKEN_COMMA);

            function_def->function_definition_args_size += 1;

            function_def->function_definition_args =
                (AST_T **)realloc(
                    function_def->function_definition_args,
                    function_def->function_definition_args_size * sizeof(struct AST_STRUCT *));

            AST_T *arg = parser_parse_variable(parser, scope);
            function_def->function_definition_args[function_def->function_definition_args_size - 1] = arg;
        }
    }

    parser_eat(parser, TOKEN_STRUCT::TOKEN_RPAREN);

    parser_eat(parser, TOKEN_STRUCT::TOKEN_LBRACE);

    function_def->function_definition_body = parser_parse_statements(parser, scope);

    parser_eat(parser, TOKEN_STRUCT::TOKEN_RBRACE);

    function_def->scope = scope;

    return function_def;
}

AST_T *parser_parse_variable(parser_T *parser, scope_T *scope)
{
    char *token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID);

    if (parser->current_token->type == TOKEN_STRUCT::TOKEN_LPAREN)
    {
        return parser_parse_function_call(parser, scope);
    }

    AST_T *ast_variable = init_ast(AST_T::AST_VARIABLE);
    ast_variable->variable_name = token_value;

    ast_variable->scope = scope;

    return ast_variable;
}

AST_T *parser_parse_variable_definition(parser_T *parser, scope_T *scope)
{
    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID); // var
    char *variable_definition_variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID);     // var name
    parser_eat(parser, TOKEN_STRUCT::TOKEN_EQUALS); // var name =
    AST_T *variable_definition_value = parser_parse_express(parser, scope);
    //

    AST_T *variable_definition = init_ast(AST_T::AST_VARIABLE_DEFINITION);
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_definition_value = variable_definition_value;

    variable_definition->scope = scope;

    return variable_definition;
}

AST_T *parser_parse_string(parser_T *parser, scope_T *scope)
{
    AST_T *ast_string = init_ast(AST_T::AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRUCT::TOKEN_STRING);

    ast_string->scope = scope;

    return ast_string;
}

AST_T *parser_parse_id(parser_T *parser, scope_T *scope)
{
    if (strcmp(parser->current_token->value, "var") == 0)
    {

        return parser_parse_variable_definition(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "function") == 0)
    {
        return parser_parse_function_definition(parser, scope);
    }
    else
    {
        return parser_parse_variable(parser, scope);
    }
}

AST_T *parser_parse_int(parser_T *parser, scope_T *scope)
{
    AST_T *ast_integer = init_ast(AST_T::AST_INTEGER);
    std::istringstream(parser->current_token->value) >> ast_integer->int_value;

    parser_eat(parser, TOKEN_STRUCT::TOKEN_INTEGER);

    ast_integer->scope = scope;

    return ast_integer;
}

AST_T *parser_parse_float(parser_T *parser, scope_T *scope)
{
    AST_T *ast_float = init_ast(AST_T::AST_FLOAT);
    std::istringstream(parser->current_token->value) >> ast_float->float_value;
    printf("%f\n", ast_float->float_value);

    parser_eat(parser, TOKEN_STRUCT::TOKEN_FLOAT);

    ast_float->scope = scope;

    return ast_float;
}