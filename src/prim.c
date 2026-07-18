#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// --- ESTRUTURAS DE DADOS (Conforme especificado no TP) ---
typedef struct No {
    int destino;
    int peso;
    struct No* prox;
} No;

typedef struct {
    int V;       // Número de vértices
    int A;       // Número de arestas
    No** lista;  // Lista de adjacência
} Grafo;

/**
 * Algoritmo de Prim para encontrar a Árvore Geradora Mínima (AGM).
 * Trata grafos desconexos encontrando a floresta geradora mínima.
 */
void algoritmoPrim(Grafo* g) {
    if (g == NULL || g->V == 0) {
        printf("Erro: Grafo vazio ou nao inicializado.\n");
        return;
    }

    int V = g->V;
    int* pai = (int*)malloc(V * sizeof(int));
    int* chave = (int*)malloc(V * sizeof(int));
    bool* naAGM = (bool*)malloc(V * sizeof(bool));

    // Inicialização das estruturas auxiliares
    for (int i = 0; i < V; i++) {
        chave[i] = INT_MAX; // Inicializa com "infinito"
        naAGM[i] = false;   // Nenhum vértice está na AGM ainda
        pai[i] = -1;        // Nenhum pai definido
    }

    int peso_total_componentes = 0;

    printf("\n=== ÁRVORE GERADORA MÍNIMA (PRIM) ===\n");

    // Loop externo: garante o tratamento de grafos desconexos
    for (int raiz = 0; raiz < V; raiz++) {
        // Se o vértice já foi incluído em uma árvore de um componente anterior, pula
        if (naAGM[raiz]) continue;

        // O vértice que inicia o componente atual ganha peso 0 para ser o primeiro escolhido
        chave[raiz] = 0;

        // Executa o algoritmo para o componente conexo atual
        for (int count = 0; count < V; count++) {
            int u = -1;
            int min = INT_MAX;

            // 1. Busca linear pelo vértice com a menor chave que ainda não está na AGM
            for (int v = 0; v < V; v++) {
                if (!naAGM[v] && chave[v] < min) {
                    min = chave[v];
                    u = v;
                }
            }

            // Se u continuar -1, significa que os vértices restantes pertencem a outro componente isolado
            if (u == -1) break;

            naAGM[u] = true;
            
            // Se o nó possui um pai válido, ele faz parte de uma aresta da AGM
            if (pai[u] != -1) {
                peso_total_componentes += chave[u];
                printf("Aresta: %d - %d | Peso: %d\n", pai[u], u, chave[u]);
            }

            // 2. Atualiza as chaves e os pais dos vértices adjacentes de u
            No* adj = g->lista[u];
            while (adj != NULL) {
                int v = adj->destino;
                int peso = adj->peso;

                // Se v não está na AGM e o peso da aresta (u, v) é menor que a chave atual de v
                if (!naAGM[v] && peso < chave[v]) {
                    pai[v] = u;
                    chave[v] = peso;
                }
                adj = adj->prox;
            }
        }
    }

    printf("-------------------------------------\n");
    printf("Peso Total da AGM: %d\n", peso_total_componentes); // Exibe o peso total exigido [cite: 42, 104]
    printf("=====================================\n");

    // Liberação cuidadosa da memória local alocada
    free(pai);
    free(chave);
    free(naAGM);
}