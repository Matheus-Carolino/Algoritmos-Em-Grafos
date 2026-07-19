#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    //Cria um novo Nó com alocação dinâmica
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
        // Tratamento de erro para ponteiro nulo
        if (grafo == NULL)
            return NULL;

        grafo->V = vertices;
        grafo->A = arestas;

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
        // Tratamento de erro para ponteiro nulo
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
        // Tratamento de erro para ponteiro nulo
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
        // Tratamento de erro para ponteiro nulo
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
    GrafoStatus DFSRecursiva(Grafo* grafo, int origem, int *visitado) {
        // Verifica se o grafo é válido
        if (grafo == NULL || visitado == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

        if (origem < 0 || origem >= grafo->V)
            return ERRO_VERTICE_INVALIDO;

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
        return GRAFO_OK;
    }

    // Realiza uma busca em profundidade (DFS) de forma iterativa
    GrafoStatus DFSIterativa(Grafo* grafo, int origem, int* visitado) {
        // Verifica se o grafo é válido
        if (grafo == NULL || visitado == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

        if (origem < 0 || origem >= grafo->V)
            return ERRO_VERTICE_INVALIDO;

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
        return GRAFO_OK;
    }

    // Identifica os componentes conexos do grafo utilizando DFS recursiva
    // Obs.: Funciona apenas para grafos não direcionados
    GrafoStatus DFSRecCompConexos(Grafo* grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

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
        return GRAFO_OK;
    }

    // Identifica os componentes conexos do grafo utilizando DFS iterativa
    // Obs.: Funciona apenas para grafos não direcionados
    GrafoStatus DFSIteCompConexos(Grafo* grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

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
        return GRAFO_OK;
    }

    // Busca em largura (BFS)
    GrafoStatus BFS(Grafo *grafo, int origem) {

        // Verifica se o grafo é válido
        if (grafo == NULL || grafo->lista == NULL) {
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        if (origem < 0 || origem >= grafo->V) {
            return ERRO_VERTICE_INVALIDO;
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

        int impressos = 0;
        while (inicio < fim) {
            // Desenfileira o vértice mais antigo da fila
            int vert = fila[inicio++];
            if (impressos > 0) {
                printf(" -> ");
            }
            printf("%d", vert);
            impressos++;

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
        return GRAFO_OK;
    }

    // Ordenação Topologica do Grafo
    GrafoStatus ordemTopologica(Grafo *grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL || grafo->lista == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

        int V = grafo->V;

        // Aloca e zera o vetor que guardará a quantidade de arestas que chegam a cada vértice
        int *grauEntrada = (int*)calloc(V, sizeof(int));

        // Aloca o vetor que funcionará como uma fila estática para armazenar os vértices prontos para processamento
        int *fila = (int*)malloc(V * sizeof(int));

        if (grauEntrada == NULL || fila == NULL) {
            free(grauEntrada);
            free(fila);
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        // Ponteiros de controle da fila
        int inicio = 0;
        int fim = 0;

        // Contador para verificar a presença de ciclos ao fim da execução
        int verticesProcessados = 0;

        // Calcula o grau de entrada de cada vértice mapeando a lista de adjacência
        for (int i = 0; i < V; i++) {
            No* atual = grafo->lista[i];
            while (atual != NULL) {
                // Garante que o vértice de destino está dentro dos limites válidos do grafo
                if (atual->destino >= 0 && atual->destino < V)
                    grauEntrada[atual->destino]++;
                atual = atual->prox;
            }
        }

        // Inicializa a fila com todos os vértices que não possuem dependências de entrada (grau == 0)
        for (int i = 0; i < V; i++) {
            if (grauEntrada[i] == 0)
                fila[fim++] = i;
        }

        printf("\n=== ORDENACAO TOPOLOGICA ===\n");
        printf("Ordem: ");

        // Processa a fila utilizando uma variação do Algoritmo de Kahn
        while (inicio < fim) {
            // Desenfileira o próximo vértice da fila
            int u = fila[inicio++];

            printf("%d", u);
            verticesProcessados++;

            if (verticesProcessados < V)
                printf(" -> ");

            // Atualiza o impacto da remoção do vértice 'u' sobre os seus vizinhos adjacentes
            No* atual = grafo->lista[u];
            while (atual != NULL) {
                int v = atual->destino;
                grauEntrada[v]--;

                if (grauEntrada[v] == 0) {
                    /*
                     * Inserção ordenada customizada na fila:
                     * Mantém a fila interna ordenada de forma decrescente para priorizar vértices
                     * de maior valor em cenários de desempate
                     */
                    int inserido = 0;

                    // Varre a porção ativa da fila (elementos ainda não processados) buscando a posição correta
                    for (int i = inicio; i < fim; i++) {
                        if (v > fila[i]) {
                            // Desloca os elementos subsequentes para a direita a fim de abrir espaço para o vértice 'v'
                            for (int j = fim; j > i; j--)
                                fila[j] = fila[j-1];
                            fila[i] = v;     // Insere o vértice na posição correta encontrada
                            inserido = 1;    // Sinaliza que a inserção prioritária foi concluída
                            break;           // Interrompe a busca de posição
                        }
                    }

                    // Caso não precise ser inserido no meio (menor prioridade atual), adiciona normalmente ao final da fila
                    if (!inserido)
                        fila[fim] = v;
                    fim++;
                }
                atual = atual->prox;
            }
        }
        printf("\n");

        // Verificação de Ciclos
        if (verticesProcessados < V)
            printf("O grafo possui pelo menos um ciclo. A ordenacao acima esta incompleta.\n");

        free(grauEntrada);
        free(fila);

        return GRAFO_OK;
    }

    // Árvore Geradora Mínima (AGM) utilizando o Algoritmo de Prim.
    GrafoStatus algoritmoPrim(Grafo* g) {
        // Verifica se o grafo é válido
        if (g == NULL || g->lista == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

        int V = g->V;

        // Alocação dos vetores auxiliares para construção da AGM
        int* pai = (int*)malloc(V * sizeof(int));   // Armazena a aresta que conecta o vértice à árvore
        int* chave = (int*)malloc(V * sizeof(int)); // Guarda o menor peso de aresta para conectar o vértice à AGM
        int* naAGM = (int*)calloc(V, sizeof(int));  // Mapeia se o vértice já foi incluído na árvore (0 = não, 1 = sim)

        // Verifica se o grafo é válido
        if (pai == NULL || chave == NULL || naAGM == NULL) {
            free(pai);
            free(chave);
            free(naAGM);
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        // Inicialização dos vetores de controle
        for (int i = 0; i < V; i++) {
            chave[i] = INT_MAX; // Define "infinito" como peso inicial
            pai[i] = -1;        // -1 indica que nenhum pai ou conexão foi definida ainda
        }

        // Acumulador para calcular a soma total dos pesos de todas as arestas da AGM
        int peso_total_componentes = 0;

        printf("\n=== ÁRVORE GERADORA MÍNIMA (PRIM) ===\n");

        // Loop externo para garantir que todos os componentes conexos isolados do grafo sejam processados
        for (int raiz = 0; raiz < V; raiz++) {
            // Se o vértice já foi processado e incluído em uma AGM anterior, ignora
            if (naAGM[raiz]) continue;

            // O vértice escolhido como raiz do componente inicia com peso zero para ser selecionado primeiro
            chave[raiz] = 0;

            // Loop principal para processar e expandir os vértices do componente atual
            for (int count = 0; count < V; count++) {
                int u = -1;
                int min = INT_MAX;

                // Encontra o vértice fora da AGM que possui a menor chave de conexão atual
                for (int v = 0; v < V; v++) {
                    if (!naAGM[v] && chave[v] < min) {
                        min = chave[v];
                        u = v;
                    }
                }

                // Se não encontrar nenhum vértice acessível restante, o componente atual foi completamente processado
                if (u == -1) break;

                // Inclui formalmente o vértice 'u' na Árvore Geradora Mínima
                naAGM[u] = 1;

                // Se o vértice possui um pai associado, significa que uma aresta válida da AGM foi fechada
                if (pai[u] != -1) {
                    peso_total_componentes += chave[u]; // Acumula o peso da aresta no custo total
                    printf("Aresta: %d - %d | Peso: %d\n", pai[u], u, chave[u]);
                }

                // Atualiza as chaves de corte e os pais de todos os vizinhos adjacentes ao vértice 'u' recém-incluído
                No* adj = g->lista[u];
                while (adj != NULL) {
                    int v = adj->destino;
                    int peso = adj->peso;

                    // Se o vizinho está fora da AGM e o peso desta nova aresta
                    // for menor do que a menor chave registrada anteriormente para ele, atualiza o vínculo
                    if (!naAGM[v] && peso < chave[v]) {
                        pai[v] = u;       // Define 'u' como o melhor ponto de conexão para o vértice 'v'
                        chave[v] = peso;  // Atualiza o custo de entrada para este novo menor peso encontrado
                    }
                    adj = adj->prox; // Avança para o próximo vizinho na lista de adjacência
                }
            }
        }

        printf("=====================================\n");
        printf("Peso Total da AGM: %d\n", peso_total_componentes); // Exibe o peso consolidado de toda a estrutura
        printf("=====================================\n");

        // Liberação segura de todas as memórias locais alocadas
        free(pai);
        free(chave);
        free(naAGM);

        return GRAFO_OK;
    }

    //Algoritmo de Dijkstra para Caminhos Mínimos.
    GrafoStatus algoritmoDijkstra(Grafo *grafo, int origem) {
        // Verifica se o grafo é válido
        if (grafo == NULL || grafo->lista == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

        int qtdVer = grafo->V;

        // Validação do parâmetro de origem com base nos limites do grafo
        if (origem < 0 || origem >= qtdVer)
            return ERRO_VERTICE_INVALIDO;

        // Alocação dos vetores auxiliares para o cálculo do caminho mínimo
        int *dist = (int*)malloc(qtdVer * sizeof(int));
        int *antecessor = (int*)malloc(qtdVer * sizeof(int));
        int *visitado = (int*)calloc(qtdVer, sizeof(int));

        // Validação de alocação de memória para os vetores locais
        if (dist == NULL || antecessor == NULL || visitado == NULL) {
            free(dist);
            free(antecessor);
            free(visitado);
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        // Inicialização das distâncias estimadas e dos antecessores
        for (int i = 0; i < qtdVer; i++) {
            dist[i] = INT_MAX;     // Define infinito como distância inicial
            antecessor[i] = -1;    // -1 indica que não há antecessor definido
        }
        dist[origem] = 0;          // A distância da origem para ela mesma é zero

        for (int i = 0; i < qtdVer - 1; i++) {
            int min = INT_MAX;
            int u = -1;

            // Encontra o vértice com a menor distância estimada que ainda não foi visitado
            for (int v = 0; v < qtdVer; v++) {
                if (!visitado[v] && dist[v] < min) {
                    min = dist[v];
                    u = v;
                }
            }

            // Se os vértices restantes forem inacessíveis a partir da origem, interrompe o laço
            if (u == -1) break;

            // Marca o vértice selecionado como visitado (fechado)
            visitado[u] = 1;

            // Relaxamento das arestas adjacentes do vértice 'u'
            No *adj = grafo->lista[u];
            while (adj != NULL) {
                int v = adj->destino;
                int peso = adj->peso;

                // Se o vizinho não foi visitado e o caminho por 'u' for menor do que o anteriormente registrado
                if (!visitado[v] && dist[u] != INT_MAX && dist[u] + peso < dist[v]) {
                    dist[v] = dist[u] + peso; // Atualiza a menor distância
                    antecessor[v] = u;        // Define 'u' como o caminho de chegada para 'v'
                }
                adj = adj->prox; // Avança para o próximo vizinho na lista de adjacência
            }
        }

        // Exibição dos resultados
        printf("\n=== DIJKSTRA (Origem: %d) ===\n", origem);
        printf("Vertice | Distancia | Caminho\n");
        printf("--------|-----------|------------------\n");

        // Aloca uma pilha temporária para inverter e reconstruir a ordem do caminho de cada vértice
        int *caminhoAux = (int*)malloc(qtdVer * sizeof(int));
        if (caminhoAux == NULL) {
            free(dist);
            free(antecessor);
            free(visitado);
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        for (int i = 0; i < qtdVer; i++) {

            if (dist[i] == INT_MAX) {
                printf("%-7d | Infinito  | -\n", i);
            } else {
                printf("%-7d | %-9d | ", i, dist[i]);

                // Reconstrói o caminho de trás para frente (do vértice alvo até a origem)
                int tam = 0;
                int atual = i;
                while (atual != -1) {
                    caminhoAux[tam++] = atual;
                    atual = antecessor[atual];
                }

                // Desempilha e imprime o caminho na ordem correta
                for (int j = tam - 1; j >= 0; j--) {
                    printf("%d", caminhoAux[j]);
                    if (j > 0) {
                        printf(" -> ");
                    }
                }
                printf("\n");
            }
        }

        // Liberação segura de todas as memórias alocadas localmente
        free(caminhoAux);
        free(dist);
        free(antecessor);
        free(visitado);

        return GRAFO_OK;
    }

    // --- Funções para Estatisticas dos Grafos ---
    //Verifica se o grafo é direcionado
    bool isDirecionado(Grafo *grafo) {
        if (grafo == NULL) return false;

        for (int i = 0; i < grafo->V; i++) {
            No *atual = grafo->lista[i];
            while (atual != NULL) {
                int v = atual->destino;
                bool temVolta = false;

                No *reverso = grafo->lista[v];
                while (reverso != NULL) {
                    if (reverso->destino == i) {
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

    //Auxiliares e função para detectar ciclos por DFS (cores)
    bool dfsCiclo(Grafo *grafo, int i, int *cor, int pai, bool direcionado) {
        cor[i] = 1;
        No *atual = grafo->lista[i];

        while (atual != NULL) {
            int v = atual->destino;
            if (cor[v] == 1) {
                if (!direcionado && v == pai) {
                    atual = atual->prox;
                    continue;
                }
                return true; // Encontrou uma aresta de volta para um nó em processamento
            }

            if (cor[v] == 0 && dfsCiclo(grafo, v, cor, i, direcionado)) {
                return true;
            }
            atual = atual->prox;
        }
        cor[i] = 2; // 2 = Totalmente processado (Preto)
        return false;
    }

    bool temCiclo(Grafo *grafo, bool direcionado) {
        if (grafo == NULL || grafo->V == 0) return false;

        int *cor = (int*) calloc(grafo->V, sizeof(int));
        for (int i = 0; i < grafo->V; i++) {
            if (cor[i] == 0) {
                if (dfsCiclo(grafo, i, cor, -1, direcionado)) {
                    free(cor);
                    return true;
                }
            }
        }
        free(cor);
        return false;
    }

    //Auxiliares e função principal para verificar se o grafo é conexo
    void dfsConexo(Grafo *grafo, int i, bool *visitado) {
        visitado[i] = true;
        No *atual = grafo->lista[i];
        while (atual != NULL) {
            if (!visitado[atual->destino]) {
                dfsConexo(grafo, atual->destino, visitado);
            }
            atual = atual->prox;
        }
    }

    bool isConexo(Grafo *grafo) {
        if (grafo == NULL || grafo->V == 0) return false;

        bool *visitado = (bool*) calloc(grafo->V, sizeof(bool));
        // Dispara a busca a partir do primeiro vértice (0)
        dfsConexo(grafo, 0, visitado);

        // Se algum vértice não foi alcançado, o grafo é desconexo
        for (int i = 0; i < grafo->V; i++) {
            if (!visitado[i]) {
                free(visitado);
                return false;
            }
        }
        free(visitado);
        return true;
    }

    //Calcula a densidade do grafo
    float calcularDensidade(Grafo *grafo, bool direcionado) {
        if (grafo == NULL || grafo->V <= 1) return 0.0;

        if (direcionado) {
            return (float)grafo->A / (grafo->V * (grafo->V - 1));
        }
        return (float)grafo->A / ((grafo->V * (grafo->V - 1)) / 2.0);
    }

    //Calcula e imprime os graus de cada vértice baseado no tipo de grafo
    void calcularImprimirGraus(Grafo *grafo, bool direcionado) {
        if (grafo == NULL) return;

        int *grauEntrada = (int*) calloc(grafo->V, sizeof(int));
        int *grauSaida = (int*) calloc(grafo->V, sizeof(int));

        for (int u = 0; u < grafo->V; u++) {
            No *atual = grafo->lista[u];
            while (atual != NULL) {
                grauSaida[u]++;
                grauEntrada[atual->destino]++;
                atual = atual->prox;
            }
        }

        printf("- Grau de cada vertice:\n");
        for (int i = 0; i < grafo->V; i++) {
            if (direcionado) {
                printf("  Vertice %d: Grau de Entrada = %d | Grau de Saida = %d\n", i, grauEntrada[i], grauSaida[i]);
            } else {
                printf("  Vertice %d: Grau = %d\n", i, grauSaida[i]);
            }
        }
        free(grauEntrada);
        free(grauSaida);
    }

    //Executor de tudo que foi pedido da opção 8
    void exibirEstatisticas(Grafo *grafo) {
        if (grafo == NULL) {
            printf("Erro: Nenhum grafo carregado na memoria.\n");
            return;
        }

        printf("\n=====================================");
        printf("\n===  OPCAO 8 - ESTATISTICAS DO GRAFO ===");
        printf("\n=====================================\n");

        printf("- Numero de vertices: %d\n", grafo->V);
        printf("- Numero de arestas: %d\n", grafo->A);

        bool direcionado = isDirecionado(grafo);
        printf("- Tipo de grafo: %s\n", direcionado ? "Direcionado (Digrafo)" : "Nao-direcionado");

        calcularImprimirGraus(grafo, direcionado);

        bool conexo = isConexo(grafo);
        printf("- Conectividade: %s\n", conexo ? "CONEXO" : "DESCONEXO");

        bool ciclos = temCiclo(grafo, direcionado);
        printf("- Presenca de ciclos: %s\n", ciclos ? "SIM, contem ciclos" : "NAO possui ciclos (Aciclico)");

        float densidade = calcularDensidade(grafo, direcionado);
        printf("- Densidade do grafo: %.4f\n", densidade);
        printf("=====================================\n");
    }