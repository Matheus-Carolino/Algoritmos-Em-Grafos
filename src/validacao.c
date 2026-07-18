#include <stdio.h>
#include <stdlib.h>

#include "validacao.h"
    void imprimirMensagemGrafo(GrafoStatus status) {
        switch (status) {
            case GRAFO_OK:
                printf("Grafo carregado com sucesso!\n");
                break;
            case ERRO_ARQUIVO_NAO_ENCONTRADO:
                printf("Erro: O arquivo não foi encontrado!\n");
                break;
            case ERRO_FORMATO_INVALIDO:
                printf("Erro: Formato de arquivo invalido!\n");
                break;
            case ERRO_NUMERO_INVALIDO:
                printf("Erro: Número vertices e/ou arestas deve ser maior que zero!\n");
                break;
            case ERRO_ARESTA_INVALIDA:
                printf("Erro: Uma ou mais arestas possuem vertices invalidos!\n");
                break;
            case ERRO_MEMORIA_INSUFICIENTE:
                printf("Erro: Falha ao alocar memória para o grafo!\n");
                break;
            default:
                printf("Erro: Desconhecido!");
                break;
        }
    }