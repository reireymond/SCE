// Inclui o cabeçalho da view principal, que contém as declarações das
// funções que serão implementadas neste arquivo.
#include "main_view.h"
// Inclui a biblioteca padrão de entrada/saída para usar 'printf' e 'scanf'.
#include <stdio.h>
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_tela', 'pausar', etc.
#include "utils/utils.h"
// Inclui o cabeçalho do model de configuração para chamar a função 'definirModoDeArmazenamento'.
#include "model/config_armazenamento/config_armazenamento.h"
// I
#include "controller/armazenamento/armazenamento_controller.h"

// Inclui os cabeçalhos de todas as outras views de submenus.
// Isso é necessário para que o menu de "Gestão de Dados" possa chamar os menus específicos
// de cada entidade (cliente, produtora, etc.).
#include "view/produtora/produtora_view.h"
#include "view/cliente/cliente_view.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "view/recurso/recurso_view.h"
#include "view/fornecedor/fornecedor_view.h"
#include "view/operador/operador_view.h"

// Implementação da função que exibe o menu de escolha do modo de armazenamento.
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

    // Com base na escolha do usuário, chama a função para definir o modo de armazenamento.
    switch (opcao) {
        case 1: definirModoDeArmazenamento(sistema, ARQUIVO_BINARIO); break;
        case 2: definirModoDeArmazenamento(sistema, ARQUIVO_TEXTO); break;
        case 3: definirModoDeArmazenamento(sistema, MEMORIA); break;
        default:
            // Se a opção for inválida, assume um modo padrão (Binário) e informa o usuário.
            printf("Opcao invalida. Usando o modo padrao (Binario).\n");
            definirModoDeArmazenamento(sistema, ARQUIVO_BINARIO);
            break;
    }
}

// Implementação da função que exibe o menu principal e controla a navegação.
void menuPrincipalView(Sistema *sistema) {
    int opcao;
    // O loop continua exibindo o menu até que o usuário escolha a opção 0 para sair.
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

        // Direciona o fluxo com base na escolha do usuário.
        switch (opcao)
        {
        case 1:
            // Chama a função que exibe o submenu de Gestão de Dados.
            menuGestaoDeDadosView(sistema);
            break;
        // Os casos de 2 a 5 são módulos ainda não implementados.
        case 2:
        case 3:
        case 4:
        case 5:
            menuImportarExportarView(sistema);
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

// Implementação da função que exibe o menu de Gestão de Dados.
void menuGestaoDeDadosView(Sistema *sistema) {
    int opcao;
    // O loop continua até o usuário escolher a opção 0 para voltar ao menu principal.
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

        // Cada opção chama a função de menu da view correspondente,
        // navegando para o submódulo de cadastro escolhido.
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
    TipoArmazenamento modoAtual = obterModoDeArmazenamento(sistema); // Pega o modo atual

    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|              IMPORTAR / EXPORTAR DADOS              |\n");
        printf("+=====================================================+\n");
        printf("| Modo de Salvamento Atual: %s           |\n",
               (modoAtual == ARQUIVO_TEXTO) ? "Arquivo Texto" :
               (modoAtual == ARQUIVO_BINARIO) ? "Arquivo Binario" : "Apenas Memoria");
        printf("+-----------------------------------------------------+\n");
        printf("| [1] Carregar dados (Texto/Binario) para esta sessao |\n"); // Chama importarDadosDeOutroFormatoController
        printf("| [2] Converter arquivos (.txt <-> .dat)              |\n"); // Chama uma NOVA função (transferirDadosEntreFormatosController)
        printf("| [3] Importar de arquivo externo (XML - futuro)      |\n"); // Função futura
        printf("| [4] Exportar para arquivo externo (XML - futuro)    |\n"); // Função futura
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar ao Menu Principal                        |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");

        // ... (leitura e validação da opção) ...
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            opcao = -1; // Força opção inválida
        } else {
            limpar_buffer();
        }

        switch(opcao) {
            case 1:
                importarDadosDeOutroFormatoController(sistema); // Função existente
                pausar();
                break;
            case 2:
            case 3:
            case 4:
                printf("\nFuncao ainda nao implementada.\n");
                pausar();
                break;
            case 0: break;
            default:
                printf("\nOpcao invalida!\n");
                pausar();
                break;
        }
        // Atualiza o modoAtual caso a função de transferência o tenha modificado
        // (embora a função de transferência em si não deva mudar o modo da sessão atual)
        modoAtual = obterModoDeArmazenamento(sistema);

    } while (opcao != 0);
}