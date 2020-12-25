#pragma once

#include "headers.hpp"
#include "extern.hpp"

namespace Direct {
// finished with all objects we use; release them
void finiObjects();

bool InitDDraw(HWND hWnd); // DirectDraw initialisieren
void Reset();
void CheckMouse(); // Maus abfragen
short CheckKey(); // Tastatur abfragen
}
