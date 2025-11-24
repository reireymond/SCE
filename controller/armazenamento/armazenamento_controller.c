#include "armazenamento_controller.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"
#include "controller/main/main_controller.h"
#include "utils/utils.h"
#include "utils/validation.h"
// Includes para as funcoes de salvar/carregar
#include "model/produtora/produtora_model.h"
#include "model/cliente/cliente_model.h"
#include "model/equipe_interna/equipe_interna_model.h"
#include "model/recurso/recurso_model.h"
#include "model/fornecedor/fornecedor_model.h"
#include "model/operador/operador_model.h"
#include "model/evento/evento_model.h"
#include "model/transacao/transacao_model.h"

void transferirDadosDeArmazenamento(Sistema *sistema) {
    
    int op1, op2;
    TipoArmazenamento origem, destino;

    limpar_tela();
    printf("=== CONVERTER ARQUIVOS ===\n");
    printf("Origem: [1] Binario [2] Texto: ");
    scanf("%d", &op1);
    if(op1 == 1) origem = ARQUIVO_BINARIO; else origem = ARQUIVO_TEXTO;

    printf("Destino: [1] Binario [2] Texto: ");
    scanf("%d", &op2);
    if(op2 == 1) destino = ARQUIVO_BINARIO; else destino = ARQUIVO_TEXTO;

    if (origem == destino) { printf("Mesmo formato, nada a fazer.\n"); return; }

    printf("\nTem certeza que deseja converter de [%s] para [%s]? (s/n): ", 
           (origem == ARQUIVO_BINARIO ? "Binario" : "Texto"),
           (destino == ARQUIVO_BINARIO ? "Binario" : "Texto"));
    
    char conf; 
    scanf(" %c", &conf); 
    limpar_buffer();

    if (conf != 's' && conf != 'S') {
        printf("Transferencia cancelada.\n");
        return;
    }
    
    Sistema temp;
    memset(&temp, 0, sizeof(Sistema));

    printf("Lendo origem...\n");
    definirModoDeArmazenamento(&temp, origem);
    carregarTodosOsDadosController(&temp);

    printf("Salvando destino...\n");
    definirModoDeArmazenamento(&temp, destino);
    
    // Salva tudo no formato novo
    salvarProdutora(&temp);
    salvarClientes(&temp);
    salvarEquipeInterna(&temp);
    salvarRecursos(&temp);
    salvarFornecedores(&temp);
    salvarOperadores(&temp);
    salvarEventos(&temp);
    salvarTransacoes(&temp);


    liberarTodaMemoriaController(&temp);
   
    printf(" Conversao concluida com sucesso! Novos arquivos criados.\n"); 
}