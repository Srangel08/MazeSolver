// ============================================================
// player.c
// Implementacion del jugador y la mecanica de rastro limitado.
// El rastro se implementa con un buffer circular de capacidad fija.
// ============================================================

#include "player.h"
#include <stdlib.h>

Jugador* crear_jugador(char simbolo, int capacidad_rastro, int x_inicial, int y_inicial) {
    Jugador* j = malloc(sizeof(Jugador));
    if (!j) return NULL;

    j->simbolo      = simbolo;
    j->x            = x_inicial;
    j->y            = y_inicial;
    j->capacidad    = capacidad_rastro;
    j->cantidad     = 0;
    j->inicio       = 0;
    j->fin          = 0;
    j->pasos_dados  = 0;

    j->buffer_rastro = malloc(capacidad_rastro * sizeof(Punto));
    if (!j->buffer_rastro) {
        free(j);
        return NULL;
    }
    return j;
}

int mover_jugador(Jugador* jug, Laberinto* lab, int dx, int dy) {
    int nx = jug->x + dx;
    int ny = jug->y + dy;

    // Limites del laberinto
    if (nx < 0 || nx >= lab->ancho || ny < 0 || ny >= lab->alto) return -1;
    // Colision con pared
    if (lab->cuadricula[ny][nx] == '#') return -1;

    // Guardar posicion actual en el buffer circular antes de movernos
    if (jug->cantidad < jug->capacidad) {
        jug->buffer_rastro[jug->fin] = (Punto){jug->x, jug->y};
        jug->fin = (jug->fin + 1) % jug->capacidad;
        jug->cantidad++;
    } else {
        // Buffer lleno: sobreescribimos usando 'fin' y desplazamos ambos indices
        jug->buffer_rastro[jug->fin] = (Punto){jug->x, jug->y};
        jug->inicio = (jug->inicio + 1) % jug->capacidad;
        jug->fin    = (jug->fin + 1) % jug->capacidad;
    }

    // Actualizar posicion del jugador
    jug->x = nx;
    jug->y = ny;
    jug->pasos_dados++;

    return (lab->cuadricula[ny][nx] == 'E') ? 1 : 0;
}

void liberar_jugador(Jugador* jug) {
    if (jug) {
        free(jug->buffer_rastro);
        free(jug);
    }
}

int es_rastro(Jugador* jug, int x, int y) {
    for (int i = 0; i < jug->cantidad; i++) {
        int idx = (jug->inicio + i) % jug->capacidad;
        if (jug->buffer_rastro[idx].x == x && jug->buffer_rastro[idx].y == y)
            return 1;
    }
    return 0;
}