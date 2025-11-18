#include "view/main/main_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "model/config_armazenamento/config_armazenamento.h"
#include "controller/armazenamento/armazenamento_controller.h"

// Includes das outras Views
#include "view/produtora/produtora_view.h"
#include "view/cliente/cliente_view.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "view/recurso/recurso_view.h"
#include "view/fornecedor/fornecedor_view.h"
#include "view/operador/operador_view.h"
#include "view/evento/evento_view.h"      
#include "view/transacao/transacao_view.h"

void menuEscolherArmazenamentoView(Sistema *sistema) {
    int opcao;
    printf("+=====================================================+\n");
    printf("|        CONFIGURACAO DE ARMAZENAMENTO                |\n");
    printf("|      Escolha como os dados devem ser salvos:        |\n");
    printf("+=====================================================+\n");
    printf("| [1] Arquivo Binario (Mais rapido)                   |\n");
    printf("| [2] Arquivo de Texto (Legivel)                      |\n");
    printf("| [3] Apenas em Memoria (Dados nao serao salvos)      |\n");
    printf("+=====================================================+\n");
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
        printf("+=====================================================+\n");
        printf("|                  MENU PRINCIPAL                     |\n");
        printf("+=====================================================+\n");
        printf("| [1] Gestao de Dados                                 |\n");
        printf("| [2] Gestao de Eventos                               |\n");
        printf("| [3] Transacoes                                      |\n");
        printf("| [4] FeedBack                                        |\n");
        printf("| [5] Importar ou Exportar Dados                      |\n");
        printf("| [0] Sair                                            |\n");
        printf("+=====================================================+\n");
        printf("\nEscolha sua opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
        case 1: menuGestaoDeDadosView(sistema); break;
        case 2: menuEventosView(sistema); break;
        case 3: menuTransacoesView(sistema); break; 
        case 4:
            printf("\nModulo de Feedback (Relatorios) ainda nao implementado.\n");
            pausar();
            break;
        case 5: menuImportarExportarView(sistema); break;
        case 0: printf("\nSaindo do programa...\n"); break;
        default: printf("\nOpcao invalida! Tente novamente.\n"); pausar(); break;
        }

    } while (opcao != 0);
}

void menuGestaoDeDadosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|                 GESTAO DE DADOS                     |\n");
        printf("+=====================================================+\n");
        printf("| [1] Gerenciar Produtora                             |\n");
        printf("| [2] Gerenciar Clientes                              |\n");
        printf("| [3] Gerenciar Equipe Interna                        |\n");
        printf("| [4] Gerenciar Recursos e Equipamentos               |\n");
        printf("| [5] Gerenciar Fornecedores e Parceiros              |\n");
        printf("| [6] Gerenciar Operadores do Sistema                 |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar ao Menu Principal                        |\n");
        printf("+=====================================================+\n");
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

void menuImportarExportarView(Sistema *sistema) {
    int opcao;
    TipoArmazenamento modoAtual;

    do {
        modoAtual = obterModoDeArmazenamento(sistema);
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|              IMPORTAR / EXPORTAR DADOS              |\n");
        printf("+=====================================================+\n");
        printf("| Modo de Salvamento Atual: %s           |\n",
               (modoAtual == ARQUIVO_TEXTO) ? "Arquivo Texto  " :
               (modoAtual == ARQUIVO_BINARIO) ? "Arquivo Binario" : "Apenas Memoria ");
        printf("+-----------------------------------------------------+\n");
        printf("| [1] Carregar dados (Texto/Binario) para esta sessao |\n");
        printf("| [2] Converter arquivos (.txt <-> .dat)              |\n");
        printf("| [3] Importar de arquivo externo (XML - futuro)      |\n");
        printf("| [4] Exportar para arquivo externo (XML - futuro)    |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar ao Menu Principal                        |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            opcao = -1;
        } else {
            limpar_buffer();
        }

        switch(opcao) {
            case 1: importarDadosDeOutroFormatoController(sistema); pausar(); break;
            case 2: transferirDadosDeArmazenamento(sistema); pausar(); break;
            case 3: case 4: printf("\nFuncao ainda nao implementada.\n"); pausar(); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); pausar(); break;
        }
    } while (opcao != 0);
}