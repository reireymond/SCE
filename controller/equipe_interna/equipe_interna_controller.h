#ifndef EQUIPE_INTERNA_CONTROLLER_H
#define EQUIPE_INTERNA_CONTROLLER_H

#include "model/sistema.h"

// Controller para gerenciar membros da equipe interna do sistema

// Recebe um ponteiro para o sistema para acessar a lista de membros
void adicionarEquipeController(Sistema *sistema);

// Permite selecionar o membro e atualizar suas informações
void alterarEquipeController(Sistema *sistema);

// Localiza o membro na lista e remove o registro correspondente
void excluirEquipeController(Sistema *sistema);

#endif
