#include <stdio.h>
#include "compilador.h"
#include "pseudo_instruction.h"


int main(int argc, char *argv[]){
   if(argc < 2 || argc == 3 || argc > 4){
        fprintf(stderr, "Use: %s <arquivo_entrada.asm> [-o <arquivo_saida>]\n", argv[0]);
        return 1;
   } 

    char *inputFile = argv[1];
    char *outputFile = NULL;
    FILE *outputStream = stdout;

    if(argc == 4){
        if(strcmp(argv[2], "-o") == 0){
            outputFile = argv[3];
            outputStream = fopen(outputFile, "w");
            if(outputStream == NULL){
                perror("Erro ao abrir arquivo de saida");
                return 1;
            }
        }
        else{
            fprintf(stderr, "Uso: %s <arquivo_entrada.asm> [-o <arquivo_saida]\n", argv[0]);
            return 1;
        }
    }

    char linha[MAX_LINHAS][TAM_LINHAS];
    int num_linhas = ler(inputFile, linha);

    if(num_linhas == 0){
        if(outputStream != stdout){
            fclose(outputStream);
        }
        return 1;
    }
   
    for (int i = 0; i < num_linhas; i++) {
        if(linha[i][0] == '\0' || linha[i][0] == '#'){
            continue;
        }

        AnL resultado = {0};
        analisarLin(linha[i], &resultado);

        if(resultado.instrucao[0] == '\0'){
            continue; // Linha vazia ou sem instrução
        }

        ExpansaoResultado expansao = expandirLinha(&resultado);

         for (int j = 0; j < expansao.count; j++) {
            AnL* resultado_final = &expansao.instrucoes[j];

            Instrucao* inst = buscar_instrucao(resultado_final->instrucao);
            if (inst) {
                char binario[33];
                if(montar(resultado_final, inst, binario)){
                    fprintf(outputStream, "%s\n", binario);
                } 
                else {
                    fprintf(stderr, "Erro de sintaxe na linha %d (expansao %d): %s\n", i+1, j+1, linha[i]);
                }
            } else {
                fprintf(stderr, "-> Instrução desconhecida na linha %d: %s!\n", i+1, resultado_final->instrucao);
            }
        }
    }

    if(outputStream != stdout) {
            fclose(outputStream);
            printf("Codigo binario salvo em %s\n", outputFile);
        }
    
        return 0;
}
