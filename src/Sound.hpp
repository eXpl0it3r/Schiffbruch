#pragma once

#include "headers.hpp"
#include "extern.hpp"

namespace Sound
{
	void LoadSound(short Sound);				// Lädt einen Sound in den Speicher		
	void PlaySound(short Sound, short Volume);	// Spiel eine Wavedatei mit Prozentualer Lautstärke
	void StopSound(short Sound);				// Stoppt ein Sound
} // namespace Sound
