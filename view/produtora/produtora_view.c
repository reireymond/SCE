// Inclui o cabeçalho da view da produtora, que contém as declarações das funções
// que serão implementadas neste arquivo.
#include "produtora_view.h"
// Inclui a biblioteca padrão de entrada/saída para usar funções como 'printf' e 'scanf'.
#include <stdio.h>
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_tela' e 'pausar'.
#include "utils/utils.h"
// Inclui o cabeçalho do controller da produtora para poder chamar as funções que
// executam as ações (adicionarProdutoraController, etc.).
#include "controller/produtora/produtora_controller.h"

// Implementação da função que exibe e gerencia o menu da produtora.
void menuProdutoraView(Sistema *sistema) {
    int opcao;
    // O loop 'do-while' assegura que o menu seja exibido repetidamente até que o
    // usuário escolha a opção 0 para voltar ao menu anterior.
    do {
        // Limpa a tela antes de cada exibição do menu para manter a interface limpa.
       limpar_tela();
        printf("+=====================================================+\n");
        printf("|                   MENU PRODUTORA                   |\n");
        printf("+=====================================================+\n");
        printf("| [1] Cadastrar Dados                                 |\n");
        printf("| [2] Alterar Dados                                   |\n");
        printf("| [3] Ver Detalhes                                    |\n");
        printf("| [4] Excluir Dados                                   |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                         |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        // Lê a opção numérica do usuário.
        scanf("%d", &opcao);
        // Limpa o buffer de entrada para evitar que o caractere de nova linha ('\n')
        // afete as leituras subsequentes.
        limpar_buffer();

        // O 'switch' direciona o fluxo do programa com base na escolha do usuário.
        switch (opcao) {
            // Cada caso chama a função do controller apropriada para realizar a ação.
            case 1: adicionarProdutoraController(sistema); break;
            case 2: alterarProdutoraController(sistema); break;
            // O caso 3 chama outra função da própria view para apenas exibir os dados.
            case 3: verDetalhesProdutoraView(sistema); break;
            case 4: excluirProdutoraController(sistema); break;
            case 0: break; // A opção 0 não faz nada, o que fará com que o loop termine.
            default: printf("\nOpcao invalida!\n"); break;
        }
        // Se a opção não for 0 (sair), o programa pausa, aguardando que o usuário
        // pressione Enter para continuar. Isso permite que o usuário veja o resultado da ação.
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// Implementação da função que exibe os detalhes da produtora.
void verDetalhesProdutoraView(Sistema *sistema) {
    // Verifica se os dados da produtora já foram cadastrados.
    // Se o ponteiro for nulo, significa que não há nada para mostrar.
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para exibir.\n");
        return; // Retorna para a função que a chamou.
    }
   // Inicia a exibição da produtora com uma caixa de destaque
        printf("+=====================================================+\n");
        printf("|          DETALHES DA PRODUTORA CADASTRADA           |\n");
        printf("+=====================================================+\n");

        // Cada linha exibe um campo da estrutura 'Produtora', alinhado e dentro do retângulo
        printf("| Nome Fantasia: %-32s \n", sistema->dados_produtora->nome_fantasia);
        printf("| Razao Social: %-32s \n", sistema->dados_produtora->razao_social);
        printf("| Nome do Responsavel: %-32s \n", sistema->dados_produtora->nome_do_responsavel);
        printf("| CNPJ: %-32s \n", sistema->dados_produtora->cnpj);
        printf("| Inscricao Estadual: %-32s \n", sistema->dados_produtora->inscricao_estadual);
        printf("| Endereco: %-32s \n", sistema->dados_produtora->endereco);
        printf("| Telefone: %-32s \n", sistema->dados_produtora->telefone);
        printf("| Telefone do Responsavel: %-32s \n", sistema->dados_produtora->telefone_responsavel);
        printf("| E-mail: %-32s \n", sistema->dados_produtora->email);

        // Para o float, '%.2f' formata o número com duas casas decimais
        // O '%%' é usado para imprimir o caractere de porcentagem literal
        printf("| Margem de Lucro: %-30.2f%% \n", sistema->dados_produtora->margem_lucro);

        // Linha final para fechar o retângulo
        printf("+=====================================================+\n");


}