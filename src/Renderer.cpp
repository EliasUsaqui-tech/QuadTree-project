#include "../include/Renderer.h"
#include <sstream>
#include <iostream>
#include <iomanip>

Renderer::Renderer(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title), zoomLevel(1.0f) {

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

    // Configurar vista inicial
    view.reset(sf::FloatRect(0, 0, width, height));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    window.setView(view);

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

void Renderer::drawLine(const sf::Vertex& start, const sf::Vertex& end) {
    sf::Vertex line[] = {start, end};
    window.draw(line, 2, sf::Lines);
}

void Renderer::drawRect(const sf::RectangleShape& rect) {
    window.draw(rect);
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

// ============================================
// Metodos de zoom
// ============================================
void Renderer::zoomIn() {
    zoomLevel *= 0.8f;
    view.zoom(0.8f);
    window.setView(view);
    std::cout << "Zoom: " << std::fixed << std::setprecision(2) << (1.0f / zoomLevel) << "x\n";
}

void Renderer::zoomOut() {
    zoomLevel *= 1.25f;
    view.zoom(1.25f);
    window.setView(view);
    std::cout << "Zoom: " << std::fixed << std::setprecision(2) << (1.0f / zoomLevel) << "x\n";
}

void Renderer::resetView() {
    zoomLevel = 1.0f;
    view.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(view);
    std::cout << "Vista restablecida\n";
}

void Renderer::moveView(float dx, float dy) {
    view.move(dx, dy);
    window.setView(view);
}