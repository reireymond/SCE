#include "validation.h"
#include "utils.h"
#include <stdlib.h>
#include <ctype.h>

// Função auxiliar interna para pegar só numeros de uma string
// (Usada para limpar CPF/CNPJ antes de validar)
static void extrair_apenas_numeros(const char *entrada, char *saida) {
    int j = 0;
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (isdigit(entrada[i])) {
            saida[j++] = entrada[i];
        }
    }
    saida[j] = '\0';
}

// --- Funções de Validação Específicas  ---

static int validar_nao_vazio(const char *str) {
    return str[0] != '\0';
}

static int validar_nome(const char *str) {
    if (!validar_nao_vazio(str)) return 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        // Aceita letras ou espaços. Se não for nenhum dos dois, erro.
        if (!isalpha(str[i]) && !isspace(str[i])) {
            return 0;
        }
    }
    return 1;
}

static int validar_email(const char *str) {
    // Validação básica de júnior: tem que ter um '@' e um '.'
    // Não precisamos de regex complexo aqui.
    char *arroba = strchr(str, '@');
    if (arroba == NULL) return 0;

    char *ponto = strchr(arroba, '.'); // Ponto deve vir depois do @
    if (ponto == NULL) return 0;

    return 1;
}

static int validar_telefone(const char *str) {
    if (!validar_nao_vazio(str)) return 0;
    
    // Aceita numeros, (), - e espaço
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]) && str[i] != '(' && str[i] != ')' && str[i] != '-' && !isspace(str[i])) {
            return 0;
        }
    }
    return 1;
}

static int validar_data(const char *str) {
    // Formato fixo: DD/MM/AAAA (10 caracteres)
    if (strlen(str) != 10) return 0;
    if (str[2] != '/' || str[5] != '/') return 0;
    return 1; // Assume valido se tiver as barras (logica simples)
}

// Validação matemática de CPF (Algoritmo padrão)
static int validar_cpf(const char *cpf_bruto) {
    char numeros[12];
    extrair_apenas_numeros(cpf_bruto, numeros);

    if (strlen(numeros) != 11) return 0;

    // Elimina CPFs com todos digitos iguais (ex: 111.111.111-11)
    int iguais = 1;
    for(int i = 1; i < 11; i++) {
        if(numeros[i] != numeros[0]) iguais = 0;
    }
    if(iguais) return 0;

    int soma = 0, resto, d1, d2;

    // Primeiro digito
    for (int i = 0; i < 9; i++) soma += (numeros[i] - '0') * (10 - i);
    resto = soma % 11;
    d1 = (resto < 2) ? 0 : 11 - resto;
    if ((numeros[9] - '0') != d1) return 0;

    // Segundo digito
    soma = 0;
    for (int i = 0; i < 10; i++) soma += (numeros[i] - '0') * (11 - i);
    resto = soma % 11;
    d2 = (resto < 2) ? 0 : 11 - resto;
    if ((numeros[10] - '0') != d2) return 0;

    return 1;
}

// Validação matemática de CNPJ (Algoritmo padrão)
static int validar_cnpj(const char *cnpj_bruto) {
    char numeros[15];
    extrair_apenas_numeros(cnpj_bruto, numeros);

    if (strlen(numeros) != 14) return 0;

    // Pesos do algoritmo do CNPJ
    int peso1[] = {5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    int peso2[] = {6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    int soma = 0, resto, d1, d2;

    // Primeiro digito
    for (int i = 0; i < 12; i++) soma += (numeros[i] - '0') * peso1[i];
    resto = soma % 11;
    d1 = (resto < 2) ? 0 : 11 - resto;
    if ((numeros[12] - '0') != d1) return 0;

    // Segundo digito
    soma = 0;
    for (int i = 0; i < 13; i++) soma += (numeros[i] - '0') * peso2[i];
    resto = soma % 11;
    d2 = (resto < 2) ? 0 : 11 - resto;
    if ((numeros[13] - '0') != d2) return 0;

    return 1;
}

// --- Funções Principais de Leitura ---

int ler_texto_valido(char *buffer, int tamanho, TipoValidacao tipo) {
    int valido = 0;
    char temp_numeros[20]; 

    do {
        // fgets lê até o ENTER
        if (fgets(buffer, tamanho, stdin) == NULL) return 0;
        
        // Remove o \n do final
        buffer[strcspn(buffer, "\n")] = 0;

        switch (tipo) {
            case VALIDAR_NAO_VAZIO:
                valido = validar_nao_vazio(buffer);
                if (!valido) printf("Erro: O campo nao pode ser vazio. Tente: ");
                break;
            case VALIDAR_NOME:
                valido = validar_nome(buffer);
                if (!valido) printf("Erro: Use apenas letras. Tente: ");
                break;
            case VALIDAR_EMAIL:
                valido = validar_email(buffer);
                if (!valido) printf("Erro: E-mail invalido. Tente: ");
                break;
            case VALIDAR_TELEFONE:
                valido = validar_telefone(buffer);
                if (!valido) printf("Erro: Telefone invalido. Tente: ");
                break;
            case VALIDAR_CPF:
                valido = validar_cpf(buffer);
                if (!valido) printf("Erro: CPF invalido. Tente: ");
                break;
            case VALIDAR_CNPJ:
                valido = validar_cnpj(buffer);
                if (!valido) printf("Erro: CNPJ invalido. Tente: ");
                break;
            case VALIDAR_DATA:
                valido = validar_data(buffer);
                if (!valido) printf("Erro: Formato DD/MM/AAAA invalido. Tente: ");
                break;
            case VALIDAR_CPF_CNPJ:
                extrair_apenas_numeros(buffer, temp_numeros);
                if (strlen(temp_numeros) == 11) valido = validar_cpf(temp_numeros);
                else if (strlen(temp_numeros) == 14) valido = validar_cnpj(temp_numeros);
                else valido = 0;
                
                if (!valido) printf("Erro: CPF ou CNPJ invalido. Tente: ");
                break;
        }
    } while (!valido);
    
    return 1;
}

int ler_float_positivo(float *valor) {
    int valido = 0;
    do {
        if (scanf("%f", valor) != 1 || *valor < 0) {
            printf("Valor invalido. Digite um numero positivo: ");
            limpar_buffer();
        } else {
            valido = 1;
        }
    } while (!valido);
    limpar_buffer(); // Limpa o \n que sobra
    return 1;
}

int ler_inteiro_valido(int *valor, int min, int max) {
    int valido = 0;
    do {
        if (scanf("%d", valor) != 1 || *valor < min || *valor > max) {
            printf("Opcao invalida (%d a %d). Tente: ", min, max);
            limpar_buffer();
        } else {
            valido = 1;
        }
    } while (!valido);
    limpar_buffer();
    return 1;
}