#include "asm_header.h"

FILE * logfile = NULL;

int main(void)
{
    open_logfile();

    FILE * input_file  = open_inputfile("test.asm");
    FILE * output_file = open_outputfile("test.code");
    
    assembler(input_file, output_file);

    fclose(logfile);
    return 0;
}