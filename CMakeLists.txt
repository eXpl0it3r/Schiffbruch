cmake_minimum_required(VERSION 3.21)
project(Schiffbruch LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(Schiffbruch WIN32
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

    src/Action.hpp
    src/Application.hpp
    src/constants.hpp
    src/ddutil.h
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
)

target_link_libraries(Schiffbruch PRIVATE sfml-graphics sfml-audio sfml-main dxguid ddraw)
target_compile_features(Schiffbruch PRIVATE cxx_std_17)

# Copy DLLs next to the executable
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Schiffbruch POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Schiffbruch> $<TARGET_FILE_DIR:Schiffbruch> COMMAND_EXPAND_LISTS)
endif()

install(DIRECTORY bin/ DESTINATION ${CMAKE_INSTALL_BINDIR})

install(TARGETS Schiffbruch)