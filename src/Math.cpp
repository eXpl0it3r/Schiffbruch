#include "Math.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>

#include "extern.hpp"

#include "Action.hpp"
#include "Menu.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

namespace Math
{
    float Schrittx, Schritty; // Zum Figur laufen lassen
    ZWEID GuyPosScreenStart; // Absolute StartPosition bei einem Schritt (Für CalcGuyKoor)

    void MouseInGame(short button, short push, short xDiff, short yDiff)
    {
        char text[1024], textTemp[1024]; // Text für Infoleiste

        // Info anzeigen
        ZWEID Erg = Renderer::GetTile(MousePosition.x + Camera.x, MousePosition.y + Camera.y); // Die angeklickte Kachel
        if (Scape[Erg.x][Erg.y].Entdeckt)
        {
            LoadString(g_hInst, 45 + Scape[Erg.x][Erg.y].Art, text, 1024);
            if (Scape[Erg.x][Erg.y].Objekt != -1 && Scape[Erg.x][Erg.y].Objekt != Meerwellen)
            {
                LoadString(g_hInst, MIT, textTemp, 1024);
                strcat(text, " ");
                strcat(text, textTemp);
                strcat(text, " ");

                if (Scape[Erg.x][Erg.y].Objekt >= Baum1 && Scape[Erg.x][Erg.y].Objekt <= Baum4)
                {
                    LoadString(g_hInst, BAUMTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt >= Fluss1 && Scape[Erg.x][Erg.y].Objekt <= Schleuse6)
                {
                    LoadString(g_hInst, FLUSSTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Busch)
                {
                    LoadString(g_hInst, BUSCHTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Zelt)
                {
                    LoadString(g_hInst, ZELTTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Feld)
                {
                    LoadString(g_hInst, FELDTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Boot)
                {
                    LoadString(g_hInst, BOOTTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Rohr)
                {
                    LoadString(g_hInst, ROHRTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Sos)
                {
                    LoadString(g_hInst, SOSTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Haus1)
                {
                    LoadString(g_hInst, HAUS1TEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Haus2)
                {
                    LoadString(g_hInst, HAUS2TEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Haus3)
                {
                    LoadString(g_hInst, HAUS3TEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == BaumGross)
                {
                    LoadString(g_hInst, BAUMGROSSTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Feuerstelle)
                {
                    LoadString(g_hInst, FEUERSTELLETEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Feuer)
                {
                    LoadString(g_hInst, FEUERTEXT, textTemp, 1024);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Wrack || Scape[Erg.x][Erg.y].Objekt == Wrack2)
                {
                    LoadString(g_hInst, WRACKTEXT, textTemp, 1024);
                }
                strcat(text, textTemp);

                if (Scape[Erg.x][Erg.y].Objekt >= Feld && Scape[Erg.x][Erg.y].Objekt <= Feuerstelle)
                {
                    // Baufortschrittanzeigen
                    strcat(text, " ");
                    strcat(text, "(");
                    std::sprintf(textTemp, "%d", Scape[Erg.x][Erg.y].AkNummer * 100 / Bmp[Scape[Erg.x][Erg.y].Objekt].AkAnzahl);
                    strcat(text, textTemp);
                    strcat(text, "%");
                    strcat(text, ")");
                    // benötigte Rohstoffe anzeigen
                    World::MakeResourceString(Erg.x, Erg.y, -1);
                    strcat(text, RohString);
                }
            }
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(text, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);
        }

        // rechte Maustastescrollen
        if (button == 1 && push == 0)
        {
            Camera.x += xDiff;
            Camera.y += yDiff;
            CursorTyp = CuRichtung;
        }

        // Wenn Maustaste gedrückt wird
        if (button == 0 && push == 1)
        {
            if (Erg.x != -1 && Erg.y != -1 &&
                Scape[Erg.x][Erg.y].Entdeckt && !Guy.Aktiv &&
                (Erg.x != Guy.Pos.x || Erg.y != Guy.Pos.y) &&
                Erg.x > 0 && Erg.x < MAXXKACH - 1 &&
                Erg.y > 0 && Erg.y < MAXYKACH - 1)
            {
                // Klicksound abspielen
                PlaySound(Sound::Click2, 100);
                if (Erg.x == RouteZiel.x && Erg.y == RouteZiel.y)
                {
                    Routing::MarkRoute(false);
                    Bmp[ButtonStop].Phase = 0;
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
                    if (Routing::FindTheWay())
                    {
                        Routing::MarkRoute(true);
                    }
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
            else
            {
                PlaySound(Sound::Click, 100);
            }
        }
    }

    void MouseInPanel(short button, short push)
    {
        // wenn die Maus in der Minimap ist ->
        if (PointInRectangle(MousePosition.x, MousePosition.y, rcKarte) && button == 0 && push != -1)
        {
            // Mauskoordinaten in Minimap
            short mx = MousePosition.x - static_cast<short>(rcKarte.left);
            short my = MousePosition.y - static_cast<short>(rcKarte.top);
            Camera.x = KXPIXEL / 4 * (mx - my) + MAXXKACH * KXPIXEL / 2
                - static_cast<short>((rcSpielflaeche.right - rcSpielflaeche.left) / 2);
            Camera.y = KXPIXEL / 7 * (my + mx)
                - static_cast<short>((rcSpielflaeche.bottom - rcSpielflaeche.top) / 2);
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonGitter].rcDes))
        {
            if (Gitter)
            {
                Renderer::DrawText(GITTERAUS, TXTTEXTFELD, 2);
            }
            else
            {
                Renderer::DrawText(GITTERAN, TXTTEXTFELD, 2);
            }

            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Gitter = !Gitter;
                World::Generate();
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonAnimation].rcDes))
        {
            if (LAnimation)
            {
                Renderer::DrawText(ANIMATIONAUS, TXTTEXTFELD, 2);
            }
            else
            {
                Renderer::DrawText(ANIMATIONAN, TXTTEXTFELD, 2);
            }

            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                LAnimation = !LAnimation;
                World::Generate();
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonSound].rcDes))
        {
            if (Soundzustand == 1)
            {
                Renderer::DrawText(SOUNDAUS, TXTTEXTFELD, 2);
            }
            else if (Soundzustand == 0)
            {
                Renderer::DrawText(SOUNDAN, TXTTEXTFELD, 2);
            }
            else
            {
                Renderer::DrawText(KEINSOUND, TXTTEXTFELD, 2);
            }

            if (button == 0 && push == 1)
            {
                if (Soundzustand == 1)
                {
                    for (short i = 1; i < Sound::Count; i++)
                    {
                        Sound::StopSound(i);
                    }
                    Soundzustand = 0;
                }
                else if (Soundzustand == 0)
                {
                    Soundzustand = 1;
                    PlaySound(Sound::Click2, 100);
                }
                else
                {
                    PlaySound(Sound::Click, 100);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonBeenden].rcDes))
        {
            Renderer::DrawText(BEENDEN, TXTTEXTFELD, 2);
            Bmp[ButtonBeenden].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                Guy.Aktiv = false;
                Guy.Aktion = static_cast<short>(Action::Actions::Quit);
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonNew].rcDes))
        {
            Renderer::DrawText(NEU, TXTTEXTFELD, 2);
            Bmp[ButtonNew].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                Guy.Aktiv = false;
                Guy.Aktion = static_cast<short>(Action::Actions::Restart);
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonTagNeu].rcDes))
        {
            Renderer::DrawText(TAGNEU2, TXTTEXTFELD, 2);
            Bmp[ButtonTagNeu].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                Guy.Aktiv = false;
                Guy.Aktion = static_cast<short>(Action::Actions::DayRestart);
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonAction].rcDes))
        {
            if (HauptMenue == Menu::Action)
            {
                Renderer::DrawText(MEAKTIONZU, TXTTEXTFELD, 2);
            }
            else
            {
                Renderer::DrawText(MEAKTIONAUF, TXTTEXTFELD, 2);
            }
            Bmp[ButtonAction].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (HauptMenue == Menu::Action)
                {
                    HauptMenue = Menu::None;
                }
                else
                {
                    HauptMenue = Menu::Action;
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonBauen].rcDes) && Bmp[ButtonBauen].Phase != -1)
        {
            if (HauptMenue == Menu::Build)
            {
                Renderer::DrawText(MEBAUENZU, TXTTEXTFELD, 2);
            }
            else
            {
                Renderer::DrawText(MEBAUENAUF, TXTTEXTFELD, 2);
            }
            Bmp[ButtonBauen].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (HauptMenue == Menu::Build)
                {
                    HauptMenue = Menu::None;
                }
                else
                {
                    HauptMenue = Menu::Build;
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonInventory].rcDes))
        {
            if (HauptMenue == Menu::Inventory)
            {
                Renderer::DrawText(MEINVENTARZU, TXTTEXTFELD, 2);
            }
            else
            {
                Renderer::DrawText(MEINVENTARAUF, TXTTEXTFELD, 2);
            }

            Bmp[ButtonInventory].Animation = true;

            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (HauptMenue == Menu::Inventory)
                {
                    HauptMenue = Menu::None;
                }
                else
                {
                    HauptMenue = Menu::Inventory;
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonWeiter].rcDes) && Bmp[ButtonWeiter].Phase != -1)
        {
            Renderer::DrawText(WEITER, TXTTEXTFELD, 2);

            Bmp[ButtonWeiter].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Bmp[ButtonStop].Phase = 0;
                Routing::MarkRoute(false);
                RouteZiel.x = -1;
                RouteZiel.y = -1;
                Guy.PosAlt = Guy.PosScreen;
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
                {
                case Zelt:
                    Guy.Aktion = static_cast<short>(Action::Actions::Tent);
                    break;
                case Feld:
                    Guy.Aktion = static_cast<short>(Action::Actions::Field);
                    break;
                case Boot:
                    Guy.Aktion = static_cast<short>(Action::Actions::Boat);
                    break;
                case Rohr:
                    Guy.Aktion = static_cast<short>(Action::Actions::Pipe);
                    break;
                case Sos:
                    Guy.Aktion = static_cast<short>(Action::Actions::SosSign);
                    break;
                case Haus1:
                    Guy.Aktion = static_cast<short>(Action::Actions::House1);
                    break;
                case Haus2:
                    Guy.Aktion = static_cast<short>(Action::Actions::House2);
                    break;
                case Haus3:
                    Guy.Aktion = static_cast<short>(Action::Actions::House3);
                    break;
                case Feuerstelle:
                    Guy.Aktion = static_cast<short>(Action::Actions::Fireplace);
                    break;
                default:
                    break;
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonStop].rcDes) && Bmp[ButtonStop].Phase != -1)
        {
            Renderer::DrawText(STOP, TXTTEXTFELD, 2);

            Bmp[ButtonStop].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                Guy.Aktion = static_cast<short>(Action::Actions::Cancel);
                Bmp[ButtonStop].Phase = -1;
            }
        }

        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonAblegen].rcDes) && Bmp[ButtonAblegen].Phase != -1)
        {
            Renderer::DrawText(BEGINNABLEGEN, TXTTEXTFELD, 2);
            Bmp[ButtonAblegen].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1)
                {
                    Guy.Aktion = static_cast<short>(Action::Actions::Undock);
                }
                else
                {
                    Guy.Aktion = static_cast<short>(Action::Actions::Dock);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonSearch].rcDes) && HauptMenue == Menu::Action && Bmp[ButtonSearch].Phase != -1)
        {
            Renderer::DrawText(BEGINNSUCHEN, TXTTEXTFELD, 2);
            Bmp[ButtonSearch].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                Guy.Aktion = static_cast<short>(Action::Actions::Search);
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonFood].rcDes) && HauptMenue == Menu::Action && Bmp[ButtonFood].Phase != -1)
        {
            Renderer::DrawText(BEGINNESSEN, TXTTEXTFELD, 2);
            Bmp[ButtonFood].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Busch || Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Feld) &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Phase == Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl - 1)
                {
                    Guy.Aktion = static_cast<short>(Action::Actions::Eat);
                }
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Fluss1 &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Schleuse6 ||
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Rohr &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Phase == 1)
                {
                    Guy.Aktion = static_cast<short>(Action::Actions::Drink);
                }
                else
                {
                    PapierText = Renderer::DrawText(KEINESSENTRINKEN, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonSleep].rcDes) &&
            HauptMenue == Menu::Action && Bmp[ButtonSleep].Phase != -1)
        {
            Renderer::DrawText(BEGINNSCHLAFEN, TXTTEXTFELD, 2);
            Bmp[ButtonSleep].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1)
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Sleep);
                }
                else
                {
                    PapierText = Renderer::DrawText(NICHTAUFWASSERSCHLAFEN, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonTimber].rcDes) &&
            HauptMenue == Menu::Action && Bmp[ButtonTimber].Phase != -1)
        {
            Renderer::DrawText(BEGINNFAELLEN, TXTTEXTFELD, 2);
            Bmp[ButtonTimber].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                if (Guy.Inventar[ResourceTrunk] <= 10)
                {
                    if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Baum1 &&
                        Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Baum4)
                    {
                        Guy.Aktion = static_cast<short>(Action::Actions::Log);
                    }
                    else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BaumGross ||
                        Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Haus1 &&
                        Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Haus3)
                    {
                        PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
                    }
                    else
                    {
                        PapierText = Renderer::DrawText(KEINBAUM, TXTPAPIER, 1);
                    }
                }
                else
                {
                    PapierText = Renderer::DrawText(ROHSTAMMZUVIEL, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonFish].rcDes) &&
            HauptMenue == Menu::Action && Bmp[ButtonFish].Phase != -1)
        {
            Renderer::DrawText(BEGINNANGELN, TXTTEXTFELD, 2);
            Bmp[ButtonFish].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Fluss1 &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Schleuse6 ||
                    BootsFahrt)
                {
                    Guy.Aktion = static_cast<short>(Action::Actions::Fish);
                }
                else
                {
                    PapierText = Renderer::DrawText(KEINWASSER, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonLight].rcDes) &&
            HauptMenue == Menu::Action && Bmp[ButtonLight].Phase != -1)
        {
            Renderer::DrawText(BEGINNANZUENDEN, TXTTEXTFELD, 2);
            Bmp[ButtonLight].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Feuerstelle &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
                {
                    Guy.Aktion = static_cast<short>(Action::Actions::Light);
                }
                else
                {
                    PapierText = Renderer::DrawText(KEINEFEUERST, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonLook].rcDes) &&
            HauptMenue == Menu::Action && Bmp[ButtonLook].Phase != -1)
        {
            Renderer::DrawText(BEGINNAUSSCHAU, TXTTEXTFELD, 2);
            Bmp[ButtonLook].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1)
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Lookout);
                }
                else
                {
                    PapierText = Renderer::DrawText(WELLENZUHOCH, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonTreasure].rcDes) &&
            HauptMenue == Menu::Action && Bmp[ButtonTreasure].Phase != -1)
        {
            Renderer::DrawText(BEGINNSCHATZ, TXTTEXTFELD, 2);
            Bmp[ButtonTreasure].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1 &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0 &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1)
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Treasure);
                }
                else
                {
                    PapierText = Renderer::DrawText(GRABENBEDINGUNGEN, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonSlingshot].rcDes) &&
            HauptMenue == Menu::Action && Bmp[ButtonSlingshot].Phase != -1)
        {
            Renderer::DrawText(BEGINNSCHLEUDER, TXTTEXTFELD, 2);
            Bmp[ButtonSlingshot].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Guy.AkNummer = 0;
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Baum1 &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Baum4)
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Slingshot);
                }
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BaumGross ||
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Haus1 &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Haus3)
                {
                    PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
                }
                else
                {
                    PapierText = Renderer::DrawText(KEINVOGEL, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonTreasureMap].rcDes) &&
            HauptMenue == Menu::Action && Bmp[ButtonTreasureMap].Phase != -1)
        {
            Renderer::DrawText(BEGINNSCHATZKARTE, TXTTEXTFELD, 2);
            Bmp[ButtonTreasureMap].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                Renderer::DrawTreasureMap();
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonField].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonField].Phase != -1)
        {
            LoadString(g_hInst, BEGINNFELD, StdString, 1024);
            World::MakeResourceString(-1, -1, Feld);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[ButtonField].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1 &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0 &&
                    Scape[Guy.Pos.x][Guy.Pos.y].Art == 4)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[ButtonStop].Phase = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Field);
                }
                else if (Bmp[ButtonWeiter].Phase != -1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Feld)
                {
                    Bmp[ButtonStop].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = static_cast<short>(Action::Actions::Field);
                }
                else
                {
                    PapierText = Renderer::DrawText(FELDBEDINGUNGEN, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonTent].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonTent].Phase != -1)
        {
            LoadString(g_hInst, BEGINNZELT, StdString, 1024);
            World::MakeResourceString(-1, -1, Zelt);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[ButtonTent].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1
                    && Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0
                    && Scape[Guy.Pos.x][Guy.Pos.y].Art != -1)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[ButtonStop].Phase = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Tent);
                }
                else if (Bmp[ButtonWeiter].Phase != -1
                    && Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt)
                {
                    Bmp[ButtonStop].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = static_cast<short>(Action::Actions::Tent);
                }
                else
                {
                    PapierText = Renderer::DrawText(ZELTBEDINGUNGEN, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonBoat].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonBoat].Phase != -1)
        {
            LoadString(g_hInst, BEGINNBOOT, StdString, 1024);
            World::MakeResourceString(-1, -1, Boot);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[ButtonBoat].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1 && Scape[Guy.Pos.x][Guy.Pos.y].Art == 2
                    && (Scape[Guy.Pos.x - 1][Guy.Pos.y].Art == 1 || Scape[Guy.Pos.x][Guy.Pos.y - 1].Art == 1
                        || Scape[Guy.Pos.x + 1][Guy.Pos.y].Art == 1 || Scape[Guy.Pos.x][Guy.Pos.y + 1].Art == 1))
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[ButtonStop].Phase = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Boat);
                }
                else if (Bmp[ButtonWeiter].Phase != -1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Boot)
                {
                    Bmp[ButtonStop].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = static_cast<short>(Action::Actions::Boat);
                }
                else
                {
                    PapierText = Renderer::DrawText(BOOTBEDINGUNGEN, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonPipe].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonPipe].Phase != -1)
        {
            LoadString(g_hInst, BEGINNROHR, StdString, 1024);
            World::MakeResourceString(-1, -1, Rohr);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[ButtonPipe].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1 && Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[ButtonStop].Phase = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Pipe);
                }
                else if (Bmp[ButtonWeiter].Phase != -1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Rohr)
                {
                    Bmp[ButtonStop].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = static_cast<short>(Action::Actions::Pipe);
                }
                else
                {
                    PapierText = Renderer::DrawText(ROHRBEDINGUNGEN, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonSos].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonSos].Phase != -1)
        {
            LoadString(g_hInst, BEGINNSOS, StdString, 1024);
            World::MakeResourceString(-1, -1, Sos);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[ButtonSos].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1 && Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[ButtonStop].Phase = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::SosSign);
                }
                else if (Bmp[ButtonWeiter].Phase != -1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Sos)
                {
                    Bmp[ButtonStop].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = static_cast<short>(Action::Actions::SosSign);
                }
                else
                {
                    PapierText = Renderer::DrawText(SOSBEDINGUNGEN, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonHouse1].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonHouse1].Phase != -1)
        {
            LoadString(g_hInst, BEGINNHAUS1, StdString, 1024);
            World::MakeResourceString(-1, -1, Haus1);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[ButtonHouse1].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Baum1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Baum4)
                {
                    PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
                }
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BaumGross)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[ButtonStop].Phase = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::House1);
                }
                else if (Bmp[ButtonWeiter].Phase != -1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus1)
                {
                    Bmp[ButtonStop].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = static_cast<short>(Action::Actions::House1);
                }
                else
                {
                    PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonHouse2].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonHouse2].Phase != -1)
        {
            LoadString(g_hInst, BEGINNHAUS2, StdString, 1024);
            World::MakeResourceString(-1, -1, Haus2);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[ButtonHouse2].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Baum1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Baum4)
                {
                    PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
                }
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BaumGross)
                {
                    PapierText = Renderer::DrawText(NICHTOHNELEITER, TXTPAPIER, 1);
                }
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus1)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[ButtonStop].Phase = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::House2);
                }
                else if (Bmp[ButtonWeiter].Phase != -1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus2)
                {
                    Bmp[ButtonStop].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = static_cast<short>(Action::Actions::House2);
                }
                else
                {
                    PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonHouse3].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonHouse3].Phase != -1)
        {
            LoadString(g_hInst, BEGINNHAUS3, StdString, 1024);
            World::MakeResourceString(-1, -1, Haus3);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[ButtonHouse3].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Baum1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Baum4)
                {
                    PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
                }
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BaumGross || Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus1)
                {
                    PapierText = Renderer::DrawText(NICHTOHNEPLATTFORM, TXTPAPIER, 1);
                }
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus2)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[ButtonStop].Phase = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::House3);
                }
                else if (Bmp[ButtonWeiter].Phase != -1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3)
                {
                    Bmp[ButtonStop].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = static_cast<short>(Action::Actions::House3);
                }
                else
                {
                    PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonFireplace].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonFireplace].Phase != -1)
        {
            LoadString(g_hInst, BEGINNFEUERSTELLE, StdString, 1024);
            World::MakeResourceString(-1, -1, Feuerstelle);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[ButtonFireplace].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1 && Scape[Guy.Pos.x][Guy.Pos.y].Typ == 0)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[ButtonStop].Phase = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Fireplace);
                }
                else if (Bmp[ButtonWeiter].Phase != -1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Feuerstelle)
                {
                    Bmp[ButtonStop].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = static_cast<short>(Action::Actions::Fireplace);
                }
                else
                {
                    PapierText = Renderer::DrawText(FEUERSTELLENBEDINGUNGEN, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[ButtonDestroy].rcDes) &&
            HauptMenue == Menu::Build && Bmp[ButtonDestroy].Phase != -1)
        {
            Renderer::DrawText(BEGINNDESTROY, TXTTEXTFELD, 2);
            Bmp[ButtonDestroy].Animation = true;
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Feld
                    && Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Feuerstelle)
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Destroy);
                }
                else
                {
                    PapierText = Renderer::DrawText(KEINBAUWERK, TXTPAPIER, 1);
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[InventoryPaper].rcDes) && HauptMenue == Menu::Inventory)
        {
            for (short i = ResourceBranch; i <= ResourceSlingshot; i++)
            {
                if (PointInRectangle(MousePosition.x, MousePosition.y, Bmp[i].rcDes) && Guy.Inventar[i] > 0)
                {
                    if (button == 0 && push == 1)
                    {
                        if (TwoClicks == -1)
                        {
                            CursorTyp = i;
                            TwoClicks = i;
                        }
                        else
                        {
                            World::CheckUsage(i);
                        }
                    }
                    switch (i)
                    {
                    case ResourceBranch:
                        Renderer::DrawText(AST, TXTTEXTFELD, 2);
                        break;
                    case ResourceStone:
                        Renderer::DrawText(STEIN, TXTTEXTFELD, 2);
                        break;
                    case ResourceAxe:
                        Renderer::DrawText(AXT, TXTTEXTFELD, 2);
                        break;
                    case ResourceLeaf:
                        Renderer::DrawText(BLATT, TXTTEXTFELD, 2);
                        break;
                    case ResourceTrunk:
                        Renderer::DrawText(STAMM, TXTTEXTFELD, 2);
                        break;
                    case ResourceHarrow:
                        Renderer::DrawText(EGGE, TXTTEXTFELD, 2);
                        break;
                    case ResourceVine:
                        Renderer::DrawText(LIANE, TXTTEXTFELD, 2);
                        break;
                    case ResourceFishingRod:
                        Renderer::DrawText(ANGEL, TXTTEXTFELD, 2);
                        break;
                    case ResourceHammer:
                        Renderer::DrawText(HAMMERTEXT, TXTTEXTFELD, 2);
                        break;
                    case ResourceTelescope:
                        Renderer::DrawText(FERNROHR, TXTTEXTFELD, 2);
                        break;
                    case ResourceMatchstick:
                        Renderer::DrawText(STREICHHOLZ, TXTTEXTFELD, 2);
                        break;
                    case ResourceShovel:
                        Renderer::DrawText(SCHAUFEL, TXTTEXTFELD, 2);
                        break;
                    case ResourceMap:
                        Renderer::DrawText(KARTE, TXTTEXTFELD, 2);
                        break;
                    case ResourceSlingshot:
                        Renderer::DrawText(SCHLEUDER, TXTTEXTFELD, 2);
                        break;
                    default:
                        break;
                    }

                    break;
                }
            }
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, TextBereich[TXTTAGESZEIT].rcText))
        {
            Renderer::DrawText(SOSPAET, TXTTEXTFELD, 2);
        }
        else if (PointInRectangle(MousePosition.x, MousePosition.y, TextBereich[TXTCHANCE].rcText))
        {
            Renderer::DrawText(CHANCETEXT, TXTTEXTFELD, 2);
        }
        else // TwoClicks löschen
        {
            if (button == 0 && push == 1)
            {
                PlaySound(Sound::Click, 100);
            }
            TwoClicks = -1;
        }
    }

    bool PointInTriangle(short X, short Y, short X0, short Y0, short X1, short Y1, short X3, short Y3)
    {
        auto x = static_cast<float>(X);
        auto y = static_cast<float>(Y);
        auto x0 = static_cast<float>(X0);
        auto y0 = static_cast<float>(Y0);
        auto x1 = static_cast<float>(X1);
        auto y1 = static_cast<float>(Y1);
        auto x3 = static_cast<float>(X3);
        auto y3 = static_cast<float>(Y3);

        float c = (x - x1) / (x0 - x1);
        if (c < 0)
        {
            return false;
        }
        float d = ((y - y3) * (x0 - x1) - (x - x1) * (y0 - y3)) / ((y1 - y3) * (x0 - x1));
        if (d < 0)
        {
            return false;
        }
        float b = ((y - y0) * (x1 - x0) - (x - x0) * (y1 - y0)) / ((x1 - x0) * (y3 - y1));
        if (b < 0)
        {
            return false;
        }
        float a = (x - x0) / (x1 - x0) - b;
        if (a < 0)
        {
            return false;
        }

        return true;
    }

    bool PointInRectangle(short x, short y, RECT rectangle)
    {
        return x <= rectangle.right && x >= rectangle.left &&
            y <= rectangle.bottom && y >= rectangle.top;
    }

    void CalculateGuyCoordinates()
    {
        if (Step >= Steps)
        {
            RoutePunkt++;

            if (RoutePunkt >= (RouteLaenge > 1 ? 2 * (RouteLaenge - 1) : 1)
                || Guy.Aktion == static_cast<int>(Action::Actions::Cancel) && RouteLaenge > 1)
            {
                if (RouteLaenge > 1)
                {
                    Bmp[ButtonStop].Phase = -1;
                }
                Bmp[Guy.Zustand].Phase = 0;
                Guy.Aktiv = false;
                RouteZiel.x = -1;
                RouteZiel.y = -1;
                return;
            }
            Guy.Pos.x = Route[(RoutePunkt + 1) / 2].x;
            Guy.Pos.y = Route[(RoutePunkt + 1) / 2].y;
            World::Discover();

            if (BootsFahrt)
            {
                World::AddTime(0, Scape[Route[(RoutePunkt + 1) / 2].x][Route[(RoutePunkt + 1) / 2].y].LaufZeit * 3);
            }
            else
            {
                World::AddTime(0, Scape[Route[(RoutePunkt + 1) / 2].x][Route[(RoutePunkt + 1) / 2].y].LaufZeit * 5);
            }
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);

            if (Guy.Zustand == GuySchiff || Guy.Zustand == GuySchwimmen)
            {
                Guy.Zustand -= 2; // nichts machen
            }
            else if (BootsFahrt)
            {
                Guy.Zustand = GuyBootLinks;
            }
            else
            {
                Guy.Zustand = GuyLinks;
            }

            if (RouteLaenge > 1) // Bei normaler Routenabarbeitung die Richung Kachelmäßig rausfinden
            {
                if (Route[RoutePunkt / 2].x > Route[RoutePunkt / 2 + 1].x)
                {
                    Guy.Zustand += 0;
                }
                else if (Route[RoutePunkt / 2].x < Route[RoutePunkt / 2 + 1].x)
                {
                    Guy.Zustand += 2;
                }
                else if (Route[RoutePunkt / 2].y < Route[RoutePunkt / 2 + 1].y)
                {
                    Guy.Zustand += 3;
                }
                else
                {
                    if (Route[RoutePunkt / 2].y > Route[RoutePunkt / 2 + 1].y)
                    {
                        Guy.Zustand += 1;
                    }
                }
            }
            else
            {
                if (RouteKoor[RoutePunkt].x > RouteKoor[RoutePunkt + 1].x
                    && RouteKoor[RoutePunkt].y >= RouteKoor[RoutePunkt + 1].y)
                {
                    Guy.Zustand += 0;
                }
                else if (RouteKoor[RoutePunkt].x <= RouteKoor[RoutePunkt + 1].x
                    && RouteKoor[RoutePunkt].y > RouteKoor[RoutePunkt + 1].y)
                {
                    Guy.Zustand += 1;
                }
                else if (RouteKoor[RoutePunkt].x < RouteKoor[RoutePunkt + 1].x
                    && RouteKoor[RoutePunkt].y <= RouteKoor[RoutePunkt + 1].y)
                {
                    Guy.Zustand += 2;
                }
                else if (RouteKoor[RoutePunkt].x >= RouteKoor[RoutePunkt + 1].x
                    && RouteKoor[RoutePunkt].y < RouteKoor[RoutePunkt + 1].y)
                {
                    Guy.Zustand += 3;
                }
            }

            // Differenz zwischen Ziel und Start
            const short dX = RouteKoor[RoutePunkt + 1].x - RouteKoor[RoutePunkt].x;
            const short dY = RouteKoor[RoutePunkt + 1].y - RouteKoor[RoutePunkt].y;

            GuyPosScreenStart.x = RouteKoor[RoutePunkt].x;
            GuyPosScreenStart.y = RouteKoor[RoutePunkt].y;
            Step = 0;

            if (std::abs(dX) > std::abs(dY))
            {
                if (dX > 0)
                {
                    Schrittx = 1;
                }
                else
                {
                    Schrittx = -1;
                }
                if (dX == 0)
                {
                    Schritty = 0;
                }
                else
                {
                    Schritty = static_cast<float>(dY) / static_cast<float>(dX * Schrittx);
                }
                Steps = std::abs(dX);
            }
            else
            {
                if (dY > 0)
                {
                    Schritty = 1;
                }
                else
                {
                    Schritty = -1;
                }
                if (dY == 0)
                {
                    Schrittx = 0;
                }
                else
                {
                    Schrittx = static_cast<float>(dX) / static_cast<float>(dY * Schritty);
                }
                Steps = std::abs(dY);
            }
        }

        if (Bild % Scape[Guy.Pos.x][Guy.Pos.y].LaufZeit == 0)
        {
            Step++;
            short i;
            if (BootsFahrt)
            {
                i = 4;
            }
            else
            {
                i = 2;
            }
            if (Step % i == 0)
            {
                Bmp[Guy.Zustand].Phase++;
                if (Bmp[Guy.Zustand].Phase >= Bmp[Guy.Zustand].Anzahl)
                {
                    Bmp[Guy.Zustand].Phase = 0;
                }
            }
            Guy.PosScreen.x = GuyPosScreenStart.x + ROUND(Step*Schrittx);
            Guy.PosScreen.y = GuyPosScreenStart.y + ROUND(Step*Schritty);
            if (Spielzustand == State::Intro || Spielzustand == State::Rescued) // Beim Intro fährt die Kamera mit
            {
                Camera.x = Guy.PosScreen.x - static_cast<short>(rcGesamt.right / 2);
                Camera.y = Guy.PosScreen.y - static_cast<short>(rcGesamt.bottom / 2);
            }
        }
    }

    void CalculateTileCoordinates()
    {
        // Bildschirmkoordinaten berechnen und speichern
        for (short y = 0; y < MAXYKACH; y++)
        {
            for (short x = 0; x < MAXXKACH; x++)
            {
                Scape[x][y].xScreen = KXPIXEL / 2 * MAXXKACH + 32 +
                    x * KXPIXEL / 2 - y * KYPIXEL / 2 +
                    -6 * y + x; // seltsame Korrekturen
                Scape[x][y].yScreen =
                    x * KXPIXEL / 2 + y * KYPIXEL / 2 - 16 * Scape[x][y].Hoehe +
                    -13 * x + -8 * y; // seltsame Korrekturen

                if (x == 0 && y == 0)
                {
                    ScapeGrenze.top = Scape[x][y].yScreen;
                }
                if (x == 0 && y == MAXYKACH - 1)
                {
                    ScapeGrenze.left = Scape[x][y].xScreen;
                }
                if (x == MAXXKACH - 1 && y == MAXYKACH - 1)
                {
                    ScapeGrenze.bottom = Scape[x][y].yScreen + KYPIXEL;
                }
                if (x == MAXXKACH - 1 && y == 0)
                {
                    ScapeGrenze.right = Scape[x][y].xScreen + KXPIXEL;
                }
            }
        }
    }

    bool LineIntersect(ZWEID startPosition, ZWEID position, bool store)
    {
        float sX, sY;
        auto erg = false;

        Steps = 0;

        short dX = startPosition.x - position.x;
        short dY = startPosition.y - position.y;
        float x = startPosition.x;
        float y = startPosition.y;

        if (std::abs(dX) > std::abs(dY))
        {
            if (dX > 0)
            {
                sX = -1;
            }
            else
            {
                sX = 1;
            }
            if (dX == 0)
            {
                sY = 0;
            }
            else
            {
                sY = static_cast<float>(dY) / static_cast<float>(dX * sX);
            }
            Steps = std::abs(dX);
        }
        else
        {
            if (dY > 0)
            {
                sY = -1;
            }
            else
            {
                sY = 1;
            }
            if (dY == 0)
            {
                sX = 0;
            }
            else
            {
                sX = static_cast<float>(dX) / static_cast<float>(dY * sY);
            }
            Steps = std::abs(dY);
        }

        for (short i = 0; i < Steps; i++)
        {
            if (!Scape[ROUND(x)][ROUND(y)].Begehbar)
            {
                erg = true;
            }
            if (store)
            {
                Route[RouteLaenge].x = ROUND(x);
                Route[RouteLaenge].y = ROUND(y);
                RouteLaenge++;
            }
            auto nextX = x + sX;
            auto nextY = y + sY;
            if (ROUND(y) != ROUND(nextY) && ROUND(x) != ROUND(nextX))
            {
                if (Scape[ROUND(x)][ROUND(nextY)].Begehbar)
                {
                    if (store)
                    {
                        Route[RouteLaenge].x = ROUND(x);
                        Route[RouteLaenge].y = ROUND(nextY);
                        RouteLaenge++;
                    }
                }
                else
                {
                    if (Scape[ROUND(nextX)][ROUND(y)].Begehbar)
                    {
                        if (store)
                        {
                            Route[RouteLaenge].x = ROUND(nextX);
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
            y = nextY;
            x = nextX;
        }
        // MessageBeep(MB_OK);
        return erg;
    }

    void CalculateRectangle(RECT area)
    {
        if (rcRectdes.left < area.left)
        {
            rcRectsrc.left = rcRectsrc.left + area.left - rcRectdes.left;
            rcRectdes.left = area.left;
        }
        if (rcRectdes.top < area.top)
        {
            rcRectsrc.top = rcRectsrc.top + area.top - rcRectdes.top;
            rcRectdes.top = area.top;
        }
        if (rcRectdes.right > area.right)
        {
            rcRectsrc.right = rcRectsrc.right + area.right - rcRectdes.right;
            rcRectdes.right = area.right;
        }
        if (rcRectdes.bottom > area.bottom)
        {
            rcRectsrc.bottom = rcRectsrc.bottom + area.bottom - rcRectdes.bottom;
            rcRectdes.bottom = area.bottom;
        }
    }

    void DisableButtonAnimations()
    {
        for (short i = ButtonGitter; i <= ButtonDestroy; i++)
        {
            Bmp[i].Animation = false;
        }
    }

    void CalculateCreditCoordinates()
    {
        short i;

        if (AbspannZustand == 0)
        {
            for (short k = 1; k < 10; k++)
            {
                if (AbspannListe[AbspannNr][k].Bild == -1)
                {
                    break;
                }
                if (!AbspannListe[AbspannNr][k].Aktiv)
                {
                    continue;
                }
                i = 150 / LastBild;
                Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top -= i;

                if (Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top < MAXY - 400)
                {
                    if (!AbspannListe[AbspannNr][k + 1].Aktiv && AbspannListe[AbspannNr][k + 1].Bild != -1)
                    {
                        AbspannListe[AbspannNr][k + 1].Aktiv = true;
                    }
                }
                if (Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top < 0)
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
                            AbspannNr = GuyLinks;
                            AbspannZustand = 1;
                        }
                    }
                }
            }
        }
        else if (AbspannZustand == 1)
        {
            i = LastBild / Bmp[AbspannNr].Geschwindigkeit;
            if (i < 1)
            {
                i = 1;
            }
            if (Bild % i == 0)
            {
                Bmp[AbspannNr].Phase++;
                if (Bmp[AbspannNr].Phase >= Bmp[AbspannNr].Anzahl)
                {
                    Bmp[AbspannNr].Phase = 0;
                    AbspannNr++;
                    if (AbspannNr > GuySchleuder)
                    {
                        AbspannNr = GuyLinks;
                    }
                }
            }
        }
    }

    void Animations()
    {
        short i, j, loop; // Zwischenspeicher

        for (short y = 0; y < MAXYKACH; y++)
        {
            for (short x = 0; x < MAXXKACH; x++)
            {
                j = Scape[x][y].Objekt;
                if (j == -1 || !Bmp[j].Animation)
                {
                    continue;
                }
                i = LastBild / Bmp[j].Geschwindigkeit;
                if (i < 1)
                {
                    i = 1;
                }
                if (Bild % i == 0)
                {
                    // Die Baumfällenanimation nur ein mal abspielen
                    if (j < Baum1Down || j > Baum4Down || Scape[x][y].Phase != Bmp[j].Anzahl - 1)
                    {
                        Scape[x][y].Phase++;
                    }
                    if (Scape[x][y].Phase >= Bmp[j].Anzahl)
                    {
                        Scape[x][y].Phase = 0;
                    }
                }
            }
        }

        for (j = ButtonGitter; j <= ButtonDestroy; j++)
        {
            if (!Bmp[j].Animation)
            {
                continue;
            }
            i = LastBild / Bmp[j].Geschwindigkeit;
            if (i < 1)
            {
                i = 1;
            }
            if (Bild % i == 0)
            {
                Bmp[j].Phase++;
                if (Bmp[j].Phase >= Bmp[j].Anzahl)
                {
                    Bmp[j].Phase = 0;
                }
            }
        }

        // Spielfigur

        // laufen
        if (Guy.Zustand >= GuyLinks && Guy.Zustand <= GuyUnten
            || Guy.Zustand >= GuyBootLinks && Guy.Zustand <= GuyBootUnten
            || Guy.Zustand == GuySchiff || Guy.Zustand == GuySchwimmen)
        {
            i = LastBild / Bmp[Guy.Zustand].Geschwindigkeit;
            if (i < 1)
            {
                i = 1;
            }
            if (LastBild - Bmp[Guy.Zustand].Geschwindigkeit < 0)
            {
                loop = 2;
            }
            else
            {
                loop = 1;
            }
            if (BootsFahrt)
            {
                loop *= 2;
            }
            for (short k = 0; k < loop; k++)
            {
                if (Bild % i == 0 && Guy.Aktiv)
                {
                    CalculateGuyCoordinates();
                }
            }
            return;
        }
        // sonstige Aktionen
        if (Guy.Zustand >= GuySuchen && Guy.Zustand <= GuySchleuder
            && Bmp[Guy.Zustand].Phase != Bmp[Guy.Zustand].Anzahl)
        {
            i = LastBild / Bmp[Guy.Zustand].Geschwindigkeit;
            if (i < 1)
            {
                i = 1;
            }
            if (Bild % i == 0)
            {
                Bmp[Guy.Zustand].Phase++;
                if (Bmp[Guy.Zustand].Phase >= Bmp[Guy.Zustand].Anzahl)
                {
                    Bmp[Guy.Zustand].Phase = 0;
                    if (PapierText == -1)
                    {
                        Guy.Aktiv = false;
                    }
                }
            }
        }
    }
} // namespace Math
