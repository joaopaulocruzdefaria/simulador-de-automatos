#include <string.h>
#include "conversao_lambda.h"

int converter_multiplos_iniciais_para_lambda(const Automato *origem, Automato *destino) {
    if (origem->qtd_estados >= MAX_ESTADOS) {
        return 0;
    }

    *destino = *origem;

    int estado_novo = origem->qtd_estados;
    destino->qtd_estados = origem->qtd_estados + 1;

    for (int q = 0; q < origem->qtd_estados; q++) {
        if (origem->eh_inicial[q]) {
            destino->eh_inicial[q] = false;
            destino->transicao_lambda[estado_novo][q] = true;
        }
    }
    destino->eh_inicial[estado_novo] = true;
    strcpy(destino->nomes[estado_novo], "q_novo");
    return 1;
}

void calcular_fecho_lambda(const Automato *a, int estado, bool fecho[]) {
    bool mudou = true;

    for (int i = 0; i < MAX_ESTADOS; i++) {
        fecho[i] = false;
    }
    fecho[estado] = true;

    while (mudou) {
        mudou = false;
        for (int p = 0; p < a->qtd_estados; p++) {
            if (!fecho[p]) {
                continue;
            }
            for (int q = 0; q < a->qtd_estados; q++) {
                if (a->transicao_lambda[p][q] && !fecho[q]) {
                    fecho[q] = true;
                    mudou = true;
                }
            }
        }
    }
}

static void calcular_todos_fechos(const Automato *a, bool fechos[][MAX_ESTADOS]) {
    for (int q = 0; q < a->qtd_estados; q++) {
        calcular_fecho_lambda(a, q, fechos[q]);
    }
}

static void adicionar_destinos(const Automato *origem, Automato *destino,
                               bool fechos[][MAX_ESTADOS], int q, int p, int simbolo) {
    for (int r = 0; r < origem->qtd_estados; r++) {
        if (!origem->transicao[p][simbolo][r]) {
            continue;
        }
        for (int t = 0; t < origem->qtd_estados; t++) {
            if (fechos[r][t]) {
                destino->transicao[q][simbolo][t] = true;
            }
        }
    }
}

void remover_transicoes_lambda(const Automato *origem, Automato *destino) {
    bool fechos[MAX_ESTADOS][MAX_ESTADOS];

    inicializar_automato(destino);
    destino->qtd_estados = origem->qtd_estados;
    destino->qtd_simbolos = origem->qtd_simbolos;
    memcpy(destino->alfabeto, origem->alfabeto, sizeof(origem->alfabeto));
    memcpy(destino->nomes, origem->nomes, sizeof(origem->nomes));
    memcpy(destino->eh_inicial, origem->eh_inicial, sizeof(origem->eh_inicial));

    calcular_todos_fechos(origem, fechos);

    for (int q = 0; q < origem->qtd_estados; q++) {
        for (int p = 0; p < origem->qtd_estados; p++) {
            if (!fechos[q][p]) {
                continue;
            }
            if (origem->eh_final[p]) {
                destino->eh_final[q] = true;
            }
            for (int s = 0; s < origem->qtd_simbolos; s++) {
                adicionar_destinos(origem, destino, fechos, q, p, s);
            }
        }
    }
}