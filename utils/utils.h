// Evita múltiplas inclusões do arquivo, prevenindo redefinições de funções
#ifndef UTILS_H
#define UTILS_H

// Limpa o buffer de entrada para evitar problemas após scanf
void limpar_buffer();

// Pausa a execução até o usuário pressionar Enter
void pausar();

// Limpa a tela do console (Windows: cls, Linux/macOS: clear)
void limpar_tela();

#endif // UTILS_H
