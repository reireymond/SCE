#ifndef SISTEMA_H
#define SISTEMA_H

#include "model/config_armazenamento/config_armazenamento.h"

// Estrutura para armazenar os dados da produtora.
typedef struct
{
    char nome_fantasia[100];
    char razao_social[100];
    char nome_do_responsavel[100];
    char cnpj[20];
    char inscricao_estadual[20];
    char endereco[150];
    char telefone[20];
    char telefone_responsavel[20];
    char email[50];
    float margem_lucro;
} Produtora;

// Estrutura para armazenar os dados de um cliente.
typedef struct
{
    int codigo;
    char razao_social[100];
    char cnpj[20];
    char endereco[150];
    char telefone[20];
    char email[50];
    char nome_do_contato[100];
} Cliente;

// Estrutura para armazenar os dados de um membro da equipe interna.
typedef struct
{
    int codigo;
    char nome[100];
    char cpf[15];
    char funcao[50];
    float valor_diaria;
} EquipeInterna;

// Estrutura para armazenar os dados de um recurso ou equipamento.
typedef struct
{
    int codigo;
    char descricao[150];
    char categoria[50];
    int quantidade_estoque;
    float preco_custo;
    float valor_locacao;
} Recurso;

// Estrutura para armazenar os dados de um fornecedor ou parceiro.
typedef struct
{
    int codigo;
    char nome_fantasia[100];
    char razao_social[100];
    char cnpj[20];
    char endereco[150];
    char telefone[20];
    char tipo_servico[100];
} Fornecedor;

// Estrutura para armazenar os dados de um operador do sistema.
typedef struct
{
    int codigo;
    char nome[100];
    char usuario[50];
    char senha[50];
} Operador;

// Estrutura central que armazena todos os dados do sistema.
typedef struct
{
    // Configuração do modo de armazenamento
    TipoArmazenamento modo_de_armazenamento;

    // Dados da Produtora
    Produtora *dados_produtora;

    // Listas de cadastros
    Cliente *lista_clientes;
    int num_clientes;
    int capacidade_clientes;

    EquipeInterna *lista_equipe;
    int num_equipe;
    int capacidade_equipe;

    Recurso *lista_recursos;
    int num_recursos;
    int capacidade_recursos;

    Fornecedor *lista_fornecedores;
    int num_fornecedores;
    int capacidade_fornecedores;

    Operador *lista_operadores;
    int num_operadores;
    int capacidade_operadores;

} Sistema;

#endif