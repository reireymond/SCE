#include "controller/transacao/transacao_controller.h"
#include "model/recurso/recurso_model.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils/utils.h"
#include "utils/validation.h"

// Função auxiliar para registar transacao
void adicionarTransacao(Sistema *s, TipoTransacao tipo, char *desc, float valor, char *vencimento) {
    if (s->num_transacoes == s->capacidade_transacoes) {
        int nova = (s->capacidade_transacoes == 0) ? 10 : s->capacidade_transacoes * 2;
        s->lista_transacoes = realloc(s->lista_transacoes, nova * sizeof(Transacao));
        s->capacidade_transacoes = nova;
    }
    Transacao *t = &s->lista_transacoes[s->num_transacoes++];
    t->codigo = s->num_transacoes;
    t->tipo = tipo;
    t->status = PENDENTE;
    t->valor = valor;
    strcpy(t->descricao, desc);
    strcpy(t->data_vencimento, vencimento);
    strcpy(t->data_pagamento, ""); 
    salvarTransacoes(s);
}

// Função para registar a compra de equipamentos (Nota Fiscal)
// Isso atualiza o estoque e calcula o preço de aluguel sozinho
void lancarAquisicaoController(Sistema *sistema) {
    limpar_tela();
    printf("=== REGISTAR COMPRA (NOTA FISCAL) ===\n");

    float frete_total, imposto_total;
    int total_itens_nota;
    float valor_total_produtos = 0;
    
    // 1. Pede os totais da nota para fazer o rateio (divisão)
    printf("Valor TOTAL do Frete (R$): ");
    ler_float_positivo(&frete_total);

    printf("Valor TOTAL dos Impostos (R$): ");
    ler_float_positivo(&imposto_total);

    printf("Quantidade TOTAL de itens comprados: ");
    ler_int_valido(&total_itens_nota, 1, 10000);

    // Calcula quanto cada produto paga de frete e imposto
    float frete_por_unidade = frete_total / total_itens_nota;
    float imposto_por_unidade = imposto_total / total_itens_nota;

    // 2. Loop para adicionar os produtos da nota
    int continuar = 1;
    while (continuar == 1) {
        // Aumenta a memoria da lista de recursos se tiver cheia
        if (sistema->num_recursos == sistema->capacidade_recursos) {
            int nova_cap = (sistema->capacidade_recursos == 0) ? 10 : sistema->capacidade_recursos * 2;
            Recurso *temp = realloc(sistema->lista_recursos, nova_cap * sizeof(Recurso));
            if (temp == NULL) {
                printf("Erro: Sem memoria!\n");
                return;
            }
            sistema->lista_recursos = temp;
            sistema->capacidade_recursos = nova_cap;
        }

        // Pega o ponteiro para o novo recurso
        Recurso *novo = &sistema->lista_recursos[sistema->num_recursos];
        novo->codigo = sistema->num_recursos + 1;

        printf("\n--- Novo Item da Nota --- \n");
        printf("Descricao: ");
        ler_string_valida(novo->descricao, sizeof(novo->descricao), VALIDATE_NAO_VAZIA);

        printf("Categoria: ");
        ler_string_valida(novo->categoria, sizeof(novo->categoria), VALIDATE_NAO_VAZIA);

        printf("Quantidade comprada: ");
        ler_int_valido(&novo->quantidade_estoque, 1, 1000);

        printf("Preco de Custo Unitario (R$): ");
        ler_float_positivo(&novo->preco_custo);

        // X
        valor_total_produtos += (novo->preco_custo * novo->quantidade_estoque);

        // 3. CALCULO OBRIGATORIO DO PDF
        // Preço Locação = Custo + Frete Unit + Imposto Unit + Lucro
        float custo_final = novo->preco_custo + frete_por_unidade + imposto_por_unidade;
        
        
        // Pega a margem da produtora (se não tiver, usa 20%)
        float margem = 20.0;
        if (sistema->dados_produtora != NULL) {
            margem = sistema->dados_produtora->margem_lucro;
        }

        // Aplica a margem
        novo->valor_locacao = custo_final * (1 + (margem / 100.0));

        printf("--> Custo Final Calc.: R$ %.2f\n", custo_final);
        printf("--> Valor Locacao Calc.: R$ %.2f\n", novo->valor_locacao);

        // Salva o recurso no sistema
        sistema->num_recursos++;
        salvarRecursos(sistema); 

        printf("\nTem mais itens na nota? (1-Sim, 0-Nao): ");
        scanf("%d", &continuar);
        limpar_buffer();
    }

    // Cria a conta a pagar (Dívida)
    float total_final_nota = valor_total_produtos + frete_total + imposto_total;
    
    // Nota: Precisas de ter a função 'adicionarTransacao' (ver ponto 3 abaixo)
    adicionarTransacao(sistema, CONTA_A_PAGAR, "Compra de Equipamentos (Nota Fiscal)", total_final_nota, "30/12/2024");
    
    printf("\nTransacao de R$ %.2f registada no Contas a Pagar.\n", total_final_nota);
    printf("Compra finalizada e estoque atualizado!\n");
}

void gerenciarContasReceberController(Sistema *sistema) {
    limpar_tela();
    printf("=== CONTAS A RECEBER ===\n");
    printf("Saldo atual: R$ %.2f\n\n", sistema->saldo_caixa);

    for(int i=0; i < sistema->num_transacoes; i++) {
        Transacao *t = &sistema->lista_transacoes[i];
        if(t->tipo == CONTA_A_RECEBER) {
            printf("[%d] %s | R$ %.2f | Venc: %s | Status: %s\n", 
                   t->codigo, t->descricao, t->valor, t->data_vencimento, 
                   (t->status == PAGA ? "PAGO" : "PENDENTE"));
        }
    }

    int cod;
    printf("\nDigite o codigo para dar baixa (ou 0 para voltar): ");
    scanf("%d", &cod);
    if(cod == 0) return;

    // Procura e paga
    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod && sistema->lista_transacoes[i].tipo == CONTA_A_RECEBER) {
            if(sistema->lista_transacoes[i].status == PAGA) {
                printf("Esta conta ja foi paga!\n");
            } else {
                sistema->lista_transacoes[i].status = PAGA;
                sistema->saldo_caixa += sistema->lista_transacoes[i].valor; // Entra dinheiro
                printf("Recebimento confirmado! Novo saldo: %.2f\n", sistema->saldo_caixa);
                salvarTransacoes(sistema);
            }
            return;
        }
    }
    printf("Codigo nao encontrado.\n");
}

void gerenciarContasPagarController(Sistema *sistema) {
    limpar_tela();
    printf("=== CONTAS A PAGAR ===\n");
    printf("Saldo atual: R$ %.2f\n\n", sistema->saldo_caixa);

    for(int i=0; i < sistema->num_transacoes; i++) {
        Transacao *t = &sistema->lista_transacoes[i];
        if(t->tipo == CONTA_A_PAGAR) {
            printf("[%d] %s | R$ %.2f | Venc: %s | Status: %s\n", 
                   t->codigo, t->descricao, t->valor, t->data_vencimento, 
                   (t->status == PAGA ? "PAGO" : "PENDENTE"));
        }
    }

    int cod;
    printf("\nDigite o codigo para pagar (ou 0 para voltar): ");
    scanf("%d", &cod);
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod && sistema->lista_transacoes[i].tipo == CONTA_A_PAGAR) {
             if(sistema->lista_transacoes[i].status == PAGA) {
                printf("Esta conta ja foi paga!\n");
            } else {
                if (sistema->saldo_caixa >= sistema->lista_transacoes[i].valor) {
                    sistema->lista_transacoes[i].status = PAGA;
                    sistema->saldo_caixa -= sistema->lista_transacoes[i].valor; // Sai dinheiro
                    printf("Pagamento realizado! Novo saldo: %.2f\n", sistema->saldo_caixa);
                    salvarTransacoes(sistema);
                } else {
                    printf("Erro: Saldo insuficiente no caixa!\n");
                }
            }
            return;
        }
    }
    printf("Codigo nao encontrado.\n");
}

void verSaldoCaixaController(Sistema *sistema) {
    printf("\nSaldo atual do caixa: R$ %.2f\n", sistema->saldo_caixa);
}

// Função auxiliar necessária para o passo 1
void adicionarTransacao(Sistema *s, TipoTransacao tipo, char *desc, float valor, char *venc) {
    // Expande memória se necessário (igual fazes nos recursos)
    if (s->num_transacoes == s->capacidade_transacoes) {
        int nova = (s->capacidade_transacoes == 0) ? 10 : s->capacidade_transacoes * 2;
        s->lista_transacoes = realloc(s->lista_transacoes, nova * sizeof(Transacao));
        s->capacidade_transacoes = nova;
    }
    
    Transacao *t = &s->lista_transacoes[s->num_transacoes];
    t->codigo = s->num_transacoes + 1;
    t->tipo = tipo;
    t->status = PENDENTE;
    t->valor = valor;
    strcpy(t->descricao, desc);
    strcpy(t->data_vencimento, venc);
    
    s->num_transacoes++;
    salvarTransacoes(s);
}

void verSaldoCaixaController(Sistema *sistema) {
    limpar_tela();
    printf("=== FLUXO DE CAIXA ===\n");
    printf("Saldo Atual Disponivel: R$ %.2f\n", sistema->saldo_caixa);
    pausar();
}

void gerenciarContasPagarController(Sistema *sistema) {
    limpar_tela();
    printf("=== PAGAR CONTAS ===\n");
    // Listar contas pendentes
    for(int i=0; i < sistema->num_transacoes; i++) {
        Transacao *t = &sistema->lista_transacoes[i];
        if (t->tipo == CONTA_A_PAGAR && t->status == PENDENTE) {
            printf("[%d] %s - Valor: R$ %.2f (Venc: %s)\n", t->codigo, t->descricao, t->valor, t->data_vencimento);
        }
    }
    
    int cod;
    printf("\nDigite o codigo da conta para pagar (0 para sair): ");
    scanf("%d", &cod);
    if (cod == 0) return;

    // Lógica de pagamento
    for(int i=0; i < sistema->num_transacoes; i++) {
        if (sistema->lista_transacoes[i].codigo == cod) {
            if (sistema->saldo_caixa >= sistema->lista_transacoes[i].valor) {
                sistema->saldo_caixa -= sistema->lista_transacoes[i].valor; // Tira do caixa
                sistema->lista_transacoes[i].status = PAGA; // Marca como paga
                printf("Conta paga com sucesso!\n");
                salvarTransacoes(sistema);
            } else {
                printf("Erro: Saldo insuficiente!\n");
            }
            return;
        }
    }
}