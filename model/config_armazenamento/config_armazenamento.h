#ifndef CONFIG_ARMAZENAMENTO_H
#define CONFIG_ARMAZENAMENTO_H

// Incluimos o sistema.h para saber o que é "TipoArmazenamento" e "Sistema"
#include "model/sistema.h"

void definirModoDeArmazenamento(Sistema *sistema, TipoArmazenamento modo);
TipoArmazenamento obterModoDeArmazenamento(Sistema *sistema);

#endif