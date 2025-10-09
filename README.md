# Sistema de Controle de Eventos (SCE)

![Status do Projeto](https://img.shields.io/badge/status-em%20desenvolvimento-yellowgreen)
![Linguagem](https://img.shields.io/badge/linguagem-C-blue.svg)
![Licença](https://img.shields.io/badge/licen%C3%A7a-MIT-green)

> Projeto acadêmico da disciplina de Algoritmos II do IFMG - Campus Formiga. Um sistema de gestão completo para produtoras de eventos, desenvolvido em C.

---

### Tabela de Conteúdos
1. [📖 Sobre o Projeto](#-sobre-o-projeto)
2. [✨ Funcionalidades](#-funcionalidades)
3. [🏛️ Arquitetura de Persistência](#️-arquitetura-de-persistência)
4. [🛠️ Tecnologias Utilizadas](#️-tecnologias-utilizadas)
5. [🚀 Como Compilar e Executar](#-como-compilar-e-executar)
6. [📂 Estrutura de Arquivos](#-estrutura-de-arquivos)
7. [👤 Autor](#-autor)

---

## 📖 Sobre o Projeto

O **SCE** é um sistema de controle gerencial desenvolvido para uma Produtora de Eventos fictícia. O objetivo é centralizar e automatizar todas as operações do negócio, desde o cadastro inicial de clientes e fornecedores até o faturamento final de um evento e a análise de relatórios gerenciais.

Este projeto está sendo construído como requisito avaliativo para a disciplina de **Algoritmos II** no curso de Ciência da Computação do **IFMG - Campus Formiga**, sob a orientação do Prof. Manoel Pereira Junior.

## ✨ Funcionalidades

O sistema está organizado nos seguintes módulos:

#### Módulo de Cadastros e Gestão de Dados
- [x] Cadastro completo da Produtora, Clientes e Fornecedores.
- [x] Gestão da Equipe Interna (funcionários) e seus respectivos custos.
- [x] Controle de Recursos e Equipamentos (estoque, preço de custo e locação).
- [x] Cadastro de Operadores do sistema com usuário e senha.

#### Módulo de Orçamentos e Gestão de Eventos
- [ ] Criação e gerenciamento de orçamentos detalhados para clientes.
- [ ] Sistema de aprovação de eventos com alocação de recursos.
- [ ] Controle inteligente para evitar alocação de um mesmo recurso para múltiplos eventos na mesma data.
- [ ] Finalização e faturamento do evento, com consolidação de todos os custos.

#### Módulo de Transações
- [ ] Lançamento de custos individuais em cada evento.
- [ ] Controle de Caixa (pagamentos à vista).
- [ ] Gestão de Contas a Receber e Contas a Pagar.
- [ ] Módulo de Aquisição de Equipamentos, com cálculo automático de preço de locação baseado em nota fiscal.

#### Módulo de Feedback e Relatórios
- [ ] Emissão de relatórios em tela ou exportados para arquivo `.csv`.
- [ ] Relatórios de Clientes, Eventos, Equipamentos, Contas a Pagar/Receber, e mais.
- [ ] Filtros personalizáveis para todos os relatórios (por data, cliente, status, etc.).

#### Módulo de Importação/Exportação de Dados
- [ ] Ferramentas para importar e exportar dados de tabelas específicas em formato `.xml`.

## 🏛️ Arquitetura de Persistência

Um dos requisitos centrais do projeto é a flexibilidade no armazenamento de dados. O SCE foi projetado com uma **camada de persistência genérica**, permitindo que o sistema opere com diferentes backends de armazenamento sem alterar a lógica de negócios.

Atualmente, os seguintes métodos de persistência são suportados:
* **Em Memória:** Os dados são mantidos em memória RAM e perdidos ao fechar o sistema (ideal para testes).
* **Arquivo de Texto (.txt):** Todos os dados são salvos em arquivos de texto legíveis.
* **Arquivo Binário (.bin):** Os dados são salvos em formato binário, oferecendo mais performance e ocupando menos espaço.

## 🛠️ Tecnologias Utilizadas

* **Linguagem C (Padrão C99):** Todo o sistema foi desenvolvido em C puro, com foco em gerenciamento de memória, ponteiros e manipulação de arquivos.
* **Git & GitHub:** Para controle de versão e gerenciamento do projeto.

## 🚀 Como Compilar e Executar

**Pré-requisitos:**
* Um compilador C (como o GCC).

```bash
# 1. Clone o repositório
git clone [https://github.com/reireymond/Projeto_AII_Res.git](https://github.com/reireymond/Projeto_AII_Res.git)

# 2. Navegue até o diretório do projeto
cd Projeto_AII_Res

# 3. Compile todos os arquivos .c
gcc *.c -o sce_executavel -lm

# 4. Execute o programa
./sce_executavel
```

## 📂 Estrutura de Arquivos

O código fonte está organizado da seguinte maneira para garantir a modularidade e a fácil manutenção:

* `main.c`: Ponto de entrada do programa, contém o loop principal e o menu inicial.
* `menus.h` / `menus.c`: Contém as funções responsáveis por exibir todos os menus da interface do usuário.
* `funcoes.h` / `funcoes.c`: Contém a lógica de negócios e as funcionalidades de cada módulo do sistema.
* `structs.h`: Define todas as estruturas de dados (structs) utilizadas no projeto.
* `persistencia.h` / `persistencia_*.c`: Arquivos responsáveis pela camada de abstração e implementações de persistência.

## 👤 Autor

* **Kaua Teixeira Nascimento**
* **LinkedIn:** [linkedin.com/in/kaua-teixeira-nascimento-ba6b03331](https://www.linkedin.com/in/kaua-teixeira-nascimento-ba6b03331)
* **GitHub:** [@reireymond](https://github.com/reireymond)

---
> Este projeto está sob a licença MIT. Veja o arquivo `LICENSE` para mais detalhes.
