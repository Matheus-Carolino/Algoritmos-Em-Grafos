#ifndef GRAFOS_BUSCAS_H
#define GRAFOS_BUSCAS_H

    #include "grafo.h"

    //Assinatura das funções de busca em profundidade (DFS)
    GrafoStatus DFSRecursiva(Grafo* grafo, int origem, int *visitado);
    GrafoStatus DFSIterativa(Grafo* grafo, int origem, int* visitado);
    GrafoStatus DFSCompConexos(Grafo* grafo, int recursiva);

    //Assinatura da função de busca em largura (BFS)
    GrafoStatus BFS(Grafo *grafo, int vertice);

    //Assinatura da função de ordem topológica
    GrafoStatus ordemTopologica(Grafo *grafo);

    //Assinatura dos algoritmos de Prim e Dijkstra
    GrafoStatus algoritmoPrim(Grafo* g);
    GrafoStatus algoritmoDijkstra(Grafo *grafo, int origem);

    //funções extras
    GrafoStatus caminhoCritico (Grafo *grafo);
    Grafo* criarGrafoTransposto(Grafo *grafo);
    GrafoStatus algoritmoKosaraju (Grafo *grafo);

#endif //GRAFOS_BUSCAS_H
