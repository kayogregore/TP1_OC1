#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIN_SIZE 5

char* Converter(char* valor) {
    int num = atoi(valor); // Converte a string para um inteiro

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

void MontarInstrucao(char* instrucao, FILE* output) {
    char funct7[8] = "0000000";
    char rs2[6] = "00000";
    char rs1[6] = "00000";
    char funct3[4] = "000";
    char rd[6] = "00000";
    char opcode[8] = "0000000";
    char immediate[13] = "000000000000";

    char* binario = NULL;

    char comando[10];
    char arg1[10], arg2[10], arg3[10], arg4[10], arg5[10];

    sscanf(instrucao, "%s %[^,], %[^,], %[^,], %[^,], %[^\n]", comando, arg1, arg2, arg3, arg4, arg5);

    char flag_formato = '0';

    if (strcmp(comando, "sub") == 0) {
        flag_formato = 'R';
        strcpy(funct7, "0100000");
        strcpy(funct3, "000");
        strcpy(opcode, "0110011");
    } else if (strcmp(comando, "srl") == 0) {
        flag_formato = 'R';
        strcpy(funct7, "0000000");
        strcpy(funct3, "101");
        strcpy(opcode, "0110011");
    } else if (strcmp(comando, "or") == 0) {
        flag_formato = 'R';
        strcpy(funct7, "0000000");
        strcpy(funct3, "110");
        strcpy(opcode, "0110011");
    } else if (strcmp(comando, "lh") == 0) {
        flag_formato = 'I';
        strcpy(funct3, "001");
        strcpy(opcode, "0000011");
    } else if (strcmp(comando, "sh") == 0) {
        flag_formato = 'I';
        strcpy(funct3, "001");
        strcpy(opcode, "0100011");
    } else if (strcmp(comando, "andi") == 0) {
        flag_formato = 'I';
        strcpy(funct3, "111");
        strcpy(opcode, "0010011");
    } else if (strcmp(comando, "beq") == 0) {
        flag_formato = 'I';
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
        
        fprintf(output, "%s %s %s %s %s %s ", funct7, rs2, rs1, funct3, rd, opcode);
    }

    //conversao para formato I
    if (flag_formato == 'I'){
        // Converte o valor imediato para binário usando o complemento de dois
        if (arg4[0] >= '0' && arg4[0] <= '9') {
            binario = Converter(arg4);
            // ConverterComplemento(binario);
            copiarValor(binario, immediate);
            free(binario);
        }
        // Converte os registradores para binário
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
            copiarValor(binario, rs2);
            free(binario);
        }
        if (arg5[0] == 'x') {
            arg5[0] = '0';
            binario = Converter(arg5);
            // ConverterComplemento(binario);
            copiarValor(binario, immediate);
            free(binario);
        }

        fprintf(output, "%s %s %s %s %s ", immediate, rs1, funct3, rd, opcode);
    }

    // Contador de bits
    int bits_count = 0;
    if (flag_formato == 'R') {
        bits_count += strlen(funct7) + strlen(rs2) + strlen(rs1) + strlen(funct3) + strlen(rd) + strlen(opcode);
    } else if (flag_formato == 'I') {
        bits_count += strlen(immediate) + strlen(rs1) + strlen(funct3) + strlen(rd) + strlen(opcode);
    }
    fprintf(output, "%d\n", bits_count); // Adiciona o contador de bits no final da linha
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso correto: %s arquivo_entrada.asm -o arquivo_saida\n", argv[0]);
        return 1;
    }

    FILE *file;
    FILE *output;
    char *filename = argv[1]; // Nome do arquivo asm
    char *outputFilename = argv[3]; // Nome do arquivo de saída
    char linha[100]; // String para armazenar uma linha do arquivo

    // Abrir o arquivo para leitura
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    // Abrir o arquivo de saída para escrita
    output = fopen(outputFilename, "w");
    if (output == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        fclose(file);
        return 1;
    }

    // Ler linha por linha e montar as instruções
    while (fgets(linha, sizeof(linha), file)) {
        MontarInstrucao(linha, output);
    }

    // Fechar os arquivos
    fclose(file);
    fclose(output);

    printf("Arquivo de saída gerado com sucesso: %s\n", outputFilename);

    return 0;
}
