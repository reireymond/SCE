#include "login_controller.h"
#include <stdio.h>
#include <string.h>
#include "model/sistema.h"
#include "controller/operador/operador_controller.h" // Para adicionar o primeiro operador
#include "utils/utils.h"         // Para limpar_buffer, limpar_tela
#include "utils/validation.h"    // Para ler_string_valida

// Função auxiliar para buscar um operador pelo usuário (pode ser movida para operador_controller se preferir)
static Operador* buscarOperadorPorUsuario(Sistema *sistema, const char *usuario) {
    for (int i = 0; i < sistema->num_operadores; i++) {
        // strcmp retorna 0 se as strings forem iguais
        if (strcmp(sistema->lista_operadores[i].usuario, usuario) == 0) {
            return &sistema->lista_operadores[i]; // Retorna o ponteiro para o operador encontrado
        }
    }
    return NULL; // Retorna NULL se não encontrar
}

int realizarLoginOperador(Sistema *sistema) {
    // Caso 1: Nenhum operador cadastrado, força o cadastro do primeiro
    if (sistema->num_operadores == 0) {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|                PRIMEIRO ACESSO AO SISTEMA             |\n");
        printf("+=====================================================+\n");
        printf("| Nenhum operador encontrado. Cadastre o primeiro:    |\n");
        printf("+=====================================================+\n\n");
        adicionarOperadorController(sistema); // Chama a função existente para cadastrar
        printf("\nPrimeiro operador cadastrado com sucesso!\n");
        printf("Por favor, realize o login com as credenciais criadas.\n");
        pausar();
        // Após cadastrar, o sistema deve prosseguir para a tela de login normal
        // (o fluxo continua abaixo)
    }

    // Caso 2: Já existem operadores, solicita login
    char usuario_digitado[50];
    char senha_digitada[50];
    Operador *operador_encontrado;
    int tentativas = 0;
    const int MAX_TENTATIVAS = 3;

    while (tentativas < MAX_TENTATIVAS) {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|                 LOGIN DO OPERADOR                   |\n");
        printf("+=====================================================+\n");
        printf("Usuario: ");
        ler_string_valida(usuario_digitado, sizeof(usuario_digitado), VALIDATE_NOT_EMPTY);

        printf("Senha: ");
        // Para segurança básica, idealmente a senha não seria exibida.
        // Implementar um sistema de asteriscos é mais complexo e depende do SO.
        // Aqui, usamos a leitura simples por simplicidade.
        ler_string_valida(senha_digitada, sizeof(senha_digitada), VALIDATE_NOT_EMPTY);
        printf("+=====================================================+\n");


        operador_encontrado = buscarOperadorPorUsuario(sistema, usuario_digitado);

        // Verifica se o operador foi encontrado E se a senha confere
        if (operador_encontrado != NULL && strcmp(operador_encontrado->senha, senha_digitada) == 0) {
            printf("\nLogin realizado com sucesso! Bem-vindo, %s.\n", operador_encontrado->nome);
            pausar();
            return 1; // Sucesso no login
        } else {
            tentativas++;
            printf("\nUsuario ou senha incorretos! Tentativa %d de %d.\n", tentativas, MAX_TENTATIVAS);
            if (tentativas < MAX_TENTATIVAS) {
                pausar();
            }
        }
    }

    printf("\nNumero maximo de tentativas excedido. Encerrando o programa.\n");
    pausar();
    return 0; // Falha no login após tentativas
}