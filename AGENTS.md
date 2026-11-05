# Diretrizes do Projeto — Simulador de Autômatos (LFA)

Este arquivo define as regras estritas de desenvolvimento e estilo de código para todos os agentes e desenvolvedores trabalhando neste repositório.

## 1. Contexto e Objetivo
Trabalho acadêmico da disciplina de Linguagens Formais e Autômatos (LFA), dividido entre 4 integrantes (João Paulo, Pedro Galvão, Bernardo e Otávio). O foco principal é que o código seja **fácil de ler, entender e explicar oralmente para o professor**.

## 2. Regras Obrigatórias de Código

1. **C Puro (Standard C):**
   - Utilizar apenas C puro (`<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<stdbool.h>`).
   - Compilável com `gcc -Wall -Wextra -std=c99` sem avisos.

2. **100% em Português (PT-BR):**
   - Todos os nomes de `struct`, variáveis, funções, constantes (`#define`), arquivos e mensagens de saída devem estar em português claro (sem acentos em identificadores).
   - Exemplo: `Automato`, `qtd_estados`, `eh_final`, `estado_inicial`, `converter_afn_para_afd`, `testar_palavra`, `uniao_afd`.

3. **Simplicidade Extrema (Foco Didático):**
   - **Proibido** usar estruturas complexas desnecessárias (listas encadeadas genéricas, árvores, tabelas hash, ponteiros triplos ou macros obscuras).
   - **Preferir alocação estática com limites fixos** (ex: `#define MAX_ESTADOS 64`, `#define MAX_SIMBOLOS 26`), usando matrizes simples e vetores de inteiros/booleanos.
   - Funções curtas, diretas, com laços `for` e `while` explícitos que reflitam diretamente a definição matemática vista em aula.

4. **Pouco ou Nenhum Comentário:**
   - **Não escreva comentários explicando o óbvio, cabeçalhos de funções ou blocos de documentação (Doxygen).**
   - O código deve ter zero ou quase zero comentários. A clareza deve vir exclusivamente de bons nomes em português e lógica limpa.

## 3. Requisitos do Simulador

1. Converter AFN com múltiplos estados iniciais para AFN-$\lambda$ e vice-versa.
2. Converter AFN para AFD (Construção de Subconjuntos).
3. Minimizar AFD.
4. Teste de aceitação de palavras.
5. União de 2 AFDs.
6. Interseção de 2 AFDs.
7. Subtração (diferença) de 2 AFDs.
8. Complemento de 1 AFD.

## 4. Divisão de Módulos Sugerida

- **Otávio:** Estrutura base (`automato.h`/`automato.c`), I/O, Teste de Palavras (Req. 4) e Conversão AFN múltiplos iniciais $\leftrightarrow$ AFN-$\lambda$ (Req. 1).
- **Pedro Galvão:** Conversão de AFN para AFD (Req. 2).
- **Bernardo:** Minimização de AFD (Req. 3).
- **João Paulo:** Operações sobre AFDs — União, Interseção, Subtração e Complemento (Req. 5, 6, 7 e 8).
