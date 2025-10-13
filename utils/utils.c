// Inclui os cabeçalhos padrão do C para entrada/saída (stdio) e
// para a função 'system' (stdlib), que executa comandos do sistema operacional.
#include <stdio.h>
#include <stdlib.h>
// Inclui o cabeçalho correspondente que contém as declarações das funções aqui implementadas.
#include "utils.h"

// Implementação da função para limpar o buffer de entrada.
// Esta função é essencial após leituras com scanf para remover o caractere
// de nova linha ('\n') ou outros caracteres que sobram no buffer.
void limpar_buffer()
{
    // Declara uma variável para armazenar cada caractere lido.
    int c;
    // Loop que continua lendo caracteres do buffer de entrada (stdin)
    // até encontrar uma nova linha ('\n') ou o fim do arquivo (EOF).
    // O caractere lido é descartado, efetivamente esvaziando o buffer.
    while ((c = getchar()) != '\n' && c != EOF)
        ; // O ponto e vírgula indica um corpo de loop vazio.
}

// Implementação da função para pausar a execução do programa.
void pausar()
{
    // Exibe uma mensagem instruindo o usuário a pressionar Enter para continuar.
    printf("\nPressione Enter para continuar...");
    // A função getchar() aguarda que o usuário digite um caractere (neste caso, Enter)
    // para que o programa prossiga.
    getchar();
}

// Implementação da função para limpar a tela do console.
void limpar_tela() {
    // A diretiva de pré-processador '#ifdef' verifica se a macro '_WIN32' está definida.
    // Esta macro é comumente definida por compiladores em ambientes Windows.
    #ifdef _WIN32
        // Se estiver no Windows, executa o comando 'cls' para limpar a tela.
        system("cls");
    // Se a macro '_WIN32' não estiver definida, o código dentro do '#else' é compilado.
    #else
        // Em outros sistemas (como Linux ou macOS), executa o comando 'clear'.
        system("clear");
    #endif
}