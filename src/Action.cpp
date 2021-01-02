#include "Action.hpp"

#include "types.hpp"
#include "extern.hpp"

#include "Game.hpp"
#include "Renderer.hpp"
#include "Routing.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

namespace Action
{
    void Handler(Actions event)
    {
        if (event != Actions::Nothing)
        {
            Routing::MarkRoute(false);
            RouteZiel.x = -1;
            RouteZiel.y = -1;
        }

        switch (event)
        {
        case Actions::Nothing:
            break;
        case Actions::Search:
            Search();
            break;
        case Actions::Eat:
            Eat();
            break;
        case Actions::Drink:
            Drink();
            break;
        case Actions::Log:
            Log();
            break;
        case Actions::Field:
            Field();
            break;
        case Actions::DayEnd:
            DayEnd();
            break;
        case Actions::Rescued:
            Rescued();
            break;
        case Actions::Tent:
            Tent();
            break;
        case Actions::Sleep:
            Sleep();
            break;
        case Actions::Cancel:
            Cancel();
            break;
        case Actions::Fish:
            Fish();
            break;
        case Actions::Boat:
            Boat();
            break;
        case Actions::Undock:
            Undock();
            break;
        case Actions::Dock:
            Dock();
            break;
        case Actions::Pipe:
            Pipe();
            break;
        case Actions::Destroy:
            Destroy();
            break;
        case Actions::SosSign:
            SosSign();
            break;
        case Actions::House1:
            House1();
            break;
        case Actions::House2:
            House2();
            break;
        case Actions::House3:
            House3();
            break;
        case Actions::Fireplace:
            Fireplace();
            break;
        case Actions::Light:
            Light();
            break;
        case Actions::Lookout:
            Lookout();
            break;
        case Actions::Treasure:
            Treasure();
            break;
        case Actions::Intro:
            Intro();
            break;
        case Actions::Slingshot:
            Slingshot();
            break;
        case Actions::Quit:
            Quit();
            break;
        case Actions::Restart:
            Restart();
            break;
        case Actions::DayRestart:
            RestartDay();
            break;
        case Actions::Death:
            Death();
            break;
        }
    }

    void Intro()
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
            for (short x = Guy.Pos.x; x < MAXXKACH; x++) // Zielkoordinate für Introroute finden
            {
                if (Scape[x][Guy.Pos.y].Art != 1)
                    break;
                RouteZiel.x = x - 1;
            }
            Routing::FindTheWay();
            break;
        case 2:
            Guy.PosScreen.y -= 10;
            Guy.Aktiv = true;
            Guy.Zustand = GuySchiffDown;
            PlaySound(Sound::Splat, 100);
            PlaySound(Sound::Crash, 100);
            break;
        case 3:
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Wrack;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Wrack].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Wrack].rcDes.top);

            World::ChangeBoatRide();
            Guy.Pos.x += 2;
            Guy.PosScreen.y += 10;
            Guy.Zustand = GuySchwimmen;
            Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
                                 Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].x) / 2,
                                (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].y +
                                 Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y) / 2);
            break;
        case 4:
            StopSound(Sound::Swim); // Sound hier sofort stoppen
            Guy.Zustand = GuyLinks;
            Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
                                 Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x) / 2,
                                (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y +
                                 Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2);
            break;
        case 5:
            Guy.PosAlt = Guy.PosScreen;
            Spielzustand = State::Game;
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            PapierText = Renderer::DrawText(INTROTEXT, TXTPAPIER, 1);
            Game::SaveGame();
            break;
        }
    }

    void Restart()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            ZWEID Erg = Renderer::GetTile(Guy.PosAlt.x, Guy.PosAlt.y);
            if (Erg.x == Guy.Pos.x && Erg.y == Guy.Pos.y)
            {
                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            }
            else if (RoutePunkt % 2 == 0)
                Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); // Nur bis zur Mitte der aktuellen Kacheln laufen
            else
            {
                Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
            }
            TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
            break;
        case 2:
            Guy.Aktiv = true;
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootWarten;
            }
            else
            {
                Guy.Zustand = GuyWarten;
            }

            PapierText = Renderer::DrawText(NEUBEGINNEN, TXTPAPIER, 1);
            break;
        case 3:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            if (BootsFahrt)
                Guy.Zustand = GuyBootLinks;
            else
                Guy.Zustand = GuyLinks;

            if (Frage == 1)
            {
                Game::NewGame(true);
                return;
            }
            Frage = -1;
            break;
        }
    }

    void RestartDay()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            ZWEID Erg = Renderer::GetTile(Guy.PosAlt.x, Guy.PosAlt.y);
            if (Erg.x == Guy.Pos.x && Erg.y == Guy.Pos.y)
            {
                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            }
            else if (RoutePunkt % 2 == 0)
            {
                Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); // Nur bis zur Mitte der aktuellen Kacheln laufen
            }
            else
            {
                Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
            }

            TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
            break;
        case 2:
            Guy.Aktiv = true;
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootWarten;
            }
            else
            {
                Guy.Zustand = GuyWarten;
            }

            PapierText = Renderer::DrawText(TAGNEU, TXTPAPIER, 1);
            break;
        case 3:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootLinks;
            }
            else
            {
                Guy.Zustand = GuyLinks;
            }

            if (Frage == 1)
            {
                Game::NewGame(false);
                return;
            }
            Frage = -1;
            break;
        }
    }

    void Quit()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            ZWEID Erg = Renderer::GetTile(Guy.PosAlt.x, Guy.PosAlt.y);
            if (Erg.x == Guy.Pos.x && Erg.y == Guy.Pos.y)
            {
                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            }
            else if (RoutePunkt % 2 == 0)
            {
                Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); // Nur bis zur Mitte der aktuellen Kacheln laufen
            }
            else
            {
                Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
            }

            TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
            break;
        case 2:
            Guy.Aktiv = true;
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootWarten;
            }
            else
            {
                Guy.Zustand = GuyWarten;
            }

            PapierText = Renderer::DrawText(SPIELVERLASSEN, TXTPAPIER, 1);
            break;
        case 3:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootLinks;
            }
            else
            {
                Guy.Zustand = GuyLinks;
            }

            if (Frage == 1)
            {
                if (Guy.Resource[GESUNDHEIT] > 10)
                {
                    Game::SaveGame();
                }

                Spielzustand = State::Outro;
            }
            Frage = -1;
            break;
        }
    }

    void Death()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Guy.Aktiv = true;
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootWarten;
            }
            else
            {
                Guy.Zustand = GuyWarten;
            }

            PapierText = Renderer::DrawText(TOD, TXTPAPIER, 1);
            break;
        case 2:
            if (!BootsFahrt)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyHinlegen;
            }
            break;
        case 3:
            Guy.Aktiv = true;
            Nacht = false;
            Renderer::Fade(100, 100, 100);
            if (BootsFahrt)
                Guy.Zustand = GuyBootTod;
            else
                Guy.Zustand = GuyTod;

            break;
        case 4:
            Guy.Aktiv = true;
            Nacht = true;
            Guy.Zustand = GuyWarten;
            PapierText = Renderer::DrawText(TAGNEU, TXTPAPIER, 1);
            break;
        case 5:
            Nacht = false;
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootLinks;
            }
            else
            {
                Guy.Zustand = GuyLinks;
            }

            Guy.Aktion = static_cast<short>(Actions::Nothing);
            if (Frage == 2)
            {
                Spielzustand = State::Outro;
            }
            else
            {
                Game::NewGame(false);
            }

            Frage = -1;
            break;
        }
    }

    void Cancel()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x = Guy.PosScreen.x;
            Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y = Guy.PosScreen.y;

            Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
                                 Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x) / 2,
                                (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y +
                                 Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2);
            break;
        case 2:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Destroy()
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
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite + 4,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y
                                + Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe);
            break;
        case 2:
        case 4:
            Guy.Aktiv = true;
            Guy.Zustand = GuyFaellen;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 5);
            break;
        case 3:
        case 5:
            Guy.Aktiv = true;
            Guy.Zustand = GuySchlagen;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 5);
            break;
        case 6:
            {
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Sos)
                {
                    Chance -= 0.1f;
                }

                short i = Scape[Guy.Pos.x][Guy.Pos.y].Objekt; // Um sich kurz das Objekt zu merken 

                if (i >= Haus1 && i <= Haus3)
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt = BaumGross;
                }
                else
                {
                    Scape[Guy.Pos.x][Guy.Pos.y].Objekt = -1;
                    Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = 0;
                    Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = 0;
                    Scape[Guy.Pos.x][Guy.Pos.y].Phase = -1;
                }
                Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = 0;

                if (i == Rohr)
                {
                    World::FillPipe();
                }

                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
                break;
            }
        case 7:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Search()
    {
        ZWEID Ziel;
        short i;

        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        while (true)
        {
            Ziel.x = Scape[Guy.Pos.x][Guy.Pos.y].xScreen + rand() % KXPIXEL;
            Ziel.y = Scape[Guy.Pos.x][Guy.Pos.y].yScreen + rand() % KYPIXEL;
            ZWEID Erg = Renderer::GetTile(Ziel.x, Ziel.y);
            if (Erg.x == Guy.Pos.x && Erg.y == Guy.Pos.y)
            {
                break; // Wenn das gefundene Ziel in der Kachel, dann fertig
            }
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
        case 3:
        case 5:
        case 7:
            if (BootsFahrt)
            {
                if (Guy.AkNummer == 1)
                {
                    Guy.Aktiv = true;
                    Guy.PosScreen.y -= 2;
                    Guy.Zustand = GuyTauchen1;
                    PlaySound(Sound::Splat, 100);
                }
            }
            else
            {
                Routing::ShortRoute(Ziel.x, Ziel.y);
            }

            break;
        case 2:
        case 4:
        case 6:
        case 8:
            Guy.Aktiv = true;
            if (BootsFahrt)
            {
                if (Guy.AkNummer == 2)
                {
                    Guy.PosScreen.y += 5;
                }

                Guy.Zustand = GuyTauchen2;
            }
            else
            {
                Guy.Zustand = GuySuchen;
            }

            World::AddTime(0, 4);
            break;
        case 9:
            if (BootsFahrt)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyTauchen3;
                PlaySound(Sound::Splat, 100);
            }
            break;
        case 10:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 11:
            Guy.Aktiv = true;
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootLinks;
            }

            // Auf Strand und Fluss
            if (Scape[Guy.Pos.x][Guy.Pos.y].Art == 2 || Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Fluss1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Schleuse6)
            {
                if (Guy.Inventar[ResourceStone] < 10)
                {
                    PapierText = Renderer::DrawText(ROHSTEINGEFUNDEN, TXTPAPIER, 1);
                    Guy.Inventar[ResourceStone] += 3;

                    if (Guy.Inventar[ResourceStone] > 10)
                    {
                        Guy.Inventar[ResourceStone] = 10;
                    }
                }
                else
                {
                    PapierText = Renderer::DrawText(ROHSTEINZUVIEL, TXTPAPIER, 1);
                }
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Busch)
            {
                i = rand() % 2;
                switch (i)
                {
                case 0:
                    if (Guy.Inventar[ResourceBranch] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHASTGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[ResourceBranch]++;
                    }
                    else
                    {
                        PapierText = Renderer::DrawText(ROHASTZUVIEL, TXTPAPIER, 1);
                    }
                    break;
                case 1:
                    if (Guy.Inventar[ResourceLeaf] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHBLATTGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[ResourceLeaf]++;
                    }
                    else
                    {
                        PapierText = Renderer::DrawText(ROHBLATTZUVIEL, TXTPAPIER, 1);
                    }
                    break;
                }
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Baum1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= BaumGross ||
                     Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Haus1 && Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Haus3)
            {
                i = rand() % 3;
                switch (i)
                {
                case 0:
                    if (Guy.Inventar[ResourceBranch] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHASTGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[ResourceBranch]++;
                    }
                    else
                    {
                        PapierText = Renderer::DrawText(ROHASTZUVIEL, TXTPAPIER, 1);
                    }

                    break;
                case 1:
                    if (Guy.Inventar[ResourceLeaf] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHBLATTGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[ResourceLeaf]++;
                    }
                    else
                    {
                        PapierText = Renderer::DrawText(ROHBLATTZUVIEL, TXTPAPIER, 1);
                    }

                    break;
                case 2:
                    if (Guy.Inventar[ResourceVine] < 10)
                    {
                        PapierText = Renderer::DrawText(ROHLIANEGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[ResourceVine]++;
                    }
                    else
                    {
                        PapierText = Renderer::DrawText(ROHLIANEZUVIEL, TXTPAPIER, 1);
                    }

                    break;
                }
            }
            else if (BootsFahrt)
            {
                if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Wrack)
                {
                    if (Guy.Inventar[ResourceTelescope] == 0)
                    {
                        PapierText = Renderer::DrawText(FERNROHRGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[ResourceTelescope] = 1;
                        Bmp[ButtonLook].Phase = 0;
                        Guy.Inventar[ResourceHammer] = 1;
                        Bmp[ButtonHouse1].Phase = 0;
                        Bmp[ButtonHouse2].Phase = 0;
                        Bmp[ButtonHouse3].Phase = 0;
                    }
                    else
                    {
                        PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
                    }
                }
                else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Wrack2)
                {
                    if (Guy.Inventar[ResourceMap] == 0)
                    {
                        PapierText = Renderer::DrawText(KARTEGEFUNDEN, TXTPAPIER, 1);
                        Guy.Inventar[ResourceMap] = 1;
                        Bmp[ButtonTreasureMap].Phase = 0;
                        Guy.Inventar[ResourceShovel] = 1;
                        Bmp[ButtonTreasure].Phase = 0;
                    }
                    else
                    {
                        PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
                    }
                }
                else
                {
                    PapierText = Renderer::DrawText(NICHTSGEFUNDEN2, TXTPAPIER, 1);
                }
            }
            else
            {
                PapierText = Renderer::DrawText(NICHTSGEFUNDEN, TXTPAPIER, 1);
            }

            break;
        case 12:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Eat()
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
        case 2:
        case 3:
            Guy.Aktiv = true;
            Guy.Zustand = GuyEssen;
            World::AddResource(NAHRUNG, 15);
            World::AddTime(0, 2);
            break;
        case 4:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 5:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Slingshot()
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
            Guy.Zustand = GuySchleuder;
            Guy.PosScreen.x += 5;
            World::AddTime(0, 2);
            PlaySound(Sound::Slingshot, 100);
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
            Guy.Zustand = GuySuchen;
            World::AddResource(NAHRUNG, 5);
            World::AddTime(0, 20);
            break;
        case 5:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 6:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Drink()
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
        case 2:
        case 3:
            Guy.Aktiv = true;
            Guy.Zustand = GuyTrinken;
            World::AddResource(WASSER, 30);
            World::AddTime(0, 3);
            break;
        case 4:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 5:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Log()
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
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            Guy.Aktiv = true;
            Guy.Zustand = GuyFaellen;
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 10);
            break;
        case 7:
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyWarten;
                short i = Scape[Guy.Pos.x][Guy.Pos.y].Objekt + (Baum1Down - Baum1);
                Scape[Guy.Pos.x][Guy.Pos.y].Objekt = i;
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
                Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x -= 17;
                PlaySound(Sound::Timber, 100);
                break;
            }
        case 8:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 9:
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = -1;
            Guy.Inventar[ResourceTrunk]++;
            if (Guy.Inventar[ResourceTrunk] > 10)
            {
                Guy.Inventar[ResourceTrunk] = 10;
            }
            Guy.Inventar[ResourceBranch] += 5;
            if (Guy.Inventar[ResourceBranch] > 10)
            {
                Guy.Inventar[ResourceBranch] = 10;
            }
            Guy.Inventar[ResourceLeaf] += 5;
            if (Guy.Inventar[ResourceLeaf] > 10)
            {
                Guy.Inventar[ResourceLeaf] = 10;
            }
            Guy.Inventar[ResourceVine] += 2;
            if (Guy.Inventar[ResourceVine] > 10)
            {
                Guy.Inventar[ResourceVine] = 10;
            }
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Fish()
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
            case Fluss1:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 35,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
                break;
            case Fluss2:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 19,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
                break;
            case Fluss3:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 22,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 20);
                break;
            case Fluss4:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 34,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 23);
                break;
            case Fluss6:
            case Fluss7:
            case Muendung2:
            case Quelle2:
            case Schleuse2:
            case Schleuse3:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 34,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 33);
                break;
            case Fluss5:
            case Fluss9:
            case Muendung1:
            case Quelle1:
            case Schleuse1:
            case Schleuse5:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 20,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 33);
                break;
            case Fluss8:
            case Muendung4:
            case Quelle3:
            case Schleuse4:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 22,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
                break;
            case Fluss10:
            case Muendung3:
            case Quelle4:
            case Schleuse6:
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + 32,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + 26);
                break;
            }
            break;
        case 2:
            Guy.Aktiv = true;
            PlaySound(Sound::Fish, 100);
            if (BootsFahrt)
            {
                Guy.PosScreen.y -= 2;
                Guy.Zustand = GuyBootAngeln1;
            }
            switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
            {
            case Fluss1:
            case Fluss6:
            case Fluss7:
            case Muendung2:
            case Quelle2:
            case Schleuse2:
            case Schleuse3:
                Guy.Zustand = GuyAngeln1Links;
                break;
            case Fluss2:
            case Fluss5:
            case Fluss9:
            case Muendung1:
            case Quelle1:
            case Schleuse1:
            case Schleuse5:
                Guy.Zustand = GuyAngeln1Oben;
                break;
            case Fluss3:
            case Fluss8:
            case Muendung4:
            case Quelle3:
            case Schleuse4:
                Guy.Zustand = GuyAngeln1Rechts;
                break;
            case Fluss4:
            case Fluss10:
            case Muendung3:
            case Quelle4:
            case Schleuse6:
                Guy.Zustand = GuyAngeln1Unten;
                break;
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            Guy.Aktiv = true;
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootAngeln2;
            }

            switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
            {
            case Fluss1:
            case Fluss6:
            case Fluss7:
            case Muendung2:
            case Quelle2:
            case Schleuse2:
            case Schleuse3:
                Guy.Zustand = GuyAngeln2Links;
                break;
            case Fluss2:
            case Fluss5:
            case Fluss9:
            case Muendung1:
            case Quelle1:
            case Schleuse1:
            case Schleuse5:
                Guy.Zustand = GuyAngeln2Oben;
                break;
            case Fluss3:
            case Fluss8:
            case Muendung4:
            case Quelle3:
            case Schleuse4:
                Guy.Zustand = GuyAngeln2Rechts;
                break;
            case Fluss4:
            case Fluss10:
            case Muendung3:
            case Quelle4:
            case Schleuse6:
                Guy.Zustand = GuyAngeln2Unten;
                break;
            }
            Guy.Resource[GESUNDHEIT] += 2;
            World::AddTime(0, 20);
            break;
        case 7:
            Guy.Aktiv = true;
            if (BootsFahrt)
            {
                Guy.Zustand = GuyBootAngeln3;
            }

            switch (Scape[Guy.Pos.x][Guy.Pos.y].Objekt)
            {
            case Fluss1:
            case Fluss6:
            case Fluss7:
            case Muendung2:
            case Quelle2:
            case Schleuse2:
            case Schleuse3:
                Guy.Zustand = GuyAngeln3Links;
                break;
            case Fluss2:
            case Fluss5:
            case Fluss9:
            case Muendung1:
            case Quelle1:
            case Schleuse1:
            case Schleuse5:
                Guy.Zustand = GuyAngeln3Oben;
                break;
            case Fluss3:
            case Fluss8:
            case Muendung4:
            case Quelle3:
            case Schleuse4:
                Guy.Zustand = GuyAngeln3Rechts;
                break;
            case Fluss4:
            case Fluss10:
            case Muendung3:
            case Quelle4:
            case Schleuse6:
                Guy.Zustand = GuyAngeln3Unten;
                break;
            }
            break;
        case 8:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 9:
            Guy.Resource[NAHRUNG] += 20;
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Light()
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
            Guy.Zustand = GuyAnzuenden;
            Guy.PosScreen.x += 5;
            World::AddTime(0, 1);
            break;
        case 3:
            Guy.Aktiv = true;
            Guy.Zustand = GuyWarten;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Feuer;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Feuer].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Feuer].rcDes.top);
            Chance += 2 + 2 * Scape[Guy.Pos.x][Guy.Pos.y].Hoehe;
            World::AddTime(0, 2);
            Guy.PosScreen.x -= 5;
            break;
        case 4:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 5:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Lookout()
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
            Guy.Zustand = GuyAusschau;
            World::AddTime(0, 40);
            Chance += 1 + Scape[Guy.Pos.x][Guy.Pos.y].Hoehe;
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GuyWarten;
            World::AddTime(0, 40);
            break;
        case 3:
            Guy.Aktiv = true;
            Guy.Zustand = GuyAusschau;
            World::AddTime(0, 40);
            break;
        case 4:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 5:
            Chance -= 1 + Scape[Guy.Pos.x][Guy.Pos.y].Hoehe;
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Treasure()
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
            Guy.Zustand = GuySchaufeln;
            break;
        case 2:
            World::AddTime(0, 20);
            World::AddResource(WASSER, -10);
            World::AddResource(NAHRUNG, -10);
            Guy.PosScreen.x += 5;
            Guy.PosScreen.y -= 1;
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            if (Guy.Pos.x == SchatzPos.x && Guy.Pos.y == SchatzPos.y && !SchatzGef)
            {
                PapierText = Renderer::DrawText(SCHATZGEFUNDEN, TXTPAPIER, 1);
                Guy.Inventar[ResourceMatchstick] = 1;
                Bmp[ButtonLight].Phase = 0;
                SchatzGef = true;
            }
            else
            {
                PapierText = Renderer::DrawText(KEINSCHATZ, TXTPAPIER, 1);
            }
            break;
        case 3:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Field()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            for (short i = 0; i < ImageCount; i++)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[Feld].Rohstoff[i];
            }
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Feld;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Feld].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Feld].rcDes.top);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Feld].Anzahl;
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckResource())
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
            Guy.Aktiv = true;
            Guy.Zustand = GuyFeld;
            break;
        case 19:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Feld;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Feld].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Feld].rcDes.top);
            break;
        case 20:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[ButtonStop].Phase = -1;
            if (Bmp[Feld].First)
            {
                PapierText = Renderer::DrawText(FELDHILFE, TXTPAPIER, 1);
                Bmp[Feld].First = false;
            }
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void DayEnd()
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
            Bmp[ButtonStop].Phase = -1;
            if (Guy.Zustand == GuySchlafZelt || Guy.Zustand == GuySchlafen ||
                Guy.Zustand == GuySchlafHaus || BootsFahrt)
            {
                break;
            }
            Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.x = Guy.PosScreen.x;
            Scape[Guy.Pos.x][Guy.Pos.y].GPosAlt.y = Guy.PosScreen.y;
            Erg = Renderer::GetTile(Guy.PosAlt.x, Guy.PosAlt.y);
            if (Erg.x == Guy.Pos.x && Erg.y == Guy.Pos.y)
            {
                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            }
            else if (RoutePunkt % 2 == 0)
            {
                Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); // Nur bis zur Mitte der aktuellen Kacheln laufen
            }
            else
            {
                Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
            }
            break;
        case 2:
            Renderer::Fade(95, 80, 80);
            Stunden = 12;
            Minuten = 0;
            if (Guy.Zustand == GuySchlafZelt || Guy.Zustand == GuySchlafen ||
                Guy.Zustand == GuySchlafHaus || BootsFahrt)
            {
                break;
            }
            // Wohnbare Objekte in der Umgebung suchen 
            Erg.x = -1;
            Erg.y = -1;
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt || Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3)
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Objekt == Haus3)
            {
                Erg.x = Guy.Pos.x - 1;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Objekt == Haus3)
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y - 1;
            }
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Objekt == Haus3)
            {
                Erg.x = Guy.Pos.x + 1;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Objekt == Haus3)
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y + 1;
            }
            else if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Objekt == Zelt)
            {
                Erg.x = Guy.Pos.x - 1;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Objekt == Zelt)
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y - 1;
            }
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Objekt == Zelt)
            {
                Erg.x = Guy.Pos.x + 1;
                Erg.y = Guy.Pos.y;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Objekt == Zelt)
            {
                Erg.x = Guy.Pos.x;
                Erg.y = Guy.Pos.y + 1;
            }
            if (Erg.x != -1 && Erg.y != -1)
            {
                Guy.Pos.x = Erg.x;
                Guy.Pos.y = Erg.y;
                if (Scape[Erg.x][Erg.y].Objekt == Zelt &&
                    Scape[Erg.x][Erg.y].Phase < Bmp[Scape[Erg.x][Erg.y].Objekt].Anzahl)
                {
                    Routing::ShortRoute(Scape[Erg.x][Erg.y].xScreen + Scape[Erg.x][Erg.y].ObPos.x + 3,
                                        Scape[Erg.x][Erg.y].yScreen + Scape[Erg.x][Erg.y].ObPos.y + 20);
                }
                else if (Scape[Erg.x][Erg.y].Objekt == Haus3 &&
                         Scape[Erg.x][Erg.y].Phase < Bmp[Scape[Erg.x][Erg.y].Objekt].Anzahl)
                {
                    Routing::ShortRoute(Scape[Erg.x][Erg.y].xScreen + Scape[Erg.x][Erg.y].ObPos.x +
                                        Bmp[BaumGross].Breite / 2,
                                        Scape[Erg.x][Erg.y].yScreen + Scape[Erg.x][Erg.y].ObPos.y +
                                        Bmp[BaumGross].Hoehe + 1);
                }
            }
            break;
        case 3:
            Renderer::Fade(90, 70, 70);
            Stunden = 12;
            Minuten = 0;
            if (Guy.Zustand == GuySchlafZelt || Guy.Zustand == GuySchlafen ||
                Guy.Zustand == GuySchlafHaus || BootsFahrt)
            {
                break;
            }
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyKlettern1;
            }
            break;
        case 4:
            Renderer::Fade(70, 60, 60);
            Stunden = 12;
            Minuten = 0;
            if (Guy.Zustand == GuySchlafZelt || Guy.Zustand == GuySchlafen ||
                Guy.Zustand == GuySchlafHaus || BootsFahrt)
                break;
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyGehInZelt;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                     Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyGehInHaus;
            }
            else
            {
                Guy.PosScreen.x += 3;
                Guy.Aktiv = true;
                Guy.Zustand = GuyHinlegen;
            }
            break;
        case 5:
            Renderer::Fade(55, 50, 55);
            Stunden = 12;
            Minuten = 0;
            if (BootsFahrt)
            {
                break;
            }
            Guy.Aktiv = true;
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                if (Guy.Zustand != GuySchlafZelt)
                {
                    Guy.PosScreen.x += 4;
                }
                Guy.Zustand = GuySchlafZelt;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                     Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                if (Guy.Zustand != GuySchlafHaus)
                {
                    Guy.PosScreen.x += 14;
                }
                Guy.Zustand = GuySchlafHaus;
            }
            else
            {
                Guy.Zustand = GuySchlafen;
            }
            break;
        case 6:
            Renderer::Fade(25, 25, 35);
            Stunden = 12;
            Minuten = 0;
            if (BootsFahrt)
            {
                break;
            }
            Guy.Aktiv = true;
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Zustand = GuySchlafZelt;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                     Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Zustand = GuySchlafHaus;
            }
            else
            {
                Guy.Zustand = GuySchlafen;
            }
            break;
        case 7:
            Renderer::Fade(0, 0, 0); // Nicht verwirren lassen, da das Bild in Zeige() schwarz übermalt wird
            Nacht = true;
            Stunden = 12;
            Minuten = 0;
            PlaySound(Sound::Wolf, 100);
            // Falsche Objekte Löschen
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Baum1Down &&
                Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Baum4Down)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Objekt = -1;
                Guy.Inventar[ResourceTrunk]++;
                if (Guy.Inventar[ResourceTrunk] > 10)
                {
                    Guy.Inventar[ResourceTrunk] = 10;
                }
            }

            // Je nach Schlafort Zustand verändern
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                World::AddResource(GESUNDHEIT, -5);
                if (Guy.Resource[GESUNDHEIT] <= 0)
                {
                    Guy.Aktiv = true;
                    PapierText = Renderer::DrawText(TAGENDE5, TXTPAPIER, 1);
                    Guy.AkNummer = 2;
                    Guy.Aktion = static_cast<short>(Actions::Death);
                    Stunden = 0;
                    Minuten = 0;
                }
                else
                {
                    Guy.Aktiv = true;
                    PapierText = Renderer::DrawText(TAGENDE2, TXTPAPIER, 1);
                }
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                     Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                World::AddResource(GESUNDHEIT, +20);
                Guy.Aktiv = true;
                PapierText = Renderer::DrawText(TAGENDE4, TXTPAPIER, 1);
            }
            else if (BootsFahrt)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyBootWarten;
                PapierText = Renderer::DrawText(TAGENDE3, TXTPAPIER, 1);
                Guy.AkNummer = 2;
                Guy.Aktion = static_cast<short>(Actions::Death);
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
                    Guy.Aktion = static_cast<short>(Actions::Death);
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
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Zustand = GuySchlafZelt;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                     Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Zustand = GuySchlafHaus;
            }
            else
            {
                Guy.Zustand = GuySchlafen;
            }
            break;
        case 9:
            Renderer::Fade(40, 40, 40);
            Stunden = 0;
            Minuten = 0;

            Stunden = 0;
            Minuten = 0;
            Guy.Aktiv = true;
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Zustand = GuySchlafZelt;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                     Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Zustand = GuySchlafHaus;
            }
            else
            {
                Guy.Zustand = GuySchlafen;
            }
            break;
        case 10:
            Renderer::Fade(70, 60, 60);
            Stunden = 0;
            Minuten = 0;
            StopSound(Sound::Snore);
            Guy.Aktiv = true;
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.PosScreen.x -= 14;
                Guy.Zustand = GuyGehausHaus;
            }
            else
            {
                Guy.Zustand = GuyAufstehen;
            }
            break;
        case 11:
            Renderer::Fade(90, 80, 80);
            Stunden = 0;
            Minuten = 0;
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyKlettern2;
            }
            break;
        case 12:
            Renderer::Fade(100, 100, 100);
            Stunden = 0;
            Minuten = 0;
            Guy.Zustand = GuyLinks;
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            if (Guy.Resource[GESUNDHEIT] > 10)
            {
                Game::SaveGame();
            }
            break;
        }
    }

    void Rescued()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            ZWEID Erg = Renderer::GetTile(Guy.PosAlt.x, Guy.PosAlt.y);
            if (Erg.x == Guy.Pos.x && Erg.y == Guy.Pos.y)
            {
                Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            }
            else if (RoutePunkt % 2 == 0)
            {
                Routing::ShortRoute(RouteKoor[RoutePunkt].x, RouteKoor[RoutePunkt].y); //Nur bis zur Mitte der aktuellen Kacheln laufen
            }
            else
            {
                Routing::ShortRoute(RouteKoor[RoutePunkt + 1].x, RouteKoor[RoutePunkt + 1].y);
            }
            TwoClicks = -1; // Keine Ahnung warum ich das hier machen muß
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GuyWarten;
            PapierText = Renderer::DrawText(GERETTET, TXTPAPIER, 1);
            break;
        case 3:
            if (Frage == 2)
            {
                Guy.Aktion = static_cast<short>(Actions::Nothing);
                Frage = -1;
                break;
            }
            Spielzustand = State::Rescued;
            Frage = -1;
            break;
        case 4:
            // Route herstellen
            Guy.Aktiv = true;
            Guy.Zustand = GuyLinks;
            RoutePunkt = -1;
            Steps = 0;
            Step = 0;
            RouteStart.x = Guy.Pos.x;
            RouteStart.y = Guy.Pos.y;
            RouteZiel.y = Guy.Pos.y;
            for (short x = MAXXKACH - 1; x > 1; x--) // Position des Rettungsschiffs festlegen
            {
                if (Scape[x][Guy.Pos.y].Art != 1)
                {
                    break;
                }
                RouteZiel.x = x + 1;
            }
            // Schiff hinbauen
            Scape[RouteZiel.x][RouteZiel.y].Phase = 0;
            Scape[RouteZiel.x][RouteZiel.y].Objekt = GuySchiff;
            Scape[RouteZiel.x][RouteZiel.y].ObPos.x = 10;
            Scape[RouteZiel.x][RouteZiel.y].ObPos.y = 10;
            RouteZiel.x -= 2;
            Routing::FindTheWay();
            Guy.Zustand = GuyLinks;
            break;
        case 5:
            Guy.Zustand = GuyLinks;
            Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x +
                                 Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].x) / 2,
                                (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].y +
                                 Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2);
            break;
        case 6:
            Guy.Pos.x += 2;
            Guy.Zustand = GuySchwimmen;
            Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
                                 Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x) / 2,
                                (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y +
                                 Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2);
            break;
        case 7:
            Guy.PosScreen.y -= 10;
            if (!BootsFahrt)
            {
                World::ChangeBoatRide();
            }
            Guy.Aktiv = true;
            Guy.Zustand = GuySchiff;
            RoutePunkt = -1;
            Steps = 0;
            Step = 0;
            RouteStart.x = Guy.Pos.x;
            RouteStart.y = Guy.Pos.y;
            RouteZiel.y = Guy.Pos.y;
            RouteZiel.x = MAXXKACH - 2;
            Routing::FindTheWay();
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Meerwellen].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Meerwellen].rcDes.top);
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Meerwellen;
            break;
        case 8:
            Guy.Aktiv = true;
            Guy.Zustand = GuySchiff;
            break;
        case 9:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            Guy.Zustand = GuyLinks;
            Spielzustand = State::Outro;
            break;
        }
    }

    void Tent()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Zelt;
            for (short i = 0; i < ImageCount; i++)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[Zelt].Rohstoff[i];
            }
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Zelt].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Zelt].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckResource())
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
        case 2:
        case 3:
        case 12:
        case 13:
            Guy.Aktiv = true;
            Guy.Zustand = GuyBindenUnten;
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
        case 7:
        case 8:
            Guy.Aktiv = true;
            Guy.Zustand = GuyBindenOben;
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
            Bmp[ButtonStop].Phase = -1;
            if (Bmp[Zelt].First)
            {
                PapierText = Renderer::DrawText(ZELTHILFE, TXTPAPIER, 1);
                Bmp[Zelt].First = false;
            }
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Boat()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Boot;
            for (short i = 0; i < ImageCount; i++)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[Boot].Rohstoff[i];
            }
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Boot].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Boot].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Boot].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckResource())
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
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Boot].Anzahl + 1);
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
            Guy.Aktiv = true;
            Guy.Zustand = GuySchlagen;
            World::AddResource(WASSER, -2);
            World::AddResource(NAHRUNG, -2);
            World::AddTime(0, 15);
            break;
        case 7:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 22,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 16);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Boot].Anzahl + 2);
            break;
        case 11:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 14,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 11);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Boot].Anzahl + 3);
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
            Bmp[ButtonStop].Phase = -1;
            if (Bmp[Boot].First)
            {
                PapierText = Renderer::DrawText(BOOTHILFE, TXTPAPIER, 1);
                Bmp[Boot].First = false;
            }
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Pipe()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Rohr;
            for (short i = 0; i < ImageCount; i++)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[Rohr].Rohstoff[i];
            }
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Rohr].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Rohr].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Rohr].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckResource())
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
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Rohr].Anzahl + 1);
            break;
        case 4:
        case 5:
        case 6:
        case 11:
        case 12:
        case 13:
            Guy.Aktiv = true;
            Guy.Zustand = GuySchlagen;
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
            Guy.Aktiv = true;
            Guy.Zustand = GuyFaellen;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 5);
            break;
        case 10:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 17,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 13);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Rohr].Anzahl + 2);
            break;
        case 17:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 18:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            World::FillPipe();
            Bmp[ButtonStop].Phase = -1;
            if (Bmp[Rohr].First)
            {
                PapierText = Renderer::DrawText(ROHRHILFE, TXTPAPIER, 1);
                Bmp[Rohr].First = false;
            }
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void SosSign()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Sos;
            for (short i = 0; i < ImageCount; i++)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[Sos].Rohstoff[i];
            }
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Sos].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Sos].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Sos].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckResource())
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
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Sos].Anzahl + 1);
            break;
        case 7:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 12,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 9);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Sos].Anzahl + 2);
            break;
        case 10:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 19,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 12);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Sos].Anzahl + 3);
            break;
        case 13:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 21,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 5);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Sos].Anzahl + 4);
            break;
        case 16:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 28,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 8);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Sos].Anzahl + 5);
            break;
        case 2:
        case 5:
        case 8:
        case 11:
        case 14:
        case 17:
            Guy.Aktiv = true;
            Guy.PosScreen.x += 4;
            Guy.Zustand = GuyHinlegen;
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
            Guy.Aktiv = true;
            Guy.PosScreen.x -= 4;
            Guy.Zustand = GuyAufstehen;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 1);
            break;
        case 19:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 20:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            if (Scape[Guy.Pos.x][Guy.Pos.y].Art == 0 || Scape[Guy.Pos.x][Guy.Pos.y].Art == 4)
                Chance += 1;
            else
                Chance += 2; // Dürfte nur noch der Strand übrig sein
            Bmp[ButtonStop].Phase = -1;
            if (Bmp[Sos].First)
            {
                PapierText = Renderer::DrawText(SOSHILFE, TXTPAPIER, 1);
                Bmp[Sos].First = false;
            }
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Fireplace()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Feuerstelle;
            for (short i = 0; i < ImageCount; i++)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[Feuerstelle].Rohstoff[i];
            }
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Feuerstelle].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = static_cast<short>(Bmp[Feuerstelle].rcDes.left);
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = static_cast<short>(Bmp[Feuerstelle].rcDes.top);
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckResource())
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
            Guy.Zustand = GuyHinlegen;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 1);
            break;
        case 3:
            Guy.Aktiv = true;
            Guy.PosScreen.x -= 4;
            Guy.Zustand = GuyAufstehen;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 1);
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Feuerstelle].Anzahl + 1);
            break;
        case 4:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 15);
            break;
        case 5:
        case 6:
        case 7:
            Guy.Aktiv = true;
            Guy.Zustand = GuyBindenOben;
            World::AddResource(WASSER, -1);
            World::AddResource(NAHRUNG, -1);
            World::AddTime(0, 1);
            if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer != 5)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Feuerstelle].Anzahl + Scape[Guy.Pos.x][Guy.Pos.y].AkNummer - 4);
            }
            break;
        case 8:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 9:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[ButtonStop].Phase = -1;
            if (Bmp[Feuerstelle].First)
            {
                PapierText = Renderer::DrawText(FEUERSTELLEHILFE, TXTPAPIER, 1);
                Bmp[Feuerstelle].First = false;
            }
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void House1()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            for (short i = 0; i < ImageCount; i++)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[Haus1].Rohstoff[i];
            }
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Haus1].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Haus1;
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckResource())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                                Bmp[BaumGross].Breite / 2 - 3,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                                Bmp[BaumGross].Hoehe + 1);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer;
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus1].Anzahl + 1);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus1].Anzahl + 2);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 14:
        case 15:
        case 16:
        case 17:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus1].Anzahl + 3);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 18:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 19:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[ButtonStop].Phase = -1;
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void House2()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            for (short i = 0; i < ImageCount; i++)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[Haus2].Rohstoff[i];
            }
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Haus2].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Haus2;
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckResource())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                                Bmp[BaumGross].Breite / 2,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                                Bmp[BaumGross].Hoehe + 1);
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GuyKlettern1;
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
            World::AddTime(0, 1);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer2;
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus2].Anzahl + 1);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 11:
        case 12:
        case 13:
        case 14:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus2].Anzahl + 2);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 15:
        case 16:
        case 17:
        case 18:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus2].Anzahl + 3);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 19:
            Guy.Aktiv = true;
            Guy.Zustand = GuyKlettern2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus2].Anzahl + 4);
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
            World::AddTime(0, 1);
            break;
        case 20:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 21:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[ButtonStop].Phase = -1;
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void House3()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
            for (short i = 0; i < ImageCount; i++)
            {
                Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] = Bmp[Haus3].Rohstoff[i];
            }
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = Bmp[Haus3].Anzahl;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Haus3;
        }
        Scape[Guy.Pos.x][Guy.Pos.y].AkNummer++;
        if (!World::CheckResource())
        {
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer--;
            return;
        }
        switch (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                                Bmp[BaumGross].Breite / 2,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                                Bmp[BaumGross].Hoehe + 1);
            break;
        case 2:
            Guy.Aktiv = true;
            Guy.Zustand = GuyKlettern1;
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
            World::AddTime(0, 1);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer2;
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus3].Anzahl + 1);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 11:
        case 12:
        case 13:
        case 14:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus3].Anzahl + 2);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 15:
        case 16:
        case 17:
        case 18:
            Guy.Aktiv = true;
            Guy.Zustand = GuyHammer2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus3].Anzahl + 3);
            World::AddResource(NAHRUNG, -0.5);
            World::AddResource(WASSER, -0.5);
            World::AddTime(0, 1);
            break;
        case 19:
            Guy.Aktiv = true;
            Guy.Zustand = GuyKlettern2;
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = static_cast<short>(Bmp[Haus3].Anzahl + 4);
            World::AddResource(NAHRUNG, -1);
            World::AddResource(WASSER, -1);
            World::AddTime(0, 1);
            break;
        case 20:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            break;
        case 21:
            Scape[Guy.Pos.x][Guy.Pos.y].Phase = 0;
            Bmp[ButtonStop].Phase = -1;
            if (Bmp[Haus3].First)
            {
                PapierText = Renderer::DrawText(HAUS3HILFE, TXTPAPIER, 1);
                Bmp[Haus3].First = false;
            }
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Sleep()
    {
        if (Guy.AkNummer == 0)
        {
            Guy.PosAlt = Guy.PosScreen; // Die Originalposition merken
        }
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 3,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 20);
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                     Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                                    Bmp[BaumGross].Breite / 2 + 1,
                                    Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                                    Bmp[BaumGross].Hoehe + 1);
            }
            break;
        case 2:
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyKlettern1;
                World::AddResource(NAHRUNG, -1);
                World::AddResource(WASSER, -1);
            }
            break;
        case 3:
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyGehInZelt;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                     Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyGehInHaus;
            }
            else
            {
                Guy.PosScreen.x += 3;
                Guy.Aktiv = true;
                Guy.Zustand = GuyHinlegen;
            }
            break;
        case 4:
        case 5:
            Guy.Aktiv = true;
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Zelt &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                if (Guy.AkNummer == 4)
                {
                    Guy.PosScreen.x += 4;
                }
                Guy.Zustand = GuySchlafZelt;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                     Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                if (Guy.AkNummer == 4)
                {
                    Guy.PosScreen.x += 14;
                }
                Guy.Zustand = GuySchlafHaus;
            }
            else
            {
                Guy.Zustand = GuySchlafen;
            }
            World::AddResource(GESUNDHEIT, 5);
            World::AddTime(0, 30);
            break;
        case 6:
            Guy.Aktiv = true;
            StopSound(Sound::Snore);
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.PosScreen.x -= 14;
                Guy.Zustand = GuyGehausHaus;
            }
            else
            {
                Guy.Zustand = GuyAufstehen;
            }
            break;
        case 7:
            if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Haus3 &&
                Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl)
            {
                Guy.Aktiv = true;
                Guy.Zustand = GuyKlettern2;
                World::AddResource(NAHRUNG, -1);
                World::AddResource(WASSER, -1);
            }
            break;
        case 8:
            Routing::ShortRoute(Guy.PosAlt.x, Guy.PosAlt.y);
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            break;
        }
    }

    void Undock()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            Routing::ShortRoute(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x + 14,
                                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y + 11);
            break;
        case 2:
            World::ChangeBoatRide();
            Guy.PosScreen.x = Scape[Guy.Pos.x][Guy.Pos.y].xScreen +
                              Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x +
                              Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite / 2;
            Guy.PosScreen.y = Scape[Guy.Pos.x][Guy.Pos.y].yScreen +
                              Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y +
                              Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe / 2;
            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = -1;
            if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Art == 1)
            {
                Guy.Pos.x--;
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Art == 1)
            {
                Guy.Pos.y--;
            }
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Art == 1)
            {
                Guy.Pos.x++;
            }
            else
            {
                if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Art == 1)
                {
                    Guy.Pos.y++;
                }
            }

            Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
                                 Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x) / 2,
                                (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y +
                                 Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2);

            break;
        case 3:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            Guy.PosAlt.x = Guy.PosScreen.x;
            Guy.PosAlt.y = Guy.PosScreen.y;
            break;
        }
    }

    void Dock()
    {
        Guy.AkNummer++;
        switch (Guy.AkNummer)
        {
        case 1:
            if (Scape[Guy.Pos.x - 1][Guy.Pos.y].Art != 1)
            {
                Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
                                     Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].x) / 2,
                                    (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].y +
                                     Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y) / 2);
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y - 1].Art != 1)
            {
                Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].x +
                                     Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x) / 2,
                                    (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y +
                                     Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].y) / 2);
            }
            else if (Scape[Guy.Pos.x + 1][Guy.Pos.y].Art != 1)
            {
                Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x +
                                     Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].x) / 2,
                                    (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].y +
                                     Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2);
            }
            else if (Scape[Guy.Pos.x][Guy.Pos.y + 1].Art != 1)
            {
                Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].x +
                                     Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x) / 2,
                                    (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y +
                                     Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].y) / 2);
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

            Scape[Guy.Pos.x][Guy.Pos.y].Objekt = Boot;
            Scape[Guy.Pos.x][Guy.Pos.y].AkNummer = Bmp[Boot].AkAnzahl;

            World::ChangeBoatRide();
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.x = Guy.PosScreen.x -
                                                  Scape[Guy.Pos.x][Guy.Pos.y].xScreen -
                                                  Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Breite / 2;
            Scape[Guy.Pos.x][Guy.Pos.y].ObPos.y = Guy.PosScreen.y -
                                                  Scape[Guy.Pos.x][Guy.Pos.y].yScreen -
                                                  Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Hoehe / 2;

            Routing::ShortRoute((Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
                                 Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x) / 2,
                                (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y +
                                 Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2);

            break;
        case 3:
            Guy.Aktion = static_cast<short>(Actions::Nothing);
            Guy.PosAlt.x = Guy.PosScreen.x;
            Guy.PosAlt.y = Guy.PosScreen.y;
            break;
        }
    }
} // namesapce Action