# QuadTree Particle Simulator

Simulador de particulas 2D con QuadTree para deteccion eficiente de vecinos y colisiones.

## Caracteristicas

- Tres distribuciones de particulas: uniforme, clusters y alta densidad.
- Consultas espaciales: circular (radio) y rectangular.
- Deteccion de colisiones entre particulas con visualizacion de lineas rojas.
- Insercion de particulas en tiempo real (tecla I y clic derecho).
- Comparacion en tiempo real contra fuerza bruta (comparaciones, speedup).
- Benchmark automatico con guardado en CSV (resultados_benchmark.csv).
- Zoom y desplazamiento de la vista (teclas +, -, Home y arrastre con rueda del mouse).
- Visualizacion de subdivisiones del QuadTree.
- Interfaz grafica con SFML.

## Requisitos

- C++17
- SFML 2.5 o superior
- CMake 3.10 o superior

## Controles

| Tecla / Accion | Funcion |
|----------------|---------|
| 1 | Distribucion uniforme |
| 2 | Distribucion con clusters |
| 3 | Distribucion de alta densidad |
| Q | Activar/desactivar modo consulta circular (clic izquierdo para posicionar) |
| R | Activar/desactivar modo consulta rectangular (dos clics para definir esquinas) |
| Shift + R | Reconstruir QuadTree manualmente |
| I | Insertar 1 particula aleatoria |
| Shift + I | Insertar 10 particulas aleatorias |
| C | Mostrar/ocultar lineas de colisiones |
| B | Ejecutar benchmark y guardar resultados en CSV |
| P | Pausar/Reanudar simulacion |
| + o = | Acercar zoom |
| - | Alejar zoom |
| Home | Restablecer vista normal |
| Clic izquierdo | Posicionar consulta circular / definir esquina de rectangulo |
| Clic derecho | Insertar 5 particulas alrededor del cursor |
| Clic medio (rueda) + arrastrar | Mover la vista |
| Rueda del mouse | Zoom in / zoom out |
| ESC | Salir |

## Compilacion

### Linux / macOS

mkdir build && cd build
cmake ..
make
./quadtree_simulator

### Windows (con MinGW y MSYS2)

Asegurate de tener SFML instalado (por ejemplo, con pacman en MSYS2):

pacman -S mingw-w64-x86_64-sfml

Luego, desde la terminal de MSYS2:

mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
./quadtree_simulator.exe

### Windows (con CLion)

1. Abre el proyecto en CLion.
2. Configura el toolchain con MinGW (recomendado MSYS2).
3. CLion detectara CMakeLists.txt y compilara automaticamente.
4. Haz clic en Run.

## Estructura del Proyecto

quadtree_simulator/
    include/
        Particle.h
        QuadTree.h
        Simulator.h
        Renderer.h
    src/
        main.cpp
        Particle.cpp
        QuadTree.cpp
        Simulator.cpp
        Renderer.cpp
    CMakeLists.txt
    README.md

## Resultados del Benchmark

Al presionar B se ejecuta un benchmark completo y se guardan los datos en resultados_benchmark.csv. El archivo contiene las siguientes columnas:

- Distribucion
- Particulas
- Espacio (ancho x alto)
- Capacidad del QuadTree
- Radio minimo y maximo
- Velocidad minima y maxima
- Tiempo de construccion (ms)
- Tiempo de consulta con QuadTree (ms)
- Tiempo de busqueda ingenua (ms)
- Comparaciones QuadTree
- Comparaciones Fuerza Bruta
- Nodos visitados
- Resultados encontrados
- Speedup
- Colisiones detectadas con QuadTree
- Colisiones detectadas con Fuerza Bruta

Estos datos son utiles para el analisis de rendimiento y la elaboracion de graficas comparativas.

## Notas

- El proyecto ha sido desarrollado como parte del curso de Algoritmos y Estructuras de Datos.
- La implementacion del QuadTree es desde cero, sin usar librerias externas para la estructura.
- La comparacion contra fuerza bruta demuestra la eficiencia del QuadTree en consultas espaciales y deteccion de colisiones.

## Licencia

Este proyecto es de uso academico.
