# Especificação de Requisitos e Roteiro — Simulador de Autômatos (LFA)

## 1. Visão Geral
Simulador de Autômatos Finitos desenvolvido em C puro para a disciplina de Linguagens Formais e Autômatos (LFA). O objetivo do projeto é implementar representações e operações clássicas sobre Autômatos Finitos Determinísticos (AFD), Não-Determinísticos (AFN) e com movimentos vazios (AFN-$\lambda$), prezando pela clareza algorítmica e facilidade de explicação em arguição oral.

## 2. Requisitos Funcionais

| ID | Requisito | Entrada | Saída | Descrição |
| :--- | :--- | :--- | :--- | :--- |
| **RF01** | Conversão AFN (múltiplos estados iniciais) $\leftrightarrow$ AFN-$\lambda$ | AFN com múltiplos estados iniciais / AFN-$\lambda$ | AFN-$\lambda$ com estado inicial único / AFN equivalente sem transições $\lambda$ | Ida: unificar múltiplos estados iniciais criando um novo estado inicial com transições $\lambda$. Volta: calcular o fecho-$\lambda$ ($\lambda$-closure) e remover transições vazias. |
| **RF02** | Conversão de AFN para AFD | AFN | AFD equivalente | Algoritmo de construção de subconjuntos (*subset construction*). |
| **RF03** | Minimização de AFD | AFD | AFD mínimo equivalente | Remoção de estados inalcançáveis/inúteis e unificação de estados equivalentes (algoritmo de preenchimento de tabela / Myhill-Nerode). |
| **RF04** | Teste de Palavras | Autômato e cadeia de símbolos | Aceita / Rejeita | Simulação da leitura de uma palavra passo a passo sobre o autômato. |
| **RF05** | União de Autômatos | 2 AFDs ($M_1, M_2$) | 1 AFD ($M_1 \cup M_2$) | Construção do autômato produto aceitando $L(M_1) \cup L(M_2)$. |
| **RF06** | Interseção de Autômatos | 2 AFDs ($M_1, M_2$) | 1 AFD ($M_1 \cap M_2$) | Construção do autômato produto aceitando $L(M_1) \cap L(M_2)$. |
| **RF07** | Subtração (Diferença) de Autômatos | 2 AFDs ($M_1, M_2$) | 1 AFD ($M_1 - M_2$) | Construção do autômato produto aceitando palavras em $L(M_1)$ que não estão em $L(M_2)$. |
| **RF08** | Complemento de Autômato | 1 AFD ($M_1$) | 1 AFD ($\overline{M_1}$) | Totalização da função de transição (estado poço/erro, se necessário) e inversão dos estados finais e não-finais. |

## 3. Requisitos Não-Funcionais e Restrições

- **RNF01 — Linguagem:** Todo o projeto deve ser escrito estritamente em **C puro** (padrão C99, apenas biblioteca padrão).
- **RNF02 — Idioma:** Todo o código-fonte (identificadores, variáveis, funções, structs, mensagens de interface e documentação) deve estar em **Português (PT-BR)**.
- **RNF03 — Simplicidade e Didática:** A implementação deve ser a mais simples e direta possível (uso de matrizes/vetores de tamanho fixo via `#define`, evitando alocação dinâmica complexa), pois o código será defendido e explicado ao professor.
- **RNF04 — Código Limpo de Comentários:** Manter **pouco ou nenhum comentário** ao longo do código. O próprio nome das variáveis e funções em português deve tornar o código autoexplicativo.

---

## 4. Checklist Passo a Passo por Integrante

Para facilitar o desenvolvimento ponto a ponto e o entendimento de cada algoritmo, siga a ordem abaixo dentro do seu módulo.

### 1. Otávio — Base + Req. 1 e Req. 4 (`automato.h`, `automato.c`, `main.c`)

- [ ] **Passo 1 (Estrutura Base em `automato.h`):**
  - Definir constantes `MAX_ESTADOS` (ex: 64) e `MAX_SIMBOLOS` (ex: 26).
  - Criar a `struct Automato` contendo: `qtd_estados`, `qtd_simbolos`, vetor `alfabeto[MAX_SIMBOLOS]`, vetores booleanos `eh_inicial[MAX_ESTADOS]` e `eh_final[MAX_ESTADOS]`, matriz de transições `transicao[MAX_ESTADOS][MAX_SIMBOLOS][MAX_ESTADOS]` e matriz de transições vazias `transicao_lambda[MAX_ESTADOS][MAX_ESTADOS]`.
- [ ] **Passo 2 (Inicialização e Entrada/Saída em `automato.c`):**
  - Implementar `inicializar_automato(Automato *a)` zerando todas as matrizes e vetores.
  - Implementar `ler_automato(Automato *a)` e `imprimir_automato(Automato *a)` mostrando estados, iniciais, finais e tabela de transições.
- [ ] **Passo 3 (Req. 1 Parte A — Múltiplos Iniciais para AFN-$\lambda$):**
  - Implementar `converter_multiplos_iniciais_para_lambda(Automato *origem, Automato *destino)`:
    1. Copiar o autômato de origem para o destino.
    2. Criar um único novo estado inicial $q_{novo}$.
    3. Para cada estado $q$ que era inicial na origem, desmarcar `eh_inicial[q]` e adicionar `transicao_lambda[q_novo][q] = true`.
- [ ] **Passo 4 (Req. 1 Parte B — AFN-$\lambda$ para AFN sem $\lambda$):**
  - Implementar função auxiliar `calcular_fecho_lambda(Automato *a, int estado, bool fecho[])` (busca em largura/profundidade marcando todos os estados alcançáveis apenas por transições $\lambda$).
  - Implementar `remover_transicoes_lambda(Automato *origem, Automato *destino)`:
    1. Para cada estado $q$, calcular seu fecho-$\lambda$.
    2. Se o fecho-$\lambda$ de $q$ contém algum estado final, $q$ passa a ser final.
    3. Para cada símbolo $s$, a nova transição de $q$ por $s$ será a união das transições por $s$ dos estados no fecho-$\lambda$ de $q$ (seguida do fecho-$\lambda$ dos destinos).
    4. Zerar a matriz `transicao_lambda`.
- [ ] **Passo 5 (Req. 4 — Teste de Palavras):**
  - Implementar `testar_palavra(Automato *a, const char *palavra)`:
    1. Começar com o conjunto `atuais` contendo os estados iniciais (e seus fechos-$\lambda$).
    2. Para cada caractere da palavra, descobrir o índice do símbolo no alfabeto e calcular o conjunto `proximos` com todos os destinos possíveis a partir de `atuais`.
    3. Ao terminar a palavra, retornar `true` se algum estado em `atuais` for final.
- [ ] **Passo 6 (Menu Principal em `main.c`):**
  - Criar menu interativo no terminal chamando as funções de todos os integrantes.

---

### 2. Pedro Galvão — Conversão de AFN para AFD (`conversao_afd.c`)

- [ ] **Passo 1 (Representação de Subconjuntos):**
  - Na Construção de Subconjuntos, cada estado do novo AFD é um **subconjunto de estados do AFN** (representado por um vetor `bool subconjunto[MAX_ESTADOS]`).
  - Criar uma matriz local `subconjuntos[MAX_ESTADOS][MAX_ESTADOS]` para guardar os subconjuntos já descobertos e uma variável `qtd_subconjuntos`.
- [ ] **Passo 2 (Função Auxiliar de Busca de Subconjunto):**
  - Implementar `buscar_ou_criar_subconjunto(...)`: recebe um vetor `bool conjunto[MAX_ESTADOS]`, verifica se existe um subconjunto idêntico na matriz `subconjuntos`; se existir, retorna o índice dele; se não existir, adiciona na matriz e retorna o novo índice.
- [ ] **Passo 3 (Estado Inicial do AFD):**
  - O subconjunto 0 (estado inicial do AFD) deve conter todos os estados $q$ do AFN onde `eh_inicial[q] == true`.
- [ ] **Passo 4 (Laço Principal da Construção de Subconjuntos — Req. 2):**
  - Implementar `converter_afn_para_afd(Automato *afn, Automato *afd)`:
    1. Percorrer cada subconjunto $i$ já criado (enquanto $i < \text{qtd\_subconjuntos}$).
    2. Para cada símbolo $s$ do alfabeto, montar o conjunto `destino` unindo todas as transições `afn->transicao[q][s]` de todos os estados $q$ que pertencem ao subconjunto $i$.
    3. Se `destino` não for vazio, chamar `buscar_ou_criar_subconjunto` para obter o estado $j$ no AFD e registrar `afd->transicao[i][s][j] = true`.
- [ ] **Passo 5 (Definir Estados Finais do AFD):**
  - Para cada subconjunto $i$ do AFD, se ele contiver pelo menos um estado $q$ que era final no AFN (`afn->eh_final[q] == true`), marcar `afd->eh_final[i] = true`.

---

### 3. Bernardo — Minimização de AFD (`minimizacao.c`)

- [ ] **Passo 1 (Função Auxiliar de Destino Único do AFD):**
  - Implementar `obter_destino_afd(Automato *afd, int estado, int simbolo)` que retorna o único estado de destino (ou `-1` se não houver transição).
- [ ] **Passo 2 (Remoção de Estados Inalcançáveis):**
  - Implementar `remover_inalcancaveis(Automato *afd)`:
    1. Fazer uma busca simples a partir do estado inicial marcando `alcancavel[q] = true` para todos os estados visitados.
    2. Reindexar apenas os estados alcançáveis para que o autômato fique enxuto antes de minimizar.
- [ ] **Passo 3 (Tabela de Pares Distinguíveis — Base Myhill-Nerode):**
  - Criar matriz `bool distinguivel[MAX_ESTADOS][MAX_ESTADOS]` iniciada com `false`.
  - Marcar `distinguivel[p][q] = true` para todo par $(p, q)$ onde um é final e o outro não é final (`eh_final[p] != eh_final[q]`).
- [ ] **Passo 4 (Propagação de Distinção):**
  - Repetir em um laço `while (mudou)`:
    - Para cada par $(p, q)$ ainda não marcado (`!distinguivel[p][q]`):
      - Testar todos os símbolos $s$: descobrir $p' = \delta(p, s)$ e $q' = \delta(q, s)$.
      - Se `distinguivel[p'][q'] == true`, marcar `distinguivel[p][q] = true` e `mudou = true`.
- [ ] **Passo 5 (Fusão dos Estados Equivalentes — Req. 3):**
  - Implementar `minimizar_afd(Automato *afd, Automato *minimo)`:
    1. Estados $p$ e $q$ com `distinguivel[p][q] == false` são equivalentes e recebem o mesmo índice de grupo `grupo[p] = grupo[q]`.
    2. Construir o autômato `minimo` onde cada grupo vira um único estado, copiando as transições, estado inicial e estados finais.

---

### 4. João Paulo — Operações de Conjunto sobre AFDs (`operacoes_afd.c`)

- [ ] **Passo 1 (Totalização / Completar AFD com Estado Poço):**
  - Implementar `completar_afd(Automato *afd)`:
    1. Verificar se existe algum estado $q$ e símbolo $s$ sem transição definida.
    2. Se faltar transição, criar um novo estado poço $q_{poco}$ no final, fazer todas as transições faltantes apontarem para $q_{poco}$ e fazer $q_{poco}$ apontar para si mesmo em todos os símbolos.
- [ ] **Passo 2 (Req. 8 — Complemento de AFD):**
  - Implementar `complemento_afd(Automato *origem, Automato *destino)`:
    1. Copiar `origem` para `destino` e chamar `completar_afd(destino)`.
    2. Percorrer todos os estados $q$ invertendo `destino->eh_final[q] = !destino->eh_final[q]`.
- [ ] **Passo 3 (Função Base de Produto Cartesiano de 2 AFDs):**
  - Implementar `produto_afd(Automato *afd1, Automato *afd2, Automato *resultado, int tipo_operacao)`:
    1. Copiar e completar `afd1` e `afd2` (garantindo que tenham o mesmo alfabeto).
    2. Mapear cada par de estados $(q_1, q_2)$ para um estado único `indice = q1 * afd2->qtd_estados + q2`.
    3. Definir as transições: se $q_1 \xrightarrow{s} p_1$ em `afd1` e $q_2 \xrightarrow{s} p_2$ em `afd2`, então $(q_1, q_2) \xrightarrow{s} (p_1, p_2)$ em `resultado`.
    4. O estado inicial será o par $(inicial_1, inicial_2)$.
- [ ] **Passo 4 (Req. 5, 6 e 7 — União, Interseção e Subtração):**
  - Definir os estados finais de $(q_1, q_2)$ no produto cartesiano conforme a operação:
    - `uniao_afd` (**Req. 5**): final se `afd1->eh_final[q1] || afd2->eh_final[q2]`.
    - `intersecao_afd` (**Req. 6**): final se `afd1->eh_final[q1] && afd2->eh_final[q2]`.
    - `subtracao_afd` (**Req. 7**): final se `afd1->eh_final[q1] && !afd2->eh_final[q2]`.
