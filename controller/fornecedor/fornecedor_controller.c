#include "fornecedor_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/fornecedor/fornecedor_model.h"
#include "view/fornecedor/fornecedor_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Função para adicionar um novo fornecedor ao sistema
void adicionarFornecedorController(Sistema *sistema) {
    // Verifica se o vetor de fornecedores está cheio; se sim, dobra a capacidade
    if (sistema->num_fornecedores == sistema->capacidade_fornecedores) {
        int nova_capacidade = (sistema->capacidade_fornecedores == 0) ? 10 : sistema->capacidade_fornecedores * 2;
        Fornecedor *temp = realloc(sistema->lista_fornecedores, nova_capacidade * sizeof(Fornecedor));

        // Verifica se houve erro na alocação de memória
        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para fornecedores!\n");
            return;
        }

        // Atualiza ponteiro e capacidade
        sistema->lista_fornecedores = temp;
        sistema->capacidade_fornecedores = nova_capacidade;
    }

    // Cria um novo fornecedor na próxima posição do vetor
    Fornecedor *novo_fornecedor = &sistema->lista_fornecedores[sistema->num_fornecedores];
    novo_fornecedor->codigo = sistema->num_fornecedores + 1; // Gera código automático

    // Coleta as informações do novo fornecedor
    printf("\n--- Cadastro de Novo Fornecedor (Codigo: %d) ---\n", novo_fornecedor->codigo);
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

    // Incrementa o número total de fornecedores
    sistema->num_fornecedores++;

    // Salva as alterações em arquivo
    salvarFornecedores(sistema);
    printf("\nFornecedor '%s' cadastrado com sucesso!\n", novo_fornecedor->nome_fantasia);
}

// Função para alterar os dados de um fornecedor existente
void alterarFornecedorController(Sistema *sistema) {
    // Exibe a lista de fornecedores atuais
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return; // Sai se não houver fornecedores
    
    int codigo, indice = -1;
    printf("\nDigite o codigo do fornecedor que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Busca o fornecedor pelo código informado
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    // Caso não seja encontrado
    if (indice == -1) {
        printf("\nFornecedor com codigo %d nao encontrado.\n", codigo);
        return;
    }

    // Ponteiro para o fornecedor que será alterado
    Fornecedor *fornecedor = &sistema->lista_fornecedores[indice];
    int opcao;

    // Menu de opções de alteração
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

        // Executa a alteração conforme a opção escolhida
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

    // Salva as modificações
    salvarFornecedores(sistema);
}

// Função para excluir um fornecedor do sistema
void excluirFornecedorController(Sistema *sistema) {
    // Exibe a lista de fornecedores
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return; // Sai se não houver fornecedores

    int codigo;
    printf("\nDigite o codigo do fornecedor que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Busca o fornecedor pelo código informado
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o fornecedor %s? (s/n): ", sistema->lista_fornecedores[i].nome_fantasia);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            // Confirma a exclusão
            if (confirmacao == 's' || confirmacao == 'S') {
                // Substitui o fornecedor excluído pelo último da lista
                sistema->lista_fornecedores[i] = sistema->lista_fornecedores[sistema->num_fornecedores - 1];
                sistema->num_fornecedores--;
                printf("\nFornecedor excluido com sucesso!\n");
                salvarFornecedores(sistema); // Atualiza arquivo
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }

    // Caso o fornecedor não seja encontrado
    printf("\nFornecedor com codigo %d nao encontrado.\n", codigo);
}
