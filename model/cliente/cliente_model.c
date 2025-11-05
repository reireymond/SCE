// Inclui o cabeçalho do model de cliente, que contém as declarações das funções implementadas aqui.
#include "model/cliente/cliente_model.h"
// Inclui cabeçalhos padrão do C para manipulação de arquivos (stdio), alocação de memória (stdlib)
// e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define o caminho e o nome do arquivo binário onde os dados dos clientes serão armazenados.
#define CLIENTES_DATA_FILE "data/clientes.dat"
// Define o caminho e o nome do arquivo de texto onde os dados dos clientes serão armazenados.
#define CLIENTES_TEXT_FILE "data/clientes.txt"

// Implementação da função para salvar os dados dos clientes.
void salvarClientes(Sistema *sistema) {
    // Obtém o modo de armazenamento configurado no sistema (MEMORIA, ARQUIVO_TEXTO ou ARQUIVO_BINARIO).
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for apenas MEMORIA, a função não faz nada e retorna, pois não há persistência em disco.
    if (modo == MEMORIA) return;

    // Declara um ponteiro de arquivo que será usado para escrita.
    FILE *arquivo = NULL;

    // Se o modo de armazenamento for ARQUIVO_BINARIO...
    if (modo == ARQUIVO_BINARIO) {
        // Abre o arquivo binário no modo de escrita binária ("wb").
        arquivo = fopen(CLIENTES_DATA_FILE, "wb");
        // Se a abertura do arquivo for bem-sucedida
        if (arquivo) {
            // Escreve o número total de clientes no início do arquivo.
            fwrite(&sistema->num_clientes, sizeof(int), 1, arquivo);
            // Escreve todo o array (bloco de memória) da lista de clientes de uma só vez no arquivo.
            fwrite(sistema->lista_clientes, sizeof(Cliente), sistema->num_clientes, arquivo);
        }
    // Se o modo de armazenamento for ARQUIVO_TEXTO...
    } else if (modo == ARQUIVO_TEXTO) {
        // Abre o arquivo de texto no modo de escrita de texto ("w").
        arquivo = fopen(CLIENTES_TEXT_FILE, "w");
        // Se a abertura do arquivo for bem-sucedida
        if (arquivo) {
            // Escreve o número total de clientes na primeira linha do arquivo.
            fprintf(arquivo, "%d\n", sistema->num_clientes);
            // Itera sobre cada cliente na lista.
            for (int i = 0; i < sistema->num_clientes; i++) {
                // Escreve os dados de cada cliente em linhas separadas, de forma formatada.
                fprintf(arquivo, "%d\n%s\n%s\n%s\n%s\n%s\n%s\n",
                        sistema->lista_clientes[i].codigo,
                        sistema->lista_clientes[i].razao_social,
                        sistema->lista_clientes[i].cnpj,
                        sistema->lista_clientes[i].endereco,
                        sistema->lista_clientes[i].telefone,
                        sistema->lista_clientes[i].email,
                        sistema->lista_clientes[i].nome_do_contato);
            }
        }
    }

    // Se o arquivo foi aberto (ponteiro não é nulo), fecha-o para liberar o recurso.
    if (arquivo) {
        fclose(arquivo);
    } else {
        // Se o arquivo não pôde ser aberto, informa o erro ao usuário.
        perror("Erro ao abrir arquivo para salvar clientes");
    }
}

// Implementação da função para carregar os dados dos clientes a partir de um arquivo.
void carregarClientes(Sistema *sistema) {
    // Obtém o modo de armazenamento configurado.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for MEMORIA, não há nada a carregar, então a função retorna.
    if (modo == MEMORIA) return;

    // Declara um ponteiro de arquivo.
    FILE *arquivo = NULL;
    // Define o nome do arquivo e o modo de abertura com base na configuração do sistema.
    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? CLIENTES_DATA_FILE : CLIENTES_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r"; // "rb" para leitura binária, "r" para leitura de texto.

    // Tenta abrir o arquivo.
    arquivo = fopen(nomeArquivo, modoAbertura);
    // Se o arquivo não pôde ser aberto (pode não existir na primeira execução), a função simplesmente retorna.
    if (arquivo == NULL) {
        return;
    }

    // Se o modo for ARQUIVO_BINARIO...
    if (modo == ARQUIVO_BINARIO) {
        // Lê o número de clientes do início do arquivo.
        fread(&sistema->num_clientes, sizeof(int), 1, arquivo);
        // Se houver clientes para carregar...
        if (sistema->num_clientes > 0) {
            // Aloca memória dinamicamente para a lista de clientes com o tamanho exato necessário.
            sistema->lista_clientes = malloc(sistema->num_clientes * sizeof(Cliente));
            // Verifica se a alocação de memória foi bem-sucedida.
            if (sistema->lista_clientes == NULL) {
                printf("Erro ao alocar memoria para carregar clientes.\n");
                sistema->num_clientes = 0; // Zera o número de clientes para evitar problemas.
                fclose(arquivo);
                return;
            }
            // Lê o bloco de dados contendo todos os clientes diretamente para a memória alocada.
            fread(sistema->lista_clientes, sizeof(Cliente), sistema->num_clientes, arquivo);
            // Atualiza a capacidade da lista para refletir o número de clientes carregados.
            sistema->capacidade_clientes = sistema->num_clientes;
        }
    // Se o modo for ARQUIVO_TEXTO...
    } else {
        // Lê o número de clientes da primeira linha do arquivo de texto.
        fscanf(arquivo, "%d\n", &sistema->num_clientes);
        // Se houver clientes para carregar...
        if (sistema->num_clientes > 0) {
            // Aloca memória para a lista e atualiza a capacidade.
            sistema->lista_clientes = malloc(sistema->num_clientes * sizeof(Cliente));
            sistema->capacidade_clientes = sistema->num_clientes;
            // Itera para ler cada cliente individualmente.
            for (int i = 0; i < sistema->num_clientes; i++) {
                // Cria um ponteiro para o cliente atual para simplificar o código.
                Cliente *c = &sistema->lista_clientes[i];
                // Lê o código do cliente.
                fscanf(arquivo, "%d\n", &c->codigo);
                // Lê cada campo de string usando fgets, que é mais seguro que fscanf para strings.
                // Em seguida, remove o caractere de nova linha '\n' que o fgets captura.
                fgets(c->razao_social, sizeof(c->razao_social), arquivo); c->razao_social[strcspn(c->razao_social, "\n")] = 0;
                fgets(c->cnpj, sizeof(c->cnpj), arquivo); c->cnpj[strcspn(c->cnpj, "\n")] = 0;
                fgets(c->endereco, sizeof(c->endereco), arquivo); c->endereco[strcspn(c->endereco, "\n")] = 0;
                fgets(c->telefone, sizeof(c->telefone), arquivo); c->telefone[strcspn(c->telefone, "\n")] = 0;
                fgets(c->email, sizeof(c->email), arquivo); c->email[strcspn(c->email, "\n")] = 0;
                fgets(c->nome_do_contato, sizeof(c->nome_do_contato), arquivo); c->nome_do_contato[strcspn(c->nome_do_contato, "\n")] = 0;
            }
        }
    }

    // Fecha o arquivo após a conclusão da leitura.
    fclose(arquivo);
}

// Implementação da função para liberar a memória alocada para a lista de clientes.
void liberarMemoriaClientes(Sistema *sistema) {
    // Verifica se o ponteiro da lista não é nulo, para evitar tentar liberar memória já liberada.
    if (sistema->lista_clientes != NULL) {
        // Libera o bloco de memória que foi alocado para a lista de clientes.
        free(sistema->lista_clientes);
        // Define o ponteiro como nulo para indicar que a memória não está mais alocada.
        // Isso previne o uso acidental do ponteiro antigo (dangling pointer).
        sistema->lista_clientes = NULL;
        // Zera os contadores de número e capacidade de clientes.
        sistema->num_clientes = 0;
        sistema->capacidade_clientes = 0;
    }
}