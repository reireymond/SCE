// Inclui o cabeçalho do controller, com as declarações das funções deste módulo
#include "equipe_interna_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/equipe_interna/equipe_interna_model.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Adiciona um novo membro à equipe
void adicionarEquipeController(Sistema *sistema) {
    // Expande o vetor caso atinja a capacidade máxima
    if (sistema->num_equipe == sistema->capacidade_equipe) {
        int nova_capacidade = (sistema->capacidade_equipe == 0) ? 10 : sistema->capacidade_equipe * 2;
        EquipeInterna *temp = realloc(sistema->lista_equipe, nova_capacidade * sizeof(EquipeInterna));

        if (temp == NULL) {
            printf("\nErro de alocacao de memoria!\n");
            return;
        }
        sistema->lista_equipe = temp;
        sistema->capacidade_equipe = nova_capacidade;
    }

    // Ponteiro para o próximo membro livre
    EquipeInterna *novo_membro = &sistema->lista_equipe[sistema->num_equipe];
    novo_membro->codigo = sistema->num_equipe + 1;

    printf("\n--- Cadastro de Novo Membro (Codigo: %d) ---\n", novo_membro->codigo);

    // Entrada e validação dos dados
    printf("Nome: ");
    ler_texto_valido(novo_membro->nome, sizeof(novo_membro->nome), VALIDAR_NOME);

    printf("CPF: ");
    ler_texto_valido(novo_membro->cpf, sizeof(novo_membro->cpf), VALIDAR_CPF);

    printf("Funcao: ");
    ler_texto_valido(novo_membro->funcao, sizeof(novo_membro->funcao), VALIDAR_NAO_VAZIO);

    printf("Valor da diaria: R$ ");
    ler_float_positivo(&novo_membro->valor_diaria);

    sistema->num_equipe++;
    salvarEquipeInterna(sistema);

    printf("\nMembro '%s' cadastrado com sucesso!\n", novo_membro->nome);
}

// Altera os dados de um membro existente
void alterarEquipeController(Sistema *sistema) {
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do membro que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Busca o membro pelo código
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

    EquipeInterna *membro = &sistema->lista_equipe[indice];
    int opcao;

    // Menu de edição
    do {
        limpar_tela();
        printf("--- Alterando Membro: %s ---\n", membro->nome);
        printf("1. Alterar Nome\n");
        printf("2. Alterar CPF\n");
        printf("3. Alterar Funcao\n");
        printf("4. Alterar Valor da Diaria\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        ler_inteiro_valido(&opcao, 0, 4);

        switch (opcao) {
            case 1:
                printf("Novo Nome: ");
                ler_texto_valido(membro->nome, sizeof(membro->nome), VALIDAR_NOME);
                break;
            case 2:
                printf("Novo CPF: ");
                ler_texto_valido(membro->cpf, sizeof(membro->cpf), VALIDAR_CPF);
                break;
            case 3:
                printf("Nova Funcao: ");
                ler_texto_valido(membro->funcao, sizeof(membro->funcao), VALIDAR_NAO_VAZIO);
                break;
            case 4:
                printf("Novo Valor da Diaria: R$ ");
                ler_float_positivo(&membro->valor_diaria);
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);

    salvarEquipeInterna(sistema);
}

// Exclui um membro da equipe
void excluirEquipeController(Sistema *sistema) {
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return;

    int codigo;
    printf("\nDigite o codigo do membro que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o membro pelo código
    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o membro %s? (s/n): ", sistema->lista_equipe[i].nome);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Substitui o excluído pelo último da lista
                sistema->lista_equipe[i] = sistema->lista_equipe[sistema->num_equipe - 1];
                sistema->num_equipe--;
                salvarEquipeInterna(sistema);
                printf("\nMembro excluido com sucesso!\n");
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }

    printf("\nMembro com codigo %d nao encontrado.\n", codigo);
}
