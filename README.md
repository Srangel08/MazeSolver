# The MazeSolver
## Documentación general del proyecto

| Atributo | Detalle |
| :--- | :--- |
| **Asignatura** | Estructuras de Datos y Algoritmos |
| **Área** | Área 2 — Desarrollo de Juegos y Simulaciones |
| **Tipo** | Juego de laberintos en consola (C, estándar C11)|
| **Versión** | 2.0 — Con grafo de conectividad + modo IA |

---

## 1. ¿Qué es The Maze Solver?
The Maze Solver es un juego de laberintos en consola escrito en C. El programa genera automáticamente un laberinto nuevo en cada partida y le propone al jugador encontrar la salida. También incluye un modo automático donde la inteligencia artificial resuelve el laberinto por su cuenta, mostrando el recorrido en tiempo real.

**Nota:** Cada laberinto es único: se genera procedimentalmente mediante el algoritmo DFS en cada ejecución, garantizando que siempre existe exactamente un camino entre cualquier par de celdas.

---

## 2. ¿Cómo se juega?

### 2.1 Inicio y configuración
Al ejecutar el programa se muestra el menú principal con cinco opciones:

| Opción | Descripción |
| :--- | :--- |
| **1 — Jugar** | Modo humano: el jugador controla el personaje hasta llegar a la salida.|
| **2 — Modo IA** | La IA resuelve el laberinto automáticamente usando BFS y lo anima en pantalla.|
| **3 — Personalizar** | Cambia los símbolos de pared, suelo, meta, rastro y ruta sugerida. |
| **4 — Personaje** | Elige el símbolo que representa al jugador (@, P, X, O, &). |
| **5 — Salir** | Termina el programa. |

### 2.2 Controles durante el juego
| Tecla | Acción |
| :--- | :--- |
| **W / ↑** | Mover hacia arriba |
| **S / ↓** | Mover hacia abajo |
| **A / ←** | Mover hacia la izquierda |
| **D / →** | Mover hacia la derecha |
| **H** | Mostrar / ocultar la ruta más corta hasta la salida (calculada con BFS)|
| **Q** | Salir al menú principal |

### 2.3 Pantalla de juego
| Símbolo | Significado |
| :--- | :--- |
| **#** | Pared (no se puede atravesar)|
| **[Espacio]** | Suelo transitable |
| **E** | Meta / salida del laberinto|
| **.** | Rastro del jugador (últimas 20 posiciones) |
| **\*** | Ruta sugerida por BFS (se activa con H)|
| **@ / P / X…** | Posición actual del jugador

### 2.4 Dificultades disponibles
| Dificultad | Tamaño del laberinto | Celdas aprox. |
| :--- | :--- | :--- |
| **FÁCIL** | 5×5 hasta 10×10[cite: 1] | ≤ 100 |
| **MEDIO** | 11×11 hasta 20×20[cite: 1] | ≤ 400 |
| **DIFÍCIL** | 21×21 hasta 30×30[cite: 1] | ≤ 900 |
| **EXTREMO** | Mayor de 30×30[cite: 1] | > 900 |

> **Importante:** El tamaño debe ser siempre un número impar (5, 7, 9, 11…). Esto es un requisito del algoritmo DFS: las celdas de suelo siempre ocupan posiciones impares, y las paredes los espacios pares entre ellas.

---

## 3. Pantalla de resultados.
Al llegar a la salida el programa muestra tres datos:
* Pasos realizados por el jugador.
* Ruta más corta posible (calculada desde el inicio con BFS).
* Calificación: PERFECTO, Muy bien o Puedes mejorar.

*Si el jugador usó la ruta sugerida al 100 %, obtendrá la calificación PERFECTO aunque haya tardado más tiempo.*

---

## 4. Modo automático (IA)
El modo IA (opción 2 del menú) genera un laberinto del tamaño elegido y lo resuelve sin intervención del jugador. La ejecución se muestra en pantalla paso a paso: se puede ver al agente (símbolo A) avanzar por el laberinto y dejar un rastro (.) a medida que avanza hacia la meta.

Al finalizar, el programa indica cuántos pasos tomó la ruta óptima. La IA siempre llega en el número mínimo posible de pasos porque utiliza BFS, que garantiza el camino más corto en grafos no ponderados.

---

## 5. Estructura del proyecto
| Archivo | Responsabilidad |
| :--- | :--- |
| **graph.h / graph.c** | Define y gestiona el grafo de conectividad (lista de adyacencia). |
| **maze.h / maze.c** | Generación del laberinto (DFS), construcción del grafo, BFS y modo IA. |
| **player.h / player.c** | Control del jugador, movimiento y buffer circular de rastro. |
| **ui.h / ui.c** | Menús, renderizado en consola con colores ANSI, captura de teclas. |
| **main.c** | Punto de entrada; orquesta los modos de juego.|
| **Makefile** | Compilación con gcc -Wall -Wextra -std=c11. |

---

## 6. Compilación y ejecución
Requiere `gcc` y `make`. 

**En Linux / macOS / WSL:**
```bash
cd maze_corregido
make
./maze_solver
