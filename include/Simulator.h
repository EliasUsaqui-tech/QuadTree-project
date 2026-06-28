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
    std::vector<Particle> particles;
    std::vector<Particle*> particlePtrs;
    std::unique_ptr<QuadTree> quadtree;
    std::mt19937 rng;

public:
    Simulator(double width, double height, int numParticles, int capacity = 4);

    void generateUniform();
    void generateClusters(int numClusters = 3);
    void generateHighDensity();
    void update(double dt);
    void buildQuadTree();
    
    std::vector<Particle*> queryCircle(double x, double y, double radius);
    std::vector<Particle*> bruteForceCircle(double cx, double cy, double radius);
    
    const std::vector<Particle>& getParticles() const;
    QuadTree* getQuadTree() const;
    double getWidth() const;
    double getHeight() const;
    int getNumParticles() const;

    struct BenchmarkResult {
        double buildTime;
        double queryTime;
        int comparisons;
        int nodesVisited;
        int results;
    };

    BenchmarkResult benchmarkQuery(double x, double y, double radius);
};