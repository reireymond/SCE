// Inclui o cabeçalho correspondente a este arquivo de implementação,
// garantindo que a implementação das funções corresponda às suas declarações.
#include "model/config_armazenamento/config_armazenamento.h"
// Inclui a definição completa da estrutura 'Sistema', necessária para acessar seus campos.
#include "model/sistema.h"

// Implementação da função para definir o modo de armazenamento.
// Esta função atualiza o campo 'modo_de_armazenamento' na estrutura principal do sistema.
void definirModoDeArmazenamento(Sistema *sistema, TipoArmazenamento modo)
{
   // Verifica se o ponteiro para a estrutura do sistema não é nulo antes de usá-lo.
   // Esta é uma boa prática para evitar erros de "segmentation fault".
   if (sistema) {
       // Atribui o modo de armazenamento recebido como parâmetro ao campo correspondente na estrutura 'sistema'.
       sistema->modo_de_armazenamento = modo;
   }
}

// Implementação da função para obter o modo de armazenamento atual.
// Esta função lê e retorna o valor do campo 'modo_de_armazenamento' da estrutura do sistema.
TipoArmazenamento obterModoDeArmazenamento(Sistema *sistema)
{
   // Verifica se o ponteiro para a estrutura do sistema é válido.
   if (sistema) {
       // Retorna o modo de armazenamento que está atualmente configurado.
       return sistema->modo_de_armazenamento;
   }
   // Caso o ponteiro 'sistema' seja nulo (o que indicaria um erro de programação),
   // retorna um modo padrão (ARQUIVO_BINARIO) para evitar comportamento indefinido.
   return ARQUIVO_BINARIO;
}