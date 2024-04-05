#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define DELIMITADORES " \t\n"

// Função para converter um caractere hexadecimal em seu valor decimal correspondente
int hexParaDec(char hexChar) {
    if (hexChar >= '0' && hexChar <= '9') {
        return hexChar - '0';
    } else if (hexChar >= 'a' && hexChar <= 'f') {
        return hexChar - 'a' + 10;
    } else if (hexChar >= 'A' && hexChar <= 'F') {
        return hexChar - 'A' + 10;
    }
    return -1; // Retorna -1 para caracteres inválidos
}

// Função para converter uma string hexadecimal em um número decimal
unsigned int hexParaDecString(char *hexString) {
    unsigned int resultado = 0;
    int comprimento = strlen(hexString);
    int i;
    for (i = 0; i < comprimento; i++) {
        int valorDigito = hexParaDec(hexString[i]);
        if (valorDigito == -1) {
            printf("Caractere hexadecimal inválido: %c\n", hexString[i]);
            return 0; // Retornar 0 se encontrar um caractere inválido
        }
        resultado = resultado * 16 + valorDigito;
    }
    return resultado;
}

// Função para converter um número decimal em uma string binária
void decParaBinString(unsigned int decimal, char *binarioString) {
    int i;
    for (i = 31; i >= 0; i--) {
        binarioString[i] = (decimal & 1) + '0';
        decimal >>= 1;
    }
    binarioString[32] = '\0';
}

char conversao(char *palavra){
    unsigned int decimal;
        if (palavra[0] == 'x') {
            // Se a palavra começar com 'x', assume-se que é um número hexadecimal
            decimal = hexParaDecString(palavra + 1);
        } else {
            // Caso contrário, assume-se que é um número decimal
            sscanf(palavra, "%u", &decimal);
        }

        char binarioString[33]; // Uma string binária tem 32 bits + 1 para o caractere nulo
        decParaBinString(decimal, binarioString);
        printf(" %s\n", binarioString);
    return 0;
}

int main() {
    FILE *arquivo = fopen("/home/kayo/Área de Trabalho/tp's_oc/arquivo.asm", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    char palavra[MAX_WORD_LENGTH];

    while (fscanf(arquivo, "%s", palavra) == 1) {
        if (strcmp(palavra, "lh") == 0) {
            printf("lh");
        } else if (strcmp(palavra, "sh") == 0) {
            printf("sh");
        } else if (strcmp(palavra, "sub") == 0) { 
            printf("sub");
        } else if (strcmp(palavra, "or") == 0) {
            printf("or");
        } else if (strcmp(palavra, "addi") == 0) {
            printf("addi");
        } else if (strcmp(palavra, "srl") == 0) {
            printf("srl");
        } else if (strcmp(palavra, "beq") == 0) {
            printf("beq");
        } else if (strcmp(palavra, "\n") == 0) {
            printf("\n");
        } else {
            conversao(palavra);
        }
        
    }

    fclose(arquivo);
    return EXIT_SUCCESS;
}
