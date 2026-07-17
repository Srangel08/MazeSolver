// ============================================================
// maze.c
// Implementacion de:
//   - DFS iterativo para generacion del laberinto perfecto
//   - Construccion del Grafo (lista de adyacencia) desde la cuadricula
//   - BFS sobre el Grafo para encontrar la ruta minima
//   - Modo IA que recorre y dibuja el camino BFS paso a paso
// ============================================================


#include "maze.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
  #include <windows.h>  // Sleep(ms)
#else
  #include <unistd.h>   // usleep(us)
#endif

// ---- Utilidades internas ----------------------------------------

static void intercambiar(int* a, int* b) {
    int temp = *a; *a = *b; *b = temp;
}

// Mezcla aleatoriamente las 4 direcciones (Fisher-Yates)
static void mezclar_direcciones(int dirs[4][2]) {
    int indices[4] = {0, 1, 2, 3};
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        intercambiar(&indices[i], &indices[j]);
    }
    int temp[4][2];
    for (int i = 0; i < 4; i++) {
        temp[i][0] = dirs[indices[i]][0];
        temp[i][1] = dirs[indices[i]][1];
    }
    memcpy(dirs, temp, sizeof(temp));
}

// Pausa multiplataforma en milisegundos
static void pausa_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

// ---- Paso 1: DFS iterativo para tallado del laberinto -----------
//
// Algoritmo "recursive backtracker" implementado con pila explicita.
// Parte de (1,1), marca la celda como suelo y empuja en la pila.
// En cada iteracion:
//   - Elige aleatoriamente una de las 4 direcciones a distancia 2.
//   - Si el vecino sigue siendo pared y esta dentro del borde, lo
//     "talla": libera la celda intermedia y la celda destino.
//   - Si no hay vecinos disponibles, hace backtrack (pop).
// Resultado: laberinto perfecto (sin ciclos, un unico camino entre
// cualquier par de celdas).
//
static void dfs_generar(Laberinto* lab) {
    int ancho = lab->ancho;
    int alto  = lab->alto;
    char** grid = lab->cuadricula;

    // Pila explicita de puntos
    Punto* pila = malloc(ancho * alto * sizeof(Punto));
    int tope = 0;

    int inicio_x = 1, inicio_y = 1;
    grid[inicio_y][inicio_x] = ' ';
    pila[tope++] = (Punto){inicio_x, inicio_y};

    // Las 4 direcciones en saltos de 2 (entre celdas de suelo)
    int dirs[4][2] = {{0,2}, {0,-2}, {2,0}, {-2,0}};

    while (tope > 0) {
        Punto actual = pila[tope - 1];   // peek (no pop todavia)
        int x = actual.x, y = actual.y;

        mezclar_direcciones(dirs);       // orden aleatorio
        int vecino_encontrado = 0;

        for (int i = 0; i < 4; i++) {
            int nx = x + dirs[i][0];
            int ny = y + dirs[i][1];

            // Debe estar dentro del area interior (no el borde)
            if (nx > 0 && nx < ancho - 1 && ny > 0 && ny < alto - 1
                && grid[ny][nx] == '#') {

                // Talla la celda intermedia (pared entre dos celdas)
                int mx = x + dirs[i][0] / 2;
                int my = y + dirs[i][1] / 2;
                grid[my][mx] = ' ';
                grid[ny][nx] = ' ';

                pila[tope++] = (Punto){nx, ny};
                vecino_encontrado = 1;
                break;  // avanzar, no todos los vecinos de una vez
            }
        }

        if (!vecino_encontrado) {
            tope--;  // backtrack
        }
    }

    free(pila);
}

// ---- Paso 2: Construccion del Grafo desde la cuadricula ---------
//
// Recorre todas las celdas transitables (' ' o 'E').
// Por cada celda, revisa sus 4 vecinos directos (distancia 1).
// Si el vecino tambien es transitable, agrega la arista.
// Para evitar aristas duplicadas, solo agrega cuando nx>x o ny>y.
//
static void construir_grafo(Laberinto* lab) {
    int ancho = lab->ancho;
    int alto  = lab->alto;
    char** grid = lab->cuadricula;

    int movs[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (grid[y][x] == '#') continue;

            int u = xy_a_idx(x, y, ancho);

            for (int i = 0; i < 4; i++) {
                int nx = x + movs[i][0];
                int ny = y + movs[i][1];

                if (nx < 0 || nx >= ancho || ny < 0 || ny >= alto) continue;
                if (grid[ny][nx] == '#') continue;

                int v = xy_a_idx(nx, ny, ancho);

                // Solo procesar cada par (u,v) una vez
                if (v > u) {
                    agregar_arista(lab->grafo, u, v);
                }
            }
        }
    }
}

// ---- BFS auxiliar: celda mas lejana desde (sx, sy) --------------
//
// Usa BFS sobre el grafo para medir distancias.
// Se emplea para colocar la meta en el punto mas alejado del inicio.
//
static Punto encontrar_celda_mas_lejana(Laberinto* lab, int sx, int sy) {
    int ancho = lab->ancho;
    int total = lab->grafo->num_vertices;

    int* dist    = malloc(total * sizeof(int));
    int* visitado = calloc(total, sizeof(int));
    int* cola    = malloc(total * sizeof(int));

    for (int i = 0; i < total; i++) dist[i] = -1;

    int frente = 0, final = 0;
    int idx_ini = xy_a_idx(sx, sy, ancho);
    dist[idx_ini] = 0;
    visitado[idx_ini] = 1;
    cola[final++] = idx_ini;

    int max_dist = 0;
    int idx_lejano = idx_ini;

    while (frente < final) {
        int v = cola[frente++];

        NodoAdyacencia* vecino = lab->grafo->lista[v];
        while (vecino) {
            int w = vecino->vertice;
            if (!visitado[w]) {
                visitado[w] = 1;
                dist[w] = dist[v] + 1;
                cola[final++] = w;
                if (dist[w] > max_dist) {
                    max_dist  = dist[w];
                    idx_lejano = w;
                }
            }
            vecino = vecino->siguiente;
        }
    }

    free(dist);
    free(visitado);
    free(cola);

    return (Punto){ idx_a_x(idx_lejano, ancho), idx_a_y(idx_lejano, ancho) };
}

// ---- Crear laberinto completo -----------------------------------

Laberinto* crear_laberinto(int ancho, int alto) {
    Laberinto* lab = malloc(sizeof(Laberinto));
    if (!lab) return NULL;
    lab->ancho = ancho;
    lab->alto  = alto;
    lab->grafo = NULL;

    // Asignar cuadricula: todo paredes al inicio
    lab->cuadricula = malloc(alto * sizeof(char*));
    if (!lab->cuadricula) { free(lab); return NULL; }

    for (int y = 0; y < alto; y++) {
        lab->cuadricula[y] = malloc(ancho * sizeof(char));
        if (!lab->cuadricula[y]) {
            for (int j = 0; j < y; j++) free(lab->cuadricula[j]);
            free(lab->cuadricula);
            free(lab);
            return NULL;
        }
        memset(lab->cuadricula[y], '#', ancho);
    }

    // --- DFS: talla el laberinto perfecto en la cuadricula ---
    dfs_generar(lab);

    lab->inicio_x = 1;
    lab->inicio_y = 1;

    // --- Construir Grafo desde la cuadricula tallada ---
    lab->grafo = crear_grafo(ancho * alto, ancho, alto);
    if (!lab->grafo) {
        liberar_laberinto(lab);
        return NULL;
    }
    construir_grafo(lab);

    // --- BFS para ubicar la meta en la celda mas lejana ---
    Punto meta = encontrar_celda_mas_lejana(lab, lab->inicio_x, lab->inicio_y);
    lab->meta_x = meta.x;
    lab->meta_y = meta.y;
    lab->cuadricula[meta.y][meta.x] = 'E';

    return lab;
}

void liberar_laberinto(Laberinto* lab) {
    if (!lab) return;
    for (int y = 0; y < lab->alto; y++) {
        free(lab->cuadricula[y]);
    }
    free(lab->cuadricula);
    liberar_grafo(lab->grafo);
    free(lab);
}

// ---- BFS sobre el Grafo: camino mas corto -----------------------
//
// Algoritmo BFS clasico con reconstruccion de camino.
// Explora el grafo (lista de adyacencia) en anchura garantizando
// que la primera vez que se llega a la meta es por la ruta minima.
// Reconstruye el camino siguiendo el array de padres hacia atras.
//
Camino* encontrar_camino_mas_corto(Laberinto* lab, int desde_x, int desde_y) {
    int ancho = lab->ancho;
    int total = lab->grafo->num_vertices;
    int meta_idx = xy_a_idx(lab->meta_x, lab->meta_y, ancho);
    int ini_idx  = xy_a_idx(desde_x, desde_y, ancho);

    if (ini_idx == meta_idx) {
        Camino* c = malloc(sizeof(Camino));
        c->puntos = malloc(sizeof(Punto));
        c->puntos[0] = (Punto){desde_x, desde_y};
        c->longitud = 1;
        return c;
    }

    int* visitado = calloc(total, sizeof(int));
    int* padre    = malloc(total * sizeof(int));
    int* cola     = malloc(total * sizeof(int));

    for (int i = 0; i < total; i++) padre[i] = -1;

    int frente = 0, final = 0;
    int encontrado = 0;

    visitado[ini_idx] = 1;
    cola[final++] = ini_idx;

    while (frente < final && !encontrado) {
        int v = cola[frente++];

        NodoAdyacencia* vecino = lab->grafo->lista[v];
        while (vecino && !encontrado) {
            int w = vecino->vertice;
            if (!visitado[w]) {
                visitado[w] = 1;
                padre[w] = v;
                cola[final++] = w;
                if (w == meta_idx) {
                    encontrado = 1;
                }
            }
            vecino = vecino->siguiente;
        }
    }

    free(cola);
    free(visitado);

    if (!encontrado) {
        free(padre);
        return NULL;
    }

    // Contar longitud del camino
    int pasos = 0;
    int cur = meta_idx;
    while (cur != -1) {
        pasos++;
        cur = padre[cur];
    }

    // Construir array de puntos en orden inicio->meta
    Camino* camino = malloc(sizeof(Camino));
    camino->longitud = pasos;
    camino->puntos   = malloc(pasos * sizeof(Punto));

    cur = meta_idx;
    for (int i = pasos - 1; i >= 0; i--) {
        camino->puntos[i] = (Punto){ idx_a_x(cur, ancho), idx_a_y(cur, ancho) };
        cur = padre[cur];
    }

    free(padre);
    return camino;
}

void liberar_camino(Camino* cam) {
    if (cam) {
        free(cam->puntos);
        free(cam);
    }
}

int obtener_pasos_minimos(Laberinto* lab, int desde_x, int desde_y) {
    Camino* c = encontrar_camino_mas_corto(lab, desde_x, desde_y);
    if (!c) return -1;
    int pasos = c->longitud - 1;
    liberar_camino(c);
    return pasos;
}

// ---- Modo IA ----------------------------------------------------
//
// Obtiene el camino BFS desde el inicio hasta la meta y lo dibuja
// paso a paso en consola, con una pausa entre cada celda para
// hacer visible el recorrido.
//
void modo_ia(Laberinto* lab) {
    printf("\n  === MODO IA (BFS) ===\n");
    printf("  Buscando ruta optima desde el inicio hasta la salida...\n\n");
    pausa_ms(600);

    Camino* camino = encontrar_camino_mas_corto(lab, lab->inicio_x, lab->inicio_y);
    if (!camino) {
        printf("  [IA] No se encontro ruta.\n");
        return;
    }

    int ancho = lab->ancho;
    int alto  = lab->alto;
    // Copia de la cuadricula para dibujar el avance sin modificar el original
    char** dibujo = malloc(alto * sizeof(char*));
    for (int y = 0; y < alto; y++) {
        dibujo[y] = malloc(ancho * sizeof(char));
        memcpy(dibujo[y], lab->cuadricula[y], ancho);
    }

    for (int paso = 0; paso < camino->longitud; paso++) {
        int px = camino->puntos[paso].x;
        int py = camino->puntos[paso].y;

        // Marcar la celda anterior como rastro, la actual como agente IA
        if (paso > 0) {
            int ax = camino->puntos[paso - 1].x;
            int ay = camino->puntos[paso - 1].y;
            if (!(ax == lab->meta_x && ay == lab->meta_y)) {
                dibujo[ay][ax] = '.';
            }
        }
        if (!(px == lab->meta_x && py == lab->meta_y)) {
            dibujo[py][px] = 'A';   // agente IA
        }

        // Limpiar pantalla y dibujar
        printf("\033[2J\033[H");
        printf("  === MODO IA (BFS) ===   Paso %d / %d\n\n",
               paso + 1, camino->longitud);

        for (int y = 0; y < alto; y++) {
            printf("  ");
            for (int x = 0; x < ancho; x++) {
                char c = dibujo[y][x];
                switch (c) {
                    case '#': printf("\033[31m#\033[0m"); break;
                    case 'E': printf("\033[35mE\033[0m"); break;
                    case 'A': printf("\033[1;32mA\033[0m"); break;
                    case '.': printf("\033[33m.\033[0m"); break;
                    default:  printf(" "); break;
                }
            }
            printf("\n");
        }
        printf("\n  Ruta optima: %d pasos\n", camino->longitud - 1);
        pausa_ms(120);
    }

    printf("\n  [IA] Meta alcanzada en %d pasos optimos.\n", camino->longitud - 1);
    printf("  Presione ENTER para continuar...");
    while (getchar() != '\n');

    for (int y = 0; y < alto; y++) free(dibujo[y]);
    free(dibujo);
    liberar_camino(camino);
}
