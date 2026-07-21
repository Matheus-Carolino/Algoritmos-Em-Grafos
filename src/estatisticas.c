#include "estatisticas.h"

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

    bool temCiclo(Grafo *grafo, bool direcionado) {
        if (grafo == NULL || grafo->V == 0) return false;

        int *cor = (int*) calloc(grafo->V, sizeof(int));
        if (cor == NULL)
            return false;

        for (int i = 0; i < grafo->V; i++) {
            if (cor[i] == 0) {
                if (dfsCiclo(grafo, i, cor, -1, direcionado)) {
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

    //Calcula a densidade do grafo
    float calcularDensidade(Grafo *grafo, bool direcionado) {
        if (grafo == NULL || grafo->V <= 1) return 0.0;

        if (direcionado) {
            return (float)grafo->A / (grafo->V * (grafo->V - 1));
        }
        return (float)grafo->A / ((grafo->V * (grafo->V - 1)) / 2.0);
    }

    //Calcula e imprime os graus de cada vértice baseado no tipo de grafo
    void calcularImprimirGraus(Grafo *grafo, bool direcionado) {
        if (grafo == NULL) return;

        int *grauEntrada = (int*) calloc(grafo->V, sizeof(int));
        int *grauSaida = (int*) calloc(grafo->V, sizeof(int));

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
            if (direcionado) {
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

        bool direcionado = grafo->direcionado;
        printf("- Tipo de grafo: %s\n", direcionado ? "Direcionado (Digrafo)" : "Nao-direcionado");

        calcularImprimirGraus(grafo, direcionado);

        bool conexo = isConexo(grafo);
        printf("- Conectividade: %s\n", conexo ? "CONEXO" : "DESCONEXO");

        bool ciclos = temCiclo(grafo, direcionado);
        printf("- Presenca de ciclos: %s\n", ciclos ? "SIM, contem ciclos" : "NAO possui ciclos (Aciclico)");

        float densidade = calcularDensidade(grafo, direcionado);
        printf("- Densidade do grafo: %.4f\n\n", densidade);
    }