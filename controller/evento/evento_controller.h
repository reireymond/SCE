#ifndef EVENTO_CONTROLLER_H
#define EVENTO_CONTROLLER_H

#include "model/sistema.h"

void adicionarEventoController(Sistema *sistema);
void editarEventoController(Sistema *sistema);
void alterarStatusEventoController(Sistema *sistema);
void cancelarEventoController(Sistema *sistema); // NOVO
void finalizarEventoController(Sistema *sistema);
void detalharEventoController(Sistema *sistema);

#endif