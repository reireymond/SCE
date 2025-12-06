#include "exportacao_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exportacao/exportacao_view.h"
#include "utils/utils.h"

//pega o texto
void extrair_dado(char *linha, char *destino) {
    char *inicio = strchr(linha, '>');
    char *fim = strrchr(linha, '<');
    
    if (inicio && fim && fim > inicio) {
        inicio++;
        int tamanho = fim - inicio;
        strncpy(destino, inicio, tamanho);
        destino[tamanho] = '\0'; // Fecha string
    } else {
        strcpy(destino, "");
    }
}

//exportacao

void exportar_clientes(Sistema *s, FILE *f) {
    fprintf(f, "<tabela-cliente>\n");
    for(int i=0; i < s->num_clientes; i++) {
        Cliente *c = &s->lista_clientes[i];
        fprintf(f, "  <registro>\n");
        fprintf(f, "    <codigo>%d</codigo>\n", c->codigo);
        fprintf(f, "    <razao_social>%s</razao_social>\n", c->razao_social);
        fprintf(f, "    <endereco>%s</endereco>\n", c->endereco);
        fprintf(f, "    <cnpj>%s</cnpj>\n", c->cnpj);
        fprintf(f, "    <telefone>%s</telefone>\n", c->telefone);
        fprintf(f, "    <email>%s</email>\n", c->email);
        fprintf(f, "    <nome_contato>%s</nome_contato>\n", c->nome_do_contato);
        fprintf(f, "  </registro>\n");
    }
    fprintf(f, "</tabela-cliente>\n");
}

void exportar_equipamentos(Sistema *s, FILE *f) {
    fprintf(f, "<tabela-equipamento>\n");
    for(int i=0; i < s->num_recursos; i++) {
        Recurso *r = &s->lista_recursos[i];
        fprintf(f, "  <registro>\n");
        fprintf(f, "    <codigo>%d</codigo>\n", r->codigo);
        fprintf(f, "    <descricao>%s</descricao>\n", r->descricao);
        fprintf(f, "    <categoria>%s</categoria>\n", r->categoria);
        fprintf(f, "    <quantidade_estoque>%d</quantidade_estoque>\n", r->quantidade_estoque);
        fprintf(f, "    <preco_custo>%.2f</preco_custo>\n", r->preco_custo);
        fprintf(f, "    <valor_locacao>%.2f</valor_locacao>\n", r->valor_locacao);
        fprintf(f, "  </registro>\n");
    }
    fprintf(f, "</tabela-equipamento>\n");
}

void exportar_eventos(Sistema *s, FILE *f) {
    fprintf(f, "<tabela-evento>\n");
    for(int i=0; i < s->num_eventos; i++) {
        Evento *e = &s->lista_eventos[i];
        char st[20];
        if(e->status == ORCAMENTO) strcpy(st, "Orcamento");
        else if(e->status == APROVADO) strcpy(st, "Aprovado");
        else strcpy(st, "Finalizado");

        fprintf(f, "  <registro>\n");
        fprintf(f, "    <codigo_evento>%d</codigo_evento>\n", e->codigo); // XML pede codigo_evento
        fprintf(f, "    <nome_evento>%s</nome_evento>\n", e->nome_evento);
        fprintf(f, "    <codigo_cliente>%d</codigo_cliente>\n", e->codigo_cliente);
        fprintf(f, "    <status>%s</status>\n", st);
        fprintf(f, "  </registro>\n");
    }
    fprintf(f, "</tabela-evento>\n");
}

//importacao
void importar_clientes(Sistema *s, FILE *f) {
    char linha[200], dado[100];
    Cliente temp;
    int lendo = 0;
    int cont = 0;

    while(fgets(linha, sizeof(linha), f)) {
        if(strstr(linha, "<registro>")) {
            memset(&temp, 0, sizeof(Cliente)); // Limpa lixo
            lendo = 1;
        }
        if(lendo) {
            if(strstr(linha, "<codigo>")) { 
                extrair_dado(linha, dado); temp.codigo = atoi(dado); 
            }
            if(strstr(linha, "<razao_social>")) { 
                extrair_dado(linha, temp.razao_social); 
            }
            if(strstr(linha, "<endereco>")) { 
                extrair_dado(linha, temp.endereco); 
            }
            if(strstr(linha, "<cnpj>")) { 
                extrair_dado(linha, temp.cnpj); 
            }
            if(strstr(linha, "<telefone>")) { 
                extrair_dado(linha, temp.telefone); 
            }
            if(strstr(linha, "<email>")) { 
                extrair_dado(linha, temp.email); 
            }
            if(strstr(linha, "<nome_contato>")) { 
                extrair_dado(linha, temp.nome_do_contato); 
            }
        }
        if(strstr(linha, "</registro>")) {
            // salva no sistema
            lendo = 0;
            
            // aumenta vetor
            if (s->num_clientes == s->capacidade_clientes) {
                int nova = (s->capacidade_clientes == 0) ? 10 : s->capacidade_clientes * 2;
                s->lista_clientes = realloc(s->lista_clientes, nova * sizeof(Cliente));
                s->capacidade_clientes = nova;
            }
            s->lista_clientes[s->num_clientes] = temp;
            s->num_clientes++;
            cont++;
        }
    }
    msg_importacao_sucesso(cont);
}

void importar_equipamentos(Sistema *s, FILE *f) {
    char linha[200], dado[100];
    Recurso temp;
    int lendo = 0;
    int cont = 0;

    while(fgets(linha, sizeof(linha), f)) {
        if(strstr(linha, "<registro>")) {
            memset(&temp, 0, sizeof(Recurso));
            lendo = 1;
        }
        if(lendo) {
            if(strstr(linha, "<codigo>")) { 
                extrair_dado(linha, dado); temp.codigo = atoi(dado); 
            }
            if(strstr(linha, "<descricao>")) { 
                extrair_dado(linha, temp.descricao); 
            }
            if(strstr(linha, "<categoria>")) { 
                extrair_dado(linha, temp.categoria); 
            }
            if(strstr(linha, "<quantidade_estoque>")) { 
                extrair_dado(linha, dado); temp.quantidade_estoque = atoi(dado); 
            }
            if(strstr(linha, "<preco_custo>")) { 
                extrair_dado(linha, dado); temp.preco_custo = atof(dado); 
            }
            if(strstr(linha, "<valor_locacao>")) { 
                extrair_dado(linha, dado); temp.valor_locacao = atof(dado); 
            }
        }
        if(strstr(linha, "</registro>")) {
            lendo = 0;
            if (s->num_recursos == s->capacidade_recursos) {
                int nova = (s->capacidade_recursos == 0) ? 10 : s->capacidade_recursos * 2;
                s->lista_recursos = realloc(s->lista_recursos, nova * sizeof(Recurso));
                s->capacidade_recursos = nova;
            }
            s->lista_recursos[s->num_recursos] = temp;
            s->num_recursos++;
            cont++;
        }
    }
    msg_importacao_sucesso(cont);
}

void importar_eventos(Sistema *s, FILE *f) {
    char linha[200], dado[100];
    Evento temp;
    int lendo = 0;
    int cont = 0;

    while(fgets(linha, sizeof(linha), f)) {
        if(strstr(linha, "<registro>")) {
            memset(&temp, 0, sizeof(Evento));
            lendo = 1;
        }
        if(lendo) {
            if(strstr(linha, "<codigo_evento>")) { 
                extrair_dado(linha, dado); temp.codigo = atoi(dado); 
            }
            if(strstr(linha, "<nome_evento>")) { 
                extrair_dado(linha, temp.nome_evento); 
            }
            if(strstr(linha, "<codigo_cliente>")) { 
                extrair_dado(linha, dado); temp.codigo_cliente = atoi(dado); 
            }
            if(strstr(linha, "<status>")) { 
                extrair_dado(linha, dado);
                if(strcmp(dado, "Aprovado") == 0) temp.status = APROVADO;
                else if(strcmp(dado, "Finalizado") == 0) temp.status = FINALIZADO;
                else temp.status = ORCAMENTO;
            }
        }
        if(strstr(linha, "</registro>")) {
            lendo = 0;
            if (s->num_eventos == s->capacidade_eventos) {
                int nova = (s->capacidade_eventos == 0) ? 10 : s->capacidade_eventos * 2;
                s->lista_eventos = realloc(s->lista_eventos, nova * sizeof(Evento));
                s->capacidade_eventos = nova;
            }
            s->lista_eventos[s->num_eventos] = temp;
            s->num_eventos++;
            cont++;
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
        pedirNomeArquivo(arquivo);

        if (op == 1) { 
            msg_gravando_arquivo();//exporta
            FILE *f = fopen(arquivo, "w");
            if (!f) { msg_erro_arquivo_xml(); continue; }

            fprintf(f, "<dados>\n");
            if(tabela == 1) exportar_clientes(sistema, f);
            if(tabela == 2) exportar_equipamentos(sistema, f);
            if(tabela == 3) exportar_eventos(sistema, f);
            fprintf(f, "</dados>\n");

            fclose(f);
            msg_exportacao_sucesso(arquivo);
        }
        else if (op == 2) { //importa
            msg_lendo_arquivo();
            FILE *f = fopen(arquivo, "r");
            if (!f) { msg_erro_arquivo_xml(); continue; }

            if(tabela == 1) importar_clientes(sistema, f);
            if(tabela == 2) importar_equipamentos(sistema, f);
            if(tabela == 3) importar_eventos(sistema, f);
            
            fclose(f);

        }

    } while (op != 0);
}