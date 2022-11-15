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
    printf("%s\n",  code.strings[4]);
    printf("%lu\n", code.num_of_lines);
    printf("%lu\n", code.num_of_symbols);*/

    for (size_t i = 0; i < code.num_of_lines; i++)
    {
        if( get_do_command(code.strings[i], &stk) )
            break;
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

    if(!strcmp("hlt", cmd))
    {
        return 1;
    }

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
        stack_push(stk, (second_value + first_value));
        return 0;
    }

    if(!strcmp("sub", cmd))
    {
        int first_value = 0, second_value = 0;
        if (stk->elemAmt < 2)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot sub last two elements\nThere are less than 2 elements\n");
            printf("Cannot sub: there are less than 2 elements\n");
            return 1;
        }
        stack_pop(stk, &first_value);
        stack_pop(stk, &second_value); 
        stack_push(stk, (second_value - first_value));
        return 0;
    }

    if(!strcmp("div", cmd))
    {
        int first_value = 0, second_value = 0;
        if (stk->elemAmt < 2)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot div last two elements\nThere are less than 2 elements\n");
            printf("Cannot div: there are less than 2 elements\n");
            return 1;
        }
        stack_pop(stk, &first_value);
        stack_pop(stk, &second_value); 
        stack_push(stk, (second_value / first_value));
        return 0;
    }

    if(!strcmp("mul", cmd)) 
    {
        int first_value = 0, second_value = 0;
        if (stk->elemAmt < 2)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot mul last two elements\nThere are less than 2 elements\n");
            printf("Cannot mul: there are less than 2 elements\n");
            return 1;
        }
        stack_pop(stk, &first_value);
        stack_pop(stk, &second_value); 
        stack_push(stk, (second_value * first_value));
        return 0;
    }

    if (!strcmp("out", cmd))
    {
        if (stk->elemAmt == 0)
        {
            printf("Error: there is no values to print\n");
            return 0;
        }
        
        if (stk->elemAmt == 1)
        {
            printf("Answer = %d\n", stk->data[0]);
        }

        if (stk->elemAmt > 1)
        {
            printf("Error: not enough operations, there are more than one last value\n");
            for (size_t i = 0; i < stk->elemAmt - 1; i++)
            {
                printf("[%lu] - %d\n", i, stk->data[i]);
            }
        }

        return 0;
    }

    printf("   ERROR!!!\n");
    printf("Invalid syntax\n");

    fprintf(logfile, "\n   ERROR!!!\n");
    fprintf(logfile, "Invalid syntax\n");
    return 1;
}