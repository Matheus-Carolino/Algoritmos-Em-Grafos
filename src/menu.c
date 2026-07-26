#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "grafo.h"
#include "algoritmos.h"
#include "estatisticas.h"
#include "teste.h"

    //--- Funções auxiliares do menu ---

    // Carrega um grafo de arquivo, substituindo o grafo atualmente em memória (se houver)
    void carregarGrafoDeArquivo(Grafo** grafo) {
        char op;
        printf("\nO grafo a ser carregado é direcionado? (S | N)\n");
        scanf(" %c", &op); // O espaço ignora caracteres de espaço em branco
        printf("\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

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
        if (fgets(nomeArquivo, sizeof(nomeArquivo), stdin) != NULL) {
            // Verifica se o '\n' está na string
            char *pos = strchr(nomeArquivo, '\n');

            if (pos != NULL) {
                // Se encontrou o \n, a linha inteira coube no buffer e remove o \n
                *pos = '\0';
            } else {
                // Se não encontrou o \n, o texto era maior que 100 caracteres.
                // Precisamos descartar o resto da linha para não poluir a próxima leitura!
                while ((c = getchar()) != '\n' && c != EOF);
            }
        }

        //Evita lixo de memória residual
        if (*grafo != NULL) {
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

        // Verifica se o vértice está no escopo
        if (inicio < 0 || inicio >= grafo->V) {
            printf("Erro: vértice inválido!\n");
            return;
        }

        // Inicializa vértice de controle com todas as posições zeradas
        int *visitado = (int*)calloc(grafo->V, sizeof(int));
        if (visitado == NULL) {
            printf("Erro: Falha ao alocar memória!\n");
            return;
        }
        
        //escolha da implementação recursiva ou iterativa
        char op;
        printf("\nQual implementação de DFS você deseja usar? (R | I)\n");
        scanf(" %c", &op); // O espaço ignora caracteres de espaço em branco
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
        } else
            printf("Opção inválida!");

        free(visitado);
    }

    // Executa a Busca em Largura a partir de um vértice escolhido pelo usuário
    void executarBuscaEmLargura(Grafo* grafo) {
        int inicio;
        printf("\nDigite o vértice de início para a BFS (0 a %d): ", grafo->V-1);
        scanf("%d", &inicio);

        // Verifica se o vértice está no escopo
        if (inicio < 0 || inicio >= grafo->V) {
            printf("Erro: vértice inválido!\n");
            return;
        }

        GrafoStatus status = BFS(grafo, inicio);
        if (status != GRAFO_OK)
            imprimirMensagemGrafo(status);
    }

    // Executa o algoritmo de Dijkstra a partir de um vértice de origem escolhido pelo usuário
    void executarDijkstra(Grafo* grafo) {
        int origem;
        printf("\nDigite o vertice de origem para o Algoritmo de Dijkstra (0 a %d): ", grafo->V-1);
        scanf("%d", &origem);

        // Verifica se o vértice está no escopo
        if (origem < 0 || origem >= grafo->V) {
            printf("Erro: vértice inválido!\n");
            return;
        }

        GrafoStatus status = algoritmoDijkstra(grafo, origem);
        if (status != GRAFO_OK)
            imprimirMensagemGrafo(status);
    }

    // Executa a identificação de componentes do grafo.
    void executarComponentes(Grafo *grafo) {
        // Para grafos não direcionados, identifica os componentes conexos utilizando DFS.
        // Para grafos direcionados, identifica os componentes fortemente conexos utilizando o algoritmo de Kosaraju.
        GrafoStatus status = grafo->direcionado? algoritmoKosaraju(grafo) : DFSCompConexos(grafo, 0);

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

        Grafo* grafo = NULL;
        GrafoStatus status;
        int opcao;

        do {
            // Imprime o menu com a moldura corretamente
            printf(
                "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                "┃           SISTEMA DE GRAFOS            ┃\n"
                "┃  1- Carregar grafo de arquivo          ┃\n"
                "┃  2- Mostrar grafo (lista adjacencia)   ┃\n"
                "┃  3- Busca em Profundidade (DFS)        ┃\n"
                "┃  4- Busca em Largura (BFS)             ┃\n"
                "┃  5- Ordenacao Topologica               ┃\n"
                "┃  6- Arvore Geradora Minima (Prim)      ┃\n"
                "┃  7- Menor Caminho (Dijkstra)           ┃\n"
                "┃  8- Estatisticas do grafo              ┃\n"
                "┃  9- Componentes (Kosaraju/DFS)         ┃\n"
                "┃  10- Caminho Critico                   ┃\n"
                "┃  11- Executar Bateria de testes        ┃\n"
                "┃  12- Sair                              ┃\n"
                "┃                                        ┃\n"
                "┃ [PROJETO] :: TEAM 6                    ┃\n"
                "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
            );

            printf("\nEscolha uma opção: ");
            scanf("%d", &opcao);

            // Se nenhum grafo foi carregado ainda
            if (opcao >= 2 && opcao <= 11 && grafo == NULL) {
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
                    executarComponentes(grafo);
                    break;
                case 10:
                    status = caminhoCritico(grafo);
                    if (status != GRAFO_OK)
                        imprimirMensagemGrafo(status);
                    break;
                case 11:
                    printf("\n");
                    bateriaDeTestes(grafo);
                    break;
                case 12:
                    exibirEncerramento();
                    break;
                default:
                    printf("\nOpção inválida!\n");
            }

            if (opcao != 12) {
                printf("\n");
                PAUSE_SCREEN();
                CLEAR_SCREEN();
            }

        } while (opcao != 12);

        liberarGrafo(grafo);
    }
