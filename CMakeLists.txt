cmake_minimum_required(VERSION 3.20)
project(quadTree_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/SFML/SFML-2.6.1/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# ============================================
# ARCHIVOS FUENTE
# ============================================
set(SOURCES
        src/main.cpp
        src/Particle.cpp
        src/QuadTree.cpp
        src/Simulator.cpp
        src/Renderer.cpp
)

# ============================================
# CREAR EJECUTABLE
# ============================================
add_executable(${PROJECT_NAME} ${SOURCES})

# ============================================
# INCLUIR DIRECTORIOS
# ============================================
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ============================================
# ENLAZAR SFML
# ============================================
target_link_libraries(${PROJECT_NAME}
        sfml-graphics
        sfml-window
        sfml-system
)


if (WIN32)
    file(GLOB SFML_DLLS "C:/SFML/SFML-2.6.1/bin/*.dll")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Copiando todas las DLLs de SFML..."
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_DLLS} $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copiando todas las DLLs de SFML a la carpeta del ejecutable..."
    )
endif()