#include "produtora_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"
#include "model/produtora/produtora.h"
#include "view/produtora/produtora_view.h"

void adicionarProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora != NULL) {
        printf("\nUma produtora ja foi cadastrada. Exclua a existente para cadastrar uma nova.\n");
        return;
    }

    sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
    if (sistema->dados_produtora == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria.\n");
        return;
    }

    printf("\n--- Cadastro dos Dados da Produtora ---\n");
    printf("Nome Fantasia: ");
    fgets(sistema->dados_produtora->nome_fantasia, 100, stdin);
    sistema->dados_produtora->nome_fantasia[strcspn(sistema->dados_produtora->nome_fantasia, "\n")] = 0;
    // ... (resto dos fgets para pegar os dados)
    printf("Margem de Lucro (%%): ");
    scanf("%f", &sistema->dados_produtora->margem_lucro);
    limpar_buffer();

    salvarProdutora(sistema);
    printf("\nDados da produtora cadastrados com sucesso!\n");
}

void alterarProdutoraController(Sistema *sistema) {
    // A lógica de alteração do seu arquivo original viria aqui
    printf("\nFuncao de alterar ainda nao implementada.\n");
}

void excluirProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para excluir.\n");
        return;
    }

    char confirmacao;
    printf("\nTem certeza? (s/n): ");
    scanf(" %c", &confirmacao);
    limpar_buffer();

    if (confirmacao == 's' || confirmacao == 'S') {
        free(sistema->dados_produtora);
        sistema->dados_produtora = NULL;
        salvarProdutora(sistema);
        printf("\nDados da produtora excluidos com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
}