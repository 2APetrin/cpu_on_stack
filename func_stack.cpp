#include "header_cpu.h"

void _stack_ctor(my_stack * stk, size_t cap, struct var_info info)
{
    assert(stk    != NULL);
    /*assert(info.name != NULL);
    assert(info.name != NULL);*/

    assert(info.name != NULL);
    assert(info.func != NULL);
    assert(info.file != NULL);
    assert(info.line  > 0);

    stk->capacity  = cap;
    stk->elemAmt   = 0;
    stk->data      = (elem *) calloc(stk->capacity, sizeof(elem));
    
    stk->info = info;

    /*stk->info.name = info.name;
    stk->info.file = info.file;
    stk->info.line = info.line;
    stk->info.func = info.func;*/

    stack_check(stk);
}

void _stack_check(my_stack * stk, const char * func_name, const char * file_name, int lineofcall)
{
    size_t num_of_err = err_check(stk);
    if (num_of_err)
    {
        fprintf(logfile, "\n   ERROR!!!\nerror catched in:\nfile - %s,\nfunc - %s,\nline - %d\n", file_name, func_name, lineofcall);
        fprintf(logfile, "error code - %lu\n\n", num_of_err);
        fclose(logfile);

        abort();
    }
}

size_t err_check(my_stack * stk)
{
    size_t sum = 0;

    if (stk == NULL)
        sum += BAD_STK_POINTER;
    if (stk->data == NULL)
        sum += BAD_DATA_POINTER; 
    if (stk->elemAmt > stk->capacity)
        sum += BIG_ELEM_AMT;
    if (stk->info.name == NULL)
        sum += BAD_STK_NAME;
    if (stk->capacity < MIN_CAPACITY)
        sum += BAD_CAPACITY;
    if (stk->info.file == NULL)
        sum += BAD_STK_FILENAME;
    if (stk->info.func == NULL)
        sum += BAD_STK_FUNCNAME;
    
    return sum;
}

void stack_dump(my_stack * stk, const char * func_name, const char * file_name, int lineofcall, int entry_reason)//написать вход или выход из функции
{
    stack_check(stk);

    fprintf(logfile, "\n-----------------------------------------------------------------------\n");

    fprintf(logfile, "   DUMP\n was called:\nfile - %s,\nfunc - %s,\nline - %d\n\n", file_name, func_name, lineofcall);

    fprintf(logfile, get_dump_reason(entry_reason));

    fprintf(logfile, "\n   Stack info:\n");
    fprintf(logfile, "element amount = %lu\n", stk->elemAmt);
    fprintf(logfile, "stack capacity = %lu\n", stk->capacity);
    fprintf(logfile, "pointer to data - %p\n", stk->data);

    char ch = ' ';
    
    for (size_t i = 0; i < stk->capacity; i++)
    {
        ch = ' ';
        if (i < stk->elemAmt)
            ch = '*';
        
        fprintf(logfile, "%c[%2lu] - %d\n", ch, (i + 1), stk->data[i]);
    }

    fprintf(logfile, "\n   Variable info:\n");
    fprintf(logfile, "variable name - %s\n", stk->info.name);
    fprintf(logfile, "creation file - %s\n", stk->info.file);
    fprintf(logfile, "creation func - %s\n", stk->info.func);
    fprintf(logfile, "creation line - %d\n", stk->info.line);

    fprintf(logfile, "-----------------------------------------------------------------------\n");

    stack_check(stk);
}

const char * get_dump_reason(int entry_reason)
{
    if (entry_reason == DUMP_ENTRY)
        return "Reason - Entry into function\n";
    if (entry_reason == DUMP_EXITING)
        return "Reason - Exiting from function\n";
    if (entry_reason == DUMP_FOR_ERROR)
        return "Reason - Dumping from middle of function, because of error\n";
    
    return "Unknown reason\n";
}

void stack_dtor(my_stack * stk)
{
    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_ENTRY);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = 0;
    }

    free(stk->data);
    stk->data = NULL;
    stk->capacity = 0;
    stk->elemAmt  = 0;
    stk = NULL;
}

void openfile(const char * name)
{
    if ((logfile = fopen(name, "w")) == NULL)
    {
        fprintf(logfile, "Cannot open logfile\n");
        fclose(logfile);

        abort();
    }
}

void stack_push(my_stack * stk, elem val)
{
    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_ENTRY);

    if (stk->elemAmt == stk->capacity)
    {
        stack_resize(SIZEUP, stk);
    }

    if (stk->elemAmt < stk->capacity)
    {
        stk->data[stk->elemAmt++] = val;
    }
    else
    {
        fprintf(logfile, "\n   WARNING!!!\nStack overflow)0)0))0))0)\nLast element (%d) was not added to stack\nThere is dump under\n", val);
        stack_dump(stk, LOCATION, DUMP_FOR_ERROR);
    }

    if ((stk->capacity >= (MIN_CAPACITY * 2)) && (stk->elemAmt <= (stk->capacity / 4)))
    {
        stack_resize(SIZEDOWN, stk);
    }

    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_EXITING);
}

void stack_pop(my_stack * stk, int * var)
{
    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_ENTRY);

    if (stk->elemAmt == stk->capacity)
    {
        stack_resize(SIZEUP, stk);
    }

    if (stk->elemAmt > 0)
    {
        *var = stk->data[stk->elemAmt - 1];
        stk->data[stk->elemAmt - 1] = 0;
        stk->elemAmt--; 
    }
    else
    {
        fprintf(logfile, "\n   WARNING!!!\nThere are no elements in stack data\nAny value was poped\nThere is dump under\n");
        stack_dump(stk, LOCATION, DUMP_FOR_ERROR);
    }

    if ((stk->capacity >= (MIN_CAPACITY * 2)) && (stk->elemAmt <= (stk->capacity / 4)))
    {
        stack_resize(SIZEDOWN, stk);
    }

    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_EXITING);
}

int stack_resize(int size_up, my_stack * stk)
{
    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_ENTRY);
    
    fprintf(logfile, "\n-----------------------------------------------------------------------\n");
    fprintf(logfile, "   REALLOCATION!!!\ntype of reallocation - %d\n", size_up);
    fprintf(logfile, "-----------------------------------------------------------------------\n");

    if (size_up)
    {
        stk->data = (elem *) realloc(stk->data, stk->capacity * 2 * sizeof(elem));
        stk->capacity *=2;
        for (size_t i = stk->elemAmt; i < stk->capacity; i++)
        {
            stk->data[i] = 0;
        }
    }
    else
    {
        stk->data = (elem *) realloc(stk->data, stk->capacity / 2 * sizeof(elem));
        stk->capacity /= 2;
    }

    stack_check(stk);
    stack_dump(stk, LOCATION, DUMP_EXITING);
    
    return 0;
}