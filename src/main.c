#include <stdio.h>
#include "validacao.h"
#include "grafo.h"

int main(void) {
    printf("Teste de Leitura do arquivo grafo\n");
    Grafo *grafo;
    GrafoStatus status;

    status = lerGrafoDeArquivo("../data/grafo.txt", &grafo);
    imprimirMensagemGrafo(status);
    liberarGrafo(grafo);
    return 0;
}
