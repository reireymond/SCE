#ifndef RELATORIO_VIEW_H
#define RELATORIO_VIEW_H
#include "model/sistema.h"
// Menus
void menuRelatoriosView();
int perguntarTipoSaida();

// formularios dos Filtros que serao usados
void form_filtro_cliente(int *min, int *max, int *ordem_alfabetica);
void form_filtro_eventos(int *cod_cliente, char *data_ini, char *data_fim, int *status_op);
void form_filtro_cronograma(int *cod_recurso, char *data_ini, char *data_fim);
void form_filtro_equipamentos(int *min, int *max, char *categoria);
void form_filtro_financeiro(int *cod_entidade, char *data_ini, char *data_fim);

// cabecalhos e Linhas para Tela
void cabecalho_cliente();
void linha_cliente(Cliente *c);
void cabecalho_evento();
void linha_evento(Evento *e);
void cabecalho_cronograma();
void linha_cronograma(char *nome_recurso, char *data, char *hora, char *evento_nome);
void cabecalho_equipamento();
void linha_equipamento(Recurso *r);
void cabecalho_financeiro();
void linha_financeiro(Transacao *t);

void msg_relatorio_concluido();
void msg_arquivo_salvo(char *nome);
void msg_erro_arquivo();

#endif
