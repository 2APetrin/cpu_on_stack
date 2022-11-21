#include "asm_header.h"

int assembler(FILE * in, FILE * out)
{
    struct my_code code = {};
    
    code_ctor (&code, in); //инициализация кода
    write_code(&code, out);
    
    code_dtor (&code);
    
    fclose(out);
    return 0;
}


int write_code(my_code * code, FILE * out)
{
    for (size_t i = 0; i < code->num_of_lines; i++)
    {
        if ( print_code(code->strings[i], out, i) )
            break;
    }

    return 0;
}


int print_code(char * cmd_ptr, FILE * out, size_t line_n)
{
    //printf("%s\n", cmd_ptr);
    char command[10] = "";
    int len = 0;
    sscanf(cmd_ptr, "%s%n", command, &len); //сканит только буквы без пробелов + выдает кол-во считанных символов 
    //printf("символов считано - %d\n", len);

    if (!is_blank(cmd_ptr)) // проверка на пустую строку
    {
        //printf("blank worked\n");
        return 0;
    }

    if (!strcmp("hlt", command))
    {
        //printf("hlt worked\n");

        cmd_verify(cmd_ptr, line_n, HLT);

        fprintf(out, "%d\n", HLT);
        return 1;
    }

    if (!strcmp("push", command))
    {
        cmd_verify(cmd_ptr, line_n, PUSH);
        //printf("push worked\n");
        int val = 0;
        int val_len = 0;
        sscanf(cmd_ptr + len, "%d%n", &val, &val_len);
        if (val_len == 0)
        {
            printf("Too few arguments in line: %lu\n", line_n + 1);
            abort();
        }
        //printf("push val - %d\n", val);
        fprintf(out, "%d %d\n", PUSH, val);

        return 0;
    }

    if (!strcmp("add", command))
    {
        cmd_verify(cmd_ptr, line_n, ADD);
        fprintf(out, "%d\n", ADD);
        return 0;
    }

    if (!strcmp("sub", command))
    {
        cmd_verify(cmd_ptr, line_n, SUB);
        fprintf(out, "%d\n", SUB);
        return 0;
    }

    if (!strcmp("mul", command))
    {
        cmd_verify(cmd_ptr, line_n, MUL);
        fprintf(out, "%d\n", MUL);
        return 0;
    }

    if (!strcmp("div", command))
    {
        cmd_verify(cmd_ptr, line_n, DIV);
        fprintf(out, "%d\n", DIV);
        return 0;
    }

    if (!strcmp("pop", command))
    {
        cmd_verify(cmd_ptr, line_n, POP);
        fprintf(out, "%d\n", POP);
        return 0;
    }

    if (!strcmp("out", command))
    {
        cmd_verify(cmd_ptr, line_n, OUT);
        fprintf(out, "%d\n", OUT);
        return 0;
    }

    printf("Syntax error: command not found. Line: %lu\n", line_n + 1);
    abort();    

    return 0;
}

void cmd_verify(char * cmd_ptr, size_t line_n, int cmd_type)
{
    //printf("cmd_verify entered\n");
    int i = 1;
    int flag = 0;
    if (cmd_type != PUSH)
    {
        if (isalnum(cmd_ptr[0]))
            flag = 1;
        while (cmd_ptr[i] != 0 && cmd_ptr[i] != '\n')
        {
            if (isspace(cmd_ptr[i - 1]) && isalnum(cmd_ptr[i]))
                flag++;
            if (flag > 1)
            {
                printf("Too many arguments in line: %lu\n", line_n + 1);
                abort();
            }
            i++;
        }
    }
    else
    {
        if (isalnum(cmd_ptr[0]))
            flag = 1;
        while (cmd_ptr[i] != 0 && cmd_ptr[i] != '\n')
        {
            if (isspace(cmd_ptr[i - 1]) && isalnum(cmd_ptr[i]))
                flag++;
            if (flag > 2)
            {
                printf("Too many arguments in line: %lu\n", line_n + 1);
                abort();
            }
            i++;
        }
    }
}


//**********************************************************************************************************


void code_ctor(my_code * code, FILE * stream)
{
    assert(code   != NULL);
    assert(stream != NULL);

    fseek(stream, 0L, SEEK_END);
    code->num_of_symbols = (size_t) ftell(stream);
    rewind(stream);

    code->text = (char *) calloc(code->num_of_symbols + 1, sizeof(char));
    fread(code->text, sizeof(char), code->num_of_symbols, stream);
    fclose(stream);
    code->text[code->num_of_symbols] = 0;

    code->num_of_lines = getnum_of_lines(code);
    code->strings = (char **) calloc(code->num_of_lines, sizeof(char *));
    assert(code->strings != NULL);

    get_indexes(code);
    make_n_o(code);
}

size_t getnum_of_lines(struct my_code * cod)
{
    assert(cod != NULL);

    size_t count = 0;
    for (size_t i = 0; i < cod->num_of_symbols; i++)
    {
        if (cod->text[i] == '\n' /*|| cod->text[i] == 0*/)
            count++;
    }

    printf("\nnum of lines - %lu\n\n", count);
    return count;
}

void get_indexes(my_code * code)
{
    assert(code != NULL);

    code->strings[0] = code->text;

    for (size_t i = 1; i < code->num_of_lines; i++)
    {
        code->strings[i] = get_next_ptr(code->strings[i - 1]);
    }
}

char * get_next_ptr(char * pr_ptr)
{
    assert(pr_ptr != NULL);

    int i = 0;
    while (*(pr_ptr + i) != '\n' && *(pr_ptr + i) != 0)
    {
        i++;
    }

    return (pr_ptr + i + 1);
}

void code_dtor(my_code * code)
{
    assert(code != NULL);

    free(code->text);
    code->text = NULL;
    free(code->strings);
    code->strings = NULL;
    code = NULL;
}

void make_n_o(my_code * code)
{
    for (size_t i = 0; i < code->num_of_symbols + 1; i++)
    {
        if(code->text[i] == '\n')
        {
            code->text[i] = 0;
        }
    }
}

int is_blank(char * ptr)
{
    assert(ptr != NULL);

    int i = 0, ch = *(ptr + i);

    while (ch != 0)
    {
        if (!isblank(*(ptr + i)))
            return 1;
        i++;
        ch = *(ptr + i);
    }

    return 0;
}