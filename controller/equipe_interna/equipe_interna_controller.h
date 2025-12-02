#ifndef EQUIPE_INTERNA_CONTROLLER_H
#define EQUIPE_INTERNA_CONTROLLER_H

#include "model/sistema.h"

// Adiciona um novo membro à equipe interna
void adicionarEquipeInternaController(Sistema *sistema);

// Altera os dados de um membro existente
void alterarEquipeInternaController(Sistema *sistema);

// Exclui um membro da equipe interna
void excluirEquipeInternaController(Sistema *sistema);

#endif