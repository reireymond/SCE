#ifndef FORNECEDOR_VIEW_H
#define FORNECEDOR_VIEW_H
#include "model/sistema.h"

void menuFornecedoresView(Sistema *sistema);
void listarFornecedoresView(Sistema *sistema);

void formulario_novo_fornecedor(Fornecedor *f);
int pedir_id_fornecedor(const char *acao);
int menu_alterar_fornecedor(Fornecedor *f);
int confirmar_exclusao_fornecedor(char *nome);

#endif