#include "../include/QuadTree.h"
#include <cmath>
#include <iostream>

// Constructor del rectangulo
QuadTree::Rect::Rect(double x, double y, double w, double h)
    : x(x), y(y), w(w), h(h) {}

// Verifica si un punto esta dentro del rectangulo
bool QuadTree::Rect::contains(double px, double py) const {
    return px >= x && px <= x + w && py >= y && py <= y + h;
}

// Verifica si dos rectangulos se intersectan
bool QuadTree::Rect::intersects(const Rect& other) const {
    return !(other.x > x + w || other.x + other.w < x ||
             other.y > y + h || other.y + other.h < y);
}

// Constructor del QuadTree
QuadTree::QuadTree(const Rect& boundary, int capacity)
    : boundary(boundary), capacity(capacity), divided(false) {}

// Divide el nodo actual en 4 subnodos (NW, NE, SW, SE)
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

// Inserta una particula en el QuadTree
bool QuadTree::insert(Particle* p) {
    // Si la particula no esta dentro del nodo, no se inserta
    if (!boundary.contains(p->x, p->y)) {
        return false;
    }

    // Si el nodo esta dividido, insertar en los hijos
    if (divided) {
        return northwest->insert(p) || northeast->insert(p) ||
               southwest->insert(p) || southeast->insert(p);
    }

    // Si el nodo no esta lleno, insertar aqui
    if (particles.size() < capacity) {
        particles.push_back(p);
        return true;
    }

    // Nodo lleno: subdividir y redistribuir particulas
    subdivide();

    std::vector<Particle*> oldParticles = particles;
    particles.clear();

    // Reinsertar las particulas existentes en los hijos
    for (Particle* oldP : oldParticles) {
        bool inserted = northwest->insert(oldP) || northeast->insert(oldP) ||
                        southwest->insert(oldP) || southeast->insert(oldP);
        if (!inserted) {
            std::cout << "Error: Particula " << oldP->id << " no se pudo insertar\n";
        }
    }

    // Insertar la nueva particula
    return northwest->insert(p) || northeast->insert(p) ||
           southwest->insert(p) || southeast->insert(p);
}

// Elimina todas las particulas y subnodos
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

// Busca particulas dentro de un rectangulo (consulta por rango)
std::vector<Particle*> QuadTree::queryRange(const Rect& range, int& comparisons) {
    std::vector<Particle*> found;
    // Si el nodo no intersecta con el rango, no hay resultados
    if (!boundary.intersects(range)) return found;

    comparisons++; // Contar este nodo como visitado

    if (!divided) {
        // Nodo hoja: revisar cada particula
        for (Particle* p : particles) {
            comparisons++; // Contar particula revisada
            if (range.contains(p->x, p->y)) {
                found.push_back(p);
            }
        }
    } else {
        // Nodo interno: buscar recursivamente en los hijos
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

// Busca particulas dentro de un circulo (consulta circular)
std::vector<Particle*> QuadTree::queryCircle(double cx, double cy, double radius, int& comparisons) {
    // Crear un rectangulo que encierra al circulo
    Rect range(cx - radius, cy - radius, radius * 2, radius * 2);
    auto candidates = queryRange(range, comparisons);
    std::vector<Particle*> result;
    double r2 = radius * radius;
    // Filtrar solo las particulas que estan dentro del circulo exacto
    for (Particle* p : candidates) {
        double dx = p->x - cx;
        double dy = p->y - cy;
        if (dx*dx + dy*dy <= r2) {
            result.push_back(p);
        }
    }
    return result;
}

// Deteccion de colisiones entre particulas dentro del mismo nodo
std::vector<std::pair<Particle*, Particle*>> QuadTree::detectCollisions(double radius) {
    std::vector<std::pair<Particle*, Particle*>> collisions;

    if (!divided) {
        // Nodo hoja: revisar todos los pares de particulas
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                double dx = particles[i]->x - particles[j]->x;
                double dy = particles[i]->y - particles[j]->y;
                double dist = sqrt(dx*dx + dy*dy);
                if (dist < radius) {
                    collisions.push_back({particles[i], particles[j]});
                }
            }
        }
    } else {
        // Revisar colisiones dentro de cada hijo
        auto nw = northwest->detectCollisions(radius);
        auto ne = northeast->detectCollisions(radius);
        auto sw = southwest->detectCollisions(radius);
        auto se = southeast->detectCollisions(radius);

        collisions.insert(collisions.end(), nw.begin(), nw.end());
        collisions.insert(collisions.end(), ne.begin(), ne.end());
        collisions.insert(collisions.end(), sw.begin(), sw.end());
        collisions.insert(collisions.end(), se.begin(), se.end());

    }
    return collisions;
}

// Dibuja las subdivisiones del QuadTree
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

// Retorna el numero total de nodos en el arbol
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

// Metodo obsoleto (se usa el contador por referencia en queryRange)
int QuadTree::getComparisons() const {
    return 0;
}