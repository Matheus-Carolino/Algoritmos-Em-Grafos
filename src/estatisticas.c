#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafo.h" 
#include "validacao.h"

// verifica se o grafo é direcionado
bool isDirecionado(Grafo *G) {
    if (G == NULL) return false;
    
    for (int u = 0; u < G->V; u++) {
        No *atual = G->lista[u];
        while (atual != NULL) {
            int v = atual->destino;
            bool temVolta = false;
            
            No *reverso = G->lista[v];
            while (reverso != NULL) {
                if (reverso->destino == u) {
                    temVolta = true;
                    break;
                }
                reverso = reverso->prox;
            }
            if (!temVolta) return true; 
            
            atual = atual->prox;
        }
    }
    return false;
}

// Calcula e imprime os graus de cada vértice baseado no tipo de grafo
void calcularImprimirGraus(Grafo *G, bool direcionado) {
    if (G == NULL) return;

    int *grauEntrada = (int*) calloc(G->V, sizeof(int));
    int *grauSaida = (int*) calloc(G->V, sizeof(int));
    
    for (int u = 0; u < G->V; u++) {
        No *atual = G->lista[u];
        while (atual != NULL) {
            grauSaida[u]++;
            grauEntrada[atual->destino]++;
            atual = atual->prox;
        }
    }
    
    printf("- Grau de cada vertice:\n");
    for (int i = 0; i < G->V; i++) {
        if (direcionado) {
            printf("  Vertice %d: Grau de Entrada = %d | Grau de Saida = %d\n", i, grauEntrada[i], grauSaida[i]);
        } else {
            printf("  Vertice %d: Grau = %d\n", i, grauSaida[i]);
        }
    }
    
    free(grauEntrada);
    free(grauSaida);
}

// Auxiliares e função principal para detectar ciclos por DFS (Cores)
bool dfsCiclo(Grafo *G, int u, int *cor, int pai, bool direcionado) {
    cor[u] = 1; 
    No *atual = G->lista[u];
    
    while (atual != NULL) {
        int v = atual->destino;
        if (cor[v] == 1) {
            if (!direcionado && v == pai) {
                atual = atual->prox;
                continue;
            }
            return true; // Encontrou uma aresta de volta para um nó em processamento
        }
        
        if (cor[v] == 0 && dfsCiclo(G, v, cor, u, direcionado)) {
            return true;
        }
        atual = atual->prox;
    }
    cor[u] = 2; // 2 = Totalmente processado (Preto)
    return false;
}

bool temCiclo(Grafo *G, bool direcionado) {
    if (G == NULL || G->V == 0) return false;
    
    int *cor = (int*) calloc(G->V, sizeof(int));
    for (int i = 0; i < G->V; i++) {
        if (cor[i] == 0) {
            if (dfsCiclo(G, i, cor, -1, direcionado)) {
                free(cor);
                return true;
            }
        }
    }
    free(cor);
    return false;
}

// Auxiliares e função principal para verificar se o grafo é conexo
void dfsConexo(Grafo *G, int u, bool *visitado) {
    visitado[u] = true;
    No *atual = G->lista[u];
    while (atual != NULL) {
        if (!visitado[atual->destino]) {
            dfsConexo(G, atual->destino, visitado);
        }
        atual = atual->prox;
    }
} 

bool isConexo(Grafo *G) {
    if (G == NULL || G->V == 0) return false;
    
    bool *visitado = (bool*) calloc(G->V, sizeof(bool));
    // Dispara a busca a partir do primeiro vértice (0)
    dfsConexo(G, 0, visitado);
    
    // Se algum vértice não foi alcançado, o grafo é desconexo
    for (int i = 0; i < G->V; i++) {
        if (!visitado[i]) {
            free(visitado);
            return false; 
        }
    }
    
    free(visitado);
    return true; 
}

//calcula a densidade do grafo 
float calcularDensidade(Grafo *G, bool direcionado) {
    if (G == NULL || G->V <= 1) return 0.0;
    
    if (direcionado) {
        return (float)G->A / (G->V * (G->V - 1));
    } else {
        return (float)G->A / ((G->V * (G->V - 1)) / 2.0);
    }
}

//Executor de tudo que foi pedido da opção 8
void exibirEstatisticas(Grafo *G) {
    if (G == NULL) {
        printf("Erro: Nenhum grafo carregado na memoria.\n");
        return;
    }
    
    printf("\n=====================================");
    printf("\n===  OPCAO 8 - ESTATISTICAS DO GRAFO ===");
    printf("\n=====================================\n");
    
    printf("- Numero de vertices: %d\n", G->V);
    printf("- Numero de arestas: %d\n", G->A);
    
    bool direcionado = isDirecionado(G);
    printf("- Tipo de grafo: %s\n", direcionado ? "Direcionado (Digrafo)" : "Nao-direcionado");
    
    calcularImprimirGraus(G, direcionado);
    
    bool conexo = isConexo(G);
    printf("- Conectividade: %s\n", conexo ? "CONEXO" : "DESCONEXO");
    
    bool ciclos = temCiclo(G, direcionado);
    printf("- Presenca de ciclos: %s\n", ciclos ? "SIM, contem ciclos" : "NAO possui ciclos (Aciclico)");
    
    float densidade = calcularDensidade(G, direcionado);
    printf("- Densidade do grafo: %.4f\n", densidade);
    printf("=====================================\n");
}