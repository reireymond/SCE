#ifndef EVENTO_CONTROLLER_H
#define EVENTO_CONTROLLER_H
#include "model/sistema.h"
void adicionarEventoController(Sistema *sistema); // (Criar Orçamento)
void alterarStatusEventoController(Sistema *sistema); // (Aprovar)
void finalizarEventoController(Sistema *sistema); // (Finalizar e Faturar)
void detalharEventoController(Sistema *sistema); // (Ver itens de um evento)
#endif
