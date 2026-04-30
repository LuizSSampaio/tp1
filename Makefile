# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g
TARGET = exe

# Arquivos objeto
OBJS = aluno.o tp.o

# Alvo principal
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Regra para aluno.o
aluno.o: aluno.c aluno.h
	$(CC) $(CFLAGS) -c aluno.c

# Regra para pratica.o
tp.o: tp.c aluno.h
	$(CC) $(CFLAGS) -c tp.c

# Limpeza
clean:
	rm -f $(OBJS) $(TARGET)

# Limpeza completa
purge: clean
	rm -f output.out

.PHONY: clean purge
