#include "compilador.h"
#include "pseudo_instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Tabela de instrução - 02;
Instrucao tabela[MAX_INSTRU] = {
    {"add",  R_TYPE, "0110011", "000", "0000000"},
    {"sub",  R_TYPE, "0110011", "000", "0100000"},
    {"sll",  R_TYPE, "0110011", "001", "0000000"},
    {"xor",  R_TYPE, "0110011", "100", "0000000"},
    {"srl",  R_TYPE, "0110011", "101", "0000000"},
    {"sra",  R_TYPE, "0110011", "101", "0100000"},
    {"or",   R_TYPE, "0110011", "110", "0000000"},
    {"and",  R_TYPE, "0110011", "111", "0000000"},
    {"lr.d",  R_TYPE, "0110011", "011", "0001000"},
    {"sc.d",  R_TYPE, "0110011", "111", "0001100"},
    {"sb",  S_TYPE, "0100011", "000", ""},
    {"sh",  S_TYPE, "0100011", "001", ""},
    {"sw",  S_TYPE, "0100011", "010", ""},
    {"sd",  S_TYPE, "0100011", "111", ""},
    {"beq",  B_TYPE, "1100011", "000", ""},
    {"bne",  B_TYPE, "1100011", "010", ""},
    {"blt",  B_TYPE, "1100011", "100", ""},
    {"bge",  B_TYPE, "1100011", "101", ""},
    {"bltu",  B_TYPE, "1100011", "110", ""},
    {"bgeu",  B_TYPE, "1100011", "111", ""},
    {"lui",  U_TYPE, "0110111", "", ""},
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

// Tabela que mapeia nomes para funções
PseudoTradutor tabela_tradutores[] = {
    {"mv",  traduzir_mv},
    {"nop", traduzir_nop},
    {"not", traduzir_not}
    // Para adicionar 'neg', crie a função 'traduzir_neg' e adicione aqui:
    // {"neg", traduzir_neg}
};

int num_tradutores = sizeof(tabela_tradutores) / sizeof(tabela_tradutores[0]);

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
// Em compilador.c
void analisarLin(char *linha, AnL *resultado) {
    char *comment = strchr(linha, '#');
    if (comment) {
        *comment = '\0';
    }
    char copia_linha[TAM_LINHAS];

    strncpy(copia_linha, linha, sizeof(copia_linha));
    copia_linha[sizeof(copia_linha) - 1] = '\0';

    // 1. Pega a instrução
    char* token = strtok(copia_linha, " \t");
    if (!token) return;
    strncpy(resultado->instrucao, token, TAM_TOKEN);

    // 2. Pega o resto da linha como uma única string
    char* resto = strtok(NULL, "");
    if (!resto) {
        resultado->qtd_op = 0;
        return;
    }
    while (isspace(*resto)) resto++; // Remove espaços em branco iniciais

    // 3. Verifica se tem o formato offset(reg)
    char* paren_inicio = strchr(resto, '(');
    char* paren_fim = strchr(resto, ')');
    char* virgula = strchr(resto, ',');

    if (paren_inicio && paren_fim && virgula && virgula < paren_inicio) {
        // Formato: rs2, offset(rs1)
 
        *virgula = '\0';
        strncpy(resultado->operandos[0], resto, TAM_TOKEN);
        
        // Extrai imm (entre a vírgula e o parêntese)
        *paren_inicio = '\0';
        strncpy(resultado->operandos[2], virgula + 1, TAM_TOKEN);

        // Extrai rs1 (dentro dos parênteses)
        *paren_fim = '\0';
        strncpy(resultado->operandos[1], paren_inicio + 1, TAM_TOKEN);
        
        resultado->qtd_op = 3;
    } else {
        // Formato padrão (usa strtok no resto da linha)
        int i = 0;
        token = strtok(resto, " \t,");
        while (token != NULL && i < MAX_TOKEN) {
            strncpy(resultado->operandos[i], token, TAM_TOKEN);
            i++;
            token = strtok(NULL, " \t,");
        }
        resultado->qtd_op = i;
    }

    for (int i = 0; i < num_tradutores; i++) {
        if (strcmp(resultado->instrucao, tabela_tradutores[i].nome) == 0) {
            tabela_tradutores[i].traduzir(resultado);
            break; // Encontrou e traduziu, pode sair do laço
        }
    }
}

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

    if(inst -> tipo == R_TYPE && linha -> qtd_op == 3){ //Instrução do tipo R
        int rd = registrador_int(linha -> operandos[0]);
        int rs1 = registrador_int(linha -> operandos[1]);
        int rs2 = registrador_int(linha -> operandos[2]);

        if (rd < 0 || rs1 < 0 || rs2 < 0 ){
            return 0; // Registrador inválido
        }

        char rd_bin[6], rs1_bin[6], rs2_bin[6]; // Variáveis para armazenar os registradores em binário

        int_bin(rd, 5, rd_bin); //
        int_bin(rs1, 5, rs1_bin);
        int_bin(rs2, 5, rs2_bin);

        snprintf(saida_bin, 33, "%s%s%s%s%s%s", inst -> funct7, rs2_bin, rs1_bin, inst -> funct3, rd_bin, inst -> opcode);

        return 1;
    }
    else if(inst -> tipo == I_TYPE && linha -> qtd_op == 3){
        int rd = registrador_int(linha -> operandos[0]);
        int rs1 = registrador_int(linha -> operandos[1]);
        long imm = strtol(linha -> operandos[2], &endptr, 0); // Converte o operando imediato para inteiro

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
    else if(inst -> tipo == B_TYPE && linha -> qtd_op == 3){
        int rs1 = registrador_int(linha -> operandos[0]);
        int rs2 = registrador_int(linha -> operandos[1]);
        long imm = strtol(linha -> operandos[2], &endptr, 0);

        if (rs1 < 0 || rs2 < 0){
            return 0;
        }

        if(imm % 2 != 0){
            return 0; // Imediato deve ser par
        }

        char imm_bin[15];
        int_bin(imm, 14, imm_bin);

        char imm_12 = imm_bin[0], imm_11 = imm_bin[1], imm_10_5[8], imm_4_1[6];
        int_bin((imm >> 12) & 1, 1, &imm_12);
        int_bin((imm >> 11) & 1, 1, &imm_11);
        int_bin((imm >> 5) & 0x3F, 6, imm_10_5);
        int_bin((imm >> 1) & 0xF, 4, imm_4_1);

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
    else if(inst -> tipo == U_TYPE && linha -> qtd_op == 2){
        int rd = registrador_int(linha -> operandos[0]);
        long imm = strtol(linha -> operandos[1], &endptr, 0);

        if (rd < 0){
            return 0;
        }

        char rd_bin[6], imm_bin[21];
        int_bin(rd, 5, rd_bin);
        int_bin(imm, 20, imm_bin);

        snprintf(saida_bin, 33, "%s%s%s", imm_bin, rd_bin, inst -> opcode);

        return 1;
    }
    else if(inst -> tipo == UJ_TYPE && linha -> qtd_op == 2){
        int rd = registrador_int(linha -> operandos[0]);
        long imm = strtol(linha -> operandos[1], &endptr, 0);

        if (rd < 0){
            return 0;
        }

        char rd_bin[6], imm_bin[21];
        int_bin(rd, 5, rd_bin);
        int_bin(imm, 20, imm_bin);

        snprintf(saida_bin, 33, "%s%s%s", imm_bin, rd_bin, inst -> opcode);

        return 1;
    }
    else{
        return 0; // Instrução não reconhecida ou número de operandos inválido
    }
}


