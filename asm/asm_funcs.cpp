#include "asm_header.h"

int run_asm(FILE * in_stream, FILE * out_stream)
{
    assert(in_stream);
    assert(out_stream);

    fseek(in_stream, 0L, SEEK_END);
    size_t num_of_letters = (size_t) ftell(in_stream);
    rewind(in_stream);

    char * prog_text = (char *) calloc(num_of_letters + 1, sizeof(char));
    fread(prog_text, sizeof(char), num_of_letters, in_stream);
    fclose(in_stream);

    size_t num_of_cmds = getnum_of_cmds(prog_text, num_of_letters);
    //printf("n of cmds = %lu\n", num_of_cmds);
    struct token * toks = (struct token *) calloc (num_of_cmds, sizeof(struct token));

    get_tokens(toks, num_of_cmds, prog_text);

    /*for (size_t i = 0; i < num_of_cmds; i++)
    {
        printf("%s, line - %lu\n", toks[i].text, toks[i].line); 
    }*/

    char flag = 1;

    for (size_t i = 0; i < num_of_cmds; i++)
    {
        if (tok_info_init(&toks[i]))
        {
            flag = 0;
        }
    }

    if (check_code(toks, num_of_cmds))
        flag = 0;

    if (flag)
    {
        elem * out_array = (elem *) calloc (num_of_cmds, sizeof(elem));

        for (size_t i = 0; i < num_of_cmds; i++)
        {
            if (toks[i].type != NUM)
            {
                out_array[i] = toks[i].type;
            }
            else
            {
                out_array[i] = toks[i].val;
            }
        }

        fwrite(out_array, sizeof(int), num_of_cmds, out_stream);

        /*for (size_t i = 0; i < num_of_cmds; i++)
        {
            printf("%d\n", out_array[i]);
        }*/

        free(out_array);
    }

    fclose(out_stream);
    free(prog_text);
    free(toks);

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
        //printf("slashn found\n"); 
    }

    word_ptr[i] = '\0';

    return (word_ptr + i + 1);
}

int check_code(struct token * array, size_t num_of_cmds)
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
            printf("Error: commands after hlt\n");
            return_flag = 1;
        }

        if (array[i].type == 0)
        {
            hlt_flag = 0;
        }
    }

    if (array[0].type == NUM)
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

int tok_info_init(struct token * tkn)
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

    printf("Error: syntax error in line %lu: %s\n",tkn->line, tkn->text);
    return 1;
}

int check_tkn_for_num(char * str)
{
    assert(str);

    //printf("enter check_tkn_for_num. str = %s\n", str);
    for (int i = 0; str[i]; i++)
    {
        if (!isdigit(str[i]))
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

    if (!strcmp(str, "pop"))
    {
        return POP;
    }

    if (!strcmp(str, "out"))
    {
        return OUT;
    }

    return ERROR;
}

char * getptr_toks_zero(char * prog_text)
{
    assert(prog_text);

    int i = 0;
    while (!isalpha(prog_text[i]))
        i++;
    
    return prog_text + i;
}

size_t getnum_of_cmds(char * prog_text, size_t num)
{
    assert(prog_text);

    size_t i = 0;
    if (isalnum(prog_text[0])) 
        i++;

    for (size_t k = 1; k < num; k++)
    {
        if (isspace(prog_text[k - 1]) && isalnum(prog_text[k])) 
            i++;
    }

    return i;
}