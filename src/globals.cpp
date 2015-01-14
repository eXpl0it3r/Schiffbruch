#pragma once

#include "types.hpp"
#include "constants.hpp"
#include "extern.hpp"

BOOL    bActive = false;			// is application active?
BOOL	Spielzustand = SZNICHTS;	// in welchem Zustand ist das Spiel?
BOOL	MouseAktiv = false;			// Mouse angestellt?
BOOL	LAnimation = true;			// Ist die Landschaftanimation angeschaltet?
HWND	hwnd;
BOOL	Gitter;						// Gitternetz an/aus
RECT	ScapeGrenze;				// Diese Koordinaten zeigen die Größe der Landschaft an
SHORT	Flusslaenge[FLUSSANZAHL];
SHORT	CursorTyp;					// Welcher Cursortyp?
BOOL	Button0down;				// linke Maustaste gedrückt gehalten
BOOL	Button1down;				// rechte Maustaste gedrückt gehalten
SHORT	RouteLaenge;				// Länge
SHORT	RoutePunkt;					// Aktueller Index in RouteKoor
FLOAT	Schrittx, Schritty;			// Zum Figur laufen lassen
LONG	Zeit;						// Start der Sekunde
SHORT	Bild, LastBild;				// Anzahl der Bilder in der Sekunde
RECT	rcRectdes, rcRectsrc;		// Ständig benötigte Variable zum Blitten
SHORT	Tag, Stunden, Minuten;		// Wieviel Uhr (0-12h)
CHAR	StdString[1024];			// Standard string
CHAR	RohString[1024];			// Darin wird gespeichert, wieviel Rohstoffe noch benötigt werden
SHORT	PapierText;					// Wieviel Papier? (in Pixel) -1 = Kein Text
SHORT	HauptMenue;					// Welches Menü?
SHORT	TwoClicks;					// Für Aktionen mit zwei Mausklicks
FLOAT	Chance;						// Wie groß ist die Chance am Tag gerettet zu werden
BOOL	BootsFahrt;					// Gerade mit dem Boot unterwegs?
BOOL	Nacht;						// Wird die Tageszusammenfassung angezeigt?
SHORT	Soundzustand;				// -1 = keine Soundkarte;0=kein Sound;1=Sound an
BOOL	Spielbeenden = false;		// Wenn true wird das Spiel sofort beendet
SHORT	Frage;						// -1=KeineFrage;0=Frage wird gestellt;1=Ja;2=Nein
DOUBLE	pi = 3.1415926535;			// pi, was sonst
SHORT	AbspannNr = 0;				// Zähler für Abspann
SHORT	AbspannZustand = 0;			// Wo im Abspann
BOOL	SchatzGef = false;			// wurde der Schatz gefunden

// Pathfinding
short	Step, Steps;
int  	LenMap[MAXXKACH][MAXYKACH];

//Bereiche
//                             links,      oben,                      rechts,                      unten
RECT   rcGesamt =       {          0,         0,                        MAXX,                       MAXY };
RECT   rcSpielflaeche = {          0,         0,                  MAXX - 195,                  MAXY - 20 };
RECT   rcPanel =        { MAXX - 205,         0,                        MAXX,                       MAXY };
RECT   rcKarte =        { MAXX - 158,        23, rcKarte.left + MAXXKACH * 2, rcKarte.top + MAXYKACH * 2 };
RECT   rcTextFeld1 =    {          0, MAXY - 20,                  MAXX - 195,                       MAXY };

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

short Vierecke[13][4][13] = //0=Passt nicht 1=1runter 2=gleiche Hoehe 3=1hoch
{//		  0	  1	  2	  3	  4	  5	  6	  7	  8	  9	 10	 11	 12		
	{ { { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 1 }, { 1 } },//0
	{ { 2 }, { 1 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 2 }, { 0 }, { 1 }, { 1 }, { 0 } },//1
	/*0*/{ { 2 }, { 0 }, { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 1 }, { 1 }, { 0 }, { 0 } },//2 Kante
	{ { 2 }, { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 1 }, { 0 }, { 0 }, { 1 } } //3
	},
	{ { { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 } },
	{ { 2 }, { 1 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 2 }, { 0 }, { 1 }, { 1 }, { 0 } },
	/*1*/{ { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 } },
	{ { 3 }, { 3 }, { 0 }, { 2 }, { 0 }, { 3 }, { 3 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 2 } }
	},
	{ { { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 1 }, { 1 } },
	{ { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 } },
	/*2*/{ { 3 }, { 0 }, { 3 }, { 0 }, { 2 }, { 0 }, { 3 }, { 3 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 } },
	{ { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 } }
	},
	{ { { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 } },
	{ { 3 }, { 2 }, { 0 }, { 3 }, { 0 }, { 0 }, { 0 }, { 3 }, { 3 }, { 0 }, { 2 }, { 2 }, { 0 } },
	/*3*/{ { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 } },
	{ { 2 }, { 2 }, { 0 }, { 1 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 1 }, { 0 }, { 0 }, { 1 } }
	},
	{ { { 3 }, { 0 }, { 2 }, { 0 }, { 3 }, { 3 }, { 0 }, { 0 }, { 3 }, { 0 }, { 0 }, { 2 }, { 2 } },
	{ { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 } },
	/*4*/{ { 2 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 2 }, { 0 }, { 1 }, { 1 }, { 0 }, { 0 } },
	{ { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 2 }, { 0 }, { 0 } }
	},//  0	  1	  2	  3	  4	  5	  6	  7	  8	  9	 10	 11	 12	
	{ { { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 } },
	{ { 2 }, { 1 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 2 }, { 0 }, { 1 }, { 1 }, { 0 } },
	/*5*/{ { 2 }, { 0 }, { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 1 }, { 1 }, { 0 }, { 0 } },
	{ { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 2 }, { 0 }, { 0 } }
	},
	{ { { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 1 }, { 1 } },
	{ { 2 }, { 1 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 2 }, { 0 }, { 1 }, { 1 }, { 0 } },
	/*6*/{ { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 } },
	{ { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 } }
	},
	{ { { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 1 }, { 1 } },
	{ { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 } },
	/*7*/{ { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 } },
	{ { 2 }, { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 1 }, { 0 }, { 0 }, { 1 } }
	},
	{ { { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 } },
	{ { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 } },
	/*8*/{ { 2 }, { 0 }, { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 1 }, { 1 }, { 0 }, { 0 } },
	{ { 2 }, { 2 }, { 0 }, { 1 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 1 }, { 0 }, { 0 }, { 1 } }
	},
	{ { { 3 }, { 0 }, { 2 }, { 0 }, { 3 }, { 3 }, { 0 }, { 0 }, { 3 }, { 0 }, { 0 }, { 2 }, { 2 } },
	{ { 3 }, { 2 }, { 0 }, { 3 }, { 0 }, { 0 }, { 0 }, { 3 }, { 3 }, { 0 }, { 2 }, { 2 }, { 0 } },
	/*9*/{ { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 } },
	{ { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 2 }, { 0 }, { 0 } }
	},//  0	  1	  2	  3	  4	  5	  6	  7	  8	  9	 10	 11	 12	
	{ { { 3 }, { 0 }, { 2 }, { 0 }, { 3 }, { 3 }, { 0 }, { 0 }, { 3 }, { 0 }, { 0 }, { 2 }, { 2 } },
	{ { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 } },
	/*10*/{ { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 } },
	{ { 3 }, { 3 }, { 0 }, { 2 }, { 0 }, { 3 }, { 3 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 2 } }
	},
	{ { { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 } },
	{ { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 } },
	/*11*/{ { 3 }, { 0 }, { 3 }, { 0 }, { 2 }, { 0 }, { 3 }, { 3 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 } },
	{ { 3 }, { 3 }, { 0 }, { 2 }, { 0 }, { 3 }, { 3 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 2 } }
	},
	{ { { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 } },
	{ { 3 }, { 2 }, { 0 }, { 3 }, { 0 }, { 0 }, { 0 }, { 3 }, { 3 }, { 0 }, { 2 }, { 2 }, { 0 } },
	/*12*/{ { 3 }, { 0 }, { 3 }, { 0 }, { 2 }, { 0 }, { 3 }, { 3 }, { 0 }, { 2 }, { 2 }, { 0 }, { 0 } },
	{ { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 }, { 0 }, { 0 }, { 2 }, { 0 } }
	},

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
WAV Wav[WAVANZ];
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
IDirectInput*           g_pDI = NULL;
IDirectInputDevice*     g_pMouse = NULL;
IDirectInputDevice*     g_pKey = NULL;
HANDLE                  g_hMouseEvent = NULL;
HINSTANCE				g_hInst = NULL;
BOOL					MouseInit = false;

// DirectSound
LPDIRECTSOUND8			lpds;				// DirectSoundObjekt
DSBUFFERDESC            dsbdesc;
LPDIRECTSOUNDBUFFER     lpdsb;
LPDIRECTSOUNDBUFFER     lpdsbPrimary;
LPDIRECTSOUNDBUFFER     lpdsbWav[WAVANZ];	// Wavedateispeicher
