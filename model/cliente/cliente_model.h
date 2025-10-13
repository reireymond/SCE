// Garante que este arquivo de cabeçalho seja incluído apenas uma vez durante a compilação,
// prevenindo erros de redefinição de funções.
#ifndef CLIENTE_MODEL_H
#define CLIENTE_MODEL_H

// Inclui a definição da estrutura principal do sistema, pois as funções
// declaradas aqui precisam manipular os dados dos clientes contidos nela.
#include "model/sistema.h"

// Declaração da função para salvar a lista de clientes.
// A implementação desta função decidirá se salva em arquivo binário ou de texto,
// dependendo da configuração do sistema.
void salvarClientes(Sistema *sistema);

// Declaração da função para carregar a lista de clientes.
// Esta função lerá os dados de um arquivo (binário ou texto) e os carregará
// para a memória, preenchendo a lista de clientes na estrutura do sistema.
void carregarClientes(Sistema *sistema);

// Declaração da função para liberar a memória alocada para a lista de clientes.
// É uma função importante para garantir que não haja vazamento de memória
// ao final da execução do programa.
void liberarMemoriaClientes(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif