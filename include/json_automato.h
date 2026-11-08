#ifndef JSON_AUTOMATO_H
#define JSON_AUTOMATO_H

#include <stdbool.h>
#include "automato.h"

#define MAX_ITENS     600
#define TAMANHO_TEXTO 64

typedef struct {
    int  quantidade;
    char itens[MAX_ITENS][TAMANHO_TEXTO];
} ListaTextos;

typedef struct {
    ListaTextos aceitas;
    ListaTextos rejeitadas;
} PalavrasTeste;

bool carregar_automato_json(const char *caminho, const char *nome_automato, Automato *a);
bool carregar_palavras_teste(const char *caminho, PalavrasTeste *teste);

#endif