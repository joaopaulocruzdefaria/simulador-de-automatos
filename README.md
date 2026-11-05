# Simulador de Autômatos Finitos (LFA)

Trabalho prático da disciplina de **Linguagens Formais e Autômatos (LFA)** implementado em **C puro (C99)**.

## Regras do Projeto
- **100% em C puro** (`<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<stdbool.h>`).
- **100% em Português (PT-BR)** (nomes de structs, funções, variáveis e arquivos).
- **Foco Didático:** código simples, direto, usando matrizes estáticas e sem comentários desnecessários, pensado para que cada integrante consiga explicar linha a linha para o professor.

---

## Divisão da Equipe e Checklist de Desenvolvimento

Consulte [`REQUISITOS.md`](REQUISITOS.md) para o passo a passo detalhado de cada algoritmo e [`AGENTS.md`](AGENTS.md) para os padrões de código.

### 1. Otávio — Base + Req. 1 e Req. 4 (`automato.h`, `automato.c`, `main.c`)
- [ ] Definir `struct Automato` em `automato.h`
- [ ] Implementar leitura e impressão de autômatos no terminal
- [ ] **Req. 1 (Ida):** Converter AFN com múltiplos estados iniciais para AFN-$\lambda$
- [ ] **Req. 1 (Volta):** Converter AFN-$\lambda$ para AFN (usando fecho-$\lambda$)
- [ ] **Req. 4:** Implementar teste de aceitação de palavras
- [ ] Criar menu principal integrando os módulos em `main.c`

### 2. Pedro Galvão — Req. 2 (`conversao_afd.c`)
- [ ] Representar subconjuntos de estados e função de busca/criação de subconjunto
- [ ] **Req. 2:** Implementar conversão de AFN para AFD (Construção de Subconjuntos)
- [ ] Mapear estados iniciais e finais dos subconjuntos gerados

### 3. Bernardo — Req. 3 (`minimizacao.c`)
- [ ] Remover estados inalcançáveis a partir do estado inicial
- [ ] Construir e propagar a tabela de pares distinguíveis (Myhill-Nerode)
- [ ] **Req. 3:** Fundir estados equivalentes gerando o AFD mínimo

### 4. João Paulo — Req. 5, 6, 7 e 8 (`operacoes_afd.c`)
- [ ] Implementar `completar_afd` (estado poço/erro para transições indefinidas)
- [ ] **Req. 8:** Implementar Complemento de 1 AFD
- [ ] Implementar Produto Cartesiano de 2 AFDs
- [ ] **Req. 5:** Implementar União de 2 AFDs ($L_1 \cup L_2$)
- [ ] **Req. 6:** Implementar Interseção de 2 AFDs ($L_1 \cap L_2$)
- [ ] **Req. 7:** Implementar Subtração de 2 AFDs ($L_1 - L_2$)
