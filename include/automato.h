#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <stdbool.h>

#define MAX_ESTADOS   64
#define MAX_SIMBOLOS  26
#define TAMANHO_NOME  64

typedef struct {
    int  qtd_estados;
    int  qtd_simbolos;
    char alfabeto[MAX_SIMBOLOS];
    char nomes[MAX_ESTADOS][TAMANHO_NOME];
    bool eh_inicial[MAX_ESTADOS];
    bool eh_final[MAX_ESTADOS];
    bool transicao[MAX_ESTADOS][MAX_SIMBOLOS][MAX_ESTADOS];
    bool transicao_lambda[MAX_ESTADOS][MAX_ESTADOS];
} Automato;

void inicializar_automato(Automato *a);
int  indice_simbolo(const Automato *a, char simbolo);
void limpar_buffer(void);
int  ler_automato(Automato *a);
void imprimir_automato(const Automato *a);

#endif