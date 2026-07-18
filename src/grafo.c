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
