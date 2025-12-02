#ifndef SISTEMA_H
#define SISTEMA_H

// --- TIPOS DE ARMAZENAMENTO ---
typedef enum { MEMORIA, ARQUIVO_TEXTO, ARQUIVO_BINARIO } TipoArmazenamento;

// --- STATUS E ENUMS ---
typedef enum { ORCAMENTO, APROVADO, FINALIZADO, CANCELADO } StatusEvento;
typedef enum { CONTA_A_RECEBER, CONTA_A_PAGAR, MOVIMENTACAO_CAIXA } TipoTransacao;
typedef enum { PENDENTE, PAGA } StatusTransacao;

// --- ESTRUTURAS BASICAS ---
typedef struct {
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

typedef struct {
    int codigo;
    char razao_social[100];
    char cnpj[20];
    char endereco[150];
    char telefone[20];
    char email[50];
    char nome_do_contato[100];
} Cliente;

typedef struct {
    int codigo;
    char nome[100];
    char cpf[15];
    char funcao[50];
    float valor_diaria;
} EquipeInterna;

typedef struct {
    int codigo;
    char descricao[150];
    char categoria[50];
    int quantidade_estoque;
    float preco_custo;
    float valor_locacao;
} Recurso;

typedef struct {
    int codigo;
    char nome_fantasia[100];
    char razao_social[100];
    char cnpj[20];
    char endereco[150];
    char telefone[20];
    char tipo_servico[100];
    float valor_servico;
} Fornecedor;

typedef struct {
    int codigo;
    char nome[100];
    char usuario[50];
    char senha[50];
} Operador;

// --- ESTRUTURAS DE EVENTO (LISTAS INTERNAS) ---
typedef struct {
    int codigo_recurso;
    int quantidade;
    float custo_locacao_momento;
} ItemRecursoEvento;

typedef struct {
    int codigo_equipe;
    float custo_diaria_momento;
} ItemEquipeEvento;

// Estrutura pro fornecedor dentro do evento
typedef struct {
    int codigo_fornecedor;
    float valor_cobrado; // Quanto custou esse servico
} ItemFornecedorEvento;

// --- ESTRUTURAS PRINCIPAIS (EVENTO E TRANSACAO) ---
typedef struct {
    int codigo;
    char nome_evento[150];
    int codigo_cliente;
    StatusEvento status;

    char data_inicio[15];
    char hora_inicio[6];
    char data_fim[15];
    char hora_fim[6];
    char local[150];

    // Listas dinamicas dentro do evento
    ItemRecursoEvento *lista_recursos_alocados;
    int num_recursos_alocados;
    
    ItemEquipeEvento *lista_equipe_alocada;
    int num_equipe_alocada;

    //Lista de fornecedores no evento
    ItemFornecedorEvento *lista_fornecedores_alocados;
    int num_fornecedores_alocados;

    float custo_total_previsto;
    float valor_final_faturado;
} Evento;

typedef struct {
    int codigo;
    TipoTransacao tipo;
    StatusTransacao status;
    char descricao[150];
    float valor;
    char data_vencimento[15];
    char data_pagamento[15];
    
    int codigo_cliente_ref;
    int codigo_evento_ref;
} Transacao;

// --- SISTEMA GERAL ---
typedef struct Sistema {
    TipoArmazenamento modo_de_armazenamento;

    Produtora *dados_produtora;

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

    Evento *lista_eventos;
    int num_eventos;
    int capacidade_eventos;

    Transacao *lista_transacoes;
    int num_transacoes;
    int capacidade_transacoes;

    float saldo_caixa;
} Sistema;

#endif