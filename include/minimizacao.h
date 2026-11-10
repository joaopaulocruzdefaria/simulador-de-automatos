#ifndef MINIMIZACAO_H
#define MINIMIZACAO_H

#include <stdbool.h>
#include "automato.h"

int  obter_destino_afd(const Automato *afd, int estado, int simbolo);
bool eh_afd(const Automato *a);
void remover_inalcancaveis(Automato *afd);
int  minimizar_afd(const Automato *afd, Automato *minimo);

#endif
