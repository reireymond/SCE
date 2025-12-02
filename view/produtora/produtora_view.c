#include "produtora_view.h"
#include <stdio.h>
#include "utils/validation.h"
#include "utils/utils.h"
#include "controller/produtora/produtora_controller.h"

void menuProdutoraView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|                   MENU PRODUTORA                    |\n");
        printf("+=====================================================+\n");
        printf("| [1] Cadastrar Dados da Produtora                    |\n");
        printf("| [2] Alterar Dados                                   |\n");
        printf("| [3] Ver Detalhes                                    |\n");
        printf("| [4] Excluir Dados                                   |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarProdutoraController(sistema); break;
            case 2: alterarProdutoraController(sistema); break;
            case 3: verDetalhesProdutoraView(sistema); break;
            case 4: excluirProdutoraController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// Mantive a função de ver detalhes igual, pois ela é uma lista e não um menu
void verDetalhesProdutoraView(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        printf("\nNenhuma produtora cadastrada para exibir.\n");
        return;
    }
    printf("+=====================================================+\n");
    printf("|          DETALHES DA PRODUTORA CADASTRADA           |\n");
    printf("+=====================================================+\n");
    printf("| Nome Fantasia: %-36s |\n", sistema->dados_produtora->nome_fantasia);
    printf("| Razao Social:  %-36s |\n", sistema->dados_produtora->razao_social);
    printf("| Responsavel:   %-36s |\n", sistema->dados_produtora->nome_do_responsavel);
    printf("| CNPJ:          %-36s |\n", sistema->dados_produtora->cnpj);
    printf("| Endereco:      %-36s |\n", sistema->dados_produtora->endereco);
    printf("| Telefone:      %-36s |\n", sistema->dados_produtora->telefone);
    printf("| Email:         %-36s |\n", sistema->dados_produtora->email);
    printf("| Margem Lucro:  %-35.2f%% |\n", sistema->dados_produtora->margem_lucro);
    printf("+=====================================================+\n");
}

void formulario_produtora(Produtora *p) {
    printf("\n--- Preencha os dados da Produtora ---\n");
    
    printf("Nome Fantasia: ");
    ler_texto_valido(p->nome_fantasia, sizeof(p->nome_fantasia), VALIDAR_NAO_VAZIO);

    printf("Razao Social: ");
    ler_texto_valido(p->razao_social, sizeof(p->razao_social), VALIDAR_NAO_VAZIO);

    printf("Nome do Responsavel: ");
    ler_texto_valido(p->nome_do_responsavel, sizeof(p->nome_do_responsavel), VALIDAR_NOME);

    printf("CNPJ: ");
    ler_texto_valido(p->cnpj, sizeof(p->cnpj), VALIDAR_CNPJ);

    printf("Inscricao Estadual: ");
    ler_texto_valido(p->inscricao_estadual, sizeof(p->inscricao_estadual), VALIDAR_NAO_VAZIO);

    printf("Endereco: ");
    ler_texto_valido(p->endereco, sizeof(p->endereco), VALIDAR_NAO_VAZIO);

    printf("Telefone: ");
    ler_texto_valido(p->telefone, sizeof(p->telefone), VALIDAR_TELEFONE);

    printf("Telefone Resp.: ");
    ler_texto_valido(p->telefone_responsavel, sizeof(p->telefone_responsavel), VALIDAR_TELEFONE);

    printf("E-mail: ");
    ler_texto_valido(p->email, sizeof(p->email), VALIDAR_EMAIL);

    printf("Margem de Lucro (%%): ");
    ler_float_positivo(&p->margem_lucro);
}

int menu_alterar_campo_produtora(Produtora *p) {
    int opcao;
    limpar_tela();
    printf("--- Alterar Produtora: %s ---\n", p->nome_fantasia);
    printf("1. Nome Fantasia\n2. Razao Social\n3. Responsavel\n4. CNPJ\n");
    printf("5. Inscricao Est.\n6. Endereco\n7. Telefone\n8. Tel. Resp.\n");
    printf("9. Email\n10. Margem Lucro\n0. Sair\nEscolha: ");
    ler_inteiro_valido(&opcao, 0, 10);
    
    if(opcao != 0) printf("\n>> Digite o novo valor: ");
    
    switch(opcao) {
        case 1: ler_texto_valido(p->nome_fantasia, sizeof(p->nome_fantasia), VALIDAR_NAO_VAZIO); break;
        case 2: ler_texto_valido(p->razao_social, sizeof(p->razao_social), VALIDAR_NAO_VAZIO); break;
        case 10: ler_float_positivo(&p->margem_lucro); break;
    }
    return opcao;
}

