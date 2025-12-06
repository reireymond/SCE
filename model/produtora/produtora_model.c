#include "produtora_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define PRODUTORA_DATA_FILE "data/produtora.dat"
#define PRODUTORA_TEXT_FILE "data/produtora.txt"

void remover_quebra_linha_produtora(char *str) {
    str[strcspn(str, "\n")] = 0;
}

void salvarProdutora(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? PRODUTORA_DATA_FILE : PRODUTORA_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "wb" : "w");
    if (!arquivo) return;

    int existe = (sistema->dados_produtora != NULL);
    
    if (modo == ARQUIVO_BINARIO) {
        fwrite(&existe, sizeof(int), 1, arquivo);
        if (existe) fwrite(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
    } else {
        fprintf(arquivo, "Numero de produtora cadastrada: %d\n", existe);
        if (existe) {
            Produtora *p = sistema->dados_produtora;
            fprintf(arquivo, "Nome fantasia: %s\nRazao social: %s\nNome do responsavel: %s\nCNPJ: %s\nInscricao estadual:%s\nEndereco: %s\nTelefone: %s\nTelefone do responsavel:  %s\nE-mail: %s\nMargem de lucro: %.2f%%\n",
                    p->nome_fantasia, p->razao_social, p->nome_do_responsavel,
                    p->cnpj, p->inscricao_estadual, p->endereco, p->telefone,
                    p->telefone_responsavel, p->email, p->margem_lucro);
        }
    }
    fclose(arquivo);
}

void carregarProdutora(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? PRODUTORA_DATA_FILE : PRODUTORA_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "rb" : "r");
    if (!arquivo) return;

    int existe = 0;
    if (modo == ARQUIVO_BINARIO) {
        fread(&existe, sizeof(int), 1, arquivo);
        if (existe) {
            sistema->dados_produtora = malloc(sizeof(Produtora));
            fread(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
        }
    } else {
        char linha[256];
        char *valor;

        if(fgets(linha, sizeof(linha), arquivo)) {
            valor = strchr(linha, ':');
            if(valor) existe = atoi(valor + 1);
        }

        if (existe) {
            sistema->dados_produtora = malloc(sizeof(Produtora));
            Produtora *p = sistema->dados_produtora;

            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) { strcpy(p->nome_fantasia, valor + 2); remover_quebra_linha_produtora(p->nome_fantasia); }
            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) { strcpy(p->razao_social, valor + 2); remover_quebra_linha_produtora(p->razao_social); }
            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) { strcpy(p->nome_do_responsavel, valor + 2); remover_quebra_linha_produtora(p->nome_do_responsavel); }
            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) { strcpy(p->cnpj, valor + 2); remover_quebra_linha_produtora(p->cnpj); }
            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) { strcpy(p->inscricao_estadual, valor + 2); remover_quebra_linha_produtora(p->inscricao_estadual); }
            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) { strcpy(p->endereco, valor + 2); remover_quebra_linha_produtora(p->endereco); }
            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) { strcpy(p->telefone, valor + 2); remover_quebra_linha_produtora(p->telefone); }
            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) { strcpy(p->telefone_responsavel, valor + 2); remover_quebra_linha_produtora(p->telefone_responsavel); }
            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) { strcpy(p->email, valor + 2); remover_quebra_linha_produtora(p->email); }
            fgets(linha, sizeof(linha), arquivo);
            valor = strchr(linha, ':'); if(valor) p->margem_lucro = atof(valor + 1);
        }
    }
    fclose(arquivo);
}

void liberarMemoriaProdutora(Sistema *sistema) {
    if (sistema->dados_produtora) {
        free(sistema->dados_produtora);
        sistema->dados_produtora = NULL;
    }
}