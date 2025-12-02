#include "cliente_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "utils/validation.h"
#include "controller/cliente/cliente_controller.h"

void menuClientesView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|                   MENU CLIENTES                     |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Cliente                          |\n");
        printf("| [2] Alterar Cliente Existente                       |\n");
        printf("| [3] Listar Todos os Clientes                        |\n");
        printf("| [4] Excluir Cliente                                 |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarClienteController(sistema); break;
            case 2: alterarClienteController(sistema); break;
            case 3: listarClientesView(sistema); break;
            case 4: excluirClienteController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarClientesView(Sistema *sistema) {
    if (sistema->num_clientes == 0) {
        printf("\nNenhum cliente cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Clientes ---\n");
    for (int i = 0; i < sistema->num_clientes; i++) {
        printf("ID: %d | Nome: %s | Tel: %s\n", 
               sistema->lista_clientes[i].codigo, 
               sistema->lista_clientes[i].razao_social, 
               sistema->lista_clientes[i].telefone);
    }
}

void mensagem_erro(const char *msg) {
    printf("\n %s Erro \n", msg);
    pausar();
}

void mensagem_sucesso(const char *msg) {
    printf("\n %s Sucesso\n", msg);
    pausar();
}

void mensagem_aviso(const char *msg) {
    printf("\n %s Aviso \n", msg);
    pausar();
}

void formulario_novo_cliente(Cliente *c) {
    printf("\n--- Preencha os dados do Cliente (ID: %d) ---\n", c->codigo);
    
    printf("Nome / Razao Social: "); 
    ler_texto_valido(c->razao_social, sizeof(c->razao_social), VALIDAR_NAO_VAZIO);
    
    printf("CPF / CNPJ: "); 
    ler_texto_valido(c->cnpj, sizeof(c->cnpj), VALIDAR_CPF_CNPJ);
    
    printf("Endereco Completo: "); 
    ler_texto_valido(c->endereco, sizeof(c->endereco), VALIDAR_NAO_VAZIO);
    
    printf("Telefone: "); 
    ler_texto_valido(c->telefone, sizeof(c->telefone), VALIDAR_TELEFONE);
    
    printf("E-mail: "); 
    ler_texto_valido(c->email, sizeof(c->email), VALIDAR_EMAIL);
    
    printf("Nome do Contato: "); 
    ler_texto_valido(c->nome_do_contato, sizeof(c->nome_do_contato), VALIDAR_NOME);
}

int pedir_id_cliente(const char *acao) {
    int codigo;
    printf("\nDigite o ID do cliente para %s: ", acao);
    scanf("%d", &codigo);
    limpar_buffer();
    return codigo;
}

int menu_alterar_cliente(Cliente *c) {
    int opcao;
    limpar_tela();
    printf("--- Alterando Cliente: %s ---\n", c->razao_social);
    printf("1. Nome\n2. CPF/CNPJ\n3. Endereco\n4. Telefone\n5. Email\n6. Contato\n0. Sair e Salvar\n");
    printf("Escolha o campo para editar: ");
    ler_inteiro_valido(&opcao, 0, 6);
    return opcao;
}

void atualizar_campo_cliente(Cliente *c, int opcao) {
    printf("\n>> ");
    switch (opcao) {
        case 1: printf("Novo Nome: "); ler_texto_valido(c->razao_social, sizeof(c->razao_social), VALIDAR_NAO_VAZIO); break;
        case 2: printf("Novo CPF/CNPJ: "); ler_texto_valido(c->cnpj, sizeof(c->cnpj), VALIDAR_CPF_CNPJ); break;
        case 3: printf("Novo Endereco: "); ler_texto_valido(c->endereco, sizeof(c->endereco), VALIDAR_NAO_VAZIO); break;
        case 4: printf("Novo Telefone: "); ler_texto_valido(c->telefone, sizeof(c->telefone), VALIDAR_TELEFONE); break;
        case 5: printf("Novo Email: "); ler_texto_valido(c->email, sizeof(c->email), VALIDAR_EMAIL); break;
        case 6: printf("Novo Contato: "); ler_texto_valido(c->nome_do_contato, sizeof(c->nome_do_contato), VALIDAR_NOME); break;
    }
}

int confirmar_exclusao(const char *nome) {
    char conf;
    printf("\nTem certeza que deseja EXCLUIR '%s'? (s/n): ", nome);
    scanf(" %c", &conf);
    limpar_buffer();
    return (conf == 's' || conf == 'S');
}