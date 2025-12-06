#include "exportacao_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "view/exportacao/exportacao_view.h"
#include "utils/utils.h"

int ler_campo(char *linha, char *rotulo, char *destino) {
    // ve se a linha comeca com o  que quero
    if (strncmp(linha, rotulo, strlen(rotulo)) == 0) {
        strcpy(destino, linha + strlen(rotulo));
        
        // tira o enter do final
        destino[strcspn(destino, "\n")] = 0;
        return 1;
    }
    return 0;
}

//exporta

void exportar_clientes(Sistema *s, FILE *f) {
    // escreve os clientes no arquivo
    fprintf(f, "LISTA DE CLIENTES\n");
    for(int i=0; i < s->num_clientes; i++) {
        Cliente *c = &s->lista_clientes[i];
        fprintf(f, "REGISTRO\n");
        fprintf(f, "Codigo: %d\n", c->codigo);
        fprintf(f, "Razao Social: %s\n", c->razao_social);
        fprintf(f, "Endereco: %s\n", c->endereco);
        fprintf(f, "CNPJ: %s\n", c->cnpj);
        fprintf(f, "Telefone: %s\n", c->telefone);
        fprintf(f, "Email: %s\n", c->email);
        fprintf(f, "Contato: %s\n", c->nome_do_contato);
    }
    fprintf(f, "FIM\n");
}

void exportar_equipamentos(Sistema *s, FILE *f) {
    fprintf(f, "LISTA DE EQUIPAMENTOS\n");
    for(int i=0; i < s->num_recursos; i++) {
        Recurso *r = &s->lista_recursos[i];
        fprintf(f, "--- REGISTRO ---\n");
        fprintf(f, "Codigo: %d\n", r->codigo);
        fprintf(f, "Descricao: %s\n", r->descricao);
        fprintf(f, "Categoria: %s\n", r->categoria);
        fprintf(f, "Estoque: %d\n", r->quantidade_estoque);
        fprintf(f, "Custo: %.2f\n", r->preco_custo);
        fprintf(f, "Locacao: %.2f\n", r->valor_locacao);
    }
    fprintf(f, "FIM\n");
}

void exportar_eventos(Sistema *s, FILE *f) {
    fprintf(f, "--- LISTA DE EVENTOS ---\n");
    for(int i=0; i < s->num_eventos; i++) {
        Evento *e = &s->lista_eventos[i];
        
        // arruma o status pra string
        char st[20];
        if(e->status == ORCAMENTO) strcpy(st, "Orcamento");
        else if(e->status == APROVADO) strcpy(st, "Aprovado");
        else strcpy(st, "Finalizado");

        fprintf(f, "--- REGISTRO ---\n");
        fprintf(f, "Codigo: %d\n", e->codigo);
        fprintf(f, "Evento: %s\n", e->nome_evento);
        fprintf(f, "Cliente ID: %d\n", e->codigo_cliente);
        fprintf(f, "Status: %s\n", st);
    }
    fprintf(f, "--- FIM ---\n");
}

//importar

void importar_clientes(Sistema *s, FILE *f) {
    char linha[200], valor[100];
    Cliente temp;
    int lendo = 0; 
    int cont = 0;

    while(fgets(linha, sizeof(linha), f)) {
        // tira o enter
        linha[strcspn(linha, "\n")] = 0;

        // comeca a ler um novo
        if(strcmp(linha, "REGISTRO") == 0) {
            memset(&temp, 0, sizeof(Cliente));
            lendo = 1;
            continue;
        }

        if(lendo) {
            // pegar os dados
            if(ler_campo(linha, "Codigo: ", valor)) temp.codigo = atoi(valor);
            else if(ler_campo(linha, "Razao Social: ", temp.razao_social));
            else if(ler_campo(linha, "Endereco: ", temp.endereco));
            else if(ler_campo(linha, "CNPJ: ", temp.cnpj));
            else if(ler_campo(linha, "Telefone: ", temp.telefone));
            else if(ler_campo(linha, "Email: ", temp.email));
            else if(ler_campo(linha, "Contato: ", temp.nome_do_contato));
            
            // salva se acabou
            else if(strcmp(linha, "REGISTRO") == 0 || strcmp(linha, "FIM") == 0) {
                
                // aumenta o espaco se precisar
                if (s->num_clientes == s->capacidade_clientes) {
                    int nova = (s->capacidade_clientes == 0) ? 10 : s->capacidade_clientes * 2;
                    s->lista_clientes = realloc(s->lista_clientes, nova * sizeof(Cliente));
                    s->capacidade_clientes = nova;
                }
                s->lista_clientes[s->num_clientes] = temp;
                s->num_clientes++;
                cont++;
                
                // limpa pra ler o proximo se for o caso
                if(strcmp(linha, "REGISTRO") == 0) {
                    memset(&temp, 0, sizeof(Cliente));
                    lendo = 1;
                } else {
                    lendo = 0;
                }
            }
        }
    }
    msg_importacao_sucesso(cont);
}

void importar_equipamentos(Sistema *s, FILE *f) {
    char linha[200], valor[100];
    Recurso temp;
    int lendo = 0;
    int cont = 0;

    while(fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = 0;

        if(strcmp(linha, "REGISTRO") == 0) {
            memset(&temp, 0, sizeof(Recurso));
            lendo = 1;
            continue;
        }

        if(lendo) {
            if(ler_campo(linha, "Codigo: ", valor)) temp.codigo = atoi(valor);
            else if(ler_campo(linha, "Descricao: ", temp.descricao));
            else if(ler_campo(linha, "Categoria: ", temp.categoria));
            else if(ler_campo(linha, "Estoque: ", valor)) temp.quantidade_estoque = atoi(valor);
            else if(ler_campo(linha, "Custo: ", valor)) temp.preco_custo = atof(valor);
            else if(ler_campo(linha, "Locacao: ", valor)) temp.valor_locacao = atof(valor);
            
            else if(strcmp(linha, "REGISTRO") == 0 || strcmp(linha, "FIM") == 0) {
                // aumenta memoria
                if (s->num_recursos == s->capacidade_recursos) {
                    int nova = (s->capacidade_recursos == 0) ? 10 : s->capacidade_recursos * 2;
                    s->lista_recursos = realloc(s->lista_recursos, nova * sizeof(Recurso));
                    s->capacidade_recursos = nova;
                }
                s->lista_recursos[s->num_recursos] = temp;
                s->num_recursos++;
                cont++;

                if(strcmp(linha, "REGISTRO") == 0) {
                    memset(&temp, 0, sizeof(Recurso));
                    lendo = 1;
                } else lendo = 0;
            }
        }
    }
    msg_importacao_sucesso(cont);
}

void importar_eventos(Sistema *s, FILE *f) {
    char linha[200], valor[100];
    Evento temp;
    int lendo = 0;
    int cont = 0;

    while(fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = 0;

        if(strcmp(linha, "--- REGISTRO ---") == 0) {
            memset(&temp, 0, sizeof(Evento));
            lendo = 1;
            continue;
        }

        if(lendo) {
            if(ler_campo(linha, "Codigo: ", valor)) temp.codigo = atoi(valor);
            else if(ler_campo(linha, "Evento: ", temp.nome_evento));
            else if(ler_campo(linha, "Cliente ID: ", valor)) temp.codigo_cliente = atoi(valor);
            else if(ler_campo(linha, "Status: ", valor)) {
                if(strcmp(valor, "Aprovado") == 0) temp.status = APROVADO;
                else if(strcmp(valor, "Finalizado") == 0) temp.status = FINALIZADO;
                else temp.status = ORCAMENTO;
            }
            
            else if(strcmp(linha, " REGISTRO") == 0 || strcmp(linha, "FIM") == 0) {
                // aumenta vetor
                if (s->num_eventos == s->capacidade_eventos) {
                    int nova = (s->capacidade_eventos == 0) ? 10 : s->capacidade_eventos * 2;
                    s->lista_eventos = realloc(s->lista_eventos, nova * sizeof(Evento));
                    s->capacidade_eventos = nova;
                }
                s->lista_eventos[s->num_eventos] = temp;
                s->num_eventos++;
                cont++;

                if(strcmp(linha, "REGISTRO") == 0) {
                    memset(&temp, 0, sizeof(Evento));
                    lendo = 1;
                } else lendo = 0;
            }
        }
    }
    msg_importacao_sucesso(cont);
}

void menuXMLController(Sistema *sistema) {
    int op = 0;
    do {
        menuXMLView();
        op = obterOpcaoXML();
        
        if (op == 0) return;

        int tabela = menuEscolherTabela();
        if (tabela == 0) continue;

        char arquivo[50];
        // escolhe o nome do arquivo
        if (tabela == 1) strcpy(arquivo, "clientes_export.txt");
        else if (tabela == 2) strcpy(arquivo, "equipamentos_export.txt");
        else strcpy(arquivo, "eventos_export.txt");

        if (op == 1) { 
            msg_gravando_arquivo(arquivo);
            FILE *f = fopen(arquivo, "w"); // abre pra escrever
            if (!f) { msg_erro_arquivo_xml(arquivo); continue; }

            if(tabela == 1) exportar_clientes(sistema, f);
            if(tabela == 2) exportar_equipamentos(sistema, f);
            if(tabela == 3) exportar_eventos(sistema, f);

            fclose(f);
            msg_exportacao_sucesso(arquivo);
        }
        else if (op == 2) { 
            msg_lendo_arquivo(arquivo);
            FILE *f = fopen(arquivo, "r"); // abre pra ler
            if (!f) { msg_erro_arquivo_xml(arquivo); continue; }

            if(tabela == 1) importar_clientes(sistema, f);
            if(tabela == 2) importar_equipamentos(sistema, f);
            if(tabela == 3) importar_eventos(sistema, f);
            
            fclose(f);
        }

    } while (op != 0);
}
