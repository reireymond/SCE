// Inclui o cabeçalho do controller, que contém as declarações das funções
// que serão implementadas neste arquivo.
#include "equipe_interna_controller.h"
// Inclui os cabeçalhos padrão do C para funções de entrada/saída (stdio),
// alocação de memória (stdlib) e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Inclui o cabeçalho do model para poder chamar a função 'salvarEquipeInterna'.
#include "model/equipe_interna/equipe_interna_model.h"
// Inclui o cabeçalho da view para poder chamar a função 'listarEquipeInternaView'.
#include "view/equipe_interna/equipe_interna_view.h"
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_buffer'.
#include "utils/utils.h"
// Inclui o cabeçalho de validação para usar funções como 'ler_string_valida'.
#include "utils/validation.h"

// Função para adicionar um novo membro da equipe ao sistema.
void adicionarEquipeController(Sistema *sistema) {
    // Verifica se a lista de membros da equipe atingiu sua capacidade máxima.
    if (sistema->num_equipe == sistema->capacidade_equipe) {
        // Se a capacidade for 0, define uma capacidade inicial de 10; caso contrário, dobra a capacidade atual.
        int nova_capacidade = (sistema->capacidade_equipe == 0) ? 10 : sistema->capacidade_equipe * 2;
        // Tenta realocar a memória da lista para o novo tamanho.
        EquipeInterna *temp = realloc(sistema->lista_equipe, nova_capacidade * sizeof(EquipeInterna));
        // Se a realocação de memória falhar (realloc retorna NULL), exibe um erro e encerra a função.
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para equipe!\n");
            return;
        }
        // Se a realocação for bem-sucedida, atualiza o ponteiro da lista e a capacidade no sistema.
        sistema->lista_equipe = temp;
        sistema->capacidade_equipe = nova_capacidade;
    }

    // Obtém um ponteiro para a próxima posição livre no array da equipe.
    EquipeInterna *novo_membro = &sistema->lista_equipe[sistema->num_equipe];
    // Define o código do novo membro de forma sequencial.
    novo_membro->codigo = sistema->num_equipe + 1;

    // Exibe o cabeçalho para o formulário de cadastro.
    printf("\n--- Cadastro de Novo Membro (Codigo: %d) ---\n", novo_membro->codigo);
    // Solicita e lê cada informação do novo membro, validando as entradas.
    printf("Nome: ");
    ler_string_valida(novo_membro->nome, sizeof(novo_membro->nome), VALIDATE_NAME);

    printf("CPF: ");
    ler_string_valida(novo_membro->cpf, sizeof(novo_membro->cpf), VALIDATE_CPF);

    printf("Funcao: ");
    ler_string_valida(novo_membro->funcao, sizeof(novo_membro->funcao), VALIDATE_NOT_EMPTY);
    
    printf("Valor da diaria: R$ ");
    // scanf é usado aqui para ler um float. A função de validação específica para floats será usada em 'alterar'.
    ler_float_positivo(&novo_membro->valor_diaria);

    // Incrementa o contador de membros da equipe.
    sistema->num_equipe++;

    // Chama a função do model para salvar a lista atualizada em arquivo.
    salvarEquipeInterna(sistema);
    printf("\nMembro '%s' cadastrado com sucesso!\n", novo_membro->nome);
}

// Função para alterar os dados de um membro da equipe.
void alterarEquipeController(Sistema *sistema) {
    // Exibe a lista de membros para que o usuário possa ver os códigos e escolher quem alterar.
    listarEquipeInternaView(sistema);
    // Se não houver membros, informa o usuário e retorna.
    if (sistema->num_equipe == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do membro que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o membro na lista pelo código fornecido.
    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) {
            indice = i; // Guarda o índice do membro encontrado.
            break;      // Sai do loop.
        }
    }

    // Se o índice não foi alterado, o membro não foi encontrado.
    if (indice == -1) {
        printf("\nMembro com codigo %d nao encontrado.\n", codigo);
        return;
    }
    
    // Obtém um ponteiro para o membro a ser alterado.
    EquipeInterna *membro = &sistema->lista_equipe[indice];
    int opcao;
    // Loop do menu de alteração.
    do {
        limpar_tela();
        printf("--- Alterando Membro: %s ---\n", membro->nome);
        printf("1. Alterar Nome\n");
        printf("2. Alterar CPF\n");
        printf("3. Alterar Funcao\n");
        printf("4. Alterar Valor da Diaria\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        // Lê a opção do usuário, garantindo que seja um número válido no intervalo [0, 4].
        ler_int_valido(&opcao, 0, 4);

        // Processa a opção escolhida pelo usuário.
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
                // Usa a função de validação para garantir que o float seja um número positivo.
                ler_float_positivo(&membro->valor_diaria);
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);

    // Salva as alterações no arquivo.
    salvarEquipeInterna(sistema);
}

// Função para excluir um membro da equipe.
void excluirEquipeController(Sistema *sistema) {
    // Mostra a lista de membros para o usuário.
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return;

    int codigo;
    printf("\nDigite o codigo do membro que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o membro pelo código.
    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) {
            char confirmacao;
            // Pede confirmação ao usuário antes de excluir.
            printf("Tem certeza que deseja excluir o membro %s? (s/n): ", sistema->lista_equipe[i].nome);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Para remover o item do array, o último elemento é movido para a posição do item a ser excluído.
                sistema->lista_equipe[i] = sistema->lista_equipe[sistema->num_equipe - 1];
                // O contador total de membros é decrementado.
                sistema->num_equipe--;
                printf("\nMembro excluido com sucesso!\n");
                // Salva o novo estado da lista no arquivo.
                salvarEquipeInterna(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            // Retorna da função, pois a operação foi concluída.
            return;
        }
    }
    // Se o loop terminar e o membro não for encontrado, informa o usuário.
    printf("\nMembro com codigo %d nao encontrado.\n", codigo);
}