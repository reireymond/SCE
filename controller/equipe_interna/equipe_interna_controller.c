#include "equipe_interna_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/equipe_interna/equipe_interna_model.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

static int obterProximoIdEquipe(Sistema *sistema) {
    int max = 0;
    for(int i=0; i<sistema->num_equipe; i++) {
        if(sistema->lista_equipe[i].codigo > max) max = sistema->lista_equipe[i].codigo;
    }
    return max + 1;
}

void adicionarEquipeController(Sistema *sistema) {
    if (sistema->num_equipe == sistema->capacidade_equipe) {
        int nova_cap = (sistema->capacidade_equipe == 0) ? 10 : sistema->capacidade_equipe * 2;
        EquipeInterna *temp = realloc(sistema->lista_equipe, nova_cap * sizeof(EquipeInterna));
        if (!temp) return;
        sistema->lista_equipe = temp;
        sistema->capacidade_equipe = nova_cap;
    }

    EquipeInterna *novo = &sistema->lista_equipe[sistema->num_equipe];
    novo->codigo = obterProximoIdEquipe(sistema); // CORREÇÃO DE ID

    printf("\n--- Novo Membro (ID: %d) ---\n", novo->codigo);
    printf("Nome: "); ler_texto_valido(novo->nome, sizeof(novo->nome), VALIDAR_NOME);
    printf("CPF: "); ler_texto_valido(novo->cpf, sizeof(novo->cpf), VALIDAR_CPF);
    printf("Funcao: "); ler_texto_valido(novo->funcao, sizeof(novo->funcao), VALIDAR_NAO_VAZIO);
    printf("Valor Diaria: R$ "); ler_float_positivo(&novo->valor_diaria);

    sistema->num_equipe++;
    salvarEquipeInterna(sistema);
    printf("\nMembro cadastrado!\n");
}

void alterarEquipeController(Sistema *sistema) {
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return;
    int codigo, indice = -1;
    printf("\nID para alterar: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) { indice = i; break; }
    }
    if (indice == -1) { printf("Nao encontrado.\n"); return; }
    
    EquipeInterna *e = &sistema->lista_equipe[indice];
    int opcao;
    do {
        limpar_tela();
        printf("--- Editando: %s ---\n", e->nome);
        printf("1. Nome\n2. CPF\n3. Funcao\n4. Valor Diaria\n0. Sair\nEscolha: ");
        ler_inteiro_valido(&opcao, 0, 4);
        switch(opcao) {
            case 1: printf("Novo Nome: "); ler_texto_valido(e->nome, sizeof(e->nome), VALIDAR_NOME); break;
            case 2: printf("Novo CPF: "); ler_texto_valido(e->cpf, sizeof(e->cpf), VALIDAR_CPF); break;
            case 3: printf("Nova Funcao: "); ler_texto_valido(e->funcao, sizeof(e->funcao), VALIDAR_NAO_VAZIO); break;
            case 4: printf("Nova Diaria: R$ "); ler_float_positivo(&e->valor_diaria); break;
            case 0: printf("Salvo.\n"); break;
        }
    } while(opcao != 0);
    salvarEquipeInterna(sistema);
}

void excluirEquipeController(Sistema *sistema) {
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return;
    int codigo;
    printf("\nID para excluir: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) {
            char conf; printf("Excluir %s? (s/n): ", sistema->lista_equipe[i].nome);
            scanf(" %c", &conf); limpar_buffer();
            if (conf == 's' || conf == 'S') {
                sistema->lista_equipe[i] = sistema->lista_equipe[sistema->num_equipe - 1];
                sistema->num_equipe--;
                salvarEquipeInterna(sistema);
                printf("\nExcluido!\n");
            } else printf("Cancelado.\n");
            return;
        }
    }
    printf("Nao encontrado.\n");
}