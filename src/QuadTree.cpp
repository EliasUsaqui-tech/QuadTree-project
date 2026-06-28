#include "../include/QuadTree.h"
#include <cmath>

// Rect implementation
QuadTree::Rect::Rect(double x, double y, double w, double h)
    : x(x), y(y), w(w), h(h) {}

bool QuadTree::Rect::contains(double px, double py) const {
    return px >= x && px <= x + w && py >= y && py <= y + h;
}

bool QuadTree::Rect::intersects(const Rect& other) const {
    return !(other.x > x + w || other.x + other.w < x ||
             other.y > y + h || other.y + other.h < y);
}

// QuadTree implementation
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
    if (!boundary.contains(p->x, p->y)) return false;

    if (!divided && particles.size() < capacity) {
        particles.push_back(p);
        return true;
    }

    if (!divided) subdivide();

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

std::vector<Particle*> QuadTree::queryRange(const Rect& range) {
    std::vector<Particle*> found;
    if (!boundary.intersects(range)) return found;

    for (Particle* p : particles) {
        if (range.contains(p->x, p->y)) {
            found.push_back(p);
        }
    }

    if (divided) {
        auto nw = northwest->queryRange(range);
        auto ne = northeast->queryRange(range);
        auto sw = southwest->queryRange(range);
        auto se = southeast->queryRange(range);
        found.insert(found.end(), nw.begin(), nw.end());
        found.insert(found.end(), ne.begin(), ne.end());
        found.insert(found.end(), sw.begin(), sw.end());
        found.insert(found.end(), se.begin(), se.end());
    }
    return found;
}

std::vector<Particle*> QuadTree::queryCircle(double cx, double cy, double radius) {
    Rect range(cx - radius, cy - radius, radius * 2, radius * 2);
    auto candidates = queryRange(range);
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

void QuadTree::draw(sf::RenderWindow& window) const {
    sf::RectangleShape rect(sf::Vector2f(boundary.w, boundary.h));
    rect.setPosition(boundary.x, boundary.y);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color(100, 100, 100, 50));
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
    int count = particles.size();
    if (divided) {
        count += northwest->getComparisons();
        count += northeast->getComparisons();
        count += southwest->getComparisons();
        count += southeast->getComparisons();
    }
    return count;
}