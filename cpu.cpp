#include "header_cpu.h"

FILE * open_inputfile(const char * filename)
{
    FILE * return_ptr = fopen(filename, "r");

    if (return_ptr == NULL)
    {
        printf("Cannot open inputfile. Programm shutdown\n");
        abort();  
    }
    return return_ptr;
}

int run_cpu(FILE * stream)
{
    my_stack stk = {};
    stack_ctor(&stk, MIN_CAPACITY);

    int    cmd    = 0;
    size_t line_n = 1;
    fscanf(stream, "%d", &cmd);

    while (cmd != 0)
    {
        if (do_cmd(cmd, stream, &stk, line_n))
        {
            printf("Compilation error in line: %lu\n", line_n);
            break;
        }

        fscanf(stream, "%d", &cmd);  
        line_n++;
    }

    stack_dtor(&stk);
    return 0;
}

int do_cmd(int cmd, FILE * stream, my_stack * stk, size_t line_n)
{
    if (cmd == PUSH)
    {
        int val = 0;
        fscanf(stream, "%d", &val);
        stack_push(stk, val);
        return 0;
    }

    if (cmd == ADD)
    {
        int val1 = 0, val2 = 0;
        
        if (stk->elemAmt < 2)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot add last two elements\nThere are less than 2 elements\nError line - %lu\n", line_n);
            printf("Error in line %lu: there are less than 2 elements\n", line_n);
            return 1;
        }

        stack_pop (stk, &val1);
        stack_pop (stk, &val2); 
        stack_push(stk, (val2 + val1));

        return 0;
    }

    if (cmd == SUB)
    {
        int val1 = 0, val2 = 0;
        
        if (stk->elemAmt < 2)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot sub last two elements\nThere are less than 2 elements\nError line - %lu\n", line_n);
            printf("Error in line %lu: there are less than 2 elements\n", line_n);
            return 1;
        }

        stack_pop (stk, &val1);
        stack_pop (stk, &val2); 
        stack_push(stk, (val2 - val1));

        return 0;
    }

    if (cmd == MUL)
    {
        int val1 = 0, val2 = 0;
        
        if (stk->elemAmt < 2)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot mul last two elements\nThere are less than 2 elements\nError line - %lu\n", line_n);
            printf("Error in line %lu: there are less than 2 elements\n", line_n);
            return 1;
        }

        stack_pop (stk, &val1);
        stack_pop (stk, &val2); 
        stack_push(stk, (val2 * val1));

        return 0;
    }

    if (cmd == DIV)
    {
        int val1 = 0, val2 = 0;
        
        if (stk->elemAmt < 2)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot sub last two elements\nThere are less than 2 elements\nError line - %lu\n", line_n);
            printf("Error in line %lu: there are less than 2 elements\n", line_n);
            return 1;
        }

        stack_pop (stk, &val1);
        stack_pop (stk, &val2); 
        stack_push(stk, (val2 / val1));

        return 0;
    }

    if (cmd == POP)
    {
        if (stk->elemAmt == 0)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot pop an element\nThere are less than 1 element\nError line - %lu\n", line_n);
            printf("Error in line %lu: there are less than 1 element\n", line_n);
            return 1;
        }

        int val1 = 0;
        stack_pop (stk, &val1);
        return 0;
    }

    if (cmd == OUT)
    {
        if (stk->elemAmt == 0)
        {
            fprintf(logfile, "\n   ERROR!!!\nCannot out an element\nThere are less than 1 element\nError line - %lu\n", line_n);
            printf("Error in line %lu: there are less than 1 element\n", line_n);
            return 1;
        }

        int val1 = 0;
        stack_pop (stk, &val1);
        printf("Line %lu, out: %d", line_n, val1);
        
        return 0;
    }

    return 1;
}