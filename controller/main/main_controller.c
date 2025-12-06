#include "main_controller.h"
#include <stdio.h>
#include "view/main/main_view.h"
#include "utils/utils.h"
#include "model/config_armazenamento/config_armazenamento.h"
#include "model/produtora/produtora_model.h"
#include "model/cliente/cliente_model.h"
#include "model/equipe_interna/equipe_interna_model.h"
#include "model/recurso/recurso_model.h"
#include "model/fornecedor/fornecedor_model.h"
#include "model/operador/operador_model.h"
#include "model/evento/evento_model.h"
#include "model/transacao/transacao_model.h"
// Define o tipo de armazenamento inicial
void configuracaoInicialController(Sistema *sistema) {
   
    int opcao = menuEscolherArmazenamentoView();

    // Aplica a logica aqui no controller
    if(opcao == 2) definirModoDeArmazenamento(sistema, ARQUIVO_TEXTO);
    else if(opcao == 3) definirModoDeArmazenamento(sistema, MEMORIA);
    else definirModoDeArmazenamento(sistema, ARQUIVO_BINARIO);
}


// Carrega todos os dados do sistema
void carregarTodosOsDadosController(Sistema *sistema) {
    carregarProdutora(sistema);
    carregarClientes(sistema);
    carregarEquipeInterna(sistema);
    carregarRecursos(sistema);
    carregarFornecedores(sistema);
    carregarOperadores(sistema);
    carregarEventos(sistema);
    carregarTransacoes(sistema);
    carregarCaixa(sistema);
}


// Libera toda a memoria alocada pelo sistema antes de encerrar
void liberarTodaMemoriaController(Sistema *sistema) {
    liberarMemoriaProdutora(sistema);
    liberarMemoriaClientes(sistema);
    liberarMemoriaEquipe(sistema);
    liberarMemoriaRecursos(sistema);
    liberarMemoriaFornecedores(sistema);
    liberarMemoriaOperadores(sistema);
    liberarMemoriaEventos(sistema);
    liberarMemoriaTransacoes(sistema);
}

