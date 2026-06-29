#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Particle.h"

class QuadTree {
public:
    struct Rect {
        double x, y, w, h;
        Rect(double x = 0, double y = 0, double w = 0, double h = 0);
        bool contains(double px, double py) const;
        bool intersects(const Rect& other) const;
    };

    QuadTree(const Rect& boundary, int capacity);
    
    bool insert(Particle* p);
    void clear();

    std::vector<Particle*> queryRange(const Rect& range, int& comparisons);
    std::vector<Particle*> queryCircle(double cx, double cy, double radius, int& comparisons);

    // Detecta colisiones usando un multiplicador sobre la suma de radios
    std::vector<std::pair<Particle*, Particle*>> detectCollisions(double multiplier);

    void draw(sf::RenderWindow& window) const;
    int getTotalNodes() const;
    int getComparisons() const;

private:
    Rect boundary;
    int capacity;
    std::vector<Particle*> particles;
    bool divided;
    std::unique_ptr<QuadTree> northwest;
    std::unique_ptr<QuadTree> northeast;
    std::unique_ptr<QuadTree> southwest;
    std::unique_ptr<QuadTree> southeast;

    void subdivide();
};