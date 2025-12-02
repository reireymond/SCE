#include "armazenamento_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/armazenamento/armazenamento_controller.h"

void menuConverterArquivosView(Sistema *sistema) {
    int op1, op2;
    TipoArmazenamento origem, destino;

    limpar_tela();
    printf("=== CONVERTER ARQUIVOS (TEXTO <-> BINARIO) ===\n");
    printf("ATENCAO: Isso vai recarregar os dados e salvar no novo formato.\n\n");
    
    printf("Qual o formato ATUAL dos arquivos? \n");
    printf("[1] Binario (.dat)\n");
    printf("[2] Texto (.txt)\n");
    printf("Escolha: ");
    scanf("%d", &op1);
    limpar_buffer();
    
    if(op1 == 1) origem = ARQUIVO_BINARIO; else origem = ARQUIVO_TEXTO;

    printf("\nPara qual formato deseja CONVERTER? \n");
    printf("[1] Binario (.dat)\n");
    printf("[2] Texto (.txt)\n");
    printf("Escolha: ");
    scanf("%d", &op2);
    limpar_buffer();

    if(op2 == 1) destino = ARQUIVO_BINARIO; else destino = ARQUIVO_TEXTO;

    if (origem == destino) { 
        printf("\nVoce escolheu o mesmo formato. Nada a fazer.\n"); 
        return; 
    }

    printf("\nTem certeza? (s/n): ");
    char conf; 
    scanf(" %c", &conf); 
    limpar_buffer();

    if (conf == 's' || conf == 'S') {
        converterDadosController(sistema, origem, destino);
    } else {
        printf("Cancelado.\n");
    }
}

void msg_conversao_sucesso() {
    printf("\nProcesso interno finalizado! Arquivos convertidos com sucesso.\n");
    pausar();
}