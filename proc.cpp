#include "header.h"

void run_cpu(FILE * stream)
{
    assert(stream != NULL);
    
    struct my_stack stk = {};
    stack_ctor(&stk, MIN_CAPACITY);

    struct my_code code = {};
    code_ctor(&code, stream);

    for (size_t i = 0; i < code.num_of_lines; i++)
    {
        if( get_do_command(code.strings[i], &stk) )
            break;
    }

    stack_dump(&stk, LOCATION);

    code_dtor(&code);
    stack_dtor(&stk);
}

void code_ctor(my_code * code, FILE * stream)
{
    assert(code   != NULL);
    assert(stream != NULL);

    fseek(stream, 0L, SEEK_END);
    code->num_of_symbols = (size_t) ftell(stream);
    rewind(stream);

    code->text = (char *) calloc(code->num_of_symbols + 1, sizeof(char));
    fread(code->text, sizeof(char), code->num_of_symbols, stream);
    fclose(stream);
    code->text[code->num_of_symbols] = 0;

    code->num_of_lines = getnum_of_lines(code);
    code->strings = (char **) calloc(code->num_of_lines, sizeof(char *));
    assert(code->strings != NULL);

    get_indexes(code);
    make_n_o(code);
}

size_t getnum_of_lines(struct my_code * cod)
{
    assert(cod != NULL);

    size_t count = 0;
    for (size_t i = 0; i < cod->num_of_symbols; i++)
    {
        if (cod->text[i] == '\n')
            count++;
    }

    //printf("num of lines - %lu\n", count);
    return count;
}

void get_indexes(my_code * code)
{
    assert(code != NULL);

    code->strings[0] = code->text;

    for (size_t i = 1; i < code->num_of_lines; i++)
    {
        code->strings[i] = get_next_ptr(code->strings[i - 1]);
    }
}

char * get_next_ptr(char * pr_ptr)
{
    assert(pr_ptr != NULL);

    int i = 0;
    while (*(pr_ptr + i) != '\n')
    {
        i++;
    }

    return (pr_ptr + i + 1);
}

void code_dtor(my_code * code)
{
    assert(code != NULL);

    free(code->text);
    code->text = NULL;
    free(code->strings);
    code->strings = NULL;
    code = NULL;
}

void make_n_o(my_code * code)
{
    for (size_t i = 0; i < code->num_of_symbols + 1; i++)
    {
        if(code->text[i] == '\n')
        {
            code->text[i] = 0;
        }
    }
}

int get_do_command(char * command, my_stack * stk)
{
    char cmd[10] = "";
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
            for (size_t i = 0; i < stk->elemAmt; i++)
            {
                printf("[%lu] - %d\n", i + 1, stk->data[i]);
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