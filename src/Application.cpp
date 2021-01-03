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
    : m_window({MAX_SCREEN_X, MAX_SCREEN_Y}, name, sf::Style::Default)
, m_name(name)
, m_time(std::time(nullptr))
{
    s_instance = this;

    m_window.setMouseCursorVisible(false);

    puts(name.data());

    Sound::Init();
    Soundzustand = 1; // Activate sound

    Direct::InitDDraw();

    Spielzustand = State::LOGO;
    Game::InitWaves(); // Nur zum Wavinitialisieren

    m_screenContent.create(MAX_SCREEN_X, MAX_SCREEN_Y);
    m_textOverlay.create(MAX_SCREEN_X, MAX_SCREEN_Y);
    m_landscape.create(2 * MAX_SURFACE_X, 2 * MAX_SURFACE_Y);

    srand(static_cast<unsigned>(std::time(nullptr))); // Random initialisieren
}

void Application::process_events()
{
    sf::Event event;

    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            Direct::finiObjects();
            m_window.close();
        } else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::F4) {
                Direct::finiObjects();
                m_window.close();
            }
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
        if (timer.getElapsedTime() > sf::milliseconds(1000)) {
            while (m_window.isOpen()) {
                m_window.clear(sf::Color::Black);
                process_events();
                Bild++;
                std::time_t Zeitsave = std::time(nullptr);

                if (m_time + 5 < Zeitsave) {
                    m_time = Zeitsave;
                    LastBild = (LastBild + Bild / 5) / 2;
                    Bild = 0;

                    if (LastBild == 0) {
                        LastBild = 50;
                    }

                    // BilderproSec ausgeben
                    /*
                    Renderer::Textloeschen(TXTFPS);
                    TextBereich[TXTFPS].Aktiv = true;
                    std::sprintf(StdString, "%d", LastBild);
                    Renderer::DrawString(StdString,(short)TextBereich[TXTFPS].rcText.left,(short)TextBereich[TXTFPS].rcText.top,1);
                    */
                }

                if (Spielzustand == State::LOGO) {
                    if (Direct::CheckKey() == 2) { // Das Keyboard abfragen
                        break;
                    }
                    Renderer::ShowLogo(); // Bild auffrischen

                } else if ((Spielzustand == State::INTRO) || (Spielzustand == State::RESCUED)) {
                    if (Direct::CheckKey() == 0) { // Das Keyboard abfragen
                        m_window.close();
                        break;
                    }

                    Math::Animationen(); // Animationen weiterschalten

                    if (!Guy.IsActive) { // Aktionen starten
                        Action::handler(Guy.CurrentAction);
                    }

                    Renderer::ShowIntro(); // Bild auffrischen
                } else if (Spielzustand == State::GAME) {
                    // Hide system cursor
//                    SetCursor(nullptr);

                    if ((Hours >= 12) && (Minutes != 0) && (Guy.CurrentAction != Action::DAY_END)) { // Hier ist der Tag zuende
                        if (Guy.CurrentAction == Action::LOOKOUT) {
                            Chance -= 1 + Landscape[Guy.Pos.x][Guy.Pos.y].Height;
                        }

                        Guy.IsActive = false;
                        Guy.ActionNumber = 0;
                        Guy.CurrentAction = Action::DAY_END;
                    }

                    World::UpdateButtons(); // Die Spezialkn�pfe umschalten
                    Direct::CheckMouse(m_window); // Den MouseZustand abchecken

                    if (Direct::CheckKey() == 0) { // Das Keyboard abfragen
                        m_window.close();
                        break;
                    }

                    Renderer::LimitScroll(); // Das Scrollen an die Grenzen der Landschaft anpassen
                    Math::Animationen(); // Die Animationsphasen weiterschalten

                    if (!Guy.IsActive) { // Die Aktionen starten
                        Action::handler(Guy.CurrentAction);
                    }

                    Renderer::Show(); // Das Bild zeichnen
                } else if (Spielzustand == State::OUTRO) {
                    if (Direct::CheckKey() == 0) {
                        m_window.close();
                        break;
                    }

                    Math::AbspannCalc();
                    Renderer::ShowCredits();
                }

                puts("main rendering");


//                texture.loadFromImage(*lpDDSScape);
//                sprite.setTexture(texture);
//                m_window.draw(sprite);


                // TODO: this is probably the worst and least efficient way to render things
                // I'm almost proud.
//                texture.loadFromImage(*lpDDSBack);
//                sprite.setTexture(*lpDDSBack);
//                m_window.draw(sprite);
//                if (darknessOverlay) {
//                    assert(darknessOverlay->getSize().x > 0 && darknessOverlay->getSize().y > 0);
//                    sprite.setTexture(*darknessOverlay);
//                    m_window.draw(sprite);
//                }

                m_landscape.display();
                landscape.setPosition(m_landscapeOffset);
                m_window.draw(landscape);
//                m_landscape.clear(sf::Color::Transparent);

                m_screenContent.display();
                m_window.draw(sprite);
                m_screenContent.clear(sf::Color::Transparent);

                m_textOverlay.display();
                m_window.draw(text);
                m_textOverlay.clear(sf::Color::Transparent);

////                m_landscapeOverlay.clear();

                m_window.display();
                sf::sleep(sf::milliseconds(16)); // idk, try 60 fps or something
            }

            timer.restart();
        }
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
    printf("landscape x: %d, y: %d\n", x, y);
    s_instance->m_landscapeOffset.x = -x;
    s_instance->m_landscapeOffset.y = -y;
}

void Application::clearScreenContent()
{
    s_instance->m_screenContent.clear();
}

sf::Image Application::landscapeImage()
{
    s_instance->m_landscape.display();
    return s_instance->m_landscape.getTexture().copyToImage();
}
