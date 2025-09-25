#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLIENTES_DATA_FILE "data/clientes.dat"
#define CLIENTES_TEXT_FILE "data/clientes.txt"

void salvarClientes(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = NULL;
    if (modo == ARQUIVO_BINARIO) {
        arquivo = fopen(CLIENTES_DATA_FILE, "wb");
        if (arquivo) {
            fwrite(&sistema->num_clientes, sizeof(int), 1, arquivo);
            fwrite(sistema->lista_clientes, sizeof(Cliente), sistema->num_clientes, arquivo);
        }
    } else if (modo == ARQUIVO_TEXTO) {
        arquivo = fopen(CLIENTES_TEXT_FILE, "w");
        if (arquivo) {
            fprintf(arquivo, "%d\n", sistema->num_clientes);
            for (int i = 0; i < sistema->num_clientes; i++) {
                fprintf(arquivo, "%d\n%s\n%s\n%s\n%s\n%s\n%s\n",
                        sistema->lista_clientes[i].codigo,
                        sistema->lista_clientes[i].razao_social,
                        sistema->lista_clientes[i].cnpj,
                        sistema->lista_clientes[i].endereco,
                        sistema->lista_clientes[i].telefone,
                        sistema->lista_clientes[i].email,
                        sistema->lista_clientes[i].nome_do_contato);
            }
        }
    }

    if (arquivo) fclose(arquivo);
}

void carregarClientes(Sistema *sistema) {
    // Implemente a lógica de carregamento baseada no seu código original aqui
    // Lembre-se de verificar o 'modo' (BINARIO ou TEXTO)
}

void liberarMemoriaClientes(Sistema *sistema) {
    if (sistema->lista_clientes != NULL) {
        free(sistema->lista_clientes);
        sistema->lista_clientes = NULL;
        sistema->num_clientes = 0;
        sistema->capacidade_clientes = 0;
    }
}