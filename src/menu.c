#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "grafo.h"
#include "algoritmos.h"
#include "estatisticas.h"

    //--- Funções de Renderização da Janela ---
    void display(char *string) {
        printf("┃");
        while (*string) {
            printf("%c", *string);
            string++;
            //SLEEP_MS(1);
        }
        printf("┃\n");
    }

    void print_upperwindow(int n) {
        for (int i = 0; i < n; i++) {
            if (i == 0) printf("┏");
            else if (i == n - 1) printf("┓");
            else printf("━");
            //SLEEP_MS(1);
        }
        printf("\n");
    }

    void print_lowerwindow(int n) {
        for (int i = 0; i < n; i++) {
            if (i == 0) printf("┗");
            else if (i == n - 1) printf("┛");
            else printf("━");
            //SLEEP_MS(1);
        }
        printf("\n");
    }

    //--- Funções auxiliares do menu ---

    // Carrega um grafo de arquivo, substituindo o grafo atualmente em memória (se houver)
    void carregarGrafoDeArquivo(Grafo** grafo) {
        char op;
        printf("\nO grafo a ser carregado é direcionado? (S | N)\n");
        scanf(" %c", &op); // O espaço limpa o buffer para evitar bugs
        printf("\n");

        int direcionado;
        if (op == 'N' || op == 'n') {
            direcionado = 0;
        } else if (op == 'S' || op == 's') {
            direcionado = 1;
        } else {
            printf("Opção inválida!");
            return;
        }

        char nomeArquivo[100];
        printf("Digite o caminho do arquivo (ex: ../data/grafo1.txt): ");
        scanf("%99s", nomeArquivo); // Evita buffer overflow

        //Evita lixo de memória residual
        if (grafo != NULL) {
            liberarGrafo(*grafo);
        }

        //Faz a leitura do arquivo, retorna o Status e atualiza o grafo
        GrafoStatus status = lerGrafoDeArquivo(nomeArquivo, grafo, direcionado);
        imprimirMensagemGrafo(status);
    }

    // Executa a Busca em Profundidade (recursiva ou iterativa)
    void executarBuscaEmProfundidade(Grafo* grafo) {
        int inicio;

        printf("\nDigite o vértice de início para a DFS (0 a %d): ", grafo->V-1);
        scanf("%d", &inicio);

        // Verifica se o vértice informado pertence ao escopo do grafo
        if (inicio < 0 || inicio >= grafo->V) {
            printf("Erro: vértice inválido.\n");
            return;
        }

        int *visitado = (int*)calloc(grafo->V, sizeof(int));
        if (visitado == NULL) {
            printf("Erro: memória insuficiente.\n");
            return;
        }

        //escolha da implementação recursiva ou iterativa
        char op;
        printf("\nQual implementação de DFS você deseja usar? (R | I)\n");
        scanf(" %c", &op); // O espaço limpa o buffer para evitar bugs
        printf("\n");

        //evita problemas com Case
        GrafoStatus status;
        if (op == 'R' || op == 'r') {
            printf("=== BUSCA EM PROFUNDIDADE RECURSIVA (Origem %d) ===\n", inicio);
            printf("Ordem: ");
            status = DFSRecursiva(grafo, inicio, visitado);
            printf("\n");

            if (status != GRAFO_OK) {
                imprimirMensagemGrafo(status);
                free(visitado);
                return;
            }

            if (!grafo->direcionado)
                DFSRecCompConexos(grafo);
            else
                algoritmoKosaraju(grafo);

        } else if (op == 'I' || op == 'i') {
            printf("=== BUSCA EM PROFUNDIDADE ITERATIVA (Origem %d) ===\n", inicio);
            printf("Ordem: ");
            status = DFSIterativa(grafo, inicio, visitado);
            printf("\n");

            if (status != GRAFO_OK) {
                imprimirMensagemGrafo(status);
                free(visitado);
                return;
            }

            if (!grafo->direcionado)
                DFSIteCompConexos(grafo);
            else
                algoritmoKosaraju(grafo);

        } else
            printf("Opção inválida!");

        free(visitado);
    }

    // Executa a Busca em Largura a partir de um vértice escolhido pelo usuário
    void executarBuscaEmLargura(Grafo* grafo) {
        int origem;
        printf("\nDigite o vértice de início para a BFS (0 a %d): ", grafo->V-1);
        scanf("%d", &origem);

        // Verifica se o vértice informado pertence ao escopo do grafo
        if (origem < 0 || origem >= grafo->V) {
            printf("Erro: Vértice inválido!\n");
            return;
        }

        GrafoStatus status = BFS(grafo, origem);
        if (status != GRAFO_OK)
            imprimirMensagemGrafo(status);
    }

    // Executa o algoritmo de Dijkstra a partir de um vértice de origem escolhido pelo usuário
    void executarDijkstra(Grafo* grafo) {
        int origem;
        printf("\nDigite o vertice de ORIGEM: ");
        scanf("%d", &origem);

        // Verifica se o vértice informado pertence ao escopo do grafo
        if (origem < 0 || origem >= grafo->V) {
            printf("Erro: Vértice inválido!\n");
            return;
        }

        GrafoStatus status = algoritmoDijkstra(grafo, origem);
        if (status != GRAFO_OK)
            imprimirMensagemGrafo(status);
    }

    void exibirEncerramento() {
        printf("\nEncerrando o sistema. Até a próxima!\n");
        printf("  _______ ______          __  __    __   \n");
        printf(" |__   __|  ____|   /\\   |  \\/  |  / /   \n");
        printf("    | |  | |__     /  \\  | \\  / | / /_   \n");
        printf("    | |  |  __|   / /\\ \\ | |\\/| || '_ \\  \n");
        printf("    | |  | |____ / ____ \\| |  | || (_) | \n");
        printf("    |_|  |______/_/    \\_\\_|  |_| \\___/  \n\n");
        #ifndef _WIN32
                printf("\033[0m");
                CLEAR_SCREEN();
        #endif
    }

    //--- Função do Menu ---
    void exibirMenu() {
        CONFIG_TERMINAL();
        CLEAR_SCREEN();

        // Strings limpas e sem as barras nas pontas para alinhar perfeitamente com a função display
        char m1[] = "           SISTEMA DE GRAFOS           ";
        char m2[] = "  1- Carregar grafo de arquivo         ";
        char m3[] = "  2- Mostrar grafo (lista adjacencia)  ";
        char m4[] = "  3- Busca em Profundidade (DFS)       ";
        char m5[] = "  4- Busca em Largura (BFS)            ";
        char m6[] = "  5- Ordenacao Topologica              ";
        char m7[] = "  6- Arvore Geradora Minima (Prim)     ";
        char m8[] = "  7- Menor Caminho (Dijkstra)          ";
        char m9[] = "  8- Estatisticas do grafo             ";
        char m10[] = "  9- Sair                              ";
        char cr[] = " [PROJETO] :: TEAM 6                   ";

        Grafo* grafo = NULL;
        GrafoStatus status;
        int opcao;

        do {
            // Imprime o menu com a moldura corretamente
            print_upperwindow(41);
            display(m1); display(m2); display(m3); display(m4); display(m5);
            display(m6); display(m7); display(m8); display(m9); display(m10);
            display(cr);
            print_lowerwindow(41);

            printf("\nEscolha uma opção: ");
            scanf("%d", &opcao);

            // Se nenhum grafo foi carregado ainda
            if (opcao >= 2 && opcao <= 8 && grafo == NULL) {
                printf("\nErro: Nenhum grafo carregado! Escolha a opção 1 primeiro.\n\n");
                PAUSE_SCREEN();
                CLEAR_SCREEN();
                continue;
            }

            switch (opcao) {
                case 1:
                    carregarGrafoDeArquivo(&grafo);
                    break;
                case 2:
                    mostrarGrafo(grafo);
                    break;
                case 3: {
                    executarBuscaEmProfundidade(grafo);
                    break;
                }
                case 4: {
                    executarBuscaEmLargura(grafo);
                    break;
                }
                case 5:
                    status = ordemTopologica(grafo);
                    if (status != GRAFO_OK)
                        imprimirMensagemGrafo(status);
                    break;
                case 6:
                    status = algoritmoPrim(grafo);
                    if (status != GRAFO_OK)
                        imprimirMensagemGrafo(status);
                    break;
                case 7:
                    executarDijkstra(grafo);
                    break;
                case 8:
                    printf("\n");
                    exibirEstatisticas(grafo);
                    break;
                case 9:
                    exibirEncerramento();
                    break;
                default:
                    printf("\nOpção inválida!\n");
            }

            if (opcao != 9) {
                printf("\n");
                PAUSE_SCREEN();
                CLEAR_SCREEN();
            }

        } while (opcao != 9);

        liberarGrafo(grafo);
    }
