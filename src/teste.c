#include "teste.h"

    void bateriaDeTestes(Grafo* grafo) {
        struct timespec inicio, fim;
        double tempoOT, tempoMC, tempoDFSIterativo, tempoDFSRecursivo, tempoBFS, tempoAGM, tempoKosaraju, tempoCC;

        int* visitados = (int*) calloc (grafo->V, grafo->V);

        printf("=== Executando bateria de testes ===\n");

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        ordemTopologica(grafo);
        clock_gettime(CLOCK_MONOTONIC, &fim);
        tempoOT = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec -inicio.tv_nsec)/1e9;

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        algoritmoDijkstra(grafo, 0);
        clock_gettime(CLOCK_MONOTONIC, &fim);
        tempoMC = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec -inicio.tv_nsec)/1e9;

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        printf("\n=== DFS Recursiva ===\n");
        DFSRecursiva(grafo, 0, visitados);
        printf("\n");
        clock_gettime(CLOCK_MONOTONIC, &fim);
        tempoDFSRecursivo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec -inicio.tv_nsec)/1e9;

        for (int i = 0; i < grafo->V; i++) {
            visitados[i] = 0;
        }

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        printf("\n=== DFS Iterativa ===\n");
        DFSIterativa(grafo, 0, visitados);
        printf("\n");
        clock_gettime(CLOCK_MONOTONIC, &fim);
        tempoDFSIterativo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec -inicio.tv_nsec)/1e9;

        clock_gettime(CLOCK_MONOTONIC, &inicio);;
        BFS(grafo,0);
        clock_gettime(CLOCK_MONOTONIC, &fim);
        tempoBFS = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec -inicio.tv_nsec)/1e9;

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        algoritmoPrim(grafo);
        clock_gettime(CLOCK_MONOTONIC, &fim);
        tempoAGM = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec -inicio.tv_nsec)/1e9;

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        algoritmoKosaraju(grafo);
        clock_gettime(CLOCK_MONOTONIC, &fim);
        tempoKosaraju = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec -inicio.tv_nsec)/1e9;

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        printf("\n=== Caminho crítico ===\n");
        caminhoCritico(grafo);
        clock_gettime(CLOCK_MONOTONIC, &fim);
        tempoCC = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec -inicio.tv_nsec)/1e9;

        printf("\n=== FIM bateria de testes ===\n");

        printf("Tempo de Execução da Função Ordem Topológica : %lf\n", tempoOT);
        printf("Tempo de Execução de Dijkstra : %lf\n", tempoMC);
        printf("Tempo de Execução de DFS iterativa : %lf\n", tempoDFSIterativo);
        printf("Tempo de Execução de DFS Recursiva : %lf\n", tempoDFSRecursivo);
        printf("Tempo de Execução de Arvore Geradora Minima (Prim) : %lf\n", tempoAGM);
        printf("Tempo de Execução de BFS : %lf\n", tempoBFS);
        printf("Tempo de Execução de Caminho Critico : %lf\n", tempoCC);
        printf("Tempo de Execução de Kosaraju : %lf\n", tempoKosaraju);
    }
