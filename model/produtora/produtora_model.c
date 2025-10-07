// Inclui o cabeçalho do model da produtora, que contém as declarações das funções
// implementadas neste arquivo.
#include "model/produtora/produtora_model.h"
// Inclui os cabeçalhos padrão do C para manipulação de arquivos (stdio) e
// alocação dinâmica de memória (stdlib).
#include <stdio.h>
#include <stdlib.h>

// Define os nomes dos arquivos para persistência de dados da produtora.
// Usar macros torna o código mais claro e facilita a alteração dos nomes dos arquivos no futuro.
#define PRODUTORA_DATA_FILE "data/produtora.dat" // Arquivo para armazenamento em formato binário.
#define PRODUTORA_TEXT_FILE "data/produtora.txt" // Arquivo para armazenamento em formato de texto.

// Implementação da função para salvar os dados da produtora.
void salvarProdutora(Sistema *sistema) {
    // Obtém o modo de armazenamento configurado (MEMORIA, ARQUIVO_TEXTO, ARQUIVO_BINARIO).
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for apenas em memória, não há nada a salvar em disco, então a função retorna.
    if (modo == MEMORIA) return;

    // Abre o arquivo binário para escrita.
    FILE *arquivo = fopen(PRODUTORA_DATA_FILE, "wb");
    // Se o arquivo não puder ser aberto, exibe um erro e retorna.
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo da produtora");
        return;
    }

    // Verifica se existem dados da produtora a serem salvos (o ponteiro não é nulo).
    // Esta variável 'produtora_existe' funcionará como um indicador dentro do arquivo.
    int produtora_existe = (sistema->dados_produtora != NULL);
    // Grava o indicador no arquivo. 1 se os dados existem, 0 se não.
    fwrite(&produtora_existe, sizeof(int), 1, arquivo);
    // Se a produtora existe, grava a estrutura completa da produtora no arquivo.
    if (produtora_existe) {
        fwrite(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
    }
    
    // A lógica para salvar em arquivo de texto.
    if (modo == ARQUIVO_TEXTO) {
        // Abre o arquivo de texto para escrita.
        arquivo = fopen(PRODUTORA_TEXT_FILE, "w");
        if (!arquivo) {
            perror("Nao foi possivel abrir o arquivo de texto da produtora");
            return;
        }
        // Grava o mesmo indicador de existência na primeira linha.
        fprintf(arquivo, "%d\n", produtora_existe);
        // Se a produtora existe, grava cada campo em uma nova linha.
        if (produtora_existe) {
            Produtora *p = sistema->dados_produtora;
            fprintf(arquivo, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%.2f\n",
                    p->nome_fantasia, p->razao_social, p->nome_do_responsavel,
                    p->cnpj, p->inscricao_estadual, p->endereco, p->telefone,
                    p->telefone_responsavel, p->email, p->margem_lucro);
        }
    }
    // Fecha o arquivo que foi aberto para escrita.
    fclose(arquivo);
}

// Implementação da função para carregar os dados da produtora.
void carregarProdutora(Sistema *sistema) {
    // Obtém o modo de armazenamento. Se for memória, não faz nada.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    // Abre o arquivo binário para leitura.
    FILE *arquivo = fopen(PRODUTORA_DATA_FILE, "rb");
    // Se o arquivo foi aberto com sucesso...
    if (arquivo) {
        int produtora_existe = 0;
        // Lê o indicador para saber se há dados de produtora no arquivo.
        fread(&produtora_existe, sizeof(int), 1, arquivo);
        // Se o indicador for verdadeiro (diferente de 0)...
        if (produtora_existe) {
            // Aloca memória para a estrutura da produtora.
            sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
            // Se a alocação de memória funcionar...
            if (sistema->dados_produtora) {
                // Lê os dados da produtora do arquivo para a estrutura recém-alocada.
                fread(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
            }
        }
        // Fecha o arquivo.
        fclose(arquivo);
    }
}

// Implementação da função para liberar a memória alocada para os dados da produtora.
void liberarMemoriaProdutora(Sistema *sistema) {
    // Verifica se o ponteiro 'dados_produtora' não é nulo.
    if (sistema->dados_produtora) {
        // Libera a memória.
        free(sistema->dados_produtora);
        // Define o ponteiro como nulo para evitar seu uso acidental posteriormente.
        sistema->dados_produtora = NULL;
    }
}