#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Tipos de validação para strings
typedef enum {
    VALIDATE_NAO_VAZIA,   // Não vazio
    VALIDATE_NOME,        // Apenas letras e espaços
    VALIDATE_EMAIL,       // Formato básico de e-mail
    VALIDATE_TELEFONE,       // Números e caracteres de telefone
    VALIDATE_CPF,         // CPF válido
    VALIDATE_CNPJ,        // CNPJ válido
    VALIDATE_CPF_CNPJ     // CPF ou CNPJ válido
} ValidationType;

// Lê e valida uma string conforme o tipo
int ler_string_valida(char *buffer, int length, ValidationType type);

// Lê float positivo
int ler_float_positivo(float *value);

// Lê inteiro dentro de um intervalo
int ler_int_valido(int *value, int min, int max);

#endif
