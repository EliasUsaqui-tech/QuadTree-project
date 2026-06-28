# QuadTree Particle Simulator

Simulador de partículas 2D con QuadTree para detección eficiente de vecinos.

## Características
- 3 distribuciones de partículas (uniforme, clusters, alta densidad)
- Consultas por círculo con resaltado de resultados
- Comparación en tiempo real con fuerza bruta
- Benchmark automático con diferentes tamaños

## Requisitos
- C++17
- SFML 2.5+

## Controles
- `1` - Distribución uniforme
- `2` - Distribución con clusters
- `3` - Distribución de alta densidad
- `Q` - Activar/desactivar modo consulta
- `R` - Reconstruir QuadTree
- `Clic izquierdo` - Posicionar consulta
- `ESC` - Salir

## Compilación
```bash
mkdir build && cd build
cmake ..
make
./quadtree_simulator