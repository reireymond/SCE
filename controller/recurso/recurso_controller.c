#include "recurso_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/recurso/recurso_model.h"
#include "view/recurso/recurso_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Função para adicionar um novo recurso ao sistema
void adicionarRecursoController(Sistema *sistema) {
    // Verifica se é necessário aumentar a capacidade do vetor de recursos
    if (sistema->num_recursos == sistema->capacidade_recursos) {
        int nova_capacidade = (sistema->capacidade_recursos == 0) ? 10 : sistema->capacidade_recursos * 2;
        Recurso *temp = realloc(sistema->lista_recursos, nova_capacidade * sizeof(Recurso));
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para recursos!\n");
            return;
        }
        sistema->lista_recursos = temp;
        sistema->capacidade_recursos = nova_capacidade;
    }

    // Cria um novo recurso e atribui o código automaticamente
    Recurso *novo_recurso = &sistema->lista_recursos[sistema->num_recursos];
    novo_recurso->codigo = sistema->num_recursos + 1;

    // Solicita os dados do recurso
    printf("\n--- Cadastro de Novo Recurso (Codigo: %d) ---\n", novo_recurso->codigo);
    printf("Descricao: ");
    ler_string_valida(novo_recurso->descricao, sizeof(novo_recurso->descricao), VALIDATE_NOT_EMPTY);

    printf("Categoria: ");
    ler_string_valida(novo_recurso->categoria, sizeof(novo_recurso->categoria), VALIDATE_NOT_EMPTY);

    printf("Quantidade em Estoque: ");
    int estoque;
    ler_int_valido(&estoque, 0, 9999); // Lê um número válido entre 0 e 9999
    novo_recurso->quantidade_estoque = estoque;

    printf("Preco de Custo (unitario): R$ ");
    ler_float_positivo(&novo_recurso->preco_custo);

    printf("Valor da Locacao (diaria): R$ ");
    ler_float_positivo(&novo_recurso->valor_locacao);

    // Incrementa a quantidade total de recursos
    sistema->num_recursos++;

    // Salva os dados atualizados
    salvarRecursos(sistema);
    printf("\nRecurso '%s' cadastrado com sucesso!\n", novo_recurso->descricao);
}

// Função para alterar os dados de um recurso já cadastrado
void alterarRecursoController(Sistema *sistema) {
    listarRecursosView(sistema); // Mostra todos os recursos cadastrados
    if (sistema->num_recursos == 0) return;

    // Pede o código do recurso que será alterado
    int codigo, indice = -1;
    printf("\nDigite o codigo do recurso para alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Busca o recurso pelo código
    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    // Caso não encontre o recurso
    if (indice == -1) {
        printf("\nRecurso com codigo %d nao encontrado.\n", codigo);
        return;
    }

    Recurso *recurso = &sistema->lista_recursos[indice];
    int opcao;
    do {
        limpar_tela();
        // Exibe menu de edição
        printf("--- Alterando Recurso: %s ---\n", recurso->descricao);
        printf("1. Alterar Descricao\n");
        printf("2. Alterar Categoria\n");
        printf("3. Alterar Quantidade em Estoque\n");
        printf("4. Alterar Preco de Custo\n");
        printf("5. Alterar Valor da Locacao\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        ler_int_valido(&opcao, 0, 5);

        // Executa a alteração conforme a opção escolhida
        switch (opcao) {
            case 1:
                printf("Nova Descricao: ");
                ler_string_valida(recurso->descricao, sizeof(recurso->descricao), VALIDATE_NOT_EMPTY);
                break;
            case 2:
                printf("Nova Categoria: ");
                ler_string_valida(recurso->categoria, sizeof(recurso->categoria), VALIDATE_NOT_EMPTY);
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

    // Salva as alterações feitas
    salvarRecursos(sistema);
}

// Função para excluir um recurso do sistema
void excluirRecursoController(Sistema *sistema) {
    listarRecursosView(sistema); // Mostra os recursos atuais
    if (sistema->num_recursos == 0) return;

    // Pede o código do recurso a excluir
    int codigo;
    printf("\nDigite o codigo do recurso para excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o recurso pelo código
    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) {
            // Confirmação da exclusão
            char confirmacao;
            printf("Tem certeza que deseja excluir o recurso %s? (s/n): ", sistema->lista_recursos[i].descricao);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            // Caso o usuário confirme
            if (confirmacao == 's' || confirmacao == 'S') {
                // Substitui o recurso excluído pelo último da lista
                sistema->lista_recursos[i] = sistema->lista_recursos[sistema->num_recursos - 1];
                sistema->num_recursos--;
                printf("\nRecurso excluido com sucesso!\n");
                salvarRecursos(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }

    // Caso o código não seja encontrado
    printf("\nRecurso com codigo %d nao encontrado.\n", codigo);
}
