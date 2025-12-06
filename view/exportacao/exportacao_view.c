#include "exportacao_view.h"
#include <stdio.h>
#include <string.h>
#include "utils/utils.h"
#include "utils/validation.h"

void menuXMLView() {
    limpar_tela();
    printf("+=====================================================+\n");
    printf("|              IMPORTACAO E EXPORTACAO XML            |\n");
    printf("+=====================================================+\n");
    printf("| [1] Exportar Dados (Gerar XML)                      |\n");
    printf("| [2] Importar Dados (Ler XML)                        |\n");
    printf("+-----------------------------------------------------+\n");
    printf("| [0] Voltar                                          |\n");
    printf("+=====================================================+\n");
    printf("Escolha uma opcao: ");
}

int obterOpcaoXML() {
    int op;
    ler_inteiro_valido(&op, 0, 2);
    return op;
}

int menuEscolherTabela() {
    int op;
    printf("\nQual tabela deseja manipular?\n");
    printf("[1] Clientes\n");
    printf("[2] Equipamentos (Recursos)\n");
    printf("[3] Eventos\n");
    printf("[0] Cancelar\n");
    printf("Escolha: ");
    ler_inteiro_valido(&op, 0, 3);
    return op;
}

void pedirNomeArquivo(char *buffer) {
    printf("\nDigite o nome do arquivo (ex: dados.xml): ");
    fflush(stdin);
    fgets(buffer, 50, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
  
}

void msg_exportacao_sucesso(char *arquivo) {
    printf("\n[SUCESSO] Dados exportados para '%s'!\n", arquivo);
    pausar();
}

void msg_importacao_sucesso(int qtd) {
    printf("\n[SUCESSO] Importacao finalizada! %d registros processados.\n", qtd);
    pausar();
}

void msg_erro_arquivo_xml() {
    printf("\n[ERRO] Nao foi possivel abrir/criar o arquivo.\n");
    pausar();
}

void msg_lendo_arquivo() {
    printf("Lendo arquivo XML... Aguarde...\n");
}

void msg_gravando_arquivo() {
    printf("Gravando arquivo XML... Aguarde...\n");
}