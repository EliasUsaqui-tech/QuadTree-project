#include "../include/Simulator.h"
#include <chrono>
#include <cmath>

Simulator::Simulator(double width, double height, int numParticles, int capacity)
    : width(width), height(height), numParticles(numParticles), 
      quadTreeCapacity(capacity), rng(std::random_device{}()) {
    particles.reserve(numParticles);
    particlePtrs.reserve(numParticles);
}

void Simulator::generateUniform() {
    particles.clear();
    particlePtrs.clear();
    std::uniform_real_distribution<double> distX(0, width);
    std::uniform_real_distribution<double> distY(0, height);
    std::uniform_real_distribution<double> distV(-100, 100);
    std::uniform_real_distribution<double> distR(2, 6);

    for (int i = 0; i < numParticles; ++i) {
        particles.emplace_back(i, distX(rng), distY(rng), 
                               distV(rng), distV(rng), distR(rng));
        particlePtrs.push_back(&particles.back());
    }
}

void Simulator::generateClusters(int numClusters) {
    particles.clear();
    particlePtrs.clear();
    std::uniform_real_distribution<double> distV(-100, 100);
    std::uniform_real_distribution<double> distR(2, 6);
    
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
        
        particles.emplace_back(i, distX(rng), distY(rng), 
                               distV(rng), distV(rng), distR(rng));
        particlePtrs.push_back(&particles.back());
    }
}

void Simulator::generateHighDensity() {
    particles.clear();
    particlePtrs.clear();
    std::uniform_real_distribution<double> distV(-100, 100);
    std::uniform_real_distribution<double> distR(2, 6);

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
        particles.emplace_back(i, x, y, distV(rng), distV(rng), distR(rng));
        particlePtrs.push_back(&particles.back());
    }
}

void Simulator::update(double dt) {
    for (auto& p : particles) {
        p.update(dt, width, height);
    }
}

void Simulator::buildQuadTree() {
    quadtree = std::make_unique<QuadTree>(
        QuadTree::Rect(0, 0, width, height), quadTreeCapacity);
    for (Particle* p : particlePtrs) {
        quadtree->insert(p);
    }
}

std::vector<Particle*> Simulator::queryCircle(double x, double y, double radius) {
    if (!quadtree) return {};
    return quadtree->queryCircle(x, y, radius);
}

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

const std::vector<Particle>& Simulator::getParticles() const { return particles; }
QuadTree* Simulator::getQuadTree() const { return quadtree.get(); }
double Simulator::getWidth() const { return width; }
double Simulator::getHeight() const { return height; }
int Simulator::getNumParticles() const { return numParticles; }

Simulator::BenchmarkResult Simulator::benchmarkQuery(double x, double y, double radius) {
    BenchmarkResult result;
    
    auto start = std::chrono::high_resolution_clock::now();
    buildQuadTree();
    auto end = std::chrono::high_resolution_clock::now();
    result.buildTime = std::chrono::duration<double>(end - start).count();
    
    start = std::chrono::high_resolution_clock::now();
    auto qtResult = queryCircle(x, y, radius);
    end = std::chrono::high_resolution_clock::now();
    result.queryTime = std::chrono::duration<double>(end - start).count();
    result.results = qtResult.size();
    result.comparisons = quadtree ? quadtree->getComparisons() : 0;
    result.nodesVisited = quadtree ? quadtree->getTotalNodes() : 0;
    
    return result;
}