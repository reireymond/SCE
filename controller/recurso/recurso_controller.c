// Cabeçalho do controller de recursos
#include "recurso_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Model, View e utilitários do sistema
#include "model/recurso/recurso_model.h"
#include "view/recurso/recurso_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Adiciona um novo recurso ao sistema
void adicionarRecursoController(Sistema *sistema) {
    // Verifica e expande a capacidade da lista, se necessário
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

    // Cria e preenche os dados do novo recurso
    Recurso *novo_recurso = &sistema->lista_recursos[sistema->num_recursos];
    novo_recurso->codigo = sistema->num_recursos + 1;

    printf("\n--- Cadastro de Novo Recurso (Codigo: %d) ---\n", novo_recurso->codigo);
    printf("Descricao: ");
    ler_texto_valido(novo_recurso->descricao, sizeof(novo_recurso->descricao), VALIDAR_NAO_VAZIO);

    printf("Categoria: ");
    ler_texto_valido(novo_recurso->categoria, sizeof(novo_recurso->categoria), VALIDAR_NAO_VAZIO);

    printf("Quantidade em Estoque: ");
    int estoque;
    ler_inteiro_valido(&estoque, 0, 9999);
    novo_recurso->quantidade_estoque = estoque;

    printf("Preco de Custo (unitario): R$ ");
    ler_float_positivo(&novo_recurso->preco_custo);

    printf("Valor da Locacao (diaria): R$ ");
    ler_float_positivo(&novo_recurso->valor_locacao);

    sistema->num_recursos++;
    salvarRecursos(sistema);
    printf("\nRecurso '%s' cadastrado com sucesso!\n", novo_recurso->descricao);
}

// Permite alterar dados de um recurso existente
void alterarRecursoController(Sistema *sistema) {
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;

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
        ler_inteiro_valido(&opcao, 0, 5);

        // Executa a opção escolhida
        switch (opcao) {
            case 1:
                printf("Nova Descricao: ");
                ler_texto_valido(recurso->descricao, sizeof(recurso->descricao), VALIDAR_NAO_VAZIO);
                break;
            case 2:
                printf("Nova Categoria: ");
                ler_texto_valido(recurso->categoria, sizeof(recurso->categoria), VALIDAR_NAO_VAZIO);
                break;
            case 3:
                printf("Nova Quantidade em Estoque: ");
                int estoque;
                ler_inteiro_valido(&estoque, 0, 9999);
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

    salvarRecursos(sistema);
}

// Exclui um recurso do sistema
void excluirRecursoController(Sistema *sistema) {
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;

    int codigo;
    printf("\nDigite o codigo do recurso para excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o recurso e pede confirmação antes da exclusão
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
