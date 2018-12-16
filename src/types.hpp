#pragma once

#include "headers.hpp"

struct TEXTBEREICH
{
    bool Aktiv; // Steht Text in diesem Bereich?
    RECT rcText; // Die Position des Ausgabe
};

struct ZWEID
{
    short x;
    short y;
};

struct RGBSTRUCT
{
    BYTE r, g, b;
};

struct GUY
{
    bool Aktiv; // Ist er aktiv?
    short Aktion; // Welche Aktion (Suchen, fischen ...) (Übergeordnet über Zustand)
    ZWEID Pos; // KachelPosition der Spielfigur 
    ZWEID PosAlt; // Die ursprünglich Position in der Kachel (für die Aktionsprozeduren)
    ZWEID PosScreen; // Absolute Position der Spielfigur
    short Zustand; // Was macht er gerade? (Animation)(linkslaufen,rechtslaufen...,angeln..)
    short AkNummer; // Bei welcher Aktion (für die Aktionsprozeduren)
    float Resource[3]; // Wieviel Wasservorrat usw
    short Inventar[BILDANZ]; // Welche Rohstoffe usw. besitzt man
};

struct BMP
{
    LPDIRECTDRAWSURFACE4 Surface; // in welcher Surface gespeichert?
    bool Animation; // Läuft die Animations?
    short Anzahl; // Anzahl der Animationsphasen
    short Phase; // die aktuelle Phase
    RECT rcSrc; // Quelle des 1. Bildes
    RECT rcDes; // Falls es immer an die gleiche Stelle gezeichnet wird. (Buttons)
    short Breite; // Die Breite des Bildes
    short Hoehe; // Die Hoehe des Bildes
    short Geschwindigkeit; // Wieviel Bilder/sec
    short Sound; // Welcher Sound gehört dazu
    //zum bauen
    short Rohstoff[BILDANZ]; // Anzahl des i.Rohstoffs, den man zum Bau benötigt
    short AkAnzahl; // Anzahl der Aktionsfaellen, die zum Bau benötigt werden
    bool First; // Ist es das erstemal gebaut, dann Hilfetext
};

struct WAV
{
    char* Dateiname; // Dateiname der Wavdatei
    bool Loop; // Nur einmal abspielen und ständig
    short Volume; // Die Standardlautstärke in Prozent
};

struct ABSPANN
{
    bool Aktiv; // Bewegt sich gerade
    short Bild; // welches Bild
};

struct SCAPE
{
    short Typ; // Flach, Hang usw.
    short Art; // Sand, Moor ...
    short Hoehe; // Die Hoehe der Kachel
    bool Markiert; // Ist diese Kachel markiert?
    short xScreen, yScreen; // Die Koordinaten in der Scape-Surface
    bool Begehbar; // notwendig für Pathfinding
    bool Entdeckt; // Ist dieses Feld schon aufgedeckt?
    short LaufZeit; // LaufZeit auf dieser Kachel (1 am schnellsten...)
    short Objekt; // Welches Objekt steht drauf (z.Bsp. Flüsse)
    bool Reverse; // Wird die Objektanimation umgekehrt abgespielt (für flüsse)
    ZWEID ObPos; // Die Koordinaten des Objekts (relativ zu xScreen und yScreen)
    float Phase; // Welche Animationsphase oder Bildversion
    short AkNummer; // Welche Aktionsnummer (um Baustellen vortzusetzen)
    ZWEID GPosAlt; // Damit der Guy an der richtigen Stelle (x,y) weiterbaut
    short Rohstoff[BILDANZ]; //Anzahl des i.Rohstoffs, den man noch zum bauen braucht
    float Timer; //Bis jetzt nur fürs Feuer nötig
};

struct FLUSSLAUF
{
    short x, y;
};
