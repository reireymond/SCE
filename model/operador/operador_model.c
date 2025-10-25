// Inclui o cabeçalho do model de operador, garantindo que a implementação
// esteja de acordo com as funções declaradas.
#include "model/operador/operador_model.h"
// Inclui os cabeçalhos padrão do C para manipulação de arquivos (stdio.h),
// alocação de memória (stdlib.h) e funções de string (string.h).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Inclui o cabeçalho de configuração para obter o modo de armazenamento
#include "model/config_armazenamento/config_armazenamento.h"

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
            // Grava todo o array da lista de operadores de uma só vez, se houver operadores.
            if (sistema->num_operadores > 0) {
                 fwrite(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
            }
            fclose(arquivo); // Fecha o arquivo aqui dentro do if(arquivo)
        } else {
            // Se falhar ao abrir, exibe erro e retorna
            perror("Nao foi possivel abrir o arquivo binario de operadores");
            return; // <-- CORREÇÃO: Parar a execução em caso de erro
        }
    } else if (modo == ARQUIVO_TEXTO) { // Usa else if para clareza
        // Abre o arquivo de texto para escrita ("w").
        arquivo = fopen(OPERADORES_TEXT_FILE, "w");
        // Se a abertura for bem-sucedida...
        if (arquivo) {
            // Grava a quantidade de operadores na primeira linha.
            fprintf(arquivo, "%d\n", sistema->num_operadores);
            // Percorre a lista de operadores.
            for (int i = 0; i < sistema->num_operadores; i++) {
                // Grava os dados de cada operador em linhas separadas.
                fprintf(arquivo, "%d\n%s\n%s\n%s\n",
                        sistema->lista_operadores[i].codigo,
                        sistema->lista_operadores[i].nome,
                        sistema->lista_operadores[i].usuario,
                        sistema->lista_operadores[i].senha);
            }
            fclose(arquivo); // Fecha o arquivo aqui dentro do if(arquivo)
        } else {
            // Se falhar ao abrir, exibe erro e retorna
            perror("Nao foi possivel abrir o arquivo de texto de operadores");
            return; // <-- CORREÇÃO: Parar a execução em caso de erro
        }
    }
    // Não é necessário mais nenhum código aqui fora
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
            } else {
                // Se o malloc falhar, imprime um erro e zera num_operadores
                printf("Erro ao alocar memoria para carregar operadores.\n");
                sistema->num_operadores = 0;
            }
        } else {
             sistema->num_operadores = 0; // Garante que seja 0 se a leitura for 0
             sistema->lista_operadores = NULL; // Garante que lista seja NULL
             sistema->capacidade_operadores = 0;
        }
    } else { // Se o modo for ARQUIVO_TEXTO...
        // Lê a quantidade de operadores da primeira linha.
        if (fscanf(arquivo, "%d\n", &sistema->num_operadores) != 1) {
            // Se a leitura falhar, considera como 0 operadores
             sistema->num_operadores = 0;
        }

        // Se houver operadores...
        if (sistema->num_operadores > 0) {
            // Aloca a memória e define a capacidade.
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));

            if (sistema->lista_operadores) {
                 sistema->capacidade_operadores = sistema->num_operadores;
                // Loop para ler os dados de cada operador.
                for (int i = 0; i < sistema->num_operadores; i++) {
                    // Ponteiro para o operador atual para simplificar o código.
                    Operador *op = &sistema->lista_operadores[i];
                    // Lê o código do operador.
                    if (fscanf(arquivo, "%d\n", &op->codigo) != 1) {
                         printf("Erro ao ler codigo do operador %d no arquivo texto.\n", i + 1);
                         // Tratar erro: talvez parar leitura, zerar num_operadores?
                         // Por simplicidade, continua, mas pode carregar dados incompletos.
                         sistema->num_operadores = i; // Atualiza para o número lido com sucesso
                         break;
                    }
                    // Lê as strings (nome, usuário, senha) e remove o caractere de nova linha.
                    if (fgets(op->nome, sizeof(op->nome), arquivo) == NULL) break; // Sai se houver erro
                    op->nome[strcspn(op->nome, "\n")] = 0;

                    if (fgets(op->usuario, sizeof(op->usuario), arquivo) == NULL) break;
                    op->usuario[strcspn(op->usuario, "\n")] = 0;

                    if (fgets(op->senha, sizeof(op->senha), arquivo) == NULL) break;
                    op->senha[strcspn(op->senha, "\n")] = 0;
                }
            } else {
                 printf("Erro ao alocar memoria para carregar operadores (texto).\n");
                 sistema->num_operadores = 0;
            }
        } else {
             sistema->num_operadores = 0; // Garante que seja 0 se a leitura for 0 ou negativa
             sistema->lista_operadores = NULL;
             sistema->capacidade_operadores = 0;
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