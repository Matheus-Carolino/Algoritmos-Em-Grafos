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

    //Assinatura das funções de busca em profundidade (DFS)
    GrafoStatus DFSRecursiva(Grafo* grafo, int origem, int *visitado);
    GrafoStatus DFSRecCompConexos(Grafo* grafo);
    GrafoStatus DFSIterativa(Grafo* grafo, int origem, int* visitado);
    GrafoStatus DFSIteCompConexos(Grafo* grafo);

    //Assinatura da função de busca em largura (BFS)
    GrafoStatus BFS(Grafo *grafo, int vertice);

    //Assinatura da função de ordem topológica
    GrafoStatus ordemTopologica(Grafo *grafo);

#endif //GRAFOS_GRAFO_H
