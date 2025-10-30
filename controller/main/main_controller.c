// Inclui o cabeçalho do controller principal, contendo as declarações das funções
#include "main_controller.h"
#include <stdio.h>
// Inclui o cabeçalho da view principal para poder chamar a função 'menuEscolherArmazenamentoView'.
#include "view/main/main_view.h"
// Inclui o cabeçalho de utilitários para usar a função 'pausar'.
#include "utils/utils.h"
// possa chamar as funções de carregamento e liberação de memória de cada entidade.
#include "model/produtora/produtora_model.h"
#include "model/cliente/cliente_model.h"
#include "model/equipe_interna/equipe_interna_model.h"
#include "model/recurso/recurso_model.h"
#include "model/fornecedor/fornecedor_model.h"
#include "model/operador/operador_model.h"

// Implementação da função que gerencia a configuração inicial do sistema.
void configuracaoInicialController(Sistema *sistema) {
    // A única responsabilidade deste controller é chamar a view correspondente.
    // A view 'menuEscolherArmazenamentoView' irá interagir com o usuário
    // e, a partir da escolha dele, chamar a função do model para definir o modo de armazenamento.
    menuEscolherArmazenamentoView(sistema);
}

// Implementação da função que carrega todos os dados do sistema.
void carregarTodosOsDadosController(Sistema *sistema) {
    // Esta função chama sequencialmente a função de carregamento de cada model.
    // Cada uma dessas funções verificará o modo de armazenamento e carregará
    // os dados da memória, de um arquivo de texto ou de um arquivo binário, conforme apropriado.
    carregarProdutora(sistema);
    carregarClientes(sistema);
    carregarEquipeInterna(sistema);
    carregarRecursos(sistema);
    carregarFornecedores(sistema);
    carregarOperadores(sistema);
}

// Implementação da função que libera toda a memória alocada pelo sistema.
void liberarTodaMemoriaController(Sistema *sistema) {
    // Esta função centraliza a limpeza da memória, chamando a função de liberação
    // de cada model. Isso garante que todos os ponteiros de listas dinâmicas
    // sejam liberados corretamente antes que o programa termine.
    liberarMemoriaProdutora(sistema);
    liberarMemoriaClientes(sistema);
    liberarMemoriaEquipe(sistema);
    liberarMemoriaRecursos(sistema);
    liberarMemoriaFornecedores(sistema);
    liberarMemoriaOperadores(sistema);
}