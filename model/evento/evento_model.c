#include "evento_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define EVENTOS_DATA_FILE "data/eventos.dat"
#define EVENTOS_TEXT_FILE "data/eventos.txt"

void salvarEventos(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if(modo == MEMORIA) return;

    FILE *f = fopen(modo == ARQUIVO_BINARIO ? EVENTOS_DATA_FILE : EVENTOS_TEXT_FILE, 
                    modo == ARQUIVO_BINARIO ? "wb" : "w");
    if(!f) return;

    if(modo == ARQUIVO_BINARIO) {
        fwrite(&sistema->num_eventos, sizeof(int), 1, f);
        for(int i=0; i<sistema->num_eventos; i++) {
            Evento *e = &sistema->lista_eventos[i];
            // Salva estrutura principal
            fwrite(&e->codigo, sizeof(int), 1, f);
            fwrite(e->nome_evento, sizeof(char), 150, f);
            fwrite(&e->codigo_cliente, sizeof(int), 1, f);
            fwrite(&e->status, sizeof(StatusEvento), 1, f);
            
            // Salva datas e horas
            fwrite(e->data_inicio, sizeof(char), 15, f);
            fwrite(e->hora_inicio, sizeof(char), 6, f); 
            fwrite(e->data_fim, sizeof(char), 15, f);
            fwrite(e->hora_fim, sizeof(char), 6, f);    
            
            fwrite(e->local, sizeof(char), 150, f);
            fwrite(&e->custo_total_previsto, sizeof(float), 1, f);
            fwrite(&e->valor_final_faturado, sizeof(float), 1, f);
            
            // Salva Listas internas
            fwrite(&e->num_recursos_alocados, sizeof(int), 1, f);
            if(e->num_recursos_alocados > 0) fwrite(e->lista_recursos_alocados, sizeof(ItemRecursoEvento), e->num_recursos_alocados, f);

            fwrite(&e->num_equipe_alocada, sizeof(int), 1, f);
            if(e->num_equipe_alocada > 0) fwrite(e->lista_equipe_alocada, sizeof(ItemEquipeEvento), e->num_equipe_alocada, f);

            // NOVO: Salva fornecedores binario
            fwrite(&e->num_fornecedores_alocados, sizeof(int), 1, f);
            if(e->num_fornecedores_alocados > 0) 
                fwrite(e->lista_fornecedores_alocados, sizeof(ItemFornecedorEvento), e->num_fornecedores_alocados, f);
        }
    } else {
        fprintf(f, "%d\n", sistema->num_eventos);
        for(int i=0; i<sistema->num_eventos; i++) {
            Evento *e = &sistema->lista_eventos[i];
            // Salva data e hora no txt
            fprintf(f, "%d\n%s\n%d\n%d\n%s\n%s\n%s\n%s\n%s\n%.2f\n%.2f\n",
                e->codigo, e->nome_evento, e->codigo_cliente, e->status,
                e->data_inicio, e->hora_inicio, 
                e->data_fim, e->hora_fim,       
                e->local,
                e->custo_total_previsto, e->valor_final_faturado);
            
            fprintf(f, "%d\n", e->num_recursos_alocados);
            for(int j=0; j<e->num_recursos_alocados; j++)
                fprintf(f, "%d %d %.2f\n", e->lista_recursos_alocados[j].codigo_recurso, e->lista_recursos_alocados[j].quantidade, e->lista_recursos_alocados[j].custo_locacao_momento);
            
            fprintf(f, "%d\n", e->num_equipe_alocada);
            for(int j=0; j<e->num_equipe_alocada; j++)
                fprintf(f, "%d %.2f\n", e->lista_equipe_alocada[j].codigo_equipe, e->lista_equipe_alocada[j].custo_diaria_momento);

            // NOVO: Salva fornecedores txt
            fprintf(f, "%d\n", e->num_fornecedores_alocados);
            for(int j=0; j<e->num_fornecedores_alocados; j++)
                fprintf(f, "%d %.2f\n", e->lista_fornecedores_alocados[j].codigo_fornecedor, e->lista_fornecedores_alocados[j].valor_cobrado);
        }
    }
    fclose(f);
}

void carregarEventos(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if(modo == MEMORIA) return;

    FILE *f = fopen(modo == ARQUIVO_BINARIO ? EVENTOS_DATA_FILE : EVENTOS_TEXT_FILE, 
                    modo == ARQUIVO_BINARIO ? "rb" : "r");
    if(!f) return;

    if(modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_eventos, sizeof(int), 1, f);
        if(sistema->num_eventos > 0) {
            sistema->lista_eventos = malloc(sistema->num_eventos * sizeof(Evento));
            sistema->capacidade_eventos = sistema->num_eventos;
            
            for(int i=0; i<sistema->num_eventos; i++) {
                Evento *e = &sistema->lista_eventos[i];
                fread(&e->codigo, sizeof(int), 1, f);
                fread(e->nome_evento, sizeof(char), 150, f);
                fread(&e->codigo_cliente, sizeof(int), 1, f);
                fread(&e->status, sizeof(StatusEvento), 1, f);
                
                // Lendo datas e horas binarias
                fread(e->data_inicio, sizeof(char), 15, f);
                fread(e->hora_inicio, sizeof(char), 6, f); 
                fread(e->data_fim, sizeof(char), 15, f);
                fread(e->hora_fim, sizeof(char), 6, f);    
                
                fread(e->local, sizeof(char), 150, f);
                fread(&e->custo_total_previsto, sizeof(float), 1, f);
                fread(&e->valor_final_faturado, sizeof(float), 1, f);

                fread(&e->num_recursos_alocados, sizeof(int), 1, f);
                if(e->num_recursos_alocados > 0) {
                    e->lista_recursos_alocados = malloc(e->num_recursos_alocados * sizeof(ItemRecursoEvento));
                    fread(e->lista_recursos_alocados, sizeof(ItemRecursoEvento), e->num_recursos_alocados, f);
                } else e->lista_recursos_alocados = NULL;

                fread(&e->num_equipe_alocada, sizeof(int), 1, f);
                if(e->num_equipe_alocada > 0) {
                    e->lista_equipe_alocada = malloc(e->num_equipe_alocada * sizeof(ItemEquipeEvento));
                    fread(e->lista_equipe_alocada, sizeof(ItemEquipeEvento), e->num_equipe_alocada, f);
                } else e->lista_equipe_alocada = NULL;

                // NOVO: Carrega fornecedores binario
                fread(&e->num_fornecedores_alocados, sizeof(int), 1, f);
                if(e->num_fornecedores_alocados > 0) {
                    e->lista_fornecedores_alocados = malloc(e->num_fornecedores_alocados * sizeof(ItemFornecedorEvento));
                    fread(e->lista_fornecedores_alocados, sizeof(ItemFornecedorEvento), e->num_fornecedores_alocados, f);
                } else e->lista_fornecedores_alocados = NULL;
            }
        }
    } else {
        fscanf(f, "%d\n", &sistema->num_eventos);
        if(sistema->num_eventos > 0) {
            sistema->lista_eventos = malloc(sistema->num_eventos * sizeof(Evento));
            sistema->capacidade_eventos = sistema->num_eventos;
            for(int i=0; i<sistema->num_eventos; i++) {
                Evento *e = &sistema->lista_eventos[i];
                fscanf(f, "%d\n", &e->codigo);
                fgets(e->nome_evento, 150, f); e->nome_evento[strcspn(e->nome_evento, "\n")] = 0;
                fscanf(f, "%d\n%d\n", &e->codigo_cliente, (int*)&e->status);
                
                // Lendo datas e horas do texto
                fgets(e->data_inicio, 15, f); e->data_inicio[strcspn(e->data_inicio, "\n")] = 0;
                fgets(e->hora_inicio, 6, f); e->hora_inicio[strcspn(e->hora_inicio, "\n")] = 0; 
                fgets(e->data_fim, 15, f); e->data_fim[strcspn(e->data_fim, "\n")] = 0;
                fgets(e->hora_fim, 6, f); e->hora_fim[strcspn(e->hora_fim, "\n")] = 0; 
                
                fgets(e->local, 150, f); e->local[strcspn(e->local, "\n")] = 0;
                fscanf(f, "%f\n%f\n", &e->custo_total_previsto, &e->valor_final_faturado);

                fscanf(f, "%d\n", &e->num_recursos_alocados);
                if(e->num_recursos_alocados > 0) {
                    e->lista_recursos_alocados = malloc(e->num_recursos_alocados * sizeof(ItemRecursoEvento));
                    for(int j=0; j<e->num_recursos_alocados; j++)
                        fscanf(f, "%d %d %f\n", &e->lista_recursos_alocados[j].codigo_recurso, &e->lista_recursos_alocados[j].quantidade, &e->lista_recursos_alocados[j].custo_locacao_momento);
                } else e->lista_recursos_alocados = NULL;

                fscanf(f, "%d\n", &e->num_equipe_alocada);
                if(e->num_equipe_alocada > 0) {
                    e->lista_equipe_alocada = malloc(e->num_equipe_alocada * sizeof(ItemEquipeEvento));
                    for(int j=0; j<e->num_equipe_alocada; j++)
                        fscanf(f, "%d %f\n", &e->lista_equipe_alocada[j].codigo_equipe, &e->lista_equipe_alocada[j].custo_diaria_momento);
                } else e->lista_equipe_alocada = NULL;

                // NOVO: Carrega fornecedores texto
                fscanf(f, "%d\n", &e->num_fornecedores_alocados);
                if(e->num_fornecedores_alocados > 0) {
                    e->lista_fornecedores_alocados = malloc(e->num_fornecedores_alocados * sizeof(ItemFornecedorEvento));
                    for(int j=0; j<e->num_fornecedores_alocados; j++)
                        fscanf(f, "%d %f\n", &e->lista_fornecedores_alocados[j].codigo_fornecedor, &e->lista_fornecedores_alocados[j].valor_cobrado);
                } else e->lista_fornecedores_alocados = NULL;
            }
        }
    }
    fclose(f);
}

void liberarMemoriaEventos(Sistema *sistema) {
    if (sistema->lista_eventos != NULL) {
        for (int i = 0; i < sistema->num_eventos; i++) {
            if (sistema->lista_eventos[i].lista_recursos_alocados) 
                free(sistema->lista_eventos[i].lista_recursos_alocados);
            if (sistema->lista_eventos[i].lista_equipe_alocada) 
                free(sistema->lista_eventos[i].lista_equipe_alocada);
            // NOVO: Limpa a memoria dos fornecedores
            if (sistema->lista_eventos[i].lista_fornecedores_alocados) 
                free(sistema->lista_eventos[i].lista_fornecedores_alocados);
        }
        free(sistema->lista_eventos);
        sistema->lista_eventos = NULL;
        sistema->num_eventos = 0;
        sistema->capacidade_eventos = 0;
    }
}