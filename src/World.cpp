#include "Action.hpp"
#include "Renderer.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

#include <cstdio>
#include <cstring>

#include "extern.hpp"

namespace World
{
    FLUSSLAUF RiverCourse[FLUSSANZAHL][MAXFLUSS];

    void MakeResourceString(const short x, const short y, const short object)
    {
        char tempString[1024];

        RohString[0] = 0;
        auto noResources = true;
        if (object == -1)
        {
            for (auto i : Scape[x][y].Rohstoff)
            {
                if (i != 0)
                {
                    noResources = false;
                }
            }
        }
        else
        {
            for (auto i : Bmp[object].Rohstoff)
            {
                if (i != 0)
                {
                    noResources = false;
                }
            }
        }
        if (noResources)
        {
            return;
        }

        strcat(RohString, " ->");
        for (short i = 0; i < ImageCount; i++)
        {
            if (object == -1)
            {
                if (Scape[x][y].Rohstoff[i] == 0)
                {
                    continue;
                }
            }
            else
            {
                if (Bmp[object].Rohstoff[i] == 0)
                {
                    continue;
                }
            }
            strcat(RohString, " ");
            switch (i)
            {
            case ResourceBranch:
                LoadString(g_hInst, AST, tempString, 1024);
                break;
            case ResourceStone:
                LoadString(g_hInst, STEIN, tempString, 1024);
                break;
            case ResourceLeaf:
                LoadString(g_hInst, BLATT, tempString, 1024);
                break;
            case ResourceVine:
                LoadString(g_hInst, LIANE, tempString, 1024);
                break;
            case ResourceTrunk:
                LoadString(g_hInst, STAMM, tempString, 1024);
                break;
            default:
                break;
            }
            strcat(RohString, tempString);
            strcat(RohString, "=");
            if (object == -1)
            {
                std::sprintf(tempString, "%d", Scape[x][y].Rohstoff[i]);
            }
            else
            {
                std::sprintf(tempString, "%d", Bmp[object].Rohstoff[i]);
            }
            strcat(RohString, tempString);
        }
    }

    void AddTime(const short hours, const short minutes)
    {
        Stunden += hours;
        Minuten += minutes;
        if (Minuten >= 60)
        {
            Minuten -= 60;
            Stunden++;
        }
        for (short y = 0; y < MAXYKACH; y++)
        {
            for (auto& x : Scape)
            {
                // Feuer nach einer bestimmten Zeit ausgehen lassen
                if (x[y].Objekt == Feuer)
                {
                    x[y].Timer += static_cast<float>((60 * hours + minutes) * 0.0005);
                    if (x[y].Timer >= 1)
                    {
                        x[y].Objekt = -1;
                        x[y].Timer = 0;
                        x[y].ObPos.x = 0;
                        x[y].ObPos.y = 0;
                        x[y].Phase = -1;
                        Chance -= 2 + 2 * x[y].Hoehe;
                    }
                }
                if (x[y].Phase == -1 || x[y].Objekt != Feld && x[y].Objekt != Busch)
                {
                    continue; // Wenn kein Fruchtobjekt weiter
                }
                if (x[y].Phase >= Bmp[x[y].Objekt].Anzahl)
                {
                    continue;
                }
                if (x[y].Objekt == Feld)
                {
                    x[y].Phase += static_cast<float>(60 * hours + minutes) * 0.005f;
                }
                else if (x[y].Objekt == Busch)
                {
                    x[y].Phase += static_cast<float>(60 * hours + minutes) * 0.0005f; // pro Minute Reifungsprozess fortführen
                }
                if (x[y].Phase > Bmp[x[y].Objekt].Anzahl - 1)
                {
                    x[y].Phase = static_cast<float>(Bmp[x[y].Objekt].Anzahl) - 1;
                }
            }
        }
        AddResource(GESUNDHEIT, (60 * hours + minutes) * (Guy.Resource[WASSER] - 50 + Guy.Resource[NAHRUNG] - 50) / 1000);

        if (Spielzustand == State::Game && !BootsFahrt)
        {
            for (short i = 0; i <= 60 * hours + minutes; i++)
            {
                if (Chance == 0)
                {
                    break;
                }
                if (rand() % static_cast<int>(1 / (Chance / 72000)) == 1)
                {
                    Guy.Aktiv = false;
                    Guy.AkNummer = 0;
                    Guy.Aktion = static_cast<short>(Action::Actions::Rescued);
                    break;
                }
            }
        }
    }

    void AddResource(const short resourceType, const float amount) // Fügt wassser usw hinzu
    {
        Guy.Resource[resourceType] += amount;
        if (Guy.Resource[resourceType] > 100)
        {
            Guy.Resource[resourceType] = 100;
        }
        if (Guy.Resource[resourceType] < 0)
        {
            Guy.Resource[resourceType] = 0;
        }

        // Wann tod
        if (Guy.Resource[GESUNDHEIT] <= 0 && Guy.Aktion != static_cast<int>(Action::Actions::Death) &&
            Guy.Aktion != static_cast<int>(Action::Actions::DayEnd) && Spielzustand == State::Game)
        {
            Guy.Aktiv = false;
            Guy.AkNummer = 0;
            Guy.Aktion = static_cast<short>(Action::Actions::Death);
        }
    }

    void Generate()
    {
        // Die Kartehintergrundfarbe
        rcRectdes.left = 0;
        rcRectdes.top = 0;
        rcRectdes.right = 2 * MAXXKACH;
        rcRectdes.bottom = 2 * MAXYKACH;
        ddbltfx.dwFillColor = Renderer::RGB2DWORD(247, 222, 191);
        lpDDSKarte->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

        // Die Landschaftshintergrundfarbe
        rcRectdes.left = 0;
        rcRectdes.top = 0;
        rcRectdes.right = MAXSCAPEX;
        rcRectdes.bottom = MAXSCAPEY;
        ddbltfx.dwFillColor = Renderer::RGB2DWORD(0, 0, 0);
        lpDDSScape->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

        for (short y = 0; y < MAXYKACH; y++)
        {
            for (short x = 0; x < MAXXKACH; x++)
            {
                if (!Scape[x][y].Entdeckt)
                {
                    continue; // Nicht entdeckte Felder nicht malen
                }

                rcRectdes.left = Scape[x][y].xScreen;
                rcRectdes.top = Scape[x][y].yScreen;
                rcRectdes.right = rcRectdes.left + KXPIXEL;
                rcRectdes.bottom = rcRectdes.top + KYPIXEL;

                if (Scape[x][y].Art == 4)
                {
                    rcRectsrc.left = KXPIXEL * Scape[x][y].Typ;
                    rcRectsrc.right = KXPIXEL * Scape[x][y].Typ + KXPIXEL;
                    rcRectsrc.top = 0;
                    rcRectsrc.bottom = KYPIXEL;
                }
                else if (Scape[x][y].Art == 0) // trockenes Land
                {
                    rcRectsrc.left = KXPIXEL * Scape[x][y].Typ;
                    rcRectsrc.right = KXPIXEL * Scape[x][y].Typ + KXPIXEL;
                    rcRectsrc.top = 4 * KYPIXEL;
                    rcRectsrc.bottom = 5 * KYPIXEL;
                }
                else
                {
                    if (Scape[x][y].Typ == 0 && Scape[x][y].Art == 1)
                    {
                        rcRectsrc.left = 0 * KXPIXEL;
                        rcRectsrc.top = 3 * KYPIXEL;
                        rcRectsrc.right = 1 * KXPIXEL;
                        rcRectsrc.bottom = 4 * KYPIXEL;
                    }
                    if (Scape[x][y].Typ == 0 && Scape[x][y].Art == 2)
                    {
                        rcRectsrc.left = 1 * KXPIXEL;
                        rcRectsrc.top = 3 * KYPIXEL;
                        rcRectsrc.right = 2 * KXPIXEL;
                        rcRectsrc.bottom = 4 * KYPIXEL;
                    }
                    if (Scape[x][y].Typ == 0 && Scape[x][y].Art == 3)
                    {
                        rcRectsrc.left = 2 * KXPIXEL;
                        rcRectsrc.top = 3 * KYPIXEL;
                        rcRectsrc.right = 3 * KXPIXEL;
                        rcRectsrc.bottom = 4 * KYPIXEL;
                    }
                }

                // Landschaftskacheln zeichnen
                Renderer::Blit(lpDDSMisc, lpDDSScape, true);

                // Gitter drüberlegen
                if (Gitter)
                {
                    rcRectsrc.left = KXPIXEL * Scape[x][y].Typ;
                    rcRectsrc.right = KXPIXEL * Scape[x][y].Typ + KXPIXEL;
                    rcRectsrc.top = 1 * KYPIXEL;
                    rcRectsrc.bottom = 1 * KYPIXEL + KYPIXEL;
                    Renderer::Blit(lpDDSMisc, lpDDSScape, true);
                }

                // Landschaftsobjekte zeichnen (falls Animationen ausgeschaltet sind)
                if (!LAnimation && Scape[x][y].Objekt != -1)
                {
                    if (Scape[x][y].Objekt >= Meerwellen && Scape[x][y].Objekt <= Schleuse6)
                    {
                        rcRectsrc.left = Bmp[Scape[x][y].Objekt].rcSrc.left;
                        rcRectsrc.right = Bmp[Scape[x][y].Objekt].rcSrc.right;

                        if (Scape[x][y].Objekt == Meerwellen)
                        {
                            const auto i = static_cast<short>(rand() % 6);
                            rcRectsrc.top = Bmp[Scape[x][y].Objekt].rcSrc.top + i * Bmp[Scape[x][y].Objekt].Hoehe;
                            rcRectsrc.bottom = Bmp[Scape[x][y].Objekt].rcSrc.bottom + i * Bmp[Scape[x][y].Objekt].Hoehe;
                        }
                        else
                        {
                            rcRectsrc.top = Bmp[Scape[x][y].Objekt].rcSrc.top;
                            rcRectsrc.bottom = Bmp[Scape[x][y].Objekt].rcSrc.bottom;
                        }

                        rcRectdes.left = Scape[x][y].xScreen + Bmp[Scape[x][y].Objekt].rcDes.left;
                        rcRectdes.right = Scape[x][y].xScreen + Bmp[Scape[x][y].Objekt].rcDes.right;
                        rcRectdes.top = Scape[x][y].yScreen + Bmp[Scape[x][y].Objekt].rcDes.top;
                        rcRectdes.bottom = Scape[x][y].yScreen + Bmp[Scape[x][y].Objekt].rcDes.bottom;

                        // Landschaftsobjekt zeichnen
                        Renderer::Blit(lpDDSAnimation, lpDDSScape, true);
                    }
                }

                // MiniMap zeichnen
                rcRectdes.left = 2 * x;
                rcRectdes.top = 2 * y;
                rcRectdes.right = rcRectdes.left + 2;
                rcRectdes.bottom = rcRectdes.top + 2;

                if (Scape[x][y].Art == 1 && Scape[x][y].Typ == 0) // Meer
                {
                    ddbltfx.dwFillColor = Renderer::RGB2DWORD(228, 207, 182);
                }
                else
                {
                    if (Scape[x][y].Typ == 0 && (Scape[x][y].Art == 2 || Scape[x][y].Art == 3)) // Strand
                    {
                        ddbltfx.dwFillColor = Renderer::RGB2DWORD(112, 103, 93);
                    }
                    else // Land
                    {
                        ddbltfx.dwFillColor = Renderer::RGB2DWORD(139 + Scape[x][y].Hoehe * 20, 128 + Scape[x][y].Hoehe * 20, 115 + Scape[x][y].Hoehe * 20);
                    }
                }

                lpDDSKarte->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);
            }
        }
    }

    void CheckSpecialButton()
    {
        if (Scape[Guy.Pos.x][Guy.Pos.y].Objekt >= Feld
            && Scape[Guy.Pos.x][Guy.Pos.y].Objekt <= Feuerstelle
            && Scape[Guy.Pos.x][Guy.Pos.y].Phase >= Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl
            && Bmp[ButtonStop].Phase == -1)
        {
            if (Bmp[ButtonWeiter].Phase == -1)
            {
                Bmp[ButtonWeiter].Phase = 0;
            }
        }
        else
        {
            Bmp[ButtonWeiter].Phase = -1;
        }

        if (Bmp[ButtonStop].Phase == -1 && (Scape[Guy.Pos.x][Guy.Pos.y].Objekt == Boot &&
                                            Scape[Guy.Pos.x][Guy.Pos.y].Phase < Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Anzahl ||
                                            BootsFahrt &&
                                            (Scape[Guy.Pos.x - 1][Guy.Pos.y].Art != 1 && Scape[Guy.Pos.x - 1][Guy.Pos.y].Objekt == -1 ||
                                             Scape[Guy.Pos.x][Guy.Pos.y - 1].Art != 1 && Scape[Guy.Pos.x][Guy.Pos.y - 1].Objekt == -1 ||
                                             Scape[Guy.Pos.x + 1][Guy.Pos.y].Art != 1 && Scape[Guy.Pos.x + 1][Guy.Pos.y].Objekt == -1 ||
                                             Scape[Guy.Pos.x][Guy.Pos.y + 1].Art != 1 && Scape[Guy.Pos.x][Guy.Pos.y + 1].Objekt == -1)))
        {
            if (Bmp[ButtonAblegen].Phase == -1)
            {
                Bmp[ButtonAblegen].Phase = 0;
            }
        }
        else
        {
            Bmp[ButtonAblegen].Phase = -1;
        }
    }

    bool CheckResource()
    {
        short required = 0; // Anzahl der Gesamtbenötigten Rohstoffe
        for (auto i : Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].Rohstoff)
        {
            required += i;
        }

        auto GebrauchtTmp = required / static_cast<float>(Bmp[Scape[Guy.Pos.x][Guy.Pos.y].Objekt].AkAnzahl); // Soviel Rohstoffe werden für diesen Schritt benötigt
        auto Gebraucht = static_cast<short>(GebrauchtTmp * Scape[Guy.Pos.x][Guy.Pos.y].AkNummer -
                                            static_cast<short>(GebrauchtTmp * (Scape[Guy.Pos.x][Guy.Pos.y].AkNummer - 1))); // Soviel Rohstoffe werden für diesen Schritt benötigt

        while (true)
        {
            auto check = false; // Wenn kein Rohstoff mehr vorhanden nur noch einmal die While-Schleife
            for (short i = 0; i < ImageCount; i++)
            {
                if (Gebraucht == 0)
                {
                    return true;
                }
                if (Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i] > 0 &&
                    Guy.Inventar[i] > 0)
                {
                    Guy.Inventar[i]--;
                    Scape[Guy.Pos.x][Guy.Pos.y].Rohstoff[i]--;
                    Gebraucht--;
                    if (Gebraucht == 0)
                        return true;
                    check = true;
                }
            }
            if (check == false)
            {
                break;
            }
        }
        PapierText = Renderer::DrawText(ROHSTOFFNICHT, TXTPAPIER, 1);
        Guy.AkNummer = 0;
        Guy.Aktion = static_cast<short>(Action::Actions::Cancel);
        Bmp[ButtonStop].Phase = -1;
        return false;
    }

    void Compute(const short minSize, const short maxSize) // Groesse der Insel in Anzahl der Landkacheln
    {
        short i, j;
        bool found;

        short Vierecke[13][4][13] = // 0=Passt nicht 1=1runter 2=gleiche Hoehe 3=1hoch
        {
            //		  0	  1	  2	  3	  4	  5	  6	  7	  8	  9	 10	 11	 12		
            {
                {{2}, {0}, {1}, {0}, {2}, {2}, {0}, {0}, {2}, {0}, {0}, {1}, {1}}, //0
                {{2}, {1}, {0}, {2}, {0}, {0}, {0}, {2}, {2}, {0}, {1}, {1}, {0}}, //1
                /*0*/{{2}, {0}, {2}, {0}, {1}, {0}, {2}, {2}, {0}, {1}, {1}, {0}, {0}}, //2 Kante
                {{2}, {2}, {0}, {1}, {0}, {2}, {2}, {0}, {0}, {1}, {0}, {0}, {1}} //3
            },
            {
                {{0}, {2}, {0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {0}},
                {{2}, {1}, {0}, {2}, {0}, {0}, {0}, {2}, {2}, {0}, {1}, {1}, {0}},
                /*1*/{{0}, {2}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {0}, {0}, {2}, {0}},
                {{3}, {3}, {0}, {2}, {0}, {3}, {3}, {0}, {0}, {2}, {0}, {0}, {2}}
            },
            {
                {{2}, {0}, {1}, {0}, {2}, {2}, {0}, {0}, {2}, {0}, {0}, {1}, {1}},
                {{0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {0}, {0}, {2}},
                /*2*/{{3}, {0}, {3}, {0}, {2}, {0}, {3}, {3}, {0}, {2}, {2}, {0}, {0}},
                {{0}, {0}, {2}, {0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}}
            },
            {
                {{0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {2}, {0}, {0}, {0}},
                {{3}, {2}, {0}, {3}, {0}, {0}, {0}, {3}, {3}, {0}, {2}, {2}, {0}},
                /*3*/{{0}, {0}, {0}, {2}, {0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}},
                {{2}, {2}, {0}, {1}, {0}, {0}, {2}, {0}, {0}, {1}, {0}, {0}, {1}}
            },
            {
                {{3}, {0}, {2}, {0}, {3}, {3}, {0}, {0}, {3}, {0}, {0}, {2}, {2}},
                {{0}, {0}, {0}, {0}, {2}, {2}, {0}, {0}, {0}, {2}, {0}, {0}, {0}},
                /*4*/{{2}, {0}, {2}, {0}, {0}, {0}, {2}, {2}, {0}, {1}, {1}, {0}, {0}},
                {{0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {2}, {0}, {0}}
            }, //  0	  1	  2	  3	  4	  5	  6	  7	  8	  9	 10	 11	 12	
            {
                {{0}, {2}, {0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {0}},
                {{2}, {1}, {0}, {2}, {0}, {0}, {0}, {2}, {2}, {0}, {1}, {1}, {0}},
                /*5*/{{2}, {0}, {2}, {0}, {1}, {0}, {2}, {2}, {0}, {1}, {1}, {0}, {0}},
                {{0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {2}, {0}, {0}}
            },
            {
                {{2}, {0}, {1}, {0}, {2}, {2}, {0}, {0}, {2}, {0}, {0}, {1}, {1}},
                {{2}, {1}, {0}, {2}, {0}, {0}, {0}, {2}, {2}, {0}, {1}, {1}, {0}},
                /*6*/{{0}, {2}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {0}, {0}, {2}, {0}},
                {{0}, {0}, {2}, {0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}}
            },
            {
                {{2}, {0}, {1}, {0}, {2}, {2}, {0}, {0}, {2}, {0}, {0}, {1}, {1}},
                {{0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {0}, {0}, {2}},
                /*7*/{{0}, {0}, {0}, {2}, {0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}},
                {{2}, {2}, {0}, {1}, {0}, {2}, {2}, {0}, {0}, {1}, {0}, {0}, {1}}
            },
            {
                {{0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {2}, {0}, {0}, {0}},
                {{0}, {0}, {0}, {0}, {2}, {2}, {0}, {0}, {0}, {2}, {0}, {0}, {0}},
                /*8*/{{2}, {0}, {2}, {0}, {1}, {0}, {2}, {2}, {0}, {1}, {1}, {0}, {0}},
                {{2}, {2}, {0}, {1}, {0}, {2}, {2}, {0}, {0}, {1}, {0}, {0}, {1}}
            },
            {
                {{3}, {0}, {2}, {0}, {3}, {3}, {0}, {0}, {3}, {0}, {0}, {2}, {2}},
                {{3}, {2}, {0}, {3}, {0}, {0}, {0}, {3}, {3}, {0}, {2}, {2}, {0}},
                /*9*/{{0}, {0}, {0}, {2}, {0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}},
                {{0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {2}, {0}, {0}}
            }, //  0	  1	  2	  3	  4	  5	  6	  7	  8	  9	 10	 11	 12	
            {
                {{3}, {0}, {2}, {0}, {3}, {3}, {0}, {0}, {3}, {0}, {0}, {2}, {2}},
                {{0}, {0}, {0}, {0}, {2}, {2}, {0}, {0}, {0}, {2}, {0}, {0}, {0}},
                /*10*/{{0}, {2}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {0}, {0}, {2}, {0}},
                {{3}, {3}, {0}, {2}, {0}, {3}, {3}, {0}, {0}, {2}, {0}, {0}, {2}}
            },
            {
                {{0}, {2}, {0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {0}},
                {{0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {0}, {0}, {2}},
                /*11*/{{3}, {0}, {3}, {0}, {2}, {0}, {3}, {3}, {0}, {2}, {2}, {0}, {0}},
                {{3}, {3}, {0}, {2}, {0}, {3}, {3}, {0}, {0}, {2}, {0}, {0}, {2}}
            },
            {
                {{0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}, {2}, {0}, {0}, {0}},
                {{3}, {2}, {0}, {3}, {0}, {0}, {0}, {3}, {3}, {0}, {2}, {2}, {0}},
                /*12*/{{3}, {0}, {3}, {0}, {2}, {0}, {3}, {3}, {0}, {2}, {2}, {0}, {0}},
                {{0}, {0}, {2}, {0}, {0}, {0}, {0}, {2}, {0}, {0}, {0}, {2}, {0}}
            },
        };

        short centerX = MAXXKACH / 2 - 1;
        short centerY = MAXYKACH / 2 - 1;

        for (short m = 0; m < 1000; m++) // 100mal wiederholen, oder bis eine geeignete Insel gefunden ist
        {
            for (short y = 0; y < MAXYKACH; y++)
            {
                for (auto& x : Scape)
                {
                    x[y].Typ = 0;
                    x[y].Art = 0;
                    x[y].Hoehe = 0;
                    x[y].Markiert = false;
                    x[y].Begehbar = true;
                    x[y].Entdeckt = false;
                    x[y].LaufZeit = 1;
                    x[y].Objekt = -1;
                    x[y].Reverse = false;
                    x[y].ObPos.x = 0;
                    x[y].ObPos.y = 0;
                    x[y].Phase = -1;
                    x[y].AkNummer = 0;
                    x[y].GPosAlt.x = 0;
                    x[y].GPosAlt.y = 0;

                    for (i = 0; i < ImageCount; i++)
                    {
                        x[y].Rohstoff[i] = 0;
                    }
                    x[y].Timer = 0;
                }
            }

            short x; // Startposition der Berechnung
            short y;
            Scape[centerX][centerY].Typ = 0; // Gipfel festlegen (Flach)	
            Scape[centerX][centerY].Hoehe = GIPFEL; // und mit der Hoehe 

            // Spiralring von Innen aus gezählt
            for (short l = 0; l <= centerY - 1; l++)
            {
                if (l >= centerX)
                {
                    break;
                }
                y = centerY - l - 1;

                // Als erstes den oberen Bereich von links nach rechts durchgehen 
                for (x = centerX - l; x <= centerX + l; x++)
                {
                    found = false;
                    i = 0;
                    while (!found) // Passendes Teil finden und Hoehe festlegen
                    {
                        i += 1;
                        if (i == 1000)
                        {
                            found = true;
                        }

                        Scape[x][y].Typ = static_cast<short>(rand() % 13);
                        for (j = 0; j < 10; j++)
                        {
                            if (!(Scape[x][y].Typ == 0 || Scape[x][y].Typ == 1))
                            {
                                Scape[x][y].Typ = static_cast<short>(rand() % 13);
                            }
                        }

                        if (x == centerX - l || x != centerX - l
                            && (Vierecke[Scape[x][y + 1].Typ][1][Scape[x][y].Typ] != 0
                                && Vierecke[Scape[x - 1][y].Typ][2][Scape[x][y].Typ] != 0))
                        {
                            if (Vierecke[Scape[x][y + 1].Typ][1][Scape[x][y].Typ] == 1)
                            {
                                Scape[x][y].Hoehe = Scape[x][y + 1].Hoehe - 1;
                                if (Scape[x][y].Hoehe < 0)
                                {
                                    Scape[x][y].Typ = 0;
                                    Scape[x][y].Hoehe = 0;
                                }
                                found = true;
                            }
                            if (Vierecke[Scape[x][y + 1].Typ][1][Scape[x][y].Typ] == 2)
                            {
                                Scape[x][y].Hoehe = Scape[x][y + 1].Hoehe;
                                found = true;
                            }
                            if (Vierecke[Scape[x][y + 1].Typ][1][Scape[x][y].Typ] == 3)
                            {
                                Scape[x][y].Hoehe = Scape[x][y + 1].Hoehe + 1;
                                found = true;
                            }
                        }
                        // Verzwickte Fälle ausfiltern
                        if (Vierecke[Scape[x][y].Typ][2][3] == 2 && Vierecke[Scape[x + 1][y + 1].Typ][1][4] == 2 ||
                            Vierecke[Scape[x][y].Typ][2][1] == 2 && Vierecke[Scape[x + 1][y + 1].Typ][1][2] == 2)
                        {
                            found = false;
                        }
                        // Nebeninseln vermeiden
                        if (Scape[x - 1][y].Typ == 0 && Scape[x - 1][y].Hoehe == 0 &&
                            (Scape[x][y + 1].Typ == 0 && Scape[x][y + 1].Hoehe == 0))
                        {
                            Scape[x][y].Typ = 0;
                            Scape[x][y].Hoehe = 0;
                        }
                    }
                }

                // Teil rechts-oben  
                x = centerX + l + 1;
                y = centerY - l - 1;
                found = false;
                i = 0;

                while (!found) // Passendes Teil finden und Hoehe festlegen
                {
                    i += 1;
                    if (i == 1000)
                    {
                        found = true;
                    }

                    Scape[x][y].Typ = static_cast<short>(rand() % 13);

                    for (j = 0; j < 10; j++)
                    {
                        if (!(Scape[x][y].Typ == 0 || Scape[x][y].Typ == 5))
                        {
                            Scape[x][y].Typ = static_cast<short>(rand() % 13);
                        }
                    }

                    if (Vierecke[Scape[x - 1][y].Typ][2][Scape[x][y].Typ] == 1)
                    {
                        Scape[x][y].Hoehe = Scape[x - 1][y].Hoehe - 1;
                        if (Scape[x][y].Hoehe < 0)
                        {
                            Scape[x][y].Typ = 0;
                            Scape[x][y].Hoehe = 0;
                        }
                        found = true;
                    }
                    if (Vierecke[Scape[x - 1][y].Typ][2][Scape[x][y].Typ] == 2)
                    {
                        Scape[x][y].Hoehe = Scape[x - 1][y].Hoehe;
                        found = true;
                    }
                    if (Vierecke[Scape[x - 1][y].Typ][2][Scape[x][y].Typ] == 3)
                    {
                        Scape[x][y].Hoehe = Scape[x - 1][y].Hoehe + 1;
                        found = true;
                    }

                    // Verzwickte Fälle ausfiltern
                    if (Vierecke[Scape[x][y].Typ][3][2] == 2 && Vierecke[Scape[x - 1][y + 1].Typ][2][3] == 2 ||
                        Vierecke[Scape[x][y].Typ][3][4] == 2 && Vierecke[Scape[x - 1][y + 1].Typ][2][1] == 2)
                    {
                        found = false;
                    }

                    // Nebeninseln vermeiden
                    if (Scape[x - 1][y].Typ == 0 && Scape[x - 1][y].Hoehe == 0)
                    {
                        Scape[x][y].Typ = 0;
                        Scape[x][y].Hoehe = 0;
                    }
                }

                // Den rechten Bereich von oben nach unten durchgehen 
                x = centerX + l + 1;
                for (y = centerY - l; y <= centerY + l; y++)
                {
                    found = false;
                    i = 0;
                    while (!found) // Passendes Teil finden und Hoehe festlegen
                    {
                        i += 1;
                        if (i == 1000)
                        {
                            found = true;
                        }

                        Scape[x][y].Typ = static_cast<short>(rand() % 13);
                        for (j = 0; j < 10; j++)
                        {
                            if (!(Scape[x][y].Typ == 0 || Scape[x][y].Typ == 4))
                            {
                                Scape[x][y].Typ = static_cast<short>(rand() % 13);
                            }
                        }

                        if (Vierecke[Scape[x - 1][y].Typ][2][Scape[x][y].Typ] != 0
                            && Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] != 0)
                        {
                            if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] == 1)
                            {
                                Scape[x][y].Hoehe = Scape[x][y - 1].Hoehe - 1;

                                if (Scape[x][y].Hoehe < 0)
                                {
                                    Scape[x][y].Typ = 0;
                                    Scape[x][y].Hoehe = 0;
                                }
                                found = true;
                            }
                            if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] == 2)
                            {
                                Scape[x][y].Hoehe = Scape[x][y - 1].Hoehe;
                                found = true;
                            }
                            if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] == 3)
                            {
                                Scape[x][y].Hoehe = Scape[x][y - 1].Hoehe + 1;
                                found = true;
                            }
                        }

                        // Verzwickte Fälle ausfiltern
                        if (Vierecke[Scape[x][y].Typ][3][2] == 2 && Vierecke[Scape[x - 1][y + 1].Typ][2][3] == 2 ||
                            Vierecke[Scape[x][y].Typ][3][4] == 2 && Vierecke[Scape[x - 1][y + 1].Typ][2][1] == 2)
                        {
                            found = false;
                        }

                        // Nebeninseln vermeiden
                        if (Scape[x - 1][y].Typ == 0 && Scape[x - 1][y].Hoehe == 0 &&
                            (Scape[x][y - 1].Typ == 0 && Scape[x][y - 1].Hoehe == 0))
                        {
                            Scape[x][y].Typ = 0;
                            Scape[x][y].Hoehe = 0;
                        }
                    }
                }

                // Teil rechts-unten  
                x = centerX + l + 1;
                y = centerY + l + 1;
                found = false;
                i = 0;

                while (!found) // Passendes Teil finden und Hoehe festlegen
                {
                    i += 1;
                    if (i == 1000)
                    {
                        found = true;
                    }

                    Scape[x][y].Typ = static_cast<short>(rand() % 13);

                    for (j = 0; j < 10; j++)
                    {
                        if (!(Scape[x][y].Typ == 0 || Scape[x][y].Typ == 8))
                        {
                            Scape[x][y].Typ = static_cast<short>(rand() % 13);
                        }
                    }

                    if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] == 1)
                    {
                        Scape[x][y].Hoehe = Scape[x][y - 1].Hoehe - 1;
                        if (Scape[x][y].Hoehe < 0)
                        {
                            Scape[x][y].Typ = 0;
                            Scape[x][y].Hoehe = 0;
                        }
                        found = true;
                    }
                    if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] == 2)
                    {
                        Scape[x][y].Hoehe = Scape[x][y - 1].Hoehe;
                        found = true;
                    }
                    if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] == 3)
                    {
                        Scape[x][y].Hoehe = Scape[x][y - 1].Hoehe + 1;
                        found = true;
                    }

                    // Verzwickte Fälle ausfiltern
                    if (Vierecke[Scape[x][y].Typ][0][1] == 2 && Vierecke[Scape[x - 1][y - 1].Typ][3][2] == 2 ||
                        Vierecke[Scape[x][y].Typ][0][3] == 2 && Vierecke[Scape[x - 1][y - 1].Typ][3][4] == 2)
                    {
                        found = false;
                    }

                    // Nebeninsel vermeiden
                    if (Scape[x][y - 1].Typ == 0 && Scape[x][y - 1].Hoehe == 0)
                    {
                        Scape[x][y].Typ = 0;
                        Scape[x][y].Hoehe = 0;
                    }
                }

                // Den unteren Bereich von rechts nach links durchgehen 
                y = centerY + l + 1;
                for (x = centerX + l; x >= centerX - l; x--)
                {
                    found = false;
                    i = 0;

                    while (!found) // Passendes Teil finden und Hoehe festlegen
                    {
                        i += 1;
                        if (i == 1000)
                        {
                            found = true;
                        }

                        Scape[x][y].Typ = static_cast<short>(rand() % 13);

                        for (j = 0; j < 10; j++)
                        {
                            if (!(Scape[x][y].Typ == 0 || Scape[x][y].Typ == 3))
                            {
                                Scape[x][y].Typ = static_cast<short>(rand() % 13);
                            }
                        }

                        if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] != 0
                            && Vierecke[Scape[x + 1][y].Typ][0][Scape[x][y].Typ] != 0)
                        {
                            if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] == 1)
                            {
                                Scape[x][y].Hoehe = Scape[x][y - 1].Hoehe - 1;
                                if (Scape[x][y].Hoehe < 0)
                                {
                                    Scape[x][y].Typ = 0;
                                    Scape[x][y].Hoehe = 0;
                                }
                                found = true;
                            }
                            if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] == 2)
                            {
                                Scape[x][y].Hoehe = Scape[x][y - 1].Hoehe;
                                found = true;
                            }
                            if (Vierecke[Scape[x][y - 1].Typ][3][Scape[x][y].Typ] == 3)
                            {
                                Scape[x][y].Hoehe = Scape[x][y - 1].Hoehe + 1;
                                found = true;
                            }
                        }

                        // Verzwickte Fälle ausfiltern
                        if (Vierecke[Scape[x][y].Typ][0][1] == 2 && Vierecke[Scape[x - 1][y - 1].Typ][3][2] == 2
                            || Vierecke[Scape[x][y].Typ][0][3] == 2 && Vierecke[Scape[x - 1][y - 1].Typ][3][4] == 2)
                        {
                            found = false;
                        }

                        // Nebeninseln vermeiden
                        if (Scape[x + 1][y].Typ == 0 && Scape[x + 1][y].Hoehe == 0
                            && (Scape[x][y - 1].Typ == 0 && Scape[x][y - 1].Hoehe == 0))
                        {
                            Scape[x][y].Typ = 0;
                            Scape[x][y].Hoehe = 0;
                        }
                    }
                }

                // Teil links-unten
                x = centerX - l - 1;
                y = centerY + l + 1;
                found = false;
                i = 0;
                while (!found) // Passendes Teil finden und Hoehe festlegen
                {
                    i += 1;
                    if (i == 1000)
                    {
                        found = true;
                    }

                    Scape[x][y].Typ = static_cast<short>(rand() % 13);
                    for (j = 0; j < 10; j++)
                    {
                        if (!(Scape[x][y].Typ == 0 || Scape[x][y].Typ == 7))
                        {
                            Scape[x][y].Typ = static_cast<short>(rand() % 13);
                        }
                    }

                    if (Vierecke[Scape[x + 1][y].Typ][0][Scape[x][y].Typ] == 1)
                    {
                        Scape[x][y].Hoehe = Scape[x + 1][y].Hoehe - 1;
                        if (Scape[x][y].Hoehe < 0)
                        {
                            Scape[x][y].Typ = 0;
                            Scape[x][y].Hoehe = 0;
                        }
                        found = true;
                    }
                    if (Vierecke[Scape[x + 1][y].Typ][0][Scape[x][y].Typ] == 2)
                    {
                        Scape[x][y].Hoehe = Scape[x + 1][y].Hoehe;
                        found = true;
                    }
                    if (Vierecke[Scape[x + 1][y].Typ][0][Scape[x][y].Typ] == 3)
                    {
                        Scape[x][y].Hoehe = Scape[x + 1][y].Hoehe + 1;
                        found = true;
                    }

                    // Verzwickte Fälle ausfiltern
                    if (Vierecke[Scape[x][y].Typ][1][2] == 2 && Vierecke[Scape[x + 1][y - 1].Typ][0][3] == 2
                        || Vierecke[Scape[x][y].Typ][1][4] == 2 && Vierecke[Scape[x + 1][y - 1].Typ][0][1] == 2)
                    {
                        found = false;
                    }

                    // Nebeninsel vermeiden
                    if (Scape[x + 1][y].Typ == 0 && Scape[x + 1][y].Hoehe == 0)
                    {
                        Scape[x][y].Typ = 0;
                        Scape[x][y].Hoehe = 0;
                    }
                }

                // Den linken Bereich von unten nach oben durchgehen 
                x = centerX - l - 1;
                for (y = centerY + l; y >= centerY - l - 1; y--)
                {
                    found = false;
                    i = 0;
                    while (!found) // Passendes Teil finden und Hoehe festlegen
                    {
                        i += 1;
                        if (i == 1000)
                        {
                            found = true;
                        }
                        Scape[x][y].Typ = static_cast<short>(rand() % 13);
                        for (j = 0; j < 10; j++)
                        {
                            if (!(Scape[x][y].Typ == 0 || Scape[x][y].Typ == 2))
                            {
                                Scape[x][y].Typ = static_cast<short>(rand() % 13);
                            }
                        }

                        if (Vierecke[Scape[x + 1][y].Typ][0][Scape[x][y].Typ] != 0
                            && Vierecke[Scape[x][y + 1].Typ][1][Scape[x][y].Typ] != 0)
                        {
                            if (Vierecke[Scape[x][y + 1].Typ][1][Scape[x][y].Typ] == 1)
                            {
                                Scape[x][y].Hoehe = Scape[x][y + 1].Hoehe - 1;
                                if (Scape[x][y].Hoehe < 0)
                                {
                                    Scape[x][y].Typ = 0;
                                    Scape[x][y].Hoehe = 0;
                                }
                                found = true;
                            }
                            if (Vierecke[Scape[x][y + 1].Typ][1][Scape[x][y].Typ] == 2)
                            {
                                Scape[x][y].Hoehe = Scape[x][y + 1].Hoehe;
                                found = true;
                            }
                            if (Vierecke[Scape[x][y + 1].Typ][1][Scape[x][y].Typ] == 3)
                            {
                                Scape[x][y].Hoehe = Scape[x][y + 1].Hoehe + 1;
                                found = true;
                            }
                        }

                        // Verzwickte Fälle ausfiltern
                        if (Vierecke[Scape[x][y].Typ][1][2] == 2 && Vierecke[Scape[x + 1][y - 1].Typ][0][3] == 2
                            || Vierecke[Scape[x][y].Typ][1][4] == 2 && Vierecke[Scape[x + 1][y - 1].Typ][0][1] == 2)
                        {
                            found = false;
                        }

                        // Nebeninseln vermeiden
                        if (Scape[x + 1][y].Typ == 0 && Scape[x + 1][y].Hoehe == 0
                            && Scape[x][y + 1].Typ == 0 && Scape[x][y + 1].Hoehe == 0)
                        {
                            Scape[x][y].Typ = 0;
                            Scape[x][y].Hoehe = 0;
                        }
                    }
                }
            }
            short amount = 0; // Anzahl der Landstücke
            auto checkRand = true; // Reicht die Insel bis zum Rand?
            for (y = 0; y < MAXYKACH; y++) // Landfläche zählen
            {
                for (x = 0; x < MAXXKACH; x++)
                {
                    if (Scape[x][y].Hoehe > 0)
                    {
                        amount++;
                    }

                    if (Scape[x][y].Typ == 0)
                    {
                        Scape[x][y].LaufZeit = 1;
                    }
                    else
                    {
                        Scape[x][y].LaufZeit = 2;
                    }

                    if (Scape[x][y].Typ != 0 && (x <= 2 || x >= MAXXKACH - 2 || y <= 2 || y >= MAXYKACH - 2))
                    {
                        checkRand = false;
                    }
                }
            }
            if (amount > minSize && amount < maxSize && checkRand)
            {
                break;
            }
        }
    }

    void Ocean() // Das Meer und den Strand bestimmen
    {
        short x, y;

        for (y = 0; y < MAXYKACH; y++) // Meer rausfinden
        {
            for (x = 0; x < MAXXKACH; x++)
            {
                if (Scape[x][y].Hoehe < 0 || Scape[x][y].Hoehe == 0 && Scape[x][y].Typ == 0)
                {
                    Scape[x][y].Typ = 0;
                    Scape[x][y].Hoehe = 0;
                    Scape[x][y].Art = 1;
                    Scape[x][y].Objekt = Meerwellen;
                    Scape[x][y].ObPos.x = static_cast<short>(Bmp[Meerwellen].rcDes.left);
                    Scape[x][y].ObPos.y = static_cast<short>(Bmp[Meerwellen].rcDes.top);
                    if (rand() % 2 == 0)
                    {
                        Scape[x][y].Reverse = true;
                    }
                    Scape[x][y].Begehbar = false;
                    Scape[x][y].Phase = static_cast<float>(Bmp[Scape[x][y].Objekt].Anzahl -
                                                           rand() % Bmp[Scape[x][y].Objekt].Anzahl - 1);
                }
            }
        }
        for (y = 1; y < MAXYKACH - 1; y++) // Strand rausfinden
        {
            for (x = 1; x < MAXXKACH - 1; x++) // Alle Möglichkeiten durchgehen
            {
                if (Scape[x][y].Typ == 0 && Scape[x][y].Hoehe == 0)
                {
                    short Anzahl = 0; // Anzahl von angrenzenden Landstücken rausfinden
                    if (Scape[x - 1][y].Typ != 0)
                    {
                        Anzahl++;
                    }
                    if (Scape[x - 1][y - 1].Typ != 0)
                    {
                        Anzahl++;
                    }
                    if (Scape[x][y - 1].Typ != 0)
                    {
                        Anzahl++;
                    }
                    if (Scape[x + 1][y - 1].Typ != 0)
                    {
                        Anzahl++;
                    }
                    if (Scape[x + 1][y].Typ != 0)
                    {
                        Anzahl++;
                    }
                    if (Scape[x + 1][y + 1].Typ != 0)
                    {
                        Anzahl++;
                    }
                    if (Scape[x][y + 1].Typ != 0)
                    {
                        Anzahl++;
                    }
                    if (Scape[x - 1][y + 1].Typ != 0)
                    {
                        Anzahl++;
                    }

                    if (Anzahl >= 1 && Anzahl < 6)
                    {
                        Scape[x][y].Art = 2;
                        Scape[x][y].Objekt = -1;
                        Scape[x][y].Reverse = false;
                        Scape[x][y].Begehbar = true;
                        Scape[x][y].Phase = -1;
                        continue;
                    }
                    if (Anzahl >= 6)
                    {
                        Scape[x][y].Art = 3;
                        Scape[x][y].Objekt = -1;
                        Scape[x][y].Reverse = false;
                        Scape[x][y].Begehbar = false;
                        Scape[x][y].Phase = -1;
                        continue;
                    }
                    Scape[x][y].Art = 1; // sonst Meer
                    Scape[x][y].Objekt = Meerwellen;
                    Scape[x][y].ObPos.x = static_cast<short>(Bmp[Meerwellen].rcDes.left);
                    Scape[x][y].ObPos.y = static_cast<short>(Bmp[Meerwellen].rcDes.top);
                    Scape[x][y].Phase = static_cast<float>(Bmp[Scape[x][y].Objekt].Anzahl -
                                                           rand() % Bmp[Scape[x][y].Objekt].Anzahl - 1);
                    if (rand() % 2 == 0)
                    {
                        Scape[x][y].Reverse = true;
                    }
                    Scape[x][y].Begehbar = false;
                }
            }
        }
    }

    void ChangeBoatRide()
    {
        BootsFahrt = !BootsFahrt;
        // Begehbarkeit umdrehen
        for (short y = 0; y < MAXYKACH; y++)
        {
            for (auto& x : Scape)
            {
                x[y].Begehbar = !x[y].Begehbar;
            }
        }
    }

    void CheckPipe(const short x, const short y)
    {
        Scape[x][y].Phase = 1;
        if (Scape[x][y].Art == 0)
        {
            Scape[x][y].Art = 4;
        }
        if (Scape[x - 1][y].Art == 0)
        {
            Scape[x - 1][y].Art = 4;
        }
        if (Scape[x - 1][y - 1].Art == 0)
        {
            Scape[x - 1][y - 1].Art = 4;
        }
        if (Scape[x][y - 1].Art == 0)
        {
            Scape[x][y - 1].Art = 4;
        }
        if (Scape[x + 1][y - 1].Art == 0)
        {
            Scape[x + 1][y - 1].Art = 4;
        }
        if (Scape[x + 1][y].Art == 0)
        {
            Scape[x + 1][y].Art = 4;
        }
        if (Scape[x + 1][y + 1].Art == 0)
        {
            Scape[x + 1][y + 1].Art = 4;
        }
        if (Scape[x][y + 1].Art == 0)
        {
            Scape[x][y + 1].Art = 4;
        }
        if (Scape[x - 1][y + 1].Art == 0)
        {
            Scape[x - 1][y + 1].Art = 4;
        }

        if (Scape[x - 1][y].Objekt == Rohr && Scape[x - 1][y].Phase == 0)
        {
            CheckPipe(x - 1, y);
        }
        if (Scape[x][y - 1].Objekt == Rohr && Scape[x][y - 1].Phase == 0)
        {
            CheckPipe(x, y - 1);
        }
        if (Scape[x + 1][y].Objekt == Rohr && Scape[x + 1][y].Phase == 0)
        {
            CheckPipe(x + 1, y);
        }
        if (Scape[x][y + 1].Objekt == Rohr && Scape[x][y + 1].Phase == 0)
        {
            CheckPipe(x, y + 1);
        }
    }

    void FillPipe()
    {
        for (short y = 0; y < MAXYKACH; y++)
        {
            for (auto& x : Scape)
            {
                if (x[y].Objekt == Rohr && x[y].Phase < Bmp[Rohr].Anzahl)
                {
                    x[y].Phase = 0;
                }
                if (x[y].Art == 4)
                {
                    x[y].Art = 0;
                }
                if (x[y].Objekt >= Schleuse1 && x[y].Objekt <= Schleuse6)
                {
                    x[y].Objekt -= 14;
                    x[y].ObPos.x = static_cast<short>(Bmp[x[y].Objekt].rcDes.left);
                    x[y].ObPos.y = static_cast<short>(Bmp[x[y].Objekt].rcDes.top);
                }
            }
        }

        // StartRohr finden
        for (short y = 0; y < MAXYKACH; y++)
        {
            for (short x = 0; x < MAXXKACH; x++)
            {
                if (Scape[x][y].Objekt >= Fluss1 && Scape[x][y].Objekt <= Schleuse6)
                {
                    if (Scape[x][y].Art == 0)
                    {
                        Scape[x][y].Art = 4;
                    }
                    if (Scape[x - 1][y].Art == 0)
                    {
                        Scape[x - 1][y].Art = 4;
                    }
                    if (Scape[x - 1][y - 1].Art == 0)
                    {
                        Scape[x - 1][y - 1].Art = 4;
                    }
                    if (Scape[x][y - 1].Art == 0)
                    {
                        Scape[x][y - 1].Art = 4;
                    }
                    if (Scape[x + 1][y - 1].Art == 0)
                    {
                        Scape[x + 1][y - 1].Art = 4;
                    }
                    if (Scape[x + 1][y].Art == 0)
                    {
                        Scape[x + 1][y].Art = 4;
                    }
                    if (Scape[x + 1][y + 1].Art == 0)
                    {
                        Scape[x + 1][y + 1].Art = 4;
                    }
                    if (Scape[x][y + 1].Art == 0)
                    {
                        Scape[x][y + 1].Art = 4;
                    }
                    if (Scape[x - 1][y + 1].Art == 0)
                    {
                        Scape[x - 1][y + 1].Art = 4;
                    }
                }
                if (Scape[x][y].Objekt != Rohr || Scape[x][y].Phase >= Bmp[Rohr].Anzahl)
                {
                    continue;
                }
                if (Scape[x - 1][y].Objekt >= Fluss5 && Scape[x - 1][y].Objekt <= Fluss10)
                {
                    Scape[x - 1][y].Objekt += 14;
                    Scape[x - 1][y].ObPos.x = static_cast<short>(Bmp[Scape[x - 1][y].Objekt].rcDes.left);
                    Scape[x - 1][y].ObPos.y = static_cast<short>(Bmp[Scape[x - 1][y].Objekt].rcDes.top);
                    CheckPipe(x, y);
                }
                else if (Scape[x - 1][y].Objekt >= Schleuse1 && Scape[x - 1][y].Objekt <= Schleuse6)
                {
                    CheckPipe(x, y);
                }
                if (Scape[x][y - 1].Objekt >= Fluss5 && Scape[x][y - 1].Objekt <= Fluss10)
                {
                    Scape[x][y - 1].Objekt += 14;
                    Scape[x][y - 1].ObPos.x = static_cast<short>(Bmp[Scape[x][y - 1].Objekt].rcDes.left);
                    Scape[x][y - 1].ObPos.y = static_cast<short>(Bmp[Scape[x][y - 1].Objekt].rcDes.top);
                    CheckPipe(x, y);
                }
                else if (Scape[x][y - 1].Objekt >= Schleuse1 && Scape[x][y - 1].Objekt <= Schleuse6)
                {
                    CheckPipe(x, y);
                }
                if (Scape[x + 1][y].Objekt >= Fluss5 && Scape[x + 1][y].Objekt <= Fluss10)
                {
                    Scape[x + 1][y].Objekt += 14;
                    Scape[x + 1][y].ObPos.x = static_cast<short>(Bmp[Scape[x + 1][y].Objekt].rcDes.left);
                    Scape[x + 1][y].ObPos.y = static_cast<short>(Bmp[Scape[x + 1][y].Objekt].rcDes.top);
                    CheckPipe(x, y);
                }
                else if (Scape[x + 1][y].Objekt >= Schleuse1 && Scape[x + 1][y].Objekt <= Schleuse6)
                {
                    CheckPipe(x, y);
                }
                if (Scape[x][y + 1].Objekt >= Fluss5 && Scape[x][y + 1].Objekt <= Fluss10)
                {
                    Scape[x][y + 1].Objekt += 14;
                    Scape[x][y + 1].ObPos.x = static_cast<short>(Bmp[Scape[x][y + 1].Objekt].rcDes.left);
                    Scape[x][y + 1].ObPos.y = static_cast<short>(Bmp[Scape[x][y + 1].Objekt].rcDes.top);
                    CheckPipe(x, y);
                }
                else if (Scape[x][y + 1].Objekt >= Schleuse1 && Scape[x][y + 1].Objekt <= Schleuse6)
                {
                    CheckPipe(x, y);
                }
            }
        }

        // Felder auf trockenen Wiesen löschen
        for (short y = 0; y < MAXYKACH; y++)
        {
            for (auto& x : Scape)
            {
                if (x[y].Objekt == Feld && x[y].Art == 0)
                {
                    x[y].Objekt = -1;
                    x[y].ObPos.x = 0;
                    x[y].ObPos.y = 0;
                    x[y].Phase = -1;
                    x[y].AkNummer = 0;
                }
            }
        }
        Generate();
    }

    bool CheckRiver(const short x, const short y) // Nachprüfen ob auf aktuellem Teil ein Fluss ist (Nur für die Fluss-Routine)
    {
        for (short i = 0; i < FLUSSANZAHL; i++)
        {
            for (short j = 0; j < MAXFLUSS; j++)
            {
                if (x == RiverCourse[i][j].x && y == RiverCourse[i][j].y)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void River() // Anzahl der Flüsse und der minimale Länge
    {
        short i;
        short l = 0;
        short direction = 0; // Aktuelle Fliesrichtung von 0-3
        short x0 = 0;
        short y0 = 0;
        short x1 = 0;
        short y1 = 0;
        short x2 = 0; // x2,y2 Koordinate des zu prüfenden Teils
        short y2 = 0;
        auto found = false;
        FLUSSLAUF riverTemp[MAXFLUSS]; // Zum zwischenspeichern des Versuchs
        short riverLength[FLUSSANZAHL];

        for (auto& m : RiverCourse)
        {
            for (auto& j : m)
            {
                j.x = -1;
                j.y = -1;
            }
        }

        for (short m = 0; m < FLUSSANZAHL; m++)
        {
            auto done = false;
            short lengthTemp = 0;

            for (auto& j : riverTemp)
            {
                j.x = -1;
                j.y = -1;
            }
            for (short k = 0; k < 1000; k++)
            {
                for (short o = 0; o < 10000; o++)
                {
                    found = true;

                    x0 = static_cast<short>(rand() % MAXXKACH); // geeignete Quelle bestimmen
                    y0 = static_cast<short>(rand() % MAXYKACH);
                    if (CheckRiver(x0, y0))
                    {
                        found = false;
                    }
                    if (Scape[x0][y0].Typ != 0 || Scape[x0][y0].Hoehe < 2)
                    {
                        found = false;
                    }
                    if (found)
                    {
                        break;
                    }
                }
                if (!found)
                {
                    MessageBeep(MB_OK);
                    break;
                } // Wenn keine Quelle mehr gefunden aufhören
                RiverCourse[m][0].x = x0;
                RiverCourse[m][0].y = y0;
                auto strand = false;

                for (i = 1; i < MAXFLUSS; i++)
                {
                    found = false;
                    if (!strand)
                    {
                        direction = static_cast<short>(rand() % 4); // Auf dem Strand geradeausfliessen
                    }
                    for (short l2 = 0; l2 < 4; l2++)
                    {
                        l = static_cast<short>((direction + l2) % 4); // Im Urzeigersinn nachprüfen und bei der vorgegeben Richtung anfangen

                        x1 = RiverCourse[m][i - 1].x;
                        y1 = RiverCourse[m][i - 1].y;

                        if (l == 0)
                        {
                            x2 = x1 - 1;
                            y2 = y1;
                            if ((Scape[x1][y1].Typ == 0 || Scape[x1][y1].Typ == 2)
                                && (Scape[x2][y2].Typ == 0 || Scape[x2][y2].Typ == 2))
                            {
                                found = !CheckRiver(x2, y2);
                                if (found)
                                {
                                    break;
                                }
                            }
                        }
                        if (l == 1)
                        {
                            x2 = x1;
                            y2 = y1 - 1;
                            if ((Scape[x1][y1].Typ == 0 || Scape[x1][y1].Typ == 1)
                                && (Scape[x2][y2].Typ == 0 || Scape[x2][y2].Typ == 1))
                            {
                                found = !CheckRiver(x2, y2);
                                if (found)
                                {
                                    break;
                                }
                            }
                        }
                        if (l == 2)
                        {
                            x2 = x1 + 1;
                            y2 = y1;
                            if ((Scape[x1][y1].Typ == 0 || Scape[x1][y1].Typ == 4)
                                && (Scape[x2][y2].Typ == 0 || Scape[x2][y2].Typ == 4))
                            {
                                found = !CheckRiver(x2, y2);
                                if (found)
                                {
                                    break;
                                }
                            }
                        }
                        if (l == 3)
                        {
                            x2 = x1;
                            y2 = y1 + 1;
                            if ((Scape[x1][y1].Typ == 0 || Scape[x1][y1].Typ == 3)
                                && (Scape[x2][y2].Typ == 0 || Scape[x2][y2].Typ == 3))
                            {
                                found = !CheckRiver(x2, y2);
                                if (found)
                                {
                                    break;
                                }
                            }
                        }
                    }
                    if (!found)
                        break;

                    RiverCourse[m][i].x = x2;
                    RiverCourse[m][i].y = y2;
                    direction = l;

                    if (Scape[RiverCourse[m][i].x][RiverCourse[m][i].y].Art == 2 // Auf Strand die Richtung beibehalten
                        && Scape[RiverCourse[m][i].x][RiverCourse[m][i].y].Typ == 0)
                    {
                        if (strand == true)
                        {
                            break; // Nur ein Strandstück überfliessen
                        }
                        strand = true;
                    }

                    if (Scape[RiverCourse[m][i].x][RiverCourse[m][i].y].Art == 1 // im meer aufhören
                        && Scape[RiverCourse[m][i].x][RiverCourse[m][i].y].Typ == 0)
                    {
                        done = true;
                        break;
                    }
                }
                if (done)
                {
                    if (i > lengthTemp)
                    {
                        // neue Variante speichern
                        if (found)
                        {
                            lengthTemp = i;
                        }
                        else
                        {
                            lengthTemp = i - 1;
                        }

                        for (short j = 0; j <= lengthTemp; j++)
                        {
                            riverTemp[j].x = RiverCourse[m][j].x;
                            riverTemp[j].y = RiverCourse[m][j].y;
                        }
                    }
                }
                for (i = 0; i < MAXFLUSS; i++)
                {
                    RiverCourse[m][i].x = -1;
                    RiverCourse[m][i].y = -1;
                }
                done = false;
            }
            // Den längsten Fluss nehmen
            riverLength[m] = lengthTemp;
            for (short j = 0; j <= riverLength[m]; j++)
            {
                RiverCourse[m][j].x = riverTemp[j].x;
                RiverCourse[m][j].y = riverTemp[j].y;
            }

            // Die richtigen Wasserkacheln auswählen
            x0, y0, x1, y1, x2, y2 = -1;
            for (m = 0; m < FLUSSANZAHL; m++)
            {
                for (i = 0; i <= riverLength[m]; i++)
                {
                    // Für die Kachel, einen Vorgang davor
                    Scape[x1][y1].ObPos.x = static_cast<short>(Bmp[Scape[x1][y1].Objekt].rcDes.left);
                    Scape[x1][y1].ObPos.y = static_cast<short>(Bmp[Scape[x1][y1].Objekt].rcDes.top);

                    x1 = RiverCourse[m][i].x;
                    y1 = RiverCourse[m][i].y;

                    Scape[x1][y1].Phase = 0;

                    if (Scape[x1][y1].Art == 0)
                    {
                        Scape[x1][y1].Art = 4;
                    }
                    if (Scape[x1 - 1][y1].Art == 0)
                    {
                        Scape[x1 - 1][y1].Art = 4;
                    }
                    if (Scape[x1 - 1][y1 - 1].Art == 0)
                    {
                        Scape[x1 - 1][y1 - 1].Art = 4;
                    }
                    if (Scape[x1][y1 - 1].Art == 0)
                    {
                        Scape[x1][y1 - 1].Art = 4;
                    }
                    if (Scape[x1 + 1][y1 - 1].Art == 0)
                    {
                        Scape[x1 + 1][y1 - 1].Art = 4;
                    }
                    if (Scape[x1 + 1][y1].Art == 0)
                    {
                        Scape[x1 + 1][y1].Art = 4;
                    }
                    if (Scape[x1 + 1][y1 + 1].Art == 0)
                    {
                        Scape[x1 + 1][y1 + 1].Art = 4;
                    }
                    if (Scape[x1][y1 + 1].Art == 0)
                    {
                        Scape[x1][y1 + 1].Art = 4;
                    }
                    if (Scape[x1 - 1][y1 + 1].Art == 0)
                    {
                        Scape[x1 - 1][y1 + 1].Art = 4;
                    }

                    if (i < riverLength[m])
                    {
                        x2 = RiverCourse[m][i + 1].x;
                        y2 = RiverCourse[m][i + 1].y;
                    }

                    if (i != 0)
                    {
                        x0 = RiverCourse[m][i - 1].x;
                        y0 = RiverCourse[m][i - 1].y;
                    }
                    else
                    {
                        // Quellen
                        if (x2 > x1)
                        {
                            Scape[x1][y1].Objekt = Quelle1;
                            continue;
                        }
                        if (x2 < x1)
                        {
                            Scape[x1][y1].Objekt = Quelle4;
                            continue;
                        }
                        if (y2 > y1)
                        {
                            Scape[x1][y1].Objekt = Quelle2;
                            continue;
                        }
                        if (y2 < y1)
                        {
                            Scape[x1][y1].Objekt = Quelle3;
                            continue;
                        }
                    }

                    // Alle Möglichkeiten durchgehen

                    if (Scape[x1][y1].Typ == 1)
                    {
                        Scape[x1][y1].Objekt = Fluss1;
                    }
                    if (Scape[x1][y1].Typ == 2)
                    {
                        Scape[x1][y1].Objekt = Fluss2;
                    }
                    if (Scape[x1][y1].Typ == 3)
                    {
                        Scape[x1][y1].Objekt = Fluss3;
                    }
                    if (Scape[x1][y1].Typ == 4)
                    {
                        Scape[x1][y1].Objekt = Fluss4;
                    }

                    if (Scape[x1][y1].Typ == 0)
                    {
                        if (x0 < x1 && y0 == y1)
                        {
                            if (Scape[x1][y1].Art == 2)
                            {
                                Scape[x1][y1].Objekt = Muendung3; // Mündung
                            }
                            else
                            {
                                if (x1 < x2 && y1 == y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss5;
                                }
                                if (x1 == x2 && y1 < y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss7;
                                }
                                if (x1 == x2 && y1 > y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss9;
                                }
                            }
                        }
                        if (x0 == x1 && y0 < y1)
                        {
                            if (Scape[x1][y1].Art == 2)
                            {
                                Scape[x1][y1].Objekt = Muendung4; // Mündung
                            }
                            else
                            {
                                if (x1 < x2 && y1 == y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss8;
                                }
                                if (x1 == x2 && y1 < y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss6;
                                }
                                if (x1 > x2 && y1 == y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss9;
                                    Scape[x1][y1].Reverse = true;
                                }
                            }
                        }
                        if (x0 > x1 && y0 == y1)
                        {
                            if (Scape[x1][y1].Art == 2)
                            {
                                Scape[x1][y1].Objekt = Muendung1; // Mündung
                            }
                            else
                            {
                                if (x1 > x2 && y1 == y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss5;
                                }
                                if (x1 == x2 && y1 < y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss10;
                                }
                                if (x1 == x2 && y1 > y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss8;
                                }
                                Scape[x1][y1].Reverse = true;
                            }
                        }
                        if (x0 == x1 && y0 > y1)
                        {
                            if (Scape[x1][y1].Art == 2)
                            {
                                Scape[x1][y1].Objekt = Muendung2; // Mündung
                            }
                            else
                            {
                                if (x1 == x2 && y1 > y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss6;
                                }

                                if (x1 > x2 && y1 == y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss7;
                                }

                                Scape[x1][y1].Reverse = true;
                                if (x1 < x2 && y1 == y2)
                                {
                                    Scape[x1][y1].Objekt = Fluss10;
                                    Scape[x1][y1].Reverse = false;
                                }
                            }
                        }
                    }
                }
                // Für das letzte Flussstück
                Scape[x1][y1].ObPos.x = static_cast<short>(Bmp[Scape[x1][y1].Objekt].rcDes.left);
                Scape[x1][y1].ObPos.y = static_cast<short>(Bmp[Scape[x1][y1].Objekt].rcDes.top);
            }
        }
    }

    void Trees(const short percent)
    {
        ZWEID Pos; // Da steht der Baum
        auto largeTreeExists = false; // gibt es bereits einen großen Baum

        for (short y = 0; y < MAXYKACH; y++) //Alle Kacheln durchgehen
        {
            for (short x = 0; x < MAXXKACH; x++)
            {
                if (Scape[x][y].Objekt != -1 || Scape[x][y].Art == 3 && Scape[x][y].Typ == 0)
                {
                    continue;
                }

                // Wenn schon ein Objekt da ist oder Treibsand ist, dann mit nächsten Teil weitermachen
                if (rand() % (100 / percent) != 0) // Die Wahrscheinlichkeit für einen Baum bestimmen
                {
                    continue;
                }

                while (true)
                {
                    Pos.x = static_cast<short>(rand() % KXPIXEL);
                    Pos.y = static_cast<short>(rand() % KYPIXEL);
                    ZWEID Erg = Renderer::GetTile(Scape[x][y].xScreen + Pos.x, Scape[x][y].yScreen + Pos.y);

                    if (Erg.x == x && Erg.y == y)
                    {
                        break;
                    }
                }
                if (Scape[x][y].Art == 2 && Scape[x][y].Typ == 0) // Bei Strand nur Palmen nehmen
                {
                    Scape[x][y].Objekt = Baum2;
                }
                else
                {
                    auto r = static_cast<short>(rand() % 5); // random speicherung
                    Scape[x][y].Objekt = Baum1 + r;

                    if (rand() % 50 == 1 || !largeTreeExists)
                    {
                        Scape[x][y].Objekt = BaumGross;
                        largeTreeExists = true;
                    }
                }
                // Linke obere Ecke speichern
                Scape[x][y].ObPos.x = Pos.x - static_cast<short>(Bmp[Scape[x][y].Objekt].Breite) / 2;
                Scape[x][y].ObPos.y = Pos.y - static_cast<short>(Bmp[Scape[x][y].Objekt].Hoehe);
                // Startphase
                if (Scape[x][y].Objekt == Busch)
                {
                    Scape[x][y].Phase = static_cast<float>(Bmp[Scape[x][y].Objekt].Anzahl) - 1;
                }
                else
                {
                    Scape[x][y].Phase = static_cast<float>(Bmp[Scape[x][y].Objekt].Anzahl -
                                                           rand() % Bmp[Scape[x][y].Objekt].Anzahl - 1);
                }
            }
        }
    }

    void PirateWreck()
    {
        short x = 0;
        short y = 0;

        const auto direction = static_cast<short>(rand() % 3);
        switch (direction)
        {
        case 0:
            x = MAXXKACH / 2;
            for (short i = 0; i < MAXYKACH; i++)
            {
                if (Scape[x][i].Art != 1)
                {
                    y = i - 1;
                    break;
                }
            }
            break;
        case 1:
            y = MAXYKACH / 2;
            for (short i = MAXXKACH - 1; i >= 0; i--)
            {
                if (Scape[i][y].Art != 1)
                {
                    x = i + 1;
                    break;
                }
            }
            break;
        case 2:
            x = MAXXKACH / 2;
            for (short i = MAXYKACH - 1; i >= 0; i--)
            {
                if (Scape[x][i].Art != 1)
                {
                    y = i + 1;
                    break;
                }
            }
            break;
        default:
            break;
        }
        Scape[x][y].Objekt = Wrack2;
        Scape[x][y].ObPos.x = static_cast<short>(Bmp[Wrack2].rcDes.left);
        Scape[x][y].ObPos.y = static_cast<short>(Bmp[Wrack2].rcDes.top);
    }

    void Treasure()
    {
        while (true)
        {
            // Diese Kachel wird angeschaut
            short x = rand() % (MAXXKACH - 1);
            short y = rand() % (MAXYKACH - 1);

            // nur auf flachen Kacheln ohne Objekt
            if (Scape[x][y].Objekt == -1 && Scape[x][y].Typ == 0 && Scape[x][y].Art != 3)
            {
                if (SchatzPos.x == -1)
                {
                    SchatzPos.x = x;
                    SchatzPos.y = y;
                }

                lpDDSScape->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr);
                lpDDSSchatzkarte->Lock(nullptr, &ddsd2, DDLOCK_WAIT, nullptr);

                for (short i = 0; i < SKARTEX; i++)
                {
                    for (short j = 0; j < SKARTEY; j++)
                    {
                        Renderer::GetPixel(static_cast<short>(i + Scape[SchatzPos.x][SchatzPos.y].xScreen - SKARTEX / 2 + KXPIXEL / 2),
                                           static_cast<short>(j + Scape[SchatzPos.x][SchatzPos.y].yScreen - SKARTEY / 2 + 30), &ddsd);
                        Renderer::PutPixel(i, j, Renderer::RGB2DWORD((rgbStruct.r * 30 + rgbStruct.g * 59 + rgbStruct.b * 11) / 100,
                                                                     (rgbStruct.r * 30 + rgbStruct.g * 59 + rgbStruct.b * 11) / 100,
                                                                     (rgbStruct.r * 30 + rgbStruct.g * 59 + rgbStruct.b * 11) / 100 * 3 / 4), &ddsd2);
                    }
                }

                lpDDSScape->Unlock(nullptr);
                lpDDSSchatzkarte->Unlock(nullptr);

                rcRectsrc = Bmp[Cross].rcSrc;
                rcRectdes.left = SKARTEX / 2 - Bmp[Cross].Breite / 2;
                rcRectdes.right = rcRectdes.left + Bmp[Cross].Breite;
                rcRectdes.top = SKARTEY / 2 - Bmp[Cross].Hoehe / 2;
                rcRectdes.bottom = rcRectdes.top + Bmp[Cross].Hoehe;
                Renderer::Blit(Bmp[Cross].Surface, lpDDSSchatzkarte, true);

                lpDDSSchatzkarte->Lock(nullptr, &ddsd2, DDLOCK_WAIT, nullptr);

                // Weichzeichnen
                for (short i = 0; i < SKARTEX; i++)
                {
                    for (short j = 0; j < SKARTEY; j++)
                    {
                        if (i > 0 && i < SKARTEX - 1 && j > 0 && j < SKARTEY - 1)
                        {
                            Renderer::GetPixel(i - 1, j, &ddsd2);
                            RGBSTRUCT rgbleft = rgbStruct;
                            Renderer::GetPixel(i, j - 1, &ddsd2);
                            RGBSTRUCT rgbtop = rgbStruct;
                            Renderer::GetPixel(i + 1, j, &ddsd2);
                            RGBSTRUCT rgbright = rgbStruct;
                            Renderer::GetPixel(i, j + 1, &ddsd2);
                            RGBSTRUCT rgbbottom = rgbStruct;
                            Renderer::GetPixel(i, j, &ddsd2);
                            Renderer::PutPixel(i, j, Renderer::RGB2DWORD(
                                                   (rgbleft.r + rgbtop.r + rgbright.r + rgbbottom.r + rgbStruct.r) / 5,
                                                   (rgbleft.g + rgbtop.g + rgbright.g + rgbbottom.g + rgbStruct.g) / 5,
                                                   (rgbleft.b + rgbtop.b + rgbright.b + rgbbottom.b + rgbStruct.b) / 5),
                                               &ddsd2);
                        }
                    }
                }
                lpDDSSchatzkarte->Unlock(nullptr);
                break;
            }
        }
    }

    void CheckUsage(const short object)
    {
        if (object == ResourceStone && TwoClicks == ResourceBranch || object == ResourceBranch && TwoClicks == ResourceStone)
        {
            if (Guy.Inventar[ResourceAxe] < 1)
            {
                Guy.Inventar[ResourceStone]--;
                Guy.Inventar[ResourceBranch]--;
                Guy.Inventar[ResourceAxe] = 1;
                Bmp[ButtonTimber].Phase = 0;
                Bmp[ButtonBoat].Phase = 0;
                Bmp[ButtonPipe].Phase = 0;
                PapierText = Renderer::DrawText(BAUEAXT, TXTPAPIER, 1);
                PlaySound(Sound::Invention, 100);
            }
            else if (Guy.Inventar[ResourceHarrow] < 1)
            {
                Guy.Inventar[ResourceStone]--;
                Guy.Inventar[ResourceBranch]--;
                Guy.Inventar[ResourceHarrow] = 1;
                Bmp[ButtonField].Phase = 0;
                PapierText = Renderer::DrawText(BAUEEGGE, TXTPAPIER, 1);
                PlaySound(Sound::Invention, 100);
            }
            else
            {
                PapierText = Renderer::DrawText(STEINPLUSASTNICHTS, TXTPAPIER, 1);
            }
        }
        else if (object == ResourceVine && TwoClicks == ResourceBranch || object == ResourceBranch && TwoClicks == ResourceVine)
        {
            if (Guy.Inventar[ResourceFishingRod] < 1)
            {
                Guy.Inventar[ResourceVine]--;
                Guy.Inventar[ResourceBranch]--;
                Guy.Inventar[ResourceFishingRod] = 1;
                Bmp[ButtonFish].Phase = 0;
                PapierText = Renderer::DrawText(BAUEANGEL, TXTPAPIER, 1);
                PlaySound(Sound::Invention, 100);
            }
            else
            {
                PapierText = Renderer::DrawText(ASTPLUSLIANENICHTS, TXTPAPIER, 1);
            }
        }
        else if (object == ResourceVine && TwoClicks == ResourceStone || object == ResourceStone && TwoClicks == ResourceVine)
        {
            if (Guy.Inventar[ResourceSlingshot] < 1)
            {
                Guy.Inventar[ResourceVine]--;
                Guy.Inventar[ResourceStone]--;
                Guy.Inventar[ResourceSlingshot] = 1;
                Bmp[ButtonSlingshot].Phase = 0;
                PapierText = Renderer::DrawText(BAUESCHLEUDER, TXTPAPIER, 1);
                PlaySound(Sound::Invention, 100);
            }
            else
            {
                PapierText = Renderer::DrawText(STEINPLUSLIANENICHTS, TXTPAPIER, 1);
            }
        }
        else
        {
            PapierText = Renderer::DrawText(NICHTBASTELN, TXTPAPIER, 1);
        }
        TwoClicks = -1;
    }

    void Discover()
    {
        auto changed = false;

        for (short i = -1; i <= 1; i++)
        {
            for (short j = -1; j <= 1; j++)
            {
                if (!Scape[Guy.Pos.x + i][Guy.Pos.y + j].Entdeckt)
                {
                    Scape[Guy.Pos.x + i][Guy.Pos.y + j].Entdeckt = true;
                    changed = true;
                }
            }
        }

        if (changed)
        {
            Generate();
        }
    }
} // namespace World
