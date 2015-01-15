#include "Application.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "World.hpp"

#include "types.hpp"

#include <ctime>
#include <cstdlib>
#include <exception>

Application::Application(const std::string& name, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
: m_window({ MAXX, MAXY }, name, sf::Style::Fullscreen)
, m_name(name)
, m_hWnd(m_window.getSystemHandle())
, m_hInstance(hInstance)
, m_hPrevInstance(hPrevInstance)
, m_lpCmdLine(lpCmdLine)
, m_nCmdShow(nCmdShow)
{
	// Set globals
	g_hInst = m_hInstance;
	hwnd = m_hWnd;

	Direct::InitDDraw();
	Direct::InitDSound();

	srand((unsigned)time(nullptr));		// Random initialisieren
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
		else if (event.type == sf::Event::GainedFocus)
		{
			bActive = TRUE;
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

int Application::run()
{
	sf::Clock timer;

	while (m_window.isOpen())
	{
		process_events();

		if (timer.getElapsedTime() > sf::milliseconds(1000))
		{
			if (Refresh() == 0)
				m_window.close();

			timer.restart();
		}
	}

	return 0;
}

short Application::Refresh()
{
	long Zeitsave;

	if (Spielzustand == SZNICHTS)
	{
		Spielzustand = SZLOGO;
		Game::InitStructs(); // Nur zum Wavinitialisieren
	}
	while (1)
	{
		Bild++;
		Zeitsave = time(nullptr);
		if (Zeit + 5 < Zeitsave)
		{
			Zeit = Zeitsave;
			LastBild = (LastBild + Bild / 5) / 2;
			Bild = 0;
			if (LastBild == 0) LastBild = 50;

			// BilderproSec ausgeben
			/*
			Renderer::Textloeschen(TXTFPS);
			TextBereich[TXTFPS].Aktiv = true;
			std::sprintf(StdString, "%d", LastBild);
			Renderer::DrawString(StdString,(short)TextBereich[TXTFPS].rcText.left,(short)TextBereich[TXTFPS].rcText.top,1);
			*/
		}
		if (Spielzustand == SZLOGO)
		{
			if (Direct::CheckKey() == 2) break;	// Das Keyboard abfragen
			Renderer::ZeigeLogo();				// Bild auffrischen
		}
		else if ((Spielzustand == SZINTRO) || (Spielzustand == SZGERETTET))
		{
			if (Direct::CheckKey() == 0) return(0); // Das Keyboard abfragen

			Math::Animationen();									// Animationen weiterschalten
			if (!Guy.Aktiv) Event(Guy.Aktion);						// Aktionen starten
			if (Guy.Pos.x == RouteStart.x) Renderer::ZeigeIntro();	// Bild auffrischen (if-Abfrage nötig (seltsamerweise))
		}
		else if (Spielzustand == SZSPIEL)
		{
			// Hide system cursor
			SetCursor(NULL);

			if ((Stunden >= 12) && (Minuten != 0) && (Guy.Aktion != Action::DAY_END)) // Hier ist der Tag zuende
			{
				if (Guy.Aktion == Action::LOOKOUT) Chance -= 1 + Scape[Guy.Pos.x][Guy.Pos.y].Hoehe;
				Guy.Aktiv = false;
				Guy.AkNummer = 0;
				Guy.Aktion = Action::DAY_END;
			}

			World::CheckSpzButton();				// Die Spezialknöpfe umschalten
			if (MouseAktiv) Direct::CheckMouse();	// Den MouseZustand abchecken
			if (Direct::CheckKey() == 0) return(0);	// Das Keyboard abfragen
			Renderer::LimitScroll();				// Das Scrollen an die Grenzen der Landschaft anpassen
			Math::Animationen();					// Die Animationsphasen weiterschalten
			if (!Guy.Aktiv) Event(Guy.Aktion);		// Die Aktionen starten
			Renderer::Zeige();						// Das Bild zeichnen
			if (Spielbeenden) return(0);

		}
		else if (Spielzustand == SZABSPANN)
		{
			if (Direct::CheckKey() == 0) return(0);
			Math::AbspannCalc();
			Renderer::ZeigeAbspann();
		}
	}
	return(1);
}

void Application::Event(short Eventnr)
{
	if (Eventnr != Action::NOTHING)
	{
		Routing::MarkRoute(false);
		RouteZiel.x = -1;
		RouteZiel.y = -1;
	}
	switch (Eventnr)
	{
	case Action::NOTHING:
		break;
	case Action::SEARCH:
		Action::search();
		break;
	case Action::EAT:
		Action::eat();
		break;
	case Action::DRINK:
		Action::drink();
		break;
	case Action::LOG:
		Action::log();
		break;
	case Action::FIELD:
		Action::field();
		break;
	case Action::DAY_END:
		Action::day_end();
		break;
	case Action::RESCUED:
		Action::rescued();
		break;
	case Action::TENT:
		Action::tent();
		break;
	case Action::SLEEP:
		Action::sleep();
		break;
	case Action::CANCEL:
		Action::cancel();
		break;
	case Action::FISH:
		Action::fish();
		break;
	case Action::BOAT:
		Action::boat();
		break;
	case Action::UNDOCK:
		Action::undock();
		break;
	case Action::DOCK:
		Action::dock();
		break;
	case Action::PIPE:
		Action::pipe();
		break;
	case Action::DESTROY:
		Action::destroy();
		break;
	case Action::SOS_SIGN:
		Action::sos();
		break;
	case Action::HOUSE1:
		Action::house1();
		break;
	case Action::HOUSE2:
		Action::house2();
		break;
	case Action::HOUSE3:
		Action::house3();
		break;
	case Action::FIREPLACE:
		Action::fireplace();
		break;
	case Action::LIGHT:
		Action::light();
		break;
	case Action::LOOKOUT:
		Action::lookout();
		break;
	case Action::TREASURE:
		Action::treasure();
		break;
	case Action::INTRO:
		Action::intro();
		break;
	case Action::SLINGSHOT:
		Action::slingshot();
		break;
	case Action::QUIT:
		Action::quit();
		break;
	case Action::RESTART:
		Action::restart();
		break;
	case Action::DAY_RESTART:
		Action::day_restart();
		break;
	case Action::DEATH:
		Action::death();
		break;
	}
}
