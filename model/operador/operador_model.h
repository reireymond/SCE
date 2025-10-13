// Previne que este arquivo de cabeçalho seja incluído múltiplas vezes,
// o que causaria erros de compilação por definições duplicadas.
#ifndef OPERADOR_MODEL_H
#define OPERADOR_MODEL_H

// Inclui a definição da estrutura principal 'Sistema'. As funções declaradas aqui
// precisam de acesso a esta estrutura para manipular a lista de operadores.
#include "model/sistema.h"

// Declaração da função para salvar os dados dos operadores.
// Esta função será responsável por escrever a lista de operadores no
// modo de armazenamento configurado (seja arquivo binário ou de texto).
void salvarOperadores(Sistema *sistema);

// Declaração da função para carregar os dados dos operadores.
// Ao iniciar o programa, esta função lê os dados de um arquivo e popula
// a lista de operadores na memória.
void carregarOperadores(Sistema *sistema);

// Declaração da função para liberar a memória alocada para a lista de operadores.
// Essencial para ser chamada ao encerrar o programa para prevenir vazamentos de memória.
void liberarMemoriaOperadores(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif