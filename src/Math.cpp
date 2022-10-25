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

#define ROUND(s) ((short) ((short)(s-0.5) == (short) s ? s+1:s)) //Zahlen runden Macro

namespace Math {
float Schrittx, Schritty; // Zum Figur laufen lassen
Coordinate GuyPosScreenStart; // Absolute StartPosition bei einem Schritt (Für CalcGuyKoor)

void UpdateMousePosition(short Button, short Push, short xDiff, short yDiff)
{
//    char Text[1024], TextTmp[1024]; // Text für Infoleiste
    std::string Text;


    // Info anzeigen
    Coordinate Erg = Renderer::GetTile((MousePosition.x + Camera.x), (MousePosition.y + Camera.y)); // Die angeklickte Kachel

    if (Erg.x >= 0 && Erg.y >= 0 && Landscape[Erg.x][Erg.y].Discovered) {
        Text = GetLanguageString(45 + Landscape[Erg.x][Erg.y].Terrain);

        if ((Landscape[Erg.x][Erg.y].Object != Tiles::INVALID) && (Landscape[Erg.x][Erg.y].Object != Tiles::SEA_WAVES)) {
            Text += " ";
            Text += GetLanguageString(STRING_WITH);
            Text += " ";

            if ((Landscape[Erg.x][Erg.y].Object >= Tiles::TREE_1) && (Landscape[Erg.x][Erg.y].Object <= Tiles::TREE_4)) {
                Text += GetLanguageString(STRING_TREE);
            } else if ((Landscape[Erg.x][Erg.y].Object >= Tiles::RIVER_1) && (Landscape[Erg.x][Erg.y].Object <= Tiles::FLOODGATE_6)) {
                Text += GetLanguageString(FLUSSTEXT);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::BUSH) {
                Text += GetLanguageString(STRING_BUSH);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::TENT) {
                Text += GetLanguageString(STRING_TENT);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::FIELD) {
                Text += GetLanguageString(STRING_FIELD);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::BOAT) {
                Text += GetLanguageString(BOOTTEXT);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::PIPE) {
                Text += GetLanguageString(ROHRTEXT);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::SOS) {
                Text += GetLanguageString(SOSTEXT);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::HOUSE_1) {
                Text += GetLanguageString(HAUS1TEXT);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::HOUSE_2) {
                Text += GetLanguageString(STRING_TREE_AND_PLATFORM);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::HOUSE_3) {
                Text += GetLanguageString(HAUS3TEXT);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::TREE_BIG) {
                Text += GetLanguageString(STRING_BIG_TREE);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::BONFIRE) {
                Text += GetLanguageString(STRING_BONFIRE);
            } else if (Landscape[Erg.x][Erg.y].Object == Tiles::FIRE) {
                Text += GetLanguageString(STRING_SMOKE_SIGNAL);
            } else if ((Landscape[Erg.x][Erg.y].Object == Tiles::WRECK_1) || (Landscape[Erg.x][Erg.y].Object == Tiles::WRECK_2)) {
                Text += GetLanguageString(WRACKTEXT);
            }

            if ((Landscape[Erg.x][Erg.y].Object >= Tiles::FIELD) &&
                    (Landscape[Erg.x][Erg.y].Object <= Tiles::BONFIRE)) {
                // Baufortschrittanzeigen
                Text += " (";
                Text += std::to_string((Landscape[Erg.x][Erg.y].ConstructionActionStep * 100) / Bmp[Landscape[Erg.x][Erg.y].Object].RequiredActionCases);
                Text += "%)";
                // benötigte Rohstoffe anzeigen
                World::RawMaterialsDescriptionString(Erg.x, Erg.y, -1);
                Text += RohString;
            }
        }

        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(Text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);
    }

    // rechte Maustastescrollen
    if ((Button == 1) && (Push == 0)) {
        Camera.x += xDiff;
        Camera.y += yDiff;
        CursorTyp = Tiles::CURSOR_DIRECTION;
    }

    // Wenn Maustaste gedrückt wird
    if ((Button == 0) && (Push == 1)) {
        if ((Erg.x != -1) && (Erg.y != -1) &&
                (Landscape[Erg.x][Erg.y].Discovered) && (!Guy.IsActive) &&
                ((Erg.x != Guy.Pos.x) || (Erg.y != Guy.Pos.y)) &&
                (Erg.x > 0) && (Erg.x < MAX_TILES_X - 1) &&
                (Erg.y > 0) && (Erg.y < MAX_TILESY - 1)) {
            // Klicksound abspielen
            PlaySound(Sound::CLICK2, 100);

            if ((Erg.x == RouteDestination.x) && (Erg.y == RouteDestination.y)) {
                Routing::MarkRoute(false);
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.IsActive = true;
                RoutePunkt = -1;
                Steps = 0;
                Step = 0;
            } else {
                Routing::MarkRoute(false);
                RouteStart.x = Guy.Pos.x;
                RouteStart.y = Guy.Pos.y;
                RouteDestination.x = Erg.x;
                RouteDestination.y = Erg.y;

                if (Routing::FindTheWay()) {
                    Routing::MarkRoute(true);
                } else { // Wenn keine Route gefunden
                    RouteStart.x = -1;
                    RouteStart.y = -1;
                    RouteDestination.x = -1;
                    RouteDestination.y = -1;
//                    MessageBeep(MB_OK);
                }
            }
        } else {
            PlaySound(Sound::CLICK, 100);
        }
    }
}

void MouseInPanel(short Button, short Push)
{
    // wenn die Maus in der Minimap ist ->
    if ((InRect(MousePosition.x, MousePosition.y, rcKarte)) && (Button == 0) && (Push != -1)) {
        // Mauskoordinaten in Minimap
        short mx = MousePosition.x - static_cast<short>(rcKarte.left);
        short my = MousePosition.y - static_cast<short>(rcKarte.top);
        Camera.x = ((TILE_SIZE_X / 4) * (mx - my) + MAX_TILES_X * TILE_SIZE_X / 2)
                   - static_cast<short>((rcPlayingSurface.right - rcPlayingSurface.left) / 2);
        Camera.y = ((TILE_SIZE_X / 7) * (my + mx))
                   - static_cast<short>((rcPlayingSurface.bottom - rcPlayingSurface.top) / 2);
    } else if (InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_GRID].targetRect)) {
        if (Gitter) {
            Renderer::DrawText(GITTERAUS, TXTTEXTFELD, 2);
        } else {
            Renderer::DrawText(GITTERAN, TXTTEXTFELD, 2);
        }

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Gitter = !Gitter;
            World::Generate();
        }
    } else if (InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_ANIMATION].targetRect)) {
        if (LAnimation) {
            Renderer::DrawText(ANIMATIONAUS, TXTTEXTFELD, 2);
        } else {
            Renderer::DrawText(ANIMATIONAN, TXTTEXTFELD, 2);
        }

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            LAnimation = !LAnimation;
            World::Generate();
        }
    } else if (InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_SOUND].targetRect)) {
        if (s_SoundState == 1) {
            Renderer::DrawText(SOUNDAUS, TXTTEXTFELD, 2);
        } else if (s_SoundState == 0) {
            Renderer::DrawText(SOUNDAN, TXTTEXTFELD, 2);
        } else {
            Renderer::DrawText(KEINSOUND, TXTTEXTFELD, 2);
        }

        if ((Button == 0) && (Push == 1)) {
            if (s_SoundState == 1) {
                for (short i = 1; i < Sound::COUNT; i++) {
                    Sound::StopSound(i);
                }

                s_SoundState = 0;
            } else if (s_SoundState == 0) {
                s_SoundState = 1;
                PlaySound(Sound::CLICK2, 100);
            } else {
                PlaySound(Sound::CLICK, 100);
            }
        }
    } else if (InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_END].targetRect)) {
        Renderer::DrawText(BEENDEN, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_END].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;
            Guy.IsActive = false;
            Guy.CurrentAction = Action::QUIT;
        }
    } else if (InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_NEW].targetRect)) {
        Renderer::DrawText(NEU, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_NEW].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;
            Guy.IsActive = false;
            Guy.CurrentAction = Action::RESTART;
        }
    } else if (InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_NEW_DAY].targetRect)) {
        Renderer::DrawText(TAGNEU2, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_NEW_DAY].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;
            Guy.IsActive = false;
            Guy.CurrentAction = Action::DAY_RESTART;
        }
    } else if (InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_ACTION].targetRect)) {
        if (HauptMenue == Menu::ACTION) {
            Renderer::DrawText(MEAKTIONZU, TXTTEXTFELD, 2);
        } else {
            Renderer::DrawText(MEAKTIONAUF, TXTTEXTFELD, 2);
        }

        Bmp[Tiles::BUTTON_ACTION].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if (HauptMenue == Menu::ACTION) {
                HauptMenue = Menu::NONE;
            } else {
                HauptMenue = Menu::ACTION;
            }
        }
    } else if (InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_CONSTRUCT].targetRect) &&
               (Bmp[Tiles::BUTTON_CONSTRUCT].AnimationPhase != -1)) {
        if (HauptMenue == Menu::BUILD) {
            Renderer::DrawText(MEBAUENZU, TXTTEXTFELD, 2);
        } else {
            Renderer::DrawText(MEBAUENAUF, TXTTEXTFELD, 2);
        }

        Bmp[Tiles::BUTTON_CONSTRUCT].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if (HauptMenue == Menu::BUILD) {
                HauptMenue = Menu::NONE;
            } else {
                HauptMenue = Menu::BUILD;
            }
        }
    } else if (InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_INVENTORY].targetRect)) {
        if (HauptMenue == Menu::INVENTORY) {
            Renderer::DrawText(MEINVENTARZU, TXTTEXTFELD, 2);
        } else {
            Renderer::DrawText(MEINVENTARAUF, TXTTEXTFELD, 2);
        }

        Bmp[Tiles::BUTTON_INVENTORY].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if (HauptMenue == Menu::INVENTORY) {
                HauptMenue = Menu::NONE;
            } else {
                HauptMenue = Menu::INVENTORY;
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_CONTINUE].targetRect)) &&
               (Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1)) {
        Renderer::DrawText(WEITER, TXTTEXTFELD, 2);

        Bmp[Tiles::BUTTON_CONTINUE].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
            Routing::MarkRoute(false);
            RouteDestination.x = -1;
            RouteDestination.y = -1;
            Guy.OriginalPosition = Guy.ScreenPosition;
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);

            switch (Landscape[Guy.Pos.x][Guy.Pos.y].Object) {
            case Tiles::TENT:
                Guy.CurrentAction = Action::TENT;
                break;

            case Tiles::FIELD:
                Guy.CurrentAction = Action::FIELD;
                break;

            case Tiles::BOAT:
                Guy.CurrentAction = Action::BOAT;
                break;

            case Tiles::PIPE:
                Guy.CurrentAction = Action::PIPE;
                break;

            case Tiles::SOS:
                Guy.CurrentAction = Action::SOS_SIGN;
                break;

            case Tiles::HOUSE_1:
                Guy.CurrentAction = Action::HOUSE1;
                break;

            case Tiles::HOUSE_2:
                Guy.CurrentAction = Action::HOUSE2;
                break;

            case Tiles::HOUSE_3:
                Guy.CurrentAction = Action::HOUSE3;
                break;

            case Tiles::BONFIRE:
                Guy.CurrentAction = Action::FIREPLACE;
                break;
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_STOP].targetRect)) &&
               (Bmp[Tiles::BUTTON_STOP].AnimationPhase != -1)) {
        Renderer::DrawText(STOP, TXTTEXTFELD, 2);

        Bmp[Tiles::BUTTON_STOP].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;
            Guy.CurrentAction = Action::CANCEL;
            Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;
        }
    }

    else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_LAY_DOWN].targetRect)) &&
             (Bmp[Tiles::BUTTON_LAY_DOWN].AnimationPhase != -1)) {
        Renderer::DrawText(BEGINNABLEGEN, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_LAY_DOWN].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;

            if (Landscape[Guy.Pos.x][Guy.Pos.y].Terrain != 1) {
                Guy.CurrentAction = Action::UNDOCK;
            } else {
                Guy.CurrentAction = Action::DOCK;
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_SEARCH].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_SEARCH].AnimationPhase != -1)) {
        Renderer::DrawText(STRING_BEGIN_SEARCH, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_SEARCH].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;
            Guy.CurrentAction = Action::SEARCH;
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_EAT].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_EAT].AnimationPhase != -1)) {
        Renderer::DrawText(BEGINNESSEN, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_EAT].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;

            if (((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::BUSH) ||
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::FIELD)) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase == Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount - 1)) {
                Guy.CurrentAction = Action::EAT;
            } else if (((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::RIVER_1) &&
                        (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::FLOODGATE_6)) ||
                       ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::PIPE) &&
                        (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase == 1))) {
                Guy.CurrentAction = Action::DRINK;
            } else {
                PapierText = Renderer::DrawText(STRING_NOTHING_EAT_DRINK, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_SLEEP].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_SLEEP].AnimationPhase != -1)) {
        Renderer::DrawText(STRING_BEGIN_SLEEP, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_SLEEP].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if (Landscape[Guy.Pos.x][Guy.Pos.y].Terrain != 1) {
                Guy.ActionStep = 0;
                Guy.CurrentAction = Action::SLEEP;
            } else {
                PapierText = Renderer::DrawText(NICHTAUFWASSERSCHLAFEN, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_CHOP].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_CHOP].AnimationPhase != -1)) {
        Renderer::DrawText(BEGINNFAELLEN, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_CHOP].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;

            if (Guy.Inventory[Tiles::RAW_TREE_TRUNK] <= 10) {
                if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::TREE_1) &&
                        (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::TREE_4)) {
                    Guy.CurrentAction = Action::LOG;
                } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TREE_BIG) ||
                           ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::HOUSE_1) &&
                            (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::HOUSE_3))) {
                    PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
                } else {
                    PapierText = Renderer::DrawText(KEINBAUM, TXTPAPIER, 1);
                }
            } else {
                PapierText = Renderer::DrawText(ROHSTAMMZUVIEL, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_FISH].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_FISH].AnimationPhase != -1)) {
        Renderer::DrawText(STRING_BEGIN_FISHING, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_FISH].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;

            if (((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::RIVER_1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::FLOODGATE_6)) ||
                    (IsInBoat)) {
                Guy.CurrentAction = Action::FISH;
            } else {
                PapierText = Renderer::DrawText(STRING_NO_FISHINGWATER, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_IGNITE].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_IGNITE].AnimationPhase != -1)) {
        Renderer::DrawText(STRING_LIGHT_BONFIRE, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_IGNITE].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::BONFIRE) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
                Guy.CurrentAction = Action::LIGHT;
            } else {
                PapierText = Renderer::DrawText(STRING_ONLY_LIGHT_BONFIRE, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_LOOK_OUT].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_LOOK_OUT].AnimationPhase != -1)) {
        Renderer::DrawText(STRING_BEGIN_LOOK_OUT, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_LOOK_OUT].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;

            if (Landscape[Guy.Pos.x][Guy.Pos.y].Terrain != 1) {
                Guy.ActionStep = 0;
                Guy.CurrentAction = Action::LOOKOUT;
            } else {
                PapierText = Renderer::DrawText(WELLENZUHOCH, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_TREASURE].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_TREASURE].AnimationPhase != -1)) {
        Renderer::DrawText(STRING_BEGIN_TREASUREDIG, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_TREASURE].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Terrain != 1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Type == 0) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Object == -1)) {
                Guy.ActionStep = 0;
                Guy.CurrentAction = Action::TREASURE;
            } else {
                PapierText = Renderer::DrawText(GRABENBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_SLINGSHOT].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_SLINGSHOT].AnimationPhase != -1)) {
        Renderer::DrawText(BEGINNSCHLEUDER, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_SLINGSHOT].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Guy.ActionStep = 0;

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::TREE_1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::TREE_4)) {
                Guy.ActionStep = 0;
                Guy.CurrentAction = Action::SLINGSHOT;
            } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TREE_BIG) ||
                       ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::HOUSE_1) &&
                        (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::HOUSE_3))) {
                PapierText = Renderer::DrawText(BAUMZUGROSS, TXTPAPIER, 1);
            } else {
                PapierText = Renderer::DrawText(KEINVOGEL, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_TREASUREMAP].targetRect)) &&
               (HauptMenue == Menu::ACTION) && (Bmp[Tiles::BUTTON_TREASUREMAP].AnimationPhase != -1)) {
        Renderer::DrawText(STRING_BEGIN_TREASUREMAP, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_TREASUREMAP].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);
            Renderer::DrawSchatzkarte();
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_FARM].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_FARM].AnimationPhase != -1)) {
        std::string text = GetLanguageString(BEGINNFELD);
        World::RawMaterialsDescriptionString(-1, -1, Tiles::FIELD);
        text += RohString;
        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

        Bmp[Tiles::BUTTON_FARM].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == -1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Type == 0) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Terrain == 4)) {
                Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.CurrentAction = Action::FIELD;
            } else if ((Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1) && (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::FIELD)) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                Guy.CurrentAction = Action::FIELD;
            } else {
                PapierText = Renderer::DrawText(FELDBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_TENT].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_TENT].AnimationPhase != -1)) {
        std::string text = GetLanguageString(BEGINNZELT);
        World::RawMaterialsDescriptionString(-1, -1, Tiles::TENT);
        text += RohString;
        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

        Bmp[Tiles::BUTTON_TENT].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == -1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Type == 0) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Terrain != -1)) {
                Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.CurrentAction = Action::TENT;
            } else if ((Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1) &&
                       (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT)) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                Guy.CurrentAction = Action::TENT;
            } else {
                PapierText = Renderer::DrawText(ZELTBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_BOAT].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_BOAT].AnimationPhase != -1)) {
        std::string text = GetLanguageString(BEGINNBOOT);
        World::RawMaterialsDescriptionString(-1, -1, Tiles::BOAT);
        text += RohString;
        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

        Bmp[Tiles::BUTTON_BOAT].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == -1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Terrain == 2) &&
                    ((Landscape[Guy.Pos.x - 1][Guy.Pos.y].Terrain == 1) ||
                     (Landscape[Guy.Pos.x][Guy.Pos.y - 1].Terrain == 1) ||
                     (Landscape[Guy.Pos.x + 1][Guy.Pos.y].Terrain == 1) ||
                     (Landscape[Guy.Pos.x][Guy.Pos.y + 1].Terrain == 1))) {
                Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.CurrentAction = Action::BOAT;
            } else if ((Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1) &&
                       (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::BOAT)) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                Guy.CurrentAction = Action::BOAT;
            } else {
                PapierText = Renderer::DrawText(BOOTBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_PIPE].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_PIPE].AnimationPhase != -1)) {
        std::string text = GetLanguageString(BEGINNROHR);
        World::RawMaterialsDescriptionString(-1, -1, Tiles::PIPE);
        text += RohString;
        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

        Bmp[Tiles::BUTTON_PIPE].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == -1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Type == 0)) {
                Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.CurrentAction = Action::PIPE;
            } else if ((Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1) &&
                       (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::PIPE)) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                Guy.CurrentAction = Action::PIPE;
            } else {
                PapierText = Renderer::DrawText(ROHRBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_SOS].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_SOS].AnimationPhase != -1)) {
        std::string text = GetLanguageString(STRINGBEGIN_WRITE_SOS);
        World::RawMaterialsDescriptionString(-1, -1, Tiles::SOS);
        text += RohString;
        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

        Bmp[Tiles::BUTTON_SOS].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == -1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Type == 0)) {
                Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.CurrentAction = Action::SOS_SIGN;
            } else if ((Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1) &&
                       (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::SOS)) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                Guy.CurrentAction = Action::SOS_SIGN;
            } else {
                PapierText = Renderer::DrawText(SOSBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_HOUSE_1].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_HOUSE_1].AnimationPhase != -1)) {
        std::string text = GetLanguageString(BEGINNHAUS1);
        World::RawMaterialsDescriptionString(-1, -1, Tiles::HOUSE_1);
        text += RohString;
        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

        Bmp[Tiles::BUTTON_HOUSE_1].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::TREE_1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::TREE_4)) {
                PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
            } else if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TREE_BIG) {
                Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.CurrentAction = Action::HOUSE1;
            } else if ((Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1) &&
                       (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_1)) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                Guy.CurrentAction = Action::HOUSE1;
            } else {
                PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_HOUSE_2].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_HOUSE_2].AnimationPhase != -1)) {
        std::string text = GetLanguageString(BEGINNHAUS2);
        World::RawMaterialsDescriptionString(-1, -1, Tiles::HOUSE_2);
        text += RohString;
        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

        Bmp[Tiles::BUTTON_HOUSE_2].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::TREE_1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::TREE_4)) {
                PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
            } else if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TREE_BIG) {
                PapierText = Renderer::DrawText(NICHTOHNELEITER, TXTPAPIER, 1);
            } else if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_1) {
                Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.CurrentAction = Action::HOUSE2;
            } else if ((Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1) &&
                       (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_2)) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                Guy.CurrentAction = Action::HOUSE2;
            } else {
                PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_HOUSE_3].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_HOUSE_3].AnimationPhase != -1)) {
        std::string text = GetLanguageString(STRING_BUILD_TREEHOUSE);
        World::RawMaterialsDescriptionString(-1, -1, Tiles::HOUSE_3);
        text += RohString;
        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

        Bmp[Tiles::BUTTON_HOUSE_3].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::TREE_1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::TREE_4)) {
                PapierText = Renderer::DrawText(BAUMZUKLEIN, TXTPAPIER, 1);
            } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TREE_BIG) ||
                       (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_1)) {
                PapierText = Renderer::DrawText(NICHTOHNEPLATTFORM, TXTPAPIER, 1);
            } else if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_2) {
                Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.CurrentAction = Action::HOUSE3;
            } else if ((Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1) &&
                       (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3)) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                Guy.CurrentAction = Action::HOUSE3;
            } else {
                PapierText = Renderer::DrawText(GEGENDNICHT, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_FIRE].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_FIRE].AnimationPhase != -1)) {
        std::string text = GetLanguageString(STRING_BEGIN_BONFIRE);
        World::RawMaterialsDescriptionString(-1, -1, Tiles::BONFIRE);
        text += RohString;
        TextBereich[TXTTEXTFELD].HasText = true;
        Renderer::DrawString(text.c_str(), static_cast<short>(TextBereich[TXTTEXTFELD].textRect.left),
                             static_cast<short>(TextBereich[TXTTEXTFELD].textRect.top), 2);

        Bmp[Tiles::BUTTON_FIRE].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == -1) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Type == 0)) {
                Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.CurrentAction = Action::FIREPLACE;
            } else if ((Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase != -1) &&
                       (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::BONFIRE)) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = 0;
                Guy.OriginalPosition = Guy.ScreenPosition;
                Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x,
                                    Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y);
                Guy.CurrentAction = Action::FIREPLACE;
            } else {
                PapierText = Renderer::DrawText(FEUERSTELLENBEDINGUNGEN, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_DESTROY].targetRect)) &&
               (HauptMenue == Menu::BUILD) && (Bmp[Tiles::BUTTON_DESTROY].AnimationPhase != -1)) {
        Renderer::DrawText(STRING_BEGIN_DESTROY, TXTTEXTFELD, 2);
        Bmp[Tiles::BUTTON_DESTROY].IsAnimationRunning = true;

        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK2, 100);

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::FIELD) &&
                    (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::BONFIRE)) {
                Guy.ActionStep = 0;
                Guy.CurrentAction = Action::DESTROY;
            } else {
                PapierText = Renderer::DrawText(STRING_NOTHING_TO_DESTROY, TXTPAPIER, 1);
            }
        }
    } else if ((InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::INVENTORY_PAPER].targetRect)) && (HauptMenue == Menu::INVENTORY)) {
        for (short i = Tiles::RAW_TREE_BRANCH; i <= Tiles::RAW_SLINGSHOT; i++) {
            if (InRect(MousePosition.x, MousePosition.y, Bmp[i].targetRect) && (Guy.Inventory[i] > 0)) {
                if ((Button == 0) && (Push == 1)) {
                    if (TwoClicks == -1) {
                        CursorTyp = i;
                        TwoClicks = i;
                    } else {
                        World::OnRawMaterialsUsed(i);
                    }
                }

                switch (i) {
                case Tiles::RAW_TREE_BRANCH:
                    Renderer::DrawText(AST, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_STONE:
                    Renderer::DrawText(STEIN, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_AXE:
                    Renderer::DrawText(STRING_AXE, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_LEAF:
                    Renderer::DrawText(STRING_LEAF, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_TREE_TRUNK:
                    Renderer::DrawText(STAMM, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_HOE:
                    Renderer::DrawText(EGGE, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_LIANA:
                    Renderer::DrawText(STRING_LIANA, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_FISHING_POLE:
                    Renderer::DrawText(ANGEL, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_HAMMER:
                    Renderer::DrawText(HAMMERTEXT, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_TELESCOPE:
                    Renderer::DrawText(STRING_TELESCOPE, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_MATCH:
                    Renderer::DrawText(STRING_MATCHES, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_SHOVEL:
                    Renderer::DrawText(STRING_SHOVEL, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_MAP:
                    Renderer::DrawText(STRING_MAP, TXTTEXTFELD, 2);
                    break;

                case Tiles::RAW_SLINGSHOT:
                    Renderer::DrawText(STRING_SLINGSHOT, TXTTEXTFELD, 2);
                    break;
                }

                break;
            }
        }
    } else if (InRect(MousePosition.x, MousePosition.y, TextBereich[TXTTAGESZEIT].textRect)) {
        Renderer::DrawText(SOSPAET, TXTTEXTFELD, 2);
    } else if (InRect(MousePosition.x, MousePosition.y, TextBereich[TXTCHANCE].textRect)) {
        Renderer::DrawText(CHANCETEXT, TXTTEXTFELD, 2);
    } else { // TwoClicks löschen
        if ((Button == 0) && (Push == 1)) {
            PlaySound(Sound::CLICK, 100);
        }

        TwoClicks = -1;
    }
}

#define SIGN(p1x, p1y, p2x, p2y, p3x, p3y)  (((p1x) - (p3x)) * ((p2y) - (p3y)) - ((p2x) - (p3x)) * ((p1y) - (p3y)))

bool InTriangle(float x, float y, float x0, float y0, float x1, float y1, float x3, float y3)
{
    const float d1 = SIGN(x, y, x0, y0, x1, y1);
    const float d2 = SIGN(x, y, x1, y1, x3, y3);
    const float d3 = SIGN(x, y, x3, y3, x0, y0);

    return !(
        (d1 < 0 || d2 < 0 || d3 < 0) &&
        (d1 > 0 || d2 > 0 || d3 > 0)
        );
}

bool InRect(short x, short y, RECT rcRect)
{
    if ((x <= rcRect.right) && (x >= rcRect.left) &&
            (y <= rcRect.bottom) && (y >= rcRect.top)) {
        return true;
    }

    return false;
}

void CalcGuyKoor()
{
    if (Step >= Steps) {
        RoutePunkt++;

        if ((RoutePunkt >= (RouteLaenge > 1 ? 2 * (RouteLaenge - 1) : 1) ||
                ((Guy.CurrentAction == Action::CANCEL) && (RouteLaenge > 1)))) {
            if (RouteLaenge > 1) {
                Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;
            }

            Bmp[Guy.AnimationState].AnimationPhase = 0;
            Guy.IsActive = false;
            RouteDestination.x = -1;
            RouteDestination.y = -1;
            return;
        }

        Guy.Pos.x = Route[(RoutePunkt + 1) / 2].x;
        Guy.Pos.y = Route[(RoutePunkt + 1) / 2].y;
        World::UpdateDiscovered();

        if (IsInBoat) {
            World::AddTime(0, Landscape[Route[(RoutePunkt + 1) / 2].x][Route[(RoutePunkt + 1) / 2].y].RunningTime * 3);
        } else {
            World::AddTime(0, Landscape[Route[(RoutePunkt + 1) / 2].x][Route[(RoutePunkt + 1) / 2].y].RunningTime * 5);
        }

        World::AddResource(Resources::Food, -1);
        World::AddResource(Resources::Water, -1);

        if ((Guy.AnimationState == Tiles::GUY_SHIP) || (Guy.AnimationState == Tiles::GUY_SWIM)) {
            Guy.AnimationState -= 2;    // nichts machen
        } else if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_LEFT;
        } else {
            Guy.AnimationState = Tiles::GUY_LEFT;
        }

        if (RouteLaenge > 1) { // Bei normaler Routenabarbeitung die Richung Kachelmäßig rausfinden
            if (Route[RoutePunkt / 2].x > Route[RoutePunkt / 2 + 1].x) {
                Guy.AnimationState += 0;
            } else if (Route[RoutePunkt / 2].x < Route[RoutePunkt / 2 + 1].x) {
                Guy.AnimationState += 2;
            } else if (Route[RoutePunkt / 2].y < Route[RoutePunkt / 2 + 1].y) {
                Guy.AnimationState += 3;
            } else if (Route[RoutePunkt / 2].y > Route[RoutePunkt / 2 + 1].y) {
                Guy.AnimationState += 1;
            }
        } else {
            if ((RouteKoor[RoutePunkt].x > RouteKoor[RoutePunkt + 1].x) &&
                    (RouteKoor[RoutePunkt].y >= RouteKoor[RoutePunkt + 1].y)) {
                Guy.AnimationState += 0;
            } else if ((RouteKoor[RoutePunkt].x <= RouteKoor[RoutePunkt + 1].x) &&
                       (RouteKoor[RoutePunkt].y > RouteKoor[RoutePunkt + 1].y)) {
                Guy.AnimationState += 1;
            } else if ((RouteKoor[RoutePunkt].x < RouteKoor[RoutePunkt + 1].x) &&
                       (RouteKoor[RoutePunkt].y <= RouteKoor[RoutePunkt + 1].y)) {
                Guy.AnimationState += 2;
            } else if ((RouteKoor[RoutePunkt].x >= RouteKoor[RoutePunkt + 1].x) &&
                       (RouteKoor[RoutePunkt].y < RouteKoor[RoutePunkt + 1].y)) {
                Guy.AnimationState += 3;
            }
        }

        // Differenz zwischen Ziel und Start
        short Dx = RouteKoor[RoutePunkt + 1].x - RouteKoor[RoutePunkt].x;
        short Dy = RouteKoor[RoutePunkt + 1].y - RouteKoor[RoutePunkt].y;

        GuyPosScreenStart.x = RouteKoor[RoutePunkt].x;
        GuyPosScreenStart.y = RouteKoor[RoutePunkt].y;
        Step = 0;

        if (abs(Dx) > abs(Dy)) {
            if (Dx > 0) {
                Schrittx = 1;
            } else {
                Schrittx = -1;
            }

            if (Dx == 0) {
                Schritty = 0;
            } else {
                Schritty = static_cast<float>(Dy) / static_cast<float>(Dx * Schrittx);
            }

            Steps = abs(Dx);
        } else {
            if (Dy > 0) {
                Schritty = 1;
            } else {
                Schritty = -1;
            }

            if (Dy == 0) {
                Schrittx = 0;
            } else {
                Schrittx = static_cast<float>(Dx) / static_cast<float>(Dy * Schritty);
            }

            Steps = abs(Dy);
        }
    }

    if (CurrentFrame % Landscape[Guy.Pos.x][Guy.Pos.y].RunningTime == 0) {
        Step++;
        short i;

        if (IsInBoat) {
            i = 4;
        } else {
            i = 2;
        }

        if (Step % i == 0) {
            Bmp[Guy.AnimationState].AnimationPhase++;

            if (Bmp[Guy.AnimationState].AnimationPhase >= Bmp[Guy.AnimationState].AnimationPhaseCount) {
                Bmp[Guy.AnimationState].AnimationPhase = 0;
            }
        }

        Guy.ScreenPosition.x = GuyPosScreenStart.x + ROUND(Step * Schrittx);
        Guy.ScreenPosition.y = GuyPosScreenStart.y + ROUND(Step * Schritty);

        if ((s_GameState == State::INTRO) || (s_GameState == State::RESCUED)) { // Beim Intro fährt die Kamera mit
            Camera.x = Guy.ScreenPosition.x - static_cast<short>(rcGesamt.right / 2);
            Camera.y = Guy.ScreenPosition.y - static_cast<short>(rcGesamt.bottom / 2);
        }
    }
}

void CalcKoor()
{
    // Bildschirmkoordinaten berechnen und speichern
    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            Landscape[x][y].xScreen = TILE_SIZE_X / 2 * MAX_TILES_X + 32 +
                                  x * TILE_SIZE_X / 2 - y * TILE_SIZE_Y / 2 +
                                  (-6 * y) + x; // seltsame Korrekturen
            Landscape[x][y].yScreen =
                x * TILE_SIZE_X / 2 + y * TILE_SIZE_Y / 2 - 16 * Landscape[x][y].Height +
                (-13 * x) + (-8 * y); // seltsame Korrekturen

            if ((x == 0) && (y == 0)) {
                ScapeGrenze.top = Landscape[x][y].yScreen;
            }

            if ((x == 0) && (y == MAX_TILESY - 1)) {
                ScapeGrenze.left = Landscape[x][y].xScreen;
            }

            if ((x == MAX_TILES_X - 1) && (y == MAX_TILESY - 1)) {
                ScapeGrenze.bottom = Landscape[x][y].yScreen + TILE_SIZE_Y;
            }

            if ((x == MAX_TILES_X - 1) && (y == 0)) {
                ScapeGrenze.right = Landscape[x][y].xScreen + TILE_SIZE_X;
            }
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

    if (abs(Dx) > abs(Dy)) {
        if (Dx > 0) {
            Sx = -1;
        } else {
            Sx = 1;
        }

        if (Dx == 0) {
            Sy = 0;
        } else {
            Sy = static_cast<float>(Dy) / static_cast<float>(Dx * Sx);
        }

        Steps = abs(Dx);
    } else {
        if (Dy > 0) {
            Sy = -1;
        } else {
            Sy = 1;
        }

        if (Dy == 0) {
            Sx = 0;
        } else {
            Sx = static_cast<float>(Dx) / static_cast<float>(Dy * Sy);
        }

        Steps = abs(Dy);
    }

    for (short i = 0; i < Steps; i++) {
        if (!Landscape[ROUND(x)][ROUND(y)].Walkable) {
            erg = true;
        }

        if ((store)) {
            Route[RouteLaenge].x = ROUND(x);
            Route[RouteLaenge].y = ROUND(y);
            RouteLaenge++;
        }

        float Nextx = x + Sx;
        float Nexty = y + Sy;

        if ((ROUND(y) != ROUND(Nexty)) && (ROUND(x) != ROUND(Nextx))) {
            if (Landscape[ROUND(x)][ROUND(Nexty)].Walkable) {
                if ((store)) {
                    Route[RouteLaenge].x = ROUND(x);
                    Route[RouteLaenge].y = ROUND(Nexty);
                    RouteLaenge++;
                }
            } else {
                if (Landscape[ROUND(Nextx)][ROUND(y)].Walkable) {
                    if ((store)) {
                        Route[RouteLaenge].x = ROUND(Nextx);
                        Route[RouteLaenge].y = ROUND(y);
                        RouteLaenge++;
                    }
                } else {
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
    if (rcRectdes.left < rcBereich.left) {
        rcRectsrc.left = rcRectsrc.left + rcBereich.left - rcRectdes.left;
        rcRectdes.left = rcBereich.left;
    }

    if (rcRectdes.top < rcBereich.top) {
        rcRectsrc.top = rcRectsrc.top + rcBereich.top - rcRectdes.top;
        rcRectdes.top = rcBereich.top;
    }

    if (rcRectdes.right > rcBereich.right) {
        rcRectsrc.right = rcRectsrc.right + rcBereich.right - rcRectdes.right;
        rcRectdes.right = rcBereich.right;
    }

    if (rcRectdes.bottom > rcBereich.bottom) {
        rcRectsrc.bottom = rcRectsrc.bottom + rcBereich.bottom - rcRectdes.bottom;
        rcRectdes.bottom = rcBereich.bottom;
    }
}

void ButtAniAus()
{
    for (short i = Tiles::BUTTON_GRID; i <= Tiles::BUTTON_DESTROY; i++) {
        Bmp[i].IsAnimationRunning = false;
    }
}

void AbspannCalc()
{
    short i;

    if (CreditsState == 0) {
        for (short k = 1; k < 10; k++) {
            if (CreditsList[CreditsNum][k].Picture == -1) {
                break;
            }

            if (!CreditsList[CreditsNum][k].IsRunning) {
                continue;
            }

            i = 150 / FPS;
            Bmp[CreditsList[CreditsNum][k].Picture].targetRect.top -= i;

            if (Bmp[CreditsList[CreditsNum][k].Picture].targetRect.top < MAX_SCREEN_Y - 400) {
                if ((!CreditsList[CreditsNum][k + 1].IsRunning) &&
                        (CreditsList[CreditsNum][k + 1].Picture != -1)) {
                    CreditsList[CreditsNum][k + 1].IsRunning = true;
                }
            }

            if (Bmp[CreditsList[CreditsNum][k].Picture].targetRect.top < 0) {
                CreditsList[CreditsNum][k].IsRunning = false;
                Bmp[CreditsList[CreditsNum][k].Picture].targetRect.top = MAX_SCREEN_Y - Bmp[CreditsList[CreditsNum][k].Picture].Height / 2;

                if (!CreditsList[CreditsNum][k + 1].IsRunning) {
                    if (CreditsList[CreditsNum + 1][0].Picture != -1) {
                        CreditsNum++;
                        CreditsList[CreditsNum][1].IsRunning = true;
                    } else {
                        CreditsNum = Tiles::GUY_LEFT;
                        CreditsState = 1;
                    }
                }
            }
        }
    } else if (CreditsState == 1) {
        i = FPS / Bmp[CreditsNum].Speed;

        if (i < 1) {
            i = 1;
        }

        if (CurrentFrame % i == 0) {
            Bmp[CreditsNum].AnimationPhase++;

            if (Bmp[CreditsNum].AnimationPhase >= Bmp[CreditsNum].AnimationPhaseCount) {
                Bmp[CreditsNum].AnimationPhase = 0;
                CreditsNum++;

                if (CreditsNum > Tiles::GUY_SLINGSHOT) {
                    CreditsNum = Tiles::GUY_LEFT;
                }
            }
        }
    }
}

void Animationen()
{
    short i, j, loop; // Zwischenspeicher


    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            j = Landscape[x][y].Object;

            if ((j == -1) || (!Bmp[j].IsAnimationRunning)) {
                continue;
            }

            i = FPS / Bmp[j].Speed;

            if (i < 1) {
                i = 1;
            }

            if (CurrentFrame % i == 0) {
                if ((j < Tiles::TREE_DOWN_1) || (j > Tiles::TREE_DOWN_4) || // Die Baumfällenanimation nur ein mal abspielen
                        (Landscape[x][y].AnimationPhase != Bmp[j].AnimationPhaseCount - 1)) {
                    Landscape[x][y].AnimationPhase++;
                }

                if (Landscape[x][y].AnimationPhase >= Bmp[j].AnimationPhaseCount) {
                    Landscape[x][y].AnimationPhase = 0;
                }
            }
        }

    for (j = Tiles::BUTTON_GRID; j <= Tiles::BUTTON_DESTROY; j++) {
        if (!Bmp[j].IsAnimationRunning) {
            continue;
        }

        i = FPS / Bmp[j].Speed;

        if (i < 1) {
            i = 1;
        }

        if (CurrentFrame % i == 0) {
            Bmp[j].AnimationPhase++;

            if (Bmp[j].AnimationPhase >= Bmp[j].AnimationPhaseCount) {
                Bmp[j].AnimationPhase = 0;
            }
        }
    }

    // Spielfigur

    // laufen
    if (((Guy.AnimationState >= Tiles::GUY_LEFT) && (Guy.AnimationState <= Tiles::GUY_BELOW)) ||
            ((Guy.AnimationState >= Tiles::GUY_BOAT_LEFT) && (Guy.AnimationState <= Tiles::GUY_BOAT_BELOW)) ||
            (Guy.AnimationState == Tiles::GUY_SHIP) || (Guy.AnimationState == Tiles::GUY_SWIM)) {
        i = FPS / Bmp[Guy.AnimationState].Speed;

        if (i < 1) {
            i = 1;
        }

        if (FPS - Bmp[Guy.AnimationState].Speed < 0) {
            loop = 2;
        } else {
            loop = 1;
        }

        if (IsInBoat) {
            loop = loop * 2;
        }

        for (short k = 0; k < loop; k++) if ((CurrentFrame % i == 0) && (Guy.IsActive)) {
                CalcGuyKoor();
            }

        return;
    }

    // sonstige Aktionen
    if ((Guy.AnimationState >= Tiles::GUY_SEARCH) && (Guy.AnimationState <= Tiles::GUY_SLINGSHOT) &&
            (Bmp[Guy.AnimationState].AnimationPhase != Bmp[Guy.AnimationState].AnimationPhaseCount)) {
        i = FPS / Bmp[Guy.AnimationState].Speed;

        if (i < 1) {
            i = 1;
        }

        if (CurrentFrame % i == 0) {
            Bmp[Guy.AnimationState].AnimationPhase++;

            if (Bmp[Guy.AnimationState].AnimationPhase >= Bmp[Guy.AnimationState].AnimationPhaseCount) {
                Bmp[Guy.AnimationState].AnimationPhase = 0;

                if (PapierText == -1) {
                    Guy.IsActive = false;
                }
            }
        }
    }
}
} // namespace Math
