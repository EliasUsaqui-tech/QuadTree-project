#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Particle.h"
#include "QuadTree.h"

class Renderer {
private:
    // Ventana principal de SFML
    sf::RenderWindow window;

    // Fuente para el texto de informacion
    sf::Font font;

    // Texto de informacion que se muestra en pantalla
    sf::Text infoText;

    // Circulo de consulta (para la busqueda circular)
    sf::CircleShape queryCircle;

    // Controla si se dibuja el QuadTree o no
    bool showQuadTree = true;

    // Vista de la camara (para zoom y desplazamiento)
    sf::View view;

    // Nivel de zoom actual (1.0 = normal)
    float zoomLevel;

    // Centro de la vista (no se usa directamente, pero se mantiene por si acaso)
    sf::Vector2f center;

public:
    // Constructor: inicializa la ventana y la vista
    Renderer(int width, int height, const std::string& title);

    // Limpia la pantalla con un color de fondo
    void clear();

    // Dibuja una sola particula, con opcion de resaltado (amarillo)
    void drawParticle(const Particle& p, bool highlighted = false);

    // Dibuja todas las particulas, opcionalmente resaltando algunas
    void drawParticles(const std::vector<Particle>& particles,
                       const std::vector<Particle*>& highlighted = {});

    // Dibuja el QuadTree (subdivisiones)
    void drawQuadTree(const QuadTree* tree);

    // Dibuja un circulo de consulta (para busqueda por radio)
    void drawQueryCircle(double cx, double cy, double radius);

    // Muestra el texto de informacion en la esquina superior izquierda
    void drawInfo(const std::string& info);

    // Dibuja una linea entre dos vertices (para colisiones)
    void drawLine(const sf::Vertex& start, const sf::Vertex& end);

    // Dibuja un rectangulo (para consulta rectangular)
    void drawRect(const sf::RectangleShape& rect);

    // Muestra todo lo dibujado en la ventana
    void display();

    // Verifica si la ventana esta abierta
    bool isOpen() const;

    // Procesa eventos (teclado, mouse, etc.)
    bool pollEvent(sf::Event& event);

    // Cierra la ventana
    void close();

    // Obtiene la posicion del mouse en coordenadas del mundo (con zoom)
    sf::Vector2f getMousePosition() const;

    // Actualiza el texto de informacion
    void setInfo(const std::string& info);

    // Metodos de control de la camara (zoom y desplazamiento)
    void zoomIn();      // Acerca la vista
    void zoomOut();     // Aleja la vista
    void resetView();   // Restaura la vista normal
    void moveView(float dx, float dy);  // Desplaza la vista
};