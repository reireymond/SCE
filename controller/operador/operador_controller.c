// Controller de operador
#include "operador_controller.h"

// Bibliotecas padrão
#include <stdio.h>   // Entrada/saída
#include <stdlib.h>  // Alocação de memória
#include <string.h>  // Manipulação de strings

// Model e view do operador
#include "model/operador/operador_model.h"
#include "view/operador/operador_view.h"

// Utilitários e validação
#include "utils/utils.h"
#include "utils/validation.h"

// Adiciona um novo operador
void adicionarOperadorController(Sistema *sistema) {
    // Expande capacidade da lista se necessário
    if (sistema->num_operadores == sistema->capacidade_operadores) {
        int nova_capacidade = (sistema->capacidade_operadores == 0) ? 10 : sistema->capacidade_operadores * 2;
        Operador *temp = realloc(sistema->lista_operadores, nova_capacidade * sizeof(Operador));
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria!\n");
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

// Altera os dados de um operador existente
void alterarOperadorController(Sistema *sistema) {
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do operador que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura operador pelo código
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nOperador nao encontrado.\n");
        return;
    }

    Operador *operador = &sistema->lista_operadores[indice];
    int opcao;
    do {
        limpar_tela();
        printf("--- Alterando Operador: %s ---\n", operador->usuario);
        printf("1. Alterar Nome\n2. Alterar Usuario\n3. Alterar Senha\n0. Salvar e Voltar\nEscolha: ");
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
                printf("Senha alterada.\n");
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);

    salvarOperadores(sistema);
}

// Exclui um operador
void excluirOperadorController(Sistema *sistema) {
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;

    int codigo;
    printf("\nDigite o codigo do operador para excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura operador pelo código
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o operador %s? (s/n): ", sistema->lista_operadores[i].usuario);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Substitui pelo último operador da lista
                sistema->lista_operadores[i] = sistema->lista_operadores[sistema->num_operadores - 1];
                sistema->num_operadores--;
                salvarOperadores(sistema);
                printf("\nOperador excluido com sucesso!\n");
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }
    printf("\nOperador com codigo %d nao encontrado.\n", codigo);
}
