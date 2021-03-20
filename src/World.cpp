#include "World.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Renderer.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "Application.hpp"

#include <cstdio>
#include <cstring>

namespace World {
RiverRun Rivers[NUMBER_OF_RIVERS][MAX_RIVER_LENGTH];

void RawMaterialsDescriptionString(short x, short y, short Objekt)
{
    char TmpString[1024];

    RohString[0] = char(0);
    bool keinRohstoff = true;

    if (Objekt == -1) {
        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            if (Landscape[x][y].RequiredRawMaterials[i] != 0) {
                keinRohstoff = false;
            }
        }
    } else {
        for (short i = 0; i < Tiles::SPRITE_COUNT; i++) {
            if (Bmp[Objekt].RequiredRawMaterials[i] != 0) {
                keinRohstoff = false;
            }
        }
    }

    if (keinRohstoff) {
        return;
    }

    strcat(RohString, " ->");

    for (short tile = 0; tile < SPRITE_COUNT; tile++) {
        if (Objekt == -1) {
            if (Landscape[x][y].RequiredRawMaterials[tile] == 0) {
                continue;
            }
        } else {
            if (Bmp[Objekt].RequiredRawMaterials[tile] == 0) {
                continue;
            }
        }

        strcat(RohString, " ");

        const char *name = "";
        switch (tile) {
        case Tiles::RAW_TREE_BRANCH:
            name = GetLanguageString(AST);
            break;

        case Tiles::RAW_STONE:
            name = GetLanguageString(STEIN);
            break;

        case Tiles::RAW_LEAF:
            name = GetLanguageString(STRING_LEAF);
            break;

        case Tiles::RAW_LIANA:
            name = GetLanguageString(STRING_LIANA);
            break;

        case Tiles::RAW_TREE_TRUNK:
            name = GetLanguageString(STAMM);
            break;
        }

        strcat(RohString, name);
        strcat(RohString, "=");

        if (Objekt == -1) {
            std::sprintf(TmpString, "%d", Landscape[x][y].RequiredRawMaterials[tile]);
        } else {
            std::sprintf(TmpString, "%d", Bmp[Objekt].RequiredRawMaterials[tile]);
        }

        strcat(RohString, TmpString);
    }
}

void AddTime(short h, short m)
{
    Hours += h;
    Minutes += m;

    if (Minutes >= 60) {
        Minutes -= 60;
        Hours++;
    }

    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            // Feuer nach einer bestimmten Zeit ausgehen lassen
            if (Landscape[x][y].Object == Tiles::FIRE) {
                Landscape[x][y].FireTimer += float((60 * h + m) * 0.0005);

                if (Landscape[x][y].FireTimer >= 1) {
                    Landscape[x][y].Object = Tiles::INVALID;
                    Landscape[x][y].FireTimer = 0;
                    Landscape[x][y].ObjectPosOffset.x = 0;
                    Landscape[x][y].ObjectPosOffset.y = 0;
                    Landscape[x][y].AnimationPhase = -1;
                    Chance -= 2 + 2 * Landscape[x][y].Height;
                }
            }

            if ((Landscape[x][y].AnimationPhase == -1) ||
                    ((Landscape[x][y].Object != Tiles::FIELD) &&
                     (Landscape[x][y].Object != Tiles::BUSH))) {
                continue;    // Wenn kein Fruchtobjekt weiter
            }

            if (Landscape[x][y].AnimationPhase >= Bmp[Landscape[x][y].Object].AnimationPhaseCount) {
                continue;
            }

            if (Landscape[x][y].Object == Tiles::FIELD) {
                Landscape[x][y].AnimationPhase += float((60 * h + m) * 0.005);
            } else if (Landscape[x][y].Object == Tiles::BUSH) {
                Landscape[x][y].AnimationPhase += float((60 * h + m) * 0.0005);    // pro Minute Reifungsprozess fortführen
            }

            if (Landscape[x][y].AnimationPhase > Bmp[Landscape[x][y].Object].AnimationPhaseCount - 1) {
                Landscape[x][y].AnimationPhase = static_cast<float>(Bmp[Landscape[x][y].Object].AnimationPhaseCount) - 1;
            }
        }

    AddResource(Resources::Health, (60 * h + m) * (Guy.ResourceAmount[Resources::Water] - 50 + Guy.ResourceAmount[Resources::Food] - 50) / 1000);

    if ((s_GameState == State::GAME) && (!IsInBoat)) {
        for (short i = 0; i <= (60 * h + m); i++) {
            if (Chance == 0) {
                break;
            }

            if (rand() % static_cast<int>(1 / (Chance / 72000)) == 1) {
                Guy.IsActive = false;
                Guy.ActionStep = 0;
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
    if ((Guy.ResourceAmount[Resources::Health] <= 0) && (Guy.CurrentAction != Action::DEATH) &&
            (Guy.CurrentAction != Action::DAY_END) && (s_GameState == State::GAME)) {
        Guy.IsActive = false;
        Guy.ActionStep = 0;
        Guy.CurrentAction = Action::DEATH;
    }
}

void Generate()
{
    // The map background color
    rcRectdes.left = 0;
    rcRectdes.top = 0;
    rcRectdes.right = 2 * MAX_TILES_X;
    rcRectdes.bottom = 2 * MAX_TILESY;
    sf::Image newContent;
    newContent.create(MAX_TILES_X * 2, MAX_TILESY * 2, sf::Color(247, 222, 191));
//    ddbltfx.dwFillColor = Renderer::RGB2DWORD(247, 222, 191);
//        lpDDSKarte->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

    // Die Landschaftshintergrundfarbe
    rcRectdes.left = 0;
    rcRectdes.top = 0;
    rcRectdes.right = MAX_SURFACE_X;
    rcRectdes.bottom = MAX_SURFACE_Y;

    Application::clearLandscape();
//    lpDDSScape = Renderer::createEmptyTexture(MAX_SURFACE_X, MAX_SURFACE_Y, sf::Color::Black);
//    ddbltfx.dwFillColor = Renderer::RGB2DWORD(0, 0, 0);
//        lpDDSScape->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

    for (short y = 0; y < MAX_TILESY; y++) {
        for (short x = 0; x < MAX_TILES_X; x++) {
            if (!Landscape[x][y].Discovered) {
                continue;    // Nicht entdeckte Felder nicht malen
            }

            rcRectdes.left = Landscape[x][y].xScreen;
            rcRectdes.top = Landscape[x][y].yScreen;
            rcRectdes.right = rcRectdes.left + TILE_SIZE_X;
            rcRectdes.bottom = rcRectdes.top + TILE_SIZE_Y;

            if (Landscape[x][y].Terrain == 4) {
                rcRectsrc.left = TILE_SIZE_X * Landscape[x][y].Type;
                rcRectsrc.right = TILE_SIZE_X * Landscape[x][y].Type + TILE_SIZE_X;
                rcRectsrc.top = 0;
                rcRectsrc.bottom = TILE_SIZE_Y;
            } else if (Landscape[x][y].Terrain == 0) { // trockenes Land
                rcRectsrc.left = TILE_SIZE_X * Landscape[x][y].Type;
                rcRectsrc.right = TILE_SIZE_X * Landscape[x][y].Type + TILE_SIZE_X;
                rcRectsrc.top = 4 * TILE_SIZE_Y;
                rcRectsrc.bottom = 5 * TILE_SIZE_Y;
            } else {
                if ((Landscape[x][y].Type == 0) && (Landscape[x][y].Terrain == 1)) {
                    rcRectsrc.left = 0 * TILE_SIZE_X;
                    rcRectsrc.top = 3 * TILE_SIZE_Y;
                    rcRectsrc.right = 1 * TILE_SIZE_X;
                    rcRectsrc.bottom = 4 * TILE_SIZE_Y;
                }

                if ((Landscape[x][y].Type == 0) && (Landscape[x][y].Terrain == 2)) {
                    rcRectsrc.left = 1 * TILE_SIZE_X;
                    rcRectsrc.top = 3 * TILE_SIZE_Y;
                    rcRectsrc.right = 2 * TILE_SIZE_X;
                    rcRectsrc.bottom = 4 * TILE_SIZE_Y;
                }

                if ((Landscape[x][y].Type == 0) && (Landscape[x][y].Terrain == 3)) {
                    rcRectsrc.left = 2 * TILE_SIZE_X;
                    rcRectsrc.top = 3 * TILE_SIZE_Y;
                    rcRectsrc.right = 3 * TILE_SIZE_X;
                    rcRectsrc.bottom = 4 * TILE_SIZE_Y;
                }
            }

            // Landschaftskacheln zeichnen
            Renderer::BlitToLandscape(lpDDSMisc);

            // Gitter drüberlegen
            if (Gitter) {
                rcRectsrc.left = TILE_SIZE_X * Landscape[x][y].Type;
                rcRectsrc.right = TILE_SIZE_X * Landscape[x][y].Type + TILE_SIZE_X;
                rcRectsrc.top = 1 * TILE_SIZE_Y;
                rcRectsrc.bottom = 1 * TILE_SIZE_Y + TILE_SIZE_Y;
                Renderer::BlitToLandscape(lpDDSMisc);
            }

            // Draw landscape objects (if animations are switched off)
            if ((!LAnimation) && (Landscape[x][y].Object != Tiles::INVALID)) {
                if ((Landscape[x][y].Object >= Tiles::SEA_WAVES) && (Landscape[x][y].Object <= Tiles::FLOODGATE_6)) {
                    rcRectsrc.left = Bmp[Landscape[x][y].Object].sourceRect.left;
                    rcRectsrc.right = Bmp[Landscape[x][y].Object].sourceRect.right;

                    if (Landscape[x][y].Object == Tiles::SEA_WAVES) {
                        short i = rand() % 6;
                        rcRectsrc.top = Bmp[Landscape[x][y].Object].sourceRect.top + i * Bmp[Landscape[x][y].Object].Height;
                        rcRectsrc.bottom = Bmp[Landscape[x][y].Object].sourceRect.bottom + i * Bmp[Landscape[x][y].Object].Height;
                    } else {
                        rcRectsrc.top = Bmp[Landscape[x][y].Object].sourceRect.top;
                        rcRectsrc.bottom = Bmp[Landscape[x][y].Object].sourceRect.bottom;
                    }

                    rcRectdes.left = Landscape[x][y].xScreen + Bmp[Landscape[x][y].Object].targetRect.left;
                    rcRectdes.right = Landscape[x][y].xScreen + Bmp[Landscape[x][y].Object].targetRect.right;
                    rcRectdes.top = Landscape[x][y].yScreen + Bmp[Landscape[x][y].Object].targetRect.top;
                    rcRectdes.bottom = Landscape[x][y].yScreen + Bmp[Landscape[x][y].Object].targetRect.bottom;
                    // Landschaftsobjekt zeichnen
                    Renderer::BlitToLandscape(lpDDSAnimation);
                }
            }

            // MiniMap zeichnen
            rcRectdes.left = 2 * x;
            rcRectdes.top = 2 * y;
            rcRectdes.right = rcRectdes.left + 2;
            rcRectdes.bottom = rcRectdes.top + 2;

            sf::Color c;
            if ((Landscape[x][y].Terrain == 1) && (Landscape[x][y].Type == 0)) { // Meer
                c = sf::Color(228, 207, 182);
            } else {
                if ((Landscape[x][y].Type == 0) &&
                        ((Landscape[x][y].Terrain == 2) ||
                         (Landscape[x][y].Terrain == 3))) { // Strand
                    c = sf::Color(112, 103, 93);
                } else
                    // Land
                {
                    c = sf::Color(139 + Landscape[x][y].Height * 20, 128 + Landscape[x][y].Height * 20, 115 + Landscape[x][y].Height * 20);
                }
            }

//                    printf("minimap %d %d\n", rcRectdes.left, rcRectdes.top);
            // TODO: more efficient
            for (int ix=rcRectdes.left; ix<rcRectdes.right; ix++) {
                for (int iy=rcRectdes.top; iy<rcRectdes.bottom; iy++) {
//                    printf("minimap %d %d\n", ix, iy);
                    newContent.setPixel(ix, iy, c);
                }
            }
//            Renderer::drawRect(rcRectdes.left, rcRectdes.top, rcRectdes.right - rcRectdes.left, rcRectdes.bottom - rcRectdes.top, c);
//            printf("ddskarte %d %d\n", rcRectdes.right - rcRectdes.left, rcRectdes.bottom - rcRectdes.top);
//            lpDDSKarte = Renderer::createEmptyTexture(rcRectdes.right - rcRectdes.left, rcRectdes.bottom - rcRectdes.top, c);

//                lpDDSKarte->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);
        }
    }
    lpDDSKarte->loadFromImage(newContent);
}

void UpdateButtons()
{
    if ((Landscape[Guy.Pos.x][Guy.Pos.y].Object >= Tiles::FIELD) && (Landscape[Guy.Pos.x][Guy.Pos.y].Object <= Tiles::BONFIRE) &&
            (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase >= Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount) &&
            (Bmp[Tiles::BUTTON_STOP].AnimationPhase == -1)) {
        if (Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase == -1) {
            Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase = 0;
        }
    } else {
        Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase = -1;
    }

    if ((Bmp[Tiles::BUTTON_STOP].AnimationPhase == -1) && (((Landscape[Guy.Pos.x][Guy.Pos.y].Object == Tiles::BOAT) &&
            (Landscape[Guy.Pos.x][Guy.Pos.y].AnimationPhase < Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].AnimationPhaseCount)) ||
            ((IsInBoat) &&
             (((Landscape[Guy.Pos.x - 1][Guy.Pos.y].Terrain != 1) && (Landscape[Guy.Pos.x - 1][Guy.Pos.y].Object == Tiles::INVALID)) ||
              ((Landscape[Guy.Pos.x][Guy.Pos.y - 1].Terrain != 1) && (Landscape[Guy.Pos.x][Guy.Pos.y - 1].Object == Tiles::INVALID)) ||
              ((Landscape[Guy.Pos.x + 1][Guy.Pos.y].Terrain != 1) && (Landscape[Guy.Pos.x + 1][Guy.Pos.y].Object == Tiles::INVALID)) ||
              ((Landscape[Guy.Pos.x][Guy.Pos.y + 1].Terrain != 1) && (Landscape[Guy.Pos.x][Guy.Pos.y + 1].Object == Tiles::INVALID)))))) {
        if (Bmp[Tiles::BUTTON_LAY_DOWN].AnimationPhase == -1) {
            Bmp[Tiles::BUTTON_LAY_DOWN].AnimationPhase = 0;
        }
    } else {
        Bmp[Tiles::BUTTON_LAY_DOWN].AnimationPhase = -1;
    }
}

bool CheckRawMaterials()
{
    short Benoetigt = 0; // Anzahl der Gesamtbenötigten Rohstoffe

    for (short i = 0; i < SPRITE_COUNT; i++) {
        Benoetigt += Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].RequiredRawMaterials[i];
    }

    float GebrauchtTmp = Benoetigt / static_cast<float>(Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].RequiredActionCases); // Soviel Rohstoffe werden für diesen Schritt benötigt
    short Gebraucht = static_cast<short>(GebrauchtTmp * Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep -
                                         static_cast<short>(GebrauchtTmp * (Landscape[Guy.Pos.x][Guy.Pos.y].ConstructionActionStep - 1))); // Soviel Rohstoffe werden für diesen Schritt benötigt


    while (true) {
        bool Check = false; // Wenn kein Rohstoff mehr vorhanden nur noch einmal die While-Schleife

        for (short i = 0; i < SPRITE_COUNT; i++) {
            if (Gebraucht == 0) {
                return true;
            }

            if ((Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i] > 0) &&
                    (Guy.Inventory[i] > 0)) {
                Guy.Inventory[i]--;
                Landscape[Guy.Pos.x][Guy.Pos.y].RequiredRawMaterials[i]--;
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
    Guy.ActionStep = 0;
    Guy.CurrentAction = Action::CANCEL;
    Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;
    return false;
}

void Compute(short MinimumSize, short maximumSize)// Size of the island in number of land tiles
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
                Landscape[x][y].Type = 0;
                Landscape[x][y].Terrain = 0;
                Landscape[x][y].Height = 0;
                Landscape[x][y].Marked = false;
                Landscape[x][y].Walkable = true;
                Landscape[x][y].Discovered = false;
                Landscape[x][y].RunningTime = 1;
                Landscape[x][y].Object = Tiles::INVALID;
                Landscape[x][y].ReverseAnimation = false;
                Landscape[x][y].ObjectPosOffset.x = 0;
                Landscape[x][y].ObjectPosOffset.y = 0;
                Landscape[x][y].AnimationPhase = -1;
                Landscape[x][y].ConstructionActionStep = 0;
                Landscape[x][y].GPosAlt.x = 0;
                Landscape[x][y].GPosAlt.y = 0;

                for (i = 0; i < SPRITE_COUNT; i++) {
                    Landscape[x][y].RequiredRawMaterials[i] = 0;
                }

                Landscape[x][y].FireTimer = 0;
            }

        int x; // Startposition der Berechnung
        int y;
        Landscape[MidX][MidY].Type = 0; // Gipfel festlegen (Flach)
        Landscape[MidX][MidY].Height = MOUNTAIN_HEIGHT; // und mit der Hoehe

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

                    Landscape[x][y].Type = rand() % 13;

                    for (j = 0; j < 10; j++) {
                        if (!((Landscape[x][y].Type == 0) || (Landscape[x][y].Type == 1))) {
                            Landscape[x][y].Type = rand() % 13;
                        }
                    }

                    if ((x == MidX - l) || ((x != MidX - l)
                                            && ((Vierecke[Landscape[x][y + 1].Type][1][Landscape[x][y].Type] != 0)
                                                && (Vierecke[Landscape[x - 1][y].Type][2][Landscape[x][y].Type] != 0)))) {
                        if (Vierecke[Landscape[x][y + 1].Type][1][Landscape[x][y].Type] == 1) {
                            Landscape[x][y].Height = Landscape[x][y + 1].Height - 1;

                            if (Landscape[x][y].Height < 0) {
                                Landscape[x][y].Type = 0;
                                Landscape[x][y].Height = 0;
                            }

                            hasFound = true;
                        }

                        if (Vierecke[Landscape[x][y + 1].Type][1][Landscape[x][y].Type] == 2) {
                            Landscape[x][y].Height = Landscape[x][y + 1].Height;
                            hasFound = true;
                        }

                        if (Vierecke[Landscape[x][y + 1].Type][1][Landscape[x][y].Type] == 3) {
                            Landscape[x][y].Height = Landscape[x][y + 1].Height + 1;
                            hasFound = true;
                        }
                    }

                    // Verzwickte Fälle ausfiltern
                    if (((Vierecke[Landscape[x][y].Type][2][3] == 2) && (Vierecke[Landscape[x + 1][y + 1].Type][1][4] == 2)) ||
                            ((Vierecke[Landscape[x][y].Type][2][1] == 2) && (Vierecke[Landscape[x + 1][y + 1].Type][1][2] == 2))) {
                        hasFound = false;
                    }

                    // Nebeninseln vermeiden
                    if (((Landscape[x - 1][y].Type == 0) && (Landscape[x - 1][y].Height == 0)) &&
                            ((Landscape[x][y + 1].Type == 0) && (Landscape[x][y + 1].Height == 0))) {
                        Landscape[x][y].Type = 0;
                        Landscape[x][y].Height = 0;
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

                Landscape[x][y].Type = rand() % 13;

                for (j = 0; j < 10; j++) {
                    if (!((Landscape[x][y].Type == 0) || (Landscape[x][y].Type == 5))) {
                        Landscape[x][y].Type = rand() % 13;
                    }
                }

                if (Vierecke[Landscape[x - 1][y].Type][2][Landscape[x][y].Type] == 1) {
                    Landscape[x][y].Height = Landscape[x - 1][y].Height - 1;

                    if (Landscape[x][y].Height < 0) {
                        Landscape[x][y].Type = 0;
                        Landscape[x][y].Height = 0;
                    }

                    hasFound = true;
                }

                if (Vierecke[Landscape[x - 1][y].Type][2][Landscape[x][y].Type] == 2) {
                    Landscape[x][y].Height = Landscape[x - 1][y].Height;
                    hasFound = true;
                }

                if (Vierecke[Landscape[x - 1][y].Type][2][Landscape[x][y].Type] == 3) {
                    Landscape[x][y].Height = Landscape[x - 1][y].Height + 1;
                    hasFound = true;
                }

                // Verzwickte Fälle ausfiltern
                if (((Vierecke[Landscape[x][y].Type][3][2] == 2) && (Vierecke[Landscape[x - 1][y + 1].Type][2][3] == 2)) ||
                        ((Vierecke[Landscape[x][y].Type][3][4] == 2) && (Vierecke[Landscape[x - 1][y + 1].Type][2][1] == 2))) {
                    hasFound = false;
                }

                // Nebeninseln vermeiden
                if ((Landscape[x - 1][y].Type == 0) && (Landscape[x - 1][y].Height == 0)) {
                    Landscape[x][y].Type = 0;
                    Landscape[x][y].Height = 0;
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

                    Landscape[x][y].Type = rand() % 13;

                    for (j = 0; j < 10; j++) {
                        if (!((Landscape[x][y].Type == 0) || (Landscape[x][y].Type == 4))) {
                            Landscape[x][y].Type = rand() % 13;
                        }
                    }

                    if ((Vierecke[Landscape[x - 1][y].Type][2][Landscape[x][y].Type] != 0)
                            && (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] != 0)) {
                        if (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] == 1) {
                            Landscape[x][y].Height = Landscape[x][y - 1].Height - 1;

                            if (Landscape[x][y].Height < 0) {
                                Landscape[x][y].Type = 0;
                                Landscape[x][y].Height = 0;
                            }

                            hasFound = true;
                        }

                        if (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] == 2) {
                            Landscape[x][y].Height = Landscape[x][y - 1].Height;
                            hasFound = true;
                        }

                        if (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] == 3) {
                            Landscape[x][y].Height = Landscape[x][y - 1].Height + 1;
                            hasFound = true;
                        }
                    }

                    // Verzwickte Fälle ausfiltern
                    if (((Vierecke[Landscape[x][y].Type][3][2] == 2) && (Vierecke[Landscape[x - 1][y + 1].Type][2][3] == 2)) ||
                            ((Vierecke[Landscape[x][y].Type][3][4] == 2) && (Vierecke[Landscape[x - 1][y + 1].Type][2][1] == 2))) {
                        hasFound = false;
                    }

                    // Nebeninseln vermeiden
                    if (((Landscape[x - 1][y].Type == 0) && (Landscape[x - 1][y].Height == 0)) &&
                            ((Landscape[x][y - 1].Type == 0) && (Landscape[x][y - 1].Height == 0))) {
                        Landscape[x][y].Type = 0;
                        Landscape[x][y].Height = 0;
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

                Landscape[x][y].Type = rand() % 13;

                for (j = 0; j < 10; j++) {
                    if (!((Landscape[x][y].Type == 0) || (Landscape[x][y].Type == 8))) {
                        Landscape[x][y].Type = rand() % 13;
                    }
                }

                if (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] == 1) {
                    Landscape[x][y].Height = Landscape[x][y - 1].Height - 1;

                    if (Landscape[x][y].Height < 0) {
                        Landscape[x][y].Type = 0;
                        Landscape[x][y].Height = 0;
                    }

                    hasFound = true;
                }

                if (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] == 2) {
                    Landscape[x][y].Height = Landscape[x][y - 1].Height;
                    hasFound = true;
                }

                if (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] == 3) {
                    Landscape[x][y].Height = Landscape[x][y - 1].Height + 1;
                    hasFound = true;
                }

                // Verzwickte Fälle ausfiltern
                if (((Vierecke[Landscape[x][y].Type][0][1] == 2) && (Vierecke[Landscape[x - 1][y - 1].Type][3][2] == 2)) ||
                        ((Vierecke[Landscape[x][y].Type][0][3] == 2) && (Vierecke[Landscape[x - 1][y - 1].Type][3][4] == 2))) {
                    hasFound = false;
                }

                // Nebeninsel vermeiden
                if ((Landscape[x][y - 1].Type == 0) && (Landscape[x][y - 1].Height == 0)) {
                    Landscape[x][y].Type = 0;
                    Landscape[x][y].Height = 0;
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

                    Landscape[x][y].Type = rand() % 13;

                    for (j = 0; j < 10; j++) {
                        if (!((Landscape[x][y].Type == 0) || (Landscape[x][y].Type == 3))) {
                            Landscape[x][y].Type = rand() % 13;
                        }
                    }

                    if ((Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] != 0)
                            && (Vierecke[Landscape[x + 1][y].Type][0][Landscape[x][y].Type] != 0)) {
                        if (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] == 1) {
                            Landscape[x][y].Height = Landscape[x][y - 1].Height - 1;

                            if (Landscape[x][y].Height < 0) {
                                Landscape[x][y].Type = 0;
                                Landscape[x][y].Height = 0;
                            }

                            hasFound = true;
                        }

                        if (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] == 2) {
                            Landscape[x][y].Height = Landscape[x][y - 1].Height;
                            hasFound = true;
                        }

                        if (Vierecke[Landscape[x][y - 1].Type][3][Landscape[x][y].Type] == 3) {
                            Landscape[x][y].Height = Landscape[x][y - 1].Height + 1;
                            hasFound = true;
                        }
                    }

                    // Verzwickte Fälle ausfiltern
                    if (((Vierecke[Landscape[x][y].Type][0][1] == 2) && (Vierecke[Landscape[x - 1][y - 1].Type][3][2] == 2)) ||
                            ((Vierecke[Landscape[x][y].Type][0][3] == 2) && (Vierecke[Landscape[x - 1][y - 1].Type][3][4] == 2))) {
                        hasFound = false;
                    }

                    // Nebeninseln vermeiden
                    if (((Landscape[x + 1][y].Type == 0) && (Landscape[x + 1][y].Height == 0)) &&
                            ((Landscape[x][y - 1].Type == 0) && (Landscape[x][y - 1].Height == 0))) {
                        Landscape[x][y].Type = 0;
                        Landscape[x][y].Height = 0;
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

                Landscape[x][y].Type = rand() % 13;

                for (j = 0; j < 10; j++) {
                    if (!((Landscape[x][y].Type == 0) || (Landscape[x][y].Type == 7))) {
                        Landscape[x][y].Type = rand() % 13;
                    }
                }

                if (Vierecke[Landscape[x + 1][y].Type][0][Landscape[x][y].Type] == 1) {
                    Landscape[x][y].Height = Landscape[x + 1][y].Height - 1;

                    if (Landscape[x][y].Height < 0) {
                        Landscape[x][y].Type = 0;
                        Landscape[x][y].Height = 0;
                    }

                    hasFound = true;
                }

                if (Vierecke[Landscape[x + 1][y].Type][0][Landscape[x][y].Type] == 2) {
                    Landscape[x][y].Height = Landscape[x + 1][y].Height;
                    hasFound = true;
                }

                if (Vierecke[Landscape[x + 1][y].Type][0][Landscape[x][y].Type] == 3) {
                    Landscape[x][y].Height = Landscape[x + 1][y].Height + 1;
                    hasFound = true;
                }

                // Verzwickte Fälle ausfiltern
                if (((Vierecke[Landscape[x][y].Type][1][2] == 2) && (Vierecke[Landscape[x + 1][y - 1].Type][0][3] == 2)) ||
                        ((Vierecke[Landscape[x][y].Type][1][4] == 2) && (Vierecke[Landscape[x + 1][y - 1].Type][0][1] == 2))) {
                    hasFound = false;
                }

                // Nebeninsel vermeiden
                if ((Landscape[x + 1][y].Type == 0) && (Landscape[x + 1][y].Height == 0)) {
                    Landscape[x][y].Type = 0;
                    Landscape[x][y].Height = 0;
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

                    Landscape[x][y].Type = rand() % 13;

                    for (j = 0; j < 10; j++) {
                        if (!((Landscape[x][y].Type == 0) || (Landscape[x][y].Type == 2))) {
                            Landscape[x][y].Type = rand() % 13;
                        }
                    }

                    if ((Vierecke[Landscape[x + 1][y].Type][0][Landscape[x][y].Type] != 0)
                            && (Vierecke[Landscape[x][y + 1].Type][1][Landscape[x][y].Type] != 0)) {
                        if (Vierecke[Landscape[x][y + 1].Type][1][Landscape[x][y].Type] == 1) {
                            Landscape[x][y].Height = Landscape[x][y + 1].Height - 1;

                            if (Landscape[x][y].Height < 0) {
                                Landscape[x][y].Type = 0;
                                Landscape[x][y].Height = 0;
                            }

                            hasFound = true;
                        }

                        if (Vierecke[Landscape[x][y + 1].Type][1][Landscape[x][y].Type] == 2) {
                            Landscape[x][y].Height = Landscape[x][y + 1].Height;
                            hasFound = true;
                        }

                        if (Vierecke[Landscape[x][y + 1].Type][1][Landscape[x][y].Type] == 3) {
                            Landscape[x][y].Height = Landscape[x][y + 1].Height + 1;
                            hasFound = true;
                        }
                    }

                    // Filter out tricky cases
                    if (((Vierecke[Landscape[x][y].Type][1][2] == 2) && (y > 0 && Vierecke[Landscape[x + 1][y - 1].Type][0][3] == 2)) ||
                            ((Vierecke[Landscape[x][y].Type][1][4] == 2) && (y > 0 && Vierecke[Landscape[x + 1][y - 1].Type][0][1] == 2))) {
                        hasFound = false;
                    }

                    // Avoid secondary islands
                    if (((Landscape[x + 1][y].Type == 0) && (Landscape[x + 1][y].Height == 0)) &&
                            ((Landscape[x][y + 1].Type == 0) && (Landscape[x][y + 1].Height == 0))) {
                        Landscape[x][y].Type = 0;
                        Landscape[x][y].Height = 0;
                    }
                }
            }
        }

        short Anzahl = 0; // Anzahl der Landstücke
        bool CheckRand = true; // Reicht die Insel bis zum Rand?

        for (y = 0; y < MAX_TILESY; y++) // Landfläche zählen
            for (x = 0; x < MAX_TILES_X; x++) {
                if (Landscape[x][y].Height > 0) {
                    Anzahl++;
                }

                if (Landscape[x][y].Type == 0) {
                    Landscape[x][y].RunningTime = 1;
                } else {
                    Landscape[x][y].RunningTime = 2;
                }

                if ((Landscape[x][y].Type != 0) &&
                        ((x <= 2) || (x >= MAX_TILES_X - 2) || (y <= 2) || (y >= MAX_TILESY - 2))) {
                    CheckRand = false;
                }
            }

        if ((Anzahl > MinimumSize) && (Anzahl < maximumSize) && (CheckRand)) {
            break;
        }
    }
}

void CreateSea() // Das Meer und den Strand bestimmen
{
    short x, y;

    for (y = 0; y < MAX_TILESY; y++) // Meer rausfinden
        for (x = 0; x < MAX_TILES_X; x++) {
            if ((Landscape[x][y].Height < 0) ||
                    ((Landscape[x][y].Height == 0) && (Landscape[x][y].Type == 0))) {
                Landscape[x][y].Type = 0;
                Landscape[x][y].Height = 0;
                Landscape[x][y].Terrain = 1;
                Landscape[x][y].Object = Tiles::SEA_WAVES;
                Landscape[x][y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::SEA_WAVES].targetRect.left);
                Landscape[x][y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::SEA_WAVES].targetRect.top);

                if (rand() % 2 == 0) {
                    Landscape[x][y].ReverseAnimation = true;
                }

                Landscape[x][y].Walkable = false;
                Landscape[x][y].AnimationPhase = static_cast<float>(Bmp[Landscape[x][y].Object].AnimationPhaseCount -
                                             rand() % (Bmp[Landscape[x][y].Object].AnimationPhaseCount) - 1);
            }
        }

    for (y = 1; y < MAX_TILESY - 1; y++) // Strand rausfinden
        for (x = 1; x < MAX_TILES_X - 1; x++) { // Alle Möglichkeiten durchgehen
            if ((Landscape[x][y].Type == 0) && (Landscape[x][y].Height == 0)) {
                short Anzahl = 0; // Anzahl von angrenzenden Landstücken rausfinden

                if (Landscape[x - 1][y].Type != 0) {
                    Anzahl++;
                }

                if (Landscape[x - 1][y - 1].Type != 0) {
                    Anzahl++;
                }

                if (Landscape[x][y - 1].Type != 0) {
                    Anzahl++;
                }

                if (Landscape[x + 1][y - 1].Type != 0) {
                    Anzahl++;
                }

                if (Landscape[x + 1][y].Type != 0) {
                    Anzahl++;
                }

                if (Landscape[x + 1][y + 1].Type != 0) {
                    Anzahl++;
                }

                if (Landscape[x][y + 1].Type != 0) {
                    Anzahl++;
                }

                if (Landscape[x - 1][y + 1].Type != 0) {
                    Anzahl++;
                }

                if ((Anzahl >= 1) && (Anzahl < 6)) {
                    Landscape[x][y].Terrain = 2;
                    Landscape[x][y].Object = Tiles::INVALID;
                    Landscape[x][y].ReverseAnimation = false;
                    Landscape[x][y].Walkable = true;
                    Landscape[x][y].AnimationPhase = -1;
                    continue;
                }

                if (Anzahl >= 6) {
                    Landscape[x][y].Terrain = 3;
                    Landscape[x][y].Object = Tiles::INVALID;
                    Landscape[x][y].ReverseAnimation = false;
                    Landscape[x][y].Walkable = false;
                    Landscape[x][y].AnimationPhase = -1;
                    continue;
                }

                Landscape[x][y].Terrain = 1; // sonst Meer
                Landscape[x][y].Object = Tiles::SEA_WAVES;
                Landscape[x][y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::SEA_WAVES].targetRect.left);
                Landscape[x][y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::SEA_WAVES].targetRect.top);
                Landscape[x][y].AnimationPhase = static_cast<float>(Bmp[Landscape[x][y].Object].AnimationPhaseCount -
                                             rand() % (Bmp[Landscape[x][y].Object].AnimationPhaseCount) - 1);

                if (rand() % 2 == 0) {
                    Landscape[x][y].ReverseAnimation = true;
                }

                Landscape[x][y].Walkable = false;
            }
        }
}

void ToggleIsInBoat()
{
    IsInBoat = !IsInBoat;

    // Begehbarkeit umdrehen
    for (short y = 0; y < MAX_TILESY; y++) {
        for (short x = 0; x < MAX_TILES_X; x++) {
            Landscape[x][y].Walkable = !Landscape[x][y].Walkable;
        }
    }
}

void CheckPipe(short x, short y)
{
    Landscape[x][y].AnimationPhase = 1;

    if (Landscape[x][y].Terrain == 0) {
        Landscape[x][y].Terrain = 4;
    }

    if (Landscape[x - 1][y].Terrain == 0) {
        Landscape[x - 1][y].Terrain = 4;
    }

    if (Landscape[x - 1][y - 1].Terrain == 0) {
        Landscape[x - 1][y - 1].Terrain = 4;
    }

    if (Landscape[x][y - 1].Terrain == 0) {
        Landscape[x][y - 1].Terrain = 4;
    }

    if (Landscape[x + 1][y - 1].Terrain == 0) {
        Landscape[x + 1][y - 1].Terrain = 4;
    }

    if (Landscape[x + 1][y].Terrain == 0) {
        Landscape[x + 1][y].Terrain = 4;
    }

    if (Landscape[x + 1][y + 1].Terrain == 0) {
        Landscape[x + 1][y + 1].Terrain = 4;
    }

    if (Landscape[x][y + 1].Terrain == 0) {
        Landscape[x][y + 1].Terrain = 4;
    }

    if (Landscape[x - 1][y + 1].Terrain == 0) {
        Landscape[x - 1][y + 1].Terrain = 4;
    }

    if ((Landscape[x - 1][y].Object == Tiles::PIPE) && (Landscape[x - 1][y].AnimationPhase == 0)) {
        CheckPipe(x - 1, y);
    }

    if ((Landscape[x][y - 1].Object == Tiles::PIPE) && (Landscape[x][y - 1].AnimationPhase == 0)) {
        CheckPipe(x, y - 1);
    }

    if ((Landscape[x + 1][y].Object == Tiles::PIPE) && (Landscape[x + 1][y].AnimationPhase == 0)) {
        CheckPipe(x + 1, y);
    }

    if ((Landscape[x][y + 1].Object == Tiles::PIPE) && (Landscape[x][y + 1].AnimationPhase == 0)) {
        CheckPipe(x, y + 1);
    }
}

void FillPipe()
{
    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            if ((Landscape[x][y].Object == Tiles::PIPE) && (Landscape[x][y].AnimationPhase < Bmp[Tiles::PIPE].AnimationPhaseCount)) {
                Landscape[x][y].AnimationPhase = 0;
            }

            if (Landscape[x][y].Terrain == 4) {
                Landscape[x][y].Terrain = 0;
            }

            if ((Landscape[x][y].Object >= Tiles::FLOODGATE_1) && (Landscape[x][y].Object <= Tiles::FLOODGATE_6)) {
                Landscape[x][y].Object -= 14;
                Landscape[x][y].ObjectPosOffset.x = static_cast<short>(Bmp[Landscape[x][y].Object].targetRect.left);
                Landscape[x][y].ObjectPosOffset.y = static_cast<short>(Bmp[Landscape[x][y].Object].targetRect.top);
            }
        }

    // StartRohr finden
    for (short y = 0; y < MAX_TILESY; y++)
        for (short x = 0; x < MAX_TILES_X; x++) {
            if ((Landscape[x][y].Object >= Tiles::RIVER_1) && (Landscape[x][y].Object <= Tiles::FLOODGATE_6)) {
                if (Landscape[x][y].Terrain == 0) {
                    Landscape[x][y].Terrain = 4;
                }

                if (Landscape[x - 1][y].Terrain == 0) {
                    Landscape[x - 1][y].Terrain = 4;
                }

                if (Landscape[x - 1][y - 1].Terrain == 0) {
                    Landscape[x - 1][y - 1].Terrain = 4;
                }

                if (Landscape[x][y - 1].Terrain == 0) {
                    Landscape[x][y - 1].Terrain = 4;
                }

                if (Landscape[x + 1][y - 1].Terrain == 0) {
                    Landscape[x + 1][y - 1].Terrain = 4;
                }

                if (Landscape[x + 1][y].Terrain == 0) {
                    Landscape[x + 1][y].Terrain = 4;
                }

                if (Landscape[x + 1][y + 1].Terrain == 0) {
                    Landscape[x + 1][y + 1].Terrain = 4;
                }

                if (Landscape[x][y + 1].Terrain == 0) {
                    Landscape[x][y + 1].Terrain = 4;
                }

                if (Landscape[x - 1][y + 1].Terrain == 0) {
                    Landscape[x - 1][y + 1].Terrain = 4;
                }
            }

            if ((Landscape[x][y].Object != Tiles::PIPE) || (Landscape[x][y].AnimationPhase >= Bmp[Tiles::PIPE].AnimationPhaseCount)) {
                continue;
            }

            if ((Landscape[x - 1][y].Object >= Tiles::RIVER_5) && (Landscape[x - 1][y].Object <= Tiles::RIVER_10)) {
                Landscape[x - 1][y].Object += 14;
                Landscape[x - 1][y].ObjectPosOffset.x = static_cast<short>(Bmp[Landscape[x - 1][y].Object].targetRect.left);
                Landscape[x - 1][y].ObjectPosOffset.y = static_cast<short>(Bmp[Landscape[x - 1][y].Object].targetRect.top);
                CheckPipe(x, y);
            } else if ((Landscape[x - 1][y].Object >= Tiles::FLOODGATE_1) && (Landscape[x - 1][y].Object <= Tiles::FLOODGATE_6)) {
                CheckPipe(x, y);
            }

            if ((Landscape[x][y - 1].Object >= Tiles::RIVER_5) && (Landscape[x][y - 1].Object <= Tiles::RIVER_10)) {
                Landscape[x][y - 1].Object += 14;
                Landscape[x][y - 1].ObjectPosOffset.x = static_cast<short>(Bmp[Landscape[x][y - 1].Object].targetRect.left);
                Landscape[x][y - 1].ObjectPosOffset.y = static_cast<short>(Bmp[Landscape[x][y - 1].Object].targetRect.top);
                CheckPipe(x, y);
            } else if ((Landscape[x][y - 1].Object >= Tiles::FLOODGATE_1) && (Landscape[x][y - 1].Object <= Tiles::FLOODGATE_6)) {
                CheckPipe(x, y);
            }

            if ((Landscape[x + 1][y].Object >= Tiles::RIVER_5) && (Landscape[x + 1][y].Object <= Tiles::RIVER_10)) {
                Landscape[x + 1][y].Object += 14;
                Landscape[x + 1][y].ObjectPosOffset.x = static_cast<short>(Bmp[Landscape[x + 1][y].Object].targetRect.left);
                Landscape[x + 1][y].ObjectPosOffset.y = static_cast<short>(Bmp[Landscape[x + 1][y].Object].targetRect.top);
                CheckPipe(x, y);
            } else if ((Landscape[x + 1][y].Object >= Tiles::FLOODGATE_1) && (Landscape[x + 1][y].Object <= Tiles::FLOODGATE_6)) {
                CheckPipe(x, y);
            }

            if ((Landscape[x][y + 1].Object >= Tiles::RIVER_5) && (Landscape[x][y + 1].Object <= Tiles::RIVER_10)) {
                Landscape[x][y + 1].Object += 14;
                Landscape[x][y + 1].ObjectPosOffset.x = static_cast<short>(Bmp[Landscape[x][y + 1].Object].targetRect.left);
                Landscape[x][y + 1].ObjectPosOffset.y = static_cast<short>(Bmp[Landscape[x][y + 1].Object].targetRect.top);
                CheckPipe(x, y);
            } else if ((Landscape[x][y + 1].Object >= Tiles::FLOODGATE_1) && (Landscape[x][y + 1].Object <= Tiles::FLOODGATE_6)) {
                CheckPipe(x, y);
            }
        }

    // Felder auf trockenen Wiesen löschen
    for (short y = 0; y < MAX_TILESY; y++) {
        for (short x = 0; x < MAX_TILES_X; x++) {
            if ((Landscape[x][y].Object == Tiles::FIELD) && (Landscape[x][y].Terrain == 0)) {
                Landscape[x][y].Object = Tiles::INVALID;
                Landscape[x][y].ObjectPosOffset.x = 0;
                Landscape[x][y].ObjectPosOffset.y = 0;
                Landscape[x][y].AnimationPhase = -1;
                Landscape[x][y].ConstructionActionStep = 0;
            }
        }
    }

    Generate();
}

bool CheckRiverFlow(short x, short y) // Nachprüfen ob auf aktuellem Teil ein Fluss ist (Nur für die Fluss-Routine)
{
    for (short i = 0; i < NUMBER_OF_RIVERS; i++) {
        for (short j = 0; j < MAX_RIVER_LENGTH; j++) {
            if ((x == Rivers[i][j].x) && (y == Rivers[i][j].y)) {
                return true;
            }
        }
    }

    return false;
}

void CreateRiver() // Number of rivers and the minimum length
{
    short i;
    short l = 0;
    short direction = 0; // Current flow direction from 0-3
    short x0 = 0;
    short y0 = 0;
    short x1 = 0;
    short y1 = 0;
    short x2 = 0; // x2,y2 Coordinate of the part to be tested
    short y2 = 0;
    bool hasFound = false;
    RiverRun flowTmp[MAX_RIVER_LENGTH]; // To cache the experiment
    short riverLength[NUMBER_OF_RIVERS];

    for (short m = 0; m < NUMBER_OF_RIVERS; m++)
        for (short j = 0; j < MAX_RIVER_LENGTH; j++) {
            Rivers[m][j].x = -1;
            Rivers[m][j].y = -1;
        }

    for (short m = 0; m < NUMBER_OF_RIVERS; m++) {
        bool isComplete = false;
        short tmpLength = 0;

        for (short j = 0; j < MAX_RIVER_LENGTH; j++) {
            flowTmp[j].x = -1;
            flowTmp[j].y = -1;
        }

        for (short k = 0; k < 1000; k++) {
            for (short o = 0; o < 10000; o++) {
                hasFound = true;

                x0 = rand() % MAX_TILES_X; // determine appropriate source/spring
                y0 = rand() % MAX_TILESY;

                if (CheckRiverFlow(x0, y0)) {
                    hasFound = false;
                }

                if ((Landscape[x0][y0].Type != 0) ||
                        (Landscape[x0][y0].Height < 2)) {
                    hasFound = false;
                }

                if (hasFound) {
                    break;
                }
            }

            if (!hasFound) {
//                MessageBeep(MB_OK);
                break;
            } // Wenn keine Quelle mehr gefunden aufhören

            Rivers[m][0].x = x0;
            Rivers[m][0].y = y0;
            bool Strand = false;

            for (i = 1; i < MAX_RIVER_LENGTH; i++) {
                hasFound = false;

                if (!Strand) {
                    direction = rand() % 4;    // Auf dem Strand geradeausfliessen
                }

                for (short l2 = 0; l2 < 4; l2++) {
                    l = (direction + l2) % 4; // Im Urzeigersinn nachprüfen und bei der vorgegeben Richtung anfangen

                    x1 = Rivers[m][i - 1].x;
                    y1 = Rivers[m][i - 1].y;

                    if (l == 0) {
                        x2 = x1 - 1;
                        y2 = y1;

                        if (((Landscape[x1][y1].Type == 0) || (Landscape[x1][y1].Type == 2)) &&
                                ((Landscape[x2][y2].Type == 0) || (Landscape[x2][y2].Type == 2))) {
                            hasFound = (!CheckRiverFlow(x2, y2));

                            if (hasFound) {
                                break;
                            }
                        }
                    }

                    if (l == 1) {
                        x2 = x1;
                        y2 = y1 - 1;

                        if (((Landscape[x1][y1].Type == 0) || (Landscape[x1][y1].Type == 1)) &&
                                ((Landscape[x2][y2].Type == 0) || (Landscape[x2][y2].Type == 1))) {
                            hasFound = (!CheckRiverFlow(x2, y2));

                            if (hasFound) {
                                break;
                            }
                        }
                    }

                    if (l == 2) {
                        x2 = x1 + 1;
                        y2 = y1;

                        if (((Landscape[x1][y1].Type == 0) || (Landscape[x1][y1].Type == 4)) &&
                                ((Landscape[x2][y2].Type == 0) || (Landscape[x2][y2].Type == 4))) {
                            hasFound = (!CheckRiverFlow(x2, y2));

                            if (hasFound) {
                                break;
                            }
                        }
                    }

                    if (l == 3) {
                        x2 = x1;
                        y2 = y1 + 1;

                        if (((Landscape[x1][y1].Type == 0) || (Landscape[x1][y1].Type == 3)) &&
                                ((Landscape[x2][y2].Type == 0) || (Landscape[x2][y2].Type == 3))) {
                            hasFound = (!CheckRiverFlow(x2, y2));

                            if (hasFound) {
                                break;
                            }
                        }
                    }
                }

                if (!hasFound) {
                    break;
                }

                Rivers[m][i].x = x2;
                Rivers[m][i].y = y2;
                direction = l;

                if ((Landscape[Rivers[m][i].x][Rivers[m][i].y].Terrain == 2) && // Auf Strand die Richtung beibehalten
                        (Landscape[Rivers[m][i].x][Rivers[m][i].y].Type == 0)) {
                    if (Strand == true) {
                        break;    // Nur ein Strandstück überfliessen
                    }

                    Strand = true;
                }

                if ((Landscape[Rivers[m][i].x][Rivers[m][i].y].Terrain == 1) && // im meer aufhören
                        (Landscape[Rivers[m][i].x][Rivers[m][i].y].Type == 0)) {
                    isComplete = true;
                    break;
                }
            }

            if (isComplete) {
                if (i > tmpLength) {
                    // neue Variante speichern
                    if (hasFound) {
                        tmpLength = i;
                    } else {
                        tmpLength = i - 1;
                    }

                    for (short j = 0; j <= tmpLength; j++) {
                        flowTmp[j].x = Rivers[m][j].x;
                        flowTmp[j].y = Rivers[m][j].y;
                    }
                }
            }

            for (i = 0; i < MAX_RIVER_LENGTH; i++) {
                Rivers[m][i].x = -1;
                Rivers[m][i].y = -1;
            }

            isComplete = false;
        }

        // Den längsten Fluss nehmen
        riverLength[m] = tmpLength;

        for (short j = 0; j <= riverLength[m]; j++) {
            Rivers[m][j].x = flowTmp[j].x;
            Rivers[m][j].y = flowTmp[j].y;
        }

        // Die richtigen Wasserkacheln auswählen
        x0 = y0 = x1 = y1 = x2 = y2 = -1;

        for (m = 0; m < NUMBER_OF_RIVERS; m++) {
            for (i = 0; i <= riverLength[m]; i++) {
                // For the tile, one process before
                if (x1 != -1 && y1 != -1) {
                    Landscape[x1][y1].ObjectPosOffset.x = static_cast<short>(Bmp[Landscape[x1][y1].Object].targetRect.left);
                    Landscape[x1][y1].ObjectPosOffset.y = static_cast<short>(Bmp[Landscape[x1][y1].Object].targetRect.top);
                }

                x1 = Rivers[m][i].x;
                y1 = Rivers[m][i].y;

                Landscape[x1][y1].AnimationPhase = 0;

                if (Landscape[x1][y1].Terrain == 0) {
                    Landscape[x1][y1].Terrain = 4;
                }

                if (Landscape[x1 - 1][y1].Terrain == 0) {
                    Landscape[x1 - 1][y1].Terrain = 4;
                }

                if (Landscape[x1 - 1][y1 - 1].Terrain == 0) {
                    Landscape[x1 - 1][y1 - 1].Terrain = 4;
                }

                if (Landscape[x1][y1 - 1].Terrain == 0) {
                    Landscape[x1][y1 - 1].Terrain = 4;
                }

                if (Landscape[x1 + 1][y1 - 1].Terrain == 0) {
                    Landscape[x1 + 1][y1 - 1].Terrain = 4;
                }

                if (Landscape[x1 + 1][y1].Terrain == 0) {
                    Landscape[x1 + 1][y1].Terrain = 4;
                }

                if (Landscape[x1 + 1][y1 + 1].Terrain == 0) {
                    Landscape[x1 + 1][y1 + 1].Terrain = 4;
                }

                if (Landscape[x1][y1 + 1].Terrain == 0) {
                    Landscape[x1][y1 + 1].Terrain = 4;
                }

                if (Landscape[x1 - 1][y1 + 1].Terrain == 0) {
                    Landscape[x1 - 1][y1 + 1].Terrain = 4;
                }

                if (i < riverLength[m]) {
                    x2 = Rivers[m][i + 1].x;
                    y2 = Rivers[m][i + 1].y;
                }

                if (i != 0) {
                    x0 = Rivers[m][i - 1].x;
                    y0 = Rivers[m][i - 1].y;
                } else {
                    // Quellen
                    if (x2 > x1) {
                        Landscape[x1][y1].Object = Tiles::RIVER_START_1;
                        continue;
                    }

                    if (x2 < x1) {
                        Landscape[x1][y1].Object = Tiles::RIVER_START_4;
                        continue;
                    }

                    if (y2 > y1) {
                        Landscape[x1][y1].Object = Tiles::RIVER_START_2;
                        continue;
                    }

                    if (y2 < y1) {
                        Landscape[x1][y1].Object = Tiles::RIVER_START_3;
                        continue;
                    }
                }

                // Alle Möglichkeiten durchgehen

                if (Landscape[x1][y1].Type == 1) {
                    Landscape[x1][y1].Object = Tiles::RIVER_1;
                }

                if (Landscape[x1][y1].Type == 2) {
                    Landscape[x1][y1].Object = Tiles::RIVER_2;
                }

                if (Landscape[x1][y1].Type == 3) {
                    Landscape[x1][y1].Object = Tiles::RIVER_3;
                }

                if (Landscape[x1][y1].Type == 4) {
                    Landscape[x1][y1].Object = Tiles::RIVER_4;
                }

                if (Landscape[x1][y1].Type == 0) {
                    if ((x0 < x1) && (y0 == y1)) {
                        if (Landscape[x1][y1].Terrain == 2) {
                            Landscape[x1][y1].Object = Tiles::RIVER_END_3;    // River end/mouth
                        } else {
                            if ((x1 < x2) && (y1 == y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_5;
                            }

                            if ((x1 == x2) && (y1 < y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_7;
                            }

                            if ((x1 == x2) && (y1 > y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_9;
                            }
                        }
                    }

                    if ((x0 == x1) && (y0 < y1)) {
                        if (Landscape[x1][y1].Terrain == 2) {
                            Landscape[x1][y1].Object = Tiles::RIVER_END_4;    // River end/mouth
                        } else {
                            if ((x1 < x2) && (y1 == y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_8;
                            }

                            if ((x1 == x2) && (y1 < y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_6;
                            }

                            if ((x1 > x2) && (y1 == y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_9;
                                Landscape[x1][y1].ReverseAnimation = true;
                            }
                        }
                    }

                    if ((x0 > x1) && (y0 == y1)) {
                        if (Landscape[x1][y1].Terrain == 2) {
                            Landscape[x1][y1].Object = Tiles::RIVER_END_1;    // River end/mouth
                        } else {
                            if ((x1 > x2) && (y1 == y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_5;
                            }

                            if ((x1 == x2) && (y1 < y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_10;
                            }

                            if ((x1 == x2) && (y1 > y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_8;
                            }

                            Landscape[x1][y1].ReverseAnimation = true;
                        }
                    }

                    if ((x0 == x1) && (y0 > y1)) {
                        if (Landscape[x1][y1].Terrain == 2) {
                            Landscape[x1][y1].Object = Tiles::RIVER_END_2;    // River end/mouth
                        } else {
                            if ((x1 == x2) && (y1 > y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_6;
                            }

                            if ((x1 > x2) && (y1 == y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_7;
                            }

                            Landscape[x1][y1].ReverseAnimation = true;

                            if ((x1 < x2) && (y1 == y2)) {
                                Landscape[x1][y1].Object = Tiles::RIVER_10;
                                Landscape[x1][y1].ReverseAnimation = false;
                            }
                        }
                    }
                }
            }

            //For the last stretch of the river
            Landscape[x1][y1].ObjectPosOffset.x = static_cast<short>(Bmp[Landscape[x1][y1].Object].targetRect.left);
            Landscape[x1][y1].ObjectPosOffset.y = static_cast<short>(Bmp[Landscape[x1][y1].Object].targetRect.top);
        }
    }
}

void CreateTrees(short percent)
{
    Coordinate Pos; // There is the tree
    bool hasBigTree = false; // there is already a big tree

    for (short y = 0; y < MAX_TILESY; y++)//Alle Kacheln durchgehen
        for (short x = 0; x < MAX_TILES_X; x++) {
            if ((Landscape[x][y].Object != -1) || ((Landscape[x][y].Terrain == 3) && (Landscape[x][y].Type == 0))) {
                continue;
            }

            // If there is already an object or quicksand, then continue with the next part
            if (rand() % (100 / percent) != 0) { // Determine the probability of a tree
                continue;
            }

            while (true) {
                Pos.x = rand() % TILE_SIZE_X;
                Pos.y = rand() % TILE_SIZE_Y;
                Coordinate Erg = Renderer::GetTile(Landscape[x][y].xScreen + Pos.x, Landscape[x][y].yScreen + Pos.y);

                if ((Erg.x == x) && (Erg.y == y)) {
                    break;
                }
            }

            if ((Landscape[x][y].Terrain == 2) && (Landscape[x][y].Type == 0)) { // Only make palm trees at the beach
                Landscape[x][y].Object = Tiles::TREE_2;
            } else {
                short r = rand() % 5; // random speicherung
                Landscape[x][y].Object = Tiles::TREE_1 + r;

                if ((rand() % 50 == 1) || (!hasBigTree)) {
                    Landscape[x][y].Object = Tiles::TREE_BIG;
                    hasBigTree = true;
                }
            }

            // Linke obere Ecke speichern
            Landscape[x][y].ObjectPosOffset.x = Pos.x - static_cast<short>(Bmp[Landscape[x][y].Object].Width) / 2;
            Landscape[x][y].ObjectPosOffset.y = Pos.y - static_cast<short>(Bmp[Landscape[x][y].Object].Height);

            // Startphase
            if (Landscape[x][y].Object == Tiles::BUSH) {
                Landscape[x][y].AnimationPhase = static_cast<float>(Bmp[Landscape[x][y].Object].AnimationPhaseCount) - 1;
            } else
                Landscape[x][y].AnimationPhase = static_cast<float>(Bmp[Landscape[x][y].Object].AnimationPhaseCount -
                                             rand() % (Bmp[Landscape[x][y].Object].AnimationPhaseCount) - 1);
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
            if (Landscape[x][i].Terrain != 1) {
                y = i - 1;
                break;
            }
        }

        break;

    case 1:
        y = MAX_TILESY / 2;

        for (short i = MAX_TILES_X - 1; i >= 0; i--) {
            if (Landscape[i][y].Terrain != 1) {
                x = i + 1;
                break;
            }
        }

        break;

    case 2:
        x = MAX_TILES_X / 2;

        for (short i = MAX_TILESY - 1; i >= 0; i--) {
            if (Landscape[x][i].Terrain != 1) {
                y = i + 1;
                break;
            }
        }

        break;
    }

    Landscape[x][y].Object = Tiles::WRECK_2;
    Landscape[x][y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::WRECK_2].targetRect.left);
    Landscape[x][y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::WRECK_2].targetRect.top);
}

void Treasure()
{
    puts("Rendering treasure map");
    sf::Image treasureMap;
    treasureMap.create(TREASUREMAP_WIDTH, TREASUREMAP_HEIGHT, sf::Color::Red);
    sf::Image landscape = Renderer::landscapeImage();
    while (true) {

        // This tile is viewed
        short x = rand() % (MAX_TILES_X - 1);
        short y = rand() % (MAX_TILESY - 1);

        // only on flat tiles without an object
        if ((Landscape[x][y].Object == -1) && (Landscape[x][y].Type == 0) && (Landscape[x][y].Terrain != 3)) {
            if (SchatzPos.x == -1) {
                SchatzPos.x = x;
                SchatzPos.y = y;
            }

//            lpDDSScape->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr);
//            lpDDSSchatzkarte->Lock(nullptr, &ddsd2, DDLOCK_WAIT, nullptr);

            RGBSTRUCT rgbStruct;
            for (short i = 0; i < TREASUREMAP_WIDTH; i++) {
                for (short j = 0; j < TREASUREMAP_HEIGHT; j++) {
                    rgbStruct = Renderer::GetPixel(static_cast<short>(i + Landscape[SchatzPos.x][SchatzPos.y].xScreen - TREASUREMAP_WIDTH / 2 + TILE_SIZE_X / 2),
                                       static_cast<short>(j + Landscape[SchatzPos.x][SchatzPos.y].yScreen - TREASUREMAP_HEIGHT / 2 + 30), &landscape);
//                    printf("r %d g %d b %d\n", rgbStruct.r, rgbStruct.g, rgbStruct.b);
                    Renderer::PutPixel(i, j,
                                       (rgbStruct.r * 30 + rgbStruct.g * 59 + rgbStruct.b * 11) / 100,
                                       (rgbStruct.r * 30 + rgbStruct.g * 59 + rgbStruct.b * 11) / 100,
                                       (rgbStruct.r * 30 + rgbStruct.g * 59 + rgbStruct.b * 11) / 100 * 3 / 4, &treasureMap);
                }
            }

//            lpDDSScape->Unlock(nullptr);
//            lpDDSSchatzkarte->Unlock(nullptr);

//            rcRectsrc = Bmp[Tiles::CROSS].sourceRect;
//            rcRectdes.left = TREASUREMAP_WIDTH / 2 - Bmp[Tiles::CROSS].Width / 2;
//            rcRectdes.right = rcRectdes.left + Bmp[Tiles::CROSS].Width;
//            rcRectdes.top = TREASUREMAP_HEIGHT / 2 - Bmp[Tiles::CROSS].Height / 2;
//            rcRectdes.bottom = rcRectdes.top + Bmp[Tiles::CROSS].Height;
//            lpDDSSchatzkarte->loadFromImage(treasureMap);
//            s_treasureMapSprite->setPosition(TREASUREMAP_WIDTH / 2 - Bmp[Tiles::CROSS].Width / 2, rcRectdes.left + Bmp[Tiles::CROSS].Width);
//            Renderer::Blit(Bmp[Tiles::CROSS].Surface, lpDDSSchatzkarte, true);

//            lpDDSSchatzkarte->Lock(nullptr, &ddsd2, DDLOCK_WAIT, nullptr);

            // Blur
            for (short i = 0; i < TREASUREMAP_WIDTH; i++) {
                for (short j = 0; j < TREASUREMAP_HEIGHT; j++) {
                    if ((i > 0) && (i < TREASUREMAP_WIDTH - 1) && (j > 0) && (j < TREASUREMAP_HEIGHT - 1)) {
                        RGBSTRUCT rgbleft = Renderer::GetPixel(i - 1, j, &treasureMap);
                        RGBSTRUCT rgbtop = Renderer::GetPixel(i, j - 1, &treasureMap);
                        RGBSTRUCT rgbright = Renderer::GetPixel(i + 1, j, &treasureMap);
                        RGBSTRUCT rgbbottom = Renderer::GetPixel(i, j + 1, &treasureMap);
                        rgbStruct = Renderer::GetPixel(i, j, &treasureMap);
                        Renderer::PutPixel(i, j,
                                               (rgbleft.r + rgbtop.r + rgbright.r + rgbbottom.r + rgbStruct.r) / 5,
                                               (rgbleft.g + rgbtop.g + rgbright.g + rgbbottom.g + rgbStruct.g) / 5,
                                               (rgbleft.b + rgbtop.b + rgbright.b + rgbbottom.b + rgbStruct.b) / 5, &treasureMap);
                    }
                }
            }
//            Renderer::BlitToScreen(lpDDSSchatzkarte);

//            lpDDSSchatzkarte->Unlock(nullptr);
            break;
        }
    }
    lpDDSSchatzkarte->loadFromImage(treasureMap);
}

void OnRawMaterialsUsed(short Objekt)
{
    if (((Objekt == Tiles::RAW_STONE) && (TwoClicks == Tiles::RAW_TREE_BRANCH)) || ((Objekt == Tiles::RAW_TREE_BRANCH) && (TwoClicks == Tiles::RAW_STONE))) {
        if (Guy.Inventory[Tiles::RAW_AXE] < 1) {
            Guy.Inventory[Tiles::RAW_STONE]--;
            Guy.Inventory[Tiles::RAW_TREE_BRANCH]--;
            Guy.Inventory[Tiles::RAW_AXE] = 1;
            Bmp[Tiles::BUTTON_CHOP].AnimationPhase = 0;
            Bmp[Tiles::BUTTON_BOAT].AnimationPhase = 0;
            Bmp[Tiles::BUTTON_PIPE].AnimationPhase = 0;
            PapierText = Renderer::DrawText(STRING_BUILD_AXE, TXTPAPIER, 1);
            PlaySound(Sound::INVENTION, 100);
        } else if (Guy.Inventory[Tiles::RAW_HOE] < 1) {
            Guy.Inventory[Tiles::RAW_STONE]--;
            Guy.Inventory[Tiles::RAW_TREE_BRANCH]--;
            Guy.Inventory[Tiles::RAW_HOE] = 1;
            Bmp[Tiles::BUTTON_FARM].AnimationPhase = 0;
            PapierText = Renderer::DrawText(BAUEEGGE, TXTPAPIER, 1);
            PlaySound(Sound::INVENTION, 100);
        } else {
            PapierText = Renderer::DrawText(STRING_STONE_BRANCH_NOTHING_ELSE, TXTPAPIER, 1);
        }
    } else if (((Objekt == Tiles::RAW_LIANA) && (TwoClicks == Tiles::RAW_TREE_BRANCH)) || ((Objekt == Tiles::RAW_TREE_BRANCH) && (TwoClicks == Tiles::RAW_LIANA))) {
        if (Guy.Inventory[Tiles::RAW_FISHING_POLE] < 1) {
            Guy.Inventory[Tiles::RAW_LIANA]--;
            Guy.Inventory[Tiles::RAW_TREE_BRANCH]--;
            Guy.Inventory[Tiles::RAW_FISHING_POLE] = 1;
            Bmp[Tiles::BUTTON_FISH].AnimationPhase = 0;
            PapierText = Renderer::DrawText(BAUEANGEL, TXTPAPIER, 1);
            PlaySound(Sound::INVENTION, 100);
        } else {
            PapierText = Renderer::DrawText(ASTPLUSLIANENICHTS, TXTPAPIER, 1);
        }
    } else if (((Objekt == Tiles::RAW_LIANA) && (TwoClicks == Tiles::RAW_STONE)) || ((Objekt == Tiles::RAW_STONE) && (TwoClicks == Tiles::RAW_LIANA))) {
        if (Guy.Inventory[Tiles::RAW_SLINGSHOT] < 1) {
            Guy.Inventory[Tiles::RAW_LIANA]--;
            Guy.Inventory[Tiles::RAW_STONE]--;
            Guy.Inventory[Tiles::RAW_SLINGSHOT] = 1;
            Bmp[Tiles::BUTTON_SLINGSHOT].AnimationPhase = 0;
            PapierText = Renderer::DrawText(BAUESCHLEUDER, TXTPAPIER, 1);
            PlaySound(Sound::INVENTION, 100);
        } else {
            PapierText = Renderer::DrawText(STEINPLUSLIANENICHTS, TXTPAPIER, 1);
        }
    } else {
        PapierText = Renderer::DrawText(STRING_CANT_BUILD, TXTPAPIER, 1);
    }

    TwoClicks = -1;
}

void UpdateDiscovered()
{
    bool changed = false;

    for (short i = -1; i <= 1; i++)
        for (short j = -1; j <= 1; j++) {
            if (!Landscape[Guy.Pos.x + i][Guy.Pos.y + j].Discovered) {
                Landscape[Guy.Pos.x + i][Guy.Pos.y + j].Discovered = true;
                changed = true;
            }
        }

    if (changed) {
        Generate();
    }
}
} // namespace World
