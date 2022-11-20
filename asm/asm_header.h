#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <cstring>


struct my_code
{
    char ** strings;
    char *  text;
    size_t  num_of_symbols;
    size_t  num_of_lines;
};

enum CmdCodes
{
    NTHG = 0,
    HLT  = 0,
    PUSH = 1,
    ADD  = 2,
    SUB  = 3,
    MUL  = 4,
    DIV  = 5,
    POP  = 6,
    OUT  = 7
};


extern FILE * logfile;


int    open_logfile(void);

FILE * open_inputfile (const char * filename);
FILE * open_outputfile(const char * filename);

int    assembler(FILE * in, FILE * out);

size_t getnum_of_lines(struct my_code * cod);

char * get_next_ptr(char * pr_ptr);

void   code_ctor(my_code * code, FILE * stream);

void   get_indexes(my_code * code);

void   code_dtor(my_code * code);

void   make_n_o(my_code * code);

int    print_code(char * cmd_ptr, FILE * out, size_t line_n);

int    write_code(my_code * code, FILE * out);

int    is_blank(char * ptr);

void cmd_verify(char * cmd_ptr, size_t line_n, int cmd_type);

#endif