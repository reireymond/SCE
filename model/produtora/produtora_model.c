#include "produtora_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define PRODUTORA_DATA_FILE "data/produtora.dat"
#define PRODUTORA_TEXT_FILE "data/produtora.txt"

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
        fprintf(arquivo, "%d\n", existe);
        if (existe) {
            Produtora *p = sistema->dados_produtora;
            fprintf(arquivo, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%.2f\n",
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
    if (modo == ARQUIVO_BINARIO) fread(&existe, sizeof(int), 1, arquivo);
    else fscanf(arquivo, "%d\n", &existe);

    if (existe) {
        sistema->dados_produtora = malloc(sizeof(Produtora));
        if (modo == ARQUIVO_BINARIO) {
            fread(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
        } else {
            Produtora *p = sistema->dados_produtora;
            fgets(p->nome_fantasia, 100, arquivo); p->nome_fantasia[strcspn(p->nome_fantasia, "\n")] = 0;
            fgets(p->razao_social, 100, arquivo); p->razao_social[strcspn(p->razao_social, "\n")] = 0;
            fgets(p->nome_do_responsavel, 100, arquivo); p->nome_do_responsavel[strcspn(p->nome_do_responsavel, "\n")] = 0;
            fgets(p->cnpj, 20, arquivo); p->cnpj[strcspn(p->cnpj, "\n")] = 0;
            fgets(p->inscricao_estadual, 20, arquivo); p->inscricao_estadual[strcspn(p->inscricao_estadual, "\n")] = 0;
            fgets(p->endereco, 150, arquivo); p->endereco[strcspn(p->endereco, "\n")] = 0;
            fgets(p->telefone, 20, arquivo); p->telefone[strcspn(p->telefone, "\n")] = 0;
            fgets(p->telefone_responsavel, 20, arquivo); p->telefone_responsavel[strcspn(p->telefone_responsavel, "\n")] = 0;
            fgets(p->email, 50, arquivo); p->email[strcspn(p->email, "\n")] = 0;
            fscanf(arquivo, "%f\n", &p->margem_lucro);
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