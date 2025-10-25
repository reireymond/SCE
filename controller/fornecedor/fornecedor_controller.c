// Inclui o cabeçalho do controller de fornecedor, que contém as declarações
// das funções que são implementadas neste arquivo.
#include "fornecedor_controller.h"

// Inclui cabeçalhos padrão do C para entrada/saída, alocação de memória e manipulação de strings.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclui o cabeçalho do model para permitir a persistência dos dados (armazenamento em arquivo).
#include "model/fornecedor/fornecedor_model.h"

// Inclui o cabeçalho da view de fornecedor para exibir informações na tela.
#include "view/fornecedor/fornecedor_view.h"

// Inclui o cabeçalho de utilitários com funções auxiliares como limpar_buffer().
#include "utils/utils.h"

// Inclui o cabeçalho de validação, com funções que verificam formato e conteúdo dos dados digitados.
#include "utils/validation.h"

//cadastrar um novo fornecedor no sistema.

void adicionarFornecedorController(Sistema *sistema) {
    // Verifica se o vetor de fornecedores atingiu sua capacidade máxima.
    if (sistema->num_fornecedores == sistema->capacidade_fornecedores) {
        // Se ainda não há capacidade definida, começa com 10. Caso contrário, dobra o tamanho.
        int nova_capacidade = (sistema->capacidade_fornecedores == 0) ? 10 : sistema->capacidade_fornecedores * 2;

        // Realoca a memória para armazenar mais fornecedores.
        Fornecedor *temp = realloc(sistema->lista_fornecedores, nova_capacidade * sizeof(Fornecedor));

        // Se a realocação falhar, exibe erro e interrompe o processo.
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para fornecedores!\n");
            return;
        }

        // Atualiza o ponteiro e a capacidade do sistema.
        sistema->lista_fornecedores = temp;
        sistema->capacidade_fornecedores = nova_capacidade;
    }

    // Cria um novo fornecedor na próxima posição livre.
    Fornecedor *novo_fornecedor = &sistema->lista_fornecedores[sistema->num_fornecedores];
    novo_fornecedor->codigo = sistema->num_fornecedores + 1; // Código sequencial automático.

    // Exibe o cabeçalho do formulário de cadastro.
    printf("\n--- Cadastro de Novo Fornecedor (Codigo: %d) ---\n", novo_fornecedor->codigo);

    // Solicita os dados e valida as entradas.
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

    // Incrementa o contador de fornecedores.
    sistema->num_fornecedores++;

    // Persiste os dados atualizados no arquivo.
    salvarFornecedores(sistema);
    printf("\nFornecedor '%s' cadastrado com sucesso!\n", novo_fornecedor->nome_fantasia);
}

//permitir a edição de dados de um fornecedor existente.

void alterarFornecedorController(Sistema *sistema) {
    listarFornecedoresView(sistema); // Mostra a lista de fornecedores.
    if (sistema->num_fornecedores == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do fornecedor que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Busca o fornecedor correspondente ao código informado.
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nFornecedor com codigo %d nao encontrado.\n", codigo);
        return;
    }

    // Ponteiro para o fornecedor que será alterado.
    Fornecedor *fornecedor = &sistema->lista_fornecedores[indice];
    int opcao;

    // Menu de edição.
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
        ler_int_valido(&opcao, 0, 6);

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

    salvarFornecedores(sistema); // Persiste as alterações.
}

//  remover um fornecedor cadastrado do sistema.

void excluirFornecedorController(Sistema *sistema) {
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return;

    int codigo;
    printf("\nDigite o codigo do fornecedor que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Busca o fornecedor pelo código informado.
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o fornecedor %s? (s/n): ", sistema->lista_fornecedores[i].nome_fantasia);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Substitui o fornecedor excluído pelo último da lista.
                sistema->lista_fornecedores[i] = sistema->lista_fornecedores[sistema->num_fornecedores - 1];
                sistema->num_fornecedores--;
                printf("\nFornecedor excluido com sucesso!\n");
                salvarFornecedores(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }

    printf("\nFornecedor com codigo %d nao encontrado.\n", codigo);
}
