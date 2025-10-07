// Garante que o conteúdo deste arquivo de cabeçalho seja incluído apenas uma vez
// durante o processo de compilação, prevenindo erros de redefinição.
#ifndef FORNECEDOR_MODEL_H
#define FORNECEDOR_MODEL_H

// Inclui a definição da estrutura principal 'Sistema'. As funções declaradas
// neste arquivo precisam acessar e manipular a lista de fornecedores que
// está dentro da estrutura 'Sistema'.
#include "model/sistema.h"

// Declaração da função para salvar a lista de fornecedores em um arquivo.
// A implementação desta função irá verificar o modo de armazenamento configurado
// (binário ou texto) e persistir os dados adequadamente.
void salvarFornecedores(Sistema *sistema);

// Declaração da função para carregar a lista de fornecedores de um arquivo para a memória.
// Isso permite que os dados persistam entre as execuções do programa.
void carregarFornecedores(Sistema *sistema);

// Declaração da função para liberar a memória que foi alocada dinamicamente
// para armazenar a lista de fornecedores. É crucial para evitar vazamentos de memória.
void liberarMemoriaFornecedores(Sistema *sistema);

// Fim da diretiva de pré-processador.
#endif