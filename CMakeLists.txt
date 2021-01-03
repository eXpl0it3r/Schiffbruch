project(schiffbruch)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)
find_package(SDL2)
cmake_minimum_required(VERSION 3.9)

set (CMAKE_CXX_STANDARD 20)

add_subdirectory(tools/resgen)
add_resource(MISC_BMP "bin/images/Misc.BMP")
add_resource(PANEL_BMP "bin/images/Panel.bmp")
add_resource(GUYANI_BMP "bin/images/GuyAni.bmp")
add_resource(ANIMATION_BMP "bin/images/Animation.BMP")
add_resource(BAUM_BMP "bin/images/Baum.bmp")
add_resource(CURSOR_BMP "bin/images/Cursor.BMP")
add_resource(BUTTONS_BMP "bin/images/Buttons.bmp")
add_resource(SCHRIFT2_BMP "bin/images/Schrift2.bmp")
add_resource(SCHRIFT1_BMP "bin/images/Schrift1.BMP")
add_resource(TEXTFELD_BMP "bin/images/Textfeld.bmp")
add_resource(PAPIER_BMP "bin/images/Papier.bmp")
add_resource(INVENTAR_BMP "bin/images/Inventar.bmp")
add_resource(BAU_BMP "bin/images/Bau.bmp")
add_resource(CREDITS_BMP "bin/images/Credits.bmp")
add_resource(LOGO_BMP "bin/images/Logo.bmp")

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
    src/strings.cpp

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

    ${MISC_BMP}
    ${PANEL_BMP}
    ${GUYANI_BMP}
    ${ANIMATION_BMP}
    ${BAUM_BMP}
    ${CURSOR_BMP}
    ${BUTTONS_BMP}
    ${SCHRIFT2_BMP}
    ${SCHRIFT1_BMP}
    ${TEXTFELD_BMP}
    ${PAPIER_BMP}
    ${INVENTAR_BMP}
    ${BAU_BMP}
    ${CREDITS_BMP}
    ${LOGO_BMP}
)

add_definitions(-Wno-writable-strings)

target_link_libraries(schiffbruch sfml-system sfml-window sfml-graphics sfml-audio SDL2::SDL2)

target_include_directories(schiffbruch PRIVATE miniwin/ ${PROJECT_SOURCE_DIR})

