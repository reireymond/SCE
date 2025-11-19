#include "validation.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Funcao simples pra ler texto e nao deixar vazio
int ler_texto_valido(char *buffer, int tamanho, TipoValidacao tipo) {
    fgets(buffer, tamanho, stdin);
    
    // Tira o \n do final
    buffer[strcspn(buffer, "\n")] = 0;
    
    // Se for vazio e o tipo pedir validacao
    if (tipo == VALIDAR_NAO_VAZIO && strlen(buffer) == 0) {
        printf("Erro: Nao pode ser vazio. Tente de novo: ");
        return ler_texto_valido(buffer, tamanho, tipo);
    }
    return 1;
}

int ler_float_positivo(float *valor) {
    while (scanf("%f", valor) != 1 || *valor < 0) {
        printf("Valor invalido! Digite um numero positivo: ");
        limpar_buffer();
    }
    limpar_buffer();
    return 1;
}

int ler_inteiro_valido(int *valor, int min, int max) {
    while (scanf("%d", valor) != 1 || *valor < min || *valor > max) {
        printf("Opcao invalida! Digite entre %d e %d: ", min, max);
        limpar_buffer();
    }
    limpar_buffer();
    return 1;
}