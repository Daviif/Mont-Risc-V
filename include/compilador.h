#ifndef COMPILADOR_H
#define COMPILADOR_H

#include <stdio.h>

//Definir o max de linhas e o tamanhos delas
#define MAX_LINHAS 500
#define TAM_LINHAS 100

//Definir o max de tokens e o tamanho
#define MAX_TOKEN 5
#define TAM_TOKEN 20

//Definir o maximo de instruçoes
#define MAX_INSTRU 7

//Estrutura para analisar
typedef struct{
    char instrucao[TAM_TOKEN];
    char operandos[MAX_TOKEN][TAM_TOKEN];
    int qtd_op;
} AnL;


//Enum para os tipos
typedef enum{
    R_TYPE, S_TYPE, SB_TYPE, I_TYPE, UNKNOWN_TYPE
}TipoIns;

//Estrutura das instruções
typedef struct{
    char nome[10];
    TipoIns tipo;
    char opcode[8];
    char funct3[4];
    char funct7[8]; // Para tipos R, e alguns I
} Instrucao;



//Função para ler cada linha separadamente do arquivo de entrada
int ler(char *arquivo, char linha[MAX_LINHAS][TAM_LINHAS]);


//Funcao para remover virgulas
void removerVir(char *token);

//funcao para analisar cada linha
void analisarLin(char *linha, AnL *resultado);

//Função para saber a instrução respectiva: lh sh sub or andi srl beq
Instrucao *buscar_instrucao(char *nome);

//Função do montador
int montar(AnL *linha, Instrucao *inst, char *saida_binaria);

#endif COMPILADOR_H

