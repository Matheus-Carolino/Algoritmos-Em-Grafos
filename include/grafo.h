#ifndef GRAFOS_GRAFO_H
#define GRAFOS_GRAFO_H
    #include <stdio.h>
    #include <stdlib.h>
    #include "validacao.h"

    //Estruturas do gráfico
    typedef struct No {
        int destino;
        int peso;
        struct No *prox;
    } No;

    typedef struct Grafo {
        int V;      //Número de Vértices
        int A;      //Número de Arestas
        int direcionado; // (1 - Direcionado/ 0 - Não-direcionado)
        No **lista; //Lista de adjacência
    } Grafo;

    //Assinatura das funções principais dos grafos
    Grafo* criarGrafo(int vertices, int arestas, int direcionado);
    void liberarGrafo(Grafo *grafo);
    void mostrarGrafo(Grafo *grafo);
    void adicionarAresta(Grafo *grafo, int origem, int destino, int peso);
    GrafoStatus lerGrafoDeArquivo(const char* nomeArquivo, Grafo** grafo, int direcionado);

#endif //GRAFOS_GRAFO_H
