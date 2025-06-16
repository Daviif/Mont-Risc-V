#ifndef _LEITURA_H_
#define _LEITURA_H_

#define MAX_LINHAS 500
#define TAM_LINHAS 100

#define MAX_TOKEN 5
#define TAM_TOKEN 20

typedef struct{
    char instrucao[TAM_TOKEN];
    char operandos[MAX_TOKEN][TAM_TOKEN];
    int qtd_op;
} AnL;

int ler(char *arquivo, char linha[MAX_LINHAS][TAM_LINHAS]);

void analisarLin(char *linha, AnL *resultado);

char *tirar_espaco(char *str);

#endif