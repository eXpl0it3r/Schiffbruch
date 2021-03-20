#pragma once

#include "headers.hpp"
#include "extern.hpp"
#include <SFML/Window.hpp>

namespace Direct {
// finished with all objects we use; release them
void finiObjects();

bool InitDDraw(); // DirectDraw initialisieren
void Reset();
void CheckMouse(const Coordinate newMousePos); // Maus abfragen
short CheckKey(); // Tastatur abfragen
}
