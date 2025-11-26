#include "transacao_view.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"
#include "utils/validation.h"
#include "controller/transacao/transacao_controller.h"
#include "model/recurso/recurso_model.h"

void menuTransacoesView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|               MENU FINANCEIRO / COMPRAS             |\n");
        printf("+=====================================================+\n");
        printf("| [1] Registrar Compra (Nota Fiscal)                  |\n");
        printf("| [2] Gerenciar Contas a Receber                      |\n");
        printf("| [3] Gerenciar Contas a Pagar                        |\n");
        printf("| [4] Ver Saldo do Caixa                              |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: formularioRegistrarCompraView(sistema); break; // Alterado aqui
            case 2: gerenciarContasReceberController(sistema); break;
            case 3: gerenciarContasPagarController(sistema); break;
            case 4: verSaldoCaixaController(sistema); break;
            case 0: break;
            default: printf("Opcao invalida.\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// Essa funcao inteira veio do controller. Eh pura interface com usuario.
void formularioRegistrarCompraView(Sistema *sistema) {
    if (!sistema->dados_produtora) {
        printf("Erro: Cadastre a Produtora primeiro pra saber a margem de lucro.\n"); 
        return;
    }

    printf("\n=== REGISTRAR COMPRA (NOTA FISCAL) ===\n");
    float frete_total, imposto_total;
    int qtd_itens_diferentes;

    printf("Valor Total do Frete na Nota: R$ "); 
    ler_float_positivo(&frete_total);
    
    printf("Valor Total de Impostos: R$ "); 
    ler_float_positivo(&imposto_total);
    
    printf("Quantos produtos diferentes tem na nota? "); 
    ler_inteiro_valido(&qtd_itens_diferentes, 1, 100);

    float frete_por_item = frete_total / qtd_itens_diferentes;
    float imposto_por_item = imposto_total / qtd_itens_diferentes;

    // Loop pra cadastrar cada item da nota
    for(int i=0; i < qtd_itens_diferentes; i++) {
        printf("\n--- Produto %d ---\n", i+1);
        
        // Alocacao de memoria (feita aqui mesmo pq eh Junior style)
        if (sistema->num_recursos == sistema->capacidade_recursos) {
             int nova_cap = (sistema->capacidade_recursos == 0) ? 10 : sistema->capacidade_recursos + 10;
             Recurso *temp = realloc(sistema->lista_recursos, nova_cap * sizeof(Recurso));
             if (temp == NULL) { printf("Erro de memoria!\n"); return; }
             sistema->lista_recursos = temp;
             sistema->capacidade_recursos = nova_cap;
        }
        
        Recurso *novo = &sistema->lista_recursos[sistema->num_recursos];
        novo->codigo = sistema->num_recursos + 1;

        printf("Descricao do Equipamento: "); 
        ler_texto_valido(novo->descricao, 150, VALIDAR_NAO_VAZIO);
        
        printf("Categoria (Ex: Som, Luz): "); 
        ler_texto_valido(novo->categoria, 50, VALIDAR_NAO_VAZIO);
        
        printf("Quantidade comprada: "); 
        ler_inteiro_valido(&novo->quantidade_estoque, 1, 1000);
        
        printf("Preco de Custo (Unitario): R$ "); 
        ler_float_positivo(&novo->preco_custo);

        float custo_unitario_final = novo->preco_custo + (frete_por_item / novo->quantidade_estoque) + (imposto_por_item / novo->quantidade_estoque);
        
        // Regra de negocio simples aplicada na view
        novo->valor_locacao = custo_unitario_final * (sistema->dados_produtora->margem_lucro / 100.0);

        printf("-> Valor de Locacao Calculado: R$ %.2f\n", novo->valor_locacao);
        
        sistema->num_recursos++;
    }
    // Salva recursos
    salvarRecursos(sistema);
    
    // Parte de pagamento
    int tipo_pagamento;
    printf("\nComo deseja pagar essa compra?\n");
    printf("1. A Vista (Sai do Caixa agora)\n");
    printf("2. A Prazo (Gera Conta a Pagar)\n");
    printf("Opcao: ");
    ler_inteiro_valido(&tipo_pagamento, 1, 2);

    Transacao t;
    memset(&t, 0, sizeof(Transacao));
    t.tipo = CONTA_A_PAGAR;
    
    printf("\nValor Total da Nota para Pagar: R$ "); 
    ler_float_positivo(&t.valor);
    
    printf("Descricao da conta: "); 
    ler_texto_valido(t.descricao, 100, VALIDAR_NAO_VAZIO);
    
    if (tipo_pagamento == 1) {
        if (sistema->saldo_caixa >= t.valor) {
            sistema->saldo_caixa -= t.valor;
            t.status = PAGA; 
            printf("Data do Pagamento (Hoje): ");
            ler_texto_valido(t.data_pagamento, 12, VALIDAR_DATA);
            strcpy(t.data_vencimento, t.data_pagamento);
            printf("Pagamento realizado com sucesso! Saiu do caixa.\n");
        } else {
            printf("ERRO: Sem dinheiro no caixa! Vai ficar como PENDENTE.\n");
            t.status = PENDENTE;
            printf("Data de Vencimento: "); 
            ler_texto_valido(t.data_vencimento, 12, VALIDAR_DATA);
        }
    } else {
        t.status = PENDENTE;
        printf("Data de Vencimento: "); 
        ler_texto_valido(t.data_vencimento, 12, VALIDAR_DATA);
    }
    
    // Chama o controller so pra gravar a transacao final
    registrarTransacao(sistema, t);
    printf("Compra registrada e estoque atualizado!\n");
}
