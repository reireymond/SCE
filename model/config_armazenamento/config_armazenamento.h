// Garante que o conteúdo deste arquivo de cabeçalho seja incluído apenas uma vez
#ifndef CONFIG_ARMAZENAMENTO_H
#define CONFIG_ARMAZENAMENTO_H

// os diferentes tipos de armazenamento que o sistema pode utilizar.
typedef enum
{
    MEMORIA,         // Os dados são mantidos apenas na memória RAM e são perdidos ao fechar o programa.
    ARQUIVO_TEXTO,   // Os dados são salvos em arquivos de texto legíveis por humanos (.txt).
    ARQUIVO_BINARIO  // Os dados são salvos em arquivos binários (.dat), que são mais rápidos e compactos.
} TipoArmazenamento;

// Declaração antecipada da estrutura 'Sistema'.
// Isso informa ao compilador que 'Sistema' é uma estrutura que será definida em outro lugar.
struct Sistema;

// Declaração da função que define o modo de armazenamento de dados.
void definirModoDeArmazenamento(struct Sistema *sistema, TipoArmazenamento modo);

// Declaração da função que obtém o modo de armazenamento atualmente configurado.
TipoArmazenamento obterModoDeArmazenamento(struct Sistema *sistema);


#endif