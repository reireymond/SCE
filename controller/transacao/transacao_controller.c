#include "controller/transacao/transacao_controller.h"
#include "model/recurso/recurso_model.h"
#include "model/transacao/transacao_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"
#include "utils/validation.h"

// Função genérica para salvar qualquer transação no sistema
void registrarTransacao(Sistema *sistema, Transacao t) {
    if (sistema->num_transacoes == sistema->capacidade_transacoes) {
        int nova_cap = (sistema->capacidade_transacoes == 0) ? 10 : sistema->capacidade_transacoes * 2;
        sistema->lista_transacoes = realloc(sistema->lista_transacoes, nova_cap * sizeof(Transacao));
        sistema->capacidade_transacoes = nova_cap;
    }

    // Define o código sequencial
    t.codigo = sistema->num_transacoes + 1;
    
    // Adiciona à lista
    sistema->lista_transacoes[sistema->num_transacoes] = t;
    sistema->num_transacoes++;
    
    salvarTransacoes(sistema);
}

void lancarAquisicaoController(Sistema *sistema) {
    // Verifica se tem produtora cadastrada para pegar a margem
    if (sistema->dados_produtora == NULL) {
        printf("\nErro: Cadastre a Produtora primeiro para definir a margem de lucro.\n");
        return;
    }

    limpar_tela();
    printf("=== REGISTRAR COMPRA (NOTA FISCAL) ===\n");

    float frete_total, imposto_total;
    int total_itens_nota;
    float valor_total_produtos = 0; 

    printf("Valor TOTAL do Frete (R$): ");
    ler_float_positivo(&frete_total);

    printf("Valor TOTAL dos Impostos (R$): ");
    ler_float_positivo(&imposto_total);

    printf("Quantidade TOTAL de itens comprados: ");
    ler_inteiro_valido(&total_itens_nota, 1, 10000);

    // Calculo do rateio (quanto de frete/imposto cada unidade paga)
    float frete_por_unidade = frete_total / total_itens_nota;
    float imposto_por_unidade = imposto_total / total_itens_nota;

    int continuar = 1;
    while (continuar == 1) {
        // Aumentar memoria de recursos se preciso
        if (sistema->num_recursos == sistema->capacidade_recursos) {
            int nova_cap = (sistema->capacidade_recursos == 0) ? 10 : sistema->capacidade_recursos * 2;
            Recurso *temp = realloc(sistema->lista_recursos, nova_cap * sizeof(Recurso));
            if (!temp) { printf("Erro memoria!\n"); return; }
            sistema->lista_recursos = temp;
            sistema->capacidade_recursos = nova_cap;
        }

        Recurso *novo = &sistema->lista_recursos[sistema->num_recursos];
        novo->codigo = sistema->num_recursos + 1;

        printf("\n--- Novo Item da Nota --- \n");
        printf("Descricao: ");
        ler_texto_valido(novo->descricao, sizeof(novo->descricao), VALIDAR_NAO_VAZIO);
        printf("Categoria: ");
        ler_texto_valido(novo->categoria, sizeof(novo->categoria), VALIDAR_NAO_VAZIO);
        printf("Quantidade: ");
        ler_inteiro_valido(&novo->quantidade_estoque, 1, 1000);
        printf("Custo Unitario (R$): ");
        ler_float_positivo(&novo->preco_custo);

        // Soma ao total
        valor_total_produtos += (novo->preco_custo * novo->quantidade_estoque);

        // CALCULO DO PRECO DE LOCACAO (Regra do PDF)
        float custo_final = novo->preco_custo + frete_por_unidade + imposto_por_unidade;
        float margem = sistema->dados_produtora->margem_lucro;
        
        novo->valor_locacao = custo_final * (1 + (margem / 100.0));

        printf("--> Valor Locacao Calculado: R$ %.2f\n", novo->valor_locacao);

        sistema->num_recursos++;
        
        printf("\nMais itens na nota? (1-Sim, 0-Nao): ");
        scanf("%d", &continuar);
        limpar_buffer();
    }
    salvarRecursos(sistema); 

    // --- Lógica de Pagamento (Entrada + Parcelas) ---
    float total_final_nota = valor_total_produtos + frete_total + imposto_total;
    printf("\nTotal Geral da Nota: R$ %.2f\n", total_final_nota);
    
    float entrada = 0;
    printf("Valor da Entrada (0 se for tudo a prazo): ");
    ler_float_positivo(&entrada);

    // Se tiver entrada, desconta do caixa agora
    if (entrada > 0) {
        if (entrada > sistema->saldo_caixa) {
            printf("ALERTA: Saldo de caixa insuficiente (Atual: %.2f), mas registrando saida mesmo assim.\n", sistema->saldo_caixa);
        }
        sistema->saldo_caixa -= entrada;
        
        // Registra movimentação da entrada no histórico
        Transacao t_entrada;
        memset(&t_entrada, 0, sizeof(Transacao));
        t_entrada.tipo = MOVIMENTACAO_CAIXA;
        t_entrada.status = PAGA;
        t_entrada.valor = entrada * -1; // Valor negativo pois é saída de dinheiro
        strcpy(t_entrada.descricao, "Entrada - Compra Equipamentos");
        strcpy(t_entrada.data_pagamento, "HOJE"); 
        registrarTransacao(sistema, t_entrada);
    }

    float restante = total_final_nota - entrada;
    
    if (restante > 0) {
        int parcelas;
        printf("Numero de parcelas para o restante (R$ %.2f): ", restante);
        ler_inteiro_valido(&parcelas, 1, 48);

        float valor_parcela = restante / parcelas;
        char data_venc[12]; 

        for (int i = 1; i <= parcelas; i++) {
            printf("Data vencimento parcela %d/%d (DD/MM/AAAA): ", i, parcelas);
            ler_texto_valido(data_venc, 12, VALIDAR_DATA);

            Transacao t_parc;
            memset(&t_parc, 0, sizeof(Transacao));
            t_parc.tipo = CONTA_A_PAGAR;
            t_parc.status = PENDENTE;
            t_parc.valor = valor_parcela;
            sprintf(t_parc.descricao, "Compra Equip. (Parc %d/%d)", i, parcelas);
            strcpy(t_parc.data_vencimento, data_venc);
            
            registrarTransacao(sistema, t_parc);
        }
    } else {
        printf("Compra quitada integralmente na entrada.\n");
    }
    
    printf("\nAquisicao registrada e estoque atualizado!\n");
}

void verSaldoCaixaController(Sistema *sistema) {
    limpar_tela();
    printf("=== FLUXO DE CAIXA ===\n");
    printf("Saldo Disponivel: R$ %.2f\n", sistema->saldo_caixa);
}

void gerenciarContasReceberController(Sistema *sistema) {
    limpar_tela();
    printf("=== CONTAS A RECEBER ===\n");
    for(int i=0; i < sistema->num_transacoes; i++) {
        Transacao *t = &sistema->lista_transacoes[i];
        if(t->tipo == CONTA_A_RECEBER) {
            printf("#%d | %s | R$ %.2f | Venc: %s | %s\n", t->codigo, t->descricao, t->valor, t->data_vencimento,
                   t->status == PAGA ? "PAGO" : "PENDENTE");
        }
    }
    
    int cod;
    printf("\nCodigo para RECEBER (0 voltar): ");
    scanf("%d", &cod);
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod && sistema->lista_transacoes[i].tipo == CONTA_A_RECEBER) {
            if(sistema->lista_transacoes[i].status == PENDENTE) {
                sistema->lista_transacoes[i].status = PAGA;
                // Pega data atual ficticia ou pede
                strcpy(sistema->lista_transacoes[i].data_pagamento, "DATA_ATUAL"); 
                
                sistema->saldo_caixa += sistema->lista_transacoes[i].valor;
                
                // Gera registro de entrada no caixa se quiser historico detalhado
                // mas aqui so atualizamos o status e o saldo
                
                salvarTransacoes(sistema);
                printf("Recebido! Saldo atual: %.2f\n", sistema->saldo_caixa);
            } else {
                printf("Ja estava pago.\n");
            }
            return;
        }
    }
    printf("Nao encontrado.\n");
}

void gerenciarContasPagarController(Sistema *sistema) {
    limpar_tela();
    printf("=== CONTAS A PAGAR ===\n");
    for(int i=0; i < sistema->num_transacoes; i++) {
        Transacao *t = &sistema->lista_transacoes[i];
        if(t->tipo == CONTA_A_PAGAR) {
            printf("#%d | %s | R$ %.2f | Venc: %s | %s\n", t->codigo, t->descricao, t->valor, t->data_vencimento,
                   t->status == PAGA ? "PAGO" : "PENDENTE");
        }
    }

    int cod;
    printf("\nCodigo para PAGAR (0 voltar): ");
    scanf("%d", &cod);
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod && sistema->lista_transacoes[i].tipo == CONTA_A_PAGAR) {
            if(sistema->lista_transacoes[i].status == PENDENTE) {
                if(sistema->saldo_caixa >= sistema->lista_transacoes[i].valor) {
                    sistema->lista_transacoes[i].status = PAGA;
                    strcpy(sistema->lista_transacoes[i].data_pagamento, "DATA_ATUAL");
                    
                    sistema->saldo_caixa -= sistema->lista_transacoes[i].valor;
                    salvarTransacoes(sistema);
                    printf("Pago! Saldo atual: %.2f\n", sistema->saldo_caixa);
                } else {
                    printf("Erro: Sem saldo suficiente!\n");
                }
            } else {
                printf("Ja estava pago.\n");
            }
            return;
        }
    }
    printf("Nao encontrado.\n");
}