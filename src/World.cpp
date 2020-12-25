#include "World.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Renderer.hpp"
#include "Sound.hpp"
#include "State.hpp"

#include <cstdio>
#include <cstring>

namespace World {
RiverRun Flusslauf[NUMBER_OF_RIVERS][MAX_RIVER_LENGTH];

void MakeRohString(short x, short y, short Objekt)
{
    char TmpString[1024];

    RohString[0] = char(0);
    bool keinRohstoff = true;

    if (Objekt == -1) {
        for (short i = 0; i < SPRITE_COUNT; i++) {
            if (Scape[x][y].RequiredRawMaterials[i] != 0) {
                keinRohstoff = false;
            }
        }
    } else {
        for (short i = 0; i < SPRITE_COUNT; i++) {
            if (Bmp[Objekt].RequiredRawMaterials[i] != 0) {
                keinRohstoff = false;
            }
        }
    }

    if (keinRohstoff) {
        return;
    }

    strcat(RohString, " ->");

    for (short i = 0; i < SPRITE_COUNT; i++) {
        if (Objekt == -1) {
            if (Scape[x][y].RequiredRawMaterials[i] == 0) {
                continue;
            }
        } else {
            if (Bmp[Objekt].RequiredRawMaterials[i] == 0) {
                continue;
            }
        }

        strcat(RohString, " ");

        switch (i) {
        case RAW_TREE_BRANCH:
            LoadString(g_hInst, AST, TmpString, 1024);
            break;

        case RAW_STONE:
            LoadString(g_hInst, STEIN, TmpString, 1024);
            break;

        case RAW_LEAF:
            LoadString(g_hInst, BLATT, TmpString, 1024);
            break;

        case RAW_LIANA:
            LoadString(g_hInst, LIANE, TmpString, 1024);
            break;

        case RAW_TREE_TRUNK:
            LoadString(g_hInst, STAMM, TmpString, 1024);
            break;
        }

        strcat(RohString, TmpString);
        strcat(RohString, "=");

        if (Objekt == -1) {
            std::sprintf(TmpString, "%d", Scape[x][y].RequiredRawMaterials[i]);
        } else {
            std::sprintf(TmpString, "%d", Bmp[Objekt].RequiredRawMaterials[i]);
        }

        strcat(RohString, TmpString);
    }
}

void AddTime(short h, short m)
{
    Stunden += h;
    Minuten += m;

    if (Minuten >= 60) {
        Minuten -= 60;
        Stunden++;
    }

    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            // Feuer nach einer bestimmten Zeit ausgehen lassen
            if (Scape[x][y].Object == FIRE) {
                Scape[x][y].FireTimer += float((60 * h + m) * 0.0005);

                if (Scape[x][y].FireTimer >= 1) {
                    Scape[x][y].Object = -1;
                    Scape[x][y].FireTimer = 0;
                    Scape[x][y].ObjectPosOffset.x = 0;
                    Scape[x][y].ObjectPosOffset.y = 0;
                    Scape[x][y].AnimationPhase = -1;
                    Chance -= 2 + 2 * Scape[x][y].Height;
                }
            }

            if ((Scape[x][y].AnimationPhase == -1) ||
                    ((Scape[x][y].Object != FIELD) &&
                     (Scape[x][y].Object != BUSH))) {
                continue;    // Wenn kein Fruchtobjekt weiter
            }

            if (Scape[x][y].AnimationPhase >= Bmp[Scape[x][y].Object].AnimationPhaseCount) {
                continue;
            }

            if (Scape[x][y].Object == FIELD) {
                Scape[x][y].AnimationPhase += float((60 * h + m) * 0.005);
            } else if (Scape[x][y].Object == BUSH) {
                Scape[x][y].AnimationPhase += float((60 * h + m) * 0.0005);    // pro Minute Reifungsprozess fortführen
            }

            if (Scape[x][y].AnimationPhase > Bmp[Scape[x][y].Object].AnimationPhaseCount - 1) {
                Scape[x][y].AnimationPhase = static_cast<float>(Bmp[Scape[x][y].Object].AnimationPhaseCount) - 1;
            }
        }

    AddResource(GESUNDHEIT, (60 * h + m) * (Guy.ResourceAmount[WASSER] - 50 + Guy.ResourceAmount[NAHRUNG] - 50) / 1000);

    if ((Spielzustand == State::GAME) && (!IsInBoat)) {
        for (short i = 0; i <= (60 * h + m); i++) {
            if (Chance == 0) {
                break;
            }

            if (rand() % static_cast<int>(1 / (Chance / 72000)) == 1) {
                Guy.IsActive = false;
                Guy.ActionNumber = 0;
                Guy.CurrentAction = Action::RESCUED;
                break;
            }
        }
    }
}

void AddResource(short Art, float Anzahl) // Fügt wassser usw hinzu
{
    Guy.ResourceAmount[Art] += Anzahl;

    if (Guy.ResourceAmount[Art] > 100) {
        Guy.ResourceAmount[Art] = 100;
    }

    if (Guy.ResourceAmount[Art] < 0) {
        Guy.ResourceAmount[Art] = 0;
    }

    // Wann tod
    if ((Guy.ResourceAmount[GESUNDHEIT] <= 0) && (Guy.CurrentAction != Action::DEATH) &&
            (Guy.CurrentAction != Action::DAY_END) && (Spielzustand == State::GAME)) {
        Guy.IsActive = false;
        Guy.ActionNumber = 0;
        Guy.CurrentAction = Action::DEATH;
    }
}

void Generate()
{
    // Die Kartehintergrundfarbe
    rcRectdes.left = 0;
    rcRectdes.top = 0;
    rcRectdes.right = 2 * MAX_TILES_X;
    rcRectdes.bottom = 2 * MAX_TILESY;
    lpDDSKarte->create(MAX_TILES_X, MAX_TILESY, sf::Color(247, 222, 191));
//    ddbltfx.dwFillColor = Renderer::RGB2DWORD(247, 222, 191);
//        lpDDSKarte->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

    // Die Landschaftshintergrundfarbe
    rcRectdes.left = 0;
    rcRectdes.top = 0;
    rcRectdes.right = MAX_SURFACE_X;
    rcRectdes.bottom = MAX_SURFACE_Y;
    lpDDSScape->create(MAX_SURFACE_X, MAX_SURFACE_Y, sf::Color::Black);
//    ddbltfx.dwFillColor = Renderer::RGB2DWORD(0, 0, 0);
//        lpDDSScape->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

    for (short y = 0; y < MAX_TILESY; y++) {
        for (short x = 0; x < MAX_TILES_X; x++) {
            if (!Scape[x][y].Discovered) {
                continue;    // Nicht entdeckte Felder nicht malen
            }

            rcRectdes.left = Scape[x][y].xScreen;
            rcRectdes.top = Scape[x][y].yScreen;
            rcRectdes.right = rcRectdes.left + TILE_SIZE_X;
            rcRectdes.bottom = rcRectdes.top + TILE_SIZE_Y;

            if (Scape[x][y].Terrain == 4) {
                rcRectsrc.left = TILE_SIZE_X * Scape[x][y].Type;
                rcRectsrc.right = TILE_SIZE_X * Scape[x][y].Type + TILE_SIZE_X;
                rcRectsrc.top = 0;
                rcRectsrc.bottom = TILE_SIZE_Y;
            } else if (Scape[x][y].Terrain == 0) { // trockenes Land
                rcRectsrc.left = TILE_SIZE_X * Scape[x][y].Type;
                rcRectsrc.right = TILE_SIZE_X * Scape[x][y].Type + TILE_SIZE_X;
                rcRectsrc.top = 4 * TILE_SIZE_Y;
                rcRectsrc.bottom = 5 * TILE_SIZE_Y;
            } else {
                if ((Scape[x][y].Type == 0) && (Scape[x][y].Terrain == 1)) {
                    rcRectsrc.left = 0 * TILE_SIZE_X;
                    rcRectsrc.top = 3 * TILE_SIZE_Y;
                    rcRectsrc.right = 1 * TILE_SIZE_X;
                    rcRectsrc.bottom = 4 * TILE_SIZE_Y;
                }

                if ((Scape[x][y].Type == 0) && (Scape[x][y].Terrain == 2)) {
                    rcRectsrc.left = 1 * TILE_SIZE_X;
                    rcRectsrc.top = 3 * TILE_SIZE_Y;
                    rcRectsrc.right = 2 * TILE_SIZE_X;
                    rcRectsrc.bottom = 4 * TILE_SIZE_Y;
                }

                if ((Scape[x][y].Type == 0) && (Scape[x][y].Terrain == 3)) {
                    rcRectsrc.left = 2 * TILE_SIZE_X;
                    rcRectsrc.top = 3 * TILE_SIZE_Y;
                    rcRectsrc.right = 3 * TILE_SIZE_X;
                    rcRectsrc.bottom = 4 * TILE_SIZE_Y;
                }
            }

            // Landschaftskacheln zeichnen
            Renderer::Blitten(lpDDSMisc, lpDDSScape, true);

            // Gitter drüberlegen
            if (Gitter) {
                rcRectsrc.left = TILE_SIZE_X * Scape[x][y].Type;
                rcRectsrc.right = TILE_SIZE_X * Scape[x][y].Type + TILE_SIZE_X;
                rcRectsrc.top = 1 * TILE_SIZE_Y;
                rcRectsrc.bottom = 1 * TILE_SIZE_Y + TILE_SIZE_Y;
                Renderer::Blitten(lpDDSMisc, lpDDSScape, true);
            }

            // Landschaftsobjekte zeichnen (falls Animationen ausgeschaltet sind)
            if ((!LAnimation) && (Scape[x][y].Object != -1)) {
                if ((Scape[x][y].Object >= SEA_WAVES) && (Scape[x][y].Object <= FLOODGATE_6)) {
                    rcRectsrc.left = Bmp[Scape[x][y].Object].sourceRect.left;
                    rcRectsrc.right = Bmp[Scape[x][y].Object].sourceRect.right;

                    if (Scape[x][y].Object == SEA_WAVES) {
                        short i = rand() % 6;
                        rcRectsrc.top = Bmp[Scape[x][y].Object].sourceRect.top + i * Bmp[Scape[x][y].Object].Height;
                        rcRectsrc.bottom = Bmp[Scape[x][y].Object].sourceRect.bottom + i * Bmp[Scape[x][y].Object].Height;
                    } else {
                        rcRectsrc.top = Bmp[Scape[x][y].Object].sourceRect.top;
                        rcRectsrc.bottom = Bmp[Scape[x][y].Object].sourceRect.bottom;
                    }

                    rcRectdes.left = Scape[x][y].xScreen + Bmp[Scape[x][y].Object].targetRect.left;
                    rcRectdes.right = Scape[x][y].xScreen + Bmp[Scape[x][y].Object].targetRect.right;
                    rcRectdes.top = Scape[x][y].yScreen + Bmp[Scape[x][y].Object].targetRect.top;
                    rcRectdes.bottom = Scape[x][y].yScreen + Bmp[Scape[x][y].Object].targetRect.bottom;
                    // Landschaftsobjekt zeichnen
                    Renderer::Blitten(lpDDSAnimation, lpDDSScape, true);
                }
            }

            // MiniMap zeichnen
            rcRectdes.left = 2 * x;
            rcRectdes.top = 2 * y;
            rcRectdes.right = rcRectdes.left + 2;
            rcRectdes.bottom = rcRectdes.top + 2;

            sf::Color c;
            if ((Scape[x][y].Terrain == 1) && (Scape[x][y].Type == 0)) { // Meer
                c = sf::Color(228, 207, 182);
//                ddbltfx.dwFillColor = Renderer::RGB2DWORD(228, 207, 182);
            } else {
                if ((Scape[x][y].Type == 0) &&
                        ((Scape[x][y].Terrain == 2) ||
                         (Scape[x][y].Terrain == 3))) { // Strand
                    c = sf::Color(112, 103, 93);
                } else
                    // Land
                {
                    c = sf::Color(139 + Scape[x][y].Height * 20, 128 + Scape[x][y].Height * 20, 115 + Scape[x][y].Height * 20);
                }
            }
            lpDDSKarte->create(rcRectdes.right - rcRectdes.left, rcRectdes.bottom - rcRectdes.top, c);

//                lpDDSKarte->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);
        }
    }
}

void CheckSpzButton()
{
    if ((Scape[Guy.Pos.x][Guy.Pos.y].Object >= FIELD) && (Scape[Guy.Pos.x][Guy.Pos.y].Object <= BONFIRE) &&
            (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase >= Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount) &&
            (Bmp[BUTTON_STOP].AnimationPhase == -1)) {
        if (Bmp[BUTTON_CONTINUE].AnimationPhase == -1) {
            Bmp[BUTTON_CONTINUE].AnimationPhase = 0;
        }
    } else {
        Bmp[BUTTON_CONTINUE].AnimationPhase = -1;
    }

    if ((Bmp[BUTTON_STOP].AnimationPhase == -1) && (((Scape[Guy.Pos.x][Guy.Pos.y].Object == BOAT) &&
            (Scape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) ||
            ((IsInBoat) &&
             (((Scape[Guy.Pos.x - 1][Guy.Pos.y].Terrain != 1) && (Scape[Guy.Pos.x - 1][Guy.Pos.y].Object == -1)) ||
              ((Scape[Guy.Pos.x][Guy.Pos.y - 1].Terrain != 1) && (Scape[Guy.Pos.x][Guy.Pos.y - 1].Object == -1)) ||
              ((Scape[Guy.Pos.x + 1][Guy.Pos.y].Terrain != 1) && (Scape[Guy.Pos.x + 1][Guy.Pos.y].Object == -1)) ||
              ((Scape[Guy.Pos.x][Guy.Pos.y + 1].Terrain != 1) && (Scape[Guy.Pos.x][Guy.Pos.y + 1].Object == -1)))))) {
        if (Bmp[BUTTON_LAY_DOWN].AnimationPhase == -1) {
            Bmp[BUTTON_LAY_DOWN].AnimationPhase = 0;
        }
    } else {
        Bmp[BUTTON_LAY_DOWN].AnimationPhase = -1;
    }
}

bool CheckRohstoff()
{
    short Benoetigt = 0; // Anzahl der Gesamtbenötigten Rohstoffe

    for (short i = 0; i < SPRITE_COUNT; i++) {
        Benoetigt += Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].RequiredRawMaterials[i];
    }

    float GebrauchtTmp = Benoetigt / static_cast<float>(Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Object].RequiredActionCases); // Soviel Rohstoffe werden für diesen Schritt benötigt
    short Gebraucht = static_cast<short>(GebrauchtTmp * Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber -
                                         static_cast<short>(GebrauchtTmp * (Scape[Guy.Pos.x][Guy.Pos.y].ConstructionActionNumber - 1))); // Soviel Rohstoffe werden für diesen Schritt benötigt


    while (true) {
        bool Check = false; // Wenn kein Rohstoff mehr vorhanden nur noch einmal die While-Schleife

        for (short i = 0; i < SPRITE_COUNT; i++) {
            if (Gebraucht == 0) {
                return true;
            }

            if ((Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] > 0) &&
                    (Guy.Inventory[i] > 0)) {
                Guy.Inventory[i]--;
                Scape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i]--;
                Gebraucht--;

                if (Gebraucht == 0) {
                    return true;
                }

                Check = true;
            }
        }

        if (Check == false) {
            break;
        }
    }

    PapierText = Renderer::DrawText(ROHSTOFFNICHT, TXTPAPIER, 1);
    Guy.ActionNumber = 0;
    Guy.CurrentAction = Action::CANCEL;
    Bmp[BUTTON_STOP].AnimationPhase = -1;
    return false;
}

void Compute(short MinGroesse, short MaxGroesse) // Groesse der Insel in Anzahl der Landkacheln
{
    short i, j;
    bool hasFound;

    short Vierecke[13][4][13] = { //0=Passt nicht 1=1runter 2=gleiche Hoehe 3=1hoch
        //		  0	  1	  2	  3	  4	  5	  6	  7	  8	  9	 10	 11	 12
        {   {2, 0, 1, 0, 2, 2, 0, 0, 2, 0, 0, 1, 1},//0
            {2, 1, 0, 2, 0, 0, 0, 2, 2, 0, 1, 1, 0},//1
       /*0*/{2, 0, 2, 0, 1, 0, 2, 2, 0, 1, 1, 0, 0},//2 Kante
            {2, 2, 0, 1, 0, 2, 2, 0, 0, 1, 0, 0, 1} //3
        },
        {   {0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0},
            {2, 1, 0, 2, 0, 0, 0, 2, 2, 0, 1, 1, 0},
       /*1*/{0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0},
            {3, 3, 0, 2, 0, 3, 3, 0, 0, 2, 0, 0, 2}
        },
        {   {2, 0, 1, 0, 2, 2, 0, 0, 2, 0, 0, 1, 1},
            {0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2},
       /*2*/{3, 0, 3, 0, 2, 0, 3, 3, 0, 2, 2, 0, 0},
            {0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0}
        },
        {   {0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0},
            {3, 2, 0, 3, 0, 0, 0, 3, 3, 0, 2, 2, 0},
       /*3*/{0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2},
            {2, 2, 0, 1, 0, 0, 2, 0, 0, 1, 0, 0, 1}
        },
        {   {3, 0, 2, 0, 3, 3, 0, 0, 3, 0, 0, 2, 2},
            {0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0},
       /*4*/{2, 0, 2, 0, 0, 0, 2, 2, 0, 1, 1, 0, 0},
            {0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0}
        },//  0	             3  4     5	       	 8       9	 10	 11	 12
        {   {0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0},
            {2, 1, 0, 2, 0, 0, 0, 2, 2, 0, 1, 1, 0},
       /*5*/{2, 0, 2, 0, 1, 0, 2, 2, 0, 1, 1, 0, 0},
            {0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0}
        },
        {   {2, 0, 1, 0, 2, 2, 0, 0, 2, 0, 0, 1, 1},
            {2, 1, 0, 2, 0, 0, 0, 2, 2, 0, 1, 1, 0},
       /*6*/{0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0},
            {0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0}
        },
        {   {2, 0, 1, 0, 2, 2, 0, 0, 2, 0, 0, 1, 1},
            {0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2},
       /*7*/{0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2},
            {2, 2, 0, 1, 0, 2, 2, 0, 0, 1, 0, 0, 1}
        },
        {   {0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0},
            {0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0},
       /*8*/{2, 0, 2, 0, 1, 0, 2, 2, 0, 1, 1, 0, 0},
            {2, 2, 0, 1, 0, 2, 2, 0, 0, 1, 0, 0, 1}
        },
        {   {3, 0, 2, 0, 3, 3, 0, 0, 3, 0, 0, 2, 2},
            {3, 2, 0, 3, 0, 0, 0, 3, 3, 0, 2, 2, 0},
       /*9*/{0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2},
            {0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0}
        },//  0	             3  4     5	       	 8       9	 10	 11	 12
        {   {3, 0, 2, 0, 3, 3, 0, 0, 3, 0, 0, 2, 2},
            {0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0},
      /*10*/{0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0},
            {3, 3, 0, 2, 0, 3, 3, 0, 0, 2, 0, 0, 2}
        },
        {   {0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0},
            {0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2},
      /*11*/{3, 0, 3, 0, 2, 0, 3, 3, 0, 2, 2, 0, 0},
            {3, 3, 0, 2, 0, 3, 3, 0, 0, 2, 0, 0, 2}
        },
        {   {0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0},
            {3, 2, 0, 3, 0, 0, 0, 3, 3, 0, 2, 2, 0},
      /*12*/{3, 0, 3, 0, 2, 0, 3, 3, 0, 2, 2, 0, 0},
            {0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0}
        },
    };

    short MidX = MAX_TILES_X / 2 - 1;
    short MidY = MAX_TILESY / 2 - 1;

    for (short m = 0; m < 1000; m++) { // 100mal wiederholen, oder bis eine geeignete Insel gefunden ist
        for (short y = 0; y < MAX_TILESY; y++)
            for (short x = 0; x < MAX_TILES_X; x++) {
                Scape[x][y].Type = 0;
                Scape[x][y].Terrain = 0;
                Scape[x][y].Height = 0;
                Scape[x][y].Marked = false;
                Scape[x][y].Walkable = true;
                Scape[x][y].Discovered = false;
                Scape[x][y].RunningTime = 1;
                Scape[x][y].Object = -1;
                Scape[x][y].ReverseAnimation = false;
                Scape[x][y].ObjectPosOffset.x = 0;
                Scape[x][y].ObjectPosOffset.y = 0;
                Scape[x][y].AnimationPhase = -1;
                Scape[x][y].ConstructionActionNumber = 0;
                Scape[x][y].GPosAlt.x = 0;
                Scape[x][y].GPosAlt.y = 0;

                for (i = 0; i < SPRITE_COUNT; i++) {
                    Scape[x][y].RequiredRawMaterials[i] = 0;
                }

                Scape[x][y].FireTimer = 0;
            }

        short x; // Startposition der Berechnung
        short y;
        Scape[MidX][MidY].Type = 0; // Gipfel festlegen (Flach)
        Scape[MidX][MidY].Height = MOUNTAIN_HEIGHT; // und mit der Hoehe

        // Spiralring von Innen aus gezählt
        for (short l = 0; l <= MidY - 1; l++) {
            if (l >= MidX) {
                break;
            }

            y = MidY - l - 1;

            // Als erstes den oberen Bereich von links nach rechts durchgehen
            for (x = MidX - l; x <= MidX + l; x++) {
                hasFound = false;
                i = 0;

                while (!hasFound) { // Passendes Teil finden und Hoehe festlegen
                    i += 1;

                    if (i == 1000) {
                        hasFound = true;
                    }

                    Scape[x][y].Type = rand() % 13;

                    for (j = 0; j < 10; j++) {
                        if (!((Scape[x][y].Type == 0) || (Scape[x][y].Type == 1))) {
                            Scape[x][y].Type = rand() % 13;
                        }
                    }

                    if ((x == MidX - l) || ((x != MidX - l)
                                            && ((Vierecke[Scape[x][y + 1].Type][1][Scape[x][y].Type] != 0)
                                                && (Vierecke[Scape[x - 1][y].Type][2][Scape[x][y].Type] != 0)))) {
                        if (Vierecke[Scape[x][y + 1].Type][1][Scape[x][y].Type] == 1) {
                            Scape[x][y].Height = Scape[x][y + 1].Height - 1;

                            if (Scape[x][y].Height < 0) {
                                Scape[x][y].Type = 0;
                                Scape[x][y].Height = 0;
                            }

                            hasFound = true;
                        }

                        if (Vierecke[Scape[x][y + 1].Type][1][Scape[x][y].Type] == 2) {
                            Scape[x][y].Height = Scape[x][y + 1].Height;
                            hasFound = true;
                        }

                        if (Vierecke[Scape[x][y + 1].Type][1][Scape[x][y].Type] == 3) {
                            Scape[x][y].Height = Scape[x][y + 1].Height + 1;
                            hasFound = true;
                        }
                    }

                    // Verzwickte Fälle ausfiltern
                    if (((Vierecke[Scape[x][y].Type][2][3] == 2) && (Vierecke[Scape[x + 1][y + 1].Type][1][4] == 2)) ||
                            ((Vierecke[Scape[x][y].Type][2][1] == 2) && (Vierecke[Scape[x + 1][y + 1].Type][1][2] == 2))) {
                        hasFound = false;
                    }

                    // Nebeninseln vermeiden
                    if (((Scape[x - 1][y].Type == 0) && (Scape[x - 1][y].Height == 0)) &&
                            ((Scape[x][y + 1].Type == 0) && (Scape[x][y + 1].Height == 0))) {
                        Scape[x][y].Type = 0;
                        Scape[x][y].Height = 0;
                    }
                }
            }

            // Teil rechts-oben
            x = MidX + l + 1;
            y = MidY - l - 1;
            hasFound = false;
            i = 0;

            while (!hasFound) { // Passendes Teil finden und Hoehe festlegen
                i += 1;

                if (i == 1000) {
                    hasFound = true;
                }

                Scape[x][y].Type = rand() % 13;

                for (j = 0; j < 10; j++) {
                    if (!((Scape[x][y].Type == 0) || (Scape[x][y].Type == 5))) {
                        Scape[x][y].Type = rand() % 13;
                    }
                }

                if (Vierecke[Scape[x - 1][y].Type][2][Scape[x][y].Type] == 1) {
                    Scape[x][y].Height = Scape[x - 1][y].Height - 1;

                    if (Scape[x][y].Height < 0) {
                        Scape[x][y].Type = 0;
                        Scape[x][y].Height = 0;
                    }

                    hasFound = true;
                }

                if (Vierecke[Scape[x - 1][y].Type][2][Scape[x][y].Type] == 2) {
                    Scape[x][y].Height = Scape[x - 1][y].Height;
                    hasFound = true;
                }

                if (Vierecke[Scape[x - 1][y].Type][2][Scape[x][y].Type] == 3) {
                    Scape[x][y].Height = Scape[x - 1][y].Height + 1;
                    hasFound = true;
                }

                // Verzwickte Fälle ausfiltern
                if (((Vierecke[Scape[x][y].Type][3][2] == 2) && (Vierecke[Scape[x - 1][y + 1].Type][2][3] == 2)) ||
                        ((Vierecke[Scape[x][y].Type][3][4] == 2) && (Vierecke[Scape[x - 1][y + 1].Type][2][1] == 2))) {
                    hasFound = false;
                }

                // Nebeninseln vermeiden
                if ((Scape[x - 1][y].Type == 0) && (Scape[x - 1][y].Height == 0)) {
                    Scape[x][y].Type = 0;
                    Scape[x][y].Height = 0;
                }
            }

            // Den rechten Bereich von oben nach unten durchgehen
            x = MidX + l + 1;

            for (y = MidY - l; y <= MidY + l; y++) {
                hasFound = false;
                i = 0;

                while (!hasFound) { // Passendes Teil finden und Hoehe festlegen
                    i += 1;

                    if (i == 1000) {
                        hasFound = true;
                    }

                    Scape[x][y].Type = rand() % 13;

                    for (j = 0; j < 10; j++) {
                        if (!((Scape[x][y].Type == 0) || (Scape[x][y].Type == 4))) {
                            Scape[x][y].Type = rand() % 13;
                        }
                    }

                    if ((Vierecke[Scape[x - 1][y].Type][2][Scape[x][y].Type] != 0)
                            && (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] != 0)) {
                        if (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] == 1) {
                            Scape[x][y].Height = Scape[x][y - 1].Height - 1;

                            if (Scape[x][y].Height < 0) {
                                Scape[x][y].Type = 0;
                                Scape[x][y].Height = 0;
                            }

                            hasFound = true;
                        }

                        if (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] == 2) {
                            Scape[x][y].Height = Scape[x][y - 1].Height;
                            hasFound = true;
                        }

                        if (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] == 3) {
                            Scape[x][y].Height = Scape[x][y - 1].Height + 1;
                            hasFound = true;
                        }
                    }

                    // Verzwickte Fälle ausfiltern
                    if (((Vierecke[Scape[x][y].Type][3][2] == 2) && (Vierecke[Scape[x - 1][y + 1].Type][2][3] == 2)) ||
                            ((Vierecke[Scape[x][y].Type][3][4] == 2) && (Vierecke[Scape[x - 1][y + 1].Type][2][1] == 2))) {
                        hasFound = false;
                    }

                    // Nebeninseln vermeiden
                    if (((Scape[x - 1][y].Type == 0) && (Scape[x - 1][y].Height == 0)) &&
                            ((Scape[x][y - 1].Type == 0) && (Scape[x][y - 1].Height == 0))) {
                        Scape[x][y].Type = 0;
                        Scape[x][y].Height = 0;
                    }
                }
            }

            // Teil rechts-unten
            x = MidX + l + 1;
            y = MidY + l + 1;
            hasFound = false;
            i = 0;

            while (!hasFound) { // Passendes Teil finden und Hoehe festlegen
                i += 1;

                if (i == 1000) {
                    hasFound = true;
                }

                Scape[x][y].Type = rand() % 13;

                for (j = 0; j < 10; j++) {
                    if (!((Scape[x][y].Type == 0) || (Scape[x][y].Type == 8))) {
                        Scape[x][y].Type = rand() % 13;
                    }
                }

                if (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] == 1) {
                    Scape[x][y].Height = Scape[x][y - 1].Height - 1;

                    if (Scape[x][y].Height < 0) {
                        Scape[x][y].Type = 0;
                        Scape[x][y].Height = 0;
                    }

                    hasFound = true;
                }

                if (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] == 2) {
                    Scape[x][y].Height = Scape[x][y - 1].Height;
                    hasFound = true;
                }

                if (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] == 3) {
                    Scape[x][y].Height = Scape[x][y - 1].Height + 1;
                    hasFound = true;
                }

                // Verzwickte Fälle ausfiltern
                if (((Vierecke[Scape[x][y].Type][0][1] == 2) && (Vierecke[Scape[x - 1][y - 1].Type][3][2] == 2)) ||
                        ((Vierecke[Scape[x][y].Type][0][3] == 2) && (Vierecke[Scape[x - 1][y - 1].Type][3][4] == 2))) {
                    hasFound = false;
                }

                // Nebeninsel vermeiden
                if ((Scape[x][y - 1].Type == 0) && (Scape[x][y - 1].Height == 0)) {
                    Scape[x][y].Type = 0;
                    Scape[x][y].Height = 0;
                }
            }

            // Den unteren Bereich von rechts nach links durchgehen
            y = MidY + l + 1;

            for (x = MidX + l; x >= MidX - l; x--) {
                hasFound = false;
                i = 0;

                while (!hasFound) { // Passendes Teil finden und Hoehe festlegen
                    i += 1;

                    if (i == 1000) {
                        hasFound = true;
                    }

                    Scape[x][y].Type = rand() % 13;

                    for (j = 0; j < 10; j++) {
                        if (!((Scape[x][y].Type == 0) || (Scape[x][y].Type == 3))) {
                            Scape[x][y].Type = rand() % 13;
                        }
                    }

                    if ((Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] != 0)
                            && (Vierecke[Scape[x + 1][y].Type][0][Scape[x][y].Type] != 0)) {
                        if (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] == 1) {
                            Scape[x][y].Height = Scape[x][y - 1].Height - 1;

                            if (Scape[x][y].Height < 0) {
                                Scape[x][y].Type = 0;
                                Scape[x][y].Height = 0;
                            }

                            hasFound = true;
                        }

                        if (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] == 2) {
                            Scape[x][y].Height = Scape[x][y - 1].Height;
                            hasFound = true;
                        }

                        if (Vierecke[Scape[x][y - 1].Type][3][Scape[x][y].Type] == 3) {
                            Scape[x][y].Height = Scape[x][y - 1].Height + 1;
                            hasFound = true;
                        }
                    }

                    // Verzwickte Fälle ausfiltern
                    if (((Vierecke[Scape[x][y].Type][0][1] == 2) && (Vierecke[Scape[x - 1][y - 1].Type][3][2] == 2)) ||
                            ((Vierecke[Scape[x][y].Type][0][3] == 2) && (Vierecke[Scape[x - 1][y - 1].Type][3][4] == 2))) {
                        hasFound = false;
                    }

                    // Nebeninseln vermeiden
                    if (((Scape[x + 1][y].Type == 0) && (Scape[x + 1][y].Height == 0)) &&
                            ((Scape[x][y - 1].Type == 0) && (Scape[x][y - 1].Height == 0))) {
                        Scape[x][y].Type = 0;
                        Scape[x][y].Height = 0;
                    }
                }
            }

            //Teil links-unten
            x = MidX - l - 1;
            y = MidY + l + 1;
            hasFound = false;
            i = 0;

            while (!hasFound) { // Passendes Teil finden und Hoehe festlegen
                i += 1;

                if (i == 1000) {
                    hasFound = true;
                }

                Scape[x][y].Type = rand() % 13;

                for (j = 0; j < 10; j++) {
                    if (!((Scape[x][y].Type == 0) || (Scape[x][y].Type == 7))) {
                        Scape[x][y].Type = rand() % 13;
                    }
                }

                if (Vierecke[Scape[x + 1][y].Type][0][Scape[x][y].Type] == 1) {
                    Scape[x][y].Height = Scape[x + 1][y].Height - 1;

                    if (Scape[x][y].Height < 0) {
                        Scape[x][y].Type = 0;
                        Scape[x][y].Height = 0;
                    }

                    hasFound = true;
                }

                if (Vierecke[Scape[x + 1][y].Type][0][Scape[x][y].Type] == 2) {
                    Scape[x][y].Height = Scape[x + 1][y].Height;
                    hasFound = true;
                }

                if (Vierecke[Scape[x + 1][y].Type][0][Scape[x][y].Type] == 3) {
                    Scape[x][y].Height = Scape[x + 1][y].Height + 1;
                    hasFound = true;
                }

                // Verzwickte Fälle ausfiltern
                if (((Vierecke[Scape[x][y].Type][1][2] == 2) && (Vierecke[Scape[x + 1][y - 1].Type][0][3] == 2)) ||
                        ((Vierecke[Scape[x][y].Type][1][4] == 2) && (Vierecke[Scape[x + 1][y - 1].Type][0][1] == 2))) {
                    hasFound = false;
                }

                // Nebeninsel vermeiden
                if ((Scape[x + 1][y].Type == 0) && (Scape[x + 1][y].Height == 0)) {
                    Scape[x][y].Type = 0;
                    Scape[x][y].Height = 0;
                }
            }

            // Den linken Bereich von unten nach oben durchgehen
            x = MidX - l - 1;

            for (y = MidY + l; y >= MidY - l - 1; y--) {
                hasFound = false;
                i = 0;

                while (!hasFound) { // Passendes Teil finden und Hoehe festlegen
                    i += 1;

                    if (i == 1000) {
                        hasFound = true;
                    }

                    Scape[x][y].Type = rand() % 13;

                    for (j = 0; j < 10; j++) {
                        if (!((Scape[x][y].Type == 0) || (Scape[x][y].Type == 2))) {
                            Scape[x][y].Type = rand() % 13;
                        }
                    }

                    if ((Vierecke[Scape[x + 1][y].Type][0][Scape[x][y].Type] != 0)
                            && (Vierecke[Scape[x][y + 1].Type][1][Scape[x][y].Type] != 0)) {
                        if (Vierecke[Scape[x][y + 1].Type][1][Scape[x][y].Type] == 1) {
                            Scape[x][y].Height = Scape[x][y + 1].Height - 1;

                            if (Scape[x][y].Height < 0) {
                                Scape[x][y].Type = 0;
                                Scape[x][y].Height = 0;
                            }

                            hasFound = true;
                        }

                        if (Vierecke[Scape[x][y + 1].Type][1][Scape[x][y].Type] == 2) {
                            Scape[x][y].Height = Scape[x][y + 1].Height;
                            hasFound = true;
                        }

                        if (Vierecke[Scape[x][y + 1].Type][1][Scape[x][y].Type] == 3) {
                            Scape[x][y].Height = Scape[x][y + 1].Height + 1;
                            hasFound = true;
                        }
                    }

                    // Verzwickte Fälle ausfiltern
                    if (((Vierecke[Scape[x][y].Type][1][2] == 2) && (Vierecke[Scape[x + 1][y - 1].Type][0][3] == 2)) ||
                            ((Vierecke[Scape[x][y].Type][1][4] == 2) && (Vierecke[Scape[x + 1][y - 1].Type][0][1] == 2))) {
                        hasFound = false;
                    }

                    // Nebeninseln vermeiden
                    if (((Scape[x + 1][y].Type == 0) && (Scape[x + 1][y].Height == 0)) &&
                            ((Scape[x][y + 1].Type == 0) && (Scape[x][y + 1].Height == 0))) {
                        Scape[x][y].Type = 0;
                        Scape[x][y].Height = 0;
                    }
                }
            }
        }

        short Anzahl = 0; // Anzahl der Landstücke
        bool CheckRand = true; // Reicht die Insel bis zum Rand?

        for (y = 0; y < MAX_TILESY; y++) // Landfläche zählen
            for (x = 0; x < MAX_TILES_X; x++) {
                if (Scape[x][y].Height > 0) {
                    Anzahl++;
                }

                if (Scape[x][y].Type == 0) {
                    Scape[x][y].RunningTime = 1;
                } else {
                    Scape[x][y].RunningTime = 2;
                }

                if ((Scape[x][y].Type != 0) &&
                        ((x <= 2) || (x >= MAX_TILES_X - 2) || (y <= 2) || (y >= MAX_TILESY - 2))) {
                    CheckRand = false;
                }
            }

        if ((Anzahl > MinGroesse) && (Anzahl < MaxGroesse) && (CheckRand)) {
            break;
        }
    }
}

void Meer() // Das Meer und den Strand bestimmen
{
    short x, y;

    for (y = 0; y < MAX_TILESY; y++) // Meer rausfinden
        for (x = 0; x < MAX_TILES_X; x++) {
            if ((Scape[x][y].Height < 0) ||
                    ((Scape[x][y].Height == 0) && (Scape[x][y].Type == 0))) {
                Scape[x][y].Type = 0;
                Scape[x][y].Height = 0;
                Scape[x][y].Terrain = 1;
                Scape[x][y].Object = SEA_WAVES;
                Scape[x][y].ObjectPosOffset.x = static_cast<short>(Bmp[SEA_WAVES].targetRect.left);
                Scape[x][y].ObjectPosOffset.y = static_cast<short>(Bmp[SEA_WAVES].targetRect.top);

                if (rand() % 2 == 0) {
                    Scape[x][y].ReverseAnimation = true;
                }

                Scape[x][y].Walkable = false;
                Scape[x][y].AnimationPhase = static_cast<float>(Bmp[Scape[x][y].Object].AnimationPhaseCount -
                                             rand() % (Bmp[Scape[x][y].Object].AnimationPhaseCount) - 1);
            }
        }

    for (y = 1; y < MAX_TILESY - 1; y++) // Strand rausfinden
        for (x = 1; x < MAX_TILES_X - 1; x++) { // Alle Möglichkeiten durchgehen
            if ((Scape[x][y].Type == 0) && (Scape[x][y].Height == 0)) {
                short Anzahl = 0; // Anzahl von angrenzenden Landstücken rausfinden

                if (Scape[x - 1][y].Type != 0) {
                    Anzahl++;
                }

                if (Scape[x - 1][y - 1].Type != 0) {
                    Anzahl++;
                }

                if (Scape[x][y - 1].Type != 0) {
                    Anzahl++;
                }

                if (Scape[x + 1][y - 1].Type != 0) {
                    Anzahl++;
                }

                if (Scape[x + 1][y].Type != 0) {
                    Anzahl++;
                }

                if (Scape[x + 1][y + 1].Type != 0) {
                    Anzahl++;
                }

                if (Scape[x][y + 1].Type != 0) {
                    Anzahl++;
                }

                if (Scape[x - 1][y + 1].Type != 0) {
                    Anzahl++;
                }

                if ((Anzahl >= 1) && (Anzahl < 6)) {
                    Scape[x][y].Terrain = 2;
                    Scape[x][y].Object = -1;
                    Scape[x][y].ReverseAnimation = false;
                    Scape[x][y].Walkable = true;
                    Scape[x][y].AnimationPhase = -1;
                    continue;
                }

                if (Anzahl >= 6) {
                    Scape[x][y].Terrain = 3;
                    Scape[x][y].Object = -1;
                    Scape[x][y].ReverseAnimation = false;
                    Scape[x][y].Walkable = false;
                    Scape[x][y].AnimationPhase = -1;
                    continue;
                }

                Scape[x][y].Terrain = 1; // sonst Meer
                Scape[x][y].Object = SEA_WAVES;
                Scape[x][y].ObjectPosOffset.x = static_cast<short>(Bmp[SEA_WAVES].targetRect.left);
                Scape[x][y].ObjectPosOffset.y = static_cast<short>(Bmp[SEA_WAVES].targetRect.top);
                Scape[x][y].AnimationPhase = static_cast<float>(Bmp[Scape[x][y].Object].AnimationPhaseCount -
                                             rand() % (Bmp[Scape[x][y].Object].AnimationPhaseCount) - 1);

                if (rand() % 2 == 0) {
                    Scape[x][y].ReverseAnimation = true;
                }

                Scape[x][y].Walkable = false;
            }
        }
}

void ToggleIsInBoat()
{
    IsInBoat = !IsInBoat;

    // Begehbarkeit umdrehen
    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            Scape[x][y].Walkable = !Scape[x][y].Walkable;
        }
}

void CheckPipe(short x, short y)
{
    Scape[x][y].AnimationPhase = 1;

    if (Scape[x][y].Terrain == 0) {
        Scape[x][y].Terrain = 4;
    }

    if (Scape[x - 1][y].Terrain == 0) {
        Scape[x - 1][y].Terrain = 4;
    }

    if (Scape[x - 1][y - 1].Terrain == 0) {
        Scape[x - 1][y - 1].Terrain = 4;
    }

    if (Scape[x][y - 1].Terrain == 0) {
        Scape[x][y - 1].Terrain = 4;
    }

    if (Scape[x + 1][y - 1].Terrain == 0) {
        Scape[x + 1][y - 1].Terrain = 4;
    }

    if (Scape[x + 1][y].Terrain == 0) {
        Scape[x + 1][y].Terrain = 4;
    }

    if (Scape[x + 1][y + 1].Terrain == 0) {
        Scape[x + 1][y + 1].Terrain = 4;
    }

    if (Scape[x][y + 1].Terrain == 0) {
        Scape[x][y + 1].Terrain = 4;
    }

    if (Scape[x - 1][y + 1].Terrain == 0) {
        Scape[x - 1][y + 1].Terrain = 4;
    }

    if ((Scape[x - 1][y].Object == PIPE) && (Scape[x - 1][y].AnimationPhase == 0)) {
        CheckPipe(x - 1, y);
    }

    if ((Scape[x][y - 1].Object == PIPE) && (Scape[x][y - 1].AnimationPhase == 0)) {
        CheckPipe(x, y - 1);
    }

    if ((Scape[x + 1][y].Object == PIPE) && (Scape[x + 1][y].AnimationPhase == 0)) {
        CheckPipe(x + 1, y);
    }

    if ((Scape[x][y + 1].Object == PIPE) && (Scape[x][y + 1].AnimationPhase == 0)) {
        CheckPipe(x, y + 1);
    }
}

void FillPipe()
{
    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            if ((Scape[x][y].Object == PIPE) && (Scape[x][y].AnimationPhase < Bmp[PIPE].AnimationPhaseCount)) {
                Scape[x][y].AnimationPhase = 0;
            }

            if (Scape[x][y].Terrain == 4) {
                Scape[x][y].Terrain = 0;
            }

            if ((Scape[x][y].Object >= FLOODGATE_1) && (Scape[x][y].Object <= FLOODGATE_6)) {
                Scape[x][y].Object -= 14;
                Scape[x][y].ObjectPosOffset.x = static_cast<short>(Bmp[Scape[x][y].Object].targetRect.left);
                Scape[x][y].ObjectPosOffset.y = static_cast<short>(Bmp[Scape[x][y].Object].targetRect.top);
            }
        }

    // StartRohr finden
    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            if ((Scape[x][y].Object >= RIVER_1) && (Scape[x][y].Object <= FLOODGATE_6)) {
                if (Scape[x][y].Terrain == 0) {
                    Scape[x][y].Terrain = 4;
                }

                if (Scape[x - 1][y].Terrain == 0) {
                    Scape[x - 1][y].Terrain = 4;
                }

                if (Scape[x - 1][y - 1].Terrain == 0) {
                    Scape[x - 1][y - 1].Terrain = 4;
                }

                if (Scape[x][y - 1].Terrain == 0) {
                    Scape[x][y - 1].Terrain = 4;
                }

                if (Scape[x + 1][y - 1].Terrain == 0) {
                    Scape[x + 1][y - 1].Terrain = 4;
                }

                if (Scape[x + 1][y].Terrain == 0) {
                    Scape[x + 1][y].Terrain = 4;
                }

                if (Scape[x + 1][y + 1].Terrain == 0) {
                    Scape[x + 1][y + 1].Terrain = 4;
                }

                if (Scape[x][y + 1].Terrain == 0) {
                    Scape[x][y + 1].Terrain = 4;
                }

                if (Scape[x - 1][y + 1].Terrain == 0) {
                    Scape[x - 1][y + 1].Terrain = 4;
                }
            }

            if ((Scape[x][y].Object != PIPE) || (Scape[x][y].AnimationPhase >= Bmp[PIPE].AnimationPhaseCount)) {
                continue;
            }

            if ((Scape[x - 1][y].Object >= RIVER_5) && (Scape[x - 1][y].Object <= RIVER_10)) {
                Scape[x - 1][y].Object += 14;
                Scape[x - 1][y].ObjectPosOffset.x = static_cast<short>(Bmp[Scape[x - 1][y].Object].targetRect.left);
                Scape[x - 1][y].ObjectPosOffset.y = static_cast<short>(Bmp[Scape[x - 1][y].Object].targetRect.top);
                CheckPipe(x, y);
            } else if ((Scape[x - 1][y].Object >= FLOODGATE_1) && (Scape[x - 1][y].Object <= FLOODGATE_6)) {
                CheckPipe(x, y);
            }

            if ((Scape[x][y - 1].Object >= RIVER_5) && (Scape[x][y - 1].Object <= RIVER_10)) {
                Scape[x][y - 1].Object += 14;
                Scape[x][y - 1].ObjectPosOffset.x = static_cast<short>(Bmp[Scape[x][y - 1].Object].targetRect.left);
                Scape[x][y - 1].ObjectPosOffset.y = static_cast<short>(Bmp[Scape[x][y - 1].Object].targetRect.top);
                CheckPipe(x, y);
            } else if ((Scape[x][y - 1].Object >= FLOODGATE_1) && (Scape[x][y - 1].Object <= FLOODGATE_6)) {
                CheckPipe(x, y);
            }

            if ((Scape[x + 1][y].Object >= RIVER_5) && (Scape[x + 1][y].Object <= RIVER_10)) {
                Scape[x + 1][y].Object += 14;
                Scape[x + 1][y].ObjectPosOffset.x = static_cast<short>(Bmp[Scape[x + 1][y].Object].targetRect.left);
                Scape[x + 1][y].ObjectPosOffset.y = static_cast<short>(Bmp[Scape[x + 1][y].Object].targetRect.top);
                CheckPipe(x, y);
            } else if ((Scape[x + 1][y].Object >= FLOODGATE_1) && (Scape[x + 1][y].Object <= FLOODGATE_6)) {
                CheckPipe(x, y);
            }

            if ((Scape[x][y + 1].Object >= RIVER_5) && (Scape[x][y + 1].Object <= RIVER_10)) {
                Scape[x][y + 1].Object += 14;
                Scape[x][y + 1].ObjectPosOffset.x = static_cast<short>(Bmp[Scape[x][y + 1].Object].targetRect.left);
                Scape[x][y + 1].ObjectPosOffset.y = static_cast<short>(Bmp[Scape[x][y + 1].Object].targetRect.top);
                CheckPipe(x, y);
            } else if ((Scape[x][y + 1].Object >= FLOODGATE_1) && (Scape[x][y + 1].Object <= FLOODGATE_6)) {
                CheckPipe(x, y);
            }
        }

    // Felder auf trockenen Wiesen löschen
    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            if ((Scape[x][y].Object == FIELD) && (Scape[x][y].Terrain == 0)) {
                Scape[x][y].Object = -1;
                Scape[x][y].ObjectPosOffset.x = 0;
                Scape[x][y].ObjectPosOffset.y = 0;
                Scape[x][y].AnimationPhase = -1;
                Scape[x][y].ConstructionActionNumber = 0;
            }
        }

    Generate();
}

bool CheckFlow(short x, short y) // Nachprüfen ob auf aktuellem Teil ein Fluss ist (Nur für die Fluss-Routine)
{
    for (short i = 0; i < NUMBER_OF_RIVERS; i++)
        for (short j = 0; j < MAX_RIVER_LENGTH; j++) {
            if ((x == Flusslauf[i][j].x) && (y == Flusslauf[i][j].y)) {
                return true;
            }
        }

    return false;
}

void Flow() // Anzahl der Flüsse und der minimale Länge
{
    short i;
    short l = 0;
    short Richtung = 0; // Aktuelle Fliesrichtung von 0-3
    short x0 = 0;
    short y0 = 0;
    short x1 = 0;
    short y1 = 0;
    short x2 = 0; // x2,y2 Koordinate des zu prüfenden Teils
    short y2 = 0;
    bool gefunden = false;
    RiverRun Flusstmp[MAX_RIVER_LENGTH]; // Zum zwischenspeichern des Versuchs
    short Flusslaenge[NUMBER_OF_RIVERS];

    for (short m = 0; m < NUMBER_OF_RIVERS; m++)
        for (short j = 0; j < MAX_RIVER_LENGTH; j++) {
            Flusslauf[m][j].x = -1;
            Flusslauf[m][j].y = -1;
        }

    for (short m = 0; m < NUMBER_OF_RIVERS; m++) {
        bool fertig = false;
        short Laengetmp = 0;

        for (short j = 0; j < MAX_RIVER_LENGTH; j++) {
            Flusstmp[j].x = -1;
            Flusstmp[j].y = -1;
        }

        for (short k = 0; k < 1000; k++) {
            for (short o = 0; o < 10000; o++) {
                gefunden = true;

                x0 = rand() % MAX_TILES_X; // geeignete Quelle bestimmen
                y0 = rand() % MAX_TILESY;

                if (CheckFlow(x0, y0)) {
                    gefunden = false;
                }

                if ((Scape[x0][y0].Type != 0) ||
                        (Scape[x0][y0].Height < 2)) {
                    gefunden = false;
                }

                if (gefunden) {
                    break;
                }
            }

            if (!gefunden) {
                MessageBeep(MB_OK);
                break;
            } // Wenn keine Quelle mehr gefunden aufhören

            Flusslauf[m][0].x = x0;
            Flusslauf[m][0].y = y0;
            bool Strand = false;

            for (i = 1; i < MAX_RIVER_LENGTH; i++) {
                gefunden = false;

                if (!Strand) {
                    Richtung = rand() % 4;    // Auf dem Strand geradeausfliessen
                }

                for (short l2 = 0; l2 < 4; l2++) {
                    l = (Richtung + l2) % 4; // Im Urzeigersinn nachprüfen und bei der vorgegeben Richtung anfangen

                    x1 = Flusslauf[m][i - 1].x;
                    y1 = Flusslauf[m][i - 1].y;

                    if (l == 0) {
                        x2 = x1 - 1;
                        y2 = y1;

                        if (((Scape[x1][y1].Type == 0) || (Scape[x1][y1].Type == 2)) &&
                                ((Scape[x2][y2].Type == 0) || (Scape[x2][y2].Type == 2))) {
                            gefunden = (!CheckFlow(x2, y2));

                            if (gefunden) {
                                break;
                            }
                        }
                    }

                    if (l == 1) {
                        x2 = x1;
                        y2 = y1 - 1;

                        if (((Scape[x1][y1].Type == 0) || (Scape[x1][y1].Type == 1)) &&
                                ((Scape[x2][y2].Type == 0) || (Scape[x2][y2].Type == 1))) {
                            gefunden = (!CheckFlow(x2, y2));

                            if (gefunden) {
                                break;
                            }
                        }
                    }

                    if (l == 2) {
                        x2 = x1 + 1;
                        y2 = y1;

                        if (((Scape[x1][y1].Type == 0) || (Scape[x1][y1].Type == 4)) &&
                                ((Scape[x2][y2].Type == 0) || (Scape[x2][y2].Type == 4))) {
                            gefunden = (!CheckFlow(x2, y2));

                            if (gefunden) {
                                break;
                            }
                        }
                    }

                    if (l == 3) {
                        x2 = x1;
                        y2 = y1 + 1;

                        if (((Scape[x1][y1].Type == 0) || (Scape[x1][y1].Type == 3)) &&
                                ((Scape[x2][y2].Type == 0) || (Scape[x2][y2].Type == 3))) {
                            gefunden = (!CheckFlow(x2, y2));

                            if (gefunden) {
                                break;
                            }
                        }
                    }
                }

                if (!gefunden) {
                    break;
                }

                Flusslauf[m][i].x = x2;
                Flusslauf[m][i].y = y2;
                Richtung = l;

                if ((Scape[Flusslauf[m][i].x][Flusslauf[m][i].y].Terrain == 2) && // Auf Strand die Richtung beibehalten
                        (Scape[Flusslauf[m][i].x][Flusslauf[m][i].y].Type == 0)) {
                    if (Strand == true) {
                        break;    // Nur ein Strandstück überfliessen
                    }

                    Strand = true;
                }

                if ((Scape[Flusslauf[m][i].x][Flusslauf[m][i].y].Terrain == 1) && // im meer aufhören
                        (Scape[Flusslauf[m][i].x][Flusslauf[m][i].y].Type == 0)) {
                    fertig = true;
                    break;
                }
            }

            if (fertig) {
                if (i > Laengetmp) {
                    // neue Variante speichern
                    if (gefunden) {
                        Laengetmp = i;
                    } else {
                        Laengetmp = i - 1;
                    }

                    for (short j = 0; j <= Laengetmp; j++) {
                        Flusstmp[j].x = Flusslauf[m][j].x;
                        Flusstmp[j].y = Flusslauf[m][j].y;
                    }
                }
            }

            for (i = 0; i < MAX_RIVER_LENGTH; i++) {
                Flusslauf[m][i].x = -1;
                Flusslauf[m][i].y = -1;
            }

            fertig = false;
        }

        // Den längsten Fluss nehmen
        Flusslaenge[m] = Laengetmp;

        for (short j = 0; j <= Flusslaenge[m]; j++) {
            Flusslauf[m][j].x = Flusstmp[j].x;
            Flusslauf[m][j].y = Flusstmp[j].y;
        }

        // Die richtigen Wasserkacheln auswählen
        x0 = y0 = x1 = y1 = x2 = y2 = -1;

        for (m = 0; m < NUMBER_OF_RIVERS; m++) {
            for (i = 0; i <= Flusslaenge[m]; i++) {
                // Für die Kachel, einen Vorgang davor
                Scape[x1][y1].ObjectPosOffset.x = static_cast<short>(Bmp[Scape[x1][y1].Object].targetRect.left);
                Scape[x1][y1].ObjectPosOffset.y = static_cast<short>(Bmp[Scape[x1][y1].Object].targetRect.top);

                x1 = Flusslauf[m][i].x;
                y1 = Flusslauf[m][i].y;

                Scape[x1][y1].AnimationPhase = 0;

                if (Scape[x1][y1].Terrain == 0) {
                    Scape[x1][y1].Terrain = 4;
                }

                if (Scape[x1 - 1][y1].Terrain == 0) {
                    Scape[x1 - 1][y1].Terrain = 4;
                }

                if (Scape[x1 - 1][y1 - 1].Terrain == 0) {
                    Scape[x1 - 1][y1 - 1].Terrain = 4;
                }

                if (Scape[x1][y1 - 1].Terrain == 0) {
                    Scape[x1][y1 - 1].Terrain = 4;
                }

                if (Scape[x1 + 1][y1 - 1].Terrain == 0) {
                    Scape[x1 + 1][y1 - 1].Terrain = 4;
                }

                if (Scape[x1 + 1][y1].Terrain == 0) {
                    Scape[x1 + 1][y1].Terrain = 4;
                }

                if (Scape[x1 + 1][y1 + 1].Terrain == 0) {
                    Scape[x1 + 1][y1 + 1].Terrain = 4;
                }

                if (Scape[x1][y1 + 1].Terrain == 0) {
                    Scape[x1][y1 + 1].Terrain = 4;
                }

                if (Scape[x1 - 1][y1 + 1].Terrain == 0) {
                    Scape[x1 - 1][y1 + 1].Terrain = 4;
                }

                if (i < Flusslaenge[m]) {
                    x2 = Flusslauf[m][i + 1].x;
                    y2 = Flusslauf[m][i + 1].y;
                }

                if (i != 0) {
                    x0 = Flusslauf[m][i - 1].x;
                    y0 = Flusslauf[m][i - 1].y;
                } else {
                    // Quellen
                    if (x2 > x1) {
                        Scape[x1][y1].Object = RIVER_START_1;
                        continue;
                    }

                    if (x2 < x1) {
                        Scape[x1][y1].Object = RIVER_START_4;
                        continue;
                    }

                    if (y2 > y1) {
                        Scape[x1][y1].Object = RIVER_START_2;
                        continue;
                    }

                    if (y2 < y1) {
                        Scape[x1][y1].Object = RIVER_START_3;
                        continue;
                    }
                }

                // Alle Möglichkeiten durchgehen

                if (Scape[x1][y1].Type == 1) {
                    Scape[x1][y1].Object = RIVER_1;
                }

                if (Scape[x1][y1].Type == 2) {
                    Scape[x1][y1].Object = RIVER_2;
                }

                if (Scape[x1][y1].Type == 3) {
                    Scape[x1][y1].Object = RIVER_3;
                }

                if (Scape[x1][y1].Type == 4) {
                    Scape[x1][y1].Object = RIVER_4;
                }

                if (Scape[x1][y1].Type == 0) {
                    if ((x0 < x1) && (y0 == y1)) {
                        if (Scape[x1][y1].Terrain == 2) {
                            Scape[x1][y1].Object = RIVER_END_3;    // Mündung
                        } else {
                            if ((x1 < x2) && (y1 == y2)) {
                                Scape[x1][y1].Object = RIVER_5;
                            }

                            if ((x1 == x2) && (y1 < y2)) {
                                Scape[x1][y1].Object = RIVER_7;
                            }

                            if ((x1 == x2) && (y1 > y2)) {
                                Scape[x1][y1].Object = RIVER_9;
                            }
                        }
                    }

                    if ((x0 == x1) && (y0 < y1)) {
                        if (Scape[x1][y1].Terrain == 2) {
                            Scape[x1][y1].Object = RIVER_END_4;    // Mündung
                        } else {
                            if ((x1 < x2) && (y1 == y2)) {
                                Scape[x1][y1].Object = RIVER_8;
                            }

                            if ((x1 == x2) && (y1 < y2)) {
                                Scape[x1][y1].Object = RIVER_6;
                            }

                            if ((x1 > x2) && (y1 == y2)) {
                                Scape[x1][y1].Object = RIVER_9;
                                Scape[x1][y1].ReverseAnimation = true;
                            }
                        }
                    }

                    if ((x0 > x1) && (y0 == y1)) {
                        if (Scape[x1][y1].Terrain == 2) {
                            Scape[x1][y1].Object = RIVER_END_1;    // Mündung
                        } else {
                            if ((x1 > x2) && (y1 == y2)) {
                                Scape[x1][y1].Object = RIVER_5;
                            }

                            if ((x1 == x2) && (y1 < y2)) {
                                Scape[x1][y1].Object = RIVER_10;
                            }

                            if ((x1 == x2) && (y1 > y2)) {
                                Scape[x1][y1].Object = RIVER_8;
                            }

                            Scape[x1][y1].ReverseAnimation = true;
                        }
                    }

                    if ((x0 == x1) && (y0 > y1)) {
                        if (Scape[x1][y1].Terrain == 2) {
                            Scape[x1][y1].Object = RIVER_END_2;    // Mündung
                        } else {
                            if ((x1 == x2) && (y1 > y2)) {
                                Scape[x1][y1].Object = RIVER_6;
                            }

                            if ((x1 > x2) && (y1 == y2)) {
                                Scape[x1][y1].Object = RIVER_7;
                            }

                            Scape[x1][y1].ReverseAnimation = true;

                            if ((x1 < x2) && (y1 == y2)) {
                                Scape[x1][y1].Object = RIVER_10;
                                Scape[x1][y1].ReverseAnimation = false;
                            }
                        }
                    }
                }
            }

            // Für das letzte Flussstück
            Scape[x1][y1].ObjectPosOffset.x = static_cast<short>(Bmp[Scape[x1][y1].Object].targetRect.left);
            Scape[x1][y1].ObjectPosOffset.y = static_cast<short>(Bmp[Scape[x1][y1].Object].targetRect.top);
        }
    }
}

void CreateTrees(short Prozent)
{
    Coordinate Pos; // Da steht der Baum
    bool einGrosserBaum = false; // gibt es bereits einen großen Baum

    for (short y = 0; y < MAX_TILESY; y++)//Alle Kacheln durchgehen
        for (short x = 0; x < MAX_TILES_X; x++) {
            if ((Scape[x][y].Object != -1) || ((Scape[x][y].Terrain == 3) && (Scape[x][y].Type == 0))) {
                continue;
            }

            // Wenn schon ein Objekt da ist oder Treibsand ist, dann mit nächsten Teil weitermachen
            if (rand() % (100 / Prozent) != 0) { // Die Wahrscheinlichkeit für einen Baum bestimmen
                continue;
            }

            while (true) {
                Pos.x = rand() % TILE_SIZE_X;
                Pos.y = rand() % TILE_SIZE_Y;
                Coordinate Erg = Renderer::GetTile(Scape[x][y].xScreen + Pos.x, Scape[x][y].yScreen + Pos.y);

                if ((Erg.x == x) && (Erg.y == y)) {
                    break;
                }
            }

            if ((Scape[x][y].Terrain == 2) && (Scape[x][y].Type == 0)) { // Bei Strand nur Palmen nehmen
                Scape[x][y].Object = TREE_2;
            } else {
                short r = rand() % 5; // random speicherung
                Scape[x][y].Object = TREE_1 + r;

                if ((rand() % 50 == 1) || (!einGrosserBaum)) {
                    Scape[x][y].Object = TREE_BIG;
                    einGrosserBaum = true;
                }
            }

            // Linke obere Ecke speichern
            Scape[x][y].ObjectPosOffset.x = Pos.x - static_cast<short>(Bmp[Scape[x][y].Object].Width) / 2;
            Scape[x][y].ObjectPosOffset.y = Pos.y - static_cast<short>(Bmp[Scape[x][y].Object].Height);

            // Startphase
            if (Scape[x][y].Object == BUSH) {
                Scape[x][y].AnimationPhase = static_cast<float>(Bmp[Scape[x][y].Object].AnimationPhaseCount) - 1;
            } else
                Scape[x][y].AnimationPhase = static_cast<float>(Bmp[Scape[x][y].Object].AnimationPhaseCount -
                                             rand() % (Bmp[Scape[x][y].Object].AnimationPhaseCount) - 1);
        }
}

void CreatePirateWreck()
{
    short x = 0;
    short y = 0;

    short Richtung = rand() % 3;

    switch (Richtung) {
    case 0:
        x = MAX_TILES_X / 2;

        for (short i = 0; i < MAX_TILESY; i++) {
            if (Scape[x][i].Terrain != 1) {
                y = i - 1;
                break;
            }
        }

        break;

    case 1:
        y = MAX_TILESY / 2;

        for (short i = MAX_TILES_X - 1; i >= 0; i--) {
            if (Scape[i][y].Terrain != 1) {
                x = i + 1;
                break;
            }
        }

        break;

    case 2:
        x = MAX_TILES_X / 2;

        for (short i = MAX_TILESY - 1; i >= 0; i--) {
            if (Scape[x][i].Terrain != 1) {
                y = i + 1;
                break;
            }
        }

        break;
    }

    Scape[x][y].Object = WRECK_2;
    Scape[x][y].ObjectPosOffset.x = static_cast<short>(Bmp[WRECK_2].targetRect.left);
    Scape[x][y].ObjectPosOffset.y = static_cast<short>(Bmp[WRECK_2].targetRect.top);
}

void Treasure()
{
    while (true) {
        // Diese Kachel wird angeschaut
        short x = rand() % (MAX_TILES_X - 1);
        short y = rand() % (MAX_TILESY - 1);

        // nur auf flachen Kacheln ohne Objekt
        if ((Scape[x][y].Object == -1) && (Scape[x][y].Type == 0) && (Scape[x][y].Terrain != 3)) {
            if (SchatzPos.x == -1) {
                SchatzPos.x = x;
                SchatzPos.y = y;
            }

//            lpDDSScape->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr);
//            lpDDSSchatzkarte->Lock(nullptr, &ddsd2, DDLOCK_WAIT, nullptr);

            for (short i = 0; i < TREASUREMAP_WIDTH; i++)
                for (short j = 0; j < TREASUREMAP_HEIGHT; j++) {
                    Renderer::GetPixel(static_cast<short>(i + Scape[SchatzPos.x][SchatzPos.y].xScreen - TREASUREMAP_WIDTH / 2 + TILE_SIZE_X / 2),
                                       static_cast<short>(j + Scape[SchatzPos.x][SchatzPos.y].yScreen - TREASUREMAP_HEIGHT / 2 + 30), lpDDSScape);
                    Renderer::PutPixel(i, j,
                                       (rgbStruct.r * 30 + rgbStruct.g * 59 + rgbStruct.b * 11) / 100,
                                       (rgbStruct.r * 30 + rgbStruct.g * 59 + rgbStruct.b * 11) / 100,
                                       (rgbStruct.r * 30 + rgbStruct.g * 59 + rgbStruct.b * 11) / 100 * 3 / 4, lpDDSSchatzkarte);
                }

//            lpDDSScape->Unlock(nullptr);
//            lpDDSSchatzkarte->Unlock(nullptr);

            rcRectsrc = Bmp[CROSS].sourceRect;
            rcRectdes.left = TREASUREMAP_WIDTH / 2 - Bmp[CROSS].Width / 2;
            rcRectdes.right = rcRectdes.left + Bmp[CROSS].Width;
            rcRectdes.top = TREASUREMAP_HEIGHT / 2 - Bmp[CROSS].Height / 2;
            rcRectdes.bottom = rcRectdes.top + Bmp[CROSS].Height;
            Renderer::Blitten(Bmp[CROSS].Surface, lpDDSSchatzkarte, true);

//            lpDDSSchatzkarte->Lock(nullptr, &ddsd2, DDLOCK_WAIT, nullptr);

            // Weichzeichnen
            for (short i = 0; i < TREASUREMAP_WIDTH; i++)
                for (short j = 0; j < TREASUREMAP_HEIGHT; j++) {
                    if ((i > 0) && (i < TREASUREMAP_WIDTH - 1) && (j > 0) && (j < TREASUREMAP_HEIGHT - 1)) {
                        Renderer::GetPixel(i - 1, j, lpDDSSchatzkarte);
                        RGBSTRUCT rgbleft = rgbStruct;
                        Renderer::GetPixel(i, j - 1, lpDDSSchatzkarte);
                        RGBSTRUCT rgbtop = rgbStruct;
                        Renderer::GetPixel(i + 1, j, lpDDSSchatzkarte);
                        RGBSTRUCT rgbright = rgbStruct;
                        Renderer::GetPixel(i, j + 1, lpDDSSchatzkarte);
                        RGBSTRUCT rgbbottom = rgbStruct;
                        Renderer::GetPixel(i, j, lpDDSSchatzkarte);
                        Renderer::PutPixel(i, j,
                                               (rgbleft.r + rgbtop.r + rgbright.r + rgbbottom.r + rgbStruct.r) / 5,
                                               (rgbleft.g + rgbtop.g + rgbright.g + rgbbottom.g + rgbStruct.g) / 5,
                                               (rgbleft.b + rgbtop.b + rgbright.b + rgbbottom.b + rgbStruct.b) / 5, lpDDSSchatzkarte);
                    }
                }

//            lpDDSSchatzkarte->Unlock(nullptr);
            break;
        }
    }
}

void CheckBenutze(short Objekt)
{
    if (((Objekt == RAW_STONE) && (TwoClicks == RAW_TREE_BRANCH)) || ((Objekt == RAW_TREE_BRANCH) && (TwoClicks == RAW_STONE))) {
        if (Guy.Inventory[RAW_AXE] < 1) {
            Guy.Inventory[RAW_STONE]--;
            Guy.Inventory[RAW_TREE_BRANCH]--;
            Guy.Inventory[RAW_AXE] = 1;
            Bmp[BUTTON_CHOP].AnimationPhase = 0;
            Bmp[BUTTON_BOAT].AnimationPhase = 0;
            Bmp[BUTTON_PIPE].AnimationPhase = 0;
            PapierText = Renderer::DrawText(BAUEAXT, TXTPAPIER, 1);
            PlaySound(Sound::INVENTION, 100);
        } else if (Guy.Inventory[RAW_HOE] < 1) {
            Guy.Inventory[RAW_STONE]--;
            Guy.Inventory[RAW_TREE_BRANCH]--;
            Guy.Inventory[RAW_HOE] = 1;
            Bmp[BUTTON_FARM].AnimationPhase = 0;
            PapierText = Renderer::DrawText(BAUEEGGE, TXTPAPIER, 1);
            PlaySound(Sound::INVENTION, 100);
        } else {
            PapierText = Renderer::DrawText(STEINPLUSASTNICHTS, TXTPAPIER, 1);
        }
    } else if (((Objekt == RAW_LIANA) && (TwoClicks == RAW_TREE_BRANCH)) || ((Objekt == RAW_TREE_BRANCH) && (TwoClicks == RAW_LIANA))) {
        if (Guy.Inventory[RAW_FISHING_POLE] < 1) {
            Guy.Inventory[RAW_LIANA]--;
            Guy.Inventory[RAW_TREE_BRANCH]--;
            Guy.Inventory[RAW_FISHING_POLE] = 1;
            Bmp[BUTTON_FISH].AnimationPhase = 0;
            PapierText = Renderer::DrawText(BAUEANGEL, TXTPAPIER, 1);
            PlaySound(Sound::INVENTION, 100);
        } else {
            PapierText = Renderer::DrawText(ASTPLUSLIANENICHTS, TXTPAPIER, 1);
        }
    } else if (((Objekt == RAW_LIANA) && (TwoClicks == RAW_STONE)) || ((Objekt == RAW_STONE) && (TwoClicks == RAW_LIANA))) {
        if (Guy.Inventory[RAW_SLINGSHOT] < 1) {
            Guy.Inventory[RAW_LIANA]--;
            Guy.Inventory[RAW_STONE]--;
            Guy.Inventory[RAW_SLINGSHOT] = 1;
            Bmp[BUTTON_SLINGSHOT].AnimationPhase = 0;
            PapierText = Renderer::DrawText(BAUESCHLEUDER, TXTPAPIER, 1);
            PlaySound(Sound::INVENTION, 100);
        } else {
            PapierText = Renderer::DrawText(STEINPLUSLIANENICHTS, TXTPAPIER, 1);
        }
    } else {
        PapierText = Renderer::DrawText(NICHTBASTELN, TXTPAPIER, 1);
    }

    TwoClicks = -1;
}

void Entdecken()
{
    bool Aenderung = false;

    for (short i = -1; i <= 1; i++)
        for (short j = -1; j <= 1; j++) {
            if (!Scape[Guy.Pos.x + i][Guy.Pos.y + j].Discovered) {
                Scape[Guy.Pos.x + i][Guy.Pos.y + j].Discovered = true;
                Aenderung = true;
            }
        }

    if (Aenderung) {
        Generate();
    }
}
} // namespace World
