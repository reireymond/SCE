#ifndef CONFIG_ARMAZENAMENTO_H
#define CONFIG_ARMAZENAMENTO_H

// Inclui o sistema.h pra reconhecer os tipos, sem redefinir nada
#include "model/sistema.h"

// Apenas os prototipos das funcoes
void definirModoDeArmazenamento(Sistema *sistema, TipoArmazenamento modo);
TipoArmazenamento obterModoDeArmazenamento(Sistema *sistema);

#endif