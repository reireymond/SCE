#include "cliente_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define CLIENTES_DATA_FILE "data/clientes.dat"
#define CLIENTES_TEXT_FILE "data/clientes.txt"

void remover_quebra_linha_cliente(char *str) {
    str[strcspn(str, "\n")] = 0;
}

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
        char linha[256];
        char *valor;

        if(fgets(linha, sizeof(linha), arquivo)) {
            valor = strchr(linha, ':');
            if(valor) sistema->num_clientes = atoi(valor + 1);
        }

        if (sistema->num_clientes > 0) {
            sistema->lista_clientes = malloc(sistema->num_clientes * sizeof(Cliente));
            sistema->capacidade_clientes = sistema->num_clientes;
            
            for (int i = 0; i < sistema->num_clientes; i++) {
                Cliente *c = &sistema->lista_clientes[i];
                
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) c->codigo = atoi(valor + 1);
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(c->razao_social, valor + 2); remover_quebra_linha_cliente(c->razao_social); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(c->cnpj, valor + 2); remover_quebra_linha_cliente(c->cnpj); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(c->endereco, valor + 2); remover_quebra_linha_cliente(c->endereco); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(c->telefone, valor + 2); remover_quebra_linha_cliente(c->telefone); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(c->email, valor + 2); remover_quebra_linha_cliente(c->email); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(c->nome_do_contato, valor + 2); remover_quebra_linha_cliente(c->nome_do_contato); }
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