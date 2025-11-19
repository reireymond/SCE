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

// Ajuda a comparar data e hora juntas transformando tudo num numerão
// Ex: Data 01/01/2025 e Hora 14:30 vira -> 202501011430
// Assim fica facil saber se um evento comeca antes ou depois do outro
long long transformarDataHoraEmNumero(char* data, char* hora) {
    int dia, mes, ano, h, m;
    // Zera tudo pra garantir que não tenha lixo de memoria
    dia=0; mes=0; ano=0; h=0; m=0;
    
    // Le os numeros das strings
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    sscanf(hora, "%d:%d", &h, &m);
    
    // Matematica: multiplica pra jogar cada um na sua casa decimal correta (AnoMesDiaHoraMinuto)
    long long resultado = (long long)ano * 100000000; // 202500000000
    resultado += mes * 1000000;                       // 202501000000
    resultado += dia * 10000;                         // 202501010000
    resultado += h * 100;                             // 202501011400
    resultado += m;                                   // 202501011430
    
    return resultado;
}

void adicionarEventoController(Sistema *sistema) {
    // Verifica se precisa aumentar a lista de eventos na memoria
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        int nova_capacidade = (sistema->capacidade_eventos == 0) ? 10 : sistema->capacidade_eventos * 2;
        sistema->lista_eventos = realloc(sistema->lista_eventos, nova_capacidade * sizeof(Evento));
        sistema->capacidade_eventos = nova_capacidade;
    }

    // Pega o ponteiro pro proximo espaco vazio
    Evento *novo = &sistema->lista_eventos[sistema->num_eventos];
    // Limpa a memoria pra nao ter lixo antigo
    memset(novo, 0, sizeof(Evento));
    
    novo->codigo = sistema->num_eventos + 1;
    novo->status = ORCAMENTO;

    printf("\n--- Novo Orcamento (ID: %d) ---\n", novo->codigo);
    
    printf("Nome do Evento: "); 
    ler_texto_valido(novo->nome_evento, 150, VALIDAR_NAO_VAZIO);
    
    // Mostra clientes pra facilitar a escolha
    listarClientesView(sistema);
    printf("Codigo do Cliente: "); 
    ler_inteiro_valido(&novo->codigo_cliente, 1, 99999);
    
    printf("Data Inicio (DD/MM/AAAA): "); 
    ler_texto_valido(novo->data_inicio, 12, VALIDAR_DATA);
    
    // Pede a hora de inicio
    printf("Hora Inicio (HH:MM): "); 
    ler_texto_valido(novo->hora_inicio, 6, VALIDAR_NAO_VAZIO);
    
    printf("Data Fim (DD/MM/AAAA): "); 
    ler_texto_valido(novo->data_fim, 12, VALIDAR_DATA);
    
    // Pede a hora de fim
    printf("Hora Fim (HH:MM): "); 
    ler_texto_valido(novo->hora_fim, 6, VALIDAR_NAO_VAZIO);
    
    printf("Local do Evento: "); 
    ler_texto_valido(novo->local, 150, VALIDAR_NAO_VAZIO);

    // --- Parte de adicionar equipamentos ---
    int opcao = 0;
    printf("\nQuer adicionar equipamentos? (1-Sim, 0-Nao): "); 
    scanf("%d", &opcao);
    limpar_buffer();

    while(opcao == 1) {
        listarRecursosView(sistema);
        int codigo_recurso, quantidade;
        printf("Digite o Codigo do Equipamento: "); 
        scanf("%d", &codigo_recurso);
        limpar_buffer();
        
        // Procura o recurso na lista geral
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

            // Aumenta a lista de recursos DENTRO do evento
            novo->lista_recursos_alocados = realloc(novo->lista_recursos_alocados, (novo->num_recursos_alocados + 1) * sizeof(ItemRecursoEvento));
            
            // Preenche os dados do item no evento
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
            // Aumenta a lista de equipe DENTRO do evento
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

    // Tudo pronto, salva no arquivo
    sistema->num_eventos++;
    salvarEventos(sistema);
    printf("\nOrcamento criado com sucesso! Valor Previsto: R$ %.2f\n", novo->custo_total_previsto);
}

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
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
    
    // Calcula inicio e fim deste evento usando a funcao nova
    long long inicio_atual = transformarDataHoraEmNumero(evento->data_inicio, evento->hora_inicio);
    long long fim_atual = transformarDataHoraEmNumero(evento->data_fim, evento->hora_fim);
    
    int tem_conflito = 0;

    // Passa por cada recurso que o evento quer usar
    for(int i=0; i < evento->num_recursos_alocados; i++) {
        int codigo_rec = evento->lista_recursos_alocados[i].codigo_recurso;
        int qtd_pedida = evento->lista_recursos_alocados[i].quantidade;
        int estoque_total = 0;
        
        // Acha quanto tem no total desse recurso no estoque geral
        for(int k=0; k < sistema->num_recursos; k++) {
            if(sistema->lista_recursos[k].codigo == codigo_rec) {
                estoque_total = sistema->lista_recursos[k].quantidade_estoque;
            }
        }
        
        // Vê quanto ja ta sendo usado por OUTROS eventos aprovados na mesma data/hora
        int qtd_usada = 0;
        for(int j=0; j < sistema->num_eventos; j++) {
            Evento *outro = &sistema->lista_eventos[j];
            // So olha eventos que ja estao APROVADOS e que nao sao esse mesmo que estamos tentando aprovar
            if(outro->status == APROVADO && outro->codigo != evento->codigo) {
                
                // Calcula inicio e fim do outro evento
                long long inicio_outro = transformarDataHoraEmNumero(outro->data_inicio, outro->hora_inicio);
                long long fim_outro = transformarDataHoraEmNumero(outro->data_fim, outro->hora_fim);
                
                // Verifica se os horarios batem (intersecao de tempo)
                // Logica: Se (Inicio A <= Fim B) E (Fim A >= Inicio B), eles se cruzam
                if(inicio_atual <= fim_outro && fim_atual >= inicio_outro) {
                    // Se bate o horario, soma os recursos que ele usa
                    for(int r=0; r < outro->num_recursos_alocados; r++) {
                        if(outro->lista_recursos_alocados[r].codigo_recurso == codigo_rec) {
                            qtd_usada += outro->lista_recursos_alocados[r].quantidade;
                        }
                    }
                }
            }
        }

        // Verifica se sobra estoque
        if (qtd_pedida > (estoque_total - qtd_usada)) {
            printf("ERRO: Recurso ID %d sem estoque no horario %s as %s!\n", codigo_rec, evento->hora_inicio, evento->hora_fim);
            printf("Estoque Total: %d, Usado na hora: %d, Pedido: %d\n", estoque_total, qtd_usada, qtd_pedida);
            tem_conflito = 1; 
            break; // Para tudo se der erro
        }
    }

    if(tem_conflito == 0) {
        evento->status = APROVADO;
        salvarEventos(sistema);
        printf("Tudo certo! Evento APROVADO.\n");
    } else {
        printf("Nao foi possivel aprovar por falta de estoque neste horario.\n");
    }
}

void finalizarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
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

    // Gera a conta a receber automatico
    Transacao t;
    memset(&t, 0, sizeof(Transacao));
    t.tipo = CONTA_A_RECEBER;
    t.status = PENDENTE;
    t.valor = e->valor_final_faturado;
    
    // Cria uma descricao automatica
    sprintf(t.descricao, "Fatura do Evento Codigo %d", e->codigo);
    
    printf("Data de Vencimento da fatura (DD/MM/AAAA): "); 
    ler_texto_valido(t.data_vencimento, 12, VALIDAR_DATA);

    // Salva a conta
    registrarTransacao(sistema, t);
    
    // Atualiza o evento para finalizado
    e->status = FINALIZADO;
    salvarEventos(sistema);
    
    printf("Evento finalizado! Recursos liberados e conta a receber gerada.\n");
}
