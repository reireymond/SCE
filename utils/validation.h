#ifndef VALIDATION_H
#define VALIDATION_H

typedef enum {
    VALIDAR_NAO_VAZIO,
    VALIDAR_NOME,
    VALIDAR_EMAIL,
    VALIDAR_TELEFONE,
    VALIDAR_CPF,
    VALIDAR_CNPJ,
    VALIDAR_CPF_CNPJ,
    VALIDAR_DATA
} TipoValidacao;

int ler_texto_valido(char *buffer, int tamanho, TipoValidacao tipo);
int ler_float_positivo(float *valor);
int ler_inteiro_valido(int *valor, int min, int max);

#endif