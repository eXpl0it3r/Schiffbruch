project(schiffbruch)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)
find_package(SDL2)
cmake_minimum_required(VERSION 3.9)

add_executable(schiffbruch
    src/Action.cpp
    src/Application.cpp
    src/Direct.cpp
    src/Game.cpp
    src/globals.cpp
    src/main.cpp
    src/Math.cpp
    src/Renderer.cpp
    src/Routing.cpp
    src/Sound.cpp
    src/World.cpp

    src/Action.hpp
    src/Application.hpp
    src/constants.hpp
    src/Direct.hpp
    src/extern.hpp
    src/Game.hpp
    src/headers.hpp
    src/Math.hpp
    src/Menu.hpp
    src/Renderer.hpp
    src/Routing.hpp
    src/Sound.hpp
    src/State.hpp
    src/types.hpp
    src/World.hpp

#    miniwin/ddraw.cpp
##    miniwin/dsound.cpp
##    miniwin/misc.cpp
#    miniwin/misc_dx.cpp
#    miniwin/misc_io.cpp
##    miniwin/misc_msg.cpp
#    miniwin/rand.cpp
#    miniwin/thread.cpp
#    miniwin/dx.cpp
)

add_definitions(-Wno-writable-strings)

target_link_libraries(schiffbruch sfml-system sfml-window sfml-graphics sfml-audio SDL2::SDL2)

target_include_directories(schiffbruch PRIVATE miniwin/ ${PROJECT_SOURCE_DIR})

