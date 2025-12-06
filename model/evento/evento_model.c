#include "evento_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define EVENTOS_DATA_FILE "data/eventos.dat"
#define EVENTOS_TEXT_FILE "data/eventos.txt"

void remover_quebra_linha_evento(char *str) {
    str[strcspn(str, "\n")] = 0;
}

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
            fwrite(&e->codigo, sizeof(int), 1, f);
            fwrite(e->nome_evento, sizeof(char), 150, f);
            fwrite(&e->codigo_cliente, sizeof(int), 1, f);
            fwrite(&e->status, sizeof(StatusEvento), 1, f);
            fwrite(e->data_inicio, sizeof(char), 15, f);
            fwrite(e->hora_inicio, sizeof(char), 6, f); 
            fwrite(e->data_fim, sizeof(char), 15, f);
            fwrite(e->hora_fim, sizeof(char), 6, f);    
            fwrite(e->local, sizeof(char), 150, f);
            fwrite(&e->custo_total_previsto, sizeof(float), 1, f);
            fwrite(&e->valor_final_faturado, sizeof(float), 1, f);
            fwrite(&e->num_recursos_alocados, sizeof(int), 1, f);
            if(e->num_recursos_alocados > 0) fwrite(e->lista_recursos_alocados, sizeof(ItemRecursoEvento), e->num_recursos_alocados, f);
            fwrite(&e->num_equipe_alocada, sizeof(int), 1, f);
            if(e->num_equipe_alocada > 0) fwrite(e->lista_equipe_alocada, sizeof(ItemEquipeEvento), e->num_equipe_alocada, f);
            fwrite(&e->num_fornecedores_alocados, sizeof(int), 1, f);
            if(e->num_fornecedores_alocados > 0) fwrite(e->lista_fornecedores_alocados, sizeof(ItemFornecedorEvento), e->num_fornecedores_alocados, f);
        }
    } else {
        fprintf(f, "Numero de enventos: %d\n", sistema->num_eventos);
        for(int i=0; i<sistema->num_eventos; i++) {
            Evento *e = &sistema->lista_eventos[i];
            fprintf(f, " Codigo: %d\nNome do evento: %s\n Codigo do cliente: %d\nStatus: %d\nData de inicio: %s\n Hora de inicio: %s\n Data de termino: %s\n Hora de termino: %s\n Local do evento: %s\nValor do Orcamento: %.2f\nValor final:%.2f\n",
                e->codigo, e->nome_evento, e->codigo_cliente, e->status,
                e->data_inicio, e->hora_inicio, 
                e->data_fim, e->hora_fim,       
                e->local,
                e->custo_total_previsto, e->valor_final_faturado);
            
            fprintf(f, "Numero de recursos alocados: %d\n", e->num_recursos_alocados);
            for(int j=0; j<e->num_recursos_alocados; j++)
                fprintf(f, " Lista de recursos alocados: %d %d %.2f\n", e->lista_recursos_alocados[j].codigo_recurso, e->lista_recursos_alocados[j].quantidade, e->lista_recursos_alocados[j].custo_locacao_momento);
                
            fprintf(f, "Numero de equipe alocada: %d\n", e->num_equipe_alocada);
            for(int j=0; j<e->num_equipe_alocada; j++)
                fprintf(f, "Lista de equipe alocada: %d %.2f\n", e->lista_equipe_alocada[j].codigo_equipe, e->lista_equipe_alocada[j].custo_diaria_momento);

            fprintf(f, "Numero de fornecedores alocados: %d\n", e->num_fornecedores_alocados);
            for(int j=0; j<e->num_fornecedores_alocados; j++)
                fprintf(f, "Lista de fornecedores alocados: %d %.2f\n", e->lista_fornecedores_alocados[j].codigo_fornecedor, e->lista_fornecedores_alocados[j].valor_cobrado);
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

                fread(&e->num_fornecedores_alocados, sizeof(int), 1, f);
                if(e->num_fornecedores_alocados > 0) {
                    e->lista_fornecedores_alocados = malloc(e->num_fornecedores_alocados * sizeof(ItemFornecedorEvento));
                    fread(e->lista_fornecedores_alocados, sizeof(ItemFornecedorEvento), e->num_fornecedores_alocados, f);
                } else e->lista_fornecedores_alocados = NULL;
            }
        }
    } else {
        char linha[500]; char *valor;
        if(fgets(linha, sizeof(linha), f)) {
            valor = strchr(linha, ':'); if(valor) sistema->num_eventos = atoi(valor+1);
        }

        if(sistema->num_eventos > 0) {
            sistema->lista_eventos = malloc(sistema->num_eventos * sizeof(Evento));
            sistema->capacidade_eventos = sistema->num_eventos;

            for(int i=0; i<sistema->num_eventos; i++) {
                Evento *e = &sistema->lista_eventos[i];
                
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) e->codigo = atoi(valor+1);
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) { strcpy(e->nome_evento, valor+2); remover_quebra_linha_evento(e->nome_evento); }
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) e->codigo_cliente = atoi(valor+1);
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) e->status = atoi(valor+1);
                
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) { strcpy(e->data_inicio, valor+2); remover_quebra_linha_evento(e->data_inicio); }
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) { strcpy(e->hora_inicio, valor+2); remover_quebra_linha_evento(e->hora_inicio); }
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) { strcpy(e->data_fim, valor+2); remover_quebra_linha_evento(e->data_fim); }
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) { strcpy(e->hora_fim, valor+2); remover_quebra_linha_evento(e->hora_fim); }
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) { strcpy(e->local, valor+2); remover_quebra_linha_evento(e->local); }
                
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) e->custo_total_previsto = atof(valor+1);
                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) e->valor_final_faturado = atof(valor+1);

                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) e->num_recursos_alocados = atoi(valor+1);
                if(e->num_recursos_alocados > 0) {
                    e->lista_recursos_alocados = malloc(e->num_recursos_alocados * sizeof(ItemRecursoEvento));
                    for(int j=0; j<e->num_recursos_alocados; j++) {
                        fgets(linha, sizeof(linha), f); valor = strchr(linha, ':');
                        if(valor) sscanf(valor+1, "%d %d %f", &e->lista_recursos_alocados[j].codigo_recurso, &e->lista_recursos_alocados[j].quantidade, &e->lista_recursos_alocados[j].custo_locacao_momento);
                    }
                } else e->lista_recursos_alocados = NULL;

                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) e->num_equipe_alocada = atoi(valor+1);
                if(e->num_equipe_alocada > 0) {
                    e->lista_equipe_alocada = malloc(e->num_equipe_alocada * sizeof(ItemEquipeEvento));
                    for(int j=0; j<e->num_equipe_alocada; j++) {
                        fgets(linha, sizeof(linha), f); valor = strchr(linha, ':');
                        if(valor) sscanf(valor+1, "%d %f", &e->lista_equipe_alocada[j].codigo_equipe, &e->lista_equipe_alocada[j].custo_diaria_momento);
                    }
                } else e->lista_equipe_alocada = NULL;

                fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) e->num_fornecedores_alocados = atoi(valor+1);
                if(e->num_fornecedores_alocados > 0) {
                    e->lista_fornecedores_alocados = malloc(e->num_fornecedores_alocados * sizeof(ItemFornecedorEvento));
                    for(int j=0; j<e->num_fornecedores_alocados; j++) {
                        fgets(linha, sizeof(linha), f); valor = strchr(linha, ':');
                        if(valor) sscanf(valor+1, "%d %f", &e->lista_fornecedores_alocados[j].codigo_fornecedor, &e->lista_fornecedores_alocados[j].valor_cobrado);
                    }
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
            if (sistema->lista_eventos[i].lista_fornecedores_alocados)
                free(sistema->lista_eventos[i].lista_fornecedores_alocados);
        }
        free(sistema->lista_eventos);
        sistema->lista_eventos = NULL;
        sistema->num_eventos = 0;
        sistema->capacidade_eventos = 0;
    }
}