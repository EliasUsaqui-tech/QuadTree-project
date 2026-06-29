#pragma once
#include <SFML/Graphics.hpp>

struct Particle {
    int id;
    double x, y;
    double vx, vy;
    double radius;
    sf::Color color;
    sf::Color originalColor;
    float glowTime;
    bool isColliding;

    float collisionTimer;

    void setColliding(float duration = 0.5f);

    Particle(int id = 0, double x = 0, double y = 0, 
             double vx = 0, double vy = 0, double radius = 3.0);

    void update(double dt, double width, double height);
    void startGlow(float duration);
};