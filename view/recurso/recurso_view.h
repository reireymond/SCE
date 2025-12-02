#ifndef RECURSO_VIEW_H
#define RECURSO_VIEW_H
#include "model/sistema.h"

// Exibe o menu de gerenciamento de recursos
void menuRecursosView(Sistema *sistema);

void listarRecursosView(Sistema *sistema);
void formulario_novo_recurso(Recurso *r);
int pedir_id_recurso(const char *acao);
int menu_alterar_recurso(Recurso *r);
int confirmar_exclusao_recurso(char *nome);
void mensagem_sucesso(const char *msg);
void mensagem_erro(const char *msg);

#endif 
