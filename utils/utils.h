#ifndef UTILS_H
#define UTILS_H

// Limpa o buffer de entrada para evitar problemas após scanf
void limpar_buffer();

// Pausa a execução até o usuário pressionar Enter
void pausar();

// Limpa a tela do console
void limpar_tela();
void mensagem_sucesso(const char *msg);
void mensagem_erro(const char *msg);
void mensagem_aviso(const char *msg);

#endif 
