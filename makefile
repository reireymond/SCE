# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -I. -g

# Diretórios
ODIR = obj
BDIR = bin
TARGET = $(BDIR)/produtora

# Encontra todos os arquivos .c recursivamente
SOURCES := $(wildcard controller/*/*.c model/*/*.c view/*/*.c utils/*.c main.c)

# Cria a lista de arquivos objeto (.o) no diretório obj/
OBJECTS := $(patsubst %.c,$(ODIR)/%.o,$(notdir $(SOURCES)))

# Regra principal
all: $(TARGET)

# Linking: Junta todos os .o para criar o executável
$(TARGET): $(OBJECTS)
	@-mkdir $(BDIR)
	$(CC) $(OBJECTS) -o $@
	@echo "Executavel '$(TARGET)' criado com sucesso."

# Compilação: Transforma cada .c em um .o
$(ODIR)/%.o: %.c
	@-mkdir $(ODIR)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# VPATH diz ao make onde encontrar os arquivos .c
vpath %.c $(sort $(dir $(SOURCES)))

# Limpa os arquivos compilados
clean:
	@echo "Limpando arquivos gerados..."
	@rm -rf $(ODIR) $(BDIR)
	@echo "Limpeza concluida."

# Compila e executa
run: all
	./$(TARGET)

.PHONY: all clean run