#include "equipe_interna_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/equipe_interna/equipe_interna_model.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Função responsável por adicionar um novo membro à equipe
void adicionarEquipeController(Sistema *sistema) {
    // Verifica se o vetor está cheio; se sim, dobra a capacidade
    if (sistema->num_equipe == sistema->capacidade_equipe) {
        int nova_capacidade = (sistema->capacidade_equipe == 0) ? 10 : sistema->capacidade_equipe * 2;
        EquipeInterna *temp = realloc(sistema->lista_equipe, nova_capacidade * sizeof(EquipeInterna));

        // Verifica se houve erro de alocação
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para equipe!\n");
            return;
        }

        // Atualiza ponteiro e capacidade
        sistema->lista_equipe = temp;
        sistema->capacidade_equipe = nova_capacidade;
    }

    // Cria um novo membro na próxima posição do vetor
    EquipeInterna *novo_membro = &sistema->lista_equipe[sistema->num_equipe];
    novo_membro->codigo = sistema->num_equipe + 1; // Código automático

    // Coleta os dados do novo membro
    printf("\n--- Cadastro de Novo Membro (Codigo: %d) ---\n", novo_membro->codigo);
    printf("Nome: ");
    ler_string_valida(novo_membro->nome, sizeof(novo_membro->nome), VALIDATE_NAME);

    printf("CPF: ");
    ler_string_valida(novo_membro->cpf, sizeof(novo_membro->cpf), VALIDATE_CPF);

    printf("Funcao: ");
    ler_string_valida(novo_membro->funcao, sizeof(novo_membro->funcao), VALIDATE_NOT_EMPTY);
    
    printf("Valor da diaria: R$ ");
    scanf("%f", &novo_membro->valor_diaria);
    limpar_buffer();

    // Incrementa o contador de membros
    sistema->num_equipe++;

    // Salva as alterações no arquivo
    salvarEquipeInterna(sistema);
    printf("\nMembro '%s' cadastrado com sucesso!\n", novo_membro->nome);
}

// Função responsável por alterar os dados de um membro existente
void alterarEquipeController(Sistema *sistema) {
    // Mostra a lista atual de membros
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return; // Sai se não houver membros

    int codigo, indice = -1;
    printf("\nDigite o codigo do membro que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o membro pelo código informado
    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    // Caso o membro não seja encontrado
    if (indice == -1) {
        printf("\nMembro com codigo %d nao encontrado.\n", codigo);
        return;
    }
    
    // Aponta para o membro encontrado
    EquipeInterna *membro = &sistema->lista_equipe[indice];
    int opcao;

    // Menu para escolher qual campo alterar
    do {
        limpar_tela();
        printf("--- Alterando Membro: %s ---\n", membro->nome);
        printf("1. Alterar Nome\n");
        printf("2. Alterar CPF\n");
        printf("3. Alterar Funcao\n");
        printf("4. Alterar Valor da Diaria\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        ler_int_valido(&opcao, 0, 4);

        // Executa a alteração conforme a escolha do usuário
        switch (opcao) {
            case 1:
                printf("Novo Nome: ");
                ler_string_valida(membro->nome, sizeof(membro->nome), VALIDATE_NAME);
                break;
            case 2:
                printf("Novo CPF: ");
                ler_string_valida(membro->cpf, sizeof(membro->cpf), VALIDATE_CPF);
                break;
            case 3:
                printf("Nova Funcao: ");
                ler_string_valida(membro->funcao, sizeof(membro->funcao), VALIDATE_NOT_EMPTY);
                break;
            case 4:
                printf("Novo Valor da Diaria: R$ ");
                ler_float_positivo(&membro->valor_diaria);
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);

    // Salva as mudanças feitas
    salvarEquipeInterna(sistema);
}

// Função responsável por excluir um membro da equipe
void excluirEquipeController(Sistema *sistema) {
    // Mostra todos os membros cadastrados
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return; // Sai se não houver membros

    int codigo;
    printf("\nDigite o codigo do membro que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o membro pelo código
    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o membro %s? (s/n): ", sistema->lista_equipe[i].nome);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            // Confirma a exclusão
            if (confirmacao == 's' || confirmacao == 'S') {
                // Substitui o membro excluído pelo último da lista
                sistema->lista_equipe[i] = sistema->lista_equipe[sistema->num_equipe - 1];
                sistema->num_equipe--;
                printf("\nMembro excluido com sucesso!\n");
                salvarEquipeInterna(sistema); // Atualiza o arquivo
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }

    // Caso o membro não seja encontrado
    printf("\nMembro com codigo %d nao encontrado.\n", codigo);
}
