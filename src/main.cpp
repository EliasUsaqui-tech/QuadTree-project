#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../include/Simulator.h"
#include "../include/Renderer.h"

int main() {
    const int WIDTH = 1200;
    const int HEIGHT = 800;
    const int NUM_PARTICLES = 5000;
    const int QUERY_RADIUS = 80;

    sf::Clock clock;
    Simulator sim(WIDTH, HEIGHT, NUM_PARTICLES, 4);
    Renderer renderer(WIDTH, HEIGHT, "QuadTree Particle Simulator");

    sim.generateUniform();
    sim.buildQuadTree();

    double queryX = WIDTH / 2;
    double queryY = HEIGHT / 2;
    bool queryMode = false;
    bool paused = false;  // ← NUEVA VARIABLE

    std::cout << "=== QUADTREE SIMULATOR ===\n";
    std::cout << "Teclas:\n";
    std::cout << "  [1] Uniforme\n";
    std::cout << "  [2] Clusters\n";
    std::cout << "  [3] Alta densidad\n";
    std::cout << "  [Q] Activar/Desactivar modo consulta\n";
    std::cout << "  [P] Pausar/Reanudar simulación\n";  // ← NUEVA INSTRUCCIÓN
    std::cout << "  [R] Reconstruir QuadTree\n";
    std::cout << "  [Clic izquierdo] Posicionar consulta\n";
    std::cout << "  [ESC] Salir\n\n";

    while (renderer.isOpen()) {
        sf::Event event;
        while (renderer.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderer.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Num1:
                        sim.generateUniform();
                        sim.buildQuadTree();
                        std::cout << "Distribución uniforme\n";
                        break;
                    case sf::Keyboard::Num2:
                        sim.generateClusters(4);
                        sim.buildQuadTree();
                        std::cout << "Distribución con clusters\n";
                        break;
                    case sf::Keyboard::Num3:
                        sim.generateHighDensity();
                        sim.buildQuadTree();
                        std::cout << "Distribución de alta densidad\n";
                        break;
                    case sf::Keyboard::Q:
                        queryMode = !queryMode;
                        std::cout << "Modo consulta: " << (queryMode ? "ON" : "OFF") << "\n";
                        break;
                    case sf::Keyboard::P:  // ← NUEVA TECLA
                        paused = !paused;
                        std::cout << "Pausa: " << (paused ? "ON" : "OFF") << "\n";
                        break;
                    case sf::Keyboard::R:
                        sim.buildQuadTree();
                        std::cout << "QuadTree reconstruido\n";
                        break;
                    case sf::Keyboard::Escape:
                        renderer.close();
                        break;
                    default:
                        break;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto pos = renderer.getMousePosition();
                    queryX = pos.x;
                    queryY = pos.y;
                    std::cout << "Consulta en (" << queryX << ", " << queryY << ")\n";
                }
            }
        }

        // Actualizar simulación
        float dt = clock.restart().asSeconds();
        if (!paused) {  // ← SOLO ACTUALIZAR SI NO ESTÁ PAUSADO
            sim.update(dt);
            if (!queryMode) {
                sim.buildQuadTree();
            }
        }

        // Renderizar
        renderer.clear();
        renderer.drawParticles(sim.getParticles());
        renderer.drawQuadTree(sim.getQuadTree());

        // Consulta
        auto qtResult = sim.queryCircle(queryX, queryY, QUERY_RADIUS);
        auto bfResult = sim.bruteForceCircle(queryX, queryY, QUERY_RADIUS);

        renderer.drawQueryCircle(queryX, queryY, QUERY_RADIUS);

        // Resaltar resultados
        for (Particle* p : qtResult) {
            renderer.drawParticle(sim.getParticles()[p->id], true);
        }

        // Métricas
        std::stringstream info;
        info << "Partículas: " << NUM_PARTICLES << "\n";
        info << "Nodos QuadTree: " << sim.getQuadTree()->getTotalNodes() << "\n";
        info << "Resultados: " << qtResult.size() << "\n";
        info << "Comparaciones QT: " << sim.getQuadTree()->getComparisons() << "\n";
        info << "Comparaciones BF: " << NUM_PARTICLES << "\n";
        double speedup = (double)NUM_PARTICLES / std::max(1, sim.getQuadTree()->getComparisons());
        info << "Speedup: " << std::fixed << std::setprecision(2) << speedup << "x\n";
        info << "Pausa: " << (paused ? "ON" : "OFF") << "  ";
        info << "[1]Uniform [2]Clusters [3]Densa ";
        info << "[Q]Consulta [P]Pausa [R]Reconstruir";

        renderer.drawInfo(info.str());
        renderer.display();
    }

    return 0;
}