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
    char** cuadricula;   /* '#' = pared, ' ' = suelo, 'E' = meta */
    Grafo*  grafo;       /* conectividad entre celdas transitables */
    int inicio_x, inicio_y;
    int meta_x, meta_y;
} Laberinto;

// Crea el laberinto: genera con DFS iterativo, construye el grafo y fija la meta.
Laberinto* crear_laberinto(int ancho, int alto);

// Libera toda la memoria del laberinto (incluido el grafo).
void liberar_laberinto(Laberinto* lab);

// BFS sobre el grafo: retorna el camino mas corto desde (desde_x, desde_y) a la meta.
Camino* encontrar_camino_mas_corto(Laberinto* lab, int desde_x, int desde_y);

void liberar_camino(Camino* cam);

// Pasos minimos hasta la meta (usa BFS internamente).
int obtener_pasos_minimos(Laberinto* lab, int desde_x, int desde_y);

// Modo IA: recorre el laberinto de forma automatica siguiendo BFS y lo dibuja en consola.
void modo_ia(Laberinto* lab);

#endif // MAZE_H
