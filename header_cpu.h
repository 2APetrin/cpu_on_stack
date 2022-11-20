#ifndef HEADER_H
#define HEADER_H

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define stack_ctor(stk, size) _stack_ctor((stk), (size), var_info {#stk, LOCATION})
#define stack_check(stk) _stack_check((stk), LOCATION)
#define MIN_CAPACITY 8


#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


typedef int elem;

struct var_info
{
    const char * name;
    const char * func;
    const char * file;
    int          line;
};

struct my_stack
{
    elem * data;
    size_t elemAmt;
    size_t capacity;

    struct var_info info;
};

struct my_code
{
    char ** strings;
    char *  text;
    size_t  num_of_symbols;
    size_t  num_of_lines;
};

enum ErrorCodes
{
    BAD_STK_POINTER  = 1,
    BAD_DATA_POINTER = 2,
    BIG_ELEM_AMT     = 4,
    BAD_STK_NAME     = 8,
    BAD_CAPACITY     = 16,
    BAD_STK_FILENAME = 32,
    BAD_STK_FUNCNAME = 64
};

enum ResizeCodes
{
    SIZEUP   = 1,
    SIZEDOWN = 0
};

enum DumpEnterReasons
{
    DUMP_ENTRY       = 0,
    DUMP_EXITING     = 1,
    DUMP_FOR_ERROR   = 2
};


extern FILE * logfile;


void  _stack_ctor(my_stack * stk, size_t cap, struct var_info info);

void  _stack_check(my_stack * stk, const char * func_name, const char * file_name, int lineofcall);

size_t err_check(my_stack * stk);

void   stack_dump(my_stack * stk, const char * func_name, const char * file_name, int lineofcall, int enter_reason);
//написать причину входа в дамп через чар, как доп аргумент

void   stack_dtor(my_stack * stk);

void   openfile(const char * name);

void   stack_push(my_stack * stk, elem val);

void   stack_pop(my_stack * stk, int * var);

void   run_cpu(FILE * code);

size_t getnum_of_lines(struct my_code * cod);

char * get_next_ptr(char * pr_ptr);

void   code_ctor(my_code * code, FILE * stream);

void   get_indexes(my_code * code);

void   code_dtor(my_code * code);

void   make_n_o(my_code * code);

int    get_do_command(char * command, my_stack * stk);

int    stack_resize(int size_up, my_stack * stk);

const char * get_dump_reason(int entry_reason);

#endif