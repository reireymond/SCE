#ifndef LOGIN_CONTROLLER_H
#define LOGIN_CONTROLLER_H

#include "model/sistema.h" // Para acessar a estrutura Sistema

// Retorna 1 se o login for bem-sucedido, 0 caso contrário (ou se o usuário sair)
int realizarLoginOperador(Sistema *sistema);

#endif 