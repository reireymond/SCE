#include "controller/produtora/produtora_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"
#include "model/produtora/produtora_model.h"
#include "view/produtora/produtora_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Função para adicionar uma produtora no sistema
void adicionarProdutoraController(Sistema *sistema) {
    // Verifica se já existe uma produtora cadastrada
    if (sistema->dados_produtora != NULL) {
        printf("\nUma produtora ja foi cadastrada. Exclua a existente para cadastrar uma nova.\n");
        return;
    }

    // Aloca memória para os dados da produtora
    sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
    if (sistema->dados_produtora == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria para a produtora.\n");
        return;
    }

    // Coleta os dados da produtora
    printf("\n--- Cadastro dos Dados da Produtora ---\n");

    printf("Nome Fantasia: ");
    ler_string_valida(sistema->dados_produtora->nome_fantasia, sizeof(sistema->dados_produtora->nome_fantasia), VALIDATE_NOT_EMPTY);

    printf("Razao Social: ");
    ler_string_valida(sistema->dados_produtora->razao_social, sizeof(sistema->dados_produtora->razao_social), VALIDATE_NOT_EMPTY);

    printf("Nome do Responsavel: ");
    ler_string_valida(sistema->dados_produtora->nome_do_responsavel, sizeof(sistema->dados_produtora->nome_do_responsavel), VALIDATE_NAME);

    printf("CNPJ (formato XX.XXX.XXX/XXXX-XX): ");
    ler_string_valida(sistema->dados_produtora->cnpj, sizeof(sistema->dados_produtora->cnpj), VALIDATE_CNPJ);

    printf("Inscricao Estadual: ");
    ler_string_valida(sistema->dados_produtora->inscricao_estadual, sizeof(sistema->dados_produtora->inscricao_estadual), VALIDATE_NOT_EMPTY);

    printf("Endereco: ");
    ler_string_valida(sistema->dados_produtora->endereco, sizeof(sistema->dados_produtora->endereco), VALIDATE_NOT_EMPTY);

    printf("Telefone: ");
    ler_string_valida(sistema->dados_produtora->telefone, sizeof(sistema->dados_produtora->telefone), VALIDATE_PHONE);

    printf("Telefone do Responsavel: ");
    ler_string_valida(sistema->dados_produtora->telefone_responsavel, sizeof(sistema->dados_produtora->telefone_responsavel), VALIDATE_PHONE);

    printf("E-mail: ");
    ler_string_valida(sistema->dados_produtora->email, sizeof(sistema->dados_produtora->email), VALIDATE_EMAIL);

    printf("Margem de Lucro (%%): ");
    ler_float_positivo(&sistema->dados_produtora->margem_lucro);

    // Salva os dados no arquivo
    salvarProdutora(sistema);
    printf("\nDados da produtora '%s' cadastrados com sucesso!\n", sistema->dados_produtora->nome_fantasia);
}

// Função para alterar os dados da produtora
void alterarProdutoraController(Sistema *sistema) {
    // Verifica se há produtora cadastrada
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para alterar. Por favor, adicione uma primeiro.\n");
        return;
    }

    int opcao;
    do {
        limpar_tela(); // Limpa a tela
        printf("--- Alterar Dados da Produtora ---\n\n");
        verDetalhesProdutoraView(sistema); // Mostra os dados atuais

        // Exibe o menu de opções de alteração
        printf("\n\nQual campo deseja alterar?\n");
        printf("1. Nome Fantasia\n2. Razao Social\n3. Nome do Responsavel\n4. CNPJ\n5. Inscricao Estadual\n");
        printf("6. Endereco\n7. Telefone\n8. Telefone do Responsavel\n9. E-mail\n10. Margem de Lucro\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        
        // Lê a opção escolhida
        ler_int_valido(&opcao, 0, 10);

        // Executa a alteração conforme a opção
        switch (opcao) {
            case 1:
                printf("Digite o novo Nome Fantasia: ");
                ler_string_valida(sistema->dados_produtora->nome_fantasia, sizeof(sistema->dados_produtora->nome_fantasia), VALIDATE_NOT_EMPTY);
                break;
            case 2:
                printf("Digite a nova Razao Social: ");
                ler_string_valida(sistema->dados_produtora->razao_social, sizeof(sistema->dados_produtora->razao_social), VALIDATE_NOT_EMPTY);
                break;
            case 3:
                printf("Digite o novo Nome do Responsavel: ");
                ler_string_valida(sistema->dados_produtora->nome_do_responsavel, sizeof(sistema->dados_produtora->nome_do_responsavel), VALIDATE_NAME);
                break;
            case 4:
                printf("Digite o novo CNPJ (formato XX.XXX.XXX/XXXX-XX): ");
                ler_string_valida(sistema->dados_produtora->cnpj, sizeof(sistema->dados_produtora->cnpj), VALIDATE_CNPJ);
                break;
            case 5:
                printf("Digite a nova Inscricao Estadual: ");
                ler_string_valida(sistema->dados_produtora->inscricao_estadual, sizeof(sistema->dados_produtora->inscricao_estadual), VALIDATE_NOT_EMPTY);
                break;
            case 6:
                printf("Digite o novo Endereco: ");
                ler_string_valida(sistema->dados_produtora->endereco, sizeof(sistema->dados_produtora->endereco), VALIDATE_NOT_EMPTY);
                break;
            case 7:
                printf("Digite o novo Telefone: ");
                ler_string_valida(sistema->dados_produtora->telefone, sizeof(sistema->dados_produtora->telefone), VALIDATE_PHONE);
                break;
            case 8:
                printf("Digite o novo Telefone do Responsavel: ");
                ler_string_valida(sistema->dados_produtora->telefone_responsavel, sizeof(sistema->dados_produtora->telefone_responsavel), VALIDATE_PHONE);
                break;
            case 9:
                printf("Digite o novo E-mail: ");
                ler_string_valida(sistema->dados_produtora->email, sizeof(sistema->dados_produtora->email), VALIDATE_EMAIL);
                break;
            case 10:
                printf("Digite a nova Margem de Lucro (%%): ");
                ler_float_positivo(&sistema->dados_produtora->margem_lucro);
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
            default:
                // Caso inválido (não deve ocorrer)
                printf("\nOpcao invalida!\n");
                pausar();
                break;
        }
    } while (opcao != 0);

    // Salva as alterações no arquivo
    salvarProdutora(sistema);
}

// Função para excluir os dados da produtora
void excluirProdutoraController(Sistema *sistema) {
    // Verifica se existe produtora cadastrada
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para excluir.\n");
        return;
    }

    char confirmacao;
    // Confirma exclusão com o usuário
    printf("\nTem certeza que deseja excluir os dados da produtora %s? (s/n): ", sistema->dados_produtora->nome_fantasia);
    scanf(" %c", &confirmacao);
    limpar_buffer();

    // Se confirmado, libera memória e zera os dados
    if (confirmacao == 's' || confirmacao == 'S') {
        free(sistema->dados_produtora);
        sistema->dados_produtora = NULL;
        salvarProdutora(sistema); // Salva o estado vazio
        printf("\nDados da produtora excluidos com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
}
