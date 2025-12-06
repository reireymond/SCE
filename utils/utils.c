#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; 
}

void pausar()
{
    printf("\nPressione Enter para continuar...");
    getchar();
}

void limpar_tela() {
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // Linux/macOs
    #endif
}

void mensagem_sucesso(const char *msg) {
    printf("\n[SUCESSO] %s\n", msg);
}

void mensagem_erro(const char *msg) {
    printf("\n[ERRO] %s\n", msg);
}

void mensagem_aviso(const char *msg) {
    printf("\n[AVISO] %s\n", msg);
}
