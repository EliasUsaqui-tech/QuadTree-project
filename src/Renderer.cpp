#include "../include/Renderer.h"
#include <sstream>

Renderer::Renderer(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title) {
    
    // Cargar fuente (ajusta la ruta según tu sistema)
    #ifdef _WIN32
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            // Fallback
        }
    #elif __APPLE__
        if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttf")) {
            // Fallback
        }
    #else
        if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
            // Fallback
        }
    #endif
    
    infoText.setFont(font);
    infoText.setCharacterSize(14);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(10, 10);
    
    queryCircle.setFillColor(sf::Color(0, 255, 255, 40));
    queryCircle.setOutlineColor(sf::Color::Cyan);
    queryCircle.setOutlineThickness(1);
}

void Renderer::clear() {
    window.clear(sf::Color(30, 30, 40));
}

void Renderer::drawParticle(const Particle& p, bool highlighted) {
    sf::CircleShape shape(p.radius);
    shape.setPosition(p.x - p.radius, p.y - p.radius);
    shape.setFillColor(highlighted ? sf::Color::Yellow : p.color);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(highlighted ? 2 : 0);
    window.draw(shape);
}

void Renderer::drawParticles(const std::vector<Particle>& particles, 
                             const std::vector<Particle*>& highlighted) {
    for (const auto& p : particles) {
        bool isHighlighted = false;
        for (Particle* hp : highlighted) {
            if (hp->id == p.id) {
                isHighlighted = true;
                break;
            }
        }
        drawParticle(p, isHighlighted);
    }
}

void Renderer::drawQuadTree(const QuadTree* tree) {
    if (tree && showQuadTree) {
        tree->draw(window);
    }
}

void Renderer::drawQueryCircle(double cx, double cy, double radius) {
    queryCircle.setPosition(cx - radius, cy - radius);
    queryCircle.setRadius(radius);
    window.draw(queryCircle);
}

void Renderer::drawInfo(const std::string& info) {
    infoText.setString(info);
    window.draw(infoText);
}

void Renderer::display() {
    window.display();
}

bool Renderer::isOpen() const { return window.isOpen(); }
bool Renderer::pollEvent(sf::Event& event) { return window.pollEvent(event); }
void Renderer::close() { window.close(); }

sf::Vector2f Renderer::getMousePosition() const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

void Renderer::setInfo(const std::string& info) {
    infoText.setString(info);
}