#include "main_view.h"
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
    limpar_tela();
    printf("+=========================================+\n");
    printf("|      ONDE SALVAR OS DADOS?              |\n");
    printf("+=========================================+\n");
    printf("| [1] Arquivo Binario (.dat - Padrao)     |\n");
    printf("| [2] Arquivo Texto (.txt)                |\n");
    printf("| [3] Apenas Memoria (Nao salva)          |\n");
    printf("+=========================================+\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limpar_buffer();

    if(opcao == 2) definirModoDeArmazenamento(sistema, ARQUIVO_TEXTO);
    else if(opcao == 3) definirModoDeArmazenamento(sistema, MEMORIA);
    else definirModoDeArmazenamento(sistema, ARQUIVO_BINARIO);
}

void menuGestaoDeDadosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=========================================+\n");
        printf("|           CADASTROS E DADOS             |\n");
        printf("+=========================================+\n");
        printf("| [1] Produtora                           |\n");
        printf("| [2] Clientes                            |\n");
        printf("| [3] Equipe Interna                      |\n");
        printf("| [4] Recursos (Equipamentos)             |\n");
        printf("| [5] Fornecedores                        |\n");
        printf("| [6] Operadores                          |\n");
        printf("| [0] Voltar                              |\n");
        printf("+=========================================+\n");
        printf("Opcao: ");
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
            default: printf("Invalido.\n"); pausar(); break;
        }
    } while (opcao != 0);
}

void menuImportarExportarView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=========================================+\n");
        printf("|          IMPORTAR / EXPORTAR            |\n");
        printf("+=========================================+\n");
        printf("| [1] Importar Dados (Carregar)           |\n");
        printf("| [2] Converter (Binario <-> Texto)       |\n");
        printf("| [0] Voltar                              |\n");
        printf("+=========================================+\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch(opcao) {
            case 1: importarDadosDeOutroFormatoController(sistema); pausar(); break;
            case 2: transferirDadosDeArmazenamento(sistema); pausar(); break;
            case 0: break;
            default: printf("Invalido.\n"); pausar(); break;
        }
    } while (opcao != 0);
}

void menuPrincipalView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=========================================+\n");
        printf("|          SISTEMA DE EVENTOS             |\n");
        printf("+=========================================+\n");
        printf("| [1] Gestao de Dados                     |\n");
        printf("| [2] Eventos (Orcamentos)                |\n");
        printf("| [3] Financeiro (Transacoes)             |\n");
        printf("| [4] Relatorios (Futuro)                 |\n");
        printf("| [5] Importar/Exportar                   |\n");
        printf("| [0] Sair                                |\n");
        printf("+=========================================+\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: menuGestaoDeDadosView(sistema); break;
            case 2: menuEventosView(sistema); break;
            case 3: menuTransacoesView(sistema); break;
            case 4: printf("Disponivel na proxima entrega.\n"); pausar(); break;
            case 5: menuImportarExportarView(sistema); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Invalido.\n"); pausar(); break;
        }
    } while (opcao != 0);
}