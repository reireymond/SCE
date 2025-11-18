#include "fornecedor_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/fornecedor/fornecedor_model.h"
#include "view/fornecedor/fornecedor_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

static int obterProximoIdFornecedor(Sistema *sistema) {
    int max = 0;
    for(int i=0; i<sistema->num_fornecedores; i++) {
        if(sistema->lista_fornecedores[i].codigo > max) max = sistema->lista_fornecedores[i].codigo;
    }
    return max + 1;
}

void adicionarFornecedorController(Sistema *sistema) {
    if (sistema->num_fornecedores == sistema->capacidade_fornecedores) {
        int nova_cap = (sistema->capacidade_fornecedores == 0) ? 10 : sistema->capacidade_fornecedores * 2;
        Fornecedor *temp = realloc(sistema->lista_fornecedores, nova_cap * sizeof(Fornecedor));
        if (!temp) { printf("\nErro memoria!\n"); return; }
        sistema->lista_fornecedores = temp;
        sistema->capacidade_fornecedores = nova_cap;
    }

    Fornecedor *novo = &sistema->lista_fornecedores[sistema->num_fornecedores];
    novo->codigo = obterProximoIdFornecedor(sistema); // CORREÇÃO DE ID

    printf("\n--- Novo Fornecedor (ID: %d) ---\n", novo->codigo);
    printf("Nome Fantasia: "); ler_texto_valido(novo->nome_fantasia, sizeof(novo->nome_fantasia), VALIDAR_NAO_VAZIO);
    printf("Razao Social: "); ler_texto_valido(novo->razao_social, sizeof(novo->razao_social), VALIDAR_NAO_VAZIO);
    printf("CNPJ/CPF: "); ler_texto_valido(novo->cnpj, sizeof(novo->cnpj), VALIDAR_CPF_CNPJ);
    printf("Endereco: "); ler_texto_valido(novo->endereco, sizeof(novo->endereco), VALIDAR_NAO_VAZIO);
    printf("Telefone: "); ler_texto_valido(novo->telefone, sizeof(novo->telefone), VALIDAR_TELEFONE);
    printf("Tipo Servico: "); ler_texto_valido(novo->tipo_servico, sizeof(novo->tipo_servico), VALIDAR_NAO_VAZIO);

    sistema->num_fornecedores++;
    salvarFornecedores(sistema);
    printf("\nFornecedor cadastrado!\n");
}

void alterarFornecedorController(Sistema *sistema) {
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return;

    int codigo, indice = -1;
    printf("\nID para alterar: "); scanf("%d", &codigo); limpar_buffer();

    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) { indice = i; break; }
    }

    if (indice == -1) { printf("\nNao encontrado.\n"); return; }
    Fornecedor *f = &sistema->lista_fornecedores[indice];
    int opcao;
    do {
        limpar_tela();
        printf("--- Alterando: %s ---\n", f->nome_fantasia);
        printf("1. Nome\n2. Razao\n3. CNPJ\n4. Endereco\n5. Telefone\n6. Servico\n0. Sair\nEscolha: ");
        ler_inteiro_valido(&opcao, 0, 6);
        switch(opcao) {
            case 1: printf("Novo Nome: "); ler_texto_valido(f->nome_fantasia, sizeof(f->nome_fantasia), VALIDAR_NAO_VAZIO); break;
            case 2: printf("Nova Razao: "); ler_texto_valido(f->razao_social, sizeof(f->razao_social), VALIDAR_NAO_VAZIO); break;
            case 3: printf("Novo CNPJ: "); ler_texto_valido(f->cnpj, sizeof(f->cnpj), VALIDAR_CPF_CNPJ); break;
            case 4: printf("Novo Endereco: "); ler_texto_valido(f->endereco, sizeof(f->endereco), VALIDAR_NAO_VAZIO); break;
            case 5: printf("Novo Tel: "); ler_texto_valido(f->telefone, sizeof(f->telefone), VALIDAR_TELEFONE); break;
            case 6: printf("Novo Servico: "); ler_texto_valido(f->tipo_servico, sizeof(f->tipo_servico), VALIDAR_NAO_VAZIO); break;
            case 0: printf("Salvo.\n"); break;
        }
    } while(opcao != 0);
    salvarFornecedores(sistema);
}

void excluirFornecedorController(Sistema *sistema) {
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return;
    int codigo;
    printf("\nID para excluir: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) {
            char conf; printf("Excluir %s? (s/n): ", sistema->lista_fornecedores[i].nome_fantasia);
            scanf(" %c", &conf); limpar_buffer();
            if (conf == 's' || conf == 'S') {
                sistema->lista_fornecedores[i] = sistema->lista_fornecedores[sistema->num_fornecedores - 1];
                sistema->num_fornecedores--;
                salvarFornecedores(sistema);
                printf("\nExcluido!\n");
            } else printf("Cancelado.\n");
            return;
        }
    }
    printf("Nao encontrado.\n");
}