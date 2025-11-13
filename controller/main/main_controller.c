// Controller principal do sistema
#include "main_controller.h"
#include <stdio.h>

// View principal (menu de configuração)
#include "view/main/main_view.h"

// Funções utilitárias (ex: pausar)
#include "utils/utils.h"

// Models usados para carregar e liberar dados
#include "model/produtora/produtora_model.h"
#include "model/cliente/cliente_model.h"
#include "model/equipe_interna/equipe_interna_model.h"
#include "model/recurso/recurso_model.h"
#include "model/fornecedor/fornecedor_model.h"
#include "model/operador/operador_model.h"
#include "model/evento/evento_model.h"
#include "model/transacao/transacao_model.h"


// Define o tipo de armazenamento inicial (memória, texto ou binário)
void configuracaoInicialController(Sistema *sistema) {
    menuEscolherArmazenamentoView(sistema);
}


// Carrega todos os dados das entidades do sistema
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


// Libera toda a memória alocada pelo sistema antes de encerrar
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
