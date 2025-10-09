// Garante que o conteúdo deste arquivo de cabeçalho seja incluído apenas uma vez
// durante a compilação, prevenindo erros de redefinição.
#ifndef VALIDATION_H
#define VALIDATION_H

// Inclui cabeçalhos padrão do C necessários para as funções de validação.
#include <stdio.h>  // Para entrada e saída (fgets, printf).
#include <string.h> // Para manipulação de strings (strlen, strcspn, strchr).
#include <ctype.h>  // Para verificação de tipos de caracteres (isdigit, isalpha, isspace).

// Define uma enumeração para especificar os diferentes tipos de validação de string.
// Isso torna a chamada da função 'ler_string_valida' mais clara e legível.
typedef enum {
    VALIDATE_NOT_EMPTY,     // A string não pode ser vazia.
    VALIDATE_NAME,          // A string deve conter apenas letras e espaços.
    VALIDATE_EMAIL,         // A string deve ter um formato básico de e-mail (conter '@' e '.').
    VALIDATE_PHONE,         // A string deve conter apenas números e caracteres comuns de telefone.
    VALIDATE_CPF,           // A string deve ser um CPF matematicamente válido.
    VALIDATE_CNPJ,          // A string deve ser um CNPJ matematicamente válido.
    VALIDATE_CPF_CNPJ       // A string pode ser um CPF ou um CNPJ válido.
} ValidationType;

// Declaração da função que lê uma string do teclado e a valida.
// Ela continua pedindo a entrada do usuário até que uma string válida seja fornecida.
// 'buffer' é onde a string será armazenada, 'length' é o tamanho do buffer,
// e 'type' especifica o tipo de validação a ser aplicada.
int ler_string_valida(char *buffer, int length, ValidationType type);

// Declaração da função que lê um número de ponto flutuante (float) e garante que ele seja positivo.
// Continua pedindo a entrada até que um valor válido seja inserido.
int ler_float_positivo(float *value);

// Declaração da função que lê um número inteiro e garante que ele esteja dentro de um intervalo.
// Continua pedindo a entrada até que um valor válido entre 'min' e 'max' seja inserido.
int ler_int_valido(int *value, int min, int max);

// Fim da diretiva de inclusão condicional.
#endif