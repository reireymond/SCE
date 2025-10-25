#include <stdio.h>   // Entrada e saída padrão
#include <stdlib.h>  // Funções como system()
#include "utils.h"   // Declarações das funções implementadas aqui

// Limpa o buffer de entrada (stdin) para evitar problemas após scanf
void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // Corpo vazio, apenas descarta caracteres
}

// Pausa a execução do programa até o usuário pressionar Enter
void pausar()
{
    printf("\nPressione Enter para continuar...");
    getchar();
}

// Limpa a tela do console
void limpar_tela() {
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // Linux/macOs
    #endif
}
