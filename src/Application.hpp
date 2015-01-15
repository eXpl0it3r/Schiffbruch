#pragma once

#include "headers.hpp"
#include "extern.hpp"

#include <SFML/Window.hpp>

#include <string>

class Application
{
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	Application(const std::string& name, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	int run();

	short Refresh();
	void Event(short Eventnr);
	BOOL doInit();

private:
	void process_events();

private:
	sf::Window m_window;

	std::string m_name;
	MSG m_msg;
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	HINSTANCE m_hPrevInstance;
	LPSTR m_lpCmdLine;
	int m_nCmdShow;
};
