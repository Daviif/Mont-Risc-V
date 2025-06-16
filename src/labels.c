 #include "labels.h"
#include "leitura.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *tirar_espaco(char *str); 
void analisarLin(char *linha, AnL *resultado); 


/*Função para coletar as labels.*/
int primeira_passagem(int num_linhas, char linhas[MAX_LINHAS][TAM_LINHAS], Label tabela_simbolos[MAX_LABELS]) {
    int contador_labels = 0;
    int endereco_atual = 0;
    
    for (int i = 0; i < num_linhas; i++) {
        char linha_original[TAM_LINHAS]; 
        strcpy(linha_original, linhas[i]);
        char *linha_limpa = tirar_espaco(linha_original);

        if (linha_limpa[0] == '\0' || linha_limpa[0] == '#') {
            continue;
        }
        
        char *ponteiro_2p = strchr(linha_limpa, ':');
        if (ponteiro_2p) { 
            int tamanho_label = ponteiro_2p - linha_limpa; 
            if (contador_labels < MAX_LABELS) { 
                strncpy(tabela_simbolos[contador_labels].nome, linha_limpa, tamanho_label);
                tabela_simbolos[contador_labels].nome[tamanho_label] = '\0';
                tabela_simbolos[contador_labels].endereco = endereco_atual;
                contador_labels++;
            }
            linha_limpa = tirar_espaco(ponteiro_2p + 1);
        }

        if (strlen(linha_limpa) > 0 && linha_limpa[0] != '#') {
            AnL resultado = {0};
            analisarLin(linha_limpa, &resultado);

            if (strcmp(resultado.instrucao, "li") == 0 && resultado.qtd_op == 2) {
                char* endptr;
                long imm = strtol(resultado.operandos[1], &endptr, 0);
                if (*endptr == '\0') {
                    if (imm > 4095 || imm < -2048) {
                        endereco_atual += 8; // li expande para lui + addi
                    } else {
                        endereco_atual += 4;
                    }
                } else {
                     endereco_atual += 4;
                }
            } else {
                endereco_atual += 4; // Instrução regular
            }
        }
    }
    return contador_labels;
}

int buscar_endereco_label(char *nome, Label *tabela_simbolos, int num_labels) {
    for (int i = 0; i < num_labels; i++) {
        if (strcmp(tabela_simbolos[i].nome, nome) == 0) {
            return tabela_simbolos[i].endereco;
        }
    }
    return -1; // Rótulo não encontrado
}