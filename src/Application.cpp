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

Application::Application(const std::string& name, HINSTANCE instance_handle)
	: m_window({MAXX, MAXY}, name, sf::Style::Fullscreen)
	  , m_name(name)
	  , m_time(time(nullptr))
{
	// Set globals
	g_hInst = instance_handle;

	Sound::Init();
	Soundzustand = 1; // Activate sound

	Direct::InitDDraw(m_window.getSystemHandle());

	Spielzustand = State::LOGO;
	Game::InitWaves(); // Nur zum Wavinitialisieren

	srand(static_cast<unsigned>(time(nullptr))); // Random initialisieren
}

void Application::process_events()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			Direct::finiObjects();
			m_window.close();
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::F4)
			{
				Direct::finiObjects();
				m_window.close();
			}
		}
	}
}

void Application::run()
{
	sf::Clock timer;

	while (m_window.isOpen())
	{
		process_events();

		if (timer.getElapsedTime() > sf::milliseconds(1000))
		{
			while (true)
			{
				Bild++;
				time_t Zeitsave = time(nullptr);
				if (m_time + 5 < Zeitsave)
				{
					m_time = Zeitsave;
					LastBild = (LastBild + Bild / 5) / 2;
					Bild = 0;
					if (LastBild == 0)
						LastBild = 50;

					// BilderproSec ausgeben
					/*
					Renderer::Textloeschen(TXTFPS);
					TextBereich[TXTFPS].Aktiv = true;
					std::sprintf(StdString, "%d", LastBild);
					Renderer::DrawString(StdString,(short)TextBereich[TXTFPS].rcText.left,(short)TextBereich[TXTFPS].rcText.top,1);
					*/
				}

				if (Spielzustand == State::LOGO)
				{
					if (Direct::CheckKey() == 2) // Das Keyboard abfragen
						break;

					Renderer::ZeigeLogo(); // Bild auffrischen
				}
				else if ((Spielzustand == State::INTRO) || (Spielzustand == State::RESCUED))
				{
					if (Direct::CheckKey() == 0) // Das Keyboard abfragen
					{
						m_window.close();
						break;
					}

					Math::Animationen(); // Animationen weiterschalten
					if (!Guy.Aktiv) // Aktionen starten
						Action::handler(Guy.Aktion);

					Renderer::ZeigeIntro(); // Bild auffrischen
				}
				else if (Spielzustand == State::GAME)
				{
					// Hide system cursor
					SetCursor(nullptr);

					if ((Stunden >= 12) && (Minuten != 0) && (Guy.Aktion != Action::DAY_END)) // Hier ist der Tag zuende
					{
						if (Guy.Aktion == Action::LOOKOUT)
							Chance -= 1 + Scape[Guy.Pos.x][Guy.Pos.y].Hoehe;

						Guy.Aktiv = false;
						Guy.AkNummer = 0;
						Guy.Aktion = Action::DAY_END;
					}

					World::CheckSpzButton(); // Die Spezialknöpfe umschalten
					Direct::CheckMouse(); // Den MouseZustand abchecken
					if (Direct::CheckKey() == 0) // Das Keyboard abfragen
					{
						m_window.close();
						break;
					}
					Renderer::LimitScroll(); // Das Scrollen an die Grenzen der Landschaft anpassen
					Math::Animationen(); // Die Animationsphasen weiterschalten
					if (!Guy.Aktiv) // Die Aktionen starten
						Action::handler(Guy.Aktion);
					Renderer::Zeige(); // Das Bild zeichnen
				}
				else if (Spielzustand == State::OUTRO)
				{
					if (Direct::CheckKey() == 0)
					{
						m_window.close();
						break;
					}

					Math::AbspannCalc();
					Renderer::ZeigeAbspann();
				}
			}

			timer.restart();
		}
	}
}

void Application::update()
{
}
