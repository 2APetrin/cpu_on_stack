#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "filework_header.h"

#define ACCURACY 100

struct token
{
    char * text;
    int    val;
    int    tok_type;
    int    cmd_type;
    size_t line;
};

struct asm_data
{
    char         * prog_text;
    struct token * toks_array;
    int          * labels_array;
    int          * labels_count_array;
    size_t         num_of_symbols;
    size_t         num_of_toks;
    int            out_arr_len;
    int          * out_array;
};

enum labels
{
    NUM_OF_LABELS = 20,
    FREE          = -1
};

enum token_types
{
    CMD       = 1,
    NUM       = 2,
    LABEL     = 3,
    LABEL_JMP = 4,
    REGISTER  = 5
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
    SQRT   = 19,
    NROOTS = 20,
    ALLNUM = 21,
    CAST   = 22
};

enum register_number
{
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};


void   run_asm(FILE * in_stream, FILE * out_stream);

int    asm_ctor(FILE * in_stream, asm_data * asm_data);

size_t get_num_of_tokens(char * prog_text, size_t num_of_toks);

int    get_tokens(token * toks, size_t num_of_toks, char * prog_text);

char * skip_spaces(char * start_ptr, size_t * line_ptr);

char * get_end_of_word(char * word_ptr, size_t * p_line);

int    put_null_to_end_of_last_token(char * str_ptr);

int    asm_toks_init(asm_data * asm_data);

int    tok_info_init(token * tok, asm_data * asm_data, size_t itteration, int * minus_count);

int    is_jump(int cmd_type);

int    is_register(char * str);

int    check_tkn_for_string(char * str);

int    check_tkn_for_num(char * str);

int    check_tkn_for_label(char * str);

int    get_type(char * str);

int    labels_init(asm_data * asm_data);

int    check_toks_arr(struct token * tok_arr, size_t num_of_toks, asm_data asm_data);

int    get_out_arr_len(token * tok_arr, size_t arr_len);
