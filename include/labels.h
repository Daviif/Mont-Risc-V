#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include "leitura.h"

#define MAX_LABELS 50
#define TAM_LABEL 50

typedef struct {
    char nome[TAM_LABEL];
    int endereco;
} Label;

int primeira_passagem(int num_linhas, char linhas[MAX_LINHAS][TAM_LINHAS], Label tabela_simbolos[MAX_LABELS]);

int buscar_endereco_label(char* nome,Label* tabela_simbolos, int num_labels);

#endif