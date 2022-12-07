#include "asm_header.h"

int run_asm(FILE * in_stream, FILE * out_stream)
{
    assert(in_stream);
    assert(out_stream);

    struct asm_info_components ass = {};
    asm_ctor(in_stream, &ass);

    char flag = 1;

    asm_info_init(&ass, &flag);
    asm_info_init(&ass, &flag); // как сделать обработку ошибок, когда мы ставим две одинаковые метки?

    if (check_code(ass.toks_array, ass.num_of_cmds)) //доработать чек код
    {
        flag = 0;
    }

    if (flag)
    {
        ass.out_array = (elem *) calloc (ass.num_of_cmds, sizeof(elem));

        size_t minus_count = 0;
        for (size_t i = 0; i < ass.num_of_cmds; i++)
        {
            if (ass.toks_array[i].type == NUM)
            {
                ass.out_array[i - minus_count] = ass.toks_array[i].val;
            }
            if (ass.toks_array[i].type != NUM && ass.toks_array[i].type != LABEL)
            {
                ass.out_array[i - minus_count] = ass.toks_array[i].type;
            }
            if (ass.toks_array[i].type == LABEL)
            {
                if (i == 0 || ass.toks_array[i-1].type != JUMP)
                {
                    minus_count++;
                    continue;
                }
                else if (ass.toks_array[i-1].type == JUMP)
                {
                    ass.out_array[i - minus_count] = ass.toks_array[i].val;
                }
            }
        }

        fwrite(ass.out_array, sizeof(int), ass.num_of_cmds, out_stream);

        for (size_t i = 0; i < ass.num_of_cmds; i++)
        {
            printf("%d\n", ass.out_array[i]);
        }

        printf("Compilation OK\n");
        free(ass.out_array);
    }
    else
    {
        printf("Compilation error\n");
    }

    fclose(out_stream);
    free(ass.prog_text);
    free(ass.toks_array);

    asm_dtor(&ass);

    return 0;
}

int asm_dtor(struct asm_info_components * ass)
{
    assert(ass);

    free(ass->labels_array);

    ass->num_of_cmds = 0;
    ass->num_of_letters = 0;
    ass->prog_text = NULL;
    
    return 0;
}

int asm_info_init(asm_info_components * ass, char * flag)
{
    assert(ass);

    for (size_t i = 0; i < ass->num_of_cmds; i++)
    {
        if (tok_info_init(&ass->toks_array[i], ass, i))
        {
            *flag = 0;
        }
    }

    return 0;
}

int tok_info_init(struct token * tkn, struct asm_info_components * ass, size_t itteration)
{
    assert(tkn);
    //printf("\nEnter the tok_info_init\n");

    if (check_tkn_for_string(tkn->text))
    {
        tkn->type = get_type_cmd(tkn->text);
        //printf("%s cmd_type - %d\n", tkn->text, tkn->type);

        if (tkn->type == ERROR)
        {
            printf("Error: unknown command in line %lu: %s\n", tkn->line, tkn->text);
            return 1;
        }

        return 0;
    }

    if (check_tkn_for_num(tkn->text))
    {
        sscanf(tkn->text, "%d", &tkn->val);
        tkn->type = NUM;
        //printf("num = %d\n", tkn->val);

        return 0;
    }

    if (check_tkn_for_label(tkn->text))
    {
        tkn->type = LABEL;
        sscanf(tkn->text + 1, "%d", &tkn->val);

        if (itteration == 0 || ass->toks_array[itteration - 1].type != JUMP)
        {
            ass->labels_array[tkn->val] = (int) itteration;
        }
        else if (ass->toks_array[itteration - 1].type == JUMP)
        {
            if (ass->labels_array[tkn->val] != FREE)
            {
                tkn->val = ass->labels_array[tkn->val];
            }
        }

        return 0;
    }

    printf("Error: syntax error in line %lu: %s\n",tkn->line, tkn->text);
    tkn->type = ERROR;
    return 1;
}

int asm_ctor(FILE * in_stream, asm_info_components * ass)
{
    assert(in_stream);
    assert(ass);

    fseek(in_stream, 0L, SEEK_END);
    ass->num_of_letters = (size_t) ftell(in_stream);
    rewind(in_stream);

    ass->prog_text   = (char *) calloc(ass->num_of_letters + 1, sizeof(char));
    fread(ass->prog_text, sizeof(char), ass->num_of_letters, in_stream);
    fclose(in_stream);

    ass->num_of_cmds = getnum_of_cmds(ass->prog_text, ass->num_of_letters);
    ass->toks_array  = (struct token *) calloc (ass->num_of_cmds, sizeof(struct token));

    get_tokens(ass->toks_array, ass->num_of_cmds, ass->prog_text);

    /*for (size_t i = 0; i < ass->num_of_cmds; i++)
    {
        printf("%s, line - %lu\n", ass->toks_array[i].text, ass->toks_array[i].line); 
    }*/

    ass->labels_array = (int *) calloc (10, sizeof(int));
    for (int i = 0; i < NUM_OF_LABELS; i++)
    {
        ass->labels_array[i] = FREE;
    }

    return 0;
}

int get_tokens(token * toks, size_t num_of_cmds, char * prog_text)
{
    assert(toks);
    assert(prog_text);

    size_t line = 1;
    char * end_of_word = prog_text;

    for (size_t i = 0; i < num_of_cmds; i++)
    {
        toks[i].text = skip_spaces(end_of_word, &line);
        toks[i].line = line;

        if (i < num_of_cmds - 1)
            end_of_word  = get_end_of_word(toks[i].text, &line);
    }

    put_null_to_end_of_last_token(toks[num_of_cmds - 1].text);

    return 0;
}

int put_null_to_end_of_last_token(char * str_ptr)
{
    int i = 0;
    while (isgraph(str_ptr[i])) i++;
    str_ptr[i] = 0;

    return 0;
}

char * skip_spaces(char * start_ptr, size_t * p_line)
{
    assert(start_ptr);
    assert(p_line);

    int i = 0;

    while (isspace(start_ptr[i]))
    {
        if (start_ptr[i] == '\n')
        {
            *p_line += 1;
            //printf("slashn found\n"); 
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
        //printf("slashn found\n"); check_code
    }

    word_ptr[i] = '\0';

    return (word_ptr + i + 1);
}

int check_code(struct token * array, size_t num_of_cmds) // переименовать array
{
    assert(array);

    char hlt_flag = 1;
    char return_flag = 0;

    for (size_t i = 1; i < num_of_cmds; i++)
    {
        if (array[i-1].type == NUM && array[i].type == NUM)
        {
            printf("Error: two numbers in a row in line: %lu\n", array[i].line);
            return_flag = 1;
        }

        if (array[i-1].type == PUSH && array[i].type != NUM)
        {
            printf("Error: push without argument in line: %lu\n", array[i-1].line);
            return_flag = 1;
        }

        if (array[i-1].type != PUSH && array[i-1].type != NUM && array[i].type == NUM)
        {
            printf("Error: too many arguments in line: %lu\n", array[i-1].line);
            return_flag = 1;
        }

        if (array[i-1].type == HLT)
        {
            printf("Error: commands after hlt: %s\n", array[i].text);
            return_flag = 1;
        }

        if (array[i].type == 0)
        {
            hlt_flag = 0;
        }
    }

    if (array[0].type == NUM) // убрать ошибку с hlt, он может использоваться в неск. частях кода
    {
        printf("Error: number can't be first command in line: %lu\n", array[0].line);
        return_flag = 1;
    }

    if (hlt_flag)
    {
        printf("There isn't hlt command\n");
        return 1;
    }

    return return_flag;
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

int check_tkn_for_num(char * str)
{
    assert(str);

    int i = 0;
    //printf("enter check_tkn_for_num. str = %s\n", str);
    for (; str[i]; i++)
    {
        if (!(isdigit(str[i]) || str[i] == '-' || str[i] == '+'))
            return 0;
    }

    if (str[0] == '-' && i == 1)
    {
        return 0;
    }

    return 1;
}

int check_tkn_for_string(char * str)
{
    assert(str);
    
    //printf("enter check_tkn_for_string. str = %s\n", str);
    for (int i = 0; str[i] != '\0'; i++)
    {
        //printf("%c\n", str[i]);

        if (!isalpha(str[i]))
        {
            return 0;
        }
    }

    return 1;
}

int get_type_cmd(char * str)
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

    if (!strcmp(str, "popr"))
    {
        return POPR;
    }

    if (!strcmp(str, "out"))
    {
        return OUT;
    }

    if (!strcmp(str, "jump"))
    {
        return JUMP;
    }

    if (!strcmp(str, "pushr"))
    {
        return PUSHR;
    }

    return ERROR;
}

char * getptr_toks_zero(char * prog_text)
{
    assert(prog_text);

    int i = 0;
    while (!isgraph(prog_text[i]))
        i++;
    
    return prog_text + i;
}

size_t getnum_of_cmds(char * prog_text, size_t num)
{
    assert(prog_text);

    size_t i = 0;
    if (isgraph(prog_text[0])) 
        i++;

    for (size_t k = 1; k < num; k++)
    {
        if (isspace(prog_text[k - 1]) && isgraph(prog_text[k])) 
            i++;
    }

    return i;
}
