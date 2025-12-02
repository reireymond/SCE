#ifndef OPERADOR_VIEW_H
#define OPERADOR_VIEW_H
#include "model/sistema.h"

void menuOperadoresView(Sistema *sistema);
void listarOperadoresView(Sistema *sistema);

void formulario_novo_operador(Operador *op);
int pedir_id_operador(const char *acao);
int menu_alterar_operador(Operador *op);
int confirmar_exclusao_operador(char *nome);
//login
void tela_login(char *usuario_dest, char *senha_dest);
void mensagem_login_falhou(int tentativa, int max);
void mensagem_login_sucesso(char *nome);
void mensagem_primeiro_acesso();

#endif