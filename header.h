#ifndef HEADER_H
#define HEADER_H

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define stack_ctor(stk, size) _stack_ctor((stk), (size), #stk, LOCATION)
#define stack_check(stk) _stack_check((stk), LOCATION)

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

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

enum ErrorCodes
{
    BAD_STK_POINTER  = 1,
    BAD_DATA_POINTER = 2,
    BIG_ELEM_AMT     = 4,
    BAD_STK_NAME     = 8
};

extern FILE * logfile;

void  _stack_ctor(my_stack * stk, size_t cap, const char * p_name, const char * p_func, const char * p_file, int p_line);

void  _stack_check(my_stack * stk, const char * func_name, const char * file_name, int lineofcall);

size_t err_check(my_stack * stk);

void   stack_dump(my_stack * stk, const char * func_name, const char * file_name, int lineofcall);

void   stack_dtor(my_stack * stk);

void openlogfile(const char * name);

void stack_push(my_stack * stk, elem val);

void stack_pop(my_stack * stk, int * var);

#endif