#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"

    struct No {
        int destino;
        int peso;
        struct No *prox;
    };

    struct Grafo {
        int V;      //Número de Vértices
        int A;      //Número de Arestas
        No **lista; //Lista de adjacência
    };

    //--- Funções Auxiliares ---
    //Criar um novo Nó com alocação dinâmica
    No* criarNo(int destino, int peso) {
        No *novo = (No*)malloc(sizeof(No));
        novo->destino = destino;
        novo->peso = peso;
        novo->prox = NULL;
        return novo;
    }

    //inicializa o grafo com alocação dinâmica
    Grafo* criarGrafo(int vertices, int arestas) {
        Grafo *grafo = (Grafo*)malloc(sizeof(Grafo));
        grafo->V = vertices;
        grafo->A = arestas;

        //Aloca o array de listas (indexado de 0 até V - 1)
        grafo->lista = (No**)malloc((vertices) * sizeof(No*));

        //Inicializa todas as listas como vazias
        for (int i = 0; i < vertices; i++) {
            grafo->lista[i] = NULL;
        }
        return grafo;
    }

    //--- Funções principais dos Grafos ---
    //adiciona uma aresta direcionada (origem->destino)
    void adicionarAresta(Grafo *grafo, int origem, int destino, int peso) {
        No* novo = criarNo(destino, peso);
        //implementar a verificação dps!!
        //Insere o novo nó no início da lista do vértice de origem
        novo->prox = grafo->lista[origem];
        grafo->lista[origem] = novo;
    }

    // Busca em largura (BFS)
    void BFS(Grafo *grafo, int origem) {

        // Tratamento de erro para ponteiro nulo
        if (grafo == NULL || grafo->lista == NULL) {
            printf("Erro: Grafo nao inicializado ou lista vazia.\n");
            return;
        }

        // Tratamento de erro para vértice inválido
        if (origem < 0 || origem >= grafo->V) {
            printf("Erro: Vertice de origem %d invalido para um grafo com %d vertices.\n", origem, grafo->V);
            return;
        }

        // Vetores locais
        int fila[grafo->V];
        int visitado[grafo->V];
        int distancia[grafo->V];
        int inicio = 0;
        int fim = 0;

        // Inicializa os vetores de controle
        for (int i = 0; i < grafo->V; i++) {
            visitado[i] = 0; // 0 indica que o vertice ainda não foi visitado
            distancia[i] = -1; // -1 indica que o vértice ainda é inacessível
        }

        // Inicializa a estrutura com o vértice de partida
        fila[fim++] = origem;
        visitado[origem] = 1;
        distancia[origem] = 0;

        printf("Ordem de visitacao da BFS: ");

        while (inicio < fim) {
            // Desenfileira o vértice mais antigo da fila
            int vert = fila[inicio++];
            printf("%d ", vert);

            // Percorre todos os vizinhos na lista de adjacência do vértice atual
            No *aux = grafo->lista[vert];
            while (aux) {
                // Se o vizinho ainda não foi visitado, processa e enfileira
                if (!visitado[aux->destino]) {
                    visitado[aux->destino] = 1;
                    distancia[aux->destino] = distancia[vert] + 1;
                    fila[fim++] = aux->destino;
                }
                aux = aux->prox;
            }
        }

        // Mostra o resultado das distâncias a partir do vértice de origem
        printf("\n\nDistancias a partir do vertice %d:\n", origem);
        for (int i = 0; i < grafo->V; i++) {
            if (distancia[i] != -1) {
                printf("Vertice %d: distancia = %d\n", i, distancia[i]);
            } else {
                printf("Vertice %d: inacessivel\n", i);
            }
        }
    }