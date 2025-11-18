#include "armazenamento_controller.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"
#include "controller/main/main_controller.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Includes necessarios para as funcoes de salvar/carregar
#include "model/produtora/produtora_model.h"
#include "model/cliente/cliente_model.h"
#include "model/equipe_interna/equipe_interna_model.h"
#include "model/recurso/recurso_model.h"
#include "model/fornecedor/fornecedor_model.h"
#include "model/operador/operador_model.h"

void importarDadosDeOutroFormatoController(Sistema *sistema) {
    int opcao;
    TipoArmazenamento origem;
    TipoArmazenamento modo_original = obterModoDeArmazenamento(sistema);

    limpar_tela();
    printf("=== IMPORTAR DADOS ===\n");
    printf("[1] De Binario (.dat)\n");
    printf("[2] De Texto (.txt)\n");
    printf("[0] Cancelar\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limpar_buffer();

    if (opcao == 0) return;
    if (opcao == 1) origem = ARQUIVO_BINARIO;
    else if (opcao == 2) origem = ARQUIVO_TEXTO;
    else { printf("Invalido.\n"); return; }

    printf("\nIsso vai substituir os dados atuais na memoria. Continuar? (s/n): ");
    char conf; scanf(" %c", &conf); limpar_buffer();
    if (conf != 's' && conf != 'S') return;

    // Limpa e carrega no novo formato
    liberarTodaMemoriaController(sistema);
    definirModoDeArmazenamento(sistema, origem);
    carregarTodosOsDadosController(sistema);
    
    // Volta pro modo original (ou mantem se quiser, aqui voltamos pra nao baguncar)
    definirModoDeArmazenamento(sistema, modo_original);
    printf("Dados importados!\n");
}

void transferirDadosDeArmazenamento(Sistema *sistema) {
    // Simples conversao: Carrega de um jeito, Salva de outro
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

    liberarTodaMemoriaController(&temp);
    printf("Convertido com sucesso!\n");
}