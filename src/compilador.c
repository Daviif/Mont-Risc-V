#include "compilador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Função para ler cada linha separadamente
int ler(char *arquivo, char linha[MAX_LINHAS][TAM_LINHAS]){
    FILE *arq = fopen(arquivo, "r");

     if(arq == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int i = 0;
    while (i < MAX_LINHAS && fgets(linha[i], TAM_LINHAS, arq) != NULL){
        linha[i][strcspn(linha[i], "\n")] = '\0';
        i++;
    }

    fclose(arq);

    return i;
}

//Remover virgulas de um token
void removerVir(char *token){
    char *virgula = strchr(token, ',');
    if (virgula) *virgula = '\0';
}

//Analisa transofrma em token a linha
void analisarLin(char *linha, AnL *resultado){
    char copia[TAM_LINHAS];
    strncpy(copia, linha, sizeof(copia));
    copia[sizeof(copia) - 1] = '\0';

    char *token = strtok(copia, " \t,");
    if(!token) return;

    strncpy(resultado -> instrucao, token, TAM_TOKEN);

    char *paren_inicio = strchr(resultado -> instrucao, '(');
    if(paren_inicio){
        token = strtok(NULL, ",");
        if (token){
            while(isspace(*token)){
                token++;
            }
            strncpy(resultado -> operandos[0], token, TAM_TOKEN);
        }
        token = strtok(NULL, "(");
        if (token){
            while(isspace(*token)){
                token++;
            }
            strncpy(resultado -> operandos[2], token, TAM_TOKEN);
        }
        token = strtok(NULL, ")");
        if (token){
            while(isspace(*token)){
                token++;
            }
            strncpy(resultado -> operandos[1], token, TAM_TOKEN);
        }
        resultado -> qtd_op = 3;
    }
    else{
    int i = 0;
    while ((token = strtok(NULL, "\t,")) != NULL && i < MAX_TOKEN){
       removerVir(token);

       while(isspace(*token)){
        token++;
       }

       strncpy(resultado -> operandos[i], token, TAM_TOKEN);
       i++;
    }

    resultado -> qtd_op = i;
    }
}

//Tabela de instrução - 02;
Instrucao tabela[MAX_INSTRU] = {
    {"add",  R_TYPE, "0110011", "000", "0000000"},
    {"sub",  R_TYPE, "0110011", "000", "0100000"},
    {"sll",  R_TYPE, "0110011", "001", "0000000"},
    {"xor",  R_TYPE, "0110011", "100", "0000000"},
    {"srl",  R_TYPE, "0110011", "101", "0000000"},
    {"sra",  R_TYPE, "0110011", "101", "0000000"},
    {"or",   R_TYPE, "0110011", "110", "0000000"},
    {"and",  R_TYPE, "0110011", "111", "0000000"},
    {"lr.d",  R_TYPE, "0110011", "011", "0001000"},
    {"sc.d",  R_TYPE, "0110011", "111", "0001100"},
    {"sb",  S_TYPE, "0100011", "000", ""},
    {"sh",  S_TYPE, "0100011", "001", ""},
    {"sw",  S_TYPE, "0100011", "010", ""},
    {"sd",  S_TYPE, "0100011", "111", ""},
    {"beq",  SB_TYPE, "1100011", "000", ""},
    {"bne",  SB_TYPE, "1100111", "010", ""},
    {"blt",  SB_TYPE, "1100111", "100", ""},
    {"bge",  SB_TYPE, "1100111", "101", ""},
    {"bltu",  SB_TYPE, "1100111", "110", ""},
    {"bgeu",  SB_TYPE, "1100111", "111", ""},
    {"lui",  U_TYPE, "0110011", "", ""},
    {"jal",  UJ_TYPE, "0110011", "", ""},
    {"lb",  I_TYPE, "0000011", "000", ""},
    {"lh",  I_TYPE, "0000011", "001", ""},
    {"lw",  I_TYPE, "0000011", "010", ""},
    {"ld",  I_TYPE, "0000011", "011", ""},
    {"lbu",  I_TYPE, "0000011", "100", ""},
    {"lhu",  I_TYPE, "0000011", "101", ""},
    {"lwu",  I_TYPE, "0000011", "110", ""},
    {"addi",  I_TYPE, "0010011", "000", ""},
    {"slli",  I_TYPE, "0010011", "001", "000000"},
    {"xori",  I_TYPE, "0010011", "100", ""},
    {"srli",  I_TYPE, "0010011", "101", "000000"},
    {"srai",  I_TYPE, "0010011", "101", "010000"},
    {"ori",  I_TYPE, "0010011", "110", ""},
    {"andi",  I_TYPE, "0010011", "111", ""},
    {"jalr",  I_TYPE, "1100111", "000", ""},

};

//Descobrir a instrução e as funct
Instrucao *buscar_instrucao(char *nome){
    for(int i = 0; i < MAX_INSTRU; i++){
        if(strcmp(nome, tabela[i].nome) == 0){
            return &tabela[i];
        }
    }
    return NULL;
}


//Extrair o numero do registrador
int registrador_int(char *reg){
    if(reg[0] != 'x'){
        return -1;
    }

    return atoi(&reg[1]);
}

//Converter inteiro para string binário
void int_bin(int valor, int bits, char *destino){
    for(int i = bits - 1; i >= 0; i--){
        destino[bits - 1 - i] = ((valor >> i) & 1) + '0';
    }
    destino[bits] = '\0';
}

//Monta a instrução em binario
int montar(AnL *linha, Instrucao *inst, char *saida_bin){
    char *endptr;

    if(inst -> tipo == R_TYPE && linha -> qtd_op == 3){
        int rd = registrador_int(linha -> operandos[0]);
        int rs1 = registrador_int(linha -> operandos[1]);
        int rs2 = registrador_int(linha -> operandos[2]);

        if (rd < 0 || rs1 < 0 || rs2 < 0 ){
            return 0;
        }

        char rd_bin[6], rs1_bin[6], rs2_bin[6];

        int_bin(rd, 5, rd_bin);
        int_bin(rs1, 5, rs1_bin);
        int_bin(rs2, 5, rs2_bin);

        snprintf(saida_bin, 33, "%s%s%s%s%s%s", inst -> funct7, rs2_bin, rs1_bin, inst -> funct3, rd_bin, inst -> opcode);

        return 1;
    }
    else if(inst -> tipo == I_TYPE && linha -> qtd_op == 3){
        int rd = registrador_int(linha -> operandos[0]);
        int rs1 = registrador_int(linha -> operandos[1]);
        long imm = strtol(linha -> operandos[2], &endptr, 0);

        if (rd < 0 || rs1 < 0){
            return 0;
        }
        if (linha->operandos[2][0] == 'x') {
            return 0;  // se o operando imediato for um registrador, erro de montagem
        }

        char rd_bin[6], rs1_bin[6], imm_bin[13];

        int_bin(rd, 5, rd_bin);
        int_bin(rs1, 5, rs1_bin);
        int_bin(imm, 12, imm_bin);

        snprintf(saida_bin, 33, "%s%s%s%s%s",imm_bin, rs1_bin, inst -> funct3, rd_bin, inst -> opcode);

        return 1;
    }
    else if(inst -> tipo == SB_TYPE && linha -> qtd_op == 3){
        int rs1 = registrador_int(linha -> operandos[0]);
        int rs2 = registrador_int(linha -> operandos[1]);
        long imm = strtol(linha -> operandos[2], &endptr, 0);

        if (rs1 < 0 || rs2 < 0){
            return 0;
        }

        char imm_bin[15];
        int_bin(imm, 14, imm_bin);

        char imm_12 = imm_bin[0];
        char imm_11 = imm_bin[1];
        char imm_10_5[8], imm_4_1[6];
        strncpy(imm_10_5, &imm_bin[2], 6);
        imm_10_5[6] = '\0';
        strncpy(imm_4_1, &imm_bin[8], 4);
        imm_4_1[4] = '\0';

        char rs1_bin[6], rs2_bin[6];
        int_bin(rs1, 5, rs1_bin);
        int_bin(rs2, 5, rs2_bin);

        snprintf(saida_bin, 33, "%c%s%s%s%s%s%c%s", imm_12, imm_10_5, rs2_bin, rs1_bin,
        inst->funct3, imm_4_1, imm_11, inst->opcode);

        return 1;
    }
    else if (inst -> tipo == S_TYPE && linha -> qtd_op == 3){
        int rs1 = registrador_int(linha -> operandos[1]);
        int rs2 = registrador_int(linha -> operandos[0]);
        long imm = strtol(linha -> operandos[2], &endptr, 0);

        if (rs1 < 0 || rs2 < 0){
            return 0;
        }

        char imm_bin[13];
        int_bin(imm, 12, imm_bin);

        char imm_11_5[8], imm_4_0[6];
        strncpy(imm_11_5, imm_bin, 7);
        imm_11_5[7] = '\0';
        strncpy(imm_4_0, &imm_bin[7], 5);
        imm_4_0[5] = '\0';

        char rs1_bin[6], rs2_bin[6];
        int_bin(rs2, 5, rs2_bin);
        int_bin(rs1, 5, rs1_bin);

        snprintf(saida_bin, 33, "%s%s%s%s%s%s",
            imm_11_5, rs2_bin, rs1_bin, inst->funct3, imm_4_0, inst->opcode);
            
        return 1;
    }
    else{
        return 0; // Instrução não reconhecida ou número de operandos inválido
    }
    
}


