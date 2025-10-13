// Inclui os cabeçalhos necessários para a execução do programa.
// stdio.h para funções de entrada e saída, como printf e scanf.
// stdlib.h para funções de alocação de memória, como malloc e free.
// string.h para funções de manipulação de strings, como memset.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclui a definição da estrutura principal do sistema, que armazena todos os dados.
#include "model/sistema.h"
// Inclui as funções do controller principal, que gerenciam o fluxo inicial do programa.
#include "controller/main/main_controller.h"
// Inclui as funções da view principal, que exibem os menus e interagem com o usuário.
#include "view/main/main_view.h"

// Função principal que inicia a execução do programa.
int main() {
    // Declara a variável 'sistema', que é a estrutura central que conterá todos os dados do programa.
    Sistema sistema;
    // Zera a memória da estrutura 'sistema' para garantir que todos os campos comecem com valores nulos ou zero.
    // Isso evita problemas com lixo de memória que poderiam causar comportamentos inesperados.
    memset(&sistema, 0, sizeof(Sistema));

    // Chama a função do controller para realizar a configuração inicial do sistema.
    // Esta função pergunta ao usuário como ele deseja armazenar os dados (memória, arquivo de texto ou binário).
    configuracaoInicialController(&sistema);
    // Após a configuração, esta função carrega todos os dados existentes do modo de armazenamento escolhido.
    carregarTodosOsDadosController(&sistema);

    // Exibe o menu principal da aplicação, permitindo que o usuário navegue pelas funcionalidades do sistema.
    // Esta é a função que mantém o programa em execução, esperando pelas ações do usuário.
    menuPrincipalView(&sistema);

    // Antes de encerrar o programa, esta função é chamada para liberar toda a memória que foi alocada dinamicamente.
    // Isso é crucial para evitar vazamentos de memória (memory leaks).
    liberarTodaMemoriaController(&sistema);

    // Imprime uma mensagem de despedida para o usuário.
    printf("\nSaindo do programa...\n");
    // Retorna 0 para o sistema operacional, indicando que o programa foi encerrado com sucesso.
    return 0;
}