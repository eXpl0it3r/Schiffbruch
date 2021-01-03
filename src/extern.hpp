#pragma once

#include "headers.hpp"
#include "types.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics/Image.hpp>

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
extern short Tag, Hours, Minutes; // Wieviel Uhr (0-12h)
extern char StdString[1024]; // Standard string
extern char RohString[1024]; // Darin wird gespeichert, wieviel Rohstoffe noch benötigt werden
extern short PapierText; // Wieviel Papier? (in Pixel) -1 = Kein Text
extern short HauptMenue; // Welches Menü?
extern short TwoClicks; // Für Aktionen mit zwei Mausklicks
extern float Chance; // Wie groß ist die Chance am Tag gerettet zu werden
extern bool IsInBoat; // Gerade mit dem Boot unterwegs?
extern bool Night; // Wird die Tageszusammenfassung angezeigt?
extern short Soundzustand; // -1 = keine Soundkarte;0=kein Sound;1=Sound an
extern short Frage; // -1=KeineFrage;0=Frage wird gestellt;1=Ja;2=Nein
extern short AbspannNr; // Zähler für Abspann
extern short AbspannZustand; // Wo im Abspann
extern bool SchatzGef; // wurde der Schatz gefunden

extern short Step, Steps;

// Should be const
extern RECT rcGesamt;
extern RECT rcPlayingSurface;
extern RECT rcPanel;
extern RECT rcKarte;

extern Coordinate CornerCoord[13][4];

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
extern SCAPE Landscape[MAX_TILES_X][MAX_TILESY];

// DirectDraw
extern sf::Image *lpDDSPrimary; // DirectDraw primary surface
extern sf::Image *lpDDSBack; // DirectDraw back surface
extern sf::Image *lpDDSMisc; // DirectDraw Bilder surface
extern sf::Image *lpDDSPanel; // DirectDraw Panel surface
extern sf::Image *lpDDSGuyAni; // DirectDraw GuyAni surface
extern sf::Image *lpDDSAnimation; // DirectDraw Animation surface
extern sf::Image *lpDDSKarte; // DirectDraw MiniMap surface
extern sf::Image *lpDDSSchrift; // DirectDraw Schrift surface
extern sf::Image *lpDDSSchrift1; // DirectDraw Schrift1 surface
extern sf::Image *lpDDSSchrift2; // DirectDraw Schrift2 surface
extern sf::Image *lpDDSTextFeld; // DirectDraw TextFeld surface
extern sf::Image *lpDDSPaper; // DirectDraw Paier surface
extern sf::Image *lpDDSBaum; // DirectDraw Bäume surface
extern sf::Image *lpDDSBau; // DirectDraw Bauwerke surface
extern sf::Image *lpDDSCredits; // DirectDraw Credits surface
extern sf::Image *lpDDSLogo; // DirectDraw Logo surface
extern sf::Image *lpDDSCursor; // DirectDraw Cursor surface
extern sf::Image *lpDDSButtons; // DirectDraw Buttons surface
extern sf::Image *lpDDSInventar; // DirectDraw Inventar surface
extern sf::Image *lpDDSScape; // DirectDraw Landschaft surface
extern sf::Image *lpDDSSchatzkarte; // SchatzkartenSurface
extern sf::Image *darknessOverlay;

const char *GetLanguageString(const int index);
