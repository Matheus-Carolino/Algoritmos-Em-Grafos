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

        //Aloca o array de listas (indexado de 1 até V)
        grafo->lista = (No**)malloc((vertices + 1) * sizeof(No*));

        //Inicializa todas as listas como vazias
        for (int i = 0; i <= vertices; i++) {
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

