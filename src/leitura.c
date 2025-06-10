#include "leitura.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int ler(char *arquivo, char linha[MAX_LINHAS][TAM_LINHAS]){
    FILE *arq = fopen(arquivo, "r");
     if(arq == NULL){
        printf("Erro ao abrir o arquivo '%s'.\n", arquivo);
        return 0;
    }
    int i = 0;
    while (i < MAX_LINHAS && fgets(linha[i], TAM_LINHAS, arq) != NULL){
        linha[i][strcspn(linha[i], "\n\r")] = '\0'; // Remove \n e \r
        i++;
    }
    fclose(arq);
    return i;
}

void analisarLin(char *linha, AnL *resultado) {
    memset(resultado, 0, sizeof(AnL));

    char *comment = strchr(linha, '#');
    if (comment) {
        *comment = '\0';
    }

    char copia_linha[TAM_LINHAS];
    strncpy(copia_linha, linha, sizeof(copia_linha));
    copia_linha[sizeof(copia_linha) - 1] = '\0';

    char* token = strtok(copia_linha, " \t");
    if (!token) return;
    strncpy(resultado->instrucao, token, TAM_TOKEN-1);

    char* resto = strtok(NULL, "");
    if (!resto) {
        resultado->qtd_op = 0;
        return;
    }
    resto = trim_whitespace(resto);

    char* paren_inicio = strchr(resto, '(');
    char* paren_fim = strchr(resto, ')');
    char* virgula = strchr(resto, ',');

    if (paren_inicio && paren_fim && virgula && virgula < paren_inicio) {
        token = strtok(resto, ",");
        if(token) strncpy(resultado->operandos[0], trim_whitespace(token), TAM_TOKEN-1);
        
        token = strtok(NULL, "(");
        if(token) strncpy(resultado->operandos[2], trim_whitespace(token), TAM_TOKEN-1); // offset
        
        token = strtok(NULL, ")");
        if(token) strncpy(resultado->operandos[1], trim_whitespace(token), TAM_TOKEN-1); // registrador base
        
        resultado->qtd_op = 3;
    } else {
        int i = 0;
        token = strtok(resto, ", \t");
        while (token != NULL && i < MAX_TOKEN) {
            strncpy(resultado->operandos[i], token, TAM_TOKEN-1);
            i++;
            token = strtok(NULL, ", \t");
        }
        resultado->qtd_op = i;
    }
}

char *trim_whitespace(char *str) {
    if (!str) return str;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    
    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}