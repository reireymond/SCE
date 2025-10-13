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
    char nome_fantasia[100];       // Nome comercial da empresa.
    char razao_social[100];        // Nome legal da empresa.
    char nome_do_responsavel[100]; // Nome do contato principal ou proprietário.
    char cnpj[20];                 // Cadastro Nacional da Pessoa Jurídica.
    char inscricao_estadual[20];   // Registro fiscal no estado.
    char endereco[150];            // Endereço completo da empresa.
    char telefone[20];             // Telefone principal da empresa.
    char telefone_responsavel[20]; // Telefone do responsável.
    char email[50];                // E-mail de contato da empresa.
    float margem_lucro;            // Percentual de lucro padrão a ser aplicado nos orçamentos.
} Produtora;

// Define a estrutura para armazenar os dados de um cliente.
typedef struct
{
    int codigo;                    // Identificador numérico único para cada cliente.
    char razao_social[100];        // Nome ou razão social do cliente.
    char cnpj[20];                 // CPF ou CNPJ do cliente.
    char endereco[150];            // Endereço completo do cliente.
    char telefone[20];             // Telefone de contato do cliente.
    char email[50];                // E-mail de contato do cliente.
    char nome_do_contato[100];     // Nome da pessoa de referência no cliente.
} Cliente;

// Define a estrutura para armazenar os dados de um membro da equipe interna da produtora.
typedef struct
{
    int codigo;          // Identificador numérico único para cada membro da equipe.
    char nome[100];      // Nome completo do funcionário.
    char cpf[15];        // Cadastro de Pessoa Física do funcionário.
    char funcao[50];     // Cargo ou função do funcionário (ex: Técnico de Som).
    float valor_diaria;  // Custo da diária de trabalho do funcionário.
} EquipeInterna;

// Define a estrutura para armazenar os dados de um recurso ou equipamento da produtora.
typedef struct
{
    int codigo;                 // Identificador numérico único para cada recurso.
    char descricao[150];        // Descrição detalhada do equipamento (ex: Caixa de Som Ativa).
    char categoria[50];         // Categoria do equipamento (ex: Sonorização, Iluminação).
    int quantidade_estoque;     // Quantidade de unidades deste item disponíveis no estoque.
    float preco_custo;          // Valor de aquisição do equipamento.
    float valor_locacao;        // Preço da locação por diária.
} Recurso;

// Define a estrutura para armazenar os dados de um fornecedor ou parceiro terceirizado.
typedef struct
{
    int codigo;                 // Identificador numérico único para cada fornecedor.
    char nome_fantasia[100];    // Nome comercial do fornecedor.
    char razao_social[100];     // Razão social do fornecedor.
    char cnpj[20];              // CPF ou CNPJ do fornecedor.
    char endereco[150];         // Endereço completo do fornecedor.
    char telefone[20];          // Telefone de contato do fornecedor.
    char tipo_servico[100];     // Tipo de serviço prestado (ex: Buffet, Segurança).
} Fornecedor;

// Define a estrutura para armazenar os dados de um operador que utilizará o sistema.
typedef struct
{
    int codigo;        // Identificador numérico único para cada operador.
    char nome[100];    // Nome do operador.
    char usuario[50];  // Nome de usuário para login no sistema.
    char senha[50];    // Senha para login no sistema.
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