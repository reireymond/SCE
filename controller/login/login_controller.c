#include "login_controller.h"
#include <stdio.h>
#include <string.h>
#include "model/sistema.h"
#include "controller/operador/operador_controller.h" 
#include "utils/utils.h"      
#include "utils/validation.h"   

// Busca simples do operador
static Operador* buscarOperadorPorUsuario(Sistema *sistema, const char *usuario) {
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (strcmp(sistema->lista_operadores[i].usuario, usuario) == 0) {
            return &sistema->lista_operadores[i]; 
        }
    }
    return NULL; 
}

int realizarLoginOperador(Sistema *sistema) {
    // Se nao tiver ninguem cadastrado, obriga a criar o primeiro (Admin)
    if (sistema->num_operadores == 0) {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|           PRIMEIRO ACESSO - CADASTRO                |\n");
        printf("+=====================================================+\n");
        printf("| Nenhum operador encontrado. Cadastre o primeiro.    |\n");
        printf("+=====================================================+\n\n");
        
        // Isso chama o cadastro (que agora vai estar rapido tambem)
        adicionarOperadorController(sistema); 
        
        printf("\nPrimeiro operador criado! Agora faca o login.\n");
        pausar();
    }

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
        
        // Fluxo rapido: Pergunta -> Digita -> Enter -> Proximo
        printf("Usuario: ");
        ler_texto_valido(usuario_digitado, sizeof(usuario_digitado), VALIDAR_NAO_VAZIO);

        printf("Senha: ");
        ler_texto_valido(senha_digitada, sizeof(senha_digitada), VALIDAR_NAO_VAZIO);
        
        printf("+=====================================================+\n");

        operador_encontrado = buscarOperadorPorUsuario(sistema, usuario_digitado);

        // Verifica login
        if (operador_encontrado != NULL && strcmp(operador_encontrado->senha, senha_digitada) == 0) {
            printf("\nLogin realizado! Bem-vindo, %s.\n", operador_encontrado->nome);
            pausar();
            return 1; // Sucesso
        } else {
            tentativas++;
            printf("\nUsuario ou senha incorretos! Tentativa %d de %d.\n", tentativas, MAX_TENTATIVAS);
            if (tentativas < MAX_TENTATIVAS) {
                pausar();
            }
        }
    }

    printf("\nMuitas tentativas erradas. O sistema vai fechar.\n");
    pausar();
    return 0; // Falha
}