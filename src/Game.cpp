#include "Game.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

#include <fstream>

namespace Game
{
	void SaveGame()
	{
		short i;

		std::ofstream ofs("save.dat", std::ios::binary);
		if (!ofs) return;

		ofs.write((char*)Scape, sizeof(Scape));
		ofs.write((char*)&Guy, sizeof(Guy));
		ofs.write((char*)&BootsFahrt, sizeof(BootsFahrt));
		ofs.write((char*)&Camera, sizeof(Camera));
		ofs.write((char*)&Chance, sizeof(Chance));
		ofs.write((char*)&Gitter, sizeof(Gitter));
		ofs.write((char*)&HauptMenue, sizeof(HauptMenue));
		ofs.write((char*)&LAnimation, sizeof(LAnimation));
		ofs.write((char*)&Minuten, sizeof(Minuten));
		ofs.write((char*)&ScapeGrenze, sizeof(ScapeGrenze));
		ofs.write((char*)&SchatzPos, sizeof(SchatzPos));
		ofs.write((char*)&Spielzustand, sizeof(Spielzustand));
		ofs.write((char*)&Stunden, sizeof(Stunden));
		ofs.write((char*)&Tag, sizeof(Tag));
		ofs.write((char*)TextBereich, sizeof(TextBereich));
		ofs.write((char*)&SchatzGef, sizeof(SchatzGef));

		for (i = 0; i<BILDANZ; i++)
		{
			ofs.write((char*)&Bmp[i].Animation, sizeof(Bmp[i].Animation));
			ofs.write((char*)&Bmp[i].Phase, sizeof(Bmp[i].Phase));
			ofs.write((char*)&Bmp[i].First, sizeof(Bmp[i].First));
		}
	}

	bool LoadGame()
	{
		short i;

		std::ifstream ifs("save.dat", std::ios::binary); // | ios::nocreate);
		if (!ifs) return(false);
		
		ifs.read((char*)Scape, sizeof(Scape));
		ifs.read((char*)&Guy, sizeof(Guy));
		ifs.read((char*)&BootsFahrt, sizeof(BootsFahrt));
		ifs.read((char*)&Camera, sizeof(Camera));
		ifs.read((char*)&Chance, sizeof(Chance));
		ifs.read((char*)&Gitter, sizeof(Gitter));
		ifs.read((char*)&HauptMenue, sizeof(HauptMenue));
		ifs.read((char*)&LAnimation, sizeof(LAnimation));
		ifs.read((char*)&Minuten, sizeof(Minuten));
		ifs.read((char*)&ScapeGrenze, sizeof(ScapeGrenze));
		ifs.read((char*)&SchatzPos, sizeof(SchatzPos));
		ifs.read((char*)&Spielzustand, sizeof(Spielzustand));
		ifs.read((char*)&Stunden, sizeof(Stunden));
		ifs.read((char*)&Tag, sizeof(Tag));
		ifs.read((char*)TextBereich, sizeof(TextBereich));
		ifs.read((char*)&SchatzGef, sizeof(SchatzGef));
		
		for (i = 0; i<BILDANZ; i++)
		{
			ifs.read((char*)&Bmp[i].Animation, sizeof(Bmp[i].Animation));
			ifs.read((char*)&Bmp[i].Phase, sizeof(Bmp[i].Phase));
			ifs.read((char*)&Bmp[i].First, sizeof(Bmp[i].First));
		}
		return(true);
	}

	void InitStructs()
	{
		short i, k;

		// BILD
		// Standardbildinitialisierung
		for (i = 0; i<BILDANZ; i++)
		{
			Bmp[i].Animation = false;
			Bmp[i].Anzahl = 0;
			Bmp[i].Geschwindigkeit = 0;
			Bmp[i].Phase = 0;
			Bmp[i].Surface = nullptr;
			Bmp[i].rcSrc.left = 0;
			Bmp[i].rcSrc.right = 0;
			Bmp[i].rcSrc.top = 0;
			Bmp[i].rcSrc.bottom = 0;
			Bmp[i].rcDes.left = 0;
			Bmp[i].rcDes.right = 0;
			Bmp[i].rcDes.top = 0;
			Bmp[i].rcDes.bottom = 0;
			Bmp[i].Breite = 0;
			Bmp[i].Hoehe = 0;
			Bmp[i].Sound = 0;
			for (k = 0; k<BILDANZ; k++)
				Bmp[i].Rohstoff[k] = 0;
			Bmp[i].AkAnzahl = 0;
			Bmp[i].First = true;
		}

		// Guy
		for (i = GUYLINKS; i <= GUYUNTEN; i++)
		{
			Bmp[i].Animation = false;
			Bmp[i].Anzahl = 4;
			Bmp[i].Geschwindigkeit = 20;
			Bmp[i].Phase = 0;
			Bmp[i].Surface = lpDDSGuyAni;
			Bmp[i].rcSrc.left = 7 * (i - GUYLINKS);
			Bmp[i].rcSrc.right = 7 + 7 * (i - GUYLINKS);
			Bmp[i].rcSrc.top = 0;
			Bmp[i].rcSrc.bottom = 18;
			Bmp[i].Breite = 7;
			Bmp[i].Hoehe = 18;
		}

		for (i = GUYSUCHEN; i <= GUYSCHLEUDER; i++)
		{
			Bmp[i].Animation = false;
			Bmp[i].Phase = 0;
			Bmp[i].Surface = lpDDSGuyAni;
		}
		Bmp[GUYSUCHEN].Anzahl = 4;
		Bmp[GUYSUCHEN].Geschwindigkeit = 4;
		Bmp[GUYSUCHEN].rcSrc.left = 28;
		Bmp[GUYSUCHEN].rcSrc.right = 39;
		Bmp[GUYSUCHEN].rcSrc.top = 0;
		Bmp[GUYSUCHEN].rcSrc.bottom = 14;
		Bmp[GUYSUCHEN].Breite = 11;
		Bmp[GUYSUCHEN].Hoehe = 14;
		Bmp[GUYSUCHEN].Sound = Sound::CRACKLE;

		Bmp[GUYESSEN].Anzahl = 4;
		Bmp[GUYESSEN].Geschwindigkeit = 4;
		Bmp[GUYESSEN].rcSrc.left = 39;
		Bmp[GUYESSEN].rcSrc.right = 39 + 7;
		Bmp[GUYESSEN].rcSrc.top = 0;
		Bmp[GUYESSEN].rcSrc.bottom = 17;
		Bmp[GUYESSEN].Breite = 7;
		Bmp[GUYESSEN].Hoehe = 17;
		Bmp[GUYESSEN].Sound = Sound::CRACKLE;

		Bmp[GUYTRINKEN].Anzahl = 5;
		Bmp[GUYTRINKEN].Geschwindigkeit = 4;
		Bmp[GUYTRINKEN].rcSrc.left = 46;
		Bmp[GUYTRINKEN].rcSrc.right = 46 + 9;
		Bmp[GUYTRINKEN].rcSrc.top = 0;
		Bmp[GUYTRINKEN].rcSrc.bottom = 13;
		Bmp[GUYTRINKEN].Breite = 9;
		Bmp[GUYTRINKEN].Hoehe = 13;
		Bmp[GUYTRINKEN].Sound = Sound::DRINK;

		Bmp[GUYFAELLEN].Anzahl = 4;
		Bmp[GUYFAELLEN].Geschwindigkeit = 6;
		Bmp[GUYFAELLEN].rcSrc.left = 55;
		Bmp[GUYFAELLEN].rcSrc.right = 55 + 15;
		Bmp[GUYFAELLEN].rcSrc.top = 0;
		Bmp[GUYFAELLEN].rcSrc.bottom = 19;
		Bmp[GUYFAELLEN].Breite = 15;
		Bmp[GUYFAELLEN].Hoehe = 19;
		Bmp[GUYFAELLEN].Sound = Sound::LOG;

		Bmp[GUYWARTEN].Anzahl = 4;
		Bmp[GUYWARTEN].Geschwindigkeit = 2;
		Bmp[GUYWARTEN].rcSrc.left = 70;
		Bmp[GUYWARTEN].rcSrc.right = 70 + 7;
		Bmp[GUYWARTEN].rcSrc.top = 0;
		Bmp[GUYWARTEN].rcSrc.bottom = 18;
		Bmp[GUYWARTEN].Breite = 7;
		Bmp[GUYWARTEN].Hoehe = 18;

		Bmp[GUYFELD].Anzahl = 4;
		Bmp[GUYFELD].Geschwindigkeit = 4;
		Bmp[GUYFELD].rcSrc.left = 78;
		Bmp[GUYFELD].rcSrc.right = 78 + 19;
		Bmp[GUYFELD].rcSrc.top = 0;
		Bmp[GUYFELD].rcSrc.bottom = 18;
		Bmp[GUYFELD].Breite = 19;
		Bmp[GUYFELD].Hoehe = 18;

		Bmp[GUYBINDENOBEN].Anzahl = 2;
		Bmp[GUYBINDENOBEN].Geschwindigkeit = 1;
		Bmp[GUYBINDENOBEN].rcSrc.left = 97;
		Bmp[GUYBINDENOBEN].rcSrc.right = 97 + 8;
		Bmp[GUYBINDENOBEN].rcSrc.top = 0;
		Bmp[GUYBINDENOBEN].rcSrc.bottom = 18;
		Bmp[GUYBINDENOBEN].Breite = 8;
		Bmp[GUYBINDENOBEN].Hoehe = 18;

		Bmp[GUYBINDENUNTEN].Anzahl = 2;
		Bmp[GUYBINDENUNTEN].Geschwindigkeit = 1;
		Bmp[GUYBINDENUNTEN].rcSrc.left = 98;
		Bmp[GUYBINDENUNTEN].rcSrc.right = 98 + 7;
		Bmp[GUYBINDENUNTEN].rcSrc.top = 36;
		Bmp[GUYBINDENUNTEN].rcSrc.bottom = 36 + 18;
		Bmp[GUYBINDENUNTEN].Breite = 7;
		Bmp[GUYBINDENUNTEN].Hoehe = 18;

		Bmp[GUYSCHLAFZELT].Anzahl = 2;
		Bmp[GUYSCHLAFZELT].Geschwindigkeit = 1;
		Bmp[GUYSCHLAFZELT].rcSrc.left = 105;
		Bmp[GUYSCHLAFZELT].rcSrc.right = 105 + 20;
		Bmp[GUYSCHLAFZELT].rcSrc.top = 54;
		Bmp[GUYSCHLAFZELT].rcSrc.bottom = 54 + 10;
		Bmp[GUYSCHLAFZELT].Breite = 20;
		Bmp[GUYSCHLAFZELT].Hoehe = 10;
		Bmp[GUYSCHLAFZELT].Sound = Sound::SNORE;

		Bmp[GUYSCHLAFEN].Anzahl = 2;
		Bmp[GUYSCHLAFEN].Geschwindigkeit = 1;
		Bmp[GUYSCHLAFEN].rcSrc.left = 125;
		Bmp[GUYSCHLAFEN].rcSrc.right = 125 + 17;
		Bmp[GUYSCHLAFEN].rcSrc.top = 36;
		Bmp[GUYSCHLAFEN].rcSrc.bottom = 36 + 18;
		Bmp[GUYSCHLAFEN].Breite = 17;
		Bmp[GUYSCHLAFEN].Hoehe = 18;
		Bmp[GUYSCHLAFEN].Sound = Sound::SNORE;

		Bmp[GUYGEHINZELT].Anzahl = 3;
		Bmp[GUYGEHINZELT].Geschwindigkeit = 4;
		Bmp[GUYGEHINZELT].rcSrc.left = 105;
		Bmp[GUYGEHINZELT].rcSrc.right = 105 + 7;
		Bmp[GUYGEHINZELT].rcSrc.top = 0;
		Bmp[GUYGEHINZELT].rcSrc.bottom = 0 + 18;
		Bmp[GUYGEHINZELT].Breite = 7;
		Bmp[GUYGEHINZELT].Hoehe = 18;

		Bmp[GUYHINLEGEN].Anzahl = 2;
		Bmp[GUYHINLEGEN].Geschwindigkeit = 2;
		Bmp[GUYHINLEGEN].rcSrc.left = 125;
		Bmp[GUYHINLEGEN].rcSrc.right = 125 + 17;
		Bmp[GUYHINLEGEN].rcSrc.top = 0;
		Bmp[GUYHINLEGEN].rcSrc.bottom = 0 + 18;
		Bmp[GUYHINLEGEN].Breite = 17;
		Bmp[GUYHINLEGEN].Hoehe = 18;

		Bmp[GUYAUFSTEHEN].Anzahl = 2;
		Bmp[GUYAUFSTEHEN].Geschwindigkeit = 2;
		Bmp[GUYAUFSTEHEN].rcSrc.left = 142;
		Bmp[GUYAUFSTEHEN].rcSrc.right = 142 + 9;
		Bmp[GUYAUFSTEHEN].rcSrc.top = 0;
		Bmp[GUYAUFSTEHEN].rcSrc.bottom = 0 + 18;
		Bmp[GUYAUFSTEHEN].Breite = 9;
		Bmp[GUYAUFSTEHEN].Hoehe = 18;

		Bmp[GUYANGELN1LINKS].Anzahl = 6;
		Bmp[GUYANGELN1LINKS].Geschwindigkeit = 6;
		Bmp[GUYANGELN1LINKS].rcSrc.left = 151;
		Bmp[GUYANGELN1LINKS].rcSrc.right = 151 + 16;
		Bmp[GUYANGELN1LINKS].rcSrc.top = 0;
		Bmp[GUYANGELN1LINKS].rcSrc.bottom = 0 + 17;
		Bmp[GUYANGELN1LINKS].Breite = 16;
		Bmp[GUYANGELN1LINKS].Hoehe = 17;

		Bmp[GUYANGELN1OBEN].Anzahl = 6;
		Bmp[GUYANGELN1OBEN].Geschwindigkeit = 6;
		Bmp[GUYANGELN1OBEN].rcSrc.left = 167;
		Bmp[GUYANGELN1OBEN].rcSrc.right = 167 + 16;
		Bmp[GUYANGELN1OBEN].rcSrc.top = 0;
		Bmp[GUYANGELN1OBEN].rcSrc.bottom = 0 + 17;
		Bmp[GUYANGELN1OBEN].Breite = 16;
		Bmp[GUYANGELN1OBEN].Hoehe = 17;

		Bmp[GUYANGELN1RECHTS].Anzahl = 6;
		Bmp[GUYANGELN1RECHTS].Geschwindigkeit = 6;
		Bmp[GUYANGELN1RECHTS].rcSrc.left = 183;
		Bmp[GUYANGELN1RECHTS].rcSrc.right = 183 + 14;
		Bmp[GUYANGELN1RECHTS].rcSrc.top = 0;
		Bmp[GUYANGELN1RECHTS].rcSrc.bottom = 0 + 17;
		Bmp[GUYANGELN1RECHTS].Breite = 14;
		Bmp[GUYANGELN1RECHTS].Hoehe = 17;

		Bmp[GUYANGELN1UNTEN].Anzahl = 6;
		Bmp[GUYANGELN1UNTEN].Geschwindigkeit = 6;
		Bmp[GUYANGELN1UNTEN].rcSrc.left = 197;
		Bmp[GUYANGELN1UNTEN].rcSrc.right = 197 + 14;
		Bmp[GUYANGELN1UNTEN].rcSrc.top = 0;
		Bmp[GUYANGELN1UNTEN].rcSrc.bottom = 0 + 17;
		Bmp[GUYANGELN1UNTEN].Breite = 14;
		Bmp[GUYANGELN1UNTEN].Hoehe = 17;

		Bmp[GUYANGELN2LINKS].Anzahl = 4;
		Bmp[GUYANGELN2LINKS].Geschwindigkeit = 3;
		Bmp[GUYANGELN2LINKS].rcSrc.left = 211;
		Bmp[GUYANGELN2LINKS].rcSrc.right = 211 + 16;
		Bmp[GUYANGELN2LINKS].rcSrc.top = 0;
		Bmp[GUYANGELN2LINKS].rcSrc.bottom = 0 + 16;
		Bmp[GUYANGELN2LINKS].Breite = 16;
		Bmp[GUYANGELN2LINKS].Hoehe = 16;

		Bmp[GUYANGELN2OBEN].Anzahl = 4;
		Bmp[GUYANGELN2OBEN].Geschwindigkeit = 3;
		Bmp[GUYANGELN2OBEN].rcSrc.left = 227;
		Bmp[GUYANGELN2OBEN].rcSrc.right = 227 + 16;
		Bmp[GUYANGELN2OBEN].rcSrc.top = 0;
		Bmp[GUYANGELN2OBEN].rcSrc.bottom = 0 + 16;
		Bmp[GUYANGELN2OBEN].Breite = 16;
		Bmp[GUYANGELN2OBEN].Hoehe = 16;

		Bmp[GUYANGELN2RECHTS].Anzahl = 4;
		Bmp[GUYANGELN2RECHTS].Geschwindigkeit = 3;
		Bmp[GUYANGELN2RECHTS].rcSrc.left = 243;
		Bmp[GUYANGELN2RECHTS].rcSrc.right = 243 + 14;
		Bmp[GUYANGELN2RECHTS].rcSrc.top = 0;
		Bmp[GUYANGELN2RECHTS].rcSrc.bottom = 0 + 15;
		Bmp[GUYANGELN2RECHTS].Breite = 14;
		Bmp[GUYANGELN2RECHTS].Hoehe = 15;

		Bmp[GUYANGELN2UNTEN].Anzahl = 4;
		Bmp[GUYANGELN2UNTEN].Geschwindigkeit = 3;
		Bmp[GUYANGELN2UNTEN].rcSrc.left = 257;
		Bmp[GUYANGELN2UNTEN].rcSrc.right = 257 + 14;
		Bmp[GUYANGELN2UNTEN].rcSrc.top = 0;
		Bmp[GUYANGELN2UNTEN].rcSrc.bottom = 0 + 15;
		Bmp[GUYANGELN2UNTEN].Breite = 14;
		Bmp[GUYANGELN2UNTEN].Hoehe = 15;

		Bmp[GUYANGELN3LINKS].Anzahl = 3;
		Bmp[GUYANGELN3LINKS].Geschwindigkeit = 2;
		Bmp[GUYANGELN3LINKS].rcSrc.left = 271;
		Bmp[GUYANGELN3LINKS].rcSrc.right = 271 + 16;
		Bmp[GUYANGELN3LINKS].rcSrc.top = 0;
		Bmp[GUYANGELN3LINKS].rcSrc.bottom = 0 + 16;
		Bmp[GUYANGELN3LINKS].Breite = 16;
		Bmp[GUYANGELN3LINKS].Hoehe = 16;

		Bmp[GUYANGELN3OBEN].Anzahl = 3;
		Bmp[GUYANGELN3OBEN].Geschwindigkeit = 2;
		Bmp[GUYANGELN3OBEN].rcSrc.left = 285;
		Bmp[GUYANGELN3OBEN].rcSrc.right = 285 + 16;
		Bmp[GUYANGELN3OBEN].rcSrc.top = 0;
		Bmp[GUYANGELN3OBEN].rcSrc.bottom = 0 + 16;
		Bmp[GUYANGELN3OBEN].Breite = 16;
		Bmp[GUYANGELN3OBEN].Hoehe = 16;

		Bmp[GUYANGELN3RECHTS].Anzahl = 3;
		Bmp[GUYANGELN3RECHTS].Geschwindigkeit = 2;
		Bmp[GUYANGELN3RECHTS].rcSrc.left = 299;
		Bmp[GUYANGELN3RECHTS].rcSrc.right = 299 + 14;
		Bmp[GUYANGELN3RECHTS].rcSrc.top = 0;
		Bmp[GUYANGELN3RECHTS].rcSrc.bottom = 0 + 15;
		Bmp[GUYANGELN3RECHTS].Breite = 14;
		Bmp[GUYANGELN3RECHTS].Hoehe = 15;

		Bmp[GUYANGELN3UNTEN].Anzahl = 3;
		Bmp[GUYANGELN3UNTEN].Geschwindigkeit = 2;
		Bmp[GUYANGELN3UNTEN].rcSrc.left = 313;
		Bmp[GUYANGELN3UNTEN].rcSrc.right = 313 + 14;
		Bmp[GUYANGELN3UNTEN].rcSrc.top = 0;
		Bmp[GUYANGELN3UNTEN].rcSrc.bottom = 0 + 15;
		Bmp[GUYANGELN3UNTEN].Breite = 14;
		Bmp[GUYANGELN3UNTEN].Hoehe = 15;

		Bmp[GUYSCHLAGEN].Anzahl = 4;
		Bmp[GUYSCHLAGEN].Geschwindigkeit = 7;
		Bmp[GUYSCHLAGEN].rcSrc.left = 327;
		Bmp[GUYSCHLAGEN].rcSrc.right = 327 + 12;
		Bmp[GUYSCHLAGEN].rcSrc.top = 0;
		Bmp[GUYSCHLAGEN].rcSrc.bottom = 0 + 24;
		Bmp[GUYSCHLAGEN].Breite = 12;
		Bmp[GUYSCHLAGEN].Hoehe = 24;
		Bmp[GUYSCHLAGEN].Sound = Sound::HIT;

		Bmp[GUYBOOTLINKS].Anzahl = 6;
		Bmp[GUYBOOTLINKS].Geschwindigkeit = 10;
		Bmp[GUYBOOTLINKS].rcSrc.left = 339;
		Bmp[GUYBOOTLINKS].rcSrc.right = 339 + 26;
		Bmp[GUYBOOTLINKS].rcSrc.top = 0;
		Bmp[GUYBOOTLINKS].rcSrc.bottom = 0 + 21;
		Bmp[GUYBOOTLINKS].Breite = 26;
		Bmp[GUYBOOTLINKS].Hoehe = 21;

		Bmp[GUYBOOTOBEN].Anzahl = 6;
		Bmp[GUYBOOTOBEN].Geschwindigkeit = 10;
		Bmp[GUYBOOTOBEN].rcSrc.left = 365;
		Bmp[GUYBOOTOBEN].rcSrc.right = 365 + 26;
		Bmp[GUYBOOTOBEN].rcSrc.top = 0;
		Bmp[GUYBOOTOBEN].rcSrc.bottom = 0 + 21;
		Bmp[GUYBOOTOBEN].Breite = 26;
		Bmp[GUYBOOTOBEN].Hoehe = 21;

		Bmp[GUYBOOTRECHTS].Anzahl = 6;
		Bmp[GUYBOOTRECHTS].Geschwindigkeit = 10;
		Bmp[GUYBOOTRECHTS].rcSrc.left = 391;
		Bmp[GUYBOOTRECHTS].rcSrc.right = 391 + 26;
		Bmp[GUYBOOTRECHTS].rcSrc.top = 0;
		Bmp[GUYBOOTRECHTS].rcSrc.bottom = 0 + 21;
		Bmp[GUYBOOTRECHTS].Breite = 26;
		Bmp[GUYBOOTRECHTS].Hoehe = 21;

		Bmp[GUYBOOTUNTEN].Anzahl = 6;
		Bmp[GUYBOOTUNTEN].Geschwindigkeit = 10;
		Bmp[GUYBOOTUNTEN].rcSrc.left = 417;
		Bmp[GUYBOOTUNTEN].rcSrc.right = 417 + 26;
		Bmp[GUYBOOTUNTEN].rcSrc.top = 0;
		Bmp[GUYBOOTUNTEN].rcSrc.bottom = 0 + 21;
		Bmp[GUYBOOTUNTEN].Breite = 26;
		Bmp[GUYBOOTUNTEN].Hoehe = 21;

		Bmp[GUYBOOTANGELN1].Anzahl = 6;
		Bmp[GUYBOOTANGELN1].Geschwindigkeit = 6;
		Bmp[GUYBOOTANGELN1].rcSrc.left = 443;
		Bmp[GUYBOOTANGELN1].rcSrc.right = 443 + 26;
		Bmp[GUYBOOTANGELN1].rcSrc.top = 0;
		Bmp[GUYBOOTANGELN1].rcSrc.bottom = 0 + 25;
		Bmp[GUYBOOTANGELN1].Breite = 26;
		Bmp[GUYBOOTANGELN1].Hoehe = 25;

		Bmp[GUYBOOTANGELN2].Anzahl = 4;
		Bmp[GUYBOOTANGELN2].Geschwindigkeit = 3;
		Bmp[GUYBOOTANGELN2].rcSrc.left = 469;
		Bmp[GUYBOOTANGELN2].rcSrc.right = 469 + 26;
		Bmp[GUYBOOTANGELN2].rcSrc.top = 0;
		Bmp[GUYBOOTANGELN2].rcSrc.bottom = 0 + 25;
		Bmp[GUYBOOTANGELN2].Breite = 26;
		Bmp[GUYBOOTANGELN2].Hoehe = 25;

		Bmp[GUYBOOTANGELN3].Anzahl = 3;
		Bmp[GUYBOOTANGELN3].Geschwindigkeit = 2;
		Bmp[GUYBOOTANGELN3].rcSrc.left = 495;
		Bmp[GUYBOOTANGELN3].rcSrc.right = 495 + 26;
		Bmp[GUYBOOTANGELN3].rcSrc.top = 0;
		Bmp[GUYBOOTANGELN3].rcSrc.bottom = 0 + 25;
		Bmp[GUYBOOTANGELN3].Breite = 26;
		Bmp[GUYBOOTANGELN3].Hoehe = 25;

		Bmp[GUYTAUCHEN1].Anzahl = 5;
		Bmp[GUYTAUCHEN1].Geschwindigkeit = 5;
		Bmp[GUYTAUCHEN1].rcSrc.left = 521;
		Bmp[GUYTAUCHEN1].rcSrc.right = 521 + 26;
		Bmp[GUYTAUCHEN1].rcSrc.top = 0;
		Bmp[GUYTAUCHEN1].rcSrc.bottom = 0 + 27;
		Bmp[GUYTAUCHEN1].Breite = 26;
		Bmp[GUYTAUCHEN1].Hoehe = 27;

		Bmp[GUYTAUCHEN2].Anzahl = 4;
		Bmp[GUYTAUCHEN2].Geschwindigkeit = 3;
		Bmp[GUYTAUCHEN2].rcSrc.left = 547;
		Bmp[GUYTAUCHEN2].rcSrc.right = 547 + 26;
		Bmp[GUYTAUCHEN2].rcSrc.top = 0;
		Bmp[GUYTAUCHEN2].rcSrc.bottom = 0 + 17;
		Bmp[GUYTAUCHEN2].Breite = 26;
		Bmp[GUYTAUCHEN2].Hoehe = 17;

		Bmp[GUYTAUCHEN3].Anzahl = 2;
		Bmp[GUYTAUCHEN3].Geschwindigkeit = 2;
		Bmp[GUYTAUCHEN3].rcSrc.left = 573;
		Bmp[GUYTAUCHEN3].rcSrc.right = 573 + 26;
		Bmp[GUYTAUCHEN3].rcSrc.top = 0;
		Bmp[GUYTAUCHEN3].rcSrc.bottom = 0 + 17;
		Bmp[GUYTAUCHEN3].Breite = 26;
		Bmp[GUYTAUCHEN3].Hoehe = 17;

		Bmp[GUYHAMMER].Anzahl = 2;
		Bmp[GUYHAMMER].Geschwindigkeit = 4;
		Bmp[GUYHAMMER].rcSrc.left = 599;
		Bmp[GUYHAMMER].rcSrc.right = 599 + 9;
		Bmp[GUYHAMMER].rcSrc.top = 0;
		Bmp[GUYHAMMER].rcSrc.bottom = 0 + 18;
		Bmp[GUYHAMMER].Breite = 9;
		Bmp[GUYHAMMER].Hoehe = 18;
		Bmp[GUYHAMMER].Sound = Sound::HAMMER;

		Bmp[GUYKLETTERN1].Anzahl = 6;
		Bmp[GUYKLETTERN1].Geschwindigkeit = 2;
		Bmp[GUYKLETTERN1].rcSrc.left = 608;
		Bmp[GUYKLETTERN1].rcSrc.right = 608 + 7;
		Bmp[GUYKLETTERN1].rcSrc.top = 0;
		Bmp[GUYKLETTERN1].rcSrc.bottom = 0 + 34;
		Bmp[GUYKLETTERN1].Breite = 7;
		Bmp[GUYKLETTERN1].Hoehe = 34;

		Bmp[GUYKLETTERN2].Anzahl = 6;
		Bmp[GUYKLETTERN2].Geschwindigkeit = 2;
		Bmp[GUYKLETTERN2].rcSrc.left = 615;
		Bmp[GUYKLETTERN2].rcSrc.right = 615 + 7;
		Bmp[GUYKLETTERN2].rcSrc.top = 0;
		Bmp[GUYKLETTERN2].rcSrc.bottom = 0 + 34;
		Bmp[GUYKLETTERN2].Breite = 7;
		Bmp[GUYKLETTERN2].Hoehe = 34;

		Bmp[GUYHAMMER2].Anzahl = 2;
		Bmp[GUYHAMMER2].Geschwindigkeit = 4;
		Bmp[GUYHAMMER2].rcSrc.left = 622;
		Bmp[GUYHAMMER2].rcSrc.right = 622 + 7;
		Bmp[GUYHAMMER2].rcSrc.top = 0;
		Bmp[GUYHAMMER2].rcSrc.bottom = 0 + 34;
		Bmp[GUYHAMMER2].Breite = 7;
		Bmp[GUYHAMMER2].Hoehe = 34;
		Bmp[GUYHAMMER2].Sound = Sound::HAMMER;

		Bmp[GUYGEHINHAUS].Anzahl = 3;
		Bmp[GUYGEHINHAUS].Geschwindigkeit = 2;
		Bmp[GUYGEHINHAUS].rcSrc.left = 631;
		Bmp[GUYGEHINHAUS].rcSrc.right = 631 + 9;
		Bmp[GUYGEHINHAUS].rcSrc.top = 0;
		Bmp[GUYGEHINHAUS].rcSrc.bottom = 0 + 34;
		Bmp[GUYGEHINHAUS].Breite = 9;
		Bmp[GUYGEHINHAUS].Hoehe = 34;

		Bmp[GUYSCHLAFHAUS].Anzahl = 2;
		Bmp[GUYSCHLAFHAUS].Geschwindigkeit = 1;
		Bmp[GUYSCHLAFHAUS].rcSrc.left = 640;
		Bmp[GUYSCHLAFHAUS].rcSrc.right = 640 + 10;
		Bmp[GUYSCHLAFHAUS].rcSrc.top = 0;
		Bmp[GUYSCHLAFHAUS].rcSrc.bottom = 0 + 34;
		Bmp[GUYSCHLAFHAUS].Breite = 10;
		Bmp[GUYSCHLAFHAUS].Hoehe = 34;
		Bmp[GUYSCHLAFHAUS].Sound = Sound::SNORE;

		Bmp[GUYGEHAUSHAUS].Anzahl = 3;
		Bmp[GUYGEHAUSHAUS].Geschwindigkeit = 2;
		Bmp[GUYGEHAUSHAUS].rcSrc.left = 650;
		Bmp[GUYGEHAUSHAUS].rcSrc.right = 650 + 9;
		Bmp[GUYGEHAUSHAUS].rcSrc.top = 0;
		Bmp[GUYGEHAUSHAUS].rcSrc.bottom = 0 + 34;
		Bmp[GUYGEHAUSHAUS].Breite = 9;
		Bmp[GUYGEHAUSHAUS].Hoehe = 34;

		Bmp[GUYANZUENDEN].Anzahl = 6;
		Bmp[GUYANZUENDEN].Geschwindigkeit = 5;
		Bmp[GUYANZUENDEN].rcSrc.left = 659;
		Bmp[GUYANZUENDEN].rcSrc.right = 659 + 19;
		Bmp[GUYANZUENDEN].rcSrc.top = 0;
		Bmp[GUYANZUENDEN].rcSrc.bottom = 0 + 18;
		Bmp[GUYANZUENDEN].Breite = 19;
		Bmp[GUYANZUENDEN].Hoehe = 18;

		Bmp[GUYAUSSCHAU].Anzahl = 4;
		Bmp[GUYAUSSCHAU].Geschwindigkeit = 1;
		Bmp[GUYAUSSCHAU].rcSrc.left = 678;
		Bmp[GUYAUSSCHAU].rcSrc.right = 678 + 10;
		Bmp[GUYAUSSCHAU].rcSrc.top = 0;
		Bmp[GUYAUSSCHAU].rcSrc.bottom = 0 + 18;
		Bmp[GUYAUSSCHAU].Breite = 10;
		Bmp[GUYAUSSCHAU].Hoehe = 18;

		Bmp[GUYSCHAUFELN].Anzahl = 10;
		Bmp[GUYSCHAUFELN].Geschwindigkeit = 3;
		Bmp[GUYSCHAUFELN].rcSrc.left = 688;
		Bmp[GUYSCHAUFELN].rcSrc.right = 688 + 17;
		Bmp[GUYSCHAUFELN].rcSrc.top = 0;
		Bmp[GUYSCHAUFELN].rcSrc.bottom = 0 + 19;
		Bmp[GUYSCHAUFELN].Breite = 17;
		Bmp[GUYSCHAUFELN].Hoehe = 19;
		Bmp[GUYSCHAUFELN].Sound = Sound::DIG;

		Bmp[GUYSCHIFF].Anzahl = 4;
		Bmp[GUYSCHIFF].Geschwindigkeit = 10;
		Bmp[GUYSCHIFF].rcSrc.left = 297;
		Bmp[GUYSCHIFF].rcSrc.right = 297 + 48;
		Bmp[GUYSCHIFF].rcSrc.top = 0;
		Bmp[GUYSCHIFF].rcSrc.bottom = 0 + 38;
		Bmp[GUYSCHIFF].Breite = 48;
		Bmp[GUYSCHIFF].Hoehe = 38;
		Bmp[GUYSCHIFF].Surface = lpDDSBau;
		Bmp[GUYSCHIFF].Sound = Sound::STORM;

		Bmp[GUYSCHIFFDOWN].Anzahl = 6;
		Bmp[GUYSCHIFFDOWN].Geschwindigkeit = 3;
		Bmp[GUYSCHIFFDOWN].rcSrc.left = 345;
		Bmp[GUYSCHIFFDOWN].rcSrc.right = 345 + 46;
		Bmp[GUYSCHIFFDOWN].rcSrc.top = 0;
		Bmp[GUYSCHIFFDOWN].rcSrc.bottom = 0 + 40;
		Bmp[GUYSCHIFFDOWN].Breite = 46;
		Bmp[GUYSCHIFFDOWN].Hoehe = 40;
		Bmp[GUYSCHIFFDOWN].Surface = lpDDSBau;
		Bmp[GUYSCHIFFDOWN].Sound = Sound::SPLAT;

		Bmp[GUYSCHWIMMEN].Anzahl = 4;
		Bmp[GUYSCHWIMMEN].Geschwindigkeit = 10;
		Bmp[GUYSCHWIMMEN].rcSrc.left = 705;
		Bmp[GUYSCHWIMMEN].rcSrc.right = 705 + 15;
		Bmp[GUYSCHWIMMEN].rcSrc.top = 0;
		Bmp[GUYSCHWIMMEN].rcSrc.bottom = 0 + 9;
		Bmp[GUYSCHWIMMEN].Breite = 15;
		Bmp[GUYSCHWIMMEN].Hoehe = 9;
		Bmp[GUYSCHWIMMEN].Sound = Sound::SWIM;

		Bmp[GUYTOD].Anzahl = 6;
		Bmp[GUYTOD].Geschwindigkeit = 1;
		Bmp[GUYTOD].rcSrc.left = 743;
		Bmp[GUYTOD].rcSrc.right = 743 + 16;
		Bmp[GUYTOD].rcSrc.top = 0;
		Bmp[GUYTOD].rcSrc.bottom = 0 + 10;
		Bmp[GUYTOD].Breite = 16;
		Bmp[GUYTOD].Hoehe = 10;

		Bmp[GUYBOOTTOD].Anzahl = 6;
		Bmp[GUYBOOTTOD].Geschwindigkeit = 1;
		Bmp[GUYBOOTTOD].rcSrc.left = 759;
		Bmp[GUYBOOTTOD].rcSrc.right = 759 + 26;
		Bmp[GUYBOOTTOD].rcSrc.top = 0;
		Bmp[GUYBOOTTOD].rcSrc.bottom = 0 + 18;
		Bmp[GUYBOOTTOD].Breite = 26;
		Bmp[GUYBOOTTOD].Hoehe = 18;

		Bmp[GUYBOOTWARTEN].Anzahl = 4;
		Bmp[GUYBOOTWARTEN].Geschwindigkeit = 2;
		Bmp[GUYBOOTWARTEN].rcSrc.left = 0;
		Bmp[GUYBOOTWARTEN].rcSrc.right = 0 + 26;
		Bmp[GUYBOOTWARTEN].rcSrc.top = 72;
		Bmp[GUYBOOTWARTEN].rcSrc.bottom = 72 + 18;
		Bmp[GUYBOOTWARTEN].Breite = 26;
		Bmp[GUYBOOTWARTEN].Hoehe = 20;

		Bmp[GUYSCHLEUDER].Anzahl = 5;
		Bmp[GUYSCHLEUDER].Geschwindigkeit = 4;
		Bmp[GUYSCHLEUDER].rcSrc.left = 720;
		Bmp[GUYSCHLEUDER].rcSrc.right = 720 + 23;
		Bmp[GUYSCHLEUDER].rcSrc.top = 0;
		Bmp[GUYSCHLEUDER].rcSrc.bottom = 0 + 20;
		Bmp[GUYSCHLEUDER].Breite = 23;
		Bmp[GUYSCHLEUDER].Hoehe = 20;

		// Cursor
		for (i = CUPFEIL; i <= CUUHR; i++)
		{
			Bmp[i].Animation = false;
			Bmp[i].Anzahl = 1;
			Bmp[i].Geschwindigkeit = 0;
			Bmp[i].Phase = 0;
			Bmp[i].Surface = lpDDSCursor;
			Bmp[i].rcSrc.left = (i - CUPFEIL) * 18;
			Bmp[i].rcSrc.top = 0;
			Bmp[i].rcSrc.right = Bmp[i].rcSrc.left + 18;
			Bmp[i].rcSrc.bottom = 18;
			Bmp[i].Breite = 18;
			Bmp[i].Hoehe = 18;
		}

		// Landschaftsanimationen
		for (i = MEERWELLEN; i <= SCHLEUSE6; i++)
		{
			Bmp[i].Animation = true;
			Bmp[i].Anzahl = 7;
			Bmp[i].Geschwindigkeit = 7;
			Bmp[i].Phase = 0;
			Bmp[i].Surface = lpDDSAnimation;
			Bmp[i].Sound = Sound::RIVER;
		}

		Bmp[MEERWELLEN].Geschwindigkeit = 3;
		Bmp[MEERWELLEN].Breite = 28;
		Bmp[MEERWELLEN].Hoehe = 13;
		Bmp[MEERWELLEN].rcSrc.left = 0;
		Bmp[MEERWELLEN].rcSrc.right = Bmp[MEERWELLEN].rcSrc.left + Bmp[MEERWELLEN].Breite;
		Bmp[MEERWELLEN].rcSrc.top = 0;
		Bmp[MEERWELLEN].rcSrc.bottom = Bmp[MEERWELLEN].rcSrc.top + Bmp[MEERWELLEN].Hoehe;
		Bmp[MEERWELLEN].rcDes.left = 9;
		Bmp[MEERWELLEN].rcDes.right = Bmp[MEERWELLEN].rcDes.left + Bmp[MEERWELLEN].Breite;
		Bmp[MEERWELLEN].rcDes.top = 23;
		Bmp[MEERWELLEN].rcDes.bottom = Bmp[MEERWELLEN].rcDes.top + Bmp[MEERWELLEN].Hoehe;
		Bmp[MEERWELLEN].Sound = Sound::SURF;

		Bmp[FLUSS1].Breite = 34;
		Bmp[FLUSS1].Hoehe = 8;
		Bmp[FLUSS1].rcSrc.left = 28;
		Bmp[FLUSS1].rcSrc.right = Bmp[FLUSS1].rcSrc.left + Bmp[FLUSS1].Breite;
		Bmp[FLUSS1].rcSrc.top = 0;
		Bmp[FLUSS1].rcSrc.bottom = Bmp[FLUSS1].rcSrc.top + Bmp[FLUSS1].Hoehe;
		Bmp[FLUSS1].rcDes.left = 10;
		Bmp[FLUSS1].rcDes.right = Bmp[FLUSS1].rcDes.left + Bmp[FLUSS1].Breite;
		Bmp[FLUSS1].rcDes.top = 18;
		Bmp[FLUSS1].rcDes.bottom = Bmp[FLUSS1].rcDes.top + Bmp[FLUSS1].Hoehe;

		Bmp[FLUSS2].Breite = 34;
		Bmp[FLUSS2].Hoehe = 8;
		Bmp[FLUSS2].rcSrc.left = 62;
		Bmp[FLUSS2].rcSrc.right = Bmp[FLUSS2].rcSrc.left + Bmp[FLUSS2].Breite;
		Bmp[FLUSS2].rcSrc.top = 0;
		Bmp[FLUSS2].rcSrc.bottom = Bmp[FLUSS2].rcSrc.top + Bmp[FLUSS2].Hoehe;
		Bmp[FLUSS2].rcDes.left = 10;
		Bmp[FLUSS2].rcDes.right = Bmp[FLUSS2].rcDes.left + Bmp[FLUSS2].Breite;
		Bmp[FLUSS2].rcDes.top = 19;
		Bmp[FLUSS2].rcDes.bottom = Bmp[FLUSS2].rcDes.top + Bmp[FLUSS2].Hoehe;

		Bmp[FLUSS3].Breite = 34;
		Bmp[FLUSS3].Hoehe = 34;
		Bmp[FLUSS3].rcSrc.left = 96;
		Bmp[FLUSS3].rcSrc.right = Bmp[FLUSS3].rcSrc.left + Bmp[FLUSS3].Breite;
		Bmp[FLUSS3].rcSrc.top = 0;
		Bmp[FLUSS3].rcSrc.bottom = Bmp[FLUSS3].rcSrc.top + Bmp[FLUSS3].Hoehe;
		Bmp[FLUSS3].rcDes.left = 10;
		Bmp[FLUSS3].rcDes.right = Bmp[FLUSS3].rcDes.left + Bmp[FLUSS3].Breite;
		Bmp[FLUSS3].rcDes.top = 5;
		Bmp[FLUSS3].rcDes.bottom = Bmp[FLUSS3].rcDes.top + Bmp[FLUSS3].Hoehe;

		Bmp[FLUSS4].Breite = 34;
		Bmp[FLUSS4].Hoehe = 34;
		Bmp[FLUSS4].rcSrc.left = 130;
		Bmp[FLUSS4].rcSrc.right = Bmp[FLUSS4].rcSrc.left + Bmp[FLUSS4].Breite;
		Bmp[FLUSS4].rcSrc.top = 0;
		Bmp[FLUSS4].rcSrc.bottom = Bmp[FLUSS4].rcSrc.top + Bmp[FLUSS4].Hoehe;
		Bmp[FLUSS4].rcDes.left = 10;
		Bmp[FLUSS4].rcDes.right = Bmp[FLUSS4].rcDes.left + Bmp[FLUSS4].Breite;
		Bmp[FLUSS4].rcDes.top = 5;
		Bmp[FLUSS4].rcDes.bottom = Bmp[FLUSS4].rcDes.top + Bmp[FLUSS4].Hoehe;

		Bmp[FLUSS5].Breite = 34;
		Bmp[FLUSS5].Hoehe = 18;
		Bmp[FLUSS5].rcSrc.left = 164;
		Bmp[FLUSS5].rcSrc.right = Bmp[FLUSS5].rcSrc.left + Bmp[FLUSS5].Breite;
		Bmp[FLUSS5].rcSrc.top = 0;
		Bmp[FLUSS5].rcSrc.bottom = Bmp[FLUSS5].rcSrc.top + Bmp[FLUSS5].Hoehe;
		Bmp[FLUSS5].rcDes.left = 10;
		Bmp[FLUSS5].rcDes.right = Bmp[FLUSS5].rcDes.left + Bmp[FLUSS5].Breite;
		Bmp[FLUSS5].rcDes.top = 21;
		Bmp[FLUSS5].rcDes.bottom = Bmp[FLUSS5].rcDes.top + Bmp[FLUSS5].Hoehe;

		Bmp[FLUSS6].Breite = 34;
		Bmp[FLUSS6].Hoehe = 18;
		Bmp[FLUSS6].rcSrc.left = 198;
		Bmp[FLUSS6].rcSrc.right = Bmp[FLUSS6].rcSrc.left + Bmp[FLUSS6].Breite;
		Bmp[FLUSS6].rcSrc.top = 0;
		Bmp[FLUSS6].rcSrc.bottom = Bmp[FLUSS6].rcSrc.top + Bmp[FLUSS6].Hoehe;
		Bmp[FLUSS6].rcDes.left = 10;
		Bmp[FLUSS6].rcDes.right = Bmp[FLUSS6].rcDes.left + Bmp[FLUSS6].Breite;
		Bmp[FLUSS6].rcDes.top = 21;
		Bmp[FLUSS6].rcDes.bottom = Bmp[FLUSS6].rcDes.top + Bmp[FLUSS6].Hoehe;

		Bmp[FLUSS7].Breite = 18;
		Bmp[FLUSS7].Hoehe = 18;
		Bmp[FLUSS7].rcSrc.left = 232;
		Bmp[FLUSS7].rcSrc.right = Bmp[FLUSS7].rcSrc.left + Bmp[FLUSS7].Breite;
		Bmp[FLUSS7].rcSrc.top = 0;
		Bmp[FLUSS7].rcSrc.bottom = Bmp[FLUSS7].rcSrc.top + Bmp[FLUSS7].Hoehe;
		Bmp[FLUSS7].rcDes.left = 10;
		Bmp[FLUSS7].rcDes.right = Bmp[FLUSS7].rcDes.left + Bmp[FLUSS7].Breite;
		Bmp[FLUSS7].rcDes.top = 21;
		Bmp[FLUSS7].rcDes.bottom = Bmp[FLUSS7].rcDes.top + Bmp[FLUSS7].Hoehe;

		Bmp[FLUSS8].Breite = 18;
		Bmp[FLUSS8].Hoehe = 18;
		Bmp[FLUSS8].rcSrc.left = 250;
		Bmp[FLUSS8].rcSrc.right = Bmp[FLUSS8].rcSrc.left + Bmp[FLUSS8].Breite;
		Bmp[FLUSS8].rcSrc.top = 0;
		Bmp[FLUSS8].rcSrc.bottom = Bmp[FLUSS8].rcSrc.top + Bmp[FLUSS8].Hoehe;
		Bmp[FLUSS8].rcDes.left = 26;
		Bmp[FLUSS8].rcDes.right = Bmp[FLUSS8].rcDes.left + Bmp[FLUSS8].Breite;
		Bmp[FLUSS8].rcDes.top = 21;
		Bmp[FLUSS8].rcDes.bottom = Bmp[FLUSS8].rcDes.top + Bmp[FLUSS8].Hoehe;

		Bmp[FLUSS9].Breite = 34;
		Bmp[FLUSS9].Hoehe = 15;
		Bmp[FLUSS9].rcSrc.left = 268;
		Bmp[FLUSS9].rcSrc.right = Bmp[FLUSS9].rcSrc.left + Bmp[FLUSS9].Breite;
		Bmp[FLUSS9].rcSrc.top = 0;
		Bmp[FLUSS9].rcSrc.bottom = Bmp[FLUSS9].rcSrc.top + Bmp[FLUSS9].Hoehe;
		Bmp[FLUSS9].rcDes.left = 10;
		Bmp[FLUSS9].rcDes.right = Bmp[FLUSS9].rcDes.left + Bmp[FLUSS9].Breite;
		Bmp[FLUSS9].rcDes.top = 20;
		Bmp[FLUSS9].rcDes.bottom = Bmp[FLUSS9].rcDes.top + Bmp[FLUSS9].Hoehe;

		Bmp[FLUSS10].Breite = 34;
		Bmp[FLUSS10].Hoehe = 13;
		Bmp[FLUSS10].rcSrc.left = 302;
		Bmp[FLUSS10].rcSrc.right = Bmp[FLUSS10].rcSrc.left + Bmp[FLUSS10].Breite;
		Bmp[FLUSS10].rcSrc.top = 0;
		Bmp[FLUSS10].rcSrc.bottom = Bmp[FLUSS10].rcSrc.top + Bmp[FLUSS10].Hoehe;
		Bmp[FLUSS10].rcDes.left = 10;
		Bmp[FLUSS10].rcDes.right = Bmp[FLUSS10].rcDes.left + Bmp[FLUSS10].Breite;
		Bmp[FLUSS10].rcDes.top = 26;
		Bmp[FLUSS10].rcDes.bottom = Bmp[FLUSS10].rcDes.top + Bmp[FLUSS10].Hoehe;

		Bmp[MUENDUNG1].Breite = 42;
		Bmp[MUENDUNG1].Hoehe = 22;
		Bmp[MUENDUNG1].rcSrc.left = 336;
		Bmp[MUENDUNG1].rcSrc.right = Bmp[MUENDUNG1].rcSrc.left + Bmp[MUENDUNG1].Breite;
		Bmp[MUENDUNG1].rcSrc.top = 0;
		Bmp[MUENDUNG1].rcSrc.bottom = Bmp[MUENDUNG1].rcSrc.top + Bmp[MUENDUNG1].Hoehe;
		Bmp[MUENDUNG1].rcDes.left = 2;
		Bmp[MUENDUNG1].rcDes.right = Bmp[MUENDUNG1].rcDes.left + Bmp[MUENDUNG1].Breite;
		Bmp[MUENDUNG1].rcDes.top = 17;
		Bmp[MUENDUNG1].rcDes.bottom = Bmp[MUENDUNG1].rcDes.top + Bmp[MUENDUNG1].Hoehe;

		Bmp[MUENDUNG2].Breite = 40;
		Bmp[MUENDUNG2].Hoehe = 22;
		Bmp[MUENDUNG2].rcSrc.left = 378;
		Bmp[MUENDUNG2].rcSrc.right = Bmp[MUENDUNG2].rcSrc.left + Bmp[MUENDUNG2].Breite;
		Bmp[MUENDUNG2].rcSrc.top = 0;
		Bmp[MUENDUNG2].rcSrc.bottom = Bmp[MUENDUNG2].rcSrc.top + Bmp[MUENDUNG2].Hoehe;
		Bmp[MUENDUNG2].rcDes.left = 10;
		Bmp[MUENDUNG2].rcDes.right = Bmp[MUENDUNG2].rcDes.left + Bmp[MUENDUNG2].Breite;
		Bmp[MUENDUNG2].rcDes.top = 17;
		Bmp[MUENDUNG2].rcDes.bottom = Bmp[MUENDUNG2].rcDes.top + Bmp[MUENDUNG2].Hoehe;

		Bmp[MUENDUNG3].Breite = 40;
		Bmp[MUENDUNG3].Hoehe = 22;
		Bmp[MUENDUNG3].rcSrc.left = 418;
		Bmp[MUENDUNG3].rcSrc.right = Bmp[MUENDUNG3].rcSrc.left + Bmp[MUENDUNG3].Breite;
		Bmp[MUENDUNG3].rcSrc.top = 0;
		Bmp[MUENDUNG3].rcSrc.bottom = Bmp[MUENDUNG3].rcSrc.top + Bmp[MUENDUNG3].Hoehe;
		Bmp[MUENDUNG3].rcDes.left = 10;
		Bmp[MUENDUNG3].rcDes.right = Bmp[MUENDUNG3].rcDes.left + Bmp[MUENDUNG3].Breite;
		Bmp[MUENDUNG3].rcDes.top = 21;
		Bmp[MUENDUNG3].rcDes.bottom = Bmp[MUENDUNG3].rcDes.top + Bmp[MUENDUNG3].Hoehe;

		Bmp[MUENDUNG4].Breite = 42;
		Bmp[MUENDUNG4].Hoehe = 22;
		Bmp[MUENDUNG4].rcSrc.left = 458;
		Bmp[MUENDUNG4].rcSrc.right = Bmp[MUENDUNG4].rcSrc.left + Bmp[MUENDUNG4].Breite;
		Bmp[MUENDUNG4].rcSrc.top = 0;
		Bmp[MUENDUNG4].rcSrc.bottom = Bmp[MUENDUNG4].rcSrc.top + Bmp[MUENDUNG4].Hoehe;
		Bmp[MUENDUNG4].rcDes.left = 2;
		Bmp[MUENDUNG4].rcDes.right = Bmp[MUENDUNG4].rcDes.left + Bmp[MUENDUNG4].Breite;
		Bmp[MUENDUNG4].rcDes.top = 21;
		Bmp[MUENDUNG4].rcDes.bottom = Bmp[MUENDUNG4].rcDes.top + Bmp[MUENDUNG4].Hoehe;

		Bmp[QUELLE1].Breite = 30;
		Bmp[QUELLE1].Hoehe = 19;
		Bmp[QUELLE1].rcSrc.left = 500;
		Bmp[QUELLE1].rcSrc.right = Bmp[QUELLE1].rcSrc.left + Bmp[QUELLE1].Breite;
		Bmp[QUELLE1].rcSrc.top = 0;
		Bmp[QUELLE1].rcSrc.bottom = Bmp[QUELLE1].rcSrc.top + Bmp[QUELLE1].Hoehe;
		Bmp[QUELLE1].rcDes.left = 16;
		Bmp[QUELLE1].rcDes.right = Bmp[QUELLE1].rcDes.left + Bmp[QUELLE1].Breite;
		Bmp[QUELLE1].rcDes.top = 19;
		Bmp[QUELLE1].rcDes.bottom = Bmp[QUELLE1].rcDes.top + Bmp[QUELLE1].Hoehe;

		Bmp[QUELLE2].Breite = 26;
		Bmp[QUELLE2].Hoehe = 19;
		Bmp[QUELLE2].rcSrc.left = 530;
		Bmp[QUELLE2].rcSrc.right = Bmp[QUELLE2].rcSrc.left + Bmp[QUELLE2].Breite;
		Bmp[QUELLE2].rcSrc.top = 0;
		Bmp[QUELLE2].rcSrc.bottom = Bmp[QUELLE2].rcSrc.top + Bmp[QUELLE2].Hoehe;
		Bmp[QUELLE2].rcDes.left = 9;
		Bmp[QUELLE2].rcDes.right = Bmp[QUELLE2].rcDes.left + Bmp[QUELLE2].Breite;
		Bmp[QUELLE2].rcDes.top = 20;
		Bmp[QUELLE2].rcDes.bottom = Bmp[QUELLE2].rcDes.top + Bmp[QUELLE2].Hoehe;

		Bmp[QUELLE3].Breite = 25;
		Bmp[QUELLE3].Hoehe = 16;
		Bmp[QUELLE3].rcSrc.left = 556;
		Bmp[QUELLE3].rcSrc.right = Bmp[QUELLE3].rcSrc.left + Bmp[QUELLE3].Breite;
		Bmp[QUELLE3].rcSrc.top = 0;
		Bmp[QUELLE3].rcSrc.bottom = Bmp[QUELLE3].rcSrc.top + Bmp[QUELLE3].Hoehe;
		Bmp[QUELLE3].rcDes.left = 19;
		Bmp[QUELLE3].rcDes.right = Bmp[QUELLE3].rcDes.left + Bmp[QUELLE3].Breite;
		Bmp[QUELLE3].rcDes.top = 19;
		Bmp[QUELLE3].rcDes.bottom = Bmp[QUELLE3].rcDes.top + Bmp[QUELLE3].Hoehe;

		Bmp[QUELLE4].Breite = 25;
		Bmp[QUELLE4].Hoehe = 15;
		Bmp[QUELLE4].rcSrc.left = 581;
		Bmp[QUELLE4].rcSrc.right = Bmp[QUELLE4].rcSrc.left + Bmp[QUELLE4].Breite;
		Bmp[QUELLE4].rcSrc.top = 0;
		Bmp[QUELLE4].rcSrc.bottom = Bmp[QUELLE4].rcSrc.top + Bmp[QUELLE4].Hoehe;
		Bmp[QUELLE4].rcDes.left = 8;
		Bmp[QUELLE4].rcDes.right = Bmp[QUELLE4].rcDes.left + Bmp[QUELLE4].Breite;
		Bmp[QUELLE4].rcDes.top = 21;
		Bmp[QUELLE4].rcDes.bottom = Bmp[QUELLE4].rcDes.top + Bmp[QUELLE4].Hoehe;

		Bmp[SCHLEUSE1].Breite = 35;
		Bmp[SCHLEUSE1].Hoehe = 22;
		Bmp[SCHLEUSE1].rcSrc.left = 164;
		Bmp[SCHLEUSE1].rcSrc.right = Bmp[SCHLEUSE1].rcSrc.left + Bmp[SCHLEUSE1].Breite;
		Bmp[SCHLEUSE1].rcSrc.top = 126;
		Bmp[SCHLEUSE1].rcSrc.bottom = Bmp[SCHLEUSE1].rcSrc.top + Bmp[SCHLEUSE1].Hoehe;
		Bmp[SCHLEUSE1].rcDes.left = 10;
		Bmp[SCHLEUSE1].rcDes.right = Bmp[SCHLEUSE1].rcDes.left + Bmp[SCHLEUSE1].Breite;
		Bmp[SCHLEUSE1].rcDes.top = 17;
		Bmp[SCHLEUSE1].rcDes.bottom = Bmp[SCHLEUSE1].rcDes.top + Bmp[SCHLEUSE1].Hoehe;

		Bmp[SCHLEUSE2].Breite = 34;
		Bmp[SCHLEUSE2].Hoehe = 23;
		Bmp[SCHLEUSE2].rcSrc.left = 199;
		Bmp[SCHLEUSE2].rcSrc.right = Bmp[SCHLEUSE2].rcSrc.left + Bmp[SCHLEUSE2].Breite;
		Bmp[SCHLEUSE2].rcSrc.top = 126;
		Bmp[SCHLEUSE2].rcSrc.bottom = Bmp[SCHLEUSE2].rcSrc.top + Bmp[SCHLEUSE2].Hoehe;
		Bmp[SCHLEUSE2].rcDes.left = 10;
		Bmp[SCHLEUSE2].rcDes.right = Bmp[SCHLEUSE2].rcDes.left + Bmp[SCHLEUSE2].Breite;
		Bmp[SCHLEUSE2].rcDes.top = 16;
		Bmp[SCHLEUSE2].rcDes.bottom = Bmp[SCHLEUSE2].rcDes.top + Bmp[SCHLEUSE2].Hoehe;

		Bmp[SCHLEUSE3].Breite = 34;
		Bmp[SCHLEUSE3].Hoehe = 22;
		Bmp[SCHLEUSE3].rcSrc.left = 233;
		Bmp[SCHLEUSE3].rcSrc.right = Bmp[SCHLEUSE3].rcSrc.left + Bmp[SCHLEUSE3].Breite;
		Bmp[SCHLEUSE3].rcSrc.top = 126;
		Bmp[SCHLEUSE3].rcSrc.bottom = Bmp[SCHLEUSE3].rcSrc.top + Bmp[SCHLEUSE3].Hoehe;
		Bmp[SCHLEUSE3].rcDes.left = 10;
		Bmp[SCHLEUSE3].rcDes.right = Bmp[SCHLEUSE3].rcDes.left + Bmp[SCHLEUSE3].Breite;
		Bmp[SCHLEUSE3].rcDes.top = 17;
		Bmp[SCHLEUSE3].rcDes.bottom = Bmp[SCHLEUSE3].rcDes.top + Bmp[SCHLEUSE3].Hoehe;

		Bmp[SCHLEUSE4].Breite = 33;
		Bmp[SCHLEUSE4].Hoehe = 23;
		Bmp[SCHLEUSE4].rcSrc.left = 268;
		Bmp[SCHLEUSE4].rcSrc.right = Bmp[SCHLEUSE4].rcSrc.left + Bmp[SCHLEUSE4].Breite;
		Bmp[SCHLEUSE4].rcSrc.top = 105;
		Bmp[SCHLEUSE4].rcSrc.bottom = Bmp[SCHLEUSE4].rcSrc.top + Bmp[SCHLEUSE4].Hoehe;
		Bmp[SCHLEUSE4].rcDes.left = 11;
		Bmp[SCHLEUSE4].rcDes.right = Bmp[SCHLEUSE4].rcDes.left + Bmp[SCHLEUSE4].Breite;
		Bmp[SCHLEUSE4].rcDes.top = 16;
		Bmp[SCHLEUSE4].rcDes.bottom = Bmp[SCHLEUSE4].rcDes.top + Bmp[SCHLEUSE4].Hoehe;

		Bmp[SCHLEUSE5].Breite = 34;
		Bmp[SCHLEUSE5].Hoehe = 17;
		Bmp[SCHLEUSE5].rcSrc.left = 302;
		Bmp[SCHLEUSE5].rcSrc.right = Bmp[SCHLEUSE5].rcSrc.left + Bmp[SCHLEUSE5].Breite;
		Bmp[SCHLEUSE5].rcSrc.top = 91;
		Bmp[SCHLEUSE5].rcSrc.bottom = Bmp[SCHLEUSE5].rcSrc.top + Bmp[SCHLEUSE5].Hoehe;
		Bmp[SCHLEUSE5].rcDes.left = 10;
		Bmp[SCHLEUSE5].rcDes.right = Bmp[SCHLEUSE5].rcDes.left + Bmp[SCHLEUSE5].Breite;
		Bmp[SCHLEUSE5].rcDes.top = 20;
		Bmp[SCHLEUSE5].rcDes.bottom = Bmp[SCHLEUSE5].rcDes.top + Bmp[SCHLEUSE5].Hoehe;

		Bmp[SCHLEUSE6].Breite = 35;
		Bmp[SCHLEUSE6].Hoehe = 23;
		Bmp[SCHLEUSE6].rcSrc.left = 336;
		Bmp[SCHLEUSE6].rcSrc.right = Bmp[SCHLEUSE6].rcSrc.left + Bmp[SCHLEUSE6].Breite;
		Bmp[SCHLEUSE6].rcSrc.top = 154;
		Bmp[SCHLEUSE6].rcSrc.bottom = Bmp[SCHLEUSE6].rcSrc.top + Bmp[SCHLEUSE6].Hoehe;
		Bmp[SCHLEUSE6].rcDes.left = 10;
		Bmp[SCHLEUSE6].rcDes.right = Bmp[SCHLEUSE6].rcDes.left + Bmp[SCHLEUSE6].Breite;
		Bmp[SCHLEUSE6].rcDes.top = 16;
		Bmp[SCHLEUSE6].rcDes.bottom = Bmp[SCHLEUSE6].rcDes.top + Bmp[SCHLEUSE6].Hoehe;

		// Bauwerke
		for (i = FELD; i <= FEUERSTELLE; i++)
		{
			Bmp[i].Animation = false;
			Bmp[i].Geschwindigkeit = 0;
			Bmp[i].Phase = 0;
		}

		Bmp[FELD].Anzahl = 3;
		Bmp[FELD].Surface = lpDDSBau;
		Bmp[FELD].Breite = 42;
		Bmp[FELD].Hoehe = 27;
		Bmp[FELD].rcSrc.left = 0;
		Bmp[FELD].rcSrc.right = 0 + Bmp[FELD].Breite;
		Bmp[FELD].rcSrc.top = 0;
		Bmp[FELD].rcSrc.bottom = 0 + Bmp[FELD].Hoehe;
		Bmp[FELD].rcDes.left = 5;
		Bmp[FELD].rcDes.right = Bmp[FELD].rcDes.left + Bmp[FELD].Breite;
		Bmp[FELD].rcDes.top = 15;
		Bmp[FELD].rcDes.bottom = Bmp[FELD].rcDes.top + Bmp[FELD].Hoehe;
		Bmp[FELD].AkAnzahl = 20;

		Bmp[ZELT].Anzahl = 1;
		Bmp[ZELT].Surface = lpDDSBau;
		Bmp[ZELT].Breite = 23;
		Bmp[ZELT].Hoehe = 20;
		Bmp[ZELT].rcSrc.left = 42;
		Bmp[ZELT].rcSrc.right = 42 + Bmp[ZELT].Breite;
		Bmp[ZELT].rcSrc.top = 0;
		Bmp[ZELT].rcSrc.bottom = 0 + Bmp[ZELT].Hoehe;
		Bmp[ZELT].rcDes.left = 14;
		Bmp[ZELT].rcDes.right = Bmp[ZELT].rcDes.left + Bmp[ZELT].Breite;
		Bmp[ZELT].rcDes.top = 9;
		Bmp[ZELT].rcDes.bottom = Bmp[ZELT].rcDes.top + Bmp[ZELT].Hoehe;
		Bmp[ZELT].Rohstoff[ROHAST] = 5;
		Bmp[ZELT].Rohstoff[ROHBLATT] = 5;
		Bmp[ZELT].AkAnzahl = 16;

		Bmp[BOOT].Anzahl = 2;
		Bmp[BOOT].Surface = lpDDSBau;
		Bmp[BOOT].Breite = 26;
		Bmp[BOOT].Hoehe = 18;
		Bmp[BOOT].rcSrc.left = 65;
		Bmp[BOOT].rcSrc.right = 65 + Bmp[BOOT].Breite;
		Bmp[BOOT].rcSrc.top = 0;
		Bmp[BOOT].rcSrc.bottom = 0 + Bmp[BOOT].Hoehe;
		Bmp[BOOT].rcDes.left = 14;
		Bmp[BOOT].rcDes.right = Bmp[BOOT].rcDes.left + Bmp[BOOT].Breite;
		Bmp[BOOT].rcDes.top = 20;
		Bmp[BOOT].rcDes.bottom = Bmp[BOOT].rcDes.top + Bmp[BOOT].Hoehe;
		Bmp[BOOT].Rohstoff[ROHAST] = 2;
		Bmp[BOOT].Rohstoff[ROHSTAMM] = 1;
		Bmp[BOOT].AkAnzahl = 16;

		Bmp[ROHR].Anzahl = 2;
		Bmp[ROHR].Surface = lpDDSBau;
		Bmp[ROHR].Breite = 34;
		Bmp[ROHR].Hoehe = 21;
		Bmp[ROHR].rcSrc.left = 91;
		Bmp[ROHR].rcSrc.right = 91 + Bmp[ROHR].Breite;
		Bmp[ROHR].rcSrc.top = 0;
		Bmp[ROHR].rcSrc.bottom = 0 + Bmp[ROHR].Hoehe;
		Bmp[ROHR].rcDes.left = 11;
		Bmp[ROHR].rcDes.right = Bmp[ROHR].rcDes.left + Bmp[ROHR].Breite;
		Bmp[ROHR].rcDes.top = 16;
		Bmp[ROHR].rcDes.bottom = Bmp[ROHR].rcDes.top + Bmp[ROHR].Hoehe;
		Bmp[ROHR].Rohstoff[ROHSTAMM] = 1;
		Bmp[ROHR].AkAnzahl = 18;

		Bmp[SOS].Anzahl = 1;
		Bmp[SOS].Surface = lpDDSBau;
		Bmp[SOS].Breite = 36;
		Bmp[SOS].Hoehe = 19;
		Bmp[SOS].rcSrc.left = 125;
		Bmp[SOS].rcSrc.right = 125 + Bmp[SOS].Breite;
		Bmp[SOS].rcSrc.top = 0;
		Bmp[SOS].rcSrc.bottom = 0 + Bmp[SOS].Hoehe;
		Bmp[SOS].rcDes.left = 11;
		Bmp[SOS].rcDes.right = Bmp[SOS].rcDes.left + Bmp[SOS].Breite;
		Bmp[SOS].rcDes.top = 20;
		Bmp[SOS].rcDes.bottom = Bmp[SOS].rcDes.top + Bmp[SOS].Hoehe;
		Bmp[SOS].Rohstoff[ROHSTEIN] = 10;
		Bmp[SOS].AkAnzahl = 20;

		Bmp[HAUS1].Anzahl = 1;
		Bmp[HAUS1].Surface = lpDDSBau;
		Bmp[HAUS1].Breite = 26;
		Bmp[HAUS1].Hoehe = 41;
		Bmp[HAUS1].rcSrc.left = 161;
		Bmp[HAUS1].rcSrc.right = 161 + Bmp[HAUS1].Breite;
		Bmp[HAUS1].rcSrc.top = 0;
		Bmp[HAUS1].rcSrc.bottom = 0 + Bmp[HAUS1].Hoehe;
		Bmp[HAUS1].rcDes.left = 0;
		Bmp[HAUS1].rcDes.right = Bmp[HAUS1].rcDes.left + Bmp[HAUS1].Breite;
		Bmp[HAUS1].rcDes.top = 0;
		Bmp[HAUS1].rcDes.bottom = Bmp[HAUS1].rcDes.top + Bmp[HAUS1].Hoehe;
		Bmp[HAUS1].Rohstoff[ROHAST] = 4;
		Bmp[HAUS1].AkAnzahl = 19;
		Bmp[HAUS1].Sound = Sound::FOREST;

		Bmp[HAUS2].Anzahl = 1;
		Bmp[HAUS2].Surface = lpDDSBau;
		Bmp[HAUS2].Breite = 34;
		Bmp[HAUS2].Hoehe = 41;
		Bmp[HAUS2].rcSrc.left = 187;
		Bmp[HAUS2].rcSrc.right = 187 + Bmp[HAUS2].Breite;
		Bmp[HAUS2].rcSrc.top = 0;
		Bmp[HAUS2].rcSrc.bottom = 0 + Bmp[HAUS2].Hoehe;
		Bmp[HAUS2].rcDes.left = 0;
		Bmp[HAUS2].rcDes.right = Bmp[HAUS2].rcDes.left + Bmp[HAUS2].Breite;
		Bmp[HAUS2].rcDes.top = 0;
		Bmp[HAUS2].rcDes.bottom = Bmp[HAUS2].rcDes.top + Bmp[HAUS2].Hoehe;
		Bmp[HAUS2].Rohstoff[ROHAST] = 3;
		Bmp[HAUS2].Rohstoff[ROHSTAMM] = 3;
		Bmp[HAUS2].AkAnzahl = 21;
		Bmp[HAUS2].Sound = Sound::FOREST;

		Bmp[HAUS3].Anzahl = 1;
		Bmp[HAUS3].Surface = lpDDSBau;
		Bmp[HAUS3].Breite = 34;
		Bmp[HAUS3].Hoehe = 41;
		Bmp[HAUS3].rcSrc.left = 221;
		Bmp[HAUS3].rcSrc.right = 221 + Bmp[HAUS3].Breite;
		Bmp[HAUS3].rcSrc.top = 0;
		Bmp[HAUS3].rcSrc.bottom = 0 + Bmp[HAUS3].Hoehe;
		Bmp[HAUS3].rcDes.left = 0;
		Bmp[HAUS3].rcDes.right = Bmp[HAUS3].rcDes.left + Bmp[HAUS3].Breite;
		Bmp[HAUS3].rcDes.top = 0;
		Bmp[HAUS3].rcDes.bottom = Bmp[HAUS3].rcDes.top + Bmp[HAUS3].Hoehe;
		Bmp[HAUS3].Rohstoff[ROHAST] = 4;
		Bmp[HAUS3].Rohstoff[ROHSTAMM] = 4;
		Bmp[HAUS3].Rohstoff[ROHBLATT] = 5;
		Bmp[HAUS3].AkAnzahl = 21;
		Bmp[HAUS3].Sound = Sound::FOREST;

		Bmp[FEUERSTELLE].Anzahl = 1;
		Bmp[FEUERSTELLE].Surface = lpDDSBau;
		Bmp[FEUERSTELLE].Breite = 21;
		Bmp[FEUERSTELLE].Hoehe = 19;
		Bmp[FEUERSTELLE].rcSrc.left = 255;
		Bmp[FEUERSTELLE].rcSrc.right = 255 + Bmp[FEUERSTELLE].Breite;
		Bmp[FEUERSTELLE].rcSrc.top = 0;
		Bmp[FEUERSTELLE].rcSrc.bottom = 0 + Bmp[FEUERSTELLE].Hoehe;
		Bmp[FEUERSTELLE].rcDes.left = 15;
		Bmp[FEUERSTELLE].rcDes.right = Bmp[FEUERSTELLE].rcDes.left + Bmp[FEUERSTELLE].Breite;
		Bmp[FEUERSTELLE].rcDes.top = 10;
		Bmp[FEUERSTELLE].rcDes.bottom = Bmp[FEUERSTELLE].rcDes.top + Bmp[FEUERSTELLE].Hoehe;
		Bmp[FEUERSTELLE].Rohstoff[ROHAST] = 5;
		Bmp[FEUERSTELLE].Rohstoff[ROHSTAMM] = 4;
		Bmp[FEUERSTELLE].AkAnzahl = 9;

		// Allgemein Bäume
		for (i = BAUM1; i <= BAUM4; i++)
		{
			Bmp[i].Animation = true;
			Bmp[i].Anzahl = 4;
			Bmp[i].Geschwindigkeit = 4;
			Bmp[i].Phase = 0;
			Bmp[i].Surface = lpDDSBaum;
			Bmp[i].Sound = Sound::FOREST;
		}
		// Baum1
		Bmp[BAUM1].rcSrc.left = 0;
		Bmp[BAUM1].rcSrc.top = 0;
		Bmp[BAUM1].rcSrc.right = 21;
		Bmp[BAUM1].rcSrc.bottom = 26;
		Bmp[BAUM1].Breite = (short)(Bmp[BAUM1].rcSrc.right - Bmp[BAUM1].rcSrc.left);
		Bmp[BAUM1].Hoehe = (short)(Bmp[BAUM1].rcSrc.bottom - Bmp[BAUM1].rcSrc.top);

		// Baum2
		Bmp[BAUM2].rcSrc.left = 21;
		Bmp[BAUM2].rcSrc.top = 0;
		Bmp[BAUM2].rcSrc.right = 42;
		Bmp[BAUM2].rcSrc.bottom = 26;
		Bmp[BAUM2].Breite = (short)(Bmp[BAUM2].rcSrc.right - Bmp[BAUM2].rcSrc.left);
		Bmp[BAUM2].Hoehe = (short)(Bmp[BAUM2].rcSrc.bottom - Bmp[BAUM2].rcSrc.top);

		// Baum3
		Bmp[BAUM3].rcSrc.left = 42;
		Bmp[BAUM3].rcSrc.top = 0;
		Bmp[BAUM3].rcSrc.right = 64;
		Bmp[BAUM3].rcSrc.bottom = 27;
		Bmp[BAUM3].Breite = (short)(Bmp[BAUM3].rcSrc.right - Bmp[BAUM3].rcSrc.left);
		Bmp[BAUM3].Hoehe = (short)(Bmp[BAUM3].rcSrc.bottom - Bmp[BAUM3].rcSrc.top);

		// Baum4
		Bmp[BAUM4].rcSrc.left = 64;
		Bmp[BAUM4].rcSrc.top = 0;
		Bmp[BAUM4].rcSrc.right = 81;
		Bmp[BAUM4].rcSrc.bottom = 16;
		Bmp[BAUM4].Breite = (short)(Bmp[BAUM4].rcSrc.right - Bmp[BAUM4].rcSrc.left);
		Bmp[BAUM4].Hoehe = (short)(Bmp[BAUM4].rcSrc.bottom - Bmp[BAUM4].rcSrc.top);

		// Baumgroß
		Bmp[BAUMGROSS].rcSrc.left = 238;
		Bmp[BAUMGROSS].rcSrc.top = 0;
		Bmp[BAUMGROSS].rcSrc.right = 238 + 26;
		Bmp[BAUMGROSS].rcSrc.bottom = 41;
		Bmp[BAUMGROSS].Breite = (short)(Bmp[BAUMGROSS].rcSrc.right - Bmp[BAUMGROSS].rcSrc.left);
		Bmp[BAUMGROSS].Hoehe = (short)(Bmp[BAUMGROSS].rcSrc.bottom - Bmp[BAUMGROSS].rcSrc.top);
		Bmp[BAUMGROSS].Animation = false;
		Bmp[BAUMGROSS].Anzahl = 1;
		Bmp[BAUMGROSS].Geschwindigkeit = 0;
		Bmp[BAUMGROSS].Phase = 0;
		Bmp[BAUMGROSS].Surface = lpDDSBaum;
		Bmp[BAUMGROSS].Sound = Sound::FOREST;

		// Feuer
		Bmp[FEUER].Anzahl = 3;
		Bmp[FEUER].Surface = lpDDSBau;
		Bmp[FEUER].Breite = 21;
		Bmp[FEUER].Hoehe = 28;
		Bmp[FEUER].rcSrc.left = 276;
		Bmp[FEUER].rcSrc.right = 276 + Bmp[FEUER].Breite;
		Bmp[FEUER].rcSrc.top = 0;
		Bmp[FEUER].rcSrc.bottom = 0 + Bmp[FEUER].Hoehe;
		Bmp[FEUER].rcDes.left = 15;
		Bmp[FEUER].rcDes.right = Bmp[FEUER].rcDes.left + Bmp[FEUER].Breite;
		Bmp[FEUER].rcDes.top = 1;
		Bmp[FEUER].rcDes.bottom = Bmp[FEUER].rcDes.top + Bmp[FEUER].Hoehe;
		Bmp[FEUER].Animation = true;
		Bmp[FEUER].Geschwindigkeit = 6;
		Bmp[FEUER].Phase = 0;
		Bmp[FEUER].Sound = Sound::FIRE;

		// WRACK
		Bmp[WRACK].Anzahl = 3;
		Bmp[WRACK].Surface = lpDDSBau;
		Bmp[WRACK].Breite = 24;
		Bmp[WRACK].Hoehe = 18;
		Bmp[WRACK].rcSrc.left = 391;
		Bmp[WRACK].rcSrc.right = 391 + Bmp[WRACK].Breite;
		Bmp[WRACK].rcSrc.top = 0;
		Bmp[WRACK].rcSrc.bottom = 0 + Bmp[WRACK].Hoehe;
		Bmp[WRACK].rcDes.left = 15;
		Bmp[WRACK].rcDes.right = Bmp[WRACK].rcDes.left + Bmp[WRACK].Breite;
		Bmp[WRACK].rcDes.top = 20;
		Bmp[WRACK].rcDes.bottom = Bmp[WRACK].rcDes.top + Bmp[WRACK].Hoehe;
		Bmp[WRACK].Animation = true;
		Bmp[WRACK].Geschwindigkeit = 5;
		Bmp[WRACK].Phase = 0;

		// WRACK2
		Bmp[WRACK2].Anzahl = 3;
		Bmp[WRACK2].Surface = lpDDSBau;
		Bmp[WRACK2].Breite = 27;
		Bmp[WRACK2].Hoehe = 26;
		Bmp[WRACK2].rcSrc.left = 415;
		Bmp[WRACK2].rcSrc.right = 415 + Bmp[WRACK2].Breite;
		Bmp[WRACK2].rcSrc.top = 0;
		Bmp[WRACK2].rcSrc.bottom = 0 + Bmp[WRACK2].Hoehe;
		Bmp[WRACK2].rcDes.left = 15;
		Bmp[WRACK2].rcDes.right = Bmp[WRACK2].rcDes.left + Bmp[WRACK2].Breite;
		Bmp[WRACK2].rcDes.top = 16;
		Bmp[WRACK2].rcDes.bottom = Bmp[WRACK2].rcDes.top + Bmp[WRACK2].Hoehe;
		Bmp[WRACK2].Animation = true;
		Bmp[WRACK2].Geschwindigkeit = 5;
		Bmp[WRACK2].Phase = 0;

		// Früchte
		// Busch
		Bmp[BUSCH].rcSrc.left = 81;
		Bmp[BUSCH].rcSrc.top = 0;
		Bmp[BUSCH].rcSrc.right = 81 + 20;
		Bmp[BUSCH].rcSrc.bottom = 13;
		Bmp[BUSCH].Breite = (short)(Bmp[BUSCH].rcSrc.right - Bmp[BUSCH].rcSrc.left);
		Bmp[BUSCH].Hoehe = (short)(Bmp[BUSCH].rcSrc.bottom - Bmp[BUSCH].rcSrc.top);
		Bmp[BUSCH].Animation = false;
		Bmp[BUSCH].Anzahl = 3;
		Bmp[BUSCH].Geschwindigkeit = 0;
		Bmp[BUSCH].Phase = 0;
		Bmp[BUSCH].Surface = lpDDSBaum;

		// Buttons

		// StandardBmponsinitialisierung
		for (i = BUTTGITTER; i <= BUTTDESTROY; i++)
		{
			Bmp[i].Animation = false;
			Bmp[i].Surface = lpDDSButtons;
			Bmp[i].Anzahl = 1;
			Bmp[i].Phase = 0;
		}

		// ButtGitter
		Bmp[BUTTGITTER].rcSrc.left = 0;
		Bmp[BUTTGITTER].rcSrc.top = 0;
		Bmp[BUTTGITTER].rcSrc.right = 28;
		Bmp[BUTTGITTER].rcSrc.bottom = 28;
		Bmp[BUTTGITTER].rcDes.left = rcPanel.left + 173;
		Bmp[BUTTGITTER].rcDes.top = rcPanel.top + 26;
		Bmp[BUTTGITTER].rcDes.right = Bmp[BUTTGITTER].rcDes.left + 28;
		Bmp[BUTTGITTER].rcDes.bottom = Bmp[BUTTGITTER].rcDes.top + 28;
		Bmp[BUTTGITTER].Breite = (short)(Bmp[BUTTGITTER].rcSrc.right - Bmp[BUTTGITTER].rcSrc.left);
		Bmp[BUTTGITTER].Hoehe = (short)(Bmp[BUTTGITTER].rcSrc.bottom - Bmp[BUTTGITTER].rcSrc.top);
		Bmp[BUTTGITTER].Anzahl = 2;

		// BUTTANIMATION
		Bmp[BUTTANIMATION].rcSrc.left = 0;
		Bmp[BUTTANIMATION].rcSrc.top = 56;
		Bmp[BUTTANIMATION].rcSrc.right = 28;
		Bmp[BUTTANIMATION].rcSrc.bottom = 56 + 28;
		Bmp[BUTTANIMATION].rcDes.left = rcPanel.left + 173;
		Bmp[BUTTANIMATION].rcDes.top = rcPanel.top + 60;
		Bmp[BUTTANIMATION].rcDes.right = Bmp[BUTTANIMATION].rcDes.left + 28;
		Bmp[BUTTANIMATION].rcDes.bottom = Bmp[BUTTANIMATION].rcDes.top + 28;
		Bmp[BUTTANIMATION].Breite = (short)(Bmp[BUTTANIMATION].rcSrc.right - Bmp[BUTTANIMATION].rcSrc.left);
		Bmp[BUTTANIMATION].Hoehe = (short)(Bmp[BUTTANIMATION].rcSrc.bottom - Bmp[BUTTANIMATION].rcSrc.top);
		Bmp[BUTTANIMATION].Anzahl = 2;

		// BUTTBEENDEN
		Bmp[BUTTBEENDEN].rcSrc.left = 0;
		Bmp[BUTTBEENDEN].rcSrc.top = 112;
		Bmp[BUTTBEENDEN].rcSrc.right = 20;
		Bmp[BUTTBEENDEN].rcSrc.bottom = 112 + 20;
		Bmp[BUTTBEENDEN].rcDes.left = rcPanel.left + 60;
		Bmp[BUTTBEENDEN].rcDes.top = rcPanel.top + 2;
		Bmp[BUTTBEENDEN].rcDes.right = Bmp[BUTTBEENDEN].rcDes.left + 20;
		Bmp[BUTTBEENDEN].rcDes.bottom = Bmp[BUTTBEENDEN].rcDes.top + 20;
		Bmp[BUTTBEENDEN].Breite = (short)(Bmp[BUTTBEENDEN].rcSrc.right - Bmp[BUTTBEENDEN].rcSrc.left);
		Bmp[BUTTBEENDEN].Hoehe = (short)(Bmp[BUTTBEENDEN].rcSrc.bottom - Bmp[BUTTBEENDEN].rcSrc.top);
		Bmp[BUTTBEENDEN].Anzahl = 4;
		Bmp[BUTTBEENDEN].Geschwindigkeit = 4;

		// BUTTNEU
		Bmp[BUTTNEU].rcSrc.left = 0;
		Bmp[BUTTNEU].rcSrc.top = 192;
		Bmp[BUTTNEU].rcSrc.right = 20;
		Bmp[BUTTNEU].rcSrc.bottom = 192 + 20;
		Bmp[BUTTNEU].rcDes.left = rcPanel.left + 100;
		Bmp[BUTTNEU].rcDes.top = rcPanel.top + 2;
		Bmp[BUTTNEU].rcDes.right = Bmp[BUTTNEU].rcDes.left + 20;
		Bmp[BUTTNEU].rcDes.bottom = Bmp[BUTTNEU].rcDes.top + 20;
		Bmp[BUTTNEU].Breite = (short)(Bmp[BUTTNEU].rcSrc.right - Bmp[BUTTNEU].rcSrc.left);
		Bmp[BUTTNEU].Hoehe = (short)(Bmp[BUTTNEU].rcSrc.bottom - Bmp[BUTTNEU].rcSrc.top);
		Bmp[BUTTNEU].Anzahl = 2;
		Bmp[BUTTNEU].Geschwindigkeit = 3;


		// BUTTTAGNEU
		Bmp[BUTTTAGNEU].rcSrc.left = 0;
		Bmp[BUTTTAGNEU].rcSrc.top = 232;
		Bmp[BUTTTAGNEU].rcSrc.right = 20;
		Bmp[BUTTTAGNEU].rcSrc.bottom = 232 + 20;
		Bmp[BUTTTAGNEU].rcDes.left = rcPanel.left + 140;
		Bmp[BUTTTAGNEU].rcDes.top = rcPanel.top + 2;
		Bmp[BUTTTAGNEU].rcDes.right = Bmp[BUTTTAGNEU].rcDes.left + 20;
		Bmp[BUTTTAGNEU].rcDes.bottom = Bmp[BUTTTAGNEU].rcDes.top + 20;
		Bmp[BUTTTAGNEU].Breite = (short)(Bmp[BUTTTAGNEU].rcSrc.right - Bmp[BUTTTAGNEU].rcSrc.left);
		Bmp[BUTTTAGNEU].Hoehe = (short)(Bmp[BUTTTAGNEU].rcSrc.bottom - Bmp[BUTTTAGNEU].rcSrc.top);
		Bmp[BUTTTAGNEU].Anzahl = 2;
		Bmp[BUTTTAGNEU].Geschwindigkeit = 2;

		// BUTTSOUND
		Bmp[BUTTSOUND].rcSrc.left = 0;
		Bmp[BUTTSOUND].rcSrc.top = 272;
		Bmp[BUTTSOUND].rcSrc.right = 28;
		Bmp[BUTTSOUND].rcSrc.bottom = 272 + 28;
		Bmp[BUTTSOUND].rcDes.left = rcPanel.left + 173;
		Bmp[BUTTSOUND].rcDes.top = rcPanel.top + 94;
		Bmp[BUTTSOUND].rcDes.right = Bmp[BUTTSOUND].rcDes.left + 28;
		Bmp[BUTTSOUND].rcDes.bottom = Bmp[BUTTSOUND].rcDes.top + 28;
		Bmp[BUTTSOUND].Breite = (short)(Bmp[BUTTSOUND].rcSrc.right - Bmp[BUTTSOUND].rcSrc.left);
		Bmp[BUTTSOUND].Hoehe = (short)(Bmp[BUTTSOUND].rcSrc.bottom - Bmp[BUTTSOUND].rcSrc.top);
		Bmp[BUTTSOUND].Anzahl = 2;

		// ButtAktion
		Bmp[BUTTAKTION].rcSrc.left = 28;
		Bmp[BUTTAKTION].rcSrc.top = 0;
		Bmp[BUTTAKTION].rcSrc.right = 28 + 35;
		Bmp[BUTTAKTION].rcSrc.bottom = 35;
		Bmp[BUTTAKTION].rcDes.left = rcPanel.left + 29;
		Bmp[BUTTAKTION].rcDes.top = rcPanel.top + 191;
		Bmp[BUTTAKTION].rcDes.right = Bmp[BUTTAKTION].rcDes.left + 35;
		Bmp[BUTTAKTION].rcDes.bottom = Bmp[BUTTAKTION].rcDes.top + 35;
		Bmp[BUTTAKTION].Breite = (short)(Bmp[BUTTAKTION].rcSrc.right - Bmp[BUTTAKTION].rcSrc.left);
		Bmp[BUTTAKTION].Hoehe = (short)(Bmp[BUTTAKTION].rcSrc.bottom - Bmp[BUTTAKTION].rcSrc.top);
		Bmp[BUTTAKTION].Anzahl = 3;
		Bmp[BUTTAKTION].Geschwindigkeit = 6;

		// BUTTBAUEN
		Bmp[BUTTBAUEN].rcSrc.left = 203;
		Bmp[BUTTBAUEN].rcSrc.top = 0;
		Bmp[BUTTBAUEN].rcSrc.right = 203 + 35;
		Bmp[BUTTBAUEN].rcSrc.bottom = 35;
		Bmp[BUTTBAUEN].rcDes.left = rcPanel.left + 70;
		Bmp[BUTTBAUEN].rcDes.top = rcPanel.top + 191;
		Bmp[BUTTBAUEN].rcDes.right = Bmp[BUTTBAUEN].rcDes.left + 35;
		Bmp[BUTTBAUEN].rcDes.bottom = Bmp[BUTTBAUEN].rcDes.top + 35;
		Bmp[BUTTBAUEN].Breite = (short)(Bmp[BUTTBAUEN].rcSrc.right - Bmp[BUTTBAUEN].rcSrc.left);
		Bmp[BUTTBAUEN].Hoehe = (short)(Bmp[BUTTBAUEN].rcSrc.bottom - Bmp[BUTTBAUEN].rcSrc.top);
		Bmp[BUTTBAUEN].Anzahl = 4;
		Bmp[BUTTBAUEN].Geschwindigkeit = 5;


		// BUTTINVENTAR
		Bmp[BUTTINVENTAR].rcSrc.left = 168;
		Bmp[BUTTINVENTAR].rcSrc.top = 0;
		Bmp[BUTTINVENTAR].rcSrc.right = 168 + 35;
		Bmp[BUTTINVENTAR].rcSrc.bottom = 35;
		Bmp[BUTTINVENTAR].rcDes.left = rcPanel.left + 152;
		Bmp[BUTTINVENTAR].rcDes.top = rcPanel.top + 191;
		Bmp[BUTTINVENTAR].rcDes.right = Bmp[BUTTINVENTAR].rcDes.left + 35;
		Bmp[BUTTINVENTAR].rcDes.bottom = Bmp[BUTTINVENTAR].rcDes.top + 35;
		Bmp[BUTTINVENTAR].Breite = (short)(Bmp[BUTTINVENTAR].rcSrc.right - Bmp[BUTTINVENTAR].rcSrc.left);
		Bmp[BUTTINVENTAR].Hoehe = (short)(Bmp[BUTTINVENTAR].rcSrc.bottom - Bmp[BUTTINVENTAR].rcSrc.top);
		Bmp[BUTTINVENTAR].Anzahl = 4;
		Bmp[BUTTINVENTAR].Geschwindigkeit = 4;

		// BUTTWEITER
		Bmp[BUTTWEITER].rcSrc.left = 343;
		Bmp[BUTTWEITER].rcSrc.top = 0;
		Bmp[BUTTWEITER].rcSrc.right = 343 + 35;
		Bmp[BUTTWEITER].rcSrc.bottom = 35;
		Bmp[BUTTWEITER].rcDes.left = rcPanel.left + 111;
		Bmp[BUTTWEITER].rcDes.top = rcPanel.top + 191;
		Bmp[BUTTWEITER].rcDes.right = Bmp[BUTTWEITER].rcDes.left + 35;
		Bmp[BUTTWEITER].rcDes.bottom = Bmp[BUTTWEITER].rcDes.top + 35;
		Bmp[BUTTWEITER].Breite = (short)(Bmp[BUTTWEITER].rcSrc.right - Bmp[BUTTWEITER].rcSrc.left);
		Bmp[BUTTWEITER].Hoehe = (short)(Bmp[BUTTWEITER].rcSrc.bottom - Bmp[BUTTWEITER].rcSrc.top);
		Bmp[BUTTWEITER].Anzahl = 4;
		Bmp[BUTTWEITER].Geschwindigkeit = 4;
		Bmp[BUTTWEITER].Phase = -1;

		// BUTTSTOP
		Bmp[BUTTSTOP].rcSrc.left = 378;
		Bmp[BUTTSTOP].rcSrc.top = 0;
		Bmp[BUTTSTOP].rcSrc.right = 378 + 35;
		Bmp[BUTTSTOP].rcSrc.bottom = 35;
		Bmp[BUTTSTOP].rcDes.left = rcPanel.left + 111;
		Bmp[BUTTSTOP].rcDes.top = rcPanel.top + 191;
		Bmp[BUTTSTOP].rcDes.right = Bmp[BUTTSTOP].rcDes.left + 35;
		Bmp[BUTTSTOP].rcDes.bottom = Bmp[BUTTSTOP].rcDes.top + 35;
		Bmp[BUTTSTOP].Breite = (short)(Bmp[BUTTSTOP].rcSrc.right - Bmp[BUTTSTOP].rcSrc.left);
		Bmp[BUTTSTOP].Hoehe = (short)(Bmp[BUTTSTOP].rcSrc.bottom - Bmp[BUTTSTOP].rcSrc.top);
		Bmp[BUTTSTOP].Anzahl = 4;
		Bmp[BUTTSTOP].Geschwindigkeit = 4;
		Bmp[BUTTSTOP].Phase = -1;

		// BUTTABLEGEN
		Bmp[BUTTABLEGEN].rcSrc.left = 483;
		Bmp[BUTTABLEGEN].rcSrc.top = 0;
		Bmp[BUTTABLEGEN].rcSrc.right = 483 + 35;
		Bmp[BUTTABLEGEN].rcSrc.bottom = 35;
		Bmp[BUTTABLEGEN].rcDes.left = rcPanel.left + 111;
		Bmp[BUTTABLEGEN].rcDes.top = rcPanel.top + 191;
		Bmp[BUTTABLEGEN].rcDes.right = Bmp[BUTTABLEGEN].rcDes.left + 35;
		Bmp[BUTTABLEGEN].rcDes.bottom = Bmp[BUTTABLEGEN].rcDes.top + 35;
		Bmp[BUTTABLEGEN].Breite = (short)(Bmp[BUTTABLEGEN].rcSrc.right - Bmp[BUTTABLEGEN].rcSrc.left);
		Bmp[BUTTABLEGEN].Hoehe = (short)(Bmp[BUTTABLEGEN].rcSrc.bottom - Bmp[BUTTABLEGEN].rcSrc.top);
		Bmp[BUTTABLEGEN].Anzahl = 4;
		Bmp[BUTTABLEGEN].Geschwindigkeit = 3;
		Bmp[BUTTSTOP].Phase = -1;

		// BUTTSUCHEN
		Bmp[BUTTSUCHEN].rcSrc.left = 63;
		Bmp[BUTTSUCHEN].rcSrc.top = 0;
		Bmp[BUTTSUCHEN].rcSrc.right = 63 + 35;
		Bmp[BUTTSUCHEN].rcSrc.bottom = 35;
		Bmp[BUTTSUCHEN].rcDes.left = rcPanel.left + 29;
		Bmp[BUTTSUCHEN].rcDes.top = rcPanel.top + 270;
		Bmp[BUTTSUCHEN].rcDes.right = Bmp[BUTTSUCHEN].rcDes.left + 35;
		Bmp[BUTTSUCHEN].rcDes.bottom = Bmp[BUTTSUCHEN].rcDes.top + 35;
		Bmp[BUTTSUCHEN].Breite = (short)(Bmp[BUTTSUCHEN].rcSrc.right - Bmp[BUTTSUCHEN].rcSrc.left);
		Bmp[BUTTSUCHEN].Hoehe = (short)(Bmp[BUTTSUCHEN].rcSrc.bottom - Bmp[BUTTSUCHEN].rcSrc.top);
		Bmp[BUTTSUCHEN].Anzahl = 4;
		Bmp[BUTTSUCHEN].Geschwindigkeit = 4;

		// BUTTESSEN
		Bmp[BUTTESSEN].rcSrc.left = 133;
		Bmp[BUTTESSEN].rcSrc.top = 0;
		Bmp[BUTTESSEN].rcSrc.right = 133 + 35;
		Bmp[BUTTESSEN].rcSrc.bottom = 35;
		Bmp[BUTTESSEN].rcDes.left = rcPanel.left + 70;
		Bmp[BUTTESSEN].rcDes.top = rcPanel.top + 270;
		Bmp[BUTTESSEN].rcDes.right = Bmp[BUTTESSEN].rcDes.left + 35;
		Bmp[BUTTESSEN].rcDes.bottom = Bmp[BUTTESSEN].rcDes.top + 35;
		Bmp[BUTTESSEN].Breite = (short)(Bmp[BUTTESSEN].rcSrc.right - Bmp[BUTTESSEN].rcSrc.left);
		Bmp[BUTTESSEN].Hoehe = (short)(Bmp[BUTTESSEN].rcSrc.bottom - Bmp[BUTTESSEN].rcSrc.top);
		Bmp[BUTTESSEN].Anzahl = 4;
		Bmp[BUTTESSEN].Geschwindigkeit = 4;

		// BUTTSCHLAFEN
		Bmp[BUTTSCHLAFEN].rcSrc.left = 308;
		Bmp[BUTTSCHLAFEN].rcSrc.top = 0;
		Bmp[BUTTSCHLAFEN].rcSrc.right = 308 + 35;
		Bmp[BUTTSCHLAFEN].rcSrc.bottom = 35;
		Bmp[BUTTSCHLAFEN].rcDes.left = rcPanel.left + 111;
		Bmp[BUTTSCHLAFEN].rcDes.top = rcPanel.top + 270;
		Bmp[BUTTSCHLAFEN].rcDes.right = Bmp[BUTTSCHLAFEN].rcDes.left + 35;
		Bmp[BUTTSCHLAFEN].rcDes.bottom = Bmp[BUTTSCHLAFEN].rcDes.top + 35;
		Bmp[BUTTSCHLAFEN].Breite = (short)(Bmp[BUTTSCHLAFEN].rcSrc.right - Bmp[BUTTSCHLAFEN].rcSrc.left);
		Bmp[BUTTSCHLAFEN].Hoehe = (short)(Bmp[BUTTSCHLAFEN].rcSrc.bottom - Bmp[BUTTSCHLAFEN].rcSrc.top);
		Bmp[BUTTSCHLAFEN].Anzahl = 4;
		Bmp[BUTTSCHLAFEN].Geschwindigkeit = 3;

		// BUTTFAELLEN
		Bmp[BUTTFAELLEN].rcSrc.left = 98;
		Bmp[BUTTFAELLEN].rcSrc.top = 0;
		Bmp[BUTTFAELLEN].rcSrc.right = 98 + 35;
		Bmp[BUTTFAELLEN].rcSrc.bottom = 35;
		Bmp[BUTTFAELLEN].rcDes.left = rcPanel.left + 152;
		Bmp[BUTTFAELLEN].rcDes.top = rcPanel.top + 270;
		Bmp[BUTTFAELLEN].rcDes.right = Bmp[BUTTFAELLEN].rcDes.left + 35;
		Bmp[BUTTFAELLEN].rcDes.bottom = Bmp[BUTTFAELLEN].rcDes.top + 35;
		Bmp[BUTTFAELLEN].Breite = (short)(Bmp[BUTTFAELLEN].rcSrc.right - Bmp[BUTTFAELLEN].rcSrc.left);
		Bmp[BUTTFAELLEN].Hoehe = (short)(Bmp[BUTTFAELLEN].rcSrc.bottom - Bmp[BUTTFAELLEN].rcSrc.top);
		Bmp[BUTTFAELLEN].Anzahl = 4;
		Bmp[BUTTFAELLEN].Geschwindigkeit = 4;
		Bmp[BUTTFAELLEN].Phase = -1;

		// BUTTANGELN
		Bmp[BUTTANGELN].rcSrc.left = 413;
		Bmp[BUTTANGELN].rcSrc.top = 0;
		Bmp[BUTTANGELN].rcSrc.right = 413 + 35;
		Bmp[BUTTANGELN].rcSrc.bottom = 35;
		Bmp[BUTTANGELN].rcDes.left = rcPanel.left + 29;
		Bmp[BUTTANGELN].rcDes.top = rcPanel.top + 325;
		Bmp[BUTTANGELN].rcDes.right = Bmp[BUTTANGELN].rcDes.left + 35;
		Bmp[BUTTANGELN].rcDes.bottom = Bmp[BUTTANGELN].rcDes.top + 35;
		Bmp[BUTTANGELN].Breite = (short)(Bmp[BUTTANGELN].rcSrc.right - Bmp[BUTTANGELN].rcSrc.left);
		Bmp[BUTTANGELN].Hoehe = (short)(Bmp[BUTTANGELN].rcSrc.bottom - Bmp[BUTTANGELN].rcSrc.top);
		Bmp[BUTTANGELN].Anzahl = 4;
		Bmp[BUTTANGELN].Geschwindigkeit = 3;
		Bmp[BUTTANGELN].Phase = -1;

		// BUTTANZUENDEN
		Bmp[BUTTANZUENDEN].rcSrc.left = 28;
		Bmp[BUTTANZUENDEN].rcSrc.top = 175;
		Bmp[BUTTANZUENDEN].rcSrc.right = 28 + 35;
		Bmp[BUTTANZUENDEN].rcSrc.bottom = 175 + 35;
		Bmp[BUTTANZUENDEN].rcDes.left = rcPanel.left + 70;
		Bmp[BUTTANZUENDEN].rcDes.top = rcPanel.top + 325;
		Bmp[BUTTANZUENDEN].rcDes.right = Bmp[BUTTANZUENDEN].rcDes.left + 35;
		Bmp[BUTTANZUENDEN].rcDes.bottom = Bmp[BUTTANZUENDEN].rcDes.top + 35;
		Bmp[BUTTANZUENDEN].Breite = (short)(Bmp[BUTTANZUENDEN].rcSrc.right - Bmp[BUTTANZUENDEN].rcSrc.left);
		Bmp[BUTTANZUENDEN].Hoehe = (short)(Bmp[BUTTANZUENDEN].rcSrc.bottom - Bmp[BUTTANZUENDEN].rcSrc.top);
		Bmp[BUTTANZUENDEN].Anzahl = 3;
		Bmp[BUTTANZUENDEN].Geschwindigkeit = 4;
		Bmp[BUTTANZUENDEN].Phase = -1;

		// BUTTAUSSCHAU
		Bmp[BUTTAUSSCHAU].rcSrc.left = 63;
		Bmp[BUTTAUSSCHAU].rcSrc.top = 175;
		Bmp[BUTTAUSSCHAU].rcSrc.right = 63 + 35;
		Bmp[BUTTAUSSCHAU].rcSrc.bottom = 175 + 35;
		Bmp[BUTTAUSSCHAU].rcDes.left = rcPanel.left + 111;
		Bmp[BUTTAUSSCHAU].rcDes.top = rcPanel.top + 325;
		Bmp[BUTTAUSSCHAU].rcDes.right = Bmp[BUTTAUSSCHAU].rcDes.left + 35;
		Bmp[BUTTAUSSCHAU].rcDes.bottom = Bmp[BUTTAUSSCHAU].rcDes.top + 35;
		Bmp[BUTTAUSSCHAU].Breite = (short)(Bmp[BUTTAUSSCHAU].rcSrc.right - Bmp[BUTTAUSSCHAU].rcSrc.left);
		Bmp[BUTTAUSSCHAU].Hoehe = (short)(Bmp[BUTTAUSSCHAU].rcSrc.bottom - Bmp[BUTTAUSSCHAU].rcSrc.top);
		Bmp[BUTTAUSSCHAU].Anzahl = 4;
		Bmp[BUTTAUSSCHAU].Geschwindigkeit = 3;
		Bmp[BUTTAUSSCHAU].Phase = -1;

		// BUTTSCHATZKARTE
		Bmp[BUTTSCHATZKARTE].rcSrc.left = 98;
		Bmp[BUTTSCHATZKARTE].rcSrc.top = 175;
		Bmp[BUTTSCHATZKARTE].rcSrc.right = 98 + 35;
		Bmp[BUTTSCHATZKARTE].rcSrc.bottom = 175 + 35;
		Bmp[BUTTSCHATZKARTE].rcDes.left = rcPanel.left + 152;
		Bmp[BUTTSCHATZKARTE].rcDes.top = rcPanel.top + 325;
		Bmp[BUTTSCHATZKARTE].rcDes.right = Bmp[BUTTSCHATZKARTE].rcDes.left + 35;
		Bmp[BUTTSCHATZKARTE].rcDes.bottom = Bmp[BUTTSCHATZKARTE].rcDes.top + 35;
		Bmp[BUTTSCHATZKARTE].Breite = (short)(Bmp[BUTTSCHATZKARTE].rcSrc.right - Bmp[BUTTSCHATZKARTE].rcSrc.left);
		Bmp[BUTTSCHATZKARTE].Hoehe = (short)(Bmp[BUTTSCHATZKARTE].rcSrc.bottom - Bmp[BUTTSCHATZKARTE].rcSrc.top);
		Bmp[BUTTSCHATZKARTE].Anzahl = 4;
		Bmp[BUTTSCHATZKARTE].Geschwindigkeit = 3;
		Bmp[BUTTSCHATZKARTE].Phase = -1;

		// BUTTSCHATZ
		Bmp[BUTTSCHATZ].rcSrc.left = 133;
		Bmp[BUTTSCHATZ].rcSrc.top = 175;
		Bmp[BUTTSCHATZ].rcSrc.right = 133 + 35;
		Bmp[BUTTSCHATZ].rcSrc.bottom = 175 + 35;
		Bmp[BUTTSCHATZ].rcDes.left = rcPanel.left + 29;
		Bmp[BUTTSCHATZ].rcDes.top = rcPanel.top + 380;
		Bmp[BUTTSCHATZ].rcDes.right = Bmp[BUTTSCHATZ].rcDes.left + 35;
		Bmp[BUTTSCHATZ].rcDes.bottom = Bmp[BUTTSCHATZ].rcDes.top + 35;
		Bmp[BUTTSCHATZ].Breite = (short)(Bmp[BUTTSCHATZ].rcSrc.right - Bmp[BUTTSCHATZ].rcSrc.left);
		Bmp[BUTTSCHATZ].Hoehe = (short)(Bmp[BUTTSCHATZ].rcSrc.bottom - Bmp[BUTTSCHATZ].rcSrc.top);
		Bmp[BUTTSCHATZ].Anzahl = 4;
		Bmp[BUTTSCHATZ].Geschwindigkeit = 3;
		Bmp[BUTTSCHATZ].Phase = -1;

		// BUTTSCHLEUDER
		Bmp[BUTTSCHLEUDER].rcSrc.left = 168;
		Bmp[BUTTSCHLEUDER].rcSrc.top = 175;
		Bmp[BUTTSCHLEUDER].rcSrc.right = 168 + 35;
		Bmp[BUTTSCHLEUDER].rcSrc.bottom = 175 + 35;
		Bmp[BUTTSCHLEUDER].rcDes.left = rcPanel.left + 70;
		Bmp[BUTTSCHLEUDER].rcDes.top = rcPanel.top + 380;
		Bmp[BUTTSCHLEUDER].rcDes.right = Bmp[BUTTSCHLEUDER].rcDes.left + 35;
		Bmp[BUTTSCHLEUDER].rcDes.bottom = Bmp[BUTTSCHLEUDER].rcDes.top + 35;
		Bmp[BUTTSCHLEUDER].Breite = (short)(Bmp[BUTTSCHLEUDER].rcSrc.right - Bmp[BUTTSCHLEUDER].rcSrc.left);
		Bmp[BUTTSCHLEUDER].Hoehe = (short)(Bmp[BUTTSCHLEUDER].rcSrc.bottom - Bmp[BUTTSCHLEUDER].rcSrc.top);
		Bmp[BUTTSCHLEUDER].Anzahl = 4;
		Bmp[BUTTSCHLEUDER].Geschwindigkeit = 3;
		Bmp[BUTTSCHLEUDER].Phase = -1;

		// BUTTFELD
		Bmp[BUTTFELD].rcSrc.left = 238;
		Bmp[BUTTFELD].rcSrc.top = 0;
		Bmp[BUTTFELD].rcSrc.right = 238 + 35;
		Bmp[BUTTFELD].rcSrc.bottom = 35;
		Bmp[BUTTFELD].rcDes.left = rcPanel.left + 70;
		Bmp[BUTTFELD].rcDes.top = rcPanel.top + 270;
		Bmp[BUTTFELD].rcDes.right = Bmp[BUTTFELD].rcDes.left + 35;
		Bmp[BUTTFELD].rcDes.bottom = Bmp[BUTTFELD].rcDes.top + 35;
		Bmp[BUTTFELD].Breite = (short)(Bmp[BUTTFELD].rcSrc.right - Bmp[BUTTFELD].rcSrc.left);
		Bmp[BUTTFELD].Hoehe = (short)(Bmp[BUTTFELD].rcSrc.bottom - Bmp[BUTTFELD].rcSrc.top);
		Bmp[BUTTFELD].Anzahl = 4;
		Bmp[BUTTFELD].Geschwindigkeit = 3;
		Bmp[BUTTFELD].Phase = -1;

		// BUTTZELT
		Bmp[BUTTZELT].rcSrc.left = 273;
		Bmp[BUTTZELT].rcSrc.top = 0;
		Bmp[BUTTZELT].rcSrc.right = 273 + 35;
		Bmp[BUTTZELT].rcSrc.bottom = 35;
		Bmp[BUTTZELT].rcDes.left = rcPanel.left + 29;
		Bmp[BUTTZELT].rcDes.top = rcPanel.top + 270;
		Bmp[BUTTZELT].rcDes.right = Bmp[BUTTZELT].rcDes.left + 35;
		Bmp[BUTTZELT].rcDes.bottom = Bmp[BUTTZELT].rcDes.top + 35;
		Bmp[BUTTZELT].Breite = (short)(Bmp[BUTTZELT].rcSrc.right - Bmp[BUTTZELT].rcSrc.left);
		Bmp[BUTTZELT].Hoehe = (short)(Bmp[BUTTZELT].rcSrc.bottom - Bmp[BUTTZELT].rcSrc.top);
		Bmp[BUTTZELT].Anzahl = 3;
		Bmp[BUTTZELT].Geschwindigkeit = 3;
		Bmp[BUTTZELT].Phase = 0;

		// BUTTBOOT
		Bmp[BUTTBOOT].rcSrc.left = 448;
		Bmp[BUTTBOOT].rcSrc.top = 0;
		Bmp[BUTTBOOT].rcSrc.right = 448 + 35;
		Bmp[BUTTBOOT].rcSrc.bottom = 35;
		Bmp[BUTTBOOT].rcDes.left = rcPanel.left + 111;
		Bmp[BUTTBOOT].rcDes.top = rcPanel.top + 270;
		Bmp[BUTTBOOT].rcDes.right = Bmp[BUTTBOOT].rcDes.left + 35;
		Bmp[BUTTBOOT].rcDes.bottom = Bmp[BUTTBOOT].rcDes.top + 35;
		Bmp[BUTTBOOT].Breite = (short)(Bmp[BUTTBOOT].rcSrc.right - Bmp[BUTTBOOT].rcSrc.left);
		Bmp[BUTTBOOT].Hoehe = (short)(Bmp[BUTTBOOT].rcSrc.bottom - Bmp[BUTTBOOT].rcSrc.top);
		Bmp[BUTTBOOT].Anzahl = 3;
		Bmp[BUTTBOOT].Geschwindigkeit = 3;
		Bmp[BUTTBOOT].Phase = -1;

		// BUTTROHR
		Bmp[BUTTROHR].rcSrc.left = 518;
		Bmp[BUTTROHR].rcSrc.top = 0;
		Bmp[BUTTROHR].rcSrc.right = 518 + 35;
		Bmp[BUTTROHR].rcSrc.bottom = 35;
		Bmp[BUTTROHR].rcDes.left = rcPanel.left + 152;
		Bmp[BUTTROHR].rcDes.top = rcPanel.top + 270;
		Bmp[BUTTROHR].rcDes.right = Bmp[BUTTROHR].rcDes.left + 35;
		Bmp[BUTTROHR].rcDes.bottom = Bmp[BUTTROHR].rcDes.top + 35;
		Bmp[BUTTROHR].Breite = (short)(Bmp[BUTTROHR].rcSrc.right - Bmp[BUTTROHR].rcSrc.left);
		Bmp[BUTTROHR].Hoehe = (short)(Bmp[BUTTROHR].rcSrc.bottom - Bmp[BUTTROHR].rcSrc.top);
		Bmp[BUTTROHR].Anzahl = 4;
		Bmp[BUTTROHR].Geschwindigkeit = 2;
		Bmp[BUTTROHR].Phase = -1;

		// BUTTSOS
		Bmp[BUTTSOS].rcSrc.left = 588;
		Bmp[BUTTSOS].rcSrc.top = 0;
		Bmp[BUTTSOS].rcSrc.right = 588 + 35;
		Bmp[BUTTSOS].rcSrc.bottom = 35;
		Bmp[BUTTSOS].rcDes.left = rcPanel.left + 29;
		Bmp[BUTTSOS].rcDes.top = rcPanel.top + 325;
		Bmp[BUTTSOS].rcDes.right = Bmp[BUTTSOS].rcDes.left + 35;
		Bmp[BUTTSOS].rcDes.bottom = Bmp[BUTTSOS].rcDes.top + 35;
		Bmp[BUTTSOS].Breite = (short)(Bmp[BUTTSOS].rcSrc.right - Bmp[BUTTSOS].rcSrc.left);
		Bmp[BUTTSOS].Hoehe = (short)(Bmp[BUTTSOS].rcSrc.bottom - Bmp[BUTTSOS].rcSrc.top);
		Bmp[BUTTSOS].Anzahl = 3;
		Bmp[BUTTSOS].Geschwindigkeit = 2;

		// BUTTHAUS1
		Bmp[BUTTHAUS1].rcSrc.left = 623;
		Bmp[BUTTHAUS1].rcSrc.top = 0;
		Bmp[BUTTHAUS1].rcSrc.right = 623 + 35;
		Bmp[BUTTHAUS1].rcSrc.bottom = 35;
		Bmp[BUTTHAUS1].rcDes.left = rcPanel.left + 70;
		Bmp[BUTTHAUS1].rcDes.top = rcPanel.top + 325;
		Bmp[BUTTHAUS1].rcDes.right = Bmp[BUTTHAUS1].rcDes.left + 35;
		Bmp[BUTTHAUS1].rcDes.bottom = Bmp[BUTTHAUS1].rcDes.top + 35;
		Bmp[BUTTHAUS1].Breite = (short)(Bmp[BUTTHAUS1].rcSrc.right - Bmp[BUTTHAUS1].rcSrc.left);
		Bmp[BUTTHAUS1].Hoehe = (short)(Bmp[BUTTHAUS1].rcSrc.bottom - Bmp[BUTTHAUS1].rcSrc.top);
		Bmp[BUTTHAUS1].Anzahl = 5;
		Bmp[BUTTHAUS1].Geschwindigkeit = 3;
		Bmp[BUTTHAUS1].Phase = -1;

		// BUTTHAUS2
		Bmp[BUTTHAUS2].rcSrc.left = 658;
		Bmp[BUTTHAUS2].rcSrc.top = 0;
		Bmp[BUTTHAUS2].rcSrc.right = 658 + 35;
		Bmp[BUTTHAUS2].rcSrc.bottom = 35;
		Bmp[BUTTHAUS2].rcDes.left = rcPanel.left + 111;
		Bmp[BUTTHAUS2].rcDes.top = rcPanel.top + 325;
		Bmp[BUTTHAUS2].rcDes.right = Bmp[BUTTHAUS2].rcDes.left + 35;
		Bmp[BUTTHAUS2].rcDes.bottom = Bmp[BUTTHAUS2].rcDes.top + 35;
		Bmp[BUTTHAUS2].Breite = (short)(Bmp[BUTTHAUS2].rcSrc.right - Bmp[BUTTHAUS2].rcSrc.left);
		Bmp[BUTTHAUS2].Hoehe = (short)(Bmp[BUTTHAUS2].rcSrc.bottom - Bmp[BUTTHAUS2].rcSrc.top);
		Bmp[BUTTHAUS2].Anzahl = 4;
		Bmp[BUTTHAUS2].Geschwindigkeit = 3;
		Bmp[BUTTHAUS2].Phase = -1;

		// BUTTHAUS3
		Bmp[BUTTHAUS3].rcSrc.left = 693;
		Bmp[BUTTHAUS3].rcSrc.top = 0;
		Bmp[BUTTHAUS3].rcSrc.right = 693 + 35;
		Bmp[BUTTHAUS3].rcSrc.bottom = 35;
		Bmp[BUTTHAUS3].rcDes.left = rcPanel.left + 152;
		Bmp[BUTTHAUS3].rcDes.top = rcPanel.top + 325;
		Bmp[BUTTHAUS3].rcDes.right = Bmp[BUTTHAUS3].rcDes.left + 35;
		Bmp[BUTTHAUS3].rcDes.bottom = Bmp[BUTTHAUS3].rcDes.top + 35;
		Bmp[BUTTHAUS3].Breite = (short)(Bmp[BUTTHAUS3].rcSrc.right - Bmp[BUTTHAUS3].rcSrc.left);
		Bmp[BUTTHAUS3].Hoehe = (short)(Bmp[BUTTHAUS3].rcSrc.bottom - Bmp[BUTTHAUS3].rcSrc.top);
		Bmp[BUTTHAUS3].Anzahl = 4;
		Bmp[BUTTHAUS3].Geschwindigkeit = 3;
		Bmp[BUTTHAUS3].Phase = -1;

		// BUTTFEUERST
		Bmp[BUTTFEUERST].rcSrc.left = 728;
		Bmp[BUTTFEUERST].rcSrc.top = 0;
		Bmp[BUTTFEUERST].rcSrc.right = 728 + 35;
		Bmp[BUTTFEUERST].rcSrc.bottom = 35;
		Bmp[BUTTFEUERST].rcDes.left = rcPanel.left + 29;
		Bmp[BUTTFEUERST].rcDes.top = rcPanel.top + 380;
		Bmp[BUTTFEUERST].rcDes.right = Bmp[BUTTFEUERST].rcDes.left + 35;
		Bmp[BUTTFEUERST].rcDes.bottom = Bmp[BUTTFEUERST].rcDes.top + 35;
		Bmp[BUTTFEUERST].Breite = (short)(Bmp[BUTTFEUERST].rcSrc.right - Bmp[BUTTFEUERST].rcSrc.left);
		Bmp[BUTTFEUERST].Hoehe = (short)(Bmp[BUTTFEUERST].rcSrc.bottom - Bmp[BUTTFEUERST].rcSrc.top);
		Bmp[BUTTFEUERST].Anzahl = 4;
		Bmp[BUTTFEUERST].Geschwindigkeit = 3;

		// BUTTFRAGEZ
		Bmp[BUTTFRAGEZ].rcSrc.left = 203;
		Bmp[BUTTFRAGEZ].rcSrc.top = 175;
		Bmp[BUTTFRAGEZ].rcSrc.right = 203 + 35;
		Bmp[BUTTFRAGEZ].rcSrc.bottom = 175 + 35;
		Bmp[BUTTFRAGEZ].rcDes.left = 0;
		Bmp[BUTTFRAGEZ].rcDes.top = 0;
		Bmp[BUTTFRAGEZ].rcDes.right = 0 + 35;
		Bmp[BUTTFRAGEZ].rcDes.bottom = 0 + 35;
		Bmp[BUTTFRAGEZ].Breite = (short)(Bmp[BUTTFRAGEZ].rcSrc.right - Bmp[BUTTFRAGEZ].rcSrc.left);
		Bmp[BUTTFRAGEZ].Hoehe = (short)(Bmp[BUTTFRAGEZ].rcSrc.bottom - Bmp[BUTTFRAGEZ].rcSrc.top);
		Bmp[BUTTFRAGEZ].Anzahl = 1;
		Bmp[BUTTFRAGEZ].Geschwindigkeit = 0;

		// BUTTDESTROY
		Bmp[BUTTDESTROY].rcSrc.left = 553;
		Bmp[BUTTDESTROY].rcSrc.top = 0;
		Bmp[BUTTDESTROY].rcSrc.right = 553 + 35;
		Bmp[BUTTDESTROY].rcSrc.bottom = 35;
		Bmp[BUTTDESTROY].rcDes.left = rcPanel.left + 152;
		Bmp[BUTTDESTROY].rcDes.top = rcPanel.top + 380;
		Bmp[BUTTDESTROY].rcDes.right = Bmp[BUTTDESTROY].rcDes.left + 35;
		Bmp[BUTTDESTROY].rcDes.bottom = Bmp[BUTTDESTROY].rcDes.top + 35;
		Bmp[BUTTDESTROY].Breite = (short)(Bmp[BUTTDESTROY].rcSrc.right - Bmp[BUTTDESTROY].rcSrc.left);
		Bmp[BUTTDESTROY].Hoehe = (short)(Bmp[BUTTDESTROY].rcSrc.bottom - Bmp[BUTTDESTROY].rcSrc.top);
		Bmp[BUTTDESTROY].Anzahl = 4;
		Bmp[BUTTDESTROY].Geschwindigkeit = 5;

		// SpzAni
		for (i = BAUM1DOWN; i <= BAUM4DOWN; i++)
		{
			Bmp[i].Animation = true;
			Bmp[i].Surface = lpDDSBaum;
			Bmp[i].Phase = 0;
			Bmp[i].rcDes.left = 0;
			Bmp[i].rcDes.top = 0;
			Bmp[i].rcDes.right = 0;
			Bmp[i].rcDes.bottom = 0;
			Bmp[i].Anzahl = 3;
			Bmp[i].Geschwindigkeit = 4;
		}
		// BAUM1DOWN
		Bmp[BAUM1DOWN].rcSrc.left = 101;
		Bmp[BAUM1DOWN].rcSrc.top = 0;
		Bmp[BAUM1DOWN].rcSrc.right = 101 + 35;
		Bmp[BAUM1DOWN].rcSrc.bottom = 27;
		Bmp[BAUM1DOWN].Breite = (short)(Bmp[BAUM1DOWN].rcSrc.right - Bmp[BAUM1DOWN].rcSrc.left);
		Bmp[BAUM1DOWN].Hoehe = (short)(Bmp[BAUM1DOWN].rcSrc.bottom - Bmp[BAUM1DOWN].rcSrc.top);

		// BAUM2DOWN
		Bmp[BAUM2DOWN].rcSrc.left = 136;
		Bmp[BAUM2DOWN].rcSrc.top = 0;
		Bmp[BAUM2DOWN].rcSrc.right = 136 + 36;
		Bmp[BAUM2DOWN].rcSrc.bottom = 27;
		Bmp[BAUM2DOWN].Breite = (short)(Bmp[BAUM2DOWN].rcSrc.right - Bmp[BAUM2DOWN].rcSrc.left);
		Bmp[BAUM2DOWN].Hoehe = (short)(Bmp[BAUM2DOWN].rcSrc.bottom - Bmp[BAUM2DOWN].rcSrc.top);

		// BAUM3DOWN
		Bmp[BAUM3DOWN].rcSrc.left = 172;
		Bmp[BAUM3DOWN].rcSrc.top = 0;
		Bmp[BAUM3DOWN].rcSrc.right = 172 + 34;
		Bmp[BAUM3DOWN].rcSrc.bottom = 28;
		Bmp[BAUM3DOWN].Breite = (short)(Bmp[BAUM3DOWN].rcSrc.right - Bmp[BAUM3DOWN].rcSrc.left);
		Bmp[BAUM3DOWN].Hoehe = (short)(Bmp[BAUM3DOWN].rcSrc.bottom - Bmp[BAUM3DOWN].rcSrc.top);

		// BAUM4DOWN
		Bmp[BAUM4DOWN].rcSrc.left = 206;
		Bmp[BAUM4DOWN].rcSrc.top = 0;
		Bmp[BAUM4DOWN].rcSrc.right = 206 + 32;
		Bmp[BAUM4DOWN].rcSrc.bottom = 17;
		Bmp[BAUM4DOWN].Breite = (short)(Bmp[BAUM4DOWN].rcSrc.right - Bmp[BAUM4DOWN].rcSrc.left);
		Bmp[BAUM4DOWN].Hoehe = (short)(Bmp[BAUM4DOWN].rcSrc.bottom - Bmp[BAUM4DOWN].rcSrc.top);

		// Sonstiges

		// Säule1
		Bmp[SAEULE1].Anzahl = 1;
		Bmp[SAEULE1].rcSrc.left = 205;
		Bmp[SAEULE1].rcSrc.top = 115;
		Bmp[SAEULE1].rcSrc.right = Bmp[SAEULE1].rcSrc.left + 11;
		Bmp[SAEULE1].rcSrc.bottom = Bmp[SAEULE1].rcSrc.top + 115;
		Bmp[SAEULE1].rcDes.left = rcPanel.left + 52;
		Bmp[SAEULE1].rcDes.top = rcPanel.top + 471;
		Bmp[SAEULE1].rcDes.right = Bmp[SAEULE1].rcDes.left + 11;
		Bmp[SAEULE1].rcDes.bottom = Bmp[SAEULE1].rcDes.top + 115;
		Bmp[SAEULE1].Breite = (short)(Bmp[SAEULE1].rcSrc.right - Bmp[SAEULE1].rcSrc.left);
		Bmp[SAEULE1].Hoehe = (short)(Bmp[SAEULE1].rcSrc.bottom - Bmp[SAEULE1].rcSrc.top);
		Bmp[SAEULE1].Surface = lpDDSPanel;

		// Säule2
		Bmp[SAEULE2].Anzahl = 1;
		Bmp[SAEULE2].rcSrc.left = 216;
		Bmp[SAEULE2].rcSrc.top = 115;
		Bmp[SAEULE2].rcSrc.right = Bmp[SAEULE2].rcSrc.left + 11;
		Bmp[SAEULE2].rcSrc.bottom = Bmp[SAEULE2].rcSrc.top + 115;
		Bmp[SAEULE2].rcDes.left = rcPanel.left + 114;
		Bmp[SAEULE2].rcDes.top = rcPanel.top + 471;
		Bmp[SAEULE2].rcDes.right = Bmp[SAEULE2].rcDes.left + 11;
		Bmp[SAEULE2].rcDes.bottom = Bmp[SAEULE2].rcDes.top + 115;
		Bmp[SAEULE2].Breite = (short)(Bmp[SAEULE2].rcSrc.right - Bmp[SAEULE2].rcSrc.left);
		Bmp[SAEULE2].Hoehe = (short)(Bmp[SAEULE2].rcSrc.bottom - Bmp[SAEULE2].rcSrc.top);
		Bmp[SAEULE2].Surface = lpDDSPanel;

		// Säule3
		Bmp[SAEULE3].Anzahl = 1;
		Bmp[SAEULE3].rcSrc.left = 227;
		Bmp[SAEULE3].rcSrc.top = 115;
		Bmp[SAEULE3].rcSrc.right = Bmp[SAEULE3].rcSrc.left + 11;
		Bmp[SAEULE3].rcSrc.bottom = Bmp[SAEULE3].rcSrc.top + 115;
		Bmp[SAEULE3].rcDes.left = rcPanel.left + 175;
		Bmp[SAEULE3].rcDes.top = rcPanel.top + 471;
		Bmp[SAEULE3].rcDes.right = Bmp[SAEULE3].rcDes.left + 11;
		Bmp[SAEULE3].rcDes.bottom = Bmp[SAEULE3].rcDes.top + 115;
		Bmp[SAEULE3].Breite = (short)(Bmp[SAEULE3].rcSrc.right - Bmp[SAEULE3].rcSrc.left);
		Bmp[SAEULE3].Hoehe = (short)(Bmp[SAEULE3].rcSrc.bottom - Bmp[SAEULE3].rcSrc.top);
		Bmp[SAEULE3].Surface = lpDDSPanel;

		// Rohstoffe
		for (i = ROHAST; i <= ROHSCHLEUDER; i++)
		{
			Bmp[i].Anzahl = 1;
			Bmp[i].rcSrc.left = 18 + (i - ROHAST) * 16;
			Bmp[i].rcSrc.top = 0;
			Bmp[i].rcSrc.right = Bmp[i].rcSrc.left + 16;
			Bmp[i].rcSrc.bottom = Bmp[i].rcSrc.top + 15;
			Bmp[i].Breite = (short)(Bmp[i].rcSrc.right - Bmp[i].rcSrc.left);
			Bmp[i].Hoehe = (short)(Bmp[i].rcSrc.bottom - Bmp[i].rcSrc.top);
			Bmp[i].Surface = lpDDSInventar;
		}
		// RohAst
		Bmp[ROHAST].rcDes.left = rcPanel.left + 34;
		Bmp[ROHAST].rcDes.top = rcPanel.top + 280;
		Bmp[ROHAST].rcDes.right = Bmp[ROHAST].rcDes.left + 16;
		Bmp[ROHAST].rcDes.bottom = Bmp[ROHAST].rcDes.top + 15;
		// ROHSTEIN
		Bmp[ROHSTEIN].rcDes.left = rcPanel.left + 34;
		Bmp[ROHSTEIN].rcDes.top = rcPanel.top + 300;
		Bmp[ROHSTEIN].rcDes.right = Bmp[ROHSTEIN].rcDes.left + 16;
		Bmp[ROHSTEIN].rcDes.bottom = Bmp[ROHSTEIN].rcDes.top + 15;
		// ROHSTAMM
		Bmp[ROHSTAMM].rcDes.left = rcPanel.left + 34;
		Bmp[ROHSTAMM].rcDes.top = rcPanel.top + 360;
		Bmp[ROHSTAMM].rcDes.right = Bmp[ROHSTAMM].rcDes.left + 16;
		Bmp[ROHSTAMM].rcDes.bottom = Bmp[ROHSTAMM].rcDes.top + 15;
		// ROHAXT
		Bmp[ROHAXT].rcDes.left = rcPanel.left + 150;
		Bmp[ROHAXT].rcDes.top = rcPanel.top + 280;
		Bmp[ROHAXT].rcDes.right = Bmp[ROHAXT].rcDes.left + 16;
		Bmp[ROHAXT].rcDes.bottom = Bmp[ROHAXT].rcDes.top + 15;
		// ROHBLATT
		Bmp[ROHBLATT].rcDes.left = rcPanel.left + 34;
		Bmp[ROHBLATT].rcDes.top = rcPanel.top + 320;
		Bmp[ROHBLATT].rcDes.right = Bmp[ROHBLATT].rcDes.left + 16;
		Bmp[ROHBLATT].rcDes.bottom = Bmp[ROHBLATT].rcDes.top + 15;
		// ROHEGGE
		Bmp[ROHEGGE].rcDes.left = rcPanel.left + 150;
		Bmp[ROHEGGE].rcDes.top = rcPanel.top + 300;
		Bmp[ROHEGGE].rcDes.right = Bmp[ROHEGGE].rcDes.left + 16;
		Bmp[ROHEGGE].rcDes.bottom = Bmp[ROHEGGE].rcDes.top + 15;
		// ROHLIANE
		Bmp[ROHLIANE].rcDes.left = rcPanel.left + 34;
		Bmp[ROHLIANE].rcDes.top = rcPanel.top + 340;
		Bmp[ROHLIANE].rcDes.right = Bmp[ROHLIANE].rcDes.left + 16;
		Bmp[ROHLIANE].rcDes.bottom = Bmp[ROHLIANE].rcDes.top + 15;
		// ROHANGEL
		Bmp[ROHANGEL].rcDes.left = rcPanel.left + 150;
		Bmp[ROHANGEL].rcDes.top = rcPanel.top + 320;
		Bmp[ROHANGEL].rcDes.right = Bmp[ROHANGEL].rcDes.left + 16;
		Bmp[ROHANGEL].rcDes.bottom = Bmp[ROHANGEL].rcDes.top + 15;
		// ROHHAMMER
		Bmp[ROHHAMMER].rcDes.left = rcPanel.left + 150;
		Bmp[ROHHAMMER].rcDes.top = rcPanel.top + 340;
		Bmp[ROHHAMMER].rcDes.right = Bmp[ROHHAMMER].rcDes.left + 16;
		Bmp[ROHHAMMER].rcDes.bottom = Bmp[ROHHAMMER].rcDes.top + 15;
		// ROHFERNROHR
		Bmp[ROHFERNROHR].rcDes.left = rcPanel.left + 150;
		Bmp[ROHFERNROHR].rcDes.top = rcPanel.top + 360;
		Bmp[ROHFERNROHR].rcDes.right = Bmp[ROHFERNROHR].rcDes.left + 16;
		Bmp[ROHFERNROHR].rcDes.bottom = Bmp[ROHFERNROHR].rcDes.top + 15;
		// ROHSTREICHHOLZ
		Bmp[ROHSTREICHHOLZ].rcDes.left = rcPanel.left + 105;
		Bmp[ROHSTREICHHOLZ].rcDes.top = rcPanel.top + 280;
		Bmp[ROHSTREICHHOLZ].rcDes.right = Bmp[ROHSTREICHHOLZ].rcDes.left + 16;
		Bmp[ROHSTREICHHOLZ].rcDes.bottom = Bmp[ROHSTREICHHOLZ].rcDes.top + 15;
		// ROHSCHAUFEL
		Bmp[ROHSCHAUFEL].rcDes.left = rcPanel.left + 105;
		Bmp[ROHSCHAUFEL].rcDes.top = rcPanel.top + 320;
		Bmp[ROHSCHAUFEL].rcDes.right = Bmp[ROHSCHAUFEL].rcDes.left + 16;
		Bmp[ROHSCHAUFEL].rcDes.bottom = Bmp[ROHSCHAUFEL].rcDes.top + 15;
		// ROHKARTE
		Bmp[ROHKARTE].rcDes.left = rcPanel.left + 105;
		Bmp[ROHKARTE].rcDes.top = rcPanel.top + 320;
		Bmp[ROHKARTE].rcDes.right = Bmp[ROHKARTE].rcDes.left + 16;
		Bmp[ROHKARTE].rcDes.bottom = Bmp[ROHKARTE].rcDes.top + 15;
		// ROHSCHLEUDER
		Bmp[ROHSCHLEUDER].rcDes.left = rcPanel.left + 105;
		Bmp[ROHSCHLEUDER].rcDes.top = rcPanel.top + 340;
		Bmp[ROHSCHLEUDER].rcDes.right = Bmp[ROHSCHLEUDER].rcDes.left + 16;
		Bmp[ROHSCHLEUDER].rcDes.bottom = Bmp[ROHSCHLEUDER].rcDes.top + 15;

		// ROEMISCH1
		Bmp[ROEMISCH1].Anzahl = 1;
		Bmp[ROEMISCH1].rcSrc.left = 0;
		Bmp[ROEMISCH1].rcSrc.top = 0;
		Bmp[ROEMISCH1].rcSrc.right = Bmp[ROEMISCH1].rcSrc.left + 3;
		Bmp[ROEMISCH1].rcSrc.bottom = Bmp[ROEMISCH1].rcSrc.top + 13;
		Bmp[ROEMISCH1].rcDes.left = rcPanel.left + 50;
		Bmp[ROEMISCH1].rcDes.top = rcPanel.top;
		Bmp[ROEMISCH1].rcDes.right = Bmp[ROEMISCH1].rcDes.left + 3;
		Bmp[ROEMISCH1].rcDes.bottom = Bmp[ROEMISCH1].rcDes.top + 13;
		Bmp[ROEMISCH1].Breite = (short)(Bmp[ROEMISCH1].rcSrc.right - Bmp[ROEMISCH1].rcSrc.left);
		Bmp[ROEMISCH1].Hoehe = (short)(Bmp[ROEMISCH1].rcSrc.bottom - Bmp[ROEMISCH1].rcSrc.top);
		Bmp[ROEMISCH1].Surface = lpDDSInventar;

		// ROEMISCH2
		Bmp[ROEMISCH2].Anzahl = 1;
		Bmp[ROEMISCH2].rcSrc.left = 3;
		Bmp[ROEMISCH2].rcSrc.top = 0;
		Bmp[ROEMISCH2].rcSrc.right = Bmp[ROEMISCH2].rcSrc.left + 15;
		Bmp[ROEMISCH2].rcSrc.bottom = Bmp[ROEMISCH2].rcSrc.top + 10;
		Bmp[ROEMISCH2].rcDes.left = rcPanel.left + 50;
		Bmp[ROEMISCH2].rcDes.top = rcPanel.top;
		Bmp[ROEMISCH2].rcDes.right = Bmp[ROEMISCH2].rcDes.left + 15;
		Bmp[ROEMISCH2].rcDes.bottom = Bmp[ROEMISCH2].rcDes.top + 10;
		Bmp[ROEMISCH2].Breite = (short)(Bmp[ROEMISCH2].rcSrc.right - Bmp[ROEMISCH2].rcSrc.left);
		Bmp[ROEMISCH2].Hoehe = (short)(Bmp[ROEMISCH2].rcSrc.bottom - Bmp[ROEMISCH2].rcSrc.top);
		Bmp[ROEMISCH2].Surface = lpDDSInventar;

		// INVPAPIER
		Bmp[INVPAPIER].Anzahl = 1;
		Bmp[INVPAPIER].rcSrc.left = 0;
		Bmp[INVPAPIER].rcSrc.top = 15;
		Bmp[INVPAPIER].rcSrc.right = Bmp[INVPAPIER].rcSrc.left + 178;
		Bmp[INVPAPIER].rcSrc.bottom = Bmp[INVPAPIER].rcSrc.top + 114;
		Bmp[INVPAPIER].rcDes.left = rcPanel.left + 15;
		Bmp[INVPAPIER].rcDes.top = rcPanel.top + 270;
		Bmp[INVPAPIER].rcDes.right = Bmp[INVPAPIER].rcDes.left + 178;
		Bmp[INVPAPIER].rcDes.bottom = Bmp[INVPAPIER].rcDes.top + 114;
		Bmp[INVPAPIER].Breite = (short)(Bmp[INVPAPIER].rcSrc.right - Bmp[INVPAPIER].rcSrc.left);
		Bmp[INVPAPIER].Hoehe = (short)(Bmp[INVPAPIER].rcSrc.bottom - Bmp[INVPAPIER].rcSrc.top);
		Bmp[INVPAPIER].Surface = lpDDSInventar;

		// RING
		Bmp[RING].Anzahl = 1;
		Bmp[RING].rcSrc.left = 205;
		Bmp[RING].rcSrc.top = 230;
		Bmp[RING].rcSrc.right = Bmp[RING].rcSrc.left + 37;
		Bmp[RING].rcSrc.bottom = Bmp[RING].rcSrc.top + 150;
		Bmp[RING].rcDes.left = rcPanel.left + 5;
		Bmp[RING].rcDes.top = rcPanel.top - 113;
		Bmp[RING].rcDes.right = Bmp[RING].rcDes.left + 30;
		Bmp[RING].rcDes.bottom = Bmp[RING].rcDes.top;
		Bmp[RING].Breite = (short)(Bmp[RING].rcSrc.right - Bmp[RING].rcSrc.left);
		Bmp[RING].Hoehe = (short)(Bmp[RING].rcSrc.bottom - Bmp[RING].rcSrc.top);
		Bmp[RING].Surface = lpDDSPanel;

		// KREUZ
		Bmp[KREUZ].Anzahl = 1;
		Bmp[KREUZ].rcSrc.left = 205;
		Bmp[KREUZ].rcSrc.top = 380;
		Bmp[KREUZ].rcSrc.right = Bmp[KREUZ].rcSrc.left + 40;
		Bmp[KREUZ].rcSrc.bottom = Bmp[KREUZ].rcSrc.top + 22;
		Bmp[KREUZ].Breite = (short)(Bmp[KREUZ].rcSrc.right - Bmp[KREUZ].rcSrc.left);
		Bmp[KREUZ].Hoehe = (short)(Bmp[KREUZ].rcSrc.bottom - Bmp[KREUZ].rcSrc.top);
		Bmp[KREUZ].Surface = lpDDSPanel;

		// JA
		Bmp[JA].Anzahl = 1;
		Bmp[JA].rcSrc.left = 0;
		Bmp[JA].rcSrc.top = 154;
		Bmp[JA].rcSrc.right = Bmp[JA].rcSrc.left + 41;
		Bmp[JA].rcSrc.bottom = Bmp[JA].rcSrc.top + 42;
		Bmp[JA].Breite = (short)(Bmp[JA].rcSrc.right - Bmp[JA].rcSrc.left);
		Bmp[JA].Hoehe = (short)(Bmp[JA].rcSrc.bottom - Bmp[JA].rcSrc.top);
		Bmp[JA].Surface = lpDDSPapier;

		// NEIN
		Bmp[NEIN].Anzahl = 1;
		Bmp[NEIN].rcSrc.left = 41;
		Bmp[NEIN].rcSrc.top = 154;
		Bmp[NEIN].rcSrc.right = Bmp[NEIN].rcSrc.left + 100;
		Bmp[NEIN].rcSrc.bottom = Bmp[NEIN].rcSrc.top + 39;
		Bmp[NEIN].Breite = (short)(Bmp[NEIN].rcSrc.right - Bmp[NEIN].rcSrc.left);
		Bmp[NEIN].Hoehe = (short)(Bmp[NEIN].rcSrc.bottom - Bmp[NEIN].rcSrc.top);
		Bmp[NEIN].Surface = lpDDSPapier;

		// Sonne
		Bmp[SONNE].Anzahl = 1;
		Bmp[SONNE].rcSrc.left = 205;
		Bmp[SONNE].rcSrc.top = 65;
		Bmp[SONNE].rcSrc.right = Bmp[SONNE].rcSrc.left + 51;
		Bmp[SONNE].rcSrc.bottom = Bmp[SONNE].rcSrc.top + 50;
		Bmp[SONNE].rcDes.left = rcPanel.left + 30;
		Bmp[SONNE].rcDes.top = rcPanel.top + 630;
		Bmp[SONNE].rcDes.right = Bmp[SONNE].rcDes.left + 152;
		Bmp[SONNE].rcDes.bottom = Bmp[SONNE].rcDes.top + 55;
		Bmp[SONNE].Breite = (short)(Bmp[SONNE].rcSrc.right - Bmp[SONNE].rcSrc.left);
		Bmp[SONNE].Hoehe = (short)(Bmp[SONNE].rcSrc.bottom - Bmp[SONNE].rcSrc.top);
		Bmp[SONNE].Surface = lpDDSPanel;

		// PROGRAMMIERUNG
		Bmp[PROGRAMMIERUNG].Anzahl = 1;
		Bmp[PROGRAMMIERUNG].rcSrc.left = 0;
		Bmp[PROGRAMMIERUNG].rcSrc.top = 0;
		Bmp[PROGRAMMIERUNG].rcSrc.right = Bmp[PROGRAMMIERUNG].rcSrc.left + 348;
		Bmp[PROGRAMMIERUNG].rcSrc.bottom = Bmp[PROGRAMMIERUNG].rcSrc.top + 49;
		Bmp[PROGRAMMIERUNG].Breite = (short)(Bmp[PROGRAMMIERUNG].rcSrc.right - Bmp[PROGRAMMIERUNG].rcSrc.left);
		Bmp[PROGRAMMIERUNG].Hoehe = (short)(Bmp[PROGRAMMIERUNG].rcSrc.bottom - Bmp[PROGRAMMIERUNG].rcSrc.top);
		Bmp[PROGRAMMIERUNG].rcDes.left = MAXX / 2 - Bmp[PROGRAMMIERUNG].Breite / 2;
		Bmp[PROGRAMMIERUNG].rcDes.top = MAXY - Bmp[PROGRAMMIERUNG].Hoehe / 2;
		Bmp[PROGRAMMIERUNG].rcDes.right = Bmp[PROGRAMMIERUNG].rcDes.left + Bmp[PROGRAMMIERUNG].Breite;
		Bmp[PROGRAMMIERUNG].rcDes.bottom = Bmp[PROGRAMMIERUNG].rcDes.top + Bmp[PROGRAMMIERUNG].Hoehe;
		Bmp[PROGRAMMIERUNG].Surface = lpDDSCredits;

		// DIRKPLATE
		Bmp[DIRKPLATE].Anzahl = 1;
		Bmp[DIRKPLATE].rcSrc.left = 0;
		Bmp[DIRKPLATE].rcSrc.top = 49;
		Bmp[DIRKPLATE].rcSrc.right = Bmp[DIRKPLATE].rcSrc.left + 196;
		Bmp[DIRKPLATE].rcSrc.bottom = Bmp[DIRKPLATE].rcSrc.top + 47;
		Bmp[DIRKPLATE].Breite = (short)(Bmp[DIRKPLATE].rcSrc.right - Bmp[DIRKPLATE].rcSrc.left);
		Bmp[DIRKPLATE].Hoehe = (short)(Bmp[DIRKPLATE].rcSrc.bottom - Bmp[DIRKPLATE].rcSrc.top);
		Bmp[DIRKPLATE].rcDes.left = MAXX / 2 - Bmp[DIRKPLATE].Breite / 2;
		Bmp[DIRKPLATE].rcDes.top = MAXY - Bmp[DIRKPLATE].Hoehe / 2;
		Bmp[DIRKPLATE].rcDes.right = Bmp[DIRKPLATE].rcDes.left + Bmp[DIRKPLATE].Breite;
		Bmp[DIRKPLATE].rcDes.bottom = Bmp[DIRKPLATE].rcDes.top + Bmp[DIRKPLATE].Hoehe;
		Bmp[DIRKPLATE].Surface = lpDDSCredits;

		// MATTHIAS
		Bmp[MATTHIAS].Anzahl = 1;
		Bmp[MATTHIAS].rcSrc.left = 0;
		Bmp[MATTHIAS].rcSrc.top = 96;
		Bmp[MATTHIAS].rcSrc.right = Bmp[MATTHIAS].rcSrc.left + 379;
		Bmp[MATTHIAS].rcSrc.bottom = Bmp[MATTHIAS].rcSrc.top + 47;
		Bmp[MATTHIAS].Breite = (short)(Bmp[MATTHIAS].rcSrc.right - Bmp[MATTHIAS].rcSrc.left);
		Bmp[MATTHIAS].Hoehe = (short)(Bmp[MATTHIAS].rcSrc.bottom - Bmp[MATTHIAS].rcSrc.top);
		Bmp[MATTHIAS].rcDes.left = MAXX / 2 - Bmp[MATTHIAS].Breite / 2;
		Bmp[MATTHIAS].rcDes.top = MAXY - Bmp[MATTHIAS].Hoehe / 2;
		Bmp[MATTHIAS].rcDes.right = Bmp[MATTHIAS].rcDes.left + Bmp[MATTHIAS].Breite;
		Bmp[MATTHIAS].rcDes.bottom = Bmp[MATTHIAS].rcDes.top + Bmp[MATTHIAS].Hoehe;
		Bmp[MATTHIAS].Surface = lpDDSCredits;

		// TESTSPIELER
		Bmp[TESTSPIELER].Anzahl = 1;
		Bmp[TESTSPIELER].rcSrc.left = 0;
		Bmp[TESTSPIELER].rcSrc.top = 143;
		Bmp[TESTSPIELER].rcSrc.right = Bmp[TESTSPIELER].rcSrc.left + 210;
		Bmp[TESTSPIELER].rcSrc.bottom = Bmp[TESTSPIELER].rcSrc.top + 55;
		Bmp[TESTSPIELER].Breite = (short)(Bmp[TESTSPIELER].rcSrc.right - Bmp[TESTSPIELER].rcSrc.left);
		Bmp[TESTSPIELER].Hoehe = (short)(Bmp[TESTSPIELER].rcSrc.bottom - Bmp[TESTSPIELER].rcSrc.top);
		Bmp[TESTSPIELER].rcDes.left = MAXX / 2 - Bmp[TESTSPIELER].Breite / 2;
		Bmp[TESTSPIELER].rcDes.top = MAXY - Bmp[TESTSPIELER].Hoehe / 2;
		Bmp[TESTSPIELER].rcDes.right = Bmp[TESTSPIELER].rcDes.left + Bmp[TESTSPIELER].Breite;
		Bmp[TESTSPIELER].rcDes.bottom = Bmp[TESTSPIELER].rcDes.top + Bmp[TESTSPIELER].Hoehe;
		Bmp[TESTSPIELER].Surface = lpDDSCredits;

		// TOBIAS
		Bmp[TOBIAS].Anzahl = 1;
		Bmp[TOBIAS].rcSrc.left = 0;
		Bmp[TOBIAS].rcSrc.top = 198;
		Bmp[TOBIAS].rcSrc.right = Bmp[TOBIAS].rcSrc.left + 273;
		Bmp[TOBIAS].rcSrc.bottom = Bmp[TOBIAS].rcSrc.top + 56;
		Bmp[TOBIAS].Breite = (short)(Bmp[TOBIAS].rcSrc.right - Bmp[TOBIAS].rcSrc.left);
		Bmp[TOBIAS].Hoehe = (short)(Bmp[TOBIAS].rcSrc.bottom - Bmp[TOBIAS].rcSrc.top);
		Bmp[TOBIAS].rcDes.left = MAXX / 2 - Bmp[TOBIAS].Breite / 2;
		Bmp[TOBIAS].rcDes.top = MAXY - Bmp[TOBIAS].Hoehe / 2;
		Bmp[TOBIAS].rcDes.right = Bmp[TOBIAS].rcDes.left + Bmp[TOBIAS].Breite;
		Bmp[TOBIAS].rcDes.bottom = Bmp[TOBIAS].rcDes.top + Bmp[TOBIAS].Hoehe;
		Bmp[TOBIAS].Surface = lpDDSCredits;

		// SIGRID
		Bmp[SIGRID].Anzahl = 1;
		Bmp[SIGRID].rcSrc.left = 0;
		Bmp[SIGRID].rcSrc.top = 254;
		Bmp[SIGRID].rcSrc.right = Bmp[SIGRID].rcSrc.left + 226;
		Bmp[SIGRID].rcSrc.bottom = Bmp[SIGRID].rcSrc.top + 56;
		Bmp[SIGRID].Breite = (short)(Bmp[SIGRID].rcSrc.right - Bmp[SIGRID].rcSrc.left);
		Bmp[SIGRID].Hoehe = (short)(Bmp[SIGRID].rcSrc.bottom - Bmp[SIGRID].rcSrc.top);
		Bmp[SIGRID].rcDes.left = MAXX / 2 - Bmp[SIGRID].Breite / 2;
		Bmp[SIGRID].rcDes.top = MAXY - Bmp[SIGRID].Hoehe / 2;
		Bmp[SIGRID].rcDes.right = Bmp[SIGRID].rcDes.left + Bmp[SIGRID].Breite;
		Bmp[SIGRID].rcDes.bottom = Bmp[SIGRID].rcDes.top + Bmp[SIGRID].Hoehe;
		Bmp[SIGRID].Surface = lpDDSCredits;

		// PATHFINDING
		Bmp[PATHFINDING].Anzahl = 1;
		Bmp[PATHFINDING].rcSrc.left = 0;
		Bmp[PATHFINDING].rcSrc.top = 353;
		Bmp[PATHFINDING].rcSrc.right = Bmp[PATHFINDING].rcSrc.left + 233;
		Bmp[PATHFINDING].rcSrc.bottom = Bmp[PATHFINDING].rcSrc.top + 59;
		Bmp[PATHFINDING].Breite = (short)(Bmp[PATHFINDING].rcSrc.right - Bmp[PATHFINDING].rcSrc.left);
		Bmp[PATHFINDING].Hoehe = (short)(Bmp[PATHFINDING].rcSrc.bottom - Bmp[PATHFINDING].rcSrc.top);
		Bmp[PATHFINDING].rcDes.left = MAXX / 2 - Bmp[PATHFINDING].Breite / 2;
		Bmp[PATHFINDING].rcDes.top = MAXY - Bmp[PATHFINDING].Hoehe / 2;
		Bmp[PATHFINDING].rcDes.right = Bmp[PATHFINDING].rcDes.left + Bmp[PATHFINDING].Breite;
		Bmp[PATHFINDING].rcDes.bottom = Bmp[PATHFINDING].rcDes.top + Bmp[PATHFINDING].Hoehe;
		Bmp[PATHFINDING].Surface = lpDDSCredits;

		// JOHN
		Bmp[JOHN].Anzahl = 1;
		Bmp[JOHN].rcSrc.left = 0;
		Bmp[JOHN].rcSrc.top = 412;
		Bmp[JOHN].rcSrc.right = Bmp[JOHN].rcSrc.left + 347;
		Bmp[JOHN].rcSrc.bottom = Bmp[JOHN].rcSrc.top + 56;
		Bmp[JOHN].Breite = (short)(Bmp[JOHN].rcSrc.right - Bmp[JOHN].rcSrc.left);
		Bmp[JOHN].Hoehe = (short)(Bmp[JOHN].rcSrc.bottom - Bmp[JOHN].rcSrc.top);
		Bmp[JOHN].rcDes.left = MAXX / 2 - Bmp[JOHN].Breite / 2;
		Bmp[JOHN].rcDes.top = MAXY - Bmp[JOHN].Hoehe / 2;
		Bmp[JOHN].rcDes.right = Bmp[JOHN].rcDes.left + Bmp[JOHN].Breite;
		Bmp[JOHN].rcDes.bottom = Bmp[JOHN].rcDes.top + Bmp[JOHN].Hoehe;
		Bmp[JOHN].Surface = lpDDSCredits;

		// HEIKO
		Bmp[HEIKO].Anzahl = 1;
		Bmp[HEIKO].rcSrc.left = 0;
		Bmp[HEIKO].rcSrc.top = 468;
		Bmp[HEIKO].rcSrc.right = Bmp[HEIKO].rcSrc.left + 236;
		Bmp[HEIKO].rcSrc.bottom = Bmp[HEIKO].rcSrc.top + 47;
		Bmp[HEIKO].Breite = (short)(Bmp[HEIKO].rcSrc.right - Bmp[HEIKO].rcSrc.left);
		Bmp[HEIKO].Hoehe = (short)(Bmp[HEIKO].rcSrc.bottom - Bmp[HEIKO].rcSrc.top);
		Bmp[HEIKO].rcDes.left = MAXX / 2 - Bmp[HEIKO].Breite / 2;
		Bmp[HEIKO].rcDes.top = MAXY - Bmp[HEIKO].Hoehe / 2;
		Bmp[HEIKO].rcDes.right = Bmp[HEIKO].rcDes.left + Bmp[HEIKO].Breite;
		Bmp[HEIKO].rcDes.bottom = Bmp[HEIKO].rcDes.top + Bmp[HEIKO].Hoehe;
		Bmp[HEIKO].Surface = lpDDSCredits;

		// GISELA
		Bmp[GISELA].Anzahl = 1;
		Bmp[GISELA].rcSrc.left = 0;
		Bmp[GISELA].rcSrc.top = 515;
		Bmp[GISELA].rcSrc.right = Bmp[GISELA].rcSrc.left + 232;
		Bmp[GISELA].rcSrc.bottom = Bmp[GISELA].rcSrc.top + 47;
		Bmp[GISELA].Breite = (short)(Bmp[GISELA].rcSrc.right - Bmp[GISELA].rcSrc.left);
		Bmp[GISELA].Hoehe = (short)(Bmp[GISELA].rcSrc.bottom - Bmp[GISELA].rcSrc.top);
		Bmp[GISELA].rcDes.left = MAXX / 2 - Bmp[GISELA].Breite / 2;
		Bmp[GISELA].rcDes.top = MAXY - Bmp[GISELA].Hoehe / 2;
		Bmp[GISELA].rcDes.right = Bmp[GISELA].rcDes.left + Bmp[GISELA].Breite;
		Bmp[GISELA].rcDes.bottom = Bmp[GISELA].rcDes.top + Bmp[GISELA].Hoehe;
		Bmp[GISELA].Surface = lpDDSCredits;

		// WEITEREHILFE
		Bmp[WEITEREHILFE].Anzahl = 1;
		Bmp[WEITEREHILFE].rcSrc.left = 0;
		Bmp[WEITEREHILFE].rcSrc.top = 562;
		Bmp[WEITEREHILFE].rcSrc.right = Bmp[WEITEREHILFE].rcSrc.left + 258;
		Bmp[WEITEREHILFE].rcSrc.bottom = Bmp[WEITEREHILFE].rcSrc.top + 46;
		Bmp[WEITEREHILFE].Breite = (short)(Bmp[WEITEREHILFE].rcSrc.right - Bmp[WEITEREHILFE].rcSrc.left);
		Bmp[WEITEREHILFE].Hoehe = (short)(Bmp[WEITEREHILFE].rcSrc.bottom - Bmp[WEITEREHILFE].rcSrc.top);
		Bmp[WEITEREHILFE].rcDes.left = MAXX / 2 - Bmp[WEITEREHILFE].Breite / 2;
		Bmp[WEITEREHILFE].rcDes.top = MAXY - Bmp[WEITEREHILFE].Hoehe / 2;
		Bmp[WEITEREHILFE].rcDes.right = Bmp[WEITEREHILFE].rcDes.left + Bmp[WEITEREHILFE].Breite;
		Bmp[WEITEREHILFE].rcDes.bottom = Bmp[WEITEREHILFE].rcDes.top + Bmp[WEITEREHILFE].Hoehe;
		Bmp[WEITEREHILFE].Surface = lpDDSCredits;

		// DPSOFTWARE
		Bmp[DPSOFTWARE].Anzahl = 1;
		Bmp[DPSOFTWARE].rcSrc.left = 0;
		Bmp[DPSOFTWARE].rcSrc.top = 608;
		Bmp[DPSOFTWARE].rcSrc.right = Bmp[DPSOFTWARE].rcSrc.left + 291;
		Bmp[DPSOFTWARE].rcSrc.bottom = Bmp[DPSOFTWARE].rcSrc.top + 99;
		Bmp[DPSOFTWARE].Breite = (short)(Bmp[DPSOFTWARE].rcSrc.right - Bmp[DPSOFTWARE].rcSrc.left);
		Bmp[DPSOFTWARE].Hoehe = (short)(Bmp[DPSOFTWARE].rcSrc.bottom - Bmp[DPSOFTWARE].rcSrc.top);
		Bmp[DPSOFTWARE].rcDes.left = MAXX / 2 - Bmp[DPSOFTWARE].Breite / 2;
		Bmp[DPSOFTWARE].rcDes.top = MAXY - Bmp[DPSOFTWARE].Hoehe / 2;
		Bmp[DPSOFTWARE].rcDes.right = Bmp[DPSOFTWARE].rcDes.left + Bmp[DPSOFTWARE].Breite;
		Bmp[DPSOFTWARE].rcDes.bottom = Bmp[DPSOFTWARE].rcDes.top + Bmp[DPSOFTWARE].Hoehe;
		Bmp[DPSOFTWARE].Surface = lpDDSCredits;

		// SCHWARZ
		Bmp[SCHWARZ].Anzahl = 1;
		Bmp[SCHWARZ].rcSrc.left = 0;
		Bmp[SCHWARZ].rcSrc.top = 608;
		Bmp[SCHWARZ].rcSrc.right = Bmp[SCHWARZ].rcSrc.left + 1;
		Bmp[SCHWARZ].rcSrc.bottom = Bmp[SCHWARZ].rcSrc.top + 1;
		Bmp[SCHWARZ].Breite = (short)(Bmp[SCHWARZ].rcSrc.right - Bmp[SCHWARZ].rcSrc.left);
		Bmp[SCHWARZ].Hoehe = (short)(Bmp[SCHWARZ].rcSrc.bottom - Bmp[SCHWARZ].rcSrc.top);
		Bmp[SCHWARZ].rcDes.left = MAXX / 2 - Bmp[SCHWARZ].Breite / 2;
		Bmp[SCHWARZ].rcDes.top = MAXY - Bmp[SCHWARZ].Hoehe / 2;
		Bmp[SCHWARZ].rcDes.right = Bmp[SCHWARZ].rcDes.left + Bmp[SCHWARZ].Breite;
		Bmp[SCHWARZ].rcDes.bottom = Bmp[SCHWARZ].rcDes.top + Bmp[SCHWARZ].Hoehe;
		Bmp[SCHWARZ].Surface = lpDDSCredits;

		// SOUNDS
		Bmp[SOUNDS].Anzahl = 1;
		Bmp[SOUNDS].rcSrc.left = 0;
		Bmp[SOUNDS].rcSrc.top = 310;
		Bmp[SOUNDS].rcSrc.right = Bmp[SOUNDS].rcSrc.left + 144;
		Bmp[SOUNDS].rcSrc.bottom = Bmp[SOUNDS].rcSrc.top + 43;
		Bmp[SOUNDS].Breite = (short)(Bmp[SOUNDS].rcSrc.right - Bmp[SOUNDS].rcSrc.left);
		Bmp[SOUNDS].Hoehe = (short)(Bmp[SOUNDS].rcSrc.bottom - Bmp[SOUNDS].rcSrc.top);
		Bmp[SOUNDS].rcDes.left = MAXX / 2 - Bmp[SOUNDS].Breite / 2;
		Bmp[SOUNDS].rcDes.top = MAXY - Bmp[SOUNDS].Hoehe / 2;
		Bmp[SOUNDS].rcDes.right = Bmp[SOUNDS].rcDes.left + Bmp[SOUNDS].Breite;
		Bmp[SOUNDS].rcDes.bottom = Bmp[SOUNDS].rcDes.top + Bmp[SOUNDS].Hoehe;
		Bmp[SOUNDS].Surface = lpDDSCredits;

		// MUSIK
		Bmp[MUSIK].Anzahl = 1;
		Bmp[MUSIK].rcSrc.left = 160;
		Bmp[MUSIK].rcSrc.top = 310;
		Bmp[MUSIK].rcSrc.right = Bmp[MUSIK].rcSrc.left + 124;
		Bmp[MUSIK].rcSrc.bottom = Bmp[MUSIK].rcSrc.top + 39;
		Bmp[MUSIK].Breite = (short)(Bmp[MUSIK].rcSrc.right - Bmp[MUSIK].rcSrc.left);
		Bmp[MUSIK].Hoehe = (short)(Bmp[MUSIK].rcSrc.bottom - Bmp[MUSIK].rcSrc.top);
		Bmp[MUSIK].rcDes.left = MAXX / 2 - Bmp[MUSIK].Breite / 2;
		Bmp[MUSIK].rcDes.top = MAXY - Bmp[MUSIK].Hoehe / 2;
		Bmp[MUSIK].rcDes.right = Bmp[MUSIK].rcDes.left + Bmp[MUSIK].Breite;
		Bmp[MUSIK].rcDes.bottom = Bmp[MUSIK].rcDes.top + Bmp[MUSIK].Hoehe;
		Bmp[MUSIK].Surface = lpDDSCredits;

		for (i = 0; i<10; i++) for (k = 0; k<10; k++)
		{
			AbspannListe[i][k].Aktiv = false;
			AbspannListe[i][k].Bild = -1;
		}
		AbspannListe[0][0].Bild = PROGRAMMIERUNG;
		AbspannListe[0][1].Aktiv = true; // nur den hier true setzen, löst dann alles andere aus
		AbspannListe[0][1].Bild = DIRKPLATE;
		AbspannListe[1][0].Bild = MUSIK;
		AbspannListe[1][1].Bild = HEIKO;
		AbspannListe[2][0].Bild = SOUNDS;
		AbspannListe[2][1].Bild = DIRKPLATE;
		AbspannListe[3][0].Bild = TESTSPIELER;
		AbspannListe[3][1].Bild = MATTHIAS;
		AbspannListe[3][2].Bild = TOBIAS;
		AbspannListe[3][3].Bild = SIGRID;
		AbspannListe[4][0].Bild = PATHFINDING;
		AbspannListe[4][1].Bild = JOHN;
		AbspannListe[5][0].Bild = WEITEREHILFE;
		AbspannListe[5][1].Bild = HEIKO;
		AbspannListe[5][2].Bild = GISELA;
		AbspannListe[6][0].Bild = SCHWARZ;
		AbspannListe[6][1].Bild = DPSOFTWARE;

		// Sounds
		for (i = 0; i<Sound::COUNT; i++)
		{
			Wav[i].Dateiname = "sounds\\klick.wav";
			Wav[i].Loop = false;
			Wav[i].Volume = 100;
		}

		Wav[Sound::STORM].Dateiname = "sounds\\sturm.wav";
		Wav[Sound::STORM].Volume = 85;

		Wav[Sound::SWIM].Dateiname = "sounds\\schwimmen.wav";
		Wav[Sound::SWIM].Volume = 90;

		Wav[Sound::SPLAT].Dateiname = "sounds\\platsch.wav";
		Wav[Sound::SPLAT].Volume = 95;

		Wav[Sound::LOG].Dateiname = "sounds\\faellen.wav";
		Wav[Sound::LOG].Volume = 100;

		Wav[Sound::HIT].Dateiname = "sounds\\schlagen.wav";
		Wav[Sound::HIT].Volume = 100;

		Wav[Sound::SLINGSHOT].Dateiname = "sounds\\schleuder.wav";
		Wav[Sound::SLINGSHOT].Volume = 100;

		Wav[Sound::DIG].Dateiname = "sounds\\schaufeln.wav";
		Wav[Sound::DIG].Volume = 90;

		Wav[Sound::HAMMER].Dateiname = "sounds\\hammer.wav";
		Wav[Sound::HAMMER].Volume = 100;

		Wav[Sound::CRASH].Dateiname = "sounds\\crash.wav";
		Wav[Sound::CRASH].Volume = 100;

		Wav[Sound::SNORE].Dateiname = "sounds\\schnarchen.wav";
		Wav[Sound::SNORE].Volume = 90;

		Wav[Sound::DRINK].Dateiname = "sounds\\trinken.wav";
		Wav[Sound::DRINK].Volume = 95;

		Wav[Sound::CRACKLE].Dateiname = "sounds\\knistern.wav";
		Wav[Sound::CRACKLE].Volume = 90;

		Wav[Sound::FISH].Dateiname = "sounds\\angel.wav";
		Wav[Sound::FISH].Volume = 100;

		Wav[Sound::FOREST].Dateiname = "sounds\\wald.wav";
		Wav[Sound::FOREST].Volume = 90;

		Wav[Sound::FIRE].Dateiname = "sounds\\feuer.wav";
		Wav[Sound::FIRE].Volume = 100;

		Wav[Sound::SURF].Dateiname = "sounds\\brandung.wav";
		Wav[Sound::SURF].Volume = 100;

		Wav[Sound::TIMBER].Dateiname = "sounds\\baumfaellt.wav";
		Wav[Sound::TIMBER].Volume = 100;

		Wav[Sound::RIVER].Dateiname = "sounds\\fluss.wav";
		Wav[Sound::RIVER].Volume = 85;

		Wav[Sound::CLICK].Dateiname = "sounds\\klick.wav";
		Wav[Sound::CLICK].Volume = 95;

		Wav[Sound::CLICK2].Dateiname = "sounds\\klick2.wav";
		Wav[Sound::CLICK2].Volume = 95;

		Wav[Sound::OUTRO].Dateiname = "sounds\\abspann.wav";
		Wav[Sound::OUTRO].Volume = 100;
		Wav[Sound::OUTRO].Loop = true;

		Wav[Sound::LOGO].Dateiname = "sounds\\logo.wav";
		Wav[Sound::LOGO].Volume = 100;
		Wav[Sound::LOGO].Loop = true;

		Wav[Sound::WOLF].Dateiname = "sounds\\wolf.wav";
		Wav[Sound::WOLF].Volume = 90;

		Wav[Sound::INVENTION].Dateiname = "sounds\\erfindung.wav";
		Wav[Sound::INVENTION].Volume = 95;

		// Testweise alle Sounds gleich in den Speicher laden
		for (i = 1; i<Sound::COUNT; i++) Sound::LoadSound(i);

		// Textausgabe
		TextBereich[TXTTEXTFELD].Aktiv = false;
		TextBereich[TXTTEXTFELD].rcText.left = 9;
		TextBereich[TXTTEXTFELD].rcText.top = MAXY - 17;
		TextBereich[TXTTEXTFELD].rcText.right = MAXX - 200;
		TextBereich[TXTTEXTFELD].rcText.bottom = MAXY - 2;

		TextBereich[TXTFPS].Aktiv = false;
		TextBereich[TXTFPS].rcText.left = MAXX - 40;
		TextBereich[TXTFPS].rcText.top = 3;
		TextBereich[TXTFPS].rcText.right = TextBereich[TXTFPS].rcText.left + 2 * S1XPIXEL;
		TextBereich[TXTFPS].rcText.bottom = TextBereich[TXTFPS].rcText.top + S1YPIXEL;

		TextBereich[TXTTAGESZEIT].Aktiv = true;
		TextBereich[TXTTAGESZEIT].rcText.left = MAXX - 110;
		TextBereich[TXTTAGESZEIT].rcText.top = MAXY - 20;
		TextBereich[TXTTAGESZEIT].rcText.right = TextBereich[TXTTAGESZEIT].rcText.left + 5 * S2XPIXEL;
		TextBereich[TXTTAGESZEIT].rcText.bottom = TextBereich[TXTTAGESZEIT].rcText.top + S2YPIXEL;

		TextBereich[TXTPAPIER].Aktiv = false;
		TextBereich[TXTPAPIER].rcText.left = 450;
		TextBereich[TXTPAPIER].rcText.top = 160;
		TextBereich[TXTPAPIER].rcText.right = 830;
		TextBereich[TXTPAPIER].rcText.bottom = 560;

		TextBereich[TXTCHANCE].Aktiv = false;
		TextBereich[TXTCHANCE].rcText.left = Bmp[RING].rcDes.left + 5;
		TextBereich[TXTCHANCE].rcText.top = Bmp[RING].rcDes.top + Bmp[RING].Hoehe + 10;
		TextBereich[TXTCHANCE].rcText.right = TextBereich[TXTCHANCE].rcText.left + 3 * S2XPIXEL;
		TextBereich[TXTCHANCE].rcText.bottom = TextBereich[TXTCHANCE].rcText.top + S2YPIXEL;

		Guy.Resource[WASSER] = 100;
		Guy.Resource[NAHRUNG] = 100;
		Guy.Resource[GESUNDHEIT] = 100;

		for (i = ROHAST; i <= ROHSCHLEUDER; i++)
		{
			Guy.Inventar[i] = 0;
		}
		CursorTyp = CUPFEIL;
		Gitter = false;
		PapierText = -1;
		HauptMenue = 0;
		TwoClicks = -1;
		Nacht = false;
		Frage = -1;
		LastBild = 100;
		Bild = 0;
		MousePosition.x = MAXX / 2;
		MousePosition.y = MAXY / 2;
		Button0down = false;
		Button1down = false;
		SchatzPos.x = -1;
		SchatzPos.y = -1;
		SchatzGef = false;
	}

	void NeuesSpiel(bool neu)
	{
		short x, y;
		bool LoadOK;
		
		Game::InitStructs();
		
		if (!neu) LoadOK = Game::LoadGame();

		if ((!LoadOK) || (neu))
		{
			// Für die Statusanzeige
			rcRectdes.left = 0;
			rcRectdes.top = 0;
			rcRectdes.right = MAXX;
			rcRectdes.bottom = MAXY;
			ddbltfx.dwFillColor = Renderer::RGB2DWORD(70, 70, 100);
			lpDDSPrimary->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);
			ddbltfx.dwFillColor = Renderer::RGB2DWORD(255, 0, 255);
			lpDDSSchrift->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

			// Landschaft erzeugen

			Renderer::DrawString("Erschaffe Landschaft...", 5, 5, 2);
			rcRectdes.left = 0;
			rcRectdes.top = 0;
			rcRectdes.right = MAXX;
			rcRectdes.bottom = MAXY;
			lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
			World::Compute(200, 600);

			Renderer::DrawString("Ueberflute Land...", 5, 35, 2);
			rcRectdes.left = 0;
			rcRectdes.top = 0;
			rcRectdes.right = MAXX;
			rcRectdes.bottom = MAXY;
			lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
			World::Meer();

			Renderer::DrawString("Lege Fluss fest...", 5, 65, 2);
			rcRectdes.left = 0;
			rcRectdes.top = 0;
			rcRectdes.right = MAXX;
			rcRectdes.bottom = MAXY;
			lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
			World::Fluss();
			Math::CalcKoor();

			Renderer::DrawString("Pflanze Baeume...", 5, 95, 2);
			rcRectdes.left = 0;
			rcRectdes.top = 0;
			rcRectdes.right = MAXX;
			rcRectdes.bottom = MAXY;
			lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
			World::Baeume(30);

			World::Piratenwrack();

			// Guy Position
			Guy.Pos.x = 1;
			Guy.Pos.y = MAXYKACH / 2;
			Guy.PosScreen.x =
				(Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][0].x +
				Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][2].x) / 2;
			Guy.PosScreen.y =
				(Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][1].y +
				Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Typ][3].y) / 2;

			Camera.x = Guy.PosScreen.x - static_cast<short>(rcGesamt.right / 2);
			Camera.y = Guy.PosScreen.y - static_cast<short>(rcGesamt.bottom / 2);

			Chance = 0;

			BootsFahrt = false;
			if (!BootsFahrt) World::ChangeBootsFahrt();

			Tag = 1; Stunden = 0; Minuten = 0;

			Spielzustand = State::INTRO;
			Guy.Aktiv = false;
			Guy.Zustand = GUYSCHIFF;
			Guy.AkNummer = 0;
			Guy.Aktion = Action::INTRO;
		}

		// SchriftSurface löschen
		rcRectdes.left = 0;
		rcRectdes.top = 0;
		rcRectdes.right = MAXX;
		rcRectdes.bottom = MAXY;
		ddbltfx.dwFillColor = Renderer::RGB2DWORD(255, 0, 255);
		lpDDSSchrift->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

		BOOL Anitmp = LAnimation;
		bool Entdeckttmp[MAXXKACH][MAXYKACH];

		LAnimation = false;
		// Schatzvergraben und Schatzkarte malen
		for (y = 0; y<MAXYKACH; y++) for (x = 0; x<MAXXKACH; x++)
		{
			Entdeckttmp[x][y] = Scape[x][y].Entdeckt;
			Scape[x][y].Entdeckt = true;
		}
		
		World::Generate(); // Einmal vor dem Schatz schon entdeckt malen
		World::Schatz();

		for (y = 0; y<MAXYKACH; y++) for (x = 0; x<MAXXKACH; x++) Scape[x][y].Entdeckt = Entdeckttmp[x][y];
		World::Entdecken();
		LAnimation = Anitmp;
		World::Generate(); // Und nochmal ohne das die Gegend entdeckt ist
		Guy.PosAlt = Guy.PosScreen;
	}

} // namespace Game
