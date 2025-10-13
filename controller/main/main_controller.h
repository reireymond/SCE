// Previne a inclusão múltipla deste arquivo de cabeçalho, uma prática padrão
// para evitar erros de compilação por redefinição de funções.
#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

// Inclui a definição da estrutura principal 'Sistema'. Todas as funções neste
// controller operam sobre esta estrutura, gerenciando o estado geral da aplicação.
#include "model/sistema.h"

// Funções para a configuração inicial e o ciclo de vida dos dados do sistema.

// Declaração da função que gerencia a configuração inicial do programa.
// Ela é responsável por chamar a view que pergunta ao usuário
// como ele deseja que os dados sejam armazenados (memória, texto ou binário).
void configuracaoInicialController(Sistema *sistema);

// Declaração da função para carregar todos os dados do sistema.
// Com base no modo de armazenamento escolhido, esta função chama as funções
// de carregamento de cada um dos models (cliente, fornecedor, etc.).
void carregarTodosOsDadosController(Sistema *sistema);

// Declaração da função para liberar toda a memória alocada dinamicamente.
// É uma função de limpeza crucial, chamada no final da execução do programa
// para garantir que não haja vazamentos de memória.
void liberarTodaMemoriaController(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif // MAIN_CONTROLLER_H