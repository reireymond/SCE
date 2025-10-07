// Inclui o cabeçalho do model de recurso, que contém as declarações das funções
// implementadas neste arquivo.
#include "model/recurso/recurso_model.h"
// Inclui cabeçalhos padrão do C para manipulação de arquivos (stdio),
// alocação de memória (stdlib) e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define os nomes dos arquivos que serão usados para persistir os dados dos recursos.
// Usar macros facilita a manutenção do código, pois os nomes dos arquivos
// ficam centralizados e fáceis de alterar.
#define RECURSOS_DATA_FILE "data/recursos.dat" // Arquivo para dados em formato binário.
#define RECURSOS_TEXT_FILE "data/recursos.txt" // Arquivo para dados em formato de texto.

// Implementação da função para salvar os dados dos recursos.
void salvarRecursos(Sistema *sistema) {
    // Obtém o modo de armazenamento configurado no sistema.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for 'MEMORIA', não salva em disco e a função retorna.
    if (modo == MEMORIA) return;

    // Inicializa o ponteiro de arquivo.
    FILE *arquivo = NULL;
    // Verifica qual o modo de persistência para abrir o arquivo correto.
    if (modo == ARQUIVO_BINARIO) {
        // Abre o arquivo binário em modo de escrita ("wb").
        arquivo = fopen(RECURSOS_DATA_FILE, "wb");
        // Se a abertura do arquivo foi bem-sucedida...
        if (arquivo) {
            // Primeiro, grava o número total de recursos.
            fwrite(&sistema->num_recursos, sizeof(int), 1, arquivo);
            // Em seguida, grava todo o array da lista de recursos de uma só vez.
            fwrite(sistema->lista_recursos, sizeof(Recurso), sistema->num_recursos, arquivo);
        }
    } else { // Se o modo for ARQUIVO_TEXTO...
        // Abre o arquivo de texto em modo de escrita ("w").
        arquivo = fopen(RECURSOS_TEXT_FILE, "w");
        // Se o arquivo foi aberto com sucesso...
        if (arquivo) {
            // Grava o número total de recursos na primeira linha.
            fprintf(arquivo, "%d\n", sistema->num_recursos);
            // Itera sobre cada recurso na lista.
            for (int i = 0; i < sistema->num_recursos; i++) {
                // Grava os dados de cada recurso em linhas separadas.
                fprintf(arquivo, "%d\n%s\n%s\n%d\n%.2f\n%.2f\n",
                        sistema->lista_recursos[i].codigo,
                        sistema->lista_recursos[i].descricao,
                        sistema->lista_recursos[i].categoria,
                        sistema->lista_recursos[i].quantidade_estoque,
                        sistema->lista_recursos[i].preco_custo,
                        sistema->lista_recursos[i].valor_locacao);
            }
        }
    }

    // Se o arquivo foi aberto, fecha-o para liberar os recursos.
    if (arquivo) fclose(arquivo);
    // Caso contrário, exibe uma mensagem de erro no console.
    else perror("Nao foi possivel abrir o arquivo de recursos");
}

// Implementação da função para carregar os dados dos recursos do disco.
void carregarRecursos(Sistema *sistema) {
    // Obtém o modo de armazenamento configurado.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for 'MEMORIA', não há nada para carregar.
    if (modo == MEMORIA) return;

    // Define o nome do arquivo e o modo de abertura (leitura) com base na configuração.
    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? RECURSOS_DATA_FILE : RECURSOS_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    // Tenta abrir o arquivo para leitura.
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    // Se o arquivo não puder ser aberto (ex: primeira execução do programa), a função retorna.
    if (!arquivo) return;

    // Processa o arquivo com base no modo.
    if (modo == ARQUIVO_BINARIO) {
        // Lê a quantidade de recursos do arquivo.
        fread(&sistema->num_recursos, sizeof(int), 1, arquivo);
        // Se houver algum recurso para carregar...
        if (sistema->num_recursos > 0) {
            // Aloca a memória necessária para a lista.
            sistema->lista_recursos = malloc(sistema->num_recursos * sizeof(Recurso));
            // Se a alocação de memória for bem-sucedida...
            if (sistema->lista_recursos) {
                // Lê o bloco completo de dados dos recursos do arquivo para a memória.
                fread(sistema->lista_recursos, sizeof(Recurso), sistema->num_recursos, arquivo);
                // Define a capacidade da lista como o número de itens carregados.
                sistema->capacidade_recursos = sistema->num_recursos;
            }
        }
    } else { // Se o modo for ARQUIVO_TEXTO...
        // Lê a quantidade de recursos da primeira linha do arquivo.
        fscanf(arquivo, "%d\n", &sistema->num_recursos);
        // Se houver recursos...
        if (sistema->num_recursos > 0) {
            // Aloca a memória e ajusta a capacidade.
            sistema->lista_recursos = malloc(sistema->num_recursos * sizeof(Recurso));
            sistema->capacidade_recursos = sistema->num_recursos;
            // Loop para ler os dados de cada recurso.
            for (int i = 0; i < sistema->num_recursos; i++) {
                // Ponteiro para o recurso atual para facilitar o acesso.
                Recurso *r = &sistema->lista_recursos[i];
                // Lê o código do recurso.
                fscanf(arquivo, "%d\n", &r->codigo);
                // Lê as strings, uma por linha, e remove o caractere '\n' do final.
                fgets(r->descricao, sizeof(r->descricao), arquivo); r->descricao[strcspn(r->descricao, "\n")] = 0;
                fgets(r->categoria, sizeof(r->categoria), arquivo); r->categoria[strcspn(r->categoria, "\n")] = 0;
                // Lê os valores numéricos.
                fscanf(arquivo, "%d\n", &r->quantidade_estoque);
                fscanf(arquivo, "%f\n", &r->preco_custo);
                fscanf(arquivo, "%f\n", &r->valor_locacao);
            }
        }
    }
    // Fecha o arquivo após a leitura.
    fclose(arquivo);
}

// Implementação da função para liberar a memória da lista de recursos.
void liberarMemoriaRecursos(Sistema *sistema) {
    // Verifica se o ponteiro da lista não é nulo para evitar erros.
    if (sistema->lista_recursos != NULL) {
        // Libera a memória alocada para a lista.
        free(sistema->lista_recursos);
        // Define o ponteiro como nulo para segurança.
        sistema->lista_recursos = NULL;
        // Zera as variáveis de controle da lista.
        sistema->num_recursos = 0;
        sistema->capacidade_recursos = 0;
    }
}