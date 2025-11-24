#include "validation.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Mantém apenas numeros na string
void limpar_numeros(char *dest, const char *src) {
    while (*src) {
        if (isdigit(*src)) *dest++ = *src;
        src++;
    }
    *dest = 0; // Finaliza string
}

// Funcao genérica para calcular digito verificador (serve p/ CPF e CNPJ)
int calcular_digito(const char *str, int *pesos, int qtd_para_calcular) {
    int soma = 0;
    for (int i = 0; i < qtd_para_calcular; i++) {
        soma += (str[i] - '0') * pesos[i];
    }
    int resto = soma % 11;
    return (resto < 2) ? 0 : 11 - resto;
}


int validar_cpf_logica(const char *cpf_bruto) {
    char cpf[12]; limpar_numeros(cpf, cpf_bruto);
    if (strlen(cpf) != 11) return 0;
    
    // Verifica digitos iguais (111.111.111-11)
    int i; for (i=0; i<10 && cpf[i]==cpf[i+1]; i++); if (i==10) return 0;

    int p1[] = {10,9,8,7,6,5,4,3,2};
    int p2[] = {11,10,9,8,7,6,5,4,3,2};

    if (calcular_digito(cpf, p1, 9) != (cpf[9] - '0')) return 0;
    if (calcular_digito(cpf, p2, 10) != (cpf[10] - '0')) return 0;
    return 1;
}

int validar_cnpj_logica(const char *cnpj_bruto) {
    char cnpj[15]; limpar_numeros(cnpj, cnpj_bruto);
    if (strlen(cnpj) != 14) return 0;

    int p1[] = {5,4,3,2,9,8,7,6,5,4,3,2};
    int p2[] = {6,5,4,3,2,9,8,7,6,5,4,3,2};

    if (calcular_digito(cnpj, p1, 12) != (cnpj[12] - '0')) return 0;
    if (calcular_digito(cnpj, p2, 13) != (cnpj[13] - '0')) return 0;
    return 1;
}

int validar_data_logica(char *data) {
    int d, m, a;
    if (sscanf(data, "%d/%d/%d", &d, &m, &a) != 3) return 0;
    if (a < 1900 || a > 2100 || m < 1 || m > 12 || d < 1) return 0;

    // Tabela de dias por mes (index 0 vazio para mes 1 bater com index 1)
    int dias_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Ajuste ano bissexto
    if ((a % 4 == 0 && a % 100 != 0) || a % 400 == 0) dias_mes[2] = 29;

    return d <= dias_mes[m];
}


int ler_texto_valido(char *buffer, int tamanho, TipoValidacao tipo) {
    fflush(stdin);
    if (!fgets(buffer, tamanho, stdin)) return 0;
    buffer[strcspn(buffer, "\n")] = 0; // Tira o enter

    if (strlen(buffer) == 0) return ler_texto_valido(buffer, tamanho, tipo); 

    int valido = 1;
    char *msg = "Dado invalido";

    switch (tipo) {
        case VALIDAR_NOME:
            for(int i=0; buffer[i]; i++) if(isdigit(buffer[i])) valido = 0;
            break;
        case VALIDAR_EMAIL:
            { // Chaves criam escopo para variaveis locais no switch
                char *at = strchr(buffer, '@');
                valido = (at && strchr(at, '.')); // Tem @ e ponto depois
            }
            break;
        case VALIDAR_TELEFONE: 
            { char n[20]; limpar_numeros(n, buffer); valido = strlen(n) >= 8; }
            break;
        case VALIDAR_DATA:     valido = validar_data_logica(buffer); break;
        case VALIDAR_CPF:      valido = validar_cpf_logica(buffer); break;
        case VALIDAR_CNPJ:     valido = validar_cnpj_logica(buffer); break;
        case VALIDAR_CPF_CNPJ: valido = validar_cpf_logica(buffer) || validar_cnpj_logica(buffer); break;
        default: break;
    }

    if (!valido) {
        printf("Erro: %s. Tente novamente: ", msg);
        return ler_texto_valido(buffer, tamanho, tipo);
    }
    return 1;
}

int ler_float_positivo(float *valor) {
    while (scanf("%f", valor) != 1 || *valor < 0) {
        printf("Invalido! Digite numero positivo: "); limpar_buffer();
    }
    limpar_buffer(); return 1;
}

int ler_inteiro_valido(int *valor, int min, int max) {
    while (scanf("%d", valor) != 1 || *valor < min || *valor > max) {
        printf("Opcao invalida [%d-%d]: ", min, max); limpar_buffer();
    }
    limpar_buffer(); return 1;
}