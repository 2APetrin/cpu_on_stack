#include "header_cpu.h"

FILE * logfile = NULL;

int main(void)
{
    openfile("log.txt");

    FILE * codefile = open_inputfile("/home/tony/Code/my_stack/asm/test.code");
    run_cpu(codefile);

    fclose(logfile);
    return 0;
}