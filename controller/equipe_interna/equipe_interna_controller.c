#include "equipe_interna_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/equipe_interna/equipe_interna.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "utils/utils.h"

void adicionarEquipeController(Sistema *sistema) {
    if (sistema->num_equipe == sistema->capacidade_equipe) {
        int nova_capacidade = (sistema->capacidade_equipe == 0) ? 10 : sistema->capacidade_equipe * 2;
        EquipeInterna *temp = realloc(sistema->lista_equipe, nova_capacidade * sizeof(EquipeInterna));
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para equipe!\n");
            return;
        }
        sistema->lista_equipe = temp;
        sistema->capacidade_equipe = nova_capacidade;
    }

    EquipeInterna *novo_membro = &sistema->lista_equipe[sistema->num_equipe];
    novo_membro->codigo = sistema->num_equipe + 1;

    printf("\n--- Cadastro de Novo Membro (Codigo: %d) ---\n", novo_membro->codigo);
    printf("Nome: ");
    fgets(novo_membro->nome, sizeof(novo_membro->nome), stdin);
    novo_membro->nome[strcspn(novo_membro->nome, "\n")] = 0;

    printf("CPF: ");
    fgets(novo_membro->cpf, sizeof(novo_membro->cpf), stdin);
    novo_membro->cpf[strcspn(novo_membro->cpf, "\n")] = 0;

    printf("Funcao: ");
    fgets(novo_membro->funcao, sizeof(novo_membro->funcao), stdin);
    novo_membro->funcao[strcspn(novo_membro->funcao, "\n")] = 0;

    printf("Valor da diaria: R$ ");
    scanf("%f", &novo_membro->valor_diaria);
    limpar_buffer();

    sistema->num_equipe++;

    salvarEquipeInterna(sistema);
    printf("\nMembro '%s' cadastrado com sucesso!\n", novo_membro->nome);
}

void alterarEquipeController(Sistema *sistema) {
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do membro que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nMembro com codigo %d nao encontrado.\n", codigo);
        return;
    }
    
    // Lógica para alterar o membro selecionado (adapte do seu código antigo)
    printf("\nFuncao de alterar equipe ainda nao totalmente implementada.\n");
    salvarEquipeInterna(sistema);
}

void excluirEquipeController(Sistema *sistema) {
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return;

    int codigo;
    printf("\nDigite o codigo do membro que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o membro %s? (s/n): ", sistema->lista_equipe[i].nome);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                sistema->lista_equipe[i] = sistema->lista_equipe[sistema->num_equipe - 1];
                sistema->num_equipe--;
                printf("\nMembro excluido com sucesso!\n");
                salvarEquipeInterna(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }
    printf("\nMembro com codigo %d nao encontrado.\n", codigo);
}