#include "fornecedor_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/fornecedor/fornecedor.h"
#include "view/fornecedor/fornecedor_view.h"
#include "utils/utils.h"

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
    fgets(novo_fornecedor->nome_fantasia, sizeof(novo_fornecedor->nome_fantasia), stdin);
    novo_fornecedor->nome_fantasia[strcspn(novo_fornecedor->nome_fantasia, "\n")] = 0;

    printf("Razao Social: ");
    fgets(novo_fornecedor->razao_social, sizeof(novo_fornecedor->razao_social), stdin);
    novo_fornecedor->razao_social[strcspn(novo_fornecedor->razao_social, "\n")] = 0;
    
    printf("CNPJ / CPF: ");
    fgets(novo_fornecedor->cnpj, sizeof(novo_fornecedor->cnpj), stdin);
    novo_fornecedor->cnpj[strcspn(novo_fornecedor->cnpj, "\n")] = 0;

    printf("Endereco: ");
    fgets(novo_fornecedor->endereco, sizeof(novo_fornecedor->endereco), stdin);
    novo_fornecedor->endereco[strcspn(novo_fornecedor->endereco, "\n")] = 0;

    printf("Telefone: ");
    fgets(novo_fornecedor->telefone, sizeof(novo_fornecedor->telefone), stdin);
    novo_fornecedor->telefone[strcspn(novo_fornecedor->telefone, "\n")] = 0;

    printf("Tipo de Servico: ");
    fgets(novo_fornecedor->tipo_servico, sizeof(novo_fornecedor->tipo_servico), stdin);
    novo_fornecedor->tipo_servico[strcspn(novo_fornecedor->tipo_servico, "\n")] = 0;

    sistema->num_fornecedores++;

    salvarFornecedores(sistema);
    printf("\nFornecedor '%s' cadastrado com sucesso!\n", novo_fornecedor->nome_fantasia);
}

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
        printf("1. Alterar Nome Fantasia\n");
        printf("2. Alterar Razao Social\n");
        printf("3. Alterar CNPJ / CPF\n");
        printf("4. Alterar Endereco\n");
        printf("5. Alterar Telefone\n");
        printf("6. Alterar Tipo de Servico\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                printf("Novo Nome Fantasia: ");
                fgets(fornecedor->nome_fantasia, sizeof(fornecedor->nome_fantasia), stdin);
                fornecedor->nome_fantasia[strcspn(fornecedor->nome_fantasia, "\n")] = 0;
                break;
            case 2:
                printf("Nova Razao Social: ");
                fgets(fornecedor->razao_social, sizeof(fornecedor->razao_social), stdin);
                fornecedor->razao_social[strcspn(fornecedor->razao_social, "\n")] = 0;
                break;
            case 3:
                printf("Novo CNPJ / CPF: ");
                fgets(fornecedor->cnpj, sizeof(fornecedor->cnpj), stdin);
                fornecedor->cnpj[strcspn(fornecedor->cnpj, "\n")] = 0;
                break;
            case 4:
                printf("Novo Endereco: ");
                fgets(fornecedor->endereco, sizeof(fornecedor->endereco), stdin);
                fornecedor->endereco[strcspn(fornecedor->endereco, "\n")] = 0;
                break;
            case 5:
                printf("Novo Telefone: ");
                fgets(fornecedor->telefone, sizeof(fornecedor->telefone), stdin);
                fornecedor->telefone[strcspn(fornecedor->telefone, "\n")] = 0;
                break;
            case 6:
                printf("Novo Tipo de Servico: ");
                fgets(fornecedor->tipo_servico, sizeof(fornecedor->tipo_servico), stdin);
                fornecedor->tipo_servico[strcspn(fornecedor->tipo_servico, "\n")] = 0;
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                pausar();
        }
    } while (opcao != 0);

    salvarFornecedores(sistema);
}

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