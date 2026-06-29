#pragma once
#include <vector>
#include <memory>
#include <random>
#include "Particle.h"
#include "QuadTree.h"

class Simulator {
private:
    double width, height;
    int numParticles;
    int quadTreeCapacity;

    double minRadius;
    double maxRadius;
    double minSpeed;
    double maxSpeed;

    std::vector<Particle> particles;
    std::vector<Particle*> particlePtrs;
    std::unique_ptr<QuadTree> quadtree;
    std::mt19937 rng;

public:
    Simulator(double width, double height, int numParticles, int capacity = 4);

    void setParticleParams(double minR, double maxR, double minS, double maxS);

    void generateUniform();
    void generateClusters(int numClusters = 3);
    void generateHighDensity();

    void addParticle(double x, double y, double vx, double vy, double radius);
    void addRandomParticle();
    void addParticlesAtMouse(double mx, double my, int count = 10);

    void update(double dt);
    void buildQuadTree();

    std::vector<Particle*> queryCircle(double x, double y, double radius, int& comparisons);
    std::vector<Particle*> bruteForceCircle(double cx, double cy, double radius);
    std::vector<std::pair<Particle*, Particle*>> bruteForceCollisions(double radius);


    // Colisiones reales (multiplicador 1.0)
    std::vector<std::pair<Particle*, Particle*>> detectCollisions(double radius);
    // Posibles colisiones (multiplicador 1.8)
    std::vector<std::pair<Particle*, Particle*>> detectPossibleCollisions(double radius);

    void updateCollisionStatus(double radius);

    const std::vector<Particle>& getParticles() const;
    QuadTree* getQuadTree() const;
    double getWidth() const;
    double getHeight() const;
    int getNumParticles() const;

    double getMinRadius() const { return minRadius; }
    double getMaxRadius() const { return maxRadius; }
    double getMinSpeed() const { return minSpeed; }
    double getMaxSpeed() const { return maxSpeed; }

    struct BenchmarkResult {
        double buildTime;
        double queryTime;
        int comparisons;
        int nodesVisited;
        int results;
    };

    BenchmarkResult benchmarkQuery(double x, double y, double radius);
};