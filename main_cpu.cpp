#include "header_cpu.h"

FILE * logfile = NULL;

int main(void)
{
    openfile("log.txt");

    my_stack stk = {};
    stack_ctor(&stk, 10);
    stack_push(&stk, 10);
    int i = 0;
    stack_pop (&stk, &i);
    stack_dtor(&stk);

    fclose(logfile);
    return 0;
}