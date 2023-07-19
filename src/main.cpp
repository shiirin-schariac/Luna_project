#include <stdio.h>
#include <fstream>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"

#define MAX_LIMIT 100

const char *filename = (char *)"examples/main.luna";

void print_help()
{
    printf("Usage:\ntheia.out <filename>");
    exit(1);
}

void clearFile()
{
    std::ofstream clearFile(filename, std::ios::trunc); // 打开文件以清空内容

    if (!clearFile)
    {
        printf("Something wrong to initialization.\n");
        return;
    }

    clearFile.close(); // 关闭文件

    return;
}

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        for (int i = 1; i < argc; i++)
        {
            int len = strlen(argv[i]);
            char *last_five = &argv[i][len - 5];

            if (strcmp(last_five, ".luna") == 0)
            {
                lexer_T *lexer = init_lexer(
                    get_file_contents(argv[i]));
                parser_T *parser = init_parser(lexer);
                AST_T *root = parser_parse(parser, parser->scope);
                visitor_T *visitor = init_visitor();
                visitor_visit(visitor, root);
            }

            else
            {
                print_help();
            }
        }
    }
    else
    {
        clearFile();
        char input[MAX_LIMIT];

        std::ofstream outfile;
        outfile.open(filename, std::ios::app);

        if (!outfile)
        {
            printf("Something wrong to initialization.\n");
            return 1;
        }

        while (1)
        {
            printf("Welcome to the Luna language v. 1.2.1!\n>>> ");
            fgets(input, MAX_LIMIT, stdin);

            if (!strcmp(input, "exit\n"))
            {
                printf("Thank you for using!\n");
                return 0;
            }

            outfile << input << std::endl;

            lexer_T *lexer = init_lexer(get_file_contents(filename));
            parser_T *parser = init_parser(lexer);
            AST_T *root = parser_parse(parser, parser->scope);
            visitor_T *visitor = init_visitor();
            visitor_visit(visitor, root);
        }
    }

    return 0;
}
