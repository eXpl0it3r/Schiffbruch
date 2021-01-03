#pragma once

#include "headers.hpp"
#include "extern.hpp"

#include <SFML/Graphics.hpp>

#include <string>

class Application
{
public:
    Application(const std::string &name);

    void run();
    void update();

    static void drawToScreen(const sf::Drawable &sprite);
    static void clearScreenContent();
    static void drawSprite(const sf::Sprite &sprite);


    static void clearText(const int x, const int y, const int width, const int height);
    static void drawToText(const sf::Sprite &sprite);
    static void setTextPos(const int x, const int y);

    static void clearLandscape();
    static void drawToLandscape(const sf::Sprite &sprite);
    static void setLandscapeOffset(const int x, const int y);

    static sf::Image landscapeImage();

private:
    void process_events();

private:
    sf::RenderWindow m_window;
    sf::RenderTexture m_screenContent;
    sf::RenderTexture m_textOverlay;
    sf::RenderTexture m_landscape;

    std::string m_name;
    std::time_t m_time; // Beginning of the second
    sf::Vector2f m_landscapeOffset;
    sf::Vector2f m_textOffset;

    // TODO: get rid of
    static Application *s_instance;
};
