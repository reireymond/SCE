# Makefile Simples para Windows (Junior)

# Compilador
CC = gcc

# Opcoes (Wall mostra avisos, -g ajuda a debugar, -I. procura headers na pasta atual)
CFLAGS = -Wall -I. -g

# Nome do programa final
TARGET = produtora.exe

# Pastas
ODIR = obj
BDIR = bin

# Acha todos os arquivos .c automaticamente
SOURCES = $(wildcard controller/*/*.c model/*/*.c view/*/*.c utils/*.c main.c)

# Cria lista de arquivos .o (objetos)
OBJECTS = $(patsubst %.c,$(ODIR)/%.o,$(notdir $(SOURCES)))

# Regra principal
all: criar_pastas $(BDIR)/$(TARGET)

# Cria as pastas se nao existirem (comando Windows)
criar_pastas:
	@if not exist $(ODIR) mkdir $(ODIR)
	@if not exist $(BDIR) mkdir $(BDIR)

# Linka tudo e cria o executavel
$(BDIR)/$(TARGET): $(OBJECTS)
	@echo Criando executavel...
	$(CC) $(OBJECTS) -o $@
	@echo Sucesso! Execute: $(BDIR)\$(TARGET)

# Compila cada arquivo .c individualmente
# Precisamos dizer pro make onde achar os .c usando VPATH
VPATH = controller/armazenamento:controller/cliente:controller/equipe_interna:controller/evento:controller/fornecedor:controller/login:controller/main:controller/operador:controller/produtora:controller/recurso:controller/transacao:model/cliente:model/config_armazenamento:model/equipe_interna:model/evento:model/fornecedor:model/operador:model/produtora:model/recurso:model/transacao:view/cliente:view/equipe_interna:view/evento:view/fornecedor:view/main:view/operador:view/produtora:view/recurso:view/transacao:utils

$(ODIR)/%.o: %.c
	@echo Compilando $<...
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa tudo (comando Windows)
clean:
	@if exist $(ODIR) rmdir /s /q $(ODIR)
	@if exist $(BDIR) rmdir /s /q $(BDIR)
	@echo Limpo.

run: all
	$(BDIR)\$(TARGET)