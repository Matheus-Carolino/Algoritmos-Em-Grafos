#ifndef GRAFOS_MENU_H
#define GRAFOS_MENU_H

    #include "grafo.h"

    // Tratamento multiplataforma para o Sleep e Limpeza de Tela
    #ifdef _WIN32
        #include <windows.h>
        #define CLEAR_SCREEN() system("cls")
        #define PAUSE_SCREEN() system("pause")
        #define SLEEP_MS(ms) Sleep(ms)
        #define CONFIG_TERMINAL() { SetConsoleOutputCP(CP_UTF8); system("color FF"); }
    #else
        #include <unistd.h>
        #define CLEAR_SCREEN() printf("\033[H\033[2J")
        #define PAUSE_SCREEN() { printf("Pressione ENTER para continuar..."); getchar(); getchar(); }
        #define SLEEP_MS(ms) usleep((ms) * 1000)
        #define CONFIG_TERMINAL() printf("\033[37;44m")
    #endif

    //Assinatura das funções de exibir o menu
    void display(char *string);
    void print_upperwindow(int n);
    void print_lowerwindow(int n);

    void exibirMenu();

#endif //GRAFOS_MENU_H
