// Previne a inclusão múltipla deste arquivo de cabeçalho, evitando erros de
// compilação por redefinição de funções ou tipos.
#ifndef RECURSO_MODEL_H
#define RECURSO_MODEL_H

// Inclui a definição da estrutura principal 'Sistema'. Todas as funções neste
// módulo precisam de um ponteiro para 'Sistema' para poder acessar e
// manipular a lista de recursos e equipamentos.
#include "model/sistema.h"

// Declaração da função para salvar a lista de recursos em um arquivo.
// A implementação decidirá o formato (binário ou texto) com base na
// configuração atual do sistema.
void salvarRecursos(Sistema *sistema);

// Declaração da função para carregar a lista de recursos de um arquivo para a memória.
// Esta função é executada no início do programa para restaurar os dados da última sessão.
void carregarRecursos(Sistema *sistema);

// Declaração da função para liberar a memória alocada para a lista de recursos.
// É uma função de limpeza essencial para ser chamada ao final da execução do programa
// para evitar vazamentos de memória.
void liberarMemoriaRecursos(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif