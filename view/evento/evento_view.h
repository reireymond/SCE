#ifndef EVENTO_VIEW_H
#define EVENTO_VIEW_H
#include "model/sistema.h"

void menuEventosView(Sistema *sistema);
void listarEventosView(Sistema *sistema);

void formulario_novo_evento_basico(Evento *e);
int perguntar_se_adiciona_recurso();
void formulario_adicionar_recurso(int *cod, int *qtd);
int perguntar_se_adiciona_equipe();
void formulario_adicionar_equipe(int *cod);
int perguntar_se_adiciona_fornecedor();
void formulario_adicionar_fornecedor(int *cod);

int pedir_id_evento(const char *acao);


void msg_fornecedor_adicionado_sucesso(char *nome_fornecedor, float custo);
void msg_recurso_adicionado();
void msg_recurso_nao_encontrado();
void msg_conflito_estoque(int cod, int total, int usado);
void msg_evento_aprovado();


void msg_verificando_disponibilidade();
void msg_conflito_equipe(int cod, char *nome_evento);
void msg_conflito_fornecedor(int cod, char *nome_evento);
void msg_erro_aprovacao_conflito();
void msg_membro_equipe_adicionado();
void msg_membro_nao_encontrado();
void msg_evento_finalizado_sucesso(float valor);

#endif
