#include "cpu_header.h"

FILE * logfile = NULL;

int main(void)
{
    open_logfile("log.txt");

    FILE * codefile = open_inputfile("/home/tony/Code/my_stack/asm/test.code");
    run_cpu(codefile);

    fclose(logfile);
    return 0;
}