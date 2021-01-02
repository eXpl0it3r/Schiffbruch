#pragma once

#include "headers.hpp"

#include <SFML/Graphics.hpp>

#include <string>

class Application
{
public:
    Application(const std::string& name, HINSTANCE instanceHandle);

    void Run();
    static void Update();

private:
    void ProcessEvents();

    sf::Window m_window;

    std::string m_name;
    std::time_t m_time; // Beginning of the second
};
