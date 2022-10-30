#include "header.h"

int main(void)
{
    openlogfile("log.txt");
    struct my_stack stk = {};

    size_t cap = 10;
    stack_ctor(&stk, cap);

    //stk.data = NULL;
    stack_dump(&stk, LOCATION);

    for (int i = 0; i < 11; i++)
    {
        stack_push(&stk, 10);
    }

    int pop = 0;
    stack_pop(&stk, &pop);
    printf("pop - %d\n", pop);

    stack_dump(&stk, LOCATION);

    stack_dtor(&stk);

    fclose(logfile);
    return 0;
}