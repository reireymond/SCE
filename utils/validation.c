#include "validation.h"
#include "utils.h" // Para usar o limpar_buffer
#include <stdlib.h> // Para atoi

//Funções de validação específicas (privadas para este arquivo) 


static void extrair_apenas_numeros(const char *str_in, char *str_out)
{
    int j = 0;
    for (int i = 0; str_in[i] != '\0'; i++)
    {
        if (isdigit(str_in[i]))
        {
            str_out[j++] = str_in[i];
        }
    }
    str_out[j] = '\0';
}

static int is_not_empty(const char *str)
{
    return str[0] != '\0';
}

static int is_valid_name(const char *str)
{
    if (!is_not_empty(str))
    {
        return 0;
    }
    for (int i = 0; str[i] != '\0'; i++)
    {
        // Permite apenas letras e espaços
        if (!isalpha(str[i]) && !isspace(str[i]))
        {
            return 0;
        }
    }
    return 1;
}

static int is_valid_email(const char *str)
{
    // Validação muito básica: verifica se tem um '@' e um '.' depois do '@'
    const char *at = strchr(str, '@');
    if (at == NULL)
    {
        return 0;
    }
    const char *dot = strchr(at, '.');
    if (dot == NULL || dot == at + 1)
    {
        return 0;
    }
    return 1;
}

static int is_valid_phone(const char *str)
{
    if (!is_not_empty(str))
    {
        return 0;
    }
    for (int i = 0; str[i] != '\0'; i++)
    {
        // Permite números, '()', '-' e espaços
        if (!isdigit(str[i]) && str[i] != '(' && str[i] != ')' && str[i] != '-' && !isspace(str[i]))
        {
            return 0;
        }
    }
    return 1;
}

//FUNÇÕES DE VALIDAÇÃO DE CPF E CNPJ 


static int is_valid_cpf(const char *cpf_str)
{
    char numeros[12];
    extrair_apenas_numeros(cpf_str, numeros);

    if (strlen(numeros) != 11)
    {
        return 0;
    }

    // Verifica se todos os dígitos são iguais (ex: 111.111.111-11)
    int todos_iguais = 1;
    for (int i = 1; i < 11; i++)
    {
        if (numeros[i] != numeros[0])
        {
            todos_iguais = 0;
            break;
        }
    }
    if (todos_iguais)
    {
        return 0;
    }

    int soma, resto, digito1, digito2;

    // Cálculo do 1º dígito verificador
    soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (numeros[i] - '0') * (10 - i);
    }
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;

    if ((numeros[9] - '0') != digito1)
    {
        return 0;
    }

    // Cálculo do 2º dígito verificador
    soma = 0;
    for (int i = 0; i < 10; i++)
    {
        soma += (numeros[i] - '0') * (11 - i);
    }
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;

    if ((numeros[10] - '0') != digito2)
    {
        return 0;
    }

    return 1; // CPF Válido
}


static int is_valid_cnpj(const char *cnpj_str)
{
    char numeros[15];
    extrair_apenas_numeros(cnpj_str, numeros);

    if (strlen(numeros) != 14)
    {
        return 0;
    }

    // Verifica se todos os dígitos são iguais
    int todos_iguais = 1;
    for (int i = 1; i < 14; i++)
    {
        if (numeros[i] != numeros[0])
        {
            todos_iguais = 0;
            break;
        }
    }
    if (todos_iguais)
    {
        return 0;
    }

    int soma, resto, digito1, digito2;
    int pesos1[] = {5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    int pesos2[] = {6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};

    // Cálculo do 1º dígito verificador
    soma = 0;
    for (int i = 0; i < 12; i++)
    {
        soma += (numeros[i] - '0') * pesos1[i];
    }
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;

    if ((numeros[12] - '0') != digito1)
    {
        return 0;
    }

    // Cálculo do 2º dígito verificador
    soma = 0;
    for (int i = 0; i < 13; i++)
    {
        soma += (numeros[i] - '0') * pesos2[i];
    }
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;

    if ((numeros[13] - '0') != digito2)
    {
        return 0;
    }

    return 1; // CNPJ Válido
}
 

int ler_string_valida(char *buffer, int length, ValidationType type)
{
    int valid = 0;
    char numeros_limpos[20]; // Buffer auxiliar para CPF/CNPJ

    do
    {
        if (fgets(buffer, length, stdin) == NULL)
        {
            buffer[0] = '\0';
            return 0;
        }
        buffer[strcspn(buffer, "\n")] = 0; // Remove a quebra de linha

        switch (type)
        {
        case VALIDATE_NOT_EMPTY:
            valid = is_not_empty(buffer);
            if (!valid)
                printf("Entrada invalida. O campo nao pode ser vazio. Tente novamente: ");
            break;

        case VALIDATE_NAME:
            valid = is_valid_name(buffer);
            if (!valid)
                printf("Entrada invalida. Digite um nome valido (apenas letras e espacos). Tente novamente: ");
            break;

        case VALIDATE_EMAIL:
            valid = is_valid_email(buffer);
            if (!valid)
                printf("Entrada invalida. Digite um e-mail valido. Tente novamente: ");
            break;

        case VALIDATE_PHONE:
            valid = is_valid_phone(buffer);
            if (!valid)
                printf("Entrada invalida. Digite um telefone valido. Tente novamente: ");
            break;

        case VALIDATE_CPF:
            valid = is_valid_cpf(buffer);
            if (!valid)
                printf("CPF invalido. Digite um CPF matematicamente valido. Tente novamente: ");
            break;

        case VALIDATE_CNPJ:
            valid = is_valid_cnpj(buffer);
            if (!valid)
                printf("CNPJ invalido. Digite um CNPJ matematicamente valido. Tente novamente: ");
            break;

        case VALIDATE_CPF_CNPJ:
            extrair_apenas_numeros(buffer, numeros_limpos);
            if (strlen(numeros_limpos) == 11)
            {
                valid = is_valid_cpf(numeros_limpos);
            }
            else if (strlen(numeros_limpos) == 14)
            {
                valid = is_valid_cnpj(numeros_limpos);
            }
            else
            {
                valid = 0; // Nem 11 nem 14 dígitos
            }

            if (!valid)
                printf("CPF/CNPJ invalido. Verifique os digitos e tente novamente: ");
            break;
        }

    } while (!valid);
    return 1;
}

int ler_float_positivo(float *value)
{
    int valid = 0;
    do
    {
        if (scanf("%f", value) != 1 || *value < 0)
        {
            printf("Entrada invalida. Digite um numero positivo. Tente novamente: ");
            limpar_buffer();
        }
        else
        {
            valid = 1;
        }
    } while (!valid);
    limpar_buffer();
    return 1;
}

int ler_int_valido(int *value, int min, int max)
{
    int valid = 0;
    do
    {
        if (scanf("%d", value) != 1 || *value < min || *value > max)
        {
            printf("Opcao invalida. Digite um numero entre %d e %d. Tente novamente: ", min, max);
            limpar_buffer();
        }
        else
        {
            valid = 1;
        }
    } while (!valid);
    limpar_buffer();
    return 1;
}