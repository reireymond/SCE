#include "operador_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "utils/validation.h"
#include "controller/operador/operador_controller.h"

void menuOperadoresView(Sistema *sistema) {
    int opcao;
    do {
       limpar_tela();
        printf("+=====================================================+\n");
        printf("|             MENU OPERADORES DO SISTEMA              |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Operador                         |\n");
        printf("| [2] Alterar Operador Existente                      |\n");
        printf("| [3] Listar Todos os Operadores                      |\n");
        printf("| [4] Excluir Operador                                |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarOperadorController(sistema); break;
            case 2: alterarOperadorController(sistema); break;
            case 3: listarOperadoresView(sistema); break;
            case 4: excluirOperadorController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarOperadoresView(Sistema *sistema) {
    if (sistema->num_operadores == 0) {
        printf("\nNenhum operador cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Operadores ---\n");
    for (int i = 0; i < sistema->num_operadores; i++) {
        printf("ID: %d | Nome: %s | Usuario: %s\n", 
               sistema->lista_operadores[i].codigo,
               sistema->lista_operadores[i].nome,
               sistema->lista_operadores[i].usuario);
    }
}
void formulario_novo_operador(Operador *op) {
    printf("\n--- Novo Operador (ID: %d) ---\n", op->codigo);
    printf("Nome: "); ler_texto_valido(op->nome, sizeof(op->nome), VALIDAR_NOME);
    printf("Usuario: "); ler_texto_valido(op->usuario, sizeof(op->usuario), VALIDAR_NAO_VAZIO);
    printf("Senha: "); ler_texto_valido(op->senha, sizeof(op->senha), VALIDAR_NAO_VAZIO);
}

int menu_alterar_operador(Operador *op) {
    int opcao;
    limpar_tela();
    printf("--- Editando: %s ---\n", op->usuario);
    printf("1. Nome\n2. Usuario\n3. Senha\n0. Sair\nEscolha: ");
    ler_inteiro_valido(&opcao, 0, 3);
    
    if(opcao != 0) printf("\n>> Novo Dado: ");
    switch(opcao) {
        case 1: ler_texto_valido(op->nome, sizeof(op->nome), VALIDAR_NOME); break;
        case 2: ler_texto_valido(op->usuario, sizeof(op->usuario), VALIDAR_NAO_VAZIO); break;
        case 3: ler_texto_valido(op->senha, sizeof(op->senha), VALIDAR_NAO_VAZIO); break;
    }
    return opcao;
}
void tela_login(char *usuario_dest, char *senha_dest) {
    limpar_tela();
    printf("+=====================================================+\n");
    printf("|                 LOGIN DO OPERADOR                   |\n");
    printf("+=====================================================+\n");
    printf("Usuario: ");
    ler_texto_valido(usuario_dest, 50, VALIDAR_NAO_VAZIO);
    printf("Senha: ");
    ler_texto_valido(senha_dest, 50, VALIDAR_NAO_VAZIO);
}

void mensagem_login_falhou(int tentativa, int max) {
    printf("\nUsuario ou senha incorretos! Tentativa %d de %d.\n", tentativa, max);
    pausar();
}

void mensagem_login_sucesso(char *nome) {
    printf("\nBem-vindo, %s.\n", nome);
    pausar();
}

void mensagem_primeiro_acesso() {
    limpar_tela();
    printf("PRIMEIRO ACESSO: Cadastre o usuario administrador.\n");
}