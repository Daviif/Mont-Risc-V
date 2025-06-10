#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "montador.h"


ABI tabela_ABI[] = {
    {"zero", 0}, {"ra", 1}, {"sp", 2}, {"gp", 3}, {"tp", 4},
    {"t0", 5}, {"t1", 6}, {"t2", 7}, {"s0", 8},{"fp", 8}, {"s1", 9},
    {"a0", 10}, {"a1", 11}, {"a2", 12}, {"a3", 13}, {"a4", 14},
    {"a5", 15}, {"a6", 16}, {"a7", 17}, {"s2", 18}, {"s3", 19},
    {"s4", 20}, {"s5", 21}, {"s6", 22}, {"s7", 23}, {"s8", 24},
    {"s9", 25}, {"s10", 26}, {"s11", 27}, {"t3", 28}, {"t4", 29},
    {"t5", 30}, {"t6", 31}
};

int num_nomes_ABI = sizeof(tabela_ABI) / sizeof(tabela_ABI[0]); 

int registrador_int(char *reg){
    for (int i = 0; i < num_nomes_ABI; i++){
        if(strcmp(reg, tabela_ABI[i].nome) == 0) return tabela_ABI[i].registrador;
    }
    
    if(reg[0] != 'x' || !isdigit(reg[1])) return -1;
    
    return atoi(&reg[1]);
}

void int_bin(long valor, int bits, char *destino){
    for(int i = bits - 1; i >= 0; i--){
        destino[bits - 1 - i] = ((valor >> i) & 1) + '0';
    }
    destino[bits] = '\0';
}

int montar(AnL *linha, Instrucao *inst, char *saida_binaria, int endereco_atual, Label *tabela_simbolos, int num_labels) {
    char *endptr;

    if (inst->tipo == R_TYPE && linha->qtd_op == 3) {
        int rd = registrador_int(linha->operandos[0]);
        int rs1 = registrador_int(linha->operandos[1]);
        int rs2 = registrador_int(linha->operandos[2]);
        if (rd < 0 || rs1 < 0 || rs2 < 0) return 0;

        char rd_bin[6], rs1_bin[6], rs2_bin[6];
        int_bin(rd, 5, rd_bin); int_bin(rs1, 5, rs1_bin); int_bin(rs2, 5, rs2_bin);

        snprintf(saida_binaria, 33, "%s%s%s%s%s%s", inst->funct7, rs2_bin, rs1_bin, inst->funct3, rd_bin, inst->opcode);
        return 1;
    } 
    else if (inst->tipo == I_TYPE && linha->qtd_op == 3) {
        int rd = registrador_int(linha->operandos[0]);
        int rs1 = registrador_int(linha->operandos[1]);
        long imm = strtol(linha->operandos[2], &endptr, 0);
        if (rd < 0 || rs1 < 0 || (*endptr != '\0' && !isspace(*endptr))) return 0;

        char rd_bin[6], rs1_bin[6], imm_bin[13];
        int_bin(rd, 5, rd_bin); int_bin(rs1, 5, rs1_bin); int_bin(imm, 12, imm_bin);

        snprintf(saida_binaria, 33, "%s%s%s%s%s", imm_bin, rs1_bin, inst->funct3, rd_bin, inst->opcode);
        return 1;
    } 
    else if (inst->tipo == S_TYPE && linha->qtd_op == 3) {
        int rs2 = registrador_int(linha->operandos[0]);
        int rs1 = registrador_int(linha->operandos[1]);
        long imm = strtol(linha->operandos[2], &endptr, 0);
        if (rs1 < 0 || rs2 < 0 || (*endptr != '\0' && !isspace(*endptr))) return 0;
        
        char imm_bin[13];
        int_bin(imm, 12, imm_bin);
        char imm_11_5[8], imm_4_0[6];
        strncpy(imm_11_5, imm_bin, 7); imm_11_5[7] = '\0';
        strncpy(imm_4_0, &imm_bin[7], 5); imm_4_0[5] = '\0';

        char rs1_bin[6], rs2_bin[6];
        int_bin(rs1, 5, rs1_bin); int_bin(rs2, 5, rs2_bin);

        snprintf(saida_binaria, 33, "%s%s%s%s%s%s", imm_11_5, rs2_bin, rs1_bin, inst->funct3, imm_4_0, inst->opcode);
        return 1;
    }
    else if (inst->tipo == B_TYPE && linha->qtd_op == 3) {
        int rs1 = registrador_int(linha->operandos[0]);
        int rs2 = registrador_int(linha->operandos[1]);
        long imm;

        long val = strtol(linha->operandos[2], &endptr, 0);
        if (*endptr == '\0' || isspace(*endptr)) {
            imm = val;
        } else { 
            int target_addr = buscar_endereco_label(linha->operandos[2], tabela_simbolos, num_labels);
            if (target_addr == -1) return 0;
            imm = target_addr - endereco_atual;
        }
        
        if (rs1 < 0 || rs2 < 0) return 0;
        
        char imm_bin[14];
        int_bin(imm, 13, imm_bin);

        char imm12 = imm_bin[0], imm11 = imm_bin[1];
        char imm10_5[7]; strncpy(imm10_5, &imm_bin[2], 6); imm10_5[6] = '\0';
        char imm4_1[5]; strncpy(imm4_1, &imm_bin[8], 4); imm4_1[4] = '\0';

        char rs1_bin[6], rs2_bin[6];
        int_bin(rs1, 5, rs1_bin); int_bin(rs2, 5, rs2_bin);

        snprintf(saida_binaria, 33, "%c%s%s%s%s%s%c%s", imm12, imm10_5, rs2_bin, rs1_bin, inst->funct3, imm4_1, imm11, inst->opcode);
        return 1;
    }
    else if (inst->tipo == U_TYPE && linha->qtd_op == 2) {
        int rd = registrador_int(linha->operandos[0]);
        long imm = strtol(linha->operandos[1], &endptr, 0);
        if (rd < 0 || (*endptr != '\0' && !isspace(*endptr))) return 0;
        
        char rd_bin[6], imm_bin[21];
        int_bin(rd, 5, rd_bin); int_bin(imm, 20, imm_bin);

        snprintf(saida_binaria, 33, "%s%s%s", imm_bin, rd_bin, inst->opcode);
        return 1;
    }
    else if (inst->tipo == UJ_TYPE && linha->qtd_op == 2) {
        int rd = registrador_int(linha->operandos[0]);
        long imm;

        long val = strtol(linha->operandos[1], &endptr, 0);
        if (*endptr == '\0' || isspace(*endptr)) {
            imm = val;
        } else {
            int target_addr = buscar_endereco_label(linha->operandos[1], tabela_simbolos, num_labels);
            if (target_addr == -1) return 0;
            imm = target_addr - endereco_atual;
        }

        if (rd < 0) return 0;

        char imm_bin_full[22];
        int_bin(imm, 21, imm_bin_full);

        char imm20 = imm_bin_full[0];
        char imm19_12[9]; strncpy(imm19_12, &imm_bin_full[1], 8); imm19_12[8] = '\0';
        char imm11 = imm_bin_full[9];
        char imm10_1[11]; strncpy(imm10_1, &imm_bin_full[10], 10); imm10_1[10] = '\0';
        
        char rd_bin[6];
        int_bin(rd, 5, rd_bin);
        
        snprintf(saida_binaria, 33, "%c%s%c%s%s%s", imm20, imm10_1, imm11, imm19_12, rd_bin, inst->opcode);
        return 1;
    }
    // ADICIONE ESTE BLOCO
    else if (inst->tipo == SYS_TYPE && linha->qtd_op == 0) {
        // Para ecall e ebreak
        // O formato é: funct12 | x0 | funct3 | x0 | opcode
        snprintf(saida_binaria, 33, "%s00000%s00000%s", inst->funct12, inst->funct3, inst->opcode);
        return 1;
    }
    
    return 0; // Se nenhuma condição for atendida, falha
}