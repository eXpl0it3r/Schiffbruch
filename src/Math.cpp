#include "Math.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Game.hpp"
#include "Menu.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

#include <cstdio>
#include <cstring>

namespace Math
{
    float Schrittx, Schritty; // Zum Figur laufen lassen
    ZWEID GuyPosScreenStart; // Absolute StartPosition bei einem Schritt (Für CalcGuyKoor)

    void MouseInSpielflaeche(short Button, short Push, short xDiff, short yDiff)
    {
        char Text[1024], TextTmp[1024]; // Text für Infoleiste

        // Info anzeigen
        ZWEID Erg = Renderer::GetKachel((MousePosition.x + Camera.x), (MousePosition.y + Camera.y)); // Die angeklickte Kachel
        if (Scape[Erg.x][Erg.y].Entdeckt)
        {
            LoadString(g_hInst, 45 + Scape[Erg.x][Erg.y].Art, Text, 1024);
            if ((Scape[Erg.x][Erg.y].Objekt != -1) && (Scape[Erg.x][Erg.y].Objekt != SEA_WAVES))
            {
                LoadString(g_hInst, MIT, TextTmp, 1024);
                strcat(Text, " ");
                strcat(Text, TextTmp);
                strcat(Text, " ");

                if ((Scape[Erg.x][Erg.y].Objekt >= TREE_1) && (Scape[Erg.x][Erg.y].Objekt <= TREE_4))
                LoadString(g_hInst, BAUMTEXT, TextTmp, 1024);
                else if ((Scape[Erg.x][Erg.y].Objekt >= RIVER_1) && (Scape[Erg.x][Erg.y].Objekt <= FLOODGATE_6))
                LoadString(g_hInst, FLUSSTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == BUSH)
                LoadString(g_hInst, BUSCHTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == TENT)
                LoadString(g_hInst, ZELTTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == FIELD)
                LoadString(g_hInst, FELDTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == BOAT)
                LoadString(g_hInst, BOOTTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == PIPE)
                LoadString(g_hInst, ROHRTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == SOS)
                LoadString(g_hInst, SOSTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == HOUSE_1)
                LoadString(g_hInst, HAUS1TEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == HOUSE_2)
                LoadString(g_hInst, HAUS2TEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == HOUSE_3)
                LoadString(g_hInst, HAUS3TEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == TREE_BIG)
                LoadString(g_hInst, BAUMGROSSTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == BONFIRE)
                LoadString(g_hInst, FEUERSTELLETEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Objekt == FIRE)
                LoadString(g_hInst, FEUERTEXT, TextTmp, 1024);
                else if ((Scape[Erg.x][Erg.y].Objekt == WRECK_1) || (Scape[Erg.x][Erg.y].Objekt == WRECK_2))
                LoadString(g_hInst, WRACKTEXT, TextTmp, 1024);
                strcat(Text, TextTmp);

                if ((Scape[Erg.x][Erg.y].Objekt >= FIELD) &&
                    (Scape[Erg.x][Erg.y].Objekt <= BONFIRE))
                {
                    // Baufortschrittanzeigen
                    strcat(Text, " ");
                    strcat(Text, "(");
                    std::sprintf(TextTmp, "%d", (Scape[Erg.x][Erg.y].AkNummer * 100) / Bmp[Scape[Erg.x][Erg.y].Objekt].AkAnzahl);
                    strcat(Text, TextTmp);
                    strcat(Text, "%");
                    strcat(Text, ")");
                    // benötigte Rohstoffe anzeigen
                    World::MakeRohString(Erg.x, Erg.y, -1);
                    strcat(Text, RohString);
                }
            }
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(Text, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);
        }

        // rechte Maustastescrollen
        if ((Button == 1) && (Push == 0))
        {
            Camera.x += xDiff;
            Camera.y += yDiff;
            CursorTyp = CURSOR_DIRECTION;
        }

        // Wenn Maustaste gedrückt wird
        if ((Button == 0) && (Push == 1))
        {
            if ((Erg.x != -1) && (Erg.y != -1) &&
                (Scape[Erg.x][Erg.y].Entdeckt) && (!Guy.Aktiv) &&
                ((Erg.x != Guy.Pos.x) || (Erg.y != Guy.Pos.y)) &&
                (Erg.x > 0) && (Erg.x < MAX_TILES_X - 1) &&
                (Erg.y > 0) && (Erg.y < MAX_TILESY - 1))
            {
                // Klicksound abspielen
                PlaySound(Sound::CLICK2, 100);
                if ((Erg.x == RouteZiel.x) && (Erg.y == RouteZiel.y))
                {
                    Routing::MarkRoute(false);
                    Bmp[BUTTON_STOP].Phase = 0;
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
            else PlaySound(Sound::CLICK, 100);
        }
    }

    void MouseInPanel(short Button, short Push)
    {
        // wenn die Maus in der Minimap ist ->
        if ((InRect(MousePosition.x, MousePosition.y, rcKarte)) && (Button == 0) && (Push != -1))
        {
            // Mauskoordinaten in Minimap
            short mx = MousePosition.x - static_cast<short>(rcKarte.left);
            short my = MousePosition.y - static_cast<short>(rcKarte.top);
            Camera.x = ((TILE_SIZE_X / 4) * (mx - my) + MAX_TILES_X * TILE_SIZE_X / 2)
                - static_cast<short>((rcSpielflaeche.right - rcSpielflaeche.left) / 2);
            Camera.y = ((TILE_SIZE_X / 7) * (my + mx))
                - static_cast<short>((rcSpielflaeche.bottom - rcSpielflaeche.top) / 2);
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_GRID].rcDes))
        {
            if (Gitter) Renderer::DrawText(GITTERAUS, TXTTEXTFELD, 2);
            else Renderer::DrawText(GITTERAN, TXTTEXTFELD, 2);

            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Gitter = !Gitter;
                World::Generate();
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_ANIMATION].rcDes))
        {
            if (LAnimation) Renderer::DrawText(ANIMATIONAUS, TXTTEXTFELD, 2);
            else Renderer::DrawText(ANIMATIONAN, TXTTEXTFELD, 2);

            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                LAnimation = !LAnimation;
                World::Generate();
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SOUND].rcDes))
        {
            if (Soundzustand == 1) Renderer::DrawText(SOUNDAUS, TXTTEXTFELD, 2);
            else if (Soundzustand == 0) Renderer::DrawText(SOUNDAN, TXTTEXTFELD, 2);
            else Renderer::DrawText(KEINSOUND, TXTTEXTFELD, 2);

            if ((Button == 0) && (Push == 1))
            {
                if (Soundzustand == 1)
                {
                    for (short i = 1; i < Sound::COUNT; i++) Sound::StopSound(i);
                    Soundzustand = 0;
                }
                else if (Soundzustand == 0)
                {
                    Soundzustand = 1;
                    PlaySound(Sound::CLICK2, 100);
                }
                else PlaySound(Sound::CLICK, 100);
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_END].rcDes))
        {
            Renderer::DrawText(BEENDEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_END].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                Guy.Aktiv = false;
                Guy.Aktion = Action::QUIT;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_NEW].rcDes))
        {
            Renderer::DrawText(NEU, TXTTEXTFELD, 2);
            Bmp[BUTTON_NEW].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                Guy.Aktiv = false;
                Guy.Aktion = Action::RESTART;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_NEW_DAY].rcDes))
        {
            Renderer::DrawText(TAGNEU2, TXTTEXTFELD, 2);
            Bmp[BUTTON_NEW_DAY].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                Guy.Aktiv = false;
                Guy.Aktion = Action::DAY_RESTART;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_ACTION].rcDes))
        {
            if (HauptMenue == Menu::ACTION) Renderer::DrawText(MEAKTIONZU, TXTTEXTFELD, 2);
            else Renderer::DrawText(MEAKTIONAUF, TXTTEXTFELD, 2);
            Bmp[BUTTON_ACTION].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if (HauptMenue == Menu::ACTION) HauptMenue = Menu::NONE;
                else HauptMenue = Menu::ACTION;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_CONSTRUCT].rcDes) &&
            (Bmp[BUTTON_CONSTRUCT].Phase != -1))
        {
            if (HauptMenue == Menu::BUILD) Renderer::DrawText(MEBAUENZU, TXTTEXTFELD, 2);
            else Renderer::DrawText(MEBAUENAUF, TXTTEXTFELD, 2);
            Bmp[BUTTON_CONSTRUCT].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if (HauptMenue == Menu::BUILD) HauptMenue = Menu::NONE;
                else HauptMenue = Menu::BUILD;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_INVENTORY].rcDes))
        {
            if (HauptMenue == Menu::INVENTORY) Renderer::DrawText(MEINVENTARZU, TXTTEXTFELD, 2);
            else Renderer::DrawText(MEINVENTARAUF, TXTTEXTFELD, 2);
            Bmp[BUTTON_INVENTORY].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if (HauptMenue == Menu::INVENTORY) HauptMenue = Menu::NONE;
                else HauptMenue = Menu::INVENTORY;
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_CONTINUE].rcDes)) &&
            (Bmp[BUTTON_CONTINUE].Phase != -1))
        {
            Renderer::DrawText(WEITER, TXTTEXTFELD, 2);

            Bmp[BUTTON_CONTINUE].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Bmp[BUTTON_STOP].Phase = 0;
                Routing::MarkRoute(false);
                RouteZiel.x = -1;
                RouteZiel.y = -1;
                Guy.PosAlt = Guy.PosScreen;
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
                {
                case TENT: Guy.Aktion = Action::TENT;
                    break;
                case FIELD: Guy.Aktion = Action::FIELD;
                    break;
                case BOAT: Guy.Aktion = Action::BOAT;
                    break;
                case PIPE: Guy.Aktion = Action::PIPE;
                    break;
                case SOS: Guy.Aktion = Action::SOS_SIGN;
                    break;
                case HOUSE_1: Guy.Aktion = Action::HOUSE1;
                    break;
                case HOUSE_2: Guy.Aktion = Action::HOUSE2;
                    break;
                case HOUSE_3: Guy.Aktion = Action::HOUSE3;
                    break;
                case BONFIRE: Guy.Aktion = Action::FIREPLACE;
                    break;
                }
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_STOP].rcDes)) &&
            (Bmp[BUTTON_STOP].Phase != -1))
        {
            Renderer::DrawText(STOP, TXTTEXTFELD, 2);

            Bmp[BUTTON_STOP].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                Guy.Aktion = Action::CANCEL;
                Bmp[BUTTON_STOP].Phase = -1;
            }
        }

        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_LAY_DOWN].rcDes)) &&
            (Bmp[BUTTON_LAY_DOWN].Phase != -1))
        {
            Renderer::DrawText(BEGINNABLEGEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_LAY_DOWN].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1) Guy.Aktion = Action::UNDOCK;
                else Guy.Aktion = Action::DOCK;
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SEARCH].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_SEARCH].Phase != -1))
        {
            Renderer::DrawText(BEGINNSUCHEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_SEARCH].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                Guy.Aktion = Action::SEARCH;
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_EAT].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_EAT].Phase != -1))
        {
            Renderer::DrawText(BEGINNESSEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_EAT].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                if (((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BUSH) ||
                        (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == FIELD)) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Phase == Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl - 1))
                    Guy.Aktion = Action::EAT;
                else if (((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= RIVER_1) &&
                        (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= FLOODGATE_6)) ||
                    ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == PIPE) &&
                        (Scape[Guy.Pos.x][Guy.Pos.y].Phase == 1)))
                    Guy.Aktion = Action::DRINK;
                else PapierText = Renderer::DrawText(KEINESSENTRINKEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SLEEP].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_SLEEP].Phase != -1))
        {
            Renderer::DrawText(BEGINNSCHLAFEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_SLEEP].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1)
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = Action::SLEEP;
                }
                else PapierText = Renderer::DrawText(NICHTAUFWASSERSCHLAFEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_CHOP].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_CHOP].Phase != -1))
        {
            Renderer::DrawText(BEGINNFAELLEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_CHOP].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                if (Guy.Inventar[RAW_TREE_TRUNK] <= 10)
                {
                    if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= TREE_1) &&
                        (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= TREE_4))
                    {
                        Guy.Aktion = Action::LOG;
                    }
                    else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TREE_BIG) ||
                        ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= HOUSE_1) &&
                            (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= HOUSE_3)))
                        PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
                    else PapierText = Renderer::DrawText(KEINBAUM, TXTPAPIER, 1);
                }
                else PapierText = Renderer::DrawText(ROHSTAMMZUVIEL, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_FISH].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_FISH].Phase != -1))
        {
            Renderer::DrawText(BEGINNANGELN, TXTTEXTFELD, 2);
            Bmp[BUTTON_FISH].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                if (((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= RIVER_1) &&
                        (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= FLOODGATE_6)) ||
                    (BootsFahrt))
                    Guy.Aktion = Action::FISH;
                else PapierText = Renderer::DrawText(KEINWASSER, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_IGNITE].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_IGNITE].Phase != -1))
        {
            Renderer::DrawText(BEGINNANZUENDEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_IGNITE].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BONFIRE) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
                    Guy.Aktion = Action::LIGHT;
                else PapierText = Renderer::DrawText(KEINEFEUERST, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_LOOK_OUT].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_LOOK_OUT].Phase != -1))
        {
            Renderer::DrawText(BEGINNAUSSCHAU, TXTTEXTFELD, 2);
            Bmp[BUTTON_LOOK_OUT].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                if (Scape[Guy.Pos.x][Guy.Pos.y].Art != 1)
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = Action::LOOKOUT;
                }
                else PapierText = Renderer::DrawText(WELLENZUHOCH, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_TREASURE].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_TREASURE].Phase != -1))
        {
            Renderer::DrawText(BEGINNSCHATZ, TXTTEXTFELD, 2);
            Bmp[BUTTON_TREASURE].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Art != 1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Type == 0) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1))
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = Action::TREASURE;
                }
                else PapierText = Renderer::DrawText(GRABENBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SLINGSHOT].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_SLINGSHOT].Phase != -1))
        {
            Renderer::DrawText(BEGINNSCHLEUDER, TXTTEXTFELD, 2);
            Bmp[BUTTON_SLINGSHOT].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.AkNummer = 0;
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= TREE_1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= TREE_4))
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = Action::SLINGSHOT;
                }
                else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TREE_BIG) ||
                    ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= HOUSE_1) &&
                        (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= HOUSE_3)))
                    PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
                else PapierText = Renderer::DrawText(KEINVOGEL, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_TREASUREMAP].rcDes)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_TREASUREMAP].Phase != -1))
        {
            Renderer::DrawText(BEGINNSCHATZKARTE, TXTTEXTFELD, 2);
            Bmp[BUTTON_TREASUREMAP].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Renderer::DrawSchatzkarte();
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_FARM].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_FARM].Phase != -1))
        {
            LoadString(g_hInst, BEGINNFELD, StdString, 1024);
            World::MakeRohString(-1, -1, FIELD);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[BUTTON_FARM].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Type == 0) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Art == 4))
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.Aktion = Action::FIELD;
                }
                else if ((Bmp[BUTTON_CONTINUE].Phase != -1) && (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == FIELD))
                {
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = Action::FIELD;
                }
                else PapierText = Renderer::DrawText(FELDBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_TENT].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_TENT].Phase != -1))
        {
            LoadString(g_hInst, BEGINNZELT, StdString, 1024);
            World::MakeRohString(-1, -1, TENT);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[BUTTON_TENT].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Type == 0) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Art != -1))
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.Aktion = Action::TENT;
                }
                else if ((Bmp[BUTTON_CONTINUE].Phase != -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT))
                {
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = Action::TENT;
                }
                else PapierText = Renderer::DrawText(ZELTBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_BOAT].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_BOAT].Phase != -1))
        {
            LoadString(g_hInst, BEGINNBOOT, StdString, 1024);
            World::MakeRohString(-1, -1, BOAT);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[BUTTON_BOAT].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Art == 2) &&
                    ((Scape[Guy.Pos.x - 1][Guy.Pos.y].Art == 1) ||
                        (Scape[Guy.Pos.x][Guy.Pos.y - 1].Art == 1) ||
                        (Scape[Guy.Pos.x + 1][Guy.Pos.y].Art == 1) ||
                        (Scape[Guy.Pos.x][Guy.Pos.y + 1].Art == 1)))
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.Aktion = Action::BOAT;
                }
                else if ((Bmp[BUTTON_CONTINUE].Phase != -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BOAT))
                {
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = Action::BOAT;
                }
                else PapierText = Renderer::DrawText(BOOTBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_PIPE].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_PIPE].Phase != -1))
        {
            LoadString(g_hInst, BEGINNROHR, StdString, 1024);
            World::MakeRohString(-1, -1, PIPE);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[BUTTON_PIPE].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Type == 0))
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.Aktion = Action::PIPE;
                }
                else if ((Bmp[BUTTON_CONTINUE].Phase != -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == PIPE))
                {
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = Action::PIPE;
                }
                else PapierText = Renderer::DrawText(ROHRBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SOS].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_SOS].Phase != -1))
        {
            LoadString(g_hInst, BEGINNSOS, StdString, 1024);
            World::MakeRohString(-1, -1, SOS);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[BUTTON_SOS].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Type == 0))
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.Aktion = Action::SOS_SIGN;
                }
                else if ((Bmp[BUTTON_CONTINUE].Phase != -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == SOS))
                {
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = Action::SOS_SIGN;
                }
                else PapierText = Renderer::DrawText(SOSBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_HOUSE_1].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_HOUSE_1].Phase != -1))
        {
            LoadString(g_hInst, BEGINNHAUS1, StdString, 1024);
            World::MakeRohString(-1, -1, HOUSE_1);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[BUTTON_HOUSE_1].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= TREE_1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= TREE_4))
                    PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TREE_BIG)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.Aktion = Action::HOUSE1;
                }
                else if ((Bmp[BUTTON_CONTINUE].Phase != -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_1))
                {
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = Action::HOUSE1;
                }
                else PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_HOUSE_2].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_HOUSE_2].Phase != -1))
        {
            LoadString(g_hInst, BEGINNHAUS2, StdString, 1024);
            World::MakeRohString(-1, -1, HOUSE_2);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[BUTTON_HOUSE_2].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= TREE_1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= TREE_4))
                    PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TREE_BIG)
                    PapierText = Renderer::DrawText(NICHTOHNELEITER, TXTPAPIER, 1);
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_1)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.Aktion = Action::HOUSE2;
                }
                else if ((Bmp[BUTTON_CONTINUE].Phase != -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_2))
                {
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = Action::HOUSE2;
                }
                else PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_HOUSE_3].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_HOUSE_3].Phase != -1))
        {
            LoadString(g_hInst, BEGINNHAUS3, StdString, 1024);
            World::MakeRohString(-1, -1, HOUSE_3);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[BUTTON_HOUSE_3].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= TREE_1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= TREE_4))
                    PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
                else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TREE_BIG) ||
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_1))
                    PapierText = Renderer::DrawText(NICHTOHNEPLATTFORM, TXTPAPIER, 1);
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_2)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.Aktion = Action::HOUSE3;
                }
                else if ((Bmp[BUTTON_CONTINUE].Phase != -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3))
                {
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = Action::HOUSE3;
                }
                else PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_FIRE].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_FIRE].Phase != -1))
        {
            LoadString(g_hInst, BEGINNFEUERSTELLE, StdString, 1024);
            World::MakeRohString(-1, -1, BONFIRE);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].Aktiv = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].rcText.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].rcText.top), 2);

            Bmp[BUTTON_FIRE].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Type == 0))
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.Aktion = Action::FIREPLACE;
                }
                else if ((Bmp[BUTTON_CONTINUE].Phase != -1) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BONFIRE))
                {
                    Bmp[BUTTON_STOP].Phase = 0;
                    Guy.PosAlt = Guy.PosScreen;
                    Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                    Guy.Aktion = Action::FIREPLACE;
                }
                else PapierText = Renderer::DrawText(FEUERSTELLENBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_DESTROY].rcDes)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_DESTROY].Phase != -1))
        {
            Renderer::DrawText(BEGINNDESTROY, TXTTEXTFELD, 2);
            Bmp[BUTTON_DESTROY].Animation = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= FIELD) &&
                    (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= BONFIRE))
                {
                    Guy.AkNummer = 0;
                    Guy.Aktion = Action::DESTROY;
                }
                else PapierText = Renderer::DrawText(KEINBAUWERK, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[INVENTORY_PAPER].rcDes)) && (HauptMenue == Menu::INVENTORY))
        {
            for (short i = RAW_TREE_BRANCH; i <= RAW_SLINGSHOT; i++)
            {
                if (InRect(MousePosition.x, MousePosition.y, Bmp[i].rcDes) && (Guy.Inventar[i] > 0))
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
                    case RAW_TREE_BRANCH: Renderer::DrawText(AST, TXTTEXTFELD, 2);
                        break;
                    case RAW_STONE: Renderer::DrawText(STEIN, TXTTEXTFELD, 2);
                        break;
                    case RAW_AXE: Renderer::DrawText(AXT, TXTTEXTFELD, 2);
                        break;
                    case RAW_LEAF: Renderer::DrawText(BLATT, TXTTEXTFELD, 2);
                        break;
                    case RAW_TREE_TRUNK: Renderer::DrawText(STAMM, TXTTEXTFELD, 2);
                        break;
                    case RAW_HOE: Renderer::DrawText(EGGE, TXTTEXTFELD, 2);
                        break;
                    case RAW_LIANA: Renderer::DrawText(LIANE, TXTTEXTFELD, 2);
                        break;
                    case RAW_FISHING_POLE: Renderer::DrawText(ANGEL, TXTTEXTFELD, 2);
                        break;
                    case RAW_HAMMER: Renderer::DrawText(HAMMERTEXT, TXTTEXTFELD, 2);
                        break;
                    case RAW_TELESCOPE: Renderer::DrawText(FERNROHR, TXTTEXTFELD, 2);
                        break;
                    case RAW_MATCH: Renderer::DrawText(STREICHHOLZ, TXTTEXTFELD, 2);
                        break;
                    case RAW_SHOVEL: Renderer::DrawText(SCHAUFEL, TXTTEXTFELD, 2);
                        break;
                    case RAW_MAP: Renderer::DrawText(KARTE, TXTTEXTFELD, 2);
                        break;
                    case RAW_SLINGSHOT: Renderer::DrawText(SCHLEUDER, TXTTEXTFELD, 2);
                        break;
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
            if ((Button == 0) && (Push == 1)) PlaySound(Sound::CLICK, 100);
            TwoClicks = -1;
        }
    }

    bool InDreieck(short X, short Y, short X0, short Y0, short X1, short Y1, short X3, short Y3)
    {
        float x = static_cast<float>(X);
        float y = static_cast<float>(Y);
        float x0 = static_cast<float>(X0);
        float y0 = static_cast<float>(Y0);
        float x1 = static_cast<float>(X1);
        float y1 = static_cast<float>(Y1);
        float x3 = static_cast<float>(X3);
        float y3 = static_cast<float>(Y3);

        float c = (x - x1) / (x0 - x1);
        if (c < 0) return false;
        float d = ((y - y3) * (x0 - x1) - (x - x1) * (y0 - y3)) / ((y1 - y3) * (x0 - x1));
        if (d < 0) return false;
        float b = ((y - y0) * (x1 - x0) - (x - x0) * (y1 - y0)) / ((x1 - x0) * (y3 - y1));
        if (b < 0) return false;
        float a = (x - x0) / (x1 - x0) - b;
        if (a < 0) return false;
        return true;
    }

    bool InRect(short x, short y, RECT rcRect)
    {
        if ((x <= rcRect.right) && (x >= rcRect.left) &&
            (y <= rcRect.bottom) && (y >= rcRect.top))
            return true;
        return false;
    }

    void CalcGuyKoor()
    {
        if (Step >= Steps)
        {
            RoutePunkt++;

            if ((RoutePunkt >= (RouteLaenge > 1 ? 2 * (RouteLaenge - 1) : 1) ||
                ((Guy.Aktion == Action::CANCEL) && (RouteLaenge > 1))))
            {
                if (RouteLaenge > 1) Bmp[BUTTON_STOP].Phase = -1;
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

            if ((Guy.Zustand == GUY_SHIP) || (Guy.Zustand == GUY_SWIM)) Guy.Zustand -= 2; // nichts machen
            else if (BootsFahrt) Guy.Zustand = GUY_BOAT_LEFT;
            else Guy.Zustand = GUY_LEFT;

            if (RouteLaenge > 1) // Bei normaler Routenabarbeitung die Richung Kachelmäßig rausfinden
            {
                if (Route[RoutePunkt / 2].x > Route[RoutePunkt / 2 + 1].x) Guy.Zustand += 0;
                else if (Route[RoutePunkt / 2].x < Route[RoutePunkt / 2 + 1].x) Guy.Zustand += 2;
                else if (Route[RoutePunkt / 2].y < Route[RoutePunkt / 2 + 1].y) Guy.Zustand += 3;
                else if (Route[RoutePunkt / 2].y > Route[RoutePunkt / 2 + 1].y) Guy.Zustand += 1;
            }
            else
            {
                if ((RouteKoor[RoutePunkt].x > RouteKoor[RoutePunkt + 1].x) &&
                    (RouteKoor[RoutePunkt].y >= RouteKoor[RoutePunkt + 1].y))
                    Guy.Zustand += 0;
                else if ((RouteKoor[RoutePunkt].x <= RouteKoor[RoutePunkt + 1].x) &&
                    (RouteKoor[RoutePunkt].y > RouteKoor[RoutePunkt + 1].y))
                    Guy.Zustand += 1;
                else if ((RouteKoor[RoutePunkt].x < RouteKoor[RoutePunkt + 1].x) &&
                    (RouteKoor[RoutePunkt].y <= RouteKoor[RoutePunkt + 1].y))
                    Guy.Zustand += 2;
                else if ((RouteKoor[RoutePunkt].x >= RouteKoor[RoutePunkt + 1].x) &&
                    (RouteKoor[RoutePunkt].y < RouteKoor[RoutePunkt + 1].y))
                    Guy.Zustand += 3;
            }

            // Differenz zwischen Ziel und Start
            short Dx = RouteKoor[RoutePunkt + 1].x - RouteKoor[RoutePunkt].x;
            short Dy = RouteKoor[RoutePunkt + 1].y - RouteKoor[RoutePunkt].y;

            GuyPosScreenStart.x = RouteKoor[RoutePunkt].x;
            GuyPosScreenStart.y = RouteKoor[RoutePunkt].y;
            Step = 0;

            if (abs(Dx) > abs(Dy))
            {
                if (Dx > 0) Schrittx = 1;
                else Schrittx = -1;
                if (Dx == 0) Schritty = 0;
                else Schritty = static_cast<float>(Dy) / static_cast<float>(Dx * Schrittx);
                Steps = abs(Dx);
            }
            else
            {
                if (Dy > 0) Schritty = 1;
                else Schritty = -1;
                if (Dy == 0) Schrittx = 0;
                else Schrittx = static_cast<float>(Dx) / static_cast<float>(Dy * Schritty);
                Steps = abs(Dy);
            }
        }

        if (Bild % Scape[Guy.Pos.x][Guy.Pos.y].LaufZeit == 0)
        {
            Step++;
            short i;
            if (BootsFahrt) i = 4;
            else i = 2;
            if (Step % i == 0)
            {
                Bmp[Guy.Zustand].Phase++;
                if (Bmp[Guy.Zustand].Phase >= Bmp[Guy.Zustand].Anzahl) Bmp[Guy.Zustand].Phase = 0;
            }
            Guy.PosScreen.x = GuyPosScreenStart.x + ROUND(Step*Schrittx);
            Guy.PosScreen.y = GuyPosScreenStart.y + ROUND(Step*Schritty);
            if ((Spielzustand == State::INTRO) || (Spielzustand == State::RESCUED)) // Beim Intro fährt die Kamera mit
            {
                Camera.x = Guy.PosScreen.x - static_cast<short>(rcGesamt.right / 2);
                Camera.y = Guy.PosScreen.y - static_cast<short>(rcGesamt.bottom / 2);
            }
        }
    }

    void CalcKoor()
    {
        // Bildschirmkoordinaten berechnen und speichern
        for (short y = 0; y < MAX_TILESY; y++)
            for (short x = 0; x < MAX_TILES_X; x++)
            {
                Scape[x][y].xScreen = TILE_SIZE_X / 2 * MAX_TILES_X + 32 +
                    x * TILE_SIZE_X / 2 - y * TILE_SIZE_Y / 2 +
                    (-6 * y) + x; // seltsame Korrekturen
                Scape[x][y].yScreen =
                    x * TILE_SIZE_X / 2 + y * TILE_SIZE_Y / 2 - 16 * Scape[x][y].Height +
                    (-13 * x) + (-8 * y); // seltsame Korrekturen

                if ((x == 0) && (y == 0))
                    ScapeGrenze.top = Scape[x][y].yScreen;
                if ((x == 0) && (y == MAX_TILESY - 1))
                    ScapeGrenze.left = Scape[x][y].xScreen;
                if ((x == MAX_TILES_X - 1) && (y == MAX_TILESY - 1))
                    ScapeGrenze.bottom = Scape[x][y].yScreen + TILE_SIZE_Y;
                if ((x == MAX_TILES_X - 1) && (y == 0))
                    ScapeGrenze.right = Scape[x][y].xScreen + TILE_SIZE_X;
            }
    }

    bool LineIntersect(ZWEID LineStartPos, ZWEID Pos, bool store)
    {
        float Sx, Sy;
        bool erg = false;

        Steps = 0;

        short Dx = LineStartPos.x - Pos.x;
        short Dy = LineStartPos.y - Pos.y;
        float x = LineStartPos.x;
        float y = LineStartPos.y;

        if (abs(Dx) > abs(Dy))
        {
            if (Dx > 0) Sx = -1;
            else Sx = 1;
            if (Dx == 0) Sy = 0;
            else Sy = static_cast<float>(Dy) / static_cast<float>(Dx * Sx);
            Steps = abs(Dx);
        }
        else
        {
            if (Dy > 0) Sy = -1;
            else Sy = 1;
            if (Dy == 0) Sx = 0;
            else Sx = static_cast<float>(Dx) / static_cast<float>(Dy * Sy);
            Steps = abs(Dy);
        }

        for (short i = 0; i < Steps; i++)
        {
            if (!Scape[ROUND(x)][ROUND(y)].Begehbar) erg = true;
            if ((store))
            {
                Route[RouteLaenge].x = ROUND(x);
                Route[RouteLaenge].y = ROUND(y);
                RouteLaenge++;
            }
            float Nextx = x + Sx;
            float Nexty = y + Sy;
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
        if (rcRectdes.left < rcBereich.left)
        {
            rcRectsrc.left = rcRectsrc.left + rcBereich.left - rcRectdes.left;
            rcRectdes.left = rcBereich.left;
        }
        if (rcRectdes.top < rcBereich.top)
        {
            rcRectsrc.top = rcRectsrc.top + rcBereich.top - rcRectdes.top;
            rcRectdes.top = rcBereich.top;
        }
        if (rcRectdes.right > rcBereich.right)
        {
            rcRectsrc.right = rcRectsrc.right + rcBereich.right - rcRectdes.right;
            rcRectdes.right = rcBereich.right;
        }
        if (rcRectdes.bottom > rcBereich.bottom)
        {
            rcRectsrc.bottom = rcRectsrc.bottom + rcBereich.bottom - rcRectdes.bottom;
            rcRectdes.bottom = rcBereich.bottom;
        }
    }

    void ButtAniAus()
    {
        for (short i = BUTTON_GRID; i <= BUTTON_DESTROY; i++)
        {
            Bmp[i].Animation = false;
        }
    }

    void AbspannCalc()
    {
        short i;

        if (AbspannZustand == 0)
        {
            for (short k = 1; k < 10; k++)
            {
                if (AbspannListe[AbspannNr][k].Bild == -1) break;
                if (!AbspannListe[AbspannNr][k].Aktiv) continue;
                i = 150 / LastBild;
                Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top -= i;

                if (Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top < MAX_SCREEN_Y - 400)
                {
                    if ((!AbspannListe[AbspannNr][k + 1].Aktiv) &&
                        (AbspannListe[AbspannNr][k + 1].Bild != -1))
                        AbspannListe[AbspannNr][k + 1].Aktiv = true;
                }
                if (Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top < 0)
                {
                    AbspannListe[AbspannNr][k].Aktiv = false;
                    Bmp[AbspannListe[AbspannNr][k].Bild].rcDes.top = MAX_SCREEN_Y - Bmp[AbspannListe[AbspannNr][k].Bild].Hoehe / 2;
                    if (!AbspannListe[AbspannNr][k + 1].Aktiv)
                    {
                        if (AbspannListe[AbspannNr + 1][0].Bild != -1)
                        {
                            AbspannNr++;
                            AbspannListe[AbspannNr][1].Aktiv = true;
                        }
                        else
                        {
                            AbspannNr = GUY_LEFT;
                            AbspannZustand = 1;
                        }
                    }
                }
            }
        }
        else if (AbspannZustand == 1)
        {
            i = LastBild / Bmp[AbspannNr].Geschwindigkeit;
            if (i < 1) i = 1;
            if (Bild % i == 0)
            {
                Bmp[AbspannNr].Phase++;
                if (Bmp[AbspannNr].Phase >= Bmp[AbspannNr].Anzahl)
                {
                    Bmp[AbspannNr].Phase = 0;
                    AbspannNr++;
                    if (AbspannNr > GUY_SLINGSHOT) AbspannNr = GUY_LEFT;
                }
            }
        }
    }

    void Animationen()
    {
        short i, j, loop; // Zwischenspeicher


        for (short y = 0; y < MAX_TILESY; y++)
            for (short x = 0; x < MAX_TILES_X; x++)
            {
                j = Scape[x][y].Objekt;
                if ((j == -1) || (!Bmp[j].Animation)) continue;
                i = LastBild / Bmp[j].Geschwindigkeit;
                if (i < 1) i = 1;
                if (Bild % i == 0)
                {
                    if ((j < TREE_DOWN_1) || (j > TREE_DOWN_4) || // Die Baumfällenanimation nur ein mal abspielen
                        (Scape[x][y].Phase != Bmp[j].Anzahl - 1))
                        Scape[x][y].Phase++;
                    if (Scape[x][y].Phase >= Bmp[j].Anzahl)
                        Scape[x][y].Phase = 0;
                }
            }

        for (j = BUTTON_GRID; j <= BUTTON_DESTROY; j++)
        {
            if (!Bmp[j].Animation) continue;
            i = LastBild / Bmp[j].Geschwindigkeit;
            if (i < 1) i = 1;
            if (Bild % i == 0)
            {
                Bmp[j].Phase++;
                if (Bmp[j].Phase >= Bmp[j].Anzahl) Bmp[j].Phase = 0;
            }
        }

        // Spielfigur

        // laufen
        if (((Guy.Zustand >= GUY_LEFT) && (Guy.Zustand <= GUY_BELOW)) ||
            ((Guy.Zustand >= GUY_BOAT_LEFT) && (Guy.Zustand <= GUY_BOAT_BELOW)) ||
            (Guy.Zustand == GUY_SHIP) || (Guy.Zustand == GUY_SWIM))
        {
            i = LastBild / Bmp[Guy.Zustand].Geschwindigkeit;
            if (i < 1) i = 1;
            if (LastBild - Bmp[Guy.Zustand].Geschwindigkeit < 0) loop = 2;
            else loop = 1;
            if (BootsFahrt) loop = loop * 2;
            for (short k = 0; k < loop; k++) if ((Bild % i == 0) && (Guy.Aktiv)) CalcGuyKoor();
            return;
        }
        // sonstige Aktionen
        if ((Guy.Zustand >= GUY_SEARCH) && (Guy.Zustand <= GUY_SLINGSHOT) &&
            (Bmp[Guy.Zustand].Phase != Bmp[Guy.Zustand].Anzahl))
        {
            i = LastBild / Bmp[Guy.Zustand].Geschwindigkeit;
            if (i < 1) i = 1;
            if (Bild % i == 0)
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
