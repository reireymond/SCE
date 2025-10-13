// Previne a inclusão duplicada deste arquivo de cabeçalho, um mecanismo de segurança
// para evitar erros de compilação por redefinição de tipos e funções.
#ifndef PRODUTORA_MODEL_H
#define PRODUTORA_MODEL_H

// Inclui a definição da estrutura principal do sistema. As funções neste módulo
// precisam de um ponteiro para a estrutura 'Sistema' para poder acessar e
// modificar os dados da produtora.
#include "model/sistema.h"

// Declaração da função que salva os dados da produtora.
// Esta função irá persistir as informações da empresa no arquivo correspondente
// (binário ou texto), conforme a configuração do sistema.
void salvarProdutora(Sistema *sistema);

// Declaração da função que carrega os dados da produtora.
// No início do programa, esta função lê as informações da produtora do arquivo
// e as carrega para a memória.
void carregarProdutora(Sistema *sistema);

// Declaração da função para liberar a memória alocada para os dados da produtora.
// Como os dados da produtora são alocados dinamicamente, esta função é
// essencial para liberar a memória ao fechar o programa e evitar memory leaks.
void liberarMemoriaProdutora(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif