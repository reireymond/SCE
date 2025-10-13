// Inclui o cabeçalho do controller de fornecedor, que contém as declarações
// das funções que são implementadas neste arquivo.
#include "fornecedor_controller.h"
// Inclui cabeçalhos padrão do C para funções de entrada/saída (stdio),
// alocação de memória (stdlib) e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Inclui o cabeçalho do model de fornecedor para poder chamar a função 'salvarFornecedores'.
#include "model/fornecedor/fornecedor_model.h"
// Inclui o cabeçalho da view de fornecedor para poder chamar a função 'listarFornecedoresView'.
#include "view/fornecedor/fornecedor_view.h"
// Inclui o cabeçalho de utilitários para usar funções auxiliares como 'limpar_buffer'.
#include "utils/utils.h"
// Inclui o cabeçalho de validação para usar funções como 'ler_string_valida' e 'ler_int_valido'.
#include "utils/validation.h"

// Função para adicionar um novo fornecedor ao sistema.
void adicionarFornecedorController(Sistema *sistema) {
    // Verifica se a lista de fornecedores atingiu sua capacidade máxima.
    if (sistema->num_fornecedores == sistema->capacidade_fornecedores) {
        // Se a capacidade for 0, define uma capacidade inicial; caso contrário, dobra a capacidade.
        int nova_capacidade = (sistema->capacidade_fornecedores == 0) ? 10 : sistema->capacidade_fornecedores * 2;
        // Tenta realocar a memória da lista para o novo tamanho, preservando os dados existentes.
        Fornecedor *temp = realloc(sistema->lista_fornecedores, nova_capacidade * sizeof(Fornecedor));
        // Se a realocação de memória falhar, exibe uma mensagem de erro e termina a função.
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para fornecedores!\n");
            return;
        }
        // Atualiza o ponteiro da lista e a capacidade no sistema.
        sistema->lista_fornecedores = temp;
        sistema->capacidade_fornecedores = nova_capacidade;
    }

    // Obtém um ponteiro para a próxima posição livre no array de fornecedores.
    Fornecedor *novo_fornecedor = &sistema->lista_fornecedores[sistema->num_fornecedores];
    // Atribui um código sequencial ao novo fornecedor.
    novo_fornecedor->codigo = sistema->num_fornecedores + 1;

    // Exibe o cabeçalho do formulário de cadastro.
    printf("\n--- Cadastro de Novo Fornecedor (Codigo: %d) ---\n", novo_fornecedor->codigo);
    // Solicita e lê cada informação do novo fornecedor, usando as funções de validação.
    printf("Nome Fantasia: ");
    ler_string_valida(novo_fornecedor->nome_fantasia, sizeof(novo_fornecedor->nome_fantasia), VALIDATE_NOT_EMPTY);

    printf("Razao Social: ");
    ler_string_valida(novo_fornecedor->razao_social, sizeof(novo_fornecedor->razao_social), VALIDATE_NOT_EMPTY);
    
    printf("CNPJ / CPF: ");
    ler_string_valida(novo_fornecedor->cnpj, sizeof(novo_fornecedor->cnpj), VALIDATE_CPF_CNPJ);

    printf("Endereco: ");
    ler_string_valida(novo_fornecedor->endereco, sizeof(novo_fornecedor->endereco), VALIDATE_NOT_EMPTY);

    printf("Telefone: ");
    ler_string_valida(novo_fornecedor->telefone, sizeof(novo_fornecedor->telefone), VALIDATE_PHONE);

    printf("Tipo de Servico: ");
    ler_string_valida(novo_fornecedor->tipo_servico, sizeof(novo_fornecedor->tipo_servico), VALIDATE_NOT_EMPTY);

    // Incrementa o número total de fornecedores.
    sistema->num_fornecedores++;

    // Salva a lista atualizada no arquivo.
    salvarFornecedores(sistema);
    printf("\nFornecedor '%s' cadastrado com sucesso!\n", novo_fornecedor->nome_fantasia);
}

// Função para alterar os dados de um fornecedor existente.
void alterarFornecedorController(Sistema *sistema) {
    // Exibe a lista de fornecedores para que o usuário escolha qual alterar.
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return;
    
    int codigo, indice = -1;
    printf("\nDigite o codigo do fornecedor que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o fornecedor pelo código informado.
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) {
            indice = i; // Armazena o índice do fornecedor encontrado.
            break;
        }
    }

    // Se o fornecedor não foi encontrado, exibe uma mensagem e retorna.
    if (indice == -1) {
        printf("\nFornecedor com codigo %d nao encontrado.\n", codigo);
        return;
    }

    // Obtém um ponteiro para o fornecedor que será modificado.
    Fornecedor *fornecedor = &sistema->lista_fornecedores[indice];
    int opcao;
    // Loop para o menu de alteração.
    do {
        limpar_tela();
        printf("--- Alterando Fornecedor: %s ---\n", fornecedor->nome_fantasia);
        printf("1. Alterar Nome Fantasia\n");
        printf("2. Alterar Razao Social\n");
        printf("3. Alterar CNPJ / CPF\n");
        printf("4. Alterar Endereco\n");
        printf("5. Alterar Telefone\n");
        printf("6. Alterar Tipo de Servico\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        // Lê a opção do usuário, validando o intervalo de entrada.
        ler_int_valido(&opcao, 0, 6);

        // Executa a ação correspondente à opção escolhida.
        switch (opcao) {
            case 1:
                printf("Novo Nome Fantasia: ");
                ler_string_valida(fornecedor->nome_fantasia, sizeof(fornecedor->nome_fantasia), VALIDATE_NOT_EMPTY);
                break;
            case 2:
                printf("Nova Razao Social: ");
                ler_string_valida(fornecedor->razao_social, sizeof(fornecedor->razao_social), VALIDATE_NOT_EMPTY);
                break;
            case 3:
                printf("Novo CNPJ / CPF: ");
                ler_string_valida(fornecedor->cnpj, sizeof(fornecedor->cnpj), VALIDATE_CPF_CNPJ);
                break;
            case 4:
                printf("Novo Endereco: ");
                ler_string_valida(fornecedor->endereco, sizeof(fornecedor->endereco), VALIDATE_NOT_EMPTY);
                break;
            case 5:
                printf("Novo Telefone: ");
                ler_string_valida(fornecedor->telefone, sizeof(fornecedor->telefone), VALIDATE_PHONE);
                break;
            case 6:
                printf("Novo Tipo de Servico: ");
                ler_string_valida(fornecedor->tipo_servico, sizeof(fornecedor->tipo_servico), VALIDATE_NOT_EMPTY);
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);

    // Salva todas as alterações no arquivo.
    salvarFornecedores(sistema);
}

// Função para excluir um fornecedor do sistema.
void excluirFornecedorController(Sistema *sistema) {
    // Exibe a lista de fornecedores.
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return;

    int codigo;
    printf("\nDigite o codigo do fornecedor que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o fornecedor pelo código.
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) {
            char confirmacao;
            // Pede confirmação ao usuário antes de prosseguir com a exclusão.
            printf("Tem certeza que deseja excluir o fornecedor %s? (s/n): ", sistema->lista_fornecedores[i].nome_fantasia);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Move o último fornecedor da lista para a posição do que está sendo excluído.
                sistema->lista_fornecedores[i] = sistema->lista_fornecedores[sistema->num_fornecedores - 1];
                // Reduz o contador de fornecedores em uma unidade.
                sistema->num_fornecedores--;
                printf("\nFornecedor excluido com sucesso!\n");
                // Salva o estado atualizado da lista no arquivo.
                salvarFornecedores(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            // Retorna, pois a operação (exclusão ou cancelamento) foi finalizada.
            return;
        }
    }
    // Se o loop terminar sem encontrar o fornecedor, exibe uma mensagem.
    printf("\nFornecedor com codigo %d nao encontrado.\n", codigo);
}