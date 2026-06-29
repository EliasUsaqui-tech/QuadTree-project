#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include "../include/Simulator.h"
#include "../include/Renderer.h"

void configurarParametros(int& numParticles, int& width, int& height,
                          int& capacity, double& minRadius, double& maxRadius,
                          double& minSpeed, double& maxSpeed) {
    std::cout << "\n========================================\n";
    std::cout << "   CONFIGURACION DE SIMULACION\n";
    std::cout << "========================================\n\n";

    std::cout << "1. NUMERO DE PARTICULAS:\n";
    std::cout << "  [1] 1,000   (pequeno)\n";
    std::cout << "  [2] 5,000   (mediano)\n";
    std::cout << "  [3] 10,000  (grande)\n";
    std::cout << "  [4] 15,000  (extra grande)\n";
    std::cout << "  [5] Personalizado\n";
    std::cout << "Opcion: ";

    int option;
    std::cin >> option;

    switch (option) {
        case 1: numParticles = 1000; break;
        case 2: numParticles = 5000; break;
        case 3: numParticles = 10000; break;
        case 4: numParticles = 15000; break;
        case 5:
            std::cout << "Ingrese numero de particulas: ";
            std::cin >> numParticles;
            break;
        default:
            std::cout << "Opcion invalida. Usando 5000.\n";
            numParticles = 5000;
            break;
    }

    std::cout << "\n2. TAMANO DEL ESPACIO 2D:\n";
    std::cout << "  [1] Pequeno (800x600)\n";
    std::cout << "  [2] Mediano (1200x800) - RECOMENDADO\n";
    std::cout << "  [3] Grande (1600x1200)\n";
    std::cout << "  [4] Personalizado\n";
    std::cout << "Opcion: ";
    std::cin >> option;

    switch (option) {
        case 1: width = 800; height = 600; break;
        case 2: width = 1200; height = 800; break;
        case 3: width = 1600; height = 1200; break;
        case 4:
            std::cout << "Ingrese ancho: ";
            std::cin >> width;
            std::cout << "Ingrese alto: ";
            std::cin >> height;
            break;
        default:
            std::cout << "Opcion invalida. Usando 1200x800.\n";
            width = 1200; height = 800;
            break;
    }

    std::cout << "\n3. CAPACIDAD MAXIMA POR NODO:\n";
    std::cout << "  [1] 2  (mas nodos, consultas mas rapidas)\n";
    std::cout << "  [2] 4  (balanceado) - RECOMENDADO\n";
    std::cout << "  [3] 8  (menos nodos, menos memoria)\n";
    std::cout << "  [4] 16 (pocos nodos, consultas mas lentas)\n";
    std::cout << "  [5] Personalizado\n";
    std::cout << "Opcion: ";
    std::cin >> option;

    switch (option) {
        case 1: capacity = 2; break;
        case 2: capacity = 4; break;
        case 3: capacity = 8; break;
        case 4: capacity = 16; break;
        case 5:
            std::cout << "Ingrese capacidad: ";
            std::cin >> capacity;
            break;
        default:
            std::cout << "Opcion invalida. Usando 4.\n";
            capacity = 4;
            break;
    }

    std::cout << "\n4. RADIO DE PARTICULAS:\n";
    std::cout << "  [1] Pequeno (1-3)\n";
    std::cout << "  [2] Mediano (2-6) - RECOMENDADO\n";
    std::cout << "  [3] Grande (5-10)\n";
    std::cout << "  [4] Personalizado\n";
    std::cout << "Opcion: ";
    std::cin >> option;

    switch (option) {
        case 1: minRadius = 1; maxRadius = 3; break;
        case 2: minRadius = 2; maxRadius = 6; break;
        case 3: minRadius = 5; maxRadius = 10; break;
        case 4:
            std::cout << "Ingrese radio minimo: ";
            std::cin >> minRadius;
            std::cout << "Ingrese radio maximo: ";
            std::cin >> maxRadius;
            break;
        default:
            std::cout << "Opcion invalida. Usando 2-6.\n";
            minRadius = 2; maxRadius = 6;
            break;
    }

    std::cout << "\n5. VELOCIDAD DE PARTICULAS:\n";
    std::cout << "  [1] Muy Lenta (2-10)\n";
    std::cout << "  [2] Lenta (10-30)\n";
    std::cout << "  [3] Media (50-150) - RECOMENDADO\n";
    std::cout << "  [4] Rapida (100-300)\n";
    std::cout << "  [5] Muy Rapida (200-500)\n";
    std::cout << "  [6] Personalizado\n";
    std::cout << "Opcion: ";
    std::cin >> option;

    switch (option) {
        case 1: minSpeed = 2; maxSpeed = 10; break;
        case 2: minSpeed = 10; maxSpeed = 30; break;
        case 3: minSpeed = 50; maxSpeed = 150; break;
        case 4: minSpeed = 100; maxSpeed = 300; break;
        case 5: minSpeed = 200; maxSpeed = 500; break;
        case 6:
            std::cout << "Ingrese velocidad minima: ";
            std::cin >> minSpeed;
            std::cout << "Ingrese velocidad maxima: ";
            std::cin >> maxSpeed;
            break;
        default:
            std::cout << "Opcion invalida. Usando 50-150.\n";
            minSpeed = 50; maxSpeed = 150;
            break;
    }

    std::cout << "\n========================================\n";
    std::cout << "   RESUMEN DE CONFIGURACION\n";
    std::cout << "========================================\n";
    std::cout << "Particulas:    " << numParticles << "\n";
    std::cout << "Espacio:       " << width << "x" << height << "\n";
    std::cout << "Capacidad:     " << capacity << "\n";
    std::cout << "Radio:         " << minRadius << "-" << maxRadius << "\n";
    std::cout << "Velocidad:     " << minSpeed << "-" << maxSpeed << "\n";
    std::cout << "========================================\n\n";
}

int main() {
    int NUM_PARTICLES;
    int WIDTH, HEIGHT;
    int QUADTREE_CAPACITY;
    double MIN_RADIUS, MAX_RADIUS;
    double MIN_SPEED, MAX_SPEED;
    const int QUERY_RADIUS = 40;
    const double COLLISION_RADIUS = 20.0;

    configurarParametros(NUM_PARTICLES, WIDTH, HEIGHT, QUADTREE_CAPACITY,
                         MIN_RADIUS, MAX_RADIUS, MIN_SPEED, MAX_SPEED);

    sf::Clock clock;
    Simulator sim(WIDTH, HEIGHT, NUM_PARTICLES, QUADTREE_CAPACITY);
    sim.setParticleParams(MIN_RADIUS, MAX_RADIUS, MIN_SPEED, MAX_SPEED);

    Renderer renderer(WIDTH, HEIGHT, "QuadTree Particle Simulator");

    sim.generateUniform();
    sim.buildQuadTree();

    double queryX = WIDTH / 2;
    double queryY = HEIGHT / 2;
    bool queryMode = false;
    bool paused = false;
    bool showCollisions = true;
    bool benchmarkDone = false;

    bool rectMode = false;
    double rectX1 = 0, rectY1 = 0, rectX2 = 0, rectY2 = 0;
    bool drawingRect = false;

    std::string distName = "Uniforme";
    int distType = 0;

    std::cout << "=== CONTROLES ===\n";
    std::cout << "  [1] Uniforme    [2] Clusters    [3] Alta densidad\n";
    std::cout << "  [Q] Consulta (circulo)    [R] Consulta rectangular\n";
    std::cout << "  [P] Pausa       [Shift+R] Reconstruir\n";
    std::cout << "  [I] Insertar 1 particula aleatoria\n";
    std::cout << "  [Shift+I] Insertar 10 particulas aleatorias\n";
    std::cout << "  [C] Mostrar/Ocultar colisiones\n";
    std::cout << "  [B] Benchmark   [ESC] Salir\n\n";

    bool isDragging = false;
    sf::Vector2f dragStart;

    // BUCLE PRINCIPAL
    while (renderer.isOpen()) {
        sf::Event event;
        // PROCESAR EVENTOS
        while (renderer.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderer.close();
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    renderer.zoomIn();
                } else {
                    renderer.zoomOut();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    isDragging = true;
                    dragStart = renderer.getMousePosition();
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    isDragging = false;
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                if (isDragging) {
                    sf::Vector2f currentPos = renderer.getMousePosition();
                    sf::Vector2f delta = dragStart - currentPos;
                    renderer.moveView(delta.x, delta.y);
                    dragStart = currentPos;
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Add:
                    case sf::Keyboard::Equal:
                        renderer.zoomIn();
                        break;
                    case sf::Keyboard::Subtract:
                        renderer.zoomOut();
                        break;
                    case sf::Keyboard::Home:
                        renderer.resetView();
                        break;
                    case sf::Keyboard::Num1:
                        sim.generateUniform();
                        sim.buildQuadTree();
                        distName = "Uniforme";
                        distType = 0;
                        benchmarkDone = false;
                        std::cout << "Distribucion uniforme\n";
                        break;
                    case sf::Keyboard::Num2:
                        sim.generateClusters(4);
                        sim.buildQuadTree();
                        distName = "Clusters";
                        distType = 1;
                        benchmarkDone = false;
                        std::cout << "Distribucion con clusters\n";
                        break;
                    case sf::Keyboard::Num3:
                        sim.generateHighDensity();
                        sim.buildQuadTree();
                        distName = "AltaDensidad";
                        distType = 2;
                        benchmarkDone = false;
                        std::cout << "Distribucion de alta densidad\n";
                        break;
                    case sf::Keyboard::Q:
                        queryMode = !queryMode;
                        rectMode = false;
                        drawingRect = false;
                        std::cout << "Modo consulta (circulo): " << (queryMode ? "ON" : "OFF") << "\n";
                        break;
                    case sf::Keyboard::R:
                        if (event.key.shift) {
                            sim.buildQuadTree();
                            benchmarkDone = false;
                            std::cout << "QuadTree reconstruido\n";
                        } else {
                            rectMode = !rectMode;
                            queryMode = false;
                            drawingRect = false;
                            if (rectMode) {
                                std::cout << "Modo rectangulo activado. Haz clic para definir la esquina.\n";
                            } else {
                                std::cout << "Modo rectangulo desactivado\n";
                            }
                        }
                        break;
                    case sf::Keyboard::P:
                        paused = !paused;
                        std::cout << "Pausa: " << (paused ? "ON" : "OFF") << "\n";
                        break;
                    case sf::Keyboard::C:
                        showCollisions = !showCollisions;
                        std::cout << "Colisiones: " << (showCollisions ? "VISIBLES" : "OCULTAS") << "\n";
                        break;
                    case sf::Keyboard::I:
                        if (event.key.shift) {
                            for (int i = 0; i < 10; i++) {
                                sim.addRandomParticle();
                            }
                            std::cout << "10 particulas insertadas. Total: " << sim.getNumParticles() << "\n";
                        } else {
                            sim.addRandomParticle();
                            std::cout << "Particula insertada. Total: " << sim.getNumParticles() << "\n";
                        }
                        benchmarkDone = false;
                        break;
                    case sf::Keyboard::B:
                        if (!benchmarkDone) {
                            std::cout << "\n=== EJECUTANDO BENCHMARK ===\n";

                            auto benchmark = sim.benchmarkQuery(queryX, queryY, QUERY_RADIUS);

                            auto startBF = std::chrono::high_resolution_clock::now();
                            auto bfResult = sim.bruteForceCircle(queryX, queryY, QUERY_RADIUS);
                            auto endBF = std::chrono::high_resolution_clock::now();
                            double bfTime = std::chrono::duration<double>(endBF - startBF).count();

                            int qtComparisons = 0;
                            auto qtResult = sim.queryCircle(queryX, queryY, QUERY_RADIUS, qtComparisons);

                            auto qtCollisions = sim.detectCollisions(COLLISION_RADIUS);
                            auto bfCollisions = sim.bruteForceCollisions(COLLISION_RADIUS);

                            std::cout << "\nRESULTADOS DEL BENCHMARK:\n";
                            std::cout << "----------------------------------------\n";
                            std::cout << "1. Tiempo de construccion:     " << std::fixed << std::setprecision(3) << benchmark.buildTime * 1000 << " ms\n";
                            std::cout << "2. Tiempo consulta QT:         " << std::fixed << std::setprecision(3) << benchmark.queryTime * 1000 << " ms\n";
                            std::cout << "3. Tiempo busqueda ingenua:    " << std::fixed << std::setprecision(3) << bfTime * 1000 << " ms\n";
                            std::cout << "4. Comparaciones QT:           " << qtComparisons << "\n";
                            std::cout << "5. Comparaciones BF:           " << sim.getNumParticles() << "\n";
                            std::cout << "6. Nodos visitados:            " << benchmark.nodesVisited << "\n";
                            std::cout << "7. Resultados encontrados:     " << benchmark.results << "\n";
                            std::cout << "8. Speedup:                    " << std::fixed << std::setprecision(2) << (double)sim.getNumParticles() / std::max(1, qtComparisons) << "x\n";
                            std::cout << "9. Colisiones QT:              " << qtCollisions.size() << "\n";
                            std::cout << "10. Colisiones BF:             " << bfCollisions.size() << "\n";
                            std::cout << "----------------------------------------\n\n";

                            std::ofstream file("resultados_benchmark.csv", std::ios::app);
                            if (file.is_open()) {
                                file.seekp(0, std::ios::end);
                                if (file.tellp() == 0) {
                                    file << "Distribucion,Particulas,Espacio,Capacidad,RadioMin,RadioMax,VelMin,VelMax,TiempoConstruccion_ms,TiempoConsultaQT_ms,TiempoBF_ms,ComparacionesQT,ComparacionesBF,NodosVisitados,Resultados,Speedup,ColisionesQT,ColisionesBF\n";
                                }

                                file << distName << ","
                                     << sim.getNumParticles() << ","
                                     << WIDTH << "x" << HEIGHT << ","
                                     << QUADTREE_CAPACITY << ","
                                     << MIN_RADIUS << "," << MAX_RADIUS << ","
                                     << MIN_SPEED << "," << MAX_SPEED << ","
                                     << std::fixed << std::setprecision(3) << benchmark.buildTime * 1000 << ","
                                     << std::fixed << std::setprecision(3) << benchmark.queryTime * 1000 << ","
                                     << std::fixed << std::setprecision(3) << bfTime * 1000 << ","
                                     << qtComparisons << ","
                                     << sim.getNumParticles() << ","
                                     << benchmark.nodesVisited << ","
                                     << benchmark.results << ","
                                     << std::fixed << std::setprecision(2) << (double)sim.getNumParticles() / std::max(1, qtComparisons) << ","
                                     << qtCollisions.size() << ","
                                     << bfCollisions.size() << "\n";

                                file.close();
                                std::cout << "Datos guardados en 'resultados_benchmark.csv'\n\n";
                            }

                            benchmarkDone = true;
                        } else {
                            std::cout << "Ya se ejecuto el benchmark. Cambia de distribucion.\n\n";
                        }
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

                    if (rectMode) {
                        if (!drawingRect) {
                            rectX1 = pos.x;
                            rectY1 = pos.y;
                            drawingRect = true;
                            renderer.addMessage("Esquina 1: (" + std::to_string((int)rectX1) + ", " + std::to_string((int)rectY1) + "). Haz clic para la esquina 2.");
                        } else {
                            rectX2 = pos.x;
                            rectY2 = pos.y;
                            drawingRect = false;
                            rectMode = false;

                            double minX = std::min(rectX1, rectX2);
                            double minY = std::min(rectY1, rectY2);
                            double maxX = std::max(rectX1, rectX2);
                            double maxY = std::max(rectY1, rectY2);

                            sim.buildQuadTree();

                            QuadTree::Rect rect(minX, minY, maxX - minX, maxY - minY);
                            int comparisons = 0;
                            auto results = sim.getQuadTree()->queryRange(rect, comparisons);

                            double speedup = (double)sim.getNumParticles() / std::max(1, comparisons);

                            renderer.addMessage("CONSULTA RECTANGULAR");
                            renderer.addMessage("Region: (" + std::to_string((int)minX) + ", " + std::to_string((int)minY) + ") a (" + std::to_string((int)maxX) + ", " + std::to_string((int)maxY) + ")");
                            renderer.addMessage("Resultados: " + std::to_string(results.size()) + " particulas");
                            renderer.addMessage("Comparaciones QT: " + std::to_string(comparisons));
                            renderer.addMessage("Speedup: " + std::to_string(speedup) + "x");

                            rectX1 = minX;
                            rectY1 = minY;
                            rectX2 = maxX;
                            rectY2 = maxY;
                        }
                    } else if (queryMode) {
                        queryX = pos.x;
                        queryY = pos.y;
                        sim.buildQuadTree();
                        int comparisons = 0;
                        auto results = sim.queryCircle(queryX, queryY, QUERY_RADIUS, comparisons);

                        double speedup = (double)sim.getNumParticles() / std::max(1, comparisons);

                        renderer.addMessage("CONSULTA CIRCULAR");
                        renderer.addMessage("Centro: (" + std::to_string((int)queryX) + ", " + std::to_string((int)queryY) + ")");
                        renderer.addMessage("Radio: " + std::to_string(QUERY_RADIUS));
                        renderer.addMessage("Resultados: " + std::to_string(results.size()) + " particulas");
                        renderer.addMessage("Comparaciones QT: " + std::to_string(comparisons));
                        renderer.addMessage("Speedup: " + std::to_string(speedup) + "x");

                        benchmarkDone = false;
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2f pos = renderer.getMousePosition();
                    sim.addParticlesAtMouse(pos.x, pos.y, 1);
                    sim.buildQuadTree();
                    benchmarkDone = false;
                    renderer.addMessage("Insertadas particula en (" + std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y) + ")");
                }
            }
        } // FIN DE PROCESAMIENTO DE EVENTOS

        // ============================================
        // ACTUALIZAR SIMULACION
        // ============================================
        float dt = clock.restart().asSeconds();
        if (!paused) {
            sim.update(dt);
            if (!queryMode && !rectMode) {
                sim.buildQuadTree();
            }
        }

        sim.updateCollisionStatus(COLLISION_RADIUS);

        // ============================================
        // RENDERIZAR
        // ============================================
        renderer.clear();
        renderer.drawParticles(sim.getParticles());
        renderer.drawQuadTree(sim.getQuadTree());

        int qtComparisons = 0;
        auto qtResult = sim.queryCircle(queryX, queryY, QUERY_RADIUS, qtComparisons);

        if (queryMode) {
            renderer.drawQueryCircle(queryX, queryY, QUERY_RADIUS);
            for (Particle* p : qtResult) {
                renderer.drawParticle(sim.getParticles()[p->id], true);
            }
        }

        if (rectMode && drawingRect) {
            auto pos = renderer.getMousePosition();
            sf::RectangleShape rect;
            float minX = std::min((float)rectX1, pos.x);
            float minY = std::min((float)rectY1, pos.y);
            float maxX = std::max((float)rectX1, pos.x);
            float maxY = std::max((float)rectY1, pos.y);
            rect.setPosition(minX, minY);
            rect.setSize(sf::Vector2f(maxX - minX, maxY - minY));
            rect.setFillColor(sf::Color(255, 255, 0, 50));
            rect.setOutlineColor(sf::Color::Yellow);
            rect.setOutlineThickness(2);
            renderer.drawRect(rect);
        }

        if (!rectMode && !drawingRect && rectX1 != rectX2 && rectY1 != rectY2) {
            sf::RectangleShape rect;
            float minX = std::min((float)rectX1, (float)rectX2);
            float minY = std::min((float)rectY1, (float)rectY2);
            float maxX = std::max((float)rectX1, (float)rectX2);
            float maxY = std::max((float)rectY1, (float)rectY2);
            rect.setPosition(minX, minY);
            rect.setSize(sf::Vector2f(maxX - minX, maxY - minY));
            rect.setFillColor(sf::Color(255, 255, 0, 50));
            rect.setOutlineColor(sf::Color::Yellow);
            rect.setOutlineThickness(2);
            renderer.drawRect(rect);

            QuadTree::Rect range(minX, minY, maxX - minX, maxY - minY);
            int comparisons = 0;
            auto results = sim.getQuadTree()->queryRange(range, comparisons);
            for (Particle* p : results) {
                renderer.drawParticle(sim.getParticles()[p->id], true);
            }
        }

        // Obtener colisiones reales y todas las posibles
        auto collisions = sim.detectCollisions(COLLISION_RADIUS);
        auto allPossible = sim.detectPossibleCollisions(COLLISION_RADIUS);

        // Filtrar posibles: eliminar los pares que ya están en collisions
        std::vector<std::pair<Particle*, Particle*>> possibleCollisions;
        for (auto& p : allPossible) {
            bool isReal = false;
            for (auto& c : collisions) {
                if ((c.first == p.first && c.second == p.second) ||
                    (c.first == p.second && c.second == p.first)) {
                    isReal = true;
                    break;
                }
            }
            if (!isReal) {
                possibleCollisions.push_back(p);
            }
        }

        // Dibujar líneas rojas solo para posibles (no reales)
        if (showCollisions) {
            for (auto& col : possibleCollisions) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(col.first->x, col.first->y), sf::Color::Red),
                    sf::Vertex(sf::Vector2f(col.second->x, col.second->y), sf::Color::Red)
                };
                renderer.drawLine(line[0], line[1]);
            }
        }

        std::stringstream info;
        info << "Particulas: " << sim.getNumParticles() << "\n";
        info << "Nodos QT: " << sim.getQuadTree()->getTotalNodes() << "\n";
        info << "Resultados: " << qtResult.size() << "\n";
        info << "Comp QT: " << qtComparisons << "\n";
        info << "Comp BF: " << sim.getNumParticles() << "\n";
        double speedup = (double)sim.getNumParticles() / std::max(1, qtComparisons);
        info << "Speedup: " << std::fixed << std::setprecision(2) << speedup << "x\n";
        info << "Colisiones reales: " << collisions.size() << "\n";
        info << "Posibles (lineas rojas): " << possibleCollisions.size() << "\n";
        info << "Pausa: " << (paused ? "ON" : "OFF") << "  ";
        info << "[1]Unif [2]Clust [3]Dens ";
        info << "[Q]Circle [R]Rect ";
        info << "[I]Ins [Shift+I]x10 ";
        info << "[C]Col [B]Bench [P]Pausa [Shift+R]Rebuild";

        renderer.drawInfo(info.str());
        renderer.drawMessages();
        renderer.display();
    }

    return 0;
}