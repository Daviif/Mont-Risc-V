#ifndef TABELA_INSTRUCOES_H
#define TABELA_INSTRUCOES_H

#define MAX_INSTRU 40

typedef enum {
    R_TYPE, S_TYPE, B_TYPE, I_TYPE, U_TYPE, UJ_TYPE, SYS_TYPE
} TipoIns;

typedef struct {
    char nome[10];
    TipoIns tipo;
    char opcode[8];
    char funct3[4];
    char funct7[8];
    char funct12[13];
} Instrucao;

Instrucao *buscar_instrucao(char *nome);

#endif