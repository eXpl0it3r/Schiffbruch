#pragma once

#include "headers.hpp"
#include "types.hpp"
#include "extern.hpp"

namespace Renderer {
//DWORD RGB2DWORD(BYTE r, BYTE g, BYTE b); // Rechnet 24Bit RGB in 16 Bit um (Für ddbltfx.dwFillColor)
void DrawObjects(); // Alles Landschaftsdetails zeichnen, die nicht im Speicher sind
void DrawGuy(); // Zeichnet die Spielfigur
void DrawPanel(); // Zeichnet die Karte
void DrawPicture(short x, short y, short i, RECT target, bool Reverse, short Fruit); // Zeichnet die Bmp-Bilder
void DrawPaper(); // Zeichnet das Papier für den Text
void HideText(short Bereich); // Löscht einen Breich in der Textsurface
void DrawString(const char *string, short x, short y, short Art); // Schreibt einen String an eine gewünschte Stelle
short DrawText(int TEXT, short Bereich, short Art); // Schreibt einen Text in ein Rechteck
void DrawSchatzkarte(); // Schatzkarte malen
void LimitScroll(); // Verhindert das Scrollen aus dem Bild
void Blitten(sf::Image *from, sf::Image *to, bool Transp);
void PutPixel(short x, short y, uint8_t r, uint8_t g, uint8_t b, sf::Image *img); // Schreibt Pixel in eine Surface (davor lock())
void GetPixel(short x, short y, sf::Image *img); // Das Ergebnis wird in rgbStruct gespeichert
void Fade(short RP, short GP, short BP); // Bild abdunkeln mittels Gamma-Werten (in Prozent pro Farbwert)
Coordinate GetTile(short x, short y); // Welche Kachel ist unter den angegebenen Koordinaten
void Show(); // Landschaft anzeigen
void ShowIntro(); // Für das Intro zuständig
void ShowCredits(); // Für den Abspann
void ShowLogo(); // Zeigt das Logo
void CreditsBlt(short Bild, short Prozent); // Zeichnet die Bilder im Abspann
} // namespace Renderer
