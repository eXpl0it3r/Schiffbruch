#include "Application.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "State.hpp"
#include "World.hpp"

#include "types.hpp"

#include <ctime>
#include <cstdlib>
#include <exception>
#include <cassert>

Application *Application::s_instance;

Application::Application(const std::string &name)
    : m_window({MAX_SCREEN_X, MAX_SCREEN_Y}, name, sf::Style::None)
, m_name(name)
, m_time(std::time(nullptr))
{
    // holy shit sfml
    // have to do this entire dance because SFML is a piling steam of shit
    const sf::Vector2u size(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height) ;
    m_window.setSize(size);
    m_window.setPosition({0, 0});

    s_instance = this;

    m_screenContent.create(MAX_SCREEN_X, MAX_SCREEN_Y);
    m_screenContent.clear(sf::Color::Transparent);
    m_textOverlay.create(MAX_SCREEN_X, MAX_SCREEN_Y);
    m_textOverlay.clear(sf::Color::Transparent);
    m_landscape.create(2 * MAX_SURFACE_X, 2 * MAX_SURFACE_Y);
    m_landscape.clear(sf::Color::Transparent);

    m_window.setMouseCursorVisible(false);

    puts(name.data());

    Sound::Init();
    s_SoundState = 1; // Activate sound

    if (!Direct::InitDDraw()) {
        puts("Failed to init graphics");
        exit(1);
    }

    s_GameState = State::LOGO;
    Game::InitWaves(); // Nur zum Wavinitialisieren
    Game::InitStructs();

    srand(static_cast<unsigned>(std::time(nullptr))); // Random initialisieren
}

void Application::process_events()
{
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch(event.type){
        case sf::Event::Closed:
            Direct::finiObjects();
            m_window.close();
            break;
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
            if (s_GameState == State::GAME) {
                sf::Vector2f mappedPos = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                Direct::CheckMouse(Coordinate({short(mappedPos.x), short(mappedPos.y)}));
            }
            break;
        case sf::Event::MouseMoved:
            if (s_GameState == State::GAME) {
                sf::Vector2f mappedPos = m_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                Direct::CheckMouse(Coordinate({short(mappedPos.x), short(mappedPos.y)}));
            }
            break;
        case sf::Event::KeyReleased:
            if (event.key.code == sf::Keyboard::F4) {
                Direct::finiObjects();
                m_window.close();
            }
            break;
        default:
            break;
        }
    }
}

void Application::run()
{
    sf::Clock timer;

    sf::Texture texture;
    sf::Sprite sprite;
    sprite.setTexture(m_screenContent.getTexture());

    sf::Sprite landscape;
    landscape.setTexture(m_landscape.getTexture());
    sf::Sprite text;
    text.setTexture(m_textOverlay.getTexture());

    while (m_window.isOpen()) {
        m_window.clear(sf::Color::Black);
        process_events();
        CurrentFrame++;
        FPS = 60;

        // BilderproSec ausgeben
        //                    /*
        //                Renderer::HideText(TXTFPS);
        //                TextBereich[TXTFPS].HasText = true;
        //                std::sprintf(StdString, "%d", LastBild);
        //                Renderer::DrawString(StdString,(short)TextBereich[TXTFPS].textRect.left,(short)TextBereich[TXTFPS].textRect.top,1);
        //                    */

        if (s_GameState == State::LOGO) {
            if (Direct::CheckKey() == 2) { // Das Keyboard abfragen
                continue;
            }
            Renderer::ShowLogo(); // Bild auffrischen
        } else if ((s_GameState == State::INTRO) || (s_GameState == State::RESCUED)) {
            if (Direct::CheckKey() == 0) { // Das Keyboard abfragen
                m_window.close();
                break;
            }

            Math::Animationen(); // Animationen weiterschalten

            if (!Guy.IsActive) { // Aktionen starten
                Actions::handler(Guy.CurrentAction);
            }

            Renderer::ShowIntro(); // Bild auffrischen
        } else if (s_GameState == State::GAME) {
            if ((Hours >= 12) && (Minutes != 0) && (Guy.CurrentAction != Action::DAY_END)) { // Hier ist der Tag zuende
                if (Guy.CurrentAction == Action::LOOKOUT) {
                    Chance -= 1 + Landscape[Guy.Pos.x][Guy.Pos.y].Height;
                }

                Guy.IsActive = false;
                Guy.ActionStep = 0;
                Guy.CurrentAction = Action::DAY_END;
            }

            World::UpdateButtons(); // Die Spezialkn�pfe umschalten

            if (Direct::CheckKey() == 0) { // Das Keyboard abfragen
                m_window.close();
                break;
            }

            Renderer::LimitScroll(); // Das Scrollen an die Grenzen der Landschaft anpassen
            Math::Animationen(); // Die Animationsphasen weiterschalten

            if (!Guy.IsActive) { // Die Aktionen starten
                Actions::handler(Guy.CurrentAction);
            }

            Renderer::Show(); // Das Bild zeichnen
        } else if (s_GameState == State::OUTRO) {
            if (Direct::CheckKey() == 0) {
                m_window.close();
                break;
            }

            Math::AbspannCalc();
            Renderer::ShowCredits();
        }
        sprite.setColor(s_darknessColor);
        landscape.setColor(s_darknessColor);

        m_landscape.display();
        landscape.setPosition(m_landscapeOffset);
        m_window.draw(landscape);

        m_screenContent.display();
        m_window.draw(sprite);
        m_screenContent.clear(sf::Color::Transparent);

        m_textOverlay.display();
        m_window.draw(text);

        m_window.display();
        sf::sleep(sf::milliseconds(16)); // idk, try 60 fps or something
    }
}

void Application::update()
{
}

void Application::drawToScreen(const sf::Drawable &sprite)
{
    s_instance->m_screenContent.draw(sprite);
}

void Application::drawSprite(const sf::Sprite &sprite)
{
    s_instance->m_screenContent.draw(sprite);
}

void Application::clearText(const int x, const int y, const int width, const int height)
{
    sf::RectangleShape shape(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Transparent);
    s_instance->m_textOverlay.draw(shape, sf::RenderStates(sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::Zero)));
}

void Application::drawToText(const sf::Sprite &sprite)
{
    s_instance->m_textOverlay.draw(sprite);
}

void Application::clearLandscape()
{
    s_instance->m_landscape.clear();
}

void Application::drawToLandscape(const sf::Sprite &sprite)
{
    s_instance->m_landscape.draw(sprite);
}

void Application::setLandscapeOffset(const int x, const int y)
{
    s_instance->m_landscapeOffset.x = -x;
    s_instance->m_landscapeOffset.y = -y;
}

void Application::clearScreenContent(const sf::Color &color)
{
    s_instance->m_screenContent.clear(color);
}

sf::Image Application::landscapeImage()
{
    s_instance->m_landscape.display();
    return s_instance->m_landscape.getTexture().copyToImage();
}
