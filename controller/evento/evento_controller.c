#include "evento_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/evento/evento_model.h"
#include "view/evento/evento_view.h"
#include "utils/utils.h"
#include "utils/validation.h"
#include "model/sistema.h"
#include "view/cliente/cliente_view.h"
#include "view/recurso/recurso_view.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "controller/transacao/transacao_controller.h"
// Isso facilita saber se um evento acontece antes ou depois de outro
long long transformarDataHoraEmNumero(char* data, char* hora) {
    int dia, mes, ano, h, m;
    // Zera as variaveis pra garantir
    dia=0; mes=0; ano=0; h=0; m=0;
    
    // Pega os numeros do texto
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    sscanf(hora, "%d:%d", &h, &m);
    
    // Matematica: joga cada parte pra sua casa decimal correta
    // Ex: 2025 01 01 14 30
    long long resultado = (long long)ano * 100000000; 
    resultado += mes * 1000000;                       
    resultado += dia * 10000;                         
    resultado += h * 100;                             
    resultado += m;                                   
    
    return resultado;
}

void adicionarEventoController(Sistema *sistema) {
    // Verificacao de seguranca: Nao deixa criar evento sem cliente
    if (sistema->num_clientes == 0) {
        printf("\nAtencao: Nao ha clientes cadastrados no sistema.\n");
        printf("Por favor, cadastre um cliente antes de criar um orcamento.\n");
        return; // Sai da funcao e volta pro menu
    }

    // Verifica se tem espaco na memoria
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        int nova_capacidade = (sistema->capacidade_eventos == 0) ? 10 : sistema->capacidade_eventos * 2;
        sistema->lista_eventos = realloc(sistema->lista_eventos, nova_capacidade * sizeof(Evento));
        sistema->capacidade_eventos = nova_capacidade;
    }

    Evento *novo = &sistema->lista_eventos[sistema->num_eventos];
    // Limpa memoria
    memset(novo, 0, sizeof(Evento));
    
    novo->codigo = sistema->num_eventos + 1;
    novo->status = ORCAMENTO;

    printf("\n--- Novo Orcamento (ID: %d) ---\n", novo->codigo);
    
    printf("Nome do Evento: "); 
    ler_texto_valido(novo->nome_evento, 150, VALIDAR_NAO_VAZIO);
    
    listarClientesView(sistema);
    printf("Codigo do Cliente: "); 
    ler_inteiro_valido(&novo->codigo_cliente, 1, 99999);
    
    printf("Data Inicio (DD/MM/AAAA): "); 
    ler_texto_valido(novo->data_inicio, 12, VALIDAR_DATA);
    
    printf("Hora Inicio (HH:MM): "); 
    ler_texto_valido(novo->hora_inicio, 10, VALIDAR_NAO_VAZIO);
    
    printf("Data Fim (DD/MM/AAAA): "); 
    ler_texto_valido(novo->data_fim, 12, VALIDAR_DATA);
    
    printf("Hora Fim (HH:MM): "); 
    ler_texto_valido(novo->hora_fim, 10, VALIDAR_NAO_VAZIO);
    
    printf("Local do Evento: "); 
    ler_texto_valido(novo->local, 150, VALIDAR_NAO_VAZIO);

    // --- Parte de adicionar equipamentos ---
    int opcao = 0;
    printf("\nQuer adicionar equipamentos? (1-Sim, 0-Nao): "); 
    scanf("%d", &opcao);
    limpar_buffer();

    while(opcao == 1) {
        // Tambem verifica se tem recursos antes de tentar listar
        if (sistema->num_recursos == 0) {
            printf("Nenhum equipamento cadastrado para adicionar.\n");
            break;
        }

        listarRecursosView(sistema);
        int codigo_recurso, quantidade;
        printf("Digite o Codigo do Equipamento: "); 
        scanf("%d", &codigo_recurso);
        limpar_buffer();
        
        Recurso *recurso_encontrado = NULL;
        for(int i=0; i < sistema->num_recursos; i++) {
            if(sistema->lista_recursos[i].codigo == codigo_recurso) {
                recurso_encontrado = &sistema->lista_recursos[i];
            }
        }

        if(recurso_encontrado != NULL) {
            printf("Quantidade: "); 
            scanf("%d", &quantidade);
            limpar_buffer();

            novo->lista_recursos_alocados = realloc(novo->lista_recursos_alocados, (novo->num_recursos_alocados + 1) * sizeof(ItemRecursoEvento));
            
            ItemRecursoEvento *item = &novo->lista_recursos_alocados[novo->num_recursos_alocados];
            item->codigo_recurso = recurso_encontrado->codigo;
            item->quantidade = quantidade;
            item->custo_locacao_momento = recurso_encontrado->valor_locacao;
            
            novo->num_recursos_alocados++;
            // Soma no custo total previsto
            novo->custo_total_previsto += (recurso_encontrado->valor_locacao * quantidade);
            printf("Equipamento adicionado ao orcamento!\n");
        } else {
            printf("Equipamento nao achado.\n");
        }
        printf("Adicionar mais um? (1-Sim, 0-Nao): "); 
        scanf("%d", &opcao);
        limpar_buffer();
    }

    // --- Parte de adicionar equipe ---
    printf("\nQuer adicionar equipe? (1-Sim, 0-Nao): "); 
    scanf("%d", &opcao);
    limpar_buffer();

    while(opcao == 1) {
         if (sistema->num_equipe == 0) {
            printf("Nenhuma equipe cadastrada para adicionar.\n");
            break;
        }

        listarEquipeInternaView(sistema);
        int codigo_equipe; 
        printf("Codigo do Funcionario: "); 
        scanf("%d", &codigo_equipe);
        limpar_buffer();
        
        EquipeInterna *membro_encontrado = NULL;
        for(int i=0; i < sistema->num_equipe; i++) {
            if(sistema->lista_equipe[i].codigo == codigo_equipe) {
                membro_encontrado = &sistema->lista_equipe[i];
            }
        }

        if(membro_encontrado != NULL) {
            novo->lista_equipe_alocada = realloc(novo->lista_equipe_alocada, (novo->num_equipe_alocada + 1) * sizeof(ItemEquipeEvento));
            
            ItemEquipeEvento *item = &novo->lista_equipe_alocada[novo->num_equipe_alocada];
            item->codigo_equipe = membro_encontrado->codigo;
            item->custo_diaria_momento = membro_encontrado->valor_diaria;
            
            novo->num_equipe_alocada++;
            novo->custo_total_previsto += membro_encontrado->valor_diaria;
            printf("Funcionario adicionado!\n");
        } else {
            printf("Funcionario nao achado.\n");
        }
        printf("Adicionar mais gente? (1-Sim, 0-Nao): "); 
        scanf("%d", &opcao);
        limpar_buffer();
    }

    sistema->num_eventos++;
    salvarEventos(sistema);
    printf("\nOrcamento criado com sucesso! Valor Previsto: R$ %.2f\n", novo->custo_total_previsto);
}

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;

    int codigo;
    printf("\nDigite o ID do orcamento para APROVAR: "); 
    scanf("%d", &codigo);
    limpar_buffer();

    Evento *evento = NULL;
    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == codigo) {
            evento = &sistema->lista_eventos[i];
        }
    }

    if (evento == NULL || evento->status != ORCAMENTO) { 
        printf("Erro: Evento nao existe ou ja foi aprovado/finalizado.\n"); 
        return; 
    }

    printf("Verificando se tem estoque nas datas e horas...\n");
    
    // Converte data e hora do evento que queremos aprovar para numero
    long long inicio_atual = transformarDataHoraEmNumero(evento->data_inicio, evento->hora_inicio);
    long long fim_atual = transformarDataHoraEmNumero(evento->data_fim, evento->hora_fim);
    
    int tem_conflito = 0;

    // Verifica cada recurso do orcamento
    for(int i=0; i < evento->num_recursos_alocados; i++) {
        int codigo_rec = evento->lista_recursos_alocados[i].codigo_recurso;
        int qtd_pedida = evento->lista_recursos_alocados[i].quantidade;
        int estoque_total = 0;
        
        // Pega o estoque maximo
        for(int k=0; k < sistema->num_recursos; k++) {
            if(sistema->lista_recursos[k].codigo == codigo_rec) {
                estoque_total = sistema->lista_recursos[k].quantidade_estoque;
            }
        }
        
        // Verifica o quanto ja esta ocupado por outros eventos NESSE horario
        int qtd_usada = 0;
        for(int j=0; j < sistema->num_eventos; j++) {
            Evento *outro = &sistema->lista_eventos[j];
            
            // Compara apenas com eventos ja APROVADOS
            if(outro->status == APROVADO && outro->codigo != evento->codigo) {
                
                // Pega data e hora do outro evento
                long long inicio_outro = transformarDataHoraEmNumero(outro->data_inicio, outro->hora_inicio);
                long long fim_outro = transformarDataHoraEmNumero(outro->data_fim, outro->hora_fim);
                
                // Verifica se as datas se cruzam
                // Logica: Se o inicio do meu for antes do fim do outro E o meu fim for depois do inicio do outro
                if(inicio_atual <= fim_outro && fim_atual >= inicio_outro) {
                    // Se cruza, soma a quantidade que o outro evento esta usando
                    for(int r=0; r < outro->num_recursos_alocados; r++) {
                        if(outro->lista_recursos_alocados[r].codigo_recurso == codigo_rec) {
                            qtd_usada += outro->lista_recursos_alocados[r].quantidade;
                        }
                    }
                }
            }
        }

        // Se a quantidade pedida for maior que o que sobrou livre, da erro
        if (qtd_pedida > (estoque_total - qtd_usada)) {
            printf("ERRO: Conflito de agenda! Recurso ID %d nao tem estoque suficiente.\n", codigo_rec);
            printf("Data/Hora solicitada: %s %s a %s %s\n", evento->data_inicio, evento->hora_inicio, evento->data_fim, evento->hora_fim);
            printf("Estoque Total: %d, Ja ocupado nesse horario: %d, Voce pediu: %d\n", estoque_total, qtd_usada, qtd_pedida);
            tem_conflito = 1; 
            break; 
        }
    }

    if(tem_conflito == 0) {
        evento->status = APROVADO;
        salvarEventos(sistema);
        printf("Tudo certo! Orcamento APROVADO e estoque reservado.\n");
    } else {
        printf("Nao foi possivel aprovar o orcamento por falta de estoque na data/hora.\n");
    }
}

void finalizarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;

    int codigo;
    printf("\nDigite o ID para FINALIZAR e cobrar: "); 
    scanf("%d", &codigo);
    limpar_buffer();
    
    Evento *e = NULL;
    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == codigo) {
            e = &sistema->lista_eventos[i];
        }
    }

    if (e == NULL || e->status != APROVADO) {
        printf("Erro: O evento precisa estar APROVADO pra finalizar.\n"); 
        return;
    }

    printf("\n--- Fechamento do Evento ---\n");
    printf("Custo que estava previsto: R$ %.2f\n", e->custo_total_previsto);
    
    printf("Valor Final para Cobrar do Cliente: R$ "); 
    ler_float_positivo(&e->valor_final_faturado);

    Transacao t;
    memset(&t, 0, sizeof(Transacao));
    t.tipo = CONTA_A_RECEBER;
    t.status = PENDENTE;
    t.valor = e->valor_final_faturado;
    
    sprintf(t.descricao, "Fatura do Evento Codigo %d", e->codigo);
    
    printf("Data de Vencimento da fatura (DD/MM/AAAA): "); 
    ler_texto_valido(t.data_vencimento, 12, VALIDAR_DATA);

    registrarTransacao(sistema, t);
    
    e->status = FINALIZADO;
    salvarEventos(sistema);
    
    printf("Evento finalizado! Recursos liberados e conta a receber gerada.\n");
}