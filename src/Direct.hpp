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

	void InitDDraw();	// DirectDraw initialisieren
	void InitDInput();	// DirectInput initialisieren
	void InitDSound();	// DirectSound initialisieren
	void SetAcquire();	// Maus aktivieren
	void CheckMouse();	// Maus abfragen
	short CheckKey();	// Tastatur abfragen

}