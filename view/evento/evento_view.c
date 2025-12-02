#include "evento_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "utils/validation.h"
#include "controller/evento/evento_controller.h"

void menuEventosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|           GESTAO DE EVENTOS E ORCAMENTOS            |\n");
        printf("+=====================================================+\n");
        printf("| [1] Criar Novo Orcamento                            |\n");
        printf("| [2] Aprovar Evento (Verificar Estoque)              |\n");
        printf("| [3] Finalizar Evento (Faturamento)                  |\n");
        printf("| [4] Listar Eventos                                  |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        ler_inteiro_valido(&opcao, 0, 4);

        switch (opcao) {
            case 1: adicionarEventoController(sistema); break;
            case 2: alterarStatusEventoController(sistema); break;
            case 3: finalizarEventoController(sistema); break;
            case 4: listarEventosView(sistema); break;
            case 0: break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}
void listarEventosView(Sistema *sistema) {
    if (sistema->num_eventos == 0) {
        printf("\nNenhum evento cadastrado.\n"); 
        return;
    }
    printf("\n--- Lista de Eventos ---\n");
    for (int i = 0; i < sistema->num_eventos; i++) {
        Evento *e = &sistema->lista_eventos[i];
        char status_texto[20];
        
        if(e->status == ORCAMENTO) sprintf(status_texto, "Orcamento");
        else if(e->status == APROVADO) sprintf(status_texto, "Aprovado");
        else sprintf(status_texto, "Finalizado");
        
        printf("#%d | %s \n", e->codigo, e->nome_evento);
        printf("    Cliente ID: %d\n", e->codigo_cliente);
        printf("    Inicio: %s as %s\n", e->data_inicio, e->hora_inicio);
        printf("    Fim:    %s as %s\n", e->data_fim, e->hora_fim);
        printf("    Status: %s\n", status_texto);
        printf("    Previsto: R$ %.2f\n", e->custo_total_previsto);
        printf("------------------------------------------------------\n");
    }
}

void formulario_novo_evento_basico(Evento *e) {
    printf("\n--- Novo Evento (ID: %d) ---\n", e->codigo);
    
    printf("Nome do Evento: "); 
    ler_texto_valido(e->nome_evento, sizeof(e->nome_evento), VALIDAR_NAO_VAZIO);
    
    printf("ID do Cliente: "); 
    ler_inteiro_valido(&e->codigo_cliente, 1, 999999);
    
    printf("Data Inicio (DD/MM/AAAA): "); 
    ler_texto_valido(e->data_inicio, sizeof(e->data_inicio), VALIDAR_DATA);
    
    printf("Hora Inicio (HH:MM): "); 
    ler_texto_valido(e->hora_inicio, sizeof(e->hora_inicio), VALIDAR_NAO_VAZIO);
    
    printf("Data Fim (DD/MM/AAAA): "); 
    ler_texto_valido(e->data_fim, sizeof(e->data_fim), VALIDAR_DATA);
    
    printf("Hora Fim (HH:MM): "); 
    ler_texto_valido(e->hora_fim, sizeof(e->hora_fim), VALIDAR_NAO_VAZIO);
    
    printf("Local: "); 
    ler_texto_valido(e->local, sizeof(e->local), VALIDAR_NAO_VAZIO);
}

//recursos
int perguntar_se_adiciona_recurso() {
    int op;
    printf("\nAdicionar Equipamento/Recurso? (1-Sim, 0-Nao): ");
    ler_inteiro_valido(&op, 0, 1);
    return op;
}

void formulario_adicionar_recurso(int *cod, int *qtd) {
    printf("Digite o ID do Equipamento: ");
    ler_inteiro_valido(cod, 1, 999999);
    
    printf("Digite a Quantidade: ");
    ler_inteiro_valido(qtd, 1, 9999);
}

//equipe interna
int perguntar_se_adiciona_equipe() {
    int op;
    printf("\nAdicionar Membro da Equipe? (1-Sim, 0-Nao): ");
    ler_inteiro_valido(&op, 0, 1);
    return op;
}

void formulario_adicionar_equipe(int *cod) {
    printf("Digite o ID do Funcionario: ");
    ler_inteiro_valido(cod, 1, 999999);
}

// fornecedo
int perguntar_se_adiciona_fornecedor() {
    int op;
    printf("\nAdicionar Servico de Fornecedor Externo? (1-Sim, 0-Nao): ");
    ler_inteiro_valido(&op, 0, 1);
    return op;
}

void formulario_adicionar_fornecedor(int *cod) {
    printf("Digite o ID do Fornecedor: ");
    ler_inteiro_valido(cod, 1, 999999);
}

int pedir_id_evento(const char *acao) {
    int id;
    printf("\nDigite o ID do Evento para %s: ", acao);
    ler_inteiro_valido(&id, 1, 999999);
    return id;
}

void formulario_finalizar_evento(float *valor_final) {
    printf("\n--- Finalizando Evento ---\n");
    printf("Informe o Valor Final Faturado (Real): R$ ");
    ler_float_positivo(valor_final);
}

void msg_fornecedor_adicionado_sucesso(char *nome_fornecedor, float custo) {
    printf(">> Fornecedor '%s' adicionado! Custo: R$ %.2f\n", nome_fornecedor, custo);
}

void msg_recurso_adicionado() {
    printf(">> Item adicionado ao orcamento!\n");
}

void msg_recurso_nao_encontrado() {
    printf(">> ERRO: ID nao encontrado. Tente novamente.\n");
}

void msg_conflito_estoque(int cod, int total, int usado) {
    printf("\n[ALERTA] Conflito de Estoque!\n");
    printf("Recurso ID %d nao tem saldo suficiente nesse horario.\n", cod);
    printf("Total: %d | Ja reservado para outros eventos: %d\n", total, usado);
}

void msg_evento_aprovado() {
    printf("\n[SUCESSO] Estoque verificado e Evento APROVADO!\n");
    pausar();
}

