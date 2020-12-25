#pragma once

#include "headers.hpp"
#include "extern.hpp"

namespace Sound {
void Init();
void LoadSound(short Sound); // Lädt einen Sound in den Speicher
void PlaySound(short Sound, short Volume); // Spiel eine Wavedatei mit Prozentualer Lautstärke
void StopSound(short Sound); // Stoppt ein Sound

enum Type {
    NOTHING = 0,
    STORM = 1,
    SWIM = STORM + 1,
    SPLAT = STORM + 2,
    LOG = STORM + 3,
    HIT = STORM + 4,
    SLINGSHOT = STORM + 5,
    DIG = STORM + 6,
    HAMMER = STORM + 7,
    CRASH = STORM + 8,
    SNORE = STORM + 9,
    DRINK = STORM + 10,
    CRACKLE = STORM + 11,
    FISH = STORM + 12,
    FOREST = 14,
    FIRE = FOREST + 1,
    SURF = FOREST + 2,
    TIMBER = FOREST + 3,
    RIVER = FOREST + 4,
    CLICK = 19,
    CLICK2 = CLICK + 1,
    LOGO = CLICK + 2,
    OUTRO = CLICK + 3,
    WOLF = CLICK + 4,
    INVENTION = CLICK + 5,
    COUNT = INVENTION + 1, // Number of sounds
};
} // namespace Sound
