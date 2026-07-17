CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2 -D_GNU_SOURCE -Isrc

# Nombre del ejecutable final
TARGET  = maze_solver.exe

# Lista de archivos fuente dentro de la carpeta 'src'
SRCS    = src/main.c src/maze.c src/player.c src/ui.c src/graph.c

# Convierte las rutas para que los .o vayan a la carpeta 'build'
OBJS    = $(patsubst src/%.c, build/%.o, $(SRCS))

all: $(TARGET)

# Enlace de los archivos objeto para crear el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilación de cada .c a su respectivo .o en build/ (Compatible con Windows)
build/%.o: src/%.c
	@if not exist build mkdir build
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza de archivos generados para compilar desde cero si es necesario
clean:
	@if exist build rmdir /s /q build
	@if exist $(TARGET) del /q $(TARGET)

.PHONY: all clean