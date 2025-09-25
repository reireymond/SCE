#include "model/produtora/produtora_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRODUTORA_DATA_FILE "data/produtora.dat"
#define PRODUTORA_TEXT_FILE "data/produtora.txt"

void salvarProdutora(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    int produtora_existe = (sistema->dados_produtora != NULL);
    FILE *arquivo = NULL;

    if (modo == ARQUIVO_BINARIO) {
        arquivo = fopen(PRODUTORA_DATA_FILE, "wb");
        if (arquivo) {
            fwrite(&produtora_existe, sizeof(int), 1, arquivo);
            if (produtora_existe) {
                fwrite(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
            }
        }
    } else { // ARQUIVO_TEXTO
        arquivo = fopen(PRODUTORA_TEXT_FILE, "w");
        if (arquivo) {
            fprintf(arquivo, "%d\n", produtora_existe);
            if (produtora_existe) {
                fprintf(arquivo, "%s\n", sistema->dados_produtora->nome_fantasia);
                fprintf(arquivo, "%s\n", sistema->dados_produtora->razao_social);
                fprintf(arquivo, "%s\n", sistema->dados_produtora->nome_do_responsavel);
                fprintf(arquivo, "%s\n", sistema->dados_produtora->cnpj);
                fprintf(arquivo, "%s\n", sistema->dados_produtora->inscricao_estadual);
                fprintf(arquivo, "%s\n", sistema->dados_produtora->endereco);
                fprintf(arquivo, "%s\n", sistema->dados_produtora->telefone);
                fprintf(arquivo, "%s\n", sistema->dados_produtora->telefone_responsavel);
                fprintf(arquivo, "%s\n", sistema->dados_produtora->email);
                fprintf(arquivo, "%.2f\n", sistema->dados_produtora->margem_lucro);
            }
        }
    }

    if (arquivo) fclose(arquivo);
    else perror("Nao foi possivel abrir o arquivo da produtora");
}

void carregarProdutora(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    int produtora_existe = 0;
    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? PRODUTORA_DATA_FILE : PRODUTORA_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&produtora_existe, sizeof(int), 1, arquivo);
        if (produtora_existe) {
            sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
            if (sistema->dados_produtora) {
                fread(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
            }
        }
    } else { // ARQUIVO_TEXTO
        fscanf(arquivo, "%d\n", &produtora_existe);
        if (produtora_existe) {
            sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
            if (sistema->dados_produtora) {
                Produtora *p = sistema->dados_produtora;
                fgets(p->nome_fantasia, sizeof(p->nome_fantasia), arquivo); p->nome_fantasia[strcspn(p->nome_fantasia, "\n")] = 0;
                fgets(p->razao_social, sizeof(p->razao_social), arquivo); p->razao_social[strcspn(p->razao_social, "\n")] = 0;
                fgets(p->nome_do_responsavel, sizeof(p->nome_do_responsavel), arquivo); p->nome_do_responsavel[strcspn(p->nome_do_responsavel, "\n")] = 0;
                fgets(p->cnpj, sizeof(p->cnpj), arquivo); p->cnpj[strcspn(p->cnpj, "\n")] = 0;
                fgets(p->inscricao_estadual, sizeof(p->inscricao_estadual), arquivo); p->inscricao_estadual[strcspn(p->inscricao_estadual, "\n")] = 0;
                fgets(p->endereco, sizeof(p->endereco), arquivo); p->endereco[strcspn(p->endereco, "\n")] = 0;
                fgets(p->telefone, sizeof(p->telefone), arquivo); p->telefone[strcspn(p->telefone, "\n")] = 0;
                fgets(p->telefone_responsavel, sizeof(p->telefone_responsavel), arquivo); p->telefone_responsavel[strcspn(p->telefone_responsavel, "\n")] = 0;
                fgets(p->email, sizeof(p->email), arquivo); p->email[strcspn(p->email, "\n")] = 0;
                fscanf(arquivo, "%f\n", &p->margem_lucro);
            }
        }
    }
    fclose(arquivo);
}