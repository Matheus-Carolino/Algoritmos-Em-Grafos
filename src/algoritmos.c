    #include "algoritmos.h"
    #include <limits.h>

    // ----- Funções de busca -----

    // Realiza uma busca em profundidade (DFS) de forma recursiva
    GrafoStatus DFSRecursiva(Grafo* grafo, int origem, int *visitado) {
        // Verifica se o grafo é válido
        if (grafo == NULL || visitado == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;
        // Verifica se a origem está no escopo do grafo
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
        // Verifica se a origem está no escopo do grafo
        if (origem < 0 || origem >= grafo->V)
            return ERRO_VERTICE_INVALIDO;

        // Inicializa a pilha auxiliar
        int* pilha = (int*)malloc(grafo->V*sizeof(int));
        if (pilha == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;
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

            // Se não encontrou vizinhos não visitados, desempilha
            // Obs.: desempilhar só depois de visitar os vértices adjacentes
            // mantém a ordem igual a da DFS Recursiva
            if (!encontrou)
                topo--;
        }

        free(pilha);
        return GRAFO_OK;
    }

    // Identifica os componentes conexos do grafo utilizando DFS recursiva
    // Obs.: Funciona apenas para grafos não direcionados
    GrafoStatus DFSCompConexos(Grafo* grafo, int recursiva) {
        // Verifica se o grafo é válido
        if (grafo == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

        // Inicializa o vetor para controlar os vértices visitados
        // O calloc já inicia tudo com 0
        int *visitado = (int*)calloc(grafo->V, sizeof(int));
        if (visitado == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

        // Contador para numerar os componentes conexos
        int contador = 1;

        if (recursiva)
            printf("\n=== COMPONENTES CONEXOS (DFS RECURSIVA) ===\n");
        else
            printf("\n=== COMPONENTES CONEXOS (DFS ITERATIVA) ===\n");

        // Percorre todos os vértices do grafo
        for (int i = 0; i < grafo->V; i++) {
            // Inicia uma nova DFS caso o vértice ainda não tenha sido visitado
            if (!visitado[i] && grafo->lista[i] != NULL) {
                printf("Componente %d: ", contador++);

                if (recursiva)
                    DFSRecursiva(grafo, i, visitado);
                else
                    DFSIterativa(grafo, i, visitado);

                printf("\n");
            }
        }

        free(visitado);
        return GRAFO_OK;
    }

    // Busca em largura (BFS)
    GrafoStatus BFS(Grafo *grafo, int origem) {
        // Verifica se o grafo é válido
        if (grafo == NULL || grafo->lista == NULL) {
            return ERRO_MEMORIA_INSUFICIENTE;
        }
        // Verifica se a origem está no escopo do grafo
        if (origem < 0 || origem >= grafo->V) {
            return ERRO_VERTICE_INVALIDO;
        }

        // Vetores locais
        int *fila = (int*) malloc(grafo->V * sizeof(int));
        // Calloc garante que o vetor seja iniciado com todas as posições zeradas
        int *visitado = (int*) calloc(grafo->V, sizeof(int));
        int *distancia = (int*) malloc(grafo->V * sizeof(int));

        if (fila == NULL || visitado == NULL || distancia == NULL) {
            free(fila);
            free(visitado);
            free(distancia);
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        int inicio = 0;
        int fim = 0;

        // Inicializa o vetor de distância
        for (int i = 0; i < grafo->V; i++) {
            distancia[i] = -1; // -1 indica que o vértice ainda é inacessível
        }

        // Inicializa a estrutura com o vértice de partida
        fila[fim++] = origem;
        visitado[origem] = 1;
        distancia[origem] = 0;

        printf("\n=== BUSCA EM LARGURA (Origem: %d) ===\n", origem);
        printf("Ordem: ");

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

        free(fila);
        free(visitado);
        free(distancia);
        return GRAFO_OK;
    }

    // Ordenação Topologica do Grafo (Variação do Algoritmo de Kahn)
    // Obs.: funciona apenas para DAGs (Grafos Acíclicos Direcionados)
    GrafoStatus ordemTopologica(Grafo *grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL || grafo->lista == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

        // Verifica se o grafo é direcionado (se for, não é um DAG)
        if (!grafo->direcionado) {
            return ERRO_GRAFO_NAO_DIRECIONADO;
        }

        int V = grafo->V; //Número de vértices

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
    // Obs.: para grafos desconexos ela fornece uma floresta geradora mínima (uma AGM por componente)
    GrafoStatus algoritmoPrim(Grafo* g) {
        // Verifica se o grafo é válido
        if (g == NULL || g->lista == NULL)
            return ERRO_MEMORIA_INSUFICIENTE;

        // Verifica se o grafo é direcionado
        if (g->direcionado) {
            return ERRO_GRAFO_DIRECIONADO;
        }

        int V = g->V;

        // Alocação dos vetores auxiliares para construção da AGM
        int* pai = (int*)malloc(V * sizeof(int));   // Armazena a aresta que conecta o vértice à árvore
        int* chave = (int*)malloc(V * sizeof(int)); // Guarda o menor peso de aresta para conectar o vértice à AGM
        int* naAGM = (int*)calloc(V, sizeof(int));  // Mapeia se o vértice já foi incluído na árvore (0 = não, 1 = sim)

        // Verifica se a alocação deu erro
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

    //--- Funções extras ---

    // Busca e retorna o peso de uma aresta que vai do vértice1 ao vértice2
    int getPesoAresta(Grafo* grafo, int vertice1, int vertice2) {
        No* atual = grafo->lista[vertice1];
        while (atual != NULL) {
            if (atual->destino == vertice2) {
                return atual->peso;
            }
            atual = atual->prox;
        }

        // Se a aresta não existe, retorna -1
        return -1;
    }

    // Imprime a distância e o percurso do caminho crítico (maior caminho) de um DAG
    GrafoStatus caminhoCritico (Grafo *grafo) {
        // Verifica se o grafo é válido
        if (grafo == NULL || grafo->V == 0) {
            return ERRO_MEMORIA_INSUFICIENTE;
        }
        // Verifica se o grafo é direcionado
        if (!grafo->direcionado) {
            return ERRO_GRAFO_NAO_DIRECIONADO;
        }

        //Primeira Parte: Verificar se é um DAG
        No* atual;
        int *zeroSetasApontando = (int*) calloc(grafo->V, sizeof(int));
        int *setasApontando = (int*) calloc(grafo->V, sizeof(int));
        int inicio = 0;
        int fim = 0;
        int vertices = 0;
        int temp = 0;
        int zeros = 0;

        //Segunda Parte : Achar o maior caminho
        int *maiorCaminho = (int*) calloc(grafo->V, sizeof(int));
        int *pais = (int*) malloc(grafo->V*sizeof(int));
        int *caminho = (int *) malloc(grafo->V*sizeof(int));
        int peso = 0;
        int maiorDistancia = 0;
        int ultimoVertice = 0;
        int verticeAtual;
        int indiceCaminho = 0;

        //Verificando se algumas das alocações deu erruo
        if (maiorCaminho == NULL || pais == NULL || caminho == NULL || zeroSetasApontando == NULL || setasApontando == NULL) {
            free(zeroSetasApontando);
            free(setasApontando);
            free(maiorCaminho);
            free(pais);
            free(caminho);
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        // Inicializando o vetor de pais
        for (int i = 0; i < grafo->V; i++) {
            pais[i] = -1;
        }

        // Calculando o grau de entrada dos vértices
        for (int i = 0; i < grafo->V; i++) {
            atual = grafo->lista[i];
            while (atual != NULL) {
                setasApontando[atual->destino]++;
                atual = atual->prox;
            }
        }

        // Inserindo vértices sem predecessores
        for (int i = 0; i < grafo->V; i ++) {
            if (setasApontando[i] == 0) {
                zeroSetasApontando[fim] = i;
                fim++;
            }
        }

        // Algoritmo de Kahn (Ordenação Topológica) adaptado
        while (inicio < fim) {
            // Pega o primeiro vértice sem predecessor da fila
            zeros = zeroSetasApontando[inicio];
            vertices++;

            // Diminui o grau de entrada dos vértices que eram sucessores dele
            atual = grafo->lista[zeros];
            while (atual != NULL) {
                temp = atual->destino;
                setasApontando[temp]--;

                // Se o vértice atual não possui mais vértices predecessores
                // Adiciona ele na fila
                if (setasApontando[temp] == 0) {
                    zeroSetasApontando[fim] = temp;
                    fim++;
                }

                atual = atual->prox;
            }
            inicio++;
        }

        // Libera vetor que não será mais utilizado
        free(setasApontando);

        // Se foram processados mais vértices do que o grafo possui
        // Então o grafo contém ciclo
        if (vertices < grafo->V) {
            free(zeroSetasApontando);
            free(maiorCaminho);
            free(pais);
            free(caminho);
            return ERRO_GRAFO_COM_CICLOS; // ciclico;
        }

        // Percorrendo a ordem topológica
        for (int i = 0; i < grafo->V; i++) {
            zeros = zeroSetasApontando[i];
            atual = grafo->lista[zeros];
            while(atual != NULL) {
                temp = atual->destino;
                peso = atual->peso;

                // Se passar pelo vértice atual a distância do maior caminho é maior que a distância atual
                if (maiorCaminho[zeros] + peso > maiorCaminho[temp]) {
                    maiorCaminho[temp] = maiorCaminho[zeros] + peso;
                    // Se sim, guarda no vetor pais para imprimir o percurso
                    pais[temp] = zeros;
                }

                atual = atual->prox;
            }
        }

        // Libera vetor que não será mais utilizado
        free(zeroSetasApontando);

        // Procura o maior caminho do vetor
        maiorDistancia = maiorCaminho[0];
        for (int i = 0; i < grafo->V; i++) {
            if (maiorCaminho[i] > maiorDistancia) {
                maiorDistancia = maiorCaminho[i];
                ultimoVertice = i;
            }
        }

        // Libera vetor que não será mais utilizado
        free(maiorCaminho);

        // Reconstrói o percuso no vetor caminho
        verticeAtual = ultimoVertice;

        while (verticeAtual != -1) { //vai do ultimo vertice ate a origem
            caminho[indiceCaminho++] = verticeAtual;
            verticeAtual = pais[verticeAtual];
        }

        printf("\n=== CAMINHO CRITICO ===\n");
        printf("A maior distancia que pode ser percorrida nesse DAG é: %d\n", maiorDistancia);

        printf("Caminho Critico: ");

        //imprime o caminho de tras para frente para ficar o caminho do grafo original
        for (int i = indiceCaminho - 1;i >= 0; i--) {
            printf("%d", caminho[i]);

            if (i > 0) {
                printf(" -(%d)-> ", getPesoAresta(grafo, caminho[i], caminho[i-1]));
            }

        }

        free(pais);
        free(caminho);
        return GRAFO_OK;
    }

    // Realiza uma DFS e registra a ordem de término dos vértices na pilha.
    void DFSPilha(Grafo *grafo, int indice, int *visitados, int *pilha, int *topo){
        // Verifica se o grafo é válido
        if (grafo == NULL) {
            return;
        }

        int destino;
        visitados[indice] = 1;

        // Percorre todos os vértices adjacentes
        No* atual = grafo->lista[indice];
        while (atual != NULL) {
            destino = atual->destino;
            if (!visitados[destino]) {
                DFSPilha(grafo, destino,visitados, pilha, topo);
            }

            atual = atual->prox;
        }

        // Empilha o vértice após visitar todos os seus descendentes
        pilha[++(*topo)] = indice;
    }

    // Cria o grafo transposto, invertendo a direção de todas as arestas.
    Grafo* criarGrafoTransposto(Grafo *grafo) {
        // Verifica se o grafo é valido
        if (grafo == NULL || grafo->V == 0) {
            return NULL;
        }
        // Cria e aloca memória para o novo gráfico
        Grafo* grafoTransposto = criarGrafo(grafo->V, grafo->A, grafo->direcionado);
        if (grafoTransposto == NULL) {
            return NULL;
        }

        No *atual;
        for (int i = 0; i < grafo->V;i++) {
            // Para cada aresta que vai de i para atual->destino
            atual = grafo->lista[i];
            while (atual != NULL) {
                // Adiciona uma aresta que vai de atual->destino para o vértice i (aresta inversa)
                adicionarAresta(grafoTransposto, atual->destino,i, atual->peso);
                atual = atual->prox;
            }
        }

        return grafoTransposto;
    }

    // Identifica e imprime os componentes fortemente conexos de um grafo direcionado
    GrafoStatus algoritmoKosaraju(Grafo *grafo) {
        // Verifica se o grafo informado é válido
        if (grafo == NULL) {
            return ERRO_MEMORIA_INSUFICIENTE;
        }
        // Verifica se o grafo é direcionado
        if (!grafo->direcionado)
            return ERRO_GRAFO_NAO_DIRECIONADO;

        int numeroComponentes = 0;
        int vertice;

        // Vetor utilizado para marcar os vértices já visitados
        int* visitados = (int *) calloc (grafo->V, sizeof(int));

        // Pilha que vai armazenar os vértices em ordem decrescente de finalização da DFS
        int* pilha = (int*) malloc(grafo->V * sizeof(int));

        // Verifica se a alocação falhou em algum vetor
        if (visitados == NULL || pilha == NULL) {
            free(visitados);
            free(pilha);
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        int topo = -1;

        // Obtém a ordem de término da DFS na pilha
        for (int i = 0; i < grafo->V; i++) {
            if (!visitados[i]) {
                DFSPilha(grafo, i, visitados, pilha, &topo);
            }
        }

        // Reinicia os vetor de controle dos visitados para próxima DFS
        for (int i = 0; i < grafo->V; i++) {
            visitados[i] = 0;
        }

        // Cria o grafo com todas as arestas invertidas
        Grafo* grafoTransposto = criarGrafoTransposto(grafo);
        if (grafoTransposto == NULL) {
            free(visitados);
            free(pilha);
            return ERRO_MEMORIA_INSUFICIENTE;
        }

        printf("\n=== COMPONENTES FORTEMENTE CONEXOS (KOSARAJU) ===\n\n");

        // Processa o vértice na ordem correta
        while (topo >= 0) {
            vertice = pilha[topo--];

            // Imprime cada componente fortemente conexo do grafo transposto
            if (!visitados[vertice]){
                numeroComponentes++;

                printf("Componente de %d: ", numeroComponentes);

                DFSRecursiva(grafoTransposto, vertice, visitados);
                printf("\n");
            }
        }

        // Libera a memória alocada dinamicamente
        free(visitados);
        free(pilha);
        liberarGrafo(grafoTransposto);
        return GRAFO_OK;
    }

