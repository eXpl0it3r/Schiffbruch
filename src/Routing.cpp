#include "Routing.hpp"

#include "Math.hpp"

namespace Routing {
int LenMap[MAX_TILES_X][MAX_TILESY];
Coordinate SaveRoute[MAX_TILES_X * MAX_TILESY]; // Zum zwischenspeichern der Route
Coordinate NewPos; // Nur innerhalb des Pathfindings benutzt

void MarkRoute(bool Mark)
{
    for (short i = 0; i < RouteLaenge; i++) {
        Landscape[Route[i].x][Route[i].y].Marked = Mark;
    }
}

short RotateRight(short Dir) // Richtungskoordinate rechtsrum umrechnen
{
    switch (Dir) {
    case 2: {
        NewPos.x++;
        NewPos.y++;
        Dir = 4;
        break;
    }

    case 4: {
        NewPos.x--;
        NewPos.y++;
        Dir = 8;
        break;
    }

    case 8: {
        NewPos.x--;
        NewPos.y--;
        Dir = 1;
        break;
    }

    case 1: {
        NewPos.x++;
        NewPos.y--;
        Dir = 2;
        break;
    }
    }

    return Dir;
}

bool FindTheWay()
{
    short Dir;

    Coordinate Plist[MAX_TILES_X * MAX_TILESY]; // Besuchte Punkte merken
    short Llist[MAX_TILES_X * MAX_TILESY]; // Länge vom Punkt zum Ziel

    Coordinate ShPos{0, 0};
    Coordinate BestLine{0, 0};
    Coordinate ShortKoor{0, 0};

    for (short AI = 0; AI < MAX_TILESY; AI++)
        for (short BI = 0; BI < MAX_TILES_X; BI++) {
            LenMap[AI][BI] = 65535;
            Llist[AI * BI] = 0;
            Plist[AI * BI].x = 0;
            Plist[AI * BI].y = 0;
        }

    short ShortEntf = -1;
    RouteLaenge = 0;

    short PCnt = 1;
    Plist[0] = RouteStart;
    short DiffX = (RouteStart.x - RouteDestination.x);
    short DiffY = (RouteStart.y - RouteDestination.y);
    Llist[0] = (DiffX * DiffX) + (DiffY * DiffY);

    LenMap[RouteStart.x][RouteStart.y] = 0;
    Coordinate Pos = RouteStart;
    NewPos = Pos;
    bool GoalReached = false;

    while ((!GoalReached) && (PCnt > 0)) {
        // den mit der kürzesten Entfernung zum Ziel finden (der in der Liste ist)
        short Shortest = 0;

        for (short CI = 0; CI <= PCnt - 1; CI++) {
            if (Llist[CI] < Llist[Shortest]) {
                Shortest = CI;
            }
        }

        // Mit dem Nächsten weitermachen
        Pos = Plist[Shortest];

        // Den kürzesten merken
        if ((ShortEntf > Llist[Shortest]) || (ShortEntf == -1)) {
            ShortEntf = Llist[Shortest];
            ShortKoor = Plist[Shortest];
        }

        // Den Nächsten aus der Liste löschen
        Plist[Shortest] = Plist[PCnt - 1];
        Llist[Shortest] = Llist[PCnt - 1];
        PCnt--;
        NewPos = Pos;
        Dir = 2;
        NewPos.y--; // Oben nachschauen anfangen

        for (short BI = 0; BI <= 3; BI++) { // In jede Richtung schauen
            // ist das Feld noch nicht besucht und frei?
            if ((LenMap[NewPos.x][NewPos.y] == 65535) &&
                    (Landscape[NewPos.x][NewPos.y].Walkable)) {
                // Wieviele Schritte braucht man um zu diesem Feld zu kommen
                short StepCnt = LenMap[Pos.x][Pos.y] + 1;
                LenMap[NewPos.x][NewPos.y] = StepCnt;
                Plist[PCnt] = NewPos;
                // Die Entfernung in die Liste aufnehmen
                DiffX = (NewPos.x - RouteDestination.x);
                DiffY = (NewPos.y - RouteDestination.y);
                Llist[PCnt] = (DiffX * DiffX) + (DiffY * DiffY);
                PCnt++;
            }

            // Ziel erreicht?
            if ((NewPos.x == RouteDestination.x) && (NewPos.y == RouteDestination.y)) {
                GoalReached = true;
                BI = 3;
            }

            Dir = RotateRight(Dir);
        }
    }

    if ((PCnt == 0) || (!Landscape[RouteDestination.x][RouteDestination.y].Walkable)) {
        RouteDestination.x = ShortKoor.x;
        RouteDestination.y = ShortKoor.y;

        if (FindTheWay()) {
            return true;
        }

        return false;
    }

    if (GoalReached) { // Punkt rückwärts durchgehen und Abkürzungen finden
        Pos = RouteDestination;
        Coordinate LineStartPos = Pos;

        while ((Pos.x != RouteStart.x) || (Pos.y != RouteStart.y)) {
            NewPos = Pos;
            int ShStep = 65535;
            Dir = 2;
            NewPos.y--; // Zuerst nach oben probieren

            for (short AI = 0; AI <= 3; AI++) {
                if (LenMap[NewPos.x][NewPos.y] < ShStep) {
                    ShStep = LenMap[NewPos.x][NewPos.y];
                    ShPos = NewPos;
                }

                Dir = RotateRight(Dir);
            }

            Pos = ShPos;

            // Linie beste Linie ohne Unterbrechung finden
            if (!Math::LineIntersect(LineStartPos, Pos, false)) {
                BestLine = Pos;
            }

            if ((Pos.x == RouteStart.x) && (Pos.y == RouteStart.y)) {
                Pos = BestLine;
                Math::LineIntersect(LineStartPos, Pos, true);
                LineStartPos = Pos;
            }
        }

        Route[RouteLaenge].x = RouteStart.x;
        Route[RouteLaenge].y = RouteStart.y;
        RouteLaenge++;

        SortRoute(); // Sortieren
    }

    return true;
}

bool CheckRoute(short x, short y, bool save, short Laenge) // Nachprüfen ob auf aktuellem Teil in der Route ist
{
    short i;

    if (!save) {
        for (i = 0; i < RouteLaenge; i++) {
            if ((x == Route[i].x) && (y == Route[i].y)) {
                return true;
            }
        }
    } else {
        for (i = 0; i <= Laenge; i++) {
            if ((x == SaveRoute[i].x) && (y == SaveRoute[i].y)) {
                return true;
            }
        }
    }

    return false;
}

void SortRoute()
{
    Coordinate Pos;

    Pos.x = RouteStart.x;
    Pos.y = RouteStart.y;

    for (short i = 0; i < RouteLaenge; i++) { // Alle Teile vom Start durchgehen
        SaveRoute[i].x = Pos.x;
        SaveRoute[i].y = Pos.y;

        RouteKoor[2 * i].x =
            (Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][0].x +
             Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][2].x) / 2;
        RouteKoor[2 * i].y =
            (Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][1].y +
             Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][3].y) / 2;

        NewPos.x = Pos.x;
        NewPos.y = Pos.y - 1; // oben mit nachschauen anfangen
        short Dir = 2;

        for (short j = 0; j <= 3; j++) {
            if ((CheckRoute(NewPos.x, NewPos.y, false, RouteLaenge)) &&
                    (!CheckRoute(NewPos.x, NewPos.y, true, i))) {
                switch (j) {
                case 0:
                    RouteKoor[2 * i + 1].x =
                        (Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][1].x +
                         Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][2].x) / 2;
                    RouteKoor[2 * i + 1].y =
                        (Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][1].y +
                         Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][2].y) / 2;
                    break;

                case 1:
                    RouteKoor[2 * i + 1].x =
                        (Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][2].x +
                         Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][3].x) / 2;
                    RouteKoor[2 * i + 1].y =
                        (Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][2].y +
                         Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][3].y) / 2;
                    break;

                case 2:
                    RouteKoor[2 * i + 1].x =
                        (Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][3].x +
                         Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][0].x) / 2;
                    RouteKoor[2 * i + 1].y =
                        (Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][3].y +
                         Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][0].y) / 2;
                    break;

                case 3:
                    RouteKoor[2 * i + 1].x =
                        (Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][0].x +
                         Landscape[Pos.x][Pos.y].xScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][1].x) / 2;
                    RouteKoor[2 * i + 1].y =
                        (Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][0].y +
                         Landscape[Pos.x][Pos.y].yScreen + CornerCoord[Landscape[Pos.x][Pos.y].Type][1].y) / 2;
                    break;
                }

                break;
            }

            Dir = RotateRight(Dir);
        }

        Pos.x = NewPos.x;
        Pos.y = NewPos.y;
    }

    for (short i = 0; i <= RouteLaenge; i++) { // Wieder in die Originalroute speichern
        Route[i].x = SaveRoute[i].x;
        Route[i].y = SaveRoute[i].y;
    }
}

void ShortRoute(short Zielx, short Ziely)
{
    RouteLaenge = 1;
    Route[0].x = Guy.Pos.x;
    Route[0].y = Guy.Pos.y;
    RouteKoor[0].x = Guy.ScreenPosition.x;
    RouteKoor[0].y = Guy.ScreenPosition.y;
    Route[1].x = Guy.Pos.x;
    Route[1].y = Guy.Pos.y;
    RouteKoor[1].x = Zielx;
    RouteKoor[1].y = Ziely;

    // Die Animation gleich anschließend starten
    Guy.IsActive = true;

    if ((IsInBoat) && (Guy.AnimationState != Tiles::GUY_SWIM)) {
        Guy.AnimationState = Tiles::GUY_BOAT_LEFT;
    } else if (Guy.AnimationState != Tiles::GUY_SWIM) {
        Guy.AnimationState = Tiles::GUY_LEFT;
    }

    RoutePunkt = -1;
    Steps = 0;
    Step = 0;
}
} // namespace Route
