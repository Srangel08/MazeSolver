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
    Punto* buffer_rastro;   
    int inicio;
    int fin;
    int capacidad;
    int cantidad;
    int pasos_dados;
} Jugador;

Jugador* crear_jugador(char simbolo, int capacidad_rastro, int x_inicial, int y_inicial);

int mover_jugador(Jugador* jug, Laberinto* lab, int dx, int dy);

void liberar_jugador(Jugador* jug);


int es_rastro(Jugador* jug, int x, int y);

#endif // PLAYER_H
