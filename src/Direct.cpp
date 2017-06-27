#include "Direct.hpp"

#include "types.hpp"

#include "Action.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

#include <SFML/Window.hpp>

namespace Direct
{
	LPDIRECTDRAW4 lpDD = NULL; // DirectDraw object
	bool          Button0down; // linke Maustaste gedrückt gehalten
	bool          Button1down; // rechte Maustaste gedrückt gehalten

	void finiObjects()
	{
		if(lpDD != nullptr)
		{
			if(lpDDSPrimary != nullptr)
			{
				lpDDSPrimary->Release();
				lpDDSPrimary = nullptr;
			}
			if(lpDDPal != nullptr)
			{
				lpDDPal->Release();
				lpDDPal = nullptr;
			}
			lpDD->Release();
			lpDD = nullptr;
		}
	}

	void InitDDraw(HWND hWnd)
	{
		DDSCAPS2     ddscaps;
		LPDIRECTDRAW pDD;

		// Create the main DirectDraw object
		HRESULT ddrval = DirectDrawCreate(nullptr, &pDD, nullptr);
		if (ddrval != DD_OK)
			goto error;
		ddrval = pDD->QueryInterface(IID_IDirectDraw4, reinterpret_cast<LPVOID *>(& lpDD));
		if (ddrval != DD_OK)
			goto error;

		// Get exclusive mode
		ddrval = lpDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
		if (ddrval != DD_OK)
			goto error;

		// Set the video mode to 800x600x16
		ddrval = lpDD->SetDisplayMode(MAXX, MAXY, 32, 0, 0);
		if (ddrval != DD_OK)
		{
			switch (ddrval)
			{
			case DDERR_GENERIC:
			case DDERR_INVALIDMODE:
			case DDERR_INVALIDOBJECT:
			case DDERR_INVALIDPARAMS:
			case DDERR_LOCKEDSURFACES:
			case DDERR_NOEXCLUSIVEMODE:
			case DDERR_SURFACEBUSY:
			case DDERR_UNSUPPORTED:
			case DDERR_UNSUPPORTEDMODE: {MessageBeep(MB_OK); break; }
			case DDERR_WASSTILLDRAWING:
				;
			}

			goto error;
		}

		ZeroMemory(&ddsd2, sizeof(ddsd2));
		ddsd2.dwSize = sizeof(ddsd2);

		// Create the primary surface with 1 back buffer
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 1; // Anzahl ??
		ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, nullptr);
		if (ddrval != DD_OK)
			goto error;

		// für gamma-ablenden
		lpDDSPrimary->QueryInterface(IID_IDirectDrawGammaControl, reinterpret_cast<void **>(&lpDDGammaControl));
		lpDDGammaControl->GetGammaRamp(0, &DDGammaOld);
		lpDDGammaControl->GetGammaRamp(0, &DDGammaRamp);

		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);

		if (ddrval != DD_OK)
			goto error;

		ddsd.dwSize = sizeof(ddsd);  // Tell DirectDraw which members are valid. 
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		// In diese Surface sollen die Bausteine geladen werden
		lpDD->CreateSurface(&ddsd, &lpDDSMisc, nullptr);
		lpDDSMisc = DDLoadBitmap(lpDD, Misc, 0, 0);
		DDSetColorKey(lpDDSMisc, RGB(255, 0, 255));
		// In diese Surface sollen das Panel geladen werden
		lpDD->CreateSurface(&ddsd, &lpDDSPanel, nullptr);
		lpDDSPanel = DDLoadBitmap(lpDD, Panel, 0, 0);
		DDSetColorKey(lpDDSPanel, RGB(255, 0, 255));
		// In diese Surface sollen die Animation der Figur gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSGuyAni, nullptr);
		lpDDSGuyAni = DDLoadBitmap(lpDD, GuyAni, 0, 0);
		DDSetColorKey(lpDDSGuyAni, RGB(255, 0, 255));
		// In diese Surface sollen die Landschaftsanimationen gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSAnimation, nullptr);
		lpDDSAnimation = DDLoadBitmap(lpDD, Animation, 0, 0);
		DDSetColorKey(lpDDSAnimation, RGB(255, 0, 255));
		// In diese Surface soll die Schrift1 gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSSchrift1, nullptr);
		lpDDSSchrift1 = DDLoadBitmap(lpDD, Schrift1, 0, 0);
		DDSetColorKey(lpDDSSchrift1, RGB(255, 0, 255));
		// In diese Surface soll die Schrift2 gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSSchrift2, nullptr);
		lpDDSSchrift2 = DDLoadBitmap(lpDD, Schrift2, 0, 0);
		DDSetColorKey(lpDDSSchrift2, RGB(255, 0, 255));
		// In diese Surface soll das Papier gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSPapier, nullptr);
		lpDDSPapier = DDLoadBitmap(lpDD, Papier, 0, 0);
		DDSetColorKey(lpDDSPapier, RGB(255, 0, 255));
		// In diese Surface solln die Bäume gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSBaum, nullptr);
		lpDDSBaum = DDLoadBitmap(lpDD, Baum, 0, 0);
		DDSetColorKey(lpDDSBaum, RGB(255, 0, 255));
		// In diese Surface solln die Cursor gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSCursor, nullptr);
		lpDDSCursor = DDLoadBitmap(lpDD, Cursorbmp, 0, 0);
		DDSetColorKey(lpDDSCursor, RGB(255, 0, 255));
		// In diese Surface solln die Buttons gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSButtons, nullptr);
		lpDDSButtons = DDLoadBitmap(lpDD, Buttons, 0, 0);
		DDSetColorKey(lpDDSButtons, RGB(255, 0, 255));
		// In diese Surface solln das TextFeld gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSTextFeld, nullptr);
		lpDDSTextFeld = DDLoadBitmap(lpDD, TextFeld, 0, 0);
		DDSetColorKey(lpDDSTextFeld, RGB(255, 0, 255));
		// In diese Surface solln das Inventar gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSInventar, nullptr);
		lpDDSInventar = DDLoadBitmap(lpDD, Inventarbmp, 0, 0);
		DDSetColorKey(lpDDSInventar, RGB(255, 0, 255));
		// In diese Surface solln die Bauwerke gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSBau, nullptr);
		lpDDSBau = DDLoadBitmap(lpDD, Bau, 0, 0);
		DDSetColorKey(lpDDSBau, RGB(255, 0, 255));
		// In diese Surface solln die Credits gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSCredits, nullptr);
		lpDDSCredits = DDLoadBitmap(lpDD, Credits, 0, 0);
		DDSetColorKey(lpDDSCredits, RGB(0, 0, 0)); //Ausnahmsweise schwarz
		// In diese Surface solln das Logo gespeichert werden
		lpDD->CreateSurface(&ddsd, &lpDDSLogo, nullptr);
		lpDDSLogo = DDLoadBitmap(lpDD, Logo, 0, 0);

		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);  // Tell DirectDraw which members are valid. 
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		// In diese Surface soll die MiniMap gespeichert werden
		ddsd.dwWidth = 2 * MAXXKACH;
		ddsd.dwHeight = 2 * MAXYKACH;
		lpDD->CreateSurface(&ddsd, &lpDDSKarte, nullptr);
		// In diese Surface soll die Landschaft gespeichert werden
		ddsd.dwWidth = MAXSCAPEX;
		ddsd.dwHeight = MAXSCAPEY;
		lpDD->CreateSurface(&ddsd, &lpDDSScape, nullptr);
		// In diese Surface soll die Schrift gespeichert werden
		ddsd.dwWidth = MAXX;
		ddsd.dwHeight = MAXY;
		lpDD->CreateSurface(&ddsd, &lpDDSSchrift, nullptr);
		DDSetColorKey(lpDDSSchrift, RGB(255, 0, 255));
		// In diese Surface soll die Schatzkarte gespeichert werden
		ddsd.dwWidth = SKARTEX;
		ddsd.dwHeight = SKARTEY;
		lpDD->CreateSurface(&ddsd, &lpDDSSchatzkarte, nullptr);


		ddbltfx.dwSize = sizeof(ddbltfx);
		ddpf.dwSize = sizeof(ddpf);
		lpDDSSchrift->GetPixelFormat(&ddpf);

	error:
		if (ddrval != DD_OK)
		{
			finiObjects();
			MessageBox(hWnd, ("DirectDraw Init FAILED: " + std::to_string(ddrval)).c_str(), "Landscape", MB_OK);
			DestroyWindow(hWnd);
		}
	}

	void Reset()
	{
		Button0down = false;
		Button1down = false;
	}

	void CheckMouse()
	{
		// Mausbewegung
		short xDiff = MousePosition.x - sf::Mouse::getPosition().x; // Die Differenz zur vorherigen Position ((Für Scrollen)
		MousePosition.x = sf::Mouse::getPosition().x;
		if (MousePosition.x < 0)
			MousePosition.x = 0;
		if (MousePosition.x > MAXX - 2)
			MousePosition.x = MAXX - 2;
		short yDiff = MousePosition.y - sf::Mouse::getPosition().y; // Die Differenz zur vorherigen Position ((Für Scrollen)
		MousePosition.y = sf::Mouse::getPosition().y;
		if (MousePosition.y < 0)
			MousePosition.y = 0;
		if (MousePosition.y > MAXY - 2)
			MousePosition.y = MAXY - 2;

		if (TwoClicks == -1)
		{
			if (Guy.Aktiv)
			{
				if (Math::InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSTOP].rcDes) && (Bmp[BUTTSTOP].Phase != -1))
					CursorTyp = CUPFEIL;
				else
					CursorTyp = CUUHR;
			}
			else
				CursorTyp = CUPFEIL;
		}

		short Button = -1;	// Welcher Knopf ist gedrückt worden
		short Push = 0;		// Knopf gedrückt(1) oder losgelassen(-1) oder gedrückt(0) gehalten

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Button = 0;
			if (Button0down)
				Push = 0;
			else
			{
				Push = 1;
				Button0down = true;
			}
		}
		else
		{
			if (Button0down)
			{
				Button = 0;
				Push = -1;
				Button0down = false;
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			Button = 1;
			if (Button1down)
				Push = 0;
			else
			{
				Push = 1;
				Button1down = true;
			}
		}
		else
		{
			if (Button1down)
			{
				Button = 1;
				Push = -1;
				Button1down = false;
			}
		}

		// Wenn ein Text steht, dann bei Mausdruck Text weg 
		if (PapierText != -1)
		{
			if (Frage == 0)
			{
				if (Math::InRect(MousePosition.x, MousePosition.y, Bmp[JA].rcDes))
				{
					CursorTyp = CUPFEIL;
					if ((Button == 0) && (Push == 1))
					{
						Frage = 1;
						Renderer::Textloeschen(TXTPAPIER);
						PapierText = -1;
						Guy.Aktiv = false;
						Sound::PlaySound(Sound::CLICK2, 100);
					}
				}
				else if (Math::InRect(MousePosition.x, MousePosition.y, Bmp[NEIN].rcDes))
				{
					CursorTyp = CUPFEIL;
					if ((Button == 0) && (Push == 1))
					{
						Frage = 2;
						Renderer::Textloeschen(TXTPAPIER);
						PapierText = -1;
						Guy.Aktiv = false;
						Sound::PlaySound(Sound::CLICK2, 100);
					}
				}
				else if ((Button == 0) && (Push == 1))
					Sound::PlaySound(Sound::CLICK, 100);
			}
			else if ((Button != -1) && (Push == 1))
			{
				Renderer::Textloeschen(TXTPAPIER);
				PapierText = -1;
				Guy.Aktiv = false;
			}
			return;

		}

		// Animationen und Text löschen (werden dann von den MouseIn.. Sachen neu angestellt
		Renderer::Textloeschen(TXTTEXTFELD);
		Math::ButtAniAus();

		// Wenn der Guy aktiv dann linke Mouse-Buttons ignorieren
		if ((Guy.Aktiv) && (Button == 0))
		{
			if (!(Math::InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSTOP].rcDes)) ||
				(Bmp[BUTTSTOP].Phase == -1))
				Button = -1;
		}

		// die Maus ist in der Spielflaeche ->
		if (Math::InRect(MousePosition.x, MousePosition.y, rcSpielflaeche))
			Math::MouseInSpielflaeche(Button, Push, xDiff, yDiff);
		// die Maus ist im Panel ->
		if (Math::InRect(MousePosition.x, MousePosition.y, rcPanel))
			Math::MouseInPanel(Button, Push);
	}

	short CheckKey()
	{
		if (Spielzustand == State::LOGO)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) // Logo Abbrechen
			{
				Sound::StopSound(Sound::LOGO);
				Game::NeuesSpiel(false);
				return 2;
			}
		}
		else if (Spielzustand == State::INTRO)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) // Intro Abbrechen
			{
				Sound::StopSound(Sound::STORM);		// Sound hier sofort stoppen
				Sound::StopSound(Sound::SWIM);	// Sound hier sofort stoppen
				Guy.Aktiv = false;
				for (short x = Guy.Pos.x; x<MAXXKACH; x++)
				{
					Guy.Pos.x = x;
					World::Entdecken();
					if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1)
						break;
				}
				Scape[Guy.Pos.x - 2][Guy.Pos.y].Objekt = WRACK;
				Scape[Guy.Pos.x - 2][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[WRACK].rcDes.left);
				Scape[Guy.Pos.x - 2][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[WRACK].rcDes.top);

				Guy.PosScreen.x =
					(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
					Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x) / 2;
				Guy.PosScreen.y =
					(Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y +
					Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2;
				RouteStart.x = -1;
				RouteStart.y = -1;
				RouteZiel.x = -1;
				RouteZiel.y = -1;
				Camera.x = Guy.PosScreen.x - static_cast<short>(rcSpielflaeche.right / 2);
				Camera.y = Guy.PosScreen.y - static_cast<short>(rcSpielflaeche.bottom / 2);
				if (BootsFahrt) World::ChangeBootsFahrt();
				Guy.Zustand = GUYLINKS;
				Guy.Aktion = Action::NOTHING;
				Spielzustand = State::GAME;
				Guy.PosAlt = Guy.PosScreen;
				Game::SaveGame();
				return 1;
			}
		}
		else if (Spielzustand == State::RESCUED)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				Spielzustand = State::OUTRO;
				return 1;
			}
		}
		else if (Spielzustand == State::GAME)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				Camera.x += 10;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				Camera.x -= 10;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				Camera.y += 10;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				Camera.y -= 10;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				Guy.AkNummer = 0;
				Guy.Aktiv = false;
				Guy.Aktion = Action::QUIT;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11))
			{
				Guy.AkNummer = 0;
				Guy.Aktiv = false;
				Guy.Aktion = Action::RESTART;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
			{
				Gitter = !Gitter;
				World::Generate();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				LAnimation = !LAnimation;
				World::Generate();
			}

			// Development
			/*
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			{
				short x,y;
				for (y=0;y<MAXYKACH;y++)
				for (x=0;x<MAXXKACH;x++)
				Scape[x][y].Entdeckt = true;
				World::Generate();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
			{
				Guy.Inventar[ROHAST] = 10;
				Guy.Inventar[ROHSTEIN] = 10;
				Guy.Inventar[ROHBLATT] = 10;
				Guy.Inventar[ROHLIANE] = 10;
				Guy.Inventar[ROHSTAMM] = 9;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				Guy.Inventar[ROHAXT]   = 1;
				Guy.Inventar[ROHEGGE]  = 1;
				Guy.Inventar[ROHANGEL]  = 1;
				Guy.Inventar[ROHHAMMER]   = 1;
				Guy.Inventar[ROHFERNROHR] = 1;
				Guy.Inventar[ROHSTREICHHOLZ] = 1;
				Guy.Inventar[ROHSCHAUFEL] = 1;
				Guy.Inventar[ROHKARTE] = 1;
				Guy.Inventar[ROHSCHLEUDER] = 1;

				Bmp[BUTTFAELLEN].Phase  = 0;
				Bmp[BUTTANGELN].Phase  = 0;
				Bmp[BUTTANZUENDEN].Phase  = 0;
				Bmp[BUTTAUSSCHAU].Phase = 0;
				Bmp[BUTTSCHATZKARTE].Phase = 0;
				Bmp[BUTTSCHATZ].Phase = 0;
				Bmp[BUTTSCHLEUDER].Phase = 0;
				Bmp[BUTTFELD].Phase  = 0;
				Bmp[BUTTBOOT].Phase  = 0;
				Bmp[BUTTROHR].Phase  = 0;
				Bmp[BUTTHAUS1].Phase = 0;
				Bmp[BUTTHAUS2].Phase = 0;
				Bmp[BUTTHAUS3].Phase = 0;
			}*/

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				if (Soundzustand == 0)
					Soundzustand = 1;
				else if (Soundzustand == 1)
					Soundzustand = 0;
			}
		}
		else if (Spielzustand == State::OUTRO)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				Sound::StopSound(Sound::OUTRO);
				return 0;
			}
		}
		return 1;
	}

} // namespace Direct
