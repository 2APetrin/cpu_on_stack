#include "asm_header.h"

int open_logfile(void)
{
    if ((logfile = fopen("asm_logfile.txt", "w")) == NULL)
    {
        printf("Cannot open logfile. Programm shutdown\n");
        abort();
    }
    return 0;
}

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
    FILE * return_ptr = fopen(filename, "w");
    
    if (return_ptr == NULL)
    {
        printf("Cannot open outputfile. Programm shutdown\n");
        abort();  
    }
    return return_ptr;
}
