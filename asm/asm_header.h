#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef int elem;

struct token
{
    char * text;
    int    val;
    int    type;
    size_t line;
};

struct my_code
{
    char ** strings;
    char *  text;
    size_t  num_of_symbols;
    size_t  num_of_lines;
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
    POP   = 6,
    OUT   = 7,
    NUM   = 3802
};


int    run_asm(FILE * in_stream, FILE * out_stream);

size_t getnum_of_cmds(char * prog_text, size_t num);

int    open_logfile(const char * filename);

FILE * open_inputfile(const char * filename);

FILE * open_outputfile(const char * filename);

char * getptr_toks_zero(char * prog_text);

int    tok_info_init(struct token * tkn);

int    check_tkn_for_num(char * str);

int    check_tkn_for_string(char * str);

int    get_type_cmd(char * str);

int    check_code(struct token * array, size_t num_of_cmds);

int    get_tokens(token * toks, size_t num_of_cmds, char * prog_text);

char * skip_spaces(char * start_ptr, size_t * line);

char * get_end_of_word(char * word_ptr, size_t * p_line);
