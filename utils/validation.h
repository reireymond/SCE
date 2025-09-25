#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Enum para diferentes tipos de validação de string
typedef enum {
    VALIDATE_NOT_EMPTY, // Apenas verifica se não está vazio
    VALIDATE_NAME,      // Valida para nomes (não vazio, sem caracteres especiais)
    VALIDATE_CNPJ,      // Valida um formato básico de CNPJ (XX.XXX.XXX/XXXX-XX)
    VALIDATE_EMAIL,     // Valida um formato básico de e-mail
    VALIDATE_PHONE      // Valida para telefone (permite números, () e -)
} ValidationType;

int ler_string_valida(char *buffer, int length, ValidationType type);

int ler_float_positivo(float *value);

int ler_int_valido(int *value, int min, int max);

#endif