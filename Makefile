# Define o compilador
CC = gcc

# Adiciona -Iinclude para que o compilador encontre os headers na pasta include/
CFLAGS = -Wall -g -Iinclude

# Define os diretórios de fonte e de objetos
SRCDIR = src
OBJDIR = obj
OUTDIR = out

# Define o alvo final. O executável "montador" será criado na pasta raiz.
TARGET = obj/montador

# Encontra todos os arquivos .c no diretório de fontes
SRCS = $(wildcard $(SRCDIR)/*.c)

# Gera os nomes dos arquivos objeto, colocando-os no diretório de objetos
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Regra padrão: compila o alvo principal
all: $(TARGET)

# Regra para criar o executável final na pasta raiz
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra para compilar os arquivos objeto .o
# Cria o diretório de objetos se ele não existir
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para executar e mostrar o resultado NO TERMINAL
run: all
	./$(TARGET) examples/entrada.asm

# Nova regra para testar a gravação em arquivo
test_file: all
	./$(TARGET) examples/entrada.asm -o out/saida.bin
	@echo "Execucao concluida. 'saida.bin' gerado em $(OUTDIR)/"

# Regra para limpar os arquivos gerados (o executável na raiz e a pasta de objetos)
clean:
	rm -f $(TARGET)
	rm -rf $(OBJDIR)

# Declara as regras que não são nomes de arquivos
.PHONY: all clean run test_file