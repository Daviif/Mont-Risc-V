#ifndef PSEUDO_INSTRUCTION_H
#define PSEUDO_INSTRUCTION_H

#include "leitura.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPANSOES 2

typedef struct{
    AnL instrucoes[MAX_EXPANSOES];
    int count;
} ExpansaoResultado;

typedef struct {
    const char *nome;
    void (*traduzir)(AnL*); // Ponteiro para a função de tradução
} PseudoTradutor;

ExpansaoResultado expandirLinha(AnL *resultadoAnL);

#endif