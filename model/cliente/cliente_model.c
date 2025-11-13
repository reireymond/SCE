// Inclui o cabeçalho do model de cliente (declarações das funções)
#include "model/cliente/cliente_model.h"
// Inclui bibliotecas padrão para arquivos, memória e strings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Caminhos dos arquivos de dados
#define CLIENTES_DATA_FILE "data/clientes.dat"
#define CLIENTES_TEXT_FILE "data/clientes.txt"

// Salva os dados dos clientes (binário ou texto)
void salvarClientes(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return; // Se só estiver em memória, sai

    FILE *arquivo = NULL;

    if (modo == ARQUIVO_BINARIO) {
        arquivo = fopen(CLIENTES_DATA_FILE, "wb"); // Abre para escrita binária
        if (arquivo) {
            fwrite(&sistema->num_clientes, sizeof(int), 1, arquivo); // Escreve quantidade
            fwrite(sistema->lista_clientes, sizeof(Cliente), sistema->num_clientes, arquivo); // Escreve todos os clientes
        }
    } else if (modo == ARQUIVO_TEXTO) {
        arquivo = fopen(CLIENTES_TEXT_FILE, "w"); // Abre para escrita texto
        if (arquivo) {
            fprintf(arquivo, "%d\n", sistema->num_clientes); // Primeira linha: total de clientes
            for (int i = 0; i < sistema->num_clientes; i++) {
                // Escreve os dados formatados
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
    else perror("Erro ao abrir arquivo para salvar clientes");
}

// Carrega os dados dos clientes do arquivo (binário ou texto)
void carregarClientes(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = NULL;
    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? CLIENTES_DATA_FILE : CLIENTES_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";

    arquivo = fopen(nomeArquivo, modoAbertura);
    if (arquivo == NULL) return; // Se ainda não existir, não faz nada

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_clientes, sizeof(int), 1, arquivo); // Lê total de clientes
        if (sistema->num_clientes > 0) {
            sistema->lista_clientes = malloc(sistema->num_clientes * sizeof(Cliente));
            if (sistema->lista_clientes == NULL) {
                printf("Erro ao alocar memoria para carregar clientes.\n");
                sistema->num_clientes = 0;
                fclose(arquivo);
                return;
            }
            fread(sistema->lista_clientes, sizeof(Cliente), sistema->num_clientes, arquivo); // Lê todos os clientes
            sistema->capacidade_clientes = sistema->num_clientes;
        }
    } else {
        fscanf(arquivo, "%d\n", &sistema->num_clientes); // Lê quantidade de clientes
        if (sistema->num_clientes > 0) {
            sistema->lista_clientes = malloc(sistema->num_clientes * sizeof(Cliente));
            sistema->capacidade_clientes = sistema->num_clientes;
            for (int i = 0; i < sistema->num_clientes; i++) {
                Cliente *c = &sistema->lista_clientes[i];
                fscanf(arquivo, "%d\n", &c->codigo);
                // Lê campos de texto e remove '\n'
                fgets(c->razao_social, sizeof(c->razao_social), arquivo); c->razao_social[strcspn(c->razao_social, "\n")] = 0;
                fgets(c->cnpj, sizeof(c->cnpj), arquivo); c->cnpj[strcspn(c->cnpj, "\n")] = 0;
                fgets(c->endereco, sizeof(c->endereco), arquivo); c->endereco[strcspn(c->endereco, "\n")] = 0;
                fgets(c->telefone, sizeof(c->telefone), arquivo); c->telefone[strcspn(c->telefone, "\n")] = 0;
                fgets(c->email, sizeof(c->email), arquivo); c->email[strcspn(c->email, "\n")] = 0;
                fgets(c->nome_do_contato, sizeof(c->nome_do_contato), arquivo); c->nome_do_contato[strcspn(c->nome_do_contato, "\n")] = 0;
            }
        }
    }

    fclose(arquivo);
}

// Libera a memória alocada para a lista de clientes
void liberarMemoriaClientes(Sistema *sistema) {
    if (sistema->lista_clientes != NULL) {
        free(sistema->lista_clientes);
        sistema->lista_clientes = NULL;
        sistema->num_clientes = 0;
        sistema->capacidade_clientes = 0;
    }
}
