// Impede que o cabeçalho seja incluído múltiplas vezes no mesmo arquivo,
#ifndef SISTEMA_H
#define SISTEMA_H

// Inclui o cabeçalho de configuração de armazenamento, que define
#include "model/config_armazenamento/config_armazenamento.h"

// Define a estrutura para armazenar os dados da empresa produtora de eventos.
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

// Define a estrutura para armazenar os dados de um cliente.
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

// Define a estrutura para armazenar os dados de um membro da equipe interna da produtora.
typedef struct
{
    int codigo;          
    char nome[100];      
    char cpf[15];        
    char funcao[50];     
    float valor_diaria;  
} EquipeInterna;

// Define a estrutura para armazenar os dados de um recurso ou equipamento da produtora.
typedef struct
{
    int codigo;                 
    char descricao[150];        
    char categoria[50];         
    int quantidade_estoque;     
    float preco_custo;          
    float valor_locacao;        
} Recurso;

// Define a estrutura para armazenar os dados de um fornecedor ou parceiro terceirizado.
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

// Define a estrutura para armazenar os dados de um operador que utilizará o sistema.
typedef struct
{
    int codigo;       
    char nome[100];    
    char usuario[50];  
    char senha[50];    
} Operador;

// Enum para o status do evento
typedef enum {
    ORCAMENTO,
    APROVADO,
    FINALIZADO
} StatusEvento;

// Enum para tipos de transação
typedef enum {
    CONTA_A_RECEBER,
    CONTA_A_PAGAR,
    MOVIMENTACAO_CAIXA
} TipoTransacao;

// Enum para status da transação
typedef enum {
    PENDENTE,
    PAGA
} StatusTransacao;

// Struct para armazenar um recurso alocado em um evento
typedef struct {
    int codigo_recurso;
    int quantidade;
    float custo_locacao_momento; // Preço no momento do orçamento
} ItemRecursoEvento;

// Struct para armazenar um membro da equipe alocado
typedef struct {
    int codigo_equipe;
    float custo_diaria_momento; // Custo no momento do orçamento
} ItemEquipeEvento;

// Struct para armazenar um serviço de fornecedor contratado
typedef struct {
    int codigo_fornecedor;
    char descricao_servico[150]; // Descrição do serviço contratado
    float custo_contratado;
} ItemFornecedorEvento;

// Define a estrutura principal para um Evento (que começa como Orçamento)
typedef struct {
    int codigo;
    char nome_evento[150];
    int codigo_cliente;
    StatusEvento status; // ORCAMENTO, APROVADO, FINALIZADO

    char data_inicio[15]; // Formato "dd/mm/aaaa"
    char data_fim[15];    // Formato "dd/mm/aaaa"
    char local[150];

    // Listas dinâmicas dos itens alocados
    ItemRecursoEvento *lista_recursos_alocados;
    int num_recursos_alocados;
    int capacidade_recursos_alocados;

    ItemEquipeEvento *lista_equipe_alocada;
    int num_equipe_alocada;
    int capacidade_equipe_alocada;

    ItemFornecedorEvento *lista_servicos_contratados;
    int num_servicos_contratados;
    int capacidade_servicos_contratados;

    // Custos e Faturamento
    float custo_total_previsto; // Soma de todos os custos
    float valor_final_faturado; // Valor a ser cobrado do cliente

} Evento;

// Define a estrutura para uma transação financeira
typedef struct {
    int codigo;
    TipoTransacao tipo;     // CONTA_A_RECEBER, CONTA_A_PAGAR, MOVIMENTACAO_CAIXA
    StatusTransacao status; // PENDENTE, PAGA
    char descricao[150];
    float valor;
    char data_vencimento[15]; // "dd/mm/aaaa"
    char data_pagamento[15];  // "dd/mm/aaaa" (preenchido na baixa)

    // Códigos para vincular a transação (ex: ao cliente ou fornecedor)
    int codigo_cliente_ref;
    int codigo_fornecedor_ref;
    int codigo_evento_ref;

} Transacao;

// Estrutura central que agrega todos os dados do sistema em um único lugar.
typedef struct Sistema
{
    // Armazena o modo de persistência de dados escolhido pelo usuário (Memória, Arquivo Texto ou Binário).
    TipoArmazenamento modo_de_armazenamento;

    // Ponteiro para os dados da produtora. Apenas uma produtora pode ser cadastrada.
    Produtora *dados_produtora;

    // Ponteiro para a lista dinâmica de clientes cadastrados.
    Cliente *lista_clientes;
    // Número atual de clientes na lista.
    int num_clientes;
    // Capacidade total do array 'lista_clientes', usada para gerenciar o crescimento da lista.
    int capacidade_clientes;

    // Ponteiro para a lista dinâmica de membros da equipe.
    EquipeInterna *lista_equipe;
    // Número atual de membros na lista.
    int num_equipe;
    // Capacidade total do array 'lista_equipe'.
    int capacidade_equipe;

    // Ponteiro para a lista dinâmica de recursos e equipamentos.
    Recurso *lista_recursos;
    // Número atual de recursos na lista.
    int num_recursos;
    // Capacidade total do array 'lista_recursos'.
    int capacidade_recursos;

    // Ponteiro para a lista dinâmica de fornecedores e parceiros.
    Fornecedor *lista_fornecedores;
    // Número atual de fornecedores na lista.
    int num_fornecedores;
    // Capacidade total do array 'lista_fornecedores'.
    int capacidade_fornecedores;

    // Ponteiro para a lista dinâmica de operadores do sistema.
    Operador *lista_operadores;
    // Número atual de operadores na lista.
    int num_operadores;
    // Capacidade total do array 'lista_operadores'.
    int capacidade_operadores;

    // Ponteiro para a lista dinâmica de eventos/orçamentos
    Evento *lista_eventos;
    int num_eventos;
    int capacidade_eventos;

    // Ponteiro para a lista dinâmica de transações financeiras
    Transacao *lista_transacoes;
    int num_transacoes;
    int capacidade_transacoes;

    // Saldo de caixa atual
    float saldo_caixa;

} Sistema;

#endif