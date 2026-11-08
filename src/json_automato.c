#include <stdio.h>
#include <string.h>
#include "json_automato.h"

#define TAMANHO_ARQUIVO 65536

static char texto[TAMANHO_ARQUIVO];
static ListaTextos lista;

static bool eh_abertura(char caractere) {
    return caractere == '[' || caractere == '{';
}

static bool eh_fechamento(char caractere) {
    return caractere == ']' || caractere == '}';
}

static bool ler_arquivo(const char *caminho) {
    FILE *arquivo = fopen(caminho, "r");
    size_t lidos;

    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir '%s'.\n", caminho);
        return false;
    }
    lidos = fread(texto, 1, TAMANHO_ARQUIVO - 1, arquivo);
    texto[lidos] = '\0';
    if (fgetc(arquivo) != EOF) {
        printf("Erro: arquivo grande demais.\n");
        fclose(arquivo);
        return false;
    }
    fclose(arquivo);
    return true;
}

static char *encontrar_bloco(char *base, const char *chave, int *tamanho) {
    char padrao[TAMANHO_TEXTO + 3];
    char *inicio;
    int profundidade = 0;
    int i = 0;

    snprintf(padrao, sizeof(padrao), "\"%s\"", chave);
    inicio = strstr(base, padrao);
    if (inicio == NULL) {
        printf("Erro: chave '%s' nao encontrada no arquivo.\n", chave);
        return NULL;
    }
    inicio = strpbrk(inicio + strlen(padrao), "[{");
    if (inicio == NULL) {
        printf("Erro: formato invalido depois de '%s'.\n", chave);
        return NULL;
    }
    do {
        if (inicio[i] == '"') {
            i++;
            while (inicio[i] != '"' && inicio[i] != '\0') {
                i++;
            }
        } else if (eh_abertura(inicio[i])) {
            profundidade++;
        } else if (eh_fechamento(inicio[i])) {
            profundidade--;
        }
        if (inicio[i] != '\0') {
            i++;
        }
    } while (profundidade > 0 && inicio[i] != '\0');

    if (profundidade != 0) {
        printf("Erro: '%s' tem colchetes ou chaves sem fechar.\n", chave);
        return NULL;
    }
    *tamanho = i;
    return inicio;
}

static bool guardar_texto(const char *json, int *posicao) {
    char *destino;
    int tamanho = 0;

    if (lista.quantidade >= MAX_ITENS) {
        printf("Erro: itens demais em uma lista.\n");
        return false;
    }
    destino = lista.itens[lista.quantidade];
    (*posicao)++;
    while (json[*posicao] != '"' && json[*posicao] != '\0') {
        if (tamanho >= TAMANHO_TEXTO - 1) {
            printf("Erro: texto com mais de %d caracteres.\n", TAMANHO_TEXTO - 1);
            return false;
        }
        destino[tamanho] = json[*posicao];
        tamanho++;
        (*posicao)++;
    }
    destino[tamanho] = '\0';
    lista.quantidade++;
    return true;
}

static bool ler_lista(char *bloco, const char *chave) {
    int tamanho;
    char *conteudo = encontrar_bloco(bloco, chave, &tamanho);

    if (conteudo == NULL) {
        return false;
    }
    lista.quantidade = 0;
    for (int i = 0; i < tamanho; i++) {
        if (conteudo[i] == '"' && !guardar_texto(conteudo, &i)) {
            return false;
        }
    }
    return true;
}

static int indice_estado(const Automato *a, const char *nome) {
    for (int i = 0; i < a->qtd_estados; i++) {
        if (strcmp(a->nomes[i], nome) == 0) {
            return i;
        }
    }
    return -1;
}

static bool carregar_alfabeto(char *bloco, Automato *a) {
    if (!ler_lista(bloco, "alfabeto")) {
        return false;
    }
    if (lista.quantidade < 1 || lista.quantidade > MAX_SIMBOLOS) {
        printf("Erro: o alfabeto deve ter de 1 a %d simbolos.\n", MAX_SIMBOLOS);
        return false;
    }
    for (int i = 0; i < lista.quantidade; i++) {
        char simbolo = lista.itens[i][0];

        if (strlen(lista.itens[i]) != 1 || simbolo == '&') {
            printf("Erro: simbolo '%s' invalido no alfabeto.\n", lista.itens[i]);
            return false;
        }
        if (indice_simbolo(a, simbolo) != -1) {
            printf("Erro: simbolo '%c' repetido no alfabeto.\n", simbolo);
            return false;
        }
        a->alfabeto[a->qtd_simbolos] = simbolo;
        a->qtd_simbolos++;
    }
    return true;
}

static bool carregar_estados(char *bloco, Automato *a) {
    if (!ler_lista(bloco, "estados")) {
        return false;
    }
    if (lista.quantidade < 1 || lista.quantidade > MAX_ESTADOS) {
        printf("Erro: o automato deve ter de 1 a %d estados.\n", MAX_ESTADOS);
        return false;
    }
    for (int i = 0; i < lista.quantidade; i++) {
        if (indice_estado(a, lista.itens[i]) != -1) {
            printf("Erro: estado '%s' repetido.\n", lista.itens[i]);
            return false;
        }
        strcpy(a->nomes[i], lista.itens[i]);
        a->qtd_estados++;
    }
    return true;
}

static bool carregar_conjunto(char *bloco, const char *chave, Automato *a,
                              bool *marcados, int minimo) {
    if (!ler_lista(bloco, chave)) {
        return false;
    }
    if (lista.quantidade < minimo) {
        printf("Erro: '%s' precisa ter pelo menos %d estado(s).\n", chave, minimo);
        return false;
    }
    for (int i = 0; i < lista.quantidade; i++) {
        int estado = indice_estado(a, lista.itens[i]);

        if (estado == -1) {
            printf("Erro: estado '%s' (em '%s') nao foi declarado.\n", lista.itens[i], chave);
            return false;
        }
        marcados[estado] = true;
    }
    return true;
}

static bool carregar_transicoes(char *bloco, Automato *a) {
    if (!ler_lista(bloco, "transicoes")) {
        return false;
    }
    if (lista.quantidade % 3 != 0) {
        printf("Erro: cada transicao deve ter origem, destino e simbolo.\n");
        return false;
    }
    for (int i = 0; i < lista.quantidade; i += 3) {
        int origem = indice_estado(a, lista.itens[i]);
        int destino = indice_estado(a, lista.itens[i + 1]);
        const char *simbolo = lista.itens[i + 2];

        if (origem == -1 || destino == -1) {
            printf("Erro: transicao com estado nao declarado (%s, %s, %s).\n",
                   lista.itens[i], lista.itens[i + 1], simbolo);
            return false;
        }
        if (strcmp(simbolo, "&") == 0) {
            a->transicao_lambda[origem][destino] = true;
            continue;
        }
        int posicao = strlen(simbolo) == 1 ? indice_simbolo(a, simbolo[0]) : -1;
        if (posicao == -1) {
            printf("Erro: simbolo '%s' nao pertence ao alfabeto.\n", simbolo);
            return false;
        }
        a->transicao[origem][posicao][destino] = true;
    }
    return true;
}

bool carregar_automato_json(const char *caminho, const char *nome_automato, Automato *a) {
    int tamanho;
    char *bloco;

    if (!ler_arquivo(caminho)) {
        return false;
    }
    bloco = encontrar_bloco(texto, nome_automato, &tamanho);
    if (bloco == NULL) {
        return false;
    }
    bloco[tamanho] = '\0';

    inicializar_automato(a);
    return carregar_alfabeto(bloco, a)
        && carregar_estados(bloco, a)
        && carregar_conjunto(bloco, "estados-iniciais", a, a->eh_inicial, 1)
        && carregar_conjunto(bloco, "estados-finais", a, a->eh_final, 0)
        && carregar_transicoes(bloco, a);
}

bool carregar_palavras_teste(const char *caminho, PalavrasTeste *teste) {
    int tamanho;
    char *bloco;

    if (!ler_arquivo(caminho)) {
        return false;
    }
    bloco = encontrar_bloco(texto, "palavra-teste", &tamanho);
    if (bloco == NULL) {
        return false;
    }
    bloco[tamanho] = '\0';

    if (!ler_lista(bloco, "aceita")) {
        return false;
    }
    teste->aceitas = lista;
    if (!ler_lista(bloco, "rejeita")) {
        return false;
    }
    teste->rejeitadas = lista;
    return true;
}