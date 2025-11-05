# Nome do compilador (funciona tanto para gcc no Linux quanto no MinGW)
CC = gcc

# Flags de compilação
CFLAGS = -Wall -I. -g

# --- Detecção de Sistema Operacional e Configuração ---

# Padrão: Comandos Unix (Linux, macOS, MinGW/Git Bash)
TARGET_EXEC = produtora
RUN_CMD = ./bin/$(TARGET_EXEC)
MKDIR = mkdir -p
RM_FILES = rm -f
RM_DIRS = rm -rf

# Verifica se o sistema é Windows (rodando em um shell como MinGW ou Git Bash)
ifeq ($(findstring MINGW,$(shell uname -s)),MINGW)
	TARGET_EXEC = produtora.exe
	RUN_CMD = bin/$(TARGET_EXEC)
    # No MinGW/Git Bash, os comandos Unix (rm -rf, mkdir -p) já funcionam.
else ifeq ($(OS),Windows_NT)
    # Verifica se o sistema é Windows nativo (rodando em CMD ou PowerShell)
	TARGET_EXEC = produtora.exe
	RUN_CMD = bin/$(TARGET_EXEC)
    # Se for Windows nativo (CMD), usamos os comandos específicos
	MKDIR = mkdir
	RM_FILES = del /Q
	RM_DIRS = rmdir /S /Q
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

# Regra para criar os diretórios de saída (obj e bin) - Agora multiplataforma
$(ODIR) $(BDIR):
	@echo "Criando diretório: $@"
	@$(MKDIR) $@

# Garante que as regras de diretório sejam executadas antes das outras
$(TARGET): $(ODIR) $(BDIR) $(OBJECTS)
	@echo "Linkando para criar o executavel..."
	$(CC) $(OBJECTS) -o $@
	@echo "Executavel '$(TARGET)' criado com sucesso."

# Regra de Compilação: transforma cada arquivo .c em um arquivo .o
# Esta regra foi unificada e corrigida para usar TAB
$(ODIR)/%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# VPATH informa ao 'make' onde encontrar os arquivos de código fonte
vpath %.c $(sort $(dir $(SOURCES)))

# --- Comandos Adicionais ---

# Limpa os arquivos compilados (objetos e executável)
clean:
	@echo "Limpando arquivos gerados..."
	# Limpa diretórios de forma recursiva usando o comando específico do SO
	@$(RM_DIRS) $(ODIR) $(BDIR) 2> /dev/null || true
	@echo "Limpeza concluida."

# Compila e executa o programa
run: all
	$(RUN_CMD)

# Alvos que não são arquivos reais, para evitar conflitos
.PHONY: all clean run