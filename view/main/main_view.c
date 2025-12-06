#include "main_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "model/config_armazenamento/config_armazenamento.h"
#include "controller/armazenamento/armazenamento_controller.h"
#include "controller/relatorio/relatorio_controller.h" 
#include "controller/exportacao/exportacao_controller.h"
#include "view/armazenamento/armazenamento_view.h"
#include "view/produtora/produtora_view.h"
#include "view/cliente/cliente_view.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "view/recurso/recurso_view.h"
#include "view/fornecedor/fornecedor_view.h"
#include "view/operador/operador_view.h"
#include "view/evento/evento_view.h"
#include "view/transacao/transacao_view.h"

int menuEscolherArmazenamentoView() {
    int opcao;
    limpar_tela();
    printf("+=====================================================+\n");
    printf("|              TIPO DE ARMAZENAMENTO                  |\n");
    printf("+=====================================================+\n");
    printf("| [1] Arquivo Binario (.dat)                          |\n");
    printf("| [2] Arquivo Texto (.txt)                            |\n");
    printf("| [3] Apenas Memoria (Nao salva ao fechar)            |\n");
    printf("+=====================================================+\n");
    printf("Escolha uma opcao: ");
    
    scanf("%d", &opcao);
    limpar_buffer();

    return opcao;
}

void menuGestaoDeDadosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|               MENU CADASTROS E DADOS                |\n");
        printf("+=====================================================+\n");
        printf("| [1] Produtora                                       |\n");
        printf("| [2] Clientes                                        |\n");
        printf("| [3] Equipe Interna                                  |\n");
        printf("| [4] Recursos                                        |\n");
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
        printf("| [4] Relatorios e Feedback                           |\n");
        printf("| [5] Importar/Exportar Dados (XML)                   |\n"); 
        printf("| [6] Converter Dados                                 |\n"); 
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
            case 4: menuRelatoriosController(sistema); break; 
            case 5: menuXMLController(sistema); break;
            case 6:menuConverterArquivosView(sistema);break;
            case 0: printf("Saindo do sistema...\n"); break;
            default: printf("Opcao invalida.\n"); pausar(); break;
        }
    } while (opcao != 0);
}
