#ifndef GRAFOS_MENU_H
#define GRAFOS_MENU_H

    #include "grafo.h"

    // Tratamento multiplataforma para Windows e Linux
    #ifdef _WIN32
        #include <windows.h>
        #define CLEAR_SCREEN() system("cls")
        #define PAUSE_SCREEN() system("pause")
        #define CONFIG_TERMINAL() { SetConsoleOutputCP(CP_UTF8); system("color FF"); }
    #else
        #include <unistd.h>
        #define CLEAR_SCREEN() printf("\033[H\033[2J")
        #define PAUSE_SCREEN() { printf("Pressione ENTER para continuar..."); getchar(); getchar(); }
        #define CONFIG_TERMINAL() printf("\033[37;44m")
    #endif

    //--- Funções públicas do menu ---
    void exibirMenu();

#endif //GRAFOS_MENU_H
