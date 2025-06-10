#include "../include/pseudo_instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

ExpansaoResultado expandirLinha(AnL *resultadoAnL) {
    ExpansaoResultado expansao = {0};

    // 'li' (Load Immediate)
    if (strcmp(resultadoAnL->instrucao, "li") == 0) {
        long imm = strtol(resultadoAnL->operandos[1], NULL, 0);

        if (imm > 4095 || imm < -2048) { // Expande para LUI + ADDI
            int upper = (imm >> 12);
            int lower = imm & 0xFFF;
            if ((lower >> 11) & 1) {
                upper++; // Ajuste de extensão de sinal
            }
            char upper_str[20], lower_str[20];
            snprintf(upper_str, sizeof(upper_str), "%d", upper);
            snprintf(lower_str, sizeof(lower_str), "%d", lower);

            strcpy(expansao.instrucoes[0].instrucao, "lui");
            strcpy(expansao.instrucoes[0].operandos[0], resultadoAnL->operandos[0]);
            strcpy(expansao.instrucoes[0].operandos[1], upper_str);
            expansao.instrucoes[0].qtd_op = 2;

            strcpy(expansao.instrucoes[1].instrucao, "addi");
            strcpy(expansao.instrucoes[1].operandos[0], resultadoAnL->operandos[0]);
            strcpy(expansao.instrucoes[1].operandos[1], resultadoAnL->operandos[0]);
            strcpy(expansao.instrucoes[1].operandos[2], lower_str);
            expansao.instrucoes[1].qtd_op = 3;
            
            expansao.count = 2;
        } else { // Converte para 'addi rd, x0, imm'
            strcpy(resultadoAnL->instrucao, "addi");
            strcpy(resultadoAnL->operandos[2], resultadoAnL->operandos[1]);
            strcpy(resultadoAnL->operandos[1], "x0");
            resultadoAnL->qtd_op = 3;
            expansao.instrucoes[0] = *resultadoAnL;
            expansao.count = 1;
        }
    }
    // 'mv' (Move) -> addi rd, rs, 0
    else if (strcmp(resultadoAnL->instrucao, "mv") == 0) {
        strcpy(resultadoAnL->instrucao, "addi");
        strcpy(resultadoAnL->operandos[2], "0");
        resultadoAnL->qtd_op = 3;
        expansao.instrucoes[0] = *resultadoAnL;
        expansao.count = 1;
    }
    // 'nop' (No Operation) -> addi x0, x0, 0
    else if (strcmp(resultadoAnL->instrucao, "nop") == 0) {
        strcpy(resultadoAnL->instrucao, "addi");
        strcpy(resultadoAnL->operandos[0], "x0");
        strcpy(resultadoAnL->operandos[1], "x0");
        strcpy(resultadoAnL->operandos[2], "0");
        resultadoAnL->qtd_op = 3;
        expansao.instrucoes[0] = *resultadoAnL;
        expansao.count = 1;
    }
    // 'not' (Bitwise Not) -> xori rd, rs, -1
    else if (strcmp(resultadoAnL->instrucao, "not") == 0) {
        strcpy(resultadoAnL->instrucao, "xori");
        strcpy(resultadoAnL->operandos[2], "-1");
        resultadoAnL->qtd_op = 3;
        expansao.instrucoes[0] = *resultadoAnL;
        expansao.count = 1;
    }
     // 'j' (Jump) -> jal x0, label
    else if (strcmp(resultadoAnL->instrucao, "j") == 0) {
        strcpy(resultadoAnL->instrucao, "jal");
        strcpy(resultadoAnL->operandos[1], resultadoAnL->operandos[0]); // Move label para operando [1]
        strcpy(resultadoAnL->operandos[0], "x0"); // Define rd como x0
        resultadoAnL->qtd_op = 2;
        expansao.instrucoes[0] = *resultadoAnL;
        expansao.count = 1;
    }
    else { // Para qualquer outra instrução, apenas copia
        expansao.instrucoes[0] = *resultadoAnL;
        expansao.count = 1;
    }
    
    return expansao;
}