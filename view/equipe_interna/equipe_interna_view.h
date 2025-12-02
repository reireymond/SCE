#ifndef EQUIPE_INTERNA_VIEW_H
#define EQUIPE_INTERNA_VIEW_H
#include "model/sistema.h"

void menuEquipeInternaView(Sistema *sistema);
void listarEquipeInternaView(Sistema *sistema);

void formulario_novo_equipe(EquipeInterna *e);
int pedir_id_equipe(const char *acao);
int menu_alterar_equipe(EquipeInterna *e);
int confirmar_exclusao_equipe(char *nome);

#endif