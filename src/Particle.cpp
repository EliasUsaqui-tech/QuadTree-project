#include "../include/Particle.h"
#include <cstdlib>

// Constructor de la particula
Particle::Particle(int id, double x, double y, double vx, double vy, double radius)
    : id(id), x(x), y(y), vx(vx), vy(vy), radius(radius) {
    // Asigna un color aleatorio (RGB)
    color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
}

// Actualiza la posicion de la particula segun su velocidad y el tiempo transcurrido
void Particle::update(double dt, double width, double height) {
    // Movimiento con velocidad constante
    x += vx * dt;
    y += vy * dt;

    // Rebote en los bordes (invierte velocidad si toca los limites)
    if (x < 0 || x > width) vx = -vx;
    if (y < 0 || y > height) vy = -vy;

    // Asegura que la particula no se salga de los limites
    x = std::max(0.0, std::min(x, width));
    y = std::max(0.0, std::min(y, height));
}