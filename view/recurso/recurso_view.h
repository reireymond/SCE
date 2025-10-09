// Previne a inclusão múltipla deste arquivo de cabeçalho. É uma guarda de compilação
// para evitar erros de redefinição de funções caso este arquivo seja incluído mais
// de uma vez na mesma unidade de compilação.
#ifndef RECURSO_VIEW_H
#define RECURSO_VIEW_H

// Inclui a definição da estrutura principal 'Sistema'. As funções da view precisam
// de um ponteiro para 'Sistema' para poderem ler e exibir os dados dos recursos
// que estão armazenados na estrutura.
#include "model/sistema.h"

// Declaração da função que exibe o menu de gerenciamento de recursos e equipamentos.
// Este menu apresenta ao usuário as opções (Adicionar, Alterar, Listar, Excluir)
// e, com base na escolha, chama a função do controller correspondente para executar a ação.
void menuRecursosView(Sistema *sistema);

// Declaração da função que exibe a lista formatada de todos os recursos cadastrados.
// Esta função percorre a lista de recursos no sistema e imprime os detalhes de cada
// um na tela de forma organizada e legível.
void listarRecursosView(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif