# MazeSolver
# THE MAZE SOLVER
## Documentación General del Proyecto

| Atributo | Detalle |
| :--- | :--- |
| **Asignatura** | Estructuras de Datos y Algoritmos[cite: 1] |
| **Área** | Área 2 — Desarrollo de Juegos y Simulaciones[cite: 1] |
| **Tipo** | Juego de laberintos en consola (C, estándar C11)[cite: 1] |
| **Versión** | 2.0 — Con grafo de conectividad + modo IA[cite: 1] |

---

## 1. ¿Qué es The Maze Solver?[cite: 1]
The Maze Solver es un juego de laberintos en consola escrito en C[cite: 1]. El programa genera automáticamente un laberinto nuevo en cada partida y le propone al jugador encontrar la salida[cite: 1]. También incluye un modo automático donde la inteligencia artificial resuelve el laberinto por su cuenta, mostrando el recorrido en tiempo real[cite: 1].

**Nota:** Cada laberinto es único: se genera procedimentalmente mediante el algoritmo DFS en cada ejecución, garantizando que siempre existe exactamente un camino entre cualquier par de celdas[cite: 1].

---

## 2. ¿Cómo se juega?[cite: 1]

### 2.1 Inicio y configuración[cite: 1]
Al ejecutar el programa se muestra el menú principal con cinco opciones:[cite: 1]

| Opción | Descripción |
| :--- | :--- |
| **1 — Jugar** | Modo humano: el jugador controla el personaje hasta llegar a la salida.[cite: 1] |
| **2 — Modo IA** | La IA resuelve el laberinto automáticamente usando BFS y lo anima en pantalla.[cite: 1] |
| **3 — Personalizar** | Cambia los símbolos de pared, suelo, meta, rastro y ruta sugerida.[cite: 1] |
| **4 — Personaje** | Elige el símbolo que representa al jugador (@, P, X, O, &).[cite: 1] |
| **5 — Salir** | Termina el programa.[cite: 1] |

### 2.2 Controles durante el juego[cite: 1]
| Tecla | Acción |
| :--- | :--- |
| **W / ↑** | Mover hacia arriba[cite: 1] |
| **S / ↓** | Mover hacia abajo[cite: 1] |
| **A / ←** | Mover hacia la izquierda[cite: 1] |
| **D / →** | Mover hacia la derecha[cite: 1] |
| **H** | Mostrar / ocultar la ruta más corta hasta la salida (calculada con BFS)[cite: 1] |
| **Q** | Salir al menú principal[cite: 1] |

### 2.3 Pantalla de juego[cite: 1]
| Símbolo | Significado |
| :--- | :--- |
| **#** | Pared (no se puede atravesar)[cite: 1] |
| **[Espacio]** | Suelo transitable[cite: 1] |
| **E** | Meta / salida del laberinto[cite: 1] |
| **.** | Rastro del jugador (últimas 20 posiciones)[cite: 1] |
| **\*** | Ruta sugerida por BFS (se activa con H)[cite: 1] |
| **@ / P / X…** | Posición actual del jugador[cite: 1] |

### 2.4 Dificultades disponibles[cite: 1]
| Dificultad | Tamaño del laberinto | Celdas aprox. |
| :--- | :--- | :--- |
| **FÁCIL** | 5×5 hasta 10×10[cite: 1] | ≤ 100[cite: 1] |
| **MEDIO** | 11×11 hasta 20×20[cite: 1] | ≤ 400[cite: 1] |
| **DIFÍCIL** | 21×21 hasta 30×30[cite: 1] | ≤ 900[cite: 1] |
| **EXTREMO** | Mayor de 30×30[cite: 1] | > 900[cite: 1] |

> **Importante:** El tamaño debe ser siempre un número impar (5, 7, 9, 11…). Esto es un requisito del algoritmo DFS: las celdas de suelo siempre ocupan posiciones impares, y las paredes los espacios pares entre ellas[cite: 1].

---

## 3. Pantalla de resultados[cite: 1]
Al llegar a la salida el programa muestra tres datos:[cite: 1]
* Pasos realizados por el jugador[cite: 1].
* Ruta más corta posible (calculada desde el inicio con BFS)[cite: 1].
* Calificación: PERFECTO, Muy bien o Puedes mejorar[cite: 1].

*Si el jugador usó la ruta sugerida al 100 %, obtendrá la calificación PERFECTO aunque haya tardado más tiempo[cite: 1].*

---

## 4. Modo automático (IA)[cite: 1]
El modo IA (opción 2 del menú) genera un laberinto del tamaño elegido y lo resuelve sin intervención del jugador[cite: 1]. La ejecución se muestra en pantalla paso a paso: se puede ver al agente (símbolo A) avanzar por el laberinto y dejar un rastro (.) a medida que avanza hacia la meta[cite: 1].

Al finalizar, el programa indica cuántos pasos tomó la ruta óptima[cite: 1]. La IA siempre llega en el número mínimo posible de pasos porque utiliza BFS, que garantiza el camino más corto en grafos no ponderados[cite: 1].

---

## 5. Estructura del proyecto[cite: 1]
| Archivo | Responsabilidad |
| :--- | :--- |
| **graph.h / graph.c** | Define y gestiona el grafo de conectividad (lista de adyacencia).[cite: 1] |
| **maze.h / maze.c** | Generación del laberinto (DFS), construcción del grafo, BFS y modo IA.[cite: 1] |
| **player.h / player.c** | Control del jugador, movimiento y buffer circular de rastro.[cite: 1] |
| **ui.h / ui.c** | Menús, renderizado en consola con colores ANSI, captura de teclas.[cite: 1] |
| **main.c** | Punto de entrada; orquesta los modos de juego.[cite: 1] |
| **Makefile** | Compilación con gcc -Wall -Wextra -std=c11.[cite: 1] |

---

## 6. Compilación y ejecución[cite: 1]
Requiere `gcc` y `make`[cite: 1]. 

**En Linux / macOS / WSL:**[cite: 1]
```bash
cd maze_corregido
make
./maze_solver
