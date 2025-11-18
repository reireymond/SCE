#ifndef EVENTO_CONTROLLER_H
#define EVENTO_CONTROLLER_H

#include "model/sistema.h"

void adicionarEventoController(Sistema *sistema);
void alterarStatusEventoController(Sistema *sistema); // Aprovar
void finalizarEventoController(Sistema *sistema);     // Fechar e cobrar

#endif