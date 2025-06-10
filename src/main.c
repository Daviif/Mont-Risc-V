#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leitura.h"
#include "tabela_instrucoes.h"
#include "montador.h"
#include "tabela_simbolos.h"
#include "pseudo_instruction.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc == 3 || argc > 4) {
        fprintf(stderr, "Use: %s <arquivo_entrada.asm> [-o <arquivo_saida>]\n", argv[0]);
        return 1;
    }

    char *inputFile = argv[1];
    char *outputFile = NULL;
    FILE *outputStream = stdout;

    if (argc == 4) {
        if (strcmp(argv[2], "-o") == 0) {
            outputFile = argv[3];
            outputStream = fopen(outputFile, "w");
            if (outputStream == NULL) {
                perror("Erro ao abrir arquivo de saida");
                return 1;
            }
        } else {
            fprintf(stderr, "Uso: %s <arquivo_entrada.asm> [-o <arquivo_saida>]\n", argv[0]);
            if (outputStream != stdout) fclose(outputStream);
            return 1;
        }
    }

    char linhas[MAX_LINHAS][TAM_LINHAS];
    int num_linhas = ler(inputFile, linhas);
    if (num_linhas == 0) {
        fprintf(stderr, "Arquivo de entrada vazio ou nao encontrado.\n");
        if (outputStream != stdout) fclose(outputStream);
        return 1;
    }

    Label tabela_simbolos[MAX_LABELS];
    int num_labels = primeira_passagem(num_linhas, linhas, tabela_simbolos);

    int endereco_atual = 0;
    for (int i = 0; i < num_linhas; i++) {
        char linha_original[TAM_LINHAS];
        strcpy(linha_original, linhas[i]);
        
        char* instrucao_inicio = linha_original;

        // -------------------------------------------------------------------
        // LÓGICA CORRIGIDA PARA PROCESSAR RÓTULOS E COMENTÁRIOS
        // -------------------------------------------------------------------
        char* comment_ptr = strchr(instrucao_inicio, '#');
        char* colon_ptr = strchr(instrucao_inicio, ':');

        // Um ':' só é um rótulo se ele existir E vier ANTES de um '#' (ou se não houver '#')
        if (colon_ptr != NULL && (comment_ptr == NULL || colon_ptr < comment_ptr)) {
            instrucao_inicio = colon_ptr + 1;
        }
        // -------------------------------------------------------------------

        char *linha_atual = trim_whitespace(instrucao_inicio);
        
        if (linha_atual[0] == '\0' || linha_atual[0] == '#') {
            continue; 
        }

        AnL resultado = {0};
        analisarLin(linha_atual, &resultado);

        if (resultado.instrucao[0] == '\0') {
            continue;
        }
        
        ExpansaoResultado expansao = expandirLinha(&resultado);

        for (int j = 0; j < expansao.count; j++) {
            AnL* resultado_final = &expansao.instrucoes[j];
            Instrucao* inst = buscar_instrucao(resultado_final->instrucao);
            
            if (inst) {
                char binario[33];
                if (montar(resultado_final, inst, binario, endereco_atual, tabela_simbolos, num_labels)) {
                    fprintf(outputStream, "%s\n", binario);
                } else {
                    fprintf(stderr, "Erro de montagem na linha %d: %s\n", i + 1, linhas[i]);
                }
            } else {
                fprintf(stderr, "Instrucao desconhecida na linha %d: %s\n", i + 1, resultado_final->instrucao);
            }
            endereco_atual += 4;
        }
    }

    if (outputStream != stdout) {
        fclose(outputStream);
        printf("Codigo binario salvo em %s\n", outputFile);
    }

    return 0;
}