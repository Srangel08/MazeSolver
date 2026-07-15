# Documentación Técnica
## Rationale de Diseño, Algoritmos y Estructuras de Datos

---

# 1. Arquitectura del sistema

El proyecto sigue una **arquitectura modular en capas**, donde cada módulo encapsula una única responsabilidad.

| Capa | Módulo | Depende de |
|------|--------|------------|
| Estructura de datos | `graph.h / graph.c` | Ninguna |
| Dominio del juego | `maze.h / maze.c` | `graph.h` |
| Entidad jugador | `player.h / player.c` | `maze.h` |
| Presentación | `ui.h / ui.c` | `maze.h`, `player.h` |
| Orquestación | `main.c` | `maze.h`, `player.h`, `ui.h` |

> **📘 Nota**
>
> La capa del grafo (`graph.h`) no conoce nada del juego; únicamente implementa la estructura de datos. Esto permite reemplazar la implementación interna del grafo sin modificar `maze.c` ni el resto del sistema.

---

# 2. Estructuras de Datos

## 2.1 Matriz Bidimensional (`char**`)

```c
char** cuadricula;   // cuadricula[y][x]

// '#' = pared
// ' ' = camino
// 'E' = meta
```

### Razón de diseño

La matriz representa la parte **visual** del laberinto.

### Ventajas

- Acceso O(1) a cualquier celda.
- Muy sencilla de dibujar en consola.
- Poco consumo de memoria.

### Limitación

La matriz conoce únicamente el contenido de una celda, **no** conoce las conexiones entre ellas.

---

## 2.2 Grafo mediante Lista de Adyacencia

```c
typedef struct NodoAdyacencia {
    int vertice;
    struct NodoAdyacencia* siguiente;
} NodoAdyacencia;

typedef struct {
    int num_vertices;
    int ancho, alto;
    NodoAdyacencia** lista;
} Grafo;
```

### Razón de diseño

El grafo almacena exclusivamente la **conectividad** entre celdas transitables.

### Ventajas

- DFS y BFS recorren únicamente vecinos existentes.
- No es necesario revisar paredes.
- El recorrido depende únicamente del grado del vértice.

---

## 2.3 Buffer Circular (Rastro del jugador)

```c
typedef struct {
    Punto* buffer_rastro;
    int inicio;
    int fin;
    int capacidad;
    int cantidad;
} Jugador;
```

### Razón de diseño

Permite mantener únicamente las últimas posiciones del jugador.

**Complejidad**

- Inserción → **O(1)**
- Eliminación del más antiguo → **O(1)**

No es necesario desplazar elementos.

---

# 3. Algoritmos

# 3.1 DFS Iterativo — Generación del Laberinto

## ¿Por qué DFS?

Se eligió DFS porque produce:

- Pasillos largos
- Menos bifurcaciones
- Laberintos más desafiantes

BFS y Prim generan laberintos mucho más abiertos.

---

## Descripción del algoritmo

1. Inicializar toda la cuadrícula con paredes (`#`).
2. Marcar `(1,1)` como camino.
3. Insertar esa celda en una pila.
4. Mientras la pila no esté vacía:
   - Observar el tope.
   - Mezclar las cuatro direcciones (Fisher-Yates).
   - Buscar vecinos válidos.
   - Si existe uno:
     - Romper la pared.
     - Marcar la nueva celda.
     - Apilarla.
   - Si no existe:
     - Hacer **Backtracking (Pop).**

---

### Fragmento principal

```c
while (tope > 0) {

    Punto actual = pila[tope - 1];

    mezclar_direcciones(dirs);

    int vecino_encontrado = 0;

    for (int i = 0; i < 4; i++) {

        int nx = x + dirs[i][0];
        int ny = y + dirs[i][1];

        if (dentro_del_borde && grid[ny][nx] == '#') {

            grid[my][mx] = ' ';
            grid[ny][nx] = ' ';

            pila[tope++] = (Punto){nx, ny};

            vecino_encontrado = 1;

            break;
        }
    }

    if (!vecino_encontrado)
        tope--;
}
```

### Complejidad

| Complejidad | Valor |
|-------------|-------|
| Tiempo | **O(V)** |
| Espacio | **O(V)** |

---

# 3.2 Construcción del Grafo

Después del DFS se recorre toda la cuadrícula para conectar cada celda transitable con sus vecinos.

Para evitar duplicados únicamente se agrega una arista cuando:

```
vecino > actual
```

### Complejidad

```
O(V + E)
```

---

# 3.3 BFS — Ruta más corta

## ¿Por qué BFS?

Porque garantiza el camino mínimo en grafos no ponderados.

DFS encuentra una ruta, pero no necesariamente la más corta.

A* sería innecesariamente complejo para este proyecto.

---

## Funcionamiento

1. Encolar inicio.
2. Marcar visitado.
3. Mientras la cola tenga elementos:
   - Desencolar.
   - Revisar vecinos.
   - Marcar padre.
   - Encolar nuevos vecinos.
4. Reconstruir el camino usando el arreglo `padre[]`.

---

### Fragmento principal

```c
while (frente < final && !encontrado) {

    int v = cola[frente++];

    NodoAdyacencia* vecino = lab->grafo->lista[v];

    while (vecino && !encontrado) {

        int w = vecino->vertice;

        if (!visitado[w]) {

            visitado[w] = 1;
            padre[w] = v;
            cola[final++] = w;

            if (w == meta_idx)
                encontrado = 1;
        }

        vecino = vecino->siguiente;
    }
}
```

### Complejidad

| Complejidad | Valor |
|-------------|-------|
| Tiempo | **O(V+E)** |
| Espacio | **O(V)** |

En un laberinto perfecto:

```
E = V - 1

↓

O(V + E)

↓

O(V)
```

---

# 3.4 Modo IA

La IA ejecuta BFS una sola vez.

Luego reproduce la ruta paso a paso:

```
A → . → . → . → E
```

Costo total

```
BFS      O(V)

+

Animación O(L)

=

O(V)
```

---

# 4. ¿Grafo o Matriz?

## Comparación

| Criterio | Matriz | Grafo |
|----------|---------|--------|
| Acceso a celda | O(1) | O(1) |
| Obtener vecinos | Revisar 4 posiciones | Solo vecinos reales |
| Memoria | O(ancho × alto) | O(V+E) |
| BFS/DFS | O(V) | O(V+E) |
| Pesos | ❌ | ✅ |
| Cambios dinámicos | Muy fáciles | Más complejos |

---

## Cuándo usar solo Matriz

✔ Menor memoria

✔ Código más simple

✔ Más rápida

✔ Vecinos fijos

---

## Cuándo usar Grafo

✔ Dijkstra

✔ A*

✔ Bellman-Ford

✔ Laberintos con ciclos

✔ Aristas con pesos

✔ Algoritmos múltiples

---

## Veredicto

### Para este proyecto

✅ La **matriz sola sería más eficiente.**

### Sin embargo...

El **grafo** fue implementado porque:

- era un requisito académico;
- demuestra manejo de estructuras de datos;
- facilita futuras extensiones.

---

# 5. Rationale de Diseño

| Decisión | Alternativa | Motivo |
|----------|-------------|--------|
| DFS iterativo | DFS recursivo | Evita Stack Overflow |
| Lista de adyacencia | Matriz de adyacencia | O(V+E) vs O(V²) |
| Índice lineal | Punto(x,y) | Acceso O(1) |
| Buffer circular | Lista enlazada | Inserción O(1) |
| graph separado | Todo en maze.c | Responsabilidad única |
| Meta más lejana | Meta fija | Mayor dificultad |
| `*3/2` entero | `*1.5` float | Evita punto flotante |

---

# 6. Resumen de Complejidades

| Operación | Tiempo | Espacio |
|-----------|---------|----------|
| DFS | O(V) | O(V) |
| Construcción del grafo | O(V+E) | O(V+E) |
| BFS | O(V+E) | O(V) |
| BFS meta lejana | O(V+E) | O(V) |
| Renderizado | O(ancho × alto) | O(1) |
| Buffer circular | O(capacidad) | O(capacidad) |
| Total por partida | O(V) | O(V+E) |

---

## 📘 Observación Final

En un laberinto perfecto:

- **V** = número de celdas transitables
- **E = V − 1**

Por tanto,

```
O(V + E)

↓

O(V)
```

---

## Tecnologías Utilizadas

- Lenguaje **C**
- DFS (Depth First Search)
- BFS (Breadth First Search)
- Lista de Adyacencia
- Buffer Circular
- Memoria dinámica (`malloc` / `free`)
- Arquitectura Modular
