//#pragma once

#include "types.hpp"
#include "constants.hpp"
#include "extern.hpp"

#include <SFML/Audio.hpp>

#include <vector>

int Spielzustand = 0; // in welchem Zustand ist das Spiel?
bool LAnimation = true; // Ist die Landschaftanimation angeschaltet?
bool Gitter; // Gitternetz an/aus
RECT ScapeGrenze; // Diese Koordinaten zeigen die Größe der Landschaft an
short CursorTyp; // Welcher Cursortyp?
short RouteLaenge; // Länge
short RoutePunkt; // Aktueller Index in RouteKoor
short Bild, LastBild; // Anzahl der Bilder in der Sekunde
RECT rcRectdes, rcRectsrc; // Ständig benötigte Variable zum Blitten
short Tag, Stunden, Minuten; // Wieviel Uhr (0-12h)
char StdString[1024]; // Standard string
char RohString[1024]; // Darin wird gespeichert, wieviel Rohstoffe noch benötigt werden
short PapierText; // Wieviel Papier? (in Pixel) -1 = Kein Text
short HauptMenue; // Welches Menü?
short TwoClicks; // Für Aktionen mit zwei Mausklicks
float Chance; // Wie groß ist die Chance am Tag gerettet zu werden
bool BootsFahrt; // Gerade mit dem Boot unterwegs?
bool Nacht; // Wird die Tageszusammenfassung angezeigt?
short Soundzustand; // -1 = keine Soundkarte;0=kein Sound;1=Sound an
short Frage; // -1=KeineFrage;0=Frage wird gestellt;1=Ja;2=Nein
short AbspannNr = 0; // Zähler für Abspann
short AbspannZustand = 0; // Wo im Abspann
bool SchatzGef = false; // wurde der Schatz gefunden

// Pathfinding
short Step, Steps;

//Bereiche
//                             links,      oben,                      rechts,                             unten
RECT rcGesamt = {0, 0, MAXX, MAXY};
RECT rcSpielflaeche = {0, 0, MAXX - 195, MAXY - 20};
RECT rcPanel = {MAXX - 205, 0, MAXX, MAXY};
RECT rcKarte = {MAXX - 158, 27, rcKarte.left + MAXXKACH * 2, rcKarte.top + (MAXYKACH + 13) * 2};

//Die Eckkoordinaten der Kacheln (Achtung: mit überlappendem Rand)
ZWEID EckKoor[13][4] =
{//       0           1           2           3          Ecke
    {{-1, 29}, {26, 15}, {54, 30}, {27, 44}},//0
    {{-1, 13}, {26, 15}, {54, 30}, {27, 28}},//1
    {{-1, 29}, {26, 15}, {54, 14}, {27, 28}},//2
    {{-1, 29}, {26, -1}, {54, 14}, {27, 44}},//3
    {{-1, 13}, {26, -1}, {54, 30}, {27, 44}},//4
    {{-1, 13}, {26, 15}, {54, 30}, {27, 44}},//5
    {{-1, 29}, {26, 15}, {54, 30}, {27, 28}},//6		Kacheltyp
    {{-1, 29}, {26, 15}, {54, 14}, {27, 44}},//7
    {{-1, 29}, {26, -1}, {54, 30}, {27, 44}},//8
    {{-1, 13}, {26, -1}, {54, 14}, {27, 44}},//9
    {{-1, 13}, {26, -1}, {54, 30}, {27, 28}},//10
    {{-1, 13}, {26, 15}, {54, 14}, {27, 28}},//11
    {{-1, 29}, {26, -1}, {54, 14}, {27, 28}},//12

};

TEXTBEREICH TextBereich[TEXTANZ];

ZWEID Camera, // aktueller Kartenausschnitt
      MousePosition, //     "    Mauskoordinaten
      RouteZiel, RouteStart, // Koordinaten des Starts und des Endes der Route
      Route[MAXXKACH * MAXYKACH], // Liste der Routenpunkte
      RouteKoor[2 * MAXXKACH * MAXYKACH], // Liste der Routenkoordinaten
      SchatzPos; // Hier ist der Schatz vergraben
RGBSTRUCT rgbStruct; // Hier werden die Farben eines Pixels zwischengespeichert
GUY Guy;
BMP Bmp[BILDANZ];
WAV Wav[25]; // Sound::COUNT
ABSPANN AbspannListe[10][10]; // Namenabfolge im Abspann
SCAPE Scape[MAXXKACH][MAXYKACH];

// DirectDraw
LPDIRECTDRAWSURFACE4 lpDDSPrimary = nullptr; // DirectDraw primary surface
LPDIRECTDRAWSURFACE4 lpDDSBack = nullptr; // DirectDraw back surface
LPDIRECTDRAWSURFACE4 lpDDSMisc = nullptr; // DirectDraw Bilder surface
LPDIRECTDRAWSURFACE4 lpDDSPanel = nullptr; // DirectDraw Panel surface
LPDIRECTDRAWSURFACE4 lpDDSGuyAni = nullptr; // DirectDraw GuyAni surface
LPDIRECTDRAWSURFACE4 lpDDSAnimation = nullptr; // DirectDraw Animation surface
LPDIRECTDRAWSURFACE4 lpDDSKarte = nullptr; // DirectDraw MiniMap surface
LPDIRECTDRAWSURFACE4 lpDDSSchrift = nullptr; // DirectDraw Schrift surface
LPDIRECTDRAWSURFACE4 lpDDSSchrift1 = nullptr; // DirectDraw Schrift1 surface
LPDIRECTDRAWSURFACE4 lpDDSSchrift2 = nullptr; // DirectDraw Schrift2 surface
LPDIRECTDRAWSURFACE4 lpDDSTextFeld = nullptr; // DirectDraw TextFeld surface
LPDIRECTDRAWSURFACE4 lpDDSPapier = nullptr; // DirectDraw Paier surface
LPDIRECTDRAWSURFACE4 lpDDSBaum = nullptr; // DirectDraw Bäume surface
LPDIRECTDRAWSURFACE4 lpDDSBau = nullptr; // DirectDraw Bauwerke surface
LPDIRECTDRAWSURFACE4 lpDDSCredits = nullptr; // DirectDraw Credits surface
LPDIRECTDRAWSURFACE4 lpDDSLogo = nullptr; // DirectDraw Logo surface
LPDIRECTDRAWSURFACE4 lpDDSCursor = nullptr; // DirectDraw Cursor surface
LPDIRECTDRAWSURFACE4 lpDDSButtons = nullptr; // DirectDraw Buttons surface
LPDIRECTDRAWSURFACE4 lpDDSInventar = nullptr; // DirectDraw Inventar surface
LPDIRECTDRAWSURFACE4 lpDDSScape = nullptr; // DirectDraw Landschaft surface
LPDIRECTDRAWSURFACE4 lpDDSSchatzkarte = nullptr; // SchatzkartenSurface
LPDIRECTDRAWPALETTE lpDDPal = nullptr; // DirectDraw palette
DDBLTFX ddbltfx; // DirectDraw Effekte 
DDPIXELFORMAT ddpf;
DDSURFACEDESC2 ddsd, ddsd2; // Zwischenspeicher der SurfaceEigenschaften

LPDIRECTDRAWGAMMACONTROL lpDDGammaControl = nullptr; // Die drei sind für das Gammaablenden
DDGAMMARAMP DDGammaRamp;
DDGAMMARAMP DDGammaOld;

// Directinput
HINSTANCE g_hInst = nullptr;
