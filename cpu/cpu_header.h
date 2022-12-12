#pragma once

#include "stack_header.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


struct cpu_s
{
    struct my_stack   cpu_stack;
    elem            * executable_code_array;
    //elem            * cpu_ram; // зачем нужна оперативная память?
    elem              register_ax;
    elem              register_bx;
    elem              register_cx;
    elem              register_dx;
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

enum CmdCodes
{
    ERROR = -1,
    HLT   = 0,
    PUSH  = 1,
    ADD   = 2,
    SUB   = 3,
    MUL   = 4,
    DIV   = 5,
    OUT   = 6,
    POPR  = 7, // изменить на pop ax
    PUSHR = 8,
    JUMP  = 9,
    LABEL = 1000,
    AX    = 1001,
    BX    = 1002,
    CX    = 1003,
    DX    = 1004,
    NUM   = 3802
};

FILE * open_inputfile(const char * filename);

FILE * open_outputfile(const char * filename);

int    run_cpu(FILE * in_stream);

size_t get_num_of_symbols(FILE * in_stream);

int    execute_code(int * array, size_t len, my_stack * stk);

int    cpu_ctor(struct cpu_s * proc);