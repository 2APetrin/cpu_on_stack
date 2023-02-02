#pragma once

#include "stack_header.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct cpu_s
{
    struct my_stack stack;
    size_t exe_code_len;
    int *  exe_code_arr;
    int *  ram;
    int    reg_ax;
    int    reg_bx;
    int    reg_cx;
    int    reg_dx;
} cpu;

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
    JMP_NE = 18
};

enum register_number
{
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};


FILE * open_inputfile(const char * filename);

FILE * open_outputfile(const char * filename);

int    run_cpu(FILE * in_stream);

int    cpu_ctor(cpu * cpu);

int    get_num_of_symbols(cpu * cpu, FILE * in_stream);

int    get_exe_arr(cpu * cpu, FILE * in_stream);

int    execute_code(cpu cpu);

int get_register_val(int * ret_val, int reg_num, cpu cpu);

int register_fill(int reg_num, int value, cpu cpu);