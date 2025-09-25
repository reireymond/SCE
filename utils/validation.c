#include "validation.h"
#include "utils.h" // Para usar o limpar_buffer

// Funções de validação específicas (privadas para este arquivo)
static int is_not_empty(const char *str) {
    return str[0] != '\0';
}

static int is_valid_name(const char *str) {
    if (!is_not_empty(str)) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && !isspace(str[i])) {
            return 0; // Permite apenas letras e espaços
        }
    }
    return 1;
}

static int is_valid_cnpj(const char *str) {
    // Validação simples de formato. Uma validação real de CNPJ é muito mais complexa.
    if (strlen(str) != 18) return 0;
    // Formato esperado: XX.XXX.XXX/XXXX-XX
    for(int i = 0; i < 18; i++) {
        if (i == 2 || i == 6) {
            if (str[i] != '.') return 0;
        } else if (i == 10) {
            if (str[i] != '/') return 0;
        } else if (i == 15) {
            if (str[i] != '-') return 0;
        } else if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

static int is_valid_email(const char *str) {
    // Validação muito básica: verifica se tem um '@' e um '.' depois do '@'
    const char *at = strchr(str, '@');
    if (at == NULL) return 0;
    const char *dot = strchr(at, '.');
    if (dot == NULL || dot == at + 1) return 0;
    return 1;
}

static int is_valid_phone(const char *str) {
    if (!is_not_empty(str)) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]) && str[i] != '(' && str[i] != ')' && str[i] != '-' && !isspace(str[i])) {
            return 0;
        }
    }
    return 1;
}


// Implementação das funções públicas
int ler_string_valida(char *buffer, int length, ValidationType type) {
    int valid = 0;
    do {
        if (fgets(buffer, length, stdin) == NULL) {
            // Falha na leitura
            buffer[0] = '\0';
            return 0; 
        }
        buffer[strcspn(buffer, "\n")] = 0; // Remove a quebra de linha

        switch (type) {
            case VALIDATE_NOT_EMPTY:
                valid = is_not_empty(buffer);
                if (!valid) printf("Entrada invalida. O campo nao pode ser vazio. Tente novamente: ");
                break;
            case VALIDATE_NAME:
                valid = is_valid_name(buffer);
                if (!valid) printf("Entrada invalida. Digite um nome valido (apenas letras e espacos). Tente novamente: ");
                break;
            case VALIDATE_CNPJ:
                valid = is_valid_cnpj(buffer);
                if (!valid) printf("Entrada invalida. Digite o CNPJ no formato XX.XXX.XXX/XXXX-XX. Tente novamente: ");
                break;
            case VALIDATE_EMAIL:
                valid = is_valid_email(buffer);
                if (!valid) printf("Entrada invalida. Digite um e-mail valido. Tente novamente: ");
                break;
            case VALIDATE_PHONE:
                valid = is_valid_phone(buffer);
                if (!valid) printf("Entrada invalida. Digite um telefone valido. Tente novamente: ");
                break;
        }
    } while (!valid);
    return 1;
}

int ler_float_positivo(float *value) {
    int valid = 0;
    do {
        if (scanf("%f", value) != 1 || *value < 0) {
            printf("Entrada invalida. Digite um numero positivo. Tente novamente: ");
            limpar_buffer();
        } else {
            valid = 1;
        }
    } while (!valid);
    limpar_buffer();
    return 1;
}

int ler_int_valido(int *value, int min, int max) {
    int valid = 0;
    do {
        if (scanf("%d", value) != 1 || *value < min || *value > max) {
            printf("Opcao invalida. Digite um numero entre %d e %d. Tente novamente: ", min, max);
            limpar_buffer();
        } else {
            valid = 1;
        }
    } while (!valid);
    limpar_buffer();
    return 1;
}