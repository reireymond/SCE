# Sistema de Gerenciamento de Eventos (SGE)

![Status do Projeto](https://img.shields.io/badge/status-em%20desenvolvimento-yellowgreen)
![Linguagem](https://img.shields.io/badge/C-C99-blue.svg)
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
7. [👤 Autores](#-autores)

---

## 📖 Sobre o Projeto

O **SGE** é um sistema de controle gerencial desenvolvido para uma Produtora de Eventos fictícia. O objetivo é centralizar e automatizar todas as operações do negócio, desde o cadastro inicial de clientes e fornecedores até o faturamento final de um evento e a análise de relatórios gerenciais.

Este projeto está sendo construído como requisito avaliativo para a disciplina de **Algoritmos II** no curso de Ciência da Computação do **IFMG - Campus Formiga**, sob a orientação do Prof. Manoel Pereira Junior.

## ✨ Funcionalidades

O sistema está organizado nos seguintes módulos:

#### 1. Módulo de Cadastros e Gestão de Dados
- [x] **Produtora:** Cadastro dos dados da empresa e margem de lucro padrão.
- [x] **Clientes:** Gestão completa (CRUD) de clientes.
- [x] **Equipe Interna:** Cadastro de funcionários e valor da diária.
- [x] **Recursos e Equipamentos:** Controle de estoque, preço de custo e valor de locação.
- [x] **Fornecedores:** Cadastro de parceiros terceirizados.
- [x] **Operadores:** Controle de acesso com usuário e senha.

#### 2. Módulo de Orçamentos e Gestão de Eventos
- [x] **Criação de Orçamentos:** Alocação provisória de recursos, equipe e serviços externos.
- [x] **Aprovação de Eventos:**
  - [x] Verificação automática de conflito de datas (recursos indisponíveis no período).
  - [x] Reserva efetiva de estoque após aprovação.
- [x] **Finalização:** Encerramento do evento, liberação dos recursos de volta ao estoque e faturamento final.

#### 3. Módulo de Transações
- [x] **Aquisição de Equipamentos (Nota Fiscal):**
  - [x] Entrada de produtos com cálculo automático de rateio de frete e impostos.
  - [x] Precificação automática da locação baseada na margem de lucro e custos.
- [x] **Contas a Receber:** Gerenciamento de faturas geradas pelos eventos finalizados.
- [x] **Contas a Pagar:** Controle de pagamentos a fornecedores e aquisições parceladas.
- [x] **Fluxo de Caixa:** Controle de saldo disponível e baixas de pagamentos/recebimentos.

#### 4. Módulo de Feedback e Relatórios
- [ ] Relatórios em tela e exportação para arquivo `.csv`.
- [ ] Listagens filtradas (Clientes, Eventos, Cronograma de Alocação, Contas a Pagar/Receber).

#### 5. Módulo de Importação/Exportação de Dados
- [ ] Importação e exportação de dados específicos em formato `.xml` (Requisito final).
- [x] Conversão interna entre formatos de persistência (Binário <-> Texto).

## 🏛️ Arquitetura de Persistência

Um dos requisitos centrais do projeto é a flexibilidade no armazenamento de dados. O SCE foi projetado com uma **camada de persistência genérica**, permitindo que o sistema opere com diferentes backends de armazenamento sem alterar a lógica de negócios.

Atualmente, os seguintes métodos de persistência são suportados:
* **Em Memória:** Os dados são mantidos em memória RAM e perdidos ao fechar o sistema (ideal para testes).
* **Arquivo de Texto (.txt):** Todos os dados são salvos em arquivos de texto legíveis.
* **Arquivo Binário (.bin):** Os dados são salvos em formato binário, oferecendo mais performance e ocupando menos espaço.

## 🛠️ Tecnologias Utilizadas

* **Linguagem C (Padrão C99):** Todo o sistema foi desenvolvido em C puro, com foco em gerenciamento de memória, ponteiros e manipulação de arquivos.
* **Make/Makefile:** Para automação do processo de compilação.
* **Git & GitHub:** Para controle de versão e gerenciamento do projeto.

## 🚀 Como Compilar e Executar

**Pré-requisitos:**
* Um compilador C (como o GCC).

```bash
# 1. Clone o repositório
git clone https://github.com/reireymond/EventMananger.git

# 2. Navegue até o diretório do projeto
cd EventMananger

# 3. Compile o projeto
# No Linux, use o comando 'make'
make

# No Windows (com MinGW instalado), use o comando 'mingw32-make'
mingw32-make

# 4. Execute o programa
# No Linux
./produtora

# No Windows
Produtora.exe

```

## 📂 Estrutura de Arquivos

O código fonte está organizado da seguinte maneira para garantir a modularidade e a fácil manutenção:

* `main.c`: Ponto de entrada e loop principal.
* `model/`: Definições das estruturas (structs) e funções de persistência (salvar/carregar).
* `view/`: Menus e interfaces de interação com o usuário (printf/scanf).
* `controller/`: Regras de negócio, validações lógicas e orquestração entre Model e View.
* `utils/`: Bibliotecas auxiliares para validação de dados (CPF, CNPJ, Datas) e interface.
* `Makefile`: Arquivo com as regras para compilar o projeto de forma automatizada.

## 👤 Autores

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/reireymond">
        <img src="https://github.com/reireymond.png?size=100" width="100px;" alt="Foto de Kaua Teixeira Nascimento no GitHub"/>
        <br />
        <sub>
          <b>Kaua Teixeira N.</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/bielbieloliveirag8-eng">
        <img src="https://github.com/bielbieloliveirag8-eng.png?size=100" width="100px;" alt="Foto de Gabriel Mendonça de Oliveira no GitHub"/>
        <br />
        <sub>
          <b>Gabriel Mendonça O.</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

> Este projeto está sob a licença MIT. Veja o arquivo `LICENSE` para mais detalhes.
