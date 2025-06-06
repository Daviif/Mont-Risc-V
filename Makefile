# Define o compilador a ser usado (GNU C Compiler)
CC = gcc

# Define as flags de compilação:
# -Wall: Habilita todos os avisos do compilador
# -g: Inclui informações de depuração no executável
CFLAGS = -Wall -g

# Define o nome do arquivo executável final
TARGET = montador

# Lista todos os arquivos de código-fonte (.c)
SRCS = main.c compilador.c

# Gera automaticamente a lista de arquivos objeto (.o) a partir da lista de fontes
OBJS = $(SRCS:.c=.o)

# A primeira regra é a padrão, que será executada ao rodar "make"
# Depende do executável final (TARGET)
all: $(TARGET)

# Regra para linkar os arquivos objeto e criar o executável final
# Depende de todos os arquivos objeto (.o)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra de padrão para compilar arquivos .c em arquivos .o
# Diz ao 'make' como criar um arquivo .o a partir de um .c
# Depende do arquivo .c correspondente e do cabeçalho compilador.h
%.o: %.c compilador.h
	$(CC) $(CFLAGS) -c $< -o $@

# Regra "clean" para limpar os arquivos gerados pela compilação
# Remove o executável e todos os arquivos objeto
clean:
	rm -f $(TARGET) $(OBJS)

# Declara que "all" e "clean" não são nomes de arquivos
.PHONY: all clean