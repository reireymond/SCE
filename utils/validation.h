#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Tipos de validação (Nomes traduzidos e simplificados)
typedef enum {
    VALIDAR_NAO_VAZIO,   // Texto obrigatorio
    VALIDAR_NOME,        // Apenas letras
    VALIDAR_EMAIL,       // Tem @
    VALIDAR_TELEFONE,    // Numeros e tracos
    VALIDAR_CPF,         // Matematica do CPF
    VALIDAR_CNPJ,        // Matematica do CNPJ
    VALIDAR_CPF_CNPJ,    // Tenta um, se falhar tenta o outro
    VALIDAR_DATA         // Formato DD/MM/AAAA
} TipoValidacao;

// Lê string do teclado e valida
int ler_texto_valido(char *buffer, int tamanho, TipoValidacao tipo);

// Lê numero decimal positivo
int ler_float_positivo(float *valor);

// Lê numero inteiro dentro de um intervalo (minimo e maximo)
int ler_inteiro_valido(int *valor, int min, int max);

#endif