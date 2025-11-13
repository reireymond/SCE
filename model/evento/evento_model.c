#include "model/evento/evento_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

// Define os nomes dos arquivos de persistência
#define EVENTOS_DATA_FILE "data/eventos.dat"
#define EVENTOS_TEXT_FILE "data/eventos.txt"

/**
 * Salva a lista de eventos (e seus sub-itens) no modo de armazenamento configurado.
 * Esta função é complexa pois precisa salvar as listas dinâmicas aninhadas
 * (recursos, equipe, serviços) de CADA evento.
 */
void salvarEventos(Sistema *sistema)
{
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA)
        return;

    FILE *arquivo = NULL;

    if (modo == ARQUIVO_BINARIO)
    {
        arquivo = fopen(EVENTOS_DATA_FILE, "wb");
        if (!arquivo)
        {
            perror("Erro ao abrir arquivo binario de eventos");
            return;
        }

        // 1. Escreve o número total de eventos
        fwrite(&sistema->num_eventos, sizeof(int), 1, arquivo);

        // 2. Itera por cada evento
        for (int i = 0; i < sistema->num_eventos; i++)
        {
            Evento *e = &sistema->lista_eventos[i];

            // 2a. Escreve os campos "planos" da estrutura Evento
            fwrite(&e->codigo, sizeof(int), 1, arquivo);
            fwrite(e->nome_evento, sizeof(char), 150, arquivo);
            fwrite(&e->codigo_cliente, sizeof(int), 1, arquivo);
            fwrite(&e->status, sizeof(StatusEvento), 1, arquivo);
            fwrite(e->data_inicio, sizeof(char), 11, arquivo);
            fwrite(e->data_fim, sizeof(char), 11, arquivo);
            fwrite(e->local, sizeof(char), 150, arquivo);
            fwrite(&e->custo_total_previsto, sizeof(float), 1, arquivo);
            fwrite(&e->valor_final_faturado, sizeof(float), 1, arquivo);

            // 2b. Escreve a lista de Recursos Alocados
            fwrite(&e->num_recursos_alocados, sizeof(int), 1, arquivo);
            if (e->num_recursos_alocados > 0)
            {
                fwrite(e->lista_recursos_alocados, sizeof(ItemRecursoEvento), e->num_recursos_alocados, arquivo);
            }

            // 2c. Escreve a lista de Equipe Alocada
            fwrite(&e->num_equipe_alocada, sizeof(int), 1, arquivo);
            if (e->num_equipe_alocada > 0)
            {
                fwrite(e->lista_equipe_alocada, sizeof(ItemEquipeEvento), e->num_equipe_alocada, arquivo);
            }

            // 2d. Escreve a lista de Serviços Contratados
            fwrite(&e->num_servicos_contratados, sizeof(int), 1, arquivo);
            if (e->num_servicos_contratados > 0)
            {
                fwrite(e->lista_servicos_contratados, sizeof(ItemFornecedorEvento), e->num_servicos_contratados, arquivo);
            }
        }
    }
    else // ARQUIVO_TEXTO
    {
        arquivo = fopen(EVENTOS_TEXT_FILE, "w");
        if (!arquivo)
        {
            perror("Erro ao abrir arquivo de texto de eventos");
            return;
        }

        fprintf(arquivo, "%d\n", sistema->num_eventos);
        for (int i = 0; i < sistema->num_eventos; i++)
        {
            Evento *e = &sistema->lista_eventos[i];

            // 1. Campos "planos" do evento
            fprintf(arquivo, "%d\n%s\n%d\n%d\n%s\n%s\n%s\n%.2f\n%.2f\n",
                    e->codigo, e->nome_evento, e->codigo_cliente, (int)e->status,
                    e->data_inicio, e->data_fim, e->local,
                    e->custo_total_previsto, e->valor_final_faturado);

            // 2. Lista de Recursos
            fprintf(arquivo, "%d\n", e->num_recursos_alocados);
            for (int j = 0; j < e->num_recursos_alocados; j++)
            {
                ItemRecursoEvento *item = &e->lista_recursos_alocados[j];
                fprintf(arquivo, "%d\n%d\n%.2f\n",
                        item->codigo_recurso, item->quantidade, item->custo_locacao_momento);
            }

            // 3. Lista de Equipe
            fprintf(arquivo, "%d\n", e->num_equipe_alocada);
            for (int j = 0; j < e->num_equipe_alocada; j++)
            {
                ItemEquipeEvento *item = &e->lista_equipe_alocada[j];
                fprintf(arquivo, "%d\n%.2f\n",
                        item->codigo_equipe, item->custo_diaria_momento);
            }

            // 4. Lista de Serviços
            fprintf(arquivo, "%d\n", e->num_servicos_contratados);
            for (int j = 0; j < e->num_servicos_contratados; j++)
            {
                ItemFornecedorEvento *item = &e->lista_servicos_contratados[j];
                fprintf(arquivo, "%d\n%s\n%.2f\n",
                        item->codigo_fornecedor, item->descricao_servico, item->custo_contratado);
            }
        }
    }

    fclose(arquivo);
}

/**
 * Carrega os dados dos eventos do arquivo para a memória.
 * Também precisa carregar as sub-listas de cada evento.
 */
void carregarEventos(Sistema *sistema)
{
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA)
        return;

    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? EVENTOS_DATA_FILE : EVENTOS_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    if (!arquivo)
        return; // Arquivo não existe (normal na primeira execução)

    if (modo == ARQUIVO_BINARIO)
    {
        // 1. Lê o número total de eventos
        fread(&sistema->num_eventos, sizeof(int), 1, arquivo);
        if (sistema->num_eventos > 0)
        {
            // 2. Aloca a lista principal de eventos
            sistema->lista_eventos = malloc(sistema->num_eventos * sizeof(Evento));
            if (!sistema->lista_eventos)
            {
                printf("Erro de alocacao ao carregar eventos.\n");
                sistema->num_eventos = 0;
                fclose(arquivo);
                return;
            }
            sistema->capacidade_eventos = sistema->num_eventos;

            // 3. Itera por cada evento para carregar seus dados
            for (int i = 0; i < sistema->num_eventos; i++)
            {
                Evento *e = &sistema->lista_eventos[i];

                // 3a. Lê os campos "planos"
                fread(&e->codigo, sizeof(int), 1, arquivo);
                fread(e->nome_evento, sizeof(char), 150, arquivo);
                fread(&e->codigo_cliente, sizeof(int), 1, arquivo);
                fread(&e->status, sizeof(StatusEvento), 1, arquivo);
                fread(e->data_inicio, sizeof(char), 11, arquivo);
                fread(e->data_fim, sizeof(char), 11, arquivo);
                fread(e->local, sizeof(char), 150, arquivo);
                fread(&e->custo_total_previsto, sizeof(float), 1, arquivo);
                fread(&e->valor_final_faturado, sizeof(float), 1, arquivo);

                // Inicializa ponteiros internos
                e->lista_recursos_alocados = NULL;
                e->capacidade_recursos_alocados = 0;
                e->lista_equipe_alocada = NULL;
                e->capacidade_equipe_alocada = 0;
                e->lista_servicos_contratados = NULL;
                e->capacidade_servicos_contratados = 0;

                // 3b. Lê a lista de Recursos Alocados
                fread(&e->num_recursos_alocados, sizeof(int), 1, arquivo);
                if (e->num_recursos_alocados > 0)
                {
                    e->lista_recursos_alocados = malloc(e->num_recursos_alocados * sizeof(ItemRecursoEvento));
                    if (e->lista_recursos_alocados)
                    {
                        fread(e->lista_recursos_alocados, sizeof(ItemRecursoEvento), e->num_recursos_alocados, arquivo);
                        e->capacidade_recursos_alocados = e->num_recursos_alocados;
                    }
                }

                // 3c. Lê a lista de Equipe Alocada
                fread(&e->num_equipe_alocada, sizeof(int), 1, arquivo);
                if (e->num_equipe_alocada > 0)
                {
                    e->lista_equipe_alocada = malloc(e->num_equipe_alocada * sizeof(ItemEquipeEvento));
                    if (e->lista_equipe_alocada)
                    {
                        fread(e->lista_equipe_alocada, sizeof(ItemEquipeEvento), e->num_equipe_alocada, arquivo);
                        e->capacidade_equipe_alocada = e->num_equipe_alocada;
                    }
                }

                // 3d. Lê a lista de Serviços Contratados
                fread(&e->num_servicos_contratados, sizeof(int), 1, arquivo);
                if (e->num_servicos_contratados > 0)
                {
                    e->lista_servicos_contratados = malloc(e->num_servicos_contratados * sizeof(ItemFornecedorEvento));
                    if (e->lista_servicos_contratados)
                    {
                        fread(e->lista_servicos_contratados, sizeof(ItemFornecedorEvento), e->num_servicos_contratados, arquivo);
                        e->capacidade_servicos_contratados = e->num_servicos_contratados;
                    }
                }
            }
        }
    }
    else // ARQUIVO_TEXTO
    {
        fscanf(arquivo, "%d\n", &sistema->num_eventos);
        if (sistema->num_eventos > 0)
        {
            sistema->lista_eventos = malloc(sistema->num_eventos * sizeof(Evento));
            if (!sistema->lista_eventos)
            {
                printf("Erro de alocacao ao carregar eventos (texto).\n");
                sistema->num_eventos = 0;
                fclose(arquivo);
                return;
            }
            sistema->capacidade_eventos = sistema->num_eventos;

            for (int i = 0; i < sistema->num_eventos; i++)
            {
                Evento *e = &sistema->lista_eventos[i];
                int status_int;

                // 1. Lê campos "planos"
                fscanf(arquivo, "%d\n", &e->codigo);
                fgets(e->nome_evento, sizeof(e->nome_evento), arquivo);
                e->nome_evento[strcspn(e->nome_evento, "\n")] = 0;
                fscanf(arquivo, "%d\n", &e->codigo_cliente);
                fscanf(arquivo, "%d\n", &status_int);
                e->status = (StatusEvento)status_int;
                fgets(e->data_inicio, sizeof(e->data_inicio), arquivo);
                e->data_inicio[strcspn(e->data_inicio, "\n")] = 0;
                fgets(e->data_fim, sizeof(e->data_fim), arquivo);
                e->data_fim[strcspn(e->data_fim, "\n")] = 0;
                fgets(e->local, sizeof(e->local), arquivo);
                e->local[strcspn(e->local, "\n")] = 0;
                fscanf(arquivo, "%f\n", &e->custo_total_previsto);
                fscanf(arquivo, "%f\n", &e->valor_final_faturado);

                // Inicializa ponteiros
                e->lista_recursos_alocados = NULL;
                e->capacidade_recursos_alocados = 0;
                e->lista_equipe_alocada = NULL;
                e->capacidade_equipe_alocada = 0;
                e->lista_servicos_contratados = NULL;
                e->capacidade_servicos_contratados = 0;

                // 2. Lê Lista de Recursos
                fscanf(arquivo, "%d\n", &e->num_recursos_alocados);
                if (e->num_recursos_alocados > 0)
                {
                    e->lista_recursos_alocados = malloc(e->num_recursos_alocados * sizeof(ItemRecursoEvento));
                    if (e->lista_recursos_alocados)
                    {
                        e->capacidade_recursos_alocados = e->num_recursos_alocados;
                        for (int j = 0; j < e->num_recursos_alocados; j++)
                        {
                            ItemRecursoEvento *item = &e->lista_recursos_alocados[j];
                            fscanf(arquivo, "%d\n%d\n%f\n", &item->codigo_recurso, &item->quantidade, &item->custo_locacao_momento);
                        }
                    }
                }

                // 3. Lê Lista de Equipe
                fscanf(arquivo, "%d\n", &e->num_equipe_alocada);
                if (e->num_equipe_alocada > 0)
                {
                    e->lista_equipe_alocada = malloc(e->num_equipe_alocada * sizeof(ItemEquipeEvento));
                    if (e->lista_equipe_alocada)
                    {
                        e->capacidade_equipe_alocada = e->num_equipe_alocada;
                        for (int j = 0; j < e->num_equipe_alocada; j++)
                        {
                            ItemEquipeEvento *item = &e->lista_equipe_alocada[j];
                            fscanf(arquivo, "%d\n%f\n", &item->codigo_equipe, &item->custo_diaria_momento);
                        }
                    }
                }

                // 4. Lê Lista de Serviços
                fscanf(arquivo, "%d\n", &e->num_servicos_contratados);
                if (e->num_servicos_contratados > 0)
                {
                    e->lista_servicos_contratados = malloc(e->num_servicos_contratados * sizeof(ItemFornecedorEvento));
                    if (e->lista_servicos_contratados)
                    {
                        e->capacidade_servicos_contratados = e->num_servicos_contratados;
                        for (int j = 0; j < e->num_servicos_contratados; j++)
                        {
                            ItemFornecedorEvento *item = &e->lista_servicos_contratados[j];
                            fscanf(arquivo, "%d\n", &item->codigo_fornecedor);
                            fgets(item->descricao_servico, sizeof(item->descricao_servico), arquivo);
                            item->descricao_servico[strcspn(item->descricao_servico, "\n")] = 0;
                            fscanf(arquivo, "%f\n", &item->custo_contratado);
                        }
                    }
                }
            }
        }
    }

    fclose(arquivo);
}

/**
 * Libera a memória alocada para a lista de eventos.
 * Precisa iterar e liberar as sub-listas de cada evento primeiro.
 */
void liberarMemoriaEventos(Sistema *sistema)
{
    if (sistema->lista_eventos != NULL)
    {
        // 1. Itera por cada evento e libera suas listas internas
        for (int i = 0; i < sistema->num_eventos; i++)
        {
            Evento *e = &sistema->lista_eventos[i];

            if (e->lista_recursos_alocados != NULL)
            {
                free(e->lista_recursos_alocados);
            }
            if (e->lista_equipe_alocada != NULL)
            {
                free(e->lista_equipe_alocada);
            }
            if (e->lista_servicos_contratados != NULL)
            {
                free(e->lista_servicos_contratados);
            }
            // As estruturas internas (e->lista_... ) são zeradas
            // quando a lista principal (sistema->lista_eventos) é liberada.
        }

        // 2. Libera a lista principal de eventos
        free(sistema->lista_eventos);
        sistema->lista_eventos = NULL;
        sistema->num_eventos = 0;
        sistema->capacidade_eventos = 0;
    }
}