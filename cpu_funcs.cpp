#include "cpu_header.h"

int run_cpu(FILE * in_stream)
{
    assert(in_stream);

    struct my_stack stk = {};
    stack_ctor(&stk, MIN_CAPACITY);

    size_t num_of_symbols = get_num_of_symbols(in_stream);
    //printf("Num of bytes = %lu\n", num_of_symbols);
    size_t array_length = num_of_symbols / sizeof(int);

    int * in_array = (int *) calloc (array_length, sizeof(int));

    fread(in_array, sizeof(int), num_of_symbols, in_stream);
    fclose(in_stream);

    /*for (size_t i = 0; i < array_length; i++)
    {
        printf("%d\n", in_array[i]);
    }*/

    if (execute_code(in_array, array_length, &stk))
    {
        printf("Error: unknown command\n");
        abort();
    }

    free(in_array);
    stack_dtor(&stk);   

    return 0;
}

int execute_code(int * array, size_t len, my_stack * stk)
{
    assert(array);
    assert(stk);

    for (size_t i = 0; i < len; i++)
    {
        if (array[i] == HLT)
        {
            return 0;
        }

        if (array[i] == PUSH)
        {
            stack_push(stk, array[++i]);
            continue;
        }

        if (array[i] == ADD)
        {
            if (stk->elemAmt < 2)
            {
                printf("Error: cannot ADD elements, there are less than two elements in stack\n");
                abort();
            }
            
            int val1 = 0, val2 = 0;
            stack_pop (stk, &val1);
            stack_pop (stk, &val2); 
            stack_push(stk, (val2 + val1));
            
            continue;
        }

        if (array[i] == SUB)
        {
            if (stk->elemAmt < 2)
            {
                printf("Error: cannot SUB elements, there are less than two elements in stack\n");
                abort();
            }

            int val1 = 0, val2 = 0;
            stack_pop (stk, &val1);
            stack_pop (stk, &val2); 
            stack_push(stk, (val2 - val1));
            
            continue;
        }

        if (array[i] == MUL)
        {
            if (stk->elemAmt < 2)
            {
                printf("Error: cannot MUL elements, there are less than two elements in stack\n");
                abort();
            }

            int val1 = 0, val2 = 0;
            stack_pop (stk, &val1);
            stack_pop (stk, &val2); 
            stack_push(stk, (val2 * val1));
            
            continue;
        }

        if (array[i] == DIV)
        {
            if (stk->elemAmt < 2)
            {
                printf("Error: cannot DIV elements, there are less than two elements in stack\n");
                abort();
            }

            int val1 = 0, val2 = 0;
            stack_pop (stk, &val1);
            stack_pop (stk, &val2); 
            stack_push(stk, (val2 / val1));
            
            continue;
        }

        if (array[i] == POP)
        {
            if (stk->elemAmt < 1)
            {
                printf("Error: cannot POP elements, there is less than one element in stack\n");
                abort();
            }

            int val1 = 0;
            stack_pop (stk, &val1);

            continue;
        }

        if (array[i] == OUT)
        {
            if (stk->elemAmt < 1)
            {
                printf("Error: cannot OUT elements, there is less than one element in stack\n");
                abort();
            }

            int val1 = 0;
            stack_pop (stk, &val1);
            printf("OUT: %d\n", val1);

            continue;
        }

        return 1;
    }
    return 1;
}

size_t get_num_of_symbols(FILE * in_stream)
{
    assert(in_stream);

    fseek(in_stream, 0L, SEEK_END);
    size_t num_of_letters = (size_t) ftell(in_stream);
    rewind(in_stream);

    return num_of_letters;
}
