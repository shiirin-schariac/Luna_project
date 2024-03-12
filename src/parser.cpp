#include "include/parser.h"
#include "include/scope.h"
#include "include/LR_0.h"
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
    else if (parser->current_token->type == TOKEN_STRUCT::TOKEN_EOF)
    {
        return;
    }
    else
    {
        printf("Unexpected token %s , with type %d", parser->current_token->value, parser->current_token->type);
        exit(1);
    }

    return;
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
        
        if(parser->current_token->type != TOKEN_STRUCT::TOKEN_SEMI&&parser->current_token->type == TOKEN_STRUCT::TOKEN_ID)
        {
            printf("Missing semi!\n");
            exit(1);
        }
    }

    return compound;
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
    else if (strcmp(parser->current_token->value, "if") == 0)
    {
        return parser_parse_if_compound(parser, scope);
    }
    else
    {
        return parser_parse_variable(parser, scope);
    }
}

AST_T *parser_update_next_node(parser_T *parser, scope_T *scope)
{
    AST_T *next_node = 0;
    // printf("now the token type is %d\n",parser->current_token->type);

    switch (parser->current_token->type)
    {
    case TOKEN_STRUCT::TOKEN_INTEGER:
        next_node = parser_parse_int(parser, scope);
        next_node->expression_node_type = transfer_token_lr_type(TOKEN_STRUCT::TOKEN_INTEGER);
        break;

    case TOKEN_STRUCT::TOKEN_FLOAT:
        next_node = parser_parse_float(parser, scope);
        next_node->expression_node_type = transfer_token_lr_type(TOKEN_STRUCT::TOKEN_FLOAT);
        break;

    case TOKEN_STRUCT::TOKEN_ID:
        next_node = parser_parse_variable(parser, scope);
        next_node->expression_node_type = transfer_token_lr_type(TOKEN_STRUCT::TOKEN_ID);
        break;

    case TOKEN_STRUCT::TOKEN_SEMI:
        next_node = init_ast(AST_T::AST_NOOP);
        next_node->expression_node_type = transfer_token_lr_type(TOKEN_STRUCT::TOKEN_SEMI);
        break;

    case TOKEN_STRUCT::TOKEN_COMMA:
        next_node = init_ast(AST_T::AST_NOOP);
        next_node->expression_node_type = transfer_token_lr_type(TOKEN_STRUCT::TOKEN_COMMA);
        break;

    default:
        next_node = init_ast(AST_T::AST_NOOP);
        next_node->expression_node_type = transfer_token_lr_type((TOKEN_STRUCT::token_type)parser->current_token->type);
        parser_eat(parser, (TOKEN_STRUCT::token_type)parser->current_token->type);
        break;
    }

    // update the lr type of the next node
    // parser_eat(parser, parser->current_token->type);

    return next_node;
}

AST_T *parser_parse_express(parser_T *parser, scope_T *scope)
{
    if (parser->current_token->type == TOKEN_STRUCT::TOKEN_STRING)
    {
        return parser_parse_string(parser, scope);
    }

    std::vector<AST_T *> nodes;
    nodes.push_back(init_ast(AST_T::AST_NOOP));
    nodes[0]->expression_node_type = LR_0::LR_START;
    nodes[0]->expression_state = 0;

    int now_type = LR_0::LR_START;
    int now_state = 0;

    int l_parens = parser->current_token->type == TOKEN_STRUCT::TOKEN_LPAREN ? 1 : 0;

    AST_T *next_token_node = parser_update_next_node(parser, scope);
    AST_T *next_node = next_token_node;

    while (now_type != LR_0::LR_END)
    // the condition loop ends until the type of expression's last node s END
    {
        // get the operation from the LR(0) parsing table
        char *operation = search_parsetable(now_state, next_node->expression_node_type);

        // LR0 rule: s means to change the state of the expression and push the node into the stack
        // r means to do some simplification of the expression
        if (operation[0] == 'r')
        {
            switch (operation[1] - '0')
            {
            case 1:
            {
                AST_T *top_node = init_ast(AST_T::AST_EXPR);
                top_node->expression_node_type = LR_0::LR_END;
                top_node->expression_left = nodes.back();

                return top_node;
                break;
            }

            case 2:
            {
                AST_T *ast_oprt = init_ast(AST_T::AST_ADD);
                ast_oprt->expression_node_type = LR_0::LR_EXPR;
                ast_oprt->expression_right = nodes.back();
                nodes.pop_back();
                nodes.pop_back();
                ast_oprt->expression_left = nodes.back();
                nodes.pop_back();
                next_node = ast_oprt;
                now_type = nodes.back()->expression_node_type;
                now_state = nodes.back()->expression_state;
                break;
            }

            case 3:
            {
                AST_T *ast_oprt = init_ast(AST_T::AST_SUB);
                ast_oprt->expression_node_type = LR_0::LR_EXPR;
                ast_oprt->expression_right = nodes.back();
                nodes.pop_back();
                nodes.pop_back();
                ast_oprt->expression_left = nodes.back();
                nodes.pop_back();
                next_node = ast_oprt;
                now_type = nodes.back()->expression_node_type;
                now_state = nodes.back()->expression_state;
                break;
            }

            case 4:
            {
                next_node = nodes.back();
                next_node->expression_node_type = LR_0::LR_EXPR;
                nodes.pop_back();
                now_type = nodes.back()->expression_node_type;
                now_state = nodes.back()->expression_state;
                break;
            }

            case 5:
            {
                AST_T *ast_oprt = init_ast(AST_T::AST_MUL);
                ast_oprt->expression_node_type = LR_0::LR_TERM;
                ast_oprt->expression_right = nodes.back();
                nodes.pop_back();
                nodes.pop_back();
                ast_oprt->expression_left = nodes.back();
                nodes.pop_back();
                next_node = ast_oprt;
                now_type = nodes.back()->expression_node_type;
                now_state = nodes.back()->expression_state;
                break;
            }

            case 6:
            {
                AST_T *ast_oprt = init_ast(AST_T::AST_DIV);
                ast_oprt->expression_node_type = LR_0::LR_TERM;
                ast_oprt->expression_right = nodes.back();
                nodes.pop_back();
                nodes.pop_back();
                ast_oprt->expression_left = nodes.back();
                nodes.pop_back();
                next_node = ast_oprt;
                now_type = nodes.back()->expression_node_type;
                now_state = nodes.back()->expression_state;
                break;
            }

            case 7:
            {
                next_node = nodes.back();
                next_node->expression_node_type = LR_0::LR_TERM;
                nodes.pop_back();
                now_type = nodes.back()->expression_node_type;
                now_state = nodes.back()->expression_state;
                break;
            }

            case 8:
            {
                next_node = nodes.back();
                next_node->expression_node_type = LR_0::LR_FACTOR;
                nodes.pop_back();
                now_type = nodes.back()->expression_node_type;
                now_state = nodes.back()->expression_state;
                break;
            }

            case 9:
            {
                nodes.pop_back();
                next_node = nodes.back();
                next_node->expression_node_type = LR_0::LR_FACTOR;
                nodes.pop_back();
                nodes.pop_back();
                now_type = nodes.back()->expression_node_type;
                now_state = nodes.back()->expression_state;
                l_parens--;
                break;
            }

            default:
                break;
            }

            // use the rule of switch return to simplify the node in stack using
        }
        else if (operation[0] == 's')
        {
            next_node->expression_state = atoi(operation + 1);
            // printf("now the state is %d\n", next_node->expression_state);

            if (next_node->expression_node_type != LR_0::LR_EXPR && next_node->expression_node_type != LR_0::LR_TERM && next_node->expression_node_type != LR_0::LR_FACTOR)
            {
                if (l_parens == 0 && parser->current_token->type == TOKEN_STRUCT::TOKEN_RPAREN)
                // if there is no lparen to be matched and we detect a rparen
                // it means the expression is over
                {
                    next_token_node = init_ast(AST_T::AST_NOOP);
                    next_token_node->expression_node_type = LR_0::LR_END;
                }
                else
                // we have to analyze the next node
                    next_token_node = parser_update_next_node(parser, scope);
            }

            nodes.push_back(next_node);
            now_state = nodes.back()->expression_state;
            now_type = nodes.back()->expression_node_type;

            next_node = next_token_node;
            // nodes.push
            // update the now_type and the next_type
        }
        else if (strcmp(operation, "acc") == 0)
        {
            break;
        }
        else
        {
            printf("Error with ???\n");
            exit(1);
        }
    }

    printf("the EXPRESSION is uncompleted\n");
    exit(1);
    return next_node;
}

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

    scope_T *new_scope = init_scope();

    function_def->function_definition_body = parser_parse_statements(parser, new_scope);

    parser_eat(parser, TOKEN_STRUCT::TOKEN_RBRACE);

    scope_add_function_definition(scope, function_def);

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

    if (parser->current_token->type == TOKEN_STRUCT::TOKEN_EQUALS)
    {
        return parser_parse_variable_assignment(parser, scope);
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

    AST_T *variable_definition = init_ast(AST_T::AST_VARIABLE_DEFINITION);
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_value = variable_definition_value;

    scope_add_variable_definition(scope, variable_definition);

    variable_definition->scope = scope;

    return variable_definition;
}

AST_T *parser_parse_variable_assignment(parser_T *parser, scope_T *scope)
{
    char *variable_assignment_variable_name = parser->prev_token->value;

    parser_eat(parser, TOKEN_STRUCT::TOKEN_EQUALS);
    AST_T *variable_assignment_value = parser_parse_express(parser, scope);

    AST_T *variable_assignment = init_ast(AST_T::AST_VARIABLE_ASSIGNMENT);
    variable_assignment->variable_assignment_variable_name = variable_assignment_variable_name;
    variable_assignment->variable_assignment_value = variable_assignment_value;

    variable_assignment->scope = scope;

    return variable_assignment;
}

// TODO: If compound analyze
// AST_T *parser_parse_if_compound(parser_T *parser, scope_T *scope) {
//     parser_eat(parser, TOKEN_STRUCT::TOKEN_ID);
//     parser_eat(parser, TOKEN_STRUCT::TOKEN_LPAREN);
    
//     AST_T *if_left_value = parser_parse_express(parser, scope);
//     if (parser->current_token->type == TOKEN_STRUCT::TOKEN_EQUALS 
//         || parser->current_token->type == TOKEN_STRUCT::TOKEN_LARGEEQUAL 
//         || parser->current_token->type == TOKEN_STRUCT::TOKEN_LARGER 
//         || parser->current_token->type == TOKEN_STRUCT::TOKEN_LESSEQUAL 
//         || parser->current_token->type == TOKEN_STRUCT::TOKEN_LESS
//         )
//     {
//         parser_eat(parser, parser->current_token->type);
//         AST_T *if_right_value = parser_parse_express(parser, scope);
//     }
// }

AST_T *parser_parse_string(parser_T *parser, scope_T *scope)
{
    AST_T *ast_string = init_ast(AST_T::AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRUCT::TOKEN_STRING);

    ast_string->scope = scope;

    return ast_string;
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

    parser_eat(parser, TOKEN_STRUCT::TOKEN_FLOAT);

    ast_float->scope = scope;

    return ast_float;
}