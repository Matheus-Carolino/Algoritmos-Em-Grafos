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
        for (int i = 0; i < vertices; i++) {
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

    // --- Funções de busca ---
    // Realiza uma busca em profundidade (DFS) de forma recursiva
    void DFSRecursiva(Grafo* grafo, int origem, int *visitado) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return;

        // Marca o vértice atual como visitado e o imprime
        visitado[origem] = 1;
        printf("%d ", origem);

        // Percorre todos os vértices adjacentes
        No* atual = grafo->lista[origem];
        while (atual != NULL) {
            // Visita recursivamente os vértices não visitados
            if (!visitado[atual->destino]) {
                printf("-> ");
                DFSRecursiva(grafo, atual->destino, visitado);
            }
            atual = atual->prox;
        }
    }

    // Realiza uma busca em profundidade (DFS) de forma iterativa
    void DFSIterativa(Grafo* grafo, int origem, int* visitado) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return;

        // Inicializa a pilha auxiliar
        int pilha[grafo->V];
        int topo = -1;

        // Empilha o vértice de origem
        pilha[++topo] = origem;
        visitado[origem] = 1;

        printf("%d ", origem);

        // Enquanto houver vértices na pilha
        while (topo != -1) {
            // Faça destino o vértice no topo da pilha
            int destino = pilha[topo];

            No* atual = grafo->lista[destino];
            int encontrou = 0;

            // Procura o primeiro vizinho ainda não visitado
            while (atual != NULL) {
                if (!visitado[atual->destino]) {
                    visitado[atual->destino] = 1;
                    pilha[++topo] = atual->destino;

                    printf("-> %d ", atual->destino);

                    encontrou = 1;
                    // Quando encontrar o primeiro vizinho, encerra o laço
                    break;
                }

                atual = atual->prox;
            }

            // Se não encontrou vizinhos não visitados
            if (!encontrou)
                topo--;
        }
    }

    // Identifica os componentes conexos do grafo utilizando DFS recursiva
    // Obs.: Funciona apenas para grafos não direcionados
    void DFSRecCompConexos(Grafo* grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return;

        // Inicializa o vetor para controlar os vértices visitados
        int visitado[grafo->V];
        for (int i = 0; i < grafo->V; i++)
            visitado[i] = 0;


        // Contador para numerar os componentes conexos
        int contador = 1;

        printf("=== DFS RECURSIVA ===\n");

        // Percorre todos os vértices do grafo
        for (int i = 0; i < grafo->V; i++) {
            // Inicia uma nova DFS caso o vértice ainda não tenha sido visitado
            if (!visitado[i] && grafo->lista[i] != NULL) {
                printf("Componente %d: ", contador++);
                DFSRecursiva(grafo, i, visitado);
                printf("\n");
            }
        }
    }

    // Identifica os componentes conexos do grafo utilizando DFS iterativa
    // Obs.: Funciona apenas para grafos não direcionados
    void DFSIteCompConexos(Grafo* grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return;

        // Inicializa o vetor para controlar dos vértices visitados
        int visitado[grafo->V];
        for (int i = 0; i < grafo->V; i++)
            visitado[i] = 0;

        // Contador para numerar os componentes conexos
        int contador = 1;

        printf("=== DFS ITERATIVA ===\n");

        // Percorre todos os vértices do grafo
        for (int i = 0; i < grafo->V; i++) {
            // Inicia uma nova DFS caso o vértice ainda não tenha sido visitado
            if (!visitado[i] && grafo->lista[i] != NULL) {
                printf("Componente %d: ", contador++);
                DFSIterativa(grafo, i, visitado);
                printf("\n");
            }
        }
    }

    // Busca em largura (BFS)
    void BFS(Grafo *grafo, int origem) {

        // Tratamento de erro para ponteiro nulo
        if (grafo == NULL || grafo->lista == NULL) {
            printf("Erro: Grafo nao inicializado ou lista vazia.\n");
            return;
        }

        // Tratamento de erro para vértice inválido
        if (origem < 0 || origem >= grafo->V) {
            printf("Erro: Vertice de origem %d invalido para um grafo com %d vertices.\n", origem, grafo->V);
            return;
        }

        // Vetores locais
        int fila[grafo->V];
        int visitado[grafo->V];
        int distancia[grafo->V];
        int inicio = 0;
        int fim = 0;

        // Inicializa os vetores de controle
        for (int i = 0; i < grafo->V; i++) {
            visitado[i] = 0; // 0 indica que o vertice ainda não foi visitado
            distancia[i] = -1; // -1 indica que o vértice ainda é inacessível
        }

        // Inicializa a estrutura com o vértice de partida
        fila[fim++] = origem;
        visitado[origem] = 1;
        distancia[origem] = 0;

        printf("Ordem de visitacao da BFS: ");

        while (inicio < fim) {
            // Desenfileira o vértice mais antigo da fila
            int vert = fila[inicio++];
            printf("%d ", vert);

            // Percorre todos os vizinhos na lista de adjacência do vértice atual
            No *aux = grafo->lista[vert];
            while (aux) {
                // Se o vizinho ainda não foi visitado, processa e enfileira
                if (!visitado[aux->destino]) {
                    if (aux->destino >= 0 && aux->destino < grafo->V) {
                        visitado[aux->destino] = 1;
                        distancia[aux->destino] = distancia[vert] + 1;
                        fila[fim++] = aux->destino;
                    }
                }
                aux = aux->prox;
            }
        }

        // Mostra o resultado das distâncias a partir do vértice de origem
        printf("\n\nDistancias a partir do vertice %d:\n", origem);
        for (int i = 0; i < grafo->V; i++) {
            if (distancia[i] != -1) {
                printf("Vertice %d: distancia = %d\n", i, distancia[i]);
            } else {
                printf("Vertice %d: inacessivel\n", i);
            }
        }
    }

    void ordemTopologica (Grafo *grafo) {
        No* atual;
        int zeroSetasApontando[grafo->V+1];
        int setasApontando[grafo->V+1];
        int inicio = 1;
        int fim = 1;
        int vertices = 0;
        int temp;
        int zeros;

        for (int i = 0; i < grafo->V + 1; i++) {
            setasApontando[i] = 0;
            zeroSetasApontando[i] = 0;
        }
        for (int i = 1; i < grafo->V+1; i++) {
            atual = grafo->lista[i];
            while (atual != NULL) {
                setasApontando[atual->destino]++;
                atual = atual->prox;
            }
        }

        for (int i = 1; i < grafo->V+1; i ++) {
            if (setasApontando[i] == 0) {
                zeroSetasApontando[fim] = i;
                fim++;
            }
        }

        while (inicio < fim) {
            zeros = zeroSetasApontando[inicio];
            vertices++;
            atual = grafo->lista[zeros];
            while (atual != NULL) {
                temp = atual->destino;
                setasApontando[temp]--;

                if (setasApontando[temp] == 0) {
                    zeroSetasApontando[fim] = temp;
                    fim++;
                }

                atual = atual->prox;
            }

            inicio++;
        }

        if (vertices < grafo->V) {
            printf("O Grafo n├úo ├® acicl├¡co\n"); // ciclico;
        } else {
            for (int i =1; i < grafo->V+1; i++) {
                printf("%d\n", zeroSetasApontando[i]); //j├í sai na ordem topol├│gica
            }
        }
    }
