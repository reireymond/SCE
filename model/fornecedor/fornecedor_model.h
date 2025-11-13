// Evita inclusão múltipla deste cabeçalho durante a compilação
#ifndef FORNECEDOR_MODEL_H
#define FORNECEDOR_MODEL_H

// Necessário para acessar a estrutura principal do sistema e sua lista de fornecedores
#include "model/sistema.h"

// Salva a lista de fornecedores no formato definido (binário ou texto)
void salvarFornecedores(Sistema *sistema);

// Carrega os dados dos fornecedores de um arquivo para a memória
void carregarFornecedores(Sistema *sistema);

// Libera a memória alocada para a lista de fornecedores
void liberarMemoriaFornecedores(Sistema *sistema);

#endif
