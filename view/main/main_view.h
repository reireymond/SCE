// Previne a inclusão múltipla deste arquivo de cabeçalho, uma prática padrão
// para evitar erros de compilação por definições duplicadas de funções.
#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

// Inclui a definição da estrutura principal 'Sistema', pois as funções
// desta view precisam de um ponteiro para a estrutura para poderem
// passar a referência dela para outras funções ou ler dados de configuração.
#include "model/sistema.h"

// Funções para exibir os menus principais e de navegação do sistema.

// Declaração da função que exibe o menu inicial para o usuário escolher o modo de armazenamento.
// Esta é uma das primeiras interações que o usuário tem com o programa.
void menuEscolherArmazenamentoView(Sistema *sistema);

// Declaração da função que exibe o menu principal da aplicação.
// A partir daqui, o usuário pode navegar para os principais módulos do sistema.
void menuPrincipalView(Sistema *sistema);

// Declaração da função que exibe o menu de "Gestão de Dados".
// Este menu agrupa todos os submódulos de cadastro (clientes, fornecedores, etc.).
void menuGestaoDeDadosView(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif // MAIN_VIEW_H