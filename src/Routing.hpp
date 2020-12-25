#pragma once

#include "headers.hpp"
#include "extern.hpp"

namespace Routing {
void MarkRoute(bool Mark); // Markiert die Route
bool FindTheWay(); // PathFinding
short RotateRight(short Dir); // Richtungskoordinate rechtsrum umrechnen
void SortRoute(); // Die Route in die richtige Reihenfolge bringen
bool CheckRoute(short x, short y, bool save, short Laenge); // Nachprüfen ob auf aktuellem Teil in der Route ist
void ShortRoute(short Zielx, short Ziely); // Macht eine Mini-Route von den Guy.Position zu den Zielk.
} // namespace Route
