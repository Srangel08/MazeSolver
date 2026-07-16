#ifndef GRAPH_H
#define GRAPH_H

// ---- Nodo de lista de adyacencia ----
typedef struct NodoAdyacencia {
    int vertice;
    struct NodoAdyacencia* siguiente;
} NodoAdyacencia;

// ---- Grafo (lista de adyacencia) ----
typedef struct {
    int num_vertices;
    int ancho;
    int alto;
    NodoAdyacencia** lista;
} Grafo;

Grafo* crear_grafo(int num_vertices, int ancho, int alto);

void agregar_arista(Grafo* g, int u, int v);

void liberar_grafo(Grafo* g);

static inline int xy_a_idx(int x, int y, int ancho) {
    return y * ancho + x;
}

static inline int idx_a_x(int idx, int ancho) {
    return idx % ancho;
}

static inline int idx_a_y(int idx, int ancho) {
    return idx / ancho;
}

#endif // GRAPH_H
