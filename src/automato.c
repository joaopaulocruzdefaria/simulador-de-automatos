#include <stdio.h>
#include <string.h>
#include "automato.h"

void inicializar_automato(Automato *a) {
    memset(a, 0, sizeof(Automato));
}

int indice_simbolo(const Automato *a, char simbolo) {
    for (int i = 0; i < a->qtd_simbolos; i++) {
        if (a->alfabeto[i] == simbolo) {
            return i;
        }
    }
    return -1;
}

void limpar_buffer(void) {
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

static int ler_inteiro(const char *mensagem, int minimo, int maximo, int *valor) {
    printf("%s", mensagem);
    if (scanf("%d", valor) != 1) {
        limpar_buffer();
        printf("Erro: entrada invalida.\n");
        return 0;
    }
    if (*valor < minimo || *valor > maximo) {
        printf("Erro: o valor deve estar entre %d e %d.\n", minimo, maximo);
        return 0;
    }
    return 1;
}

static int ler_alfabeto(Automato *a) {
    if (!ler_inteiro("Quantidade de simbolos (1-26): ", 1, MAX_SIMBOLOS, &a->qtd_simbolos)) {
        return 0;
    }
    printf("Simbolos do alfabeto (separados por espaco): ");
    for (int i = 0; i < a->qtd_simbolos; i++) {
        char simbolo;
        if (scanf(" %c", &simbolo) != 1) {
            limpar_buffer();
            printf("Erro: simbolo invalido.\n");
            return 0;
        }
        if (simbolo == '&' || indice_simbolo(a, simbolo) != -1) {
            printf("Erro: simbolo '%c' invalido ou repetido.\n", simbolo);
            return 0;
        }
        a->alfabeto[i] = simbolo;
    }
    return 1;
}

static int ler_conjunto_estados(const Automato *a, bool *marcados,
                                const char *mensagem_qtd, const char *mensagem_estados,
                                int minimo) {
    int quantidade;
    if (!ler_inteiro(mensagem_qtd, minimo, a->qtd_estados, &quantidade)) {
        return 0;
    }
    if (quantidade > 0) {
        printf("%s", mensagem_estados);
    }
    for (int i = 0; i < quantidade; i++) {
        int estado;
        if (scanf("%d", &estado) != 1) {
            limpar_buffer();
            printf("Erro: entrada invalida.\n");
            return 0;
        }
        if (estado < 0 || estado >= a->qtd_estados) {
            printf("Erro: estado %d fora do intervalo.\n", estado);
            return 0;
        }
        marcados[estado] = true;
    }
    return 1;
}

static int ler_transicoes(Automato *a) {
    int quantidade;
    int limite = MAX_ESTADOS * MAX_SIMBOLOS * MAX_ESTADOS;

    printf("Transicoes no formato 'origem simbolo destino' (use '&' para lambda).\n");
    if (!ler_inteiro("Quantidade de transicoes: ", 0, limite, &quantidade)) {
        return 0;
    }
    for (int i = 0; i < quantidade; i++) {
        int origem, destino;
        char simbolo;

        printf("Transicao %d: ", i + 1);
        if (scanf("%d %c %d", &origem, &simbolo, &destino) != 3) {
            limpar_buffer();
            printf("Erro: formato invalido.\n");
            return 0;
        }
        if (origem < 0 || origem >= a->qtd_estados || destino < 0 || destino >= a->qtd_estados) {
            printf("Erro: estado fora do intervalo.\n");
            return 0;
        }
        if (simbolo == '&') {
            a->transicao_lambda[origem][destino] = true;
        } else {
            int posicao = indice_simbolo(a, simbolo);
            if (posicao == -1) {
                printf("Erro: simbolo '%c' nao pertence ao alfabeto.\n", simbolo);
                return 0;
            }
            a->transicao[origem][posicao][destino] = true;
        }
    }
    return 1;
}

int ler_automato(Automato *a) {
    inicializar_automato(a);

    if (!ler_inteiro("Quantidade de estados (1-64): ", 1, MAX_ESTADOS, &a->qtd_estados)) {
        return 0;
    }
    if (!ler_alfabeto(a)) {
        return 0;
    }
    if (!ler_conjunto_estados(a, a->eh_inicial,
                              "Quantidade de estados iniciais: ",
                              "Estados iniciais: ", 1)) {
        return 0;
    }
    if (!ler_conjunto_estados(a, a->eh_final,
                              "Quantidade de estados finais: ",
                              "Estados finais: ", 0)) {
        return 0;
    }
    return ler_transicoes(a);
}

static void imprimir_nome(const Automato *a, int estado) {
    if (a->nomes[estado][0] == '\0') {
        printf("q%d", estado);
    } else {
        printf("%s", a->nomes[estado]);
    }
}

static void imprimir_conjunto(const Automato *a, const bool *marcados) {
    bool primeiro = true;

    printf("{");
    for (int i = 0; i < a->qtd_estados; i++) {
        if (marcados[i]) {
            if (!primeiro) {
                printf(", ");
            }
            imprimir_nome(a, i);
            primeiro = false;
        }
    }
    printf("}");
}

static void imprimir_nome_estado(const Automato *a, int estado) {
    printf("%s%s", a->eh_inicial[estado] ? "->" : "", a->eh_final[estado] ? "*" : "");
    imprimir_nome(a, estado);
}

void imprimir_automato(const Automato *a) {
    printf("\n===== AUTOMATO =====\n");

    printf("Estados  : {");
    for (int i = 0; i < a->qtd_estados; i++) {
        if (i > 0) {
            printf(", ");
        }
        imprimir_nome(a, i);
    }
    printf("}\n");

    printf("Alfabeto : {");
    for (int i = 0; i < a->qtd_simbolos; i++) {
        printf("%s%c", i > 0 ? ", " : "", a->alfabeto[i]);
    }
    printf("}\n");

    printf("Iniciais : ");
    imprimir_conjunto(a, a->eh_inicial);
    printf("\n");

    printf("Finais   : ");
    imprimir_conjunto(a, a->eh_final);
    printf("\n");

    printf("\nTabela de transicoes (->inicial, *final):\n");
    printf("estado");
    for (int s = 0; s < a->qtd_simbolos; s++) {
        printf(" | %c", a->alfabeto[s]);
    }
    printf(" | &\n");

    for (int i = 0; i < a->qtd_estados; i++) {
        imprimir_nome_estado(a, i);
        for (int s = 0; s < a->qtd_simbolos; s++) {
            printf(" | ");
            imprimir_conjunto(a, a->transicao[i][s]);
        }
        printf(" | ");
        imprimir_conjunto(a, a->transicao_lambda[i]);
        printf("\n");
    }
    printf("====================\n");
}