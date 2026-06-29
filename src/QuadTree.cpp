#include "../include/QuadTree.h"
#include <cmath>
#include <iostream>

QuadTree::Rect::Rect(double x, double y, double w, double h)
    : x(x), y(y), w(w), h(h) {}

bool QuadTree::Rect::contains(double px, double py) const {
    return px >= x && px <= x + w && py >= y && py <= y + h;
}

bool QuadTree::Rect::intersects(const Rect& other) const {
    return !(other.x > x + w || other.x + other.w < x ||
             other.y > y + h || other.y + other.h < y);
}

QuadTree::QuadTree(const Rect& boundary, int capacity)
    : boundary(boundary), capacity(capacity), divided(false) {}

void QuadTree::subdivide() {
    double half_w = boundary.w / 2;
    double half_h = boundary.h / 2;
    double cx = boundary.x;
    double cy = boundary.y;

    northwest = std::make_unique<QuadTree>(
        Rect(cx, cy, half_w, half_h), capacity);
    northeast = std::make_unique<QuadTree>(
        Rect(cx + half_w, cy, half_w, half_h), capacity);
    southwest = std::make_unique<QuadTree>(
        Rect(cx, cy + half_h, half_w, half_h), capacity);
    southeast = std::make_unique<QuadTree>(
        Rect(cx + half_w, cy + half_h, half_w, half_h), capacity);
    divided = true;
}

bool QuadTree::insert(Particle* p) {
    if (!boundary.contains(p->x, p->y)) {
        return false;
    }

    if (divided) {
        return northwest->insert(p) || northeast->insert(p) ||
               southwest->insert(p) || southeast->insert(p);
    }

    if (particles.size() < capacity) {
        particles.push_back(p);
        return true;
    }

    subdivide();

    std::vector<Particle*> oldParticles = particles;
    particles.clear();

    for (Particle* oldP : oldParticles) {
        bool inserted = northwest->insert(oldP) || northeast->insert(oldP) ||
                        southwest->insert(oldP) || southeast->insert(oldP);
        if (!inserted) {
            std::cout << "Error: Particula " << oldP->id << " no se pudo insertar\n";
        }
    }

    return northwest->insert(p) || northeast->insert(p) ||
           southwest->insert(p) || southeast->insert(p);
}

void QuadTree::clear() {
    particles.clear();
    if (divided) {
        northwest->clear();
        northeast->clear();
        southwest->clear();
        southeast->clear();
        divided = false;
    }
}

std::vector<Particle*> QuadTree::queryRange(const Rect& range, int& comparisons) {
    std::vector<Particle*> found;
    if (!boundary.intersects(range)) return found;

    comparisons++;

    if (!divided) {
        for (Particle* p : particles) {
            comparisons++;
            if (range.contains(p->x, p->y)) {
                found.push_back(p);
            }
        }
    } else {
        auto nw = northwest->queryRange(range, comparisons);
        auto ne = northeast->queryRange(range, comparisons);
        auto sw = southwest->queryRange(range, comparisons);
        auto se = southeast->queryRange(range, comparisons);
        found.insert(found.end(), nw.begin(), nw.end());
        found.insert(found.end(), ne.begin(), ne.end());
        found.insert(found.end(), sw.begin(), sw.end());
        found.insert(found.end(), se.begin(), se.end());
    }
    return found;
}

std::vector<Particle*> QuadTree::queryCircle(double cx, double cy, double radius, int& comparisons) {
    Rect range(cx - radius, cy - radius, radius * 2, radius * 2);
    auto candidates = queryRange(range, comparisons);
    std::vector<Particle*> result;
    double r2 = radius * radius;
    for (Particle* p : candidates) {
        double dx = p->x - cx;
        double dy = p->y - cy;
        if (dx*dx + dy*dy <= r2) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<std::pair<Particle*, Particle*>> QuadTree::detectCollisions(double multiplier) {
    std::vector<std::pair<Particle*, Particle*>> collisions;

    if (!divided) {
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                double dx = particles[i]->x - particles[j]->x;
                double dy = particles[i]->y - particles[j]->y;
                double dist = std::sqrt(dx*dx + dy*dy);
                double threshold = (particles[i]->radius + particles[j]->radius) * multiplier;
                if (dist < threshold) {
                    collisions.push_back({particles[i], particles[j]});
                }
            }
        }
    } else {
        auto nw = northwest->detectCollisions(multiplier);
        auto ne = northeast->detectCollisions(multiplier);
        auto sw = southwest->detectCollisions(multiplier);
        auto se = southeast->detectCollisions(multiplier);
        collisions.insert(collisions.end(), nw.begin(), nw.end());
        collisions.insert(collisions.end(), ne.begin(), ne.end());
        collisions.insert(collisions.end(), sw.begin(), sw.end());
        collisions.insert(collisions.end(), se.begin(), se.end());
    }
    return collisions;
}

void QuadTree::draw(sf::RenderWindow& window) const {
    sf::RectangleShape rect(sf::Vector2f(boundary.w, boundary.h));
    rect.setPosition(boundary.x, boundary.y);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color(100, 100, 100, 100));
    rect.setOutlineThickness(0.5f);
    window.draw(rect);

    if (divided) {
        northwest->draw(window);
        northeast->draw(window);
        southwest->draw(window);
        southeast->draw(window);
    }
}

int QuadTree::getTotalNodes() const {
    int count = 1;
    if (divided) {
        count += northwest->getTotalNodes();
        count += northeast->getTotalNodes();
        count += southwest->getTotalNodes();
        count += southeast->getTotalNodes();
    }
    return count;
}

int QuadTree::getComparisons() const {
    return 0;
}