#include "operador_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/operador/operador_model.h"
#include "view/operador/operador_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

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
    ler_string_valida(novo_operador->nome, sizeof(novo_operador->nome), VALIDATE_NAME);

    printf("Usuario: ");
    ler_string_valida(novo_operador->usuario, sizeof(novo_operador->usuario), VALIDATE_NOT_EMPTY);

    printf("Senha: ");
    ler_string_valida(novo_operador->senha, sizeof(novo_operador->senha), VALIDATE_NOT_EMPTY);

    sistema->num_operadores++;

    salvarOperadores(sistema);
    printf("\nOperador '%s' cadastrado com sucesso!\n", novo_operador->usuario);
}

void alterarOperadorController(Sistema *sistema) {
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do operador que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nOperador com codigo %d nao encontrado.\n", codigo);
        return;
    }

    Operador *operador = &sistema->lista_operadores[indice];
    int opcao;
    do {
        limpar_tela();
        printf("--- Alterando Operador: %s ---\n", operador->usuario);
        printf("1. Alterar Nome\n");
        printf("2. Alterar Usuario\n");
        printf("3. Alterar Senha\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        ler_int_valido(&opcao, 0, 3);

        switch (opcao) {
            case 1:
                printf("Novo Nome: ");
                ler_string_valida(operador->nome, sizeof(operador->nome), VALIDATE_NAME);
                break;
            case 2:
                printf("Novo Usuario: ");
                ler_string_valida(operador->usuario, sizeof(operador->usuario), VALIDATE_NOT_EMPTY);
                break;
            case 3:
                printf("Nova Senha: ");
                ler_string_valida(operador->senha, sizeof(operador->senha), VALIDATE_NOT_EMPTY);
                printf("Senha alterada com sucesso.\n");
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);
    
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