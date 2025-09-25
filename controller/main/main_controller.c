#include "main_controller.h"
#include <stdio.h>
#include "view/main/main_view.h" // O controller chama a view

// Incluir todos os models para carregar e liberar
#include "model/produtora/produtora.h"
#include "model/cliente/cliente.h"
#include "model/equipe_interna/equipe_interna.h"
#include "model/recurso/recurso.h"
#include "model/fornecedor/fornecedor.h"
#include "model/operador/operador.h"

void configuracaoInicialController(Sistema *sistema) {
    // A view é responsável pela interação com o usuário
    menuEscolherArmazenamentoView(sistema);
}

void carregarTodosOsDadosController(Sistema *sistema) {
    printf("\nCarregando dados...\n");
    carregarProdutora(sistema);
    carregarClientes(sistema);
    // carregarEquipeInterna(sistema);
    // carregarRecursos(sistema);
    // carregarFornecedores(sistema);
    // carregarOperadores(sistema);
    printf("Dados carregados.\n");
    pausar();
}

void liberarTodaMemoriaController(Sistema *sistema) {
    liberarMemoriaProdutora(sistema);
    liberarMemoriaClientes(sistema);
    // liberarMemoriaEquipe(sistema);
    // liberarMemoriaRecursos(sistema);
    // liberarMemoriaFornecedores(sistema);
    // liberarMemoriaOperadores(sistema);
}