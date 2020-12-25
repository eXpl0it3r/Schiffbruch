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
    Coordinate GuyPosScreenStart; // Absolute StartPosition bei einem Schritt (Für CalcGuyKoor)

    void MouseInSpielflaeche(short Button, short Push, short xDiff, short yDiff)
    {
        char Text[1024], TextTmp[1024]; // Text für Infoleiste

        // Info anzeigen
        Coordinate Erg = Renderer::GetKachel((MousePosition.x + Camera.x), (MousePosition.y + Camera.y)); // Die angeklickte Kachel
        if (Scape[Erg.x][Erg.y].Discovered)
        {
            LoadString(g_hInst, 45 + Scape[Erg.x][Erg.y].Terrain, Text, 1024);
            if ((Scape[Erg.x][Erg.y].Object != -1) && (Scape[Erg.x][Erg.y].Object != SEA_WAVES))
            {
                LoadString(g_hInst, MIT, TextTmp, 1024);
                strcat(Text, " ");
                strcat(Text, TextTmp);
                strcat(Text, " ");

                if ((Scape[Erg.x][Erg.y].Object >= TREE_1) && (Scape[Erg.x][Erg.y].Object <= TREE_4))
                LoadString(g_hInst, BAUMTEXT, TextTmp, 1024);
                else if ((Scape[Erg.x][Erg.y].Object >= RIVER_1) && (Scape[Erg.x][Erg.y].Object <= FLOODGATE_6))
                LoadString(g_hInst, FLUSSTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == BUSH)
                LoadString(g_hInst, BUSCHTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == TENT)
                LoadString(g_hInst, ZELTTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == FIELD)
                LoadString(g_hInst, FELDTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == BOAT)
                LoadString(g_hInst, BOOTTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == PIPE)
                LoadString(g_hInst, ROHRTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == SOS)
                LoadString(g_hInst, SOSTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == HOUSE_1)
                LoadString(g_hInst, HAUS1TEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == HOUSE_2)
                LoadString(g_hInst, HAUS2TEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == HOUSE_3)
                LoadString(g_hInst, HAUS3TEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == TREE_BIG)
                LoadString(g_hInst, BAUMGROSSTEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == BONFIRE)
                LoadString(g_hInst, FEUERSTELLETEXT, TextTmp, 1024);
                else if (Scape[Erg.x][Erg.y].Object == FIRE)
                LoadString(g_hInst, FEUERTEXT, TextTmp, 1024);
                else if ((Scape[Erg.x][Erg.y].Object == WRECK_1) || (Scape[Erg.x][Erg.y].Object == WRECK_2))
                LoadString(g_hInst, WRACKTEXT, TextTmp, 1024);
                strcat(Text, TextTmp);

                if ((Scape[Erg.x][Erg.y].Object >= FIELD) &&
                    (Scape[Erg.x][Erg.y].Object <= BONFIRE))
                {
                    // Baufortschrittanzeigen
                    strcat(Text, " ");
                    strcat(Text, "(");
                    std::sprintf(TextTmp, "%d", (Scape[Erg.x][Erg.y].ConstructionActionNumber * 100) / Bmp[Scape[Erg.x][Erg.y].Object].RequiredActionCases);
                    strcat(Text, TextTmp);
                    strcat(Text, "%");
                    strcat(Text, ")");
                    // benötigte Rohstoffe anzeigen
                    World::MakeRohString(Erg.x, Erg.y, -1);
                    strcat(Text, RohString);
                }
            }
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(Text, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);
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
                (Scape[Erg.x][Erg.y].Discovered) && (!Guy.IsActive) &&
                ((Erg.x != Guy.CurrentPosition.x) || (Erg.y != Guy.CurrentPosition.y)) &&
                (Erg.x > 0) && (Erg.x < MAX_TILES_X - 1) &&
                (Erg.y > 0) && (Erg.y < MAX_TILESY - 1))
            {
                // Klicksound abspielen
                PlaySound(Sound::CLICK2, 100);
                if ((Erg.x == RouteZiel.x) && (Erg.y == RouteZiel.y))
                {
                    Routing::MarkRoute(false);
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.IsActive = true;
                    RoutePunkt = -1;
                    Steps = 0;
                    Step = 0;
                }
                else
                {
                    Routing::MarkRoute(false);
                    RouteStart.x = Guy.CurrentPosition.x;
                    RouteStart.y = Guy.CurrentPosition.y;
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
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_GRID].targetRect))
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
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_ANIMATION].targetRect))
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
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SOUND].targetRect))
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
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_END].targetRect))
        {
            Renderer::DrawText(BEENDEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_END].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                Guy.IsActive = false;
                Guy.CurrentAction = Action::QUIT;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_NEW].targetRect))
        {
            Renderer::DrawText(NEU, TXTTEXTFELD, 2);
            Bmp[BUTTON_NEW].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                Guy.IsActive = false;
                Guy.CurrentAction = Action::RESTART;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_NEW_DAY].targetRect))
        {
            Renderer::DrawText(TAGNEU2, TXTTEXTFELD, 2);
            Bmp[BUTTON_NEW_DAY].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                Guy.IsActive = false;
                Guy.CurrentAction = Action::DAY_RESTART;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_ACTION].targetRect))
        {
            if (HauptMenue == Menu::ACTION) Renderer::DrawText(MEAKTIONZU, TXTTEXTFELD, 2);
            else Renderer::DrawText(MEAKTIONAUF, TXTTEXTFELD, 2);
            Bmp[BUTTON_ACTION].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if (HauptMenue == Menu::ACTION) HauptMenue = Menu::NONE;
                else HauptMenue = Menu::ACTION;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_CONSTRUCT].targetRect) &&
            (Bmp[BUTTON_CONSTRUCT].AnimationPhase != -1))
        {
            if (HauptMenue == Menu::BUILD) Renderer::DrawText(MEBAUENZU, TXTTEXTFELD, 2);
            else Renderer::DrawText(MEBAUENAUF, TXTTEXTFELD, 2);
            Bmp[BUTTON_CONSTRUCT].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if (HauptMenue == Menu::BUILD) HauptMenue = Menu::NONE;
                else HauptMenue = Menu::BUILD;
            }
        }
        else if (InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_INVENTORY].targetRect))
        {
            if (HauptMenue == Menu::INVENTORY) Renderer::DrawText(MEINVENTARZU, TXTTEXTFELD, 2);
            else Renderer::DrawText(MEINVENTARAUF, TXTTEXTFELD, 2);
            Bmp[BUTTON_INVENTORY].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if (HauptMenue == Menu::INVENTORY) HauptMenue = Menu::NONE;
                else HauptMenue = Menu::INVENTORY;
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_CONTINUE].targetRect)) &&
            (Bmp[BUTTON_CONTINUE].AnimationPhase != -1))
        {
            Renderer::DrawText(WEITER, TXTTEXTFELD, 2);

            Bmp[BUTTON_CONTINUE].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Bmp[BUTTON_STOP].AnimationPhase = 0;
                Routing::MarkRoute(false);
                RouteZiel.x = -1;
                RouteZiel.y = -1;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                switch (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object)
                {
                case TENT: Guy.CurrentAction = Action::TENT;
                    break;
                case FIELD: Guy.CurrentAction = Action::FIELD;
                    break;
                case BOAT: Guy.CurrentAction = Action::BOAT;
                    break;
                case PIPE: Guy.CurrentAction = Action::PIPE;
                    break;
                case SOS: Guy.CurrentAction = Action::SOS_SIGN;
                    break;
                case HOUSE_1: Guy.CurrentAction = Action::HOUSE1;
                    break;
                case HOUSE_2: Guy.CurrentAction = Action::HOUSE2;
                    break;
                case HOUSE_3: Guy.CurrentAction = Action::HOUSE3;
                    break;
                case BONFIRE: Guy.CurrentAction = Action::FIREPLACE;
                    break;
                }
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_STOP].targetRect)) &&
            (Bmp[BUTTON_STOP].AnimationPhase != -1))
        {
            Renderer::DrawText(STOP, TXTTEXTFELD, 2);

            Bmp[BUTTON_STOP].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                Guy.CurrentAction = Action::CANCEL;
                Bmp[BUTTON_STOP].AnimationPhase = -1;
            }
        }

        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_LAY_DOWN].targetRect)) &&
            (Bmp[BUTTON_LAY_DOWN].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNABLEGEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_LAY_DOWN].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                if (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Terrain != 1) Guy.CurrentAction = Action::UNDOCK;
                else Guy.CurrentAction = Action::DOCK;
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SEARCH].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_SEARCH].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNSUCHEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_SEARCH].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                Guy.CurrentAction = Action::SEARCH;
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_EAT].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_EAT].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNESSEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_EAT].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                if (((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == BUSH) ||
                        (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == FIELD)) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].AnimationPhase == Bmp[Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object].AnimationPhaseCount - 1))
                    Guy.CurrentAction = Action::EAT;
                else if (((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= RIVER_1) &&
                        (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= FLOODGATE_6)) ||
                    ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == PIPE) &&
                        (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].AnimationPhase == 1)))
                    Guy.CurrentAction = Action::DRINK;
                else PapierText = Renderer::DrawText(KEINESSENTRINKEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SLEEP].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_SLEEP].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNSCHLAFEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_SLEEP].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Terrain != 1)
                {
                    Guy.ActionNumber = 0;
                    Guy.CurrentAction = Action::SLEEP;
                }
                else PapierText = Renderer::DrawText(NICHTAUFWASSERSCHLAFEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_CHOP].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_CHOP].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNFAELLEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_CHOP].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                if (Guy.Inventory[RAW_TREE_TRUNK] <= 10)
                {
                    if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= TREE_1) &&
                        (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= TREE_4))
                    {
                        Guy.CurrentAction = Action::LOG;
                    }
                    else if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == TREE_BIG) ||
                        ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= HOUSE_1) &&
                            (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= HOUSE_3)))
                        PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
                    else PapierText = Renderer::DrawText(KEINBAUM, TXTPAPIER, 1);
                }
                else PapierText = Renderer::DrawText(ROHSTAMMZUVIEL, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_FISH].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_FISH].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNANGELN, TXTTEXTFELD, 2);
            Bmp[BUTTON_FISH].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                if (((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= RIVER_1) &&
                        (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= FLOODGATE_6)) ||
                    (BootsFahrt))
                    Guy.CurrentAction = Action::FISH;
                else PapierText = Renderer::DrawText(KEINWASSER, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_IGNITE].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_IGNITE].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNANZUENDEN, TXTTEXTFELD, 2);
            Bmp[BUTTON_IGNITE].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == BONFIRE) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].AnimationPhase < Bmp[Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object].AnimationPhaseCount))
                    Guy.CurrentAction = Action::LIGHT;
                else PapierText = Renderer::DrawText(KEINEFEUERST, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_LOOK_OUT].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_LOOK_OUT].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNAUSSCHAU, TXTTEXTFELD, 2);
            Bmp[BUTTON_LOOK_OUT].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                if (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Terrain != 1)
                {
                    Guy.ActionNumber = 0;
                    Guy.CurrentAction = Action::LOOKOUT;
                }
                else PapierText = Renderer::DrawText(WELLENZUHOCH, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_TREASURE].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_TREASURE].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNSCHATZ, TXTTEXTFELD, 2);
            Bmp[BUTTON_TREASURE].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Terrain != 1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Type == 0) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == -1))
                {
                    Guy.ActionNumber = 0;
                    Guy.CurrentAction = Action::TREASURE;
                }
                else PapierText = Renderer::DrawText(GRABENBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SLINGSHOT].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_SLINGSHOT].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNSCHLEUDER, TXTTEXTFELD, 2);
            Bmp[BUTTON_SLINGSHOT].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Guy.ActionNumber = 0;
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= TREE_1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= TREE_4))
                {
                    Guy.ActionNumber = 0;
                    Guy.CurrentAction = Action::SLINGSHOT;
                }
                else if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == TREE_BIG) ||
                    ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= HOUSE_1) &&
                        (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= HOUSE_3)))
                    PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
                else PapierText = Renderer::DrawText(KEINVOGEL, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_TREASUREMAP].targetRect)) &&
            (HauptMenue == Menu::ACTION) && (Bmp[BUTTON_TREASUREMAP].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNSCHATZKARTE, TXTTEXTFELD, 2);
            Bmp[BUTTON_TREASUREMAP].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                Renderer::DrawSchatzkarte();
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_FARM].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_FARM].AnimationPhase != -1))
        {
            LoadString(g_hInst, BEGINNFELD, StdString, 1024);
            World::MakeRohString(-1, -1, FIELD);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

            Bmp[BUTTON_FARM].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Type == 0) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Terrain == 4))
                {
                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].ConstructionActionNumber = 0;
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.CurrentAction = Action::FIELD;
                }
                else if ((Bmp[BUTTON_CONTINUE].AnimationPhase != -1) && (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == FIELD))
                {
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.OriginalPosition = Guy.ScreenPosition;
                    Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                        Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                    Guy.CurrentAction = Action::FIELD;
                }
                else PapierText = Renderer::DrawText(FELDBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_TENT].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_TENT].AnimationPhase != -1))
        {
            LoadString(g_hInst, BEGINNZELT, StdString, 1024);
            World::MakeRohString(-1, -1, TENT);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

            Bmp[BUTTON_TENT].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Type == 0) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Terrain != -1))
                {
                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].ConstructionActionNumber = 0;
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.CurrentAction = Action::TENT;
                }
                else if ((Bmp[BUTTON_CONTINUE].AnimationPhase != -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == TENT))
                {
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.OriginalPosition = Guy.ScreenPosition;
                    Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                        Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                    Guy.CurrentAction = Action::TENT;
                }
                else PapierText = Renderer::DrawText(ZELTBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_BOAT].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_BOAT].AnimationPhase != -1))
        {
            LoadString(g_hInst, BEGINNBOOT, StdString, 1024);
            World::MakeRohString(-1, -1, BOAT);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

            Bmp[BUTTON_BOAT].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Terrain == 2) &&
                    ((Scape[Guy.CurrentPosition.x - 1][Guy.CurrentPosition.y].Terrain == 1) ||
                        (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y - 1].Terrain == 1) ||
                        (Scape[Guy.CurrentPosition.x + 1][Guy.CurrentPosition.y].Terrain == 1) ||
                        (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y + 1].Terrain == 1)))
                {
                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].ConstructionActionNumber = 0;
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.CurrentAction = Action::BOAT;
                }
                else if ((Bmp[BUTTON_CONTINUE].AnimationPhase != -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == BOAT))
                {
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.OriginalPosition = Guy.ScreenPosition;
                    Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                        Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                    Guy.CurrentAction = Action::BOAT;
                }
                else PapierText = Renderer::DrawText(BOOTBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_PIPE].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_PIPE].AnimationPhase != -1))
        {
            LoadString(g_hInst, BEGINNROHR, StdString, 1024);
            World::MakeRohString(-1, -1, PIPE);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

            Bmp[BUTTON_PIPE].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Type == 0))
                {
                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].ConstructionActionNumber = 0;
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.CurrentAction = Action::PIPE;
                }
                else if ((Bmp[BUTTON_CONTINUE].AnimationPhase != -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == PIPE))
                {
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.OriginalPosition = Guy.ScreenPosition;
                    Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                        Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                    Guy.CurrentAction = Action::PIPE;
                }
                else PapierText = Renderer::DrawText(ROHRBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_SOS].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_SOS].AnimationPhase != -1))
        {
            LoadString(g_hInst, BEGINNSOS, StdString, 1024);
            World::MakeRohString(-1, -1, SOS);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

            Bmp[BUTTON_SOS].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Type == 0))
                {
                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].ConstructionActionNumber = 0;
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.CurrentAction = Action::SOS_SIGN;
                }
                else if ((Bmp[BUTTON_CONTINUE].AnimationPhase != -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == SOS))
                {
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.OriginalPosition = Guy.ScreenPosition;
                    Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                        Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                    Guy.CurrentAction = Action::SOS_SIGN;
                }
                else PapierText = Renderer::DrawText(SOSBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_HOUSE_1].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_HOUSE_1].AnimationPhase != -1))
        {
            LoadString(g_hInst, BEGINNHAUS1, StdString, 1024);
            World::MakeRohString(-1, -1, HOUSE_1);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

            Bmp[BUTTON_HOUSE_1].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= TREE_1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= TREE_4))
                    PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
                else if (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == TREE_BIG)
                {
                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].ConstructionActionNumber = 0;
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.CurrentAction = Action::HOUSE1;
                }
                else if ((Bmp[BUTTON_CONTINUE].AnimationPhase != -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == HOUSE_1))
                {
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.OriginalPosition = Guy.ScreenPosition;
                    Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                        Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                    Guy.CurrentAction = Action::HOUSE1;
                }
                else PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_HOUSE_2].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_HOUSE_2].AnimationPhase != -1))
        {
            LoadString(g_hInst, BEGINNHAUS2, StdString, 1024);
            World::MakeRohString(-1, -1, HOUSE_2);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

            Bmp[BUTTON_HOUSE_2].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= TREE_1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= TREE_4))
                    PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
                else if (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == TREE_BIG)
                    PapierText = Renderer::DrawText(NICHTOHNELEITER, TXTPAPIER, 1);
                else if (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == HOUSE_1)
                {
                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].ConstructionActionNumber = 0;
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.CurrentAction = Action::HOUSE2;
                }
                else if ((Bmp[BUTTON_CONTINUE].AnimationPhase != -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == HOUSE_2))
                {
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.OriginalPosition = Guy.ScreenPosition;
                    Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                        Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                    Guy.CurrentAction = Action::HOUSE2;
                }
                else PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_HOUSE_3].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_HOUSE_3].AnimationPhase != -1))
        {
            LoadString(g_hInst, BEGINNHAUS3, StdString, 1024);
            World::MakeRohString(-1, -1, HOUSE_3);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

            Bmp[BUTTON_HOUSE_3].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= TREE_1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= TREE_4))
                    PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
                else if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == TREE_BIG) ||
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == HOUSE_1))
                    PapierText = Renderer::DrawText(NICHTOHNEPLATTFORM, TXTPAPIER, 1);
                else if (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == HOUSE_2)
                {
                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].ConstructionActionNumber = 0;
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.CurrentAction = Action::HOUSE3;
                }
                else if ((Bmp[BUTTON_CONTINUE].AnimationPhase != -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == HOUSE_3))
                {
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.OriginalPosition = Guy.ScreenPosition;
                    Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                        Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                    Guy.CurrentAction = Action::HOUSE3;
                }
                else PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_FIRE].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_FIRE].AnimationPhase != -1))
        {
            LoadString(g_hInst, BEGINNFEUERSTELLE, StdString, 1024);
            World::MakeRohString(-1, -1, BONFIRE);
            strcat(StdString, RohString);
            TextBereich[TXTTEXTFELD].HasText = true;
            Renderer::DrawString(StdString, static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                                 static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

            Bmp[BUTTON_FIRE].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Type == 0))
                {
                    Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].ConstructionActionNumber = 0;
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.CurrentAction = Action::FIREPLACE;
                }
                else if ((Bmp[BUTTON_CONTINUE].AnimationPhase != -1) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object == BONFIRE))
                {
                    Bmp[BUTTON_STOP].AnimationPhase = 0;
                    Guy.OriginalPosition = Guy.ScreenPosition;
                    Routing::ShortRoute(Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.x,
                                        Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].GPosAlt.y);
                    Guy.CurrentAction = Action::FIREPLACE;
                }
                else PapierText = Renderer::DrawText(FEUERSTELLENBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[BUTTON_DESTROY].targetRect)) &&
            (HauptMenue == Menu::BUILD) && (Bmp[BUTTON_DESTROY].AnimationPhase != -1))
        {
            Renderer::DrawText(BEGINNDESTROY, TXTTEXTFELD, 2);
            Bmp[BUTTON_DESTROY].IsAnimationRunning = true;
            if ((Button == 0) && (Push == 1))
            {
                PlaySound(Sound::CLICK2, 100);
                if ((Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object >= FIELD) &&
                    (Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].Object <= BONFIRE))
                {
                    Guy.ActionNumber = 0;
                    Guy.CurrentAction = Action::DESTROY;
                }
                else PapierText = Renderer::DrawText(KEINBAUWERK, TXTPAPIER, 1);
            }
        }
        else if ((InRect(MousePosition.x, MousePosition.y, Bmp[INVENTORY_PAPER].targetRect)) && (HauptMenue == Menu::INVENTORY))
        {
            for (short i = RAW_TREE_BRANCH; i <= RAW_SLINGSHOT; i++)
            {
                if (InRect(MousePosition.x, MousePosition.y, Bmp[i].targetRect) && (Guy.Inventory[i] > 0))
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
        else if (InRect(MousePosition.x, MousePosition.y, TextBereich[TXTTAGESZEIT].textRect))
            Renderer::DrawText(SOSPAET, TXTTEXTFELD, 2);
        else if (InRect(MousePosition.x, MousePosition.y, TextBereich[TXTCHANCE].textRect))
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
                ((Guy.CurrentAction == Action::CANCEL) && (RouteLaenge > 1))))
            {
                if (RouteLaenge > 1) Bmp[BUTTON_STOP].AnimationPhase = -1;
                Bmp[Guy.AnimationState].AnimationPhase = 0;
                Guy.IsActive = false;
                RouteZiel.x = -1;
                RouteZiel.y = -1;
                return;
            }
            Guy.CurrentPosition.x = Route[(RoutePunkt + 1) / 2].x;
            Guy.CurrentPosition.y = Route[(RoutePunkt + 1) / 2].y;
            World::Entdecken();

            if (BootsFahrt)
                World::AddTime(0, Scape[Route[(RoutePunkt + 1) / 2].x][Route[(RoutePunkt + 1) / 2].y].RunningTime * 3);
            else World::AddTime(0, Scape[Route[(RoutePunkt + 1) / 2].x][Route[(RoutePunkt + 1) / 2].y].RunningTime * 5);
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);

            if ((Guy.AnimationState == GUY_SHIP) || (Guy.AnimationState == GUY_SWIM)) Guy.AnimationState -= 2; // nichts machen
            else if (BootsFahrt) Guy.AnimationState = GUY_BOAT_LEFT;
            else Guy.AnimationState = GUY_LEFT;

            if (RouteLaenge > 1) // Bei normaler Routenabarbeitung die Richung Kachelmäßig rausfinden
            {
                if (Route[RoutePunkt / 2].x > Route[RoutePunkt / 2 + 1].x) Guy.AnimationState += 0;
                else if (Route[RoutePunkt / 2].x < Route[RoutePunkt / 2 + 1].x) Guy.AnimationState += 2;
                else if (Route[RoutePunkt / 2].y < Route[RoutePunkt / 2 + 1].y) Guy.AnimationState += 3;
                else if (Route[RoutePunkt / 2].y > Route[RoutePunkt / 2 + 1].y) Guy.AnimationState += 1;
            }
            else
            {
                if ((RouteKoor[RoutePunkt].x > RouteKoor[RoutePunkt + 1].x) &&
                    (RouteKoor[RoutePunkt].y >= RouteKoor[RoutePunkt + 1].y))
                    Guy.AnimationState += 0;
                else if ((RouteKoor[RoutePunkt].x <= RouteKoor[RoutePunkt + 1].x) &&
                    (RouteKoor[RoutePunkt].y > RouteKoor[RoutePunkt + 1].y))
                    Guy.AnimationState += 1;
                else if ((RouteKoor[RoutePunkt].x < RouteKoor[RoutePunkt + 1].x) &&
                    (RouteKoor[RoutePunkt].y <= RouteKoor[RoutePunkt + 1].y))
                    Guy.AnimationState += 2;
                else if ((RouteKoor[RoutePunkt].x >= RouteKoor[RoutePunkt + 1].x) &&
                    (RouteKoor[RoutePunkt].y < RouteKoor[RoutePunkt + 1].y))
                    Guy.AnimationState += 3;
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

        if (Bild % Scape[Guy.CurrentPosition.x][Guy.CurrentPosition.y].RunningTime == 0)
        {
            Step++;
            short i;
            if (BootsFahrt) i = 4;
            else i = 2;
            if (Step % i == 0)
            {
                Bmp[Guy.AnimationState].AnimationPhase++;
                if (Bmp[Guy.AnimationState].AnimationPhase >= Bmp[Guy.AnimationState].AnimationPhaseCount) Bmp[Guy.AnimationState].AnimationPhase = 0;
            }
            Guy.ScreenPosition.x = GuyPosScreenStart.x + ROUND(Step*Schrittx);
            Guy.ScreenPosition.y = GuyPosScreenStart.y + ROUND(Step*Schritty);
            if ((Spielzustand == State::INTRO) || (Spielzustand == State::RESCUED)) // Beim Intro fährt die Kamera mit
            {
                Camera.x = Guy.ScreenPosition.x - static_cast<short>(rcGesamt.right / 2);
                Camera.y = Guy.ScreenPosition.y - static_cast<short>(rcGesamt.bottom / 2);
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

    bool LineIntersect(Coordinate LineStartPos, Coordinate Pos, bool store)
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
            if (!Scape[ROUND(x)][ROUND(y)].Walkable) erg = true;
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
                if (Scape[ROUND(x)][ROUND(Nexty)].Walkable)
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
                    if (Scape[ROUND(Nextx)][ROUND(y)].Walkable)
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
            Bmp[i].IsAnimationRunning = false;
        }
    }

    void AbspannCalc()
    {
        short i;

        if (AbspannZustand == 0)
        {
            for (short k = 1; k < 10; k++)
            {
                if (CreditsList[AbspannNr][k].Picture == -1) break;
                if (!CreditsList[AbspannNr][k].IsRunning) continue;
                i = 150 / LastBild;
                Bmp[CreditsList[AbspannNr][k].Picture].targetRect.top -= i;

                if (Bmp[CreditsList[AbspannNr][k].Picture].targetRect.top < MAX_SCREEN_Y - 400)
                {
                    if ((!CreditsList[AbspannNr][k + 1].IsRunning) &&
                        (CreditsList[AbspannNr][k + 1].Picture != -1))
                        CreditsList[AbspannNr][k + 1].IsRunning = true;
                }
                if (Bmp[CreditsList[AbspannNr][k].Picture].targetRect.top < 0)
                {
                    CreditsList[AbspannNr][k].IsRunning = false;
                    Bmp[CreditsList[AbspannNr][k].Picture].targetRect.top = MAX_SCREEN_Y - Bmp[CreditsList[AbspannNr][k].Picture].Height / 2;
                    if (!CreditsList[AbspannNr][k + 1].IsRunning)
                    {
                        if (CreditsList[AbspannNr + 1][0].Picture != -1)
                        {
                            AbspannNr++;
                            CreditsList[AbspannNr][1].IsRunning = true;
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
            i = LastBild / Bmp[AbspannNr].Speed;
            if (i < 1) i = 1;
            if (Bild % i == 0)
            {
                Bmp[AbspannNr].AnimationPhase++;
                if (Bmp[AbspannNr].AnimationPhase >= Bmp[AbspannNr].AnimationPhaseCount)
                {
                    Bmp[AbspannNr].AnimationPhase = 0;
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
                j = Scape[x][y].Object;
                if ((j == -1) || (!Bmp[j].IsAnimationRunning)) continue;
                i = LastBild / Bmp[j].Speed;
                if (i < 1) i = 1;
                if (Bild % i == 0)
                {
                    if ((j < TREE_DOWN_1) || (j > TREE_DOWN_4) || // Die Baumfällenanimation nur ein mal abspielen
                        (Scape[x][y].AnimationPhase != Bmp[j].AnimationPhaseCount - 1))
                        Scape[x][y].AnimationPhase++;
                    if (Scape[x][y].AnimationPhase >= Bmp[j].AnimationPhaseCount)
                        Scape[x][y].AnimationPhase = 0;
                }
            }

        for (j = BUTTON_GRID; j <= BUTTON_DESTROY; j++)
        {
            if (!Bmp[j].IsAnimationRunning) continue;
            i = LastBild / Bmp[j].Speed;
            if (i < 1) i = 1;
            if (Bild % i == 0)
            {
                Bmp[j].AnimationPhase++;
                if (Bmp[j].AnimationPhase >= Bmp[j].AnimationPhaseCount) Bmp[j].AnimationPhase = 0;
            }
        }

        // Spielfigur

        // laufen
        if (((Guy.AnimationState >= GUY_LEFT) && (Guy.AnimationState <= GUY_BELOW)) ||
            ((Guy.AnimationState >= GUY_BOAT_LEFT) && (Guy.AnimationState <= GUY_BOAT_BELOW)) ||
            (Guy.AnimationState == GUY_SHIP) || (Guy.AnimationState == GUY_SWIM))
        {
            i = LastBild / Bmp[Guy.AnimationState].Speed;
            if (i < 1) i = 1;
            if (LastBild - Bmp[Guy.AnimationState].Speed < 0) loop = 2;
            else loop = 1;
            if (BootsFahrt) loop = loop * 2;
            for (short k = 0; k < loop; k++) if ((Bild % i == 0) && (Guy.IsActive)) CalcGuyKoor();
            return;
        }
        // sonstige Aktionen
        if ((Guy.AnimationState >= GUY_SEARCH) && (Guy.AnimationState <= GUY_SLINGSHOT) &&
            (Bmp[Guy.AnimationState].AnimationPhase != Bmp[Guy.AnimationState].AnimationPhaseCount))
        {
            i = LastBild / Bmp[Guy.AnimationState].Speed;
            if (i < 1) i = 1;
            if (Bild % i == 0)
            {
                Bmp[Guy.AnimationState].AnimationPhase++;
                if (Bmp[Guy.AnimationState].AnimationPhase >= Bmp[Guy.AnimationState].AnimationPhaseCount)
                {
                    Bmp[Guy.AnimationState].AnimationPhase = 0;
                    if (PapierText == -1) Guy.IsActive = false;
                }
            }
        }
    }
} // namespace Math
