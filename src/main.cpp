#include "Application.hpp"
#include "miniwin.h"

//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main(int argc, char *argv[])
{
    HINSTANCE hInstance;
    Application app("schiffbruch", hInstance);
    app.run();
}
