#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Enum para diferentes tipos de validação de string
typedef enum {
    VALIDATE_NOT_EMPTY,     // Apenas verifica se não está vazio
    VALIDATE_NAME,          // Valida para nomes (não vazio, sem caracteres especiais)
    VALIDATE_EMAIL,         // Valida um formato básico de e-mail
    VALIDATE_PHONE,         // Valida para telefone (permite números, () e -)
    VALIDATE_CPF,           // Valida o cálculo de um CPF (NOVO)
    VALIDATE_CNPJ,          // Valida o cálculo de um CNPJ (NOVO)
    VALIDATE_CPF_CNPJ       // Valida um ou outro (NOVO)
} ValidationType;

int ler_string_valida(char *buffer, int length, ValidationType type);

int ler_float_positivo(float *value);

int ler_int_valido(int *value, int min, int max);

#endif