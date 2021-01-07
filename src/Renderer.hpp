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
void DrawString(const std::string &string, short x, short y, short Art); // Schreibt einen String an eine gewünschte Stelle
short DrawText(const int TEXT, short Bereich, short Art); // Schreibt einen Text in ein Rechteck
void DrawSchatzkarte(); // Schatzkarte malen
void LimitScroll(); // Verhindert das Scrollen aus dem Bild
//void Blit(sf::Texture *from, sf::Texture *to, bool Transp);
//void Blit(sf::Image *from, sf::Texture *to, bool Transp);
void BlitToScreen(sf::Texture *from);
void BlitToText(sf::Texture *from);
void BlitToLandscape(sf::Texture *from);
void PutPixel(short x, short y, uint8_t r, uint8_t g, uint8_t b, sf::Image *img); // Schreibt Pixel in eine Surface (davor lock())
RGBSTRUCT GetPixel(short x, short y, sf::Image *img); // Das Ergebnis wird in rgbStruct gespeichert
void Fade(short RP, short GP, short BP); // Bild abdunkeln mittels Gamma-Werten (in Prozent pro Farbwert)
Coordinate GetTile(short x, short y); // Welche Kachel ist unter den angegebenen Koordinaten
void Show(); // Landschaft anzeigen
void ShowIntro(); // Für das Intro zuständig
void ShowCredits(); // Für den Abspann
void ShowLogo(); // Zeigt das Logo
void CreditsBlt(short Bild, short Prozent); // Zeichnet die Bilder im Abspann
sf::Image landscapeImage();


void drawRect(const int x, const int y, const int width, const int height, const sf::Color &color);
void drawRectToMap(const int x, const int y, const int width, const int height, const sf::Color &color);
sf::Texture *createEmptyTexture(const size_t width, const size_t height, const sf::Color &color);
sf::Texture *loadTexture(const void *data, const size_t size, const sf::Color &mask = sf::Color(255, 0, 255));
} // namespace Renderer
