#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "model/sistema.h"  // Para acessar os dados do sistema

// Exibe o menu para escolher o modo de armazenamento (memória, texto, binário)
void menuEscolherArmazenamentoView(Sistema *sistema);

// Exibe o menu principal da aplicação
void menuPrincipalView(Sistema *sistema);

// Exibe o menu de gestão de dados (clientes, fornecedores, etc.)
void menuGestaoDeDadosView(Sistema *sistema);

// Exibe o menu de importação e exportação de dados (CORREÇÃO: Adicionada declaração)
void menuImportarExportarView(Sistema *sistema);


#endif 