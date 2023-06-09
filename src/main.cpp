#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"

void print_help()
{
    printf("Usage:\nhello.out <filename>");
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_help();
    }
    lexer_T *lexer = init_lexer(get_file_contents(argv[1]));

    // token_T *token = lexer_get_next_token(lexer);
    // printf("TOKEN(%d,%s)\n", token->type, token->value);

    // while ((token = lexer_get_next_token(lexer))->type != TOKEN_STRUCT::TOKEN_EOF)
    // {
    //     printf("TOKEN(%d,%s)\n", token->type, token->value);
    // }

    parser_T *parser = init_parser(lexer);
    AST_T *root = parser_parse(parser, parser->scope);
    visitor_T *visitor = init_visitor();
    //printf("the compound is %d\n",root->compound_size);
    visitor_visit(visitor, root);

    return 0;
}