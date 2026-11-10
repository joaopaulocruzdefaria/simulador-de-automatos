CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
FONTES = $(wildcard src/*.c)
EXECUTAVEL = simulador

executar: $(EXECUTAVEL)
	./$(EXECUTAVEL)

$(EXECUTAVEL): $(FONTES) $(wildcard include/*.h)
	$(CC) $(CFLAGS) $(FONTES) -o $(EXECUTAVEL)

testar: $(EXECUTAVEL)
	printf '1\nexemplos/entrada.json\nautomato 1\n5\nabab\n5\nabb\n0\n' | ./$(EXECUTAVEL)

limpar:
	rm -f $(EXECUTAVEL) $(EXECUTAVEL).exe

.PHONY: executar testar limpar