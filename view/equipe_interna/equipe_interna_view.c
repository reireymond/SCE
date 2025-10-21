// Inclui o cabeçalho da view da equipe interna, que contém as declarações
// das funções implementadas neste arquivo.
#include "equipe_interna_view.h"
// Inclui o cabeçalho padrão de entrada/saída para usar funções como 'printf' e 'scanf'.
#include <stdio.h>
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_tela' e 'pausar'.
#include "utils/utils.h"
// Inclui o cabeçalho do controller da equipe interna para poder chamar as funções que
// executam as ações (adicionarEquipeController, etc.).
#include "controller/equipe_interna/equipe_interna_controller.h"

// Implementação da função que exibe e gerencia o menu da equipe interna.
void menuEquipeInternaView(Sistema *sistema) {
    int opcao;
    // O loop 'do-while' garante que o menu seja exibido continuamente até que o
    // usuário escolha a opção 0 para voltar ao menu anterior.
    do {
        // Limpa a tela para uma apresentação mais organizada do menu.
        limpar_tela();
       
        printf("+=====================================================+\n");
        printf("|                MENU EQUIPE INTERNA                  |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Membro                          |\n");
        printf("| [2] Alterar Membro Existente                       |\n");
        printf("| [3] Listar Todos os Membros                        |\n");
        printf("| [4] Excluir Membro                                 |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                         |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");

        // Lê a opção digitada pelo usuário.
        scanf("%d", &opcao);
        // Limpa o buffer de entrada para evitar que o '\n' afete a próxima leitura.
        limpar_buffer();

        // Um 'switch' direciona o fluxo do programa com base na opção do usuário.
        switch (opcao) {
            // Cada caso chama a função do controller correspondente para executar a ação.
            case 1: adicionarEquipeController(sistema); break;
            case 2: alterarEquipeController(sistema); break;
            case 3: listarEquipeInternaView(sistema); break;
            case 4: excluirEquipeController(sistema); break;
            case 0: break; // A opção 0 simplesmente encerra o loop.
            default: printf("\nOpcao invalida!\n"); break;
        }
        // Se a opção não for 0 (sair), pausa a execução para que o usuário possa
        // ler o resultado da operação antes de o menu ser exibido novamente.
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// Implementação da função que exibe a lista de membros da equipe.
void listarEquipeInternaView(Sistema *sistema) {
    // Verifica se há membros cadastrados. Se não houver, exibe uma mensagem informativa.
    if (sistema->num_equipe == 0) {
        printf("\nNenhum membro da equipe cadastrado.\n");
        return; // Retorna para a função que a chamou (geralmente o menu).
    }
    // Imprime o cabeçalho da lista.
    printf("+=====================================================+\n");
    printf("|        LISTA DE MEMBROS DA EQUIPE INTERNA          |\n");
    printf("+=====================================================+\n");

if (sistema->num_equipe == 0) {
    printf("| Nenhum membro cadastrado.                          |\n");
} else {
    for (int i = 0; i < sistema->num_equipe; i++) {
        printf("+-----------------------------------------------------+\n");
        printf("| Codigo : %-42d |\n", sistema->lista_equipe[i].codigo);
        printf("| Nome   : %-42s |\n", sistema->lista_equipe[i].nome);
        printf("| CPF    : %-42s |\n", sistema->lista_equipe[i].cpf);
        printf("| Funcao : %-42s |\n", sistema->lista_equipe[i].funcao);
        printf("| Diaria : R$ %-39.2f |\n", sistema->lista_equipe[i].valor_diaria);
    }
}

        printf("+=====================================================+\n");

}