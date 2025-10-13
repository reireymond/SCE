#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model/sistema.h"
#include "controller/main/main_controller.h"
#include "view/main/main_view.h"

int main() {
    Sistema sistema;
    memset(&sistema, 0, sizeof(Sistema)); // Inicializa a estrutura

    configuracaoInicialController(&sistema); // Configuração inicial do sistema
    carregarTodosOsDadosController(&sistema); // Carrega dados existentes

    menuPrincipalView(&sistema); // Menu principal do programa

    liberarTodaMemoriaController(&sistema); // Libera memória alocada
    printf("\nSaindo do programa...\n");

    return 0;
}
