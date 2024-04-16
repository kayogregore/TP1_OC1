#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIN_SIZE 5

char* Converter(char* valor) {
    int num = atoi(valor); // Converte a string para um inteiro

    // Se o número for negativo, faça a conversão com o complemento de dois
    if (num < 0) {
        // Inverte todos os bits
        num = ~num;
        // Adiciona 1 ao resultado
        num += 1;
    }

    // Calcula o tamanho necessário para a string binária
    int bits = 0;
    int temp = num;
    while (temp > 0) {
        bits++;
        temp >>= 1; // Desloca para a direita para verificar o próximo bit
    }

    char* binary = (char*)malloc((bits + 1) * sizeof(char)); // Aloca memória para a string binária

    // Preenche a string binária com os bits
    for (int i = bits - 1; i >= 0; i--) {
        binary[i] = (num & 1) + '0'; // Obtém o bit menos significativo
        num >>= 1; // Desloca para a direita para obter o próximo bit
    }
    binary[bits] = '\0'; // Terminador nulo

    return binary;
}

void copiarValor(char string1[], char string2[]) {
    int tamanho1 = strlen(string1);
    int tamanho2 = strlen(string2);
    
    // Inicializa a string2 com zeros
    memset(string2, '0', tamanho2);
    string2[tamanho2] = '\0'; // Adiciona o terminador de string
    
    // Copia os caracteres de trás para frente
    for (int i = 0; i < tamanho1; i++) {
        string2[tamanho2 - 1 - i] = string1[tamanho1 - 1 - i];
    }
}

void inverter_bits(char *binario) {
    int length = strlen(binario);
    for (int i = 0; i < length; i++) {
        binario[i] = (binario[i] == '0') ? '1' : '0';
    }
}

// Função para adicionar 1 ao número binário
void adicionar_um(char *binario) {
    int length = strlen(binario);
    int carry = 1; // carry inicial é 1 para adicionar ao LSB
    for (int i = length - 1; i >= 0; i--) {
        if (carry == 0) break; // Se carry for 0, não é necessário continuar
        if (binario[i] == '0') {
            binario[i] = '1';
            carry = 0; // Não há carry após a adição
        } else {
            binario[i] = '0';
        }
    }
}

// Função para converter binário para complemento de dois
void ConverterComplemento(char *binario) {
    inverter_bits(binario); // Inverter todos os bits
    adicionar_um(binario); // Adicionar 1 ao resultado
}

void MontarInstrucao(char* instrucao) {
    char funct7[8] = "0000000";
    char rs2[6] = "00000";
    char rs1[6] = "00000";
    char funct3[4] = "000";
    char rd[6] = "00000";
    char opcode[8] = "0000000";
    char immediate[13] = "000000000000";

    char* binario = NULL;

    char comando[10];
    char arg1[10], arg2[10], arg3[10];

    sscanf(instrucao, "%s  %[^,], %[^,], %[^\n]", comando, arg1, arg2, arg3);

    char* delimitadores = " ,()"; // Espaço, vírgula e parênteses
    char* token;

    char flag_formato = '0';

    if (strcmp(comando, "sub") == 0) {
        flag_formato = 'R';
        sscanf(instrucao, "%s %s, %s, %s", comando, arg1, arg2, arg3);
        strcpy(funct7, "0100000");
        strcpy(funct3, "000");
        strcpy(opcode, "0110011");
    } else if (strcmp(comando, "srl") == 0) {
        flag_formato = 'R';
        sscanf(instrucao, "%s %s, %s, %s", comando, arg1, arg2, arg3);
        strcpy(funct7, "0000000");
        strcpy(funct3, "101");
        strcpy(opcode, "0110011");
    } else if (strcmp(comando, "or") == 0) {
        flag_formato = 'R';
        sscanf(instrucao, "%s %s, %s, %s", comando, arg1, arg2, arg3);
        strcpy(funct7, "0000000");
        strcpy(funct3, "110");
        strcpy(opcode, "0110011");
    } else if (strcmp(comando, "lh") == 0) {
        flag_formato = 'I';
        // Obtém a primeira palavra
        token = strtok(comando, delimitadores);
        int i = 0;

        // Continua enquanto houver palavras
        while (token != NULL) {
            if (strcmp(token, " ") == 0) {
                strcpy(comando, token);
            } else if (strcmp(token, ",") == 0) {
                strcpy(arg1, token);
            } else if (strcmp(token, "(") == 0) {
                strcpy(arg2,  token);
            } else if (strcmp(token, ")") == 0) {
                strcpy(arg3 , token);
            }
            i++;
            token = strtok(NULL, delimitadores);
        }
        strcpy(funct3, "001");
        strcpy(opcode, "0000011");
    } else if (strcmp(comando, "sh") == 0) {
        flag_formato = 'I';
token = strtok(comando, delimitadores);
        int i = 0;

        // Continua enquanto houver palavras
        while (token != NULL) {
            if (strcmp(token, " ") == 0) {
                strcpy(comando, token);
            } else if (strcmp(token, ",") == 0) {
                strcpy(arg1, token);
            } else if (strcmp(token, "(") == 0) {
                strcpy(arg2,  token);
            } else if (strcmp(token, ")") == 0) {
                strcpy(arg3 , token);
            }
            i++;
            token = strtok(NULL, delimitadores);
        }

        strcpy(funct3, "001");
        strcpy(opcode, "0100011");
    } else if (strcmp(comando, "andi") == 0) {
        flag_formato = 'I';
        sscanf(instrucao, "%s %s, %s, %s", comando, arg1, arg2, arg3);
        strcpy(funct3, "111");
        strcpy(opcode, "0010011");
    } else if (strcmp(comando, "beq") == 0) {
        flag_formato = 'I';
        sscanf(instrucao, "%s %s, %s, %s", comando, arg1, arg2, arg3);
        strcpy(funct3, "000");
        strcpy(opcode, "1100011");
    }
    
    //convercao para formato R
    if(flag_formato == 'R'){
        if (arg1[0] == 'x') {
            arg1[0] = '0';
            binario = Converter(arg1);
            copiarValor(binario, rd);
            free(binario);
        }
        if (arg2[0] == 'x') {
            arg2[0] = '0';
            binario = Converter(arg2);
            copiarValor(binario, rs1);
            free(binario);
        }
        if (arg3[0] == 'x') {
            arg3[0] = '0';
            binario = Converter(arg3);
            copiarValor(binario, immediate);
            free(binario);
        } else if (arg3[0] >= '0' && arg3[0] <= '9') {
            binario = Converter(arg3);
            copiarValor(binario, rs2);
            free(binario);
        }
        
        printf("%s%s%s%s%s%s \n", funct7, rs2, rs1, funct3, rd, opcode);
    }

    //conversao para formato I
    if (flag_formato == 'I'){
        if (arg1[0] == 'x') {
            arg1[0] = '0';
            binario = Converter(arg1);
            copiarValor(binario, rd);
            free(binario);
        }
        if (arg2[0] == 'x') {
            arg2[0] = '0';
            binario = Converter(arg2);
            copiarValor(binario, rs1);
            free(binario);
        }else if (arg2[0] >= '0' && arg2[0] <= '9') {
            binario = Converter(arg2);
            ConverterComplemento(binario);
            copiarValor(binario, immediate);
            free(binario);
        }
        if (arg3[0] == 'x') {
            arg3[0] = '0';
            binario = Converter(arg3);
            copiarValor(binario, rs2);
            free(binario);
        } else if (arg3[0] >= '0' && arg3[0] <= '9') {
            binario = Converter(arg3);
            ConverterComplemento(binario);
            copiarValor(binario, immediate);
            free(binario);
        }

        printf("%s%s%s%s%s \n", immediate, rs1, funct3, rd, opcode);
    }
}

int main() {
    FILE *file;
    char filename[] = "/home/kayo/oc1/TP1_OC1/arquivo.asm"; // Nome do arquivo asm
    char linha[100]; // String para armazenar uma linha do arquivo

    // Abre o arquivo para leitura
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Lê o arquivo linha por linha e imprime a saída no terminal
    while (fgets(linha, sizeof(linha), file)) {
        MontarInstrucao(linha);
    }

    // Fecha o arquivo
    fclose(file);

    return 0;
}
