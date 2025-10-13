#ifndef FORNECEDOR_VIEW_H
#define FORNECEDOR_VIEW_H

#include "model/sistema.h"  // Para acessar os dados do sistema

// Exibe o menu de gerenciamento de fornecedores (Adicionar, Alterar, Listar, Excluir)
void menuFornecedoresView(Sistema *sistema);

// Lista todos os fornecedores cadastrados de forma organizada
void listarFornecedoresView(Sistema *sistema);

#endif
