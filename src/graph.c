#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

Grafo* crear_grafo(int num_vertices, int ancho, int alto) {
    Grafo* g = malloc(sizeof(Grafo));
    if (!g) return NULL;

    g->num_vertices = num_vertices;
    g->ancho = ancho;
    g->alto = alto;

    g->lista = calloc(num_vertices, sizeof(NodoAdyecencia*));
    if (!g->lista) {
        free(g);
        return NULL;
    }
    return g;
}

