#ifndef MONTADOR_H
#define MONTADOR_H

#include "leitura.h"
#include "tabela_instrucoes.h"
#include "labels.h"

typedef struct{
    char *nome;
    int registrador;
}ABI;

int registrador_int(char *reg);

void int_bin(long valor, int bits, char *destino);

// Monta a instrução assembly em binário
int montar(AnL *linha, Instrucao *inst, char *saida_bin, int endereco_atual, Label* tabela_simbolos, int num_labels);

#endif