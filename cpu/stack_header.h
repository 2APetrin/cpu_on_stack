#pragma once

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

extern FILE * logfile;

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

    struct var_info stack_info;
};

enum DumpEnterReasons
{
    DUMP_ENTRY       = 0,
    DUMP_EXITING     = 1,
    DUMP_FOR_ERROR   = 2
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


int    open_logfile(const char * filename);

int   _stack_ctor(my_stack * stk, size_t cap, struct var_info info);

void  _stack_check(my_stack * stk, const char * func_name, const char * file_name, int lineofcall);

size_t err_check(my_stack * stk);

void   stack_dump(my_stack * stk, const char * func_name, const char * file_name, int lineofcall, int entry_reason);

int    stack_dtor(my_stack * stk);

void   stack_push(my_stack * stk, elem val);

void   stack_pop(my_stack * stk, elem  * var);

int    stack_resize(int size_cmd, my_stack * stk);

const char * get_dump_reason(int entry_reason);
