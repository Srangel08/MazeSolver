// ============================================================
// main.c
// Punto de entrada del juego "The Maze Solver".
//
// Opciones del menu:
//   1. Jugar (modo humano con WASD/flechas, ayuda BFS con H)
//   2. Modo automatico IA: genera laberinto y lo resuelve con BFS
//   3. Personalizar apariencia
//   4. Seleccionar personaje
//   5. Salir
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"
#include "player.h"
#include "ui.h"

#ifdef _WIN32
  #include <windows.h>
  #define SLEEP_MS(ms) Sleep(ms)
#else
  #include <unistd.h>
  #define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

// ---- Subrutina: modo humano ------------------------------------
static void jugar_modo_humano(char simbolo_jugador, Apariencia* ap) {
    limpiar_pantalla();
    printf("=== CONFIGURACION DEL LABERINTO ===\n\n");
    printf("  Referencia de dificultad:\n");
    printf("    - FACIL   : 5x5 hasta 10x10\n");
    printf("    - MEDIO   : 11x11 hasta 20x20\n");
    printf("    - DIFICIL : 21x21 hasta 30x30\n");
    printf("    - EXTREMO : mayor a 30x30\n\n");

    int ancho = leer_entero_valido("Ancho del laberinto (impar, >=5):", 5, 1);
    int alto  = leer_entero_valido("Alto del laberinto (impar, >=5):",  5, 1);

    mostrar_dificultad(ancho, alto);
    printf("\nPresione ENTER para comenzar...");
    while (getchar() != '\n');

    Laberinto* lab = crear_laberinto(ancho, alto);
    if (!lab) { printf("Error al crear el laberinto.\n"); return; }

    Jugador* jug = crear_jugador(simbolo_jugador, CAPACIDAD_RASTRO_DEFECTO,
                                  lab->inicio_x, lab->inicio_y);
    if (!jug) { liberar_laberinto(lab); printf("Error al crear el jugador.\n"); return; }

    int pasos_optimos = obtener_pasos_minimos(lab, lab->inicio_x, lab->inicio_y);

    Camino* ruta_actual = NULL;
    int ganado = 0;
    configurar_modo_juego();

    while (!ganado) {
        limpiar_pantalla();
        renderizar(lab, jug, ruta_actual, ap);

        printf("\n  Pasos: %d", jug->pasos_dados);
        if (ruta_actual) printf("  |  Ruta sugerida: %d pasos", ruta_actual->longitud - 1);
        mostrar_dificultad(ancho, alto);
        printf("  --------------------------------------------------\n");
        printf("  WASD/Flechas: mover  |  H: mostrar/ocultar ruta  |  Q: salir al menu\n");

        int tecla = leer_tecla();
        int dx = 0, dy = 0, mover = 0;

        switch (tecla) {
            case 'w': case 'W': case TECLA_ARRIBA:    dy = -1; mover = 1; break;
            case 's': case 'S': case TECLA_ABAJO:     dy =  1; mover = 1; break;
            case 'a': case 'A': case TECLA_IZQUIERDA: dx = -1; mover = 1; break;
            case 'd': case 'D': case TECLA_DERECHA:   dx =  1; mover = 1; break;
            case 'h': case 'H':
                if (ruta_actual) { liberar_camino(ruta_actual); ruta_actual = NULL; }
                else              { ruta_actual = encontrar_camino_mas_corto(lab, jug->x, jug->y); }
                break;
            case 'q': case 'Q':
                ganado = -1;
                break;
            default:
                break;
        }

        if (mover) {
            int resultado = mover_jugador(jug, lab, dx, dy);
            if (resultado == 1) {
                ganado = 1;
            } else if (resultado == 0 && ruta_actual) {
                // Invalidar la ruta al moverse (la posicion cambio)
                liberar_camino(ruta_actual);
                ruta_actual = NULL;
            }
        }
    }

    restaurar_modo_juego();

    if (ganado == 1) {
        limpiar_pantalla();
        renderizar(lab, jug, NULL, ap);
        printf("\n  Has llegado a la salida!\n");
        mostrar_resultados(jug->pasos_dados, pasos_optimos, ancho, alto);
    }

    if (ruta_actual) liberar_camino(ruta_actual);
    liberar_jugador(jug);
    liberar_laberinto(lab);
}

// ---- Subrutina: modo IA ----------------------------------------
static void jugar_modo_ia(Apariencia* ap) {
    limpiar_pantalla();
    printf("=== MODO AUTOMATICO IA ===\n\n");
    printf("  La IA resolvera el laberinto usando BFS (camino mas corto).\n\n");

    int ancho = leer_entero_valido("Ancho del laberinto (impar, >=5):", 5, 1);
    int alto  = leer_entero_valido("Alto del laberinto (impar, >=5):",  5, 1);

    limpiar_pantalla();
    printf("Generando laberinto...\n");

    Laberinto* lab = crear_laberinto(ancho, alto);
    if (!lab) { printf("Error al crear el laberinto.\n"); return; }

    // Encontramos el camino resuelto de principio a fin desde la IA
    Camino* ruta_optima = encontrar_camino_mas_corto(lab, lab->inicio_x, lab->inicio_y);
    
    if (!ruta_optima || ruta_optima->longitud <= 0) {
        printf("La IA no encontro una ruta de escape valida.\n");
        liberar_laberinto(lab);
        printf("Presione ENTER para continuar...");
        while (getchar() != '\n');
        return;
    }

    // Inicializamos un robot/jugador fantasma de la IA para recrear el recorrido visual
    Jugador* bot = crear_jugador('I', ruta_optima->longitud + 5, lab->inicio_x, lab->inicio_y);

    // Bucle controlado de animación: recorremos el camino paso a paso limpiando la consola
    for (int i = 0; i < ruta_optima->longitud; i++) {
        limpiar_pantalla();
        
        // Colocamos al bot en el paso actual
        bot->x = ruta_optima->puntos[i].x;
        bot->y = ruta_optima->puntos[i].y;
        
        // Registramos el rastro para que deje la estela amarilla detrás de él
        registrar_rastro(bot); 

        // Encabezados
        printf("Ruta optima: %d pasos\n", ruta_optima->longitud - 1);
        printf("=== MODO IA (BFS) === (Paso %d/%d)\n\n", i, ruta_optima->longitud - 1);

        // Renderizamos pasándole el bot y la ruta
        renderizar(lab, bot, NULL, ap);

        SLEEP_MS(150); // Pausa de 150 milisegundos por paso para notar el movimiento continuo
    }

    printf("\n  La IA ha resuelto el laberinto con exito!\n");
    printf("  Presione ENTER para continuar...");
    while (getchar() != '\n');

    liberar_camino(ruta_optima);
    liberar_jugador(bot);
    liberar_laberinto(lab);
}

// ---- main ------------------------------------------------------
int main(void) {
    srand((unsigned)time(NULL));

    Apariencia ap;
    inicializar_apariencia(&ap);
    char simbolo_jugador = '@';

    int corriendo = 1;
    while (corriendo) {
        int opcion = mostrar_menu_principal();
        switch (opcion) {
            case 1:
                jugar_modo_humano(simbolo_jugador, &ap);
                break;
            case 2:
                jugar_modo_ia(&ap); // Le pasamos la apariencia para el correcto dibujado
                break;
            case 3:
                menu_personalizacion(&ap);
                break;
            case 4:
                menu_seleccion_personaje(&simbolo_jugador);
                break;
            case 5:
                corriendo = 0;
                break;
        }
    }

    limpiar_pantalla();
    printf("Gracias por jugar!\n");
    return 0;
}