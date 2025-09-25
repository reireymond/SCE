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

    printf("Razao Social: ");
    fgets(sistema->dados_produtora->razao_social, 100, stdin);
    sistema->dados_produtora->razao_social[strcspn(sistema->dados_produtora->razao_social, "\n")] = 0;

    printf("Nome do Responsavel: ");
    fgets(sistema->dados_produtora->nome_do_responsavel, 100, stdin);
    sistema->dados_produtora->nome_do_responsavel[strcspn(sistema->dados_produtora->nome_do_responsavel, "\n")] = 0;

    printf("CNPJ: ");
    fgets(sistema->dados_produtora->cnpj, 20, stdin);
    sistema->dados_produtora->cnpj[strcspn(sistema->dados_produtora->cnpj, "\n")] = 0;

    printf("Inscricao Estadual: ");
    fgets(sistema->dados_produtora->inscricao_estadual, 20, stdin);
    sistema->dados_produtora->inscricao_estadual[strcspn(sistema->dados_produtora->inscricao_estadual, "\n")] = 0;

    printf("Endereco: ");
    fgets(sistema->dados_produtora->endereco, 150, stdin);
    sistema->dados_produtora->endereco[strcspn(sistema->dados_produtora->endereco, "\n")] = 0;

    printf("Telefone: ");
    fgets(sistema->dados_produtora->telefone, 20, stdin);
    sistema->dados_produtora->telefone[strcspn(sistema->dados_produtora->telefone, "\n")] = 0;

    printf("Telefone do Responsavel: ");
    fgets(sistema->dados_produtora->telefone_responsavel, 20, stdin);
    sistema->dados_produtora->telefone_responsavel[strcspn(sistema->dados_produtora->telefone_responsavel, "\n")] = 0;

    printf("E-mail: ");
    fgets(sistema->dados_produtora->email, 50, stdin);
    sistema->dados_produtora->email[strcspn(sistema->dados_produtora->email, "\n")] = 0;

    printf("Margem de Lucro (%%): ");
    scanf("%f", &sistema->dados_produtora->margem_lucro);
    limpar_buffer();

    salvarProdutora(sistema);
    printf("\nDados da produtora cadastrados com sucesso!\n");
}

void alterarProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para alterar.\n");
        return;
    }

    int opcao;
    do {
        limpar_tela();
        printf("--- Alterar Dados da Produtora ---\n");
        verDetalhesProdutoraView(sistema); // Mostra os dados atuais

        printf("\n\nQual campo deseja alterar?\n");
        printf("1. Nome Fantasia\n2. Razao Social\n3. Nome do Responsavel\n4. CNPJ\n5. Inscricao Estadual\n");
        printf("6. Endereco\n7. Telefone\n8. Telefone do Responsavel\n9. E-mail\n10. Margem de Lucro\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                printf("Novo Nome Fantasia: ");
                fgets(sistema->dados_produtora->nome_fantasia, 100, stdin);
                sistema->dados_produtora->nome_fantasia[strcspn(sistema->dados_produtora->nome_fantasia, "\n")] = 0;
                break;
            // Adicione os outros cases aqui... (ex: case 2: ... case 10: ...)
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                pausar();
                break;
        }
    } while (opcao != 0);

    salvarProdutora(sistema);
}

void excluirProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para excluir.\n");
        return;
    }

    char confirmacao;
    printf("\nTem certeza que deseja excluir os dados da produtora %s? (s/n): ", sistema->dados_produtora->nome_fantasia);
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