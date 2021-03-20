//#pragma once

#include "types.hpp"
#include "constants.hpp"
#include "extern.hpp"

#include "strings.h"

#include <SFML/Audio.hpp>

#include <vector>

int s_GameState = 0; // in welchem Zustand ist das Spiel?
bool LAnimation = true; // Ist die Landschaftanimation angeschaltet?
bool Gitter; // Gitternetz an/aus
RECT ScapeGrenze; // Diese Koordinaten zeigen die Größe der Landschaft an
short CursorTyp; // Welcher Cursortyp?
short RouteLaenge; // Länge
short RoutePunkt; // Aktueller Index in RouteKoor
short CurrentFrame, FPS; // Anzahl der Bilder in der Sekunde
RECT rcRectdes, rcRectsrc; // Ständig benötigte Variable zum Blitten
short Tag, Hours, Minutes; // Wieviel Uhr (0-12h)
char StdString[1024]; // Standard string
char RohString[1024]; // Darin wird gespeichert, wieviel Rohstoffe noch benötigt werden
short PapierText; // Wieviel Papier? (in Pixel) -1 = Kein Text
short HauptMenue; // Welches Menü?
short TwoClicks; // Für Aktionen mit zwei Mausklicks
float Chance; // Wie groß ist die Chance am Tag gerettet zu werden
bool IsInBoat; // Gerade mit dem Boot unterwegs?
bool Night; // Wird die Tageszusammenfassung angezeigt?
short s_SoundState; // -1 = keine Soundkarte;0=kein Sound;1=Sound an
short Frage; // -1=KeineFrage;0=Frage wird gestellt;1=Ja;2=Nein
short CreditsNum = 0; // Zähler für Abspann
short CreditsState = 0; // Wo im Abspann
bool SchatzGef = false; // wurde der Schatz gefunden

// Pathfinding
short Step, Steps;

//Bereiche
//                             links,      oben,                      rechts,                             unten
RECT rcGesamt = {0, 0, MAX_SCREEN_X, MAX_SCREEN_Y};
RECT rcPlayingSurface = {0, 0, MAX_SCREEN_X - 195, MAX_SCREEN_Y - 20};
RECT rcPanel = {MAX_SCREEN_X - 205, 0, MAX_SCREEN_X, MAX_SCREEN_Y};
RECT rcKarte = {MAX_SCREEN_X - 158, 27, rcKarte.left + MAX_TILES_X * 2, rcKarte.top + (MAX_TILESY + 13) * 2};

//Die Eckkoordinaten der Kacheln (Achtung: mit überlappendem Rand)
Coordinate CornerCoord[13][4] = {
    //       0           1           2           3          Ecke
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

TextArea TextBereich[TEXTANZ];


Coordinate Camera, // aktueller Kartenausschnitt
           MousePosition, //     "    Mauskoordinaten
           RouteDestination, RouteStart, // Koordinaten des Starts und des Endes der Route
           Route[MAX_TILES_X * MAX_TILESY], // Liste der Routenpunkte
           RouteKoor[2 * MAX_TILES_X * MAX_TILESY], // Liste der Routenkoordinaten
           SchatzPos; // Hier ist der Schatz vergraben
GUY Guy;
BMP Bmp[SPRITE_COUNT];
WAV Wav[25]; // Sound::COUNT
CREDITS CreditsList[10][10]; // Namenabfolge im Abspann
SCAPE Landscape[MAX_TILES_X][MAX_TILESY];

// DirectDraw
sf::Texture *screenTexture = nullptr; // DirectDraw primary surface
sf::Texture *lpDDSBack = nullptr; // DirectDraw back surface
sf::Texture *lpDDSMisc = nullptr; // DirectDraw Bilder surface
sf::Texture *lpDDSPanel = nullptr; // DirectDraw Panel surface
sf::Texture *lpDDSGuyAni = nullptr; // DirectDraw GuyAni surface
sf::Texture *lpDDSAnimation = nullptr; // DirectDraw Animation surface
sf::Texture *lpDDSKarte = nullptr; // DirectDraw MiniMap surface
sf::Texture *lpDDSSchrift1 = nullptr; // DirectDraw Schrift1 surface
sf::Texture *lpDDSSchrift2 = nullptr; // DirectDraw Schrift2 surface
sf::Texture *lpDDSTextFeld = nullptr; // DirectDraw TextFeld surface
sf::Texture *lpDDSPaper = nullptr; // DirectDraw Paier surface
sf::Texture *lpDDSBaum = nullptr; // DirectDraw Bäume surface
sf::Texture *lpDDSBau = nullptr; // DirectDraw Bauwerke surface
sf::Texture *lpDDSCredits = nullptr; // DirectDraw Credits surface
sf::Texture *lpDDSLogo = nullptr; // DirectDraw Logo surface
sf::Texture *lpDDSCursor = nullptr; // DirectDraw Cursor surface
sf::Texture *lpDDSButtons = nullptr; // DirectDraw Buttons surface
sf::Texture *lpDDSInventar = nullptr; // DirectDraw Inventar surface

sf::Texture *lpDDSSchatzkarte = nullptr; // SchatzkartenSurface
sf::Sprite *s_treasureMapSprite;

sf::Color s_darknessColor;

sf::Sprite *s_creditsSprite;
int s_previousCreditsOverlay;

sf::Sprite *minimapPlayerSprite;
sf::Texture *minimapPlayerTexture;

//LPDIRECTDRAWPALETTE lpDDPal = nullptr; // DirectDraw palette
//DDBLTFX ddbltfx; // DirectDraw Effekte
//DDPIXELFORMAT ddpf;
//DDSURFACEDESC2 ddsd, ddsd2; // Zwischenspeicher der SurfaceEigenschaften

////LPDIRECTDRAWGAMMACONTROL lpDDGammaControl = nullptr; // Die drei sind für das Gammaablenden
//DDGAMMARAMP DDGammaRamp;
//DDGAMMARAMP DDGammaOld;
