# Define o compilador
CC = gcc

# Adiciona -Iinclude para que o compilador encontre os headers na pasta include/
CFLAGS = -Wall -g -Iinclude

# Define os diretórios
SRCDIR = src
OBJDIR = obj
OUTDIR = out

# Define o alvo final, agora dentro da pasta de objetos
TARGET = $(OBJDIR)/montador

# Encontra todos os arquivos .c no diretório de fontes
SRCS = $(wildcard $(SRCDIR)/*.c)

# Gera os nomes dos arquivos objeto, colocando-os no diretório de objetos
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Regra padrão: compila o alvo principal
all: $(TARGET)

# Regra para criar o executável final
$(TARGET): $(OBJS)
	@mkdir -p $(OBJDIR) # Garante que o diretório de destino do executável existe
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra para compilar os arquivos objeto .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nova regra para executar o programa com os parâmetros corretos
run: all
	./$(TARGET) examples/entrada.asm -o $(OUTDIR)/saida.bin
	@echo "Execucao concluida. 'saida.bin' gerado em $(OUTDIR)/"

# Regra para limpar os arquivos gerados
clean:
	rm -rf $(OBJDIR)

# Declara as regras que não são nomes de arquivos
.PHONY: all clean run