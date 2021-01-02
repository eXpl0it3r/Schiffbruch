#pragma once

#include "headers.hpp"
#include "types.hpp"

extern int Spielzustand;            // In which stat is the game?
extern bool LAnimation;             // Is the landscape animation enabled?
extern bool Gitter;                 // Mesh lines on/off
extern RECT ScapeGrenze;            // These coordinates define the size of the landscape
extern short CursorTyp;             // Which cursor type?
extern short RouteLaenge;           // Length
extern short RoutePunkt;            // Current index in the RouteKoor
extern short Bild, LastBild;        // Number of frames per second
extern RECT rcRectdes, rcRectsrc;   // Constantly needed variables for blitting
extern short Tag, Stunden, Minuten; // What time it is (0-12h)
extern char StdString[1024];        // Standard string
extern char RohString[1024];        // It contains how many resources are still required
extern short PapierText;            // How much paper? (in pixel) -1 = No text
extern short HauptMenue;            // Which menu?
extern short TwoClicks;             // For actions with two mouse clicks
extern float Chance;                // How large is the chance of getting rescued on this day
extern bool BootsFahrt;             // Currently traveling with the boat?
extern bool Nacht;                  // Is the daily summary being shown?
extern short Soundzustand;          // -1 = no sound card; 0 = no sound;                1 = sound on
extern short Frage;                 // -1 = no question;   0 = question is being asked; 1 = yes;     2 = no
extern short AbspannNr;             // Counter for the credits
extern short AbspannZustand;        // Position in the credits
extern bool SchatzGef;              // Treasure was found

extern short Step, Steps;

// Should be const
extern RECT rcGesamt;
extern RECT rcSpielflaeche;
extern RECT rcPanel;
extern RECT rcKarte;

extern ZWEID EckKoor[13][4];

extern TEXTBEREICH TextBereich[TEXTANZ];

extern ZWEID Camera,                             // Current map section
             MousePosition,                      // Current mouse position
             RouteZiel, RouteStart,              // Coordinates of the start and end of the route
             Route[MAXXKACH * MAXYKACH],         // List of route points
             RouteKoor[2 * MAXXKACH * MAXYKACH], // List of route coordinates
             SchatzPos;                          // Here a treasure is buried

extern RGBSTRUCT rgbStruct; // Here the colors of the pixels are cached
extern GUY Guy;
extern BMP Bmp[ImageCount];
extern WAV Wav[25];
extern ABSPANN AbspannListe[10][10]; // Name order in the credits
extern SCAPE Scape[MAXXKACH][MAXYKACH];

// DirectDraw
extern LPDIRECTDRAWSURFACE4 lpDDSPrimary;         // DirectDraw primary surface
extern LPDIRECTDRAWSURFACE4 lpDDSBack;            // DirectDraw back surface
extern LPDIRECTDRAWSURFACE4 lpDDSMisc;            // DirectDraw image surface
extern LPDIRECTDRAWSURFACE4 lpDDSPanel;           // DirectDraw Panel surface
extern LPDIRECTDRAWSURFACE4 lpDDSGuyAni;          // DirectDraw GuyAni surface
extern LPDIRECTDRAWSURFACE4 lpDDSAnimation;       // DirectDraw Animation surface
extern LPDIRECTDRAWSURFACE4 lpDDSKarte;           // DirectDraw MiniMap surface
extern LPDIRECTDRAWSURFACE4 lpDDSSchrift;         // DirectDraw Schrift surface
extern LPDIRECTDRAWSURFACE4 lpDDSSchrift1;        // DirectDraw Schrift1 surface
extern LPDIRECTDRAWSURFACE4 lpDDSSchrift2;        // DirectDraw Schrift2 surface
extern LPDIRECTDRAWSURFACE4 lpDDSTextFeld;        // DirectDraw TextFeld surface
extern LPDIRECTDRAWSURFACE4 lpDDSPapier;          // DirectDraw Paier surface
extern LPDIRECTDRAWSURFACE4 lpDDSBaum;            // DirectDraw Bäume surface
extern LPDIRECTDRAWSURFACE4 lpDDSBau;             // DirectDraw Bauwerke surface
extern LPDIRECTDRAWSURFACE4 lpDDSCredits;         // DirectDraw Credits surface
extern LPDIRECTDRAWSURFACE4 lpDDSLogo;            // DirectDraw Logo surface
extern LPDIRECTDRAWSURFACE4 lpDDSCursor;          // DirectDraw Cursor surface
extern LPDIRECTDRAWSURFACE4 lpDDSButtons;         // DirectDraw Buttons surface
extern LPDIRECTDRAWSURFACE4 lpDDSInventar;        // DirectDraw Inventar surface
extern LPDIRECTDRAWSURFACE4 lpDDSScape;           // DirectDraw Landschaft surface
extern LPDIRECTDRAWSURFACE4 lpDDSSchatzkarte;     // SchatzkartenSurface
extern LPDIRECTDRAWPALETTE lpDDPal;               // DirectDraw palette
extern DDBLTFX ddbltfx;                           // DirectDraw Effekte 
extern DDPIXELFORMAT ddpf;
extern DDSURFACEDESC2 ddsd, ddsd2;                // Cache of the surface properties

extern LPDIRECTDRAWGAMMACONTROL lpDDGammaControl; // The three are for gamma control
extern DDGAMMARAMP DDGammaRamp;
extern DDGAMMARAMP DDGammaOld;

// Directinput
extern HINSTANCE g_hInst;
