#include "operador_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/operador/operador.h"
#include "view/operador/operador_view.h"
#include "utils/utils.h"

void adicionarOperadorController(Sistema *sistema) {
    if (sistema->num_operadores == sistema->capacidade_operadores) {
        int nova_capacidade = (sistema->capacidade_operadores == 0) ? 10 : sistema->capacidade_operadores * 2;
        Operador *temp = realloc(sistema->lista_operadores, nova_capacidade * sizeof(Operador));
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para operadores!\n");
            return;
        }
        sistema->lista_operadores = temp;
        sistema->capacidade_operadores = nova_capacidade;
    }

    Operador *novo_operador = &sistema->lista_operadores[sistema->num_operadores];
    novo_operador->codigo = sistema->num_operadores + 1;

    printf("\n--- Cadastro de Novo Operador (Codigo: %d) ---\n", novo_operador->codigo);
    printf("Nome: ");
    fgets(novo_operador->nome, sizeof(novo_operador->nome), stdin);
    novo_operador->nome[strcspn(novo_operador->nome, "\n")] = 0;

    printf("Usuario: ");
    fgets(novo_operador->usuario, sizeof(novo_operador->usuario), stdin);
    novo_operador->usuario[strcspn(novo_operador->usuario, "\n")] = 0;

    printf("Senha: ");
    fgets(novo_operador->senha, sizeof(novo_operador->senha), stdin);
    novo_operador->senha[strcspn(novo_operador->senha, "\n")] = 0;

    sistema->num_operadores++;

    salvarOperadores(sistema);
    printf("\nOperador '%s' cadastrado com sucesso!\n", novo_operador->usuario);
}

void alterarOperadorController(Sistema *sistema) {
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;

    // Lógica de alteração (adapte do seu código antigo)
    printf("\nFuncao de alterar operador ainda nao implementada.\n");
    salvarOperadores(sistema);
}

void excluirOperadorController(Sistema *sistema) {
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;

    int codigo;
    printf("\nDigite o codigo do operador para excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o operador %s? (s/n): ", sistema->lista_operadores[i].usuario);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                sistema->lista_operadores[i] = sistema->lista_operadores[sistema->num_operadores - 1];
                sistema->num_operadores--;
                printf("\nOperador excluido com sucesso!\n");
                salvarOperadores(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }
    printf("\nOperador com codigo %d nao encontrado.\n", codigo);
}