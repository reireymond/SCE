#include "fornecedor_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define FORNECEDORES_DATA_FILE "data/fornecedores.dat"
#define FORNECEDORES_TEXT_FILE "data/fornecedores.txt"

void remover_quebra_linha_fornecedor(char *str) {
    str[strcspn(str, "\n")] = 0;
}

void salvarFornecedores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? FORNECEDORES_DATA_FILE : FORNECEDORES_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "wb" : "w");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fwrite(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
        fwrite(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
    } else {
        fprintf(arquivo, "Numero de fornecedores cadastrados: %d\n", sistema->num_fornecedores);
        for (int i = 0; i < sistema->num_fornecedores; i++) {
            fprintf(arquivo, "Codigo: %d\nNome fantasia: %s\n Razao social: %s\n CNPJ: %s\nEndereco: %s\n Telefone: %s\nTipo de servico: %s\n Valor do servico: %.2f\n",
                    sistema->lista_fornecedores[i].codigo,
                    sistema->lista_fornecedores[i].nome_fantasia,
                    sistema->lista_fornecedores[i].razao_social,
                    sistema->lista_fornecedores[i].cnpj,
                    sistema->lista_fornecedores[i].endereco,
                    sistema->lista_fornecedores[i].telefone,
                    sistema->lista_fornecedores[i].tipo_servico,
                    sistema->lista_fornecedores[i].valor_servico);
        }
    }
    fclose(arquivo);
}

void carregarFornecedores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? FORNECEDORES_DATA_FILE : FORNECEDORES_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "rb" : "r");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
        if (sistema->num_fornecedores > 0) {
            sistema->lista_fornecedores = malloc(sistema->num_fornecedores * sizeof(Fornecedor));
            sistema->capacidade_fornecedores = sistema->num_fornecedores;
            fread(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
        }
    } else {
        char linha[256];
        char *valor;

        if(fgets(linha, sizeof(linha), arquivo)) {
            valor = strchr(linha, ':');
            if(valor) sistema->num_fornecedores = atoi(valor + 1);
        }

        if (sistema->num_fornecedores > 0) {
            sistema->lista_fornecedores = malloc(sistema->num_fornecedores * sizeof(Fornecedor));
            sistema->capacidade_fornecedores = sistema->num_fornecedores;

            for (int i = 0; i < sistema->num_fornecedores; i++) {
                Fornecedor *f = &sistema->lista_fornecedores[i];
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) f->codigo = atoi(valor + 1);
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(f->nome_fantasia, valor + 2); remover_quebra_linha_fornecedor(f->nome_fantasia); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(f->razao_social, valor + 2); remover_quebra_linha_fornecedor(f->razao_social); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(f->cnpj, valor + 2); remover_quebra_linha_fornecedor(f->cnpj); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(f->endereco, valor + 2); remover_quebra_linha_fornecedor(f->endereco); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(f->telefone, valor + 2); remover_quebra_linha_fornecedor(f->telefone); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(f->tipo_servico, valor + 2); remover_quebra_linha_fornecedor(f->tipo_servico); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) f->valor_servico = atof(valor + 1);
            }
        }
    }
    fclose(arquivo);
}

void liberarMemoriaFornecedores(Sistema *sistema) {
    if (sistema->lista_fornecedores) {
        free(sistema->lista_fornecedores);
        sistema->lista_fornecedores = NULL;
        sistema->num_fornecedores = 0;
        sistema->capacidade_fornecedores = 0;
    }
}