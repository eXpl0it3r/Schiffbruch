#pragma once

namespace Action
{
    enum class Actions : short
    {
        Nothing = 0,
        Search = 1,
        Eat = 2,
        Drink = 3,
        Log = 4,
        Field = 5,
        DayEnd = 6,
        Rescued = 7,
        Tent = 8,
        Sleep = 9,
        Cancel = 10,
        Fish = 11,
        Boat = 12,
        Undock = 13,
        Dock = 14,
        Pipe = 15,
        Destroy = 16,
        SosSign = 17,
        House1 = 18,
        House2 = 19,
        House3 = 20,
        Fireplace = 21,
        Light = 22,
        Lookout = 23,
        Treasure = 24,
        Intro = 25,
        Slingshot = 26,
        Quit = 27,
        Restart = 28,
        Death = 29,
        DayRestart = 30,
    };

    void Handler(Actions event);

    void Search();     // Search tile
    void Eat();        // Eat fruits
    void Drink();      // Drink water
    void Log();        // Log trees
    void Field();      // Plant fields
    void DayEnd();     // When the day ends
    void Rescued();    // When one is rescued
    void Tent();       // Built a tent
    void Sleep();      // Sleep
    void Cancel();     // Cancel action
    void Fish();       // Fish
    void Boat();       // Craft a boat
    void Undock();     // Undock from the beach into the sea
    void Dock();       // Dock from the sea onto the beach
    void Pipe();       // Build water pipe
    void Destroy();    // Destroy object
    void SosSign();    // Write SOS
    void House1();     // Build ladder
    void House2();     // Build a platform
    void House3();     // Build a tree house
    void Fireplace();  // Stack fire place
    void Light();      // Light fire place
    void Lookout();    // Keep watch
    void Treasure();   // Dig for the treasure
    void Intro();      // Active intro
    void Slingshot();  // Use the slingshot
    void Quit();       // Ask, whether they really want to exit the game
    void Restart();    // Ask, whether they really want to restart the game
    void RestartDay(); // Ask, whether they really want to restart the day
    void Death();      // Death
}
