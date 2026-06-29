#pragma once
#include <vector>
#include <memory>
#include <random>
#include "Particle.h"
#include "QuadTree.h"

class Simulator {
private:
    // Dimensiones del espacio de simulacion
    double width, height;

    // Numero total de particulas
    int numParticles;

    // Capacidad maxima por nodo del QuadTree
    int quadTreeCapacity;

    // Parametros configurables de particulas
    double minRadius;   // Radio minimo
    double maxRadius;   // Radio maximo
    double minSpeed;    // Velocidad minima
    double maxSpeed;    // Velocidad maxima

    // Almacenamiento de particulas
    std::vector<Particle> particles;        // Vector principal de particulas
    std::vector<Particle*> particlePtrs;    // Punteros a particulas para el QuadTree

    // Estructura de datos principal
    std::unique_ptr<QuadTree> quadtree;

    // Generador de numeros aleatorios
    std::mt19937 rng;

public:
    // Constructor: inicializa la simulacion con dimensiones y cantidad de particulas
    Simulator(double width, double height, int numParticles, int capacity = 4);

    // Configura los parametros de las particulas (radio y velocidad)
    void setParticleParams(double minR, double maxR, double minS, double maxS);

    // Genera diferentes distribuciones espaciales de particulas
    void generateUniform();             // Distribucion uniforme
    void generateClusters(int numClusters = 3);  // Distribucion con clusters
    void generateHighDensity();         // Distribucion con alta densidad en el centro

    // Insercion de particulas en tiempo real (durante la simulacion)
    void addParticle(double x, double y, double vx, double vy, double radius);  // Insertar una particula especifica
    void addRandomParticle();           // Insertar una particula aleatoria
    void addParticlesAtMouse(double mx, double my, int count = 10);  // Insertar varias cerca del mouse

    // Actualiza las posiciones de todas las particulas (frame por frame)
    void update(double dt);

    // Reconstruye el QuadTree con las posiciones actuales
    void buildQuadTree();

    // Consultas espaciales
    std::vector<Particle*> queryCircle(double x, double y, double radius, int& comparisons);  // Busqueda por circulo (QuadTree)
    std::vector<Particle*> bruteForceCircle(double cx, double cy, double radius);  // Busqueda por circulo (fuerza bruta)

    // Deteccion de colisiones
    std::vector<std::pair<Particle*, Particle*>> detectCollisions(double radius);  // Colisiones con QuadTree
    std::vector<std::pair<Particle*, Particle*>> bruteForceCollisions(double radius);  // Colisiones con fuerza bruta

    // Getters
    const std::vector<Particle>& getParticles() const;
    QuadTree* getQuadTree() const;
    double getWidth() const;
    double getHeight() const;
    int getNumParticles() const;

    // Getters para parametros de particulas (inline)
    double getMinRadius() const { return minRadius; }
    double getMaxRadius() const { return maxRadius; }
    double getMinSpeed() const { return minSpeed; }
    double getMaxSpeed() const { return maxSpeed; }

    // Estructura para resultados de benchmark
    struct BenchmarkResult {
        double buildTime;      // Tiempo de construccion del QuadTree
        double queryTime;      // Tiempo de consulta
        int comparisons;       // Numero de comparaciones realizadas
        int nodesVisited;      // Nodos visitados en el QuadTree
        int results;           // Cantidad de resultados encontrados
    };

    // Ejecuta un benchmark completo (construccion + consulta)
    BenchmarkResult benchmarkQuery(double x, double y, double radius);
};