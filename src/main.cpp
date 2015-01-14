#include "Application.hpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application app{"Landscape", hInstance, hPrevInstance, lpCmdLine, nCmdShow};

	return app.run();
}
