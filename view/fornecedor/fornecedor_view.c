// Inclui o cabeçalho da view de fornecedor, que contém as declarações das funções
// que serão implementadas neste arquivo.
#include "fornecedor_view.h"
// Inclui a biblioteca padrão de entrada/saída para usar funções como 'printf' e 'scanf'.
#include <stdio.h>
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_tela' e 'pausar'.
#include "utils/utils.h"
// Inclui o cabeçalho do controller de fornecedor para poder chamar as funções que executam
// as ações (adicionarFornecedorController, etc.).
#include "controller/fornecedor/fornecedor_controller.h"

// Implementação da função que exibe e gerencia o menu de fornecedores.
void menuFornecedoresView(Sistema *sistema) {
    int opcao;
    // O loop 'do-while' garante que o menu seja exibido continuamente até que o
    // usuário escolha a opção 0 para voltar ao menu anterior.
    do {
        // Limpa a tela para uma apresentação mais limpa do menu.
        limpar_tela();
        printf("\n--- Menu Fornecedores e Parceiros ---\n");
        printf("1. Adicionar Novo Fornecedor\n");
        printf("2. Alterar Fornecedor Existente\n");
        printf("3. Listar Todos os Fornecedores\n");
        printf("4. Excluir Fornecedor\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        // Lê a opção digitada pelo usuário.
        scanf("%d", &opcao);
        // Limpa o buffer de entrada para evitar problemas na próxima leitura.
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

// Implementação da função que exibe a lista de fornecedores.
void listarFornecedoresView(Sistema *sistema) {
    // Verifica se há fornecedores cadastrados. Se não houver, exibe uma mensagem.
    if (sistema->num_fornecedores == 0) {
        printf("\nNenhum fornecedor cadastrado.\n");
        return; // Retorna para a função anterior (o menu).
    }
    // Imprime o cabeçalho da lista.
    printf("\n--- Lista de Fornecedores e Parceiros ---\n");
    // Percorre o array de fornecedores.
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        // Para cada fornecedor, imprime seus dados de forma formatada e legível.
        printf("----------------------------------\n");
        printf("Codigo: %d\n", sistema->lista_fornecedores[i].codigo);
        printf("Nome Fantasia: %s\n", sistema->lista_fornecedores[i].nome_fantasia);
        printf("Razao Social: %s\n", sistema->lista_fornecedores[i].razao_social);
        printf("CNPJ / CPF: %s\n", sistema->lista_fornecedores[i].cnpj);
        printf("Endereco: %s\n", sistema->lista_fornecedores[i].endereco);
        printf("Telefone: %s\n", sistema->lista_fornecedores[i].telefone);
        printf("Tipo de Servico: %s\n", sistema->lista_fornecedores[i].tipo_servico);
    }
    // Imprime uma linha final para fechar a formatação da lista.
    printf("----------------------------------\n");
}