#ifndef RECURSO_MODEL_H
#define RECURSO_MODEL_H

#include "model/sistema.h" // Necessário para acessar a estrutura 'Sistema'

// Salva a lista de recursos no formato definido (binário ou texto)
void salvarRecursos(Sistema *sistema);

// Carrega os recursos do arquivo para a memória
void carregarRecursos(Sistema *sistema);

// Libera a memória alocada para os recursos
void liberarMemoriaRecursos(Sistema *sistema);

#endif
