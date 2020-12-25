#pragma once

#include "headers.hpp"
#include "types.hpp"

#include <SFML/Audio.hpp>

#include <vector>

extern int Spielzustand; // in welchem Zustand ist das Spiel?
extern bool LAnimation; // Ist die Landschaftanimation angeschaltet?
extern bool Gitter; // Gitternetz an/aus
extern RECT ScapeGrenze; // Diese Koordinaten zeigen die Größe der Landschaft an
extern short CursorTyp; // Welcher Cursortyp?
extern short RouteLaenge; // Länge
extern short RoutePunkt; // Aktueller Index in RouteKoor
extern short Bild, LastBild; // Anzahl der Bilder in der Sekunde
extern RECT rcRectdes, rcRectsrc; // Ständig benötigte Variable zum Blitten
extern short Tag, Stunden, Minuten; // Wieviel Uhr (0-12h)
extern char StdString[1024]; // Standard string
extern char RohString[1024]; // Darin wird gespeichert, wieviel Rohstoffe noch benötigt werden
extern short PapierText; // Wieviel Papier? (in Pixel) -1 = Kein Text
extern short HauptMenue; // Welches Menü?
extern short TwoClicks; // Für Aktionen mit zwei Mausklicks
extern float Chance; // Wie groß ist die Chance am Tag gerettet zu werden
extern bool IsInBoat; // Gerade mit dem Boot unterwegs?
extern bool Nacht; // Wird die Tageszusammenfassung angezeigt?
extern short Soundzustand; // -1 = keine Soundkarte;0=kein Sound;1=Sound an
extern short Frage; // -1=KeineFrage;0=Frage wird gestellt;1=Ja;2=Nein
extern short AbspannNr; // Zähler für Abspann
extern short AbspannZustand; // Wo im Abspann
extern bool SchatzGef; // wurde der Schatz gefunden

extern short Step, Steps;

// Should be const
extern RECT rcGesamt;
extern RECT rcSpielflaeche;
extern RECT rcPanel;
extern RECT rcKarte;

extern Coordinate EckKoor[13][4];

extern TextArea TextBereich[TEXTANZ];

extern Coordinate Camera, // aktueller Kartenausschnitt
             MousePosition, //     "    Mauskoordinaten
             RouteDestination, RouteStart, // Koordinaten des Starts und des Endes der Route
             Route[MAX_TILES_X * MAX_TILESY], // Liste der Routenpunkte
             RouteKoor[2 * MAX_TILES_X * MAX_TILESY], // Liste der Routenkoordinaten
             SchatzPos; // Hier ist der Schatz vergraben

extern RGBSTRUCT rgbStruct; // Hier werden die Farben eines Pixels zwischengespeichert
extern GUY Guy;
extern BMP Bmp[SPRITE_COUNT];
extern WAV Wav[25]; // Sound::COUNT;
extern CREDITS CreditsList[10][10]; // Namenabfolge im Abspann
extern SCAPE Scape[MAX_TILES_X][MAX_TILESY];

// DirectDraw
extern LPDIRECTDRAWSURFACE4 lpDDSPrimary; // DirectDraw primary surface
extern LPDIRECTDRAWSURFACE4 lpDDSBack; // DirectDraw back surface
extern LPDIRECTDRAWSURFACE4 lpDDSMisc; // DirectDraw Bilder surface
extern LPDIRECTDRAWSURFACE4 lpDDSPanel; // DirectDraw Panel surface
extern LPDIRECTDRAWSURFACE4 lpDDSGuyAni; // DirectDraw GuyAni surface
extern LPDIRECTDRAWSURFACE4 lpDDSAnimation; // DirectDraw Animation surface
extern LPDIRECTDRAWSURFACE4 lpDDSKarte; // DirectDraw MiniMap surface
extern LPDIRECTDRAWSURFACE4 lpDDSSchrift; // DirectDraw Schrift surface
extern LPDIRECTDRAWSURFACE4 lpDDSSchrift1; // DirectDraw Schrift1 surface
extern LPDIRECTDRAWSURFACE4 lpDDSSchrift2; // DirectDraw Schrift2 surface
extern LPDIRECTDRAWSURFACE4 lpDDSTextFeld; // DirectDraw TextFeld surface
extern LPDIRECTDRAWSURFACE4 lpDDSPaper; // DirectDraw Paier surface
extern LPDIRECTDRAWSURFACE4 lpDDSBaum; // DirectDraw Bäume surface
extern LPDIRECTDRAWSURFACE4 lpDDSBau; // DirectDraw Bauwerke surface
extern LPDIRECTDRAWSURFACE4 lpDDSCredits; // DirectDraw Credits surface
extern LPDIRECTDRAWSURFACE4 lpDDSLogo; // DirectDraw Logo surface
extern LPDIRECTDRAWSURFACE4 lpDDSCursor; // DirectDraw Cursor surface
extern LPDIRECTDRAWSURFACE4 lpDDSButtons; // DirectDraw Buttons surface
extern LPDIRECTDRAWSURFACE4 lpDDSInventar; // DirectDraw Inventar surface
extern LPDIRECTDRAWSURFACE4 lpDDSScape; // DirectDraw Landschaft surface
extern LPDIRECTDRAWSURFACE4 lpDDSSchatzkarte; // SchatzkartenSurface
extern LPDIRECTDRAWPALETTE lpDDPal; // DirectDraw palette
extern DDBLTFX ddbltfx; // DirectDraw Effekte 
extern DDPIXELFORMAT ddpf;
extern DDSURFACEDESC2 ddsd, ddsd2; // Zwischenspeicher der SurfaceEigenschaften

extern LPDIRECTDRAWGAMMACONTROL lpDDGammaControl; // Die drei sind für das Gammaablenden
extern DDGAMMARAMP DDGammaRamp;
extern DDGAMMARAMP DDGammaOld;

// Directinput
extern HINSTANCE g_hInst;
