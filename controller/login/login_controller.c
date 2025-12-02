#include "login_controller.h"
#include <string.h>
#include "controller/operador/operador_controller.h" 
#include "view/operador/operador_view.h" 
static Operador* buscarOperadorPorUsuario(Sistema *sistema, const char *usuario) {
    for(int i=0; i < sistema->num_operadores; i++) {
        if(strcmp(sistema->lista_operadores[i].usuario, usuario) == 0) {
            return &sistema->lista_operadores[i];
        }
    }
    return NULL;
}

int realizarLoginOperador(Sistema *sistema) {
    if (sistema->num_operadores == 0) {
        mensagem_primeiro_acesso(); 
        adicionarOperadorController(sistema); 
    }

    char u[50], s[50];
    int tentativas = 0;
    
    while (tentativas < 3) {
        tela_login(u, s);
        
        Operador *op = buscarOperadorPorUsuario(sistema, u);
        
        if (op && strcmp(op->senha, s) == 0) {
            mensagem_login_sucesso(op->nome);
            return 1;
        } else {
            tentativas++;
            mensagem_login_falhou(tentativas, 3);
        }
    }
    return 0;
}