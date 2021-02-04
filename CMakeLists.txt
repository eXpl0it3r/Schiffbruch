project(schiffbruch)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)
cmake_minimum_required(VERSION 3.9)

set (CMAKE_CXX_STANDARD 20)
include_directories(${PROJECT_BINARY_DIR})

#############################
## Compiler specific stuff ##
#############################
option(ENABLE_IPO "Enable link-time optimization, improves speed and memory usage (https://en.wikipedia.org/wiki/Interprocedural_optimization)")
if (ENABLE_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_AVAILABLE OUTPUT IPO_ERROR)
    if (IPO_AVAILABLE)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)

        if (CMAKE_COMPILER_IS_GNUCXX)
            message(STATUS "Enabling all link time optimizations")
            add_definitions(-fuse-linker-plugin)
        else()
            message(STATUS "Disabling linker plugin for link time optimization")
        endif()
    else()
        message(STATUS "IPO/LTO not supported: <${IPO_ERROR}>")
    endif()
else()
    message(STATUS "Not enabling IPO/LTO")
endif()

if (VC_STATIC)
    message(STATUS "Static build with MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")
else()
    add_definitions(-Wno-writable-strings)
endif()

if(MSVC)
    add_definitions(/D_USE_MATH_DEFINES)
    add_definitions(/wd4244 /wd4018 /wd4267 /wd4996 /wd4305 /wd4800 /wd4065)

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /fp:fast")
endif()

option(ENABLE_SANITIZERS "Enable runtime sanitizing (for development)")
if (ENABLE_SANITIZERS)
    message("Enabling asan and ubsan")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()

add_subdirectory(tools/resgen)
add_resource(MISC_BMP "bin/images/Misc.BMP")
add_resource(PANEL_PNG "bin/images/Panel.png")
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
add_resource(LOGO_PNG "bin/images/Logo.png")

add_resource(ABSPANN_WAV "bin/sounds/abspann.wav")
add_resource(ANGEL_WAV "bin/sounds/angel.wav")
add_resource(BAUMFAELLT_WAV "bin/sounds/baumfaellt.wav")
add_resource(BRANDUNG_WAV "bin/sounds/brandung.wav")
add_resource(CRASH_WAV "bin/sounds/crash.wav")
add_resource(ERFINDUNG_WAV "bin/sounds/erfindung.wav")
add_resource(FAELLEN_WAV "bin/sounds/faellen.wav")
add_resource(FEUER_WAV "bin/sounds/feuer.wav")
add_resource(FLUSS_WAV "bin/sounds/fluss.wav")
add_resource(HAMMER_WAV "bin/sounds/hammer.wav")
add_resource(KLICK2_WAV "bin/sounds/klick2.wav")
add_resource(KLICK_WAV "bin/sounds/klick.wav")
add_resource(KNISTERN_WAV "bin/sounds/knistern.wav")
add_resource(LOGO_WAV "bin/sounds/logo.wav")
add_resource(PLATSCH_WAV "bin/sounds/platsch.wav")
add_resource(SCHAUFELN_WAV "bin/sounds/schaufeln.wav")
add_resource(SCHLAGEN_WAV "bin/sounds/schlagen.wav")
add_resource(SCHLEUDER_WAV "bin/sounds/schleuder.wav")
add_resource(SCHNARCHEN_WAV "bin/sounds/schnarchen.wav")
add_resource(SCHWIMMEN_WAV "bin/sounds/schwimmen.wav")
add_resource(STURM_WAV "bin/sounds/sturm.wav")
add_resource(TRINKEN_WAV "bin/sounds/trinken.wav")
add_resource(WALD_WAV "bin/sounds/wald.wav")
add_resource(WOLF_WAV "bin/sounds/wolf.wav")

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
    ${PANEL_PNG}
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
    ${LOGO_PNG}

    ${ABSPANN_WAV}
    ${ANGEL_WAV}
    ${BAUMFAELLT_WAV}
    ${BRANDUNG_WAV}
    ${CRASH_WAV}
    ${ERFINDUNG_WAV}
    ${FAELLEN_WAV}
    ${FEUER_WAV}
    ${FLUSS_WAV}
    ${HAMMER_WAV}
    ${KLICK2_WAV}
    ${KLICK_WAV}
    ${KNISTERN_WAV}
    ${LOGO_WAV}
    ${PLATSCH_WAV}
    ${SCHAUFELN_WAV}
    ${SCHLAGEN_WAV}
    ${SCHLEUDER_WAV}
    ${SCHNARCHEN_WAV}
    ${SCHWIMMEN_WAV}
    ${STURM_WAV}
    ${TRINKEN_WAV}
    ${WALD_WAV}
    ${WOLF_WAV}
)

target_link_libraries(schiffbruch sfml-system sfml-window sfml-graphics sfml-audio)

target_include_directories(schiffbruch PRIVATE miniwin/ ${PROJECT_SOURCE_DIR})

install(TARGETS schiffbruch DESTINATION bin)
