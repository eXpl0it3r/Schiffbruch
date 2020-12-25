#include "Action.hpp"

#include "types.hpp"

#include "Game.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

namespace Action
{
    void handler(short event)
    {
        if (event != NOTHING)
        {
            Routing::MarkRoute(false);
            RouteZiel.x = -1;
            RouteZiel.y = -1;
        }
        switch (event)
        {
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
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            // Intro Route herstellen
            Guy.Aktiv = true;
            RoutePunkt = -1;
            Steps = 0;
            Step = 0;
            RouteStart.x = Guy.Pos.x;
            RouteStart.y = Guy.Pos.y;
            RouteZiel.y = Guy.Pos.y;
            for (short x = Guy.Pos.x; x < MAX_TILES_X; x++) // Zielkoordinate für Introroute finden
            {
                if (Scape[x][Guy.Pos.y].Art != 1) break;
                RouteZiel.x = x - 1;
            }
            Routing::FindTheWay();
            break;
        case 2:
            Guy.PosScreen.y -= 10;
            Guy.Aktiv = true;
            Guy.Zustand = GUY_SHIP_DOWN;
            PlaySound(Sound::SPLAT, 100);
            PlaySound(Sound::CRASH, 100);
            break;
        case 3:
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = WRECK_1;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[WRECK_1].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[WRECK_1].rcDes.top);

            World::ChangeBootsFahrt();
            Guy.Pos.x += 2;
            Guy.PosScreen.y += 10;
            Guy.Zustand = GUY_SWIM;
            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                                    Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].y +
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y) / 2));
            break;
        case 4:
            StopSound(Sound::SWIM); // Sound hier sofort stoppen
            Guy.Zustand = GUY_LEFT;
            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                                    Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
            break;
        case 5:
            Guy.PosAlt = Guy.PosScreen;
            Spielzustand = State::GAME;
            Guy.Aktion = NOTHING;
            PapierText = Renderer::DrawText(INTROTEXT, TXTPAPIER, 1);
            Game::SaveGame();
            break;
        }
    }

    void restart()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1: {
            ZWEID Erg = Renderer::GetKachel(Guy.PosAlt.x, Guy.PosAlt.y);
            if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y))
                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            else if (RoutePunkt % 2 == 0)
                Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); // Nur bis zur Mitte der aktuellen Kacheln laufen
            else
                Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
            TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
            break;
        }
        case 2:
            Guy.Aktiv = true;
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_WAITING;
            else
                Guy.Zustand = GUY_WAITING;

            PapierText = Renderer::DrawText(NEUBEGINNEN, TXTPAPIER, 1);
            break;
        case 3:
            Guy.Aktion = NOTHING;
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_LEFT;
            else
                Guy.Zustand = GUY_LEFT;

            if (Frage == 1)
            {
                Game::NeuesSpiel(true);
                return;
            }
            Frage = -1;
            break;
        }
    }

    void day_restart()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1: {
            ZWEID Erg = Renderer::GetKachel(Guy.PosAlt.x, Guy.PosAlt.y);
            if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y))
                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            else if (RoutePunkt % 2 == 0)
                Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); // Nur bis zur Mitte der aktuellen Kacheln laufen
            else
                Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);

            TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
            break;
        }
        case 2: {
            Guy.Aktiv = true;
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_WAITING;
            else
                Guy.Zustand = GUY_WAITING;

            PapierText = Renderer::DrawText(TAGNEU, TXTPAPIER, 1);
            break;
        }
        case 3: {
            Guy.Aktion = NOTHING;
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_LEFT;
            else
                Guy.Zustand = GUY_LEFT;

            if (Frage == 1)
            {
                Game::NeuesSpiel(false);
                return;
            }
            Frage = -1;
            break;
        }
        }
    }

    void quit()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1: {
            ZWEID Erg = Renderer::GetKachel(Guy.PosAlt.x, Guy.PosAlt.y);
            if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y))
                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            else if (RoutePunkt % 2 == 0)
                Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); // Nur bis zur Mitte der aktuellen Kacheln laufen
            else
                Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);

            TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
            break;
        }
        case 2:
            Guy.Aktiv = true;
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_WAITING;
            else
                Guy.Zustand = GUY_WAITING;

            PapierText = Renderer::DrawText(SPIELVERLASSEN, TXTPAPIER, 1);
            break;
        case 3:
            Guy.Aktion = NOTHING;
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_LEFT;
            else
                Guy.Zustand = GUY_LEFT;

            if (Frage == 1)
            {
                if (Guy.Resource[GESUNDHEIT] > 10)
                    Game::SaveGame();

                Spielzustand = State::OUTRO;
            }
            Frage = -1;
            break;
        }
    }

    void death()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Guy.Aktiv = true;
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_WAITING;
            else
                Guy.Zustand = GUY_WAITING;

            PapierText = Renderer::DrawText(TOD, TXTPAPIER, 1);
            break;
        case 2:
            if (!BootsFahrt)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_LAYING_TENT;
            }
            break;
        case 3:
            Guy.Aktiv = true;
            Nacht = false;
            Renderer::Fade(100, 100, 100);
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_DEAD;
            else
                Guy.Zustand = GUY_DEAD;

            break;
        case 4:
            Guy.Aktiv = true;
            Nacht = true;
            Guy.Zustand = GUY_WAITING;
            PapierText = Renderer::DrawText(TAGNEU, TXTPAPIER, 1);
            break;
        case 5:
            Nacht = false;
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_LEFT;
            else
                Guy.Zustand = GUY_LEFT;

            Guy.Aktion = NOTHING;
            if (Frage == 2)
                Spielzustand = State::OUTRO;
            else
                Game::NeuesSpiel(false);

            Frage = -1;
            break;
        }
    }

    void cancel()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x = Guy.PosScreen.x;
            Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y = Guy.PosScreen.y;

            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                                    Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
            break;
        case 2:
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void destroy()
    {
        if (Guy.AkNummer == 0)
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken

        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite + 4,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe);
            break;
        case 2: case 4:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_CHOP;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 5);
            break;
        case 3: case 5:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_BEAT;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 5);
            break;
        case 6:
            {
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == SOS)
                    Chance -= 0.1f;

                short i = Scape[Guy.Pos.x][Guy.Pos.y].Objekt; // Um sich kurz das Objekt zu merken 

                if ((i >= HOUSE_1) && (i <= HOUSE_3))
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt = TREE_BIG;
                else
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt = -1;
                    Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = 0;
                    Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = 0;
                    Scape[Guy.Pos.x][Guy.Pos.y].Phase = -1;
                }
                Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;

                if (i == PIPE)
                    World::FillRohr();

                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
                break;
            }
        case 7:
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void search()
    {
        ZWEID Ziel;
        short i;

        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        while (true)
        {
            Ziel.x = Scape[Guy.Pos.x][Guy.Pos.y].xScreen + rand() % TILE_SIZE_X;
            Ziel.y = Scape[Guy.Pos.x][Guy.Pos.y].yScreen + rand() % TILE_SIZE_Y;
            ZWEID Erg = Renderer::GetKachel(Ziel.x, Ziel.y);
            if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y))
                break; // Wenn das gefundene Ziel in der Kachel, dann fertig
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1: case 3: case 5: case 7:
            if (BootsFahrt)
            {
                if (Guy.AkNummer == 1)
                {
                    Guy.Aktiv = true;
                    Guy.PosScreen.y -= 2;
                    Guy.Zustand = GUY_DIVING_1;
                    PlaySound(Sound::SPLAT, 100);
                }
            }
            else
                Routing::ShortRoute(Ziel.x, Ziel.y);

            break;
        case 2: case 4: case 6: case 8:
            Guy.Aktiv = true;
            if (BootsFahrt)
            {
                if (Guy.AkNummer == 2)
                    Guy.PosScreen.y += 5;

                Guy.Zustand = GUY_DIVING_2;
            }
            else
                Guy.Zustand = GUY_SEARCH;

            World::AddTime(0, 4);
            break;
        case 9:
            if (BootsFahrt)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_DIVING_3;
                PlaySound(Sound::SPLAT, 100);
            }
            break;
        case 10:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 11:
            Guy.Aktiv = true;
            if (BootsFahrt)
                Guy.Zustand = GUY_BOAT_LEFT;

            // Auf Strand und Fluss
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Art == 2) || ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= RIVER_1) && (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= FLOODGATE_6)))
            {
                if (Guy.Inventar[RAW_STONE] < 10)
                {
                    PapierText = Renderer::DrawText(ROHSTEINGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventar[RAW_STONE] += 3;

                    if (Guy.Inventar[RAW_STONE] > 10)
                        Guy.Inventar[RAW_STONE] = 10;
                }
                else
                    PapierText = Renderer::DrawText(ROHSTEINZUVIEL, TXTPAPIER, 1);
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == BUSH)
            {
                i = rand() % 2;
                switch (i)
                {
                case 0:
                    if (Guy.Inventar[RAW_TREE_BRANCH] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHASTGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[RAW_TREE_BRANCH]++;
                    }
                    else PapierText = Renderer::DrawText(ROHASTZUVIEL, TXTPAPIER, 1);
                    break;
                case 1:
                    if (Guy.Inventar[RAW_LEAF] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHBLATTGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[RAW_LEAF]++;
                    }
                    else
                        PapierText = Renderer::DrawText(ROHBLATTZUVIEL, TXTPAPIER, 1);
                    break;
                }
            }
            else if (((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= TREE_1) && (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= TREE_BIG)) ||
                ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= HOUSE_1) && (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= HOUSE_3)))
            {
                i = rand() % 3;
                switch (i)
                {
                case 0:
                    if (Guy.Inventar[RAW_TREE_BRANCH] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHASTGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[RAW_TREE_BRANCH]++;
                    }
                    else
                        PapierText = Renderer::DrawText(ROHASTZUVIEL, TXTPAPIER, 1);

                    break;
                case 1:
                    if (Guy.Inventar[RAW_LEAF] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHBLATTGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[RAW_LEAF]++;
                    }
                    else
                        PapierText = Renderer::DrawText(ROHBLATTZUVIEL, TXTPAPIER, 1);

                    break;
                case 2:
                    if (Guy.Inventar[RAW_LIANA] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHLIANEGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[RAW_LIANA]++;
                    }
                    else
                        PapierText = Renderer::DrawText(ROHLIANEZUVIEL, TXTPAPIER, 1);

                    break;
                }
            }
            else if (BootsFahrt)
            {
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == WRECK_1)
                {
                    if (Guy.Inventar[RAW_TELESCOPE] == 0)
                    {
                        PapierText = Renderer::DrawText(FERNROHRGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[RAW_TELESCOPE] = 1;
                        Bmp[BUTTON_LOOK_OUT].Phase = 0;
                        Guy.Inventar[RAW_HAMMER] = 1;
                        Bmp[BUTTON_HOUSE_1].Phase = 0;
                        Bmp[BUTTON_HOUSE_2].Phase = 0;
                        Bmp[BUTTON_HOUSE_3].Phase = 0;
                    }
                    else
                        PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
                }
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == WRECK_2)
                {
                    if (Guy.Inventar[RAW_MAP] == 0)
                    {
                        PapierText = Renderer::DrawText(KARTEGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[RAW_MAP] = 1;
                        Bmp[BUTTON_TREASUREMAP].Phase = 0;
                        Guy.Inventar[RAW_SHOVEL] = 1;
                        Bmp[BUTTON_TREASURE].Phase = 0;
                    }
                    else
                        PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
                }
                else
                    PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
            }
            else
                PapierText = Renderer::DrawText(NICHTSGEFUNDEN, TXTPAPIER, 1);

            break;
        case 12:
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void eat()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite / 2,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe + 2);
            break;
        case 2: case 3:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_EAT;
            World::AddResource(NAHRUNG, 15);
            World::AddTime(0, 2);
            break;
        case 4:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 5:
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void slingshot()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite / 2 - 14,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe + 9);
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_SLINGSHOT;
            Guy.PosScreen.x += 5;
            World::AddTime(0, 2);
            PlaySound(Sound::SLINGSHOT, 100);
            break;
        case 3:
            Guy.PosScreen.x -= 5;
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite / 2 + 6,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe + 2);
            break;
        case 4:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_SEARCH;
            World::AddResource(NAHRUNG, 5);
            World::AddTime(0, 20);
            break;
        case 5:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 6:
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void drink()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Routing::ShortRoute(Guy.PosScreen.x - 4,
                                Guy.PosScreen.y - 2);
            break;
        case 2: case 3:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_DRINK;
            World::AddResource(WASSER, 30);
            World::AddTime(0, 3);
            break;
        case 4:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 5:
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void log()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite / 2 + 9,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe + 3);
            break;
        case 2: case 3: case 4: case 5: case 6:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_CHOP;
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 10);
            break;
        case 7:
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_WAITING;
                short i = Scape[Guy.Pos.x][Guy.Pos.y].Objekt + (TREE_DOWN_1 - TREE_1);
                Scape[Guy.Pos.x][Guy.Pos.y].Objekt = i;
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x -= 17;
                PlaySound(Sound::TIMBER, 100);
                break;
            }
        case 8:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 9:
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = -1;
            Guy.Inventar[RAW_TREE_TRUNK]++;
            if (Guy.Inventar[RAW_TREE_TRUNK] > 10) Guy.Inventar[RAW_TREE_TRUNK] = 10;
            Guy.Inventar[RAW_TREE_BRANCH] += 5;
            if (Guy.Inventar[RAW_TREE_BRANCH] > 10) Guy.Inventar[RAW_TREE_BRANCH] = 10;
            Guy.Inventar[RAW_LEAF] += 5;
            if (Guy.Inventar[RAW_LEAF] > 10) Guy.Inventar[RAW_LEAF] = 10;
            Guy.Inventar[RAW_LIANA] += 2;
            if (Guy.Inventar[RAW_LIANA] > 10) Guy.Inventar[RAW_LIANA] = 10;
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void fish()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
            {
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
            case RIVER_6: case RIVER_7: case RIVER_END_2: case RIVER_START_2: case FLOODGATE_2: case FLOODGATE_3:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 34,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 33);
                break;
            case RIVER_5: case RIVER_9: case RIVER_END_1: case RIVER_START_1: case FLOODGATE_1: case FLOODGATE_5:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 20,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 33);
                break;
            case RIVER_8: case RIVER_END_4: case RIVER_START_3: case FLOODGATE_4:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 22,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
                break;
            case RIVER_10: case RIVER_END_3: case RIVER_START_4: case FLOODGATE_6:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 32,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
                break;
            }
            break;
        case 2:
            Guy.Aktiv = true;
            PlaySound(Sound::FISH, 100);
            if (BootsFahrt)
            {
                Guy.PosScreen.y -= 2;
                Guy.Zustand = GUY_BOAT_FISHING_1;
            }
            switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
            {
            case RIVER_1: case RIVER_6: case RIVER_7: case RIVER_END_2: case RIVER_START_2: case FLOODGATE_2: case FLOODGATE_3:
                Guy.Zustand = GUY_FISHING_LEFT_1;
                break;
            case RIVER_2: case RIVER_5: case RIVER_9: case RIVER_END_1: case RIVER_START_1: case FLOODGATE_1: case FLOODGATE_5:
                Guy.Zustand = GUY_FISHING_ABOVE_1;
                break;
            case RIVER_3: case RIVER_8: case RIVER_END_4: case RIVER_START_3: case FLOODGATE_4:
                Guy.Zustand = GUY_FISHING_RIGHT_1;
                break;
            case RIVER_4: case RIVER_10: case RIVER_END_3: case RIVER_START_4: case FLOODGATE_6:
                Guy.Zustand = GUY_FISHING_BELOW_1;
                break;
            }
            break;
        case 3: case 4: case 5: case 6:
            Guy.Aktiv = true;
            if (BootsFahrt) Guy.Zustand = GUY_BOAT_FISHING_2;

            switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
            {
            case RIVER_1: case RIVER_6: case RIVER_7: case RIVER_END_2: case RIVER_START_2: case FLOODGATE_2: case FLOODGATE_3:
                Guy.Zustand = GUY_FISHING_LEFT_2;
                break;
            case RIVER_2: case RIVER_5: case RIVER_9: case RIVER_END_1: case RIVER_START_1: case FLOODGATE_1: case FLOODGATE_5:
                Guy.Zustand = GUY_FISHING_ABOVE_2;
                break;
            case RIVER_3: case RIVER_8: case RIVER_END_4: case RIVER_START_3: case FLOODGATE_4:
                Guy.Zustand = GUY_FISHING_RIGHT_2;
                break;
            case RIVER_4: case RIVER_10: case RIVER_END_3: case RIVER_START_4: case FLOODGATE_6:
                Guy.Zustand = GUY_FISHING_BELOW_2;
                break;
            }
            Guy.Resource[GESUNDHEIT] += 2;
            World::AddTime(0, 20);
            break;
        case 7:
            Guy.Aktiv = true;
            if (BootsFahrt) Guy.Zustand = GUY_BOAT_FISHING_3;

            switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
            {
            case RIVER_1: case RIVER_6: case RIVER_7: case RIVER_END_2: case RIVER_START_2: case FLOODGATE_2: case FLOODGATE_3:
                Guy.Zustand = GUY_FISHING_LEFT_3;
                break;
            case RIVER_2: case RIVER_5: case RIVER_9: case RIVER_END_1: case RIVER_START_1: case FLOODGATE_1: case FLOODGATE_5:
                Guy.Zustand = GUY_FISHING_ABOVE_3;
                break;
            case RIVER_3: case RIVER_8: case RIVER_END_4: case RIVER_START_3: case FLOODGATE_4:
                Guy.Zustand = GUY_FISHING_RIGHT_3;
                break;
            case RIVER_4: case RIVER_10: case RIVER_END_3: case RIVER_START_4: case FLOODGATE_6:
                Guy.Zustand = GUY_FISHING_BELOW_3;
                break;
            }
            break;
        case 8:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 9:
            Guy.Resource[NAHRUNG] += 20;
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void light()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite / 2 - 10,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe + 1);
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_SET_FIRE;
            Guy.PosScreen.x += 5;
            World::AddTime(0, 1);
            break;
        case 3:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_WAITING;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = FIRE;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[FIRE].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[FIRE].rcDes.top);
            Chance += 2 + 2 * Scape[Guy.Pos.x][Guy.Pos.y].Height;
            World::AddTime(0, 2);
            Guy.PosScreen.x -= 5;
            break;
        case 4:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 5:
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void lookout()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_LOOK_OUT;
            World::AddTime(0, 40);
            Chance += 1 + Scape[Guy.Pos.x][Guy.Pos.y].Height;
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_WAITING;
            World::AddTime(0, 40);
            break;
        case 3:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_LOOK_OUT;
            World::AddTime(0, 40);
            break;
        case 4:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 5:
            Chance -= 1 + Scape[Guy.Pos.x][Guy.Pos.y].Height;
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void treasure()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen;
        }
        Guy.AkNummer++;

        switch (Guy.AkNummer)
        {
        case 1:
            Guy.PosScreen.x -= 5;
            Guy.PosScreen.y += 1;
            Guy.Aktiv = true;
            Guy.Zustand = GUY_SHOVEL;
            break;
        case 2:
            World::AddTime(0, 20);
            World::AddResource(WASSER, -10);
            World::AddResource(NAHRUNG, -10);
            Guy.PosScreen.x += 5;
            Guy.PosScreen.y -= 1;
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            if (((Guy.Pos.x == SchatzPos.x) && (Guy.Pos.y == SchatzPos.y)) &&
                (!SchatzGef))
            {
                PapierText = Renderer::DrawText(SCHATZGEFUNDEN, TXTPAPIER, 1);
                Guy.Inventar[RAW_MATCH] = 1;
                Bmp[BUTTON_IGNITE].Phase = 0;
                SchatzGef = true;
            }
            else PapierText = Renderer::DrawText(KEINSCHATZ, TXTPAPIER, 1);
            break;
        case 3:
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void field()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            for (short i = 0; i < SPRITE_COUNT; i++)
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[FIELD].Rohstoff[i];
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = FIELD;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[FIELD].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[FIELD].rcDes.top);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[FIELD].Anzahl;
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckRohstoff())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 22,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 23);
            break;
        case 4:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 4;
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 25,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 21);
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 30);
            break;
        case 7:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 5;
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 28,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 19);
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 30);
            break;
        case 10:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 6;
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 31,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 17);
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 30);
            break;
        case 13:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 7;
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 34,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 15);
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 30);
            break;
        case 16:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 8;
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 36,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 13);
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 30);
            break;
        case 2: case 3: case 5: case 6: case 8: case 9: case 11: case 12: case 14: case 15: case 17: case 18:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_FARMING;
            break;
        case 19:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = FIELD;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[FIELD].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[FIELD].rcDes.top);
            break;
        case 20:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[BUTTON_STOP].Phase = -1;
            if (Bmp[FIELD].First)
            {
                PapierText = Renderer::DrawText(FELDHILFE, TXTPAPIER, 1);
                Bmp[FIELD].First = false;
            }
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void day_end()
    {
        ZWEID Erg;

        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Renderer::Fade(100, 90, 90);
            Stunden = 12;
            Minuten = 0;
            TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
            Bmp[BUTTON_STOP].Phase = -1;
            if ((Guy.Zustand == GUY_SLEEPING_TENT) || (Guy.Zustand == GUY_SLEEPING) ||
                (Guy.Zustand == GUY_SLEEP_HOUSE) || (BootsFahrt))
                break;
            Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x = Guy.PosScreen.x;
            Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y = Guy.PosScreen.y;
            Erg = Renderer::GetKachel(Guy.PosAlt.x, Guy.PosAlt.y);
            if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y)) Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            else if (RoutePunkt % 2 == 0) Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); // Nur bis zur Mitte der aktuellen Kacheln laufen
            else Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
            break;
        case 2:
            Renderer::Fade(95, 80, 80);
            Stunden = 12;
            Minuten = 0;
            if ((Guy.Zustand == GUY_SLEEPING_TENT) || (Guy.Zustand == GUY_SLEEPING) ||
                (Guy.Zustand == GUY_SLEEP_HOUSE) || (BootsFahrt))
                break;
            // Wohnbare Objekte in der Umgebung suchen 
            Erg.x = -1;
            Erg.y = -1;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) || (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3))
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Objekt == HOUSE_3)
            {
                Erg.x = Guy.Pos.x - 1;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Objekt == HOUSE_3)
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y - 1;
            }
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Objekt == HOUSE_3)
            {
                Erg.x = Guy.Pos.x + 1;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Objekt == HOUSE_3)
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y + 1;
            }
            else if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Objekt == TENT)
            {
                Erg.x = Guy.Pos.x - 1;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Objekt == TENT)
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y - 1;
            }
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Objekt == TENT)
            {
                Erg.x = Guy.Pos.x + 1;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Objekt == TENT)
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y + 1;
            }
            if ((Erg.x != -1) && (Erg.y != -1))
            {
                Guy.Pos.x = Erg.x;
                Guy.Pos.y = Erg.y;
                if ((Scape[Erg.x][Erg.y].Objekt == TENT) &&
                    (Scape[Erg.x][Erg.y].Phase < Bmp[Scape[Erg.x][Erg.y].Objekt].Anzahl))
                    Routing::ShortRoute(Scape[Erg.x][Erg.y].xScreen + Scape[Erg.x][Erg.y].ObPos.x + 3,
                                        Scape[Erg.x][Erg.y].yScreen + Scape[Erg.x][Erg.y].ObPos.y + 20);
                else if ((Scape[Erg.x][Erg.y].Objekt == HOUSE_3) &&
                    (Scape[Erg.x][Erg.y].Phase < Bmp[Scape[Erg.x][Erg.y].Objekt].Anzahl))
                    Routing::ShortRoute(Scape[Erg.x][Erg.y].xScreen + Scape[Erg.x][Erg.y].ObPos.x +
                                        Bmp[TREE_BIG].Breite / 2,
                                        Scape[Erg.x][Erg.y].yScreen + Scape[Erg.x][Erg.y].ObPos.y +
                                        Bmp[TREE_BIG].Hoehe + 1);
            }
            break;
        case 3:
            Renderer::Fade(90, 70, 70);
            Stunden = 12;
            Minuten = 0;
            if ((Guy.Zustand == GUY_SLEEPING_TENT) || (Guy.Zustand == GUY_SLEEPING) ||
                (Guy.Zustand == GUY_SLEEP_HOUSE) || (BootsFahrt))
                break;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_CLIMBING_1;
            }
            break;
        case 4:
            Renderer::Fade(70, 60, 60);
            Stunden = 12;
            Minuten = 0;
            if ((Guy.Zustand == GUY_SLEEPING_TENT) || (Guy.Zustand == GUY_SLEEPING) ||
                (Guy.Zustand == GUY_SLEEP_HOUSE) || (BootsFahrt))
                break;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_ENTER_TENT;
            }
            else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_ENTER_HOUSE;
            }
            else
            {
                Guy.PosScreen.x += 3;
                Guy.Aktiv = true;
                Guy.Zustand = GUY_LAYING_TENT;
            }
            break;
        case 5:
            Renderer::Fade(55, 50, 55);
            Stunden = 12;
            Minuten = 0;
            if (BootsFahrt) break;
            Guy.Aktiv = true;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                if (Guy.Zustand != GUY_SLEEPING_TENT) Guy.PosScreen.x += 4;
                Guy.Zustand = GUY_SLEEPING_TENT;
            }
            else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                if (Guy.Zustand != GUY_SLEEP_HOUSE) Guy.PosScreen.x += 14;
                Guy.Zustand = GUY_SLEEP_HOUSE;
            }
            else Guy.Zustand = GUY_SLEEPING;
            break;
        case 6:
            Renderer::Fade(25, 25, 35);
            Stunden = 12;
            Minuten = 0;
            if (BootsFahrt) break;
            Guy.Aktiv = true;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
                Guy.Zustand = GUY_SLEEPING_TENT;
            else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
                Guy.Zustand = GUY_SLEEP_HOUSE;
            else Guy.Zustand = GUY_SLEEPING;
            break;
        case 7:
            Renderer::Fade(0, 0, 0); // Nicht verwirren lassen, da das Bild in Zeige() schwarz übermalt wird
            Nacht = true;
            Stunden = 12;
            Minuten = 0;
            PlaySound(Sound::WOLF, 100);
            // Falsche Objekte Löschen
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= TREE_DOWN_1) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= TREE_DOWN_4))
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Objekt = -1;
                Guy.Inventar[RAW_TREE_TRUNK]++;
                if (Guy.Inventar[RAW_TREE_TRUNK] > 10) Guy.Inventar[RAW_TREE_TRUNK] = 10;
            }

            // Je nach Schlafort Zustand verändern
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                World::AddResource(GESUNDHEIT, -5);
                if (Guy.Resource[GESUNDHEIT] <= 0)
                {
                    Guy.Aktiv = true;
                    PapierText = Renderer::DrawText(TAGENDE5, TXTPAPIER, 1);
                    Guy.AkNummer = 2;
                    Guy.Aktion = DEATH;
                    Stunden = 0;
                    Minuten = 0;
                }
                else
                {
                    Guy.Aktiv = true;
                    PapierText = Renderer::DrawText(TAGENDE2, TXTPAPIER, 1);
                }
            }
            else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                World::AddResource(GESUNDHEIT, +20);
                Guy.Aktiv = true;
                PapierText = Renderer::DrawText(TAGENDE4, TXTPAPIER, 1);
            }
            else if (BootsFahrt)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_BOAT_WAITING;
                PapierText = Renderer::DrawText(TAGENDE3, TXTPAPIER, 1);
                Guy.AkNummer = 2;
                Guy.Aktion = DEATH;
                Stunden = 0;
                Minuten = 0;
            }
            else
            {
                World::AddResource(GESUNDHEIT, -20);
                if (Guy.Resource[GESUNDHEIT] <= 0)
                {
                    Guy.Aktiv = true;
                    PapierText = Renderer::DrawText(TAGENDE5, TXTPAPIER, 1);
                    Guy.AkNummer = 2;
                    Guy.Aktion = DEATH;
                    Stunden = 0;
                    Minuten = 0;
                }
                else
                {
                    Guy.Aktiv = true;
                    PapierText = Renderer::DrawText(TAGENDE1, TXTPAPIER, 1);
                }
            }
            break;
        case 8:
            Renderer::Fade(20, 20, 30);
            Nacht = false;
            Tag++;
            Stunden = 0;
            Minuten = 0;
            // if (BootsFahrt) Game::NeuesSpiel(true); // Später hier tot!!

            Guy.Aktiv = true;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
                Guy.Zustand = GUY_SLEEPING_TENT;
            else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
                Guy.Zustand = GUY_SLEEP_HOUSE;
            else Guy.Zustand = GUY_SLEEPING;
            break;
        case 9:
            Renderer::Fade(40, 40, 40);
            Stunden = 0;
            Minuten = 0;

            Stunden = 0;
            Minuten = 0;
            Guy.Aktiv = true;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
                Guy.Zustand = GUY_SLEEPING_TENT;
            else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
                Guy.Zustand = GUY_SLEEP_HOUSE;
            else Guy.Zustand = GUY_SLEEPING;
            break;
        case 10:
            Renderer::Fade(70, 60, 60);
            Stunden = 0;
            Minuten = 0;
            StopSound(Sound::SNORE);
            Guy.Aktiv = true;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.PosScreen.x -= 14;
                Guy.Zustand = GUY_EXIT_HOUSE;
            }
            else Guy.Zustand = GUY_STAND_UP;
            break;
        case 11:
            Renderer::Fade(90, 80, 80);
            Stunden = 0;
            Minuten = 0;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_CLIMBING_2;
            }
            break;
        case 12:
            Renderer::Fade(100, 100, 100);
            Stunden = 0;
            Minuten = 0;
            Guy.Zustand = GUY_LEFT;
            Guy.Aktion = NOTHING;
            if (Guy.Resource[GESUNDHEIT] > 10) Game::SaveGame();
            break;
        }
    }

    void rescued()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1: {
            ZWEID Erg = Renderer::GetKachel(Guy.PosAlt.x, Guy.PosAlt.y);
            if ((Erg.x == Guy.Pos.x) && (Erg.y == Guy.Pos.y)) Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            else if (RoutePunkt % 2 == 0) Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); //Nur bis zur Mitte der aktuellen Kacheln laufen
            else Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
            TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
            break;
        }
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_WAITING;
            PapierText = Renderer::DrawText(GERETTET, TXTPAPIER, 1);
            break;
        case 3:
            if (Frage == 2)
            {
                Guy.Aktion = NOTHING;
                Frage = -1;
                break;
            }
            Spielzustand = State::RESCUED;
            Frage = -1;
            break;
        case 4:
            // Route herstellen
            Guy.Aktiv = true;
            Guy.Zustand = GUY_LEFT;
            RoutePunkt = -1;
            Steps = 0;
            Step = 0;
            RouteStart.x = Guy.Pos.x;
            RouteStart.y = Guy.Pos.y;
            RouteZiel.y = Guy.Pos.y;
            for (short x = MAX_TILES_X - 1; x > 1; x--) // Position des Rettungsschiffs festlegen
            {
                if (Scape[x][Guy.Pos.y].Art != 1) break;
                RouteZiel.x = x + 1;
            }
            // Schiff hinbauen
            Scape[RouteZiel.x][RouteZiel.y].Phase = 0;
            Scape[RouteZiel.x][RouteZiel.y].Objekt = GUY_SHIP;
            Scape[RouteZiel.x][RouteZiel.y].ObPos.x = 10;
            Scape[RouteZiel.x][RouteZiel.y].ObPos.y = 10;
            RouteZiel.x -= 2;
            Routing::FindTheWay();
            Guy.Zustand = GUY_LEFT;
            break;
        case 5:
            Guy.Zustand = GUY_LEFT;
            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x +
                                    Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].y +
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
            break;
        case 6:
            Guy.Pos.x += 2;
            Guy.Zustand = GUY_SWIM;
            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                                    Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
            break;
        case 7:
            Guy.PosScreen.y -= 10;
            if (!BootsFahrt) World::ChangeBootsFahrt();
            Guy.Aktiv = true;
            Guy.Zustand = GUY_SHIP;
            RoutePunkt = -1;
            Steps = 0;
            Step = 0;
            RouteStart.x = Guy.Pos.x;
            RouteStart.y = Guy.Pos.y;
            RouteZiel.y = Guy.Pos.y;
            RouteZiel.x = MAX_TILES_X - 2;
            Routing::FindTheWay();
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[SEA_WAVES].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[SEA_WAVES].rcDes.top);
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = SEA_WAVES;
            break;
        case 8:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_SHIP;
            break;
        case 9:
            Guy.Aktion = NOTHING;
            Guy.Zustand = GUY_LEFT;
            Spielzustand = State::OUTRO;
            break;
        }
    }

    void tent()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = TENT;
            for (short i = 0; i < SPRITE_COUNT; i++)
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[TENT].Rohstoff[i];
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[TENT].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[TENT].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckRohstoff())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 22,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 12);
            break;
        case 2: case 3: case 12: case 13:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_TIE_DOWN;
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 15);
            break;
        case 4:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 2;
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 31,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 20);
            break;
        case 5:
            Routing::ShortRoute(Guy.PosAlt.x,
                                Guy.PosAlt.y);
            break;
        case 6:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 3,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 20);

            break;
        case 7: case 8:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_TIE_UP;
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 15);
            break;
        case 9:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 3;
            Routing::ShortRoute(Guy.PosAlt.x,
                                Guy.PosAlt.y);
            break;
        case 10:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 31,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 20);
            break;
        case 11:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 22,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 12);
            break;
        case 14:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 31,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 20);
            break;
        case 15:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 16:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[BUTTON_STOP].Phase = -1;
            if (Bmp[TENT].First)
            {
                PapierText = Renderer::DrawText(ZELTHILFE, TXTPAPIER, 1);
                Bmp[TENT].First = false;
            }
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void boat()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = BOAT;
            for (short i = 0; i < SPRITE_COUNT; i++)
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[BOAT].Rohstoff[i];
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[BOAT].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[BOAT].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[BOAT].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckRohstoff())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 30,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 21);
            break;
        case 2:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 29,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 20);
            break;
        case 3:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 28,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 19);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[BOAT].Anzahl + 1);
            break;
        case 4: case 5: case 6: case 8: case 9: case 10: case 12: case 13: case 14:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_BEAT;
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 15);
            break;
        case 7:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 22,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 16);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[BOAT].Anzahl + 2);
            break;
        case 11:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 14,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 11);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[BOAT].Anzahl + 3);
            break;
        case 15:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 16:
            if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Art == 1)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = 0;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = 10;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Art == 1)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 1;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = 25;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = 10;
            }
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Art == 1)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = 30;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = 27;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Art == 1)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 1;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = 0;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = 28;
            }
            Bmp[BUTTON_STOP].Phase = -1;
            if (Bmp[BOAT].First)
            {
                PapierText = Renderer::DrawText(BOOTHILFE, TXTPAPIER, 1);
                Bmp[BOAT].First = false;
            }
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void pipe()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = PIPE;
            for (short i = 0; i < SPRITE_COUNT; i++)
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[PIPE].Rohstoff[i];
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[PIPE].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[PIPE].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[PIPE].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckRohstoff())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 30,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 21);
            break;
        case 2:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 29,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 20);
            break;
        case 3:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 28,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 15);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[PIPE].Anzahl + 1);
            break;
        case 4: case 5: case 6: case 11: case 12: case 13:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_BEAT;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 5);
            break;
        case 7: case 8: case 9: case 14: case 15: case 16:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_CHOP;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 5);
            break;
        case 10:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 17,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 13);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[PIPE].Anzahl + 2);
            break;
        case 17:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 18:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            World::FillRohr();
            Bmp[BUTTON_STOP].Phase = -1;
            if (Bmp[PIPE].First)
            {
                PapierText = Renderer::DrawText(ROHRHILFE, TXTPAPIER, 1);
                Bmp[PIPE].First = false;
            }
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void sos()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = SOS;
            for (short i = 0; i < SPRITE_COUNT; i++)
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[SOS].Rohstoff[i];
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[SOS].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[SOS].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[SOS].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckRohstoff())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 4,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 13);
            break;
        case 4:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 12,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 17);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[SOS].Anzahl + 1);
            break;
        case 7:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 12,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 9);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[SOS].Anzahl + 2);
            break;
        case 10:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 19,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 12);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[SOS].Anzahl + 3);
            break;
        case 13:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 21,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 5);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[SOS].Anzahl + 4);
            break;
        case 16:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 28,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 8);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[SOS].Anzahl + 5);
            break;
        case 2: case 5: case 8: case 11: case 14: case 17:
            Guy.Aktiv = true;
            Guy.PosScreen.x += 4;
            Guy.Zustand = GUY_LAYING_TENT;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 1);
            break;
        case 3: case 6: case 9: case 12: case 15: case 18:
            Guy.Aktiv = true;
            Guy.PosScreen.x -= 4;
            Guy.Zustand = GUY_STAND_UP;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 1);
            break;
        case 19:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 20:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Art == 0) || (Scape[Guy.Pos.x][Guy.Pos.y].Art == 4))
                Chance += 1;
            else Chance += 2; // Dürfte nur noch der Strand übrig sein
            Bmp[BUTTON_STOP].Phase = -1;
            if (Bmp[SOS].First)
            {
                PapierText = Renderer::DrawText(SOSHILFE, TXTPAPIER, 1);
                Bmp[SOS].First = false;
            }
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void fireplace()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = BONFIRE;
            for (short i = 0; i < SPRITE_COUNT; i++)
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[BONFIRE].Rohstoff[i];
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[BONFIRE].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[BONFIRE].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[BONFIRE].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckRohstoff())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 4,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 16);
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.PosScreen.x += 4;
            Guy.Zustand = GUY_LAYING_TENT;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 1);
            break;
        case 3:
            Guy.Aktiv = true;
            Guy.PosScreen.x -= 4;
            Guy.Zustand = GUY_STAND_UP;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 1);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[BONFIRE].Anzahl + 1);
            break;
        case 4:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 15);
            break;
        case 5: case 6: case 7:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_TIE_UP;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 1);
            if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer != 5)
                Scape[Guy.Pos.x][Guy.Pos.y].Phase =
                    static_cast<short>(Bmp[BONFIRE].Anzahl + Scape[Guy.Pos.x][Guy.Pos.y].AkNummer - 4);
            break;
        case 8:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 9:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[BUTTON_STOP].Phase = -1;
            if (Bmp[BONFIRE].First)
            {
                PapierText = Renderer::DrawText(FEUERSTELLEHILFE, TXTPAPIER, 1);
                Bmp[BONFIRE].First = false;
            }
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void house1()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            for (short i = 0; i < SPRITE_COUNT; i++)
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[HOUSE_1].Rohstoff[i];
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[HOUSE_1].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = HOUSE_1;
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckRohstoff())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                                Bmp[TREE_BIG].Breite / 2 - 3,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                                Bmp[TREE_BIG].Hoehe + 1);
            break;
        case 2: case 3: case 4: case 5:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_1;
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 6: case 7: case 8: case 9:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_1;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_1].Anzahl + 1);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 10: case 11: case 12: case 13:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_1;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_1].Anzahl + 2);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 14: case 15: case 16: case 17:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_1;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_1].Anzahl + 3);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 18:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 19:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[BUTTON_STOP].Phase = -1;
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void house2()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            for (short i = 0; i < SPRITE_COUNT; i++)
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[HOUSE_2].Rohstoff[i];
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[HOUSE_2].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = HOUSE_2;
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckRohstoff())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                                Bmp[TREE_BIG].Breite / 2,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                                Bmp[TREE_BIG].Hoehe + 1);
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_CLIMBING_1;
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
            World::AddTime(0, 1);
            break;
        case 3: case 4: case 5: case 6:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_2;
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 7: case 8: case 9: case 10:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_2].Anzahl + 1);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 11: case 12: case 13: case 14:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_2].Anzahl + 2);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 15: case 16: case 17: case 18:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_2].Anzahl + 3);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 19:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_CLIMBING_2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_2].Anzahl + 4);
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
            World::AddTime(0, 1);
            break;
        case 20:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 21:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[BUTTON_STOP].Phase = -1;
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void house3()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            for (short i = 0; i < SPRITE_COUNT; i++)
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[HOUSE_3].Rohstoff[i];
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[HOUSE_3].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = HOUSE_3;
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckRohstoff())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                                Bmp[TREE_BIG].Breite / 2,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                                Bmp[TREE_BIG].Hoehe + 1);
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_CLIMBING_1;
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
            World::AddTime(0, 1);
            break;
        case 3: case 4: case 5: case 6:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_2;
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 7: case 8: case 9: case 10:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_3].Anzahl + 1);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 11: case 12: case 13: case 14:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_3].Anzahl + 2);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 15: case 16: case 17: case 18:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_HAMMER_2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_3].Anzahl + 3);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 19:
            Guy.Aktiv = true;
            Guy.Zustand = GUY_CLIMBING_2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[HOUSE_3].Anzahl + 4);
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
            World::AddTime(0, 1);
            break;
        case 20:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 21:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[BUTTON_STOP].Phase = -1;
            if (Bmp[HOUSE_3].First)
            {
                PapierText = Renderer::DrawText(HAUS3HILFE, TXTPAPIER, 1);
                Bmp[HOUSE_3].First = false;
            }
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void sleep()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 3,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 20);
            else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                                    Bmp[TREE_BIG].Breite / 2 + 1,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                                    Bmp[TREE_BIG].Hoehe + 1);
            break;
        case 2:
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_CLIMBING_1;
                World::AddResource(NAHRUNG, -1);
                World::AddResource(WASSER, -1);
            }
            break;
        case 3:
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_ENTER_TENT;
            }
            else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_ENTER_HOUSE;
            }
            else
            {
                Guy.PosScreen.x += 3;
                Guy.Aktiv = true;
                Guy.Zustand = GUY_LAYING_TENT;
            }
            break;
        case 4: case 5:
            Guy.Aktiv = true;
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == TENT) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                if (Guy.AkNummer == 4) Guy.PosScreen.x += 4;
                Guy.Zustand = GUY_SLEEPING_TENT;
            }
            else if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                if (Guy.AkNummer == 4) Guy.PosScreen.x += 14;
                Guy.Zustand = GUY_SLEEP_HOUSE;
            }
            else Guy.Zustand = GUY_SLEEPING;
            World::AddResource(GESUNDHEIT, 5);
            World::AddTime(0, 30);
            break;
        case 6:
            Guy.Aktiv = true;
            StopSound(Sound::SNORE);
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.PosScreen.x -= 14;
                Guy.Zustand = GUY_EXIT_HOUSE;
            }
            else Guy.Zustand = GUY_STAND_UP;
            break;
        case 7:
            if ((Scape[Guy.Pos.x][Guy.Pos.y].Objekt == HOUSE_3) &&
                (Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl))
            {
                Guy.Aktiv = true;
                Guy.Zustand = GUY_CLIMBING_2;
                World::AddResource(NAHRUNG, -1);
                World::AddResource(WASSER, -1);
            }
            break;
        case 8:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            Guy.Aktion = NOTHING;
            break;
        }
    }

    void undock()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 14,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 11);
            break;
        case 2:
            World::ChangeBootsFahrt();
            Guy.PosScreen.x = Scape[Guy.Pos.x][Guy.Pos.y].xScreen +
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite / 2;
            Guy.PosScreen.y = Scape[Guy.Pos.x][Guy.Pos.y].yScreen +
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe / 2;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = -1;
            if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Art == 1) Guy.Pos.x--;
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Art == 1) Guy.Pos.y--;
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Art == 1) Guy.Pos.x++;
            else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Art == 1) Guy.Pos.y++;
            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                                    Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));

            break;
        case 3:
            Guy.Aktion = NOTHING;
            Guy.PosAlt.x = Guy.PosScreen.x;
            Guy.PosAlt.y = Guy.PosScreen.y;
            break;
        }
    }

    void dock()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Art != 1)
            {
                Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                                        Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].x) / 2),
                                    ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].y +
                                        Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y) / 2));
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Art != 1)
            {
                Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].x +
                                        Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                                    ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                                        Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].y) / 2));
            }
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Art != 1)
            {
                Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x +
                                        Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].x) / 2),
                                    ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].y +
                                        Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Art != 1)
            {
                Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].x +
                                        Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x) / 2),
                                    ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y +
                                        Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].y) / 2));
            }
            break;
        case 2:
            if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Art != 1)
            {
                Guy.Pos.x--;
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Art != 1)
            {
                Guy.Pos.y--;
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 1;
            }
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Art != 1)
            {
                Guy.Pos.x++;
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Art != 1)
            {
                Guy.Pos.y++;
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 1;
            }

            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = BOAT;
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = Bmp[BOAT].AkAnzahl;

            World::ChangeBootsFahrt();
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = Guy.PosScreen.x -
                Scape[Guy.Pos.x][Guy.Pos.y].xScreen -
                Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite / 2;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = Guy.PosScreen.y -
                Scape[Guy.Pos.x][Guy.Pos.y].yScreen -
                Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe / 2;

            Routing::ShortRoute(((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                                    Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2),
                                ((Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2));

            break;
        case 3:
            Guy.Aktion = NOTHING;
            Guy.PosAlt.x = Guy.PosScreen.x;
            Guy.PosAlt.y = Guy.PosScreen.y;
            break;
        }
    }
} // namesapce Action
