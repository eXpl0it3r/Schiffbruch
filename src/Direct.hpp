#pragma once

#include "headers.hpp"

namespace Direct
{
    void InitializeDirectDraw(HWND windowHandle);
    void FinalizeObjects();
    void Reset();
    void CheckMouse();
    short CheckKey();
}
