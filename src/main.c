#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"
#include "player.h"
#include "ui.h"

static void jugar_modo_humano(char simbolo_jugador, Apariencia* ap) {
    limpiar_pantalla();
    printf("=== CONFIGURACION DEL LABERINTO ===\n\n");
    printf("    Referencia de dificultad:\n");
    printf("     - FACIL: 5x5 hasta 10x10\n");
    printf("     - MEDIO: 11x11 hasta 20x20\n");
    printf("    - DIFICIL: 21x21 hasta 30x30\n");
    printf("    - EXTREMO: mayor a 30x30\n\n");

    int ancho = leer_entero_valido("Ancho del laberinto (impar >=5):", 5, 1);
    int alto = leer_entero_valido("Alto del laberinto (impar, >=5):", 5, 1);

    mostrar_dificultad(ancho, alto);
    printf("\nPresione ENTER para comenzar...");
    while(getchar() != '\n');

    Laberinto* lab = crear_laberinto(ancho, alto);
    if (!lab) {printf("Error al crear el laberinto. \n"); return;}

    Jugador* jug = crear_jugador(simbolo_jugador, CAPACIDAD_RASTRO_DEFECTO, lab->inicio_x, lab->inicio_y);
    if (!jug) {liberar_laberinto(lab); printf("Error al crear el jugador.\n"); return;}

    int pasos_optimos = obtener_pasos_minimos(lab, lab->inicio_x, lab->inicio_y);
    Camino* ruta_actual = NULL;
    int ganado = 0;
    configurar_modo_juego();

    while (!ganado) {
        limpiar_pantalla();
        renderizar(lab, jug, ruta_actual, ap);

        printf("\n Pasos: %d", jug->pasos_dados);
        if (ruta_actual) printf(" | RUTA SUGERIDA: %d pasos", ruta_actual->longitud - 1);
        mostrar_dificultad(ancho, alto);
        printf("---------------------------------------------------\n");
        printf(" WASD/Flechas: mover  |  H: mostrar/ocultar ruta  |  Q: salir al menu\n");

        int tecla = leer_tecla();
        int dx = 0, dy = 0, mover = 0;

        switch(tecla) {
            case 'w': case 'W': case TECLA_ARRIBA: dy = -1; mover = 1; break;
            case 's': case 'S': case TECLA_ABAJO: dy = 1; mover = 1; break;
            case 'a': case 'A': case TECLA_IZQUIERDA: dx = -1; mover = 1; break;
            case 'd': case 'D': case TECLA_DERECHA: dx = 1; mover = 1; break;
            case 'h': case 'H':
                if(ruta_actual) { liberar_camino(ruta_actual); ruta_actual = NULL; }
                else {ruta_actual = encontrar_camino_mas_corto(lab, jug->x, jug->y); }
                break;
            case 'q': case 'Q':
                ganado = -1;
                break;
            default:
                break;
        }
        if(mover) {
            int resultado = mover_jugador(jug, lab, dx, dy);
            if (resultado == 1) {
                ganado = 1;
            } elseif(resultado == 0 && ruta_actual) {
                liberar_camino(ruta_actual);
                ruta_actual = NULL;
            }
        }
    }
    restaurar_modo_juego();

    if (ganado == 1) {
        limpiar_pantalla();
        renderizar(lab, jug, NULL, ap);
        printf("\n Has llegado a la salida!\n");
        mostrar_resultados(jug->pasos_dados, pasos_optimos, ancho, alto);
    }

    if (ruta_actual) liberar_camino(ruta_actual);
    liberar_jugador(jug);
    liberar_laberinto(lab);
}

// MODO IA