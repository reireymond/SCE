#include "controller/transacao/transacao_controller.h"
#include "model/recurso/recurso_model.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils/utils.h"
#include "utils/validation.h"

// Função para registar a compra de equipamentos (Nota Fiscal)
// Isso atualiza o estoque e calcula o preço de aluguel sozinho
void lancarAquisicaoController(Sistema *sistema) {
    limpar_tela();
    printf("=== REGISTAR COMPRA (NOTA FISCAL) ===\n");

    float frete_total, imposto_total;
    int total_itens_nota;
    
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

    printf("\nCompra finalizada e estoque atualizado!\n");
}

// Funções vazias (Stubs) só para o código compilar sem erros nas outras opções
void gerenciarContasReceberController(Sistema *sistema) {
    printf("\nFuncionalidade de Contas a Receber em desenvolvimento...\n");
}

void gerenciarContasPagarController(Sistema *sistema) {
    printf("\nFuncionalidade de Contas a Pagar em desenvolvimento...\n");
}

void verSaldoCaixaController(Sistema *sistema) {
    printf("\nSaldo atual do caixa: R$ %.2f\n", sistema->saldo_caixa);
}