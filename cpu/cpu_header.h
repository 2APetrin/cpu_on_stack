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
    //elem            * cpu_ram;
    elem              register_ax;
    elem              register_bx;
    elem              register_cx;
    elem              register_dx;
};

enum cmd_codes
{
    ERROR  = -1,
    HLT    = 0,
    PUSH   = 1,
    ADD    = 2,
    SUB    = 3,
    MUL    = 4,
    DIV    = 5,
    OUT    = 6,
    POP    = 7,
    PUSHR  = 8,
    JMP    = 9,
    IN     = 10,
    CALL   = 11,
    RET    = 12,
    JMP_B  = 13,
    JMP_BE = 14,
    JMP_A  = 15,
    JMP_AE = 16,
    JMP_E  = 17,
    JMP_NE = 18,
    MOD    = 19
};

FILE * open_inputfile(const char * filename);

FILE * open_outputfile(const char * filename);

int    run_cpu(FILE * in_stream);

size_t get_num_of_symbols(FILE * in_stream);

int    execute_code(int * array, size_t len, my_stack * stk);

int    cpu_ctor(struct cpu_s * proc);