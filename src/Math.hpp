#pragma once

#include "headers.hpp"
#include "types.hpp"
#include "extern.hpp"

namespace Math {
void AbspannCalc(); // Kalkuliert die Koordinaten für den Abspann
void CalcGuyKoor(); // Berechnet die neuen Koordinaten der Spielfigur
void CalcKoor(); // Bildschirmkoordinaten der Kacheln berechnnen;
void MouseInSpielflaeche(short Button, short Push, short xDiff, short yDiff); // Falls die Maus in der Spielfäche ist
void MouseInPanel(short Button, short Push); // Falls die Maus im Panel ist
bool InDreieck(short X, short Y, short X0, short Y0, short X1, short Y1, short X3, short Y3);
bool InRect(short x, short y, RECT rcRect); // Ist ein Punkt in einem Rechteck
void CalcRect(RECT rcBereich); // Beschneidet das BlitRechteck so, daß es nur innerhalb des Bereiches gemalt wird
bool LineIntersect(Coordinate LineStartPos, Coordinate Pos, bool store); // Für Pathfinding (Ist eine Gerade unterbrochen?)
void Animationen(); // Startet die einzelnen Animationen
void ButtAniAus(); // Schaltet alle Knopfanimationien aus (für MouseInPanel)
} // namespace Math
