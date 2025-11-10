// Evita múltiplas inclusões deste cabeçalho
#ifndef OPERADOR_MODEL_H
#define OPERADOR_MODEL_H

// Necessário para acessar e manipular os dados do sistema
#include "model/sistema.h"

// Salva a lista de operadores conforme o modo de armazenamento configurado
void salvarOperadores(Sistema *sistema);

// Carrega os operadores do arquivo para a memória ao iniciar o sistema
void carregarOperadores(Sistema *sistema);

// Libera a memória usada pela lista de operadores
void liberarMemoriaOperadores(Sistema *sistema);

#endif
