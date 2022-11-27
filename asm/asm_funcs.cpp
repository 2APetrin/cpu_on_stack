#include "asm_header.h"

int run_asm(FILE * in_stream, FILE * out_stream)
{
    fseek(in_stream, 0L, SEEK_END);
    size_t num_of_letters = (size_t) ftell(in_stream);
    rewind(in_stream);

    char * prog_text = (char *) calloc(num_of_letters + 1, sizeof(char));
    fread(prog_text, sizeof(char), num_of_letters, in_stream);
    fclose(in_stream);

    size_t num_of_cmds = getnum_of_cmds(prog_text, num_of_letters);
    //printf("n of cmds = %lu\n", num_of_cmds);
    struct token * toks = (struct token *) calloc (num_of_cmds, sizeof(struct token));

    char * tok = strtok(prog_text, " \n");
    toks[0].text = tok;     /*getptr_toks_zero(prog_text);*/

    //printf("%s\n", toks[0].text);

    for (int i = 1; tok; i++)
    {
        tok = strtok(NULL, " \n");
        
        if (!tok)
            continue;
        
        toks[i].text = tok;
        //printf("%s\n", tok);
    }

    for (size_t i = 0; i < num_of_cmds; i++)
    {
        //printf("\nitteration %lu:\n", i);
        //printf("%s\n", toks[i].text);
        tok_info_init(&toks[i]);
    }

    check_code(toks, num_of_cmds);

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

    /*for (size_t i = 0; i < num_of_cmds; i++)
    {
        printf("%d\n", out_array[i]);
    }*/

    fwrite(out_array, sizeof(int), num_of_cmds, out_stream);

    free(out_array);
    free(toks);

    return 0;
}

int check_code(struct token * array, size_t num_of_cmds)
{
    char flag = 1;

    for (size_t i = 1; i < num_of_cmds; i++)
    {
        if (array[i-1].type == NUM && array[i].type == NUM)
        {
            printf("Error: two numbers in a row\n");
            abort();
        }

        if (array[i-1].type == PUSH && array[i].type != NUM)
        {
            printf("Error: push without argument\n");
            abort();
        }

        if (array[i-1].type != PUSH && array[i-1].type != NUM && array[i].type == NUM)
        {
            printf("Error: too many arguments in %s\n", array[i-1].text);
            abort();
        }

        if (array[i-1].type == HLT)
        {
            printf("Error: commands after hlt\n");
            abort();
        }

        if (array[i].type == 0)
        {
            flag = 0;
        }
    }

    if (flag)
    {
        printf("There isn't hlt command\n");
        abort();
    }

    return 0;
}

int tok_info_init(struct token * tkn)
{
    //printf("\nEnter the tok_info_init\n");

    if (check_tkn_for_string(tkn->text))
    {
        tkn->type = get_type_cmd(tkn->text);
        //printf("%s cmd_type - %d\n", tkn->text, tkn->type);

        if (tkn->type == ERROR)
        {
            printf("Unknown command: %s\n", tkn->text);
            abort();
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

    printf("Syntax error: %s\n", tkn->text);
    abort();
}

int check_tkn_for_num(char * str)
{
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
    int i = 0;
    while (!isalpha(prog_text[i]))
        i++;
    
    return prog_text + i;
}

size_t getnum_of_cmds(char * prog_text, size_t num)
{
    size_t i = 0;
    if (isalpha(prog_text[0])) 
        i++;

    for (size_t k = 1; k < num; k++)
    {
        if (isspace(prog_text[k - 1]) && isalnum(prog_text[k])) 
            i++;
    }

    return i;
}