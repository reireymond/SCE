#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model/sistema.h"
#include "controller/main/main_controller.h"
#include "view/main/main_view.h"
#include "controller/login/login_controller.h" // Incluir o novo controller de login

int main() {
    Sistema sistema;
    memset(&sistema, 0, sizeof(Sistema)); // Inicializa a estrutura

    configuracaoInicialController(&sistema); // Configuração inicial do sistema
    carregarTodosOsDadosController(&sistema); // Carrega dados existentes

    // --- Início da Modificação ---
    // Chama a função de login APÓS carregar os dados
    int login_sucesso = realizarLoginOperador(&sistema);

    // Só exibe o menu principal se o login for bem-sucedido
    if (login_sucesso) {
        menuPrincipalView(&sistema); // Menu principal do programa
    }
    // --- Fim da Modificação ---

    liberarTodaMemoriaController(&sistema); // Libera memória alocada

    // Mensagem de saída movida para depois do login (ou falha)
    if (!login_sucesso) {
         printf("\nPrograma encerrado devido a falha no login.\n");
    } else {
        printf("\nSaindo do programa...\n");
    }


    return 0;
}