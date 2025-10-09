// Inclui o cabeçalho do controller de operador, que contém as declarações
// das funções implementadas neste arquivo.
#include "operador_controller.h"
// Inclui cabeçalhos padrão do C para funções de entrada/saída (stdio),
// alocação de memória (stdlib) e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Inclui o cabeçalho do model para poder chamar a função 'salvarOperadores'.
#include "model/operador/operador_model.h"
// Inclui o cabeçalho da view para poder chamar a função 'listarOperadoresView'.
#include "view/operador/operador_view.h"
// Inclui o cabeçalho de utilitários para usar funções auxiliares como 'limpar_buffer'.
#include "utils/utils.h"
// Inclui o cabeçalho de validação para usar funções como 'ler_string_valida'.
#include "utils/validation.h"

// Função para adicionar um novo operador ao sistema.
void adicionarOperadorController(Sistema *sistema) {
    // Verifica se a lista de operadores atingiu a capacidade máxima do array.
    if (sistema->num_operadores == sistema->capacidade_operadores) {
        // Se a capacidade for 0, define uma capacidade inicial; senão, dobra a capacidade.
        int nova_capacidade = (sistema->capacidade_operadores == 0) ? 10 : sistema->capacidade_operadores * 2;
        // Tenta realocar a memória da lista para o novo tamanho.
        Operador *temp = realloc(sistema->lista_operadores, nova_capacidade * sizeof(Operador));
        // Se a realocação falhar, informa o erro e encerra a função.
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para operadores!\n");
            return;
        }
        // Atualiza o ponteiro da lista e a capacidade no sistema.
        sistema->lista_operadores = temp;
        sistema->capacidade_operadores = nova_capacidade;
    }

    // Obtém um ponteiro para a próxima posição livre no array de operadores.
    Operador *novo_operador = &sistema->lista_operadores[sistema->num_operadores];
    // Atribui um código sequencial ao novo operador.
    novo_operador->codigo = sistema->num_operadores + 1;

    // Exibe o cabeçalho para o formulário de cadastro.
    printf("\n--- Cadastro de Novo Operador (Codigo: %d) ---\n", novo_operador->codigo);
    // Solicita e lê cada informação do novo operador, validando as entradas.
    printf("Nome: ");
    ler_string_valida(novo_operador->nome, sizeof(novo_operador->nome), VALIDATE_NAME);

    printf("Usuario: ");
    ler_string_valida(novo_operador->usuario, sizeof(novo_operador->usuario), VALIDATE_NOT_EMPTY);

    printf("Senha: ");
    ler_string_valida(novo_operador->senha, sizeof(novo_operador->senha), VALIDATE_NOT_EMPTY);

    // Incrementa o número total de operadores.
    sistema->num_operadores++;

    // Salva a lista atualizada no arquivo.
    salvarOperadores(sistema);
    printf("\nOperador '%s' cadastrado com sucesso!\n", novo_operador->usuario);
}

// Função para alterar os dados de um operador existente.
void alterarOperadorController(Sistema *sistema) {
    // Exibe a lista de operadores para que o usuário escolha qual alterar.
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do operador que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o operador pelo código informado.
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            indice = i; // Armazena o índice do operador encontrado.
            break;
        }
    }

    // Se o operador não foi encontrado, exibe uma mensagem e retorna.
    if (indice == -1) {
        printf("\nOperador com codigo %d nao encontrado.\n", codigo);
        return;
    }

    // Obtém um ponteiro para o operador que será modificado.
    Operador *operador = &sistema->lista_operadores[indice];
    int opcao;
    // Loop para o menu de alteração.
    do {
        limpar_tela();
        printf("--- Alterando Operador: %s ---\n", operador->usuario);
        printf("1. Alterar Nome\n");
        printf("2. Alterar Usuario\n");
        printf("3. Alterar Senha\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        // Lê a opção do usuário, validando o intervalo de entrada.
        ler_int_valido(&opcao, 0, 3);

        // Executa a ação correspondente à opção escolhida.
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
    
    // Salva todas as alterações no arquivo.
    salvarOperadores(sistema);
}

// Função para excluir um operador do sistema.
void excluirOperadorController(Sistema *sistema) {
    // Exibe a lista de operadores.
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;

    int codigo;
    printf("\nDigite o codigo do operador para excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o operador pelo código.
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            char confirmacao;
            // Pede confirmação ao usuário antes de prosseguir com a exclusão.
            printf("Tem certeza que deseja excluir o operador %s? (s/n): ", sistema->lista_operadores[i].usuario);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Move o último operador da lista para a posição do que está sendo excluído.
                sistema->lista_operadores[i] = sistema->lista_operadores[sistema->num_operadores - 1];
                // Reduz o contador de operadores.
                sistema->num_operadores--;
                printf("\nOperador excluido com sucesso!\n");
                // Salva o estado atualizado da lista no arquivo.
                salvarOperadores(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            // Retorna, pois a operação foi finalizada.
            return;
        }
    }
    // Se o loop terminar sem encontrar o operador, exibe uma mensagem.
    printf("\nOperador com codigo %d nao encontrado.\n", codigo);
}