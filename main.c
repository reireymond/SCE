#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/sistema.h"
#include "controller/main/main_controller.h"
#include "view/main/main_view.h"

int main() {
    Sistema sistema;
    memset(&sistema, 0, sizeof(Sistema));

    configuracaoInicialController(&sistema);
    carregarTodosOsDadosController(&sistema);

    menuPrincipalView(&sistema);

    liberarTodaMemoriaController(&sistema);

    printf("\nSaindo do programa...\n");
    return 0;
}