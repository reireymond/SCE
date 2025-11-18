// da view de fornecedor, que contém as declarações das funções
#include "fornecedor_view.h"
#include <stdio.h>
#include "utils/utils.h"
// do controller de fornecedor para poder chamar as funções que executam
#include "controller/fornecedor/fornecedor_controller.h"

// exibe e gerencia o menu de fornecedores.
void menuFornecedoresView(Sistema *sistema) {
    int opcao;
    
    do {
        // Limpa a tela para uma apresentação mais limpa do menu.
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|           MENU FORNECEDORES E PARCEIROS             |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Fornecedor                       |\n");
        printf("| [2] Alterar Fornecedor Existente                    |\n");
        printf("| [3] Listar Todos os Fornecedores                    |\n");
        printf("| [4] Excluir Fornecedor                              |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        // Lê a opção digitada pelo usuário.
        scanf("%d", &opcao);
        // 
        limpar_buffer();

        // Um 'switch' direciona o fluxo do programa com base na opção do usuário.
        switch (opcao) {
            // Cada caso chama a função do controller correspondente para executar a ação.
            case 1: adicionarFornecedorController(sistema); break;
            case 2: alterarFornecedorController(sistema); break;
            case 3: listarFornecedoresView(sistema); break;
            case 4: excluirFornecedorController(sistema); break;
            case 0: break; // A opção 0 simplesmente encerra o loop.
            default: printf("\nOpcao invalida!\n"); break;
        }
        // Se a opção não for 0 (sair), pausa a execução para que o usuário possa
        // ler o resultado da operação antes de o menu ser exibido novamente.
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// exibe a lista de fornecedores.
void listarFornecedoresView(Sistema *sistema) {
    // Verifica se há fornecedores cadastrados. Se não houver, exibe uma mensagem.
    if (sistema->num_fornecedores == 0) {
        printf("\nNenhum fornecedor cadastrado.\n");
        return; // Retorna para a função anterior (o menu).
    }
    // Imprime o cabeçalho da lista.
    printf("+=====================================================+\n");
    printf("|       LISTA DE FORNECEDORES E PARCEIROS             |\n");
    printf("+=====================================================+\n");

if (sistema->num_fornecedores == 0) {
    printf("| Nenhum fornecedor cadastrado.                       |\n");
} else {
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        printf("+-----------------------------------------------------+\n");
        printf("| Codigo         : %-36d |\n", sistema->lista_fornecedores[i].codigo);
        printf("| Nome Fantasia  : %-36s |\n", sistema->lista_fornecedores[i].nome_fantasia);
        printf("| Razao Social   : %-36s |\n", sistema->lista_fornecedores[i].razao_social);
        printf("| CNPJ / CPF     : %-36s |\n", sistema->lista_fornecedores[i].cnpj);
        printf("| Endereco       : %-36s |\n", sistema->lista_fornecedores[i].endereco);
        printf("| Telefone       : %-36s |\n", sistema->lista_fornecedores[i].telefone);
        printf("| Tipo de Servico: %-36s |\n", sistema->lista_fornecedores[i].tipo_servico);
    }
}

        printf("+=====================================================+\n");

}