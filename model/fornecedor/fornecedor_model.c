// Inclui o cabeçalho do model de fornecedor, que contém as declarações das funções
// que são implementadas neste arquivo.
#include "model/fornecedor/fornecedor_model.h"
// Inclui os cabeçalhos padrão do C para manipulação de arquivos (stdio.h),
// alocação de memória (stdlib.h) e funções para strings (string.h).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define os nomes dos arquivos que serão usados para persistência dos dados.
// Usar macros torna a manutenção do código mais simples, pois os nomes dos arquivos
// ficam centralizados em um único lugar.
#define FORNECEDORES_DATA_FILE "data/fornecedores.dat" // Arquivo para dados em formato binário.
#define FORNECEDORES_TEXT_FILE "data/fornecedores.txt" // Arquivo para dados em formato de texto.

// Implementação da função para salvar os dados dos fornecedores.
void salvarFornecedores(Sistema *sistema) {
    // Obtém o modo de armazenamento atual (ex: MEMORIA, ARQUIVO_TEXTO, etc.).
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for apenas em memória, não é necessário salvar em disco, então a função retorna.
    if (modo == MEMORIA) return;

    // Declara o ponteiro de arquivo, que será usado para apontar para o arquivo aberto.
    FILE *arquivo = NULL;
    // Verifica o modo de armazenamento para decidir como salvar os dados.
    if (modo == ARQUIVO_BINARIO) {
        // Abre o arquivo binário em modo de escrita ("wb").
        arquivo = fopen(FORNECEDORES_DATA_FILE, "wb");
        // Se a abertura do arquivo for bem-sucedida...
        if (arquivo) {
            // Primeiro, grava o número total de fornecedores. Isso é essencial
            // para saber quantos registros ler ao carregar os dados.
            fwrite(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
            // Em seguida, grava todo o bloco de memória da lista de fornecedores no arquivo.
            fwrite(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
        }
    } else { // Se o modo for ARQUIVO_TEXTO...
        // Abre o arquivo de texto em modo de escrita ("w").
        arquivo = fopen(FORNECEDORES_TEXT_FILE, "w");
        // Se o arquivo foi aberto com sucesso...
        if (arquivo) {
            // Grava o número de fornecedores na primeira linha do arquivo.
            fprintf(arquivo, "%d\n", sistema->num_fornecedores);
            // Percorre toda a lista de fornecedores.
            for (int i = 0; i < sistema->num_fornecedores; i++) {
                // Para cada fornecedor, grava seus dados em linhas separadas,
                // facilitando a leitura e a depuração manual do arquivo.
                fprintf(arquivo, "%d\n%s\n%s\n%s\n%s\n%s\n%s\n",
                        sistema->lista_fornecedores[i].codigo,
                        sistema->lista_fornecedores[i].nome_fantasia,
                        sistema->lista_fornecedores[i].razao_social,
                        sistema->lista_fornecedores[i].cnpj,
                        sistema->lista_fornecedores[i].endereco,
                        sistema->lista_fornecedores[i].telefone,
                        sistema->lista_fornecedores[i].tipo_servico);
            }
        }
    }

    // Se o arquivo foi aberto (ponteiro não é nulo), fecha-o.
    if (arquivo) fclose(arquivo);
    // Caso contrário, se houve um erro na abertura, exibe uma mensagem de erro no console.
    else perror("Nao foi possivel abrir o arquivo de fornecedores");
}

// Implementação da função para carregar os dados dos fornecedores.
void carregarFornecedores(Sistema *sistema) {
    // Obtém o modo de armazenamento configurado.
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    // Se o modo for 'MEMORIA', não há nada para carregar do disco.
    if (modo == MEMORIA) return;

    // Define o nome do arquivo e o modo de abertura (leitura) com base na configuração do sistema.
    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? FORNECEDORES_DATA_FILE : FORNECEDORES_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    // Tenta abrir o arquivo para leitura.
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    // Se o arquivo não puder ser aberto (ex: não existe ainda), a função simplesmente retorna.
    if (!arquivo) return;

    // Processa o arquivo com base no modo de armazenamento.
    if (modo == ARQUIVO_BINARIO) {
        // Lê o número de fornecedores gravado no início do arquivo.
        fread(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
        // Se houver algum fornecedor para carregar...
        if (sistema->num_fornecedores > 0) {
            // Aloca a quantidade exata de memória necessária para a lista.
            sistema->lista_fornecedores = malloc(sistema->num_fornecedores * sizeof(Fornecedor));
            // Se a alocação de memória for bem-sucedida...
            if (sistema->lista_fornecedores) {
                // Lê o bloco completo de dados dos fornecedores do arquivo para a memória.
                fread(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
                // Atualiza a capacidade da lista para o número de itens carregados.
                sistema->capacidade_fornecedores = sistema->num_fornecedores;
            }
        }
    } else { // Se o modo for ARQUIVO_TEXTO...
        // Lê o número de fornecedores da primeira linha do arquivo.
        fscanf(arquivo, "%d\n", &sistema->num_fornecedores);
        // Se houver algum fornecedor...
        if (sistema->num_fornecedores > 0) {
            // Aloca a memória e ajusta a capacidade.
            sistema->lista_fornecedores = malloc(sistema->num_fornecedores * sizeof(Fornecedor));
            sistema->capacidade_fornecedores = sistema->num_fornecedores;
            // Loop para ler cada fornecedor do arquivo.
            for (int i = 0; i < sistema->num_fornecedores; i++) {
                // Cria um ponteiro para o elemento atual da lista para simplificar o código.
                Fornecedor *f = &sistema->lista_fornecedores[i];
                // Lê o código do fornecedor.
                fscanf(arquivo, "%d\n", &f->codigo);
                // Lê as strings, uma por linha, e remove o caractere de nova linha '\n'.
                fgets(f->nome_fantasia, sizeof(f->nome_fantasia), arquivo); f->nome_fantasia[strcspn(f->nome_fantasia, "\n")] = 0;
                fgets(f->razao_social, sizeof(f->razao_social), arquivo); f->razao_social[strcspn(f->razao_social, "\n")] = 0;
                fgets(f->cnpj, sizeof(f->cnpj), arquivo); f->cnpj[strcspn(f->cnpj, "\n")] = 0;
                fgets(f->endereco, sizeof(f->endereco), arquivo); f->endereco[strcspn(f->endereco, "\n")] = 0;
                fgets(f->telefone, sizeof(f->telefone), arquivo); f->telefone[strcspn(f->telefone, "\n")] = 0;
                fgets(f->tipo_servico, sizeof(f->tipo_servico), arquivo); f->tipo_servico[strcspn(f->tipo_servico, "\n")] = 0;
            }
        }
    }
    // Fecha o arquivo após a leitura.
    fclose(arquivo);
}

// Implementação da função para liberar a memória da lista de fornecedores.
void liberarMemoriaFornecedores(Sistema *sistema) {
    // Somente tenta liberar a memória se o ponteiro não for nulo.
    if (sistema->lista_fornecedores != NULL) {
        // Libera o bloco de memória alocado.
        free(sistema->lista_fornecedores);
        // Define o ponteiro como nulo para evitar o uso indevido de um ponteiro inválido.
        sistema->lista_fornecedores = NULL;
        // Zera as variáveis de controle da lista.
        sistema->num_fornecedores = 0;
        sistema->capacidade_fornecedores = 0;
    }
}