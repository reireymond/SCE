// do model da produtora, com as declarações das funções implementadas aqui
#include "model/produtora/produtora_model.h"
// Bibliotecas padrão para manipulação de arquivos, memória e strings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Nomes dos arquivos usados para salvar e carregar os dados da produtora
#define PRODUTORA_DATA_FILE "data/produtora.dat"
#define PRODUTORA_TEXT_FILE "data/produtora.txt"

// Salva os dados da produtora conforme o modo de armazenamento configurado
void salvarProdutora(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    int produtora_existe = (sistema->dados_produtora != NULL);

    if (modo == ARQUIVO_BINARIO) {
        FILE *arquivo = fopen(PRODUTORA_DATA_FILE, "wb");
        if (!arquivo) {
            perror("Erro ao abrir arquivo binário da produtora");
            return;
        }
        fwrite(&produtora_existe, sizeof(int), 1, arquivo);
        if (produtora_existe) fwrite(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
        fclose(arquivo);
    } else {
        FILE *arquivo = fopen(PRODUTORA_TEXT_FILE, "w");
        if (!arquivo) {
            perror("Erro ao abrir arquivo de texto da produtora");
            return;
        }
        fprintf(arquivo, "%d\n", produtora_existe);
        if (produtora_existe) {
            Produtora *p = sistema->dados_produtora;
            fprintf(arquivo, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%.2f\n",
                    p->nome_fantasia, p->razao_social, p->nome_do_responsavel,
                    p->cnpj, p->inscricao_estadual, p->endereco, p->telefone,
                    p->telefone_responsavel, p->email, p->margem_lucro);
        }
        fclose(arquivo);
    }
}

// Carrega os dados da produtora do arquivo para a memória
void carregarProdutora(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? PRODUTORA_DATA_FILE : PRODUTORA_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";

    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    if (!arquivo) return;

    int produtora_existe = 0;
    if (modo == ARQUIVO_TEXTO)
        fscanf(arquivo, "%d\n", &produtora_existe);
    else
        fread(&produtora_existe, sizeof(int), 1, arquivo);

    if (produtora_existe) {
        sistema->dados_produtora = malloc(sizeof(Produtora));
        if (!sistema->dados_produtora) {
            printf("Erro de alocacao de memória.\n");
            fclose(arquivo);
            return;
        }

        if (modo == ARQUIVO_BINARIO) {
            fread(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
        } else {
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
    fclose(arquivo);
}

// Limpa a memoria alocada para os dados da produtora
void liberarMemoriaProdutora(Sistema *sistema) {
    if (sistema->dados_produtora) {
        free(sistema->dados_produtora);
        sistema->dados_produtora = NULL;
    }
}
