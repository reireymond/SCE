#include "recurso_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/recurso/recurso_model.h"
#include "view/recurso/recurso_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

static int obterProximoIdRecurso(Sistema *sistema) {
    int max = 0;
    for(int i=0; i<sistema->num_recursos; i++) {
        if(sistema->lista_recursos[i].codigo > max) max = sistema->lista_recursos[i].codigo;
    }
    return max + 1;
}

void adicionarRecursoController(Sistema *sistema) {
    if (sistema->num_recursos == sistema->capacidade_recursos) {
        int nova_cap = (sistema->capacidade_recursos == 0) ? 10 : sistema->capacidade_recursos * 2;
        Recurso *temp = realloc(sistema->lista_recursos, nova_cap * sizeof(Recurso));
        if (!temp) return;
        sistema->lista_recursos = temp;
        sistema->capacidade_recursos = nova_cap;
    }

    Recurso *novo = &sistema->lista_recursos[sistema->num_recursos];
    novo->codigo = obterProximoIdRecurso(sistema); // CORREÇÃO DE ID

    printf("\n--- Novo Recurso (ID: %d) ---\n", novo->codigo);
    printf("Descricao: "); ler_texto_valido(novo->descricao, sizeof(novo->descricao), VALIDAR_NAO_VAZIO);
    printf("Categoria: "); ler_texto_valido(novo->categoria, sizeof(novo->categoria), VALIDAR_NAO_VAZIO);
    printf("Estoque: "); ler_inteiro_valido(&novo->quantidade_estoque, 0, 9999);
    printf("Custo: R$ "); ler_float_positivo(&novo->preco_custo);
    printf("Locacao: R$ "); ler_float_positivo(&novo->valor_locacao);

    sistema->num_recursos++;
    salvarRecursos(sistema);
    printf("\nRecurso cadastrado!\n");
}

void alterarRecursoController(Sistema *sistema) {
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;
    int codigo, indice = -1;
    printf("\nID para alterar: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) { indice = i; break; }
    }
    if (indice == -1) { printf("Nao encontrado.\n"); return; }

    Recurso *r = &sistema->lista_recursos[indice];
    int opcao;
    do {
        limpar_tela();
        printf("--- Editando: %s ---\n", r->descricao);
        printf("1. Descricao\n2. Categoria\n3. Estoque\n4. Custo\n5. Locacao\n0. Sair\nEscolha: ");
        ler_inteiro_valido(&opcao, 0, 5);
        switch(opcao) {
            case 1: printf("Nova Descricao: "); ler_texto_valido(r->descricao, sizeof(r->descricao), VALIDAR_NAO_VAZIO); break;
            case 2: printf("Nova Categoria: "); ler_texto_valido(r->categoria, sizeof(r->categoria), VALIDAR_NAO_VAZIO); break;
            case 3: printf("Novo Estoque: "); ler_inteiro_valido(&r->quantidade_estoque, 0, 9999); break;
            case 4: printf("Novo Custo: "); ler_float_positivo(&r->preco_custo); break;
            case 5: printf("Nova Locacao: "); ler_float_positivo(&r->valor_locacao); break;
            case 0: printf("Salvo.\n"); break;
        }
    } while(opcao != 0);
    salvarRecursos(sistema);
}

void excluirRecursoController(Sistema *sistema) {
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;
    int codigo;
    printf("\nID para excluir: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) {
            char conf; printf("Excluir %s? (s/n): ", sistema->lista_recursos[i].descricao);
            scanf(" %c", &conf); limpar_buffer();
            if (conf == 's' || conf == 'S') {
                sistema->lista_recursos[i] = sistema->lista_recursos[sistema->num_recursos - 1];
                sistema->num_recursos--;
                salvarRecursos(sistema);
                printf("\nExcluido!\n");
            } else printf("Cancelado.\n");
            return;
        }
    }
    printf("Nao encontrado.\n");
}