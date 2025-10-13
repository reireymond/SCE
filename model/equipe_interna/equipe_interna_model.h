// Previne a inclusão múltipla deste arquivo de cabeçalho, o que poderia causar
// erros de compilação devido à redefinição de tipos e funções.
#ifndef EQUIPE_INTERNA_MODEL_H
#define EQUIPE_INTERNA_MODEL_H

// Inclui o cabeçalho principal do sistema. Isso é necessário porque as funções
// aqui declaradas operam sobre a estrutura 'Sistema', que é o contêiner
// central de todos os dados da aplicação.
#include "model/sistema.h"

// Declaração da função para salvar os dados da equipe interna.
// Esta função será responsável por persistir a lista de membros da equipe
// no modo de armazenamento que estiver configurado no sistema (arquivo de texto ou binário).
void salvarEquipeInterna(Sistema *sistema);

// Declaração da função para carregar os dados da equipe interna.
// Ela lerá os dados de um arquivo e os carregará para a lista em memória,
// permitindo que o sistema continue de onde parou na última execução.
void carregarEquipeInterna(Sistema *sistema);

// Declaração da função para liberar a memória alocada para a lista da equipe.
// É uma função de "limpeza", essencial para ser chamada no final do programa
// e evitar vazamentos de memória (memory leaks).
void liberarMemoriaEquipe(Sistema *sistema);

// Fim da diretiva de pré-processador.
#endif