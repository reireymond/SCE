// Previne a inclusão múltipla deste arquivo de cabeçalho, uma prática de segurança
// para evitar erros de compilação por redefinição de funções.
#ifndef EQUIPE_INTERNA_VIEW_H
#define EQUIPE_INTERNA_VIEW_H

// Inclui a definição da estrutura principal 'Sistema'. As funções da view precisam
// de um ponteiro para 'Sistema' para poderem ler e exibir os dados contidos nela,
// como a lista de membros da equipe.
#include "model/sistema.h"

// Declaração da função que exibe o menu de gerenciamento da equipe interna.
// Este menu apresenta ao usuário as opções como Adicionar, Alterar, Listar e Excluir,
// e, com base na escolha, chama a função do controller correspondente.
void menuEquipeInternaView(Sistema *sistema);

// Declaração da função que exibe a lista formatada de todos os membros da equipe.
// Esta função percorre a lista de membros no sistema e imprime os detalhes de cada
// um na tela de forma organizada e legível.
void listarEquipeInternaView(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif