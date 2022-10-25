#pragma once

#include "headers.hpp"
#include "extern.hpp"

namespace World {
void AddTime(short h, short m); // Adds hours and minutes to the time of day
void AddResource(short Art, float Anzahl); // Adds water, etc.
void Generate(); // Generate tiles in memory
void Compute(short MinimumSize, short maximumSize); // Generate a random landscape
void CreateSea(); // Fill up with sea
void CreateRiver(); // Create a river
bool CheckRiverFlow(short x, short y); // For flow ()
void CreateTrees(short percent); // Spread the trees on the island
void UpdateDiscovered(); // Reveal the tiles next to the player
void OnRawMaterialsUsed(short Objekt); // Checks whether the correct raw materials have been used (in the inventory)
bool CheckRawMaterials(); // Checks the raw materials that are needed for construction
void RawMaterialsDescriptionString(short x, short y, short Objekt); // // Writes the required raw materials in the raw string
void UpdateButtons(); // Changes the buttons (stop, next, discard, etc.)
void ToggleIsInBoat(); // change the necessary things for a boat trip (and back);
void FillPipe(); // Checks the pipes to see if they are connected to the water supply
void CheckPipe(short x, short y); // recursive procedure for fill pipe ();
void Treasure(); // Bury the treasure and draw a treasure map
void CreatePirateWreck(); // Set the pirate wreck
} // namespace World
