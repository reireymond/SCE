
#include "recurso_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "utils/validation.h"
#include "controller/recurso/recurso_controller.h"

// exibe e gerencia o menu de recursos e equipamentos.
void menuRecursosView(Sistema *sistema) {
    int opcao;
    do {
       limpar_tela();
        printf("+=====================================================+\n");
        printf("|           MENU RECURSOS E EQUIPAMENTOS              |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Recurso                          |\n");
        printf("| [2] Alterar Recurso Existente                       |\n");
        printf("| [3] Listar Todos os Recursos                        |\n");
        printf("| [4] Excluir Recurso                                 |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
 
        limpar_buffer();
        switch (opcao) {
            case 1: adicionarRecursoController(sistema); break;
            case 2: alterarRecursoController(sistema); break;
            case 3: listarRecursosView(sistema); break;
            case 4: excluirRecursoController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// exibe a lista de recursos e equipamentos.
void listarRecursosView(Sistema *sistema) {
    if (sistema->num_recursos == 0) {
        printf("\nNenhum recurso cadastrado.\n");
        return; 
    }
    
        printf("+=====================================================+\n");
        printf("|           LISTA DE RECURSOS E EQUIPAMENTOS          |\n");
        printf("+=====================================================+\n");

        // Verifica se há recursos cadastrados
        if (sistema->num_recursos == 0) {
            printf("| Nenhum recurso cadastrado.                          |\n");
        } else {
            for (int i = 0; i < sistema->num_recursos; i++) {
                
                printf("+-----------------------------------------------------+\n");
                printf("| Codigo: %-32d \n", sistema->lista_recursos[i].codigo);
                printf("| Descricao: %-32s \n", sistema->lista_recursos[i].descricao);
                printf("| Categoria: %-32s \n", sistema->lista_recursos[i].categoria);
                printf("| Estoque: %-32d \n", sistema->lista_recursos[i].quantidade_estoque);
                printf("| Preco de Custo: R$ %-28.2f \n", sistema->lista_recursos[i].preco_custo);
                printf("| Valor Locacao: R$ %-28.2f \n", sistema->lista_recursos[i].valor_locacao);
            }
        }
                printf("+=====================================================+\n");

}

void formulario_novo_recurso(Recurso *r) {
    printf("\n--- Novo Recurso (ID: %d) ---\n", r->codigo);
    printf("Descricao: "); 
    ler_texto_valido(r->descricao, sizeof(r->descricao), VALIDAR_NAO_VAZIO);
    
    printf("Categoria: "); 
    ler_texto_valido(r->categoria, sizeof(r->categoria), VALIDAR_NAO_VAZIO);
    
    printf("Estoque: "); 
    ler_inteiro_valido(&r->quantidade_estoque, 0, 9999);
    
    printf("Preco de Custo: R$ "); 
    ler_float_positivo(&r->preco_custo);
    
    printf("Valor Locacao: R$ "); 
    ler_float_positivo(&r->valor_locacao);
}

int pedir_id_recurso(const char *acao) {
    int id;
    printf("\nDigite o ID do Recurso para %s: ", acao);
    ler_inteiro_valido(&id, 1, 999999);
    return id;
}

int menu_alterar_recurso(Recurso *r) {
    int opcao;
    limpar_tela();
    printf("--- Editando: %s ---\n", r->descricao);
    printf("1. Descricao\n2. Categoria\n3. Estoque\n4. Custo\n5. Locacao\n0. Sair\nEscolha: ");
    ler_inteiro_valido(&opcao, 0, 5);

    if(opcao != 0) printf("\n>> Novo valor: ");

    switch(opcao) {
        case 1: ler_texto_valido(r->descricao, sizeof(r->descricao), VALIDAR_NAO_VAZIO); break;
        case 2: ler_texto_valido(r->categoria, sizeof(r->categoria), VALIDAR_NAO_VAZIO); break;
        case 3: ler_inteiro_valido(&r->quantidade_estoque, 0, 9999); break;
        case 4: ler_float_positivo(&r->preco_custo); break;
        case 5: ler_float_positivo(&r->valor_locacao); break;
    }
    return opcao;
}

int confirmar_exclusao_recurso(char *nome) {
    char resp;
    printf("\nTem certeza que deseja excluir '%s'? (s/n): ", nome);
    scanf(" %c", &resp); limpar_buffer();
    return (resp == 's' || resp == 'S');
}
