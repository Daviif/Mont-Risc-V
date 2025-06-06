#include <stdio.h>
#include "compilador.c"


int main(){
    char linha[MAX_LINHAS][TAM_LINHAS];

   

    int ler = le("entrada.asm", linha);

    if(ler == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
   
    for (int i = 0; i < ler; i++) {
        AnL resultado;
        analisarLin(linha[i], &resultado);

        printf("Linha %d: %s\n", i + 1, linha[i]);
        printf("Instrução: %s | Operandos: ", resultado.instrucao);
        for (int j = 0; j < resultado.qtd_op; j++) {
            printf("%s ", resultado.operandos[j]);
        }
        printf("\n");

        Instrucao* inst = buscar_instrucao(resultado.instrucao);
        if (inst) {
            printf("-> Tipo: %d | Opcode: %s | Funct3: %s | Funct7: %s\n",
                inst->tipo, inst->opcode, inst->funct3,
                inst->tipo == R_TYPE ? inst->funct7 : "-");
            
            char binario[33];
            if(montar(&resultado, inst, binario)){
                printf("-> Código binário: %s\n\n", binario);
            } 
            else {
                printf("-> Erro ao montar instrução!\n\n");
            }
        } else {
            printf("-> Instrução desconhecida!\n\n");
        }

        

    }



    return 0;
}
