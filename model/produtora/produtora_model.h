// Evita a inclusão duplicada deste cabeçalho, prevenindo erros de redefinição
#ifndef PRODUTORA_MODEL_H
#define PRODUTORA_MODEL_H

// Inclui a definição da estrutura principal do sistema
#include "model/sistema.h"

// Salva os dados da produtora conforme o modo de armazenamento configurado
void salvarProdutora(Sistema *sistema);

// Carrega os dados da produtora do arquivo para a memória
void carregarProdutora(Sistema *sistema);

// Libera a memória alocada para os dados da produtora
void liberarMemoriaProdutora(Sistema *sistema);

#endif
