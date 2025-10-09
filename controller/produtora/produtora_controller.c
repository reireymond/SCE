// Inclui o cabeçalho do controller da produtora, que contém as declarações
// das funções implementadas neste arquivo.
#include "controller/produtora/produtora_controller.h"
// Inclui cabeçalhos padrão do C para funções de entrada/saída (stdio),
// alocação de memória (stdlib) e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Inclui o cabeçalho do model para poder chamar as funções 'salvarProdutora'.
#include "model/produtora/produtora_model.h"
// Inclui o cabeçalho da view para poder chamar a função 'verDetalhesProdutoraView'.
#include "view/produtora/produtora_view.h"
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_buffer' e 'limpar_tela'.
#include "utils/utils.h"
// Inclui o cabeçalho de validação para usar funções como 'ler_string_valida' e 'ler_float_positivo'.
#include "utils/validation.h"

// Função para adicionar os dados da produtora ao sistema.
void adicionarProdutoraController(Sistema *sistema) {
    // O sistema permite apenas uma produtora. Se já houver dados cadastrados,
    // a função informa o usuário e não permite um novo cadastro.
    if (sistema->dados_produtora != NULL) {
        printf("\nUma produtora ja foi cadastrada. Exclua a existente para cadastrar uma nova.\n");
        return;
    }

    // Aloca dinamicamente memória para a estrutura 'Produtora'.
    sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
    // Se a alocação de memória falhar (retornar NULL), exibe uma mensagem de erro fatal.
    if (sistema->dados_produtora == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria para a produtora.\n");
        return;
    }

    // Apresenta ao usuário o formulário de cadastro.
    printf("\n--- Cadastro dos Dados da Produtora ---\n");

    // Solicita e lê cada um dos campos, utilizando as funções de validação para garantir a integridade dos dados.
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

    // Após preencher todos os dados, chama a função do model para salvar em arquivo.
    salvarProdutora(sistema);
    printf("\nDados da produtora '%s' cadastrados com sucesso!\n", sistema->dados_produtora->nome_fantasia);
}

// Função para alterar os dados da produtora já cadastrada.
void alterarProdutoraController(Sistema *sistema) {
    // Se não houver dados de produtora para alterar, informa o usuário e retorna.
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para alterar. Por favor, adicione uma primeiro.\n");
        return;
    }

    int opcao;
    // Loop do menu de alteração, que continua até o usuário escolher a opção 0 para sair.
    do {
        limpar_tela();
        printf("--- Alterar Dados da Produtora ---\n\n");
        // Mostra os detalhes atuais da produtora para o usuário.
        verDetalhesProdutoraView(sistema);

        // Apresenta o menu de opções para alteração.
        printf("\n\nQual campo deseja alterar?\n");
        printf("1. Nome Fantasia\n2. Razao Social\n3. Nome do Responsavel\n4. CNPJ\n5. Inscricao Estadual\n");
        printf("6. Endereco\n7. Telefone\n8. Telefone do Responsavel\n9. E-mail\n10. Margem de Lucro\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        
        // Lê a opção do usuário, garantindo que seja um número válido no intervalo de 0 a 10.
        ler_int_valido(&opcao, 0, 10);

        // Executa a ação correspondente à opção escolhida.
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
            // O caso 'default' não é mais estritamente necessário por causa da validação
            // de entrada da função 'ler_int_valido', mas é uma boa prática mantê-lo.
            default:
                printf("\nOpcao invalida!\n");
                pausar();
                break;
        }
    } while (opcao != 0);

    // Após o usuário sair do loop, salva todas as alterações no arquivo.
    salvarProdutora(sistema);
}

// Função para excluir os dados da produtora.
void excluirProdutoraController(Sistema *sistema) {
    // Se não houver dados cadastrados, não há o que excluir.
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para excluir.\n");
        return;
    }

    char confirmacao;
    // Pede uma confirmação final ao usuário para evitar a exclusão acidental.
    printf("\nTem certeza que deseja excluir os dados da produtora %s? (s/n): ", sistema->dados_produtora->nome_fantasia);
    scanf(" %c", &confirmacao);
    limpar_buffer();

    // Se o usuário confirmar a exclusão...
    if (confirmacao == 's' || confirmacao == 'S') {
        // Libera a memória que foi alocada para a estrutura da produtora.
        free(sistema->dados_produtora);
        // Define o ponteiro como nulo para indicar que os dados não existem mais.
        sistema->dados_produtora = NULL;
        // Salva o estado "vazio", ou seja, atualiza o arquivo para refletir a exclusão.
        salvarProdutora(sistema);
        printf("\nDados da produtora excluidos com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
}