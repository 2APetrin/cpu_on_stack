#include "asm_header.h"

FILE * open_inputfile(const char * filename)
{
    FILE * return_ptr = fopen(filename, "r");

    if (return_ptr == NULL)
    {
        printf("Cannot open inputfile. Programm shutdown\n");
        abort();  
    }

    return return_ptr;
}

FILE * open_outputfile(const char * filename)
{
    FILE * return_ptr = fopen(filename, "wb");

    if (return_ptr == NULL)
    {
        printf("Cannot open outputfile. Programm shutdown\n");
        abort();  
    }

    return return_ptr;
}