#include "Application.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "Renderer.hpp"
#include "State.hpp"
#include "World.hpp"

#include "types.hpp"

#include <ctime>
#include <cstdlib>

#include "extern.hpp"

Application::Application(const std::string& name, HINSTANCE instanceHandle)
    : m_window({MAXX, MAXY}, name, sf::Style::Fullscreen)
    , m_name(name)
    , m_time(std::time(nullptr))
{
    // Set globals
    g_hInst = instanceHandle;

    Sound::Initialize();
    Soundzustand = 1; // Activate sound

    Direct::InitializeDirectDraw(m_window.getSystemHandle());

    Spielzustand = State::Logo;
    Game::InitializeWaves(); // Only for Wav initialization

    srand(static_cast<unsigned>(std::time(nullptr)));
}

void Application::ProcessEvents()
{
    for (auto event = sf::Event{}; m_window.pollEvent(event);)
    {
        if (event.type == sf::Event::Closed)
        {
            Direct::FinalizeObjects();
            m_window.close();
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::F4)
            {
                Direct::FinalizeObjects();
                m_window.close();
            }
        }
    }
}

void Application::Run()
{
    sf::Clock timer;

    while (m_window.isOpen())
    {
        ProcessEvents();

        if (timer.getElapsedTime() > sf::seconds(1.f))
        {
            while (true)
            {
                Bild++;
                const auto timeSave = std::time(nullptr);
                if (m_time + 5 < timeSave)
                {
                    m_time = timeSave;
                    LastBild = static_cast<short>((LastBild + Bild / 5) / 2);
                    Bild = 0;
                    
                    if (LastBild == 0)
                    {
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

                if (Spielzustand == State::Logo)
                {
                    if (Direct::CheckKey() == 2) // Das Keyboard abfragen
                    {
                        break;
                    }

                    Renderer::DisplayLogo(); // Bild auffrischen
                }
                else if ((Spielzustand == State::Intro) || (Spielzustand == State::Rescued))
                {
                    if (Direct::CheckKey() == 0) // Das Keyboard abfragen
                        {
                        m_window.close();
                        break;
                        }

                    Math::Animations(); // Animationen weiterschalten
                    if (!Guy.Aktiv)     // Aktionen starten
                    {
                        Action::Handler(static_cast<Action::Actions>(Guy.Aktion));
                    }

                    Renderer::DisplayIntro(); // Bild auffrischen
                }
                else if (Spielzustand == State::Game)
                {
                    // Hide system cursor
                    SetCursor(nullptr);

                    if ((Stunden >= 12) && (Minuten != 0) && (Guy.Aktion != static_cast<int>(Action::Actions::DayEnd))) // Hier ist der Tag zuende
                    {
                        if (Guy.Aktion == static_cast<int>(Action::Actions::Lookout))
                        {
                            Chance -= 1 + Scape[Guy.Pos.x][Guy.Pos.y].Hoehe;
                        }

                        Guy.Aktiv = false;
                        Guy.AkNummer = 0;
                        Guy.Aktion = static_cast<short>(Action::Actions::DayEnd);
                    }

                    World::CheckSpecialButton(); // Die Spezialknöpfe umschalten
                    Direct::CheckMouse();        // Den MouseZustand abchecken
                    if (Direct::CheckKey() == 0) // Das Keyboard abfragen
                    {
                        m_window.close();
                        break;
                    }
                    Renderer::LimitScroll(); // Das Scrollen an die Grenzen der Landschaft anpassen
                    Math::Animations();      // Die Animationsphasen weiterschalten
                    if (!Guy.Aktiv)          // Die Aktionen starten
                    {
                        Action::Handler(static_cast<Action::Actions>(Guy.Aktion));
                    }
                    Renderer::DisplayLandscape(); // Das Bild zeichnen
                }
                else if (Spielzustand == State::Outro)
                {
                    if (Direct::CheckKey() == 0)
                    {
                        m_window.close();
                        break;
                    }

                    Math::CalculateCreditCoordinates();
                    Renderer::DisplayCredits();
                }
            }

            timer.restart();
        }
    }
}

void Application::Update()
{
}
