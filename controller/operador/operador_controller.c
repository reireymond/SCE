#include "operador_controller.h"
#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>  
#include "model/operador/operador_model.h"
#include "view/operador/operador_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

static int obterProximoIdOperador(Sistema *sistema) {
    int max = 0;
    for(int i=0; i<sistema->num_operadores; i++) {
        if(sistema->lista_operadores[i].codigo > max) max = sistema->lista_operadores[i].codigo;
    }
    return max + 1;
}

void adicionarOperadorController(Sistema *sistema) {
    if (sistema->num_operadores == sistema->capacidade_operadores) {
        int nova_cap = (sistema->capacidade_operadores == 0) ? 10 : sistema->capacidade_operadores * 2;
        Operador *temp = realloc(sistema->lista_operadores, nova_cap * sizeof(Operador));
        if (!temp) return;
        sistema->lista_operadores = temp;
        sistema->capacidade_operadores = nova_cap;
    }

    Operador *novo = &sistema->lista_operadores[sistema->num_operadores];
    novo->codigo = obterProximoIdOperador(sistema); // CORREÇÃO DE ID

    printf("\n--- Novo Operador (ID: %d) ---\n", novo->codigo);
    printf("Nome: "); ler_texto_valido(novo->nome, sizeof(novo->nome), VALIDAR_NOME);
    printf("Usuario: "); ler_texto_valido(novo->usuario, sizeof(novo->usuario), VALIDAR_NAO_VAZIO);
    printf("Senha: "); ler_texto_valido(novo->senha, sizeof(novo->senha), VALIDAR_NAO_VAZIO);

    sistema->num_operadores++;
    salvarOperadores(sistema);
    printf("\nOperador cadastrado!\n");
}

void alterarOperadorController(Sistema *sistema) {
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;
    int codigo, indice = -1;
    printf("\nID para alterar: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) { indice = i; break; }
    }
    if (indice == -1) { printf("Nao encontrado.\n"); return; }

    Operador *o = &sistema->lista_operadores[indice];
    int opcao;
    do {
        limpar_tela();
        printf("--- Editando: %s ---\n", o->usuario);
        printf("1. Nome\n2. Usuario\n3. Senha\n0. Sair\nEscolha: ");
        ler_inteiro_valido(&opcao, 0, 3);
        switch(opcao) {
            case 1: printf("Novo Nome: "); ler_texto_valido(o->nome, sizeof(o->nome), VALIDAR_NOME); break;
            case 2: printf("Novo Usuario: "); ler_texto_valido(o->usuario, sizeof(o->usuario), VALIDAR_NAO_VAZIO); break;
            case 3: printf("Nova Senha: "); ler_texto_valido(o->senha, sizeof(o->senha), VALIDAR_NAO_VAZIO); break;
            case 0: printf("Salvo.\n"); break;
        }
    } while(opcao != 0);
    salvarOperadores(sistema);
}

void excluirOperadorController(Sistema *sistema) {
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;
    int codigo;
    printf("\nID para excluir: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            char conf; printf("Excluir %s? (s/n): ", sistema->lista_operadores[i].usuario);
            scanf(" %c", &conf); limpar_buffer();
            if (conf == 's' || conf == 'S') {
                sistema->lista_operadores[i] = sistema->lista_operadores[sistema->num_operadores - 1];
                sistema->num_operadores--;
                salvarOperadores(sistema);
                printf("\nExcluido!\n");
            } else printf("Cancelado.\n");
            return;
        }
    }
    printf("Nao encontrado.\n");
}