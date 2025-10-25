# Nome do compilador (funciona tanto para gcc no Linux quanto no MinGW)
CC = gcc

# Flags de compilação
CFLAGS = -Wall -I. -g

# --- Detecção de Sistema Operacional e Configuração ---
# Por padrão, assume-se um sistema tipo Unix (Linux, macOS)
TARGET_EXEC = produtora
RUN_CMD = ./bin/$(TARGET_EXEC)

# Verifica se o sistema é Windows (rodando em um shell como MinGW ou Git Bash)
# A função 'uname' está disponível nesses ambientes
ifeq ($(findstring MINGW,$(shell uname -s)),MINGW)
	TARGET_EXEC = produtora.exe
	RUN_CMD = bin/$(TARGET_EXEC)
endif

# --- Diretórios e Alvos ---
ODIR = obj
BDIR = bin
TARGET = $(BDIR)/$(TARGET_EXEC)

# Encontra todos os arquivos .c recursivamente
SOURCES := $(wildcard controller/*/*.c model/*/*.c view/*/*.c utils/*.c main.c)

# Cria a lista de arquivos objeto (.o) a partir dos arquivos de código fonte
OBJECTS := $(patsubst %.c,$(ODIR)/%.o,$(notdir $(SOURCES)))

# --- Regras de Compilação ---

# Regra principal: compila tudo
all: $(TARGET)

# Regra para criar os diretórios de saída (obj e bin) - Compatível com Windows
$(ODIR):
	@if not exist $(ODIR) mkdir $(ODIR)

$(BDIR):
	@if not exist $(BDIR) mkdir $(BDIR)

# Garante que as regras de diretório sejam executadas antes das outras
$(TARGET): $(ODIR) $(BDIR) $(OBJECTS)
	@echo "Linkando para criar o executavel..."
	$(CC) $(OBJECTS) -o $@
	@echo "Executavel '$(TARGET)' criado com sucesso."

# A regra de compilação continua a mesma
$(ODIR)/%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Regra de Compilação: transforma cada arquivo .c em um arquivo .o
$(ODIR)/%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# VPATH informa ao 'make' onde encontrar os arquivos de código fonte
vpath %.c $(sort $(dir $(SOURCES)))

# --- Comandos Adicionais ---

# Limpa os arquivos compilados (objetos e executável)
clean:
	@echo "Limpando arquivos gerados..."
	@rm -rf $(ODIR) $(BDIR)
	@echo "Limpeza concluida."

# Compila e executa o programa
run: all
	$(RUN_CMD)

# Alvos que não são arquivos reais, para evitar conflitos
.PHONY: all clean run