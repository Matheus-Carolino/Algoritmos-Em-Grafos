#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "validacao.h"

    struct No {
        int destino;
        int peso;
        No *prox;
    };

    struct Grafo {
        int V;      //Número de Vértices
        int A;      //Número de Arestas
        No **lista; //Lista de adjacência
    };

    //--- Funções Auxiliares ---
    //Criar um novo Nó com alocação dinâmica
    No* criarNo(int destino, int peso) {
        No *novo = (No*)malloc(sizeof(No));
        novo->destino = destino;
        novo->peso = peso;
        novo->prox = NULL;
        return novo;
    }

    //inicializa o grafo com alocação dinâmica
    Grafo* criarGrafo(int vertices, int arestas) {
        Grafo *grafo = (Grafo*)malloc(sizeof(Grafo));
        if (grafo == NULL)
            return NULL;

        grafo->V = vertices;
        grafo->A = arestas;

        //Aloca o array de listas (indexado de 1 até V)
        grafo->lista = (No**)malloc((vertices) * sizeof(No*));
        if (grafo->lista == NULL) {
            free(grafo);
            return NULL;
        }

        //Inicializa todas as listas como vazias
        for (int i = 0; i <= vertices; i++) {
            grafo->lista[i] = NULL;
        }
        return grafo;
    }
    //liberar o grafico para evitar memory leak
    void liberarGrafo(Grafo *grafo) {
        if (grafo == NULL)
            return;
        No *atual, *prox;
        //libera cada No na lista
        for (int i = 0; i < grafo->V; i++) {
            atual = grafo->lista[i];
            while (atual != NULL) {
                prox = atual->prox;
                free(atual);
                atual = prox;
            }
        }
        //libera a lista
        free(grafo->lista);
        //libera o grafo
        free(grafo);
    }

    //função auxiliar para inserir arestas ordendas na lista
    void inserirOrdenado(No** lista, No* novo) {
        //Evita operações em ponteiros nulos
        if (lista == NULL || novo == NULL)
            return;
        No* atual = *lista;
        No* ant = NULL;
        //Percorre a lista até encontrar a posição correta da inserção
        while (atual != NULL && atual->destino < novo->destino) {
            ant = atual;
            atual = atual->prox;
        }
        //Caso 1: Inserção no início da lista (lista vazia ou menor elemento)
        if (atual == NULL) {
            novo->prox = *lista;
            *lista = novo;
        }
        //Caso 2: Inserção no meio ou no fim da lista
        else {
            ant->prox = novo;
            novo->prox = atual;
        }
    }

    //--- Funções principais dos Grafos ---
    //adiciona uma aresta direcionada (origem->destino)
    void adicionarArestaDirecionada(Grafo *grafo, int origem, int destino, int peso) {
        //evita operações com ponteiros nulos
        if (grafo == NULL)
            return;
        No* novo = criarNo(destino, peso);
        if (novo == NULL)
            return;
        //chama a função auxiliar
        inserirOrdenado(&grafo->lista[origem], novo);
    }

    //adiciona uma aresta bidirecional (origem <-> destino)
    void adicionarArestaBidirecionada(Grafo *grafo, int origem, int destino, int peso) {
        //evita operações com ponteiros nulos
        if (grafo == NULL)
            return;
        No* novoDestino = criarNo(destino, peso);
        No* novoOrigem = criarNo(origem, peso);

        if (novoOrigem == NULL || novoDestino == NULL) {
            free(novoDestino);
            free(novoOrigem);
            return;
        }
        //chama a função auxiliar
        inserirOrdenado(&grafo->lista[origem], novoDestino);
        inserirOrdenado(&grafo->lista[destino], novoOrigem);
    }

    //Lê o grafo do arquivo.txt
    GrafoStatus lerGrafoDeArquivo(const char* nomeArquivo, Grafo** grafo) {
        if (grafo == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;
        *grafo = NULL; //evita lixo de memória residual

        FILE* arquivo = fopen(nomeArquivo, "r");
        if (arquivo == NULL)
            return ERRO_ARQUIVO_NAO_ENCONTRADO;

        int V, A;
        char linha[100];
        char lixo[2];

        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            return ERRO_ARQUIVO_NAO_ENCONTRADO;
        }

        //verifica se o cabeçalho está no formato correto
        if (sscanf(linha, "%d %d %1s", &V, &A, lixo) != 2) {
            fclose(arquivo);
            return ERRO_FORMATO_INVALIDO;
        }

        //verificar se algum dos valores é negativo
        if (V < 0 || A < 0) {
            fclose(arquivo);
            return ERRO_NUMERO_INVALIDO;
        }
        //cria o grafo
        Grafo *novoGrafo = criarGrafo(V, A);

        int origem, destino, peso;
        //Loop para ler as arestas
        for (int i = 0; i < A; i++) {
            //Lê a próxima linha do arquivo
            if (fgets(linha, sizeof(linha), arquivo) == NULL) {
                fclose(arquivo);
                //O arquivo acabou antes de ler todas as arestas
                return ERRO_FORMATO_INVALIDO;
            }
            //Valida se a linha esta no formato correto
            if (sscanf(linha, "%d %d %d %1s", &origem, &destino, &peso, lixo) != 3) {
                fclose(arquivo);
                return ERRO_FORMATO_INVALIDO;
            }

            //Valida se os índices dos vértices são válidos
            if (origem < 0 || origem >= V || destino < 0 || destino >= V) {
                fclose(arquivo);
                return ERRO_ARESTA_INVALIDA;
            }

            adicionarArestaDirecionada(novoGrafo, origem, destino, peso);
        }
        //exporta o grafo preenchido e validado
        *grafo = novoGrafo;
        fclose(arquivo);
        return GRAFO_OK;
    }

