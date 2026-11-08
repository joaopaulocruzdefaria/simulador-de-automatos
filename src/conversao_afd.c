#include <stdio.h>
#include <string.h>
#include "conversao_afd.h"
#include "conversao_lambda.h"

static bool conjunto_vazio(const bool conjunto[], int qtd_estados) {
    for (int i = 0; i < qtd_estados; i++) {
        if (conjunto[i]) {
            return false;
        }
    }
    return true;
}

static bool conjuntos_iguais(const bool a[], const bool b[], int qtd_estados) {
    for (int i = 0; i < qtd_estados; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

int converter_afn_para_afd(const Automato *afn, Automato *afd) {
    Automato afn_limpo;
    remover_transicoes_lambda(afn, &afn_limpo);

    // inicializa o afd que vamos retornar no final
    inicializar_automato(afd);
    afd->qtd_simbolos = afn_limpo.qtd_simbolos;
    memcpy(afd->alfabeto, afn_limpo.alfabeto, sizeof(afn_limpo.alfabeto));

    // tabela de transição do afd
    bool tabela_estados_afd[MAX_ESTADOS][MAX_ESTADOS];
    int total_estados_afd = 0;

    // começa pelos estados iniciais do afn
    for (int q = 0; q < afn_limpo.qtd_estados; q++) {
        tabela_estados_afd[0][q] = afn_limpo.eh_inicial[q];
    }
    total_estados_afd = 1;
    afd->eh_inicial[0] = true;

    // passa linha a linha
    for (int i = 0; i < total_estados_afd; i++) {
        // passa por cada simbolo 
        for (int s = 0; s < afd->qtd_simbolos; s++) {
            // guarda para quais estados do AFN conseguimos ir
            bool destino_afn[MAX_ESTADOS] = {false};

            // calcula a união das transições de todos os estados do grupo
            for (int p = 0; p < afn_limpo.qtd_estados; p++) {
                if (!tabela_estados_afd[i][p]) {
                    continue;
                }
                for (int q = 0; q < afn_limpo.qtd_estados; q++) {
                    // [origem][simbolo][destino]
                    if (afn_limpo.transicao[p][s][q]) {
                        destino_afn[q] = true;
                    }
                }
            }

            // se não teve transição para esse símbolo, pula
            if (conjunto_vazio(destino_afn, afn_limpo.qtd_estados)) {
                continue;
            }

            // procura se esse grupo de destinos já existe na tabela do AFD
            int estado_destino_afd = -1;
            for (int k = 0; k < total_estados_afd; k++) {
                if (conjuntos_iguais(tabela_estados_afd[k], destino_afn, afn_limpo.qtd_estados)) {
                    estado_destino_afd = k;
                    break;
                }
            }

            // se não achou, cria uma nova linha na tabela do AFD
            if (estado_destino_afd == -1) {
                if (total_estados_afd >= MAX_ESTADOS) {
                    return 0;
                }
                estado_destino_afd = total_estados_afd;
                memcpy(tabela_estados_afd[estado_destino_afd], destino_afn, sizeof(bool) * MAX_ESTADOS);
                total_estados_afd++;
            }

            // liga a transição no AFD
            afd->transicao[i][s][estado_destino_afd] = true;
        }
    }

    // marca os novos estados finais e total de estados do AFD
    afd->qtd_estados = total_estados_afd;

    for (int i = 0; i < total_estados_afd; i++) {
        sprintf(afd->nomes[i], "S%d", i);

        for (int q = 0; q < afn_limpo.qtd_estados; q++) {
            if (tabela_estados_afd[i][q] && afn_limpo.eh_final[q]) {
                afd->eh_final[i] = true;
                break;
            }
        }
    }

    return 1;
}
