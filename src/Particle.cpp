#include "../include/Particle.h"
#include <cstdlib>

Particle::Particle(int id, double x, double y, double vx, double vy, double radius)
    : id(id), x(x), y(y), vx(vx), vy(vy), radius(radius) {
    color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
}

void Particle::update(double dt, double width, double height) {
    x += vx * dt;
    y += vy * dt;
    
    if (x < 0 || x > width) vx = -vx;
    if (y < 0 || y > height) vy = -vy;
    
    x = std::max(0.0, std::min(x, width));
    y = std::max(0.0, std::min(y, height));
}