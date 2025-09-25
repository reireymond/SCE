#include "recurso_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/recurso/recurso.h"
#include "view/recurso/recurso_view.h"
#include "utils/utils.h"

void adicionarRecursoController(Sistema *sistema) {
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

    Recurso *novo_recurso = &sistema->lista_recursos[sistema->num_recursos];
    novo_recurso->codigo = sistema->num_recursos + 1;

    printf("\n--- Cadastro de Novo Recurso (Codigo: %d) ---\n", novo_recurso->codigo);
    printf("Descricao: ");
    fgets(novo_recurso->descricao, sizeof(novo_recurso->descricao), stdin);
    novo_recurso->descricao[strcspn(novo_recurso->descricao, "\n")] = 0;

    printf("Categoria: ");
    fgets(novo_recurso->categoria, sizeof(novo_recurso->categoria), stdin);
    novo_recurso->categoria[strcspn(novo_recurso->categoria, "\n")] = 0;

    printf("Quantidade em Estoque: ");
    scanf("%d", &novo_recurso->quantidade_estoque);
    limpar_buffer();

    printf("Preco de Custo (unitario): R$ ");
    scanf("%f", &novo_recurso->preco_custo);
    limpar_buffer();

    printf("Valor da Locacao (diaria): R$ ");
    scanf("%f", &novo_recurso->valor_locacao);
    limpar_buffer();

    sistema->num_recursos++;

    salvarRecursos(sistema);
    printf("\nRecurso '%s' cadastrado com sucesso!\n", novo_recurso->descricao);
}

void alterarRecursoController(Sistema *sistema) {
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do recurso para alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nRecurso com codigo %d nao encontrado.\n", codigo);
        return;
    }

    Recurso *recurso = &sistema->lista_recursos[indice];
    int opcao;
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
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                printf("Nova Descricao: ");
                fgets(recurso->descricao, sizeof(recurso->descricao), stdin);
                recurso->descricao[strcspn(recurso->descricao, "\n")] = 0;
                break;
            case 2:
                printf("Nova Categoria: ");
                fgets(recurso->categoria, sizeof(recurso->categoria), stdin);
                recurso->categoria[strcspn(recurso->categoria, "\n")] = 0;
                break;
            case 3:
                printf("Nova Quantidade em Estoque: ");
                scanf("%d", &recurso->quantidade_estoque);
                limpar_buffer();
                break;
            case 4:
                printf("Novo Preco de Custo: R$ ");
                scanf("%f", &recurso->preco_custo);
                limpar_buffer();
                break;
            case 5:
                printf("Novo Valor da Locacao: R$ ");
                scanf("%f", &recurso->valor_locacao);
                limpar_buffer();
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                pausar();
        }
    } while (opcao != 0);

    salvarRecursos(sistema);
}

void excluirRecursoController(Sistema *sistema) {
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;

    int codigo;
    printf("\nDigite o codigo do recurso para excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o recurso %s? (s/n): ", sistema->lista_recursos[i].descricao);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
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
    printf("\nRecurso com codigo %d nao encontrado.\n", codigo);
}