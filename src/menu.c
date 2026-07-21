#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "menu.h"
#include "grafo.h"

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

    //--- Função do Menu ---
    void exibirMenu() {
        CONFIG_TERMINAL();
        CLEAR_SCREEN();

        int opcao = 0;
        Grafo* grafo = NULL;
        GrafoStatus status;
        char nomeArquivo[100], op;
        int origem, alvo, inicio;
        int direcionado;

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

        do {
            print_upperwindow(41);
            display(m1); display(m2); display(m3); display(m4); display(m5);
            display(m6); display(m7); display(m8); display(m9); display(m10);
            display(cr);
            print_lowerwindow(41);

            printf("\nEscolha uma opcao: ");
            scanf("%d", &opcao);

            if (opcao >= 2 && opcao <= 8 && grafo == NULL) {
                printf("\n[ERRO] Nenhum grafo carregado! Escolha a Opcao 1 primeiro.\n\n");
                PAUSE_SCREEN();
                CLEAR_SCREEN();
                continue;
            }

            switch (opcao) {
                case 1:
                    printf("O grafo a ser carregado é direcionado? (S | N)\n");
                    scanf(" %c", &op); // O espaço limpa o buffer para evitar bugs
                    printf("\n");

                    if (op == 'N' || op == 'n') {
                        direcionado = 0;
                    } else if (op == 'S' || op == 's') {
                        direcionado = 1;
                    } else {
                        printf("Opção inválida!");
                        break;
                    }

                    printf("Digite o caminho do arquivo (ex: ../data/grafo1.txt): ");
                    scanf("%99s", nomeArquivo);
                    //Evita lixo de memória residual
                    if (grafo != NULL) {
                        liberarGrafo(grafo);
                    }
                    //Faz a leitura do arquivo, retorna o Status e atualiza o grafo
                    status = lerGrafoDeArquivo(nomeArquivo, &grafo, direcionado);
                    imprimirMensagemGrafo(status);
                    break;

                case 2:
                    mostrarGrafo(grafo);
                    break;
                case 3: {
                    printf("\nDigite o vertice de inicio para a DFS (0 a %d): ", grafo->V-1);
                    scanf("%d", &origem);

                    if (origem < 0 || origem >= grafo->V) {
                        printf("Erro: vertice inválido\n");
                        break;
                    }

                    int visitado[grafo->V];
                    for (int i=0; i<grafo->V; i++)
                        visitado[i] = 0;

                    //escolha da implementação recursiva ou iterativa
                    printf("\nQual implementação de DFS você deseja usar? (R | I)\n");
                    scanf(" %c", &op); // O espaço limpa o buffer para evitar bugs
                    printf("\n");

                    //evita problemas com Case
                    if (op == 'R' || op == 'r') {
                        printf("=== BUSCA EM PROFUNDIDADE RECURSIVA (Origem %d) ===\n", origem);
                        printf("Ordem: ");
                        status = DFSRecursiva(grafo, origem, visitado);
                        //imprimirMensagemGrafo(status);
                        if (!grafo->direcionado)
                            DFSRecCompConexos(grafo);
                        else
                            Kosaraju(grafo);
                    } else if (op == 'I' || op == 'i') {
                        printf("=== BUSCA EM PROFUNDIDADE ITERATIVA (Origem %d) ===\n", origem);
                        printf("Ordem: ");
                        status = DFSIterativa(grafo, origem, visitado);
                        //imprimirMensagemGrafo(status);
                        if (!grafo->direcionado)
                            DFSIteCompConexos(grafo);
                        else
                            Kosaraju(grafo);
                    } else
                        printf("Opção inválida!");

                    break;
                }

                case 4: {
                    printf("\nDigite o vertice de inicio para a BFS (0 a %d): ", grafo->V-1);
                    scanf("%d", &inicio);

                    if (inicio < 0 || inicio >= grafo->V) {
                        printf("Erro: vertice invalido\n");
                        break;
                    }

                    status = BFS(grafo, inicio);
                    imprimirMensagemGrafo(status);
                    break;
                }

                case 5:
                    printf("\n");
                    status = ordemTopologica(grafo);
                    imprimirMensagemGrafo(status);
                    break;

                case 6:
                    printf("\n");
                    status = algoritmoPrim(grafo);
                    imprimirMensagemGrafo(status);
                    break;

                case 7:
                    printf("\nDigite o vertice de ORIGEM: ");
                    scanf("%d", &origem);

                    status = algoritmoDijkstra(grafo, origem);
                    imprimirMensagemGrafo(status);
                    break;
                case 8:
                    printf("\n");
                    exibirEstatisticas(grafo);
                    break;

                case 9:
                    printf("\nEncerrando o sistema. Ate a proxima!\n");
                    printf("  _______ ______          __  __    __   \n");
                    printf(" |__   __|  ____|   /\\   |  \\/  |  / /   \n");
                    printf("    | |  | |__     /  \\  | \\  / | / /_   \n");
                    printf("    | |  |  __|   / /\\ \\ | |\\/| || '_ \\  \n");
                    printf("    | |  | |____ / ____ \\| |  | || (_) | \n");
                    printf("    |_|  |______/_/    \\_\\_|  |_| \\___/  \n");
                    #ifndef _WIN32
                        printf("\033[0m");
                        CLEAR_SCREEN();
                    #endif
                    break;

                default: printf("\nOpcao invalida!\n");
            }

            if (opcao != 9) {
                printf("\n");
                PAUSE_SCREEN();
                CLEAR_SCREEN();
            }
        } while (opcao != 9);

        if (grafo != NULL) {
            liberarGrafo(grafo);
        }
    }