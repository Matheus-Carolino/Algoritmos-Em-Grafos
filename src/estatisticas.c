
#include <stdbool.h>
#include "estatisticas.h"
#include "algoritmos.h"

    // --- Funções para Estatisticas dos Grafos ---
    //Auxiliares e função para detectar ciclos por DFS (cores)
    bool dfsCiclo(Grafo *grafo, int i, int *cor, int pai, bool direcionado) {
        cor[i] = 1;
        No *atual = grafo->lista[i];

        while (atual != NULL) {
            int v = atual->destino;
            if (cor[v] == 1) {
                if (!direcionado && v == pai) {
                    atual = atual->prox;
                    continue;
                }
                return true; // Encontrou uma aresta de volta para um nó em processamento
            }

            if (cor[v] == 0 && dfsCiclo(grafo, v, cor, i, direcionado)) {
                return true;
            }
            atual = atual->prox;
        }
        cor[i] = 2; // 2 = Totalmente processado (Preto)
        return false;
    }

    bool temCiclo(Grafo *grafo) {
        if (grafo == NULL || grafo->V == 0) return false;

        int *cor = (int*) calloc(grafo->V, sizeof(int));
        if (cor == NULL)
            return false;

        for (int i = 0; i < grafo->V; i++) {
            if (cor[i] == 0) {
                if (dfsCiclo(grafo, i, cor, -1, grafo->direcionado)) {
                    free(cor);
                    return true;
                }
            }
        }
        free(cor);
        return false;
    }

    //Auxiliares e função principal para verificar se o grafo é conexo
    void dfsConexo(Grafo *grafo, int i, bool *visitado) {
        visitado[i] = true;
        No *atual = grafo->lista[i];
        while (atual != NULL) {
            if (!visitado[atual->destino]) {
                dfsConexo(grafo, atual->destino, visitado);
            }
            atual = atual->prox;
        }
    }

    bool isConexo(Grafo *grafo) {
        if (grafo == NULL || grafo->V == 0) return false;

        bool *visitado = (bool*) calloc(grafo->V, sizeof(bool));
        if (visitado == NULL)
            return false;
        // Dispara a busca a partir do primeiro vértice (0)
        dfsConexo(grafo, 0, visitado);

        // Se algum vértice não foi alcançado, o grafo é desconexo
        for (int i = 0; i < grafo->V; i++) {
            if (!visitado[i]) {
                free(visitado);
                return false;
            }
        }
        free(visitado);
        return true;
    }

    bool ehFortementeConexo(Grafo *grafo) {
        if (grafo == NULL || grafo->V == 0)
            return false;

        int V = grafo->V;
        bool *visitado = (bool*) calloc(V, sizeof(bool));
        if (visitado == NULL)
            return false;

        // DFS a partir do vértice 0 no grafo original
        dfsConexo(grafo, 0, visitado);
        for (int i = 0; i < V; i++) {
            if (!visitado[i]) {
                free(visitado);
                return false; // já sabemos que não é fortemente conexo
            }
        }

        // DFS a partir do vértice 0 no grafo TRANSPOSTO
        Grafo *transposto = criarGrafoTransposto(grafo);
        if (transposto == NULL) {
            free(visitado);
            return false;
        }

        for (int i = 0; i < V; i++)
            visitado[i] = 0;

        dfsConexo(transposto, 0, visitado);

        bool conexo = true;
        for (int i = 0; i < V; i++) {
            if (!visitado[i]) {
                conexo = false;
                break;
            }
        }

        free(visitado);
        liberarGrafo(transposto);
        return conexo;
    }

    //Calcula a densidade do grafo
    float calcularDensidade(Grafo *grafo) {
        if (grafo == NULL || grafo->V <= 1) return 0.0f;

        float v = (float)grafo->V;

        if (grafo->direcionado) {
            return (float)grafo->A / (v * (v - 1.0f));
        }

        return (float)grafo->A / ((v * (v - 1.0f)) / 2.0f);
    }

    //Calcula e imprime os graus de cada vértice baseado no tipo de grafo
    void calcularImprimirGraus(Grafo *grafo) {
        if (grafo == NULL) return;

        int *grauEntrada = (int*) calloc(grafo->V, sizeof(int));
        int *grauSaida = (int*) calloc(grafo->V, sizeof(int));

        if (grauEntrada == NULL || grauSaida == NULL) {
            free(grauEntrada);
            free(grauSaida);
            return;
        }

        for (int u = 0; u < grafo->V; u++) {
            No *atual = grafo->lista[u];
            while (atual != NULL) {
                grauSaida[u]++;
                grauEntrada[atual->destino]++;
                atual = atual->prox;
            }
        }

        printf("- Grau de cada vertice:\n");
        for (int i = 0; i < grafo->V; i++) {
            if (grafo->direcionado) {
                printf("  Vertice %d: Grau de Entrada = %d | Grau de Saida = %d\n", i, grauEntrada[i], grauSaida[i]);
            } else {
                printf("  Vertice %d: Grau = %d\n", i, grauSaida[i]);
            }
        }
        free(grauEntrada);
        free(grauSaida);
    }

    //Executor de tudo que foi pedido da opção 8
    void exibirEstatisticas(Grafo *grafo) {
        if (grafo == NULL) {
            printf("Erro: Nenhum grafo carregado na memoria.\n");
            return;
        }

        printf("===  ESTATISTICAS DO GRAFO ===\n\n");

        printf("- Numero de vertices: %d\n", grafo->V);
        printf("- Numero de arestas: %d\n", grafo->A);

        printf("- Tipo de grafo: %s\n", grafo->direcionado ? "Direcionado (Digrafo)" : "Nao-direcionado");

        calcularImprimirGraus(grafo);

        if (grafo->direcionado) {
            printf("- Fortemente conexo: %s\n", ehFortementeConexo(grafo) ? "SIM" : "NAO");
        }
        else {
            printf("- Conexo: %s\n", isConexo(grafo) ? "SIM" : "NAO");
        }

        bool ciclos = temCiclo(grafo);
        printf("- Presenca de ciclos: %s\n", ciclos ? "SIM, contem ciclos" : "NAO possui ciclos (Aciclico)");

        float densidade = calcularDensidade(grafo);
        printf("- Densidade do grafo: %.4f\n\n", densidade);
    }