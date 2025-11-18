// Impede incluir o arquivo mais de uma vez
#ifndef SISTEMA_H
#define SISTEMA_H

#include "model/config_armazenamento/config_armazenamento.h"

// Dados da produtora de eventos
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

// Dados de um cliente
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

// Dados de um membro da equipe interna
typedef struct
{
    int codigo;          
    char nome[100];      
    char cpf[15];        
    char funcao[50];     
    float valor_diaria;  
} EquipeInterna;

// Dados de um recurso ou equipamento
typedef struct
{
    int codigo;                 
    char descricao[150];        
    char categoria[50];         
    int quantidade_estoque;     
    float preco_custo;          
    float valor_locacao;        
} Recurso;

// Dados de um fornecedor
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

// Dados de um operador do sistema
typedef struct
{
    int codigo;       
    char nome[100];    
    char usuario[50];  
    char senha[50];    
} Operador;

// Status do evento
typedef enum {
    ORCAMENTO,
    APROVADO,
    FINALIZADO,
    CANCELADO // Novo status adicionado
} StatusEvento;

// Tipos de transacao
typedef enum {
    CONTA_A_RECEBER,
    CONTA_A_PAGAR,
    MOVIMENTACAO_CAIXA
} TipoTransacao;

// Status da transacao
typedef enum {
    PENDENTE,
    PAGA
} StatusTransacao;

// Struct para armazenar um recurso alocado em um evento
typedef struct {
    int codigo_recurso;
    int quantidade;
    float custo_locacao_momento; 
} ItemRecursoEvento;

// Struct para armazenar um membro da equipe alocado
typedef struct {
    int codigo_equipe;
    float custo_diaria_momento; 
} ItemEquipeEvento;

// Struct para armazenar um serviço de fornecedor contratado
typedef struct {
    int codigo_fornecedor;
    char descricao_servico[150]; 
    float custo_contratado;
} ItemFornecedorEvento;

// Define a estrutura principal para um Evento
typedef struct {
    int codigo;
    char nome_evento[150];
    int codigo_cliente;
    StatusEvento status; 

    char data_inicio[15]; 
    char data_fim[15];    
    char local[150];

    ItemRecursoEvento *lista_recursos_alocados;
    int num_recursos_alocados;
    int capacidade_recursos_alocados;

    ItemEquipeEvento *lista_equipe_alocada;
    int num_equipe_alocada;
    int capacidade_equipe_alocada;

    ItemFornecedorEvento *lista_servicos_contratados;
    int num_servicos_contratados;
    int capacidade_servicos_contratados;

    float custo_total_previsto; 
    float valor_final_faturado; 

} Evento;

// Define a estrutura para uma transação financeira
typedef struct {
    int codigo;
    TipoTransacao tipo;     
    StatusTransacao status; 
    char descricao[150];
    float valor;
    char data_vencimento[15]; 
    char data_pagamento[15];  

    int codigo_cliente_ref;
    int codigo_fornecedor_ref;
    int codigo_evento_ref;

} Transacao;

// Estrutura central com todos os dados do sistema
typedef struct Sistema
{
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