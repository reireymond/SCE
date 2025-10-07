#include "operador_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/operador/operador_model.h"
#include "view/operador/operador_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Função responsável por cadastrar um novo operador no sistema
void adicionarOperadorController(Sistema *sistema) {
    // Verifica se é necessário aumentar a capacidade da lista de operadores
    if (sistema->num_operadores == sistema->capacidade_operadores) {
        int nova_capacidade = (sistema->capacidade_operadores == 0) ? 10 : sistema->capacidade_operadores * 2;
        // Realoca memória para armazenar mais operadores
        Operador *temp = realloc(sistema->lista_operadores, nova_capacidade * sizeof(Operador));
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para operadores!\n");
            return;
        }
        sistema->lista_operadores = temp;
        sistema->capacidade_operadores = nova_capacidade;
    }

    // Cria um novo operador no próximo índice disponível
    Operador *novo_operador = &sistema->lista_operadores[sistema->num_operadores];
    novo_operador->codigo = sistema->num_operadores + 1;

    // Coleta os dados do operador
    printf("\n--- Cadastro de Novo Operador (Codigo: %d) ---\n", novo_operador->codigo);
    printf("Nome: ");
    ler_string_valida(novo_operador->nome, sizeof(novo_operador->nome), VALIDATE_NAME);

    printf("Usuario: ");
    ler_string_valida(novo_operador->usuario, sizeof(novo_operador->usuario), VALIDATE_NOT_EMPTY);

    printf("Senha: ");
    ler_string_valida(novo_operador->senha, sizeof(novo_operador->senha), VALIDATE_NOT_EMPTY);

    // Atualiza o total de operadores cadastrados
    sistema->num_operadores++;

    // Salva os dados no arquivo
    salvarOperadores(sistema);
    printf("\nOperador '%s' cadastrado com sucesso!\n", novo_operador->usuario);
}

// Função responsável por alterar os dados de um operador existente
void alterarOperadorController(Sistema *sistema) {
    listarOperadoresView(sistema); // Exibe a lista de operadores
    if (sistema->num_operadores == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do operador que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o operador pelo código informado
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    // Caso o operador não seja encontrado
    if (indice == -1) {
        printf("\nOperador com codigo %d nao encontrado.\n", codigo);
        return;
    }

    // Ponteiro para o operador que será alterado
    Operador *operador = &sistema->lista_operadores[indice];
    int opcao;
    do {
        limpar_tela(); // Limpa a tela a cada iteração
        printf("--- Alterando Operador: %s ---\n", operador->usuario);
        printf("1. Alterar Nome\n");
        printf("2. Alterar Usuario\n");
        printf("3. Alterar Senha\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        ler_int_valido(&opcao, 0, 3);

        // Menu de alteração de campos do operador
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
    
    // Salva as alterações realizadas
    salvarOperadores(sistema);
}

// Função responsável por excluir um operador do sistema
void excluirOperadorController(Sistema *sistema) {
    listarOperadoresView(sistema); // Exibe a lista de operadores
    if (sistema->num_operadores == 0) return;

    int codigo;
    printf("\nDigite o codigo do operador para excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o operador pelo código
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            char confirmacao;
            // Pede confirmação antes de excluir
            printf("Tem certeza que deseja excluir o operador %s? (s/n): ", sistema->lista_operadores[i].usuario);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            // Se o usuário confirmar, o operador é removido
            if (confirmacao == 's' || confirmacao == 'S') {
                sistema->lista_operadores[i] = sistema->lista_operadores[sistema->num_operadores - 1]; // Substitui pelo último da lista
                sistema->num_operadores--; // Reduz a contagem
                printf("\nOperador excluido com sucesso!\n");
                salvarOperadores(sistema); // Atualiza os dados
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }

    // Caso o código não seja encontrado
    printf("\nOperador com codigo %d nao encontrado.\n", codigo);
}
