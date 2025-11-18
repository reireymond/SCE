#include "config_armazenamento.h"
#include <stdio.h>

void definirModoDeArmazenamento(Sistema *sistema, TipoArmazenamento modo) {
    if (sistema != NULL) {
        sistema->modo_de_armazenamento = modo;
    }
}

TipoArmazenamento obterModoDeArmazenamento(Sistema *sistema) {
    if (sistema != NULL) {
        return sistema->modo_de_armazenamento;
    }
    return ARQUIVO_BINARIO;
}