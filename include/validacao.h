#ifndef GRAFOS_VALIDACAO_H
#define GRAFOS_VALIDACAO_H
    //typedef para o enum de validacao
    typedef enum {
        GRAFO_OK,
        ERRO_ARQUIVO_NAO_ENCONTRADO,
        ERRO_FORMATO_INVALIDO,
        ERRO_NUMERO_INVALIDO,
        ERRO_ARESTA_INVALIDA,
        ERRO_VERTICE_INVALIDO,
        ERRO_MEMORIA_INSUFICIENTE,
        ERRO_GRAFO_DIRECIONADO,
        ERRO_GRAFO_NAO_DIRECIONADO,
        ERRO_GRAFO_COM_CICLOS,
    } GrafoStatus;

void imprimirMensagemGrafo(GrafoStatus status);

#endif //GRAFOS_VALIDACAO_H
