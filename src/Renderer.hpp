#pragma once

#include "headers.hpp"
#include "types.hpp"

namespace Renderer
{
    inline DWORD RGB2DWORD(BYTE r, BYTE g, BYTE b); // Rechnet 24Bit RGB in 16 Bit um (Für ddbltfx.dwFillColor)

    void DrawObjects();                                                               // Alles Landschaftsdetails zeichnen, die nicht im Speicher sind
    void DrawGuy();                                                                   // Zeichnet die Spielfigur
    void DrawPanel();                                                                 // Zeichnet die Karte
    void DrawImage(short x, short y, short i, RECT Ziel, bool Reverse, short Frucht); // Zeichnet die Bmp-Bilder			
    void DrawPaper();                                                                 // Zeichnet das Papier für den Text
    void DeleteText(short Bereich);                                                   // Löscht einen Breich in der Textsurface
    void DrawString(char* string, short x, short y, short Art);                       // Schreibt einen String an eine gewünschte Stelle
    short DrawText(int TEXT, short Bereich, short Art);                               // Schreibt einen Text in ein Rechteck
    void DrawTreasureMap();                                                           // Schatzkarte malen
    void LimitScroll();                                                               // Verhindert das Scrollen aus dem Bild
    void Blit(LPDIRECTDRAWSURFACE4 lpDDSVon, LPDIRECTDRAWSURFACE4 lpDDSNach, bool Transp);
    void PutPixel(short x, short y, DWORD color, LPDDSURFACEDESC2 ddsd); // Schreibt Pixel in eine Surface (davor lock())
    void GetPixel(short x, short y, LPDDSURFACEDESC2 ddsd);              // Das Ergebnis wird in rgbStruct gespeichert
    void Fade(short RP, short GP, short BP);                             // Bild abdunkeln mittels Gamma-Werten (in Prozent pro Farbwert)
    ZWEID GetTile(short x, short y);                                     // Welche Kachel ist unter den angegebenen Koordinaten
    void DisplayLandscape();                                             // Landschaft anzeigen
    void DisplayIntro();                                                 // Für das Intro zuständig
    void DisplayCredits();                                               // Für den Abspann
    void DisplayLogo();                                                  // Zeigt das Logo
    void DisplayCreditImages(short image, short percent);                 // Zeichnet die Bilder im Abspann
} // namespace Renderer
