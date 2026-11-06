#include <stdio.h>
#include <string.h>
#include "automato.h"
#include "conversao_lambda.h"
#include "aceitacao.h"

#define TAMANHO_PALAVRA 256

static Automato atual;
static Automato resultado;
static bool carregado = false;

static void mostrar_menu(void) {
    printf("\n===== SIMULADOR DE AUTOMATOS =====\n");
    printf("1  - Ler automato\n");
    printf("2  - Imprimir automato\n");
    printf("3  - Multiplos iniciais para AFN-lambda\n");
    printf("4  - AFN-lambda para AFN\n");
    printf("5  - Testar palavra\n");
    printf("6  - AFN para AFD\n");
    printf("7  - Minimizar AFD\n");
    printf("8  - Uniao de AFDs\n");
    printf("9  - Intersecao de AFDs\n");
    printf("10 - Subtracao de AFDs\n");
    printf("11 - Complemento de AFD\n");
    printf("0  - Sair\n");
}

static int ler_opcao(void) {
    int opcao;
    int leitura;

    printf("Opcao: ");
    leitura = scanf("%d", &opcao);
    if (leitura == EOF) {
        return 0;
    }
    if (leitura != 1) {
        limpar_buffer();
        return -1;
    }
    return opcao;
}

static bool exigir_automato(void) {
    if (!carregado) {
        printf("Erro: leia um automato primeiro (opcao 1).\n");
    }
    return carregado;
}

static void opcao_nao_integrada(const char *nome) {
    printf("%s ainda nao foi integrado ao menu.\n", nome);
}

static void opcao_ler(void) {
    carregado = ler_automato(&atual) == 1;
    if (carregado) {
        printf("Automato lido com sucesso.\n");
    }
}

static void opcao_imprimir(void) {
    if (exigir_automato()) {
        imprimir_automato(&atual);
    }
}

static void opcao_multiplos_iniciais(void) {
    if (!exigir_automato()) {
        return;
    }
    if (!converter_multiplos_iniciais_para_lambda(&atual, &resultado)) {
        printf("Erro: limite de estados atingido.\n");
        return;
    }
    atual = resultado;
    imprimir_automato(&atual);
}

static void opcao_remover_lambda(void) {
    if (!exigir_automato()) {
        return;
    }
    remover_transicoes_lambda(&atual, &resultado);
    atual = resultado;
    imprimir_automato(&atual);
}

static void opcao_testar_palavra(void) {
    char palavra[TAMANHO_PALAVRA];

    if (!exigir_automato()) {
        return;
    }
    printf("Palavra (use & para a palavra vazia): ");
    if (scanf("%255s", palavra) != 1) {
        return;
    }
    if (strcmp(palavra, "&") == 0) {
        palavra[0] = '\0';
    }
    if (testar_palavra(&atual, palavra)) {
        printf("Palavra ACEITA.\n");
    } else {
        printf("Palavra REJEITADA.\n");
    }
}

int main(void) {
    int opcao;

    do {
        mostrar_menu();
        opcao = ler_opcao();

        switch (opcao) {
            case 0:  printf("Encerrando.\n"); break;
            case 1:  opcao_ler(); break;
            case 2:  opcao_imprimir(); break;
            case 3:  opcao_multiplos_iniciais(); break;
            case 4:  opcao_remover_lambda(); break;
            case 5:  opcao_testar_palavra(); break;
            case 6:  opcao_nao_integrada("AFN para AFD"); break;
            case 7:  opcao_nao_integrada("Minimizacao de AFD"); break;
            case 8:  opcao_nao_integrada("Uniao de AFDs"); break;
            case 9:  opcao_nao_integrada("Intersecao de AFDs"); break;
            case 10: opcao_nao_integrada("Subtracao de AFDs"); break;
            case 11: opcao_nao_integrada("Complemento de AFD"); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 0);

    return 0;
}