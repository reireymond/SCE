#include "main_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "model/config_armazenamento/config_armazenamento.h"
#include "controller/armazenamento/armazenamento_controller.h"

#include "view/armazenamento/armazenamento_view.h"

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
    printf("+=====================================================+\n");
    printf("|              ONDE SALVAR OS DADOS?                  |\n");
    printf("+=====================================================+\n");
    printf("| [1] Arquivo Binario (.dat - Recomendado)            |\n");
    printf("| [2] Arquivo Texto (.txt)                            |\n");
    printf("| [3] Apenas Memoria (Nao salva ao fechar)            |\n");
    printf("+=====================================================+\n");
    printf("Escolha uma opcao: ");
    
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
        printf("+=====================================================+\n");
        printf("|               MENU CADASTROS E DADOS                |\n");
        printf("+=====================================================+\n");
        printf("| [1] Produtora (Dados da Empresa)                    |\n");
        printf("| [2] Clientes                                        |\n");
        printf("| [3] Equipe Interna (Funcionarios)                   |\n");
        printf("| [4] Recursos (Equipamentos e Estoque)               |\n");
        printf("| [5] Fornecedores e Parceiros                        |\n");
        printf("| [6] Operadores do Sistema                           |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
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
            default: printf("Opcao invalida.\n"); pausar(); break;
        }
    } while (opcao != 0);
}

void menuImportarExportarView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|              MENU IMPORTAR / EXPORTAR               |\n");
        printf("+=====================================================+\n");
        printf("| [1] Converter Arquivos (Binario <-> Texto)          |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch(opcao) {
            case 1: menuConverterArquivosView(sistema); pausar(); break;
            case 0: break;
            default: printf("Opcao invalida.\n"); pausar(); break;
        }
    } while (opcao != 0);
}

void menuPrincipalView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|             SISTEMA DE GESTAO DE EVENTOS            |\n");
        printf("+=====================================================+\n");
        printf("| [1] Gestao de Cadastros (Clientes, Equipe...)       |\n");
        printf("| [2] Eventos (Orcamentos e Aprovacao)                |\n");
        printf("| [3] Financeiro (Caixa e Contas)                     |\n");
        printf("| [4] Relatorios (Em breve)                           |\n");
        printf("| [5] Importar/Exportar Dados                         |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Sair do Sistema                                 |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: menuGestaoDeDadosView(sistema); break;
            case 2: menuEventosView(sistema); break;
            case 3: menuTransacoesView(sistema); break;
            case 4: printf("Disponivel na proxima entrega.\n"); pausar(); break;
            case 5: menuImportarExportarView(sistema); break;
            case 0: printf("Saindo do sistema... Ate logo!\n"); break;
            default: printf("Opcao invalida.\n"); pausar(); break;
        }
    } while (opcao != 0);
}
