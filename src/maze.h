// ============================================================
// maze.h
// Interfaz para la generacion del laberinto y busqueda de rutas.
// El laberinto usa DOS estructuras complementarias:
//   - cuadricula char**  : representacion visual (matriz 2D)
//   - Grafo*             : conectividad real entre celdas transitables
// DFS opera sobre el Grafo para generar el laberinto perfecto.
// BFS opera sobre el Grafo para encontrar la ruta minima.
// ============================================================

#ifndef MAZE_H
#define MAZE_H

#include "graph.h"

typedef struct {
    int x, y;
} Punto;

typedef struct {
    Punto* puntos;
    int longitud;
} Camino;

typedef struct {
    int ancho;
    int alto;
    char** cuadricula;   
    Grafo*  grafo;       
    int inicio_x, inicio_y;
    int meta_x, meta_y;
} Laberinto;

Laberinto* crear_laberinto(int ancho, int alto);

void liberar_laberinto(Laberinto* lab);

Camino* encontrar_camino_mas_corto(Laberinto* lab, int desde_x, int desde_y);

void liberar_camino(Camino* cam);

int obtener_pasos_minimos(Laberinto* lab, int desde_x, int desde_y);

void modo_ia(Laberinto* lab);

#endif // MAZE_H
