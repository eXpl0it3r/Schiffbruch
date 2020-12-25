#include "Action.hpp"

#include "types.hpp"

#include "Game.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

namespace Action {
void handler(short event)
{
    if (event != NOTHING) {
        Routing::MarkRoute(false);
        RouteDestination.x = -1;
        RouteDestination.y = -1;
    }

    switch (event) {
    case NOTHING:
        break;

    case SEARCH:
        search();
        break;

    case EAT:
        eat();
        break;

    case DRINK:
        drink();
        break;

    case LOG:
        log();
        break;

    case FIELD:
        field();
        break;

    case DAY_END:
        day_end();
        break;

    case RESCUED:
        rescued();
        break;

    case TENT:
        tent();
        break;

    case SLEEP:
        sleep();
        break;

    case CANCEL:
        cancel();
        break;

    case FISH:
        fish();
        break;

    case BOAT:
        boat();
        break;

    case UNDOCK:
        undock();
        break;

    case DOCK:
        dock();
        break;

    case PIPE:
        pipe();
        break;

    case DESTROY:
        destroy();
        break;

    case SOS_SIGN:
        sos();
        break;

    case HOUSE1:
        house1();
        break;

    case HOUSE2:
        house2();
        break;

    case HOUSE3:
        house3();
        break;

    case FIREPLACE:
        fireplace();
        break;

    case LIGHT:
        light();
        break;

    case LOOKOUT:
        lookout();
        break;

    case TREASURE:
        treasure();
        break;

    case INTRO:
        intro();
        break;

    case SLINGSHOT:
        slingshot();
        break;

    case QUIT:
        quit();
        break;

    case RESTART:
        restart();
        break;

    case DAY_RESTART:
        day_restart();
        break;

    case DEATH:
        death();
        break;
    }
}

void intro()
{
    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
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
            if (Scape[x][Guy.Pos.y].Terrain != 1) {
                break;
            }

            RouteDestination.x = x - 1;
        }

        Routing::FindTheWay();
        break;

    case 2:
        Guy.ScreenPosition.y -= 10;
        Guy.IsActive = true;
        Guy.AnimationState = GUY_SHIP_DOWN;
        PlaySound(Sound::SPLAT, 100);
        PlaySound(Sound::CRASH, 100);
        break;

    case 3:
        Scape[Guy.Pos.x][Guy.Pos.y].Object = WRECK_1;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[WRECK_1].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[WRECK_1].targetRect.top);

        World::ToggleIsInBoat();
        Guy.Pos.x += 2;
        Guy.ScreenPosition.y += 10;
        Guy.AnimationState = GUY_SWIM;
        Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].x) / 2),
                            ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].y +
                              Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y) / 2));
        break;

    case 4:
        StopSound(Sound::SWIM); // Sound hier sofort stoppen
        Guy.AnimationState = GUY_LEFT;
        Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        break;

    case 5:
        Guy.OriginalPosition = Guy.ScreenPosition;
        Spielzustand = State::GAME;
        Guy.CurrentAction = NOTHING;
        PapierText = Renderer::DrawText(INTROTEXT, TXTPAPIER, 1);
        Game::SaveGame();
        break;
    }
}

void restart()
{
    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
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
            Guy.AnimationState = GUY_BOAT_WAITING;
        } else {
            Guy.AnimationState = GUY_WAITING;
        }

        PapierText = Renderer::DrawText(NEUBEGINNEN, TXTPAPIER, 1);
        break;

    case 3:
        Guy.CurrentAction = NOTHING;

        if (IsInBoat) {
            Guy.AnimationState = GUY_BOAT_LEFT;
        } else {
            Guy.AnimationState = GUY_LEFT;
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
    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
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
            Guy.AnimationState = GUY_BOAT_WAITING;
        } else {
            Guy.AnimationState = GUY_WAITING;
        }

        PapierText = Renderer::DrawText(TAGNEU, TXTPAPIER, 1);
        break;
    }

    case 3: {
        Guy.CurrentAction = NOTHING;

        if (IsInBoat) {
            Guy.AnimationState = GUY_BOAT_LEFT;
        } else {
            Guy.AnimationState = GUY_LEFT;
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
    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
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
            Guy.AnimationState = GUY_BOAT_WAITING;
        } else {
            Guy.AnimationState = GUY_WAITING;
        }

        PapierText = Renderer::DrawText(SPIELVERLASSEN, TXTPAPIER, 1);
        break;

    case 3:
        Guy.CurrentAction = NOTHING;

        if (IsInBoat) {
            Guy.AnimationState = GUY_BOAT_LEFT;
        } else {
            Guy.AnimationState = GUY_LEFT;
        }

        if (Frage == 1) {
            if (Guy.ResourceAmount[GESUNDHEIT] > 10) {
                Game::SaveGame();
            }

            Spielzustand = State::OUTRO;
        }

        Frage = -1;
        break;
    }
}

void death()
{
    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = GUY_BOAT_WAITING;
        } else {
            Guy.AnimationState = GUY_WAITING;
        }

        PapierText = Renderer::DrawText(TOD, TXTPAPIER, 1);
        break;

    case 2:
        if (!IsInBoat) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_LAYING_TENT;
        }

        break;

    case 3:
        Guy.IsActive = true;
        Night = false;
        Renderer::Fade(100, 100, 100);

        if (IsInBoat) {
            Guy.AnimationState = GUY_BOAT_DEAD;
        } else {
            Guy.AnimationState = GUY_DEAD;
        }

        break;

    case 4:
        Guy.IsActive = true;
        Night = true;
        Guy.AnimationState = GUY_WAITING;
        PapierText = Renderer::DrawText(TAGNEU, TXTPAPIER, 1);
        break;

    case 5:
        Night = false;

        if (IsInBoat) {
            Guy.AnimationState = GUY_BOAT_LEFT;
        } else {
            Guy.AnimationState = GUY_LEFT;
        }

        Guy.CurrentAction = NOTHING;

        if (Frage == 2) {
            Spielzustand = State::OUTRO;
        } else {
            Game::NewGame(false);
        }

        Frage = -1;
        break;
    }
}

void cancel()
{
    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x = Guy.ScreenPosition.x;
        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y = Guy.ScreenPosition.y;

        Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        break;

    case 2:
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void destroy()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition;    // Die Originalposition merken
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Width + 4,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Height);
        break;

    case 2:
    case 4:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_CHOP;
        World::AddResource(WASSER, -1);
        World::AddResource(NAHRUNG, -1);
        World::AddTime(0, 5);
        break;

    case 3:
    case 5:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_BEAT;
        World::AddResource(WASSER, -1);
        World::AddResource(NAHRUNG, -1);
        World::AddTime(0, 5);
        break;

    case 6: {
        if (Scape[Guy.Pos.x][Guy.Pos.y].Object == SOS) {
            Chance -= 0.1f;
        }

        short i = Scape[Guy.Pos.x][Guy.Pos.y].Object; // Um sich kurz das Objekt zu merken

        if ((i >= HOUSE_1) && (i <= HOUSE_3)) {
            Scape[Guy.Pos.x][Guy.Pos.y].Object = TREE_BIG;
        } else {
            Scape[Guy.Pos.x][Guy.Pos.y].Object = -1;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 0;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 0;
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = -1;
        }

        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber = 0;

        if (i == PIPE) {
            World::FillPipe();
        }

        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;
    }

    case 7:
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void search()
{
    Coordinate target;
    short i;

    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    while (true) {
        target.x = Scape[Guy.Pos.x][Guy.Pos.y].xScreen + rand() % TILE_SIZE_X;
        target.y = Scape[Guy.Pos.x][Guy.Pos.y].yScreen + rand() % TILE_SIZE_Y;
        Coordinate Erg = Renderer::GetTile(target.x, target.y);

        if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y)) {
            break;    // Wenn das gefundene Ziel in der Kachel, dann fertig
        }
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
    case 3:
    case 5:
    case 7:
        if (IsInBoat) {
            if (Guy.ActionNumber == 1) {
                Guy.IsActive = true;
                Guy.ScreenPosition.y -= 2;
                Guy.AnimationState = GUY_DIVING_1;
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
            if (Guy.ActionNumber == 2) {
                Guy.ScreenPosition.y += 5;
            }

            Guy.AnimationState = GUY_DIVING_2;
        } else {
            Guy.AnimationState = GUY_SEARCH;
        }

        World::AddTime(0, 4);
        break;

    case 9:
        if (IsInBoat) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_DIVING_3;
            PlaySound(Sound::SPLAT, 100);
        }

        break;

    case 10:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 11:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = GUY_BOAT_LEFT;
        }

        // Auf Strand und Fluss
        if ((Scape[Guy.Pos.x][Guy.Pos.y].Terrain == 2) || ((Scape[Guy.Pos.x][Guy.Pos.y].Object >= RIVER_1) && (Scape[Guy.Pos.x][Guy.Pos.y].Object <= FLOODGATE_6))) {
            if (Guy.Inventory[RAW_STONE] < 10) {
                PapierText = Renderer::DrawText(ROHSTEINGEFUNDEN, TXTPAPIER, 1);
                Guy.Inventory[RAW_STONE] += 3;

                if (Guy.Inventory[RAW_STONE] > 10) {
                    Guy.Inventory[RAW_STONE] = 10;
                }
            } else {
                PapierText = Renderer::DrawText(ROHSTEINZUVIEL, TXTPAPIER, 1);
            }
        } else if (Scape[Guy.Pos.x][Guy.Pos.y].Object == BUSH) {
            i = rand() % 2;

            switch (i) {
            case 0:
                if (Guy.Inventory[RAW_TREE_BRANCH] < 10) {
                    PapierText = Renderer::DrawText(ROHASTGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[RAW_TREE_BRANCH]++;
                } else {
                    PapierText = Renderer::DrawText(ROHASTZUVIEL, TXTPAPIER, 1);
                }

                break;

            case 1:
                if (Guy.Inventory[RAW_LEAF] < 10) {
                    PapierText = Renderer::DrawText(ROHBLATTGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[RAW_LEAF]++;
                } else {
                    PapierText = Renderer::DrawText(ROHBLATTZUVIEL, TXTPAPIER, 1);
                }

                break;
            }
        } else if (((Scape[Guy.Pos.x][Guy.Pos.y].Object >= TREE_1) && (Scape[Guy.Pos.x][Guy.Pos.y].Object <= TREE_BIG)) ||
                   ((Scape[Guy.Pos.x][Guy.Pos.y].Object >= HOUSE_1) && (Scape[Guy.Pos.x][Guy.Pos.y].Object <= HOUSE_3))) {
            i = rand() % 3;

            switch (i) {
            case 0:
                if (Guy.Inventory[RAW_TREE_BRANCH] < 10) {
                    PapierText = Renderer::DrawText(ROHASTGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[RAW_TREE_BRANCH]++;
                } else {
                    PapierText = Renderer::DrawText(ROHASTZUVIEL, TXTPAPIER, 1);
                }

                break;

            case 1:
                if (Guy.Inventory[RAW_LEAF] < 10) {
                    PapierText = Renderer::DrawText(ROHBLATTGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[RAW_LEAF]++;
                } else {
                    PapierText = Renderer::DrawText(ROHBLATTZUVIEL, TXTPAPIER, 1);
                }

                break;

            case 2:
                if (Guy.Inventory[RAW_LIANA] < 10) {
                    PapierText = Renderer::DrawText(ROHLIANEGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[RAW_LIANA]++;
                } else {
                    PapierText = Renderer::DrawText(ROHLIANEZUVIEL, TXTPAPIER, 1);
                }

                break;
            }
        } else if (IsInBoat) {
            if (Scape[Guy.Pos.x][Guy.Pos.y].Object == WRECK_1) {
                if (Guy.Inventory[RAW_TELESCOPE] == 0) {
                    PapierText = Renderer::DrawText(FERNROHRGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[RAW_TELESCOPE] = 1;
                    Bmp[BUTTON_LOOK_OUT].AnimationPhase = 0;
                    Guy.Inventory[RAW_HAMMER] = 1;
                    Bmp[BUTTON_HOUSE_1].AnimationPhase = 0;
                    Bmp[BUTTON_HOUSE_2].AnimationPhase = 0;
                    Bmp[BUTTON_HOUSE_3].AnimationPhase = 0;
                } else {
                    PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
                }
            } else if (Scape[Guy.Pos.x][Guy.Pos.y].Object == WRECK_2) {
                if (Guy.Inventory[RAW_MAP] == 0) {
                    PapierText = Renderer::DrawText(KARTEGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventory[RAW_MAP] = 1;
                    Bmp[BUTTON_TREASUREMAP].AnimationPhase = 0;
                    Guy.Inventory[RAW_SHOVEL] = 1;
                    Bmp[BUTTON_TREASURE].AnimationPhase = 0;
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
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void eat()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Height + 2);
        break;

    case 2:
    case 3:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_EAT;
        World::AddResource(NAHRUNG, 15);
        World::AddTime(0, 2);
        break;

    case 4:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 5:
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void slingshot()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2 - 14,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Height + 9);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_SLINGSHOT;
        Guy.ScreenPosition.x += 5;
        World::AddTime(0, 2);
        PlaySound(Sound::SLINGSHOT, 100);
        break;

    case 3:
        Guy.ScreenPosition.x -= 5;
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2 + 6,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Height + 2);
        break;

    case 4:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_SEARCH;
        World::AddResource(NAHRUNG, 5);
        World::AddTime(0, 20);
        break;

    case 5:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 6:
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void drink()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Routing::ShortRoute(Guy.ScreenPosition.x - 4,
                            Guy.ScreenPosition.y - 2);
        break;

    case 2:
    case 3:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_DRINK;
        World::AddResource(WASSER, 30);
        World::AddTime(0, 3);
        break;

    case 4:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 5:
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void log()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2 + 9,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Height + 3);
        break;

    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_CHOP;
        World::AddResource(WASSER, -2);
        World::AddResource(NAHRUNG, -2);
        World::AddTime(0, 10);
        break;

    case 7: {
        Guy.IsActive = true;
        Guy.AnimationState = GUY_WAITING;
        short i = Scape[Guy.Pos.x][Guy.Pos.y].Object + (TREE_DOWN_1 - TREE_1);
        Scape[Guy.Pos.x][Guy.Pos.y].Object = i;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x -= 17;
        PlaySound(Sound::TIMBER, 100);
        break;
    }

    case 8:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 9:
        Scape[Guy.Pos.x][Guy.Pos.y].Object = -1;
        Guy.Inventory[RAW_TREE_TRUNK]++;

        if (Guy.Inventory[RAW_TREE_TRUNK] > 10) {
            Guy.Inventory[RAW_TREE_TRUNK] = 10;
        }

        Guy.Inventory[RAW_TREE_BRANCH] += 5;

        if (Guy.Inventory[RAW_TREE_BRANCH] > 10) {
            Guy.Inventory[RAW_TREE_BRANCH] = 10;
        }

        Guy.Inventory[RAW_LEAF] += 5;

        if (Guy.Inventory[RAW_LEAF] > 10) {
            Guy.Inventory[RAW_LEAF] = 10;
        }

        Guy.Inventory[RAW_LIANA] += 2;

        if (Guy.Inventory[RAW_LIANA] > 10) {
            Guy.Inventory[RAW_LIANA] = 10;
        }

        Guy.CurrentAction = NOTHING;
        break;
    }
}

void fish()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        switch (Scape[Guy.Pos.x][Guy.Pos.y].Object) {
        case RIVER_1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 35,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
            break;

        case RIVER_2:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 19,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
            break;

        case RIVER_3:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 22,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 20);
            break;

        case RIVER_4:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 34,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 23);
            break;

        case RIVER_6:
        case RIVER_7:
        case RIVER_END_2:
        case RIVER_START_2:
        case FLOODGATE_2:
        case FLOODGATE_3:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 34,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 33);
            break;

        case RIVER_5:
        case RIVER_9:
        case RIVER_END_1:
        case RIVER_START_1:
        case FLOODGATE_1:
        case FLOODGATE_5:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 20,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 33);
            break;

        case RIVER_8:
        case RIVER_END_4:
        case RIVER_START_3:
        case FLOODGATE_4:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 22,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
            break;

        case RIVER_10:
        case RIVER_END_3:
        case RIVER_START_4:
        case FLOODGATE_6:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 32,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
            break;
        }

        break;

    case 2:
        Guy.IsActive = true;
        PlaySound(Sound::FISH, 100);

        if (IsInBoat) {
            Guy.ScreenPosition.y -= 2;
            Guy.AnimationState = GUY_BOAT_FISHING_1;
        }

        switch (Scape[Guy.Pos.x][Guy.Pos.y].Object) {
        case RIVER_1:
        case RIVER_6:
        case RIVER_7:
        case RIVER_END_2:
        case RIVER_START_2:
        case FLOODGATE_2:
        case FLOODGATE_3:
            Guy.AnimationState = GUY_FISHING_LEFT_1;
            break;

        case RIVER_2:
        case RIVER_5:
        case RIVER_9:
        case RIVER_END_1:
        case RIVER_START_1:
        case FLOODGATE_1:
        case FLOODGATE_5:
            Guy.AnimationState = GUY_FISHING_ABOVE_1;
            break;

        case RIVER_3:
        case RIVER_8:
        case RIVER_END_4:
        case RIVER_START_3:
        case FLOODGATE_4:
            Guy.AnimationState = GUY_FISHING_RIGHT_1;
            break;

        case RIVER_4:
        case RIVER_10:
        case RIVER_END_3:
        case RIVER_START_4:
        case FLOODGATE_6:
            Guy.AnimationState = GUY_FISHING_BELOW_1;
            break;
        }

        break;

    case 3:
    case 4:
    case 5:
    case 6:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = GUY_BOAT_FISHING_2;
        }

        switch (Scape[Guy.Pos.x][Guy.Pos.y].Object) {
        case RIVER_1:
        case RIVER_6:
        case RIVER_7:
        case RIVER_END_2:
        case RIVER_START_2:
        case FLOODGATE_2:
        case FLOODGATE_3:
            Guy.AnimationState = GUY_FISHING_LEFT_2;
            break;

        case RIVER_2:
        case RIVER_5:
        case RIVER_9:
        case RIVER_END_1:
        case RIVER_START_1:
        case FLOODGATE_1:
        case FLOODGATE_5:
            Guy.AnimationState = GUY_FISHING_ABOVE_2;
            break;

        case RIVER_3:
        case RIVER_8:
        case RIVER_END_4:
        case RIVER_START_3:
        case FLOODGATE_4:
            Guy.AnimationState = GUY_FISHING_RIGHT_2;
            break;

        case RIVER_4:
        case RIVER_10:
        case RIVER_END_3:
        case RIVER_START_4:
        case FLOODGATE_6:
            Guy.AnimationState = GUY_FISHING_BELOW_2;
            break;
        }

        Guy.ResourceAmount[GESUNDHEIT] += 2;
        World::AddTime(0, 20);
        break;

    case 7:
        Guy.IsActive = true;

        if (IsInBoat) {
            Guy.AnimationState = GUY_BOAT_FISHING_3;
        }

        switch (Scape[Guy.Pos.x][Guy.Pos.y].Object) {
        case RIVER_1:
        case RIVER_6:
        case RIVER_7:
        case RIVER_END_2:
        case RIVER_START_2:
        case FLOODGATE_2:
        case FLOODGATE_3:
            Guy.AnimationState = GUY_FISHING_LEFT_3;
            break;

        case RIVER_2:
        case RIVER_5:
        case RIVER_9:
        case RIVER_END_1:
        case RIVER_START_1:
        case FLOODGATE_1:
        case FLOODGATE_5:
            Guy.AnimationState = GUY_FISHING_ABOVE_3;
            break;

        case RIVER_3:
        case RIVER_8:
        case RIVER_END_4:
        case RIVER_START_3:
        case FLOODGATE_4:
            Guy.AnimationState = GUY_FISHING_RIGHT_3;
            break;

        case RIVER_4:
        case RIVER_10:
        case RIVER_END_3:
        case RIVER_START_4:
        case FLOODGATE_6:
            Guy.AnimationState = GUY_FISHING_BELOW_3;
            break;
        }

        break;

    case 8:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 9:
        Guy.ResourceAmount[NAHRUNG] += 20;
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void light()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2 - 10,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y
                            + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Height + 1);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_SET_FIRE;
        Guy.ScreenPosition.x += 5;
        World::AddTime(0, 1);
        break;

    case 3:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_WAITING;
        Scape[Guy.Pos.x][Guy.Pos.y].Object = FIRE;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[FIRE].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[FIRE].targetRect.top);
        Chance += 2 + 2 * Scape[Guy.Pos.x][Guy.Pos.y].Height;
        World::AddTime(0, 2);
        Guy.ScreenPosition.x -= 5;
        break;

    case 4:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 5:
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void lookout()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_LOOK_OUT;
        World::AddTime(0, 40);
        Chance += 1 + Scape[Guy.Pos.x][Guy.Pos.y].Height;
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_WAITING;
        World::AddTime(0, 40);
        break;

    case 3:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_LOOK_OUT;
        World::AddTime(0, 40);
        break;

    case 4:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 5:
        Chance -= 1 + Scape[Guy.Pos.x][Guy.Pos.y].Height;
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void treasure()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition;
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Guy.ScreenPosition.x -= 5;
        Guy.ScreenPosition.y += 1;
        Guy.IsActive = true;
        Guy.AnimationState = GUY_SHOVEL;
        break;

    case 2:
        World::AddTime(0, 20);
        World::AddResource(WASSER, -10);
        World::AddResource(NAHRUNG, -10);
        Guy.ScreenPosition.x += 5;
        Guy.ScreenPosition.y -= 1;
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);

        if (((Guy.Pos.x == SchatzPos.x) && (Guy.Pos.y == SchatzPos.y)) &&
                (!SchatzGef)) {
            PapierText = Renderer::DrawText(SCHATZGEFUNDEN, TXTPAPIER, 1);
            Guy.Inventory[RAW_MATCH] = 1;
            Bmp[BUTTON_IGNITE].AnimationPhase = 0;
            SchatzGef = true;
        } else {
            PapierText = Renderer::DrawText(KEINSCHATZ, TXTPAPIER, 1);
        }

        break;

    case 3:
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void field()
{
    if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken

        for (short i = 0; i < SPRITE_COUNT; i++) {
            Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[FIELD].RequiredRawMaterials[i];
        }

        Scape[Guy.Pos.x][Guy.Pos.y].Object = FIELD;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[FIELD].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[FIELD].targetRect.top);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[FIELD].AnimationPhaseCount;
    }

    Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber++;

    if (!World::CheckRohstoff()) {
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber--;
        return;
    }

    switch (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 22,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 23);
        break;

    case 4:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 4;
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 25,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 21);
        World::AddResource(WASSER, -2);
        World::AddResource(NAHRUNG, -2);
        World::AddTime(0, 30);
        break;

    case 7:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 5;
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 28,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 19);
        World::AddResource(WASSER, -2);
        World::AddResource(NAHRUNG, -2);
        World::AddTime(0, 30);
        break;

    case 10:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 6;
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 31,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 17);
        World::AddResource(WASSER, -2);
        World::AddResource(NAHRUNG, -2);
        World::AddTime(0, 30);
        break;

    case 13:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 7;
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 34,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 15);
        World::AddResource(WASSER, -2);
        World::AddResource(NAHRUNG, -2);
        World::AddTime(0, 30);
        break;

    case 16:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 8;
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 36,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 13);
        World::AddResource(WASSER, -2);
        World::AddResource(NAHRUNG, -2);
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
        Guy.AnimationState = GUY_FARMING;
        break;

    case 19:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        Scape[Guy.Pos.x][Guy.Pos.y].Object = FIELD;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[FIELD].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[FIELD].targetRect.top);
        break;

    case 20:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[FIELD].First) {
            PapierText = Renderer::DrawText(FELDHILFE, TXTPAPIER, 1);
            Bmp[FIELD].First = false;
        }

        Guy.CurrentAction = NOTHING;
        break;
    }
}

void day_end()
{
    Coordinate Erg;

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Renderer::Fade(100, 90, 90);
        Hours = 12;
        Minutes = 0;
        TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
        Bmp[BUTTON_STOP].AnimationPhase = -1;

        if ((Guy.AnimationState == GUY_SLEEPING_TENT) || (Guy.AnimationState == GUY_SLEEPING) ||
                (Guy.AnimationState == GUY_SLEEP_HOUSE) || (IsInBoat)) {
            break;
        }

        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x = Guy.ScreenPosition.x;
        Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y = Guy.ScreenPosition.y;
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

        if ((Guy.AnimationState == GUY_SLEEPING_TENT) || (Guy.AnimationState == GUY_SLEEPING) ||
                (Guy.AnimationState == GUY_SLEEP_HOUSE) || (IsInBoat)) {
            break;
        }

        // Wohnbare Objekte in der Umgebung suchen
        Erg.x = -1;
        Erg.y = -1;

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) || (Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3)) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y;
        } else if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Object == HOUSE_3) {
            Erg.x = Guy.Pos.x - 1;
            Erg.y = Guy.Pos.y;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Object == HOUSE_3) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y - 1;
        } else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Object == HOUSE_3) {
            Erg.x = Guy.Pos.x + 1;
            Erg.y = Guy.Pos.y;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Object == HOUSE_3) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y + 1;
        } else if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Object == TENT) {
            Erg.x = Guy.Pos.x - 1;
            Erg.y = Guy.Pos.y;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Object == TENT) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y - 1;
        } else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Object == TENT) {
            Erg.x = Guy.Pos.x + 1;
            Erg.y = Guy.Pos.y;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Object == TENT) {
            Erg.x = Guy.Pos.x;
            Erg.y = Guy.Pos.y + 1;
        }

        if ((Erg.x != -1) && (Erg.y != -1)) {
            Guy.Pos.x = Erg.x;
            Guy.Pos.y = Erg.y;

            if ((Scape[Erg.x][Erg.y].Object == TENT) &&
                    (Scape[Erg.x][Erg.y].AnimationPhase < Bmp[Scape[Erg.x][Erg.y].Object].AnimationPhaseCount))
                Routing::ShortRoute(Scape[Erg.x][Erg.y].xScreen + Scape[Erg.x][Erg.y].ObjectPosOffset.x + 3,
                                    Scape[Erg.x][Erg.y].yScreen + Scape[Erg.x][Erg.y].ObjectPosOffset.y + 20);
            else if ((Scape[Erg.x][Erg.y].Object == HOUSE_3) &&
                     (Scape[Erg.x][Erg.y].AnimationPhase < Bmp[Scape[Erg.x][Erg.y].Object].AnimationPhaseCount))
                Routing::ShortRoute(Scape[Erg.x][Erg.y].xScreen + Scape[Erg.x][Erg.y].ObjectPosOffset.x +
                                    Bmp[TREE_BIG].Width / 2,
                                    Scape[Erg.x][Erg.y].yScreen + Scape[Erg.x][Erg.y].ObjectPosOffset.y +
                                    Bmp[TREE_BIG].Height + 1);
        }

        break;

    case 3:
        Renderer::Fade(90, 70, 70);
        Hours = 12;
        Minutes = 0;

        if ((Guy.AnimationState == GUY_SLEEPING_TENT) || (Guy.AnimationState == GUY_SLEEPING) ||
                (Guy.AnimationState == GUY_SLEEP_HOUSE) || (IsInBoat)) {
            break;
        }

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_CLIMBING_1;
        }

        break;

    case 4:
        Renderer::Fade(70, 60, 60);
        Hours = 12;
        Minutes = 0;

        if ((Guy.AnimationState == GUY_SLEEPING_TENT) || (Guy.AnimationState == GUY_SLEEPING) ||
                (Guy.AnimationState == GUY_SLEEP_HOUSE) || (IsInBoat)) {
            break;
        }

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_ENTER_TENT;
        } else if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                   (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_ENTER_HOUSE;
        } else {
            Guy.ScreenPosition.x += 3;
            Guy.IsActive = true;
            Guy.AnimationState = GUY_LAYING_TENT;
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

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            if (Guy.AnimationState != GUY_SLEEPING_TENT) {
                Guy.ScreenPosition.x += 4;
            }

            Guy.AnimationState = GUY_SLEEPING_TENT;
        } else if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                   (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            if (Guy.AnimationState != GUY_SLEEP_HOUSE) {
                Guy.ScreenPosition.x += 14;
            }

            Guy.AnimationState = GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = GUY_SLEEPING;
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

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = GUY_SLEEPING_TENT;
        } else if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                   (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = GUY_SLEEPING;
        }

        break;

    case 7:
        Renderer::Fade(0, 0, 0);// Don't get confused as the picture in Show () will be painted over in black
        Night = true;
        Hours = 12;
        Minutes = 0;
        PlaySound(Sound::WOLF, 100);

        // Falsche Objekte Löschen
        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object >= TREE_DOWN_1) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Object <= TREE_DOWN_4)) {
            Scape[Guy.Pos.x][Guy.Pos.y].Object = -1;
            Guy.Inventory[RAW_TREE_TRUNK]++;

            if (Guy.Inventory[RAW_TREE_TRUNK] > 10) {
                Guy.Inventory[RAW_TREE_TRUNK] = 10;
            }
        }

        // Je nach Schlafort Zustand verändern
        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            World::AddResource(GESUNDHEIT, -5);

            if (Guy.ResourceAmount[GESUNDHEIT] <= 0) {
                Guy.IsActive = true;
                PapierText = Renderer::DrawText(TAGENDE5, TXTPAPIER, 1);
                Guy.ActionNumber = 2;
                Guy.CurrentAction = DEATH;
                Hours = 0;
                Minutes = 0;
            } else {
                Guy.IsActive = true;
                PapierText = Renderer::DrawText(TAGENDE2, TXTPAPIER, 1);
            }
        } else if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                   (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            World::AddResource(GESUNDHEIT, +20);
            Guy.IsActive = true;
            PapierText = Renderer::DrawText(TAGENDE4, TXTPAPIER, 1);
        } else if (IsInBoat) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_BOAT_WAITING;
            PapierText = Renderer::DrawText(TAGENDE3, TXTPAPIER, 1);
            Guy.ActionNumber = 2;
            Guy.CurrentAction = DEATH;
            Hours = 0;
            Minutes = 0;
        } else {
            World::AddResource(GESUNDHEIT, -20);

            if (Guy.ResourceAmount[GESUNDHEIT] <= 0) {
                Guy.IsActive = true;
                PapierText = Renderer::DrawText(TAGENDE5, TXTPAPIER, 1);
                Guy.ActionNumber = 2;
                Guy.CurrentAction = DEATH;
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

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = GUY_SLEEPING_TENT;
        } else if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                   (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = GUY_SLEEPING;
        }

        break;

    case 9:
        Renderer::Fade(40, 40, 40);
        Hours = 0;
        Minutes = 0;

        Hours = 0;
        Minutes = 0;
        Guy.IsActive = true;

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = GUY_SLEEPING_TENT;
        } else if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                   (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.AnimationState = GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = GUY_SLEEPING;
        }

        break;

    case 10:
        Renderer::Fade(70, 60, 60);
        Hours = 0;
        Minutes = 0;
        StopSound(Sound::SNORE);
        Guy.IsActive = true;

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.ScreenPosition.x -= 14;
            Guy.AnimationState = GUY_EXIT_HOUSE;
        } else {
            Guy.AnimationState = GUY_STAND_UP;
        }

        break;

    case 11:
        Renderer::Fade(90, 80, 80);
        Hours = 0;
        Minutes = 0;

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_CLIMBING_2;
        }

        break;

    case 12:
        Renderer::Fade(100, 100, 100);
        Hours = 0;
        Minutes = 0;
        Guy.AnimationState = GUY_LEFT;
        Guy.CurrentAction = NOTHING;

        if (Guy.ResourceAmount[GESUNDHEIT] > 10) {
            Game::SaveGame();
        }

        break;
    }
}

void rescued()
{
    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
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
        Guy.AnimationState = GUY_WAITING;
        PapierText = Renderer::DrawText(GERETTET, TXTPAPIER, 1);
        break;

    case 3:
        if (Frage == 2) {
            Guy.CurrentAction = NOTHING;
            Frage = -1;
            break;
        }

        Spielzustand = State::RESCUED;
        Frage = -1;
        break;

    case 4:
        // Route herstellen
        Guy.IsActive = true;
        Guy.AnimationState = GUY_LEFT;
        RoutePunkt = -1;
        Steps = 0;
        Step = 0;
        RouteStart.x = Guy.Pos.x;
        RouteStart.y = Guy.Pos.y;
        RouteDestination.y = Guy.Pos.y;

        for (short x = MAX_TILES_X - 1; x > 1; x--) { // Position des Rettungsschiffs festlegen
            if (Scape[x][Guy.Pos.y].Terrain != 1) {
                break;
            }

            RouteDestination.x = x + 1;
        }

        // Schiff hinbauen
        Scape[RouteDestination.x][RouteDestination.y].AnimationPhase = 0;
        Scape[RouteDestination.x][RouteDestination.y].Object = GUY_SHIP;
        Scape[RouteDestination.x][RouteDestination.y].ObjectPosOffset.x = 10;
        Scape[RouteDestination.x][RouteDestination.y].ObjectPosOffset.y = 10;
        RouteDestination.x -= 2;
        Routing::FindTheWay();
        Guy.AnimationState = GUY_LEFT;
        break;

    case 5:
        Guy.AnimationState = GUY_LEFT;
        Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x +
                              Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].x) / 2),
                            ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].y +
                              Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        break;

    case 6:
        Guy.Pos.x += 2;
        Guy.AnimationState = GUY_SWIM;
        Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        break;

    case 7:
        Guy.ScreenPosition.y -= 10;

        if (!IsInBoat) {
            World::ToggleIsInBoat();
        }

        Guy.IsActive = true;
        Guy.AnimationState = GUY_SHIP;
        RoutePunkt = -1;
        Steps = 0;
        Step = 0;
        RouteStart.x = Guy.Pos.x;
        RouteStart.y = Guy.Pos.y;
        RouteDestination.y = Guy.Pos.y;
        RouteDestination.x = MAX_TILES_X - 2;
        Routing::FindTheWay();
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[SEA_WAVES].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[SEA_WAVES].targetRect.top);
        Scape[Guy.Pos.x][Guy.Pos.y].Object = SEA_WAVES;
        break;

    case 8:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_SHIP;
        break;

    case 9:
        Guy.CurrentAction = NOTHING;
        Guy.AnimationState = GUY_LEFT;
        Spielzustand = State::OUTRO;
        break;
    }
}

void tent()
{
    if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Scape[Guy.Pos.x][Guy.Pos.y].Object = TENT;

        for (short i = 0; i < SPRITE_COUNT; i++) {
            Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[TENT].RequiredRawMaterials[i];
        }

        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[TENT].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[TENT].targetRect.top);
    }

    Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber++;

    if (!World::CheckRohstoff()) {
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber--;
        return;
    }

    switch (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 22,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 12);
        break;

    case 2:
    case 3:
    case 12:
    case 13:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_TIE_DOWN;
        World::AddResource(WASSER, -2);
        World::AddResource(NAHRUNG, -2);
        World::AddTime(0, 15);
        break;

    case 4:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 2;
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 31,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 5:
        Routing::ShortRoute(Guy.OriginalPosition.x,
                            Guy.OriginalPosition.y);
        break;

    case 6:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 3,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);

        break;

    case 7:
    case 8:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_TIE_UP;
        World::AddResource(WASSER, -2);
        World::AddResource(NAHRUNG, -2);
        World::AddTime(0, 15);
        break;

    case 9:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 3;
        Routing::ShortRoute(Guy.OriginalPosition.x,
                            Guy.OriginalPosition.y);
        break;

    case 10:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 31,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 11:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 22,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 12);
        break;

    case 14:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 31,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 15:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 16:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[TENT].First) {
            PapierText = Renderer::DrawText(ZELTHILFE, TXTPAPIER, 1);
            Bmp[TENT].First = false;
        }

        Guy.CurrentAction = NOTHING;
        break;
    }
}

void boat()
{
    if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Scape[Guy.Pos.x][Guy.Pos.y].Object = BOAT;

        for (short i = 0; i < SPRITE_COUNT; i++) {
            Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[BOAT].RequiredRawMaterials[i];
        }

        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[BOAT].AnimationPhaseCount;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[BOAT].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[BOAT].targetRect.top);
    }

    Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber++;

    if (!World::CheckRohstoff()) {
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber--;
        return;
    }

    switch (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 30,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 21);
        break;

    case 2:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 29,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 3:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 28,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 19);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[BOAT].AnimationPhaseCount + 1);
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
        Guy.AnimationState = GUY_BEAT;
        World::AddResource(WASSER, -2);
        World::AddResource(NAHRUNG, -2);
        World::AddTime(0, 15);
        break;

    case 7:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 22,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 16);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[BOAT].AnimationPhaseCount + 2);
        break;

    case 11:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 14,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 11);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[BOAT].AnimationPhaseCount + 3);
        break;

    case 15:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 16:
        if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Terrain == 1) {
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 0;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 10;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Terrain == 1) {
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 1;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 25;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 10;
        } else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Terrain == 1) {
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 30;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 27;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Terrain == 1) {
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 1;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = 0;
            Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = 28;
        }

        Bmp[BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[BOAT].First) {
            PapierText = Renderer::DrawText(BOOTHILFE, TXTPAPIER, 1);
            Bmp[BOAT].First = false;
        }

        Guy.CurrentAction = NOTHING;
        break;
    }
}

void pipe()
{
    if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Scape[Guy.Pos.x][Guy.Pos.y].Object = PIPE;

        for (short i = 0; i < SPRITE_COUNT; i++) {
            Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[PIPE].RequiredRawMaterials[i];
        }

        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[PIPE].AnimationPhaseCount;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[PIPE].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[PIPE].targetRect.top);
    }

    Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber++;

    if (!World::CheckRohstoff()) {
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber--;
        return;
    }

    switch (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 30,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 21);
        break;

    case 2:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 29,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        break;

    case 3:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 28,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 15);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[PIPE].AnimationPhaseCount + 1);
        break;

    case 4:
    case 5:
    case 6:
    case 11:
    case 12:
    case 13:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_BEAT;
        World::AddResource(WASSER, -1);
        World::AddResource(NAHRUNG, -1);
        World::AddTime(0, 5);
        break;

    case 7:
    case 8:
    case 9:
    case 14:
    case 15:
    case 16:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_CHOP;
        World::AddResource(WASSER, -1);
        World::AddResource(NAHRUNG, -1);
        World::AddTime(0, 5);
        break;

    case 10:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 17,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 13);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[PIPE].AnimationPhaseCount + 2);
        break;

    case 17:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 18:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        World::FillPipe();
        Bmp[BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[PIPE].First) {
            PapierText = Renderer::DrawText(ROHRHILFE, TXTPAPIER, 1);
            Bmp[PIPE].First = false;
        }

        Guy.CurrentAction = NOTHING;
        break;
    }
}

void sos()
{
    if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Scape[Guy.Pos.x][Guy.Pos.y].Object = SOS;

        for (short i = 0; i < SPRITE_COUNT; i++) {
            Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[SOS].RequiredRawMaterials[i];
        }

        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[SOS].AnimationPhaseCount;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[SOS].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[SOS].targetRect.top);
    }

    Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber++;

    if (!World::CheckRohstoff()) {
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber--;
        return;
    }

    switch (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 4,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 13);
        break;

    case 4:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 12,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 17);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[SOS].AnimationPhaseCount + 1);
        break;

    case 7:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 12,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 9);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[SOS].AnimationPhaseCount + 2);
        break;

    case 10:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 19,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 12);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[SOS].AnimationPhaseCount + 3);
        break;

    case 13:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 21,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 5);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[SOS].AnimationPhaseCount + 4);
        break;

    case 16:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 28,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 8);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[SOS].AnimationPhaseCount + 5);
        break;

    case 2:
    case 5:
    case 8:
    case 11:
    case 14:
    case 17:
        Guy.IsActive = true;
        Guy.ScreenPosition.x += 4;
        Guy.AnimationState = GUY_LAYING_TENT;
        World::AddResource(WASSER, -1);
        World::AddResource(NAHRUNG, -1);
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
        Guy.AnimationState = GUY_STAND_UP;
        World::AddResource(WASSER, -1);
        World::AddResource(NAHRUNG, -1);
        World::AddTime(0, 1);
        break;

    case 19:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 20:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Terrain == 0) || (Scape[Guy.Pos.x][Guy.Pos.y].Terrain == 4)) {
            Chance += 1;
        } else {
            Chance += 2;    // Dürfte nur noch der Strand übrig sein
        }

        Bmp[BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[SOS].First) {
            PapierText = Renderer::DrawText(SOSHILFE, TXTPAPIER, 1);
            Bmp[SOS].First = false;
        }

        Guy.CurrentAction = NOTHING;
        break;
    }
}

void fireplace()
{
    if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
        Scape[Guy.Pos.x][Guy.Pos.y].Object = BONFIRE;

        for (short i = 0; i < SPRITE_COUNT; i++) {
            Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[BONFIRE].RequiredRawMaterials[i];
        }

        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[BONFIRE].AnimationPhaseCount;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[BONFIRE].targetRect.left);
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[BONFIRE].targetRect.top);
    }

    Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber++;

    if (!World::CheckRohstoff()) {
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber--;
        return;
    }

    switch (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 4,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 16);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.ScreenPosition.x += 4;
        Guy.AnimationState = GUY_LAYING_TENT;
        World::AddResource(WASSER, -1);
        World::AddResource(NAHRUNG, -1);
        World::AddTime(0, 1);
        break;

    case 3:
        Guy.IsActive = true;
        Guy.ScreenPosition.x -= 4;
        Guy.AnimationState = GUY_STAND_UP;
        World::AddResource(WASSER, -1);
        World::AddResource(NAHRUNG, -1);
        World::AddTime(0, 1);
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[BONFIRE].AnimationPhaseCount + 1);
        break;

    case 4:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 15);
        break;

    case 5:
    case 6:
    case 7:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_TIE_UP;
        World::AddResource(WASSER, -1);
        World::AddResource(NAHRUNG, -1);
        World::AddTime(0, 1);

        if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber != 5)
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase =
                static_cast<short>(Bmp[BONFIRE].AnimationPhaseCount + Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber - 4);

        break;

    case 8:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 9:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[BONFIRE].First) {
            PapierText = Renderer::DrawText(FEUERSTELLEHILFE, TXTPAPIER, 1);
            Bmp[BONFIRE].First = false;
        }

        Guy.CurrentAction = NOTHING;
        break;
    }
}

void house1()
{
    if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken

        for (short i = 0; i < SPRITE_COUNT; i++) {
            Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[HOUSE_1].RequiredRawMaterials[i];
        }

        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[HOUSE_1].AnimationPhaseCount;
        Scape[Guy.Pos.x][Guy.Pos.y].Object = HOUSE_1;
    }

    Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber++;

    if (!World::CheckRohstoff()) {
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber--;
        return;
    }

    switch (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                            Bmp[TREE_BIG].Width / 2 - 3,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                            Bmp[TREE_BIG].Height + 1);
        break;

    case 2:
    case 3:
    case 4:
    case 5:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_1;
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 6:
    case 7:
    case 8:
    case 9:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_1;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_1].AnimationPhaseCount + 1);
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 10:
    case 11:
    case 12:
    case 13:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_1;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_1].AnimationPhaseCount + 2);
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 14:
    case 15:
    case 16:
    case 17:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_1;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_1].AnimationPhaseCount + 3);
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 18:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 19:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[BUTTON_STOP].AnimationPhase = -1;
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void house2()
{
    if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken

        for (short i = 0; i < SPRITE_COUNT; i++) {
            Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[HOUSE_2].RequiredRawMaterials[i];
        }

        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[HOUSE_2].AnimationPhaseCount;
        Scape[Guy.Pos.x][Guy.Pos.y].Object = HOUSE_2;
    }

    Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber++;

    if (!World::CheckRohstoff()) {
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber--;
        return;
    }

    switch (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                            Bmp[TREE_BIG].Width / 2,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                            Bmp[TREE_BIG].Height + 1);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_CLIMBING_1;
        World::AddResource(NAHRUNG, -1);
        World::AddResource(WASSER, -1);
        World::AddTime(0, 1);
        break;

    case 3:
    case 4:
    case 5:
    case 6:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_2;
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 7:
    case 8:
    case 9:
    case 10:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_2;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_2].AnimationPhaseCount + 1);
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 11:
    case 12:
    case 13:
    case 14:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_2;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_2].AnimationPhaseCount + 2);
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 15:
    case 16:
    case 17:
    case 18:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_2;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_2].AnimationPhaseCount + 3);
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 19:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_CLIMBING_2;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_2].AnimationPhaseCount + 4);
        World::AddResource(NAHRUNG, -1);
        World::AddResource(WASSER, -1);
        World::AddTime(0, 1);
        break;

    case 20:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 21:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[BUTTON_STOP].AnimationPhase = -1;
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void house3()
{
    if (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken

        for (short i = 0; i < SPRITE_COUNT; i++) {
            Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] = Bmp[HOUSE_3].RequiredRawMaterials[i];
        }

        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = Bmp[HOUSE_3].AnimationPhaseCount;
        Scape[Guy.Pos.x][Guy.Pos.y].Object = HOUSE_3;
    }

    Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber++;

    if (!World::CheckRohstoff()) {
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber--;
        return;
    }

    switch (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                            Bmp[TREE_BIG].Width / 2,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                            Bmp[TREE_BIG].Height + 1);
        break;

    case 2:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_CLIMBING_1;
        World::AddResource(NAHRUNG, -1);
        World::AddResource(WASSER, -1);
        World::AddTime(0, 1);
        break;

    case 3:
    case 4:
    case 5:
    case 6:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_2;
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 7:
    case 8:
    case 9:
    case 10:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_2;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_3].AnimationPhaseCount + 1);
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 11:
    case 12:
    case 13:
    case 14:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_2;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_3].AnimationPhaseCount + 2);
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 15:
    case 16:
    case 17:
    case 18:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_HAMMER_2;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_3].AnimationPhaseCount + 3);
        World::AddResource(NAHRUNG, -0.5);
        World::AddResource(WASSER, -0.5);
        World::AddTime(0, 1);
        break;

    case 19:
        Guy.IsActive = true;
        Guy.AnimationState = GUY_CLIMBING_2;
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = static_cast<short>(Bmp[HOUSE_3].AnimationPhaseCount + 4);
        World::AddResource(NAHRUNG, -1);
        World::AddResource(WASSER, -1);
        World::AddTime(0, 1);
        break;

    case 20:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        break;

    case 21:
        Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        Bmp[BUTTON_STOP].AnimationPhase = -1;

        if (Bmp[HOUSE_3].First) {
            PapierText = Renderer::DrawText(HAUS3HILFE, TXTPAPIER, 1);
            Bmp[HOUSE_3].First = false;
        }

        Guy.CurrentAction = NOTHING;
        break;
    }
}

void sleep()
{
    if (Guy.ActionNumber == 0) {
        Guy.OriginalPosition = Guy.ScreenPosition; // Die Originalposition merken
    }

    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount))
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 3,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 20);
        else if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                 (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount))
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                                Bmp[TREE_BIG].Width / 2 + 1,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                                Bmp[TREE_BIG].Height + 1);

        break;

    case 2:
        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_CLIMBING_1;
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
        }

        break;

    case 3:
        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_ENTER_TENT;
        } else if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                   (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_ENTER_HOUSE;
        } else {
            Guy.ScreenPosition.x += 3;
            Guy.IsActive = true;
            Guy.AnimationState = GUY_LAYING_TENT;
        }

        break;

    case 4:
    case 5:
        Guy.IsActive = true;

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            if (Guy.ActionNumber == 4) {
                Guy.ScreenPosition.x += 4;
            }

            Guy.AnimationState = GUY_SLEEPING_TENT;
        } else if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                   (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            if (Guy.ActionNumber == 4) {
                Guy.ScreenPosition.x += 14;
            }

            Guy.AnimationState = GUY_SLEEP_HOUSE;
        } else {
            Guy.AnimationState = GUY_SLEEPING;
        }

        World::AddResource(GESUNDHEIT, 5);
        World::AddTime(0, 30);
        break;

    case 6:
        Guy.IsActive = true;
        StopSound(Sound::SNORE);

        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.ScreenPosition.x -= 14;
            Guy.AnimationState = GUY_EXIT_HOUSE;
        } else {
            Guy.AnimationState = GUY_STAND_UP;
        }

        break;

    case 7:
        if ((Scape[Guy.Pos.x][Guy.Pos.y].Object == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) {
            Guy.IsActive = true;
            Guy.AnimationState = GUY_CLIMBING_2;
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
        }

        break;

    case 8:
        Routing::ShortRoute(Guy.OriginalPosition.x, Guy.OriginalPosition.y);
        Guy.CurrentAction = NOTHING;
        break;
    }
}

void undock()
{
    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x + 14,
                            Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y + 11);
        break;

    case 2:
        World::ToggleIsInBoat();
        Guy.ScreenPosition.x = Scape[Guy.Pos.x][Guy.Pos.y].xScreen +
                               Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x +
                               Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2;
        Guy.ScreenPosition.y = Scape[Guy.Pos.x][Guy.Pos.y].yScreen +
                               Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y +
                               Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Height / 2;
        Scape[Guy.Pos.x][Guy.Pos.y].Object = -1;

        if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Terrain == 1) {
            Guy.Pos.x--;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Terrain == 1) {
            Guy.Pos.y--;
        } else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Terrain == 1) {
            Guy.Pos.x++;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Terrain == 1) {
            Guy.Pos.y++;
        }

        Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));

        break;

    case 3:
        Guy.CurrentAction = NOTHING;
        Guy.OriginalPosition.x = Guy.ScreenPosition.x;
        Guy.OriginalPosition.y = Guy.ScreenPosition.y;
        break;
    }
}

void dock()
{
    Guy.ActionNumber++;

    switch (Guy.ActionNumber) {
    case 1:
        if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Terrain != 1) {
            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                                  Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].y +
                                  Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y) / 2));
        } else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Terrain != 1) {
            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].x +
                                  Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                                  Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].y) / 2));
        } else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Terrain != 1) {
            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x +
                                  Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].y +
                                  Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
        } else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Terrain != 1) {
            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].x +
                                  Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y +
                                  Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].y) / 2));
        }

        break;

    case 2:
        if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Terrain != 1) {
            Guy.Pos.x--;
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Terrain != 1) {
            Guy.Pos.y--;
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 1;
        } else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Terrain != 1) {
            Guy.Pos.x++;
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 0;
        } else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Terrain != 1) {
            Guy.Pos.y++;
            Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase = 1;
        }

        Scape[Guy.Pos.x][Guy.Pos.y].Object = BOAT;
        Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber = Bmp[BOAT].RequiredActionCases;

        World::ToggleIsInBoat();
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.x = Guy.ScreenPosition.x -
                Scape[Guy.Pos.x][Guy.Pos.y].xScreen -
                Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Width / 2;
        Scape[Guy.Pos.x][Guy.Pos.y].ObjectPosOffset.y = Guy.ScreenPosition.y -
                Scape[Guy.Pos.x][Guy.Pos.y].yScreen -
                Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].Height / 2;

        Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                              Scape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                            ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                              Scape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));

        break;

    case 3:
        Guy.CurrentAction = NOTHING;
        Guy.OriginalPosition.x = Guy.ScreenPosition.x;
        Guy.OriginalPosition.y = Guy.ScreenPosition.y;
        break;
    }
}
} // namesapce Action
