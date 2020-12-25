project(schiffbruch)
find_package(SFML COMPONENTS system window graphics REQUIRED)
cmake_minimum_required(VERSION 3.9)

add_executable(schiffbruch
    src/Action.cpp
    src/Application.cpp
    src/ddutil.cpp
    src/Direct.cpp
    src/Game.cpp
    src/globals.cpp
    src/main.cpp
    src/Math.cpp
    src/Renderer.cpp
    src/Routing.cpp
    src/Sound.cpp
    src/World.cpp
)

target_link_libraries(schiffbruch sfml-system sfml-window sfml-graphics)
