#include "Application.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "World.hpp"

#include "types.hpp"

#include <ctime>
#include <cstdlib>
#include <exception>

Application::Application(const std::string& name, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
: m_name(name)
, m_hInstance(hInstance)
, m_hPrevInstance(hPrevInstance)
, m_lpCmdLine(lpCmdLine)
, m_nCmdShow(nCmdShow)
{
	g_hInst = m_hInstance;

	if (!create_window())
		throw std::runtime_error("Failed to create a valid window.");

	hwnd = m_hWnd;

	Direct::InitDInput();
	MouseInit = true;

	ShowWindow(m_hWnd, m_nCmdShow);
	UpdateWindow(m_hWnd);

	Direct::InitDDraw();
	Direct::InitDSound();

	SetTimer(hwnd, 0, 1000, nullptr);	// Provisorisch
	srand((unsigned)time(nullptr));		// Random initialisieren
}

LRESULT CALLBACK Application::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Application* pApp = nullptr;

	if (uMsg == WM_NCCREATE)
	{
		// Get the pointer to the window from lpCreateParams which was set in create_window
		SetWindowLong(hWnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	// Get the pointer to the application
	pApp = Application::ptr(hWnd);

	// If we have the pointer, go to the message handler of the window else, use DefWindowProc
	if (pApp)
		return pApp->process_events(hWnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Application::process_events(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = hWnd;

	static BYTE phase = 0;

	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		bActive = wParam;
		Direct::SetAcquire();
		break;

	case WM_ACTIVATE: // Sent when window changes active state
		bActive = wParam;
		Direct::SetAcquire();
		break;

	case WM_CREATE:
		break;

	case WM_TIMER:
		if (Refresh() == 0)
			PostMessage(hwnd, WM_DESTROY, 0, 0);
		break;

	case WM_QUIT:
	case WM_DESTROY:
		Direct::finiObjects();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Application::create_window()
{
	WNDCLASS wc;

	// Set up and register window class
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = &Application::WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = m_name.c_str();
	wc.lpszClassName = m_name.c_str();
	RegisterClass(&wc);

	// Create a window
	m_hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		m_name.c_str(),
		m_name.c_str(),
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		nullptr,
		nullptr,
		m_hInstance,
		nullptr);

	return m_hWnd != nullptr;
}

int Application::run()
{
	while (GetMessage(&m_msg, nullptr, 0, 0))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}

	return m_msg.wParam;
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
			/*Textloeschen(TXTFPS);
			TextBereich[TXTFPS].Aktiv = true;
			std::sprintf(StdString, "%d", LastBild);
			DrawString(StdString,(short)TextBereich[TXTFPS].rcText.left,(short)TextBereich[TXTFPS].rcText.top,1);
			*/
		}
		if (Spielzustand == SZLOGO)
		{
			if (Direct::CheckKey() == 2) break; // Das Keyboard abfragen
			Renderer::ZeigeLogo(); //Bild auffrischen

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
	case Action::SOS:
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
