#include "model/config_armazenamento/config_armazenamento.h"
#include "model/sistema.h"

// Define o modo de armazenamento de dados do sistema.
void definirModoDeArmazenamento(Sistema *sistema, TipoArmazenamento modo)
{
   if (sistema) {
       sistema->modo_de_armazenamento = modo;
   }
}

// Obtém o modo de armazenamento de dados atualmente configurado.
TipoArmazenamento obterModoDeArmazenamento(Sistema *sistema)
{
   if (sistema) {
       return sistema->modo_de_armazenamento;
   }
   // Retorna um padrão em caso de erro.
   return ARQUIVO_BINARIO;
}