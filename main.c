#include <stdio.h>
#include <string.h>
#include "model/sistema.h"
#include "controller/main/main_controller.h"
#include "view/main/main_view.h"
#include "controller/login/login_controller.h" 

int main() {
    Sistema sistema;
    // Zera a memoria da estrutura sistema para evitar lixo
    memset(&sistema, 0, sizeof(Sistema)); 

    // Configura se vai usar .txt ou binario
    configuracaoInicialController(&sistema);
    
    // Carrega tudo do disco para a RAM
    carregarTodosOsDadosController(&sistema);

    // Tenta logar
    if (realizarLoginOperador(&sistema)) {
        // Se logou, mostra menu
        menuPrincipalView(&sistema);
    } else {
        printf("\nLogin falhou ou cancelado. Encerrando...\n");
    }

    // Limpa a memoria RAM antes de fechar
    liberarTodaMemoriaController(&sistema);

    return 0;
}