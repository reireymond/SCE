// Previne a inclusão múltipla deste arquivo de cabeçalho. É uma guarda de compilação
// para evitar erros de redefinição de funções, que ocorreriam se este arquivo
// fosse incluído mais de uma vez em um mesmo arquivo de código.
#ifndef PRODUTORA_VIEW_H
#define PRODUTORA_VIEW_H

// Inclui a definição da estrutura principal 'Sistema'. As funções da view precisam
// de um ponteiro para 'Sistema' para poderem ler e exibir os dados da produtora
// que estão armazenados na estrutura.
#include "model/sistema.h"

// Declaração da função que exibe o menu de gerenciamento da produtora.
// Este menu apresenta ao usuário as opções disponíveis (Cadastrar, Alterar, Ver, Excluir)
// e, com base na escolha do usuário, chama a função do controller correspondente.
void menuProdutoraView(Sistema *sistema);

// Declaração da função que exibe os detalhes dos dados da produtora cadastrada.
// Esta função é chamada para mostrar ao usuário todas as informações da empresa
// de forma clara e formatada na tela.
void verDetalhesProdutoraView(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif