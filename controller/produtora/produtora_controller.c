// Controller da produtora
#include "controller/produtora/produtora_controller.h"

// Bibliotecas padrão do C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Model, View e utilitários do sistema
#include "model/produtora/produtora_model.h"
#include "view/produtora/produtora_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Adiciona os dados da produtora ao sistema
void adicionarProdutoraController(Sistema *sistema) {
    // Permite apenas uma produtora cadastrada
    if (sistema->dados_produtora != NULL) {
        printf("\nUma produtora ja foi cadastrada. Exclua a existente para cadastrar uma nova.\n");
        return;
    }

    // Cria espaco na memoria para os dados
    sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
    if (sistema->dados_produtora == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria para a produtora.\n");
        return;
    }

    printf("\n--- Cadastro dos Dados da Produtora ---\n");

    // Coleta os dados da produtora
    printf("Nome Fantasia: ");
    ler_texto_valido(sistema->dados_produtora->nome_fantasia, sizeof(sistema->dados_produtora->nome_fantasia), VALIDAR_NAO_VAZIO);

    printf("Razao Social: ");
    ler_texto_valido(sistema->dados_produtora->razao_social, sizeof(sistema->dados_produtora->razao_social), VALIDAR_NAO_VAZIO);

    printf("Nome do Responsavel: ");
    ler_texto_valido(sistema->dados_produtora->nome_do_responsavel, sizeof(sistema->dados_produtora->nome_do_responsavel), VALIDAR_NOME);

    printf("CNPJ (formato XX.XXX.XXX/XXXX-XX): ");
    ler_texto_valido(sistema->dados_produtora->cnpj, sizeof(sistema->dados_produtora->cnpj), VALIDAR_CNPJ);

    printf("Inscricao Estadual: ");
    ler_texto_valido(sistema->dados_produtora->inscricao_estadual, sizeof(sistema->dados_produtora->inscricao_estadual), VALIDAR_NAO_VAZIO);

    printf("Endereco: ");
    ler_texto_valido(sistema->dados_produtora->endereco, sizeof(sistema->dados_produtora->endereco), VALIDAR_NAO_VAZIO);

    printf("Telefone: ");
    ler_texto_valido(sistema->dados_produtora->telefone, sizeof(sistema->dados_produtora->telefone), VALIDAR_TELEFONE);

    printf("Telefone do Responsavel: ");
    ler_texto_valido(sistema->dados_produtora->telefone_responsavel, sizeof(sistema->dados_produtora->telefone_responsavel), VALIDAR_TELEFONE);

    printf("E-mail: ");
    ler_texto_valido(sistema->dados_produtora->email, sizeof(sistema->dados_produtora->email), VALIDAR_EMAIL);

    printf("Margem de Lucro (%%): ");
    ler_float_positivo(&sistema->dados_produtora->margem_lucro);

    // Salva os dados no arquivo
    salvarProdutora(sistema);
    printf("\nDados da produtora '%s' cadastrados com sucesso!\n", sistema->dados_produtora->nome_fantasia);
}

// Altera os dados da produtora
void alterarProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para alterar. Por favor, adicione uma primeiro.\n");
        return;
    }

    int opcao;
    do {
        limpar_tela();
        printf("--- Alterar Dados da Produtora ---\n\n");
        verDetalhesProdutoraView(sistema);

        printf("\n\nQual campo deseja alterar?\n");
        printf("1. Nome Fantasia\n2. Razao Social\n3. Nome do Responsavel\n4. CNPJ\n5. Inscricao Estadual\n");
        printf("6. Endereco\n7. Telefone\n8. Telefone do Responsavel\n9. E-mail\n10. Margem de Lucro\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        ler_inteiro_valido(&opcao, 0, 10);

        switch (opcao) {
            case 1:
                printf("Digite o novo Nome Fantasia: ");
                ler_texto_valido(sistema->dados_produtora->nome_fantasia, sizeof(sistema->dados_produtora->nome_fantasia), VALIDAR_NAO_VAZIO);
                break;
            case 2:
                printf("Digite a nova Razao Social: ");
                ler_texto_valido(sistema->dados_produtora->razao_social, sizeof(sistema->dados_produtora->razao_social), VALIDAR_NAO_VAZIO);
                break;
            case 3:
                printf("Digite o novo Nome do Responsavel: ");
                ler_texto_valido(sistema->dados_produtora->nome_do_responsavel, sizeof(sistema->dados_produtora->nome_do_responsavel), VALIDAR_NOME);
                break;
            case 4:
                printf("Digite o novo CNPJ (formato XX.XXX.XXX/XXXX-XX): ");
                ler_texto_valido(sistema->dados_produtora->cnpj, sizeof(sistema->dados_produtora->cnpj), VALIDAR_CNPJ);
                break;
            case 5:
                printf("Digite a nova Inscricao Estadual: ");
                ler_texto_valido(sistema->dados_produtora->inscricao_estadual, sizeof(sistema->dados_produtora->inscricao_estadual), VALIDAR_NAO_VAZIO);
                break;
            case 6:
                printf("Digite o novo Endereco: ");
                ler_texto_valido(sistema->dados_produtora->endereco, sizeof(sistema->dados_produtora->endereco), VALIDAR_NAO_VAZIO);
                break;
            case 7:
                printf("Digite o novo Telefone: ");
                ler_texto_valido(sistema->dados_produtora->telefone, sizeof(sistema->dados_produtora->telefone), VALIDAR_TELEFONE);
                break;
            case 8:
                printf("Digite o novo Telefone do Responsavel: ");
                ler_texto_valido(sistema->dados_produtora->telefone_responsavel, sizeof(sistema->dados_produtora->telefone_responsavel), VALIDAR_TELEFONE);
                break;
            case 9:
                printf("Digite o novo E-mail: ");
                ler_texto_valido(sistema->dados_produtora->email, sizeof(sistema->dados_produtora->email), VALIDAR_EMAIL);
                break;
            case 10:
                printf("Digite a nova Margem de Lucro (%%): ");
                ler_float_positivo(&sistema->dados_produtora->margem_lucro);
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

// Exclui os dados da produtora
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
