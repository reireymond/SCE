// Inclui o cabeçalho do model de operador, garantindo que a implementação
// esteja de acordo com as funções declaradas.
#include "model/operador/operador_model.h"
// Inclui os cabeçalhos padrão do C para manipulação de arquivos (stdio.h),
// alocação de memória (stdlib.h) e funções de string (string.h).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define os nomes dos arquivos para persistência de dados dos operadores.
// Isso centraliza os nomes dos arquivos, facilitando futuras manutenções.
#define OPERADORES_DATA_FILE "data/operadores.dat" // Arquivo para armazenamento em formato binário.
#define OPERADORES_TEXT_FILE "data/operadores.txt" // Arquivo para armazenamento em formato de texto.

// Implementação da função para salvar a lista de operadores.
void salvarOperadores(Sistema *sistema) {
    // Obtém o modo de armazenamento atual configurado no sistema.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for 'MEMORIA', os dados não são salvos em disco, então a função retorna.
    if (modo == MEMORIA) return;

    // Inicializa o ponteiro de arquivo.
    FILE *arquivo = NULL;
    // Verifica o modo de armazenamento para decidir como salvar.
    if (modo == ARQUIVO_BINARIO) {
        // Abre o arquivo binário para escrita ("wb").
        arquivo = fopen(OPERADORES_DATA_FILE, "wb");
        // Se a abertura for bem-sucedida...
        if (arquivo) {
            // Grava a quantidade de operadores no início do arquivo.
            fwrite(&sistema->num_operadores, sizeof(int), 1, arquivo);
            // Grava todo o array da lista de operadores de uma só vez.
            fwrite(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
        }
    } else { // Se o modo for ARQUIVO_TEXTO...
        // Abre o arquivo de texto para escrita ("w").
        arquivo = fopen(OPERADORES_TEXT_FILE, "w");
        // Se a abertura for bem-sucedida...
        if (arquivo) {
            // Grava a quantidade de operadores na primeira linha.
            fprintf(arquivo, "%d\n", sistema->num_operadores);
            // Percorre a lista de operadores.
            for (int i = 0; i < sistema->num_operadores; i++) {
                // Grava os dados de cada operador em linhas separadas.
                // A senha também é salva no arquivo de texto.
                fprintf(arquivo, "%d\n%s\n%s\n%s\n",
                        sistema->lista_operadores[i].codigo,
                        sistema->lista_operadores[i].nome,
                        sistema->lista_operadores[i].usuario,
                        sistema->lista_operadores[i].senha);
            }
        }
    }
    // Se o arquivo foi aberto com sucesso, fecha-o.
    if (arquivo) fclose(arquivo);
    // Caso contrário, exibe uma mensagem de erro.
    else perror("Nao foi possivel abrir o arquivo de operadores");
}

// Implementação da função para carregar os dados dos operadores do disco para a memória.
void carregarOperadores(Sistema *sistema) {
    // Obtém o modo de armazenamento configurado.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se for 'MEMORIA', não há nada para carregar.
    if (modo == MEMORIA) return;

    // Define o nome do arquivo e o modo de abertura (leitura) com base na configuração.
    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? OPERADORES_DATA_FILE : OPERADORES_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    // Tenta abrir o arquivo.
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    // Se o arquivo não puder ser aberto (pode não existir), a função retorna.
    if (!arquivo) return;

    // Processa o arquivo dependendo do modo.
    if (modo == ARQUIVO_BINARIO) {
        // Lê a quantidade de operadores do arquivo.
        fread(&sistema->num_operadores, sizeof(int), 1, arquivo);
        // Se existir algum operador para ser carregado...
        if (sistema->num_operadores > 0) {
            // Aloca memória para a lista de operadores.
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            // Se a alocação de memória funcionar...
            if (sistema->lista_operadores) {
                // Lê o bloco de dados do arquivo diretamente para a memória.
                fread(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
                // Define a capacidade da lista.
                sistema->capacidade_operadores = sistema->num_operadores;
            }
        }
    } else { // Se o modo for ARQUIVO_TEXTO...
        // Lê a quantidade de operadores da primeira linha.
        fscanf(arquivo, "%d\n", &sistema->num_operadores);
        // Se houver operadores...
        if (sistema->num_operadores > 0) {
            // Aloca a memória e define a capacidade.
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            sistema->capacidade_operadores = sistema->num_operadores;
            // Loop para ler os dados de cada operador.
            for (int i = 0; i < sistema->num_operadores; i++) {
                // Ponteiro para o operador atual para simplificar o código.
                Operador *op = &sistema->lista_operadores[i];
                // Lê o código do operador.
                fscanf(arquivo, "%d\n", &op->codigo);
                // Lê as strings (nome, usuário, senha) e remove o caractere de nova linha.
                fgets(op->nome, sizeof(op->nome), arquivo); op->nome[strcspn(op->nome, "\n")] = 0;
                fgets(op->usuario, sizeof(op->usuario), arquivo); op->usuario[strcspn(op->usuario, "\n")] = 0;
                fgets(op->senha, sizeof(op->senha), arquivo); op->senha[strcspn(op->senha, "\n")] = 0;
            }
        }
    }
    // Fecha o arquivo após a leitura.
    fclose(arquivo);
}

// Implementação da função para liberar a memória da lista de operadores.
void liberarMemoriaOperadores(Sistema *sistema) {
    // Verifica se a lista de operadores não é nula.
    if (sistema->lista_operadores != NULL) {
        // Libera a memória alocada para a lista.
        free(sistema->lista_operadores);
        // Define o ponteiro como nulo para evitar o uso acidental.
        sistema->lista_operadores = NULL;
        // Zera os contadores.
        sistema->num_operadores = 0;
        sistema->capacidade_operadores = 0;
    }
}