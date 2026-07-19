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
            SLEEP_MS(1);
        }
        printf("┃\n");
    }

    void print_upperwindow(int n) {
        for (int i = 0; i < n; i++) {
            if (i == 0) printf("┏");
            else if (i == n - 1) printf("┓");
            else printf("━");
            SLEEP_MS(1);
        }
        printf("\n");
    }

    void print_lowerwindow(int n) {
        for (int i = 0; i < n; i++) {
            if (i == 0) printf("┗");
            else if (i == n - 1) printf("┛");
            else printf("━");
            SLEEP_MS(1);
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
        char nomeArquivo[100];
        int origem, alvo, inicio;

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
                    printf("Digite o caminho do arquivo (ex: ../data/grafo1.txt): ");
                    scanf("%s", nomeArquivo);
                    //Evita lixo de memória residual
                    if (grafo != NULL) {
                        liberarGrafo(grafo);
                    }
                    //Faz a leitura do arquivo, retorna o Status e atualiza o grafo
                    status = lerGrafoDeArquivo(nomeArquivo, &grafo);
                    imprimirMensagemGrafo(status);
                    break;

                case 2: printf("\n[Mock] Exibindo Grafo...\n"); break;

                case 3:
                    char op;
                    int visitado[] = {0};

                    printf("\nDigite o vertice de inicio para a DFS (0 a 5): ");
                    scanf("%d", &inicio);
                    //escolha da implementação recursiva ou iterativa
                    printf("\nQual implementação de DFS você deseja usar? (R | I)\n");
                    scanf(" %c", &op); // O espaço limpa o buffer para evitar bugs

                    //evita problemas com Case
                    if (op == 'R' || op == 'r') {
                        status = DFSRecursiva(grafo, origem, visitado);
                        imprimirMensagemGrafo(status);
                    } else if (op == 'I' || op == 'i') {
                        status = DFSIterativa(grafo, origem, visitado);
                        imprimirMensagemGrafo(status);
                    } else
                        printf("Opção inválida!");
                    break;

                case 4:
                    printf("\nDigite o vertice de inicio para a BFS (0 a 5): ");
                    scanf("%d", &inicio);

                    status = BFS(grafo, inicio);
                    imprimirMensagemGrafo(status);
                    break;

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