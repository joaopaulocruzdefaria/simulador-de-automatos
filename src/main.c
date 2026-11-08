#include <stdio.h>
#include <string.h>
#include "automato.h"
#include "conversao_lambda.h"
#include "conversao_afd.h"
#include "aceitacao.h"
#include "json_automato.h"

#define TAMANHO_PALAVRA 256
#define TAMANHO_CAMINHO 256

static Automato atual;
static Automato resultado;
static PalavrasTeste palavras_teste;
static bool carregado = false;
static char caminho_arquivo[TAMANHO_CAMINHO];
static char nome_automato[TAMANHO_TEXTO];

static void mostrar_menu(void) {
    printf("\n===== SIMULADOR DE AUTOMATOS =====\n");
    printf("1  - Carregar automato de arquivo JSON\n");
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
    printf("12 - Ler automato pelo teclado\n");
    printf("13 - Executar palavras-teste do arquivo JSON\n");
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
        printf("Erro: carregue um automato primeiro (opcao 1 ou 12).\n");
    }
    return carregado;
}

static void opcao_nao_integrada(const char *nome) {
    printf("%s ainda nao foi integrado ao menu.\n", nome);
}

static void opcao_carregar_json(void) {
    printf("Caminho do arquivo JSON: ");
    if (scanf(" %255[^\n]", caminho_arquivo) != 1) {
        return;
    }
    printf("Nome do automato (ex: automato 1): ");
    if (scanf(" %63[^\n]", nome_automato) != 1) {
        return;
    }
    carregado = carregar_automato_json(caminho_arquivo, nome_automato, &atual);
    if (carregado) {
        printf("Automato '%s' carregado com sucesso.\n", nome_automato);
        imprimir_automato(&atual);
    }
}

static void opcao_ler(void) {
    caminho_arquivo[0] = '\0';
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

static void opcao_converter_afd(void) {
    if (!exigir_automato()) {
        return;
    }
    if (!converter_afn_para_afd(&atual, &resultado)) {
        printf("Erro: limite de estados atingido na conversao para AFD.\n");
        return;
    }
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

static void executar_lista(const ListaTextos *palavras, bool esperado) {
    for (int i = 0; i < palavras->quantidade; i++) {
        bool aceita = testar_palavra(&atual, palavras->itens[i]);

        printf("  \"%s\" -> %s (%s)\n", palavras->itens[i],
               aceita ? "ACEITA" : "REJEITADA",
               aceita == esperado ? "ok" : "ERRO");
    }
}

static void opcao_palavras_teste(void) {
    if (!exigir_automato()) {
        return;
    }
    if (caminho_arquivo[0] == '\0') {
        printf("Erro: o automato atual nao veio de um arquivo JSON (use a opcao 1).\n");
        return;
    }
    if (!carregar_palavras_teste(caminho_arquivo, &palavras_teste)) {
        return;
    }
    printf("Palavras que deveriam ser aceitas:\n");
    executar_lista(&palavras_teste.aceitas, true);
    printf("Palavras que deveriam ser rejeitadas:\n");
    executar_lista(&palavras_teste.rejeitadas, false);
}

int main(void) {
    int opcao;

    do {
        mostrar_menu();
        opcao = ler_opcao();

        switch (opcao) {
            case 0:  printf("Encerrando.\n"); break;
            case 1:  opcao_carregar_json(); break;
            case 2:  opcao_imprimir(); break;
            case 3:  opcao_multiplos_iniciais(); break;
            case 4:  opcao_remover_lambda(); break;
            case 5:  opcao_testar_palavra(); break;
            case 6:  opcao_converter_afd(); break;
            case 7:  opcao_nao_integrada("Minimizacao de AFD"); break;
            case 8:  opcao_nao_integrada("Uniao de AFDs"); break;
            case 9:  opcao_nao_integrada("Intersecao de AFDs"); break;
            case 10: opcao_nao_integrada("Subtracao de AFDs"); break;
            case 11: opcao_nao_integrada("Complemento de AFD"); break;
            case 12: opcao_ler(); break;
            case 13: opcao_palavras_teste(); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 0);

    return 0;
}