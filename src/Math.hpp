#pragma once

#include "headers.hpp"
#include "types.hpp"

namespace Math
{
    void CalculateCreditCoordinates();                                    // Calculate the coordinates for the credits
    void CalculateGuyCoordinates();                                       // Calculate the new coordinates of the player
    void CalculateTileCoordinates();                                      // Calculate the display coordinates of the tile
    void MouseInGame(short button, short push, short xDiff, short yDiff); // Whether the mouse is inside the game area
    void MouseInPanel(short button, short push);                          // Whether the mouse is inside the panel area
    bool PointInTriangle(short x, short y, short x0, short y0, short x1, short y1, short x3, short y3);
    bool PointInRectangle(short x, short y, RECT rectangle);              // Is a point in a rectangle
    void CalculateRectangle(RECT area);                                   // Cut the blit rectangle so one can only draw within the area
    bool LineIntersect(ZWEID startPosition, ZWEID position, bool store);  // For path finding (Is a line interrupted?)
    void Animations();                                                    // Start each animations
    void DisableButtonAnimations();                                       // Disable all button animations (for MouseInPanel)
} // namespace Math
