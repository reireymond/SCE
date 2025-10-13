// Inclui o cabeçalho do model da produtora, que contém as declarações das funções
// implementadas neste arquivo.
#include "model/produtora/produtora_model.h"
// Inclui os cabeçalhos padrão do C para manipulação de arquivos (stdio),
// alocação de memória (stdlib) e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define os nomes dos arquivos para persistência de dados da produtora.
// Centralizar esses nomes aqui facilita a manutenção do código no futuro.
#define PRODUTORA_DATA_FILE "data/produtora.dat" // Arquivo para armazenamento em formato binário.
#define PRODUTORA_TEXT_FILE "data/produtora.txt" // Arquivo para armazenamento em formato de texto.

// Implementação da função para salvar os dados da produtora.
void salvarProdutora(Sistema *sistema) {
    // Obtém o modo de armazenamento configurado (MEMORIA, ARQUIVO_TEXTO, ou ARQUIVO_BINARIO).
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for apenas em memória, não há necessidade de salvar em disco, então a função retorna.
    if (modo == MEMORIA) return;

    // A variável 'produtora_existe' serve como um indicador (flag) no arquivo.
    // Ela valerá 1 se os dados da produtora existirem (ponteiro não nulo), e 0 caso contrário.
    int produtora_existe = (sistema->dados_produtora != NULL);

    // Se o modo de armazenamento for ARQUIVO_BINARIO...
    if (modo == ARQUIVO_BINARIO) {
        // Abre o arquivo binário em modo de escrita ("wb").
        FILE *arquivo = fopen(PRODUTORA_DATA_FILE, "wb");
        // Se o arquivo não puder ser aberto, exibe um erro e retorna para evitar falhas.
        if (!arquivo) {
            perror("Nao foi possivel abrir o arquivo binario da produtora");
            return;
        }
        // Grava o indicador de existência (1 ou 0) no início do arquivo.
        fwrite(&produtora_existe, sizeof(int), 1, arquivo);
        // Se a produtora existe, grava a estrutura completa da produtora no arquivo.
        if (produtora_existe) {
            fwrite(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
        }
        // Fecha o arquivo para garantir que todos os dados sejam escritos e o recurso liberado.
        fclose(arquivo);
    // Se o modo for ARQUIVO_TEXTO...
    } else {
        // Abre o arquivo de texto em modo de escrita ("w").
        FILE *arquivo = fopen(PRODUTORA_TEXT_FILE, "w");
        if (!arquivo) {
            perror("Nao foi possivel abrir o arquivo de texto da produtora");
            return;
        }
        // Grava o indicador de existência (1 ou 0) na primeira linha do arquivo.
        fprintf(arquivo, "%d\n", produtora_existe);
        // Se a produtora existe, grava cada campo da estrutura em uma nova linha.
        if (produtora_existe) {
            Produtora *p = sistema->dados_produtora;
            fprintf(arquivo, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%.2f\n",
                    p->nome_fantasia, p->razao_social, p->nome_do_responsavel,
                    p->cnpj, p->inscricao_estadual, p->endereco, p->telefone,
                    p->telefone_responsavel, p->email, p->margem_lucro);
        }
        // Fecha o arquivo de texto.
        fclose(arquivo);
    }
}

// Implementação da função para carregar os dados da produtora do disco para a memória.
void carregarProdutora(Sistema *sistema) {
    // Obtém o modo de armazenamento. Se for memória, não há o que carregar.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    // Define o nome do arquivo e o modo de abertura (leitura) com base na configuração do sistema.
    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? PRODUTORA_DATA_FILE : PRODUTORA_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    
    // Tenta abrir o arquivo. Se ele não existir (o que é normal na primeira execução), a função retorna.
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    if (!arquivo) return;

    int produtora_existe = 0;
    // Lê o indicador de existência do arquivo.
    if (modo == ARQUIVO_TEXTO) {
        fscanf(arquivo, "%d\n", &produtora_existe);
    } else { // ARQUIVO_BINARIO
        fread(&produtora_existe, sizeof(int), 1, arquivo);
    }

    // Se o indicador mostrar que existem dados de produtora salvos...
    if (produtora_existe) {
        // Aloca memória para a estrutura da produtora.
        sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
        // Se a alocação de memória falhar, exibe um erro e encerra a função.
        if (!sistema->dados_produtora) {
            printf("Erro de alocacao de memoria para produtora.\n");
            fclose(arquivo);
            return;
        }

        // Carrega os dados de acordo com o tipo de arquivo.
        if (modo == ARQUIVO_BINARIO) {
            // Lê o bloco de dados do arquivo binário diretamente para a estrutura alocada.
            fread(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
        } else { // ARQUIVO_TEXTO
            // Ponteiro para simplificar o acesso aos campos da estrutura.
            Produtora *p = sistema->dados_produtora;
            // Lê cada campo do arquivo de texto, linha por linha.
            // A função strcspn é usada para remover o caractere de nova linha '\n' que o fgets captura.
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
    // Fecha o arquivo após a leitura.
    fclose(arquivo);
}

// Implementação da função para liberar a memória alocada para a produtora.
void liberarMemoriaProdutora(Sistema *sistema) {
    // Verifica se o ponteiro não é nulo antes de tentar liberar a memória.
    if (sistema->dados_produtora) {
        // Libera a memória alocada com malloc.
        free(sistema->dados_produtora);
        // Define o ponteiro como nulo para evitar o uso de um ponteiro inválido ("dangling pointer").
        sistema->dados_produtora = NULL;
    }
}