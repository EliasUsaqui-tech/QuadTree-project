#include "../include/Particle.h"
#include <cstdlib>
#include <algorithm>

Particle::Particle(int id, double x, double y, double vx, double vy, double radius)
    : id(id), x(x), y(y), vx(vx), vy(vy), radius(radius),
      isColliding(false), collisionTimer(0.0f) {
    color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
}

void Particle::update(double dt, double width, double height) {
    x += vx * dt;
    y += vy * dt;

    if (x < 0 || x > width) vx = -vx;
    if (y < 0 || y > height) vy = -vy;

    x = std::max(0.0, std::min(x, width));
    y = std::max(0.0, std::min(y, height));

    if (collisionTimer > 0.0f) {
        collisionTimer -= dt;
        if (collisionTimer <= 0.0f) {
            collisionTimer = 0.0f;
            isColliding = false;
        }
    }
}

void Particle::setColliding(float duration) {
    isColliding = true;
    collisionTimer = duration;
}