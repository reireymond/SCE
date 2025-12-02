#include "controller/produtora/produtora_controller.h"
#include <stdlib.h>
#include "model/produtora/produtora_model.h"
#include "view/produtora/produtora_view.h"

void adicionarProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora != NULL) {
        mensagem_erro("Ja existe uma produtora cadastrada.");
        return;
    }

    sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
    if (sistema->dados_produtora == NULL) {
        mensagem_erro("Falha ao alocar memoria.");
        return;
    }

    // O Controller pede pra View preencher os dados no ponteiro que ele criou
    formulario_produtora(sistema->dados_produtora);

    salvarProdutora(sistema);
    mensagem_sucesso("Produtora cadastrada!");
}

void alterarProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        mensagem_erro("Nenhuma produtora para alterar.");
        return;
    }

    int op;
    do {
        
        op = menu_alterar_campo_produtora(sistema->dados_produtora);
    } while (op != 0);

    salvarProdutora(sistema);
    mensagem_sucesso("Dados atualizados.");
}

void excluirProdutoraController(Sistema *sistema) {
    if (sistema->dados_produtora == NULL) {
        mensagem_erro("Nada para excluir.");
        return;
    }
    
    free(sistema->dados_produtora);
    sistema->dados_produtora = NULL;
    salvarProdutora(sistema);
    mensagem_sucesso("Produtora excluida.");
}