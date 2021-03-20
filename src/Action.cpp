#include "Action.hpp"

#include "types.hpp"

#include "Game.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

namespace Actions {
void handler(Action::Type event)
{
    if (event != Action::NOTHING) {
        Routing::MarkRoute(false);
        RouteDestination.x = -1;
        RouteDestination.y = -1;
    }

    switch (event) {
    case Action::NOTHING:
        break;

    case Action::SEARCH:
        search();
        break;

    case Action::EAT:
        eat();
        break;

    case Action::DRINK:
        drink();
        break;

    case Action::LOG:
        log();
        break;

    case Action::FIELD:
        field();
        break;

    case Action::DAY_END:
        day_end();
        break;

    case Action::RESCUED:
        rescued();
        break;

    case Action::TENT:
        tent();
        break;

    case Action::SLEEP:
        sleep();
        break;

    case Action::CANCEL:
        cancel();
        break;

    case Action::FISH:
        fish();
        break;

    case Action::BOAT:
        boat();
        break;

    case Action::UNDOCK:
        undock();
        break;

    case Action::DOCK:
        dock();
        break;

    case Action::PIPE:
        pipe();
        break;

    case Action::DESTROY:
        destroy();
        break;

    case Action::SOS_SIGN:
        sos();
        break;

    case Action::HOUSE1:
        house1();
        break;

    case Action::HOUSE2:
        house2();
        break;

    case Action::HOUSE3:
        house3();
        break;

    case Action::FIREPLACE:
        fireplace();
        break;

    case Action::LIGHT:
        light();
        break;

    case Action::LOOKOUT:
        lookout();
        break;

    case Action::TREASURE:
        treasure();
        break;

    case Action::INTRO:
        intro();
        break;

    case Action::SLINGSHOT:
        slingshot();
        break;

    case Action::QUIT:
        quit();
        break;

    case Action::RESTART:
        restart();
        break;

    case Action::DAY_RESTART:
        day_restart();
        break;

    case Action::DEATH:
        death();
        break;
    }
}

void intro()
{
    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        // Intro Route herstellen
        Guy.IsActive = true;
        RoutePunkt = -1;
        Steps = 0;
        Step = 0;
        RouteStart.x = Guy.Pos.x;
        RouteStart.y = Guy.Pos.y;
        RouteDestination.y = Guy.Pos.y;

        for (short x = Guy.Pos.x; x < MAX_TILES_X; x++) { // Zielkoordinate für Introroute finden
            if (Landscape[x][Guy.Pos.y].Terrain != 1) {
                break;
            }

            RouteDestination.x = x - 1;
        }

        Routing::FindTheWay();
        break;

    case 2:
        Guy.ScreenPosition.y -= 10;
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_SHIP_DOWN;
        PlaySound(Sound::SPLAT, 100);
        PlaySound(Sound::CRASH, 100);
        break;

    case 3:
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::WRECK_1;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::WRECK_1].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::WRECK_1].targetRect.top);

        World::ToggleIsInBoat();
        Guy.Pos.x += 2;
        Guy.ScreenPosition.y += 10;
        Guy.AnimationState = Tiles::GUY_SWIM;
        Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].x) / 2),
                            ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].y +
                              Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y) / 2));
        break;

    case 4:
        StopSound(Sound::SWIM); // Sound hier sofort stoppen
        Guy.AnimationState = Tiles::GUY_LEFT;
        Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        break;

    case 5:
        Guy.OriginalPosition = Guy.ScreenPosition;
        s_GameState = State::GAME;
        Guy.CurrentAction = Action::NOTHING;
        PapierText = Renderer::DrawText(INTROTEXT, TXTPAPIER, 1);
        Game::SaveGame();
        break;
    }
}

void restart()
{
    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1: {
        Coordinate Erg = Renderer::GetTile(Guy.OriginalPosition.x, Guy.OriginalPosition.y);

        if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y)) {
            Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        } else if (RoutePunkt % 2 == 0) {
            Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y);    // Nur bis zur Mitte der aktuellen Kacheln laufen
        } else {
            Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
        }

        TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
        break;
    }

    case 2:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_WAITING;
        } else {
            Guy.AnimationState = Tiles::GUY_WAITING;
        }

        PapierText = Renderer::DrawText(NEUBEGINNEN, TXTPAPIER, 1);
        break;

    case 3:
        Guy.CurrentAction = Action::NOTHING;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_LEFT;
        } else {
            Guy.AnimationState = Tiles::GUY_LEFT;
        }

        if (Frage == 1) {
            Game::NewGame(true);
            return;
        }

        Frage = -1;
        break;
    }
}

void day_restart()
{
    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1: {
        Coordinate Erg = Renderer::GetTile(Guy.OriginalPosition.x, Guy.OriginalPosition.y);

        if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y)) {
            Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        } else if (RoutePunkt % 2 == 0) {
            Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y);    // Nur bis zur Mitte der aktuellen Kacheln laufen
        } else {
            Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
        }

        TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
        break;
    }

    case 2: {
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_WAITING;
        } else {
            Guy.AnimationState = Tiles::GUY_WAITING;
        }

        PapierText = Renderer::DrawText(TAGNEU, TXTPAPIER, 1);
        break;
    }

    case 3: {
        Guy.CurrentAction = Action::NOTHING;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_LEFT;
        } else {
            Guy.AnimationState = Tiles::GUY_LEFT;
        }

        if (Frage == 1) {
            Game::NewGame(false);
            return;
        }

        Frage = -1;
        break;
    }
    }
}

void quit()
{
    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1: {
        Coordinate Erg = Renderer::GetTile(Guy.OriginalPosition.x, Guy.OriginalPosition.y);

        if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y)) {
            Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        } else if (RoutePunkt % 2 == 0) {
            Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y);    // Nur bis zur Mitte der aktuellen Kacheln laufen
        } else {
            Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
        }

        TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
        break;
    }

    case 2:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_WAITING;
        } else {
            Guy.AnimationState = Tiles::GUY_WAITING;
        }

        PapierText = Renderer::DrawText(SPIELVERLASSEN, TXTPAPIER, 1);
        break;

    case 3:
        Guy.CurrentAction = Action::NOTHING;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_LEFT;
        } else {
            Guy.AnimationState = Tiles::GUY_LEFT;
        }

        if (Frage == 1) {
            if (Guy.ResourceAmount[Resources::Health] > 10) {
                Game::SaveGame();
            }

            s_GameState = State::OUTRO;
        }

        Frage = -1;
        break;
    }
}

void death()
{
    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_WAITING;
        } else {
            Guy.AnimationState = Tiles::GUY_WAITING;
        }

        PapierText = Renderer::DrawText(TOD, TXTPAPIER, 1);
        break;

    case 2:
        if (!IsInBoat) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_LAYING_TENT;
        }

        break;

    case 3:
        Guy.IsActive = true;
        Night = false;
        Renderer::Fade(100, 100, 100);

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_DEAD;
        } else {
            Guy.AnimationState = Tiles::GUY_DEAD;
        }

        break;

    case 4:
        Guy.IsActive = true;
        Night = true;
        Guy.AnimationState = Tiles::GUY_WAITING;
        PapierText = Renderer::DrawText(TAGNEU, TXTPAPIER, 1);
        break;

    case 5:
        Night = false;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_LEFT;
        } else {
            Guy.AnimationState = Tiles::GUY_LEFT;
        }

        Guy.CurrentAction = Action::NOTHING;

        if (Frage == 2) {
            s_GameState = State::OUTRO;
        } else {
            Game::NewGame(false);
        }

        Frage = -1;
        break;
    }
}

void cancel()
{
    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x = Guy.ScreenPosition.x;
        Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y = Guy.ScreenPosition.y;

        Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        break;

    case 2:
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void destroy()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition;    // Die Originalposition merken
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Width + 4,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Height);
        break;

    case 2:
    case 4:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_CHOP;
        World::AddResource(Resources::Water, -1);
        World::AddResource(Resources::Food, -1);
        World::AddTime(0, 5);
        break;

    case 3:
    case 5:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_BEAT;
        World::AddResource(Resources::Water, -1);
        World::AddResource(Resources::Food, -1);
        World::AddTime(0, 5);
        break;

    case 6: {
        if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::SOS) {
            Chance -= 0.1f;
        }

        short i = Landscape[Guy.Pos.x][Guy.Pos.y].Object; // Um sich kurz das Objekt zu merken

        if ((i >= Tiles::HOUSE_1) && (i <= Tiles::HOUSE_3)) {
            Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::INVALID;
        } else {
            Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::INVALID;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 0;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 0;
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = -1;
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = 0;

        if (i == Action::PIPE) {
            World::FillPipe();
        }

        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;
    }

    case 7:
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void search()
{
    Coordinate target;
    short i;

    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    while (true) {
        target.x = Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + rand() % TILE_SIZE_X;
        target.y = Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + rand() % TILE_SIZE_Y;
        Coordinate Erg = Renderer::GetTile(target.x, target.y);

        if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y)) {
            break;    // Wenn das gefundene Ziel in der Kachel, dann fertig
        }
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
    case 3:
    case 5:
    case 7:
        if (IsInBoat) {
            if (Guy.ActionStep == 1) {
                Guy.IsActive = true;
                Guy.ScreenPosition.y -= 2;
                Guy.AnimationState = Tiles::GUY_DIVING_1;
                PlaySound(Sound::SPLAT, 100);
            }
        } else {
            Routing::ShortRoute(target.x, target.y);
        }

        break;

    case 2:
    case 4:
    case 6:
    case 8:
        Guy.IsActive = true;

        if (IsInBoat) {
            if (Guy.ActionStep == 2) {
                Guy.ScreenPosition.y += 5;
            }

            Guy.AnimationState = Tiles::GUY_DIVING_2;
        } else {
            Guy.AnimationState = Tiles::GUY_SEARCH;
        }

        World::AddTime(0, 4);
        break;

    case 9:
        if (IsInBoat) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_DIVING_3;
            PlaySound(Sound::SPLAT, 100);
        }

        break;

    case 10:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 11:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_LEFT;
        }

        // Auf Strand und Fluss
        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Terrain == 2) || ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::RIVER_1) && (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::FLOODGATE_6))) {
            if (Guy.Inventory[Tiles::RAW_STONE] < 10) {
                PapierText = Renderer::DrawText(ROHSTEINGEFUNDEN, TXTPAPIER, 1);
                Guy.Inventory[Tiles::RAW_STONE] += 3;

                if (Guy.Inventory[Tiles::RAW_STONE] > 10) {
                    Guy.Inventory[Tiles::RAW_STONE] = 10;
                }
            } else {
                PapierText = Renderer::DrawText(ROHSTEINZUVIEL, TXTPAPIER, 1);
            }
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::BUSH) {
            i = rand() % 2;

            switch (i) {
            case 0:
                if (Guy.Inventory[Tiles::RAW_TREE_BRANCH] < 10) {
                    PapierText = Renderer::DrawText(ROHASTGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[Tiles::RAW_TREE_BRANCH]++;
                } else {
                    PapierText = Renderer::DrawText(ROHASTZUVIEL, TXTPAPIER, 1);
                }

                break;

            case 1:
                if (Guy.Inventory[Tiles::RAW_LEAF] < 10) {
                    PapierText = Renderer::DrawText(STRING_LEAF_FOUND, TXTPAPIER, 1);
                    Guy.Inventory[Tiles::RAW_LEAF]++;
                } else {
                    PapierText = Renderer::DrawText(STRING_ALREADY_HAVE_LEAF, TXTPAPIER, 1);
                }

                break;
            }
        } else if (((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::TREE_1) && (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::TREE_BIG)) ||
                   ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::HOUSE_1) && (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::HOUSE_3))) {
            i = rand() % 3;

            switch (i) {
            case 0:
                if (Guy.Inventory[Tiles::RAW_TREE_BRANCH] < 10) {
                    PapierText = Renderer::DrawText(ROHASTGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[Tiles::RAW_TREE_BRANCH]++;
                } else {
                    PapierText = Renderer::DrawText(ROHASTZUVIEL, TXTPAPIER, 1);
                }

                break;

            case 1:
                if (Guy.Inventory[Tiles::RAW_LEAF] < 10) {
                    PapierText = Renderer::DrawText(STRING_LEAF_FOUND, TXTPAPIER, 1);
                    Guy.Inventory[Tiles::RAW_LEAF]++;
                } else {
                    PapierText = Renderer::DrawText(STRING_ALREADY_HAVE_LEAF, TXTPAPIER, 1);
                }

                break;

            case 2:
                if (Guy.Inventory[Tiles::RAW_LIANA] < 10) {
                    PapierText = Renderer::DrawText(STRING_LIANA_FOUND, TXTPAPIER, 1);
                    Guy.Inventory[Tiles::RAW_LIANA]++;
                } else {
                    PapierText = Renderer::DrawText(STRING_LIANA_FULL, TXTPAPIER, 1);
                }

                break;
            }
        } else if (IsInBoat) {
            if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::WRECK_1) {
                if (Guy.Inventory[Tiles::RAW_TELESCOPE] == 0) {
                    PapierText = Renderer::DrawText(STRING_TELESCOPE_FOUND, TXTPAPIER, 1);
                    Guy.Inventory[Tiles::RAW_TELESCOPE] = 1;
                    Bmp[Tiles::BUTTON_LOOK_OUT].AnimationPhase = 0;
                    Guy.Inventory[Tiles::RAW_HAMMER] = 1;
                    Bmp[Tiles::BUTTON_HOUSE_1].AnimationPhase = 0;
                    Bmp[Tiles::BUTTON_HOUSE_2].AnimationPhase = 0;
                    Bmp[Tiles::BUTTON_HOUSE_3].AnimationPhase = 0;
                } else {
                    PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
                }
            } else if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::WRECK_2) {
                if (Guy.Inventory[Tiles::RAW_MAP] == 0) {
                    PapierText = Renderer::DrawText(KARTEGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[Tiles::RAW_MAP] = 1;
                    Bmp[Tiles::BUTTON_TREASUREMAP].AnimationPhase = 0;
                    Guy.Inventory[Tiles::RAW_SHOVEL] = 1;
                    Bmp[Tiles::BUTTON_TREASURE].AnimationPhase = 0;
                } else {
                    PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
                }
            } else {
                PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
            }
        } else {
            PapierText = Renderer::DrawText(NICHTSGEFUNDEN, TXTPAPIER, 1);
        }

        break;

    case 12:
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void eat()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Height + 2);
        break;

    case 2:
    case 3:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_EAT;
        World::AddResource(Resources::Food, 15);
        World::AddTime(0, 2);
        break;

    case 4:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 5:
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void slingshot()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2 - 14,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Height + 9);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_SLINGSHOT;
        Guy.ScreenPosition.x += 5;
        World::AddTime(0, 2);
        PlaySound(Sound::SLINGSHOT, 100);
        break;

    case 3:
        Guy.ScreenPosition.x -= 5;
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2 + 6,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Height + 2);
        break;

    case 4:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_SEARCH;
        World::AddResource(Resources::Food, 5);
        World::AddTime(0, 20);
        break;

    case 5:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 6:
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void drink()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Routing::ShortRoute(Guy.ScreenPosition.x - 4,
                            Guy.ScreenPosition.y - 2);
        break;

    case 2:
    case 3:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_DRINK;
        World::AddResource(Resources::Water, 30);
        World::AddTime(0, 3);
        break;

    case 4:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 5:
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void log()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2 + 9,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Height + 3);
        break;

    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_CHOP;
        World::AddResource(Resources::Water, -2);
        World::AddResource(Resources::Food, -2);
        World::AddTime(0, 10);
        break;

    case 7: {
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_WAITING;
        short i = Landscape[Guy.Pos.x][Guy.Pos.y].Object + (Tiles::TREE_DOWN_1 - Tiles::TREE_1);
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = i;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x -= 17;
        PlaySound(Sound::TIMBER, 100);
        break;
    }

    case 8:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 9:
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::INVALID;
        Guy.Inventory[Tiles::RAW_TREE_TRUNK]++;

        if (Guy.Inventory[Tiles::RAW_TREE_TRUNK] > 10) {
            Guy.Inventory[Tiles::RAW_TREE_TRUNK] = 10;
        }

        Guy.Inventory[Tiles::RAW_TREE_BRANCH] += 5;

        if (Guy.Inventory[Tiles::RAW_TREE_BRANCH] > 10) {
            Guy.Inventory[Tiles::RAW_TREE_BRANCH] = 10;
        }

        Guy.Inventory[Tiles::RAW_LEAF] += 5;

        if (Guy.Inventory[Tiles::RAW_LEAF] > 10) {
            Guy.Inventory[Tiles::RAW_LEAF] = 10;
        }

        Guy.Inventory[Tiles::RAW_LIANA] += 2;

        if (Guy.Inventory[Tiles::RAW_LIANA] > 10) {
            Guy.Inventory[Tiles::RAW_LIANA] = 10;
        }

        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void fish()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        switch (Landscape[Guy.Pos.x][Guy.Pos.y].Object) {
        case Tiles::RIVER_1:
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + 35,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
            break;

        case Tiles::RIVER_2:
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + 19,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
            break;

        case Tiles::RIVER_3:
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + 22,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + 20);
            break;

        case Tiles::RIVER_4:
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + 34,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + 23);
            break;

        case Tiles::RIVER_6:
        case Tiles::RIVER_7:
        case Tiles::RIVER_END_2:
        case Tiles::RIVER_START_2:
        case Tiles::FLOODGATE_2:
        case Tiles::FLOODGATE_3:
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + 34,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + 33);
            break;

        case Tiles::RIVER_5:
        case Tiles::RIVER_9:
        case Tiles::RIVER_END_1:
        case Tiles::RIVER_START_1:
        case Tiles::FLOODGATE_1:
        case Tiles::FLOODGATE_5:
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + 20,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + 33);
            break;

        case Tiles::RIVER_8:
        case Tiles::RIVER_END_4:
        case Tiles::RIVER_START_3:
        case Tiles::FLOODGATE_4:
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + 22,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
            break;

        case Tiles::RIVER_10:
        case Tiles::RIVER_END_3:
        case Tiles::RIVER_START_4:
        case Tiles::FLOODGATE_6:
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + 32,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
            break;
        }

        break;

    case 2:
        Guy.IsActive = true;
        PlaySound(Sound::FISH, 100);

        if (IsInBoat) {
            Guy.ScreenPosition.y -= 2;
            Guy.AnimationState = Tiles::GUY_BOAT_FISHING_1;
        }

        switch (Landscape[Guy.Pos.x][Guy.Pos.y].Object) {
        case Tiles::RIVER_1:
        case Tiles::RIVER_6:
        case Tiles::RIVER_7:
        case Tiles::RIVER_END_2:
        case Tiles::RIVER_START_2:
        case Tiles::FLOODGATE_2:
        case Tiles::FLOODGATE_3:
            Guy.AnimationState = Tiles::GUY_FISHING_LEFT_1;
            break;

        case Tiles::RIVER_2:
        case Tiles::RIVER_5:
        case Tiles::RIVER_9:
        case Tiles::RIVER_END_1:
        case Tiles::RIVER_START_1:
        case Tiles::FLOODGATE_1:
        case Tiles::FLOODGATE_5:
            Guy.AnimationState = Tiles::GUY_FISHING_ABOVE_1;
            break;

        case Tiles::RIVER_3:
        case Tiles::RIVER_8:
        case Tiles::RIVER_END_4:
        case Tiles::RIVER_START_3:
        case Tiles::FLOODGATE_4:
            Guy.AnimationState = Tiles::GUY_FISHING_RIGHT_1;
            break;

        case Tiles::RIVER_4:
        case Tiles::RIVER_10:
        case Tiles::RIVER_END_3:
        case Tiles::RIVER_START_4:
        case Tiles::FLOODGATE_6:
            Guy.AnimationState = Tiles::GUY_FISHING_BELOW_1;
            break;
        }

        break;

    case 3:
    case 4:
    case 5:
    case 6:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_FISHING_2;
        }

        switch (Landscape[Guy.Pos.x][Guy.Pos.y].Object) {
        case Tiles::RIVER_1:
        case Tiles::RIVER_6:
        case Tiles::RIVER_7:
        case Tiles::RIVER_END_2:
        case Tiles::RIVER_START_2:
        case Tiles::FLOODGATE_2:
        case Tiles::FLOODGATE_3:
            Guy.AnimationState = Tiles::GUY_FISHING_LEFT_2;
            break;

        case Tiles::RIVER_2:
        case Tiles::RIVER_5:
        case Tiles::RIVER_9:
        case Tiles::RIVER_END_1:
        case Tiles::RIVER_START_1:
        case Tiles::FLOODGATE_1:
        case Tiles::FLOODGATE_5:
            Guy.AnimationState = Tiles::GUY_FISHING_ABOVE_2;
            break;

        case Tiles::RIVER_3:
        case Tiles::RIVER_8:
        case Tiles::RIVER_END_4:
        case Tiles::RIVER_START_3:
        case Tiles::FLOODGATE_4:
            Guy.AnimationState = Tiles::GUY_FISHING_RIGHT_2;
            break;

        case Tiles::RIVER_4:
        case Tiles::RIVER_10:
        case Tiles::RIVER_END_3:
        case Tiles::RIVER_START_4:
        case Tiles::FLOODGATE_6:
            Guy.AnimationState = Tiles::GUY_FISHING_BELOW_2;
            break;
        }

        Guy.ResourceAmount[Resources::Health] += 2;
        World::AddTime(0, 20);
        break;

    case 7:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = Tiles::GUY_BOAT_FISHING_3;
        }

        switch (Landscape[Guy.Pos.x][Guy.Pos.y].Object) {
        case Tiles::RIVER_1:
        case Tiles::RIVER_6:
        case Tiles::RIVER_7:
        case Tiles::RIVER_END_2:
        case Tiles::RIVER_START_2:
        case Tiles::FLOODGATE_2:
        case Tiles::FLOODGATE_3:
            Guy.AnimationState = Tiles::GUY_FISHING_LEFT_3;
            break;

        case Tiles::RIVER_2:
        case Tiles::RIVER_5:
        case Tiles::RIVER_9:
        case Tiles::RIVER_END_1:
        case Tiles::RIVER_START_1:
        case Tiles::FLOODGATE_1:
        case Tiles::FLOODGATE_5:
            Guy.AnimationState = Tiles::GUY_FISHING_ABOVE_3;
            break;

        case Tiles::RIVER_3:
        case Tiles::RIVER_8:
        case Tiles::RIVER_END_4:
        case Tiles::RIVER_START_3:
        case Tiles::FLOODGATE_4:
            Guy.AnimationState = Tiles::GUY_FISHING_RIGHT_3;
            break;

        case Tiles::RIVER_4:
        case Tiles::RIVER_10:
        case Tiles::RIVER_END_3:
        case Tiles::RIVER_START_4:
        case Tiles::FLOODGATE_6:
            Guy.AnimationState = Tiles::GUY_FISHING_BELOW_3;
            break;
        }

        break;

    case 8:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 9:
        Guy.ResourceAmount[Resources::Food] += 20;
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void light()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2 - 10,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Height + 1);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_SET_FIRE;
        Guy.ScreenPosition.x += 5;
        World::AddTime(0, 1);
        break;

    case 3:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_WAITING;
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::FIRE;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::FIRE].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::FIRE].targetRect.top);
        Chance += 2 + 2 * Landscape[Guy.Pos.x][Guy.Pos.y].Height;
        World::AddTime(0, 2);
        Guy.ScreenPosition.x -= 5;
        break;

    case 4:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 5:
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void lookout()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_LOOK_OUT;
        World::AddTime(0, 40);
        Chance += 1 + Landscape[Guy.Pos.x][Guy.Pos.y].Height;
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_WAITING;
        World::AddTime(0, 40);
        break;

    case 3:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_LOOK_OUT;
        World::AddTime(0, 40);
        break;

    case 4:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 5:
        Chance -= 1 + Landscape[Guy.Pos.x][Guy.Pos.y].Height;
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void treasure()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition;
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Guy.ScreenPosition.x -= 5;
        Guy.ScreenPosition.y += 1;
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_SHOVEL;
        break;

    case 2:
        World::AddTime(0, 20);
        World::AddResource(Resources::Water, -10);
        World::AddResource(Resources::Food, -10);
        Guy.ScreenPosition.x += 5;
        Guy.ScreenPosition.y -= 1;
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);

        if (((Guy.Pos.x == SchatzPos.x) && (Guy.Pos.y == SchatzPos.y)) &&
                (!SchatzGef)) {
            PapierText = Renderer::DrawText(SCHATZGEFUNDEN, TXTPAPIER, 1);
            Guy.Inventory[Tiles::RAW_MATCH] = 1;
            Bmp[Tiles::BUTTON_IGNITE].AnimationPhase = 0;
            SchatzGef = true;
        } else {
            PapierText = Renderer::DrawText(KEINSCHATZ, TXTPAPIER, 1);
        }

        break;

    case 3:
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void field()
{
    if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken

        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[Tiles::FIELD].RequiredRawMaterials[i];
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::FIELD;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::FIELD].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::FIELD].targetRect.top);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Tiles::FIELD].AnimationPhaseCount;
    }

    Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep++;

    if (!World::CheckRawMaterials()) {
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep--;
        return;
    }

    switch (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 22,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 23);
        break;

    case 4:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 4;
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 25,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 21);
        World::AddResource(Resources::Water, -2);
        World::AddResource(Resources::Food, -2);
        World::AddTime(0, 30);
        break;

    case 7:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 5;
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 28,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 19);
        World::AddResource(Resources::Water, -2);
        World::AddResource(Resources::Food, -2);
        World::AddTime(0, 30);
        break;

    case 10:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 6;
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 31,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 17);
        World::AddResource(Resources::Water, -2);
        World::AddResource(Resources::Food, -2);
        World::AddTime(0, 30);
        break;

    case 13:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 7;
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 34,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 15);
        World::AddResource(Resources::Water, -2);
        World::AddResource(Resources::Food, -2);
        World::AddTime(0, 30);
        break;

    case 16:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 8;
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 36,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 13);
        World::AddResource(Resources::Water, -2);
        World::AddResource(Resources::Food, -2);
        World::AddTime(0, 30);
        break;

    case 2:
    case 3:
    case 5:
    case 6:
    case 8:
    case 9:
    case 11:
    case 12:
    case 14:
    case 15:
    case 17:
    case 18:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_FARMING;
        break;

    case 19:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::FIELD;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::FIELD].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::FIELD].targetRect.top);
        break;

    case 20:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[Tiles::FIELD].First) {
            PapierText = Renderer::DrawText(FELDHILFE, TXTPAPIER, 1);
            Bmp[Tiles::FIELD].First = false;
        }

        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void day_end()
{
    Coordinate Erg;

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Renderer::Fade(100, 90, 90);
        Hours = 12;
        Minutes = 0;
        TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

        if ((Guy.AnimationState == Tiles::GUY_SLEEPING_TENT) || (Guy.AnimationState == Tiles::GUY_SLEEPING) ||
                (Guy.AnimationState == Tiles::GUY_SLEEP_HOUSE) || (IsInBoat)) {
            break;
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x = Guy.ScreenPosition.x;
        Landscape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y = Guy.ScreenPosition.y;
        Erg = Renderer::GetTile(Guy.OriginalPosition.x, Guy.OriginalPosition.y);

        if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y)) {
            Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        } else if (RoutePunkt % 2 == 0) {
            Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y);    // Nur bis zur Mitte der aktuellen Kacheln laufen
        } else {
            Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
        }

        break;

    case 2:
        Renderer::Fade(95, 80, 80);
        Hours = 12;
        Minutes = 0;

        if ((Guy.AnimationState == Tiles::GUY_SLEEPING_TENT) || (Guy.AnimationState == Tiles::GUY_SLEEPING) ||
                (Guy.AnimationState == Tiles::GUY_SLEEP_HOUSE) || (IsInBoat)) {
            break;
        }

        // Wohnbare Objekte in der Umgebung suchen
        Erg.x = -1;
        Erg.y = -1;

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) || (Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3)) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y;
        } else if (Landscape[Guy.Pos.x - 1][Guy.Pos.y].Object == Tiles::HOUSE_3) {
            Erg.x = Guy.Pos.x - 1;
            Erg.y = Guy.Pos.y;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y - 1].Object == Tiles::HOUSE_3) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y - 1;
        } else if (Landscape[Guy.Pos.x + 1][Guy.Pos.y].Object == Tiles::HOUSE_3) {
            Erg.x = Guy.Pos.x + 1;
            Erg.y = Guy.Pos.y;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y + 1].Object == Tiles::HOUSE_3) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y + 1;
        } else if (Landscape[Guy.Pos.x - 1][Guy.Pos.y].Object == Tiles::TENT) {
            Erg.x = Guy.Pos.x - 1;
            Erg.y = Guy.Pos.y;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y - 1].Object == Tiles::TENT) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y - 1;
        } else if (Landscape[Guy.Pos.x + 1][Guy.Pos.y].Object == Tiles::TENT) {
            Erg.x = Guy.Pos.x + 1;
            Erg.y = Guy.Pos.y;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y + 1].Object == Tiles::TENT) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y + 1;
        }

        if ((Erg.x != -1) && (Erg.y != -1)) {
            Guy.Pos.x = Erg.x;
            Guy.Pos.y = Erg.y;

            if ((Landscape[Erg.x][Erg.y].Object == Tiles::TENT) &&
                    (Landscape[Erg.x][Erg.y].AnimationPhase < Bmp[Landscape[Erg.x][Erg.y].Object].AnimationPhaseCount))
                Routing::ShortRoute(Landscape[Erg.x][Erg.y].xScreen + Landscape[Erg.x][Erg.y].ObjectPosOffset.x + 3,
                                    Landscape[Erg.x][Erg.y].yScreen + Landscape[Erg.x][Erg.y].ObjectPosOffset.y + 20);
            else if ((Landscape[Erg.x][Erg.y].Object == Tiles::HOUSE_3) &&
                     (Landscape[Erg.x][Erg.y].AnimationPhase < Bmp[Landscape[Erg.x][Erg.y].Object].AnimationPhaseCount))
                Routing::ShortRoute(Landscape[Erg.x][Erg.y].xScreen + Landscape[Erg.x][Erg.y].ObjectPosOffset.x +
                                    Bmp[Tiles::TREE_BIG].Width / 2,
                                    Landscape[Erg.x][Erg.y].yScreen + Landscape[Erg.x][Erg.y].ObjectPosOffset.y +
                                    Bmp[Tiles::TREE_BIG].Height + 1);
        }

        break;

    case 3:
        Renderer::Fade(90, 70, 70);
        Hours = 12;
        Minutes = 0;

        if ((Guy.AnimationState == Tiles::GUY_SLEEPING_TENT) || (Guy.AnimationState == Tiles::GUY_SLEEPING) ||
                (Guy.AnimationState == Tiles::GUY_SLEEP_HOUSE) || (IsInBoat)) {
            break;
        }

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_CLIMBING_1;
        }

        break;

    case 4:
        Renderer::Fade(70, 60, 60);
        Hours = 12;
        Minutes = 0;

        if ((Guy.AnimationState == Tiles::GUY_SLEEPING_TENT) || (Guy.AnimationState == Tiles::GUY_SLEEPING) ||
                (Guy.AnimationState == Tiles::GUY_SLEEP_HOUSE) || (IsInBoat)) {
            break;
        }

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_ENTER_TENT;
        } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                   (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_ENTER_HOUSE;
        } else {
            Guy.ScreenPosition.x += 3;
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_LAYING_TENT;
        }

        break;

    case 5:
        Renderer::Fade(55, 50, 55);
        Hours = 12;
        Minutes = 0;

        if (IsInBoat) {
            break;
        }

        Guy.IsActive = true;

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            if (Guy.AnimationState != Tiles::GUY_SLEEPING_TENT) {
                Guy.ScreenPosition.x += 4;
            }

            Guy.AnimationState = Tiles::GUY_SLEEPING_TENT;
        } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                   (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            if (Guy.AnimationState != Tiles::GUY_SLEEP_HOUSE) {
                Guy.ScreenPosition.x += 14;
            }

            Guy.AnimationState = Tiles::GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = Tiles::GUY_SLEEPING;
        }

        break;

    case 6:
        Renderer::Fade(25, 25, 35);
        Hours = 12;
        Minutes = 0;

        if (IsInBoat) {
            break;
        }

        Guy.IsActive = true;

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = Tiles::GUY_SLEEPING_TENT;
        } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                   (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = Tiles::GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = Tiles::GUY_SLEEPING;
        }

        break;

    case 7:
        Renderer::Fade(0, 0, 0);// Don't get confused as the picture in Show () will be painted over in black
        Night = true;
        Hours = 12;
        Minutes = 0;
        PlaySound(Sound::WOLF, 100);

        // Falsche Objekte Löschen
        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::TREE_DOWN_1) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::TREE_DOWN_4)) {
            Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::INVALID;
            Guy.Inventory[Tiles::RAW_TREE_TRUNK]++;

            if (Guy.Inventory[Tiles::RAW_TREE_TRUNK] > 10) {
                Guy.Inventory[Tiles::RAW_TREE_TRUNK] = 10;
            }
        }

        // Je nach Schlafort Zustand verändern
        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            World::AddResource(Resources::Health, -5);

            if (Guy.ResourceAmount[Resources::Health] <= 0) {
                Guy.IsActive = true;
                PapierText = Renderer::DrawText(TAGENDE5, TXTPAPIER, 1);
                Guy.ActionStep = 2;
                Guy.CurrentAction = Action::DEATH;
                Hours = 0;
                Minutes = 0;
            } else {
                Guy.IsActive = true;
                PapierText = Renderer::DrawText(TAGENDE2, TXTPAPIER, 1);
            }
        } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                   (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            World::AddResource(Resources::Health, +20);
            Guy.IsActive = true;
            PapierText = Renderer::DrawText(TAGENDE4, TXTPAPIER, 1);
        } else if (IsInBoat) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_BOAT_WAITING;
            PapierText = Renderer::DrawText(TAGENDE3, TXTPAPIER, 1);
            Guy.ActionStep = 2;
            Guy.CurrentAction = Action::DEATH;
            Hours = 0;
            Minutes = 0;
        } else {
            World::AddResource(Resources::Health, -20);

            if (Guy.ResourceAmount[Resources::Health] <= 0) {
                Guy.IsActive = true;
                PapierText = Renderer::DrawText(TAGENDE5, TXTPAPIER, 1);
                Guy.ActionStep = 2;
                Guy.CurrentAction = Action::DEATH;
                Hours = 0;
                Minutes = 0;
            } else {
                Guy.IsActive = true;
                PapierText = Renderer::DrawText(TAGENDE1, TXTPAPIER, 1);
            }
        }

        break;

    case 8:
        Renderer::Fade(20, 20, 30);
        Night = false;
        Tag++;
        Hours = 0;
        Minutes = 0;
        // if (BootsFahrt) Game::NeuesSpiel(true); // Später hier tot!!

        Guy.IsActive = true;

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = Tiles::GUY_SLEEPING_TENT;
        } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                   (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = Tiles::GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = Tiles::GUY_SLEEPING;
        }

        break;

    case 9:
        Renderer::Fade(40, 40, 40);
        Hours = 0;
        Minutes = 0;

        Hours = 0;
        Minutes = 0;
        Guy.IsActive = true;

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = Tiles::GUY_SLEEPING_TENT;
        } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                   (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = Tiles::GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = Tiles::GUY_SLEEPING;
        }

        break;

    case 10:
        Renderer::Fade(70, 60, 60);
        Hours = 0;
        Minutes = 0;
        StopSound(Sound::SNORE);
        Guy.IsActive = true;

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.ScreenPosition.x -= 14;
            Guy.AnimationState = Tiles::GUY_EXIT_HOUSE;
        } else {
            Guy.AnimationState = Tiles::GUY_STAND_UP;
        }

        break;

    case 11:
        Renderer::Fade(90, 80, 80);
        Hours = 0;
        Minutes = 0;

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_CLIMBING_2;
        }

        break;

    case 12:
        Renderer::Fade(100, 100, 100);
        Hours = 0;
        Minutes = 0;
        Guy.AnimationState = Tiles::GUY_LEFT;
        Guy.CurrentAction = Action::NOTHING;

        if (Guy.ResourceAmount[Resources::Health] > 10) {
            Game::SaveGame();
        }

        break;
    }
}

void rescued()
{
    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1: {
        Coordinate Erg = Renderer::GetTile(Guy.OriginalPosition.x, Guy.OriginalPosition.y);

        if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y)) {
            Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        } else if (RoutePunkt % 2 == 0) {
            Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y);    //Nur bis zur Mitte der aktuellen Kacheln laufen
        } else {
            Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
        }

        TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
        break;
    }

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_WAITING;
        PapierText = Renderer::DrawText(GERETTET, TXTPAPIER, 1);
        break;

    case 3:
        if (Frage == 2) {
            Guy.CurrentAction = Action::NOTHING;
            Frage = -1;
            break;
        }

        s_GameState = State::RESCUED;
        Frage = -1;
        break;

    case 4:
        // Route herstellen
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_LEFT;
        RoutePunkt = -1;
        Steps = 0;
        Step = 0;
        RouteStart.x = Guy.Pos.x;
        RouteStart.y = Guy.Pos.y;
        RouteDestination.y = Guy.Pos.y;

        for (short x = MAX_TILES_X - 1; x > 1; x--) { // Position des Rettungsschiffs festlegen
            if (Landscape[x][Guy.Pos.y].Terrain != 1) {
                break;
            }

            RouteDestination.x = x + 1;
        }

        // Schiff hinbauen
        Landscape[RouteDestination.x][RouteDestination.y].AnimationPhase = 0;
        Landscape[RouteDestination.x][RouteDestination.y].Object = Tiles::GUY_SHIP;
        Landscape[RouteDestination.x][RouteDestination.y].ObjectPosOffset.x = 10;
        Landscape[RouteDestination.x][RouteDestination.y].ObjectPosOffset.y = 10;
        RouteDestination.x -= 2;
        Routing::FindTheWay();
        Guy.AnimationState = Tiles::GUY_LEFT;
        break;

    case 5:
        Guy.AnimationState = Tiles::GUY_LEFT;
        Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x +
                              Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].x) / 2),
                            ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].y +
                              Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        break;

    case 6:
        Guy.Pos.x += 2;
        Guy.AnimationState = Tiles::GUY_SWIM;
        Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        break;

    case 7:
        Guy.ScreenPosition.y -= 10;

        if (!IsInBoat) {
            World::ToggleIsInBoat();
        }

        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_SHIP;
        RoutePunkt = -1;
        Steps = 0;
        Step = 0;
        RouteStart.x = Guy.Pos.x;
        RouteStart.y = Guy.Pos.y;
        RouteDestination.y = Guy.Pos.y;
        RouteDestination.x = MAX_TILES_X - 2;
        Routing::FindTheWay();
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::SEA_WAVES].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::SEA_WAVES].targetRect.top);
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::SEA_WAVES;
        break;

    case 8:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_SHIP;
        break;

    case 9:
        Guy.CurrentAction = Action::NOTHING;
        Guy.AnimationState = Tiles::GUY_LEFT;
        s_GameState = State::OUTRO;
        break;
    }
}

void tent()
{
    if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::TENT;
        printf("drawing tent: %d\n", Landscape[Guy.Pos.x][Guy.Pos.y].Object);

        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[Tiles::TENT].RequiredRawMaterials[i];
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::TENT].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::TENT].targetRect.top);
    }

    Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep++;

    if (!World::CheckRawMaterials()) {
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep--;
        return;
    }

    switch (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 22,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 12);
        break;

    case 2:
    case 3:
    case 12:
    case 13:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_TIE_DOWN;
        World::AddResource(Resources::Water, -2);
        World::AddResource(Resources::Food, -2);
        World::AddTime(0, 15);
        break;

    case 4:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 2;
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 31,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 5:
        Routing::ShortRoute(Guy.OriginalPosition.x,
                            Guy.OriginalPosition.y);
        break;

    case 6:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 3,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);

        break;

    case 7:
    case 8:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_TIE_UP;
        World::AddResource(Resources::Water, -2);
        World::AddResource(Resources::Food, -2);
        World::AddTime(0, 15);
        break;

    case 9:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 3;
        Routing::ShortRoute(Guy.OriginalPosition.x,
                            Guy.OriginalPosition.y);
        break;

    case 10:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 31,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 11:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 22,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 12);
        break;

    case 14:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 31,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 15:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 16:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[Tiles::TENT].First) {
            PapierText = Renderer::DrawText(ZELTHILFE, TXTPAPIER, 1);
            Bmp[Tiles::TENT].First = false;
        }

        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void boat()
{
    if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::BOAT;

        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[Tiles::BOAT].RequiredRawMaterials[i];
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Tiles::BOAT].AnimationPhaseCount;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::BOAT].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::BOAT].targetRect.top);
    }

    Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep++;

    if (!World::CheckRawMaterials()) {
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep--;
        return;
    }

    switch (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 30,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 21);
        break;

    case 2:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 29,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 3:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 28,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 19);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::BOAT].AnimationPhaseCount + 1);
        break;

    case 4:
    case 5:
    case 6:
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
    case 14:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_BEAT;
        World::AddResource(Resources::Water, -2);
        World::AddResource(Resources::Food, -2);
        World::AddTime(0, 15);
        break;

    case 7:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 22,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 16);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::BOAT].AnimationPhaseCount + 2);
        break;

    case 11:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 14,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 11);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::BOAT].AnimationPhaseCount + 3);
        break;

    case 15:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 16:
        if (Landscape[Guy.Pos.x - 1][Guy.Pos.y].Terrain == 1) {
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 0;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 10;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y - 1].Terrain == 1) {
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 1;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 25;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 10;
        } else if (Landscape[Guy.Pos.x + 1][Guy.Pos.y].Terrain == 1) {
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 30;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 27;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y + 1].Terrain == 1) {
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 1;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 0;
            Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 28;
        }

        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[Tiles::BOAT].First) {
            PapierText = Renderer::DrawText(BOOTHILFE, TXTPAPIER, 1);
            Bmp[Tiles::BOAT].First = false;
        }

        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void pipe()
{
    if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::PIPE;

        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[Tiles::PIPE].RequiredRawMaterials[i];
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Tiles::PIPE].AnimationPhaseCount;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::PIPE].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::PIPE].targetRect.top);
    }

    Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep++;

    if (!World::CheckRawMaterials()) {
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep--;
        return;
    }

    switch (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 30,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 21);
        break;

    case 2:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 29,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 3:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 28,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 15);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::PIPE].AnimationPhaseCount + 1);
        break;

    case 4:
    case 5:
    case 6:
    case 11:
    case 12:
    case 13:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_BEAT;
        World::AddResource(Resources::Water, -1);
        World::AddResource(Resources::Food, -1);
        World::AddTime(0, 5);
        break;

    case 7:
    case 8:
    case 9:
    case 14:
    case 15:
    case 16:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_CHOP;
        World::AddResource(Resources::Water, -1);
        World::AddResource(Resources::Food, -1);
        World::AddTime(0, 5);
        break;

    case 10:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 17,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 13);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::PIPE].AnimationPhaseCount + 2);
        break;

    case 17:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 18:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        World::FillPipe();
        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[Tiles::PIPE].First) {
            PapierText = Renderer::DrawText(ROHRHILFE, TXTPAPIER, 1);
            Bmp[Tiles::PIPE].First = false;
        }

        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void sos()
{
    if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::SOS;

        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[Tiles::SOS].RequiredRawMaterials[i];
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Tiles::SOS].AnimationPhaseCount;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::SOS].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::SOS].targetRect.top);
    }

    Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep++;

    if (!World::CheckRawMaterials()) {
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep--;
        return;
    }

    switch (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 4,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 13);
        break;

    case 4:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 12,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 17);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::SOS].AnimationPhaseCount + 1);
        break;

    case 7:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 12,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 9);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::SOS].AnimationPhaseCount + 2);
        break;

    case 10:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 19,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 12);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::SOS].AnimationPhaseCount + 3);
        break;

    case 13:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 21,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 5);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::SOS].AnimationPhaseCount + 4);
        break;

    case 16:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 28,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 8);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::SOS].AnimationPhaseCount + 5);
        break;

    case 2:
    case 5:
    case 8:
    case 11:
    case 14:
    case 17:
        Guy.IsActive = true;
        Guy.ScreenPosition.x += 4;
        Guy.AnimationState = Tiles::GUY_LAYING_TENT;
        World::AddResource(Resources::Water, -1);
        World::AddResource(Resources::Food, -1);
        World::AddTime(0, 1);
        break;

    case 3:
    case 6:
    case 9:
    case 12:
    case 15:
    case 18:
        Guy.IsActive = true;
        Guy.ScreenPosition.x -= 4;
        Guy.AnimationState = Tiles::GUY_STAND_UP;
        World::AddResource(Resources::Water, -1);
        World::AddResource(Resources::Food, -1);
        World::AddTime(0, 1);
        break;

    case 19:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 20:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Terrain == 0) || (Landscape[Guy.Pos.x][Guy.Pos.y].Terrain == 4)) {
            Chance += 1;
        } else {
            Chance += 2;    // Dürfte nur noch der Strand übrig sein
        }

        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[Tiles::SOS].First) {
            PapierText = Renderer::DrawText(SOSHILFE, TXTPAPIER, 1);
            Bmp[Tiles::SOS].First = false;
        }

        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void fireplace()
{
    if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::BONFIRE;

        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[Tiles::BONFIRE].RequiredRawMaterials[i];
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Tiles::BONFIRE].AnimationPhaseCount;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::BONFIRE].targetRect.left);
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::BONFIRE].targetRect.top);
    }

    Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep++;

    if (!World::CheckRawMaterials()) {
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep--;
        return;
    }

    switch (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 4,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 16);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.ScreenPosition.x += 4;
        Guy.AnimationState = Tiles::GUY_LAYING_TENT;
        World::AddResource(Resources::Water, -1);
        World::AddResource(Resources::Food, -1);
        World::AddTime(0, 1);
        break;

    case 3:
        Guy.IsActive = true;
        Guy.ScreenPosition.x -= 4;
        Guy.AnimationState = Tiles::GUY_STAND_UP;
        World::AddResource(Resources::Water, -1);
        World::AddResource(Resources::Food, -1);
        World::AddTime(0, 1);
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::BONFIRE].AnimationPhaseCount + 1);
        break;

    case 4:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 15);
        break;

    case 5:
    case 6:
    case 7:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_TIE_UP;
        World::AddResource(Resources::Water, -1);
        World::AddResource(Resources::Food, -1);
        World::AddTime(0, 1);

        if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep != 5)
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase =
                static_cast<short>(Bmp[Tiles::BONFIRE].AnimationPhaseCount + Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep - 4);

        break;

    case 8:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 9:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[Tiles::BONFIRE].First) {
            PapierText = Renderer::DrawText(FEUERSTELLEHILFE, TXTPAPIER, 1);
            Bmp[Tiles::BONFIRE].First = false;
        }

        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void house1()
{
    if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken

        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[Tiles::HOUSE_1].RequiredRawMaterials[i];
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Tiles::HOUSE_1].AnimationPhaseCount;
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::HOUSE_1;
    }

    Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep++;

    if (!World::CheckRawMaterials()) {
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep--;
        return;
    }

    switch (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                            Bmp[Tiles::TREE_BIG].Width / 2 - 3,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                            Bmp[Tiles::TREE_BIG].Height + 1);
        break;

    case 2:
    case 3:
    case 4:
    case 5:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_1;
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 6:
    case 7:
    case 8:
    case 9:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_1;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_1].AnimationPhaseCount + 1);
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 10:
    case 11:
    case 12:
    case 13:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_1;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_1].AnimationPhaseCount + 2);
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 14:
    case 15:
    case 16:
    case 17:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_1;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_1].AnimationPhaseCount + 3);
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 18:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 19:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void house2()
{
    if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken

        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[Tiles::HOUSE_2].RequiredRawMaterials[i];
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Tiles::HOUSE_2].AnimationPhaseCount;
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::HOUSE_2;
    }

    Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep++;

    if (!World::CheckRawMaterials()) {
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep--;
        return;
    }

    switch (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                            Bmp[Tiles::TREE_BIG].Width / 2,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                            Bmp[Tiles::TREE_BIG].Height + 1);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_CLIMBING_1;
        World::AddResource(Resources::Food, -1);
        World::AddResource(Resources::Water, -1);
        World::AddTime(0, 1);
        break;

    case 3:
    case 4:
    case 5:
    case 6:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_2;
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 7:
    case 8:
    case 9:
    case 10:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_2;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_2].AnimationPhaseCount + 1);
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 11:
    case 12:
    case 13:
    case 14:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_2;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_2].AnimationPhaseCount + 2);
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 15:
    case 16:
    case 17:
    case 18:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_2;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_2].AnimationPhaseCount + 3);
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 19:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_CLIMBING_2;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_2].AnimationPhaseCount + 4);
        World::AddResource(Resources::Food, -1);
        World::AddResource(Resources::Water, -1);
        World::AddTime(0, 1);
        break;

    case 20:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 21:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void house3()
{
    if (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken

        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[Tiles::HOUSE_3].RequiredRawMaterials[i];
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Tiles::HOUSE_3].AnimationPhaseCount;
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::HOUSE_3;
    }

    Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep++;

    if (!World::CheckRawMaterials()) {
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep--;
        return;
    }

    switch (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                            Bmp[Tiles::TREE_BIG].Width / 2,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                            Bmp[Tiles::TREE_BIG].Height + 1);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_CLIMBING_1;
        World::AddResource(Resources::Food, -1);
        World::AddResource(Resources::Water, -1);
        World::AddTime(0, 1);
        break;

    case 3:
    case 4:
    case 5:
    case 6:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_2;
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 7:
    case 8:
    case 9:
    case 10:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_2;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_3].AnimationPhaseCount + 1);
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 11:
    case 12:
    case 13:
    case 14:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_2;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_3].AnimationPhaseCount + 2);
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 15:
    case 16:
    case 17:
    case 18:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_HAMMER_2;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_3].AnimationPhaseCount + 3);
        World::AddResource(Resources::Food, -0.5);
        World::AddResource(Resources::Water, -0.5);
        World::AddTime(0, 1);
        break;

    case 19:
        Guy.IsActive = true;
        Guy.AnimationState = Tiles::GUY_CLIMBING_2;
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[Tiles::HOUSE_3].AnimationPhaseCount + 4);
        World::AddResource(Resources::Food, -1);
        World::AddResource(Resources::Water, -1);
        World::AddTime(0, 1);
        break;

    case 20:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 21:
        Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[Tiles::HOUSE_3].First) {
            PapierText = Renderer::DrawText(HAUS3HILFE, TXTPAPIER, 1);
            Bmp[Tiles::HOUSE_3].First = false;
        }

        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void sleep()
{
    if (Guy.ActionStep == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount))
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 3,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                 (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount))
            Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                                Bmp[Tiles::TREE_BIG].Width / 2 + 1,
                                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                                Bmp[Tiles::TREE_BIG].Height + 1);

        break;

    case 2:
        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_CLIMBING_1;
            World::AddResource(Resources::Food, -1);
            World::AddResource(Resources::Water, -1);
        }

        break;

    case 3:
        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_ENTER_TENT;
        } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                   (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_ENTER_HOUSE;
        } else {
            Guy.ScreenPosition.x += 3;
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_LAYING_TENT;
        }

        break;

    case 4:
    case 5:
        Guy.IsActive = true;

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::TENT) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            if (Guy.ActionStep == 4) {
                Guy.ScreenPosition.x += 4;
            }

            Guy.AnimationState = Tiles::GUY_SLEEPING_TENT;
        } else if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                   (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            if (Guy.ActionStep == 4) {
                Guy.ScreenPosition.x += 14;
            }

            Guy.AnimationState = Tiles::GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = Tiles::GUY_SLEEPING;
        }

        World::AddResource(Resources::Health, 5);
        World::AddTime(0, 30);
        break;

    case 6:
        Guy.IsActive = true;
        StopSound(Sound::SNORE);

        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.ScreenPosition.x -= 14;
            Guy.AnimationState = Tiles::GUY_EXIT_HOUSE;
        } else {
            Guy.AnimationState = Tiles::GUY_STAND_UP;
        }

        break;

    case 7:
        if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::HOUSE_3) &&
                (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = Tiles::GUY_CLIMBING_2;
            World::AddResource(Resources::Food, -1);
            World::AddResource(Resources::Water, -1);
        }

        break;

    case 8:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        Guy.CurrentAction = Action::NOTHING;
        break;
    }
}

void undock()
{
    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        Routing::ShortRoute(Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 14,
                            Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 11);
        break;

    case 2:
        World::ToggleIsInBoat();
        Guy.ScreenPosition.x = Landscape[Guy.Pos.x][Guy.Pos.y].xScreen +
                               Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                               Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2;
        Guy.ScreenPosition.y = Landscape[Guy.Pos.x][Guy.Pos.y].yScreen +
                               Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                               Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Height / 2;
        Landscape[Guy.Pos.x][Guy.Pos.y].Object = -1;

        if (Landscape[Guy.Pos.x - 1][Guy.Pos.y].Terrain == 1) {
            Guy.Pos.x--;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y - 1].Terrain == 1) {
            Guy.Pos.y--;
        } else if (Landscape[Guy.Pos.x + 1][Guy.Pos.y].Terrain == 1) {
            Guy.Pos.x++;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y + 1].Terrain == 1) {
            Guy.Pos.y++;
        }

        Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));

        break;

    case 3:
        Guy.CurrentAction = Action::NOTHING;
        Guy.OriginalPosition.x = Guy.ScreenPosition.x;
        Guy.OriginalPosition.y = Guy.ScreenPosition.y;
        break;
    }
}

void dock()
{
    Guy.ActionStep++;

    switch (Guy.ActionStep) {
    case 1:
        if (Landscape[Guy.Pos.x - 1][Guy.Pos.y].Terrain != 1) {
            Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                                  Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].x) / 2),
                                ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].y +
                                  Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y) / 2));
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y - 1].Terrain != 1) {
            Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].x +
                                  Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                                ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                                  Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].y) / 2));
        } else if (Landscape[Guy.Pos.x + 1][Guy.Pos.y].Terrain != 1) {
            Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x +
                                  Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].x) / 2),
                                ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].y +
                                  Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y + 1].Terrain != 1) {
            Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].x +
                                  Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x) / 2),
                                ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y +
                                  Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].y) / 2));
        }

        break;

    case 2:
        if (Landscape[Guy.Pos.x - 1][Guy.Pos.y].Terrain != 1) {
            Guy.Pos.x--;
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y - 1].Terrain != 1) {
            Guy.Pos.y--;
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 1;
        } else if (Landscape[Guy.Pos.x + 1][Guy.Pos.y].Terrain != 1) {
            Guy.Pos.x++;
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        } else if (Landscape[Guy.Pos.x][Guy.Pos.y + 1].Terrain != 1) {
            Guy.Pos.y++;
            Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 1;
        }

        Landscape[Guy.Pos.x][Guy.Pos.y].Object = Tiles::BOAT;
        Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep = Bmp[Tiles::BOAT].RequiredActionCases;

        World::ToggleIsInBoat();
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = Guy.ScreenPosition.x -
                Landscape[Guy.Pos.x][Guy.Pos.y].xScreen -
                Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2;
        Landscape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = Guy.ScreenPosition.y -
                Landscape[Guy.Pos.x][Guy.Pos.y].yScreen -
                Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Height / 2;

        Routing::ShortRoute(((Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));

        break;

    case 3:
        Guy.CurrentAction = Action::NOTHING;
        Guy.OriginalPosition.x = Guy.ScreenPosition.x;
        Guy.OriginalPosition.y = Guy.ScreenPosition.y;
        break;
    }
}
} // namesapce Action
