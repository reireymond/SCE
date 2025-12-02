#ifndef PRODUTORA_VIEW_H
#define PRODUTORA_VIEW_H

#include "model/sistema.h"

void menuProdutoraView(Sistema *sistema);
void verDetalhesProdutoraView(Sistema *sistema);

// para usar os printfs do sistema
void formulario_produtora(Produtora *p); 
int menu_alterar_campo_produtora(Produtora *p); 
void mensagem_erro(const char *msg);

#endif