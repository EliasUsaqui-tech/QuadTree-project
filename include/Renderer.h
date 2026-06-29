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
    sf::View view;
    float zoomLevel;
    sf::Vector2f center;

    std::vector<std::string> consoleMessages;

public:
    Renderer(int width, int height, const std::string& title);

    void clear();
    void drawParticle(const Particle& p, bool highlighted = false);
    void drawParticles(const std::vector<Particle>& particles,
                       const std::vector<Particle*>& highlighted = {});
    void drawQuadTree(const QuadTree* tree);
    void drawQueryCircle(double cx, double cy, double radius);
    void drawInfo(const std::string& info);
    void drawLine(const sf::Vertex& start, const sf::Vertex& end);
    void drawRect(const sf::RectangleShape& rect);
    void drawMessages();
    void display();

    void addMessage(const std::string& msg);

    bool isOpen() const;
    bool pollEvent(sf::Event& event);
    void close();
    sf::Vector2f getMousePosition() const;
    void setInfo(const std::string& info);

    void zoomIn();
    void zoomOut();
    void resetView();
    void moveView(float dx, float dy);
};