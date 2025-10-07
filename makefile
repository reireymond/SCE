# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -I. -g

# Diretórios
ODIR = obj
BDIR = bin
TARGET = $(BDIR)/produtora
DIRS = $(ODIR) $(BDIR)

# Encontra todos os arquivos .c recursivamente
SOURCES := $(wildcard controller/*/*.c model/*/*.c view/*/*.c utils/*.c main.c)

# Cria a lista de arquivos objeto (.o) no diretório obj/
# A função 'notdir' remove o caminho do diretório, pegando apenas o nome do arquivo.
OBJECTS := $(patsubst %.c,$(ODIR)/%.o,$(notdir $(SOURCES)))

# Regra principal que o 'make' executa por padrão
all: $(TARGET)

# Regra para criar os diretórios de saída (obj e bin)
# Esta regra será executada antes de compilar, pois $(TARGET) depende dela.
$(DIRS):
	@mkdir $@

# Linking: Junta todos os arquivos .o para criar o executável final
$(TARGET): $(DIRS) $(OBJECTS)
	@echo "Linkando para criar o executavel..."
	$(CC) $(OBJECTS) -o $@
	@echo "Executavel '$(TARGET)' criado com sucesso."

# Compilação: Transforma cada arquivo .c em um arquivo .o
# Esta regra é genérica e funciona para todos os arquivos .c
$(ODIR)/%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# VPATH diz ao 'make' onde encontrar os arquivos de origem (.c)
# quando ele só conhece o nome base (ex: cliente_controller.c)
vpath %.c $(sort $(dir $(SOURCES)))

# Limpa os arquivos compilados
clean:
	@echo "Limpando arquivos gerados..."
	@rm -rf $(ODIR) $(BDIR)
	@echo "Limpeza concluida."

# Compila e executa
run: all
	./$(TARGET)

# Declara alvos que não são arquivos
.PHONY: all clean run
