#include <stdio.h>
#include "montador.c"


int main(){

    FILE *arq;
    char linha[100];

    arq = fopen("entrada.asm", "r");

    if(arq == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    while (fgets(linha, sizeof(linha), arq)) {
        printf("%s", linha);  // mostra a linha na tela
    }


    fclose(arq);

    return 0;
}
