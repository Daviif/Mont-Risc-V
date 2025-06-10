#ifndef TABELA_INSTRUCOES_H
#define TABELA_INSTRUCOES_H

#define MAX_INSTRU 40

typedef enum {
    R_TYPE, S_TYPE, B_TYPE, I_TYPE, U_TYPE, UJ_TYPE, PS_TYPE, SYS_TYPE
} TipoIns;

typedef struct {
    char nome[10]; //Nome da instrução
    TipoIns tipo;// Tipo de instrução, classificado como números.
    char opcode[8];//Opcode com 7 bits + \0
    char funct3[4];//Function 3, 3 bits + \0
    char funct7[8];//Function 7, 7 bits + \0
    char funct12[13];// Function 12, 12 bits + \0
} Instrucao;

Instrucao *buscar_instrucao(char *nome);

#endif