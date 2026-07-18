#ifndef GRAFOS_GRAFO_H
#define GRAFOS_GRAFO_H

    #include "validacao.h"

    //typedef das structs usadas
    typedef struct No No;
    typedef struct Grafo Grafo;

    //Assinaturas das funções auxiliares
    No* criarNo(int destino, int peso);
    Grafo* criarGrafo(int vertices, int arestas);
    void liberarGrafo(Grafo *grafo);
    void inserirOrdenado(No** lista, No* novo);

    //Assinatura das funções principais dos grafos
    void adicionarArestaDirecionada(Grafo *grafo, int origem, int destino, int peso);
    void adicionarArestaBidirecionada(Grafo *grafo, int origem, int destino, int peso);
    GrafoStatus lerGrafoDeArquivo(const char* nomeArquivo, Grafo** grafo);

    void DFSRecursiva(Grafo* grafo, int origem, int *visitado);
    void DFSRecCompConexos(Grafo* grafo);
    void DFSIterativa(Grafo* grafo, int origem, int* visitado);
    void DFSIteCompConexos(Grafo* grafo);
    void BFS(Grafo *grafo, int vertice);

    void ordemTopologica (Grafo *grafo)

#endif //GRAFOS_GRAFO_H
