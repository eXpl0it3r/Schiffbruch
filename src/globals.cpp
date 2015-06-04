#pragma once

#include "types.hpp"
#include "constants.hpp"
#include "extern.hpp"

#include <SFML/Audio.hpp>

BOOL	Spielzustand = 0;		// in welchem Zustand ist das Spiel?
BOOL	LAnimation = true;		// Ist die Landschaftanimation angeschaltet?
BOOL	Gitter;					// Gitternetz an/aus
RECT	ScapeGrenze;			// Diese Koordinaten zeigen die Größe der Landschaft an
SHORT	CursorTyp;				// Welcher Cursortyp?
BOOL	Button0down;			// linke Maustaste gedrückt gehalten
BOOL	Button1down;			// rechte Maustaste gedrückt gehalten
SHORT	RouteLaenge;			// Länge
SHORT	RoutePunkt;				// Aktueller Index in RouteKoor
FLOAT	Schrittx, Schritty;		// Zum Figur laufen lassen
SHORT	Bild, LastBild;			// Anzahl der Bilder in der Sekunde
RECT	rcRectdes, rcRectsrc;	// Ständig benötigte Variable zum Blitten
SHORT	Tag, Stunden, Minuten;	// Wieviel Uhr (0-12h)
CHAR	StdString[1024];		// Standard string
CHAR	RohString[1024];		// Darin wird gespeichert, wieviel Rohstoffe noch benötigt werden
SHORT	PapierText;				// Wieviel Papier? (in Pixel) -1 = Kein Text
SHORT	HauptMenue;				// Welches Menü?
SHORT	TwoClicks;				// Für Aktionen mit zwei Mausklicks
FLOAT	Chance;					// Wie groß ist die Chance am Tag gerettet zu werden
BOOL	BootsFahrt;				// Gerade mit dem Boot unterwegs?
BOOL	Nacht;					// Wird die Tageszusammenfassung angezeigt?
SHORT	Soundzustand;			// -1 = keine Soundkarte;0=kein Sound;1=Sound an
SHORT	Frage;					// -1=KeineFrage;0=Frage wird gestellt;1=Ja;2=Nein
DOUBLE	pi = 3.1415926535;		// pi, was sonst
SHORT	AbspannNr = 0;			// Zähler für Abspann
SHORT	AbspannZustand = 0;		// Wo im Abspann
BOOL	SchatzGef = false;		// wurde der Schatz gefunden

// Pathfinding
short	Step, Steps;
int  	LenMap[MAXXKACH][MAXYKACH];

//Bereiche
//                             links,      oben,                      rechts,                             unten
RECT   rcGesamt =       {          0,         0,                        MAXX,                              MAXY };
RECT   rcSpielflaeche = {          0,         0,                  MAXX - 195,                         MAXY - 20 };
RECT   rcPanel =        { MAXX - 205,         0,                        MAXX,                              MAXY };
RECT   rcKarte =        { MAXX - 158,        27, rcKarte.left + MAXXKACH * 2, rcKarte.top + (MAXYKACH + 13) * 2 };

//Die Eckkoordinaten der Kacheln (Achtung: mit überlappendem Rand)
ZWEID EckKoor[13][4] =
{//       0           1           2           3          Ecke
	{ { -1, 29 }, { 26, 15 }, { 54, 30 }, { 27, 44 } },//0
	{ { -1, 13 }, { 26, 15 }, { 54, 30 }, { 27, 28 } },//1
	{ { -1, 29 }, { 26, 15 }, { 54, 14 }, { 27, 28 } },//2
	{ { -1, 29 }, { 26, -1 }, { 54, 14 }, { 27, 44 } },//3
	{ { -1, 13 }, { 26, -1 }, { 54, 30 }, { 27, 44 } },//4
	{ { -1, 13 }, { 26, 15 }, { 54, 30 }, { 27, 44 } },//5
	{ { -1, 29 }, { 26, 15 }, { 54, 30 }, { 27, 28 } },//6		Kacheltyp
	{ { -1, 29 }, { 26, 15 }, { 54, 14 }, { 27, 44 } },//7
	{ { -1, 29 }, { 26, -1 }, { 54, 30 }, { 27, 44 } },//8
	{ { -1, 13 }, { 26, -1 }, { 54, 14 }, { 27, 44 } },//9
	{ { -1, 13 }, { 26, -1 }, { 54, 30 }, { 27, 28 } },//10
	{ { -1, 13 }, { 26, 15 }, { 54, 14 }, { 27, 28 } },//11
	{ { -1, 29 }, { 26, -1 }, { 54, 14 }, { 27, 28 } },//12

};

TEXTBEREICH TextBereich[TEXTANZ];

ZWEID Camera,						// aktueller Kartenausschnitt
MousePosition,						//     "    Mauskoordinaten
RouteZiel, RouteStart,				// Koordinaten des Starts und des Endes der Route
Route[MAXXKACH*MAXYKACH],			// Liste der Routenpunkte
RouteKoor[2 * MAXXKACH*MAXYKACH],	// Liste der Routenkoordinaten
SaveRoute[MAXXKACH*MAXYKACH],		// Zum zwischenspeichern der Route
NewPos,								// Nur innerhalb des Pathfindings benutzt
GuyPosScreenStart,					// Absolute StartPosition bei einem Schritt (Für CalcGuyKoor)
SchatzPos;							// Hier ist der Schatz vergraben
RGBSTRUCT rgbStruct;				// Hier werden die Farben eines Pixels zwischengespeichert
GUY Guy;
BMP Bmp[BILDANZ];
WAV Wav[25];						// Sound::COUNT
ABSPANN AbspannListe[10][10];		// Namenabfolge im Abspann
SCAPE Scape[MAXXKACH][MAXYKACH];
FLUSSLAUF Flusslauf[FLUSSANZAHL][MAXFLUSS];

// DirectDraw
LPDIRECTDRAW4            lpDD = NULL;				// DirectDraw object
LPDIRECTDRAWSURFACE4     lpDDSPrimary = NULL;		// DirectDraw primary surface
LPDIRECTDRAWSURFACE4     lpDDSBack = NULL;			// DirectDraw back surface
LPDIRECTDRAWSURFACE4     lpDDSMisc = NULL;			// DirectDraw Bilder surface
LPDIRECTDRAWSURFACE4     lpDDSPanel = NULL;			// DirectDraw Panel surface
LPDIRECTDRAWSURFACE4     lpDDSGuyAni = NULL;		// DirectDraw GuyAni surface
LPDIRECTDRAWSURFACE4     lpDDSAnimation = NULL;		// DirectDraw Animation surface
LPDIRECTDRAWSURFACE4     lpDDSKarte = NULL;			// DirectDraw MiniMap surface
LPDIRECTDRAWSURFACE4     lpDDSSchrift = NULL;		// DirectDraw Schrift surface
LPDIRECTDRAWSURFACE4     lpDDSSchrift1 = NULL;		// DirectDraw Schrift1 surface
LPDIRECTDRAWSURFACE4     lpDDSSchrift2 = NULL;		// DirectDraw Schrift2 surface
LPDIRECTDRAWSURFACE4     lpDDSTextFeld = NULL;		// DirectDraw TextFeld surface
LPDIRECTDRAWSURFACE4     lpDDSPapier = NULL;		// DirectDraw Paier surface
LPDIRECTDRAWSURFACE4     lpDDSBaum = NULL;			// DirectDraw Bäume surface
LPDIRECTDRAWSURFACE4     lpDDSBau = NULL;			// DirectDraw Bauwerke surface
LPDIRECTDRAWSURFACE4     lpDDSCredits = NULL;		// DirectDraw Credits surface
LPDIRECTDRAWSURFACE4     lpDDSLogo = NULL;			// DirectDraw Logo surface
LPDIRECTDRAWSURFACE4     lpDDSCursor = NULL;		// DirectDraw Cursor surface
LPDIRECTDRAWSURFACE4     lpDDSButtons = NULL;		// DirectDraw Buttons surface
LPDIRECTDRAWSURFACE4     lpDDSInventar = NULL;		// DirectDraw Inventar surface
LPDIRECTDRAWSURFACE4     lpDDSScape = NULL;			// DirectDraw Landschaft surface
LPDIRECTDRAWSURFACE4     lpDDSSchatzkarte = NULL;	// SchatzkartenSurface
LPDIRECTDRAWPALETTE      lpDDPal = NULL;			// DirectDraw palette
DDBLTFX					 ddbltfx;					// DirectDraw Effekte 
DDPIXELFORMAT			 ddpf;
DDSURFACEDESC2           ddsd, ddsd2;				// Zwischenspeicher der SurfaceEigenschaften

LPDIRECTDRAWGAMMACONTROL lpDDGammaControl = NULL;	// Die drei sind für das Gammaablenden
DDGAMMARAMP				 DDGammaRamp;
DDGAMMARAMP				 DDGammaOld;

// Directinput
HINSTANCE				g_hInst = NULL;

// Sound
sf::SoundBuffer			sound_buffers[25]; // Sound::COUNT - Wavedateispeicher
sf::Sound				sound_players[25]; // Sound::COUNT
