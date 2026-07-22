#ifndef GRAFOS_BUSCAS_H
#define GRAFOS_BUSCAS_H

    #include "grafo.h"

    //Assinatura das funções de busca em profundidade (DFS)
    GrafoStatus DFSRecursiva(Grafo* grafo, int origem, int *visitado);
    GrafoStatus DFSRecCompConexos(Grafo* grafo);
    GrafoStatus DFSIterativa(Grafo* grafo, int origem, int* visitado);
    GrafoStatus DFSIteCompConexos(Grafo* grafo);

    //Assinatura da função de busca em largura (BFS)
    GrafoStatus BFS(Grafo *grafo, int vertice);

    //Assinatura da função de ordem topológica
    GrafoStatus ordemTopologica(Grafo *grafo);

    //Assinatura dos algoritmos de Prim e Dijkstra
    GrafoStatus algoritmoPrim(Grafo* g);
    GrafoStatus algoritmoDijkstra(Grafo *grafo, int origem);

    //funções extras
    GrafoStatus algoritmoKosaraju (Grafo *grafo);
    GrafoStatus caminhoCritico (Grafo *grafo);

#endif //GRAFOS_BUSCAS_H
