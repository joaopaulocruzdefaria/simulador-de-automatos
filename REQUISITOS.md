# Especificação de Requisitos — Simulador de Autômatos (LFA)

## 1. Visão Geral
Simulador de Autômatos Finitos desenvolvido para a disciplina de Linguagens Formais e Autômatos (LFA). O objetivo do projeto é implementar representações e operações clássicas sobre Autômatos Finitos Determinísticos (AFD), Não-Determinísticos (AFN) e com movimentos vazios (AFN-$\lambda$), prezando pela clareza algorítmica e facilidade de explicação em arguição oral.

## 2. Requisitos Funcionais

| ID | Requisito | Entrada | Saída | Descrição |
| :--- | :--- | :--- | :--- | :--- |
| **RF01** | Conversão AFN (múltiplos estados iniciais) $\leftrightarrow$ AFN-$\lambda$ | AFN com múltiplos estados iniciais / AFN-$\lambda$ | AFN-$\lambda$ com estado inicial único / AFN equivalente sem transições $\lambda$ | Ida: unificar múltiplos estados iniciais criando um novo estado inicial com transições $\lambda$. Volta: calcular o fecho-$\lambda$ ($\lambda$-closure) e remover transições vazias. |
| **RF02** | Conversão de AFN para AFD | AFN | AFD equivalente | Algoritmo de construção de subconjuntos (*subset construction*). |
| **RF03** | Minimização de AFD | AFD | AFD mínimo equivalente | Remoção de estados inalcançáveis/inúteis e unificação de estados equivalentes (ex: algoritmo de preenchimento de tabela / Myhill-Nerode). |
| **RF04** | Teste de Palavras | Autômato e cadeia de símbolos | Aceita / Rejeita | Simulação da leitura de uma palavra passo a passo sobre o autômato. |
| **RF05** | União de Autômatos | 2 AFDs ($M_1, M_2$) | 1 AFD ($M_1 \cup M_2$) | Construção do autômato produto aceitando $L(M_1) \cup L(M_2)$. |
| **RF06** | Interseção de Autômatos | 2 AFDs ($M_1, M_2$) | 1 AFD ($M_1 \cap M_2$) | Construção do autômato produto aceitando $L(M_1) \cap L(M_2)$. |
| **RF07** | Subtração (Diferença) de Autômatos | 2 AFDs ($M_1, M_2$) | 1 AFD ($M_1 - M_2$) | Construção do autômato produto (ou $M_1 \cap \overline{M_2}$) aceitando palavras em $L(M_1)$ que não estão em $L(M_2)$. |
| **RF08** | Complemento de Autômato | 1 AFD ($M_1$) | 1 AFD ($\overline{M_1}$) | Totalização da função de transição (estado de erro/poço, se necessário) e inversão dos estados finais e não-finais. |

## 3. Requisitos Não-Funcionais e Restrições

- **RNF01 — Linguagem:** Todo o projeto deve ser escrito estritamente em **C puro** (padrão C99/C11, utilizando apenas a biblioteca padrão).
- **RNF02 — Idioma:** Todo o código-fonte (identificadores, variáveis, funções, structs, mensagens de interface e documentação) deve estar em **Português (PT-BR)**.
- **RNF03 — Simplicidade e Didática:** A implementação deve ser a mais simples e direta possível (uso de matrizes/vetores de tamanho fixo via `define`, evitando alocação dinâmica complexa ou estruturas de dados rebuscadas), pois o código será defendido e explicado ao professor.
- **RNF04 — Código Limpo de Comentários:** Manter **pouco ou nenhum comentário** ao longo do código. O próprio nome das variáveis e funções em português deve tornar o código autoexplicativo.

## 4. Divisão da Equipe

O projeto é dividido entre 4 integrantes:
1. **Otávio**
2. **Pedro Galvão**
3. **Bernardo**
4. **João Paulo**

### Proposta de Divisão de Tarefas

Para que todos consigam trabalhar em paralelo e tenham cargas de complexidade equivalentes para explicar ao professor, todos partem de uma mesma `struct Automato` base:

- **Integrante 1 — Otávio: Base, Teste de Palavras e Conversão AFN Múltiplos Iniciais $\leftrightarrow$ AFN-$\lambda$ (RF01 + RF04)**
  - Definição da `struct Automato`, leitura/impressão do autômato e menu principal.
  - Simulação e teste de aceitação de palavras (RF04).
  - Unificação de múltiplos estados iniciais usando transição $\lambda$ e remoção de transições $\lambda$ via fecho-$\lambda$ (RF01).

- **Integrante 2 — Pedro Galvão: Determinização — Conversão de AFN para AFD (RF02)**
  - Implementação da construção de subconjuntos (manipulação de conjuntos de estados como máscaras de bits ou matriz de subconjuntos).
  - Mapeamento dos novos estados compostos para estados simples do AFD resultante.

- **Integrante 3 — Bernardo: Minimização de AFD (RF03)**
  - Remoção de estados inalcançáveis (busca simples a partir do estado inicial).
  - Algoritmo de tabela de equivalência (marcar pares distinguíveis até convergir) e fusão dos estados equivalentes.

- **Integrante 4 — João Paulo: Operações de Conjunto sobre AFDs (RF05, RF06, RF07, RF08)**
  - Função para garantir que o AFD seja total (criação do estado poço/erro quando houver transição indefinida).
  - Complemento de AFD (RF08): inverter estados finais.
  - Produto cartesiano de dois AFDs $(q_i, q_j)$ parametrizado para resolver União (RF05), Interseção (RF06) e Subtração/Diferença (RF07) mudando apenas a condição de estado final.
