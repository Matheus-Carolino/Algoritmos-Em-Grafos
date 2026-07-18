#ifndef GRAFOS_GRAFO_H
#define GRAFOS_GRAFO_H
    //typedef das structs usadas
    typedef struct No No;
    typedef struct Grafo Grafo;

    //Assinaturas das funções auxiliares
    No* criarNo(int destino, int peso);
    Grafo* criarGrafo(int vertices, int arestas);

    //Assinatura das funções principais dos grafos
    void adicionarAresta(Grafo *grafo, int origem, int destino, int peso);
    void ordemTopologica (Grafo *grafo);

#endif //GRAFOS_GRAFO_H
