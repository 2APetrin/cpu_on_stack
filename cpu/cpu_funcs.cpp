#include "cpu_header.h"

int run_cpu(FILE * in_stream)
{
    assert(in_stream);

    /*struct my_stack stk = {};
    stack_ctor(&stk, MIN_CAPACITY);*/
    struct cpu_s cpu = {};
    cpu_ctor(&cpu);

    size_t num_of_symbols = get_num_of_symbols(in_stream);
    //printf("Num of bytes = %lu\n", num_of_symbols);
    size_t array_length = num_of_symbols / sizeof(int);

    cpu.executable_code_array = (int *) calloc (array_length, sizeof(int));

    fread(cpu.executable_code_array, sizeof(int), num_of_symbols, in_stream);
    fclose(in_stream);

    /*for (size_t i = 0; i < array_length; i++)
    {
        printf("%d\n", cpu.executable_code_array[i]);
    }*/

    if (execute_code(cpu.executable_code_array, array_length, &cpu.cpu_stack))
    {
        printf("Error: unknown command\n");
        return 1;
    }

    free(cpu.executable_code_array);
    stack_dtor(&cpu.cpu_stack);

    return 0;
}

int cpu_ctor(struct cpu_s * proc)
{
    assert(proc);
    
    //proc->cpu_stack = (struct my_stack *) calloc (1, sizeof(my_stack));
    stack_ctor(&proc->cpu_stack, MIN_CAPACITY);

    return 0;
}

int execute_code(int * array, size_t len, my_stack * stk)
{
    assert(array);
    assert(stk);

    for (size_t i = 0; i < len; i++)
    {
        // printf("loop exe code entered\n");

        // Заменить на switch

        if (array[i] == HLT)
        {
            if (stk->elemAmt != 0)
            {
                int k = 0;
                size_t reps = stk->elemAmt; 
                printf("Stack isn't empty. You've forgotten about %lu elements:\n", reps);

                for (size_t t = 0; t < reps; t++)
                {
                    stack_pop(stk, &k);
                    printf("%d\n", k);
                }
            }

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

        /*if (array[i] == POP)
        {
            if (stk->elemAmt < 1)
            {
                printf("Error: cannot POP elements, there is less than one element in stack\n");
                abort();
            }

            int val1 = 0;
            stack_pop (stk, &val1);

            continue;
        }*/ //сделать из этого попр

        if (array[i] == JMP)
        {
            i = (size_t) (array[i+1] - 1);
            continue;
        }

        if (array[i] == OUT)
        {
            if (stk->elemAmt < 1)
            {
                printf("Error: cannot OUT elements, there is less than one element in stack\n");
                abort(); // заменить на ретёрн / заменить на свой аборт
            }

            int val1 = 0;
            stack_pop (stk, &val1);
            printf("OUT: %d\n", val1);

            continue;
        }

        if (array[i] == MOD)
        {
            if (stk->elemAmt < 2)
            {
                printf("Error: cannot MOD elements, there are less than two elements in stack\n");
                abort();
            }

            int val1 = 0, val2 = 0;
            stack_pop (stk, &val1);
            stack_pop (stk, &val2); 
            stack_push(stk, (val2 % val1));
            
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
