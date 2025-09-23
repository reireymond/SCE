#ifndef CONFIG_ARMAZENAMENTO_H
#define CONFIG_ARMAZENAMENTO_H

// Enumeração dos tipos de armazenamento de dados.
typedef enum
{
    MEMORIA,
    ARQUIVO_TEXTO,
    ARQUIVO_BINARIO
} TipoArmazenamento;

// Note que as funções agora recebem o ponteiro do Sistema
// para alterar o valor diretamente na struct central.

struct Sistema;

void definirModoDeArmazenamento(struct Sistema *sistema, TipoArmazenamento modo);
TipoArmazenamento obterModoDeArmazenamento(struct Sistema *sistema);

#endif