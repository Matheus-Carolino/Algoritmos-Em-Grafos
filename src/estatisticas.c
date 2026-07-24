
#include "estatisticas.h"
#include "algoritmos.h"

    // --- Funções para Estatisticas dos Grafos ---

    // Auxiliares e função para detectar ciclos por DFS (cores)
    // Branco (0): Vértice ainda não foi processado
    // Cinza (1): Vértice sendo processado
    // Preto (2): Vértice foi completamente processado, todos os seus descendentes foram processados
    bool dfsCiclo(Grafo *grafo, int i, int *cor, int pai, bool direcionado) {
        // Marca o vértice em processamento
        cor[i] = 1;
        No *atual = grafo->lista[i];

        // Percorre os vizinhos do vértice atual
        while (atual != NULL) {
            int v = atual->destino;
            if (cor[v] == 1) {
                if (!direcionado && v == pai) {
                    atual = atual->prox;
                    continue;
                }
                // Encontrou uma aresta de volta para um nó em processamento
                // Em grafos direcionados ciclo
                return true;
            }

            // Se o vizinho ainda não foi visitado
            if (cor[v] == 0 && dfsCiclo(grafo, v, cor, i, direcionado)) {
                return true;
            }
            atual = atual->prox;
        }
        cor[i] = 2; // 2 = Totalmente processado (Preto)
        return false;
    }

    // Verificar se o grafo possui pelo menos um ciclo
    bool temCiclo(Grafo *grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL || grafo->V == 0) return false;

        // Inicia vetor das cores (inicialmente todos brancos [0] por causa de calloc)
        int *cor = (int*) calloc(grafo->V, sizeof(int));
        if (cor == NULL)
            return false;

        // Percorre todos os vértices
        for (int i = 0; i < grafo->V; i++) {
            // Se não foi visitado (Branco)
            if (cor[i] == 0) {
                // Faz uma dfs para conferir se existe um ciclo nessa componente
                if (dfsCiclo(grafo, i, cor, -1, grafo->direcionado)) {
                    free(cor);
                    // Se existe ao menos um ciclo nesse grafo, retorne verdadeiro
                    return true;
                }
            }
        }
        free(cor);
        // Se nenhum componente possui ciclo, retorne falso
        return false;
    }

    //Auxiliares e função principal para verificar se o grafo é conexo

    // Realiza um DFS marcando como visitados todos os vértices alcançáveis através de i
    void dfsConexo(Grafo *grafo, int i, bool *visitado) {
        // Marca o vértice atual como visitado
        visitado[i] = true;
        No *atual = grafo->lista[i];
        while (atual != NULL) {
            // Visita todos os vértices vizinhos não visitados
            if (!visitado[atual->destino]) {
                dfsConexo(grafo, atual->destino, visitado);
            }
            atual = atual->prox;
        }
    }

    // Verifica se um grafo **não direcionado** é conexo.
    bool isConexo(Grafo *grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL || grafo->V == 0) return false;

        // Inicia o vetor de controle visitado com todas as posições iguais a 0, em razão do calloc
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

    // Função para verificar se um grafo é fortemente conexo
    // Obs.: utiliza o Algoritmo de Kosaraju
    bool isFortementeConexo(Grafo *grafo) {
        if (grafo == NULL || grafo->V == 0)
            return false;

        int V = grafo->V;

        // Inicia o vetor de controle com todas as posições iguais a zero
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

        // DFS a partir do vértice 0 no grafo transposto
        Grafo *transposto = criarGrafoTransposto(grafo);
        if (transposto == NULL) {
            free(visitado);
            return false;
        }

        for (int i = 0; i < V; i++)
            visitado[i] = 0;

        dfsConexo(transposto, 0, visitado);

        // Se ainda existem vértices não visitados, então o grafo não é fortemente conexo
        for (int i = 0; i < V; i++) {
            if (!visitado[i]) {
                free(visitado);
                liberarGrafo(transposto);
                return false;
            }
        }

        free(visitado);
        liberarGrafo(transposto);
        // Se todos os vértices foram visitados, o grafo é fortemente conexo
        return true;
    }

    // Calcula a densidade do grafo
    // A densidade de um grafo mede o quão "próximo" ele está de ser um grafo completo (quantidade máxima de arestas)
    float calcularDensidade(Grafo *grafo) {
        if (grafo == NULL || grafo->V <= 1) return 0.0f;

        float v = (float)grafo->V;

        if (grafo->direcionado) {
            return (float)grafo->A / (v * (v - 1.0f));
        }

        return (float)grafo->A / ((v * (v - 1.0f)) / 2.0f);
    }

    // Calcula e imprime os graus de cada vértice baseado no tipo de grafo
    void calcularImprimirGraus(Grafo *grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL) return;

        // Inicia os vetores para guardar os graus (entrada e saída) de cada vértices
        int *grauEntrada = (int*) calloc(grafo->V, sizeof(int));
        int *grauSaida = (int*) calloc(grafo->V, sizeof(int));

        // Libera a memória se houve algum erro na alocação
        if (grauEntrada == NULL || grauSaida == NULL) {
            free(grauEntrada);
            free(grauSaida);
            return;
        }

        // Calcula os graus de entrada
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
                // Se o grafo é direcionado (possui tanto grau de entrada como grau de saída)
                printf("  Vertice %d: Grau de Entrada = %d | Grau de Saida = %d\n", i, grauEntrada[i], grauSaida[i]);
            } else {
                // Se o grafo é não direcionado, exibe apenas o grau do vértice
                printf("  Vertice %d: Grau = %d\n", i, grauSaida[i]);
            }
        }
        free(grauEntrada);
        free(grauSaida);
    }

    // Executor de todas as estatísticas de grafo
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
            printf("- Fortemente conexo: %s\n", isFortementeConexo(grafo) ? "SIM" : "NAO");
        }
        else {
            printf("- Conexo: %s\n", isConexo(grafo) ? "SIM" : "NAO");
        }

        bool ciclos = temCiclo(grafo);
        printf("- Presenca de ciclos: %s\n", ciclos ? "SIM, contem ciclos" : "NAO possui ciclos (Aciclico)");

        float densidade = calcularDensidade(grafo);
        printf("- Densidade do grafo: %.4f\n\n", densidade);
    }
