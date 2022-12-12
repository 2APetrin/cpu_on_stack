#include "asm_header.h"
#include "filework_header.h"

FILE * open_inputfile(const char * filename)
{
    FILE * return_ptr = fopen(filename, "rb");

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
