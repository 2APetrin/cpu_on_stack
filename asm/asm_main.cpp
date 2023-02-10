#include "asm_header.h"

int main(void)
{
    FILE * input_file  = open_inputfile ("../code/eq.asm");
    FILE * output_file = open_outputfile("../code/exe.code");

    run_asm(input_file, output_file);

    return 0;
}