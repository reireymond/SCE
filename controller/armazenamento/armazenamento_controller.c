#include "armazenamento_controller.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"
#include "controller/main/main_controller.h"
#include "model/produtora/produtora_model.h"
#include "model/cliente/cliente_model.h"
#include "model/equipe_interna/equipe_interna_model.h"
#include "model/recurso/recurso_model.h"
#include "model/fornecedor/fornecedor_model.h"
#include "model/operador/operador_model.h"
#include "model/evento/evento_model.h"
#include "model/transacao/transacao_model.h"

void converterDadosController(Sistema *sistema, TipoArmazenamento origem, TipoArmazenamento destino) {
    Sistema temp;
    memset(&temp, 0, sizeof(Sistema));

    definirModoDeArmazenamento(&temp, origem);
    carregarTodosOsDadosController(&temp);

    definirModoDeArmazenamento(&temp, destino);
    
    salvarProdutora(&temp);
    salvarClientes(&temp);
    salvarEquipeInterna(&temp);
    salvarRecursos(&temp);
    salvarFornecedores(&temp);
    salvarOperadores(&temp);
    salvarEventos(&temp);
    salvarTransacoes(&temp);

    liberarTodaMemoriaController(&temp);
   
    printf("\nProcesso interno finalizado! Arquivos convertidos.\n"); 
}
