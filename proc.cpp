#include "header.h"

void run_cpu(FILE * stream)
{
    assert(stream != NULL);
    
    struct my_stack stk = {};
    stack_ctor(&stk, 20);

    struct my_code code = {};
    code_ctor(&code, stream);

    code_dtor(&code);
    stack_dtor(&stk);
}