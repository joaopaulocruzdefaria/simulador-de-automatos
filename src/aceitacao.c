#include <string.h>
#include "aceitacao.h"
#include "conversao_lambda.h"

static void adicionar_fecho(const Automato *a, int estado, bool conjunto[]) {
    bool fecho[MAX_ESTADOS];

    calcular_fecho_lambda(a, estado, fecho);
    for (int i = 0; i < a->qtd_estados; i++) {
        if (fecho[i]) {
            conjunto[i] = true;
        }
    }
}

static void calcular_estados_iniciais(const Automato *a, bool atuais[]) {
    for (int q = 0; q < a->qtd_estados; q++) {
        if (a->eh_inicial[q]) {
            adicionar_fecho(a, q, atuais);
        }
    }
}

static void calcular_proximos(const Automato *a, const bool atuais[], int simbolo, bool proximos[]) {
    for (int p = 0; p < a->qtd_estados; p++) {
        if (!atuais[p]) {
            continue;
        }
        for (int q = 0; q < a->qtd_estados; q++) {
            if (a->transicao[p][simbolo][q]) {
                adicionar_fecho(a, q, proximos);
            }
        }
    }
}

static bool contem_estado_final(const Automato *a, const bool atuais[]) {
    for (int q = 0; q < a->qtd_estados; q++) {
        if (atuais[q] && a->eh_final[q]) {
            return true;
        }
    }
    return false;
}

bool testar_palavra(const Automato *a, const char *palavra) {
    bool atuais[MAX_ESTADOS] = {false};
    bool proximos[MAX_ESTADOS];

    calcular_estados_iniciais(a, atuais);

    for (int i = 0; palavra[i] != '\0'; i++) {
        int simbolo = indice_simbolo(a, palavra[i]);
        if (simbolo == -1) {
            return false;
        }
        memset(proximos, 0, sizeof(proximos));
        calcular_proximos(a, atuais, simbolo, proximos);
        memcpy(atuais, proximos, sizeof(atuais));
    }
    return contem_estado_final(a, atuais);
}