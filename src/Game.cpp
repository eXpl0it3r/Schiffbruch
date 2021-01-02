#include "Game.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

#include <fstream>

#include "extern.hpp"

namespace Game
{
	void SaveGame()
	{
		std::ofstream ofs("save.dat", std::ios::binary);
		if (!ofs)
		{
			return;
		}

		ofs.write(reinterpret_cast<char*>(Scape), sizeof(Scape));
		ofs.write(reinterpret_cast<char*>(&Guy), sizeof(Guy));
		ofs.write(reinterpret_cast<char*>(&BootsFahrt), sizeof(BootsFahrt));
		ofs.write(reinterpret_cast<char*>(&Camera), sizeof(Camera));
		ofs.write(reinterpret_cast<char*>(&Chance), sizeof(Chance));
		ofs.write(reinterpret_cast<char*>(&Gitter), sizeof(Gitter));
		ofs.write(reinterpret_cast<char*>(&HauptMenue), sizeof(HauptMenue));
		ofs.write(reinterpret_cast<char*>(&LAnimation), sizeof(LAnimation));
		ofs.write(reinterpret_cast<char*>(&Minuten), sizeof(Minuten));
		ofs.write(reinterpret_cast<char*>(&ScapeGrenze), sizeof(ScapeGrenze));
		ofs.write(reinterpret_cast<char*>(&SchatzPos), sizeof(SchatzPos));
		ofs.write(reinterpret_cast<char*>(&Spielzustand), sizeof(Spielzustand));
		ofs.write(reinterpret_cast<char*>(&Stunden), sizeof(Stunden));
		ofs.write(reinterpret_cast<char*>(&Tag), sizeof(Tag));
		ofs.write(reinterpret_cast<char*>(TextBereich), sizeof(TextBereich));
		ofs.write(reinterpret_cast<char*>(&SchatzGef), sizeof(SchatzGef));

		for (auto& bmp : Bmp)
		{
			ofs.write(reinterpret_cast<char*>(&bmp.Animation), sizeof(bmp.Animation));
			ofs.write(reinterpret_cast<char*>(&bmp.Phase), sizeof(bmp.Phase));
			ofs.write(reinterpret_cast<char*>(&bmp.First), sizeof(bmp.First));
		}
	}

	bool LoadGame()
	{
		std::ifstream ifs("save.dat", std::ios::binary); // | ios::nocreate);
		if (!ifs)
		{
			return (false);
		}

		ifs.read(reinterpret_cast<char*>(Scape), sizeof(Scape));
		ifs.read(reinterpret_cast<char*>(&Guy), sizeof(Guy));
		ifs.read(reinterpret_cast<char*>(&BootsFahrt), sizeof(BootsFahrt));
		ifs.read(reinterpret_cast<char*>(&Camera), sizeof(Camera));
		ifs.read(reinterpret_cast<char*>(&Chance), sizeof(Chance));
		ifs.read(reinterpret_cast<char*>(&Gitter), sizeof(Gitter));
		ifs.read(reinterpret_cast<char*>(&HauptMenue), sizeof(HauptMenue));
		ifs.read(reinterpret_cast<char*>(&LAnimation), sizeof(LAnimation));
		ifs.read(reinterpret_cast<char*>(&Minuten), sizeof(Minuten));
		ifs.read(reinterpret_cast<char*>(&ScapeGrenze), sizeof(ScapeGrenze));
		ifs.read(reinterpret_cast<char*>(&SchatzPos), sizeof(SchatzPos));
		ifs.read(reinterpret_cast<char*>(&Spielzustand), sizeof(Spielzustand));
		ifs.read(reinterpret_cast<char*>(&Stunden), sizeof(Stunden));
		ifs.read(reinterpret_cast<char*>(&Tag), sizeof(Tag));
		ifs.read(reinterpret_cast<char*>(TextBereich), sizeof(TextBereich));
		ifs.read(reinterpret_cast<char*>(&SchatzGef), sizeof(SchatzGef));

		for (auto& bmp : Bmp)
		{
			ifs.read(reinterpret_cast<char*>(&bmp.Animation), sizeof(bmp.Animation));
			ifs.read(reinterpret_cast<char*>(&bmp.Phase), sizeof(bmp.Phase));
			ifs.read(reinterpret_cast<char*>(&bmp.First), sizeof(bmp.First));
		}

		return true;
	}

	void InitializeStructs()
	{
		short i, k;

		// BILD
		// Standardbildinitialisierung
		for (i = 0; i < ImageCount; i++)
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

			for (k = 0; k < ImageCount; k++)
			{
				Bmp[i].Rohstoff[k] = 0;
			}

			Bmp[i].AkAnzahl = 0;
			Bmp[i].First = true;
		}

        // Guy
        for (i = GuyLinks; i <= GuyUnten; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Anzahl = 4;
            Bmp[i].Geschwindigkeit = 20;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSGuyAni;
            Bmp[i].rcSrc.left = 7 * (i - GuyLinks);
            Bmp[i].rcSrc.right = 7 + 7 * (i - GuyLinks);
            Bmp[i].rcSrc.top = 0;
            Bmp[i].rcSrc.bottom = 18;
            Bmp[i].Breite = 7;
            Bmp[i].Hoehe = 18;
        }

        for (i = GuySuchen; i <= GuySchleuder; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSGuyAni;
        }

        Bmp[GuySuchen].Anzahl = 4;
        Bmp[GuySuchen].Geschwindigkeit = 4;
        Bmp[GuySuchen].rcSrc.left = 28;
        Bmp[GuySuchen].rcSrc.right = 39;
        Bmp[GuySuchen].rcSrc.top = 0;
        Bmp[GuySuchen].rcSrc.bottom = 14;
        Bmp[GuySuchen].Breite = 11;
        Bmp[GuySuchen].Hoehe = 14;
        Bmp[GuySuchen].Sound = Sound::Crackle;

        Bmp[GuyEssen].Anzahl = 4;
        Bmp[GuyEssen].Geschwindigkeit = 4;
        Bmp[GuyEssen].rcSrc.left = 39;
        Bmp[GuyEssen].rcSrc.right = 39 + 7;
        Bmp[GuyEssen].rcSrc.top = 0;
        Bmp[GuyEssen].rcSrc.bottom = 17;
        Bmp[GuyEssen].Breite = 7;
        Bmp[GuyEssen].Hoehe = 17;
        Bmp[GuyEssen].Sound = Sound::Crackle;

        Bmp[GuyTrinken].Anzahl = 5;
        Bmp[GuyTrinken].Geschwindigkeit = 4;
        Bmp[GuyTrinken].rcSrc.left = 46;
        Bmp[GuyTrinken].rcSrc.right = 46 + 9;
        Bmp[GuyTrinken].rcSrc.top = 0;
        Bmp[GuyTrinken].rcSrc.bottom = 13;
        Bmp[GuyTrinken].Breite = 9;
        Bmp[GuyTrinken].Hoehe = 13;
        Bmp[GuyTrinken].Sound = Sound::Drink;

        Bmp[GuyFaellen].Anzahl = 4;
        Bmp[GuyFaellen].Geschwindigkeit = 6;
        Bmp[GuyFaellen].rcSrc.left = 55;
        Bmp[GuyFaellen].rcSrc.right = 55 + 15;
        Bmp[GuyFaellen].rcSrc.top = 0;
        Bmp[GuyFaellen].rcSrc.bottom = 19;
        Bmp[GuyFaellen].Breite = 15;
        Bmp[GuyFaellen].Hoehe = 19;
        Bmp[GuyFaellen].Sound = Sound::Log;

        Bmp[GuyWarten].Anzahl = 4;
        Bmp[GuyWarten].Geschwindigkeit = 2;
        Bmp[GuyWarten].rcSrc.left = 70;
        Bmp[GuyWarten].rcSrc.right = 70 + 7;
        Bmp[GuyWarten].rcSrc.top = 0;
        Bmp[GuyWarten].rcSrc.bottom = 18;
        Bmp[GuyWarten].Breite = 7;
        Bmp[GuyWarten].Hoehe = 18;

        Bmp[GuyFeld].Anzahl = 4;
        Bmp[GuyFeld].Geschwindigkeit = 4;
        Bmp[GuyFeld].rcSrc.left = 78;
        Bmp[GuyFeld].rcSrc.right = 78 + 19;
        Bmp[GuyFeld].rcSrc.top = 0;
        Bmp[GuyFeld].rcSrc.bottom = 18;
        Bmp[GuyFeld].Breite = 19;
        Bmp[GuyFeld].Hoehe = 18;

        Bmp[GuyBindenOben].Anzahl = 2;
        Bmp[GuyBindenOben].Geschwindigkeit = 1;
        Bmp[GuyBindenOben].rcSrc.left = 97;
        Bmp[GuyBindenOben].rcSrc.right = 97 + 8;
        Bmp[GuyBindenOben].rcSrc.top = 0;
        Bmp[GuyBindenOben].rcSrc.bottom = 18;
        Bmp[GuyBindenOben].Breite = 8;
        Bmp[GuyBindenOben].Hoehe = 18;

        Bmp[GuyBindenUnten].Anzahl = 2;
        Bmp[GuyBindenUnten].Geschwindigkeit = 1;
        Bmp[GuyBindenUnten].rcSrc.left = 98;
        Bmp[GuyBindenUnten].rcSrc.right = 98 + 7;
        Bmp[GuyBindenUnten].rcSrc.top = 36;
        Bmp[GuyBindenUnten].rcSrc.bottom = 36 + 18;
        Bmp[GuyBindenUnten].Breite = 7;
        Bmp[GuyBindenUnten].Hoehe = 18;

        Bmp[GuySchlafZelt].Anzahl = 2;
        Bmp[GuySchlafZelt].Geschwindigkeit = 1;
        Bmp[GuySchlafZelt].rcSrc.left = 105;
        Bmp[GuySchlafZelt].rcSrc.right = 105 + 20;
        Bmp[GuySchlafZelt].rcSrc.top = 54;
        Bmp[GuySchlafZelt].rcSrc.bottom = 54 + 10;
        Bmp[GuySchlafZelt].Breite = 20;
        Bmp[GuySchlafZelt].Hoehe = 10;
        Bmp[GuySchlafZelt].Sound = Sound::Snore;

        Bmp[GuySchlafen].Anzahl = 2;
        Bmp[GuySchlafen].Geschwindigkeit = 1;
        Bmp[GuySchlafen].rcSrc.left = 125;
        Bmp[GuySchlafen].rcSrc.right = 125 + 17;
        Bmp[GuySchlafen].rcSrc.top = 36;
        Bmp[GuySchlafen].rcSrc.bottom = 36 + 18;
        Bmp[GuySchlafen].Breite = 17;
        Bmp[GuySchlafen].Hoehe = 18;
        Bmp[GuySchlafen].Sound = Sound::Snore;

        Bmp[GuyGehInZelt].Anzahl = 3;
        Bmp[GuyGehInZelt].Geschwindigkeit = 4;
        Bmp[GuyGehInZelt].rcSrc.left = 105;
        Bmp[GuyGehInZelt].rcSrc.right = 105 + 7;
        Bmp[GuyGehInZelt].rcSrc.top = 0;
        Bmp[GuyGehInZelt].rcSrc.bottom = 0 + 18;
        Bmp[GuyGehInZelt].Breite = 7;
        Bmp[GuyGehInZelt].Hoehe = 18;

        Bmp[GuyHinlegen].Anzahl = 2;
        Bmp[GuyHinlegen].Geschwindigkeit = 2;
        Bmp[GuyHinlegen].rcSrc.left = 125;
        Bmp[GuyHinlegen].rcSrc.right = 125 + 17;
        Bmp[GuyHinlegen].rcSrc.top = 0;
        Bmp[GuyHinlegen].rcSrc.bottom = 0 + 18;
        Bmp[GuyHinlegen].Breite = 17;
        Bmp[GuyHinlegen].Hoehe = 18;

        Bmp[GuyAufstehen].Anzahl = 2;
        Bmp[GuyAufstehen].Geschwindigkeit = 2;
        Bmp[GuyAufstehen].rcSrc.left = 142;
        Bmp[GuyAufstehen].rcSrc.right = 142 + 9;
        Bmp[GuyAufstehen].rcSrc.top = 0;
        Bmp[GuyAufstehen].rcSrc.bottom = 0 + 18;
        Bmp[GuyAufstehen].Breite = 9;
        Bmp[GuyAufstehen].Hoehe = 18;

        Bmp[GuyAngeln1Links].Anzahl = 6;
        Bmp[GuyAngeln1Links].Geschwindigkeit = 6;
        Bmp[GuyAngeln1Links].rcSrc.left = 151;
        Bmp[GuyAngeln1Links].rcSrc.right = 151 + 16;
        Bmp[GuyAngeln1Links].rcSrc.top = 0;
        Bmp[GuyAngeln1Links].rcSrc.bottom = 0 + 17;
        Bmp[GuyAngeln1Links].Breite = 16;
        Bmp[GuyAngeln1Links].Hoehe = 17;

        Bmp[GuyAngeln1Oben].Anzahl = 6;
        Bmp[GuyAngeln1Oben].Geschwindigkeit = 6;
        Bmp[GuyAngeln1Oben].rcSrc.left = 167;
        Bmp[GuyAngeln1Oben].rcSrc.right = 167 + 16;
        Bmp[GuyAngeln1Oben].rcSrc.top = 0;
        Bmp[GuyAngeln1Oben].rcSrc.bottom = 0 + 17;
        Bmp[GuyAngeln1Oben].Breite = 16;
        Bmp[GuyAngeln1Oben].Hoehe = 17;

        Bmp[GuyAngeln1Rechts].Anzahl = 6;
        Bmp[GuyAngeln1Rechts].Geschwindigkeit = 6;
        Bmp[GuyAngeln1Rechts].rcSrc.left = 183;
        Bmp[GuyAngeln1Rechts].rcSrc.right = 183 + 14;
        Bmp[GuyAngeln1Rechts].rcSrc.top = 0;
        Bmp[GuyAngeln1Rechts].rcSrc.bottom = 0 + 17;
        Bmp[GuyAngeln1Rechts].Breite = 14;
        Bmp[GuyAngeln1Rechts].Hoehe = 17;

        Bmp[GuyAngeln1Unten].Anzahl = 6;
        Bmp[GuyAngeln1Unten].Geschwindigkeit = 6;
        Bmp[GuyAngeln1Unten].rcSrc.left = 197;
        Bmp[GuyAngeln1Unten].rcSrc.right = 197 + 14;
        Bmp[GuyAngeln1Unten].rcSrc.top = 0;
        Bmp[GuyAngeln1Unten].rcSrc.bottom = 0 + 17;
        Bmp[GuyAngeln1Unten].Breite = 14;
        Bmp[GuyAngeln1Unten].Hoehe = 17;

        Bmp[GuyAngeln2Links].Anzahl = 4;
        Bmp[GuyAngeln2Links].Geschwindigkeit = 3;
        Bmp[GuyAngeln2Links].rcSrc.left = 211;
        Bmp[GuyAngeln2Links].rcSrc.right = 211 + 16;
        Bmp[GuyAngeln2Links].rcSrc.top = 0;
        Bmp[GuyAngeln2Links].rcSrc.bottom = 0 + 16;
        Bmp[GuyAngeln2Links].Breite = 16;
        Bmp[GuyAngeln2Links].Hoehe = 16;

        Bmp[GuyAngeln2Oben].Anzahl = 4;
        Bmp[GuyAngeln2Oben].Geschwindigkeit = 3;
        Bmp[GuyAngeln2Oben].rcSrc.left = 227;
        Bmp[GuyAngeln2Oben].rcSrc.right = 227 + 16;
        Bmp[GuyAngeln2Oben].rcSrc.top = 0;
        Bmp[GuyAngeln2Oben].rcSrc.bottom = 0 + 16;
        Bmp[GuyAngeln2Oben].Breite = 16;
        Bmp[GuyAngeln2Oben].Hoehe = 16;

        Bmp[GuyAngeln2Rechts].Anzahl = 4;
        Bmp[GuyAngeln2Rechts].Geschwindigkeit = 3;
        Bmp[GuyAngeln2Rechts].rcSrc.left = 243;
        Bmp[GuyAngeln2Rechts].rcSrc.right = 243 + 14;
        Bmp[GuyAngeln2Rechts].rcSrc.top = 0;
        Bmp[GuyAngeln2Rechts].rcSrc.bottom = 0 + 15;
        Bmp[GuyAngeln2Rechts].Breite = 14;
        Bmp[GuyAngeln2Rechts].Hoehe = 15;

        Bmp[GuyAngeln2Unten].Anzahl = 4;
        Bmp[GuyAngeln2Unten].Geschwindigkeit = 3;
        Bmp[GuyAngeln2Unten].rcSrc.left = 257;
        Bmp[GuyAngeln2Unten].rcSrc.right = 257 + 14;
        Bmp[GuyAngeln2Unten].rcSrc.top = 0;
        Bmp[GuyAngeln2Unten].rcSrc.bottom = 0 + 15;
        Bmp[GuyAngeln2Unten].Breite = 14;
        Bmp[GuyAngeln2Unten].Hoehe = 15;

        Bmp[GuyAngeln3Links].Anzahl = 3;
        Bmp[GuyAngeln3Links].Geschwindigkeit = 2;
        Bmp[GuyAngeln3Links].rcSrc.left = 271;
        Bmp[GuyAngeln3Links].rcSrc.right = 271 + 16;
        Bmp[GuyAngeln3Links].rcSrc.top = 0;
        Bmp[GuyAngeln3Links].rcSrc.bottom = 0 + 16;
        Bmp[GuyAngeln3Links].Breite = 16;
        Bmp[GuyAngeln3Links].Hoehe = 16;

        Bmp[GuyAngeln3Oben].Anzahl = 3;
        Bmp[GuyAngeln3Oben].Geschwindigkeit = 2;
        Bmp[GuyAngeln3Oben].rcSrc.left = 285;
        Bmp[GuyAngeln3Oben].rcSrc.right = 285 + 16;
        Bmp[GuyAngeln3Oben].rcSrc.top = 0;
        Bmp[GuyAngeln3Oben].rcSrc.bottom = 0 + 16;
        Bmp[GuyAngeln3Oben].Breite = 16;
        Bmp[GuyAngeln3Oben].Hoehe = 16;

        Bmp[GuyAngeln3Rechts].Anzahl = 3;
        Bmp[GuyAngeln3Rechts].Geschwindigkeit = 2;
        Bmp[GuyAngeln3Rechts].rcSrc.left = 299;
        Bmp[GuyAngeln3Rechts].rcSrc.right = 299 + 14;
        Bmp[GuyAngeln3Rechts].rcSrc.top = 0;
        Bmp[GuyAngeln3Rechts].rcSrc.bottom = 0 + 15;
        Bmp[GuyAngeln3Rechts].Breite = 14;
        Bmp[GuyAngeln3Rechts].Hoehe = 15;

        Bmp[GuyAngeln3Unten].Anzahl = 3;
        Bmp[GuyAngeln3Unten].Geschwindigkeit = 2;
        Bmp[GuyAngeln3Unten].rcSrc.left = 313;
        Bmp[GuyAngeln3Unten].rcSrc.right = 313 + 14;
        Bmp[GuyAngeln3Unten].rcSrc.top = 0;
        Bmp[GuyAngeln3Unten].rcSrc.bottom = 0 + 15;
        Bmp[GuyAngeln3Unten].Breite = 14;
        Bmp[GuyAngeln3Unten].Hoehe = 15;

        Bmp[GuySchlagen].Anzahl = 4;
        Bmp[GuySchlagen].Geschwindigkeit = 7;
        Bmp[GuySchlagen].rcSrc.left = 327;
        Bmp[GuySchlagen].rcSrc.right = 327 + 12;
        Bmp[GuySchlagen].rcSrc.top = 0;
        Bmp[GuySchlagen].rcSrc.bottom = 0 + 24;
        Bmp[GuySchlagen].Breite = 12;
        Bmp[GuySchlagen].Hoehe = 24;
        Bmp[GuySchlagen].Sound = Sound::Hit;

        Bmp[GuyBootLinks].Anzahl = 6;
        Bmp[GuyBootLinks].Geschwindigkeit = 10;
        Bmp[GuyBootLinks].rcSrc.left = 339;
        Bmp[GuyBootLinks].rcSrc.right = 339 + 26;
        Bmp[GuyBootLinks].rcSrc.top = 0;
        Bmp[GuyBootLinks].rcSrc.bottom = 0 + 21;
        Bmp[GuyBootLinks].Breite = 26;
        Bmp[GuyBootLinks].Hoehe = 21;

        Bmp[GuyBootOben].Anzahl = 6;
        Bmp[GuyBootOben].Geschwindigkeit = 10;
        Bmp[GuyBootOben].rcSrc.left = 365;
        Bmp[GuyBootOben].rcSrc.right = 365 + 26;
        Bmp[GuyBootOben].rcSrc.top = 0;
        Bmp[GuyBootOben].rcSrc.bottom = 0 + 21;
        Bmp[GuyBootOben].Breite = 26;
        Bmp[GuyBootOben].Hoehe = 21;

        Bmp[GuyBootRechts].Anzahl = 6;
        Bmp[GuyBootRechts].Geschwindigkeit = 10;
        Bmp[GuyBootRechts].rcSrc.left = 391;
        Bmp[GuyBootRechts].rcSrc.right = 391 + 26;
        Bmp[GuyBootRechts].rcSrc.top = 0;
        Bmp[GuyBootRechts].rcSrc.bottom = 0 + 21;
        Bmp[GuyBootRechts].Breite = 26;
        Bmp[GuyBootRechts].Hoehe = 21;

        Bmp[GuyBootUnten].Anzahl = 6;
        Bmp[GuyBootUnten].Geschwindigkeit = 10;
        Bmp[GuyBootUnten].rcSrc.left = 417;
        Bmp[GuyBootUnten].rcSrc.right = 417 + 26;
        Bmp[GuyBootUnten].rcSrc.top = 0;
        Bmp[GuyBootUnten].rcSrc.bottom = 0 + 21;
        Bmp[GuyBootUnten].Breite = 26;
        Bmp[GuyBootUnten].Hoehe = 21;

        Bmp[GuyBootAngeln1].Anzahl = 6;
        Bmp[GuyBootAngeln1].Geschwindigkeit = 6;
        Bmp[GuyBootAngeln1].rcSrc.left = 443;
        Bmp[GuyBootAngeln1].rcSrc.right = 443 + 26;
        Bmp[GuyBootAngeln1].rcSrc.top = 0;
        Bmp[GuyBootAngeln1].rcSrc.bottom = 0 + 25;
        Bmp[GuyBootAngeln1].Breite = 26;
        Bmp[GuyBootAngeln1].Hoehe = 25;

        Bmp[GuyBootAngeln2].Anzahl = 4;
        Bmp[GuyBootAngeln2].Geschwindigkeit = 3;
        Bmp[GuyBootAngeln2].rcSrc.left = 469;
        Bmp[GuyBootAngeln2].rcSrc.right = 469 + 26;
        Bmp[GuyBootAngeln2].rcSrc.top = 0;
        Bmp[GuyBootAngeln2].rcSrc.bottom = 0 + 25;
        Bmp[GuyBootAngeln2].Breite = 26;
        Bmp[GuyBootAngeln2].Hoehe = 25;

        Bmp[GuyBootAngeln3].Anzahl = 3;
        Bmp[GuyBootAngeln3].Geschwindigkeit = 2;
        Bmp[GuyBootAngeln3].rcSrc.left = 495;
        Bmp[GuyBootAngeln3].rcSrc.right = 495 + 26;
        Bmp[GuyBootAngeln3].rcSrc.top = 0;
        Bmp[GuyBootAngeln3].rcSrc.bottom = 0 + 25;
        Bmp[GuyBootAngeln3].Breite = 26;
        Bmp[GuyBootAngeln3].Hoehe = 25;

        Bmp[GuyTauchen1].Anzahl = 5;
        Bmp[GuyTauchen1].Geschwindigkeit = 5;
        Bmp[GuyTauchen1].rcSrc.left = 521;
        Bmp[GuyTauchen1].rcSrc.right = 521 + 26;
        Bmp[GuyTauchen1].rcSrc.top = 0;
        Bmp[GuyTauchen1].rcSrc.bottom = 0 + 27;
        Bmp[GuyTauchen1].Breite = 26;
        Bmp[GuyTauchen1].Hoehe = 27;

        Bmp[GuyTauchen2].Anzahl = 4;
        Bmp[GuyTauchen2].Geschwindigkeit = 3;
        Bmp[GuyTauchen2].rcSrc.left = 547;
        Bmp[GuyTauchen2].rcSrc.right = 547 + 26;
        Bmp[GuyTauchen2].rcSrc.top = 0;
        Bmp[GuyTauchen2].rcSrc.bottom = 0 + 17;
        Bmp[GuyTauchen2].Breite = 26;
        Bmp[GuyTauchen2].Hoehe = 17;

        Bmp[GuyTauchen3].Anzahl = 2;
        Bmp[GuyTauchen3].Geschwindigkeit = 2;
        Bmp[GuyTauchen3].rcSrc.left = 573;
        Bmp[GuyTauchen3].rcSrc.right = 573 + 26;
        Bmp[GuyTauchen3].rcSrc.top = 0;
        Bmp[GuyTauchen3].rcSrc.bottom = 0 + 17;
        Bmp[GuyTauchen3].Breite = 26;
        Bmp[GuyTauchen3].Hoehe = 17;

        Bmp[GuyHammer].Anzahl = 2;
        Bmp[GuyHammer].Geschwindigkeit = 4;
        Bmp[GuyHammer].rcSrc.left = 599;
        Bmp[GuyHammer].rcSrc.right = 599 + 9;
        Bmp[GuyHammer].rcSrc.top = 0;
        Bmp[GuyHammer].rcSrc.bottom = 0 + 18;
        Bmp[GuyHammer].Breite = 9;
        Bmp[GuyHammer].Hoehe = 18;
        Bmp[GuyHammer].Sound = Sound::Hammer;

        Bmp[GuyKlettern1].Anzahl = 6;
        Bmp[GuyKlettern1].Geschwindigkeit = 2;
        Bmp[GuyKlettern1].rcSrc.left = 608;
        Bmp[GuyKlettern1].rcSrc.right = 608 + 7;
        Bmp[GuyKlettern1].rcSrc.top = 0;
        Bmp[GuyKlettern1].rcSrc.bottom = 0 + 34;
        Bmp[GuyKlettern1].Breite = 7;
        Bmp[GuyKlettern1].Hoehe = 34;

        Bmp[GuyKlettern2].Anzahl = 6;
        Bmp[GuyKlettern2].Geschwindigkeit = 2;
        Bmp[GuyKlettern2].rcSrc.left = 615;
        Bmp[GuyKlettern2].rcSrc.right = 615 + 7;
        Bmp[GuyKlettern2].rcSrc.top = 0;
        Bmp[GuyKlettern2].rcSrc.bottom = 0 + 34;
        Bmp[GuyKlettern2].Breite = 7;
        Bmp[GuyKlettern2].Hoehe = 34;

        Bmp[GuyHammer2].Anzahl = 2;
        Bmp[GuyHammer2].Geschwindigkeit = 4;
        Bmp[GuyHammer2].rcSrc.left = 622;
        Bmp[GuyHammer2].rcSrc.right = 622 + 7;
        Bmp[GuyHammer2].rcSrc.top = 0;
        Bmp[GuyHammer2].rcSrc.bottom = 0 + 34;
        Bmp[GuyHammer2].Breite = 7;
        Bmp[GuyHammer2].Hoehe = 34;
        Bmp[GuyHammer2].Sound = Sound::Hammer;

        Bmp[GuyGehInHaus].Anzahl = 3;
        Bmp[GuyGehInHaus].Geschwindigkeit = 2;
        Bmp[GuyGehInHaus].rcSrc.left = 631;
        Bmp[GuyGehInHaus].rcSrc.right = 631 + 9;
        Bmp[GuyGehInHaus].rcSrc.top = 0;
        Bmp[GuyGehInHaus].rcSrc.bottom = 0 + 34;
        Bmp[GuyGehInHaus].Breite = 9;
        Bmp[GuyGehInHaus].Hoehe = 34;

        Bmp[GuySchlafHaus].Anzahl = 2;
        Bmp[GuySchlafHaus].Geschwindigkeit = 1;
        Bmp[GuySchlafHaus].rcSrc.left = 640;
        Bmp[GuySchlafHaus].rcSrc.right = 640 + 10;
        Bmp[GuySchlafHaus].rcSrc.top = 0;
        Bmp[GuySchlafHaus].rcSrc.bottom = 0 + 34;
        Bmp[GuySchlafHaus].Breite = 10;
        Bmp[GuySchlafHaus].Hoehe = 34;
        Bmp[GuySchlafHaus].Sound = Sound::Snore;

        Bmp[GuyGehausHaus].Anzahl = 3;
        Bmp[GuyGehausHaus].Geschwindigkeit = 2;
        Bmp[GuyGehausHaus].rcSrc.left = 650;
        Bmp[GuyGehausHaus].rcSrc.right = 650 + 9;
        Bmp[GuyGehausHaus].rcSrc.top = 0;
        Bmp[GuyGehausHaus].rcSrc.bottom = 0 + 34;
        Bmp[GuyGehausHaus].Breite = 9;
        Bmp[GuyGehausHaus].Hoehe = 34;

        Bmp[GuyAnzuenden].Anzahl = 6;
        Bmp[GuyAnzuenden].Geschwindigkeit = 5;
        Bmp[GuyAnzuenden].rcSrc.left = 659;
        Bmp[GuyAnzuenden].rcSrc.right = 659 + 19;
        Bmp[GuyAnzuenden].rcSrc.top = 0;
        Bmp[GuyAnzuenden].rcSrc.bottom = 0 + 18;
        Bmp[GuyAnzuenden].Breite = 19;
        Bmp[GuyAnzuenden].Hoehe = 18;

        Bmp[GuyAusschau].Anzahl = 4;
        Bmp[GuyAusschau].Geschwindigkeit = 1;
        Bmp[GuyAusschau].rcSrc.left = 678;
        Bmp[GuyAusschau].rcSrc.right = 678 + 10;
        Bmp[GuyAusschau].rcSrc.top = 0;
        Bmp[GuyAusschau].rcSrc.bottom = 0 + 18;
        Bmp[GuyAusschau].Breite = 10;
        Bmp[GuyAusschau].Hoehe = 18;

        Bmp[GuySchaufeln].Anzahl = 10;
        Bmp[GuySchaufeln].Geschwindigkeit = 3;
        Bmp[GuySchaufeln].rcSrc.left = 688;
        Bmp[GuySchaufeln].rcSrc.right = 688 + 17;
        Bmp[GuySchaufeln].rcSrc.top = 0;
        Bmp[GuySchaufeln].rcSrc.bottom = 0 + 19;
        Bmp[GuySchaufeln].Breite = 17;
        Bmp[GuySchaufeln].Hoehe = 19;
        Bmp[GuySchaufeln].Sound = Sound::Dig;

        Bmp[GuySchiff].Anzahl = 4;
        Bmp[GuySchiff].Geschwindigkeit = 10;
        Bmp[GuySchiff].rcSrc.left = 297;
        Bmp[GuySchiff].rcSrc.right = 297 + 48;
        Bmp[GuySchiff].rcSrc.top = 0;
        Bmp[GuySchiff].rcSrc.bottom = 0 + 38;
        Bmp[GuySchiff].Breite = 48;
        Bmp[GuySchiff].Hoehe = 38;
        Bmp[GuySchiff].Surface = lpDDSBau;
        Bmp[GuySchiff].Sound = Sound::Storm;

        Bmp[GuySchiffDown].Anzahl = 6;
        Bmp[GuySchiffDown].Geschwindigkeit = 3;
        Bmp[GuySchiffDown].rcSrc.left = 345;
        Bmp[GuySchiffDown].rcSrc.right = 345 + 46;
        Bmp[GuySchiffDown].rcSrc.top = 0;
        Bmp[GuySchiffDown].rcSrc.bottom = 0 + 40;
        Bmp[GuySchiffDown].Breite = 46;
        Bmp[GuySchiffDown].Hoehe = 40;
        Bmp[GuySchiffDown].Surface = lpDDSBau;
        Bmp[GuySchiffDown].Sound = Sound::Splat;

        Bmp[GuySchwimmen].Anzahl = 4;
        Bmp[GuySchwimmen].Geschwindigkeit = 10;
        Bmp[GuySchwimmen].rcSrc.left = 705;
        Bmp[GuySchwimmen].rcSrc.right = 705 + 15;
        Bmp[GuySchwimmen].rcSrc.top = 0;
        Bmp[GuySchwimmen].rcSrc.bottom = 0 + 9;
        Bmp[GuySchwimmen].Breite = 15;
        Bmp[GuySchwimmen].Hoehe = 9;
        Bmp[GuySchwimmen].Sound = Sound::Swim;

        Bmp[GuyTod].Anzahl = 6;
        Bmp[GuyTod].Geschwindigkeit = 1;
        Bmp[GuyTod].rcSrc.left = 743;
        Bmp[GuyTod].rcSrc.right = 743 + 16;
        Bmp[GuyTod].rcSrc.top = 0;
        Bmp[GuyTod].rcSrc.bottom = 0 + 10;
        Bmp[GuyTod].Breite = 16;
        Bmp[GuyTod].Hoehe = 10;

        Bmp[GuyBootTod].Anzahl = 6;
        Bmp[GuyBootTod].Geschwindigkeit = 1;
        Bmp[GuyBootTod].rcSrc.left = 759;
        Bmp[GuyBootTod].rcSrc.right = 759 + 26;
        Bmp[GuyBootTod].rcSrc.top = 0;
        Bmp[GuyBootTod].rcSrc.bottom = 0 + 18;
        Bmp[GuyBootTod].Breite = 26;
        Bmp[GuyBootTod].Hoehe = 18;

        Bmp[GuyBootWarten].Anzahl = 4;
        Bmp[GuyBootWarten].Geschwindigkeit = 2;
        Bmp[GuyBootWarten].rcSrc.left = 0;
        Bmp[GuyBootWarten].rcSrc.right = 0 + 26;
        Bmp[GuyBootWarten].rcSrc.top = 72;
        Bmp[GuyBootWarten].rcSrc.bottom = 72 + 18;
        Bmp[GuyBootWarten].Breite = 26;
        Bmp[GuyBootWarten].Hoehe = 20;

        Bmp[GuySchleuder].Anzahl = 5;
        Bmp[GuySchleuder].Geschwindigkeit = 4;
        Bmp[GuySchleuder].rcSrc.left = 720;
        Bmp[GuySchleuder].rcSrc.right = 720 + 23;
        Bmp[GuySchleuder].rcSrc.top = 0;
        Bmp[GuySchleuder].rcSrc.bottom = 0 + 20;
        Bmp[GuySchleuder].Breite = 23;
        Bmp[GuySchleuder].Hoehe = 20;

        // Cursor
        for (i = CuPfeil; i <= CuUhr; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Anzahl = 1;
            Bmp[i].Geschwindigkeit = 0;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSCursor;
            Bmp[i].rcSrc.left = (i - CuPfeil) * 18;
            Bmp[i].rcSrc.top = 0;
            Bmp[i].rcSrc.right = Bmp[i].rcSrc.left + 18;
            Bmp[i].rcSrc.bottom = 18;
            Bmp[i].Breite = 18;
            Bmp[i].Hoehe = 18;
        }

        // Landschaftsanimationen
        for (i = Meerwellen; i <= Schleuse6; i++)
        {
            Bmp[i].Animation = true;
            Bmp[i].Anzahl = 7;
            Bmp[i].Geschwindigkeit = 7;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSAnimation;
            Bmp[i].Sound = Sound::River;
        }

        Bmp[Meerwellen].Geschwindigkeit = 3;
        Bmp[Meerwellen].Breite = 28;
        Bmp[Meerwellen].Hoehe = 13;
        Bmp[Meerwellen].rcSrc.left = 0;
        Bmp[Meerwellen].rcSrc.right = Bmp[Meerwellen].rcSrc.left + Bmp[Meerwellen].Breite;
        Bmp[Meerwellen].rcSrc.top = 0;
        Bmp[Meerwellen].rcSrc.bottom = Bmp[Meerwellen].rcSrc.top + Bmp[Meerwellen].Hoehe;
        Bmp[Meerwellen].rcDes.left = 9;
        Bmp[Meerwellen].rcDes.right = Bmp[Meerwellen].rcDes.left + Bmp[Meerwellen].Breite;
        Bmp[Meerwellen].rcDes.top = 23;
        Bmp[Meerwellen].rcDes.bottom = Bmp[Meerwellen].rcDes.top + Bmp[Meerwellen].Hoehe;
        Bmp[Meerwellen].Sound = Sound::Surf;

        Bmp[Fluss1].Breite = 34;
        Bmp[Fluss1].Hoehe = 8;
        Bmp[Fluss1].rcSrc.left = 28;
        Bmp[Fluss1].rcSrc.right = Bmp[Fluss1].rcSrc.left + Bmp[Fluss1].Breite;
        Bmp[Fluss1].rcSrc.top = 0;
        Bmp[Fluss1].rcSrc.bottom = Bmp[Fluss1].rcSrc.top + Bmp[Fluss1].Hoehe;
        Bmp[Fluss1].rcDes.left = 10;
        Bmp[Fluss1].rcDes.right = Bmp[Fluss1].rcDes.left + Bmp[Fluss1].Breite;
        Bmp[Fluss1].rcDes.top = 18;
        Bmp[Fluss1].rcDes.bottom = Bmp[Fluss1].rcDes.top + Bmp[Fluss1].Hoehe;

        Bmp[Fluss2].Breite = 34;
        Bmp[Fluss2].Hoehe = 8;
        Bmp[Fluss2].rcSrc.left = 62;
        Bmp[Fluss2].rcSrc.right = Bmp[Fluss2].rcSrc.left + Bmp[Fluss2].Breite;
        Bmp[Fluss2].rcSrc.top = 0;
        Bmp[Fluss2].rcSrc.bottom = Bmp[Fluss2].rcSrc.top + Bmp[Fluss2].Hoehe;
        Bmp[Fluss2].rcDes.left = 10;
        Bmp[Fluss2].rcDes.right = Bmp[Fluss2].rcDes.left + Bmp[Fluss2].Breite;
        Bmp[Fluss2].rcDes.top = 19;
        Bmp[Fluss2].rcDes.bottom = Bmp[Fluss2].rcDes.top + Bmp[Fluss2].Hoehe;

        Bmp[Fluss3].Breite = 34;
        Bmp[Fluss3].Hoehe = 34;
        Bmp[Fluss3].rcSrc.left = 96;
        Bmp[Fluss3].rcSrc.right = Bmp[Fluss3].rcSrc.left + Bmp[Fluss3].Breite;
        Bmp[Fluss3].rcSrc.top = 0;
        Bmp[Fluss3].rcSrc.bottom = Bmp[Fluss3].rcSrc.top + Bmp[Fluss3].Hoehe;
        Bmp[Fluss3].rcDes.left = 10;
        Bmp[Fluss3].rcDes.right = Bmp[Fluss3].rcDes.left + Bmp[Fluss3].Breite;
        Bmp[Fluss3].rcDes.top = 5;
        Bmp[Fluss3].rcDes.bottom = Bmp[Fluss3].rcDes.top + Bmp[Fluss3].Hoehe;

        Bmp[Fluss4].Breite = 34;
        Bmp[Fluss4].Hoehe = 34;
        Bmp[Fluss4].rcSrc.left = 130;
        Bmp[Fluss4].rcSrc.right = Bmp[Fluss4].rcSrc.left + Bmp[Fluss4].Breite;
        Bmp[Fluss4].rcSrc.top = 0;
        Bmp[Fluss4].rcSrc.bottom = Bmp[Fluss4].rcSrc.top + Bmp[Fluss4].Hoehe;
        Bmp[Fluss4].rcDes.left = 10;
        Bmp[Fluss4].rcDes.right = Bmp[Fluss4].rcDes.left + Bmp[Fluss4].Breite;
        Bmp[Fluss4].rcDes.top = 5;
        Bmp[Fluss4].rcDes.bottom = Bmp[Fluss4].rcDes.top + Bmp[Fluss4].Hoehe;

        Bmp[Fluss5].Breite = 34;
        Bmp[Fluss5].Hoehe = 18;
        Bmp[Fluss5].rcSrc.left = 164;
        Bmp[Fluss5].rcSrc.right = Bmp[Fluss5].rcSrc.left + Bmp[Fluss5].Breite;
        Bmp[Fluss5].rcSrc.top = 0;
        Bmp[Fluss5].rcSrc.bottom = Bmp[Fluss5].rcSrc.top + Bmp[Fluss5].Hoehe;
        Bmp[Fluss5].rcDes.left = 10;
        Bmp[Fluss5].rcDes.right = Bmp[Fluss5].rcDes.left + Bmp[Fluss5].Breite;
        Bmp[Fluss5].rcDes.top = 21;
        Bmp[Fluss5].rcDes.bottom = Bmp[Fluss5].rcDes.top + Bmp[Fluss5].Hoehe;

        Bmp[Fluss6].Breite = 34;
        Bmp[Fluss6].Hoehe = 18;
        Bmp[Fluss6].rcSrc.left = 198;
        Bmp[Fluss6].rcSrc.right = Bmp[Fluss6].rcSrc.left + Bmp[Fluss6].Breite;
        Bmp[Fluss6].rcSrc.top = 0;
        Bmp[Fluss6].rcSrc.bottom = Bmp[Fluss6].rcSrc.top + Bmp[Fluss6].Hoehe;
        Bmp[Fluss6].rcDes.left = 10;
        Bmp[Fluss6].rcDes.right = Bmp[Fluss6].rcDes.left + Bmp[Fluss6].Breite;
        Bmp[Fluss6].rcDes.top = 21;
        Bmp[Fluss6].rcDes.bottom = Bmp[Fluss6].rcDes.top + Bmp[Fluss6].Hoehe;

        Bmp[Fluss7].Breite = 18;
        Bmp[Fluss7].Hoehe = 18;
        Bmp[Fluss7].rcSrc.left = 232;
        Bmp[Fluss7].rcSrc.right = Bmp[Fluss7].rcSrc.left + Bmp[Fluss7].Breite;
        Bmp[Fluss7].rcSrc.top = 0;
        Bmp[Fluss7].rcSrc.bottom = Bmp[Fluss7].rcSrc.top + Bmp[Fluss7].Hoehe;
        Bmp[Fluss7].rcDes.left = 10;
        Bmp[Fluss7].rcDes.right = Bmp[Fluss7].rcDes.left + Bmp[Fluss7].Breite;
        Bmp[Fluss7].rcDes.top = 21;
        Bmp[Fluss7].rcDes.bottom = Bmp[Fluss7].rcDes.top + Bmp[Fluss7].Hoehe;

        Bmp[Fluss8].Breite = 18;
        Bmp[Fluss8].Hoehe = 18;
        Bmp[Fluss8].rcSrc.left = 250;
        Bmp[Fluss8].rcSrc.right = Bmp[Fluss8].rcSrc.left + Bmp[Fluss8].Breite;
        Bmp[Fluss8].rcSrc.top = 0;
        Bmp[Fluss8].rcSrc.bottom = Bmp[Fluss8].rcSrc.top + Bmp[Fluss8].Hoehe;
        Bmp[Fluss8].rcDes.left = 26;
        Bmp[Fluss8].rcDes.right = Bmp[Fluss8].rcDes.left + Bmp[Fluss8].Breite;
        Bmp[Fluss8].rcDes.top = 21;
        Bmp[Fluss8].rcDes.bottom = Bmp[Fluss8].rcDes.top + Bmp[Fluss8].Hoehe;

        Bmp[Fluss9].Breite = 34;
        Bmp[Fluss9].Hoehe = 15;
        Bmp[Fluss9].rcSrc.left = 268;
        Bmp[Fluss9].rcSrc.right = Bmp[Fluss9].rcSrc.left + Bmp[Fluss9].Breite;
        Bmp[Fluss9].rcSrc.top = 0;
        Bmp[Fluss9].rcSrc.bottom = Bmp[Fluss9].rcSrc.top + Bmp[Fluss9].Hoehe;
        Bmp[Fluss9].rcDes.left = 10;
        Bmp[Fluss9].rcDes.right = Bmp[Fluss9].rcDes.left + Bmp[Fluss9].Breite;
        Bmp[Fluss9].rcDes.top = 20;
        Bmp[Fluss9].rcDes.bottom = Bmp[Fluss9].rcDes.top + Bmp[Fluss9].Hoehe;

        Bmp[Fluss10].Breite = 34;
        Bmp[Fluss10].Hoehe = 13;
        Bmp[Fluss10].rcSrc.left = 302;
        Bmp[Fluss10].rcSrc.right = Bmp[Fluss10].rcSrc.left + Bmp[Fluss10].Breite;
        Bmp[Fluss10].rcSrc.top = 0;
        Bmp[Fluss10].rcSrc.bottom = Bmp[Fluss10].rcSrc.top + Bmp[Fluss10].Hoehe;
        Bmp[Fluss10].rcDes.left = 10;
        Bmp[Fluss10].rcDes.right = Bmp[Fluss10].rcDes.left + Bmp[Fluss10].Breite;
        Bmp[Fluss10].rcDes.top = 26;
        Bmp[Fluss10].rcDes.bottom = Bmp[Fluss10].rcDes.top + Bmp[Fluss10].Hoehe;

        Bmp[Muendung1].Breite = 42;
        Bmp[Muendung1].Hoehe = 22;
        Bmp[Muendung1].rcSrc.left = 336;
        Bmp[Muendung1].rcSrc.right = Bmp[Muendung1].rcSrc.left + Bmp[Muendung1].Breite;
        Bmp[Muendung1].rcSrc.top = 0;
        Bmp[Muendung1].rcSrc.bottom = Bmp[Muendung1].rcSrc.top + Bmp[Muendung1].Hoehe;
        Bmp[Muendung1].rcDes.left = 2;
        Bmp[Muendung1].rcDes.right = Bmp[Muendung1].rcDes.left + Bmp[Muendung1].Breite;
        Bmp[Muendung1].rcDes.top = 17;
        Bmp[Muendung1].rcDes.bottom = Bmp[Muendung1].rcDes.top + Bmp[Muendung1].Hoehe;

        Bmp[Muendung2].Breite = 40;
        Bmp[Muendung2].Hoehe = 22;
        Bmp[Muendung2].rcSrc.left = 378;
        Bmp[Muendung2].rcSrc.right = Bmp[Muendung2].rcSrc.left + Bmp[Muendung2].Breite;
        Bmp[Muendung2].rcSrc.top = 0;
        Bmp[Muendung2].rcSrc.bottom = Bmp[Muendung2].rcSrc.top + Bmp[Muendung2].Hoehe;
        Bmp[Muendung2].rcDes.left = 10;
        Bmp[Muendung2].rcDes.right = Bmp[Muendung2].rcDes.left + Bmp[Muendung2].Breite;
        Bmp[Muendung2].rcDes.top = 17;
        Bmp[Muendung2].rcDes.bottom = Bmp[Muendung2].rcDes.top + Bmp[Muendung2].Hoehe;

        Bmp[Muendung3].Breite = 40;
        Bmp[Muendung3].Hoehe = 22;
        Bmp[Muendung3].rcSrc.left = 418;
        Bmp[Muendung3].rcSrc.right = Bmp[Muendung3].rcSrc.left + Bmp[Muendung3].Breite;
        Bmp[Muendung3].rcSrc.top = 0;
        Bmp[Muendung3].rcSrc.bottom = Bmp[Muendung3].rcSrc.top + Bmp[Muendung3].Hoehe;
        Bmp[Muendung3].rcDes.left = 10;
        Bmp[Muendung3].rcDes.right = Bmp[Muendung3].rcDes.left + Bmp[Muendung3].Breite;
        Bmp[Muendung3].rcDes.top = 21;
        Bmp[Muendung3].rcDes.bottom = Bmp[Muendung3].rcDes.top + Bmp[Muendung3].Hoehe;

        Bmp[Muendung4].Breite = 42;
        Bmp[Muendung4].Hoehe = 22;
        Bmp[Muendung4].rcSrc.left = 458;
        Bmp[Muendung4].rcSrc.right = Bmp[Muendung4].rcSrc.left + Bmp[Muendung4].Breite;
        Bmp[Muendung4].rcSrc.top = 0;
        Bmp[Muendung4].rcSrc.bottom = Bmp[Muendung4].rcSrc.top + Bmp[Muendung4].Hoehe;
        Bmp[Muendung4].rcDes.left = 2;
        Bmp[Muendung4].rcDes.right = Bmp[Muendung4].rcDes.left + Bmp[Muendung4].Breite;
        Bmp[Muendung4].rcDes.top = 21;
        Bmp[Muendung4].rcDes.bottom = Bmp[Muendung4].rcDes.top + Bmp[Muendung4].Hoehe;

        Bmp[Quelle1].Breite = 30;
        Bmp[Quelle1].Hoehe = 19;
        Bmp[Quelle1].rcSrc.left = 500;
        Bmp[Quelle1].rcSrc.right = Bmp[Quelle1].rcSrc.left + Bmp[Quelle1].Breite;
        Bmp[Quelle1].rcSrc.top = 0;
        Bmp[Quelle1].rcSrc.bottom = Bmp[Quelle1].rcSrc.top + Bmp[Quelle1].Hoehe;
        Bmp[Quelle1].rcDes.left = 16;
        Bmp[Quelle1].rcDes.right = Bmp[Quelle1].rcDes.left + Bmp[Quelle1].Breite;
        Bmp[Quelle1].rcDes.top = 19;
        Bmp[Quelle1].rcDes.bottom = Bmp[Quelle1].rcDes.top + Bmp[Quelle1].Hoehe;

        Bmp[Quelle2].Breite = 26;
        Bmp[Quelle2].Hoehe = 19;
        Bmp[Quelle2].rcSrc.left = 530;
        Bmp[Quelle2].rcSrc.right = Bmp[Quelle2].rcSrc.left + Bmp[Quelle2].Breite;
        Bmp[Quelle2].rcSrc.top = 0;
        Bmp[Quelle2].rcSrc.bottom = Bmp[Quelle2].rcSrc.top + Bmp[Quelle2].Hoehe;
        Bmp[Quelle2].rcDes.left = 9;
        Bmp[Quelle2].rcDes.right = Bmp[Quelle2].rcDes.left + Bmp[Quelle2].Breite;
        Bmp[Quelle2].rcDes.top = 20;
        Bmp[Quelle2].rcDes.bottom = Bmp[Quelle2].rcDes.top + Bmp[Quelle2].Hoehe;

        Bmp[Quelle3].Breite = 25;
        Bmp[Quelle3].Hoehe = 16;
        Bmp[Quelle3].rcSrc.left = 556;
        Bmp[Quelle3].rcSrc.right = Bmp[Quelle3].rcSrc.left + Bmp[Quelle3].Breite;
        Bmp[Quelle3].rcSrc.top = 0;
        Bmp[Quelle3].rcSrc.bottom = Bmp[Quelle3].rcSrc.top + Bmp[Quelle3].Hoehe;
        Bmp[Quelle3].rcDes.left = 19;
        Bmp[Quelle3].rcDes.right = Bmp[Quelle3].rcDes.left + Bmp[Quelle3].Breite;
        Bmp[Quelle3].rcDes.top = 19;
        Bmp[Quelle3].rcDes.bottom = Bmp[Quelle3].rcDes.top + Bmp[Quelle3].Hoehe;

        Bmp[Quelle4].Breite = 25;
        Bmp[Quelle4].Hoehe = 15;
        Bmp[Quelle4].rcSrc.left = 581;
        Bmp[Quelle4].rcSrc.right = Bmp[Quelle4].rcSrc.left + Bmp[Quelle4].Breite;
        Bmp[Quelle4].rcSrc.top = 0;
        Bmp[Quelle4].rcSrc.bottom = Bmp[Quelle4].rcSrc.top + Bmp[Quelle4].Hoehe;
        Bmp[Quelle4].rcDes.left = 8;
        Bmp[Quelle4].rcDes.right = Bmp[Quelle4].rcDes.left + Bmp[Quelle4].Breite;
        Bmp[Quelle4].rcDes.top = 21;
        Bmp[Quelle4].rcDes.bottom = Bmp[Quelle4].rcDes.top + Bmp[Quelle4].Hoehe;

        Bmp[Schleuse1].Breite = 35;
        Bmp[Schleuse1].Hoehe = 22;
        Bmp[Schleuse1].rcSrc.left = 164;
        Bmp[Schleuse1].rcSrc.right = Bmp[Schleuse1].rcSrc.left + Bmp[Schleuse1].Breite;
        Bmp[Schleuse1].rcSrc.top = 126;
        Bmp[Schleuse1].rcSrc.bottom = Bmp[Schleuse1].rcSrc.top + Bmp[Schleuse1].Hoehe;
        Bmp[Schleuse1].rcDes.left = 10;
        Bmp[Schleuse1].rcDes.right = Bmp[Schleuse1].rcDes.left + Bmp[Schleuse1].Breite;
        Bmp[Schleuse1].rcDes.top = 17;
        Bmp[Schleuse1].rcDes.bottom = Bmp[Schleuse1].rcDes.top + Bmp[Schleuse1].Hoehe;

        Bmp[Schleuse2].Breite = 34;
        Bmp[Schleuse2].Hoehe = 23;
        Bmp[Schleuse2].rcSrc.left = 199;
        Bmp[Schleuse2].rcSrc.right = Bmp[Schleuse2].rcSrc.left + Bmp[Schleuse2].Breite;
        Bmp[Schleuse2].rcSrc.top = 126;
        Bmp[Schleuse2].rcSrc.bottom = Bmp[Schleuse2].rcSrc.top + Bmp[Schleuse2].Hoehe;
        Bmp[Schleuse2].rcDes.left = 10;
        Bmp[Schleuse2].rcDes.right = Bmp[Schleuse2].rcDes.left + Bmp[Schleuse2].Breite;
        Bmp[Schleuse2].rcDes.top = 16;
        Bmp[Schleuse2].rcDes.bottom = Bmp[Schleuse2].rcDes.top + Bmp[Schleuse2].Hoehe;

        Bmp[Schleuse3].Breite = 34;
        Bmp[Schleuse3].Hoehe = 22;
        Bmp[Schleuse3].rcSrc.left = 233;
        Bmp[Schleuse3].rcSrc.right = Bmp[Schleuse3].rcSrc.left + Bmp[Schleuse3].Breite;
        Bmp[Schleuse3].rcSrc.top = 126;
        Bmp[Schleuse3].rcSrc.bottom = Bmp[Schleuse3].rcSrc.top + Bmp[Schleuse3].Hoehe;
        Bmp[Schleuse3].rcDes.left = 10;
        Bmp[Schleuse3].rcDes.right = Bmp[Schleuse3].rcDes.left + Bmp[Schleuse3].Breite;
        Bmp[Schleuse3].rcDes.top = 17;
        Bmp[Schleuse3].rcDes.bottom = Bmp[Schleuse3].rcDes.top + Bmp[Schleuse3].Hoehe;

        Bmp[Schleuse4].Breite = 33;
        Bmp[Schleuse4].Hoehe = 23;
        Bmp[Schleuse4].rcSrc.left = 268;
        Bmp[Schleuse4].rcSrc.right = Bmp[Schleuse4].rcSrc.left + Bmp[Schleuse4].Breite;
        Bmp[Schleuse4].rcSrc.top = 105;
        Bmp[Schleuse4].rcSrc.bottom = Bmp[Schleuse4].rcSrc.top + Bmp[Schleuse4].Hoehe;
        Bmp[Schleuse4].rcDes.left = 11;
        Bmp[Schleuse4].rcDes.right = Bmp[Schleuse4].rcDes.left + Bmp[Schleuse4].Breite;
        Bmp[Schleuse4].rcDes.top = 16;
        Bmp[Schleuse4].rcDes.bottom = Bmp[Schleuse4].rcDes.top + Bmp[Schleuse4].Hoehe;

        Bmp[Schleuse5].Breite = 34;
        Bmp[Schleuse5].Hoehe = 17;
        Bmp[Schleuse5].rcSrc.left = 302;
        Bmp[Schleuse5].rcSrc.right = Bmp[Schleuse5].rcSrc.left + Bmp[Schleuse5].Breite;
        Bmp[Schleuse5].rcSrc.top = 91;
        Bmp[Schleuse5].rcSrc.bottom = Bmp[Schleuse5].rcSrc.top + Bmp[Schleuse5].Hoehe;
        Bmp[Schleuse5].rcDes.left = 10;
        Bmp[Schleuse5].rcDes.right = Bmp[Schleuse5].rcDes.left + Bmp[Schleuse5].Breite;
        Bmp[Schleuse5].rcDes.top = 20;
        Bmp[Schleuse5].rcDes.bottom = Bmp[Schleuse5].rcDes.top + Bmp[Schleuse5].Hoehe;

        Bmp[Schleuse6].Breite = 35;
        Bmp[Schleuse6].Hoehe = 23;
        Bmp[Schleuse6].rcSrc.left = 336;
        Bmp[Schleuse6].rcSrc.right = Bmp[Schleuse6].rcSrc.left + Bmp[Schleuse6].Breite;
        Bmp[Schleuse6].rcSrc.top = 154;
        Bmp[Schleuse6].rcSrc.bottom = Bmp[Schleuse6].rcSrc.top + Bmp[Schleuse6].Hoehe;
        Bmp[Schleuse6].rcDes.left = 10;
        Bmp[Schleuse6].rcDes.right = Bmp[Schleuse6].rcDes.left + Bmp[Schleuse6].Breite;
        Bmp[Schleuse6].rcDes.top = 16;
        Bmp[Schleuse6].rcDes.bottom = Bmp[Schleuse6].rcDes.top + Bmp[Schleuse6].Hoehe;

        // Bauwerke
        for (i = Feld; i <= Feuerstelle; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Geschwindigkeit = 0;
            Bmp[i].Phase = 0;
        }

        Bmp[Feld].Anzahl = 3;
        Bmp[Feld].Surface = lpDDSBau;
        Bmp[Feld].Breite = 42;
        Bmp[Feld].Hoehe = 27;
        Bmp[Feld].rcSrc.left = 0;
        Bmp[Feld].rcSrc.right = 0 + Bmp[Feld].Breite;
        Bmp[Feld].rcSrc.top = 0;
        Bmp[Feld].rcSrc.bottom = 0 + Bmp[Feld].Hoehe;
        Bmp[Feld].rcDes.left = 5;
        Bmp[Feld].rcDes.right = Bmp[Feld].rcDes.left + Bmp[Feld].Breite;
        Bmp[Feld].rcDes.top = 15;
        Bmp[Feld].rcDes.bottom = Bmp[Feld].rcDes.top + Bmp[Feld].Hoehe;
        Bmp[Feld].AkAnzahl = 20;

        Bmp[Zelt].Anzahl = 1;
        Bmp[Zelt].Surface = lpDDSBau;
        Bmp[Zelt].Breite = 23;
        Bmp[Zelt].Hoehe = 20;
        Bmp[Zelt].rcSrc.left = 42;
        Bmp[Zelt].rcSrc.right = 42 + Bmp[Zelt].Breite;
        Bmp[Zelt].rcSrc.top = 0;
        Bmp[Zelt].rcSrc.bottom = 0 + Bmp[Zelt].Hoehe;
        Bmp[Zelt].rcDes.left = 14;
        Bmp[Zelt].rcDes.right = Bmp[Zelt].rcDes.left + Bmp[Zelt].Breite;
        Bmp[Zelt].rcDes.top = 9;
        Bmp[Zelt].rcDes.bottom = Bmp[Zelt].rcDes.top + Bmp[Zelt].Hoehe;
        Bmp[Zelt].Rohstoff[ResourceBranch] = 5;
        Bmp[Zelt].Rohstoff[ResourceLeaf] = 5;
        Bmp[Zelt].AkAnzahl = 16;

        Bmp[Boot].Anzahl = 2;
        Bmp[Boot].Surface = lpDDSBau;
        Bmp[Boot].Breite = 26;
        Bmp[Boot].Hoehe = 18;
        Bmp[Boot].rcSrc.left = 65;
        Bmp[Boot].rcSrc.right = 65 + Bmp[Boot].Breite;
        Bmp[Boot].rcSrc.top = 0;
        Bmp[Boot].rcSrc.bottom = 0 + Bmp[Boot].Hoehe;
        Bmp[Boot].rcDes.left = 14;
        Bmp[Boot].rcDes.right = Bmp[Boot].rcDes.left + Bmp[Boot].Breite;
        Bmp[Boot].rcDes.top = 20;
        Bmp[Boot].rcDes.bottom = Bmp[Boot].rcDes.top + Bmp[Boot].Hoehe;
        Bmp[Boot].Rohstoff[ResourceBranch] = 2;
        Bmp[Boot].Rohstoff[ResourceTrunk] = 1;
        Bmp[Boot].AkAnzahl = 16;

        Bmp[Rohr].Anzahl = 2;
        Bmp[Rohr].Surface = lpDDSBau;
        Bmp[Rohr].Breite = 34;
        Bmp[Rohr].Hoehe = 21;
        Bmp[Rohr].rcSrc.left = 91;
        Bmp[Rohr].rcSrc.right = 91 + Bmp[Rohr].Breite;
        Bmp[Rohr].rcSrc.top = 0;
        Bmp[Rohr].rcSrc.bottom = 0 + Bmp[Rohr].Hoehe;
        Bmp[Rohr].rcDes.left = 11;
        Bmp[Rohr].rcDes.right = Bmp[Rohr].rcDes.left + Bmp[Rohr].Breite;
        Bmp[Rohr].rcDes.top = 16;
        Bmp[Rohr].rcDes.bottom = Bmp[Rohr].rcDes.top + Bmp[Rohr].Hoehe;
        Bmp[Rohr].Rohstoff[ResourceTrunk] = 1;
        Bmp[Rohr].AkAnzahl = 18;

        Bmp[Sos].Anzahl = 1;
        Bmp[Sos].Surface = lpDDSBau;
        Bmp[Sos].Breite = 36;
        Bmp[Sos].Hoehe = 19;
        Bmp[Sos].rcSrc.left = 125;
        Bmp[Sos].rcSrc.right = 125 + Bmp[Sos].Breite;
        Bmp[Sos].rcSrc.top = 0;
        Bmp[Sos].rcSrc.bottom = 0 + Bmp[Sos].Hoehe;
        Bmp[Sos].rcDes.left = 11;
        Bmp[Sos].rcDes.right = Bmp[Sos].rcDes.left + Bmp[Sos].Breite;
        Bmp[Sos].rcDes.top = 20;
        Bmp[Sos].rcDes.bottom = Bmp[Sos].rcDes.top + Bmp[Sos].Hoehe;
        Bmp[Sos].Rohstoff[ResourceStone] = 10;
        Bmp[Sos].AkAnzahl = 20;

        Bmp[Haus1].Anzahl = 1;
        Bmp[Haus1].Surface = lpDDSBau;
        Bmp[Haus1].Breite = 26;
        Bmp[Haus1].Hoehe = 41;
        Bmp[Haus1].rcSrc.left = 161;
        Bmp[Haus1].rcSrc.right = 161 + Bmp[Haus1].Breite;
        Bmp[Haus1].rcSrc.top = 0;
        Bmp[Haus1].rcSrc.bottom = 0 + Bmp[Haus1].Hoehe;
        Bmp[Haus1].rcDes.left = 0;
        Bmp[Haus1].rcDes.right = Bmp[Haus1].rcDes.left + Bmp[Haus1].Breite;
        Bmp[Haus1].rcDes.top = 0;
        Bmp[Haus1].rcDes.bottom = Bmp[Haus1].rcDes.top + Bmp[Haus1].Hoehe;
        Bmp[Haus1].Rohstoff[ResourceBranch] = 4;
        Bmp[Haus1].AkAnzahl = 19;
        Bmp[Haus1].Sound = Sound::Forest;

        Bmp[Haus2].Anzahl = 1;
        Bmp[Haus2].Surface = lpDDSBau;
        Bmp[Haus2].Breite = 34;
        Bmp[Haus2].Hoehe = 41;
        Bmp[Haus2].rcSrc.left = 187;
        Bmp[Haus2].rcSrc.right = 187 + Bmp[Haus2].Breite;
        Bmp[Haus2].rcSrc.top = 0;
        Bmp[Haus2].rcSrc.bottom = 0 + Bmp[Haus2].Hoehe;
        Bmp[Haus2].rcDes.left = 0;
        Bmp[Haus2].rcDes.right = Bmp[Haus2].rcDes.left + Bmp[Haus2].Breite;
        Bmp[Haus2].rcDes.top = 0;
        Bmp[Haus2].rcDes.bottom = Bmp[Haus2].rcDes.top + Bmp[Haus2].Hoehe;
        Bmp[Haus2].Rohstoff[ResourceBranch] = 3;
        Bmp[Haus2].Rohstoff[ResourceTrunk] = 3;
        Bmp[Haus2].AkAnzahl = 21;
        Bmp[Haus2].Sound = Sound::Forest;

        Bmp[Haus3].Anzahl = 1;
        Bmp[Haus3].Surface = lpDDSBau;
        Bmp[Haus3].Breite = 34;
        Bmp[Haus3].Hoehe = 41;
        Bmp[Haus3].rcSrc.left = 221;
        Bmp[Haus3].rcSrc.right = 221 + Bmp[Haus3].Breite;
        Bmp[Haus3].rcSrc.top = 0;
        Bmp[Haus3].rcSrc.bottom = 0 + Bmp[Haus3].Hoehe;
        Bmp[Haus3].rcDes.left = 0;
        Bmp[Haus3].rcDes.right = Bmp[Haus3].rcDes.left + Bmp[Haus3].Breite;
        Bmp[Haus3].rcDes.top = 0;
        Bmp[Haus3].rcDes.bottom = Bmp[Haus3].rcDes.top + Bmp[Haus3].Hoehe;
        Bmp[Haus3].Rohstoff[ResourceBranch] = 4;
        Bmp[Haus3].Rohstoff[ResourceTrunk] = 4;
        Bmp[Haus3].Rohstoff[ResourceLeaf] = 5;
        Bmp[Haus3].AkAnzahl = 21;
        Bmp[Haus3].Sound = Sound::Forest;

        Bmp[Feuerstelle].Anzahl = 1;
        Bmp[Feuerstelle].Surface = lpDDSBau;
        Bmp[Feuerstelle].Breite = 21;
        Bmp[Feuerstelle].Hoehe = 19;
        Bmp[Feuerstelle].rcSrc.left = 255;
        Bmp[Feuerstelle].rcSrc.right = 255 + Bmp[Feuerstelle].Breite;
        Bmp[Feuerstelle].rcSrc.top = 0;
        Bmp[Feuerstelle].rcSrc.bottom = 0 + Bmp[Feuerstelle].Hoehe;
        Bmp[Feuerstelle].rcDes.left = 15;
        Bmp[Feuerstelle].rcDes.right = Bmp[Feuerstelle].rcDes.left + Bmp[Feuerstelle].Breite;
        Bmp[Feuerstelle].rcDes.top = 10;
        Bmp[Feuerstelle].rcDes.bottom = Bmp[Feuerstelle].rcDes.top + Bmp[Feuerstelle].Hoehe;
        Bmp[Feuerstelle].Rohstoff[ResourceBranch] = 5;
        Bmp[Feuerstelle].Rohstoff[ResourceTrunk] = 4;
        Bmp[Feuerstelle].AkAnzahl = 9;

        // Allgemein Bäume
        for (i = Baum1; i <= Baum4; i++)
        {
            Bmp[i].Animation = true;
            Bmp[i].Anzahl = 4;
            Bmp[i].Geschwindigkeit = 4;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSBaum;
            Bmp[i].Sound = Sound::Forest;
        }

        // Baum1
        Bmp[Baum1].rcSrc.left = 0;
        Bmp[Baum1].rcSrc.top = 0;
        Bmp[Baum1].rcSrc.right = 21;
        Bmp[Baum1].rcSrc.bottom = 26;
        Bmp[Baum1].Breite = static_cast<short>(Bmp[Baum1].rcSrc.right - Bmp[Baum1].rcSrc.left);
        Bmp[Baum1].Hoehe = static_cast<short>(Bmp[Baum1].rcSrc.bottom - Bmp[Baum1].rcSrc.top);

        // Baum2
        Bmp[Baum2].rcSrc.left = 21;
        Bmp[Baum2].rcSrc.top = 0;
        Bmp[Baum2].rcSrc.right = 42;
        Bmp[Baum2].rcSrc.bottom = 26;
        Bmp[Baum2].Breite = static_cast<short>(Bmp[Baum2].rcSrc.right - Bmp[Baum2].rcSrc.left);
        Bmp[Baum2].Hoehe = static_cast<short>(Bmp[Baum2].rcSrc.bottom - Bmp[Baum2].rcSrc.top);

        // Baum3
        Bmp[Baum3].rcSrc.left = 42;
        Bmp[Baum3].rcSrc.top = 0;
        Bmp[Baum3].rcSrc.right = 64;
        Bmp[Baum3].rcSrc.bottom = 27;
        Bmp[Baum3].Breite = static_cast<short>(Bmp[Baum3].rcSrc.right - Bmp[Baum3].rcSrc.left);
        Bmp[Baum3].Hoehe = static_cast<short>(Bmp[Baum3].rcSrc.bottom - Bmp[Baum3].rcSrc.top);

        // Baum4
        Bmp[Baum4].rcSrc.left = 64;
        Bmp[Baum4].rcSrc.top = 0;
        Bmp[Baum4].rcSrc.right = 81;
        Bmp[Baum4].rcSrc.bottom = 16;
        Bmp[Baum4].Breite = static_cast<short>(Bmp[Baum4].rcSrc.right - Bmp[Baum4].rcSrc.left);
        Bmp[Baum4].Hoehe = static_cast<short>(Bmp[Baum4].rcSrc.bottom - Bmp[Baum4].rcSrc.top);

        // Baumgroß
        Bmp[BaumGross].rcSrc.left = 238;
        Bmp[BaumGross].rcSrc.top = 0;
        Bmp[BaumGross].rcSrc.right = 238 + 26;
        Bmp[BaumGross].rcSrc.bottom = 41;
        Bmp[BaumGross].Breite = static_cast<short>(Bmp[BaumGross].rcSrc.right - Bmp[BaumGross].rcSrc.left);
        Bmp[BaumGross].Hoehe = static_cast<short>(Bmp[BaumGross].rcSrc.bottom - Bmp[BaumGross].rcSrc.top);
        Bmp[BaumGross].Animation = false;
        Bmp[BaumGross].Anzahl = 1;
        Bmp[BaumGross].Geschwindigkeit = 0;
        Bmp[BaumGross].Phase = 0;
        Bmp[BaumGross].Surface = lpDDSBaum;
        Bmp[BaumGross].Sound = Sound::Forest;

        // Feuer
        Bmp[Feuer].Anzahl = 3;
        Bmp[Feuer].Surface = lpDDSBau;
        Bmp[Feuer].Breite = 21;
        Bmp[Feuer].Hoehe = 28;
        Bmp[Feuer].rcSrc.left = 276;
        Bmp[Feuer].rcSrc.right = 276 + Bmp[Feuer].Breite;
        Bmp[Feuer].rcSrc.top = 0;
        Bmp[Feuer].rcSrc.bottom = 0 + Bmp[Feuer].Hoehe;
        Bmp[Feuer].rcDes.left = 15;
        Bmp[Feuer].rcDes.right = Bmp[Feuer].rcDes.left + Bmp[Feuer].Breite;
        Bmp[Feuer].rcDes.top = 1;
        Bmp[Feuer].rcDes.bottom = Bmp[Feuer].rcDes.top + Bmp[Feuer].Hoehe;
        Bmp[Feuer].Animation = true;
        Bmp[Feuer].Geschwindigkeit = 6;
        Bmp[Feuer].Phase = 0;
        Bmp[Feuer].Sound = Sound::Fire;

        // WRACK
        Bmp[Wrack].Anzahl = 3;
        Bmp[Wrack].Surface = lpDDSBau;
        Bmp[Wrack].Breite = 24;
        Bmp[Wrack].Hoehe = 18;
        Bmp[Wrack].rcSrc.left = 391;
        Bmp[Wrack].rcSrc.right = 391 + Bmp[Wrack].Breite;
        Bmp[Wrack].rcSrc.top = 0;
        Bmp[Wrack].rcSrc.bottom = 0 + Bmp[Wrack].Hoehe;
        Bmp[Wrack].rcDes.left = 15;
        Bmp[Wrack].rcDes.right = Bmp[Wrack].rcDes.left + Bmp[Wrack].Breite;
        Bmp[Wrack].rcDes.top = 20;
        Bmp[Wrack].rcDes.bottom = Bmp[Wrack].rcDes.top + Bmp[Wrack].Hoehe;
        Bmp[Wrack].Animation = true;
        Bmp[Wrack].Geschwindigkeit = 5;
        Bmp[Wrack].Phase = 0;

        // WRACK2
        Bmp[Wrack2].Anzahl = 3;
        Bmp[Wrack2].Surface = lpDDSBau;
        Bmp[Wrack2].Breite = 27;
        Bmp[Wrack2].Hoehe = 26;
        Bmp[Wrack2].rcSrc.left = 415;
        Bmp[Wrack2].rcSrc.right = 415 + Bmp[Wrack2].Breite;
        Bmp[Wrack2].rcSrc.top = 0;
        Bmp[Wrack2].rcSrc.bottom = 0 + Bmp[Wrack2].Hoehe;
        Bmp[Wrack2].rcDes.left = 15;
        Bmp[Wrack2].rcDes.right = Bmp[Wrack2].rcDes.left + Bmp[Wrack2].Breite;
        Bmp[Wrack2].rcDes.top = 16;
        Bmp[Wrack2].rcDes.bottom = Bmp[Wrack2].rcDes.top + Bmp[Wrack2].Hoehe;
        Bmp[Wrack2].Animation = true;
        Bmp[Wrack2].Geschwindigkeit = 5;
        Bmp[Wrack2].Phase = 0;

        // Früchte
        // Busch
        Bmp[Busch].rcSrc.left = 81;
        Bmp[Busch].rcSrc.top = 0;
        Bmp[Busch].rcSrc.right = 81 + 20;
        Bmp[Busch].rcSrc.bottom = 13;
        Bmp[Busch].Breite = static_cast<short>(Bmp[Busch].rcSrc.right - Bmp[Busch].rcSrc.left);
        Bmp[Busch].Hoehe = static_cast<short>(Bmp[Busch].rcSrc.bottom - Bmp[Busch].rcSrc.top);
        Bmp[Busch].Animation = false;
        Bmp[Busch].Anzahl = 3;
        Bmp[Busch].Geschwindigkeit = 0;
        Bmp[Busch].Phase = 0;
        Bmp[Busch].Surface = lpDDSBaum;

		// Buttons

        // StandardBmponsinitialisierung
        for (i = ButtonGitter; i <= ButtonDestroy; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Surface = lpDDSButtons;
            Bmp[i].Anzahl = 1;
            Bmp[i].Phase = 0;
        }

        // ButtGitter
        Bmp[ButtonGitter].rcSrc.left = 0;
        Bmp[ButtonGitter].rcSrc.top = 0;
        Bmp[ButtonGitter].rcSrc.right = 28;
        Bmp[ButtonGitter].rcSrc.bottom = 28;
        Bmp[ButtonGitter].rcDes.left = rcPanel.left + 173;
        Bmp[ButtonGitter].rcDes.top = rcPanel.top + 26;
        Bmp[ButtonGitter].rcDes.right = Bmp[ButtonGitter].rcDes.left + 28;
        Bmp[ButtonGitter].rcDes.bottom = Bmp[ButtonGitter].rcDes.top + 28;
        Bmp[ButtonGitter].Breite = static_cast<short>(Bmp[ButtonGitter].rcSrc.right - Bmp[ButtonGitter].rcSrc.left);
        Bmp[ButtonGitter].Hoehe = static_cast<short>(Bmp[ButtonGitter].rcSrc.bottom - Bmp[ButtonGitter].rcSrc.top);
        Bmp[ButtonGitter].Anzahl = 2;

        // BUTTANIMATION
        Bmp[ButtonAnimation].rcSrc.left = 0;
        Bmp[ButtonAnimation].rcSrc.top = 56;
        Bmp[ButtonAnimation].rcSrc.right = 28;
        Bmp[ButtonAnimation].rcSrc.bottom = 56 + 28;
        Bmp[ButtonAnimation].rcDes.left = rcPanel.left + 173;
        Bmp[ButtonAnimation].rcDes.top = rcPanel.top + 60;
        Bmp[ButtonAnimation].rcDes.right = Bmp[ButtonAnimation].rcDes.left + 28;
        Bmp[ButtonAnimation].rcDes.bottom = Bmp[ButtonAnimation].rcDes.top + 28;
        Bmp[ButtonAnimation].Breite = static_cast<short>(Bmp[ButtonAnimation].rcSrc.right - Bmp[ButtonAnimation].rcSrc.left);
        Bmp[ButtonAnimation].Hoehe = static_cast<short>(Bmp[ButtonAnimation].rcSrc.bottom - Bmp[ButtonAnimation].rcSrc.top);
        Bmp[ButtonAnimation].Anzahl = 2;

        // BUTTBEENDEN
        Bmp[ButtonBeenden].rcSrc.left = 0;
        Bmp[ButtonBeenden].rcSrc.top = 112;
        Bmp[ButtonBeenden].rcSrc.right = 20;
        Bmp[ButtonBeenden].rcSrc.bottom = 112 + 20;
        Bmp[ButtonBeenden].rcDes.left = rcPanel.left + 60;
        Bmp[ButtonBeenden].rcDes.top = rcPanel.top + 2;
        Bmp[ButtonBeenden].rcDes.right = Bmp[ButtonBeenden].rcDes.left + 20;
        Bmp[ButtonBeenden].rcDes.bottom = Bmp[ButtonBeenden].rcDes.top + 20;
        Bmp[ButtonBeenden].Breite = static_cast<short>(Bmp[ButtonBeenden].rcSrc.right - Bmp[ButtonBeenden].rcSrc.left);
        Bmp[ButtonBeenden].Hoehe = static_cast<short>(Bmp[ButtonBeenden].rcSrc.bottom - Bmp[ButtonBeenden].rcSrc.top);
        Bmp[ButtonBeenden].Anzahl = 4;
        Bmp[ButtonBeenden].Geschwindigkeit = 4;

        // BUTTNEU
        Bmp[ButtonNew].rcSrc.left = 0;
        Bmp[ButtonNew].rcSrc.top = 192;
        Bmp[ButtonNew].rcSrc.right = 20;
        Bmp[ButtonNew].rcSrc.bottom = 192 + 20;
        Bmp[ButtonNew].rcDes.left = rcPanel.left + 100;
        Bmp[ButtonNew].rcDes.top = rcPanel.top + 2;
        Bmp[ButtonNew].rcDes.right = Bmp[ButtonNew].rcDes.left + 20;
        Bmp[ButtonNew].rcDes.bottom = Bmp[ButtonNew].rcDes.top + 20;
        Bmp[ButtonNew].Breite = static_cast<short>(Bmp[ButtonNew].rcSrc.right - Bmp[ButtonNew].rcSrc.left);
        Bmp[ButtonNew].Hoehe = static_cast<short>(Bmp[ButtonNew].rcSrc.bottom - Bmp[ButtonNew].rcSrc.top);
        Bmp[ButtonNew].Anzahl = 2;
        Bmp[ButtonNew].Geschwindigkeit = 3;


        // BUTTTAGNEU
        Bmp[ButtonTagNeu].rcSrc.left = 0;
        Bmp[ButtonTagNeu].rcSrc.top = 232;
        Bmp[ButtonTagNeu].rcSrc.right = 20;
        Bmp[ButtonTagNeu].rcSrc.bottom = 232 + 20;
        Bmp[ButtonTagNeu].rcDes.left = rcPanel.left + 140;
        Bmp[ButtonTagNeu].rcDes.top = rcPanel.top + 2;
        Bmp[ButtonTagNeu].rcDes.right = Bmp[ButtonTagNeu].rcDes.left + 20;
        Bmp[ButtonTagNeu].rcDes.bottom = Bmp[ButtonTagNeu].rcDes.top + 20;
        Bmp[ButtonTagNeu].Breite = static_cast<short>(Bmp[ButtonTagNeu].rcSrc.right - Bmp[ButtonTagNeu].rcSrc.left);
        Bmp[ButtonTagNeu].Hoehe = static_cast<short>(Bmp[ButtonTagNeu].rcSrc.bottom - Bmp[ButtonTagNeu].rcSrc.top);
        Bmp[ButtonTagNeu].Anzahl = 2;
        Bmp[ButtonTagNeu].Geschwindigkeit = 2;

        // BUTTSOUND
        Bmp[ButtonSound].rcSrc.left = 0;
        Bmp[ButtonSound].rcSrc.top = 272;
        Bmp[ButtonSound].rcSrc.right = 28;
        Bmp[ButtonSound].rcSrc.bottom = 272 + 28;
        Bmp[ButtonSound].rcDes.left = rcPanel.left + 173;
        Bmp[ButtonSound].rcDes.top = rcPanel.top + 94;
        Bmp[ButtonSound].rcDes.right = Bmp[ButtonSound].rcDes.left + 28;
        Bmp[ButtonSound].rcDes.bottom = Bmp[ButtonSound].rcDes.top + 28;
        Bmp[ButtonSound].Breite = static_cast<short>(Bmp[ButtonSound].rcSrc.right - Bmp[ButtonSound].rcSrc.left);
        Bmp[ButtonSound].Hoehe = static_cast<short>(Bmp[ButtonSound].rcSrc.bottom - Bmp[ButtonSound].rcSrc.top);
        Bmp[ButtonSound].Anzahl = 2;

        // ButtAktion
        Bmp[ButtonAction].rcSrc.left = 28;
        Bmp[ButtonAction].rcSrc.top = 0;
        Bmp[ButtonAction].rcSrc.right = 28 + 35;
        Bmp[ButtonAction].rcSrc.bottom = 35;
        Bmp[ButtonAction].rcDes.left = rcPanel.left + 29;
        Bmp[ButtonAction].rcDes.top = rcPanel.top + 191;
        Bmp[ButtonAction].rcDes.right = Bmp[ButtonAction].rcDes.left + 35;
        Bmp[ButtonAction].rcDes.bottom = Bmp[ButtonAction].rcDes.top + 35;
        Bmp[ButtonAction].Breite = static_cast<short>(Bmp[ButtonAction].rcSrc.right - Bmp[ButtonAction].rcSrc.left);
        Bmp[ButtonAction].Hoehe = static_cast<short>(Bmp[ButtonAction].rcSrc.bottom - Bmp[ButtonAction].rcSrc.top);
        Bmp[ButtonAction].Anzahl = 3;
        Bmp[ButtonAction].Geschwindigkeit = 6;

        // BUTTBAUEN
        Bmp[ButtonBauen].rcSrc.left = 203;
        Bmp[ButtonBauen].rcSrc.top = 0;
        Bmp[ButtonBauen].rcSrc.right = 203 + 35;
        Bmp[ButtonBauen].rcSrc.bottom = 35;
        Bmp[ButtonBauen].rcDes.left = rcPanel.left + 70;
        Bmp[ButtonBauen].rcDes.top = rcPanel.top + 191;
        Bmp[ButtonBauen].rcDes.right = Bmp[ButtonBauen].rcDes.left + 35;
        Bmp[ButtonBauen].rcDes.bottom = Bmp[ButtonBauen].rcDes.top + 35;
        Bmp[ButtonBauen].Breite = static_cast<short>(Bmp[ButtonBauen].rcSrc.right - Bmp[ButtonBauen].rcSrc.left);
        Bmp[ButtonBauen].Hoehe = static_cast<short>(Bmp[ButtonBauen].rcSrc.bottom - Bmp[ButtonBauen].rcSrc.top);
        Bmp[ButtonBauen].Anzahl = 4;
        Bmp[ButtonBauen].Geschwindigkeit = 5;

        // BUTTINVENTAR
        Bmp[ButtonInventory].rcSrc.left = 168;
        Bmp[ButtonInventory].rcSrc.top = 0;
        Bmp[ButtonInventory].rcSrc.right = 168 + 35;
        Bmp[ButtonInventory].rcSrc.bottom = 35;
        Bmp[ButtonInventory].rcDes.left = rcPanel.left + 152;
        Bmp[ButtonInventory].rcDes.top = rcPanel.top + 191;
        Bmp[ButtonInventory].rcDes.right = Bmp[ButtonInventory].rcDes.left + 35;
        Bmp[ButtonInventory].rcDes.bottom = Bmp[ButtonInventory].rcDes.top + 35;
        Bmp[ButtonInventory].Breite = static_cast<short>(Bmp[ButtonInventory].rcSrc.right - Bmp[ButtonInventory].rcSrc.left);
        Bmp[ButtonInventory].Hoehe = static_cast<short>(Bmp[ButtonInventory].rcSrc.bottom - Bmp[ButtonInventory].rcSrc.top);
        Bmp[ButtonInventory].Anzahl = 4;
        Bmp[ButtonInventory].Geschwindigkeit = 4;

        // BUTTWEITER
        Bmp[ButtonWeiter].rcSrc.left = 343;
        Bmp[ButtonWeiter].rcSrc.top = 0;
        Bmp[ButtonWeiter].rcSrc.right = 343 + 35;
        Bmp[ButtonWeiter].rcSrc.bottom = 35;
        Bmp[ButtonWeiter].rcDes.left = rcPanel.left + 111;
        Bmp[ButtonWeiter].rcDes.top = rcPanel.top + 191;
        Bmp[ButtonWeiter].rcDes.right = Bmp[ButtonWeiter].rcDes.left + 35;
        Bmp[ButtonWeiter].rcDes.bottom = Bmp[ButtonWeiter].rcDes.top + 35;
        Bmp[ButtonWeiter].Breite = static_cast<short>(Bmp[ButtonWeiter].rcSrc.right - Bmp[ButtonWeiter].rcSrc.left);
        Bmp[ButtonWeiter].Hoehe = static_cast<short>(Bmp[ButtonWeiter].rcSrc.bottom - Bmp[ButtonWeiter].rcSrc.top);
        Bmp[ButtonWeiter].Anzahl = 4;
        Bmp[ButtonWeiter].Geschwindigkeit = 4;
        Bmp[ButtonWeiter].Phase = -1;

        // BUTTSTOP
        Bmp[ButtonStop].rcSrc.left = 378;
        Bmp[ButtonStop].rcSrc.top = 0;
        Bmp[ButtonStop].rcSrc.right = 378 + 35;
        Bmp[ButtonStop].rcSrc.bottom = 35;
        Bmp[ButtonStop].rcDes.left = rcPanel.left + 111;
        Bmp[ButtonStop].rcDes.top = rcPanel.top + 191;
        Bmp[ButtonStop].rcDes.right = Bmp[ButtonStop].rcDes.left + 35;
        Bmp[ButtonStop].rcDes.bottom = Bmp[ButtonStop].rcDes.top + 35;
        Bmp[ButtonStop].Breite = static_cast<short>(Bmp[ButtonStop].rcSrc.right - Bmp[ButtonStop].rcSrc.left);
        Bmp[ButtonStop].Hoehe = static_cast<short>(Bmp[ButtonStop].rcSrc.bottom - Bmp[ButtonStop].rcSrc.top);
        Bmp[ButtonStop].Anzahl = 4;
        Bmp[ButtonStop].Geschwindigkeit = 4;
        Bmp[ButtonStop].Phase = -1;

        // BUTTABLEGEN
        Bmp[ButtonAblegen].rcSrc.left = 483;
        Bmp[ButtonAblegen].rcSrc.top = 0;
        Bmp[ButtonAblegen].rcSrc.right = 483 + 35;
        Bmp[ButtonAblegen].rcSrc.bottom = 35;
        Bmp[ButtonAblegen].rcDes.left = rcPanel.left + 111;
        Bmp[ButtonAblegen].rcDes.top = rcPanel.top + 191;
        Bmp[ButtonAblegen].rcDes.right = Bmp[ButtonAblegen].rcDes.left + 35;
        Bmp[ButtonAblegen].rcDes.bottom = Bmp[ButtonAblegen].rcDes.top + 35;
        Bmp[ButtonAblegen].Breite = static_cast<short>(Bmp[ButtonAblegen].rcSrc.right - Bmp[ButtonAblegen].rcSrc.left);
        Bmp[ButtonAblegen].Hoehe = static_cast<short>(Bmp[ButtonAblegen].rcSrc.bottom - Bmp[ButtonAblegen].rcSrc.top);
        Bmp[ButtonAblegen].Anzahl = 4;
        Bmp[ButtonAblegen].Geschwindigkeit = 3;
        Bmp[ButtonStop].Phase = -1;

        // BUTTSUCHEN
        Bmp[ButtonSearch].rcSrc.left = 63;
        Bmp[ButtonSearch].rcSrc.top = 0;
        Bmp[ButtonSearch].rcSrc.right = 63 + 35;
        Bmp[ButtonSearch].rcSrc.bottom = 35;
        Bmp[ButtonSearch].rcDes.left = rcPanel.left + 29;
        Bmp[ButtonSearch].rcDes.top = rcPanel.top + 270;
        Bmp[ButtonSearch].rcDes.right = Bmp[ButtonSearch].rcDes.left + 35;
        Bmp[ButtonSearch].rcDes.bottom = Bmp[ButtonSearch].rcDes.top + 35;
        Bmp[ButtonSearch].Breite = static_cast<short>(Bmp[ButtonSearch].rcSrc.right - Bmp[ButtonSearch].rcSrc.left);
        Bmp[ButtonSearch].Hoehe = static_cast<short>(Bmp[ButtonSearch].rcSrc.bottom - Bmp[ButtonSearch].rcSrc.top);
        Bmp[ButtonSearch].Anzahl = 4;
        Bmp[ButtonSearch].Geschwindigkeit = 4;

        // BUTTESSEN
        Bmp[ButtonFood].rcSrc.left = 133;
        Bmp[ButtonFood].rcSrc.top = 0;
        Bmp[ButtonFood].rcSrc.right = 133 + 35;
        Bmp[ButtonFood].rcSrc.bottom = 35;
        Bmp[ButtonFood].rcDes.left = rcPanel.left + 70;
        Bmp[ButtonFood].rcDes.top = rcPanel.top + 270;
        Bmp[ButtonFood].rcDes.right = Bmp[ButtonFood].rcDes.left + 35;
        Bmp[ButtonFood].rcDes.bottom = Bmp[ButtonFood].rcDes.top + 35;
        Bmp[ButtonFood].Breite = static_cast<short>(Bmp[ButtonFood].rcSrc.right - Bmp[ButtonFood].rcSrc.left);
        Bmp[ButtonFood].Hoehe = static_cast<short>(Bmp[ButtonFood].rcSrc.bottom - Bmp[ButtonFood].rcSrc.top);
        Bmp[ButtonFood].Anzahl = 4;
        Bmp[ButtonFood].Geschwindigkeit = 4;

        // BUTTSCHLAFEN
        Bmp[ButtonSleep].rcSrc.left = 308;
        Bmp[ButtonSleep].rcSrc.top = 0;
        Bmp[ButtonSleep].rcSrc.right = 308 + 35;
        Bmp[ButtonSleep].rcSrc.bottom = 35;
        Bmp[ButtonSleep].rcDes.left = rcPanel.left + 111;
        Bmp[ButtonSleep].rcDes.top = rcPanel.top + 270;
        Bmp[ButtonSleep].rcDes.right = Bmp[ButtonSleep].rcDes.left + 35;
        Bmp[ButtonSleep].rcDes.bottom = Bmp[ButtonSleep].rcDes.top + 35;
        Bmp[ButtonSleep].Breite = static_cast<short>(Bmp[ButtonSleep].rcSrc.right - Bmp[ButtonSleep].rcSrc.left);
        Bmp[ButtonSleep].Hoehe = static_cast<short>(Bmp[ButtonSleep].rcSrc.bottom - Bmp[ButtonSleep].rcSrc.top);
        Bmp[ButtonSleep].Anzahl = 4;
        Bmp[ButtonSleep].Geschwindigkeit = 3;

        // BUTTFAELLEN
        Bmp[ButtonTimber].rcSrc.left = 98;
        Bmp[ButtonTimber].rcSrc.top = 0;
        Bmp[ButtonTimber].rcSrc.right = 98 + 35;
        Bmp[ButtonTimber].rcSrc.bottom = 35;
        Bmp[ButtonTimber].rcDes.left = rcPanel.left + 152;
        Bmp[ButtonTimber].rcDes.top = rcPanel.top + 270;
        Bmp[ButtonTimber].rcDes.right = Bmp[ButtonTimber].rcDes.left + 35;
        Bmp[ButtonTimber].rcDes.bottom = Bmp[ButtonTimber].rcDes.top + 35;
        Bmp[ButtonTimber].Breite = static_cast<short>(Bmp[ButtonTimber].rcSrc.right - Bmp[ButtonTimber].rcSrc.left);
        Bmp[ButtonTimber].Hoehe = static_cast<short>(Bmp[ButtonTimber].rcSrc.bottom - Bmp[ButtonTimber].rcSrc.top);
        Bmp[ButtonTimber].Anzahl = 4;
        Bmp[ButtonTimber].Geschwindigkeit = 4;
        Bmp[ButtonTimber].Phase = -1;

        // BUTTANGELN
        Bmp[ButtonFish].rcSrc.left = 413;
        Bmp[ButtonFish].rcSrc.top = 0;
        Bmp[ButtonFish].rcSrc.right = 413 + 35;
        Bmp[ButtonFish].rcSrc.bottom = 35;
        Bmp[ButtonFish].rcDes.left = rcPanel.left + 29;
        Bmp[ButtonFish].rcDes.top = rcPanel.top + 325;
        Bmp[ButtonFish].rcDes.right = Bmp[ButtonFish].rcDes.left + 35;
        Bmp[ButtonFish].rcDes.bottom = Bmp[ButtonFish].rcDes.top + 35;
        Bmp[ButtonFish].Breite = static_cast<short>(Bmp[ButtonFish].rcSrc.right - Bmp[ButtonFish].rcSrc.left);
        Bmp[ButtonFish].Hoehe = static_cast<short>(Bmp[ButtonFish].rcSrc.bottom - Bmp[ButtonFish].rcSrc.top);
        Bmp[ButtonFish].Anzahl = 4;
        Bmp[ButtonFish].Geschwindigkeit = 3;
        Bmp[ButtonFish].Phase = -1;

        // BUTTANZUENDEN
        Bmp[ButtonLight].rcSrc.left = 28;
        Bmp[ButtonLight].rcSrc.top = 175;
        Bmp[ButtonLight].rcSrc.right = 28 + 35;
        Bmp[ButtonLight].rcSrc.bottom = 175 + 35;
        Bmp[ButtonLight].rcDes.left = rcPanel.left + 70;
        Bmp[ButtonLight].rcDes.top = rcPanel.top + 325;
        Bmp[ButtonLight].rcDes.right = Bmp[ButtonLight].rcDes.left + 35;
        Bmp[ButtonLight].rcDes.bottom = Bmp[ButtonLight].rcDes.top + 35;
        Bmp[ButtonLight].Breite = static_cast<short>(Bmp[ButtonLight].rcSrc.right - Bmp[ButtonLight].rcSrc.left);
        Bmp[ButtonLight].Hoehe = static_cast<short>(Bmp[ButtonLight].rcSrc.bottom - Bmp[ButtonLight].rcSrc.top);
        Bmp[ButtonLight].Anzahl = 3;
        Bmp[ButtonLight].Geschwindigkeit = 4;
        Bmp[ButtonLight].Phase = -1;

        // BUTTAUSSCHAU
        Bmp[ButtonLook].rcSrc.left = 63;
        Bmp[ButtonLook].rcSrc.top = 175;
        Bmp[ButtonLook].rcSrc.right = 63 + 35;
        Bmp[ButtonLook].rcSrc.bottom = 175 + 35;
        Bmp[ButtonLook].rcDes.left = rcPanel.left + 111;
        Bmp[ButtonLook].rcDes.top = rcPanel.top + 325;
        Bmp[ButtonLook].rcDes.right = Bmp[ButtonLook].rcDes.left + 35;
        Bmp[ButtonLook].rcDes.bottom = Bmp[ButtonLook].rcDes.top + 35;
        Bmp[ButtonLook].Breite = static_cast<short>(Bmp[ButtonLook].rcSrc.right - Bmp[ButtonLook].rcSrc.left);
        Bmp[ButtonLook].Hoehe = static_cast<short>(Bmp[ButtonLook].rcSrc.bottom - Bmp[ButtonLook].rcSrc.top);
        Bmp[ButtonLook].Anzahl = 4;
        Bmp[ButtonLook].Geschwindigkeit = 3;
        Bmp[ButtonLook].Phase = -1;

        // BUTTSCHATZKARTE
        Bmp[ButtonTreasureMap].rcSrc.left = 98;
        Bmp[ButtonTreasureMap].rcSrc.top = 175;
        Bmp[ButtonTreasureMap].rcSrc.right = 98 + 35;
        Bmp[ButtonTreasureMap].rcSrc.bottom = 175 + 35;
        Bmp[ButtonTreasureMap].rcDes.left = rcPanel.left + 152;
        Bmp[ButtonTreasureMap].rcDes.top = rcPanel.top + 325;
        Bmp[ButtonTreasureMap].rcDes.right = Bmp[ButtonTreasureMap].rcDes.left + 35;
        Bmp[ButtonTreasureMap].rcDes.bottom = Bmp[ButtonTreasureMap].rcDes.top + 35;
        Bmp[ButtonTreasureMap].Breite = static_cast<short>(Bmp[ButtonTreasureMap].rcSrc.right - Bmp[ButtonTreasureMap].rcSrc.left);
        Bmp[ButtonTreasureMap].Hoehe = static_cast<short>(Bmp[ButtonTreasureMap].rcSrc.bottom - Bmp[ButtonTreasureMap].rcSrc.top);
        Bmp[ButtonTreasureMap].Anzahl = 4;
        Bmp[ButtonTreasureMap].Geschwindigkeit = 3;
        Bmp[ButtonTreasureMap].Phase = -1;

        // BUTTSCHATZ
        Bmp[ButtonTreasure].rcSrc.left = 133;
        Bmp[ButtonTreasure].rcSrc.top = 175;
        Bmp[ButtonTreasure].rcSrc.right = 133 + 35;
        Bmp[ButtonTreasure].rcSrc.bottom = 175 + 35;
        Bmp[ButtonTreasure].rcDes.left = rcPanel.left + 29;
        Bmp[ButtonTreasure].rcDes.top = rcPanel.top + 380;
        Bmp[ButtonTreasure].rcDes.right = Bmp[ButtonTreasure].rcDes.left + 35;
        Bmp[ButtonTreasure].rcDes.bottom = Bmp[ButtonTreasure].rcDes.top + 35;
        Bmp[ButtonTreasure].Breite = static_cast<short>(Bmp[ButtonTreasure].rcSrc.right - Bmp[ButtonTreasure].rcSrc.left);
        Bmp[ButtonTreasure].Hoehe = static_cast<short>(Bmp[ButtonTreasure].rcSrc.bottom - Bmp[ButtonTreasure].rcSrc.top);
        Bmp[ButtonTreasure].Anzahl = 4;
        Bmp[ButtonTreasure].Geschwindigkeit = 3;
        Bmp[ButtonTreasure].Phase = -1;

        // BUTTSCHLEUDER
        Bmp[ButtonSlingshot].rcSrc.left = 168;
        Bmp[ButtonSlingshot].rcSrc.top = 175;
        Bmp[ButtonSlingshot].rcSrc.right = 168 + 35;
        Bmp[ButtonSlingshot].rcSrc.bottom = 175 + 35;
        Bmp[ButtonSlingshot].rcDes.left = rcPanel.left + 70;
        Bmp[ButtonSlingshot].rcDes.top = rcPanel.top + 380;
        Bmp[ButtonSlingshot].rcDes.right = Bmp[ButtonSlingshot].rcDes.left + 35;
        Bmp[ButtonSlingshot].rcDes.bottom = Bmp[ButtonSlingshot].rcDes.top + 35;
        Bmp[ButtonSlingshot].Breite = static_cast<short>(Bmp[ButtonSlingshot].rcSrc.right - Bmp[ButtonSlingshot].rcSrc.left);
        Bmp[ButtonSlingshot].Hoehe = static_cast<short>(Bmp[ButtonSlingshot].rcSrc.bottom - Bmp[ButtonSlingshot].rcSrc.top);
        Bmp[ButtonSlingshot].Anzahl = 4;
        Bmp[ButtonSlingshot].Geschwindigkeit = 3;
        Bmp[ButtonSlingshot].Phase = -1;

        // BUTTFELD
        Bmp[ButtonField].rcSrc.left = 238;
        Bmp[ButtonField].rcSrc.top = 0;
        Bmp[ButtonField].rcSrc.right = 238 + 35;
        Bmp[ButtonField].rcSrc.bottom = 35;
        Bmp[ButtonField].rcDes.left = rcPanel.left + 70;
        Bmp[ButtonField].rcDes.top = rcPanel.top + 270;
        Bmp[ButtonField].rcDes.right = Bmp[ButtonField].rcDes.left + 35;
        Bmp[ButtonField].rcDes.bottom = Bmp[ButtonField].rcDes.top + 35;
        Bmp[ButtonField].Breite = static_cast<short>(Bmp[ButtonField].rcSrc.right - Bmp[ButtonField].rcSrc.left);
        Bmp[ButtonField].Hoehe = static_cast<short>(Bmp[ButtonField].rcSrc.bottom - Bmp[ButtonField].rcSrc.top);
        Bmp[ButtonField].Anzahl = 4;
        Bmp[ButtonField].Geschwindigkeit = 3;
        Bmp[ButtonField].Phase = -1;

        // BUTTZELT
        Bmp[ButtonTent].rcSrc.left = 273;
        Bmp[ButtonTent].rcSrc.top = 0;
        Bmp[ButtonTent].rcSrc.right = 273 + 35;
        Bmp[ButtonTent].rcSrc.bottom = 35;
        Bmp[ButtonTent].rcDes.left = rcPanel.left + 29;
        Bmp[ButtonTent].rcDes.top = rcPanel.top + 270;
        Bmp[ButtonTent].rcDes.right = Bmp[ButtonTent].rcDes.left + 35;
        Bmp[ButtonTent].rcDes.bottom = Bmp[ButtonTent].rcDes.top + 35;
        Bmp[ButtonTent].Breite = static_cast<short>(Bmp[ButtonTent].rcSrc.right - Bmp[ButtonTent].rcSrc.left);
        Bmp[ButtonTent].Hoehe = static_cast<short>(Bmp[ButtonTent].rcSrc.bottom - Bmp[ButtonTent].rcSrc.top);
        Bmp[ButtonTent].Anzahl = 3;
        Bmp[ButtonTent].Geschwindigkeit = 3;
        Bmp[ButtonTent].Phase = 0;

        // BUTTBOOT
        Bmp[ButtonBoat].rcSrc.left = 448;
        Bmp[ButtonBoat].rcSrc.top = 0;
        Bmp[ButtonBoat].rcSrc.right = 448 + 35;
        Bmp[ButtonBoat].rcSrc.bottom = 35;
        Bmp[ButtonBoat].rcDes.left = rcPanel.left + 111;
        Bmp[ButtonBoat].rcDes.top = rcPanel.top + 270;
        Bmp[ButtonBoat].rcDes.right = Bmp[ButtonBoat].rcDes.left + 35;
        Bmp[ButtonBoat].rcDes.bottom = Bmp[ButtonBoat].rcDes.top + 35;
        Bmp[ButtonBoat].Breite = static_cast<short>(Bmp[ButtonBoat].rcSrc.right - Bmp[ButtonBoat].rcSrc.left);
        Bmp[ButtonBoat].Hoehe = static_cast<short>(Bmp[ButtonBoat].rcSrc.bottom - Bmp[ButtonBoat].rcSrc.top);
        Bmp[ButtonBoat].Anzahl = 3;
        Bmp[ButtonBoat].Geschwindigkeit = 3;
        Bmp[ButtonBoat].Phase = -1;

        // BUTTROHR
        Bmp[ButtonPipe].rcSrc.left = 518;
        Bmp[ButtonPipe].rcSrc.top = 0;
        Bmp[ButtonPipe].rcSrc.right = 518 + 35;
        Bmp[ButtonPipe].rcSrc.bottom = 35;
        Bmp[ButtonPipe].rcDes.left = rcPanel.left + 152;
        Bmp[ButtonPipe].rcDes.top = rcPanel.top + 270;
        Bmp[ButtonPipe].rcDes.right = Bmp[ButtonPipe].rcDes.left + 35;
        Bmp[ButtonPipe].rcDes.bottom = Bmp[ButtonPipe].rcDes.top + 35;
        Bmp[ButtonPipe].Breite = static_cast<short>(Bmp[ButtonPipe].rcSrc.right - Bmp[ButtonPipe].rcSrc.left);
        Bmp[ButtonPipe].Hoehe = static_cast<short>(Bmp[ButtonPipe].rcSrc.bottom - Bmp[ButtonPipe].rcSrc.top);
        Bmp[ButtonPipe].Anzahl = 4;
        Bmp[ButtonPipe].Geschwindigkeit = 2;
        Bmp[ButtonPipe].Phase = -1;

        // BUTTSOS
        Bmp[ButtonSos].rcSrc.left = 588;
        Bmp[ButtonSos].rcSrc.top = 0;
        Bmp[ButtonSos].rcSrc.right = 588 + 35;
        Bmp[ButtonSos].rcSrc.bottom = 35;
        Bmp[ButtonSos].rcDes.left = rcPanel.left + 29;
        Bmp[ButtonSos].rcDes.top = rcPanel.top + 325;
        Bmp[ButtonSos].rcDes.right = Bmp[ButtonSos].rcDes.left + 35;
        Bmp[ButtonSos].rcDes.bottom = Bmp[ButtonSos].rcDes.top + 35;
        Bmp[ButtonSos].Breite = static_cast<short>(Bmp[ButtonSos].rcSrc.right - Bmp[ButtonSos].rcSrc.left);
        Bmp[ButtonSos].Hoehe = static_cast<short>(Bmp[ButtonSos].rcSrc.bottom - Bmp[ButtonSos].rcSrc.top);
        Bmp[ButtonSos].Anzahl = 3;
        Bmp[ButtonSos].Geschwindigkeit = 2;

        // BUTTHAUS1
        Bmp[ButtonHouse1].rcSrc.left = 623;
        Bmp[ButtonHouse1].rcSrc.top = 0;
        Bmp[ButtonHouse1].rcSrc.right = 623 + 35;
        Bmp[ButtonHouse1].rcSrc.bottom = 35;
        Bmp[ButtonHouse1].rcDes.left = rcPanel.left + 70;
        Bmp[ButtonHouse1].rcDes.top = rcPanel.top + 325;
        Bmp[ButtonHouse1].rcDes.right = Bmp[ButtonHouse1].rcDes.left + 35;
        Bmp[ButtonHouse1].rcDes.bottom = Bmp[ButtonHouse1].rcDes.top + 35;
        Bmp[ButtonHouse1].Breite = static_cast<short>(Bmp[ButtonHouse1].rcSrc.right - Bmp[ButtonHouse1].rcSrc.left);
        Bmp[ButtonHouse1].Hoehe = static_cast<short>(Bmp[ButtonHouse1].rcSrc.bottom - Bmp[ButtonHouse1].rcSrc.top);
        Bmp[ButtonHouse1].Anzahl = 5;
        Bmp[ButtonHouse1].Geschwindigkeit = 3;
        Bmp[ButtonHouse1].Phase = -1;

        // BUTTHAUS2
        Bmp[ButtonHouse2].rcSrc.left = 658;
        Bmp[ButtonHouse2].rcSrc.top = 0;
        Bmp[ButtonHouse2].rcSrc.right = 658 + 35;
        Bmp[ButtonHouse2].rcSrc.bottom = 35;
        Bmp[ButtonHouse2].rcDes.left = rcPanel.left + 111;
        Bmp[ButtonHouse2].rcDes.top = rcPanel.top + 325;
        Bmp[ButtonHouse2].rcDes.right = Bmp[ButtonHouse2].rcDes.left + 35;
        Bmp[ButtonHouse2].rcDes.bottom = Bmp[ButtonHouse2].rcDes.top + 35;
        Bmp[ButtonHouse2].Breite = static_cast<short>(Bmp[ButtonHouse2].rcSrc.right - Bmp[ButtonHouse2].rcSrc.left);
        Bmp[ButtonHouse2].Hoehe = static_cast<short>(Bmp[ButtonHouse2].rcSrc.bottom - Bmp[ButtonHouse2].rcSrc.top);
        Bmp[ButtonHouse2].Anzahl = 4;
        Bmp[ButtonHouse2].Geschwindigkeit = 3;
        Bmp[ButtonHouse2].Phase = -1;

        // BUTTHAUS3
        Bmp[ButtonHouse3].rcSrc.left = 693;
        Bmp[ButtonHouse3].rcSrc.top = 0;
        Bmp[ButtonHouse3].rcSrc.right = 693 + 35;
        Bmp[ButtonHouse3].rcSrc.bottom = 35;
        Bmp[ButtonHouse3].rcDes.left = rcPanel.left + 152;
        Bmp[ButtonHouse3].rcDes.top = rcPanel.top + 325;
        Bmp[ButtonHouse3].rcDes.right = Bmp[ButtonHouse3].rcDes.left + 35;
        Bmp[ButtonHouse3].rcDes.bottom = Bmp[ButtonHouse3].rcDes.top + 35;
        Bmp[ButtonHouse3].Breite = static_cast<short>(Bmp[ButtonHouse3].rcSrc.right - Bmp[ButtonHouse3].rcSrc.left);
        Bmp[ButtonHouse3].Hoehe = static_cast<short>(Bmp[ButtonHouse3].rcSrc.bottom - Bmp[ButtonHouse3].rcSrc.top);
        Bmp[ButtonHouse3].Anzahl = 4;
        Bmp[ButtonHouse3].Geschwindigkeit = 3;
        Bmp[ButtonHouse3].Phase = -1;

        // BUTTFEUERST
        Bmp[ButtonFireplace].rcSrc.left = 728;
        Bmp[ButtonFireplace].rcSrc.top = 0;
        Bmp[ButtonFireplace].rcSrc.right = 728 + 35;
        Bmp[ButtonFireplace].rcSrc.bottom = 35;
        Bmp[ButtonFireplace].rcDes.left = rcPanel.left + 29;
        Bmp[ButtonFireplace].rcDes.top = rcPanel.top + 380;
        Bmp[ButtonFireplace].rcDes.right = Bmp[ButtonFireplace].rcDes.left + 35;
        Bmp[ButtonFireplace].rcDes.bottom = Bmp[ButtonFireplace].rcDes.top + 35;
        Bmp[ButtonFireplace].Breite = static_cast<short>(Bmp[ButtonFireplace].rcSrc.right - Bmp[ButtonFireplace].rcSrc.left);
        Bmp[ButtonFireplace].Hoehe = static_cast<short>(Bmp[ButtonFireplace].rcSrc.bottom - Bmp[ButtonFireplace].rcSrc.top);
        Bmp[ButtonFireplace].Anzahl = 4;
        Bmp[ButtonFireplace].Geschwindigkeit = 3;

        // BUTTFRAGEZ
        Bmp[ButtonQuestionMark].rcSrc.left = 203;
        Bmp[ButtonQuestionMark].rcSrc.top = 175;
        Bmp[ButtonQuestionMark].rcSrc.right = 203 + 35;
        Bmp[ButtonQuestionMark].rcSrc.bottom = 175 + 35;
        Bmp[ButtonQuestionMark].rcDes.left = 0;
        Bmp[ButtonQuestionMark].rcDes.top = 0;
        Bmp[ButtonQuestionMark].rcDes.right = 0 + 35;
        Bmp[ButtonQuestionMark].rcDes.bottom = 0 + 35;
        Bmp[ButtonQuestionMark].Breite = static_cast<short>(Bmp[ButtonQuestionMark].rcSrc.right - Bmp[ButtonQuestionMark].rcSrc.left);
        Bmp[ButtonQuestionMark].Hoehe = static_cast<short>(Bmp[ButtonQuestionMark].rcSrc.bottom - Bmp[ButtonQuestionMark].rcSrc.top);
        Bmp[ButtonQuestionMark].Anzahl = 1;
        Bmp[ButtonQuestionMark].Geschwindigkeit = 0;

        // BUTTDESTROY
        Bmp[ButtonDestroy].rcSrc.left = 553;
        Bmp[ButtonDestroy].rcSrc.top = 0;
        Bmp[ButtonDestroy].rcSrc.right = 553 + 35;
        Bmp[ButtonDestroy].rcSrc.bottom = 35;
        Bmp[ButtonDestroy].rcDes.left = rcPanel.left + 152;
        Bmp[ButtonDestroy].rcDes.top = rcPanel.top + 380;
        Bmp[ButtonDestroy].rcDes.right = Bmp[ButtonDestroy].rcDes.left + 35;
        Bmp[ButtonDestroy].rcDes.bottom = Bmp[ButtonDestroy].rcDes.top + 35;
        Bmp[ButtonDestroy].Breite = static_cast<short>(Bmp[ButtonDestroy].rcSrc.right - Bmp[ButtonDestroy].rcSrc.left);
        Bmp[ButtonDestroy].Hoehe = static_cast<short>(Bmp[ButtonDestroy].rcSrc.bottom - Bmp[ButtonDestroy].rcSrc.top);
        Bmp[ButtonDestroy].Anzahl = 4;
        Bmp[ButtonDestroy].Geschwindigkeit = 5;

        // SpzAni
        for (i = Baum1Down; i <= Baum4Down; i++)
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
        Bmp[Baum1Down].rcSrc.left = 101;
        Bmp[Baum1Down].rcSrc.top = 0;
        Bmp[Baum1Down].rcSrc.right = 101 + 35;
        Bmp[Baum1Down].rcSrc.bottom = 27;
        Bmp[Baum1Down].Breite = static_cast<short>(Bmp[Baum1Down].rcSrc.right - Bmp[Baum1Down].rcSrc.left);
        Bmp[Baum1Down].Hoehe = static_cast<short>(Bmp[Baum1Down].rcSrc.bottom - Bmp[Baum1Down].rcSrc.top);

        // BAUM2DOWN
        Bmp[Baum2Down].rcSrc.left = 136;
        Bmp[Baum2Down].rcSrc.top = 0;
        Bmp[Baum2Down].rcSrc.right = 136 + 36;
        Bmp[Baum2Down].rcSrc.bottom = 27;
        Bmp[Baum2Down].Breite = static_cast<short>(Bmp[Baum2Down].rcSrc.right - Bmp[Baum2Down].rcSrc.left);
        Bmp[Baum2Down].Hoehe = static_cast<short>(Bmp[Baum2Down].rcSrc.bottom - Bmp[Baum2Down].rcSrc.top);

        // BAUM3DOWN
        Bmp[Baum3Down].rcSrc.left = 172;
        Bmp[Baum3Down].rcSrc.top = 0;
        Bmp[Baum3Down].rcSrc.right = 172 + 34;
        Bmp[Baum3Down].rcSrc.bottom = 28;
        Bmp[Baum3Down].Breite = static_cast<short>(Bmp[Baum3Down].rcSrc.right - Bmp[Baum3Down].rcSrc.left);
        Bmp[Baum3Down].Hoehe = static_cast<short>(Bmp[Baum3Down].rcSrc.bottom - Bmp[Baum3Down].rcSrc.top);

        // BAUM4DOWN
        Bmp[Baum4Down].rcSrc.left = 206;
        Bmp[Baum4Down].rcSrc.top = 0;
        Bmp[Baum4Down].rcSrc.right = 206 + 32;
        Bmp[Baum4Down].rcSrc.bottom = 17;
        Bmp[Baum4Down].Breite = static_cast<short>(Bmp[Baum4Down].rcSrc.right - Bmp[Baum4Down].rcSrc.left);
        Bmp[Baum4Down].Hoehe = static_cast<short>(Bmp[Baum4Down].rcSrc.bottom - Bmp[Baum4Down].rcSrc.top);

		// Sonstiges

        // Säule1
        Bmp[Pillar1].Anzahl = 1;
        Bmp[Pillar1].rcSrc.left = 205;
        Bmp[Pillar1].rcSrc.top = 115;
        Bmp[Pillar1].rcSrc.right = Bmp[Pillar1].rcSrc.left + 11;
        Bmp[Pillar1].rcSrc.bottom = Bmp[Pillar1].rcSrc.top + 115;
        Bmp[Pillar1].rcDes.left = rcPanel.left + 52;
        Bmp[Pillar1].rcDes.top = rcPanel.top + 471;
        Bmp[Pillar1].rcDes.right = Bmp[Pillar1].rcDes.left + 11;
        Bmp[Pillar1].rcDes.bottom = Bmp[Pillar1].rcDes.top + 115;
        Bmp[Pillar1].Breite = static_cast<short>(Bmp[Pillar1].rcSrc.right - Bmp[Pillar1].rcSrc.left);
        Bmp[Pillar1].Hoehe = static_cast<short>(Bmp[Pillar1].rcSrc.bottom - Bmp[Pillar1].rcSrc.top);
        Bmp[Pillar1].Surface = lpDDSPanel;

        // Säule2
        Bmp[Pillar2].Anzahl = 1;
        Bmp[Pillar2].rcSrc.left = 216;
        Bmp[Pillar2].rcSrc.top = 115;
        Bmp[Pillar2].rcSrc.right = Bmp[Pillar2].rcSrc.left + 11;
        Bmp[Pillar2].rcSrc.bottom = Bmp[Pillar2].rcSrc.top + 115;
        Bmp[Pillar2].rcDes.left = rcPanel.left + 114;
        Bmp[Pillar2].rcDes.top = rcPanel.top + 471;
        Bmp[Pillar2].rcDes.right = Bmp[Pillar2].rcDes.left + 11;
        Bmp[Pillar2].rcDes.bottom = Bmp[Pillar2].rcDes.top + 115;
        Bmp[Pillar2].Breite = static_cast<short>(Bmp[Pillar2].rcSrc.right - Bmp[Pillar2].rcSrc.left);
        Bmp[Pillar2].Hoehe = static_cast<short>(Bmp[Pillar2].rcSrc.bottom - Bmp[Pillar2].rcSrc.top);
        Bmp[Pillar2].Surface = lpDDSPanel;

        // Säule3
        Bmp[Pillar3].Anzahl = 1;
        Bmp[Pillar3].rcSrc.left = 227;
        Bmp[Pillar3].rcSrc.top = 115;
        Bmp[Pillar3].rcSrc.right = Bmp[Pillar3].rcSrc.left + 11;
        Bmp[Pillar3].rcSrc.bottom = Bmp[Pillar3].rcSrc.top + 115;
        Bmp[Pillar3].rcDes.left = rcPanel.left + 175;
        Bmp[Pillar3].rcDes.top = rcPanel.top + 471;
        Bmp[Pillar3].rcDes.right = Bmp[Pillar3].rcDes.left + 11;
        Bmp[Pillar3].rcDes.bottom = Bmp[Pillar3].rcDes.top + 115;
        Bmp[Pillar3].Breite = static_cast<short>(Bmp[Pillar3].rcSrc.right - Bmp[Pillar3].rcSrc.left);
        Bmp[Pillar3].Hoehe = static_cast<short>(Bmp[Pillar3].rcSrc.bottom - Bmp[Pillar3].rcSrc.top);
        Bmp[Pillar3].Surface = lpDDSPanel;

        // Rohstoffe
        for (i = ResourceBranch; i <= ResourceSlingshot; i++)
        {
            Bmp[i].Anzahl = 1;
            Bmp[i].rcSrc.left = 18 + (i - ResourceBranch) * 16;
            Bmp[i].rcSrc.top = 0;
            Bmp[i].rcSrc.right = Bmp[i].rcSrc.left + 16;
            Bmp[i].rcSrc.bottom = Bmp[i].rcSrc.top + 15;
            Bmp[i].Breite = static_cast<short>(Bmp[i].rcSrc.right - Bmp[i].rcSrc.left);
            Bmp[i].Hoehe = static_cast<short>(Bmp[i].rcSrc.bottom - Bmp[i].rcSrc.top);
            Bmp[i].Surface = lpDDSInventar;
        }
        // RohAst
        Bmp[ResourceBranch].rcDes.left = rcPanel.left + 34;
        Bmp[ResourceBranch].rcDes.top = rcPanel.top + 280;
        Bmp[ResourceBranch].rcDes.right = Bmp[ResourceBranch].rcDes.left + 16;
        Bmp[ResourceBranch].rcDes.bottom = Bmp[ResourceBranch].rcDes.top + 15;
        // ROHSTEIN
        Bmp[ResourceStone].rcDes.left = rcPanel.left + 34;
        Bmp[ResourceStone].rcDes.top = rcPanel.top + 300;
        Bmp[ResourceStone].rcDes.right = Bmp[ResourceStone].rcDes.left + 16;
        Bmp[ResourceStone].rcDes.bottom = Bmp[ResourceStone].rcDes.top + 15;
        // ROHSTAMM
        Bmp[ResourceTrunk].rcDes.left = rcPanel.left + 34;
        Bmp[ResourceTrunk].rcDes.top = rcPanel.top + 360;
        Bmp[ResourceTrunk].rcDes.right = Bmp[ResourceTrunk].rcDes.left + 16;
        Bmp[ResourceTrunk].rcDes.bottom = Bmp[ResourceTrunk].rcDes.top + 15;
        // ROHAXT
        Bmp[ResourceAxe].rcDes.left = rcPanel.left + 150;
        Bmp[ResourceAxe].rcDes.top = rcPanel.top + 280;
        Bmp[ResourceAxe].rcDes.right = Bmp[ResourceAxe].rcDes.left + 16;
        Bmp[ResourceAxe].rcDes.bottom = Bmp[ResourceAxe].rcDes.top + 15;
        // ROHBLATT
        Bmp[ResourceLeaf].rcDes.left = rcPanel.left + 34;
        Bmp[ResourceLeaf].rcDes.top = rcPanel.top + 320;
        Bmp[ResourceLeaf].rcDes.right = Bmp[ResourceLeaf].rcDes.left + 16;
        Bmp[ResourceLeaf].rcDes.bottom = Bmp[ResourceLeaf].rcDes.top + 15;
        // ROHEGGE
        Bmp[ResourceHarrow].rcDes.left = rcPanel.left + 150;
        Bmp[ResourceHarrow].rcDes.top = rcPanel.top + 300;
        Bmp[ResourceHarrow].rcDes.right = Bmp[ResourceHarrow].rcDes.left + 16;
        Bmp[ResourceHarrow].rcDes.bottom = Bmp[ResourceHarrow].rcDes.top + 15;
        // ROHLIANE
        Bmp[ResourceVine].rcDes.left = rcPanel.left + 34;
        Bmp[ResourceVine].rcDes.top = rcPanel.top + 340;
        Bmp[ResourceVine].rcDes.right = Bmp[ResourceVine].rcDes.left + 16;
        Bmp[ResourceVine].rcDes.bottom = Bmp[ResourceVine].rcDes.top + 15;
        // ROHANGEL
        Bmp[ResourceFishingRod].rcDes.left = rcPanel.left + 150;
        Bmp[ResourceFishingRod].rcDes.top = rcPanel.top + 320;
        Bmp[ResourceFishingRod].rcDes.right = Bmp[ResourceFishingRod].rcDes.left + 16;
        Bmp[ResourceFishingRod].rcDes.bottom = Bmp[ResourceFishingRod].rcDes.top + 15;
        // ROHHAMMER
        Bmp[ResourceHammer].rcDes.left = rcPanel.left + 150;
        Bmp[ResourceHammer].rcDes.top = rcPanel.top + 340;
        Bmp[ResourceHammer].rcDes.right = Bmp[ResourceHammer].rcDes.left + 16;
        Bmp[ResourceHammer].rcDes.bottom = Bmp[ResourceHammer].rcDes.top + 15;
        // ROHFERNROHR
        Bmp[ResourceTelescope].rcDes.left = rcPanel.left + 150;
        Bmp[ResourceTelescope].rcDes.top = rcPanel.top + 360;
        Bmp[ResourceTelescope].rcDes.right = Bmp[ResourceTelescope].rcDes.left + 16;
        Bmp[ResourceTelescope].rcDes.bottom = Bmp[ResourceTelescope].rcDes.top + 15;
        // ROHSTREICHHOLZ
        Bmp[ResourceMatchstick].rcDes.left = rcPanel.left + 105;
        Bmp[ResourceMatchstick].rcDes.top = rcPanel.top + 280;
        Bmp[ResourceMatchstick].rcDes.right = Bmp[ResourceMatchstick].rcDes.left + 16;
        Bmp[ResourceMatchstick].rcDes.bottom = Bmp[ResourceMatchstick].rcDes.top + 15;
        // ROHSCHAUFEL
        Bmp[ResourceShovel].rcDes.left = rcPanel.left + 105;
        Bmp[ResourceShovel].rcDes.top = rcPanel.top + 320;
        Bmp[ResourceShovel].rcDes.right = Bmp[ResourceShovel].rcDes.left + 16;
        Bmp[ResourceShovel].rcDes.bottom = Bmp[ResourceShovel].rcDes.top + 15;
        // ROHKARTE
        Bmp[ResourceMap].rcDes.left = rcPanel.left + 105;
        Bmp[ResourceMap].rcDes.top = rcPanel.top + 320;
        Bmp[ResourceMap].rcDes.right = Bmp[ResourceMap].rcDes.left + 16;
        Bmp[ResourceMap].rcDes.bottom = Bmp[ResourceMap].rcDes.top + 15;
        // ROHSCHLEUDER
        Bmp[ResourceSlingshot].rcDes.left = rcPanel.left + 105;
        Bmp[ResourceSlingshot].rcDes.top = rcPanel.top + 340;
        Bmp[ResourceSlingshot].rcDes.right = Bmp[ResourceSlingshot].rcDes.left + 16;
        Bmp[ResourceSlingshot].rcDes.bottom = Bmp[ResourceSlingshot].rcDes.top + 15;

        // ROEMISCH1
        Bmp[Roman1].Anzahl = 1;
        Bmp[Roman1].rcSrc.left = 0;
        Bmp[Roman1].rcSrc.top = 0;
        Bmp[Roman1].rcSrc.right = Bmp[Roman1].rcSrc.left + 3;
        Bmp[Roman1].rcSrc.bottom = Bmp[Roman1].rcSrc.top + 13;
        Bmp[Roman1].rcDes.left = rcPanel.left + 50;
        Bmp[Roman1].rcDes.top = rcPanel.top;
        Bmp[Roman1].rcDes.right = Bmp[Roman1].rcDes.left + 3;
        Bmp[Roman1].rcDes.bottom = Bmp[Roman1].rcDes.top + 13;
        Bmp[Roman1].Breite = static_cast<short>(Bmp[Roman1].rcSrc.right - Bmp[Roman1].rcSrc.left);
        Bmp[Roman1].Hoehe = static_cast<short>(Bmp[Roman1].rcSrc.bottom - Bmp[Roman1].rcSrc.top);
        Bmp[Roman1].Surface = lpDDSInventar;

        // ROEMISCH2
        Bmp[Roman2].Anzahl = 1;
        Bmp[Roman2].rcSrc.left = 3;
        Bmp[Roman2].rcSrc.top = 0;
        Bmp[Roman2].rcSrc.right = Bmp[Roman2].rcSrc.left + 15;
        Bmp[Roman2].rcSrc.bottom = Bmp[Roman2].rcSrc.top + 10;
        Bmp[Roman2].rcDes.left = rcPanel.left + 50;
        Bmp[Roman2].rcDes.top = rcPanel.top;
        Bmp[Roman2].rcDes.right = Bmp[Roman2].rcDes.left + 15;
        Bmp[Roman2].rcDes.bottom = Bmp[Roman2].rcDes.top + 10;
        Bmp[Roman2].Breite = static_cast<short>(Bmp[Roman2].rcSrc.right - Bmp[Roman2].rcSrc.left);
        Bmp[Roman2].Hoehe = static_cast<short>(Bmp[Roman2].rcSrc.bottom - Bmp[Roman2].rcSrc.top);
        Bmp[Roman2].Surface = lpDDSInventar;

        // INVPAPIER
        Bmp[InventoryPaper].Anzahl = 1;
        Bmp[InventoryPaper].rcSrc.left = 0;
        Bmp[InventoryPaper].rcSrc.top = 15;
        Bmp[InventoryPaper].rcSrc.right = Bmp[InventoryPaper].rcSrc.left + 178;
        Bmp[InventoryPaper].rcSrc.bottom = Bmp[InventoryPaper].rcSrc.top + 114;
        Bmp[InventoryPaper].rcDes.left = rcPanel.left + 15;
        Bmp[InventoryPaper].rcDes.top = rcPanel.top + 270;
        Bmp[InventoryPaper].rcDes.right = Bmp[InventoryPaper].rcDes.left + 178;
        Bmp[InventoryPaper].rcDes.bottom = Bmp[InventoryPaper].rcDes.top + 114;
        Bmp[InventoryPaper].Breite = static_cast<short>(Bmp[InventoryPaper].rcSrc.right - Bmp[InventoryPaper].rcSrc.left);
        Bmp[InventoryPaper].Hoehe = static_cast<short>(Bmp[InventoryPaper].rcSrc.bottom - Bmp[InventoryPaper].rcSrc.top);
        Bmp[InventoryPaper].Surface = lpDDSInventar;

        // RING
        Bmp[Ring].Anzahl = 1;
        Bmp[Ring].rcSrc.left = 205;
        Bmp[Ring].rcSrc.top = 230;
        Bmp[Ring].rcSrc.right = Bmp[Ring].rcSrc.left + 37;
        Bmp[Ring].rcSrc.bottom = Bmp[Ring].rcSrc.top + 150;
        Bmp[Ring].rcDes.left = rcPanel.left + 5;
        Bmp[Ring].rcDes.top = rcPanel.top - 113;
        Bmp[Ring].rcDes.right = Bmp[Ring].rcDes.left + 30;
        Bmp[Ring].rcDes.bottom = Bmp[Ring].rcDes.top;
        Bmp[Ring].Breite = static_cast<short>(Bmp[Ring].rcSrc.right - Bmp[Ring].rcSrc.left);
        Bmp[Ring].Hoehe = static_cast<short>(Bmp[Ring].rcSrc.bottom - Bmp[Ring].rcSrc.top);
        Bmp[Ring].Surface = lpDDSPanel;

        // KREUZ
        Bmp[Cross].Anzahl = 1;
        Bmp[Cross].rcSrc.left = 205;
        Bmp[Cross].rcSrc.top = 380;
        Bmp[Cross].rcSrc.right = Bmp[Cross].rcSrc.left + 40;
        Bmp[Cross].rcSrc.bottom = Bmp[Cross].rcSrc.top + 22;
        Bmp[Cross].Breite = static_cast<short>(Bmp[Cross].rcSrc.right - Bmp[Cross].rcSrc.left);
        Bmp[Cross].Hoehe = static_cast<short>(Bmp[Cross].rcSrc.bottom - Bmp[Cross].rcSrc.top);
        Bmp[Cross].Surface = lpDDSPanel;

        // JA
        Bmp[Yes].Anzahl = 1;
        Bmp[Yes].rcSrc.left = 0;
        Bmp[Yes].rcSrc.top = 154;
        Bmp[Yes].rcSrc.right = Bmp[Yes].rcSrc.left + 41;
        Bmp[Yes].rcSrc.bottom = Bmp[Yes].rcSrc.top + 42;
        Bmp[Yes].Breite = static_cast<short>(Bmp[Yes].rcSrc.right - Bmp[Yes].rcSrc.left);
        Bmp[Yes].Hoehe = static_cast<short>(Bmp[Yes].rcSrc.bottom - Bmp[Yes].rcSrc.top);
        Bmp[Yes].Surface = lpDDSPapier;

        // NEIN
        Bmp[No].Anzahl = 1;
        Bmp[No].rcSrc.left = 41;
        Bmp[No].rcSrc.top = 154;
        Bmp[No].rcSrc.right = Bmp[No].rcSrc.left + 100;
        Bmp[No].rcSrc.bottom = Bmp[No].rcSrc.top + 39;
        Bmp[No].Breite = static_cast<short>(Bmp[No].rcSrc.right - Bmp[No].rcSrc.left);
        Bmp[No].Hoehe = static_cast<short>(Bmp[No].rcSrc.bottom - Bmp[No].rcSrc.top);
        Bmp[No].Surface = lpDDSPapier;

        // Sonne
        Bmp[Sun].Anzahl = 1;
        Bmp[Sun].rcSrc.left = 205;
        Bmp[Sun].rcSrc.top = 65;
        Bmp[Sun].rcSrc.right = Bmp[Sun].rcSrc.left + 51;
        Bmp[Sun].rcSrc.bottom = Bmp[Sun].rcSrc.top + 50;
        Bmp[Sun].rcDes.left = rcPanel.left + 30;
        Bmp[Sun].rcDes.top = rcPanel.top + 630;
        Bmp[Sun].rcDes.right = Bmp[Sun].rcDes.left + 152;
        Bmp[Sun].rcDes.bottom = Bmp[Sun].rcDes.top + 55;
        Bmp[Sun].Breite = static_cast<short>(Bmp[Sun].rcSrc.right - Bmp[Sun].rcSrc.left);
        Bmp[Sun].Hoehe = static_cast<short>(Bmp[Sun].rcSrc.bottom - Bmp[Sun].rcSrc.top);
        Bmp[Sun].Surface = lpDDSPanel;

        // PROGRAMMIERUNG
        Bmp[Programming].Anzahl = 1;
        Bmp[Programming].rcSrc.left = 0;
        Bmp[Programming].rcSrc.top = 0;
        Bmp[Programming].rcSrc.right = Bmp[Programming].rcSrc.left + 348;
        Bmp[Programming].rcSrc.bottom = Bmp[Programming].rcSrc.top + 49;
        Bmp[Programming].Breite = static_cast<short>(Bmp[Programming].rcSrc.right - Bmp[Programming].rcSrc.left);
        Bmp[Programming].Hoehe = static_cast<short>(Bmp[Programming].rcSrc.bottom - Bmp[Programming].rcSrc.top);
        Bmp[Programming].rcDes.left = MAXX / 2 - Bmp[Programming].Breite / 2;
        Bmp[Programming].rcDes.top = MAXY - Bmp[Programming].Hoehe / 2;
        Bmp[Programming].rcDes.right = Bmp[Programming].rcDes.left + Bmp[Programming].Breite;
        Bmp[Programming].rcDes.bottom = Bmp[Programming].rcDes.top + Bmp[Programming].Hoehe;
        Bmp[Programming].Surface = lpDDSCredits;

        // DIRKPLATE
        Bmp[DirkPlate].Anzahl = 1;
        Bmp[DirkPlate].rcSrc.left = 0;
        Bmp[DirkPlate].rcSrc.top = 49;
        Bmp[DirkPlate].rcSrc.right = Bmp[DirkPlate].rcSrc.left + 196;
        Bmp[DirkPlate].rcSrc.bottom = Bmp[DirkPlate].rcSrc.top + 47;
        Bmp[DirkPlate].Breite = static_cast<short>(Bmp[DirkPlate].rcSrc.right - Bmp[DirkPlate].rcSrc.left);
        Bmp[DirkPlate].Hoehe = static_cast<short>(Bmp[DirkPlate].rcSrc.bottom - Bmp[DirkPlate].rcSrc.top);
        Bmp[DirkPlate].rcDes.left = MAXX / 2 - Bmp[DirkPlate].Breite / 2;
        Bmp[DirkPlate].rcDes.top = MAXY - Bmp[DirkPlate].Hoehe / 2;
        Bmp[DirkPlate].rcDes.right = Bmp[DirkPlate].rcDes.left + Bmp[DirkPlate].Breite;
        Bmp[DirkPlate].rcDes.bottom = Bmp[DirkPlate].rcDes.top + Bmp[DirkPlate].Hoehe;
        Bmp[DirkPlate].Surface = lpDDSCredits;

        // MATTHIAS
        Bmp[Matthias].Anzahl = 1;
        Bmp[Matthias].rcSrc.left = 0;
        Bmp[Matthias].rcSrc.top = 96;
        Bmp[Matthias].rcSrc.right = Bmp[Matthias].rcSrc.left + 379;
        Bmp[Matthias].rcSrc.bottom = Bmp[Matthias].rcSrc.top + 47;
        Bmp[Matthias].Breite = static_cast<short>(Bmp[Matthias].rcSrc.right - Bmp[Matthias].rcSrc.left);
        Bmp[Matthias].Hoehe = static_cast<short>(Bmp[Matthias].rcSrc.bottom - Bmp[Matthias].rcSrc.top);
        Bmp[Matthias].rcDes.left = MAXX / 2 - Bmp[Matthias].Breite / 2;
        Bmp[Matthias].rcDes.top = MAXY - Bmp[Matthias].Hoehe / 2;
        Bmp[Matthias].rcDes.right = Bmp[Matthias].rcDes.left + Bmp[Matthias].Breite;
        Bmp[Matthias].rcDes.bottom = Bmp[Matthias].rcDes.top + Bmp[Matthias].Hoehe;
        Bmp[Matthias].Surface = lpDDSCredits;

        // TESTSPIELER
        Bmp[TestPlayers].Anzahl = 1;
        Bmp[TestPlayers].rcSrc.left = 0;
        Bmp[TestPlayers].rcSrc.top = 143;
        Bmp[TestPlayers].rcSrc.right = Bmp[TestPlayers].rcSrc.left + 210;
        Bmp[TestPlayers].rcSrc.bottom = Bmp[TestPlayers].rcSrc.top + 55;
        Bmp[TestPlayers].Breite = static_cast<short>(Bmp[TestPlayers].rcSrc.right - Bmp[TestPlayers].rcSrc.left);
        Bmp[TestPlayers].Hoehe = static_cast<short>(Bmp[TestPlayers].rcSrc.bottom - Bmp[TestPlayers].rcSrc.top);
        Bmp[TestPlayers].rcDes.left = MAXX / 2 - Bmp[TestPlayers].Breite / 2;
        Bmp[TestPlayers].rcDes.top = MAXY - Bmp[TestPlayers].Hoehe / 2;
        Bmp[TestPlayers].rcDes.right = Bmp[TestPlayers].rcDes.left + Bmp[TestPlayers].Breite;
        Bmp[TestPlayers].rcDes.bottom = Bmp[TestPlayers].rcDes.top + Bmp[TestPlayers].Hoehe;
        Bmp[TestPlayers].Surface = lpDDSCredits;

        // TOBIAS
        Bmp[Tobias].Anzahl = 1;
        Bmp[Tobias].rcSrc.left = 0;
        Bmp[Tobias].rcSrc.top = 198;
        Bmp[Tobias].rcSrc.right = Bmp[Tobias].rcSrc.left + 273;
        Bmp[Tobias].rcSrc.bottom = Bmp[Tobias].rcSrc.top + 56;
        Bmp[Tobias].Breite = static_cast<short>(Bmp[Tobias].rcSrc.right - Bmp[Tobias].rcSrc.left);
        Bmp[Tobias].Hoehe = static_cast<short>(Bmp[Tobias].rcSrc.bottom - Bmp[Tobias].rcSrc.top);
        Bmp[Tobias].rcDes.left = MAXX / 2 - Bmp[Tobias].Breite / 2;
        Bmp[Tobias].rcDes.top = MAXY - Bmp[Tobias].Hoehe / 2;
        Bmp[Tobias].rcDes.right = Bmp[Tobias].rcDes.left + Bmp[Tobias].Breite;
        Bmp[Tobias].rcDes.bottom = Bmp[Tobias].rcDes.top + Bmp[Tobias].Hoehe;
        Bmp[Tobias].Surface = lpDDSCredits;

        // SIGRID
        Bmp[Sigrid].Anzahl = 1;
        Bmp[Sigrid].rcSrc.left = 0;
        Bmp[Sigrid].rcSrc.top = 254;
        Bmp[Sigrid].rcSrc.right = Bmp[Sigrid].rcSrc.left + 226;
        Bmp[Sigrid].rcSrc.bottom = Bmp[Sigrid].rcSrc.top + 56;
        Bmp[Sigrid].Breite = static_cast<short>(Bmp[Sigrid].rcSrc.right - Bmp[Sigrid].rcSrc.left);
        Bmp[Sigrid].Hoehe = static_cast<short>(Bmp[Sigrid].rcSrc.bottom - Bmp[Sigrid].rcSrc.top);
        Bmp[Sigrid].rcDes.left = MAXX / 2 - Bmp[Sigrid].Breite / 2;
        Bmp[Sigrid].rcDes.top = MAXY - Bmp[Sigrid].Hoehe / 2;
        Bmp[Sigrid].rcDes.right = Bmp[Sigrid].rcDes.left + Bmp[Sigrid].Breite;
        Bmp[Sigrid].rcDes.bottom = Bmp[Sigrid].rcDes.top + Bmp[Sigrid].Hoehe;
        Bmp[Sigrid].Surface = lpDDSCredits;

        // PATHFINDING
        Bmp[PathFinding].Anzahl = 1;
        Bmp[PathFinding].rcSrc.left = 0;
        Bmp[PathFinding].rcSrc.top = 353;
        Bmp[PathFinding].rcSrc.right = Bmp[PathFinding].rcSrc.left + 233;
        Bmp[PathFinding].rcSrc.bottom = Bmp[PathFinding].rcSrc.top + 59;
        Bmp[PathFinding].Breite = static_cast<short>(Bmp[PathFinding].rcSrc.right - Bmp[PathFinding].rcSrc.left);
        Bmp[PathFinding].Hoehe = static_cast<short>(Bmp[PathFinding].rcSrc.bottom - Bmp[PathFinding].rcSrc.top);
        Bmp[PathFinding].rcDes.left = MAXX / 2 - Bmp[PathFinding].Breite / 2;
        Bmp[PathFinding].rcDes.top = MAXY - Bmp[PathFinding].Hoehe / 2;
        Bmp[PathFinding].rcDes.right = Bmp[PathFinding].rcDes.left + Bmp[PathFinding].Breite;
        Bmp[PathFinding].rcDes.bottom = Bmp[PathFinding].rcDes.top + Bmp[PathFinding].Hoehe;
        Bmp[PathFinding].Surface = lpDDSCredits;

        // JOHN
        Bmp[John].Anzahl = 1;
        Bmp[John].rcSrc.left = 0;
        Bmp[John].rcSrc.top = 412;
        Bmp[John].rcSrc.right = Bmp[John].rcSrc.left + 347;
        Bmp[John].rcSrc.bottom = Bmp[John].rcSrc.top + 56;
        Bmp[John].Breite = static_cast<short>(Bmp[John].rcSrc.right - Bmp[John].rcSrc.left);
        Bmp[John].Hoehe = static_cast<short>(Bmp[John].rcSrc.bottom - Bmp[John].rcSrc.top);
        Bmp[John].rcDes.left = MAXX / 2 - Bmp[John].Breite / 2;
        Bmp[John].rcDes.top = MAXY - Bmp[John].Hoehe / 2;
        Bmp[John].rcDes.right = Bmp[John].rcDes.left + Bmp[John].Breite;
        Bmp[John].rcDes.bottom = Bmp[John].rcDes.top + Bmp[John].Hoehe;
        Bmp[John].Surface = lpDDSCredits;

        // HEIKO
        Bmp[Heiko].Anzahl = 1;
        Bmp[Heiko].rcSrc.left = 0;
        Bmp[Heiko].rcSrc.top = 468;
        Bmp[Heiko].rcSrc.right = Bmp[Heiko].rcSrc.left + 236;
        Bmp[Heiko].rcSrc.bottom = Bmp[Heiko].rcSrc.top + 47;
        Bmp[Heiko].Breite = static_cast<short>(Bmp[Heiko].rcSrc.right - Bmp[Heiko].rcSrc.left);
        Bmp[Heiko].Hoehe = static_cast<short>(Bmp[Heiko].rcSrc.bottom - Bmp[Heiko].rcSrc.top);
        Bmp[Heiko].rcDes.left = MAXX / 2 - Bmp[Heiko].Breite / 2;
        Bmp[Heiko].rcDes.top = MAXY - Bmp[Heiko].Hoehe / 2;
        Bmp[Heiko].rcDes.right = Bmp[Heiko].rcDes.left + Bmp[Heiko].Breite;
        Bmp[Heiko].rcDes.bottom = Bmp[Heiko].rcDes.top + Bmp[Heiko].Hoehe;
        Bmp[Heiko].Surface = lpDDSCredits;

        // GISELA
        Bmp[Gisela].Anzahl = 1;
        Bmp[Gisela].rcSrc.left = 0;
        Bmp[Gisela].rcSrc.top = 515;
        Bmp[Gisela].rcSrc.right = Bmp[Gisela].rcSrc.left + 232;
        Bmp[Gisela].rcSrc.bottom = Bmp[Gisela].rcSrc.top + 47;
        Bmp[Gisela].Breite = static_cast<short>(Bmp[Gisela].rcSrc.right - Bmp[Gisela].rcSrc.left);
        Bmp[Gisela].Hoehe = static_cast<short>(Bmp[Gisela].rcSrc.bottom - Bmp[Gisela].rcSrc.top);
        Bmp[Gisela].rcDes.left = MAXX / 2 - Bmp[Gisela].Breite / 2;
        Bmp[Gisela].rcDes.top = MAXY - Bmp[Gisela].Hoehe / 2;
        Bmp[Gisela].rcDes.right = Bmp[Gisela].rcDes.left + Bmp[Gisela].Breite;
        Bmp[Gisela].rcDes.bottom = Bmp[Gisela].rcDes.top + Bmp[Gisela].Hoehe;
        Bmp[Gisela].Surface = lpDDSCredits;

        // WEITEREHILFE
        Bmp[AdditionalHelp].Anzahl = 1;
        Bmp[AdditionalHelp].rcSrc.left = 0;
        Bmp[AdditionalHelp].rcSrc.top = 562;
        Bmp[AdditionalHelp].rcSrc.right = Bmp[AdditionalHelp].rcSrc.left + 258;
        Bmp[AdditionalHelp].rcSrc.bottom = Bmp[AdditionalHelp].rcSrc.top + 46;
        Bmp[AdditionalHelp].Breite = static_cast<short>(Bmp[AdditionalHelp].rcSrc.right - Bmp[AdditionalHelp].rcSrc.left);
        Bmp[AdditionalHelp].Hoehe = static_cast<short>(Bmp[AdditionalHelp].rcSrc.bottom - Bmp[AdditionalHelp].rcSrc.top);
        Bmp[AdditionalHelp].rcDes.left = MAXX / 2 - Bmp[AdditionalHelp].Breite / 2;
        Bmp[AdditionalHelp].rcDes.top = MAXY - Bmp[AdditionalHelp].Hoehe / 2;
        Bmp[AdditionalHelp].rcDes.right = Bmp[AdditionalHelp].rcDes.left + Bmp[AdditionalHelp].Breite;
        Bmp[AdditionalHelp].rcDes.bottom = Bmp[AdditionalHelp].rcDes.top + Bmp[AdditionalHelp].Hoehe;
        Bmp[AdditionalHelp].Surface = lpDDSCredits;

        // DPSOFTWARE
        Bmp[DpSoftware].Anzahl = 1;
        Bmp[DpSoftware].rcSrc.left = 0;
        Bmp[DpSoftware].rcSrc.top = 608;
        Bmp[DpSoftware].rcSrc.right = Bmp[DpSoftware].rcSrc.left + 291;
        Bmp[DpSoftware].rcSrc.bottom = Bmp[DpSoftware].rcSrc.top + 99;
        Bmp[DpSoftware].Breite = static_cast<short>(Bmp[DpSoftware].rcSrc.right - Bmp[DpSoftware].rcSrc.left);
        Bmp[DpSoftware].Hoehe = static_cast<short>(Bmp[DpSoftware].rcSrc.bottom - Bmp[DpSoftware].rcSrc.top);
        Bmp[DpSoftware].rcDes.left = MAXX / 2 - Bmp[DpSoftware].Breite / 2;
        Bmp[DpSoftware].rcDes.top = MAXY - Bmp[DpSoftware].Hoehe / 2;
        Bmp[DpSoftware].rcDes.right = Bmp[DpSoftware].rcDes.left + Bmp[DpSoftware].Breite;
        Bmp[DpSoftware].rcDes.bottom = Bmp[DpSoftware].rcDes.top + Bmp[DpSoftware].Hoehe;
        Bmp[DpSoftware].Surface = lpDDSCredits;

        // SCHWARZ
        Bmp[Black].Anzahl = 1;
        Bmp[Black].rcSrc.left = 0;
        Bmp[Black].rcSrc.top = 608;
        Bmp[Black].rcSrc.right = Bmp[Black].rcSrc.left + 1;
        Bmp[Black].rcSrc.bottom = Bmp[Black].rcSrc.top + 1;
        Bmp[Black].Breite = static_cast<short>(Bmp[Black].rcSrc.right - Bmp[Black].rcSrc.left);
        Bmp[Black].Hoehe = static_cast<short>(Bmp[Black].rcSrc.bottom - Bmp[Black].rcSrc.top);
        Bmp[Black].rcDes.left = MAXX / 2 - Bmp[Black].Breite / 2;
        Bmp[Black].rcDes.top = MAXY - Bmp[Black].Hoehe / 2;
        Bmp[Black].rcDes.right = Bmp[Black].rcDes.left + Bmp[Black].Breite;
        Bmp[Black].rcDes.bottom = Bmp[Black].rcDes.top + Bmp[Black].Hoehe;
        Bmp[Black].Surface = lpDDSCredits;

        // SOUNDS
        Bmp[Sounds].Anzahl = 1;
        Bmp[Sounds].rcSrc.left = 0;
        Bmp[Sounds].rcSrc.top = 310;
        Bmp[Sounds].rcSrc.right = Bmp[Sounds].rcSrc.left + 144;
        Bmp[Sounds].rcSrc.bottom = Bmp[Sounds].rcSrc.top + 43;
        Bmp[Sounds].Breite = static_cast<short>(Bmp[Sounds].rcSrc.right - Bmp[Sounds].rcSrc.left);
        Bmp[Sounds].Hoehe = static_cast<short>(Bmp[Sounds].rcSrc.bottom - Bmp[Sounds].rcSrc.top);
        Bmp[Sounds].rcDes.left = MAXX / 2 - Bmp[Sounds].Breite / 2;
        Bmp[Sounds].rcDes.top = MAXY - Bmp[Sounds].Hoehe / 2;
        Bmp[Sounds].rcDes.right = Bmp[Sounds].rcDes.left + Bmp[Sounds].Breite;
        Bmp[Sounds].rcDes.bottom = Bmp[Sounds].rcDes.top + Bmp[Sounds].Hoehe;
        Bmp[Sounds].Surface = lpDDSCredits;

        // MUSIK
        Bmp[Music].Anzahl = 1;
        Bmp[Music].rcSrc.left = 160;
        Bmp[Music].rcSrc.top = 310;
        Bmp[Music].rcSrc.right = Bmp[Music].rcSrc.left + 124;
        Bmp[Music].rcSrc.bottom = Bmp[Music].rcSrc.top + 39;
        Bmp[Music].Breite = static_cast<short>(Bmp[Music].rcSrc.right - Bmp[Music].rcSrc.left);
        Bmp[Music].Hoehe = static_cast<short>(Bmp[Music].rcSrc.bottom - Bmp[Music].rcSrc.top);
        Bmp[Music].rcDes.left = MAXX / 2 - Bmp[Music].Breite / 2;
        Bmp[Music].rcDes.top = MAXY - Bmp[Music].Hoehe / 2;
        Bmp[Music].rcDes.right = Bmp[Music].rcDes.left + Bmp[Music].Breite;
        Bmp[Music].rcDes.bottom = Bmp[Music].rcDes.top + Bmp[Music].Hoehe;
        Bmp[Music].Surface = lpDDSCredits;

        for (i = 0; i < 10; i++)
        {
            for (k = 0; k < 10; k++)
            {
                AbspannListe[i][k].Aktiv = false;
                AbspannListe[i][k].Bild = -1;
            }
        }

        AbspannListe[0][0].Bild = Programming;
        AbspannListe[0][1].Aktiv = true; // nur den hier true setzen, löst dann alles andere aus
        AbspannListe[0][1].Bild = DirkPlate;
        AbspannListe[1][0].Bild = Music;
        AbspannListe[1][1].Bild = Heiko;
        AbspannListe[2][0].Bild = Sounds;
        AbspannListe[2][1].Bild = DirkPlate;
        AbspannListe[3][0].Bild = TestPlayers;
        AbspannListe[3][1].Bild = Matthias;
        AbspannListe[3][2].Bild = Tobias;
        AbspannListe[3][3].Bild = Sigrid;
        AbspannListe[4][0].Bild = PathFinding;
        AbspannListe[4][1].Bild = John;
        AbspannListe[5][0].Bild = AdditionalHelp;
        AbspannListe[5][1].Bild = Heiko;
        AbspannListe[5][2].Bild = Gisela;
        AbspannListe[6][0].Bild = Black;
        AbspannListe[6][1].Bild = DpSoftware;

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
        TextBereich[TXTCHANCE].rcText.left = Bmp[Ring].rcDes.left + 5;
        TextBereich[TXTCHANCE].rcText.top = Bmp[Ring].rcDes.top + Bmp[Ring].Hoehe + 10;
        TextBereich[TXTCHANCE].rcText.right = TextBereich[TXTCHANCE].rcText.left + 3 * S2XPIXEL;
        TextBereich[TXTCHANCE].rcText.bottom = TextBereich[TXTCHANCE].rcText.top + S2YPIXEL;

		Guy.Resource[WASSER] = 100;
		Guy.Resource[NAHRUNG] = 100;
		Guy.Resource[GESUNDHEIT] = 100;

        for (i = ResourceBranch; i <= ResourceSlingshot; i++)
        {
            Guy.Inventar[i] = 0;
        }

        CursorTyp = CuPfeil;
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
        Direct::Reset();
        SchatzPos.x = -1;
        SchatzPos.y = -1;
        SchatzGef = false;
    }

	void InitializeWaves()
	{
		// Sounds
		for (auto& sound : Wav)
		{
			sound.Dateiname = "sounds/klick.wav";
			sound.Loop = false;
			sound.Volume = 100;
		}

        Wav[Sound::Storm].Dateiname = "sounds/sturm.wav";
        Wav[Sound::Storm].Volume = 85;

        Wav[Sound::Swim].Dateiname = "sounds/schwimmen.wav";
        Wav[Sound::Swim].Volume = 90;

        Wav[Sound::Splat].Dateiname = "sounds/platsch.wav";
        Wav[Sound::Splat].Volume = 95;

        Wav[Sound::Log].Dateiname = "sounds/faellen.wav";
        Wav[Sound::Log].Volume = 100;

        Wav[Sound::Hit].Dateiname = "sounds/schlagen.wav";
        Wav[Sound::Hit].Volume = 100;

        Wav[Sound::Slingshot].Dateiname = "sounds/schleuder.wav";
        Wav[Sound::Slingshot].Volume = 100;

        Wav[Sound::Dig].Dateiname = "sounds/schaufeln.wav";
        Wav[Sound::Dig].Volume = 90;

        Wav[Sound::Hammer].Dateiname = "sounds/hammer.wav";
        Wav[Sound::Hammer].Volume = 100;

        Wav[Sound::Crash].Dateiname = "sounds/crash.wav";
        Wav[Sound::Crash].Volume = 100;

        Wav[Sound::Snore].Dateiname = "sounds/schnarchen.wav";
        Wav[Sound::Snore].Volume = 90;

        Wav[Sound::Drink].Dateiname = "sounds/trinken.wav";
        Wav[Sound::Drink].Volume = 95;

        Wav[Sound::Crackle].Dateiname = "sounds/knistern.wav";
        Wav[Sound::Crackle].Volume = 90;

        Wav[Sound::Fish].Dateiname = "sounds/angel.wav";
        Wav[Sound::Fish].Volume = 100;

        Wav[Sound::Forest].Dateiname = "sounds/wald.wav";
        Wav[Sound::Forest].Volume = 90;

        Wav[Sound::Fire].Dateiname = "sounds/feuer.wav";
        Wav[Sound::Fire].Volume = 100;

        Wav[Sound::Surf].Dateiname = "sounds/brandung.wav";
        Wav[Sound::Surf].Volume = 100;

        Wav[Sound::Timber].Dateiname = "sounds/baumfaellt.wav";
        Wav[Sound::Timber].Volume = 100;

        Wav[Sound::River].Dateiname = "sounds/fluss.wav";
        Wav[Sound::River].Volume = 85;

        Wav[Sound::Click].Dateiname = "sounds/klick.wav";
        Wav[Sound::Click].Volume = 95;

        Wav[Sound::Click2].Dateiname = "sounds/klick2.wav";
        Wav[Sound::Click2].Volume = 95;

        Wav[Sound::Outro].Dateiname = "sounds/abspann.wav";
        Wav[Sound::Outro].Volume = 100;
        Wav[Sound::Outro].Loop = true;

        Wav[Sound::Logo].Dateiname = "sounds/logo.wav";
        Wav[Sound::Logo].Volume = 100;
        Wav[Sound::Logo].Loop = true;

        Wav[Sound::Wolf].Dateiname = "sounds/wolf.wav";
        Wav[Sound::Wolf].Volume = 90;

        Wav[Sound::Invention].Dateiname = "sounds/erfindung.wav";
        Wav[Sound::Invention].Volume = 95;

		// Testweise alle Sounds gleich in den Speicher laden
		for (short i = 1; i < Sound::Count; i++)
		{
			Sound::LoadSound(i);
		}
	}

	void NewGame(bool first)
	{
		short x, y;
		auto loadOk = false;

		InitializeStructs();
		InitializeWaves();

		if (!first)
		{
			loadOk = LoadGame();
		}

		if (!loadOk || first)
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
            World::Ocean();

            Renderer::DrawString("Lege Fluss fest...", 5, 65, 2);
            rcRectdes.left = 0;
            rcRectdes.top = 0;
            rcRectdes.right = MAXX;
            rcRectdes.bottom = MAXY;
            lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
            World::River();
            Math::CalculateTileCoordinates();

            Renderer::DrawString("Pflanze Baeume...", 5, 95, 2);
            rcRectdes.left = 0;
            rcRectdes.top = 0;
            rcRectdes.right = MAXX;
            rcRectdes.bottom = MAXY;
            lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
            World::Trees(30);

            World::PirateWreck();

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
			if (!BootsFahrt)
			{
				World::ChangeBoatRide();
			}

			Tag = 1;
			Stunden = 0;
			Minuten = 0;

            Spielzustand = State::Intro;
            Guy.Aktiv = false;
            Guy.Zustand = GuySchiff;
            Guy.AkNummer = 0;
            Guy.Aktion = static_cast<short>(Action::Actions::Intro);
        }

		// SchriftSurface löschen
		rcRectdes.left = 0;
		rcRectdes.top = 0;
		rcRectdes.right = MAXX;
		rcRectdes.bottom = MAXY;
		ddbltfx.dwFillColor = Renderer::RGB2DWORD(255, 0, 255);
		lpDDSSchrift->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

		const auto animationTemp = LAnimation;
		bool discoverTemp[MAXXKACH][MAXYKACH];

		LAnimation = false;
		// Schatzvergraben und Schatzkarte malen
		for (y = 0; y < MAXYKACH; y++)
		{
			for (x = 0; x < MAXXKACH; x++)
			{
				discoverTemp[x][y] = Scape[x][y].Entdeckt;
				Scape[x][y].Entdeckt = true;
			}
		}

        World::Generate(); // Einmal vor dem Schatz schon entdeckt malen
        World::Treasure();

		for (y = 0; y < MAXYKACH; y++)
		{
			for (x = 0; x < MAXXKACH; x++)
			{
				Scape[x][y].Entdeckt = discoverTemp[x][y];
			}
		}

		World::Discover();
		LAnimation = animationTemp;
		World::Generate(); // Und nochmal ohne das die Gegend entdeckt ist
		Guy.PosAlt = Guy.PosScreen;
	}
} // namespace Game
