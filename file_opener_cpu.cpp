#include "cpu_header.h"

int open_logfile(const char * filename)
{
    if ((logfile = fopen(filename, "w")) == NULL)
    {
        printf("Cannot open logfile. Programm shutdown\n");
        abort();
    }
    return 0;
}

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