// Impede que o cabeçalho seja incluído múltiplas vezes no mesmo arquivo,
// o que causaria erros de compilação por redefinição de tipos.
#ifndef SISTEMA_H
#define SISTEMA_H

// Inclui o cabeçalho de configuração de armazenamento, que define
// a enumeração 'TipoArmazenamento' usada para controlar a persistência dos dados.
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

// Estrutura central que agrega todos os dados do sistema em um único lugar.
// Um ponteiro para esta estrutura é passado para a maioria das funções,
// permitindo que elas acessem e modifiquem o estado completo da aplicação.
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

} Sistema;

// Finaliza a diretiva de pré-processador para evitar inclusão múltipla.
#endif