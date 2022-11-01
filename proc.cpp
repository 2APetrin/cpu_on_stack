#include "header.h"

void run_cpu(FILE * stream)
{
    assert(stream != NULL);
    
    struct my_stack stk = {};
    stack_ctor(&stk, 20);

    struct my_code code = {};
    code_ctor(&code, stream);

    /*printf("%s\n",  code.strings[0]);
    printf("%s\n",  code.strings[1]);
    printf("%s\n",  code.strings[2]);
    printf("%lu\n", code.num_of_lines);
    printf("%lu\n", code.num_of_symbols);*/
    
    int command_num = 0;
    int num_of_letters_in_command = 0;

    for (size_t i = 0; i < code.num_of_lines; i++)
    {
        command_num = get_command(code.strings[i], &num_of_letters_in_command);
        do_command(command_num, num_of_letters_in_command);
    }

    code_dtor(&code);
    stack_dtor(&stk);
}

int get_command(char * command, int * n)
{
    char cmd[10];
    sscanf(command, "%s%n", cmd, n);

    //printf("%s", cmd);

    if(!strcmp("push", cmd))
    {
        return PUSH;
    }

    if (!strcmp("add", cmd))
    {
        return ADD;
    }

    return SYNTAXERROR;
}

void do_command(int command_num, int n)
{
    if(command_num == PUSH)
    {
        
    }
}