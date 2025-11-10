// Evita inclusão múltipla do mesmo cabeçalho.
#ifndef EQUIPE_INTERNA_MODEL_H
#define EQUIPE_INTERNA_MODEL_H

// Necessário para acessar a estrutura principal 'Sistema',
// onde ficam armazenadas todas as listas e dados.
#include "model/sistema.h"

// Salva os dados da equipe interna no modo de armazenamento configurado (texto ou binário).
void salvarEquipeInterna(Sistema *sistema);

// Carrega os dados da equipe interna do arquivo para a memória.
void carregarEquipeInterna(Sistema *sistema);

// Libera a memória alocada para a lista da equipe interna.
void liberarMemoriaEquipe(Sistema *sistema);

#endif
