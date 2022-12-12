#include "asm_header.h"

void run_asm(FILE * in_stream, FILE * out_stream) // дописать call
{
    assert(in_stream);
    assert(out_stream);

    struct asm_data asm_data = {};

    asm_ctor(in_stream, &asm_data);

    int flag = 1;
    if (asm_toks_init(&asm_data))
        flag = 0;

    if (labels_init(&asm_data)) //проверка на количество меток (не больше одной каждого типа)
        flag = 0;
    
    /*for (size_t i = 0; i < asm_data.num_of_toks; i++)
    {
        printf("%s, line - %lu, type - %d\n", asm_data.toks_array[i].text, asm_data.toks_array[i].line, asm_data.toks_array[i].tok_type); 
    }*/

    if (check_toks_arr(asm_data.toks_array, asm_data.num_of_toks, asm_data))
        flag = 0;
    
    if (flag)
    {
        //создание массива на запись + его заполнение + сообщение завершения компиляции
        asm_data.out_arr_len = get_out_arr_len(asm_data.toks_array, asm_data.num_of_toks);
        asm_data.out_array   = (int *) calloc ((size_t) asm_data.out_arr_len, sizeof(int));
        //printf("asm_data.out_arr_len = %d", asm_data.out_arr_len);

        size_t minus_count = 0;
        for (size_t i = 0; i < asm_data.num_of_toks; i++)
        {
            if (asm_data.toks_array[i].tok_type == NUM)
            {
                asm_data.out_array[i - minus_count] = asm_data.toks_array[i].val;
            }
            if (asm_data.toks_array[i].tok_type == CMD)
            {
                asm_data.out_array[i - minus_count] = asm_data.toks_array[i].cmd_type;
            }
            if (asm_data.toks_array[i].tok_type == LABEL)
            {
                minus_count++;
            }
            if (asm_data.toks_array[i].tok_type == LABEL_JMP)
            {
                asm_data.out_array[i - minus_count] = asm_data.toks_array[i].val;
            }
            if (asm_data.toks_array[i].tok_type == REGISTER)
            {
                asm_data.out_array[i - minus_count] = asm_data.toks_array[i].cmd_type;
            }
        }

        for (int i = 0; i < asm_data.out_arr_len; i++)
            printf("%d\n", asm_data.out_array[i]);
        
        fwrite(asm_data.out_array, sizeof(int), (size_t) asm_data.out_arr_len, out_stream);

        free(asm_data.out_array);
    }
    else
    {
        printf("Compilation error\n");
    }

    fclose(out_stream);
    free(asm_data.labels_array);
    free(asm_data.labels_count_array);
    free(asm_data.prog_text);
    free(asm_data.toks_array);
}

int asm_ctor(FILE * in_stream, asm_data * asm_data)
{
    assert(in_stream);
    assert(asm_data);

    fseek(in_stream, 0L, SEEK_END);
    asm_data->num_of_symbols = (size_t) ftell(in_stream);
    rewind(in_stream);

    asm_data->prog_text = (char *) calloc(asm_data->num_of_symbols + 1, sizeof(char));
    fread(asm_data->prog_text, sizeof(char), asm_data->num_of_symbols, in_stream);
    fclose(in_stream);
    //printf("infile len = %lu\n", asm_data->num_of_symbols);

    asm_data->num_of_toks = get_num_of_tokens(asm_data->prog_text, asm_data->num_of_symbols);
    asm_data->toks_array  = (struct token *) calloc (asm_data->num_of_toks, sizeof(struct token));

    //printf("num of toks = %lu\n", asm_data->num_of_toks);

    if (get_tokens(asm_data->toks_array, asm_data->num_of_toks, asm_data->prog_text))
        return 1;

    /*for (size_t i = 0; i < asm_data->num_of_toks; i++)
    {
        printf("%s, line - %lu\n", asm_data->toks_array[i].text, asm_data->toks_array[i].line); 
    }*/

    asm_data->labels_array       = (int *) calloc (NUM_OF_LABELS, sizeof(int));
    asm_data->labels_count_array = (int *) calloc (NUM_OF_LABELS, sizeof(int));

    for (int i = 0; i < NUM_OF_LABELS; i++)
    {
        asm_data->labels_array[i] = FREE;
    }

    return 0;
}

size_t get_num_of_tokens(char * prog_text, size_t num_of_toks)
{
    assert(prog_text);

    size_t i = 0;
    if (isgraph(prog_text[0])) 
        i++;

    for (size_t k = 1; k < num_of_toks; k++)
    {
        if (isspace(prog_text[k - 1]) && isgraph(prog_text[k])) 
            i++;
    }

    return i;
}

int get_tokens(token * toks, size_t num_of_toks, char * prog_text)
{
    assert(toks);
    assert(prog_text);

    size_t line = 1;
    char * end_of_word = prog_text;

    for (size_t i = 0; i < num_of_toks; i++)
    {
        toks[i].text = skip_spaces(end_of_word, &line);
        toks[i].line = line;

        if (i < num_of_toks - 1)
            end_of_word  = get_end_of_word(toks[i].text, &line);
    }

    put_null_to_end_of_last_token(toks[num_of_toks - 1].text);

    return 0;
}

char * skip_spaces(char * start_ptr, size_t * line_ptr)
{
    assert(start_ptr);
    assert(line_ptr);

    int i = 0;

    while (isspace(start_ptr[i]))
    {
        if (start_ptr[i] == '\n')
        {
            *line_ptr += 1;
        }

        i++;
    }

    return(start_ptr + i);
}

char * get_end_of_word(char * word_ptr, size_t * p_line)
{
    assert(word_ptr);
    assert(p_line);

    int i = 0;
    while (!isspace(word_ptr[i])) i++;

    if (word_ptr[i] == '\n')
    {
        *p_line += 1;
    }

    word_ptr[i] = '\0';

    return (word_ptr + i + 1);
}

int put_null_to_end_of_last_token(char * str_ptr)
{
    assert(str_ptr);

    int i = 0;
    while (isgraph(str_ptr[i])) i++;

    str_ptr[i] = 0;

    return 0;
}


int asm_toks_init(asm_data * asm_data)
{
    assert(asm_data);

    int ret_flag = 0;
    int minus_count = 0;

    for (size_t i = 0; i < asm_data->num_of_toks; i++)
    {
        if (tok_info_init(&asm_data->toks_array[i], asm_data, i, &minus_count))
            ret_flag = 1;
    }

    return ret_flag;
}

int tok_info_init(token * tok, asm_data * asm_data, size_t itteration, int * minus_count)
{
    assert(tok);
    assert(asm_data);

    if (check_tkn_for_string(tok->text))
    {
        if (is_register(tok->text))
        {
            tok->tok_type = REGISTER;
            tok->cmd_type = get_type(tok->text);

            if (tok->cmd_type == ERROR)
            {
                tok->tok_type = ERROR;
                printf("Error: unknown command in line %lu: %s\n", tok->line, tok->text);
                return 1;
            }
        }
        else
        {
            tok->cmd_type = get_type(tok->text);
            tok->tok_type = CMD;
            //printf("%s cmd_type - %d\n", tkn->text, tkn->type);

            if (tok->cmd_type == ERROR)
            {
                tok->tok_type = ERROR;
                printf("Error: unknown command in line %lu: %s\n", tok->line, tok->text);
                return 1;
            }
        }

        return 0;
    }

    if (check_tkn_for_num(tok->text))
    {
        sscanf(tok->text, "%d", &tok->val);
        tok->tok_type = NUM;
        //printf("num = %d\n", tkn->val);

        return 0;
    }

    if (check_tkn_for_label(tok->text)) // дописать, чтобы номер ячейки для метки был итеррация - кол-во меток до этого (сделано)
    {
        tok->tok_type = LABEL;

        //printf("sscanf ret = %d\n", sscanf(tok->text + 1, "%d", &tok->val));
        
        if (!(sscanf(tok->text + 1, "%d", &tok->val)))
        {
            printf("Error: syntax error in line %lu: %s\n",tok->line, tok->text);
            return 1;
        }

        if (itteration == 0 || !(asm_data->toks_array[itteration - 1].tok_type == CMD && is_jump(asm_data->toks_array[itteration - 1].cmd_type)))
        {
            asm_data->labels_array[tok->val] = (int) itteration - *minus_count;
            asm_data->labels_count_array[tok->val] += 1;
            
            return 0;
        }

        *minus_count += 1;

        return 0;
    }

    printf("Error: syntax error in line %lu: %s\n",tok->line, tok->text);
    tok->tok_type = ERROR;
    return 1;
}

int is_jump(int cmd_type)
{
    if (cmd_type == JMP || cmd_type == JMP_A || cmd_type == JMP_AE || cmd_type == JMP_B || cmd_type == JMP_BE || cmd_type == JMP_E || cmd_type == JMP_NE || cmd_type == CALL)
        return 1;

    return 0;
}

int is_register(char * str)
{
    if (!strcmp(str, "ax"))
        return 1;
    
    if (!strcmp(str, "bx"))
        return 1;
    
    if (!strcmp(str, "cx"))
        return 1;
    
    if (!strcmp(str, "dx"))
        return 1;
    
    return 0;
}

int check_tkn_for_string(char * str)
{
    assert(str);
    
    //printf("enter check_tkn_for_string. str = %s\n", str);
    for (int i = 0; str[i]; i++)
    {
        //printf("%c\n", str[i]);

        if (!isalpha(str[i]))
        {
            return 0;
        }
    }

    return 1;
}

int check_tkn_for_num(char * str)
{
    assert(str);

    int i = 0;
    //printf("enter check_tkn_for_num. str = %s\n", str);

    if (!(isdigit(str[i]) || str[i] == '-' || str[i] == '+'))
            return 0;
    i++;

    for (; str[i]; i++)
    {
        if (!(isdigit(str[i])))
            return 0;
    }

    if ((str[0] == '-' || str[0] == '+') && i == 1)
    {
        return 0;
    }

    return 1;
}

int check_tkn_for_label(char * str)
{
    assert(str);

    if (str[0] == ':')
    {
        int i = 1;

        for (; str[i]; i++)
        {
            if (!(isdigit(str[i])))
                return 0;
        }

        if (i == 1)
            return 0;
        
        return 1;
    }

    return 0;
}

int get_type(char * str)
{
    assert(str);

    if (!strcmp(str, "hlt"))
    {
        return HLT;
    }

    if (!strcmp(str, "push"))
    {
        return PUSH;
    }

    if (!strcmp(str, "add"))
    {
        return ADD;
    }

    if (!strcmp(str, "sub"))
    {
        return SUB;
    }

    if (!strcmp(str, "mul"))
    {
        return MUL;
    }

    if (!strcmp(str, "div"))
    {
        return DIV;
    }

    if (!strcmp(str, "pop"))
    {
        return POP;
    }

    if (!strcmp(str, "out"))
    {
        return OUT;
    }

    if (!strcmp(str, "jmp"))
    {
        return JMP;
    }

    if (!strcmp(str, "pushr"))
    {
        return PUSHR;
    }

    if (!strcmp(str, "in"))
    {
        return IN;
    }

    if (!strcmp(str, "call"))
    {
        return CALL;
    }

    if (!strcmp(str, "jb"))
    {
        return JMP_B;
    }

    if (!strcmp(str, "jbe"))
    {
        return JMP_BE;
    }

    if (!strcmp(str, "ja"))
    {
        return JMP_A;
    }

    if (!strcmp(str, "jae"))
    {
        return JMP_AE;
    }

    if (!strcmp(str, "je"))
    {
        return JMP_E;
    }

    if (!strcmp(str, "jne"))
    {
        return JMP_NE;
    }

    if (!strcmp(str, "ret"))
    {
        return RET;
    }

    if (!strcmp(str, "ax"))
    {
        return AX;
    }

    if (!strcmp(str, "bx"))
    {
        return BX;
    }

    if (!strcmp(str, "cx"))
    {
        return CX;
    }

    if (!strcmp(str, "dx"))
    {
        return DX;
    }

    return ERROR;
}

int labels_init(asm_data * asm_data)
{
    assert(asm_data);

    for (size_t i = 1; i < asm_data->num_of_toks; i++)
    {
        if (asm_data->toks_array[i].tok_type == LABEL && asm_data->toks_array[i-1].tok_type == CMD && is_jump(asm_data->toks_array[i-1].cmd_type))
        {
            asm_data->toks_array[i].tok_type = LABEL_JMP;
            asm_data->toks_array[i].val = asm_data->labels_array[asm_data->toks_array[i].val];
        }
    }

    return 0;
}

int check_toks_arr(struct token * tok_arr, size_t num_of_toks, asm_data asm_data)
{
    assert(tok_arr);

    char return_flag = 0;

    for (size_t i = 1; i < num_of_toks; i++)
    {
        if (tok_arr[i-1].tok_type == NUM && tok_arr[i].tok_type == NUM)
        {
            printf("Error: two numbers in a row in line: %lu\n", tok_arr[i].line);
            return_flag = 1;
        }

        if (tok_arr[i-1].tok_type == CMD && tok_arr[i-1].cmd_type == PUSH && tok_arr[i].tok_type != NUM)
        {
            printf("Error: push without argument in line: %lu\n", tok_arr[i-1].line);
            return_flag = 1;
        }

        if (tok_arr[i-1].tok_type == CMD && tok_arr[i-1].cmd_type != PUSH && tok_arr[i].tok_type == NUM)
        {
            printf("Error: too many arguments in line: %lu\n", tok_arr[i-1].line);
            return_flag = 1;
        }

        if (tok_arr[i-1].tok_type == CMD && is_jump(tok_arr[i-1].cmd_type) && tok_arr[i].tok_type != LABEL_JMP)
        {
            printf("Error: jump without argument in line: %lu\n", tok_arr[i-1].line);
            return_flag = 1;
        }

        if (tok_arr[i].tok_type == LABEL_JMP && tok_arr[i].val == -1)
        {
            printf("Error: undeclaired label %s in line %lu\n", tok_arr[i].text, tok_arr[i].line);
            return_flag = 1;
        }

        if (tok_arr[i-1].tok_type == CMD && tok_arr[i-1].cmd_type == POP && tok_arr[i].tok_type != REGISTER)
        {
            printf("Error: pop without argument in line: %lu\n", tok_arr[i-1].line);
            return_flag = 1;
        }

        if (tok_arr[i-1].tok_type == CMD && tok_arr[i-1].cmd_type == PUSHR && tok_arr[i].tok_type != REGISTER)
        {
            printf("Error: pushr without argument in line: %lu\n", tok_arr[i-1].line);
            return_flag = 1;
        }
    }

    if (tok_arr[0].tok_type == NUM) // убрать ошибку с hlt, он может использоваться в неск. частях кода
    {
        printf("Error: number can't be first command in line: %lu\n", tok_arr[0].line);
        return_flag = 1;
    }

    for (int i = 0; i < NUM_OF_LABELS; i++)
    {
        if (asm_data.labels_count_array[i] > 1)
        {
            printf("Error: too many labels :%d\n", i);
            return_flag = 1;
        }
    }

    return return_flag;
}

int get_out_arr_len(token * tok_arr, size_t arr_len)
{
    int count = 0;
    for (size_t i = 0; i < arr_len; i++)
    {
        if (tok_arr[i].tok_type != LABEL) count++;
    }

    return count;
}