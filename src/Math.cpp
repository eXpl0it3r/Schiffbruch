#include "Math.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "World.hpp"

#include <cstdio>
#include <cstring>

namespace Math
{
	void MouseInSpielflaeche(short Button, short Push, short xDiff, short yDiff)
	{
		ZWEID	Erg;						// Die angeklickte Kachel
		char	Text[1024], TextTmp[1024];	// Text für Infoleiste

		// Info anzeigen
		Erg = Renderer::GetKachel((MousePosition.x + Camera.x), (MousePosition.y + Camera.y));
		if (Scape[Erg.x][Erg.y].Entdeckt)
		{
			LoadString(g_hInst, 45 + Scape[Erg.x][Erg.y].Art, Text, 1024);
			if ((Scape[Erg.x][Erg.y].Objekt != -1) && (Scape[Erg.x][Erg.y].Objekt != MEERWELLEN))
			{
				LoadString(g_hInst, MIT, TextTmp, 1024);
				std::strcat(Text, " ");
				std::strcat(Text, TextTmp);
				std::strcat(Text, " ");

				if ((Scape[Erg.x][Erg.y].Objekt >= BAUM1) && (Scape[Erg.x][Erg.y].Objekt <= BAUM4))
					LoadString(g_hInst, BAUMTEXT, TextTmp, 1024);
				else if ((Scape[Erg.x][Erg.y].Objekt >= FLUSS1) && (Scape[Erg.x][Erg.y].Objekt <= SCHLEUSE6))
					LoadString(g_hInst, FLUSSTEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == BUSCH)
					LoadString(g_hInst, BUSCHTEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == ZELT)
					LoadString(g_hInst, ZELTTEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == FELD)
					LoadString(g_hInst, FELDTEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == BOOT)
					LoadString(g_hInst, BOOTTEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == ROHR)
					LoadString(g_hInst, ROHRTEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == SOS)
					LoadString(g_hInst, SOSTEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == HAUS1)
					LoadString(g_hInst, HAUS1TEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == HAUS2)
					LoadString(g_hInst, HAUS2TEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == HAUS3)
					LoadString(g_hInst, HAUS3TEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == BAUMGROSS)
					LoadString(g_hInst, BAUMGROSSTEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == FEUERSTELLE)
					LoadString(g_hInst, FEUERSTELLETEXT, TextTmp, 1024);
				else if (Scape[Erg.x][Erg.y].Objekt == FEUER)
					LoadString(g_hInst, FEUERTEXT, TextTmp, 1024);
				else if ((Scape[Erg.x][Erg.y].Objekt == WRACK) || (Scape[Erg.x][Erg.y].Objekt == WRACK2))
					LoadString(g_hInst, WRACKTEXT, TextTmp, 1024);
				std::strcat(Text, TextTmp);

				if ((Scape[Erg.x][Erg.y].Objekt >= FELD) &&
					(Scape[Erg.x][Erg.y].Objekt <= FEUERSTELLE))
				{
					// Baufortschrittanzeigen
					std::strcat(Text, " ");
					std::strcat(Text, "(");
					std::sprintf(TextTmp, "%d", (Scape[Erg.x][Erg.y].AkNummer * 100) / Bmp[Scape[Erg.x][Erg.y].Objekt].AkAnzahl);
					std::strcat(Text, TextTmp);
					std::strcat(Text, "%");
					std::strcat(Text, ")");
					// benötigte Rohstoffe anzeigen
					World::MakeRohString(Erg.x, Erg.y, -1);
					std::strcat(Text, RohString);
				}

			}
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(Text, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);
		}

		// rechte Maustastescrollen
		if ((Button == 1) && (Push == 0))
		{
			Camera.x += xDiff;
			Camera.y += yDiff;
			CursorTyp = CURICHTUNG;
		}

		// Wenn Maustaste gedrückt wird
		if ((Button == 0) && (Push == 1))
		{
			if ((Erg.x != -1) && (Erg.y != -1) &&
				(Scape[Erg.x][Erg.y].Entdeckt) && (!Guy.Aktiv) &&
				((Erg.x != Guy.Pos.x) || (Erg.y != Guy.Pos.y)) &&
				(Erg.x > 0) && (Erg.x < MAXXKACH - 1) &&
				(Erg.y > 0) && (Erg.y < MAXYKACH - 1))
			{
				// Klicksound abspielen
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Erg.x == RouteZiel.x) && (Erg.y == RouteZiel.y))
				{
					Routing::MarkRoute(false);
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktiv = true;
					RoutePunkt = -1;
					Steps = 0;
					Step = 0;
				}
				else
				{
					Routing::MarkRoute(false);
					RouteStart.x = Guy.Pos.x;
					RouteStart.y = Guy.Pos.y;
					RouteZiel.x = Erg.x;
					RouteZiel.y = Erg.y;
					if (Routing::FindTheWay()) Routing::MarkRoute(true);
					else // Wenn keine Route gefunden
					{
						RouteStart.x = -1;
						RouteStart.y = -1;
						RouteZiel.x = -1;
						RouteZiel.y = -1;
						MessageBeep(MB_OK);
					}
				}
			}
			else Sound::PlaySound(WAVKLICK, 100);
		}
	}

	void MouseInPanel(short Button, short Push)
	{
		short mx, my, i; // Mauskoordinaten in Minimap

		// wenn die Maus in der Minimap ist ->
		if ((InRect(MousePosition.x, MousePosition.y, rcKarte)) && (Button == 0) && (Push != -1))
		{
			mx = MousePosition.x - (short)rcKarte.left;
			my = MousePosition.y - (short)rcKarte.top;
			Camera.x = ((KXPIXEL / 4)*(mx - my) + MAXXKACH * KXPIXEL / 2)
				- (rcSpielflaeche.right - rcSpielflaeche.left) / 2;
			Camera.y = ((KXPIXEL / 7)*(my + mx))
				- (rcSpielflaeche.bottom - rcSpielflaeche.top) / 2;
		}
		else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTGITTER].rcDes))
		{
			if (Gitter) Renderer::DrawText(GITTERAUS, TXTTEXTFELD, 2);
			else Renderer::DrawText(GITTERAN, TXTTEXTFELD, 2);

			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Gitter = !Gitter;
				World::Generate();
			}
		}
		else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTANIMATION].rcDes))
		{
			if (LAnimation) Renderer::DrawText(ANIMATIONAUS, TXTTEXTFELD, 2);
			else Renderer::DrawText(ANIMATIONAN, TXTTEXTFELD, 2);

			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				LAnimation = !LAnimation;
				World::Generate();
			}
		}
		else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSOUND].rcDes))
		{
			if (Soundzustand == 1) Renderer::DrawText(SOUNDAUS, TXTTEXTFELD, 2);
			else if (Soundzustand == 0) Renderer::DrawText(SOUNDAN, TXTTEXTFELD, 2);
			else Renderer::DrawText(KEINSOUND, TXTTEXTFELD, 2);

			if ((Button == 0) && (Push == 1))
			{
				if (Soundzustand == 1)
				{
					for (i = 1; i<WAVANZ; i++) Sound::StopSound(i);
					Soundzustand = 0;
				}
				else if (Soundzustand == 0)
				{
					Soundzustand = 1;
					Sound::PlaySound(WAVKLICK2, 100);
				}
				else Sound::PlaySound(WAVKLICK, 100);
			}
		}
		else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTBEENDEN].rcDes))
		{
			Renderer::DrawText(BEENDEN, TXTTEXTFELD, 2);
			Bmp[BUTTBEENDEN].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				Guy.Aktiv = false;
				Guy.Aktion = Action::QUIT;
			}
		}
		else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTNEU].rcDes))
		{
			Renderer::DrawText(NEU, TXTTEXTFELD, 2);
			Bmp[BUTTNEU].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				Guy.Aktiv = false;
				Guy.Aktion = Action::RESTART;
			}
		}
		else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTTAGNEU].rcDes))
		{
			Renderer::DrawText(TAGNEU2, TXTTEXTFELD, 2);
			Bmp[BUTTTAGNEU].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				Guy.Aktiv = false;
				Guy.Aktion = Action::DAY_RESTART;
			}
		}
		else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTAKTION].rcDes))
		{
			if (HauptMenue == MEAKTION) Renderer::DrawText(MEAKTIONZU, TXTTEXTFELD, 2);
			else Renderer::DrawText(MEAKTIONAUF, TXTTEXTFELD, 2);
			Bmp[BUTTAKTION].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if (HauptMenue == MEAKTION) HauptMenue = MEKEINS;
				else HauptMenue = MEAKTION;
			}
		}
		else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTBAUEN].rcDes) &&
			(Bmp[BUTTBAUEN].Phase != -1))
		{
			if (HauptMenue == MEBAUEN) Renderer::DrawText(MEBAUENZU, TXTTEXTFELD, 2);
			else Renderer::DrawText(MEBAUENAUF, TXTTEXTFELD, 2);
			Bmp[BUTTBAUEN].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if (HauptMenue == MEBAUEN) HauptMenue = MEKEINS;
				else HauptMenue = MEBAUEN;
			}
		}
		else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTINVENTAR].rcDes))
		{
			if (HauptMenue == MEINVENTAR) Renderer::DrawText(MEINVENTARZU, TXTTEXTFELD, 2);
			else Renderer::DrawText(MEINVENTARAUF, TXTTEXTFELD, 2);
			Bmp[BUTTINVENTAR].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if (HauptMenue == MEINVENTAR) HauptMenue = MEKEINS;
				else HauptMenue = MEINVENTAR;
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTWEITER].rcDes)) &&
			(Bmp[BUTTWEITER].Phase != -1))
		{
			Renderer::DrawText(WEITER, TXTTEXTFELD, 2);

			Bmp[BUTTWEITER].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Bmp[BUTTSTOP].Phase = 0;
				Routing::MarkRoute(false);
				RouteZiel.x = -1;
				RouteZiel.y = -1;
				Guy.PosAlt = Guy.PosScreen;
				Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
					Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
				switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
				{
				case ZELT: Guy.Aktion = Action::TENT; break;
				case FELD: Guy.Aktion = Action::FIELD; break;
				case BOOT: Guy.Aktion = Action::BOAT; break;
				case ROHR: Guy.Aktion = Action::PIPE; break;
				case SOS:  Guy.Aktion = Action::SOS_SIGN;  break;
				case HAUS1:Guy.Aktion = Action::HOUSE1; break;
				case HAUS2:Guy.Aktion = Action::HOUSE2; break;
				case HAUS3:Guy.Aktion = Action::HOUSE3; break;
				case FEUERSTELLE:Guy.Aktion = Action::FIREPLACE; break;
				}
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSTOP].rcDes)) &&
			(Bmp[BUTTSTOP].Phase != -1))
		{
			Renderer::DrawText(STOP, TXTTEXTFELD, 2);

			Bmp[BUTTSTOP].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				Guy.Aktion = Action::CANCEL;
				Bmp[BUTTSTOP].Phase = -1;
			}
		}

		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTABLEGEN].rcDes)) &&
			(Bmp[BUTTABLEGEN].Phase != -1))
		{
			Renderer::DrawText(BEGINNABLEGEN, TXTTEXTFELD, 2);
			Bmp[BUTTABLEGEN].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1) Guy.Aktion = Action::UNDOCK;
				else Guy.Aktion = Action::DOCK;

			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSUCHEN].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTSUCHEN].Phase != -1))
		{
			Renderer::DrawText(BEGINNSUCHEN, TXTTEXTFELD, 2);
			Bmp[BUTTSUCHEN].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				Guy.Aktion = Action::SEARCH;
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTESSEN].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTESSEN].Phase != -1))
		{
			Renderer::DrawText(BEGINNESSEN, TXTTEXTFELD, 2);
			Bmp[BUTTESSEN].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				if (((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BUSCH) ||
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == FELD)) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Phase == Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl - 1)) Guy.Aktion = Action::EAT;
				else if (((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= FLUSS1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= SCHLEUSE6)) ||
					((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == ROHR) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Phase == 1)))
					Guy.Aktion = Action::DRINK;
				else PapierText = Renderer::DrawText(KEINESSENTRINKEN, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSCHLAFEN].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTSCHLAFEN].Phase != -1))
		{
			Renderer::DrawText(BEGINNSCHLAFEN, TXTTEXTFELD, 2);
			Bmp[BUTTSCHLAFEN].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1)
				{
					Guy.AkNummer = 0;
					Guy.Aktion = Action::SLEEP;
				}
				else PapierText = Renderer::DrawText(NICHTAUFWASSERSCHLAFEN, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTFAELLEN].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTFAELLEN].Phase != -1))
		{
			Renderer::DrawText(BEGINNFAELLEN, TXTTEXTFELD, 2);
			Bmp[BUTTFAELLEN].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				if (Guy.Inventar[ROHSTAMM] <= 10)
				{
					if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= BAUM1) &&
						(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= BAUM4))
					{
						Guy.Aktion = Action::LOG;
					}
					else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BAUMGROSS) ||
						((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= HAUS1) &&
						(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= HAUS3)))
						PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
					else PapierText = Renderer::DrawText(KEINBAUM, TXTPAPIER, 1);
				}
				else PapierText = Renderer::DrawText(ROHSTAMMZUVIEL, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTANGELN].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTANGELN].Phase != -1))
		{
			Renderer::DrawText(BEGINNANGELN, TXTTEXTFELD, 2);
			Bmp[BUTTANGELN].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				if (((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= FLUSS1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= SCHLEUSE6)) ||
					(BootsFahrt)) Guy.Aktion = Action::FISH;
				else PapierText = Renderer::DrawText(KEINWASSER, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTANZUENDEN].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTANZUENDEN].Phase != -1))
		{
			Renderer::DrawText(BEGINNANZUENDEN, TXTTEXTFELD, 2);
			Bmp[BUTTANZUENDEN].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == FEUERSTELLE) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
					Guy.Aktion = Action::LIGHT;
				else PapierText = Renderer::DrawText(KEINEFEUERST, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTAUSSCHAU].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTAUSSCHAU].Phase != -1))
		{
			Renderer::DrawText(BEGINNAUSSCHAU, TXTTEXTFELD, 2);
			Bmp[BUTTAUSSCHAU].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1)
				{
					Guy.AkNummer = 0;
					Guy.Aktion = Action::LOOKOUT;
				}
				else PapierText = Renderer::DrawText(WELLENZUHOCH, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSCHATZ].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTSCHATZ].Phase != -1))
		{
			Renderer::DrawText(BEGINNSCHATZ, TXTTEXTFELD, 2);
			Bmp[BUTTSCHATZ].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Art != 1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1))
				{
					Guy.AkNummer = 0;
					Guy.Aktion = Action::TREASURE;
				}
				else PapierText = Renderer::DrawText(GRABENBEDINGUNGEN, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSCHLEUDER].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTSCHLEUDER].Phase != -1))
		{
			Renderer::DrawText(BEGINNSCHLEUDER, TXTTEXTFELD, 2);
			Bmp[BUTTSCHLEUDER].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Guy.AkNummer = 0;
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= BAUM1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= BAUM4))
				{
					Guy.AkNummer = 0;
					Guy.Aktion = Action::SLINGSHOT;
				}
				else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BAUMGROSS) ||
					((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= HAUS1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= HAUS3)))
					PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
				else PapierText = Renderer::DrawText(KEINVOGEL, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSCHATZKARTE].rcDes)) &&
			(HauptMenue == MEAKTION) && (Bmp[BUTTSCHATZKARTE].Phase != -1))
		{
			Renderer::DrawText(BEGINNSCHATZKARTE, TXTTEXTFELD, 2);
			Bmp[BUTTSCHATZKARTE].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				Renderer::DrawSchatzkarte();
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTFELD].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTFELD].Phase != -1))
		{
			LoadString(g_hInst, BEGINNFELD, StdString, 1024);
			World::MakeRohString(-1, -1, FELD);
			std::strcat(StdString, RohString);
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(StdString, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);

			Bmp[BUTTFELD].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Art == 4))
				{
					Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktion = Action::FIELD;
				}
				else if ((Bmp[BUTTWEITER].Phase != -1) && (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == FELD))
				{
					Bmp[BUTTSTOP].Phase = 0;
					Guy.PosAlt = Guy.PosScreen;
					Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
						Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
					Guy.Aktion = Action::FIELD;
				}
				else PapierText = Renderer::DrawText(FELDBEDINGUNGEN, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTZELT].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTZELT].Phase != -1))
		{
			LoadString(g_hInst, BEGINNZELT, StdString, 1024);
			World::MakeRohString(-1, -1, ZELT);
			std::strcat(StdString, RohString);
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(StdString, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);

			Bmp[BUTTZELT].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Art != -1))
				{
					Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktion = Action::TENT;
				}
				else if ((Bmp[BUTTWEITER].Phase != -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == ZELT))
				{
					Bmp[BUTTSTOP].Phase = 0;
					Guy.PosAlt = Guy.PosScreen;
					Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
						Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
					Guy.Aktion = Action::TENT;
				}
				else PapierText = Renderer::DrawText(ZELTBEDINGUNGEN, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTBOOT].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTBOOT].Phase != -1))
		{
			LoadString(g_hInst, BEGINNBOOT, StdString, 1024);
			World::MakeRohString(-1, -1, BOOT);
			std::strcat(StdString, RohString);
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(StdString, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);

			Bmp[BUTTBOOT].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Art == 2) &&
					((Scape[Guy.Pos.x - 1][Guy.Pos.y].Art == 1) ||
					(Scape[Guy.Pos.x][Guy.Pos.y - 1].Art == 1) ||
					(Scape[Guy.Pos.x + 1][Guy.Pos.y].Art == 1) ||
					(Scape[Guy.Pos.x][Guy.Pos.y + 1].Art == 1)))
				{
					Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktion = Action::BOAT;
				}
				else if ((Bmp[BUTTWEITER].Phase != -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BOOT))
				{
					Bmp[BUTTSTOP].Phase = 0;
					Guy.PosAlt = Guy.PosScreen;
					Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
						Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
					Guy.Aktion = Action::BOAT;
				}
				else PapierText = Renderer::DrawText(BOOTBEDINGUNGEN, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTROHR].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTROHR].Phase != -1))
		{
			LoadString(g_hInst, BEGINNROHR, StdString, 1024);
			World::MakeRohString(-1, -1, ROHR);
			std::strcat(StdString, RohString);
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(StdString, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);

			Bmp[BUTTROHR].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0))
				{
					Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktion = Action::PIPE;
				}
				else if ((Bmp[BUTTWEITER].Phase != -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == ROHR))
				{
					Bmp[BUTTSTOP].Phase = 0;
					Guy.PosAlt = Guy.PosScreen;
					Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
						Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
					Guy.Aktion = Action::PIPE;
				}
				else PapierText = Renderer::DrawText(ROHRBEDINGUNGEN, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTSOS].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTSOS].Phase != -1))
		{
			LoadString(g_hInst, BEGINNSOS, StdString, 1024);
			World::MakeRohString(-1, -1, SOS);
			std::strcat(StdString, RohString);
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(StdString, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);

			Bmp[BUTTSOS].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0))
				{
					Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktion = Action::SOS_SIGN;
				}
				else if ((Bmp[BUTTWEITER].Phase != -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == SOS))
				{
					Bmp[BUTTSTOP].Phase = 0;
					Guy.PosAlt = Guy.PosScreen;
					Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
						Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
					Guy.Aktion = Action::SOS_SIGN;
				}
				else PapierText = Renderer::DrawText(SOSBEDINGUNGEN, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTHAUS1].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTHAUS1].Phase != -1))
		{
			LoadString(g_hInst, BEGINNHAUS1, StdString, 1024);
			World::MakeRohString(-1, -1, HAUS1);
			std::strcat(StdString, RohString);
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(StdString, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);

			Bmp[BUTTHAUS1].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= BAUM1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= BAUM4))
					PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
				else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BAUMGROSS)
				{
					Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktion = Action::HOUSE1;
				}
				else if ((Bmp[BUTTWEITER].Phase != -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HAUS1))
				{
					Bmp[BUTTSTOP].Phase = 0;
					Guy.PosAlt = Guy.PosScreen;
					Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
						Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
					Guy.Aktion = Action::HOUSE1;
				}
				else PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTHAUS2].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTHAUS2].Phase != -1))
		{
			LoadString(g_hInst, BEGINNHAUS2, StdString, 1024);
			World::MakeRohString(-1, -1, HAUS2);
			std::strcat(StdString, RohString);
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(StdString, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);

			Bmp[BUTTHAUS2].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= BAUM1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= BAUM4))
					PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
				else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BAUMGROSS)
					PapierText = Renderer::DrawText(NICHTOHNELEITER, TXTPAPIER, 1);
				else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HAUS1)
				{
					Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktion = Action::HOUSE2;
				}
				else if ((Bmp[BUTTWEITER].Phase != -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HAUS2))
				{
					Bmp[BUTTSTOP].Phase = 0;
					Guy.PosAlt = Guy.PosScreen;
					Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
						Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
					Guy.Aktion = Action::HOUSE2;
				}
				else PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTHAUS3].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTHAUS3].Phase != -1))
		{
			LoadString(g_hInst, BEGINNHAUS3, StdString, 1024);
			World::MakeRohString(-1, -1, HAUS3);
			std::strcat(StdString, RohString);
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(StdString, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);

			Bmp[BUTTHAUS3].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= BAUM1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= BAUM4))
					PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
				else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BAUMGROSS) ||
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HAUS1))
					PapierText = Renderer::DrawText(NICHTOHNEPLATTFORM, TXTPAPIER, 1);
				else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HAUS2)
				{
					Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktion = Action::HOUSE3;
				}
				else if ((Bmp[BUTTWEITER].Phase != -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HAUS3))
				{
					Bmp[BUTTSTOP].Phase = 0;
					Guy.PosAlt = Guy.PosScreen;
					Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
						Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
					Guy.Aktion = Action::HOUSE3;
				}
				else PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTFEUERST].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTFEUERST].Phase != -1))
		{
			LoadString(g_hInst, BEGINNFEUERSTELLE, StdString, 1024);
			World::MakeRohString(-1, -1, FEUERSTELLE);
			std::strcat(StdString, RohString);
			TextBereich[TXTTEXTFELD].Aktiv = true;
			Renderer::DrawString(StdString, (short)TextBereich[TXTTEXTFELD].rcText.left,
				(short)TextBereich[TXTTEXTFELD].rcText.top, 2);

			Bmp[BUTTFEUERST].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0))
				{
					Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
					Bmp[BUTTSTOP].Phase = 0;
					Guy.Aktion = Action::FIREPLACE;
				}
				else if ((Bmp[BUTTWEITER].Phase != -1) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt == FEUERSTELLE))
				{
					Bmp[BUTTSTOP].Phase = 0;
					Guy.PosAlt = Guy.PosScreen;
					Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
						Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
					Guy.Aktion = Action::FIREPLACE;
				}
				else PapierText = Renderer::DrawText(FEUERSTELLENBEDINGUNGEN, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTDESTROY].rcDes)) &&
			(HauptMenue == MEBAUEN) && (Bmp[BUTTDESTROY].Phase != -1))
		{
			Renderer::DrawText(BEGINNDESTROY, TXTTEXTFELD, 2);
			Bmp[BUTTDESTROY].Animation = true;
			if ((Button == 0) && (Push == 1))
			{
				Sound::PlaySound(WAVKLICK2, 100);
				if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= FELD) &&
					(Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= FEUERSTELLE))
				{
					Guy.AkNummer = 0;
					Guy.Aktion = Action::DESTROY;
				}
				else PapierText = Renderer::DrawText(KEINBAUWERK, TXTPAPIER, 1);
			}
		}
		else if ((InRect(MousePosition.x, MousePosition.y, Bmp[INVPAPIER].rcDes)) && (HauptMenue == MEINVENTAR))
		{
			for (i = ROHAST; i <= ROHSCHLEUDER; i++)
			{
				if (InRect(MousePosition.x, MousePosition.y, Bmp[i].rcDes) && (Guy.Inventar[i]>0))
				{
					if ((Button == 0) && (Push == 1))
					{
						if (TwoClicks == -1)
						{
							CursorTyp = i;
							TwoClicks = i;
						}
						else World::CheckBenutze(i);
					}
					switch (i)
					{
					case ROHAST: Renderer::DrawText(AST, TXTTEXTFELD, 2); break;
					case ROHSTEIN: Renderer::DrawText(STEIN, TXTTEXTFELD, 2); break;
					case ROHAXT: Renderer::DrawText(AXT, TXTTEXTFELD, 2); break;
					case ROHBLATT: Renderer::DrawText(BLATT, TXTTEXTFELD, 2); break;
					case ROHSTAMM: Renderer::DrawText(STAMM, TXTTEXTFELD, 2); break;
					case ROHEGGE: Renderer::DrawText(EGGE, TXTTEXTFELD, 2); break;
					case ROHLIANE: Renderer::DrawText(LIANE, TXTTEXTFELD, 2); break;
					case ROHANGEL: Renderer::DrawText(ANGEL, TXTTEXTFELD, 2); break;
					case ROHHAMMER: Renderer::DrawText(HAMMER, TXTTEXTFELD, 2); break;
					case ROHFERNROHR: Renderer::DrawText(FERNROHR, TXTTEXTFELD, 2); break;
					case ROHSTREICHHOLZ: Renderer::DrawText(STREICHHOLZ, TXTTEXTFELD, 2); break;
					case ROHSCHAUFEL: Renderer::DrawText(SCHAUFEL, TXTTEXTFELD, 2); break;
					case ROHKARTE: Renderer::DrawText(KARTE, TXTTEXTFELD, 2); break;
					case ROHSCHLEUDER: Renderer::DrawText(SCHLEUDER, TXTTEXTFELD, 2); break;
					}

					break;
				}
			}
		}
		else if (InRect(MousePosition.x, MousePosition.y, TextBereich[TXTTAGESZEIT].rcText))
			Renderer::DrawText(SOSPAET, TXTTEXTFELD, 2);
		else if (InRect(MousePosition.x, MousePosition.y, TextBereich[TXTCHANCE].rcText))
			Renderer::DrawText(CHANCETEXT, TXTTEXTFELD, 2);
		else // TwoClicks löschen
		{
			if ((Button == 0) && (Push == 1)) Sound::PlaySound(WAVKLICK, 100);
			TwoClicks = -1;
		}
	}

	bool InDreieck(short X, short Y, short X0, short Y0, short X1, short Y1, short X3, short Y3)
	{
		float	x, y, x0, y0, x1, y1, x3, y3, a, b, c, d;

		x = (float)X; y = (float)Y; x0 = (float)X0; y0 = (float)Y0; x1 = (float)X1; y1 = (float)Y1; x3 = (float)X3; y3 = (float)Y3;

		c = (x - x1) / (x0 - x1);
		if (c<0) return false;
		d = ((y - y3)*(x0 - x1) - (x - x1)*(y0 - y3)) / ((y1 - y3)*(x0 - x1));
		if (d<0) return false;
		b = ((y - y0)*(x1 - x0) - (x - x0)*(y1 - y0)) / ((x1 - x0)*(y3 - y1));
		if (b<0) return false;
		a = (x - x0) / (x1 - x0) - b;
		if (a<0) return false;
		return true;
	}

	bool InRect(short x, short y, RECT rcRect)
	{
		if ((x <= rcRect.right) && (x >= rcRect.left) &&
			(y <= rcRect.bottom) && (y >= rcRect.top))	return true;
		return false;
	}

	void CalcGuyKoor()
	{
		short	Dx, Dy;	// Differenz zwischen Ziel und Start

		if (Step >= Steps)
		{
			RoutePunkt++;

			if ((RoutePunkt >= (RouteLaenge>1 ? 2 * (RouteLaenge - 1) : 1) ||
				((Guy.Aktion == Action::CANCEL) && (RouteLaenge>1))))
			{
				if (RouteLaenge>1) Bmp[BUTTSTOP].Phase = -1;
				Bmp[Guy.Zustand].Phase = 0;
				Guy.Aktiv = false;
				RouteZiel.x = -1;
				RouteZiel.y = -1;
				return;
			}
			Guy.Pos.x = Route[(RoutePunkt + 1) / 2].x;
			Guy.Pos.y = Route[(RoutePunkt + 1) / 2].y;
			World::Entdecken();

			if (BootsFahrt)
				World::AddTime(0, Scape[Route[(RoutePunkt + 1) / 2].x][Route[(RoutePunkt + 1) / 2].y].LaufZeit * 3);
			else World::AddTime(0, Scape[Route[(RoutePunkt + 1) / 2].x][Route[(RoutePunkt + 1) / 2].y].LaufZeit * 5);
			World::AddResource(NAHRUNG, -1);
			World::AddResource(WASSER, -1);

			if ((Guy.Zustand == GUYSCHIFF) || (Guy.Zustand == GUYSCHWIMMEN)) Guy.Zustand -= 2; // nichts machen
			else if (BootsFahrt) Guy.Zustand = GUYBOOTLINKS;
			else Guy.Zustand = GUYLINKS;

			if (RouteLaenge > 1) // Bei normaler Routenabarbeitung die Richung Kachelmäßig rausfinden
			{
				if (Route[RoutePunkt / 2].x > Route[RoutePunkt / 2 + 1].x) Guy.Zustand += 0;
				else if (Route[RoutePunkt / 2].x < Route[RoutePunkt / 2 + 1].x) Guy.Zustand += 2;
				else if (Route[RoutePunkt / 2].y < Route[RoutePunkt / 2 + 1].y) Guy.Zustand += 3;
				else if (Route[RoutePunkt / 2].y > Route[RoutePunkt / 2 + 1].y) Guy.Zustand += 1;
			}
			else
			{
				if ((RouteKoor[RoutePunkt].x >  RouteKoor[RoutePunkt + 1].x) &&
					(RouteKoor[RoutePunkt].y >= RouteKoor[RoutePunkt + 1].y)) Guy.Zustand += 0;
				else if ((RouteKoor[RoutePunkt].x <= RouteKoor[RoutePunkt + 1].x) &&
					(RouteKoor[RoutePunkt].y >  RouteKoor[RoutePunkt + 1].y)) Guy.Zustand += 1;
				else if ((RouteKoor[RoutePunkt].x <  RouteKoor[RoutePunkt + 1].x) &&
					(RouteKoor[RoutePunkt].y <= RouteKoor[RoutePunkt + 1].y)) Guy.Zustand += 2;
				else if ((RouteKoor[RoutePunkt].x >= RouteKoor[RoutePunkt + 1].x) &&
					(RouteKoor[RoutePunkt].y <  RouteKoor[RoutePunkt + 1].y)) Guy.Zustand += 3;
			}

			Dx = RouteKoor[RoutePunkt + 1].x - RouteKoor[RoutePunkt].x;
			Dy = RouteKoor[RoutePunkt + 1].y - RouteKoor[RoutePunkt].y;
			GuyPosScreenStart.x = RouteKoor[RoutePunkt].x;
			GuyPosScreenStart.y = RouteKoor[RoutePunkt].y;
			Step = 0;

			if (abs(Dx)>abs(Dy))
			{
				if (Dx>0)  Schrittx = 1; else Schrittx = -1;
				if (Dx == 0)  Schritty = 0; else Schritty = (float)Dy / ((float)(Dx*Schrittx));
				Steps = abs(Dx);

			}
			else
			{
				if (Dy>0)  Schritty = 1; else Schritty = -1;
				if (Dy == 0)  Schrittx = 0; else Schrittx = (float)Dx / ((float)(Dy*Schritty));
				Steps = abs(Dy);
			}

		}

		if (Bild % Scape[Guy.Pos.x][Guy.Pos.y].LaufZeit == 0)
		{
			Step++;
			short i;
			if (BootsFahrt) i = 4; else i = 2;
			if (Step%i == 0)
			{
				Bmp[Guy.Zustand].Phase++;
				if (Bmp[Guy.Zustand].Phase >= Bmp[Guy.Zustand].Anzahl) Bmp[Guy.Zustand].Phase = 0;
			}
			Guy.PosScreen.x = GuyPosScreenStart.x + ROUND(Step*Schrittx);
			Guy.PosScreen.y = GuyPosScreenStart.y + ROUND(Step*Schritty);
			if ((Spielzustand == SZINTRO) || (Spielzustand == SZGERETTET)) // Beim Intro fährt die Kamera mit
			{
				Camera.x = Guy.PosScreen.x - rcGesamt.right / 2;
				Camera.y = Guy.PosScreen.y - rcGesamt.bottom / 2;
			}
		}
	}

	void CalcKoor()
	{
		short				x, y;
		// Bildschirmkoordinaten berechnen und speichern
		for (y = 0; y<MAXYKACH; y++)
			for (x = 0; x<MAXXKACH; x++)
			{
				Scape[x][y].xScreen = KXPIXEL / 2 * MAXXKACH + 32 +
					x * KXPIXEL / 2 - y * KYPIXEL / 2 +
					(-6 * y) + x; // seltsame Korrekturen
				Scape[x][y].yScreen =
					x * KXPIXEL / 2 + y * KYPIXEL / 2 - 16 * Scape[x][y].Hoehe +
					(-13 * x) + (-8 * y); // seltsame Korrekturen
				if ((x == 0) && (y == 0))				ScapeGrenze.top = Scape[x][y].yScreen;
				if ((x == 0) && (y == MAXYKACH - 1))		ScapeGrenze.left = Scape[x][y].xScreen;
				if ((x == MAXXKACH - 1) && (y == MAXYKACH - 1))		ScapeGrenze.bottom = Scape[x][y].yScreen + KYPIXEL;
				if ((x == MAXXKACH - 1) && (y == 0))				ScapeGrenze.right = Scape[x][y].xScreen + KXPIXEL;
			}

	}

	bool LineIntersect(ZWEID LineStartPos, ZWEID Pos, bool store)
	{
		short		i;
		float	  x, y;
		short  Dx, Dy;
		float  Sx, Sy;
		bool	  erg = false;
		float	Nextx, Nexty;

		Steps = 0;

		Dx = LineStartPos.x - Pos.x;
		Dy = LineStartPos.y - Pos.y;
		x = LineStartPos.x;
		y = LineStartPos.y;
		if (abs(Dx)>abs(Dy))
		{
			if (Dx>0)  Sx = -1; else Sx = 1;
			if (Dx == 0)  Sy = 0; else Sy = (float)Dy / ((float)(Dx*Sx));
			Steps = abs(Dx);
		}
		else
		{
			if (Dy>0)  Sy = -1; else Sy = 1;
			if (Dy == 0)  Sx = 0; else Sx = (float)Dx / ((float)(Dy*Sy));
			Steps = abs(Dy);
		}

		for (i = 0; i<Steps; i++)
		{
			if (!Scape[ROUND(x)][ROUND(y)].Begehbar)  erg = true;
			if ((store))
			{
				Route[RouteLaenge].x = ROUND(x);
				Route[RouteLaenge].y = ROUND(y);
				RouteLaenge++;
			}
			Nextx = x + Sx;
			Nexty = y + Sy;
			if ((ROUND(y) != ROUND(Nexty)) && (ROUND(x) != ROUND(Nextx)))
			{
				if (Scape[ROUND(x)][ROUND(Nexty)].Begehbar)
				{
					if ((store))
					{
						Route[RouteLaenge].x = ROUND(x);
						Route[RouteLaenge].y = ROUND(Nexty);
						RouteLaenge++;
					}
				}
				else
				{
					if (Scape[ROUND(Nextx)][ROUND(y)].Begehbar)
					{
						if ((store))
						{
							Route[RouteLaenge].x = ROUND(Nextx);
							Route[RouteLaenge].y = ROUND(y);
							RouteLaenge++;
						}
					}
					else
					{
						erg = true;
					}
				}
			}
			y = Nexty;
			x = Nextx;
		}
		// MessageBeep(MB_OK);
		return erg;
	}

	void CalcRect(RECT rcBereich)
	{
		if (rcRectdes.left< rcBereich.left)
		{
			rcRectsrc.left = rcRectsrc.left + rcBereich.left - rcRectdes.left;
			rcRectdes.left = rcBereich.left;
		}
		if (rcRectdes.top< rcBereich.top)
		{
			rcRectsrc.top = rcRectsrc.top + rcBereich.top - rcRectdes.top;
			rcRectdes.top = rcBereich.top;
		}
		if (rcRectdes.right> rcBereich.right)
		{
			rcRectsrc.right = rcRectsrc.right + rcBereich.right - rcRectdes.right;
			rcRectdes.right = rcBereich.right;
		}
		if (rcRectdes.bottom> rcBereich.bottom)
		{
			rcRectsrc.bottom = rcRectsrc.bottom + rcBereich.bottom - rcRectdes.bottom;
			rcRectdes.bottom = rcBereich.bottom;
		}
	}

	void ButtAniAus()
	{
		short i;

		for (i = BUTTGITTER; i <= BUTTDESTROY; i++)
		{
			Bmp[i].Animation = false;
		}
	}

	void AbspannCalc()
	{
		short i, k;

		if (AbspannZustand == 0)
		{
			for (k = 1; k<10; k++)
			{
				if (AbspannListe[AbspannNr][k].Bild == -1) break;
				if (!AbspannListe[AbspannNr][k].Aktiv) continue;
				i = 150 / LastBild;
				Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top -= i;

				if (Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top<MAXY - 400)
				{
					if ((!AbspannListe[AbspannNr][k + 1].Aktiv) &&
						(AbspannListe[AbspannNr][k + 1].Bild != -1))
						AbspannListe[AbspannNr][k + 1].Aktiv = true;
				}
				if (Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top<0)
				{
					AbspannListe[AbspannNr][k].Aktiv = false;
					Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top = MAXY - Bmp[AbspannListe[AbspannNr][k].Bild].Hoehe / 2;
					if (!AbspannListe[AbspannNr][k + 1].Aktiv)
					{
						if (AbspannListe[AbspannNr + 1][0].Bild != -1)
						{
							AbspannNr++;
							AbspannListe[AbspannNr][1].Aktiv = true;
						}
						else
						{
							AbspannNr = GUYLINKS;
							AbspannZustand = 1;
						}
					}
				}
			}
		}
		else if (AbspannZustand == 1)
		{
			i = LastBild / Bmp[AbspannNr].Geschwindigkeit;
			if (i<1) i = 1;
			if (Bild%i == 0)
			{
				Bmp[AbspannNr].Phase++;
				if (Bmp[AbspannNr].Phase >= Bmp[AbspannNr].Anzahl)
				{
					Bmp[AbspannNr].Phase = 0;
					AbspannNr++;
					if (AbspannNr > GUYSCHLEUDER) AbspannNr = GUYLINKS;
				}
			}
		}
	}

	void Animationen()
	{
		short x, y, i, j, k, loop; // Zwischenspeicher


		for (y = 0; y<MAXYKACH; y++)
			for (x = 0; x<MAXXKACH; x++)
			{
				j = Scape[x][y].Objekt;
				if ((j == -1) || (!Bmp[j].Animation)) continue;
				i = LastBild / Bmp[j].Geschwindigkeit;
				if (i<1) i = 1;
				if (Bild%i == 0)
				{
					if ((j >= BAUM1DOWN) && (j <= BAUM4DOWN) &&  // Die Baumfällenanimation nur ein mal abspielen
						(Scape[x][y].Phase == Bmp[j].Anzahl - 1));
					else Scape[x][y].Phase++;
					if (Scape[x][y].Phase >= Bmp[j].Anzahl) Scape[x][y].Phase = 0;
				}

			}

		for (j = BUTTGITTER; j <= BUTTDESTROY; j++)
		{
			if (!Bmp[j].Animation) continue;
			i = LastBild / Bmp[j].Geschwindigkeit;
			if (i<1) i = 1;
			if (Bild%i == 0)
			{
				Bmp[j].Phase++;
				if (Bmp[j].Phase >= Bmp[j].Anzahl) Bmp[j].Phase = 0;
			}
		}

		// Spielfigur

		// laufen
		if (((Guy.Zustand >= GUYLINKS) && (Guy.Zustand <= GUYUNTEN)) ||
			((Guy.Zustand >= GUYBOOTLINKS) && (Guy.Zustand <= GUYBOOTUNTEN)) ||
			(Guy.Zustand == GUYSCHIFF) || (Guy.Zustand == GUYSCHWIMMEN))
		{
			i = LastBild / Bmp[Guy.Zustand].Geschwindigkeit;
			if (i<1) i = 1;
			if (LastBild - Bmp[Guy.Zustand].Geschwindigkeit < 0) loop = 2; else loop = 1;
			if (BootsFahrt) loop = loop * 2;
			for (k = 0; k<loop; k++) if ((Bild%i == 0) && (Guy.Aktiv)) CalcGuyKoor();
			return;
		}
		// sonstige Aktionen
		if ((Guy.Zustand >= GUYSUCHEN) && (Guy.Zustand <= GUYSCHLEUDER) &&
			(Bmp[Guy.Zustand].Phase != Bmp[Guy.Zustand].Anzahl))
		{
			i = LastBild / Bmp[Guy.Zustand].Geschwindigkeit;
			if (i<1) i = 1;
			if (Bild%i == 0)
			{
				Bmp[Guy.Zustand].Phase++;
				if (Bmp[Guy.Zustand].Phase >= Bmp[Guy.Zustand].Anzahl)
				{
					Bmp[Guy.Zustand].Phase = 0;
					if (PapierText == -1) Guy.Aktiv = false;
				}
			}
		}
	}

} // namespace Math
