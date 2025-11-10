// Evita inclusão múltipla deste cabeçalho.
#ifndef CLIENTE_MODEL_H
#define CLIENTE_MODEL_H

// Inclui a estrutura principal do sistema (necessária para manipular clientes).
#include "model/sistema.h"

// Salva a lista de clientes (em binário ou texto, conforme configuração).
void salvarClientes(Sistema *sistema);

// Carrega os dados dos clientes do arquivo para a memória.
void carregarClientes(Sistema *sistema);

// Libera a memória alocada para a lista de clientes.
void liberarMemoriaClientes(Sistema *sistema);

#endif
