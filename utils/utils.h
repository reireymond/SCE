// Garante que este arquivo de cabeçalho seja incluído apenas uma vez
// durante a compilação, prevenindo erros de redefinição de funções.
#ifndef UTILS_H
#define UTILS_H

// Declaração da função para limpar o buffer de entrada do teclado.
// É útil para consumir caracteres extras que permanecem no buffer após
// uma leitura com scanf, evitando que eles interfiram na próxima leitura.
void limpar_buffer();

// Declaração da função para pausar a execução do programa.
// Exibe uma mensagem e aguarda que o usuário pressione a tecla Enter
// para continuar, melhorando a usabilidade em consoles.
void pausar();

// Declaração da função para limpar a tela do console.
// Esta função executa um comando do sistema operacional ('cls' no Windows,
// 'clear' em outros sistemas) para limpar o conteúdo da tela.
void limpar_tela();

// Fim da diretiva de inclusão condicional.
#endif