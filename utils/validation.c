// Inclui o cabeçalho de validação, que contém as declarações das funções aqui implementadas.
#include "validation.h"
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_buffer'.
#include "utils.h"
// Inclui a biblioteca padrão para funções como 'atoi' (converter string para inteiro).
#include <stdlib.h>


//Funções de validação específicas (privadas para este arquivo) 


static void extrair_apenas_numeros(const char *str_in, char *str_out)
{
    // 'j' será o índice para a string de saída.
    int j = 0;
    // Percorre a string de entrada caractere por caractere.
    for (int i = 0; str_in[i] != '\0'; i++)
    {
        // A função 'isdigit' verifica se o caractere é um número (0-9).
        if (isdigit(str_in[i]))
        {
            // Se for um número, copia para a string de saída.
            str_out[j++] = str_in[i];
        }
    }
    // Adiciona o caractere nulo ao final da string de saída para finalizá-la.
    str_out[j] = '\0';
}

// Verifica se uma string não está vazia.
static int is_not_empty(const char *str)
{
    // Se o primeiro caractere for o terminador nulo, a string está vazia.
    return str[0] != '\0';
}

// Verifica se uma string é um nome válido (contém apenas letras e espaços).
static int is_valid_name(const char *str)
{
    // Primeiro, verifica se a string não está vazia.
    if (!is_not_empty(str))
    {
        return 0; // Retorna 0 (falso) se estiver vazia.
    }
    // Percorre a string.
    for (int i = 0; str[i] != '\0'; i++)
    {
        // Se encontrar um caractere que não seja letra ('isalpha') nem espaço ('isspace')...
        if (!isalpha(str[i]) && !isspace(str[i]))
        {
            return 0; // ...a string é inválida.
        }
    }
    return 1; // Retorna 1 (verdadeiro) se todos os caracteres forem válidos.
}

// Realiza uma validação muito básica de formato de e-mail.
static int is_valid_email(const char *str)
{
    // Procura pela primeira ocorrência do caractere '@'.
    const char *at = strchr(str, '@');
    // Se não encontrar o '@', o e-mail é inválido.
    if (at == NULL)
    {
        return 0;
    }
    // A partir da posição do '@', procura pela primeira ocorrência de um ponto '.'.
    const char *dot = strchr(at, '.');
    // Se não houver ponto após o '@', ou se o ponto vier imediatamente após o '@', é inválido.
    if (dot == NULL || dot == at + 1)
    {
        return 0;
    }
    return 1; // Se passou nas verificações, considera o formato básico como válido.
}

// Verifica se a string contém apenas caracteres válidos para um número de telefone.
static int is_valid_phone(const char *str)
{
    if (!is_not_empty(str))
    {
        return 0;
    }
    // Permite números, parênteses, hífen e espaços.
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]) && str[i] != '(' && str[i] != ')' && str[i] != '-' && !isspace(str[i]))
        {
            return 0;
        }
    }
    return 1;
}

// Validação simples de data (formato DD/MM/AAAA)
static int is_valid_data(const char *str) {
    int len = strlen(str);
    // Uma data "DD/MM/AAAA" deve ter 10 caracteres
    if (len != 10) {
        return 0;
    }

    // Verifica se as barras estão nas posições corretas
    if (str[2] != '/' || str[5] != '/') {
        return 0;
    }

    // Verifica se os outros caracteres são dígitos
    for (int i = 0; i < len; i++) {
        if (i == 2 || i == 5) {
            continue; // Pula as barras
        }
        if (!isdigit(str[i])) {
            return 0; // Não é um dígito
        }
    }
    
    return 1; // Formato básico válido
}


// função valida cpf

// Realiza a validação matemática completa de um CPF.

static int is_valid_cpf(const char *cpf_str)
{
    char numeros[12];
    // Extrai apenas os dígitos do CPF informado.
    extrair_apenas_numeros(cpf_str, numeros);

    // Um CPF deve ter exatamente 11 dígitos.
    if (strlen(numeros) != 11)
    {
        return 0;
    }

    // Verifica se todos os dígitos são iguais (ex: 111.111.111-11), o que é inválido.
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

    // Cálculo do primeiro dígito verificador.
    soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (numeros[i] - '0') * (10 - i);
    }
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;

    // Compara o dígito calculado com o dígito informado.
    if ((numeros[9] - '0') != digito1)
    {
        return 0;
    }

    // Cálculo do segundo dígito verificador.
    soma = 0;
    for (int i = 0; i < 10; i++)
    {
        soma += (numeros[i] - '0') * (11 - i);
    }
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;

    // Compara o segundo dígito calculado com o informado.
    if ((numeros[10] - '0') != digito2)
    {
        return 0;
    }

    return 1; // Se todas as verificações passaram, o CPF é válido.
}

static int is_valid_cnpj(const char *cnpj_str)
{
    char numeros[15];
    // Limpa a string, mantendo apenas os números.
    extrair_apenas_numeros(cnpj_str, numeros);

    // Um CNPJ deve ter exatamente 14 dígitos.
    if (strlen(numeros) != 14)
    {
        return 0;
    }

    // Verifica se todos os dígitos são iguais, o que é uma formação inválida.
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

    // Cálculo do primeiro dígito verificador do CNPJ.
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

    // Cálculo do segundo dígito verificador do CNPJ.
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

    return 1; // Se passou em todas as verificações, o CNPJ é válido.
}

// Função genérica para ler e validar uma string do teclado.
int ler_string_valida(char *buffer, int length, ValidationType type)
{
    int valid = 0;
    char numeros_limpos[20]; // Buffer auxiliar para validação de CPF/CNPJ.

    // Loop que continua até que uma entrada válida seja fornecida.
    do
    {
        // Lê uma linha inteira do teclado, incluindo espaços.
        if (fgets(buffer, length, stdin) == NULL)
        {
            buffer[0] = '\0'; // Se ocorrer um erro, esvazia o buffer.
            return 0;
        }
        // Remove o caractere de nova linha ('\n') que o fgets captura no final.
        buffer[strcspn(buffer, "\n")] = 0;

        // Executa a validação apropriada com base no tipo especificado.
        switch (type)
        {
        case VALIDATE_NAO_VAZIA:
            valid = is_not_empty(buffer);
            if (!valid)
                printf("Entrada invalida. O campo nao pode ser vazio. Tente novamente: ");
            break;

        case VALIDATE_NOME:
            valid = is_valid_name(buffer);
            if (!valid)
                printf("Entrada invalida. Digite um nome valido (apenas letras e espacos). Tente novamente: ");
            break;

        case VALIDATE_EMAIL:
            valid = is_valid_email(buffer);
            if (!valid)
                printf("Entrada invalida. Digite um e-mail valido. Tente novamente: ");
            break;

        case  VALIDATE_TELEFONE:
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
             
        case VALIDATE_DATA:
            // Chama a nova função de validação de data
            valid = is_valid_data(buffer); 
            if (!valid)
                printf("Data invalida. Digite uma data no formato DD/MM/AAAA. Tente novamente: ");
            break;

        case VALIDATE_CPF_CNPJ:
            extrair_apenas_numeros(buffer, numeros_limpos);
            // Se tiver 11 dígitos, tenta validar como CPF.
            if (strlen(numeros_limpos) == 11)
            {
                valid = is_valid_cpf(numeros_limpos);
            }
            // Se tiver 14 dígitos, tenta validar como CNPJ.
            else if (strlen(numeros_limpos) == 14)
            {
                valid = is_valid_cnpj(numeros_limpos);
            }
            else
            {
                valid = 0; // Se não tiver 11 ou 14 dígitos, é inválido.
            }

            if (!valid)
                printf("CPF/CNPJ invalido. Verifique os digitos e tente novamente: ");
            break;
        }

    } while (!valid);
    return 1;
}

// Lê um número float do teclado e garante que ele não seja negativo.
int ler_float_positivo(float *value)
{
    int valid = 0;
    do
    {
        // Tenta ler um float. Se o retorno de scanf não for 1, a entrada não é um número.
        // Também verifica se o valor lido é menor que zero.
        if (scanf("%f", value) != 1 || *value < 0)
        {
            printf("Entrada invalida. Digite um numero positivo. Tente novamente: ");
            // Limpa o buffer de entrada para descartar a entrada inválida.
            limpar_buffer();
        }
        else
        {
            valid = 1; // A entrada é válida.
        }
    } while (!valid);
    // Limpa o buffer para remover o '\n' deixado pelo scanf.
    limpar_buffer();
    return 1;
}

// Lê um número inteiro do teclado e garante que ele esteja dentro de um intervalo [min, max].
int ler_int_valido(int *value, int min, int max)
{
    int valid = 0;
    do
    {
        // Tenta ler um inteiro. Se falhar, ou se o valor estiver fora do intervalo...
        if (scanf("%d", value) != 1 || *value < min || *value > max)
        {
            // ...pede para o usuário tentar novamente.
            printf("Opcao invalida. Digite um numero entre %d e %d. Tente novamente: ", min, max);
            // Limpa o buffer de entrada para a próxima tentativa.
            limpar_buffer();
        }
        else
        {
            valid = 1; // A entrada é válida.
        }
    } while (!valid);
    // Limpa o buffer para remover o '\n'.
    limpar_buffer();
    return 1;
}