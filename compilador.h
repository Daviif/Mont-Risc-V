#ifndef COMPILADOR_H
#define COMPILADOR_H

#include <stdio.h>

// definir o max de linhas e o tamanhos delas
#define MAX_LINHAS 500
#define TAM_LINHAS 100

//definir o max de tokens e o tamanho
#define MAX_TOKEN 5
#define TAM_TOKEN 20

//definir o maximo de instruçoes
#define MAX_INSTRU 7

//estrutura para analisar
typedef struct{
    char instrucao[TAM_TOKEN];
    char operandos[MAX_TOKEN][TAM_TOKEN];
    int qtd_op;
} AnL;


//estrutura das instruções
typedef enum{
    R_TYPE, S_TYPE, SB_TYPE, I_TYPE, UNKNOWN_TYPE
}TipoIns;

typedef struct{
    char nome[10];
    TipoIns tipo;
    char opcode[8];
    char funct3[4];
    char funct7[8]; // Para tipos R, e alguns I
} Instrucao;



//função para ler cada linha separadamente
int ler(char *arquivo, char linha[MAX_LINHAS][TAM_LINHAS]);


//funcao para remover virgulas
void removerVir(char *token);
//funcao para analisar cada linha
void analisarLin(char *linha, AnL *resultado);

//função para saber a instrução respectiva: lh sh sub or andi srl beq
Instrucao *buscar_instrucao(char *nome);

//montador
int montar(AnL *linha, Instrucao *inst, char *saida_binaria);

#endif COMPILADOR_H

