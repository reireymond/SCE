// Previne a inclusão múltipla deste arquivo de cabeçalho, um mecanismo padrão
// para evitar erros de compilação por redefinição de funções.
#ifndef EQUIPE_INTERNA_CONTROLLER_H
#define EQUIPE_INTERNA_CONTROLLER_H

// Inclui a definição da estrutura principal 'Sistema'. Todas as funções do controller
// recebem um ponteiro para esta estrutura, pois elas precisam interagir com os dados
// globais da aplicação, como a lista de membros da equipe.
#include "model/sistema.h"

// Declaração da função do controller para adicionar um novo membro à equipe.
// Esta função irá gerenciar o processo de coletar os dados do novo membro
// e inseri-lo na lista de equipe do sistema.
void adicionarEquipeController(Sistema *sistema);

// Declaração da função do controller para alterar os dados de um membro existente.
// Esta função será responsável pela lógica de selecionar um membro,
// solicitar as novas informações e atualizar o registro correspondente.
void alterarEquipeController(Sistema *sistema);

// Declaração da função do controller para excluir um membro da equipe.
// Irá gerenciar a localização do membro na lista e sua remoção.
void excluirEquipeController(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif