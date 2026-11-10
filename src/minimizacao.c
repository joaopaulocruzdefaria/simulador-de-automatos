#include <stdio.h>
#include <string.h>
#include "minimizacao.h"

#define TAMANHO_TABELA (MAX_ESTADOS + 1)

int obter_destino_afd(const Automato *afd, int estado, int simbolo) {
    for (int q = 0; q < afd->qtd_estados; q++) {
        if (afd->transicao[estado][simbolo][q]) {
            return q;
        }
    }
    return -1;
}

bool eh_afd(const Automato *a) {
    int qtd_iniciais = 0;

    for (int p = 0; p < a->qtd_estados; p++) {
        if (a->eh_inicial[p]) {
            qtd_iniciais++;
        }
        for (int q = 0; q < a->qtd_estados; q++) {
            if (a->transicao_lambda[p][q]) {
                return false;
            }
        }
        for (int s = 0; s < a->qtd_simbolos; s++) {
            int qtd_destinos = 0;
            for (int q = 0; q < a->qtd_estados; q++) {
                if (a->transicao[p][s][q]) {
                    qtd_destinos++;
                }
            }
            if (qtd_destinos > 1) {
                return false;
            }
        }
    }
    return qtd_iniciais == 1;
}

static int obter_estado_inicial(const Automato *afd) {
    for (int q = 0; q < afd->qtd_estados; q++) {
        if (afd->eh_inicial[q]) {
            return q;
        }
    }
    return -1;
}

void remover_inalcancaveis(Automato *afd) {
    bool alcancavel[MAX_ESTADOS] = {false};
    int fila[MAX_ESTADOS];
    int inicio = 0;
    int fim = 0;
    int inicial = obter_estado_inicial(afd);

    alcancavel[inicial] = true;
    fila[fim++] = inicial;

    while (inicio < fim) {
        int atual = fila[inicio++];
        for (int s = 0; s < afd->qtd_simbolos; s++) {
            int destino = obter_destino_afd(afd, atual, s);
            if (destino != -1 && !alcancavel[destino]) {
                alcancavel[destino] = true;
                fila[fim++] = destino;
            }
        }
    }

    int novo_indice[MAX_ESTADOS];
    int qtd_alcancaveis = 0;
    for (int q = 0; q < afd->qtd_estados; q++) {
        if (alcancavel[q]) {
            novo_indice[q] = qtd_alcancaveis++;
        } else {
            novo_indice[q] = -1;
        }
    }

    Automato original = *afd;
    inicializar_automato(afd);
    afd->qtd_estados = qtd_alcancaveis;
    afd->qtd_simbolos = original.qtd_simbolos;
    memcpy(afd->alfabeto, original.alfabeto, sizeof(original.alfabeto));

    for (int q = 0; q < original.qtd_estados; q++) {
        int novo = novo_indice[q];
        if (novo == -1) {
            continue;
        }
        if (original.nomes[q][0] == '\0') {
            sprintf(afd->nomes[novo], "q%d", q);
        } else {
            strcpy(afd->nomes[novo], original.nomes[q]);
        }
        afd->eh_inicial[novo] = original.eh_inicial[q];
        afd->eh_final[novo] = original.eh_final[q];

        for (int s = 0; s < original.qtd_simbolos; s++) {
            int destino = obter_destino_afd(&original, q, s);
            if (destino != -1) {
                afd->transicao[novo][s][novo_indice[destino]] = true;
            }
        }
    }
}

static int destino_com_poco(const Automato *afd, int estado, int simbolo) {
    int poco = afd->qtd_estados;
    if (estado == poco) {
        return poco;
    }
    int destino = obter_destino_afd(afd, estado, simbolo);
    if (destino == -1) {
        return poco;
    }
    return destino;
}

static bool eh_final_com_poco(const Automato *afd, int estado) {
    return estado < afd->qtd_estados && afd->eh_final[estado];
}

static void construir_tabela_distinguiveis(const Automato *afd,
                                           bool distinguivel[TAMANHO_TABELA][TAMANHO_TABELA]) {
    int total = afd->qtd_estados + 1;

    for (int p = 0; p < total; p++) {
        for (int q = 0; q < total; q++) {
            distinguivel[p][q] = eh_final_com_poco(afd, p) != eh_final_com_poco(afd, q);
        }
    }

    bool mudou = true;
    while (mudou) {
        mudou = false;
        for (int p = 0; p < total; p++) {
            for (int q = p + 1; q < total; q++) {
                if (distinguivel[p][q]) {
                    continue;
                }
                for (int s = 0; s < afd->qtd_simbolos; s++) {
                    int destino_p = destino_com_poco(afd, p, s);
                    int destino_q = destino_com_poco(afd, q, s);
                    if (distinguivel[destino_p][destino_q]) {
                        distinguivel[p][q] = true;
                        distinguivel[q][p] = true;
                        mudou = true;
                        break;
                    }
                }
            }
        }
    }
}

static int agrupar_equivalentes(const Automato *afd,
                                bool distinguivel[TAMANHO_TABELA][TAMANHO_TABELA],
                                int grupo[]) {
    int qtd_grupos = 0;

    for (int q = 0; q < afd->qtd_estados; q++) {
        grupo[q] = -1;
    }
    for (int p = 0; p < afd->qtd_estados; p++) {
        if (grupo[p] != -1) {
            continue;
        }
        grupo[p] = qtd_grupos;
        for (int q = p + 1; q < afd->qtd_estados; q++) {
            if (grupo[q] == -1 && !distinguivel[p][q]) {
                grupo[q] = qtd_grupos;
            }
        }
        qtd_grupos++;
    }
    return qtd_grupos;
}

static void nomear_grupo(const Automato *afd, const int grupo[], int numero_grupo, char nome[]) {
    char texto[TAMANHO_NOME * TAMANHO_TABELA];
    bool primeiro = true;

    strcpy(texto, "{");
    for (int q = 0; q < afd->qtd_estados; q++) {
        if (grupo[q] == numero_grupo) {
            if (!primeiro) {
                strcat(texto, ",");
            }
            strcat(texto, afd->nomes[q]);
            primeiro = false;
        }
    }
    strcat(texto, "}");

    texto[TAMANHO_NOME - 1] = '\0';
    strcpy(nome, texto);
}

int minimizar_afd(const Automato *afd, Automato *minimo) {
    if (!eh_afd(afd)) {
        return 0;
    }

    Automato alcancaveis = *afd;
    remover_inalcancaveis(&alcancaveis);

    bool distinguivel[TAMANHO_TABELA][TAMANHO_TABELA];
    construir_tabela_distinguiveis(&alcancaveis, distinguivel);

    int grupo[MAX_ESTADOS];
    int qtd_grupos = agrupar_equivalentes(&alcancaveis, distinguivel, grupo);

    inicializar_automato(minimo);
    minimo->qtd_estados = qtd_grupos;
    minimo->qtd_simbolos = alcancaveis.qtd_simbolos;
    memcpy(minimo->alfabeto, alcancaveis.alfabeto, sizeof(alcancaveis.alfabeto));

    for (int q = 0; q < alcancaveis.qtd_estados; q++) {
        int g = grupo[q];
        if (alcancaveis.eh_inicial[q]) {
            minimo->eh_inicial[g] = true;
        }
        if (alcancaveis.eh_final[q]) {
            minimo->eh_final[g] = true;
        }
        for (int s = 0; s < alcancaveis.qtd_simbolos; s++) {
            int destino = obter_destino_afd(&alcancaveis, q, s);
            if (destino != -1) {
                minimo->transicao[g][s][grupo[destino]] = true;
            }
        }
    }

    for (int g = 0; g < qtd_grupos; g++) {
        nomear_grupo(&alcancaveis, grupo, g, minimo->nomes[g]);
    }
    return 1;
}
