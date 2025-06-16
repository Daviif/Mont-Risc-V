# Define o compilador
CC = gcc

# Adiciona -Iinclude para que o compilador encontre os headers na pasta include/
CFLAGS = -Wall -g -Iinclude

# Define os diretórios
SRCDIR = src
OBJDIR = obj
OUTDIR = out

# Define o alvo final DENTRO DO DIRETÓRIO DE SAÍDA
TARGET = $(OBJDIR)/montador

# Lista todos os arquivos .c explicitamente
SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/leitura.c \
       $(SRCDIR)/tabela_instrucoes.c \
       $(SRCDIR)/labels.c \
       $(SRCDIR)/montador.c \
       $(SRCDIR)/pseudo_instruction.c

# Gera os nomes dos arquivos objeto, colocando-os no diretório de objetos
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Regra padrão: compila o alvo principal
all: $(TARGET)

# Regra para criar o executável final
# CRIA O DIRETÓRIO DE SAÍDA AQUI, ANTES DE CRIAR O EXECUTÁVEL
$(TARGET): $(OBJS)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Montador compilado com sucesso em $(TARGET)"

# Regra para compilar os arquivos objeto .o
# Cria o diretório de objetos se ele não existir
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para executar e mostrar o resultado NO TERMINAL
run: all
	./$(TARGET) examples/entrada.asm

# Nova regra para testar a gravação em arquivo
out_file: all
	./$(TARGET) examples/entrada.asm -o $(OUTDIR)/saida.bin
	@echo "Execucao concluida. 'saida.bin' gerado em $(OUTDIR)/"

# Regra para limpar os arquivos gerados
clean:
	@echo "Limpando arquivos gerados..."
	rm -rf $(OBJDIR)
	rm -rf $(OUTDIR)

# Declara as regras que não são nomes de arquivos
.PHONY: all clean run out_file