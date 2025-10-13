#ifndef CLIENTE_VIEW_H
#define CLIENTE_VIEW_H

#include "model/sistema.h"  // Para acessar os dados do sistema

// Exibe o menu de clientes (Adicionar, Alterar, Listar, Excluir)
void menuClientesView(Sistema *sistema);

// Lista todos os clientes cadastrados de forma organizada
void listarClientesView(Sistema *sistema);

#endif
