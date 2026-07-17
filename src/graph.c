// ============================================================
// graph.c
// Implementacion del grafo con lista de adyacencia.
// ============================================================

#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

Grafo* crear_grafo(int num_vertices, int ancho, int alto) {
    Grafo* g = malloc(sizeof(Grafo));
    if (!g) return NULL;

    g->num_vertices = num_vertices;
    g->ancho = ancho;
    g->alto  = alto;

    g->lista = calloc(num_vertices, sizeof(NodoAdyacencia*));
    if (!g->lista) {
        free(g);
        return NULL;
    }
    return g;
}

void agregar_arista(Grafo* g, int u, int v) {
  
    NodoAdyacencia* nodo_uv = malloc(sizeof(NodoAdyacencia));
    if (!nodo_uv) return;
    nodo_uv->vertice   = v;
    nodo_uv->siguiente = g->lista[u];
    g->lista[u] = nodo_uv;


    NodoAdyacencia* nodo_vu = malloc(sizeof(NodoAdyacencia));
    if (!nodo_vu) return;
    nodo_vu->vertice   = u;
    nodo_vu->siguiente = g->lista[v];
    g->lista[v] = nodo_vu;
}

void liberar_grafo(Grafo* g) {
    if (!g) return;
    for (int i = 0; i < g->num_vertices; i++) {
        NodoAdyacencia* actual = g->lista[i];
        while (actual) {
            NodoAdyacencia* siguiente = actual->siguiente;
            free(actual);
            actual = siguiente;
        }
    }
    free(g->lista);
    free(g);
}