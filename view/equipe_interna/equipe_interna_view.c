#include "equipe_interna_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "utils/validation.h"
#include "controller/equipe_interna/equipe_interna_controller.h"

void menuEquipeInternaView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|                MENU EQUIPE INTERNA                  |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Membro                           |\n");
        printf("| [2] Alterar Membro Existente                        |\n");
        printf("| [3] Listar Todos os Membros                         |\n");
        printf("| [4] Excluir Membro                                  |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarEquipeInternaController(sistema); break;
            case 2: alterarEquipeInternaController(sistema); break;
            case 3: listarEquipeInternaView(sistema); break;
            case 4: excluirEquipeInternaController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarEquipeInternaView(Sistema *sistema) {
    if (sistema->num_equipe == 0) {
        printf("\nNenhum membro cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Equipe ---\n");
    for (int i = 0; i < sistema->num_equipe; i++) {
        printf("ID: %d | Nome: %s | Funcao: %s | Diaria: R$ %.2f\n", 
               sistema->lista_equipe[i].codigo,
               sistema->lista_equipe[i].nome,
               sistema->lista_equipe[i].funcao,
               sistema->lista_equipe[i].valor_diaria);
    }
}
void formulario_novo_equipe(EquipeInterna *e) {
    printf("\n--- Novo Membro (ID: %d) ---\n", e->codigo);
    printf("Nome: "); ler_texto_valido(e->nome, sizeof(e->nome), VALIDAR_NOME);
    printf("CPF: "); ler_texto_valido(e->cpf, sizeof(e->cpf), VALIDAR_CPF);
    printf("Funcao: "); ler_texto_valido(e->funcao, sizeof(e->funcao), VALIDAR_NAO_VAZIO);
    printf("Valor Diaria: R$ "); ler_float_positivo(&e->valor_diaria);
}

int pedir_id_equipe(const char *acao) {
    int id;
    printf("\nDigite o ID do Membro para %s: ", acao);
    ler_inteiro_valido(&id, 1, 999999);
    return id;
}

int menu_alterar_equipe(EquipeInterna *e) {
    int opcao;
    limpar_tela();
    printf("--- Editando: %s ---\n", e->nome);
    printf("1. Nome\n2. CPF\n3. Funcao\n4. Valor Diaria\n0. Sair\nEscolha: ");
    ler_inteiro_valido(&opcao, 0, 4);

    if(opcao != 0) printf("\nNovo valor: ");

    switch(opcao) {
        case 1: ler_texto_valido(e->nome, sizeof(e->nome), VALIDAR_NOME); break;
        case 2: ler_texto_valido(e->cpf, sizeof(e->cpf), VALIDAR_CPF); break;
        case 3: ler_texto_valido(e->funcao, sizeof(e->funcao), VALIDAR_NAO_VAZIO); break;
        case 4: ler_float_positivo(&e->valor_diaria); break;
    }
    return opcao;
}

int confirmar_exclusao_equipe(char *nome) {
    char c;
    printf("Excluir %s? (s/n): ", nome);
    scanf(" %c", &c); limpar_buffer();
    return (c == 's' || c == 'S');
}