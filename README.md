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
- Tiempo promedio por frame (ms)

Estos datos son utiles para el analisis de rendimiento y la elaboracion de graficas comparativas.

## Resultados experimentales

Se ejecutaron 9 corridas de benchmark (3 tamanos de simulacion x 3 distribuciones espaciales), cada una iniciada desde cero para que el tiempo promedio por frame no se mezcle entre configuraciones. Espacio de simulacion: 1200x800 (1600x1200 para Uniforme-10000), capacidad del QuadTree: 4, radios 2-6, velocidades 50-150.

    | Distribucion | Particulas | Comp. QT | Comp. BF | Candidatos/objeto (QT) | Speedup | Tiempo/frame (ms) |
    |---|---|---|---|---|---|---|
    | Uniforme | 1,000 | 34 | 1,000 | 0.034 | 29.41x | 19.704 |
    | Uniforme | 5,000 | 136 | 5,000 | 0.027 | 36.76x | 93.019 |
    | Uniforme | 10,000 | 107 | 10,000 | 0.011 | 93.46x | 185.714 |
    | Clusters | 1,000 | 32 | 1,000 | 0.032 | 31.25x | 17.884 |
    | Clusters | 5,000 | 91 | 5,000 | 0.018 | 54.95x | 82.807 |
    | Clusters | 10,000 | 263 | 10,000 | 0.026 | 38.02x | 169.656 |
    | Alta densidad | 1,000 | 25 | 1,000 | 0.025 | 40.00x | 18.277 |
    | Alta densidad | 5,000 | 43 | 5,000 | 0.009 | 116.28x | 90.958 |
    | Alta densidad | 10,000 | 123 | 10,000 | 0.012 | 81.30x | 177.625 |

*Candidatos/objeto = ComparacionesQT / Particulas. Tabla completa con todas las columnas en `resultados_benchmark.csv`.*

### Analisis

- **Tiempo por frame**: crece de forma aproximadamente proporcional al numero de particulas (de ~18-20 ms con 1,000 a ~170-186 ms con 10,000), ya que cada frame reconstruye el QuadTree completo ademas de actualizar posiciones y detectar colisiones.
- **Comparaciones (QuadTree vs fuerza bruta)**: la fuerza bruta siempre compara contra el total de particulas (1,000 / 5,000 / 10,000), mientras que el QuadTree se queda en el rango de 25-263 comparaciones en todos los casos. El speedup resultante va de 29x hasta 116x segun el escenario.
- **Candidatos revisados por objeto**: se mantiene muy bajo (entre 0.009 y 0.034) en todos los tamanos, lo que confirma que el QuadTree descarta la gran mayoria de las particulas sin revisarlas una por una, sin importar cuantos objetos haya en total.
- **Efecto de la distribucion espacial**: la distribucion de Alta densidad obtiene el mejor speedup en 5,000 particulas (116.28x), porque la mayoria de las particulas quedan concentradas en una zona muy subdividida y el resto del arbol se descarta rapido. La distribucion de Clusters es la mas exigente para el QuadTree en 10,000 particulas (263 comparaciones), ya que la consulta puede caer cerca de un cluster denso y revisar mas candidatos que en una distribucion uniforme.

## Notas

- El proyecto ha sido desarrollado como parte del curso de Algoritmos y Estructuras de Datos.
- La implementacion del QuadTree es desde cero, sin usar librerias externas para la estructura.
- La comparacion contra fuerza bruta demuestra la eficiencia del QuadTree en consultas espaciales y deteccion de colisiones.

## Licencia

Este proyecto es de uso academico.
