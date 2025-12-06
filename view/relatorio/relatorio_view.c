#include "relatorio_view.h"
#include <stdio.h>
#include <string.h>
#include "utils/utils.h"
#include "utils/validation.h"

int menuRelatoriosView() {
    int op;
    limpar_tela();
    printf("+=====================================================+\n");
    printf("|                 MODULO DE RELATORIOS                |\n");
    printf("+=====================================================+\n");
    printf("| [1] Listagem de Clientes                            |\n");
    printf("| [2] Listagem de Eventos                             |\n");
    printf("| [3] Cronograma de Recursos                          |\n");
    printf("| [4] Listagem de Equipamentos                        |\n");
    printf("| [5] Contas a Receber                                |\n");
    printf("| [6] Contas a Pagar                                  |\n");
    printf("| [7] Movimentacao de Caixa                           |\n");
    printf("+-----------------------------------------------------+\n");
    printf("| [0] Voltar                                          |\n");
    printf("+=====================================================+\n");
    printf("Escolha uma opcao: ");
    
    scanf("%d", &op);
    limpar_buffer();
    
    return op;
}

int perguntarTipoSaida() {
    int op;
    printf("\nOnde deseja gerar o relatorio?\n");
    printf("[1] Imprimir na Tela\n");
    printf("[2] Salvar em Arquivo (.csv)\n");
    printf("Opcao: ");
    ler_inteiro_valido(&op, 1, 2);
    return op;
}

//filtros
void form_filtro_cliente(int *min, int *max, int *ordem) {
    printf("\nFiltros de Cliente\n");
    printf("Codigo Inicial (0 para todos): "); ler_inteiro_valido(min, 0, 999999);
    printf("Codigo Final   (0 para todos): "); ler_inteiro_valido(max, 0, 999999);
    printf("Ordenar por nome? (1-Sim, 0-Nao): "); ler_inteiro_valido(ordem, 0, 1);
}

void form_filtro_eventos(int *cod_cliente, char *data_ini, char *data_fim, int *status_op) {
    printf("\nFiltros de Evento\n");
    printf("ID do Cliente (0 para todos): "); ler_inteiro_valido(cod_cliente, 0, 999999);
    printf("Data Inicio (DD/MM/AAAA) ou '0' para ignorar: "); 
    ler_texto_valido(data_ini, 15, VALIDAR_NAO_VAZIO);
    printf("Data Fim    (DD/MM/AAAA) ou '0' para ignorar: "); 
    ler_texto_valido(data_fim, 15, VALIDAR_NAO_VAZIO);
    
    printf("Status (1-Orcamento, 2-Aprovado, 3-Finalizado, 0-Todos): ");
    ler_inteiro_valido(status_op, 0, 3);
}

void form_filtro_cronograma(int *cod_recurso, char *data_ini, char *data_fim) {
    printf("\nFiltros de Cronograma\n");
    printf("ID do Recurso (0 para todos): "); ler_inteiro_valido(cod_recurso, 0, 999999);
    printf("Data Inicio (DD/MM/AAAA) ou '0' para ignorar: "); 
    ler_texto_valido(data_ini, 15, VALIDAR_NAO_VAZIO);
    printf("Data Fim    (DD/MM/AAAA) ou '0' para ignorar: "); 
    ler_texto_valido(data_fim, 15, VALIDAR_NAO_VAZIO);
}

void form_filtro_equipamentos(int *min, int *max, char *cat) {
    printf("\nFiltros de Equipamentos\n");
    printf("Codigo Inicial (0 para todos): "); ler_inteiro_valido(min, 0, 999999);
    printf("Codigo Final   (0 para todos): "); ler_inteiro_valido(max, 0, 999999);
    printf("Categoria (digite '0' para todas): "); 
    ler_texto_valido(cat, 50, VALIDAR_NAO_VAZIO);
}

void form_filtro_financeiro(int *cod_entidade, char *data_ini, char *data_fim) {
    printf("\nFiltros Financeiros\n");
    printf("ID da Pessoa/Empresa (0 para todos):"); ler_inteiro_valido(cod_entidade, 0, 999999);
    printf("Data Vencimento Inicial (0 para ignorar):"); ler_texto_valido(data_ini, 15, VALIDAR_NAO_VAZIO);
    printf("Data Vencimento Final   (0 para ignorar):"); ler_texto_valido(data_fim, 15, VALIDAR_NAO_VAZIO);
}

// aparece na tela
void cabecalho_cliente() {
    printf("\n%-5s | %-30s | %-15s | %-20s\n", "ID", "Nome", "CPF/CNPJ", "Telefone");
    printf("------------------------------------------------------------------------------\n");
}
void linha_cliente(Cliente *c) {
    printf("%-5d | %-30s | %-15s | %-20s\n", c->codigo, c->razao_social, c->cnpj, c->telefone);
}

void cabecalho_evento() {
    printf("\n%-5s | %-20s | %-10s | %-12s | %-10s\n", "ID", "Evento", "Data", "Status", "Valor(R$)");
    printf("------------------------------------------------------------------------------\n");
}
void linha_evento(Evento *e) {
    char st[15];
    if(e->status == ORCAMENTO) strcpy(st, "Orcamento");
    else if(e->status == APROVADO) strcpy(st, "Aprovado");
    else if(e->status == FINALIZADO) strcpy(st, "Finalizado");
    else strcpy(st, "Cancelado");

    printf("%-5d | %-20s | %-10s | %-12s | %-10.2f\n", e->codigo, e->nome_evento, e->data_inicio, st, e->custo_total_previsto);
}

void cabecalho_cronograma() {
    printf("\n%-20s | %-10s | %-5s | %-20s\n", "Recurso", "Data", "Hora", "Evento Alocado");
    printf("------------------------------------------------------------------\n");
}
void linha_cronograma(char *nome_recurso, char *data, char *hora, char *evento_nome) {
    printf("%-20s | %-10s | %-5s | %-20s\n", nome_recurso, data, hora, evento_nome);
}

void cabecalho_equipamento() {
    printf("\n%-5s | %-20s | %-15s | %-5s | %-10s\n", "ID", "Descricao", "Categoria", "Qtd", "Preco");
    printf("----------------------------------------------------------------------\n");
}
void linha_equipamento(Recurso *r) {
    printf("%-5d | %-20s | %-15s | %-5d | %-10.2f\n", r->codigo, r->descricao, r->categoria, r->quantidade_estoque, r->valor_locacao);
}

void cabecalho_financeiro() {
    printf("\n%-5s | %-10s | %-10s | %-12s | %-25s\n", "ID", "Vencimento", "Valor", "Status", "Descricao");
    printf("--------------------------------------------------------------------------\n");
}
void linha_financeiro(Transacao *t) {
    char st[10];
    if(t->status == PAGA) strcpy(st, "PAGO"); else strcpy(st, "PENDENTE");
    printf("%-5d | %-10s | %-10.2f | %-12s | %-25s\n", t->codigo, t->data_vencimento, t->valor, st, t->descricao);
}

void msg_relatorio_concluido() {
    printf("\n[FIM] Relatorio exibido com sucesso.\n");
    pausar();
}
void msg_arquivo_salvo(char *nome) {
    printf("\n[SUCESSO] Arquivo gerado: %s\n", nome);
    pausar();
}
void msg_erro_arquivo() {
    printf("\n[ERRO] Nao foi possivel criar o arquivo.\n");
    pausar();
}
void msg_opcao_invalida() {
    printf("\nOpcao invalida.\n");
    pausar();
}