#include "header.h"

int main(void)
{
    openfile("log.txt");
    /*struct my_stack stk = {};

    stack_ctor(&stk, 10);*/

    FILE * fp = fopen("cod.txt", "r");
    run_cpu(fp);

    /*stack_dump(&stk, LOCATION);
    stack_dtor(&stk);*/
    fclose(logfile);
    return 0;
}