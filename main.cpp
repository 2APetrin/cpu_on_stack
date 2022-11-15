#include "header.h"

int main(void)
{
    openfile("log.txt");

    FILE * fp = NULL;
    if ((fp = fopen("cod.txt", "r")) == NULL)
    {
        fprintf(logfile, "Cannot open codefile\n");
        fclose(logfile);
        abort();
    }
    
    run_cpu(fp);

    fclose(logfile);
    return 0;
}