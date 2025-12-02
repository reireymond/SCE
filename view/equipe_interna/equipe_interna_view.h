#ifndef EQUIPE_INTERNA_VIEW_H
#define EQUIPE_INTERNA_VIEW_H

#include "model/sistema.h"  // Para acessar os dados do sistema

// Exibe o menu de gerenciamento da equipe interna (Adicionar, Alterar, Listar, Excluir)
void menuEquipeInternaView(Sistema *sistema);

// Lista todos os membros da equipe interna de forma organizada
void listarEquipeInternaView(Sistema *sistema);

void mensagem_sucesso(const char *msg);
void mensagem_erro(const char *msg);
void mensagem_aviso(const char *msg);

// O Controller chama isso para preencher dados
void formulario_novo_EquipeInterna(EquipeInterna *equipe);
int pedir_id_EquipeInterna(const char *acao); 

int menu_alterar_EquipeInterna(EquipeInterna *equipe);
void atualizar_campo_EquipeInterna(EquipeInterna *equipe, int opcao);
int confirmar_exclusao(const char *nome);


#endif
