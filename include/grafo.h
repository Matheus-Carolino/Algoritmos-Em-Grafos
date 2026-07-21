#ifndef GRAFOS_GRAFO_H
#define GRAFOS_GRAFO_H
    #include <stdbool.h>
    #include "validacao.h"

    //typedef das structs usadas
    typedef struct No No;

    typedef struct Grafo {
        int V;      //Número de Vértices
        int A;      //Número de Arestas
        int direcionado; // (1 - Direcionado/ 0 - Não-direcionado)
        No **lista; //Lista de adjacência
    } Grafo;

    //Assinaturas das funções auxiliares
    No* criarNo(int destino, int peso);
    Grafo* criarGrafo(int vertices, int arestas, int direcionado);
    void liberarGrafo(Grafo *grafo);
    void mostrarGrafo(Grafo *grafo);
    void inserirOrdenado(No** lista, No* novo);

    //Assinatura das funções principais dos grafos
    void adicionarArestaDirecionada(Grafo *grafo, int origem, int destino, int peso);
    void adicionarArestaBidirecionada(Grafo *grafo, int origem, int destino, int peso);
    GrafoStatus lerGrafoDeArquivo(const char* nomeArquivo, Grafo** grafo, int direcionado);

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

    // --- Funções para Estatisticas dos Grafos ---
    //Verifica se o grafo é direcionado
    bool isDirecionado(Grafo *grafo);

    //Auxiliares e função para detectar ciclos por DFS (cores)
    bool dfsCiclo(Grafo *grafo, int i, int *cor, int pai, bool direcionado);
    bool temCiclo(Grafo *grafo, bool direcionado);

    //Auxiliares e função principal para verificar se o grafo é conexo
    void dfsConexo(Grafo *grafo, int i, bool *visitado);
    bool isConexo(Grafo *grafo);

    //Calcula a densidade do grafo
    float calcularDensidade(Grafo *grafo, bool direcionado);

    //Calcula e imprime os graus de cada vértice baseado no tipo de grafo
    void calcularImprimirGraus(Grafo *grafo, bool direcionado);

    //Executor de tudo que foi pedido da opção 8
    void exibirEstatisticas(Grafo *grafo);

    //funções extras
    void DFSPilha (Grafo *grafo, int indice, int *visitados, int *pilha, int *sp);
    Grafo* criarGrafoTransposto(Grafo *grafo);
    GrafoStatus Kosaraju (Grafo *grafo);
    GrafoStatus caminhoCritico (Grafo *grafo); 

#endif //GRAFOS_GRAFO_H
