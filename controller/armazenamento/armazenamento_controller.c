#include "armazenamento_controller.h" // Inclui o próprio header
#include <stdio.h>                   // Para printf, scanf
#include <stdlib.h>                  // Para malloc, free, realloc (usados internamente pelos models)
#include <string.h>                  // Para memset
#include "model/config_armazenamento/config_armazenamento.h" // Para obter/definir modo e TipoArmazenamento
#include "controller/main/main_controller.h" // Para carregarTodosOsDadosController e liberarTodaMemoriaController
#include "utils/utils.h"         // Para limpar_tela, limpar_buffer, pausar
#include "utils/validation.h"    // Para ler_int_valido (opcional, pode usar scanf direto se preferir)

// --- IMPORTANTE: Inclua TODOS os headers dos SEUS models aqui ---
// Eles são necessários para que as funções salvar<Entidade> sejam encontradas
#include "model/produtora/produtora_model.h"
#include "model/cliente/cliente_model.h"
#include "model/equipe_interna/equipe_interna_model.h"
#include "model/recurso/recurso_model.h"
#include "model/fornecedor/fornecedor_model.h"
#include "model/operador/operador_model.h"
// Adicione outros #includes de models se você tiver mais entidades

// --- Implementação das funções ---

// Função para importar dados de Texto/Binário para a memória da sessão atual
// (Código que já tínhamos desenvolvido antes)
void importarDadosDeOutroFormatoController(Sistema *sistema) {
    int opcao_origem;
    TipoArmazenamento origem;
    TipoArmazenamento modo_original = obterModoDeArmazenamento(sistema);

    limpar_tela();
    printf("+=====================================================+\n");
    printf("|          IMPORTAR DADOS PARA SESSAO ATUAL           |\n");
    printf("+=====================================================+\n");
    printf("| De qual formato deseja carregar os dados?           |\n");
    printf("| [1] Arquivo de Texto (.txt)                         |\n");
    printf("| [2] Arquivo Binario (.dat)                          |\n");
    printf("+-----------------------------------------------------+\n");
    printf("| [0] Cancelar                                        |\n");
    printf("+=====================================================+\n");
    printf("Escolha a origem: ");

    if (scanf("%d", &opcao_origem) != 1 || opcao_origem < 0 || opcao_origem > 2) {
        limpar_buffer();
        printf("\nOpcao invalida!\n");
        return;
    }
    limpar_buffer();

    switch (opcao_origem) {
        case 1: origem = ARQUIVO_TEXTO; break;
        case 2: origem = ARQUIVO_BINARIO; break;
        case 0: printf("\nImportacao cancelada.\n"); return;
        default: printf("\nOpcao invalida!\n"); return; // Segurança extra
    }

    printf("\nATENCAO: Esta acao substituira todos os dados atualmente em memoria.\n");
    printf("Os dados serao carregados a partir dos arquivos do formato '%s'.\n",
           (origem == ARQUIVO_TEXTO) ? "Texto (.txt)" : "Binario (.dat)");
    printf("Deseja continuar? (s/n): ");
    char confirmacao;
    scanf(" %c", &confirmacao);
    limpar_buffer();

    if (confirmacao != 's' && confirmacao != 'S') {
        printf("\nImportacao cancelada pelo usuario.\n");
        return;
    }

    printf("\nLimpando dados atuais em memoria...\n");
    liberarTodaMemoriaController(sistema);

    definirModoDeArmazenamento(sistema, origem);

    printf("Carregando dados do formato %s...\n", (origem == ARQUIVO_TEXTO) ? "Texto" : "Binario");
    // Verifica se os arquivos de origem existem (simplificado: checa se algo foi carregado)
    // Uma melhoria seria as funções 'carregar' retornarem um status (sucesso/falha/vazio)
    int num_clientes_antes = sistema->num_clientes; // Exemplo de verificação simples
    carregarTodosOsDadosController(sistema);
    // if (num_clientes_antes == 0 && sistema->num_clientes == 0 && sistema->dados_produtora == NULL /* etc */) {
    //    printf("\nAVISO: Nenhum dado encontrado nos arquivos de origem ou erro na leitura.\n");
    // }


    definirModoDeArmazenamento(sistema, modo_original);

    printf("\n------------------------------------------------------\n");
    printf("Dados carregados com sucesso para a memoria!\n");
    printf("------------------------------------------------------\n");
    printf("O modo de salvamento automatico continua sendo: %s.\n",
           (modo_original == ARQUIVO_TEXTO) ? "Arquivo Texto (.txt)" :
           (modo_original == ARQUIVO_BINARIO) ? "Arquivo Binario (.dat)" : "Apenas Memoria (nao salva ao sair)");

     if (modo_original != MEMORIA) {
         printf("Qualquer alteracao feita a partir de agora sera salva neste formato (%s).\n",
               (modo_original == ARQUIVO_TEXTO) ? ".txt" : ".dat");
    } else {
        printf("Lembre-se que as alteracoes nao serao salvas ao sair do programa.\n");
    }
    printf("------------------------------------------------------\n");
}


// --- NOVA FUNÇÃO ---
// Função para transferir/converter dados entre os formatos Texto e Binário
void transferirDadosDeArmazenamento(Sistema *sistema) {
    int opcao_origem, opcao_destino;
    TipoArmazenamento origem, destino;

    limpar_tela();
    printf("+=====================================================+\n");
    printf("|         TRANSFERIR/CONVERTER DADOS ARMAZENADOS        |\n");
    printf("+=====================================================+\n");
    printf("| Qual o formato de ORIGEM dos dados?                 |\n");
    printf("| [1] Arquivo de Texto (.txt)                         |\n");
    printf("| [2] Arquivo Binario (.dat)                          |\n");
    printf("+-----------------------------------------------------+\n");
    printf("| [0] Cancelar                                        |\n");
    printf("+=====================================================+\n");
    printf("Escolha a origem: ");

    if (scanf("%d", &opcao_origem) != 1 || opcao_origem < 0 || opcao_origem > 2) {
        limpar_buffer(); printf("\nOpcao invalida!\n"); return;
    }
    limpar_buffer();
    if (opcao_origem == 0) { printf("\nTransferencia cancelada.\n"); return; }
    origem = (opcao_origem == 1) ? ARQUIVO_TEXTO : ARQUIVO_BINARIO;

    printf("\n| Qual o formato de DESTINO para salvar os dados?     |\n");
    printf("| [1] Arquivo de Texto (.txt)                         |\n");
    printf("| [2] Arquivo Binario (.dat)                          |\n");
    printf("+-----------------------------------------------------+\n");
    printf("| [0] Cancelar                                        |\n");
    printf("+=====================================================+\n");
    printf("Escolha o destino: ");

    if (scanf("%d", &opcao_destino) != 1 || opcao_destino < 0 || opcao_destino > 2) {
        limpar_buffer(); printf("\nOpcao invalida!\n"); return;
    }
    limpar_buffer();
    if (opcao_destino == 0) { printf("\nTransferencia cancelada.\n"); return; }
    destino = (opcao_destino == 1) ? ARQUIVO_TEXTO : ARQUIVO_BINARIO;

    if (origem == destino) {
        printf("\nO formato de origem e destino sao iguais. Nenhuma transferencia necessaria.\n");
        return;
    }

    const char* nome_origem = (origem == ARQUIVO_TEXTO) ? "Texto (.txt)" : "Binario (.dat)";
    const char* nome_destino = (destino == ARQUIVO_TEXTO) ? "Texto (.txt)" : "Binario (.dat)";

    printf("\nATENCAO: Esta acao lera todos os dados dos arquivos '%s'\n", nome_origem);
    printf("e ira SOBRESCREVER completamente os arquivos '%s'.\n", nome_destino);
    printf("Deseja continuar? (s/n): ");
    char confirmacao;
    scanf(" %c", &confirmacao);
    limpar_buffer();

    if (confirmacao != 's' && confirmacao != 'S') {
        printf("\nTransferencia cancelada pelo usuario.\n");
        return;
    }

    // --- Início da lógica de transferência ---
    printf("\nIniciando transferencia de %s para %s...\n", nome_origem, nome_destino);

    Sistema sistema_temp; // Estrutura temporária na memória stack
    memset(&sistema_temp, 0, sizeof(Sistema)); // Inicializa a estrutura temporária com zeros

    // 1. Carregar da Origem para a Memória Temporária
    printf("Lendo dados da origem (%s)...\n", nome_origem);
    definirModoDeArmazenamento(&sistema_temp, origem);
    carregarTodosOsDadosController(&sistema_temp);
    // Adicionar verificação se algum dado foi carregado seria ideal aqui

    // 2. Salvar da Memória Temporária para o Destino
    printf("Salvando dados no destino (%s)...\n", nome_destino);
    definirModoDeArmazenamento(&sistema_temp, destino); // Muda o modo para as funções 'salvar' saberem o formato
    // Chama todas as funções de salvar, passando a struct temporária
    salvarProdutora(&sistema_temp);
    salvarClientes(&sistema_temp);
    salvarEquipeInterna(&sistema_temp);
    salvarRecursos(&sistema_temp);
    salvarFornecedores(&sistema_temp);
    salvarOperadores(&sistema_temp);
    // Adicione chamadas para salvar outras entidades se houver

    // 3. Liberar Memória da Estrutura Temporária
    printf("Limpando memoria temporaria...\n");
    liberarTodaMemoriaController(&sistema_temp);

    printf("\n------------------------------------------------------\n");
    printf("Transferencia de dados concluida com sucesso!\n");
    printf("Os arquivos no formato '%s' foram atualizados.\n", nome_destino);
    printf("------------------------------------------------------\n");

    // NOTA: Esta função NÃO altera o modo de armazenamento da sessão atual (o 'sistema' principal).
    printf("O modo de salvamento da sessao atual continua: %s\n",
        (obterModoDeArmazenamento(sistema) == ARQUIVO_TEXTO) ? "Arquivo Texto" :
        (obterModoDeArmazenamento(sistema) == ARQUIVO_BINARIO) ? "Arquivo Binario" : "Apenas Memoria");

}

