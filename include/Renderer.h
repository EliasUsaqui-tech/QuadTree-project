#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Particle.h"
#include "QuadTree.h"

class Renderer {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text infoText;
    sf::CircleShape queryCircle;
    bool showQuadTree = true;

public:
    Renderer(int width, int height, const std::string& title);
    
    void clear();
    void drawParticle(const Particle& p, bool highlighted = false);
    void drawParticles(const std::vector<Particle>& particles, 
                       const std::vector<Particle*>& highlighted = {});
    void drawQuadTree(const QuadTree* tree);
    void drawQueryCircle(double cx, double cy, double radius);
    void drawInfo(const std::string& info);
    void display();

    bool isOpen() const;
    bool pollEvent(sf::Event& event);
    void close();
    sf::Vector2f getMousePosition() const;
    void setInfo(const std::string& info);
};