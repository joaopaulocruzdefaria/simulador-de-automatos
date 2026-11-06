#ifndef CONVERSAO_LAMBDA_H
#define CONVERSAO_LAMBDA_H

#include <stdbool.h>
#include "automato.h"

int  converter_multiplos_iniciais_para_lambda(const Automato *origem, Automato *destino);
void calcular_fecho_lambda(const Automato *a, int estado, bool fecho[]);
void remover_transicoes_lambda(const Automato *origem, Automato *destino);

#endif