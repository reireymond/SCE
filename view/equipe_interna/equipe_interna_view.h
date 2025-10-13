#ifndef EQUIPE_INTERNA_VIEW_H
#define EQUIPE_INTERNA_VIEW_H

#include "model/sistema.h"  // Para acessar os dados do sistema

// Exibe o menu de gerenciamento da equipe interna (Adicionar, Alterar, Listar, Excluir)
void menuEquipeInternaView(Sistema *sistema);

// Lista todos os membros da equipe interna de forma organizada
void listarEquipeInternaView(Sistema *sistema);

#endif
