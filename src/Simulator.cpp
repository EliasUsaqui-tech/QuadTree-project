#include "../include/Simulator.h"
#include <chrono>
#include <cmath>
#include <iostream>

Simulator::Simulator(double width, double height, int numParticles, int capacity)
    : width(width), height(height), numParticles(numParticles), 
      quadTreeCapacity(capacity),
      minRadius(2.0), maxRadius(6.0),
      minSpeed(50.0), maxSpeed(150.0),
      rng(std::random_device{}()) {
    particles.reserve(numParticles * 2);
    particlePtrs.reserve(numParticles * 2);
}

// ============================================
// CONFIGURAR PARAMETROS DE PARTICULAS
// ============================================
void Simulator::setParticleParams(double minR, double maxR, double minS, double maxS) {
    minRadius = minR;
    maxRadius = maxR;
    minSpeed = minS;
    maxSpeed = maxS;

    std::cout << "Parametros de particulas actualizados:\n";
    std::cout << "   Radio: " << minRadius << " - " << maxRadius << "\n";
    std::cout << "   Velocidad: " << minSpeed << " - " << maxSpeed << "\n\n";
}

// ============================================
// DISTRIBUCION UNIFORME
// ============================================
void Simulator::generateUniform() {
    particles.clear();
    particlePtrs.clear();

    std::uniform_real_distribution<double> distX(0, width);
    std::uniform_real_distribution<double> distY(0, height);
    std::uniform_real_distribution<double> distV(minSpeed, maxSpeed);
    std::uniform_real_distribution<double> distR(minRadius, maxRadius);

    for (int i = 0; i < numParticles; ++i) {
        double vx = distV(rng) * (rng() % 2 == 0 ? 1 : -1);
        double vy = distV(rng) * (rng() % 2 == 0 ? 1 : -1);

        particles.emplace_back(i, distX(rng), distY(rng),
                               vx, vy, distR(rng));
        particlePtrs.push_back(&particles.back());
    }

    std::cout << "Generada distribucion uniforme\n";
    std::cout << "   Velocidad: " << minSpeed << "-" << maxSpeed << "\n";
}

// ============================================
// DISTRIBUCION CON CLUSTERS
// ============================================
void Simulator::generateClusters(int numClusters) {
    particles.clear();
    particlePtrs.clear();

    std::uniform_real_distribution<double> distV(minSpeed, maxSpeed);
    std::uniform_real_distribution<double> distR(minRadius, maxRadius);

    std::vector<std::pair<double, double>> centers;
    for (int i = 0; i < numClusters; ++i) {
        centers.emplace_back(
            std::uniform_real_distribution<double>(width*0.1, width*0.9)(rng),
            std::uniform_real_distribution<double>(height*0.1, height*0.9)(rng)
        );
    }

    int perCluster = numParticles / numClusters;
    for (int i = 0; i < numParticles; ++i) {
        int cluster = i % numClusters;
        std::normal_distribution<double> distX(centers[cluster].first, width * 0.05);
        std::normal_distribution<double> distY(centers[cluster].second, height * 0.05);

        double vx = distV(rng) * (rng() % 2 == 0 ? 1 : -1);
        double vy = distV(rng) * (rng() % 2 == 0 ? 1 : -1);

        particles.emplace_back(i, distX(rng), distY(rng),
                               vx, vy, distR(rng));
        particlePtrs.push_back(&particles.back());
    }

    std::cout << "Generada distribucion con clusters\n";
    std::cout << "   Velocidad: " << minSpeed << "-" << maxSpeed << "\n";
}

// ============================================
// DISTRIBUCION DE ALTA DENSIDAD
// ============================================
void Simulator::generateHighDensity() {
    particles.clear();
    particlePtrs.clear();

    std::uniform_real_distribution<double> distV(minSpeed, maxSpeed);
    std::uniform_real_distribution<double> distR(minRadius, maxRadius);

    double centerX = width * 0.5;
    double centerY = height * 0.5;

    for (int i = 0; i < numParticles; ++i) {
        double x, y;
        if (i < numParticles * 0.7) {
            std::normal_distribution<double> distX(centerX, width * 0.05);
            std::normal_distribution<double> distY(centerY, height * 0.05);
            x = distX(rng);
            y = distY(rng);
        } else {
            std::uniform_real_distribution<double> distX(0, width);
            std::uniform_real_distribution<double> distY(0, height);
            x = distX(rng);
            y = distY(rng);
        }

        double vx = distV(rng) * (rng() % 2 == 0 ? 1 : -1);
        double vy = distV(rng) * (rng() % 2 == 0 ? 1 : -1);

        particles.emplace_back(i, x, y, vx, vy, distR(rng));
        particlePtrs.push_back(&particles.back());
    }

    std::cout << "Generada distribucion de alta densidad\n";
    std::cout << "   Velocidad: " << minSpeed << "-" << maxSpeed << "\n";
}

// ============================================
// INSERTAR PARTICULA INDIVIDUAL
// ============================================
void Simulator::addParticle(double x, double y, double vx, double vy, double radius) {
    int newId = particles.size();
    particles.emplace_back(newId, x, y, vx, vy, radius);
    particlePtrs.push_back(&particles.back());
    numParticles++;

    if (quadtree) {
        quadtree->insert(&particles.back());
    }

    std::cout << "Particula " << newId << " insertada en (" << x << ", " << y << ")\n";
}

// ============================================
// INSERTAR PARTICULA ALEATORIA
// ============================================
void Simulator::addRandomParticle() {
    std::uniform_real_distribution<double> distX(0, width);
    std::uniform_real_distribution<double> distY(0, height);
    std::uniform_real_distribution<double> distV(minSpeed, maxSpeed);
    std::uniform_real_distribution<double> distR(minRadius, maxRadius);

    double x = distX(rng);
    double y = distY(rng);
    double vx = distV(rng) * (rng() % 2 == 0 ? 1 : -1);
    double vy = distV(rng) * (rng() % 2 == 0 ? 1 : -1);
    double radius = distR(rng);

    addParticle(x, y, vx, vy, radius);
}

// ============================================
// INSERTAR MULTIPLES PARTICULAS EN UNA POSICION
// ============================================
void Simulator::addParticlesAtMouse(double mx, double my, int count) {
    std::uniform_real_distribution<double> distV(minSpeed, maxSpeed);
    std::uniform_real_distribution<double> distR(minRadius, maxRadius);
    std::uniform_real_distribution<double> distOffset(0, 20);

    for (int i = 0; i < count; ++i) {
        double x = mx + distOffset(rng) * (rng() % 2 == 0 ? 1 : -1);
        double y = my + distOffset(rng) * (rng() % 2 == 0 ? 1 : -1);

        x = std::max(0.0, std::min(x, width));
        y = std::max(0.0, std::min(y, height));

        double vx = distV(rng) * (rng() % 2 == 0 ? 1 : -1);
        double vy = distV(rng) * (rng() % 2 == 0 ? 1 : -1);
        double radius = distR(rng);

        addParticle(x, y, vx, vy, radius);
    }

    std::cout << count << " particulas insertadas cerca del mouse\n";
}

// ============================================
// ACTUALIZAR POSICIONES
// ============================================
void Simulator::update(double dt) {
    for (auto& p : particles) {
        p.update(dt, width, height);
    }
}

// ============================================
// CONSTRUIR QUADTREE
// ============================================
void Simulator::buildQuadTree() {
    quadtree = std::make_unique<QuadTree>(
        QuadTree::Rect(0, 0, width, height), quadTreeCapacity);
    for (Particle* p : particlePtrs) {
        quadtree->insert(p);
    }
}

// ============================================
// CONSULTA CON QUADTREE
// ============================================
std::vector<Particle*> Simulator::queryCircle(double x, double y, double radius, int& comparisons) {
    comparisons = 0;
    if (!quadtree) return {};
    return quadtree->queryCircle(x, y, radius, comparisons);
}

// ============================================
// CONSULTA CON FUERZA BRUTA
// ============================================
std::vector<Particle*> Simulator::bruteForceCircle(double cx, double cy, double radius) {
    std::vector<Particle*> found;
    double r2 = radius * radius;
    for (Particle* p : particlePtrs) {
        double dx = p->x - cx;
        double dy = p->y - cy;
        if (dx*dx + dy*dy <= r2) {
            found.push_back(p);
        }
    }
    return found;
}

// ============================================
// DETECCION DE COLISIONES CON QUADTREE
// ============================================
std::vector<std::pair<Particle*, Particle*>> Simulator::detectCollisions(double radius) {
    if (!quadtree) return {};
    return quadtree->detectCollisions(radius);
}

// ============================================
// DETECCION DE COLISIONES CON FUERZA BRUTA
// ============================================
std::vector<std::pair<Particle*, Particle*>> Simulator::bruteForceCollisions(double radius) {
    std::vector<std::pair<Particle*, Particle*>> collisions;
    double r2 = radius * radius;
    for (size_t i = 0; i < particlePtrs.size(); ++i) {
        for (size_t j = i + 1; j < particlePtrs.size(); ++j) {
            double dx = particlePtrs[i]->x - particlePtrs[j]->x;
            double dy = particlePtrs[i]->y - particlePtrs[j]->y;
            if (dx*dx + dy*dy < r2) {
                collisions.push_back({particlePtrs[i], particlePtrs[j]});
            }
        }
    }
    return collisions;
}

// ============================================
// GETTERS
// ============================================
const std::vector<Particle>& Simulator::getParticles() const { return particles; }
QuadTree* Simulator::getQuadTree() const { return quadtree.get(); }
double Simulator::getWidth() const { return width; }
double Simulator::getHeight() const { return height; }
int Simulator::getNumParticles() const { return numParticles; }

// ============================================
// BENCHMARK
// ============================================
Simulator::BenchmarkResult Simulator::benchmarkQuery(double x, double y, double radius) {
    BenchmarkResult result;

    auto start = std::chrono::high_resolution_clock::now();
    buildQuadTree();
    auto end = std::chrono::high_resolution_clock::now();
    result.buildTime = std::chrono::duration<double>(end - start).count();

    int comparisons = 0;
    start = std::chrono::high_resolution_clock::now();
    auto qtResult = queryCircle(x, y, radius, comparisons);
    end = std::chrono::high_resolution_clock::now();
    result.queryTime = std::chrono::duration<double>(end - start).count();
    result.results = qtResult.size();
    result.comparisons = comparisons;
    result.nodesVisited = quadtree ? quadtree->getTotalNodes() : 0;

    return result;
}