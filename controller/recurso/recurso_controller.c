// Inclui o cabeçalho do controller de recurso, que contém as declarações
// das funções que são implementadas neste arquivo.
#include "recurso_controller.h"
// Inclui cabeçalhos padrão do C para funções de entrada/saída (stdio),
// alocação de memória (stdlib) e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Inclui o cabeçalho do model para poder chamar a função 'salvarRecursos'.
#include "model/recurso/recurso_model.h"
// Inclui o cabeçalho da view para poder chamar a função 'listarRecursosView'.
#include "view/recurso/recurso_view.h"
// Inclui o cabeçalho de utilitários para usar funções auxiliares como 'limpar_buffer' e 'limpar_tela'.
#include "utils/utils.h"
// Inclui o cabeçalho de validação para usar funções como 'ler_string_valida' e 'ler_int_valido'.
#include "utils/validation.h"

// Função para adicionar um novo recurso ao sistema.
void adicionarRecursoController(Sistema *sistema) {
    // Verifica se a lista de recursos atingiu sua capacidade máxima.
    if (sistema->num_recursos == sistema->capacidade_recursos) {
        // Se a capacidade for 0, define uma capacidade inicial; caso contrário, dobra a capacidade.
        int nova_capacidade = (sistema->capacidade_recursos == 0) ? 10 : sistema->capacidade_recursos * 2;
        // Tenta realocar a memória da lista para o novo tamanho.
        Recurso *temp = realloc(sistema->lista_recursos, nova_capacidade * sizeof(Recurso));
        // Se a realocação falhar, exibe um erro e encerra a função.
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para recursos!\n");
            return;
        }
        // Atualiza o ponteiro da lista e a capacidade no sistema.
        sistema->lista_recursos = temp;
        sistema->capacidade_recursos = nova_capacidade;
    }

    // Obtém um ponteiro para a próxima posição livre no array de recursos.
    Recurso *novo_recurso = &sistema->lista_recursos[sistema->num_recursos];
    // Atribui um código sequencial ao novo recurso.
    novo_recurso->codigo = sistema->num_recursos + 1;

    // Exibe o cabeçalho do formulário de cadastro.
    printf("\n--- Cadastro de Novo Recurso (Codigo: %d) ---\n", novo_recurso->codigo);
    // Solicita e lê cada informação do novo recurso, validando as entradas.
    printf("Descricao: ");
    ler_string_valida(novo_recurso->descricao, sizeof(novo_recurso->descricao), VALIDATE_NAO_VAZIA);

    printf("Categoria: ");
    ler_string_valida(novo_recurso->categoria, sizeof(novo_recurso->categoria), VALIDATE_NAO_VAZIA);

    printf("Quantidade em Estoque: ");
    int estoque;
    // Lê um valor inteiro para o estoque, garantindo que esteja entre 0 e 9999.
    ler_int_valido(&estoque, 0, 9999);
    novo_recurso->quantidade_estoque = estoque;

    printf("Preco de Custo (unitario): R$ ");
    ler_float_positivo(&novo_recurso->preco_custo);

    printf("Valor da Locacao (diaria): R$ ");
    ler_float_positivo(&novo_recurso->valor_locacao);

    // Incrementa o número total de recursos.
    sistema->num_recursos++;

    // Salva a lista atualizada no arquivo.
    salvarRecursos(sistema);
    printf("\nRecurso '%s' cadastrado com sucesso!\n", novo_recurso->descricao);
}

// Função para alterar os dados de um recurso existente.
void alterarRecursoController(Sistema *sistema) {
    // Exibe a lista de recursos para que o usuário escolha qual alterar.
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do recurso para alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o recurso pelo código informado.
    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) {
            indice = i; // Armazena o índice do recurso encontrado.
            break;
        }
    }

    // Se o recurso não foi encontrado, exibe uma mensagem e retorna.
    if (indice == -1) {
        printf("\nRecurso com codigo %d nao encontrado.\n", codigo);
        return;
    }

    // Obtém um ponteiro para o recurso que será modificado.
    Recurso *recurso = &sistema->lista_recursos[indice];
    int opcao;
    // Loop para o menu de alteração.
    do {
        limpar_tela();
        printf("--- Alterando Recurso: %s ---\n", recurso->descricao);
        printf("1. Alterar Descricao\n");
        printf("2. Alterar Categoria\n");
        printf("3. Alterar Quantidade em Estoque\n");
        printf("4. Alterar Preco de Custo\n");
        printf("5. Alterar Valor da Locacao\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        // Lê a opção do usuário, validando o intervalo de entrada.
        ler_int_valido(&opcao, 0, 5);

        // Executa a ação correspondente à opção escolhida.
        switch (opcao) {
            case 1:
                printf("Nova Descricao: ");
                ler_string_valida(recurso->descricao, sizeof(recurso->descricao), VALIDATE_NAO_VAZIA);
                break;
            case 2:
                printf("Nova Categoria: ");
                ler_string_valida(recurso->categoria, sizeof(recurso->categoria), VALIDATE_NAO_VAZIA);
                break;
            case 3:
                printf("Nova Quantidade em Estoque: ");
                int estoque;
                ler_int_valido(&estoque, 0, 9999);
                recurso->quantidade_estoque = estoque;
                break;
            case 4:
                printf("Novo Preco de Custo: R$ ");
                ler_float_positivo(&recurso->preco_custo);
                break;
            case 5:
                printf("Novo Valor da Locacao: R$ ");
                ler_float_positivo(&recurso->valor_locacao);
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);

    // Salva todas as alterações no arquivo.
    salvarRecursos(sistema);
}

// Função para excluir um recurso do sistema.
void excluirRecursoController(Sistema *sistema) {
    // Exibe a lista de recursos.
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;

    int codigo;
    printf("\nDigite o codigo do recurso para excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o recurso pelo código.
    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) {
            char confirmacao;
            // Pede confirmação ao usuário antes de prosseguir com a exclusão.
            printf("Tem certeza que deseja excluir o recurso %s? (s/n): ", sistema->lista_recursos[i].descricao);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Move o último recurso da lista para a posição do que está sendo excluído.
                sistema->lista_recursos[i] = sistema->lista_recursos[sistema->num_recursos - 1];
                // Reduz o contador de recursos.
                sistema->num_recursos--;
                printf("\nRecurso excluido com sucesso!\n");
                // Salva o estado atualizado da lista no arquivo.
                salvarRecursos(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            // Retorna, pois a operação foi finalizada.
            return;
        }
    }
    // Se o loop terminar sem encontrar o recurso, exibe uma mensagem.
    printf("\nRecurso com codigo %d nao encontrado.\n", codigo);
}