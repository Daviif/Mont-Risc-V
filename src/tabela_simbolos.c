 #include "tabela_simbolos.h"
#include "leitura.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Definimos aqui, os prótotipos de funções. Basicamente prometemos que a função existe para o compilador.*/
char *trim_whitespace(char *str); 
void analisarLin(char *linha, AnL *resultado); 


/*Função para coletar as labels.*/
int primeira_passagem(int num_linhas, char linhas[MAX_LINHAS][TAM_LINHAS], Label tabela_simbolos[MAX_LABELS]) {
    int contador_labels = 0; //Contador.
    int endereco_atual = 0; //Endereço atual

    for (int i = 0; i < num_linhas; i++) {
        char linha_original[TAM_LINHAS]; //Linha original, com o tamanho máximo de linhas.
        strcpy(linha_original, linhas[i]);//Copiamos o que tem em linhas[i], para linha original.
        char *linha_limpa = trim_whitespace(linha_original);//Tiramos os espaços da linha original e guardamos em uma Linha Limpa

        if (linha_limpa[0] == '\0' || linha_limpa[0] == '#') { //Se o primeiro caracter estiver vazio ou for #(comentário)
            continue; //Então pula, continua pra próxima linha
        }
        
        char *ponteiro_colon = strchr(linha_limpa, ':'); //Ponteiro para procurar dois pontos.
        if (ponteiro_colon) { //Se ponteiro_colon, retornar um ponteiro, quer dizer que encontrou os ':'.
            int tamanho_label = ponteiro_colon - linha_limpa; //O tamanho da label, será o ponteiro - a linha limpa.
            if (contador_labels < MAX_LABELS) { //Se o contador for menor do que a definição máxima
                strncpy(tabela_simbolos[contador_labels].nome, linha_limpa, tamanho_label);//Copiar o nome da tabela_simbolos, na posição do contador
                tabela_simbolos[contador_labels].nome[tamanho_label] = '\0';
                tabela_simbolos[contador_labels].endereco = endereco_atual;
                contador_labels++;
            }
            linha_limpa = trim_whitespace(ponteiro_colon + 1);
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