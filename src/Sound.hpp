#pragma once

namespace Sound
{
    void Initialize();
    void LoadSound(short sound);               // Lädt einen Sound in den Speicher		
    void PlaySound(short sound, short volume); // Spiel eine Wavedatei mit Prozentualer Lautstärke
    void StopSound(short sound);               // Stoppt ein Sound

    enum Type
    {
        Nothing = 0,
        Storm = 1,
        Swim = Storm + 1,
        Splat = Storm + 2,
        Log = Storm + 3,
        Hit = Storm + 4,
        Slingshot = Storm + 5,
        Dig = Storm + 6,
        Hammer = Storm + 7,
        Crash = Storm + 8,
        Snore = Storm + 9,
        Drink = Storm + 10,
        Crackle = Storm + 11,
        Fish = Storm + 12,
        Forest = 14,
        Fire = Forest + 1,
        Surf = Forest + 2,
        Timber = Forest + 3,
        River = Forest + 4,
        Click = 19,
        Click2 = Click + 1,
        Logo = Click + 2,
        Outro = Click + 3,
        Wolf = Click + 4,
        Invention = Click + 5,
        Count = Invention + 1, // Number of sounds
    };
} // namespace Sound
