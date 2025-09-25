#include "controller/produtora/produtora_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"
#include "model/produtora/produtora_model.h"
#include "view/produtora/produtora_view.h"

void adicionarProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora != NULL) {
        printf("\nUma produtora ja foi cadastrada. Exclua a existente para cadastrar uma nova.\n");
        return;
    }

    sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
    if (sistema->dados_produtora == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria para a produtora.\n");
        return;
    }

    printf("\n--- Cadastro dos Dados da Produtora ---\n");

    printf("Nome Fantasia: ");
    fgets(sistema->dados_produtora->nome_fantasia, sizeof(sistema->dados_produtora->nome_fantasia), stdin);
    sistema->dados_produtora->nome_fantasia[strcspn(sistema->dados_produtora->nome_fantasia, "\n")] = 0;

    printf("Razao Social: ");
    fgets(sistema->dados_produtora->razao_social, sizeof(sistema->dados_produtora->razao_social), stdin);
    sistema->dados_produtora->razao_social[strcspn(sistema->dados_produtora->razao_social, "\n")] = 0;

    printf("Nome do Responsavel: ");
    fgets(sistema->dados_produtora->nome_do_responsavel, sizeof(sistema->dados_produtora->nome_do_responsavel), stdin);
    sistema->dados_produtora->nome_do_responsavel[strcspn(sistema->dados_produtora->nome_do_responsavel, "\n")] = 0;

    printf("CNPJ: ");
    fgets(sistema->dados_produtora->cnpj, sizeof(sistema->dados_produtora->cnpj), stdin);
    sistema->dados_produtora->cnpj[strcspn(sistema->dados_produtora->cnpj, "\n")] = 0;

    printf("Inscricao Estadual: ");
    fgets(sistema->dados_produtora->inscricao_estadual, sizeof(sistema->dados_produtora->inscricao_estadual), stdin);
    sistema->dados_produtora->inscricao_estadual[strcspn(sistema->dados_produtora->inscricao_estadual, "\n")] = 0;

    printf("Endereco: ");
    fgets(sistema->dados_produtora->endereco, sizeof(sistema->dados_produtora->endereco), stdin);
    sistema->dados_produtora->endereco[strcspn(sistema->dados_produtora->endereco, "\n")] = 0;

    printf("Telefone: ");
    fgets(sistema->dados_produtora->telefone, sizeof(sistema->dados_produtora->telefone), stdin);
    sistema->dados_produtora->telefone[strcspn(sistema->dados_produtora->telefone, "\n")] = 0;

    printf("Telefone do Responsavel: ");
    fgets(sistema->dados_produtora->telefone_responsavel, sizeof(sistema->dados_produtora->telefone_responsavel), stdin);
    sistema->dados_produtora->telefone_responsavel[strcspn(sistema->dados_produtora->telefone_responsavel, "\n")] = 0;

    printf("E-mail: ");
    fgets(sistema->dados_produtora->email, sizeof(sistema->dados_produtora->email), stdin);
    sistema->dados_produtora->email[strcspn(sistema->dados_produtora->email, "\n")] = 0;

    printf("Margem de Lucro (%%): ");
    scanf("%f", &sistema->dados_produtora->margem_lucro);
    limpar_buffer();

    salvarProdutora(sistema);
    printf("\nDados da produtora '%s' cadastrados com sucesso!\n", sistema->dados_produtora->nome_fantasia);
}

void alterarProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para alterar. Por favor, adicione uma primeiro.\n");
        return;
    }

    int opcao;
    do {
        limpar_tela();
        printf("--- Alterar Dados da Produtora ---\n\n");
        verDetalhesProdutoraView(sistema); // Chama a view para mostrar os dados

        printf("\n\nQual campo deseja alterar?\n");
        printf("1. Nome Fantasia\n2. Razao Social\n3. Nome do Responsavel\n4. CNPJ\n5. Inscricao Estadual\n");
        printf("6. Endereco\n7. Telefone\n8. Telefone do Responsavel\n9. E-mail\n10. Margem de Lucro\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                printf("Digite o novo Nome Fantasia: ");
                fgets(sistema->dados_produtora->nome_fantasia, sizeof(sistema->dados_produtora->nome_fantasia), stdin);
                sistema->dados_produtora->nome_fantasia[strcspn(sistema->dados_produtora->nome_fantasia, "\n")] = 0;
                break;
            case 2:
                printf("Digite a nova Razao Social: ");
                fgets(sistema->dados_produtora->razao_social, sizeof(sistema->dados_produtora->razao_social), stdin);
                sistema->dados_produtora->razao_social[strcspn(sistema->dados_produtora->razao_social, "\n")] = 0;
                break;
            case 3:
                printf("Digite o novo Nome do Responsavel: ");
                fgets(sistema->dados_produtora->nome_do_responsavel, sizeof(sistema->dados_produtora->nome_do_responsavel), stdin);
                sistema->dados_produtora->nome_do_responsavel[strcspn(sistema->dados_produtora->nome_do_responsavel, "\n")] = 0;
                break;
            case 4:
                printf("Digite o novo CNPJ: ");
                fgets(sistema->dados_produtora->cnpj, sizeof(sistema->dados_produtora->cnpj), stdin);
                sistema->dados_produtora->cnpj[strcspn(sistema->dados_produtora->cnpj, "\n")] = 0;
                break;
            case 5:
                printf("Digite a nova Inscricao Estadual: ");
                fgets(sistema->dados_produtora->inscricao_estadual, sizeof(sistema->dados_produtora->inscricao_estadual), stdin);
                sistema->dados_produtora->inscricao_estadual[strcspn(sistema->dados_produtora->inscricao_estadual, "\n")] = 0;
                break;
            case 6:
                printf("Digite o novo Endereco: ");
                fgets(sistema->dados_produtora->endereco, sizeof(sistema->dados_produtora->endereco), stdin);
                sistema->dados_produtora->endereco[strcspn(sistema->dados_produtora->endereco, "\n")] = 0;
                break;
            case 7:
                printf("Digite o novo Telefone: ");
                fgets(sistema->dados_produtora->telefone, sizeof(sistema->dados_produtora->telefone), stdin);
                sistema->dados_produtora->telefone[strcspn(sistema->dados_produtora->telefone, "\n")] = 0;
                break;
            case 8:
                printf("Digite o novo Telefone do Responsavel: ");
                fgets(sistema->dados_produtora->telefone_responsavel, sizeof(sistema->dados_produtora->telefone_responsavel), stdin);
                sistema->dados_produtora->telefone_responsavel[strcspn(sistema->dados_produtora->telefone_responsavel, "\n")] = 0;
                break;
            case 9:
                printf("Digite o novo E-mail: ");
                fgets(sistema->dados_produtora->email, sizeof(sistema->dados_produtora->email), stdin);
                sistema->dados_produtora->email[strcspn(sistema->dados_produtora->email, "\n")] = 0;
                break;
            case 10:
                printf("Digite a nova Margem de Lucro (%%): ");
                scanf("%f", &sistema->dados_produtora->margem_lucro);
                limpar_buffer();
                break;
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
        salvarProdutora(sistema); // Salva o estado "vazio"
        printf("\nDados da produtora excluidos com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
}