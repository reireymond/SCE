#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "model/sistema.h"


int menuEscolherArmazenamentoView();

// Menus de navegacao
void menuPrincipalView(Sistema *sistema);
void menuGestaoDeDadosView(Sistema *sistema);
void menuImportarExportarView(Sistema *sistema);

#endif 
