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

    // --- Funções de busca ---
    // Realiza uma busca em profundidade (DFS) de forma recursiva
    void DFSRecursiva(Grafo* grafo, int origem, int *visitado) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return;

        // Marca o vértice atual como visitado e o imprime
        visitado[origem] = 1;
        printf("%d ", origem);

        // Percorre todos os vértices adjacentes
        No* atual = grafo->lista[origem];
        while (atual != NULL) {
            // Visita recursivamente os vértices não visitados
            if (!visitado[atual->destino]) {
                printf("-> ");
                DFSRecursiva(grafo, atual->destino, visitado);
            }
            atual = atual->prox;
        }
    }

    // Realiza uma busca em profundidade (DFS) de forma iterativa
    void DFSIterativa(Grafo* grafo, int origem, int* visitado) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return;

        // Inicializa a pilha auxiliar
        int pilha[grafo->V];
        int topo = -1;

        // Empilha o vértice de origem
        pilha[++topo] = origem;
        visitado[origem] = 1;

        printf("%d ", origem);

        // Enquanto houver vértices na pilha
        while (topo != -1) {
            // Faça destino o vértice no topo da pilha
            int destino = pilha[topo];

            No* atual = grafo->lista[destino];
            int encontrou = 0;

            // Procura o primeiro vizinho ainda não visitado
            while (atual != NULL) {
                if (!visitado[atual->destino]) {
                    visitado[atual->destino] = 1;
                    pilha[++topo] = atual->destino;

                    printf("-> %d ", atual->destino);

                    encontrou = 1;
                    // Quando encontrar o primeiro vizinho, encerra o laço
                    break;
                }

                atual = atual->prox;
            }

            // Se não encontrou vizinhos não visitados
            if (!encontrou)
                topo--;
        }
    }

    // Identifica os componentes conexos do grafo utilizando DFS recursiva
    // Obs.: Funciona apenas para grafos não direcionados
    void DFSRecCompConexos(Grafo* grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return;

        // Inicializa o vetor para controlar os vértices visitados
        int visitado[grafo->V];
        for (int i = 0; i < grafo->V; i++)
            visitado[i] = 0;


        // Contador para numerar os componentes conexos
        int contador = 1;

        printf("=== DFS RECURSIVA ===\n");

        // Percorre todos os vértices do grafo
        for (int i = 0; i < grafo->V; i++) {
            // Inicia uma nova DFS caso o vértice ainda não tenha sido visitado
            if (!visitado[i] && grafo->lista[i] != NULL) {
                printf("Componente %d: ", contador++);
                DFSRecursiva(grafo, i, visitado);
                printf("\n");
            }
        }
    }

    // Identifica os componentes conexos do grafo utilizando DFS iterativa
    // Obs.: Funciona apenas para grafos não direcionados
    void DFSIteCompConexos(Grafo* grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return;

        // Inicializa o vetor para controlar dos vértices visitados
        int visitado[grafo->V];
        for (int i = 0; i < grafo->V; i++)
            visitado[i] = 0;

        // Contador para numerar os componentes conexos
        int contador = 1;

        printf("=== DFS ITERATIVA ===\n");

        // Percorre todos os vértices do grafo
        for (int i = 0; i < grafo->V; i++) {
            // Inicia uma nova DFS caso o vértice ainda não tenha sido visitado
            if (!visitado[i] && grafo->lista[i] != NULL) {
                printf("Componente %d: ", contador++);
                DFSIterativa(grafo, i, visitado);
                printf("\n");
            }
        }
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
                    if (aux->destino >= 0 && aux->destino < grafo->V) {
                        visitado[aux->destino] = 1;
                        distancia[aux->destino] = distancia[vert] + 1;
                        fila[fim++] = aux->destino;
                    }
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
