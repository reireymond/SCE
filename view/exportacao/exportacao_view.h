#ifndef EXPORTCAO
#define EXPORTCAO_VIEW_H

#include "model/sistema.h"

// Menu Principal do XML
void menuXMLView();
int obterOpcaoXML();

// Menu para escolher qual tabela
int menuEscolherTabela();
// Pede o nome do arquivo
void pedirNomeArquivo(char *buffer);
void msg_exportacao_sucesso(char *arquivo);
void msg_importacao_sucesso(int qtd);
void msg_erro_arquivo_xml();
void msg_lendo_arquivo();
void msg_gravando_arquivo();

#endif