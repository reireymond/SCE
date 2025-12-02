#include "fornecedor_view.h"
#include <stdio.h>
#include "utils/validation.h"
#include "utils/utils.h"
#include "controller/fornecedor/fornecedor_controller.h"

void menuFornecedoresView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|           MENU FORNECEDORES E PARCEIROS             |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Fornecedor                       |\n");
        printf("| [2] Alterar Fornecedor Existente                    |\n");
        printf("| [3] Listar Todos os Fornecedores                    |\n");
        printf("| [4] Excluir Fornecedor                              |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarFornecedorController(sistema); break;
            case 2: alterarFornecedorController(sistema); break;
            case 3: listarFornecedoresView(sistema); break;
            case 4: excluirFornecedorController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarFornecedoresView(Sistema *sistema) {
    if (sistema->num_fornecedores == 0) {
        printf("\nNenhum fornecedor cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Fornecedores ---\n");
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        printf("ID: %d | %s | Servico: %s\n", 
               sistema->lista_fornecedores[i].codigo,
               sistema->lista_fornecedores[i].nome_fantasia,
               sistema->lista_fornecedores[i].tipo_servico);
    }
}
void formulario_novo_fornecedor(Fornecedor *f) {
    printf("\n--- Novo Fornecedor (ID: %d) ---\n", f->codigo);
    printf("Nome Fantasia: "); ler_texto_valido(f->nome_fantasia, sizeof(f->nome_fantasia), VALIDAR_NAO_VAZIO);
    printf("Razao Social: "); ler_texto_valido(f->razao_social, sizeof(f->razao_social), VALIDAR_NAO_VAZIO);
    printf("CNPJ/CPF: "); ler_texto_valido(f->cnpj, sizeof(f->cnpj), VALIDAR_CPF_CNPJ);
    printf("Endereco: "); ler_texto_valido(f->endereco, sizeof(f->endereco), VALIDAR_NAO_VAZIO);
    printf("Telefone: "); ler_texto_valido(f->telefone, sizeof(f->telefone), VALIDAR_TELEFONE);
    printf("Tipo Servico: "); ler_texto_valido(f->tipo_servico, sizeof(f->tipo_servico), VALIDAR_NAO_VAZIO);
}

int pedir_id_fornecedor(const char *acao) {
    int id;
    printf("\nDigite o ID do Fornecedor para %s: ", acao);
    ler_inteiro_valido(&id, 1, 999999);
    return id;
}

int menu_alterar_fornecedor(Fornecedor *f) {
    int opcao;
    limpar_tela();
    printf("--- Alterando: %s ---\n", f->nome_fantasia);
    printf("1. Nome\n2. Razao\n3. CNPJ\n4. Endereco\n5. Telefone\n6. Servico\n0. Sair\nEscolha: ");
    ler_inteiro_valido(&opcao, 0, 6);

    if(opcao != 0) printf("\n>> Novo valor: ");

    switch(opcao) {
        case 1: ler_texto_valido(f->nome_fantasia, sizeof(f->nome_fantasia), VALIDAR_NAO_VAZIO); break;
        case 2: ler_texto_valido(f->razao_social, sizeof(f->razao_social), VALIDAR_NAO_VAZIO); break;
        case 3: ler_texto_valido(f->cnpj, sizeof(f->cnpj), VALIDAR_CPF_CNPJ); break;
        case 4: ler_texto_valido(f->endereco, sizeof(f->endereco), VALIDAR_NAO_VAZIO); break;
        case 5: ler_texto_valido(f->telefone, sizeof(f->telefone), VALIDAR_TELEFONE); break;
        case 6: ler_texto_valido(f->tipo_servico, sizeof(f->tipo_servico), VALIDAR_NAO_VAZIO); break;
    }
    return opcao;
}

int confirmar_exclusao_fornecedor(char *nome) {
    char c;
    printf("Excluir %s? (s/n): ", nome);
    scanf(" %c", &c); limpar_buffer();
    return (c == 's' || c == 'S');
}