CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
FONTES = $(wildcard src/*.c)
EXECUTAVEL = simulador

all: $(EXECUTAVEL)

$(EXECUTAVEL): $(FONTES) $(wildcard include/*.h)
	$(CC) $(CFLAGS) $(FONTES) -o $(EXECUTAVEL)

limpar:
	rm -f $(EXECUTAVEL) $(EXECUTAVEL).exe

.PHONY: all limpar