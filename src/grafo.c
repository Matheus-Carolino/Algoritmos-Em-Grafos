#include "grafo.h"

    //--- Funções Auxiliares ---
    //Cria um novo Nó com alocação dinâmica
    No* criarNo(int destino, int peso) {
        No *novo = (No*)malloc(sizeof(No));
        novo->destino = destino;
        novo->peso = peso;
        novo->prox = NULL;
        return novo;
    }

    //inicializa o grafo com alocação dinâmica
    Grafo* criarGrafo(int vertices, int arestas, int direcionado) {
        Grafo *grafo = (Grafo*)malloc(sizeof(Grafo));
        // Tratamento de erro para ponteiro nulo
        if (grafo == NULL)
            return NULL;

        grafo->V = vertices;
        grafo->A = arestas;
        grafo->direcionado = direcionado;

        //Aloca o array de listas (indexado de 0 até V - 1)
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
        // Tratamento de erro para ponteiro nulo
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

    void mostrarGrafo(Grafo *grafo) {
        // Tratamento de erro para ponteiro nulo
        if (grafo == NULL)
            return;

        printf("\n=== LISTAS DE ADJACENCIA ===\n\n");

        No* atual = NULL;
        for (int i=0; i<grafo->V; i++) {
            // Se a lista de vértices não está vazia
            printf("Vertice [%d]: ", i);
            if (grafo->lista[i] != NULL){
                atual = grafo->lista[i];
                while (atual->prox != NULL) {
                    printf("(%d, peso %d) -> ", atual->destino, atual->peso);
                    atual = atual->prox;
                }
                printf("(%d, peso %d) \n",atual->destino, atual->peso);
            }
            else {
                printf("sem conexoes\n");
            }
        }
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
        if (ant == NULL) {
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
    //adiciona uma aresta (origem->destino)
    void adicionarAresta(Grafo *grafo, int origem, int destino, int peso) {
        // Tratamento de erro para ponteiro nulo
        if (grafo == NULL)
            return;

        No* novoDestino = criarNo(destino, peso);
        if (novoDestino == NULL)
            return;

        //chama a função auxiliar
        inserirOrdenado(&grafo->lista[origem], novoDestino);

        if (!grafo->direcionado) {
            No* novoOrigem = criarNo(origem, peso);
            if (novoOrigem == NULL) {
                free(novoOrigem);
                return;
            }

            //chama a função auxiliar
            inserirOrdenado(&grafo->lista[destino], novoOrigem);
        }
    }

    //Lê e trata grafo de um arquivo
    GrafoStatus lerGrafoDeArquivo(const char* nomeArquivo, Grafo** grafo, int direcionado) {
        // Tratamento de erro para ponteiro nulo
        if (grafo == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;
        *grafo = NULL; //evita lixo de memória residual

        // Abertura de arquivo
        FILE* arquivo = fopen(nomeArquivo, "r");
        if (arquivo == NULL)
            return ERRO_ARQUIVO_NAO_ENCONTRADO;

        // Leitura do cabeçalho
        char linha[100];
        char lixo[2];

        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            return ERRO_ARQUIVO_NAO_ENCONTRADO;
        }

        int V, A;
        //verifica se o cabeçalho está no formato correto
        if (sscanf(linha, "%d %d %1s", &V, &A, lixo) != 2) {
            fclose(arquivo);
            return ERRO_FORMATO_INVALIDO;
        }

        //verificar se algum dos valores é negativo
        if (V <= 0 || A <= 0) {
            fclose(arquivo);
            return ERRO_NUMERO_INVALIDO;
        }

        // Cria o grafo
        Grafo *novoGrafo = criarGrafo(V, A, direcionado);
        if (novoGrafo == NULL) {
            fclose(arquivo);
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        int origem, destino, peso;

        // Loop para ler as arestas
        for (int i = 0; i < A; i++) {

            //Lê a próxima linha do arquivo
            if (fgets(linha, sizeof(linha), arquivo) == NULL) {
                fclose(arquivo);
                //O arquivo acabou antes de ler todas as arestas
                liberarGrafo(novoGrafo);
                return ERRO_FORMATO_INVALIDO;
            }

            //Valida se a linha esta no formato correto
            if (sscanf(linha, "%d %d %d %1s", &origem, &destino, &peso, lixo) != 3) {
                fclose(arquivo);
                liberarGrafo(novoGrafo);
                return ERRO_FORMATO_INVALIDO;
            }

            // Valida se os índices dos vértices são válidos e se o peso é positivo
            if (origem < 0 || origem >= V || destino < 0 || destino >= V || peso < 0) {
                fclose(arquivo);
                liberarGrafo(novoGrafo);
                return ERRO_ARESTA_INVALIDA;
            }

            // Adiciona aresta correspondente a característica do grafo
            adicionarAresta(novoGrafo, origem, destino, peso);
        }

        // Exporta o grafo preenchido e validado
        *grafo = novoGrafo;
        fclose(arquivo);
        return GRAFO_OK;
    }

