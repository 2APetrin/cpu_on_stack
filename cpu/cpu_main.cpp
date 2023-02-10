#include "cpu_header.h"

FILE * logfile = NULL;

int main(void)
{
    open_logfile("../cpu/logs/stack_log.txt");

    FILE * codefile = open_inputfile("../code/exe.code");
    if (run_cpu(codefile))
    {
        printf("Execution failed\n");
    }

    fclose(logfile);
    return 0;
}