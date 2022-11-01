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

    for (size_t i = 0; i < code.num_of_lines; i++)
    {
        get_do_command(code.strings[i], &stk);
    }

    stack_dump(&stk, LOCATION);

    code_dtor(&code);
    stack_dtor(&stk);
}

int get_do_command(char * command, my_stack * stk)
{
    char cmd[10];
    int  n = 0;
    sscanf(command, "%s%n", cmd, &n);

    if(!strcmp("push", cmd))
    {
        int push_value = 0;
        sscanf(command + n, "%d", &push_value);
        stack_push(stk, push_value);
        return 0;
    }

    if(!strcmp("add", cmd))
    {
        int first_value = 0, second_value = 0;
        if (stk->elemAmt < 2)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot add last two elements\nThere are less than 2 elements\n");
            printf("Cannot add: there are less than 2 elements\n");
            return 1;
        }
        stack_pop(stk, &first_value);
        stack_pop(stk, &second_value); 
        stack_push(stk, (first_value + second_value));
    }
    return 0;
}