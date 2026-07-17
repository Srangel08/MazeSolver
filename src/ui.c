// ============================================================
// ui.c
// Renderizado, menus, captura de teclas y utilidades de consola.
// ============================================================

#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #include <conio.h>
  #include <windows.h>
#else
  #include <termios.h>
  #include <unistd.h>
#endif

/* Secuencias de color ANSI */
#define COLOR_RESET      "\033[0m"
#define COLOR_PARED      "\033[31m"
#define COLOR_SUELO      "\033[0m"
#define COLOR_META       "\033[35m"
#define COLOR_RASTRO     "\033[33m"
#define COLOR_SUGERENCIA "\033[36m"
#define COLOR_JUGADOR    "\033[1;32m"

void inicializar_apariencia(Apariencia* ap) {
    ap->pared      = '#';
    ap->suelo      = ' ';
    ap->meta       = 'E';
    ap->rastro     = '.';
    ap->sugerencia = '*';
}

void limpiar_pantalla(void) {
#ifdef _WIN32
    // En Windows, system("cls") es lo más limpio para CMD/PowerShell
    system("cls");
#else
    // En Linux/macOS, las secuencias ANSI van perfecto
    printf("\033[2J\033[H");
#endif
    // Fuerza a la consola a procesar la limpieza inmediatamente
    fflush(stdout);
}

#ifndef _WIN32
static struct termios original_term;
#endif

void configurar_modo_juego(void) {
#ifndef _WIN32
    tcgetattr(STDIN_FILENO, &original_term);
    struct termios raw = original_term;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
#endif
}

void restaurar_modo_juego(void) {
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
#endif
}

int leer_tecla(void) {
#ifdef _WIN32
    int c = _getch();
    if (c == 0 || c == 0xE0) {
        int arrow = _getch();
        switch (arrow) {
            case 72: return TECLA_ARRIBA;
            case 80: return TECLA_ABAJO;
            case 75: return TECLA_IZQUIERDA;
            case 77: return TECLA_DERECHA;
            default: return 0;
        }
    }
    return c;
#else
    int c = getchar();
    if (c == 27) {
        int next = getchar();
        if (next == '[') {
            int arrow = getchar();
            switch (arrow) {
                case 'A': return TECLA_ARRIBA;
                case 'B': return TECLA_ABAJO;
                case 'C': return TECLA_DERECHA;
                case 'D': return TECLA_IZQUIERDA;
                default:  return 27;
            }
        } else {
            ungetc(next, stdin);
            return 27;
        }
    }
    return c;
#endif
}

// ---- Menu principal --------------------------------------------
int mostrar_menu_principal(void) {
    limpiar_pantalla();
    printf("==========================================\n");
    printf("         THE MAZE SOLVER\n");
    printf("==========================================\n\n");
    printf("  1. Jugar (modo humano)\n");
    printf("  2. Modo automatico IA (BFS)\n");
    printf("  3. Personalizar apariencia\n");
    printf("  4. Seleccionar personaje\n");
    printf("  5. Salir\n\n");
    printf("==========================================\n");
    printf("Elija una opcion: ");
    int opcion;
    while (1) {
        opcion = leer_entero_valido("", 1, 0);
        if (opcion >= 1 && opcion <= 5) break;
        printf("Opcion invalida. Intente de nuevo: ");
    }
    return opcion;
}

void menu_seleccion_personaje(char* simbolo) {
    limpiar_pantalla();
    printf("=== SELECCION DE PERSONAJE ===\n\n");
    printf("  1. @  (Arroba)\n");
    printf("  2. P  (Pe mayuscula)\n");
    printf("  3. X  (Equis)\n");
    printf("  4. O  (O mayuscula)\n");
    printf("  5. &  (Ampersand)\n\n");
    printf("Elija una opcion: ");
    int op = leer_entero_valido("", 1, 0);
    switch (op) {
        case 1: *simbolo = '@'; break;
        case 2: *simbolo = 'P'; break;
        case 3: *simbolo = 'X'; break;
        case 4: *simbolo = 'O'; break;
        case 5: *simbolo = '&'; break;
        default: printf("Opcion invalida, se mantiene el simbolo actual.\n");
    }
    printf("Simbolo seleccionado: %c\n", *simbolo);
    printf("Presione ENTER para continuar...");
    while (getchar() != '\n');
}

void menu_personalizacion(Apariencia* ap) {
    int salir = 0;
    while (!salir) {
        limpiar_pantalla();
        printf("=== PERSONALIZACION DE APARIENCIA ===\n\n");
        printf("  1. Pared      : '%c'\n", ap->pared);
        printf("  2. Suelo      : '%c'\n", ap->suelo);
        printf("  3. Meta       : '%c'\n", ap->meta);
        printf("  4. Rastro     : '%c'\n", ap->rastro);
        printf("  5. Sugerencia : '%c'\n", ap->sugerencia);
        printf("  6. Restaurar valores por defecto\n");
        printf("  7. Volver al menu principal\n\n");
        printf("Opcion: ");
        int op = leer_entero_valido("", 1, 0);
        switch (op) {
            case 1:
                printf("Nuevo caracter para pared: ");
                { char buf[8]; if (fgets(buf,sizeof(buf),stdin) && buf[0]!='\n') ap->pared=buf[0]; } break;
            case 2:
                printf("Nuevo caracter para suelo: ");
                { char buf[8]; if (fgets(buf,sizeof(buf),stdin) && buf[0]!='\n') ap->suelo=buf[0]; } break;
            case 3:
                printf("Nuevo caracter para meta: ");
                { char buf[8]; if (fgets(buf,sizeof(buf),stdin) && buf[0]!='\n') ap->meta=buf[0]; } break;
            case 4:
                printf("Nuevo caracter para rastro: ");
                { char buf[8]; if (fgets(buf,sizeof(buf),stdin) && buf[0]!='\n') ap->rastro=buf[0]; } break;
            case 5:
                printf("Nuevo caracter para sugerencia: ");
                { char buf[8]; if (fgets(buf,sizeof(buf),stdin) && buf[0]!='\n') ap->sugerencia=buf[0]; } break;
            case 6:
                inicializar_apariencia(ap);
                printf("Valores restaurados.\n"); break;
            case 7:
                salir = 1; break;
            default:
                printf("Opcion invalida.\n");
        }
        if (!salir) {
            printf("Presione ENTER para continuar...");
            while (getchar() != '\n');
        }
    }
}

// ---- Renderizado del laberinto ---------------------------------
void renderizar(Laberinto* lab, Jugador* jug, Camino* ruta_sug, Apariencia* ap) {
    for (int y = 0; y < lab->alto; y++) {
        for (int x = 0; x < lab->ancho; x++) {
            char c;
            const char* color;

            // PRIORIDAD 1: Si es la coordenada del jugador, se dibuja SÍ O SÍ
            if (x == jug->x && y == jug->y) {
                c     = jug->simbolo;
                color = COLOR_JUGADOR;
            } else {
                char celda = lab->cuadricula[y][x];
                if (celda == '#') {
                    c = ap->pared; color = COLOR_PARED;
                } else if (celda == 'E') {
                    c = ap->meta; color = COLOR_META;
                } else {
                    int es_sug = 0;
                    if (ruta_sug) {
                        for (int i = 0; i < ruta_sug->longitud; i++) {
                            if (ruta_sug->puntos[i].x == x &&
                                ruta_sug->puntos[i].y == y &&
                                !(x == lab->meta_x && y == lab->meta_y)) {
                                es_sug = 1;
                                break;
                            }
                        }
                    }
                    if (es_sug) {
                        c = ap->sugerencia; color = COLOR_SUGERENCIA;
                    } else if (es_rastro(jug, x, y)) {
                        c = ap->rastro; color = COLOR_RASTRO;
                    } else {
                        c = ap->suelo; color = COLOR_SUELO;
                    }
                }
            }
            printf("%s%c", color, c);
        }
        printf("%s\n", COLOR_RESET);
    }
    // Asegura que todo el mapa se dibuje en pantalla al mismo milisegundo
    fflush(stdout);
}

int leer_entero_valido(const char* mensaje, int minimo, int impar_obligatorio) {
    char buffer[100];
    int valor, leido;
    while (1) {
        if (mensaje && strlen(mensaje) > 0) printf("%s ", mensaje);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            exit(1);
        }
        leido = sscanf(buffer, "%d", &valor);
        if (leido == 1) {
            if (valor < minimo) {
                printf("El numero debe ser al menos %d.\n", minimo);
                continue;
            }
            if (impar_obligatorio && (valor % 2 == 0)) {
                printf("Debe ingresar un numero impar.\n");
                continue;
            }
            return valor;
        } else {
            printf("Entrada no valida. Intente nuevamente.\n");
        }
    }
}

void mostrar_dificultad(int ancho, int alto) {
    int area = ancho * alto;
    printf("\n  Dificultad: ");
    if      (area <= 100) printf("FACIL");
    else if (area <= 400) printf("MEDIO");
    else if (area <= 900) printf("DIFICIL");
    else                  printf("EXTREMO");
    printf(" (mapa de %d x %d)\n", ancho, alto);
}

void mostrar_resultados(int pasos_jugador, int pasos_optimos, int ancho, int alto) {
    printf("\n");
    printf("  ========================================\n");
    printf("           RESULTADOS FINALES\n");
    printf("  ========================================\n\n");
    printf("  Pasos realizados por ti : %d\n", pasos_jugador);
    printf("  Ruta mas corta posible  : %d pasos\n", pasos_optimos);

    if (pasos_jugador == pasos_optimos) {
        printf("\n  >>> PERFECTO! Tomaste la ruta optima. <<<\n");
    } else if (pasos_jugador <= pasos_optimos * 3 / 2) {
        printf("\n  >>> Muy bien! Estuviste cerca de la ruta optima. <<<\n");
    } else {
        printf("\n  >>> Puedes mejorar. Intenta usar la ayuda (tecla H). <<<\n");
    }

    mostrar_dificultad(ancho, alto);
    printf("\n  ========================================\n");
    printf("  Presione ENTER para continuar...");
    while (getchar() != '\n');
}