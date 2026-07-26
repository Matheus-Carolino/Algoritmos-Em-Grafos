#ifndef GRAFOS_ESTATISTICAS_H
#define GRAFOS_ESTATISTICAS_H

    #include <stdbool.h>
    #include "grafo.h"

    // --- Função para Estatisticas dos Grafos ---
    bool temCiclo(Grafo *grafo);
    bool isConexo(Grafo *grafo);
    bool isFortementeConexo(Grafo *grafo);
    float calcularDensidade(Grafo *grafo);
    void calcularImprimirGraus(Grafo *grafo);
    void exibirEstatisticas(Grafo *grafo);

#endif //GRAFOS_ESTATISTICAS_H
