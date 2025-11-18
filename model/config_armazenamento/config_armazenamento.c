// correspondente, garantindo que as funções estejam de acordo com suas declarações.
#include "model/config_armazenamento/config_armazenamento.h"
// Inclui a estrutura 'Sistema' para acessar e modificar seus campos.
#include "model/sistema.h"

// Define o modo de armazenamento do sistema (memória, texto ou binário)
void definirModoDeArmazenamento(Sistema *sistema, TipoArmazenamento modo)
{
   // Evita acesso nulo
   if (sistema) {
       sistema->modo_de_armazenamento = modo;
   }
}

// Retorna o modo de armazenamento atualmente configurado
TipoArmazenamento obterModoDeArmazenamento(Sistema *sistema)
{
   if (sistema) {
       return sistema->modo_de_armazenamento;
   }
   // Retorna um modo padrão caso o ponteiro seja inválido
   return ARQUIVO_BINARIO;
}
