#pragma once

#include "headers.hpp"
#include "extern.hpp"

namespace Game {
void NewGame(bool neu); // Neues Spiel starten
void InitStructs(); // Initialisiert die Structs
void InitWaves(); // Initialisiert die Wav Structs
void SaveGame(); // Speichert den Spielstand
bool LoadGame(); // Lädt den Spielstand
}
