// Inclui o cabeçalho correspondente, que contém as declarações das funções
// que serão implementadas neste arquivo.
#include "model/equipe_interna/equipe_interna_model.h"
// Inclui os cabeçalhos padrão do C para entrada/saída (stdio),
// alocação de memória (stdlib) e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// Define constantes para os nomes dos arquivos de persistência de dados.
// Usar #define torna o código mais legível e fácil de manter.
#define EQUIPE_DATA_FILE "data/equipe.dat" // Arquivo para armazenamento binário.
#define EQUIPE_TEXT_FILE "data/equipe.txt" // Arquivo para armazenamento em texto.

// Implementação da função para salvar os dados da equipe.
void salvarEquipeInterna(Sistema *sistema) {
    // Obtém o modo de armazenamento atual do sistema.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for 'MEMORIA', não há necessidade de salvar em disco, então a função retorna.
    if (modo == MEMORIA) return;

    // Inicializa o ponteiro de arquivo como nulo.
    FILE *arquivo = NULL;
    // Verifica qual o modo de persistência configurado.
    if (modo == ARQUIVO_BINARIO) {
        // Abre o arquivo binário em modo de escrita ("wb").
        arquivo = fopen(EQUIPE_DATA_FILE, "wb");
        // Se o arquivo foi aberto com sucesso...
        if (arquivo) {
            // Grava primeiro a quantidade de membros da equipe.
            fwrite(&sistema->num_equipe, sizeof(int), 1, arquivo);
            // Em seguida, grava todo o array da lista de equipe de uma só vez.
            fwrite(sistema->lista_equipe, sizeof(EquipeInterna), sistema->num_equipe, arquivo);
        }
    } else { // Se o modo for ARQUIVO_TEXTO...
        // Abre o arquivo de texto em modo de escrita ("w").
        arquivo = fopen(EQUIPE_TEXT_FILE, "w");
        // Se o arquivo foi aberto com sucesso...
        if (arquivo) {
            // Grava a quantidade de membros na primeira linha.
            fprintf(arquivo, "%d\n", sistema->num_equipe);
            // Percorre a lista de membros da equipe.
            for (int i = 0; i < sistema->num_equipe; i++) {
                // Grava os dados de cada membro em linhas separadas.
                fprintf(arquivo, "%d\n%s\n%s\n%s\n%.2f\n",
                        sistema->lista_equipe[i].codigo,
                        sistema->lista_equipe[i].nome,
                        sistema->lista_equipe[i].cpf,
                        sistema->lista_equipe[i].funcao,
                        sistema->lista_equipe[i].valor_diaria);
            }
        }
    }

    // Se o arquivo foi aberto, fecha para liberar os recursos do sistema.
    if (arquivo) fclose(arquivo);
    // Caso contrário, exibe uma mensagem de erro.
    else perror("Nao foi possivel abrir o arquivo da equipe");
}

// Implementação da função para carregar os dados da equipe.
void carregarEquipeInterna(Sistema *sistema) {
    // Obtém o modo de armazenamento atual.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se for 'MEMORIA', não faz nada.
    if (modo == MEMORIA) return;

    // Define o nome do arquivo e o modo de abertura (leitura binária "rb" ou texto "r")
    // com base na configuração do sistema.
    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? EQUIPE_DATA_FILE : EQUIPE_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    // Tenta abrir o arquivo.
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    // Se o arquivo não existir (comum na primeira execução), a função retorna sem erro.
    if (!arquivo) return;

    // Se o modo for ARQUIVO_BINARIO...
    if (modo == ARQUIVO_BINARIO) {
        // Lê a quantidade de registros do arquivo.
        fread(&sistema->num_equipe, sizeof(int), 1, arquivo);
        // Se houver registros para ler...
        if (sistema->num_equipe > 0) {
            // Aloca a memória necessária para a lista.
            sistema->lista_equipe = malloc(sistema->num_equipe * sizeof(EquipeInterna));
            // Se a alocação de memória funcionar...
            if (sistema->lista_equipe) {
                // Lê todos os registros do arquivo para a memória de uma só vez.
                fread(sistema->lista_equipe, sizeof(EquipeInterna), sistema->num_equipe, arquivo);
                // Define a capacidade da lista como o número de itens carregados.
                sistema->capacidade_equipe = sistema->num_equipe;
            }
        }
    } else { // Se o modo for ARQUIVO_TEXTO...
        // Lê a quantidade de registros da primeira linha.
        fscanf(arquivo, "%d\n", &sistema->num_equipe);
        // Se houver registros para ler...
        if (sistema->num_equipe > 0) {
            // Aloca a memória e define a capacidade.
            sistema->lista_equipe = malloc(sistema->num_equipe * sizeof(EquipeInterna));
            sistema->capacidade_equipe = sistema->num_equipe;
            // Percorre a quantidade de registros que precisam ser lidos.
            for (int i = 0; i < sistema->num_equipe; i++) {
                // Cria um ponteiro para o membro atual para facilitar o acesso.
                EquipeInterna *e = &sistema->lista_equipe[i];
                // Lê o código (int).
                fscanf(arquivo, "%d\n", &e->codigo);
                // Lê as strings, uma por linha, e remove o caractere '\n' do final.
                fgets(e->nome, sizeof(e->nome), arquivo); e->nome[strcspn(e->nome, "\n")] = 0;
                fgets(e->cpf, sizeof(e->cpf), arquivo); e->cpf[strcspn(e->cpf, "\n")] = 0;
                fgets(e->funcao, sizeof(e->funcao), arquivo); e->funcao[strcspn(e->funcao, "\n")] = 0;
                // Lê o valor da diária (float).
                fscanf(arquivo, "%f\n", &e->valor_diaria);
            }
        }
    }
    // Fecha o arquivo.
    fclose(arquivo);
}

// Implementação da função para liberar a memória da lista de equipe.
void liberarMemoriaEquipe(Sistema *sistema) {
    // Verifica se o ponteiro da lista não é nulo para evitar erros.
    if (sistema->lista_equipe != NULL) {
        // Libera a memória alocada com malloc/realloc.
        free(sistema->lista_equipe);
        // Aponta o ponteiro para NULL para evitar o uso de memória já liberada.
        sistema->lista_equipe = NULL;
        // Zera os contadores.
        sistema->num_equipe = 0;
        sistema->capacidade_equipe = 0;
    }
}