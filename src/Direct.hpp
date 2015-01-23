#pragma once

#include "headers.hpp"
#include "extern.hpp"

namespace Direct
{
	// finished with all objects we use; release them
	static void finiObjects(void)
	{
		if (lpDD != nullptr)
		{
			if (lpDDSPrimary != nullptr)
			{
				lpDDSPrimary->Release();
				lpDDSPrimary = nullptr;
			}
			if (lpDDPal != nullptr)
			{
				lpDDPal->Release();
				lpDDPal = nullptr;
			}
			lpDD->Release();
			lpDD = nullptr;
		}
	}

	void InitDDraw(HWND hWnd);	// DirectDraw initialisieren
	void InitDSound(HWND hWnd);	// DirectSound initialisieren
	void CheckMouse();			// Maus abfragen
	short CheckKey();			// Tastatur abfragen

}