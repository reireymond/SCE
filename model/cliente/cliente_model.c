#include "cliente_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

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
    } else {
        arquivo = fopen(CLIENTES_TEXT_FILE, "w");
        if (arquivo) {
            fprintf(arquivo, " Numero de clientes cadastrados:%d\n", sistema->num_clientes);
            for (int i = 0; i < sistema->num_clientes; i++) {
                fprintf(arquivo, "Codigo: %d\nNome / Razao Social: %s\nCPF/CNPJ: %s\nEndereco completo: %s\nTelefone: %s\nE-mail: %s\nNome do contato: %s\n",
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
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? CLIENTES_DATA_FILE : CLIENTES_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "rb" : "r");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_clientes, sizeof(int), 1, arquivo);
        if (sistema->num_clientes > 0) {
            sistema->lista_clientes = malloc(sistema->num_clientes * sizeof(Cliente));
            sistema->capacidade_clientes = sistema->num_clientes;
            fread(sistema->lista_clientes, sizeof(Cliente), sistema->num_clientes, arquivo);
        }
    } else {
        fscanf(arquivo, "%d\n", &sistema->num_clientes);
        if (sistema->num_clientes > 0) {
            sistema->lista_clientes = malloc(sistema->num_clientes * sizeof(Cliente));
            sistema->capacidade_clientes = sistema->num_clientes;
            for (int i = 0; i < sistema->num_clientes; i++) {
                Cliente *c = &sistema->lista_clientes[i];
                fscanf(arquivo, "%d\n", &c->codigo);
                fgets(c->razao_social, 100, arquivo); c->razao_social[strcspn(c->razao_social, "\n")] = 0;
                fgets(c->cnpj, 20, arquivo); c->cnpj[strcspn(c->cnpj, "\n")] = 0;
                fgets(c->endereco, 150, arquivo); c->endereco[strcspn(c->endereco, "\n")] = 0;
                fgets(c->telefone, 20, arquivo); c->telefone[strcspn(c->telefone, "\n")] = 0;
                fgets(c->email, 50, arquivo); c->email[strcspn(c->email, "\n")] = 0;
                fgets(c->nome_do_contato, 100, arquivo); c->nome_do_contato[strcspn(c->nome_do_contato, "\n")] = 0;
            }
        }
    }
    fclose(arquivo);
}

void liberarMemoriaClientes(Sistema *sistema) {
    if (sistema->lista_clientes) {
        free(sistema->lista_clientes);
        sistema->lista_clientes = NULL;
        sistema->num_clientes = 0;
        sistema->capacidade_clientes = 0;
    }
}