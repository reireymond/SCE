#include "main_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "model/config_armazenamento/config_armazenamento.h"

// Incluir as views dos submódulos
#include "view/produtora/produtora_view.h"
#include "view/cliente/cliente_view.h"
#include "view/equipe_interna/equipe_interna_view.h" // ADICIONAR
#include "view/recurso/recurso_view.h"             // ADICIONAR
#include "view/fornecedor/fornecedor_view.h"       // ADICIONAR
#include "view/operador/operador_view.h"           // ADICIONAR

void menuEscolherArmazenamentoView(Sistema *sistema) {
    int opcao;
    printf("\n--- Configuracao de Armazenamento ---\n");
    printf("Escolha como os dados devem ser salvos:\n");
    printf("1. Arquivo Binario (Mais rapido)\n");
    printf("2. Arquivo de Texto (Legivel)\n");
    printf("3. Apenas em Memoria (Dados nao serao salvos)\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limpar_buffer();

    switch (opcao) {
        case 1: definirModoDeArmazenamento(sistema, ARQUIVO_BINARIO); break;
        case 2: definirModoDeArmazenamento(sistema, ARQUIVO_TEXTO); break;
        case 3: definirModoDeArmazenamento(sistema, MEMORIA); break;
        default:
            printf("Opcao invalida. Usando o modo padrao (Binario).\n");
            definirModoDeArmazenamento(sistema, ARQUIVO_BINARIO);
            break;
    }
}

void menuPrincipalView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Gestao de Dados\n");
        printf("2. Gestao de Eventos\n");
        printf("3. Transacoes\n");
        printf("4. FeedBack\n");
        printf("5. Importar ou Exportar Dados\n");
        printf("0. Sair\n");
        printf("\nEscolha sua opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao)
        {
        case 1:
            menuGestaoDeDadosView(sistema);
            break;
        case 2:
            printf("\nModulo em construcao...\n");
            pausar();
            break;
        case 3:
            printf("\nModulo em construcao...\n");
            pausar();
            break;
        case 4:
            printf("\nModulo em construcao...\n");
            pausar();
            break;
        case 5:
            printf("\nModulo em construcao...\n");
            pausar();
            break;
        case 0:
            printf("\nSaindo do programa...\n");
            break;
        default:
            printf("\nOpcao invalida! Tente novamente.\n");
            pausar();
            break;
        }

    } while (opcao != 0);
}

void menuGestaoDeDadosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("\n--- GESTAO DE DADOS ---\n");
        printf("1. Gerenciar Produtora\n");
        printf("2. Gerenciar Clientes\n");
        printf("3. Gerenciar Equipe Interna\n");
        printf("4. Gerenciar Recursos e Equipamentos\n");
        printf("5. Gerenciar Fornecedores e Parceiros\n");
        printf("6. Gerenciar Operadores do Sistema\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: menuProdutoraView(sistema); break;
            case 2: menuClientesView(sistema); break;
            case 3: menuEquipeInternaView(sistema); break;
            case 4: menuRecursosView(sistema); break;
            case 5: menuFornecedoresView(sistema); break;
            case 6: menuOperadoresView(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); pausar(); break;
        }
    } while (opcao != 0);
}
