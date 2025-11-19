#include "produtora_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/produtora/produtora_controller.h"

void menuProdutoraView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|                   MENU PRODUTORA                    |\n");
        printf("+=====================================================+\n");
        printf("| [1] Cadastrar Dados da Produtora                    |\n");
        printf("| [2] Alterar Dados                                   |\n");
        printf("| [3] Ver Detalhes                                    |\n");
        printf("| [4] Excluir Dados                                   |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
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

// Mantive a função de ver detalhes igual, pois ela é uma lista e não um menu
void verDetalhesProdutoraView(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para exibir.\n");
        return;
    }
    printf("+=====================================================+\n");
    printf("|          DETALHES DA PRODUTORA CADASTRADA           |\n");
    printf("+=====================================================+\n");
    printf("| Nome Fantasia: %-36s |\n", sistema->dados_produtora->nome_fantasia);
    printf("| Razao Social:  %-36s |\n", sistema->dados_produtora->razao_social);
    printf("| Responsavel:   %-36s |\n", sistema->dados_produtora->nome_do_responsavel);
    printf("| CNPJ:          %-36s |\n", sistema->dados_produtora->cnpj);
    printf("| Endereco:      %-36s |\n", sistema->dados_produtora->endereco);
    printf("| Telefone:      %-36s |\n", sistema->dados_produtora->telefone);
    printf("| Email:         %-36s |\n", sistema->dados_produtora->email);
    printf("| Margem Lucro:  %-35.2f%% |\n", sistema->dados_produtora->margem_lucro);
    printf("+=====================================================+\n");
}