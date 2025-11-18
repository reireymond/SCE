// Controller de fornecedor
#include "fornecedor_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Model (salvar e carregar dados)
#include "model/fornecedor/fornecedor_model.h"
// View (exibição e interação com o usuário)
#include "view/fornecedor/fornecedor_view.h"
// Utilitários gerais
#include "utils/utils.h"
// Validação de dados
#include "utils/validation.h"


// Cadastra um novo fornecedor no sistema
void adicionarFornecedorController(Sistema *sistema) {
    if (sistema->num_fornecedores == sistema->capacidade_fornecedores) {
        int nova_capacidade = (sistema->capacidade_fornecedores == 0) ? 10 : sistema->capacidade_fornecedores * 2;
        Fornecedor *temp = realloc(sistema->lista_fornecedores, nova_capacidade * sizeof(Fornecedor));

        if (temp == NULL) {
            printf("\nErro de alocacao de memoria para fornecedores!\n");
            return;
        }

        sistema->lista_fornecedores = temp;
        sistema->capacidade_fornecedores = nova_capacidade;
    }

    Fornecedor *novo_fornecedor = &sistema->lista_fornecedores[sistema->num_fornecedores];
    novo_fornecedor->codigo = sistema->num_fornecedores + 1;

    printf("\n--- Cadastro de Novo Fornecedor (Codigo: %d) ---\n", novo_fornecedor->codigo);

    printf("Nome Fantasia: ");
    ler_texto_valido(novo_fornecedor->nome_fantasia, sizeof(novo_fornecedor->nome_fantasia), VALIDAR_NAO_VAZIO);
    printf("Razao Social: ");
    ler_texto_valido(novo_fornecedor->razao_social, sizeof(novo_fornecedor->razao_social), VALIDAR_NAO_VAZIO);
    printf("CNPJ / CPF: ");
    ler_texto_valido(novo_fornecedor->cnpj, sizeof(novo_fornecedor->cnpj), VALIDAR_CPF_CNPJ);
    printf("Endereco: ");
    ler_texto_valido(novo_fornecedor->endereco, sizeof(novo_fornecedor->endereco), VALIDAR_NAO_VAZIO);
    printf("Telefone: ");
    ler_texto_valido(novo_fornecedor->telefone, sizeof(novo_fornecedor->telefone), VALIDAR_TELEFONE);
    printf("Tipo de Servico: ");
    ler_texto_valido(novo_fornecedor->tipo_servico, sizeof(novo_fornecedor->tipo_servico), VALIDAR_NAO_VAZIO);

    sistema->num_fornecedores++;
    salvarFornecedores(sistema);
    printf("\nFornecedor '%s' cadastrado com sucesso!\n", novo_fornecedor->nome_fantasia);
}


// Permite alterar dados de um fornecedor existente
void alterarFornecedorController(Sistema *sistema) {
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do fornecedor que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

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

    Fornecedor *fornecedor = &sistema->lista_fornecedores[indice];
    int opcao;

    do {
        limpar_tela();
        printf("--- Alterando Fornecedor: %s ---\n", fornecedor->nome_fantasia);
        printf("1. Nome Fantasia\n2. Razao Social\n3. CNPJ / CPF\n4. Endereco\n5. Telefone\n6. Tipo de Servico\n0. Salvar e Voltar\n");
        printf("Escolha: ");
        ler_inteiro_valido(&opcao, 0, 6);

        switch (opcao) {
            case 1:
                printf("Novo Nome Fantasia: ");
                ler_texto_valido(fornecedor->nome_fantasia, sizeof(fornecedor->nome_fantasia), VALIDAR_NAO_VAZIO);
                break;
            case 2:
                printf("Nova Razao Social: ");
                ler_texto_valido(fornecedor->razao_social, sizeof(fornecedor->razao_social), VALIDAR_NAO_VAZIO);
                break;
            case 3:
                printf("Novo CNPJ / CPF: ");
                ler_texto_valido(fornecedor->cnpj, sizeof(fornecedor->cnpj), VALIDAR_CPF_CNPJ);
                break;
            case 4:
                printf("Novo Endereco: ");
                ler_texto_valido(fornecedor->endereco, sizeof(fornecedor->endereco), VALIDAR_NAO_VAZIO);
                break;
            case 5:
                printf("Novo Telefone: ");
                ler_texto_valido(fornecedor->telefone, sizeof(fornecedor->telefone), VALIDAR_TELEFONE);
                break;
            case 6:
                printf("Novo Tipo de Servico: ");
                ler_texto_valido(fornecedor->tipo_servico, sizeof(fornecedor->tipo_servico), VALIDAR_NAO_VAZIO);
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);

    salvarFornecedores(sistema);
}


// Remove um fornecedor cadastrado
void excluirFornecedorController(Sistema *sistema) {
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return;

    int codigo;
    printf("\nDigite o codigo do fornecedor que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o fornecedor %s? (s/n): ", sistema->lista_fornecedores[i].nome_fantasia);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                sistema->lista_fornecedores[i] = sistema->lista_fornecedores[sistema->num_fornecedores - 1];
                sistema->num_fornecedores--;
                salvarFornecedores(sistema);
                printf("\nFornecedor excluido com sucesso!\n");
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }

    printf("\nFornecedor com codigo %d nao encontrado.\n", codigo);
}
