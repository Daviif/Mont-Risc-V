#include "../include/compilador.h"
#include "../include/pseudo_instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Função para expandir pseudo-instruções
ExpansaoResultado expandirLinha(AnL *resultadoAnL){
    ExpansaoResultado expansao = {0};

    if(strcmp(resultadoAnL -> instrucao, "li") == 0){
        long imm = strtol(resultadoAnL -> operandos[1], NULL, 0);

        //Se o imediato for grande, converte 'li' em 'lui' e 'addi'
        if(imm > 4095 || imm < -2048){
            int upper = (imm >> 12);
            int lower = imm & 0xFFF;
            if ((lower >> 11) & 1) {
                upper++; // Sign extend
            }
            char upper_str[20], lower_str[20];
            snprintf(upper_str, sizeof(upper_str),"%d", upper);
            snprintf(lower_str, sizeof(lower_str),"%d", lower);

            // 1. Primeira instrução: LUI
            strcpy(expansao.instrucoes[0].instrucao, "lui");
            strcpy(expansao.instrucoes[0].operandos[0], resultadoAnL->operandos[0]); // rd
            strcpy(expansao.instrucoes[0].operandos[1], upper_str);
            expansao.instrucoes[0].qtd_op = 2;

            // 2. Segunda instrução: ADDI
            strcpy(expansao.instrucoes[1].instrucao, "addi");
            strcpy(expansao.instrucoes[1].operandos[0], resultadoAnL->operandos[0]); // rd
            strcpy(expansao.instrucoes[1].operandos[1], resultadoAnL->operandos[0]); // rd
            strcpy(expansao.instrucoes[1].operandos[2], lower_str);
            expansao.instrucoes[1].qtd_op = 3;
            
            expansao.count = 2; // O resultado são DUAS instruções
        } else {
            // Se o imediato for pequeno, 'li' vira 'addi rd, x0, imm'
            strcpy(resultadoAnL->instrucao, "addi");
            strcpy(resultadoAnL->operandos[2], resultadoAnL->operandos[1]);
            strcpy(resultadoAnL->operandos[1], "x0");
            resultadoAnL->qtd_op = 3;
            expansao.instrucoes[0] = *resultadoAnL;
            expansao.count = 1; // O resultado é UMA instrução
        }
    }
    else {
        // Para qualquer outra instrução, apenas copia para a lista de expansão
        expansao.instrucoes[0] = *resultadoAnL;
        expansao.count = 1; // O resultado é UMA instrução
    }
    
    return expansao;
}


void traduzir_mv(AnL *resultado) {
    strcpy(resultado->instrucao, "addi");
    strcpy(resultado->operandos[2], "0");
    resultado->qtd_op = 3;
}

void traduzir_nop(AnL *resultado) {
    strcpy(resultado->instrucao, "addi");
    strcpy(resultado->operandos[0], "x0");
    strcpy(resultado->operandos[1], "x0");
    strcpy(resultado->operandos[2], "0");
    resultado->qtd_op = 3;
}

void traduzir_not(AnL *resultado) {
    strcpy(resultado->instrucao, "xori");
    strcpy(resultado->operandos[2], "-1");
    resultado->qtd_op = 3;
}