// ============================================================
// player.h
// Control del jugador y gestion del rastro limitado (buffer circular).
// ============================================================

#ifndef PLAYER_H
#define PLAYER_H

#include "maze.h"

#define CAPACIDAD_RASTRO_DEFECTO 20

typedef struct {
    char simbolo;
    int x, y;
    Punto* buffer_rastro;   // buffer circular de las ultimas N posiciones
    int inicio;
    int fin;
    int capacidad;
    int cantidad;
    int pasos_dados;
} Jugador;

Jugador* crear_jugador(char simbolo, int capacidad_rastro, int x_inicial, int y_inicial);

// Retorna: -1 movimiento invalido, 0 movimiento ok, 1 llego a la meta
int mover_jugador(Jugador* jug, Laberinto* lab, int dx, int dy);

void liberar_jugador(Jugador* jug);

// Indica si (x,y) esta en el rastro visible del jugador
int es_rastro(Jugador* jug, int x, int y);

#endif // PLAYER_H
