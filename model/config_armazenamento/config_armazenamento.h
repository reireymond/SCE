// Garante que o conteúdo deste arquivo de cabeçalho seja incluído apenas uma vez
// durante a compilação de um arquivo de código-fonte, evitando erros de redefinição.
#ifndef CONFIG_ARMAZENAMENTO_H
#define CONFIG_ARMAZENAMENTO_H

// Define uma enumeração (um conjunto de constantes nomeadas) para representar
// os diferentes tipos de armazenamento que o sistema pode utilizar.
typedef enum
{
    MEMORIA,         // Os dados são mantidos apenas na memória RAM e são perdidos ao fechar o programa.
    ARQUIVO_TEXTO,   // Os dados são salvos em arquivos de texto legíveis por humanos (.txt).
    ARQUIVO_BINARIO  // Os dados são salvos em arquivos binários (.dat), que são mais rápidos e compactos.
} TipoArmazenamento;

// Declaração antecipada da estrutura 'Sistema'.
// Isso informa ao compilador que 'Sistema' é uma estrutura que será definida em outro lugar.
// É necessário porque as funções abaixo usam um ponteiro para 'Sistema' como parâmetro,
// mas a definição completa de 'Sistema' depende deste arquivo, criando uma dependência circular.
struct Sistema;

// Declaração da função que define o modo de armazenamento de dados.
// Ela recebe um ponteiro para a estrutura principal do sistema e o modo desejado.
void definirModoDeArmazenamento(struct Sistema *sistema, TipoArmazenamento modo);

// Declaração da função que obtém o modo de armazenamento atualmente configurado.
// Ela recebe um ponteiro para a estrutura do sistema e retorna o modo de armazenamento ativo.
TipoArmazenamento obterModoDeArmazenamento(struct Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif