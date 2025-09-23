#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

// Limpa o buffer de entrada do teclado.
void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Pausa a execução do programa até que o usuário pressione Enter.
void pausar()
{
    printf("\nPressione Enter para continuar...");
    getchar();
}

// Limpa a tela do console.
void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}