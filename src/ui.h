// ============================================================
// ui.h
// Funciones de interfaz de usuario, menus y renderizado en terminal.
// ============================================================

#ifndef UI_H
#define UI_H

#include "maze.h"
#include "player.h"

typedef struct {
    char pared;
    char suelo;
    char meta;
    char rastro;
    char sugerencia;
} Apariencia;

#define TECLA_ARRIBA     300
#define TECLA_ABAJO      301
#define TECLA_IZQUIERDA  302
#define TECLA_DERECHA    303

void inicializar_apariencia(Apariencia* ap);

int mostrar_menu_principal(void);

void menu_seleccion_personaje(char* simbolo);
void menu_personalizacion(Apariencia* ap);
void limpiar_pantalla(void);
void configurar_modo_juego(void);
void restaurar_modo_juego(void);
int  leer_tecla(void);
void renderizar(Laberinto* lab, Jugador* jug, Camino* ruta_sug, Apariencia* ap);
int  leer_entero_valido(const char* mensaje, int minimo, int impar_obligatorio);
void mostrar_resultados(int pasos_jugador, int pasos_optimos, int ancho, int alto);
void mostrar_dificultad(int ancho, int alto);

#endif // UI_H

#ifndef UI_H
#define UI_H

#include "maze.h"
#include "player.h"

typedef struct {
    char pared;
    char suelo;
    char meta;
    char rastro;
    char sugerencia;
} Apariencia;

#define TECLA_ARRIBA     300
#define TECLA_ABAJO      301
#define TECLA_IZQUIERDA  302
#define TECLA_DERECHA    303

void inicializar_apariencia(Apariencia* ap);

// Retorna opcion elegida: 1=Jugar, 2=IA, 3=Apariencia, 4=Personaje, 5=Salir
int mostrar_menu_principal(void);

void menu_seleccion_personaje(char* simbolo);
void menu_personalizacion(Apariencia* ap);
void limpiar_pantalla(void);
void configurar_modo_juego(void);
void restaurar_modo_juego(void);
int  leer_tecla(void);
void renderizar(Laberinto* lab, Jugador* jug, Camino* ruta_sug, Apariencia* ap);
int  leer_entero_valido(const char* mensaje, int minimo, int impar_obligatorio);
void mostrar_resultados(int pasos_jugador, int pasos_optimos, int ancho, int alto);
void mostrar_dificultad(int ancho, int alto);

#endif // UI_H
