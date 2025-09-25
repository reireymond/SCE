#include "produtora_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/produtora/produtora_controller.h"

void menuProdutoraView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("\n--- Menu Produtora ---\n");
        printf("1. Cadastrar Dados\n");
        printf("2. Alterar Dados\n");
        printf("3. Ver Detalhes\n");
        printf("4. Excluir Dados\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarProdutoraController(sistema); break;
            case 2: alterarProdutoraController(sistema); break;
            case 3: verDetalhesProdutoraView(sistema); break;
            case 4: excluirProdutoraController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void verDetalhesProdutoraView(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para exibir.\n");
        return;
    }
    printf("\n--- Detalhes da Produtora Cadastrada ---\n");
    printf("Nome Fantasia: %s\n", sistema->dados_produtora->nome_fantasia);
    printf("Razao Social: %s\n", sistema->dados_produtora->razao_social);
    printf("Nome do Responsavel: %s\n", sistema->dados_produtora->nome_do_responsavel);
    printf("CNPJ: %s\n", sistema->dados_produtora->cnpj);
    printf("Inscricao Estadual: %s\n", sistema->dados_produtora->inscricao_estadual);
    printf("Endereco: %s\n", sistema->dados_produtora->endereco);
    printf("Telefone: %s\n", sistema->dados_produtora->telefone);
    printf("Telefone do Responsavel: %s\n", sistema->dados_produtora->telefone_responsavel);
    printf("E-mail: %s\n", sistema->dados_produtora->email);
    printf("Margem de Lucro: %.2f%%\n", sistema->dados_produtora->margem_lucro);
}