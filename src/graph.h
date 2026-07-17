// ============================================================
// graph.h
// Grafo de conectividad del laberinto mediante lista de adyacencia.
// Cada celda transitabe es un vertice; cada paso valido es una arista.
// DFS (generacion) y BFS (solver) operan sobre esta estructura.
// ============================================================

#ifndef GRAPH_H
#define GRAPH_H

// ---- Nodo de lista de adyacencia ----
typedef struct NodoAdyacencia {
    int vertice;                  // indice lineal: y * ancho + x
    struct NodoAdyacencia* siguiente;
} NodoAdyacencia;

// ---- Grafo (lista de adyacencia) ----
typedef struct {
    int num_vertices;             // total de celdas (ancho * alto)
    int ancho;                    // columnas del laberinto
    int alto;                     // filas del laberinto
    NodoAdyacencia** lista;       // lista[v] -> vecinos del vertice v
} Grafo;

// Crea un grafo vacio con 'num_vertices' vertices
Grafo* crear_grafo(int num_vertices, int ancho, int alto);

// Agrega arista no dirigida entre u y v
void agregar_arista(Grafo* g, int u, int v);

// Libera toda la memoria del grafo
void liberar_grafo(Grafo* g);

// Convierte coordenadas (x,y) a indice lineal
static inline int xy_a_idx(int x, int y, int ancho) {
    return y * ancho + x;
}

// Convierte indice lineal a coordenada x
static inline int idx_a_x(int idx, int ancho) {
    return idx % ancho;
}

// Convierte indice lineal a coordenada y
static inline int idx_a_y(int idx, int ancho) {
    return idx / ancho;
}

#endif // GRAPH_H