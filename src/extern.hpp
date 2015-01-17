#pragma once

#include "headers.hpp"
#include "types.hpp"

extern BOOL		bActive;					// is application active?
extern BOOL		Spielzustand;				// in welchem Zustand ist das Spiel?
extern BOOL		MouseAktiv;					// Mouse angestellt?
extern BOOL		LAnimation;					// Ist die Landschaftanimation angeschaltet?
extern HWND		hwnd;
extern BOOL		Gitter;						// Gitternetz an/aus
extern RECT		ScapeGrenze;				// Diese Koordinaten zeigen die Größe der Landschaft an
extern SHORT	Flusslaenge[FLUSSANZAHL];
extern SHORT	CursorTyp;					// Welcher Cursortyp?
extern BOOL		Button0down;				// linke Maustaste gedrückt gehalten
extern BOOL		Button1down;				// rechte Maustaste gedrückt gehalten
extern SHORT	RouteLaenge;				// Länge
extern SHORT	RoutePunkt;					// Aktueller Index in RouteKoor
extern FLOAT	Schrittx, Schritty;			// Zum Figur laufen lassen
extern LONG		Zeit;						// Start der Sekunde
extern SHORT	Bild, LastBild;				// Anzahl der Bilder in der Sekunde
extern RECT		rcRectdes, rcRectsrc;		// Ständig benötigte Variable zum Blitten
extern SHORT	Tag, Stunden, Minuten;		// Wieviel Uhr (0-12h)
extern CHAR		StdString[1024];			// Standard string
extern CHAR		RohString[1024];			// Darin wird gespeichert, wieviel Rohstoffe noch benötigt werden
extern SHORT	PapierText;					// Wieviel Papier? (in Pixel) -1 = Kein Text
extern SHORT	HauptMenue;					// Welches Menü?
extern SHORT	TwoClicks;					// Für Aktionen mit zwei Mausklicks
extern FLOAT	Chance;						// Wie groß ist die Chance am Tag gerettet zu werden
extern BOOL		BootsFahrt;					// Gerade mit dem Boot unterwegs?
extern BOOL		Nacht;						// Wird die Tageszusammenfassung angezeigt?
extern SHORT	Soundzustand;				// -1 = keine Soundkarte;0=kein Sound;1=Sound an
extern BOOL		Spielbeenden;				// Wenn true wird das Spiel sofort beendet
extern SHORT	Frage;						// -1=KeineFrage;0=Frage wird gestellt;1=Ja;2=Nein
extern DOUBLE	pi;							// pi, was sonst
extern SHORT	AbspannNr;					// Zähler für Abspann
extern SHORT	AbspannZustand;				// Wo im Abspann
extern BOOL		SchatzGef;					// wurde der Schatz gefunden

extern short	Step, Steps;
extern int  	LenMap[MAXXKACH][MAXYKACH];

// Should be const
extern RECT		rcGesamt;
extern RECT		rcSpielflaeche;
extern RECT		rcPanel;
extern RECT		rcKarte;
extern RECT		rcTextFeld1;
extern ZWEID	EckKoor[13][4];
extern short	Vierecke[13][4][13];

extern TEXTBEREICH TextBereich[TEXTANZ];

extern ZWEID Camera,				// aktueller Kartenausschnitt
MousePosition,						//     "    Mauskoordinaten
RouteZiel, RouteStart,				// Koordinaten des Starts und des Endes der Route
Route[MAXXKACH*MAXYKACH],			// Liste der Routenpunkte
RouteKoor[2 * MAXXKACH*MAXYKACH],	// Liste der Routenkoordinaten
SaveRoute[MAXXKACH*MAXYKACH],		// Zum zwischenspeichern der Route
NewPos,								// Nur innerhalb des Pathfindings benutzt
GuyPosScreenStart,					// Absolute StartPosition bei einem Schritt (Für CalcGuyKoor)
SchatzPos;							// Hier ist der Schatz vergraben

extern RGBSTRUCT rgbStruct;				// Hier werden die Farben eines Pixels zwischengespeichert
extern GUY Guy;
extern BMP Bmp[BILDANZ];
extern WAV Wav[25];						// Sound::COUNT;
extern ABSPANN AbspannListe[10][10];	// Namenabfolge im Abspann
extern SCAPE Scape[MAXXKACH][MAXYKACH];
extern FLUSSLAUF Flusslauf[FLUSSANZAHL][MAXFLUSS];


// DirectDraw
extern LPDIRECTDRAW4            lpDD;				// DirectDraw object
extern LPDIRECTDRAWSURFACE4     lpDDSPrimary;		// DirectDraw primary surface
extern LPDIRECTDRAWSURFACE4     lpDDSBack;			// DirectDraw back surface
extern LPDIRECTDRAWSURFACE4     lpDDSMisc;			// DirectDraw Bilder surface
extern LPDIRECTDRAWSURFACE4     lpDDSPanel;			// DirectDraw Panel surface
extern LPDIRECTDRAWSURFACE4     lpDDSGuyAni;		// DirectDraw GuyAni surface
extern LPDIRECTDRAWSURFACE4     lpDDSAnimation;		// DirectDraw Animation surface
extern LPDIRECTDRAWSURFACE4     lpDDSKarte;			// DirectDraw MiniMap surface
extern LPDIRECTDRAWSURFACE4     lpDDSSchrift;		// DirectDraw Schrift surface
extern LPDIRECTDRAWSURFACE4     lpDDSSchrift1;		// DirectDraw Schrift1 surface
extern LPDIRECTDRAWSURFACE4     lpDDSSchrift2;		// DirectDraw Schrift2 surface
extern LPDIRECTDRAWSURFACE4     lpDDSTextFeld;		// DirectDraw TextFeld surface
extern LPDIRECTDRAWSURFACE4     lpDDSPapier;		// DirectDraw Paier surface
extern LPDIRECTDRAWSURFACE4     lpDDSBaum;			// DirectDraw Bäume surface
extern LPDIRECTDRAWSURFACE4     lpDDSBau;			// DirectDraw Bauwerke surface
extern LPDIRECTDRAWSURFACE4     lpDDSCredits;		// DirectDraw Credits surface
extern LPDIRECTDRAWSURFACE4     lpDDSLogo;			// DirectDraw Logo surface
extern LPDIRECTDRAWSURFACE4     lpDDSCursor;		// DirectDraw Cursor surface
extern LPDIRECTDRAWSURFACE4     lpDDSButtons;		// DirectDraw Buttons surface
extern LPDIRECTDRAWSURFACE4     lpDDSInventar;		// DirectDraw Inventar surface
extern LPDIRECTDRAWSURFACE4     lpDDSScape;			// DirectDraw Landschaft surface
extern LPDIRECTDRAWSURFACE4     lpDDSSchatzkarte;	// SchatzkartenSurface
extern LPDIRECTDRAWPALETTE      lpDDPal;			// DirectDraw palette
extern DDBLTFX					ddbltfx;			// DirectDraw Effekte 
extern DDPIXELFORMAT			ddpf;
extern DDSURFACEDESC2           ddsd, ddsd2;		// Zwischenspeicher der SurfaceEigenschaften

extern LPDIRECTDRAWGAMMACONTROL lpDDGammaControl;	// Die drei sind für das Gammaablenden
extern DDGAMMARAMP				DDGammaRamp;
extern DDGAMMARAMP				 DDGammaOld;

// Directinput
extern HINSTANCE			g_hInst;

// DirectSound
extern LPDIRECTSOUND8		lpds;				// DirectSoundObjekt
extern DSBUFFERDESC			dsbdesc;
extern LPDIRECTSOUNDBUFFER	lpdsb;
extern LPDIRECTSOUNDBUFFER	lpdsbPrimary;
extern LPDIRECTSOUNDBUFFER	lpdsbWav[25];		// Sound::COUNT - Wavedateispeicher
