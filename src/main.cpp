#include "Application.hpp"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    Application app("Schiffbruch", hInstance);
    app.Run();
}
