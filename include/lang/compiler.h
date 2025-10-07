#ifndef COMPILER_H
#define COMPILER_H

typedef struct
{
    char* file_name;
} Compiler;

void compile(char* file_name);
void gen_config(char* src_file);
#endif