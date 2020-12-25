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
        std::ofstream ofs("save.dat", std::ios::binary);
        if (!ofs) return;

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

        for (short i = 0; i < SPRITE_COUNT; i++)
        {
            ofs.write(reinterpret_cast<char*>(&Bmp[i].Animation), sizeof(Bmp[i].Animation));
            ofs.write(reinterpret_cast<char*>(&Bmp[i].Phase), sizeof(Bmp[i].Phase));
            ofs.write(reinterpret_cast<char*>(&Bmp[i].First), sizeof(Bmp[i].First));
        }
    }

    bool LoadGame()
    {
        std::ifstream ifs("save.dat", std::ios::binary); // | ios::nocreate);
        if (!ifs) return (false);

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

        for (short i = 0; i < SPRITE_COUNT; i++)
        {
            ifs.read(reinterpret_cast<char*>(&Bmp[i].Animation), sizeof(Bmp[i].Animation));
            ifs.read(reinterpret_cast<char*>(&Bmp[i].Phase), sizeof(Bmp[i].Phase));
            ifs.read(reinterpret_cast<char*>(&Bmp[i].First), sizeof(Bmp[i].First));
        }
        return (true);
    }

    void InitStructs()
    {
        short i, k;

        // BILD
        // Standardbildinitialisierung
        for (i = 0; i < SPRITE_COUNT; i++)
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
            for (k = 0; k < SPRITE_COUNT; k++)
                Bmp[i].Rohstoff[k] = 0;
            Bmp[i].AkAnzahl = 0;
            Bmp[i].First = true;
        }

        // Guy
        for (i = GUY_LEFT; i <= GUY_BELOW; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Anzahl = 4;
            Bmp[i].Geschwindigkeit = 20;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSGuyAni;
            Bmp[i].rcSrc.left = 7 * (i - GUY_LEFT);
            Bmp[i].rcSrc.right = 7 + 7 * (i - GUY_LEFT);
            Bmp[i].rcSrc.top = 0;
            Bmp[i].rcSrc.bottom = 18;
            Bmp[i].Breite = 7;
            Bmp[i].Hoehe = 18;
        }

        for (i = GUY_SEARCH; i <= GUY_SLINGSHOT; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSGuyAni;
        }
        Bmp[GUY_SEARCH].Anzahl = 4;
        Bmp[GUY_SEARCH].Geschwindigkeit = 4;
        Bmp[GUY_SEARCH].rcSrc.left = 28;
        Bmp[GUY_SEARCH].rcSrc.right = 39;
        Bmp[GUY_SEARCH].rcSrc.top = 0;
        Bmp[GUY_SEARCH].rcSrc.bottom = 14;
        Bmp[GUY_SEARCH].Breite = 11;
        Bmp[GUY_SEARCH].Hoehe = 14;
        Bmp[GUY_SEARCH].Sound = Sound::CRACKLE;

        Bmp[GUY_EAT].Anzahl = 4;
        Bmp[GUY_EAT].Geschwindigkeit = 4;
        Bmp[GUY_EAT].rcSrc.left = 39;
        Bmp[GUY_EAT].rcSrc.right = 39 + 7;
        Bmp[GUY_EAT].rcSrc.top = 0;
        Bmp[GUY_EAT].rcSrc.bottom = 17;
        Bmp[GUY_EAT].Breite = 7;
        Bmp[GUY_EAT].Hoehe = 17;
        Bmp[GUY_EAT].Sound = Sound::CRACKLE;

        Bmp[GUY_DRINK].Anzahl = 5;
        Bmp[GUY_DRINK].Geschwindigkeit = 4;
        Bmp[GUY_DRINK].rcSrc.left = 46;
        Bmp[GUY_DRINK].rcSrc.right = 46 + 9;
        Bmp[GUY_DRINK].rcSrc.top = 0;
        Bmp[GUY_DRINK].rcSrc.bottom = 13;
        Bmp[GUY_DRINK].Breite = 9;
        Bmp[GUY_DRINK].Hoehe = 13;
        Bmp[GUY_DRINK].Sound = Sound::DRINK;

        Bmp[GUY_CHOP].Anzahl = 4;
        Bmp[GUY_CHOP].Geschwindigkeit = 6;
        Bmp[GUY_CHOP].rcSrc.left = 55;
        Bmp[GUY_CHOP].rcSrc.right = 55 + 15;
        Bmp[GUY_CHOP].rcSrc.top = 0;
        Bmp[GUY_CHOP].rcSrc.bottom = 19;
        Bmp[GUY_CHOP].Breite = 15;
        Bmp[GUY_CHOP].Hoehe = 19;
        Bmp[GUY_CHOP].Sound = Sound::LOG;

        Bmp[GUY_WAITING].Anzahl = 4;
        Bmp[GUY_WAITING].Geschwindigkeit = 2;
        Bmp[GUY_WAITING].rcSrc.left = 70;
        Bmp[GUY_WAITING].rcSrc.right = 70 + 7;
        Bmp[GUY_WAITING].rcSrc.top = 0;
        Bmp[GUY_WAITING].rcSrc.bottom = 18;
        Bmp[GUY_WAITING].Breite = 7;
        Bmp[GUY_WAITING].Hoehe = 18;

        Bmp[GUY_FARMING].Anzahl = 4;
        Bmp[GUY_FARMING].Geschwindigkeit = 4;
        Bmp[GUY_FARMING].rcSrc.left = 78;
        Bmp[GUY_FARMING].rcSrc.right = 78 + 19;
        Bmp[GUY_FARMING].rcSrc.top = 0;
        Bmp[GUY_FARMING].rcSrc.bottom = 18;
        Bmp[GUY_FARMING].Breite = 19;
        Bmp[GUY_FARMING].Hoehe = 18;

        Bmp[GUY_TIE_UP].Anzahl = 2;
        Bmp[GUY_TIE_UP].Geschwindigkeit = 1;
        Bmp[GUY_TIE_UP].rcSrc.left = 97;
        Bmp[GUY_TIE_UP].rcSrc.right = 97 + 8;
        Bmp[GUY_TIE_UP].rcSrc.top = 0;
        Bmp[GUY_TIE_UP].rcSrc.bottom = 18;
        Bmp[GUY_TIE_UP].Breite = 8;
        Bmp[GUY_TIE_UP].Hoehe = 18;

        Bmp[GUY_TIE_DOWN].Anzahl = 2;
        Bmp[GUY_TIE_DOWN].Geschwindigkeit = 1;
        Bmp[GUY_TIE_DOWN].rcSrc.left = 98;
        Bmp[GUY_TIE_DOWN].rcSrc.right = 98 + 7;
        Bmp[GUY_TIE_DOWN].rcSrc.top = 36;
        Bmp[GUY_TIE_DOWN].rcSrc.bottom = 36 + 18;
        Bmp[GUY_TIE_DOWN].Breite = 7;
        Bmp[GUY_TIE_DOWN].Hoehe = 18;

        Bmp[GUY_SLEEPING_TENT].Anzahl = 2;
        Bmp[GUY_SLEEPING_TENT].Geschwindigkeit = 1;
        Bmp[GUY_SLEEPING_TENT].rcSrc.left = 105;
        Bmp[GUY_SLEEPING_TENT].rcSrc.right = 105 + 20;
        Bmp[GUY_SLEEPING_TENT].rcSrc.top = 54;
        Bmp[GUY_SLEEPING_TENT].rcSrc.bottom = 54 + 10;
        Bmp[GUY_SLEEPING_TENT].Breite = 20;
        Bmp[GUY_SLEEPING_TENT].Hoehe = 10;
        Bmp[GUY_SLEEPING_TENT].Sound = Sound::SNORE;

        Bmp[GUY_SLEEPING].Anzahl = 2;
        Bmp[GUY_SLEEPING].Geschwindigkeit = 1;
        Bmp[GUY_SLEEPING].rcSrc.left = 125;
        Bmp[GUY_SLEEPING].rcSrc.right = 125 + 17;
        Bmp[GUY_SLEEPING].rcSrc.top = 36;
        Bmp[GUY_SLEEPING].rcSrc.bottom = 36 + 18;
        Bmp[GUY_SLEEPING].Breite = 17;
        Bmp[GUY_SLEEPING].Hoehe = 18;
        Bmp[GUY_SLEEPING].Sound = Sound::SNORE;

        Bmp[GUY_ENTER_TENT].Anzahl = 3;
        Bmp[GUY_ENTER_TENT].Geschwindigkeit = 4;
        Bmp[GUY_ENTER_TENT].rcSrc.left = 105;
        Bmp[GUY_ENTER_TENT].rcSrc.right = 105 + 7;
        Bmp[GUY_ENTER_TENT].rcSrc.top = 0;
        Bmp[GUY_ENTER_TENT].rcSrc.bottom = 0 + 18;
        Bmp[GUY_ENTER_TENT].Breite = 7;
        Bmp[GUY_ENTER_TENT].Hoehe = 18;

        Bmp[GUY_LAYING_TENT].Anzahl = 2;
        Bmp[GUY_LAYING_TENT].Geschwindigkeit = 2;
        Bmp[GUY_LAYING_TENT].rcSrc.left = 125;
        Bmp[GUY_LAYING_TENT].rcSrc.right = 125 + 17;
        Bmp[GUY_LAYING_TENT].rcSrc.top = 0;
        Bmp[GUY_LAYING_TENT].rcSrc.bottom = 0 + 18;
        Bmp[GUY_LAYING_TENT].Breite = 17;
        Bmp[GUY_LAYING_TENT].Hoehe = 18;

        Bmp[GUY_STAND_UP].Anzahl = 2;
        Bmp[GUY_STAND_UP].Geschwindigkeit = 2;
        Bmp[GUY_STAND_UP].rcSrc.left = 142;
        Bmp[GUY_STAND_UP].rcSrc.right = 142 + 9;
        Bmp[GUY_STAND_UP].rcSrc.top = 0;
        Bmp[GUY_STAND_UP].rcSrc.bottom = 0 + 18;
        Bmp[GUY_STAND_UP].Breite = 9;
        Bmp[GUY_STAND_UP].Hoehe = 18;

        Bmp[GUY_FISHING_LEFT_1].Anzahl = 6;
        Bmp[GUY_FISHING_LEFT_1].Geschwindigkeit = 6;
        Bmp[GUY_FISHING_LEFT_1].rcSrc.left = 151;
        Bmp[GUY_FISHING_LEFT_1].rcSrc.right = 151 + 16;
        Bmp[GUY_FISHING_LEFT_1].rcSrc.top = 0;
        Bmp[GUY_FISHING_LEFT_1].rcSrc.bottom = 0 + 17;
        Bmp[GUY_FISHING_LEFT_1].Breite = 16;
        Bmp[GUY_FISHING_LEFT_1].Hoehe = 17;

        Bmp[GUY_FISHING_ABOVE_1].Anzahl = 6;
        Bmp[GUY_FISHING_ABOVE_1].Geschwindigkeit = 6;
        Bmp[GUY_FISHING_ABOVE_1].rcSrc.left = 167;
        Bmp[GUY_FISHING_ABOVE_1].rcSrc.right = 167 + 16;
        Bmp[GUY_FISHING_ABOVE_1].rcSrc.top = 0;
        Bmp[GUY_FISHING_ABOVE_1].rcSrc.bottom = 0 + 17;
        Bmp[GUY_FISHING_ABOVE_1].Breite = 16;
        Bmp[GUY_FISHING_ABOVE_1].Hoehe = 17;

        Bmp[GUY_FISHING_RIGHT_1].Anzahl = 6;
        Bmp[GUY_FISHING_RIGHT_1].Geschwindigkeit = 6;
        Bmp[GUY_FISHING_RIGHT_1].rcSrc.left = 183;
        Bmp[GUY_FISHING_RIGHT_1].rcSrc.right = 183 + 14;
        Bmp[GUY_FISHING_RIGHT_1].rcSrc.top = 0;
        Bmp[GUY_FISHING_RIGHT_1].rcSrc.bottom = 0 + 17;
        Bmp[GUY_FISHING_RIGHT_1].Breite = 14;
        Bmp[GUY_FISHING_RIGHT_1].Hoehe = 17;

        Bmp[GUY_FISHING_BELOW_1].Anzahl = 6;
        Bmp[GUY_FISHING_BELOW_1].Geschwindigkeit = 6;
        Bmp[GUY_FISHING_BELOW_1].rcSrc.left = 197;
        Bmp[GUY_FISHING_BELOW_1].rcSrc.right = 197 + 14;
        Bmp[GUY_FISHING_BELOW_1].rcSrc.top = 0;
        Bmp[GUY_FISHING_BELOW_1].rcSrc.bottom = 0 + 17;
        Bmp[GUY_FISHING_BELOW_1].Breite = 14;
        Bmp[GUY_FISHING_BELOW_1].Hoehe = 17;

        Bmp[GUY_FISHING_LEFT_2].Anzahl = 4;
        Bmp[GUY_FISHING_LEFT_2].Geschwindigkeit = 3;
        Bmp[GUY_FISHING_LEFT_2].rcSrc.left = 211;
        Bmp[GUY_FISHING_LEFT_2].rcSrc.right = 211 + 16;
        Bmp[GUY_FISHING_LEFT_2].rcSrc.top = 0;
        Bmp[GUY_FISHING_LEFT_2].rcSrc.bottom = 0 + 16;
        Bmp[GUY_FISHING_LEFT_2].Breite = 16;
        Bmp[GUY_FISHING_LEFT_2].Hoehe = 16;

        Bmp[GUY_FISHING_ABOVE_2].Anzahl = 4;
        Bmp[GUY_FISHING_ABOVE_2].Geschwindigkeit = 3;
        Bmp[GUY_FISHING_ABOVE_2].rcSrc.left = 227;
        Bmp[GUY_FISHING_ABOVE_2].rcSrc.right = 227 + 16;
        Bmp[GUY_FISHING_ABOVE_2].rcSrc.top = 0;
        Bmp[GUY_FISHING_ABOVE_2].rcSrc.bottom = 0 + 16;
        Bmp[GUY_FISHING_ABOVE_2].Breite = 16;
        Bmp[GUY_FISHING_ABOVE_2].Hoehe = 16;

        Bmp[GUY_FISHING_RIGHT_2].Anzahl = 4;
        Bmp[GUY_FISHING_RIGHT_2].Geschwindigkeit = 3;
        Bmp[GUY_FISHING_RIGHT_2].rcSrc.left = 243;
        Bmp[GUY_FISHING_RIGHT_2].rcSrc.right = 243 + 14;
        Bmp[GUY_FISHING_RIGHT_2].rcSrc.top = 0;
        Bmp[GUY_FISHING_RIGHT_2].rcSrc.bottom = 0 + 15;
        Bmp[GUY_FISHING_RIGHT_2].Breite = 14;
        Bmp[GUY_FISHING_RIGHT_2].Hoehe = 15;

        Bmp[GUY_FISHING_BELOW_2].Anzahl = 4;
        Bmp[GUY_FISHING_BELOW_2].Geschwindigkeit = 3;
        Bmp[GUY_FISHING_BELOW_2].rcSrc.left = 257;
        Bmp[GUY_FISHING_BELOW_2].rcSrc.right = 257 + 14;
        Bmp[GUY_FISHING_BELOW_2].rcSrc.top = 0;
        Bmp[GUY_FISHING_BELOW_2].rcSrc.bottom = 0 + 15;
        Bmp[GUY_FISHING_BELOW_2].Breite = 14;
        Bmp[GUY_FISHING_BELOW_2].Hoehe = 15;

        Bmp[GUY_FISHING_LEFT_3].Anzahl = 3;
        Bmp[GUY_FISHING_LEFT_3].Geschwindigkeit = 2;
        Bmp[GUY_FISHING_LEFT_3].rcSrc.left = 271;
        Bmp[GUY_FISHING_LEFT_3].rcSrc.right = 271 + 16;
        Bmp[GUY_FISHING_LEFT_3].rcSrc.top = 0;
        Bmp[GUY_FISHING_LEFT_3].rcSrc.bottom = 0 + 16;
        Bmp[GUY_FISHING_LEFT_3].Breite = 16;
        Bmp[GUY_FISHING_LEFT_3].Hoehe = 16;

        Bmp[GUY_FISHING_ABOVE_3].Anzahl = 3;
        Bmp[GUY_FISHING_ABOVE_3].Geschwindigkeit = 2;
        Bmp[GUY_FISHING_ABOVE_3].rcSrc.left = 285;
        Bmp[GUY_FISHING_ABOVE_3].rcSrc.right = 285 + 16;
        Bmp[GUY_FISHING_ABOVE_3].rcSrc.top = 0;
        Bmp[GUY_FISHING_ABOVE_3].rcSrc.bottom = 0 + 16;
        Bmp[GUY_FISHING_ABOVE_3].Breite = 16;
        Bmp[GUY_FISHING_ABOVE_3].Hoehe = 16;

        Bmp[GUY_FISHING_RIGHT_3].Anzahl = 3;
        Bmp[GUY_FISHING_RIGHT_3].Geschwindigkeit = 2;
        Bmp[GUY_FISHING_RIGHT_3].rcSrc.left = 299;
        Bmp[GUY_FISHING_RIGHT_3].rcSrc.right = 299 + 14;
        Bmp[GUY_FISHING_RIGHT_3].rcSrc.top = 0;
        Bmp[GUY_FISHING_RIGHT_3].rcSrc.bottom = 0 + 15;
        Bmp[GUY_FISHING_RIGHT_3].Breite = 14;
        Bmp[GUY_FISHING_RIGHT_3].Hoehe = 15;

        Bmp[GUY_FISHING_BELOW_3].Anzahl = 3;
        Bmp[GUY_FISHING_BELOW_3].Geschwindigkeit = 2;
        Bmp[GUY_FISHING_BELOW_3].rcSrc.left = 313;
        Bmp[GUY_FISHING_BELOW_3].rcSrc.right = 313 + 14;
        Bmp[GUY_FISHING_BELOW_3].rcSrc.top = 0;
        Bmp[GUY_FISHING_BELOW_3].rcSrc.bottom = 0 + 15;
        Bmp[GUY_FISHING_BELOW_3].Breite = 14;
        Bmp[GUY_FISHING_BELOW_3].Hoehe = 15;

        Bmp[GUY_BEAT].Anzahl = 4;
        Bmp[GUY_BEAT].Geschwindigkeit = 7;
        Bmp[GUY_BEAT].rcSrc.left = 327;
        Bmp[GUY_BEAT].rcSrc.right = 327 + 12;
        Bmp[GUY_BEAT].rcSrc.top = 0;
        Bmp[GUY_BEAT].rcSrc.bottom = 0 + 24;
        Bmp[GUY_BEAT].Breite = 12;
        Bmp[GUY_BEAT].Hoehe = 24;
        Bmp[GUY_BEAT].Sound = Sound::HIT;

        Bmp[GUY_BOAT_LEFT].Anzahl = 6;
        Bmp[GUY_BOAT_LEFT].Geschwindigkeit = 10;
        Bmp[GUY_BOAT_LEFT].rcSrc.left = 339;
        Bmp[GUY_BOAT_LEFT].rcSrc.right = 339 + 26;
        Bmp[GUY_BOAT_LEFT].rcSrc.top = 0;
        Bmp[GUY_BOAT_LEFT].rcSrc.bottom = 0 + 21;
        Bmp[GUY_BOAT_LEFT].Breite = 26;
        Bmp[GUY_BOAT_LEFT].Hoehe = 21;

        Bmp[GUY_BOAT_ABOVE].Anzahl = 6;
        Bmp[GUY_BOAT_ABOVE].Geschwindigkeit = 10;
        Bmp[GUY_BOAT_ABOVE].rcSrc.left = 365;
        Bmp[GUY_BOAT_ABOVE].rcSrc.right = 365 + 26;
        Bmp[GUY_BOAT_ABOVE].rcSrc.top = 0;
        Bmp[GUY_BOAT_ABOVE].rcSrc.bottom = 0 + 21;
        Bmp[GUY_BOAT_ABOVE].Breite = 26;
        Bmp[GUY_BOAT_ABOVE].Hoehe = 21;

        Bmp[GUY_BOAT_RIGHT].Anzahl = 6;
        Bmp[GUY_BOAT_RIGHT].Geschwindigkeit = 10;
        Bmp[GUY_BOAT_RIGHT].rcSrc.left = 391;
        Bmp[GUY_BOAT_RIGHT].rcSrc.right = 391 + 26;
        Bmp[GUY_BOAT_RIGHT].rcSrc.top = 0;
        Bmp[GUY_BOAT_RIGHT].rcSrc.bottom = 0 + 21;
        Bmp[GUY_BOAT_RIGHT].Breite = 26;
        Bmp[GUY_BOAT_RIGHT].Hoehe = 21;

        Bmp[GUY_BOAT_BELOW].Anzahl = 6;
        Bmp[GUY_BOAT_BELOW].Geschwindigkeit = 10;
        Bmp[GUY_BOAT_BELOW].rcSrc.left = 417;
        Bmp[GUY_BOAT_BELOW].rcSrc.right = 417 + 26;
        Bmp[GUY_BOAT_BELOW].rcSrc.top = 0;
        Bmp[GUY_BOAT_BELOW].rcSrc.bottom = 0 + 21;
        Bmp[GUY_BOAT_BELOW].Breite = 26;
        Bmp[GUY_BOAT_BELOW].Hoehe = 21;

        Bmp[GUY_BOAT_FISHING_1].Anzahl = 6;
        Bmp[GUY_BOAT_FISHING_1].Geschwindigkeit = 6;
        Bmp[GUY_BOAT_FISHING_1].rcSrc.left = 443;
        Bmp[GUY_BOAT_FISHING_1].rcSrc.right = 443 + 26;
        Bmp[GUY_BOAT_FISHING_1].rcSrc.top = 0;
        Bmp[GUY_BOAT_FISHING_1].rcSrc.bottom = 0 + 25;
        Bmp[GUY_BOAT_FISHING_1].Breite = 26;
        Bmp[GUY_BOAT_FISHING_1].Hoehe = 25;

        Bmp[GUY_BOAT_FISHING_2].Anzahl = 4;
        Bmp[GUY_BOAT_FISHING_2].Geschwindigkeit = 3;
        Bmp[GUY_BOAT_FISHING_2].rcSrc.left = 469;
        Bmp[GUY_BOAT_FISHING_2].rcSrc.right = 469 + 26;
        Bmp[GUY_BOAT_FISHING_2].rcSrc.top = 0;
        Bmp[GUY_BOAT_FISHING_2].rcSrc.bottom = 0 + 25;
        Bmp[GUY_BOAT_FISHING_2].Breite = 26;
        Bmp[GUY_BOAT_FISHING_2].Hoehe = 25;

        Bmp[GUY_BOAT_FISHING_3].Anzahl = 3;
        Bmp[GUY_BOAT_FISHING_3].Geschwindigkeit = 2;
        Bmp[GUY_BOAT_FISHING_3].rcSrc.left = 495;
        Bmp[GUY_BOAT_FISHING_3].rcSrc.right = 495 + 26;
        Bmp[GUY_BOAT_FISHING_3].rcSrc.top = 0;
        Bmp[GUY_BOAT_FISHING_3].rcSrc.bottom = 0 + 25;
        Bmp[GUY_BOAT_FISHING_3].Breite = 26;
        Bmp[GUY_BOAT_FISHING_3].Hoehe = 25;

        Bmp[GUY_DIVING_1].Anzahl = 5;
        Bmp[GUY_DIVING_1].Geschwindigkeit = 5;
        Bmp[GUY_DIVING_1].rcSrc.left = 521;
        Bmp[GUY_DIVING_1].rcSrc.right = 521 + 26;
        Bmp[GUY_DIVING_1].rcSrc.top = 0;
        Bmp[GUY_DIVING_1].rcSrc.bottom = 0 + 27;
        Bmp[GUY_DIVING_1].Breite = 26;
        Bmp[GUY_DIVING_1].Hoehe = 27;

        Bmp[GUY_DIVING_2].Anzahl = 4;
        Bmp[GUY_DIVING_2].Geschwindigkeit = 3;
        Bmp[GUY_DIVING_2].rcSrc.left = 547;
        Bmp[GUY_DIVING_2].rcSrc.right = 547 + 26;
        Bmp[GUY_DIVING_2].rcSrc.top = 0;
        Bmp[GUY_DIVING_2].rcSrc.bottom = 0 + 17;
        Bmp[GUY_DIVING_2].Breite = 26;
        Bmp[GUY_DIVING_2].Hoehe = 17;

        Bmp[GUY_DIVING_3].Anzahl = 2;
        Bmp[GUY_DIVING_3].Geschwindigkeit = 2;
        Bmp[GUY_DIVING_3].rcSrc.left = 573;
        Bmp[GUY_DIVING_3].rcSrc.right = 573 + 26;
        Bmp[GUY_DIVING_3].rcSrc.top = 0;
        Bmp[GUY_DIVING_3].rcSrc.bottom = 0 + 17;
        Bmp[GUY_DIVING_3].Breite = 26;
        Bmp[GUY_DIVING_3].Hoehe = 17;

        Bmp[GUY_HAMMER_1].Anzahl = 2;
        Bmp[GUY_HAMMER_1].Geschwindigkeit = 4;
        Bmp[GUY_HAMMER_1].rcSrc.left = 599;
        Bmp[GUY_HAMMER_1].rcSrc.right = 599 + 9;
        Bmp[GUY_HAMMER_1].rcSrc.top = 0;
        Bmp[GUY_HAMMER_1].rcSrc.bottom = 0 + 18;
        Bmp[GUY_HAMMER_1].Breite = 9;
        Bmp[GUY_HAMMER_1].Hoehe = 18;
        Bmp[GUY_HAMMER_1].Sound = Sound::HAMMER;

        Bmp[GUY_CLIMBING_1].Anzahl = 6;
        Bmp[GUY_CLIMBING_1].Geschwindigkeit = 2;
        Bmp[GUY_CLIMBING_1].rcSrc.left = 608;
        Bmp[GUY_CLIMBING_1].rcSrc.right = 608 + 7;
        Bmp[GUY_CLIMBING_1].rcSrc.top = 0;
        Bmp[GUY_CLIMBING_1].rcSrc.bottom = 0 + 34;
        Bmp[GUY_CLIMBING_1].Breite = 7;
        Bmp[GUY_CLIMBING_1].Hoehe = 34;

        Bmp[GUY_CLIMBING_2].Anzahl = 6;
        Bmp[GUY_CLIMBING_2].Geschwindigkeit = 2;
        Bmp[GUY_CLIMBING_2].rcSrc.left = 615;
        Bmp[GUY_CLIMBING_2].rcSrc.right = 615 + 7;
        Bmp[GUY_CLIMBING_2].rcSrc.top = 0;
        Bmp[GUY_CLIMBING_2].rcSrc.bottom = 0 + 34;
        Bmp[GUY_CLIMBING_2].Breite = 7;
        Bmp[GUY_CLIMBING_2].Hoehe = 34;

        Bmp[GUY_HAMMER_2].Anzahl = 2;
        Bmp[GUY_HAMMER_2].Geschwindigkeit = 4;
        Bmp[GUY_HAMMER_2].rcSrc.left = 622;
        Bmp[GUY_HAMMER_2].rcSrc.right = 622 + 7;
        Bmp[GUY_HAMMER_2].rcSrc.top = 0;
        Bmp[GUY_HAMMER_2].rcSrc.bottom = 0 + 34;
        Bmp[GUY_HAMMER_2].Breite = 7;
        Bmp[GUY_HAMMER_2].Hoehe = 34;
        Bmp[GUY_HAMMER_2].Sound = Sound::HAMMER;

        Bmp[GUY_ENTER_HOUSE].Anzahl = 3;
        Bmp[GUY_ENTER_HOUSE].Geschwindigkeit = 2;
        Bmp[GUY_ENTER_HOUSE].rcSrc.left = 631;
        Bmp[GUY_ENTER_HOUSE].rcSrc.right = 631 + 9;
        Bmp[GUY_ENTER_HOUSE].rcSrc.top = 0;
        Bmp[GUY_ENTER_HOUSE].rcSrc.bottom = 0 + 34;
        Bmp[GUY_ENTER_HOUSE].Breite = 9;
        Bmp[GUY_ENTER_HOUSE].Hoehe = 34;

        Bmp[GUY_SLEEP_HOUSE].Anzahl = 2;
        Bmp[GUY_SLEEP_HOUSE].Geschwindigkeit = 1;
        Bmp[GUY_SLEEP_HOUSE].rcSrc.left = 640;
        Bmp[GUY_SLEEP_HOUSE].rcSrc.right = 640 + 10;
        Bmp[GUY_SLEEP_HOUSE].rcSrc.top = 0;
        Bmp[GUY_SLEEP_HOUSE].rcSrc.bottom = 0 + 34;
        Bmp[GUY_SLEEP_HOUSE].Breite = 10;
        Bmp[GUY_SLEEP_HOUSE].Hoehe = 34;
        Bmp[GUY_SLEEP_HOUSE].Sound = Sound::SNORE;

        Bmp[GUY_EXIT_HOUSE].Anzahl = 3;
        Bmp[GUY_EXIT_HOUSE].Geschwindigkeit = 2;
        Bmp[GUY_EXIT_HOUSE].rcSrc.left = 650;
        Bmp[GUY_EXIT_HOUSE].rcSrc.right = 650 + 9;
        Bmp[GUY_EXIT_HOUSE].rcSrc.top = 0;
        Bmp[GUY_EXIT_HOUSE].rcSrc.bottom = 0 + 34;
        Bmp[GUY_EXIT_HOUSE].Breite = 9;
        Bmp[GUY_EXIT_HOUSE].Hoehe = 34;

        Bmp[GUY_SET_FIRE].Anzahl = 6;
        Bmp[GUY_SET_FIRE].Geschwindigkeit = 5;
        Bmp[GUY_SET_FIRE].rcSrc.left = 659;
        Bmp[GUY_SET_FIRE].rcSrc.right = 659 + 19;
        Bmp[GUY_SET_FIRE].rcSrc.top = 0;
        Bmp[GUY_SET_FIRE].rcSrc.bottom = 0 + 18;
        Bmp[GUY_SET_FIRE].Breite = 19;
        Bmp[GUY_SET_FIRE].Hoehe = 18;

        Bmp[GUY_LOOK_OUT].Anzahl = 4;
        Bmp[GUY_LOOK_OUT].Geschwindigkeit = 1;
        Bmp[GUY_LOOK_OUT].rcSrc.left = 678;
        Bmp[GUY_LOOK_OUT].rcSrc.right = 678 + 10;
        Bmp[GUY_LOOK_OUT].rcSrc.top = 0;
        Bmp[GUY_LOOK_OUT].rcSrc.bottom = 0 + 18;
        Bmp[GUY_LOOK_OUT].Breite = 10;
        Bmp[GUY_LOOK_OUT].Hoehe = 18;

        Bmp[GUY_SHOVEL].Anzahl = 10;
        Bmp[GUY_SHOVEL].Geschwindigkeit = 3;
        Bmp[GUY_SHOVEL].rcSrc.left = 688;
        Bmp[GUY_SHOVEL].rcSrc.right = 688 + 17;
        Bmp[GUY_SHOVEL].rcSrc.top = 0;
        Bmp[GUY_SHOVEL].rcSrc.bottom = 0 + 19;
        Bmp[GUY_SHOVEL].Breite = 17;
        Bmp[GUY_SHOVEL].Hoehe = 19;
        Bmp[GUY_SHOVEL].Sound = Sound::DIG;

        Bmp[GUY_SHIP].Anzahl = 4;
        Bmp[GUY_SHIP].Geschwindigkeit = 10;
        Bmp[GUY_SHIP].rcSrc.left = 297;
        Bmp[GUY_SHIP].rcSrc.right = 297 + 48;
        Bmp[GUY_SHIP].rcSrc.top = 0;
        Bmp[GUY_SHIP].rcSrc.bottom = 0 + 38;
        Bmp[GUY_SHIP].Breite = 48;
        Bmp[GUY_SHIP].Hoehe = 38;
        Bmp[GUY_SHIP].Surface = lpDDSBau;
        Bmp[GUY_SHIP].Sound = Sound::STORM;

        Bmp[GUY_SHIP_DOWN].Anzahl = 6;
        Bmp[GUY_SHIP_DOWN].Geschwindigkeit = 3;
        Bmp[GUY_SHIP_DOWN].rcSrc.left = 345;
        Bmp[GUY_SHIP_DOWN].rcSrc.right = 345 + 46;
        Bmp[GUY_SHIP_DOWN].rcSrc.top = 0;
        Bmp[GUY_SHIP_DOWN].rcSrc.bottom = 0 + 40;
        Bmp[GUY_SHIP_DOWN].Breite = 46;
        Bmp[GUY_SHIP_DOWN].Hoehe = 40;
        Bmp[GUY_SHIP_DOWN].Surface = lpDDSBau;
        Bmp[GUY_SHIP_DOWN].Sound = Sound::SPLAT;

        Bmp[GUY_SWIM].Anzahl = 4;
        Bmp[GUY_SWIM].Geschwindigkeit = 10;
        Bmp[GUY_SWIM].rcSrc.left = 705;
        Bmp[GUY_SWIM].rcSrc.right = 705 + 15;
        Bmp[GUY_SWIM].rcSrc.top = 0;
        Bmp[GUY_SWIM].rcSrc.bottom = 0 + 9;
        Bmp[GUY_SWIM].Breite = 15;
        Bmp[GUY_SWIM].Hoehe = 9;
        Bmp[GUY_SWIM].Sound = Sound::SWIM;

        Bmp[GUY_DEAD].Anzahl = 6;
        Bmp[GUY_DEAD].Geschwindigkeit = 1;
        Bmp[GUY_DEAD].rcSrc.left = 743;
        Bmp[GUY_DEAD].rcSrc.right = 743 + 16;
        Bmp[GUY_DEAD].rcSrc.top = 0;
        Bmp[GUY_DEAD].rcSrc.bottom = 0 + 10;
        Bmp[GUY_DEAD].Breite = 16;
        Bmp[GUY_DEAD].Hoehe = 10;

        Bmp[GUY_BOAT_DEAD].Anzahl = 6;
        Bmp[GUY_BOAT_DEAD].Geschwindigkeit = 1;
        Bmp[GUY_BOAT_DEAD].rcSrc.left = 759;
        Bmp[GUY_BOAT_DEAD].rcSrc.right = 759 + 26;
        Bmp[GUY_BOAT_DEAD].rcSrc.top = 0;
        Bmp[GUY_BOAT_DEAD].rcSrc.bottom = 0 + 18;
        Bmp[GUY_BOAT_DEAD].Breite = 26;
        Bmp[GUY_BOAT_DEAD].Hoehe = 18;

        Bmp[GUY_BOAT_WAITING].Anzahl = 4;
        Bmp[GUY_BOAT_WAITING].Geschwindigkeit = 2;
        Bmp[GUY_BOAT_WAITING].rcSrc.left = 0;
        Bmp[GUY_BOAT_WAITING].rcSrc.right = 0 + 26;
        Bmp[GUY_BOAT_WAITING].rcSrc.top = 72;
        Bmp[GUY_BOAT_WAITING].rcSrc.bottom = 72 + 18;
        Bmp[GUY_BOAT_WAITING].Breite = 26;
        Bmp[GUY_BOAT_WAITING].Hoehe = 20;

        Bmp[GUY_SLINGSHOT].Anzahl = 5;
        Bmp[GUY_SLINGSHOT].Geschwindigkeit = 4;
        Bmp[GUY_SLINGSHOT].rcSrc.left = 720;
        Bmp[GUY_SLINGSHOT].rcSrc.right = 720 + 23;
        Bmp[GUY_SLINGSHOT].rcSrc.top = 0;
        Bmp[GUY_SLINGSHOT].rcSrc.bottom = 0 + 20;
        Bmp[GUY_SLINGSHOT].Breite = 23;
        Bmp[GUY_SLINGSHOT].Hoehe = 20;

        // Cursor
        for (i = CURSOR_ARROW; i <= CURSOR_CLOCK; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Anzahl = 1;
            Bmp[i].Geschwindigkeit = 0;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSCursor;
            Bmp[i].rcSrc.left = (i - CURSOR_ARROW) * 18;
            Bmp[i].rcSrc.top = 0;
            Bmp[i].rcSrc.right = Bmp[i].rcSrc.left + 18;
            Bmp[i].rcSrc.bottom = 18;
            Bmp[i].Breite = 18;
            Bmp[i].Hoehe = 18;
        }

        // Landschaftsanimationen
        for (i = SEA_WAVES; i <= FLOODGATE_6; i++)
        {
            Bmp[i].Animation = true;
            Bmp[i].Anzahl = 7;
            Bmp[i].Geschwindigkeit = 7;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSAnimation;
            Bmp[i].Sound = Sound::RIVER;
        }

        Bmp[SEA_WAVES].Geschwindigkeit = 3;
        Bmp[SEA_WAVES].Breite = 28;
        Bmp[SEA_WAVES].Hoehe = 13;
        Bmp[SEA_WAVES].rcSrc.left = 0;
        Bmp[SEA_WAVES].rcSrc.right = Bmp[SEA_WAVES].rcSrc.left + Bmp[SEA_WAVES].Breite;
        Bmp[SEA_WAVES].rcSrc.top = 0;
        Bmp[SEA_WAVES].rcSrc.bottom = Bmp[SEA_WAVES].rcSrc.top + Bmp[SEA_WAVES].Hoehe;
        Bmp[SEA_WAVES].rcDes.left = 9;
        Bmp[SEA_WAVES].rcDes.right = Bmp[SEA_WAVES].rcDes.left + Bmp[SEA_WAVES].Breite;
        Bmp[SEA_WAVES].rcDes.top = 23;
        Bmp[SEA_WAVES].rcDes.bottom = Bmp[SEA_WAVES].rcDes.top + Bmp[SEA_WAVES].Hoehe;
        Bmp[SEA_WAVES].Sound = Sound::SURF;

        Bmp[RIVER_1].Breite = 34;
        Bmp[RIVER_1].Hoehe = 8;
        Bmp[RIVER_1].rcSrc.left = 28;
        Bmp[RIVER_1].rcSrc.right = Bmp[RIVER_1].rcSrc.left + Bmp[RIVER_1].Breite;
        Bmp[RIVER_1].rcSrc.top = 0;
        Bmp[RIVER_1].rcSrc.bottom = Bmp[RIVER_1].rcSrc.top + Bmp[RIVER_1].Hoehe;
        Bmp[RIVER_1].rcDes.left = 10;
        Bmp[RIVER_1].rcDes.right = Bmp[RIVER_1].rcDes.left + Bmp[RIVER_1].Breite;
        Bmp[RIVER_1].rcDes.top = 18;
        Bmp[RIVER_1].rcDes.bottom = Bmp[RIVER_1].rcDes.top + Bmp[RIVER_1].Hoehe;

        Bmp[RIVER_2].Breite = 34;
        Bmp[RIVER_2].Hoehe = 8;
        Bmp[RIVER_2].rcSrc.left = 62;
        Bmp[RIVER_2].rcSrc.right = Bmp[RIVER_2].rcSrc.left + Bmp[RIVER_2].Breite;
        Bmp[RIVER_2].rcSrc.top = 0;
        Bmp[RIVER_2].rcSrc.bottom = Bmp[RIVER_2].rcSrc.top + Bmp[RIVER_2].Hoehe;
        Bmp[RIVER_2].rcDes.left = 10;
        Bmp[RIVER_2].rcDes.right = Bmp[RIVER_2].rcDes.left + Bmp[RIVER_2].Breite;
        Bmp[RIVER_2].rcDes.top = 19;
        Bmp[RIVER_2].rcDes.bottom = Bmp[RIVER_2].rcDes.top + Bmp[RIVER_2].Hoehe;

        Bmp[RIVER_3].Breite = 34;
        Bmp[RIVER_3].Hoehe = 34;
        Bmp[RIVER_3].rcSrc.left = 96;
        Bmp[RIVER_3].rcSrc.right = Bmp[RIVER_3].rcSrc.left + Bmp[RIVER_3].Breite;
        Bmp[RIVER_3].rcSrc.top = 0;
        Bmp[RIVER_3].rcSrc.bottom = Bmp[RIVER_3].rcSrc.top + Bmp[RIVER_3].Hoehe;
        Bmp[RIVER_3].rcDes.left = 10;
        Bmp[RIVER_3].rcDes.right = Bmp[RIVER_3].rcDes.left + Bmp[RIVER_3].Breite;
        Bmp[RIVER_3].rcDes.top = 5;
        Bmp[RIVER_3].rcDes.bottom = Bmp[RIVER_3].rcDes.top + Bmp[RIVER_3].Hoehe;

        Bmp[RIVER_4].Breite = 34;
        Bmp[RIVER_4].Hoehe = 34;
        Bmp[RIVER_4].rcSrc.left = 130;
        Bmp[RIVER_4].rcSrc.right = Bmp[RIVER_4].rcSrc.left + Bmp[RIVER_4].Breite;
        Bmp[RIVER_4].rcSrc.top = 0;
        Bmp[RIVER_4].rcSrc.bottom = Bmp[RIVER_4].rcSrc.top + Bmp[RIVER_4].Hoehe;
        Bmp[RIVER_4].rcDes.left = 10;
        Bmp[RIVER_4].rcDes.right = Bmp[RIVER_4].rcDes.left + Bmp[RIVER_4].Breite;
        Bmp[RIVER_4].rcDes.top = 5;
        Bmp[RIVER_4].rcDes.bottom = Bmp[RIVER_4].rcDes.top + Bmp[RIVER_4].Hoehe;

        Bmp[RIVER_5].Breite = 34;
        Bmp[RIVER_5].Hoehe = 18;
        Bmp[RIVER_5].rcSrc.left = 164;
        Bmp[RIVER_5].rcSrc.right = Bmp[RIVER_5].rcSrc.left + Bmp[RIVER_5].Breite;
        Bmp[RIVER_5].rcSrc.top = 0;
        Bmp[RIVER_5].rcSrc.bottom = Bmp[RIVER_5].rcSrc.top + Bmp[RIVER_5].Hoehe;
        Bmp[RIVER_5].rcDes.left = 10;
        Bmp[RIVER_5].rcDes.right = Bmp[RIVER_5].rcDes.left + Bmp[RIVER_5].Breite;
        Bmp[RIVER_5].rcDes.top = 21;
        Bmp[RIVER_5].rcDes.bottom = Bmp[RIVER_5].rcDes.top + Bmp[RIVER_5].Hoehe;

        Bmp[RIVER_6].Breite = 34;
        Bmp[RIVER_6].Hoehe = 18;
        Bmp[RIVER_6].rcSrc.left = 198;
        Bmp[RIVER_6].rcSrc.right = Bmp[RIVER_6].rcSrc.left + Bmp[RIVER_6].Breite;
        Bmp[RIVER_6].rcSrc.top = 0;
        Bmp[RIVER_6].rcSrc.bottom = Bmp[RIVER_6].rcSrc.top + Bmp[RIVER_6].Hoehe;
        Bmp[RIVER_6].rcDes.left = 10;
        Bmp[RIVER_6].rcDes.right = Bmp[RIVER_6].rcDes.left + Bmp[RIVER_6].Breite;
        Bmp[RIVER_6].rcDes.top = 21;
        Bmp[RIVER_6].rcDes.bottom = Bmp[RIVER_6].rcDes.top + Bmp[RIVER_6].Hoehe;

        Bmp[RIVER_7].Breite = 18;
        Bmp[RIVER_7].Hoehe = 18;
        Bmp[RIVER_7].rcSrc.left = 232;
        Bmp[RIVER_7].rcSrc.right = Bmp[RIVER_7].rcSrc.left + Bmp[RIVER_7].Breite;
        Bmp[RIVER_7].rcSrc.top = 0;
        Bmp[RIVER_7].rcSrc.bottom = Bmp[RIVER_7].rcSrc.top + Bmp[RIVER_7].Hoehe;
        Bmp[RIVER_7].rcDes.left = 10;
        Bmp[RIVER_7].rcDes.right = Bmp[RIVER_7].rcDes.left + Bmp[RIVER_7].Breite;
        Bmp[RIVER_7].rcDes.top = 21;
        Bmp[RIVER_7].rcDes.bottom = Bmp[RIVER_7].rcDes.top + Bmp[RIVER_7].Hoehe;

        Bmp[RIVER_8].Breite = 18;
        Bmp[RIVER_8].Hoehe = 18;
        Bmp[RIVER_8].rcSrc.left = 250;
        Bmp[RIVER_8].rcSrc.right = Bmp[RIVER_8].rcSrc.left + Bmp[RIVER_8].Breite;
        Bmp[RIVER_8].rcSrc.top = 0;
        Bmp[RIVER_8].rcSrc.bottom = Bmp[RIVER_8].rcSrc.top + Bmp[RIVER_8].Hoehe;
        Bmp[RIVER_8].rcDes.left = 26;
        Bmp[RIVER_8].rcDes.right = Bmp[RIVER_8].rcDes.left + Bmp[RIVER_8].Breite;
        Bmp[RIVER_8].rcDes.top = 21;
        Bmp[RIVER_8].rcDes.bottom = Bmp[RIVER_8].rcDes.top + Bmp[RIVER_8].Hoehe;

        Bmp[RIVER_9].Breite = 34;
        Bmp[RIVER_9].Hoehe = 15;
        Bmp[RIVER_9].rcSrc.left = 268;
        Bmp[RIVER_9].rcSrc.right = Bmp[RIVER_9].rcSrc.left + Bmp[RIVER_9].Breite;
        Bmp[RIVER_9].rcSrc.top = 0;
        Bmp[RIVER_9].rcSrc.bottom = Bmp[RIVER_9].rcSrc.top + Bmp[RIVER_9].Hoehe;
        Bmp[RIVER_9].rcDes.left = 10;
        Bmp[RIVER_9].rcDes.right = Bmp[RIVER_9].rcDes.left + Bmp[RIVER_9].Breite;
        Bmp[RIVER_9].rcDes.top = 20;
        Bmp[RIVER_9].rcDes.bottom = Bmp[RIVER_9].rcDes.top + Bmp[RIVER_9].Hoehe;

        Bmp[RIVER_10].Breite = 34;
        Bmp[RIVER_10].Hoehe = 13;
        Bmp[RIVER_10].rcSrc.left = 302;
        Bmp[RIVER_10].rcSrc.right = Bmp[RIVER_10].rcSrc.left + Bmp[RIVER_10].Breite;
        Bmp[RIVER_10].rcSrc.top = 0;
        Bmp[RIVER_10].rcSrc.bottom = Bmp[RIVER_10].rcSrc.top + Bmp[RIVER_10].Hoehe;
        Bmp[RIVER_10].rcDes.left = 10;
        Bmp[RIVER_10].rcDes.right = Bmp[RIVER_10].rcDes.left + Bmp[RIVER_10].Breite;
        Bmp[RIVER_10].rcDes.top = 26;
        Bmp[RIVER_10].rcDes.bottom = Bmp[RIVER_10].rcDes.top + Bmp[RIVER_10].Hoehe;

        Bmp[RIVER_END_1].Breite = 42;
        Bmp[RIVER_END_1].Hoehe = 22;
        Bmp[RIVER_END_1].rcSrc.left = 336;
        Bmp[RIVER_END_1].rcSrc.right = Bmp[RIVER_END_1].rcSrc.left + Bmp[RIVER_END_1].Breite;
        Bmp[RIVER_END_1].rcSrc.top = 0;
        Bmp[RIVER_END_1].rcSrc.bottom = Bmp[RIVER_END_1].rcSrc.top + Bmp[RIVER_END_1].Hoehe;
        Bmp[RIVER_END_1].rcDes.left = 2;
        Bmp[RIVER_END_1].rcDes.right = Bmp[RIVER_END_1].rcDes.left + Bmp[RIVER_END_1].Breite;
        Bmp[RIVER_END_1].rcDes.top = 17;
        Bmp[RIVER_END_1].rcDes.bottom = Bmp[RIVER_END_1].rcDes.top + Bmp[RIVER_END_1].Hoehe;

        Bmp[RIVER_END_2].Breite = 40;
        Bmp[RIVER_END_2].Hoehe = 22;
        Bmp[RIVER_END_2].rcSrc.left = 378;
        Bmp[RIVER_END_2].rcSrc.right = Bmp[RIVER_END_2].rcSrc.left + Bmp[RIVER_END_2].Breite;
        Bmp[RIVER_END_2].rcSrc.top = 0;
        Bmp[RIVER_END_2].rcSrc.bottom = Bmp[RIVER_END_2].rcSrc.top + Bmp[RIVER_END_2].Hoehe;
        Bmp[RIVER_END_2].rcDes.left = 10;
        Bmp[RIVER_END_2].rcDes.right = Bmp[RIVER_END_2].rcDes.left + Bmp[RIVER_END_2].Breite;
        Bmp[RIVER_END_2].rcDes.top = 17;
        Bmp[RIVER_END_2].rcDes.bottom = Bmp[RIVER_END_2].rcDes.top + Bmp[RIVER_END_2].Hoehe;

        Bmp[RIVER_END_3].Breite = 40;
        Bmp[RIVER_END_3].Hoehe = 22;
        Bmp[RIVER_END_3].rcSrc.left = 418;
        Bmp[RIVER_END_3].rcSrc.right = Bmp[RIVER_END_3].rcSrc.left + Bmp[RIVER_END_3].Breite;
        Bmp[RIVER_END_3].rcSrc.top = 0;
        Bmp[RIVER_END_3].rcSrc.bottom = Bmp[RIVER_END_3].rcSrc.top + Bmp[RIVER_END_3].Hoehe;
        Bmp[RIVER_END_3].rcDes.left = 10;
        Bmp[RIVER_END_3].rcDes.right = Bmp[RIVER_END_3].rcDes.left + Bmp[RIVER_END_3].Breite;
        Bmp[RIVER_END_3].rcDes.top = 21;
        Bmp[RIVER_END_3].rcDes.bottom = Bmp[RIVER_END_3].rcDes.top + Bmp[RIVER_END_3].Hoehe;

        Bmp[RIVER_END_4].Breite = 42;
        Bmp[RIVER_END_4].Hoehe = 22;
        Bmp[RIVER_END_4].rcSrc.left = 458;
        Bmp[RIVER_END_4].rcSrc.right = Bmp[RIVER_END_4].rcSrc.left + Bmp[RIVER_END_4].Breite;
        Bmp[RIVER_END_4].rcSrc.top = 0;
        Bmp[RIVER_END_4].rcSrc.bottom = Bmp[RIVER_END_4].rcSrc.top + Bmp[RIVER_END_4].Hoehe;
        Bmp[RIVER_END_4].rcDes.left = 2;
        Bmp[RIVER_END_4].rcDes.right = Bmp[RIVER_END_4].rcDes.left + Bmp[RIVER_END_4].Breite;
        Bmp[RIVER_END_4].rcDes.top = 21;
        Bmp[RIVER_END_4].rcDes.bottom = Bmp[RIVER_END_4].rcDes.top + Bmp[RIVER_END_4].Hoehe;

        Bmp[RIVER_START_1].Breite = 30;
        Bmp[RIVER_START_1].Hoehe = 19;
        Bmp[RIVER_START_1].rcSrc.left = 500;
        Bmp[RIVER_START_1].rcSrc.right = Bmp[RIVER_START_1].rcSrc.left + Bmp[RIVER_START_1].Breite;
        Bmp[RIVER_START_1].rcSrc.top = 0;
        Bmp[RIVER_START_1].rcSrc.bottom = Bmp[RIVER_START_1].rcSrc.top + Bmp[RIVER_START_1].Hoehe;
        Bmp[RIVER_START_1].rcDes.left = 16;
        Bmp[RIVER_START_1].rcDes.right = Bmp[RIVER_START_1].rcDes.left + Bmp[RIVER_START_1].Breite;
        Bmp[RIVER_START_1].rcDes.top = 19;
        Bmp[RIVER_START_1].rcDes.bottom = Bmp[RIVER_START_1].rcDes.top + Bmp[RIVER_START_1].Hoehe;

        Bmp[RIVER_START_2].Breite = 26;
        Bmp[RIVER_START_2].Hoehe = 19;
        Bmp[RIVER_START_2].rcSrc.left = 530;
        Bmp[RIVER_START_2].rcSrc.right = Bmp[RIVER_START_2].rcSrc.left + Bmp[RIVER_START_2].Breite;
        Bmp[RIVER_START_2].rcSrc.top = 0;
        Bmp[RIVER_START_2].rcSrc.bottom = Bmp[RIVER_START_2].rcSrc.top + Bmp[RIVER_START_2].Hoehe;
        Bmp[RIVER_START_2].rcDes.left = 9;
        Bmp[RIVER_START_2].rcDes.right = Bmp[RIVER_START_2].rcDes.left + Bmp[RIVER_START_2].Breite;
        Bmp[RIVER_START_2].rcDes.top = 20;
        Bmp[RIVER_START_2].rcDes.bottom = Bmp[RIVER_START_2].rcDes.top + Bmp[RIVER_START_2].Hoehe;

        Bmp[RIVER_START_3].Breite = 25;
        Bmp[RIVER_START_3].Hoehe = 16;
        Bmp[RIVER_START_3].rcSrc.left = 556;
        Bmp[RIVER_START_3].rcSrc.right = Bmp[RIVER_START_3].rcSrc.left + Bmp[RIVER_START_3].Breite;
        Bmp[RIVER_START_3].rcSrc.top = 0;
        Bmp[RIVER_START_3].rcSrc.bottom = Bmp[RIVER_START_3].rcSrc.top + Bmp[RIVER_START_3].Hoehe;
        Bmp[RIVER_START_3].rcDes.left = 19;
        Bmp[RIVER_START_3].rcDes.right = Bmp[RIVER_START_3].rcDes.left + Bmp[RIVER_START_3].Breite;
        Bmp[RIVER_START_3].rcDes.top = 19;
        Bmp[RIVER_START_3].rcDes.bottom = Bmp[RIVER_START_3].rcDes.top + Bmp[RIVER_START_3].Hoehe;

        Bmp[RIVER_START_4].Breite = 25;
        Bmp[RIVER_START_4].Hoehe = 15;
        Bmp[RIVER_START_4].rcSrc.left = 581;
        Bmp[RIVER_START_4].rcSrc.right = Bmp[RIVER_START_4].rcSrc.left + Bmp[RIVER_START_4].Breite;
        Bmp[RIVER_START_4].rcSrc.top = 0;
        Bmp[RIVER_START_4].rcSrc.bottom = Bmp[RIVER_START_4].rcSrc.top + Bmp[RIVER_START_4].Hoehe;
        Bmp[RIVER_START_4].rcDes.left = 8;
        Bmp[RIVER_START_4].rcDes.right = Bmp[RIVER_START_4].rcDes.left + Bmp[RIVER_START_4].Breite;
        Bmp[RIVER_START_4].rcDes.top = 21;
        Bmp[RIVER_START_4].rcDes.bottom = Bmp[RIVER_START_4].rcDes.top + Bmp[RIVER_START_4].Hoehe;

        Bmp[FLOODGATE_1].Breite = 35;
        Bmp[FLOODGATE_1].Hoehe = 22;
        Bmp[FLOODGATE_1].rcSrc.left = 164;
        Bmp[FLOODGATE_1].rcSrc.right = Bmp[FLOODGATE_1].rcSrc.left + Bmp[FLOODGATE_1].Breite;
        Bmp[FLOODGATE_1].rcSrc.top = 126;
        Bmp[FLOODGATE_1].rcSrc.bottom = Bmp[FLOODGATE_1].rcSrc.top + Bmp[FLOODGATE_1].Hoehe;
        Bmp[FLOODGATE_1].rcDes.left = 10;
        Bmp[FLOODGATE_1].rcDes.right = Bmp[FLOODGATE_1].rcDes.left + Bmp[FLOODGATE_1].Breite;
        Bmp[FLOODGATE_1].rcDes.top = 17;
        Bmp[FLOODGATE_1].rcDes.bottom = Bmp[FLOODGATE_1].rcDes.top + Bmp[FLOODGATE_1].Hoehe;

        Bmp[FLOODGATE_2].Breite = 34;
        Bmp[FLOODGATE_2].Hoehe = 23;
        Bmp[FLOODGATE_2].rcSrc.left = 199;
        Bmp[FLOODGATE_2].rcSrc.right = Bmp[FLOODGATE_2].rcSrc.left + Bmp[FLOODGATE_2].Breite;
        Bmp[FLOODGATE_2].rcSrc.top = 126;
        Bmp[FLOODGATE_2].rcSrc.bottom = Bmp[FLOODGATE_2].rcSrc.top + Bmp[FLOODGATE_2].Hoehe;
        Bmp[FLOODGATE_2].rcDes.left = 10;
        Bmp[FLOODGATE_2].rcDes.right = Bmp[FLOODGATE_2].rcDes.left + Bmp[FLOODGATE_2].Breite;
        Bmp[FLOODGATE_2].rcDes.top = 16;
        Bmp[FLOODGATE_2].rcDes.bottom = Bmp[FLOODGATE_2].rcDes.top + Bmp[FLOODGATE_2].Hoehe;

        Bmp[FLOODGATE_3].Breite = 34;
        Bmp[FLOODGATE_3].Hoehe = 22;
        Bmp[FLOODGATE_3].rcSrc.left = 233;
        Bmp[FLOODGATE_3].rcSrc.right = Bmp[FLOODGATE_3].rcSrc.left + Bmp[FLOODGATE_3].Breite;
        Bmp[FLOODGATE_3].rcSrc.top = 126;
        Bmp[FLOODGATE_3].rcSrc.bottom = Bmp[FLOODGATE_3].rcSrc.top + Bmp[FLOODGATE_3].Hoehe;
        Bmp[FLOODGATE_3].rcDes.left = 10;
        Bmp[FLOODGATE_3].rcDes.right = Bmp[FLOODGATE_3].rcDes.left + Bmp[FLOODGATE_3].Breite;
        Bmp[FLOODGATE_3].rcDes.top = 17;
        Bmp[FLOODGATE_3].rcDes.bottom = Bmp[FLOODGATE_3].rcDes.top + Bmp[FLOODGATE_3].Hoehe;

        Bmp[FLOODGATE_4].Breite = 33;
        Bmp[FLOODGATE_4].Hoehe = 23;
        Bmp[FLOODGATE_4].rcSrc.left = 268;
        Bmp[FLOODGATE_4].rcSrc.right = Bmp[FLOODGATE_4].rcSrc.left + Bmp[FLOODGATE_4].Breite;
        Bmp[FLOODGATE_4].rcSrc.top = 105;
        Bmp[FLOODGATE_4].rcSrc.bottom = Bmp[FLOODGATE_4].rcSrc.top + Bmp[FLOODGATE_4].Hoehe;
        Bmp[FLOODGATE_4].rcDes.left = 11;
        Bmp[FLOODGATE_4].rcDes.right = Bmp[FLOODGATE_4].rcDes.left + Bmp[FLOODGATE_4].Breite;
        Bmp[FLOODGATE_4].rcDes.top = 16;
        Bmp[FLOODGATE_4].rcDes.bottom = Bmp[FLOODGATE_4].rcDes.top + Bmp[FLOODGATE_4].Hoehe;

        Bmp[FLOODGATE_5].Breite = 34;
        Bmp[FLOODGATE_5].Hoehe = 17;
        Bmp[FLOODGATE_5].rcSrc.left = 302;
        Bmp[FLOODGATE_5].rcSrc.right = Bmp[FLOODGATE_5].rcSrc.left + Bmp[FLOODGATE_5].Breite;
        Bmp[FLOODGATE_5].rcSrc.top = 91;
        Bmp[FLOODGATE_5].rcSrc.bottom = Bmp[FLOODGATE_5].rcSrc.top + Bmp[FLOODGATE_5].Hoehe;
        Bmp[FLOODGATE_5].rcDes.left = 10;
        Bmp[FLOODGATE_5].rcDes.right = Bmp[FLOODGATE_5].rcDes.left + Bmp[FLOODGATE_5].Breite;
        Bmp[FLOODGATE_5].rcDes.top = 20;
        Bmp[FLOODGATE_5].rcDes.bottom = Bmp[FLOODGATE_5].rcDes.top + Bmp[FLOODGATE_5].Hoehe;

        Bmp[FLOODGATE_6].Breite = 35;
        Bmp[FLOODGATE_6].Hoehe = 23;
        Bmp[FLOODGATE_6].rcSrc.left = 336;
        Bmp[FLOODGATE_6].rcSrc.right = Bmp[FLOODGATE_6].rcSrc.left + Bmp[FLOODGATE_6].Breite;
        Bmp[FLOODGATE_6].rcSrc.top = 154;
        Bmp[FLOODGATE_6].rcSrc.bottom = Bmp[FLOODGATE_6].rcSrc.top + Bmp[FLOODGATE_6].Hoehe;
        Bmp[FLOODGATE_6].rcDes.left = 10;
        Bmp[FLOODGATE_6].rcDes.right = Bmp[FLOODGATE_6].rcDes.left + Bmp[FLOODGATE_6].Breite;
        Bmp[FLOODGATE_6].rcDes.top = 16;
        Bmp[FLOODGATE_6].rcDes.bottom = Bmp[FLOODGATE_6].rcDes.top + Bmp[FLOODGATE_6].Hoehe;

        // Bauwerke
        for (i = FIELD; i <= BONFIRE; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Geschwindigkeit = 0;
            Bmp[i].Phase = 0;
        }

        Bmp[FIELD].Anzahl = 3;
        Bmp[FIELD].Surface = lpDDSBau;
        Bmp[FIELD].Breite = 42;
        Bmp[FIELD].Hoehe = 27;
        Bmp[FIELD].rcSrc.left = 0;
        Bmp[FIELD].rcSrc.right = 0 + Bmp[FIELD].Breite;
        Bmp[FIELD].rcSrc.top = 0;
        Bmp[FIELD].rcSrc.bottom = 0 + Bmp[FIELD].Hoehe;
        Bmp[FIELD].rcDes.left = 5;
        Bmp[FIELD].rcDes.right = Bmp[FIELD].rcDes.left + Bmp[FIELD].Breite;
        Bmp[FIELD].rcDes.top = 15;
        Bmp[FIELD].rcDes.bottom = Bmp[FIELD].rcDes.top + Bmp[FIELD].Hoehe;
        Bmp[FIELD].AkAnzahl = 20;

        Bmp[TENT].Anzahl = 1;
        Bmp[TENT].Surface = lpDDSBau;
        Bmp[TENT].Breite = 23;
        Bmp[TENT].Hoehe = 20;
        Bmp[TENT].rcSrc.left = 42;
        Bmp[TENT].rcSrc.right = 42 + Bmp[TENT].Breite;
        Bmp[TENT].rcSrc.top = 0;
        Bmp[TENT].rcSrc.bottom = 0 + Bmp[TENT].Hoehe;
        Bmp[TENT].rcDes.left = 14;
        Bmp[TENT].rcDes.right = Bmp[TENT].rcDes.left + Bmp[TENT].Breite;
        Bmp[TENT].rcDes.top = 9;
        Bmp[TENT].rcDes.bottom = Bmp[TENT].rcDes.top + Bmp[TENT].Hoehe;
        Bmp[TENT].Rohstoff[RAW_TREE_BRANCH] = 5;
        Bmp[TENT].Rohstoff[RAW_LEAF] = 5;
        Bmp[TENT].AkAnzahl = 16;

        Bmp[BOAT].Anzahl = 2;
        Bmp[BOAT].Surface = lpDDSBau;
        Bmp[BOAT].Breite = 26;
        Bmp[BOAT].Hoehe = 18;
        Bmp[BOAT].rcSrc.left = 65;
        Bmp[BOAT].rcSrc.right = 65 + Bmp[BOAT].Breite;
        Bmp[BOAT].rcSrc.top = 0;
        Bmp[BOAT].rcSrc.bottom = 0 + Bmp[BOAT].Hoehe;
        Bmp[BOAT].rcDes.left = 14;
        Bmp[BOAT].rcDes.right = Bmp[BOAT].rcDes.left + Bmp[BOAT].Breite;
        Bmp[BOAT].rcDes.top = 20;
        Bmp[BOAT].rcDes.bottom = Bmp[BOAT].rcDes.top + Bmp[BOAT].Hoehe;
        Bmp[BOAT].Rohstoff[RAW_TREE_BRANCH] = 2;
        Bmp[BOAT].Rohstoff[RAW_TREE_TRUNK] = 1;
        Bmp[BOAT].AkAnzahl = 16;

        Bmp[PIPE].Anzahl = 2;
        Bmp[PIPE].Surface = lpDDSBau;
        Bmp[PIPE].Breite = 34;
        Bmp[PIPE].Hoehe = 21;
        Bmp[PIPE].rcSrc.left = 91;
        Bmp[PIPE].rcSrc.right = 91 + Bmp[PIPE].Breite;
        Bmp[PIPE].rcSrc.top = 0;
        Bmp[PIPE].rcSrc.bottom = 0 + Bmp[PIPE].Hoehe;
        Bmp[PIPE].rcDes.left = 11;
        Bmp[PIPE].rcDes.right = Bmp[PIPE].rcDes.left + Bmp[PIPE].Breite;
        Bmp[PIPE].rcDes.top = 16;
        Bmp[PIPE].rcDes.bottom = Bmp[PIPE].rcDes.top + Bmp[PIPE].Hoehe;
        Bmp[PIPE].Rohstoff[RAW_TREE_TRUNK] = 1;
        Bmp[PIPE].AkAnzahl = 18;

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
        Bmp[SOS].Rohstoff[RAW_STONE] = 10;
        Bmp[SOS].AkAnzahl = 20;

        Bmp[HOUSE_1].Anzahl = 1;
        Bmp[HOUSE_1].Surface = lpDDSBau;
        Bmp[HOUSE_1].Breite = 26;
        Bmp[HOUSE_1].Hoehe = 41;
        Bmp[HOUSE_1].rcSrc.left = 161;
        Bmp[HOUSE_1].rcSrc.right = 161 + Bmp[HOUSE_1].Breite;
        Bmp[HOUSE_1].rcSrc.top = 0;
        Bmp[HOUSE_1].rcSrc.bottom = 0 + Bmp[HOUSE_1].Hoehe;
        Bmp[HOUSE_1].rcDes.left = 0;
        Bmp[HOUSE_1].rcDes.right = Bmp[HOUSE_1].rcDes.left + Bmp[HOUSE_1].Breite;
        Bmp[HOUSE_1].rcDes.top = 0;
        Bmp[HOUSE_1].rcDes.bottom = Bmp[HOUSE_1].rcDes.top + Bmp[HOUSE_1].Hoehe;
        Bmp[HOUSE_1].Rohstoff[RAW_TREE_BRANCH] = 4;
        Bmp[HOUSE_1].AkAnzahl = 19;
        Bmp[HOUSE_1].Sound = Sound::FOREST;

        Bmp[HOUSE_2].Anzahl = 1;
        Bmp[HOUSE_2].Surface = lpDDSBau;
        Bmp[HOUSE_2].Breite = 34;
        Bmp[HOUSE_2].Hoehe = 41;
        Bmp[HOUSE_2].rcSrc.left = 187;
        Bmp[HOUSE_2].rcSrc.right = 187 + Bmp[HOUSE_2].Breite;
        Bmp[HOUSE_2].rcSrc.top = 0;
        Bmp[HOUSE_2].rcSrc.bottom = 0 + Bmp[HOUSE_2].Hoehe;
        Bmp[HOUSE_2].rcDes.left = 0;
        Bmp[HOUSE_2].rcDes.right = Bmp[HOUSE_2].rcDes.left + Bmp[HOUSE_2].Breite;
        Bmp[HOUSE_2].rcDes.top = 0;
        Bmp[HOUSE_2].rcDes.bottom = Bmp[HOUSE_2].rcDes.top + Bmp[HOUSE_2].Hoehe;
        Bmp[HOUSE_2].Rohstoff[RAW_TREE_BRANCH] = 3;
        Bmp[HOUSE_2].Rohstoff[RAW_TREE_TRUNK] = 3;
        Bmp[HOUSE_2].AkAnzahl = 21;
        Bmp[HOUSE_2].Sound = Sound::FOREST;

        Bmp[HOUSE_3].Anzahl = 1;
        Bmp[HOUSE_3].Surface = lpDDSBau;
        Bmp[HOUSE_3].Breite = 34;
        Bmp[HOUSE_3].Hoehe = 41;
        Bmp[HOUSE_3].rcSrc.left = 221;
        Bmp[HOUSE_3].rcSrc.right = 221 + Bmp[HOUSE_3].Breite;
        Bmp[HOUSE_3].rcSrc.top = 0;
        Bmp[HOUSE_3].rcSrc.bottom = 0 + Bmp[HOUSE_3].Hoehe;
        Bmp[HOUSE_3].rcDes.left = 0;
        Bmp[HOUSE_3].rcDes.right = Bmp[HOUSE_3].rcDes.left + Bmp[HOUSE_3].Breite;
        Bmp[HOUSE_3].rcDes.top = 0;
        Bmp[HOUSE_3].rcDes.bottom = Bmp[HOUSE_3].rcDes.top + Bmp[HOUSE_3].Hoehe;
        Bmp[HOUSE_3].Rohstoff[RAW_TREE_BRANCH] = 4;
        Bmp[HOUSE_3].Rohstoff[RAW_TREE_TRUNK] = 4;
        Bmp[HOUSE_3].Rohstoff[RAW_LEAF] = 5;
        Bmp[HOUSE_3].AkAnzahl = 21;
        Bmp[HOUSE_3].Sound = Sound::FOREST;

        Bmp[BONFIRE].Anzahl = 1;
        Bmp[BONFIRE].Surface = lpDDSBau;
        Bmp[BONFIRE].Breite = 21;
        Bmp[BONFIRE].Hoehe = 19;
        Bmp[BONFIRE].rcSrc.left = 255;
        Bmp[BONFIRE].rcSrc.right = 255 + Bmp[BONFIRE].Breite;
        Bmp[BONFIRE].rcSrc.top = 0;
        Bmp[BONFIRE].rcSrc.bottom = 0 + Bmp[BONFIRE].Hoehe;
        Bmp[BONFIRE].rcDes.left = 15;
        Bmp[BONFIRE].rcDes.right = Bmp[BONFIRE].rcDes.left + Bmp[BONFIRE].Breite;
        Bmp[BONFIRE].rcDes.top = 10;
        Bmp[BONFIRE].rcDes.bottom = Bmp[BONFIRE].rcDes.top + Bmp[BONFIRE].Hoehe;
        Bmp[BONFIRE].Rohstoff[RAW_TREE_BRANCH] = 5;
        Bmp[BONFIRE].Rohstoff[RAW_TREE_TRUNK] = 4;
        Bmp[BONFIRE].AkAnzahl = 9;

        // Allgemein Bäume
        for (i = TREE_1; i <= TREE_4; i++)
        {
            Bmp[i].Animation = true;
            Bmp[i].Anzahl = 4;
            Bmp[i].Geschwindigkeit = 4;
            Bmp[i].Phase = 0;
            Bmp[i].Surface = lpDDSBaum;
            Bmp[i].Sound = Sound::FOREST;
        }
        // Baum1
        Bmp[TREE_1].rcSrc.left = 0;
        Bmp[TREE_1].rcSrc.top = 0;
        Bmp[TREE_1].rcSrc.right = 21;
        Bmp[TREE_1].rcSrc.bottom = 26;
        Bmp[TREE_1].Breite = static_cast<short>(Bmp[TREE_1].rcSrc.right - Bmp[TREE_1].rcSrc.left);
        Bmp[TREE_1].Hoehe = static_cast<short>(Bmp[TREE_1].rcSrc.bottom - Bmp[TREE_1].rcSrc.top);

        // Baum2
        Bmp[TREE_2].rcSrc.left = 21;
        Bmp[TREE_2].rcSrc.top = 0;
        Bmp[TREE_2].rcSrc.right = 42;
        Bmp[TREE_2].rcSrc.bottom = 26;
        Bmp[TREE_2].Breite = static_cast<short>(Bmp[TREE_2].rcSrc.right - Bmp[TREE_2].rcSrc.left);
        Bmp[TREE_2].Hoehe = static_cast<short>(Bmp[TREE_2].rcSrc.bottom - Bmp[TREE_2].rcSrc.top);

        // Baum3
        Bmp[TREE_3].rcSrc.left = 42;
        Bmp[TREE_3].rcSrc.top = 0;
        Bmp[TREE_3].rcSrc.right = 64;
        Bmp[TREE_3].rcSrc.bottom = 27;
        Bmp[TREE_3].Breite = static_cast<short>(Bmp[TREE_3].rcSrc.right - Bmp[TREE_3].rcSrc.left);
        Bmp[TREE_3].Hoehe = static_cast<short>(Bmp[TREE_3].rcSrc.bottom - Bmp[TREE_3].rcSrc.top);

        // Baum4
        Bmp[TREE_4].rcSrc.left = 64;
        Bmp[TREE_4].rcSrc.top = 0;
        Bmp[TREE_4].rcSrc.right = 81;
        Bmp[TREE_4].rcSrc.bottom = 16;
        Bmp[TREE_4].Breite = static_cast<short>(Bmp[TREE_4].rcSrc.right - Bmp[TREE_4].rcSrc.left);
        Bmp[TREE_4].Hoehe = static_cast<short>(Bmp[TREE_4].rcSrc.bottom - Bmp[TREE_4].rcSrc.top);

        // Baumgroß
        Bmp[TREE_BIG].rcSrc.left = 238;
        Bmp[TREE_BIG].rcSrc.top = 0;
        Bmp[TREE_BIG].rcSrc.right = 238 + 26;
        Bmp[TREE_BIG].rcSrc.bottom = 41;
        Bmp[TREE_BIG].Breite = static_cast<short>(Bmp[TREE_BIG].rcSrc.right - Bmp[TREE_BIG].rcSrc.left);
        Bmp[TREE_BIG].Hoehe = static_cast<short>(Bmp[TREE_BIG].rcSrc.bottom - Bmp[TREE_BIG].rcSrc.top);
        Bmp[TREE_BIG].Animation = false;
        Bmp[TREE_BIG].Anzahl = 1;
        Bmp[TREE_BIG].Geschwindigkeit = 0;
        Bmp[TREE_BIG].Phase = 0;
        Bmp[TREE_BIG].Surface = lpDDSBaum;
        Bmp[TREE_BIG].Sound = Sound::FOREST;

        // Feuer
        Bmp[FIRE].Anzahl = 3;
        Bmp[FIRE].Surface = lpDDSBau;
        Bmp[FIRE].Breite = 21;
        Bmp[FIRE].Hoehe = 28;
        Bmp[FIRE].rcSrc.left = 276;
        Bmp[FIRE].rcSrc.right = 276 + Bmp[FIRE].Breite;
        Bmp[FIRE].rcSrc.top = 0;
        Bmp[FIRE].rcSrc.bottom = 0 + Bmp[FIRE].Hoehe;
        Bmp[FIRE].rcDes.left = 15;
        Bmp[FIRE].rcDes.right = Bmp[FIRE].rcDes.left + Bmp[FIRE].Breite;
        Bmp[FIRE].rcDes.top = 1;
        Bmp[FIRE].rcDes.bottom = Bmp[FIRE].rcDes.top + Bmp[FIRE].Hoehe;
        Bmp[FIRE].Animation = true;
        Bmp[FIRE].Geschwindigkeit = 6;
        Bmp[FIRE].Phase = 0;
        Bmp[FIRE].Sound = Sound::FIRE;

        // WRACK
        Bmp[WRECK_1].Anzahl = 3;
        Bmp[WRECK_1].Surface = lpDDSBau;
        Bmp[WRECK_1].Breite = 24;
        Bmp[WRECK_1].Hoehe = 18;
        Bmp[WRECK_1].rcSrc.left = 391;
        Bmp[WRECK_1].rcSrc.right = 391 + Bmp[WRECK_1].Breite;
        Bmp[WRECK_1].rcSrc.top = 0;
        Bmp[WRECK_1].rcSrc.bottom = 0 + Bmp[WRECK_1].Hoehe;
        Bmp[WRECK_1].rcDes.left = 15;
        Bmp[WRECK_1].rcDes.right = Bmp[WRECK_1].rcDes.left + Bmp[WRECK_1].Breite;
        Bmp[WRECK_1].rcDes.top = 20;
        Bmp[WRECK_1].rcDes.bottom = Bmp[WRECK_1].rcDes.top + Bmp[WRECK_1].Hoehe;
        Bmp[WRECK_1].Animation = true;
        Bmp[WRECK_1].Geschwindigkeit = 5;
        Bmp[WRECK_1].Phase = 0;

        // WRACK2
        Bmp[WRECK_2].Anzahl = 3;
        Bmp[WRECK_2].Surface = lpDDSBau;
        Bmp[WRECK_2].Breite = 27;
        Bmp[WRECK_2].Hoehe = 26;
        Bmp[WRECK_2].rcSrc.left = 415;
        Bmp[WRECK_2].rcSrc.right = 415 + Bmp[WRECK_2].Breite;
        Bmp[WRECK_2].rcSrc.top = 0;
        Bmp[WRECK_2].rcSrc.bottom = 0 + Bmp[WRECK_2].Hoehe;
        Bmp[WRECK_2].rcDes.left = 15;
        Bmp[WRECK_2].rcDes.right = Bmp[WRECK_2].rcDes.left + Bmp[WRECK_2].Breite;
        Bmp[WRECK_2].rcDes.top = 16;
        Bmp[WRECK_2].rcDes.bottom = Bmp[WRECK_2].rcDes.top + Bmp[WRECK_2].Hoehe;
        Bmp[WRECK_2].Animation = true;
        Bmp[WRECK_2].Geschwindigkeit = 5;
        Bmp[WRECK_2].Phase = 0;

        // Früchte
        // Busch
        Bmp[BUSH].rcSrc.left = 81;
        Bmp[BUSH].rcSrc.top = 0;
        Bmp[BUSH].rcSrc.right = 81 + 20;
        Bmp[BUSH].rcSrc.bottom = 13;
        Bmp[BUSH].Breite = static_cast<short>(Bmp[BUSH].rcSrc.right - Bmp[BUSH].rcSrc.left);
        Bmp[BUSH].Hoehe = static_cast<short>(Bmp[BUSH].rcSrc.bottom - Bmp[BUSH].rcSrc.top);
        Bmp[BUSH].Animation = false;
        Bmp[BUSH].Anzahl = 3;
        Bmp[BUSH].Geschwindigkeit = 0;
        Bmp[BUSH].Phase = 0;
        Bmp[BUSH].Surface = lpDDSBaum;

        // Buttons

        // StandardBmponsinitialisierung
        for (i = BUTTON_GRID; i <= BUTTON_DESTROY; i++)
        {
            Bmp[i].Animation = false;
            Bmp[i].Surface = lpDDSButtons;
            Bmp[i].Anzahl = 1;
            Bmp[i].Phase = 0;
        }

        // ButtGitter
        Bmp[BUTTON_GRID].rcSrc.left = 0;
        Bmp[BUTTON_GRID].rcSrc.top = 0;
        Bmp[BUTTON_GRID].rcSrc.right = 28;
        Bmp[BUTTON_GRID].rcSrc.bottom = 28;
        Bmp[BUTTON_GRID].rcDes.left = rcPanel.left + 173;
        Bmp[BUTTON_GRID].rcDes.top = rcPanel.top + 26;
        Bmp[BUTTON_GRID].rcDes.right = Bmp[BUTTON_GRID].rcDes.left + 28;
        Bmp[BUTTON_GRID].rcDes.bottom = Bmp[BUTTON_GRID].rcDes.top + 28;
        Bmp[BUTTON_GRID].Breite = static_cast<short>(Bmp[BUTTON_GRID].rcSrc.right - Bmp[BUTTON_GRID].rcSrc.left);
        Bmp[BUTTON_GRID].Hoehe = static_cast<short>(Bmp[BUTTON_GRID].rcSrc.bottom - Bmp[BUTTON_GRID].rcSrc.top);
        Bmp[BUTTON_GRID].Anzahl = 2;

        // BUTTANIMATION
        Bmp[BUTTON_ANIMATION].rcSrc.left = 0;
        Bmp[BUTTON_ANIMATION].rcSrc.top = 56;
        Bmp[BUTTON_ANIMATION].rcSrc.right = 28;
        Bmp[BUTTON_ANIMATION].rcSrc.bottom = 56 + 28;
        Bmp[BUTTON_ANIMATION].rcDes.left = rcPanel.left + 173;
        Bmp[BUTTON_ANIMATION].rcDes.top = rcPanel.top + 60;
        Bmp[BUTTON_ANIMATION].rcDes.right = Bmp[BUTTON_ANIMATION].rcDes.left + 28;
        Bmp[BUTTON_ANIMATION].rcDes.bottom = Bmp[BUTTON_ANIMATION].rcDes.top + 28;
        Bmp[BUTTON_ANIMATION].Breite = static_cast<short>(Bmp[BUTTON_ANIMATION].rcSrc.right - Bmp[BUTTON_ANIMATION].rcSrc.left);
        Bmp[BUTTON_ANIMATION].Hoehe = static_cast<short>(Bmp[BUTTON_ANIMATION].rcSrc.bottom - Bmp[BUTTON_ANIMATION].rcSrc.top);
        Bmp[BUTTON_ANIMATION].Anzahl = 2;

        // BUTTBEENDEN
        Bmp[BUTTON_END].rcSrc.left = 0;
        Bmp[BUTTON_END].rcSrc.top = 112;
        Bmp[BUTTON_END].rcSrc.right = 20;
        Bmp[BUTTON_END].rcSrc.bottom = 112 + 20;
        Bmp[BUTTON_END].rcDes.left = rcPanel.left + 60;
        Bmp[BUTTON_END].rcDes.top = rcPanel.top + 2;
        Bmp[BUTTON_END].rcDes.right = Bmp[BUTTON_END].rcDes.left + 20;
        Bmp[BUTTON_END].rcDes.bottom = Bmp[BUTTON_END].rcDes.top + 20;
        Bmp[BUTTON_END].Breite = static_cast<short>(Bmp[BUTTON_END].rcSrc.right - Bmp[BUTTON_END].rcSrc.left);
        Bmp[BUTTON_END].Hoehe = static_cast<short>(Bmp[BUTTON_END].rcSrc.bottom - Bmp[BUTTON_END].rcSrc.top);
        Bmp[BUTTON_END].Anzahl = 4;
        Bmp[BUTTON_END].Geschwindigkeit = 4;

        // BUTTNEU
        Bmp[BUTTON_NEW].rcSrc.left = 0;
        Bmp[BUTTON_NEW].rcSrc.top = 192;
        Bmp[BUTTON_NEW].rcSrc.right = 20;
        Bmp[BUTTON_NEW].rcSrc.bottom = 192 + 20;
        Bmp[BUTTON_NEW].rcDes.left = rcPanel.left + 100;
        Bmp[BUTTON_NEW].rcDes.top = rcPanel.top + 2;
        Bmp[BUTTON_NEW].rcDes.right = Bmp[BUTTON_NEW].rcDes.left + 20;
        Bmp[BUTTON_NEW].rcDes.bottom = Bmp[BUTTON_NEW].rcDes.top + 20;
        Bmp[BUTTON_NEW].Breite = static_cast<short>(Bmp[BUTTON_NEW].rcSrc.right - Bmp[BUTTON_NEW].rcSrc.left);
        Bmp[BUTTON_NEW].Hoehe = static_cast<short>(Bmp[BUTTON_NEW].rcSrc.bottom - Bmp[BUTTON_NEW].rcSrc.top);
        Bmp[BUTTON_NEW].Anzahl = 2;
        Bmp[BUTTON_NEW].Geschwindigkeit = 3;


        // BUTTTAGNEU
        Bmp[BUTTON_NEW_DAY].rcSrc.left = 0;
        Bmp[BUTTON_NEW_DAY].rcSrc.top = 232;
        Bmp[BUTTON_NEW_DAY].rcSrc.right = 20;
        Bmp[BUTTON_NEW_DAY].rcSrc.bottom = 232 + 20;
        Bmp[BUTTON_NEW_DAY].rcDes.left = rcPanel.left + 140;
        Bmp[BUTTON_NEW_DAY].rcDes.top = rcPanel.top + 2;
        Bmp[BUTTON_NEW_DAY].rcDes.right = Bmp[BUTTON_NEW_DAY].rcDes.left + 20;
        Bmp[BUTTON_NEW_DAY].rcDes.bottom = Bmp[BUTTON_NEW_DAY].rcDes.top + 20;
        Bmp[BUTTON_NEW_DAY].Breite = static_cast<short>(Bmp[BUTTON_NEW_DAY].rcSrc.right - Bmp[BUTTON_NEW_DAY].rcSrc.left);
        Bmp[BUTTON_NEW_DAY].Hoehe = static_cast<short>(Bmp[BUTTON_NEW_DAY].rcSrc.bottom - Bmp[BUTTON_NEW_DAY].rcSrc.top);
        Bmp[BUTTON_NEW_DAY].Anzahl = 2;
        Bmp[BUTTON_NEW_DAY].Geschwindigkeit = 2;

        // BUTTSOUND
        Bmp[BUTTON_SOUND].rcSrc.left = 0;
        Bmp[BUTTON_SOUND].rcSrc.top = 272;
        Bmp[BUTTON_SOUND].rcSrc.right = 28;
        Bmp[BUTTON_SOUND].rcSrc.bottom = 272 + 28;
        Bmp[BUTTON_SOUND].rcDes.left = rcPanel.left + 173;
        Bmp[BUTTON_SOUND].rcDes.top = rcPanel.top + 94;
        Bmp[BUTTON_SOUND].rcDes.right = Bmp[BUTTON_SOUND].rcDes.left + 28;
        Bmp[BUTTON_SOUND].rcDes.bottom = Bmp[BUTTON_SOUND].rcDes.top + 28;
        Bmp[BUTTON_SOUND].Breite = static_cast<short>(Bmp[BUTTON_SOUND].rcSrc.right - Bmp[BUTTON_SOUND].rcSrc.left);
        Bmp[BUTTON_SOUND].Hoehe = static_cast<short>(Bmp[BUTTON_SOUND].rcSrc.bottom - Bmp[BUTTON_SOUND].rcSrc.top);
        Bmp[BUTTON_SOUND].Anzahl = 2;

        // ButtAktion
        Bmp[BUTTON_ACTION].rcSrc.left = 28;
        Bmp[BUTTON_ACTION].rcSrc.top = 0;
        Bmp[BUTTON_ACTION].rcSrc.right = 28 + 35;
        Bmp[BUTTON_ACTION].rcSrc.bottom = 35;
        Bmp[BUTTON_ACTION].rcDes.left = rcPanel.left + 29;
        Bmp[BUTTON_ACTION].rcDes.top = rcPanel.top + 191;
        Bmp[BUTTON_ACTION].rcDes.right = Bmp[BUTTON_ACTION].rcDes.left + 35;
        Bmp[BUTTON_ACTION].rcDes.bottom = Bmp[BUTTON_ACTION].rcDes.top + 35;
        Bmp[BUTTON_ACTION].Breite = static_cast<short>(Bmp[BUTTON_ACTION].rcSrc.right - Bmp[BUTTON_ACTION].rcSrc.left);
        Bmp[BUTTON_ACTION].Hoehe = static_cast<short>(Bmp[BUTTON_ACTION].rcSrc.bottom - Bmp[BUTTON_ACTION].rcSrc.top);
        Bmp[BUTTON_ACTION].Anzahl = 3;
        Bmp[BUTTON_ACTION].Geschwindigkeit = 6;

        // BUTTBAUEN
        Bmp[BUTTON_CONSTRUCT].rcSrc.left = 203;
        Bmp[BUTTON_CONSTRUCT].rcSrc.top = 0;
        Bmp[BUTTON_CONSTRUCT].rcSrc.right = 203 + 35;
        Bmp[BUTTON_CONSTRUCT].rcSrc.bottom = 35;
        Bmp[BUTTON_CONSTRUCT].rcDes.left = rcPanel.left + 70;
        Bmp[BUTTON_CONSTRUCT].rcDes.top = rcPanel.top + 191;
        Bmp[BUTTON_CONSTRUCT].rcDes.right = Bmp[BUTTON_CONSTRUCT].rcDes.left + 35;
        Bmp[BUTTON_CONSTRUCT].rcDes.bottom = Bmp[BUTTON_CONSTRUCT].rcDes.top + 35;
        Bmp[BUTTON_CONSTRUCT].Breite = static_cast<short>(Bmp[BUTTON_CONSTRUCT].rcSrc.right - Bmp[BUTTON_CONSTRUCT].rcSrc.left);
        Bmp[BUTTON_CONSTRUCT].Hoehe = static_cast<short>(Bmp[BUTTON_CONSTRUCT].rcSrc.bottom - Bmp[BUTTON_CONSTRUCT].rcSrc.top);
        Bmp[BUTTON_CONSTRUCT].Anzahl = 4;
        Bmp[BUTTON_CONSTRUCT].Geschwindigkeit = 5;


        // BUTTINVENTAR
        Bmp[BUTTON_INVENTORY].rcSrc.left = 168;
        Bmp[BUTTON_INVENTORY].rcSrc.top = 0;
        Bmp[BUTTON_INVENTORY].rcSrc.right = 168 + 35;
        Bmp[BUTTON_INVENTORY].rcSrc.bottom = 35;
        Bmp[BUTTON_INVENTORY].rcDes.left = rcPanel.left + 152;
        Bmp[BUTTON_INVENTORY].rcDes.top = rcPanel.top + 191;
        Bmp[BUTTON_INVENTORY].rcDes.right = Bmp[BUTTON_INVENTORY].rcDes.left + 35;
        Bmp[BUTTON_INVENTORY].rcDes.bottom = Bmp[BUTTON_INVENTORY].rcDes.top + 35;
        Bmp[BUTTON_INVENTORY].Breite = static_cast<short>(Bmp[BUTTON_INVENTORY].rcSrc.right - Bmp[BUTTON_INVENTORY].rcSrc.left);
        Bmp[BUTTON_INVENTORY].Hoehe = static_cast<short>(Bmp[BUTTON_INVENTORY].rcSrc.bottom - Bmp[BUTTON_INVENTORY].rcSrc.top);
        Bmp[BUTTON_INVENTORY].Anzahl = 4;
        Bmp[BUTTON_INVENTORY].Geschwindigkeit = 4;

        // BUTTWEITER
        Bmp[BUTTON_CONTINUE].rcSrc.left = 343;
        Bmp[BUTTON_CONTINUE].rcSrc.top = 0;
        Bmp[BUTTON_CONTINUE].rcSrc.right = 343 + 35;
        Bmp[BUTTON_CONTINUE].rcSrc.bottom = 35;
        Bmp[BUTTON_CONTINUE].rcDes.left = rcPanel.left + 111;
        Bmp[BUTTON_CONTINUE].rcDes.top = rcPanel.top + 191;
        Bmp[BUTTON_CONTINUE].rcDes.right = Bmp[BUTTON_CONTINUE].rcDes.left + 35;
        Bmp[BUTTON_CONTINUE].rcDes.bottom = Bmp[BUTTON_CONTINUE].rcDes.top + 35;
        Bmp[BUTTON_CONTINUE].Breite = static_cast<short>(Bmp[BUTTON_CONTINUE].rcSrc.right - Bmp[BUTTON_CONTINUE].rcSrc.left);
        Bmp[BUTTON_CONTINUE].Hoehe = static_cast<short>(Bmp[BUTTON_CONTINUE].rcSrc.bottom - Bmp[BUTTON_CONTINUE].rcSrc.top);
        Bmp[BUTTON_CONTINUE].Anzahl = 4;
        Bmp[BUTTON_CONTINUE].Geschwindigkeit = 4;
        Bmp[BUTTON_CONTINUE].Phase = -1;

        // BUTTSTOP
        Bmp[BUTTON_STOP].rcSrc.left = 378;
        Bmp[BUTTON_STOP].rcSrc.top = 0;
        Bmp[BUTTON_STOP].rcSrc.right = 378 + 35;
        Bmp[BUTTON_STOP].rcSrc.bottom = 35;
        Bmp[BUTTON_STOP].rcDes.left = rcPanel.left + 111;
        Bmp[BUTTON_STOP].rcDes.top = rcPanel.top + 191;
        Bmp[BUTTON_STOP].rcDes.right = Bmp[BUTTON_STOP].rcDes.left + 35;
        Bmp[BUTTON_STOP].rcDes.bottom = Bmp[BUTTON_STOP].rcDes.top + 35;
        Bmp[BUTTON_STOP].Breite = static_cast<short>(Bmp[BUTTON_STOP].rcSrc.right - Bmp[BUTTON_STOP].rcSrc.left);
        Bmp[BUTTON_STOP].Hoehe = static_cast<short>(Bmp[BUTTON_STOP].rcSrc.bottom - Bmp[BUTTON_STOP].rcSrc.top);
        Bmp[BUTTON_STOP].Anzahl = 4;
        Bmp[BUTTON_STOP].Geschwindigkeit = 4;
        Bmp[BUTTON_STOP].Phase = -1;

        // BUTTABLEGEN
        Bmp[BUTTON_LAY_DOWN].rcSrc.left = 483;
        Bmp[BUTTON_LAY_DOWN].rcSrc.top = 0;
        Bmp[BUTTON_LAY_DOWN].rcSrc.right = 483 + 35;
        Bmp[BUTTON_LAY_DOWN].rcSrc.bottom = 35;
        Bmp[BUTTON_LAY_DOWN].rcDes.left = rcPanel.left + 111;
        Bmp[BUTTON_LAY_DOWN].rcDes.top = rcPanel.top + 191;
        Bmp[BUTTON_LAY_DOWN].rcDes.right = Bmp[BUTTON_LAY_DOWN].rcDes.left + 35;
        Bmp[BUTTON_LAY_DOWN].rcDes.bottom = Bmp[BUTTON_LAY_DOWN].rcDes.top + 35;
        Bmp[BUTTON_LAY_DOWN].Breite = static_cast<short>(Bmp[BUTTON_LAY_DOWN].rcSrc.right - Bmp[BUTTON_LAY_DOWN].rcSrc.left);
        Bmp[BUTTON_LAY_DOWN].Hoehe = static_cast<short>(Bmp[BUTTON_LAY_DOWN].rcSrc.bottom - Bmp[BUTTON_LAY_DOWN].rcSrc.top);
        Bmp[BUTTON_LAY_DOWN].Anzahl = 4;
        Bmp[BUTTON_LAY_DOWN].Geschwindigkeit = 3;
        Bmp[BUTTON_STOP].Phase = -1;

        // BUTTSUCHEN
        Bmp[BUTTON_SEARCH].rcSrc.left = 63;
        Bmp[BUTTON_SEARCH].rcSrc.top = 0;
        Bmp[BUTTON_SEARCH].rcSrc.right = 63 + 35;
        Bmp[BUTTON_SEARCH].rcSrc.bottom = 35;
        Bmp[BUTTON_SEARCH].rcDes.left = rcPanel.left + 29;
        Bmp[BUTTON_SEARCH].rcDes.top = rcPanel.top + 270;
        Bmp[BUTTON_SEARCH].rcDes.right = Bmp[BUTTON_SEARCH].rcDes.left + 35;
        Bmp[BUTTON_SEARCH].rcDes.bottom = Bmp[BUTTON_SEARCH].rcDes.top + 35;
        Bmp[BUTTON_SEARCH].Breite = static_cast<short>(Bmp[BUTTON_SEARCH].rcSrc.right - Bmp[BUTTON_SEARCH].rcSrc.left);
        Bmp[BUTTON_SEARCH].Hoehe = static_cast<short>(Bmp[BUTTON_SEARCH].rcSrc.bottom - Bmp[BUTTON_SEARCH].rcSrc.top);
        Bmp[BUTTON_SEARCH].Anzahl = 4;
        Bmp[BUTTON_SEARCH].Geschwindigkeit = 4;

        // BUTTESSEN
        Bmp[BUTTON_EAT].rcSrc.left = 133;
        Bmp[BUTTON_EAT].rcSrc.top = 0;
        Bmp[BUTTON_EAT].rcSrc.right = 133 + 35;
        Bmp[BUTTON_EAT].rcSrc.bottom = 35;
        Bmp[BUTTON_EAT].rcDes.left = rcPanel.left + 70;
        Bmp[BUTTON_EAT].rcDes.top = rcPanel.top + 270;
        Bmp[BUTTON_EAT].rcDes.right = Bmp[BUTTON_EAT].rcDes.left + 35;
        Bmp[BUTTON_EAT].rcDes.bottom = Bmp[BUTTON_EAT].rcDes.top + 35;
        Bmp[BUTTON_EAT].Breite = static_cast<short>(Bmp[BUTTON_EAT].rcSrc.right - Bmp[BUTTON_EAT].rcSrc.left);
        Bmp[BUTTON_EAT].Hoehe = static_cast<short>(Bmp[BUTTON_EAT].rcSrc.bottom - Bmp[BUTTON_EAT].rcSrc.top);
        Bmp[BUTTON_EAT].Anzahl = 4;
        Bmp[BUTTON_EAT].Geschwindigkeit = 4;

        // BUTTSCHLAFEN
        Bmp[BUTTON_SLEEP].rcSrc.left = 308;
        Bmp[BUTTON_SLEEP].rcSrc.top = 0;
        Bmp[BUTTON_SLEEP].rcSrc.right = 308 + 35;
        Bmp[BUTTON_SLEEP].rcSrc.bottom = 35;
        Bmp[BUTTON_SLEEP].rcDes.left = rcPanel.left + 111;
        Bmp[BUTTON_SLEEP].rcDes.top = rcPanel.top + 270;
        Bmp[BUTTON_SLEEP].rcDes.right = Bmp[BUTTON_SLEEP].rcDes.left + 35;
        Bmp[BUTTON_SLEEP].rcDes.bottom = Bmp[BUTTON_SLEEP].rcDes.top + 35;
        Bmp[BUTTON_SLEEP].Breite = static_cast<short>(Bmp[BUTTON_SLEEP].rcSrc.right - Bmp[BUTTON_SLEEP].rcSrc.left);
        Bmp[BUTTON_SLEEP].Hoehe = static_cast<short>(Bmp[BUTTON_SLEEP].rcSrc.bottom - Bmp[BUTTON_SLEEP].rcSrc.top);
        Bmp[BUTTON_SLEEP].Anzahl = 4;
        Bmp[BUTTON_SLEEP].Geschwindigkeit = 3;

        // BUTTFAELLEN
        Bmp[BUTTON_CHOP].rcSrc.left = 98;
        Bmp[BUTTON_CHOP].rcSrc.top = 0;
        Bmp[BUTTON_CHOP].rcSrc.right = 98 + 35;
        Bmp[BUTTON_CHOP].rcSrc.bottom = 35;
        Bmp[BUTTON_CHOP].rcDes.left = rcPanel.left + 152;
        Bmp[BUTTON_CHOP].rcDes.top = rcPanel.top + 270;
        Bmp[BUTTON_CHOP].rcDes.right = Bmp[BUTTON_CHOP].rcDes.left + 35;
        Bmp[BUTTON_CHOP].rcDes.bottom = Bmp[BUTTON_CHOP].rcDes.top + 35;
        Bmp[BUTTON_CHOP].Breite = static_cast<short>(Bmp[BUTTON_CHOP].rcSrc.right - Bmp[BUTTON_CHOP].rcSrc.left);
        Bmp[BUTTON_CHOP].Hoehe = static_cast<short>(Bmp[BUTTON_CHOP].rcSrc.bottom - Bmp[BUTTON_CHOP].rcSrc.top);
        Bmp[BUTTON_CHOP].Anzahl = 4;
        Bmp[BUTTON_CHOP].Geschwindigkeit = 4;
        Bmp[BUTTON_CHOP].Phase = -1;

        // BUTTANGELN
        Bmp[BUTTON_FISH].rcSrc.left = 413;
        Bmp[BUTTON_FISH].rcSrc.top = 0;
        Bmp[BUTTON_FISH].rcSrc.right = 413 + 35;
        Bmp[BUTTON_FISH].rcSrc.bottom = 35;
        Bmp[BUTTON_FISH].rcDes.left = rcPanel.left + 29;
        Bmp[BUTTON_FISH].rcDes.top = rcPanel.top + 325;
        Bmp[BUTTON_FISH].rcDes.right = Bmp[BUTTON_FISH].rcDes.left + 35;
        Bmp[BUTTON_FISH].rcDes.bottom = Bmp[BUTTON_FISH].rcDes.top + 35;
        Bmp[BUTTON_FISH].Breite = static_cast<short>(Bmp[BUTTON_FISH].rcSrc.right - Bmp[BUTTON_FISH].rcSrc.left);
        Bmp[BUTTON_FISH].Hoehe = static_cast<short>(Bmp[BUTTON_FISH].rcSrc.bottom - Bmp[BUTTON_FISH].rcSrc.top);
        Bmp[BUTTON_FISH].Anzahl = 4;
        Bmp[BUTTON_FISH].Geschwindigkeit = 3;
        Bmp[BUTTON_FISH].Phase = -1;

        // BUTTANZUENDEN
        Bmp[BUTTON_IGNITE].rcSrc.left = 28;
        Bmp[BUTTON_IGNITE].rcSrc.top = 175;
        Bmp[BUTTON_IGNITE].rcSrc.right = 28 + 35;
        Bmp[BUTTON_IGNITE].rcSrc.bottom = 175 + 35;
        Bmp[BUTTON_IGNITE].rcDes.left = rcPanel.left + 70;
        Bmp[BUTTON_IGNITE].rcDes.top = rcPanel.top + 325;
        Bmp[BUTTON_IGNITE].rcDes.right = Bmp[BUTTON_IGNITE].rcDes.left + 35;
        Bmp[BUTTON_IGNITE].rcDes.bottom = Bmp[BUTTON_IGNITE].rcDes.top + 35;
        Bmp[BUTTON_IGNITE].Breite = static_cast<short>(Bmp[BUTTON_IGNITE].rcSrc.right - Bmp[BUTTON_IGNITE].rcSrc.left);
        Bmp[BUTTON_IGNITE].Hoehe = static_cast<short>(Bmp[BUTTON_IGNITE].rcSrc.bottom - Bmp[BUTTON_IGNITE].rcSrc.top);
        Bmp[BUTTON_IGNITE].Anzahl = 3;
        Bmp[BUTTON_IGNITE].Geschwindigkeit = 4;
        Bmp[BUTTON_IGNITE].Phase = -1;

        // BUTTAUSSCHAU
        Bmp[BUTTON_LOOK_OUT].rcSrc.left = 63;
        Bmp[BUTTON_LOOK_OUT].rcSrc.top = 175;
        Bmp[BUTTON_LOOK_OUT].rcSrc.right = 63 + 35;
        Bmp[BUTTON_LOOK_OUT].rcSrc.bottom = 175 + 35;
        Bmp[BUTTON_LOOK_OUT].rcDes.left = rcPanel.left + 111;
        Bmp[BUTTON_LOOK_OUT].rcDes.top = rcPanel.top + 325;
        Bmp[BUTTON_LOOK_OUT].rcDes.right = Bmp[BUTTON_LOOK_OUT].rcDes.left + 35;
        Bmp[BUTTON_LOOK_OUT].rcDes.bottom = Bmp[BUTTON_LOOK_OUT].rcDes.top + 35;
        Bmp[BUTTON_LOOK_OUT].Breite = static_cast<short>(Bmp[BUTTON_LOOK_OUT].rcSrc.right - Bmp[BUTTON_LOOK_OUT].rcSrc.left);
        Bmp[BUTTON_LOOK_OUT].Hoehe = static_cast<short>(Bmp[BUTTON_LOOK_OUT].rcSrc.bottom - Bmp[BUTTON_LOOK_OUT].rcSrc.top);
        Bmp[BUTTON_LOOK_OUT].Anzahl = 4;
        Bmp[BUTTON_LOOK_OUT].Geschwindigkeit = 3;
        Bmp[BUTTON_LOOK_OUT].Phase = -1;

        // BUTTSCHATZKARTE
        Bmp[BUTTON_TREASUREMAP].rcSrc.left = 98;
        Bmp[BUTTON_TREASUREMAP].rcSrc.top = 175;
        Bmp[BUTTON_TREASUREMAP].rcSrc.right = 98 + 35;
        Bmp[BUTTON_TREASUREMAP].rcSrc.bottom = 175 + 35;
        Bmp[BUTTON_TREASUREMAP].rcDes.left = rcPanel.left + 152;
        Bmp[BUTTON_TREASUREMAP].rcDes.top = rcPanel.top + 325;
        Bmp[BUTTON_TREASUREMAP].rcDes.right = Bmp[BUTTON_TREASUREMAP].rcDes.left + 35;
        Bmp[BUTTON_TREASUREMAP].rcDes.bottom = Bmp[BUTTON_TREASUREMAP].rcDes.top + 35;
        Bmp[BUTTON_TREASUREMAP].Breite = static_cast<short>(Bmp[BUTTON_TREASUREMAP].rcSrc.right - Bmp[BUTTON_TREASUREMAP].rcSrc.left);
        Bmp[BUTTON_TREASUREMAP].Hoehe = static_cast<short>(Bmp[BUTTON_TREASUREMAP].rcSrc.bottom - Bmp[BUTTON_TREASUREMAP].rcSrc.top);
        Bmp[BUTTON_TREASUREMAP].Anzahl = 4;
        Bmp[BUTTON_TREASUREMAP].Geschwindigkeit = 3;
        Bmp[BUTTON_TREASUREMAP].Phase = -1;

        // BUTTSCHATZ
        Bmp[BUTTON_TREASURE].rcSrc.left = 133;
        Bmp[BUTTON_TREASURE].rcSrc.top = 175;
        Bmp[BUTTON_TREASURE].rcSrc.right = 133 + 35;
        Bmp[BUTTON_TREASURE].rcSrc.bottom = 175 + 35;
        Bmp[BUTTON_TREASURE].rcDes.left = rcPanel.left + 29;
        Bmp[BUTTON_TREASURE].rcDes.top = rcPanel.top + 380;
        Bmp[BUTTON_TREASURE].rcDes.right = Bmp[BUTTON_TREASURE].rcDes.left + 35;
        Bmp[BUTTON_TREASURE].rcDes.bottom = Bmp[BUTTON_TREASURE].rcDes.top + 35;
        Bmp[BUTTON_TREASURE].Breite = static_cast<short>(Bmp[BUTTON_TREASURE].rcSrc.right - Bmp[BUTTON_TREASURE].rcSrc.left);
        Bmp[BUTTON_TREASURE].Hoehe = static_cast<short>(Bmp[BUTTON_TREASURE].rcSrc.bottom - Bmp[BUTTON_TREASURE].rcSrc.top);
        Bmp[BUTTON_TREASURE].Anzahl = 4;
        Bmp[BUTTON_TREASURE].Geschwindigkeit = 3;
        Bmp[BUTTON_TREASURE].Phase = -1;

        // BUTTSCHLEUDER
        Bmp[BUTTON_SLINGSHOT].rcSrc.left = 168;
        Bmp[BUTTON_SLINGSHOT].rcSrc.top = 175;
        Bmp[BUTTON_SLINGSHOT].rcSrc.right = 168 + 35;
        Bmp[BUTTON_SLINGSHOT].rcSrc.bottom = 175 + 35;
        Bmp[BUTTON_SLINGSHOT].rcDes.left = rcPanel.left + 70;
        Bmp[BUTTON_SLINGSHOT].rcDes.top = rcPanel.top + 380;
        Bmp[BUTTON_SLINGSHOT].rcDes.right = Bmp[BUTTON_SLINGSHOT].rcDes.left + 35;
        Bmp[BUTTON_SLINGSHOT].rcDes.bottom = Bmp[BUTTON_SLINGSHOT].rcDes.top + 35;
        Bmp[BUTTON_SLINGSHOT].Breite = static_cast<short>(Bmp[BUTTON_SLINGSHOT].rcSrc.right - Bmp[BUTTON_SLINGSHOT].rcSrc.left);
        Bmp[BUTTON_SLINGSHOT].Hoehe = static_cast<short>(Bmp[BUTTON_SLINGSHOT].rcSrc.bottom - Bmp[BUTTON_SLINGSHOT].rcSrc.top);
        Bmp[BUTTON_SLINGSHOT].Anzahl = 4;
        Bmp[BUTTON_SLINGSHOT].Geschwindigkeit = 3;
        Bmp[BUTTON_SLINGSHOT].Phase = -1;

        // BUTTFELD
        Bmp[BUTTON_FARM].rcSrc.left = 238;
        Bmp[BUTTON_FARM].rcSrc.top = 0;
        Bmp[BUTTON_FARM].rcSrc.right = 238 + 35;
        Bmp[BUTTON_FARM].rcSrc.bottom = 35;
        Bmp[BUTTON_FARM].rcDes.left = rcPanel.left + 70;
        Bmp[BUTTON_FARM].rcDes.top = rcPanel.top + 270;
        Bmp[BUTTON_FARM].rcDes.right = Bmp[BUTTON_FARM].rcDes.left + 35;
        Bmp[BUTTON_FARM].rcDes.bottom = Bmp[BUTTON_FARM].rcDes.top + 35;
        Bmp[BUTTON_FARM].Breite = static_cast<short>(Bmp[BUTTON_FARM].rcSrc.right - Bmp[BUTTON_FARM].rcSrc.left);
        Bmp[BUTTON_FARM].Hoehe = static_cast<short>(Bmp[BUTTON_FARM].rcSrc.bottom - Bmp[BUTTON_FARM].rcSrc.top);
        Bmp[BUTTON_FARM].Anzahl = 4;
        Bmp[BUTTON_FARM].Geschwindigkeit = 3;
        Bmp[BUTTON_FARM].Phase = -1;

        // BUTTZELT
        Bmp[BUTTON_TENT].rcSrc.left = 273;
        Bmp[BUTTON_TENT].rcSrc.top = 0;
        Bmp[BUTTON_TENT].rcSrc.right = 273 + 35;
        Bmp[BUTTON_TENT].rcSrc.bottom = 35;
        Bmp[BUTTON_TENT].rcDes.left = rcPanel.left + 29;
        Bmp[BUTTON_TENT].rcDes.top = rcPanel.top + 270;
        Bmp[BUTTON_TENT].rcDes.right = Bmp[BUTTON_TENT].rcDes.left + 35;
        Bmp[BUTTON_TENT].rcDes.bottom = Bmp[BUTTON_TENT].rcDes.top + 35;
        Bmp[BUTTON_TENT].Breite = static_cast<short>(Bmp[BUTTON_TENT].rcSrc.right - Bmp[BUTTON_TENT].rcSrc.left);
        Bmp[BUTTON_TENT].Hoehe = static_cast<short>(Bmp[BUTTON_TENT].rcSrc.bottom - Bmp[BUTTON_TENT].rcSrc.top);
        Bmp[BUTTON_TENT].Anzahl = 3;
        Bmp[BUTTON_TENT].Geschwindigkeit = 3;
        Bmp[BUTTON_TENT].Phase = 0;

        // BUTTBOOT
        Bmp[BUTTON_BOAT].rcSrc.left = 448;
        Bmp[BUTTON_BOAT].rcSrc.top = 0;
        Bmp[BUTTON_BOAT].rcSrc.right = 448 + 35;
        Bmp[BUTTON_BOAT].rcSrc.bottom = 35;
        Bmp[BUTTON_BOAT].rcDes.left = rcPanel.left + 111;
        Bmp[BUTTON_BOAT].rcDes.top = rcPanel.top + 270;
        Bmp[BUTTON_BOAT].rcDes.right = Bmp[BUTTON_BOAT].rcDes.left + 35;
        Bmp[BUTTON_BOAT].rcDes.bottom = Bmp[BUTTON_BOAT].rcDes.top + 35;
        Bmp[BUTTON_BOAT].Breite = static_cast<short>(Bmp[BUTTON_BOAT].rcSrc.right - Bmp[BUTTON_BOAT].rcSrc.left);
        Bmp[BUTTON_BOAT].Hoehe = static_cast<short>(Bmp[BUTTON_BOAT].rcSrc.bottom - Bmp[BUTTON_BOAT].rcSrc.top);
        Bmp[BUTTON_BOAT].Anzahl = 3;
        Bmp[BUTTON_BOAT].Geschwindigkeit = 3;
        Bmp[BUTTON_BOAT].Phase = -1;

        // BUTTROHR
        Bmp[BUTTON_PIPE].rcSrc.left = 518;
        Bmp[BUTTON_PIPE].rcSrc.top = 0;
        Bmp[BUTTON_PIPE].rcSrc.right = 518 + 35;
        Bmp[BUTTON_PIPE].rcSrc.bottom = 35;
        Bmp[BUTTON_PIPE].rcDes.left = rcPanel.left + 152;
        Bmp[BUTTON_PIPE].rcDes.top = rcPanel.top + 270;
        Bmp[BUTTON_PIPE].rcDes.right = Bmp[BUTTON_PIPE].rcDes.left + 35;
        Bmp[BUTTON_PIPE].rcDes.bottom = Bmp[BUTTON_PIPE].rcDes.top + 35;
        Bmp[BUTTON_PIPE].Breite = static_cast<short>(Bmp[BUTTON_PIPE].rcSrc.right - Bmp[BUTTON_PIPE].rcSrc.left);
        Bmp[BUTTON_PIPE].Hoehe = static_cast<short>(Bmp[BUTTON_PIPE].rcSrc.bottom - Bmp[BUTTON_PIPE].rcSrc.top);
        Bmp[BUTTON_PIPE].Anzahl = 4;
        Bmp[BUTTON_PIPE].Geschwindigkeit = 2;
        Bmp[BUTTON_PIPE].Phase = -1;

        // BUTTSOS
        Bmp[BUTTON_SOS].rcSrc.left = 588;
        Bmp[BUTTON_SOS].rcSrc.top = 0;
        Bmp[BUTTON_SOS].rcSrc.right = 588 + 35;
        Bmp[BUTTON_SOS].rcSrc.bottom = 35;
        Bmp[BUTTON_SOS].rcDes.left = rcPanel.left + 29;
        Bmp[BUTTON_SOS].rcDes.top = rcPanel.top + 325;
        Bmp[BUTTON_SOS].rcDes.right = Bmp[BUTTON_SOS].rcDes.left + 35;
        Bmp[BUTTON_SOS].rcDes.bottom = Bmp[BUTTON_SOS].rcDes.top + 35;
        Bmp[BUTTON_SOS].Breite = static_cast<short>(Bmp[BUTTON_SOS].rcSrc.right - Bmp[BUTTON_SOS].rcSrc.left);
        Bmp[BUTTON_SOS].Hoehe = static_cast<short>(Bmp[BUTTON_SOS].rcSrc.bottom - Bmp[BUTTON_SOS].rcSrc.top);
        Bmp[BUTTON_SOS].Anzahl = 3;
        Bmp[BUTTON_SOS].Geschwindigkeit = 2;

        // BUTTHAUS1
        Bmp[BUTTON_HOUSE_1].rcSrc.left = 623;
        Bmp[BUTTON_HOUSE_1].rcSrc.top = 0;
        Bmp[BUTTON_HOUSE_1].rcSrc.right = 623 + 35;
        Bmp[BUTTON_HOUSE_1].rcSrc.bottom = 35;
        Bmp[BUTTON_HOUSE_1].rcDes.left = rcPanel.left + 70;
        Bmp[BUTTON_HOUSE_1].rcDes.top = rcPanel.top + 325;
        Bmp[BUTTON_HOUSE_1].rcDes.right = Bmp[BUTTON_HOUSE_1].rcDes.left + 35;
        Bmp[BUTTON_HOUSE_1].rcDes.bottom = Bmp[BUTTON_HOUSE_1].rcDes.top + 35;
        Bmp[BUTTON_HOUSE_1].Breite = static_cast<short>(Bmp[BUTTON_HOUSE_1].rcSrc.right - Bmp[BUTTON_HOUSE_1].rcSrc.left);
        Bmp[BUTTON_HOUSE_1].Hoehe = static_cast<short>(Bmp[BUTTON_HOUSE_1].rcSrc.bottom - Bmp[BUTTON_HOUSE_1].rcSrc.top);
        Bmp[BUTTON_HOUSE_1].Anzahl = 5;
        Bmp[BUTTON_HOUSE_1].Geschwindigkeit = 3;
        Bmp[BUTTON_HOUSE_1].Phase = -1;

        // BUTTHAUS2
        Bmp[BUTTON_HOUSE_2].rcSrc.left = 658;
        Bmp[BUTTON_HOUSE_2].rcSrc.top = 0;
        Bmp[BUTTON_HOUSE_2].rcSrc.right = 658 + 35;
        Bmp[BUTTON_HOUSE_2].rcSrc.bottom = 35;
        Bmp[BUTTON_HOUSE_2].rcDes.left = rcPanel.left + 111;
        Bmp[BUTTON_HOUSE_2].rcDes.top = rcPanel.top + 325;
        Bmp[BUTTON_HOUSE_2].rcDes.right = Bmp[BUTTON_HOUSE_2].rcDes.left + 35;
        Bmp[BUTTON_HOUSE_2].rcDes.bottom = Bmp[BUTTON_HOUSE_2].rcDes.top + 35;
        Bmp[BUTTON_HOUSE_2].Breite = static_cast<short>(Bmp[BUTTON_HOUSE_2].rcSrc.right - Bmp[BUTTON_HOUSE_2].rcSrc.left);
        Bmp[BUTTON_HOUSE_2].Hoehe = static_cast<short>(Bmp[BUTTON_HOUSE_2].rcSrc.bottom - Bmp[BUTTON_HOUSE_2].rcSrc.top);
        Bmp[BUTTON_HOUSE_2].Anzahl = 4;
        Bmp[BUTTON_HOUSE_2].Geschwindigkeit = 3;
        Bmp[BUTTON_HOUSE_2].Phase = -1;

        // BUTTHAUS3
        Bmp[BUTTON_HOUSE_3].rcSrc.left = 693;
        Bmp[BUTTON_HOUSE_3].rcSrc.top = 0;
        Bmp[BUTTON_HOUSE_3].rcSrc.right = 693 + 35;
        Bmp[BUTTON_HOUSE_3].rcSrc.bottom = 35;
        Bmp[BUTTON_HOUSE_3].rcDes.left = rcPanel.left + 152;
        Bmp[BUTTON_HOUSE_3].rcDes.top = rcPanel.top + 325;
        Bmp[BUTTON_HOUSE_3].rcDes.right = Bmp[BUTTON_HOUSE_3].rcDes.left + 35;
        Bmp[BUTTON_HOUSE_3].rcDes.bottom = Bmp[BUTTON_HOUSE_3].rcDes.top + 35;
        Bmp[BUTTON_HOUSE_3].Breite = static_cast<short>(Bmp[BUTTON_HOUSE_3].rcSrc.right - Bmp[BUTTON_HOUSE_3].rcSrc.left);
        Bmp[BUTTON_HOUSE_3].Hoehe = static_cast<short>(Bmp[BUTTON_HOUSE_3].rcSrc.bottom - Bmp[BUTTON_HOUSE_3].rcSrc.top);
        Bmp[BUTTON_HOUSE_3].Anzahl = 4;
        Bmp[BUTTON_HOUSE_3].Geschwindigkeit = 3;
        Bmp[BUTTON_HOUSE_3].Phase = -1;

        // BUTTFEUERST
        Bmp[BUTTON_FIRE].rcSrc.left = 728;
        Bmp[BUTTON_FIRE].rcSrc.top = 0;
        Bmp[BUTTON_FIRE].rcSrc.right = 728 + 35;
        Bmp[BUTTON_FIRE].rcSrc.bottom = 35;
        Bmp[BUTTON_FIRE].rcDes.left = rcPanel.left + 29;
        Bmp[BUTTON_FIRE].rcDes.top = rcPanel.top + 380;
        Bmp[BUTTON_FIRE].rcDes.right = Bmp[BUTTON_FIRE].rcDes.left + 35;
        Bmp[BUTTON_FIRE].rcDes.bottom = Bmp[BUTTON_FIRE].rcDes.top + 35;
        Bmp[BUTTON_FIRE].Breite = static_cast<short>(Bmp[BUTTON_FIRE].rcSrc.right - Bmp[BUTTON_FIRE].rcSrc.left);
        Bmp[BUTTON_FIRE].Hoehe = static_cast<short>(Bmp[BUTTON_FIRE].rcSrc.bottom - Bmp[BUTTON_FIRE].rcSrc.top);
        Bmp[BUTTON_FIRE].Anzahl = 4;
        Bmp[BUTTON_FIRE].Geschwindigkeit = 3;

        // BUTTFRAGEZ
        Bmp[BUTTON_QUESTION].rcSrc.left = 203;
        Bmp[BUTTON_QUESTION].rcSrc.top = 175;
        Bmp[BUTTON_QUESTION].rcSrc.right = 203 + 35;
        Bmp[BUTTON_QUESTION].rcSrc.bottom = 175 + 35;
        Bmp[BUTTON_QUESTION].rcDes.left = 0;
        Bmp[BUTTON_QUESTION].rcDes.top = 0;
        Bmp[BUTTON_QUESTION].rcDes.right = 0 + 35;
        Bmp[BUTTON_QUESTION].rcDes.bottom = 0 + 35;
        Bmp[BUTTON_QUESTION].Breite = static_cast<short>(Bmp[BUTTON_QUESTION].rcSrc.right - Bmp[BUTTON_QUESTION].rcSrc.left);
        Bmp[BUTTON_QUESTION].Hoehe = static_cast<short>(Bmp[BUTTON_QUESTION].rcSrc.bottom - Bmp[BUTTON_QUESTION].rcSrc.top);
        Bmp[BUTTON_QUESTION].Anzahl = 1;
        Bmp[BUTTON_QUESTION].Geschwindigkeit = 0;

        // BUTTDESTROY
        Bmp[BUTTON_DESTROY].rcSrc.left = 553;
        Bmp[BUTTON_DESTROY].rcSrc.top = 0;
        Bmp[BUTTON_DESTROY].rcSrc.right = 553 + 35;
        Bmp[BUTTON_DESTROY].rcSrc.bottom = 35;
        Bmp[BUTTON_DESTROY].rcDes.left = rcPanel.left + 152;
        Bmp[BUTTON_DESTROY].rcDes.top = rcPanel.top + 380;
        Bmp[BUTTON_DESTROY].rcDes.right = Bmp[BUTTON_DESTROY].rcDes.left + 35;
        Bmp[BUTTON_DESTROY].rcDes.bottom = Bmp[BUTTON_DESTROY].rcDes.top + 35;
        Bmp[BUTTON_DESTROY].Breite = static_cast<short>(Bmp[BUTTON_DESTROY].rcSrc.right - Bmp[BUTTON_DESTROY].rcSrc.left);
        Bmp[BUTTON_DESTROY].Hoehe = static_cast<short>(Bmp[BUTTON_DESTROY].rcSrc.bottom - Bmp[BUTTON_DESTROY].rcSrc.top);
        Bmp[BUTTON_DESTROY].Anzahl = 4;
        Bmp[BUTTON_DESTROY].Geschwindigkeit = 5;

        // SpzAni
        for (i = TREE_DOWN_1; i <= TREE_DOWN_4; i++)
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
        Bmp[TREE_DOWN_1].rcSrc.left = 101;
        Bmp[TREE_DOWN_1].rcSrc.top = 0;
        Bmp[TREE_DOWN_1].rcSrc.right = 101 + 35;
        Bmp[TREE_DOWN_1].rcSrc.bottom = 27;
        Bmp[TREE_DOWN_1].Breite = static_cast<short>(Bmp[TREE_DOWN_1].rcSrc.right - Bmp[TREE_DOWN_1].rcSrc.left);
        Bmp[TREE_DOWN_1].Hoehe = static_cast<short>(Bmp[TREE_DOWN_1].rcSrc.bottom - Bmp[TREE_DOWN_1].rcSrc.top);

        // BAUM2DOWN
        Bmp[TREE_DOWN_2].rcSrc.left = 136;
        Bmp[TREE_DOWN_2].rcSrc.top = 0;
        Bmp[TREE_DOWN_2].rcSrc.right = 136 + 36;
        Bmp[TREE_DOWN_2].rcSrc.bottom = 27;
        Bmp[TREE_DOWN_2].Breite = static_cast<short>(Bmp[TREE_DOWN_2].rcSrc.right - Bmp[TREE_DOWN_2].rcSrc.left);
        Bmp[TREE_DOWN_2].Hoehe = static_cast<short>(Bmp[TREE_DOWN_2].rcSrc.bottom - Bmp[TREE_DOWN_2].rcSrc.top);

        // BAUM3DOWN
        Bmp[TREE_DOWN_3].rcSrc.left = 172;
        Bmp[TREE_DOWN_3].rcSrc.top = 0;
        Bmp[TREE_DOWN_3].rcSrc.right = 172 + 34;
        Bmp[TREE_DOWN_3].rcSrc.bottom = 28;
        Bmp[TREE_DOWN_3].Breite = static_cast<short>(Bmp[TREE_DOWN_3].rcSrc.right - Bmp[TREE_DOWN_3].rcSrc.left);
        Bmp[TREE_DOWN_3].Hoehe = static_cast<short>(Bmp[TREE_DOWN_3].rcSrc.bottom - Bmp[TREE_DOWN_3].rcSrc.top);

        // BAUM4DOWN
        Bmp[TREE_DOWN_4].rcSrc.left = 206;
        Bmp[TREE_DOWN_4].rcSrc.top = 0;
        Bmp[TREE_DOWN_4].rcSrc.right = 206 + 32;
        Bmp[TREE_DOWN_4].rcSrc.bottom = 17;
        Bmp[TREE_DOWN_4].Breite = static_cast<short>(Bmp[TREE_DOWN_4].rcSrc.right - Bmp[TREE_DOWN_4].rcSrc.left);
        Bmp[TREE_DOWN_4].Hoehe = static_cast<short>(Bmp[TREE_DOWN_4].rcSrc.bottom - Bmp[TREE_DOWN_4].rcSrc.top);

        // Sonstiges

        // Säule1
        Bmp[COLUMN_1].Anzahl = 1;
        Bmp[COLUMN_1].rcSrc.left = 205;
        Bmp[COLUMN_1].rcSrc.top = 115;
        Bmp[COLUMN_1].rcSrc.right = Bmp[COLUMN_1].rcSrc.left + 11;
        Bmp[COLUMN_1].rcSrc.bottom = Bmp[COLUMN_1].rcSrc.top + 115;
        Bmp[COLUMN_1].rcDes.left = rcPanel.left + 52;
        Bmp[COLUMN_1].rcDes.top = rcPanel.top + 471;
        Bmp[COLUMN_1].rcDes.right = Bmp[COLUMN_1].rcDes.left + 11;
        Bmp[COLUMN_1].rcDes.bottom = Bmp[COLUMN_1].rcDes.top + 115;
        Bmp[COLUMN_1].Breite = static_cast<short>(Bmp[COLUMN_1].rcSrc.right - Bmp[COLUMN_1].rcSrc.left);
        Bmp[COLUMN_1].Hoehe = static_cast<short>(Bmp[COLUMN_1].rcSrc.bottom - Bmp[COLUMN_1].rcSrc.top);
        Bmp[COLUMN_1].Surface = lpDDSPanel;

        // Säule2
        Bmp[COLUMN_2].Anzahl = 1;
        Bmp[COLUMN_2].rcSrc.left = 216;
        Bmp[COLUMN_2].rcSrc.top = 115;
        Bmp[COLUMN_2].rcSrc.right = Bmp[COLUMN_2].rcSrc.left + 11;
        Bmp[COLUMN_2].rcSrc.bottom = Bmp[COLUMN_2].rcSrc.top + 115;
        Bmp[COLUMN_2].rcDes.left = rcPanel.left + 114;
        Bmp[COLUMN_2].rcDes.top = rcPanel.top + 471;
        Bmp[COLUMN_2].rcDes.right = Bmp[COLUMN_2].rcDes.left + 11;
        Bmp[COLUMN_2].rcDes.bottom = Bmp[COLUMN_2].rcDes.top + 115;
        Bmp[COLUMN_2].Breite = static_cast<short>(Bmp[COLUMN_2].rcSrc.right - Bmp[COLUMN_2].rcSrc.left);
        Bmp[COLUMN_2].Hoehe = static_cast<short>(Bmp[COLUMN_2].rcSrc.bottom - Bmp[COLUMN_2].rcSrc.top);
        Bmp[COLUMN_2].Surface = lpDDSPanel;

        // Säule3
        Bmp[COLUMN_3].Anzahl = 1;
        Bmp[COLUMN_3].rcSrc.left = 227;
        Bmp[COLUMN_3].rcSrc.top = 115;
        Bmp[COLUMN_3].rcSrc.right = Bmp[COLUMN_3].rcSrc.left + 11;
        Bmp[COLUMN_3].rcSrc.bottom = Bmp[COLUMN_3].rcSrc.top + 115;
        Bmp[COLUMN_3].rcDes.left = rcPanel.left + 175;
        Bmp[COLUMN_3].rcDes.top = rcPanel.top + 471;
        Bmp[COLUMN_3].rcDes.right = Bmp[COLUMN_3].rcDes.left + 11;
        Bmp[COLUMN_3].rcDes.bottom = Bmp[COLUMN_3].rcDes.top + 115;
        Bmp[COLUMN_3].Breite = static_cast<short>(Bmp[COLUMN_3].rcSrc.right - Bmp[COLUMN_3].rcSrc.left);
        Bmp[COLUMN_3].Hoehe = static_cast<short>(Bmp[COLUMN_3].rcSrc.bottom - Bmp[COLUMN_3].rcSrc.top);
        Bmp[COLUMN_3].Surface = lpDDSPanel;

        // Rohstoffe
        for (i = RAW_TREE_BRANCH; i <= RAW_SLINGSHOT; i++)
        {
            Bmp[i].Anzahl = 1;
            Bmp[i].rcSrc.left = 18 + (i - RAW_TREE_BRANCH) * 16;
            Bmp[i].rcSrc.top = 0;
            Bmp[i].rcSrc.right = Bmp[i].rcSrc.left + 16;
            Bmp[i].rcSrc.bottom = Bmp[i].rcSrc.top + 15;
            Bmp[i].Breite = static_cast<short>(Bmp[i].rcSrc.right - Bmp[i].rcSrc.left);
            Bmp[i].Hoehe = static_cast<short>(Bmp[i].rcSrc.bottom - Bmp[i].rcSrc.top);
            Bmp[i].Surface = lpDDSInventar;
        }
        // RohAst
        Bmp[RAW_TREE_BRANCH].rcDes.left = rcPanel.left + 34;
        Bmp[RAW_TREE_BRANCH].rcDes.top = rcPanel.top + 280;
        Bmp[RAW_TREE_BRANCH].rcDes.right = Bmp[RAW_TREE_BRANCH].rcDes.left + 16;
        Bmp[RAW_TREE_BRANCH].rcDes.bottom = Bmp[RAW_TREE_BRANCH].rcDes.top + 15;
        // ROHSTEIN
        Bmp[RAW_STONE].rcDes.left = rcPanel.left + 34;
        Bmp[RAW_STONE].rcDes.top = rcPanel.top + 300;
        Bmp[RAW_STONE].rcDes.right = Bmp[RAW_STONE].rcDes.left + 16;
        Bmp[RAW_STONE].rcDes.bottom = Bmp[RAW_STONE].rcDes.top + 15;
        // ROHSTAMM
        Bmp[RAW_TREE_TRUNK].rcDes.left = rcPanel.left + 34;
        Bmp[RAW_TREE_TRUNK].rcDes.top = rcPanel.top + 360;
        Bmp[RAW_TREE_TRUNK].rcDes.right = Bmp[RAW_TREE_TRUNK].rcDes.left + 16;
        Bmp[RAW_TREE_TRUNK].rcDes.bottom = Bmp[RAW_TREE_TRUNK].rcDes.top + 15;
        // ROHAXT
        Bmp[RAW_AXE].rcDes.left = rcPanel.left + 150;
        Bmp[RAW_AXE].rcDes.top = rcPanel.top + 280;
        Bmp[RAW_AXE].rcDes.right = Bmp[RAW_AXE].rcDes.left + 16;
        Bmp[RAW_AXE].rcDes.bottom = Bmp[RAW_AXE].rcDes.top + 15;
        // ROHBLATT
        Bmp[RAW_LEAF].rcDes.left = rcPanel.left + 34;
        Bmp[RAW_LEAF].rcDes.top = rcPanel.top + 320;
        Bmp[RAW_LEAF].rcDes.right = Bmp[RAW_LEAF].rcDes.left + 16;
        Bmp[RAW_LEAF].rcDes.bottom = Bmp[RAW_LEAF].rcDes.top + 15;
        // ROHEGGE
        Bmp[RAW_HOE].rcDes.left = rcPanel.left + 150;
        Bmp[RAW_HOE].rcDes.top = rcPanel.top + 300;
        Bmp[RAW_HOE].rcDes.right = Bmp[RAW_HOE].rcDes.left + 16;
        Bmp[RAW_HOE].rcDes.bottom = Bmp[RAW_HOE].rcDes.top + 15;
        // ROHLIANE
        Bmp[RAW_LIANA].rcDes.left = rcPanel.left + 34;
        Bmp[RAW_LIANA].rcDes.top = rcPanel.top + 340;
        Bmp[RAW_LIANA].rcDes.right = Bmp[RAW_LIANA].rcDes.left + 16;
        Bmp[RAW_LIANA].rcDes.bottom = Bmp[RAW_LIANA].rcDes.top + 15;
        // ROHANGEL
        Bmp[RAW_FISHING_POLE].rcDes.left = rcPanel.left + 150;
        Bmp[RAW_FISHING_POLE].rcDes.top = rcPanel.top + 320;
        Bmp[RAW_FISHING_POLE].rcDes.right = Bmp[RAW_FISHING_POLE].rcDes.left + 16;
        Bmp[RAW_FISHING_POLE].rcDes.bottom = Bmp[RAW_FISHING_POLE].rcDes.top + 15;
        // ROHHAMMER
        Bmp[RAW_HAMMER].rcDes.left = rcPanel.left + 150;
        Bmp[RAW_HAMMER].rcDes.top = rcPanel.top + 340;
        Bmp[RAW_HAMMER].rcDes.right = Bmp[RAW_HAMMER].rcDes.left + 16;
        Bmp[RAW_HAMMER].rcDes.bottom = Bmp[RAW_HAMMER].rcDes.top + 15;
        // ROHFERNROHR
        Bmp[RAW_TELESCOPE].rcDes.left = rcPanel.left + 150;
        Bmp[RAW_TELESCOPE].rcDes.top = rcPanel.top + 360;
        Bmp[RAW_TELESCOPE].rcDes.right = Bmp[RAW_TELESCOPE].rcDes.left + 16;
        Bmp[RAW_TELESCOPE].rcDes.bottom = Bmp[RAW_TELESCOPE].rcDes.top + 15;
        // ROHSTREICHHOLZ
        Bmp[RAW_MATCH].rcDes.left = rcPanel.left + 105;
        Bmp[RAW_MATCH].rcDes.top = rcPanel.top + 280;
        Bmp[RAW_MATCH].rcDes.right = Bmp[RAW_MATCH].rcDes.left + 16;
        Bmp[RAW_MATCH].rcDes.bottom = Bmp[RAW_MATCH].rcDes.top + 15;
        // ROHSCHAUFEL
        Bmp[RAW_SHOVEL].rcDes.left = rcPanel.left + 105;
        Bmp[RAW_SHOVEL].rcDes.top = rcPanel.top + 320;
        Bmp[RAW_SHOVEL].rcDes.right = Bmp[RAW_SHOVEL].rcDes.left + 16;
        Bmp[RAW_SHOVEL].rcDes.bottom = Bmp[RAW_SHOVEL].rcDes.top + 15;
        // ROHKARTE
        Bmp[RAW_MAP].rcDes.left = rcPanel.left + 105;
        Bmp[RAW_MAP].rcDes.top = rcPanel.top + 320;
        Bmp[RAW_MAP].rcDes.right = Bmp[RAW_MAP].rcDes.left + 16;
        Bmp[RAW_MAP].rcDes.bottom = Bmp[RAW_MAP].rcDes.top + 15;
        // ROHSCHLEUDER
        Bmp[RAW_SLINGSHOT].rcDes.left = rcPanel.left + 105;
        Bmp[RAW_SLINGSHOT].rcDes.top = rcPanel.top + 340;
        Bmp[RAW_SLINGSHOT].rcDes.right = Bmp[RAW_SLINGSHOT].rcDes.left + 16;
        Bmp[RAW_SLINGSHOT].rcDes.bottom = Bmp[RAW_SLINGSHOT].rcDes.top + 15;

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
        Bmp[ROEMISCH1].Breite = static_cast<short>(Bmp[ROEMISCH1].rcSrc.right - Bmp[ROEMISCH1].rcSrc.left);
        Bmp[ROEMISCH1].Hoehe = static_cast<short>(Bmp[ROEMISCH1].rcSrc.bottom - Bmp[ROEMISCH1].rcSrc.top);
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
        Bmp[ROEMISCH2].Breite = static_cast<short>(Bmp[ROEMISCH2].rcSrc.right - Bmp[ROEMISCH2].rcSrc.left);
        Bmp[ROEMISCH2].Hoehe = static_cast<short>(Bmp[ROEMISCH2].rcSrc.bottom - Bmp[ROEMISCH2].rcSrc.top);
        Bmp[ROEMISCH2].Surface = lpDDSInventar;

        // INVPAPIER
        Bmp[INVENTORY_PAPER].Anzahl = 1;
        Bmp[INVENTORY_PAPER].rcSrc.left = 0;
        Bmp[INVENTORY_PAPER].rcSrc.top = 15;
        Bmp[INVENTORY_PAPER].rcSrc.right = Bmp[INVENTORY_PAPER].rcSrc.left + 178;
        Bmp[INVENTORY_PAPER].rcSrc.bottom = Bmp[INVENTORY_PAPER].rcSrc.top + 114;
        Bmp[INVENTORY_PAPER].rcDes.left = rcPanel.left + 15;
        Bmp[INVENTORY_PAPER].rcDes.top = rcPanel.top + 270;
        Bmp[INVENTORY_PAPER].rcDes.right = Bmp[INVENTORY_PAPER].rcDes.left + 178;
        Bmp[INVENTORY_PAPER].rcDes.bottom = Bmp[INVENTORY_PAPER].rcDes.top + 114;
        Bmp[INVENTORY_PAPER].Breite = static_cast<short>(Bmp[INVENTORY_PAPER].rcSrc.right - Bmp[INVENTORY_PAPER].rcSrc.left);
        Bmp[INVENTORY_PAPER].Hoehe = static_cast<short>(Bmp[INVENTORY_PAPER].rcSrc.bottom - Bmp[INVENTORY_PAPER].rcSrc.top);
        Bmp[INVENTORY_PAPER].Surface = lpDDSInventar;

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
        Bmp[RING].Breite = static_cast<short>(Bmp[RING].rcSrc.right - Bmp[RING].rcSrc.left);
        Bmp[RING].Hoehe = static_cast<short>(Bmp[RING].rcSrc.bottom - Bmp[RING].rcSrc.top);
        Bmp[RING].Surface = lpDDSPanel;

        // KREUZ
        Bmp[CROSS].Anzahl = 1;
        Bmp[CROSS].rcSrc.left = 205;
        Bmp[CROSS].rcSrc.top = 380;
        Bmp[CROSS].rcSrc.right = Bmp[CROSS].rcSrc.left + 40;
        Bmp[CROSS].rcSrc.bottom = Bmp[CROSS].rcSrc.top + 22;
        Bmp[CROSS].Breite = static_cast<short>(Bmp[CROSS].rcSrc.right - Bmp[CROSS].rcSrc.left);
        Bmp[CROSS].Hoehe = static_cast<short>(Bmp[CROSS].rcSrc.bottom - Bmp[CROSS].rcSrc.top);
        Bmp[CROSS].Surface = lpDDSPanel;

        // JA
        Bmp[YES].Anzahl = 1;
        Bmp[YES].rcSrc.left = 0;
        Bmp[YES].rcSrc.top = 154;
        Bmp[YES].rcSrc.right = Bmp[YES].rcSrc.left + 41;
        Bmp[YES].rcSrc.bottom = Bmp[YES].rcSrc.top + 42;
        Bmp[YES].Breite = static_cast<short>(Bmp[YES].rcSrc.right - Bmp[YES].rcSrc.left);
        Bmp[YES].Hoehe = static_cast<short>(Bmp[YES].rcSrc.bottom - Bmp[YES].rcSrc.top);
        Bmp[YES].Surface = lpDDSPapier;

        // NEIN
        Bmp[NO].Anzahl = 1;
        Bmp[NO].rcSrc.left = 41;
        Bmp[NO].rcSrc.top = 154;
        Bmp[NO].rcSrc.right = Bmp[NO].rcSrc.left + 100;
        Bmp[NO].rcSrc.bottom = Bmp[NO].rcSrc.top + 39;
        Bmp[NO].Breite = static_cast<short>(Bmp[NO].rcSrc.right - Bmp[NO].rcSrc.left);
        Bmp[NO].Hoehe = static_cast<short>(Bmp[NO].rcSrc.bottom - Bmp[NO].rcSrc.top);
        Bmp[NO].Surface = lpDDSPapier;

        // Sonne
        Bmp[SUN].Anzahl = 1;
        Bmp[SUN].rcSrc.left = 205;
        Bmp[SUN].rcSrc.top = 65;
        Bmp[SUN].rcSrc.right = Bmp[SUN].rcSrc.left + 51;
        Bmp[SUN].rcSrc.bottom = Bmp[SUN].rcSrc.top + 50;
        Bmp[SUN].rcDes.left = rcPanel.left + 30;
        Bmp[SUN].rcDes.top = rcPanel.top + 630;
        Bmp[SUN].rcDes.right = Bmp[SUN].rcDes.left + 152;
        Bmp[SUN].rcDes.bottom = Bmp[SUN].rcDes.top + 55;
        Bmp[SUN].Breite = static_cast<short>(Bmp[SUN].rcSrc.right - Bmp[SUN].rcSrc.left);
        Bmp[SUN].Hoehe = static_cast<short>(Bmp[SUN].rcSrc.bottom - Bmp[SUN].rcSrc.top);
        Bmp[SUN].Surface = lpDDSPanel;

        // PROGRAMMIERUNG
        Bmp[PROGRAMMING].Anzahl = 1;
        Bmp[PROGRAMMING].rcSrc.left = 0;
        Bmp[PROGRAMMING].rcSrc.top = 0;
        Bmp[PROGRAMMING].rcSrc.right = Bmp[PROGRAMMING].rcSrc.left + 348;
        Bmp[PROGRAMMING].rcSrc.bottom = Bmp[PROGRAMMING].rcSrc.top + 49;
        Bmp[PROGRAMMING].Breite = static_cast<short>(Bmp[PROGRAMMING].rcSrc.right - Bmp[PROGRAMMING].rcSrc.left);
        Bmp[PROGRAMMING].Hoehe = static_cast<short>(Bmp[PROGRAMMING].rcSrc.bottom - Bmp[PROGRAMMING].rcSrc.top);
        Bmp[PROGRAMMING].rcDes.left = MAX_SCREEN_X / 2 - Bmp[PROGRAMMING].Breite / 2;
        Bmp[PROGRAMMING].rcDes.top = MAX_SCREEN_Y - Bmp[PROGRAMMING].Hoehe / 2;
        Bmp[PROGRAMMING].rcDes.right = Bmp[PROGRAMMING].rcDes.left + Bmp[PROGRAMMING].Breite;
        Bmp[PROGRAMMING].rcDes.bottom = Bmp[PROGRAMMING].rcDes.top + Bmp[PROGRAMMING].Hoehe;
        Bmp[PROGRAMMING].Surface = lpDDSCredits;

        // DIRKPLATE
        Bmp[DIRK_PLATE].Anzahl = 1;
        Bmp[DIRK_PLATE].rcSrc.left = 0;
        Bmp[DIRK_PLATE].rcSrc.top = 49;
        Bmp[DIRK_PLATE].rcSrc.right = Bmp[DIRK_PLATE].rcSrc.left + 196;
        Bmp[DIRK_PLATE].rcSrc.bottom = Bmp[DIRK_PLATE].rcSrc.top + 47;
        Bmp[DIRK_PLATE].Breite = static_cast<short>(Bmp[DIRK_PLATE].rcSrc.right - Bmp[DIRK_PLATE].rcSrc.left);
        Bmp[DIRK_PLATE].Hoehe = static_cast<short>(Bmp[DIRK_PLATE].rcSrc.bottom - Bmp[DIRK_PLATE].rcSrc.top);
        Bmp[DIRK_PLATE].rcDes.left = MAX_SCREEN_X / 2 - Bmp[DIRK_PLATE].Breite / 2;
        Bmp[DIRK_PLATE].rcDes.top = MAX_SCREEN_Y - Bmp[DIRK_PLATE].Hoehe / 2;
        Bmp[DIRK_PLATE].rcDes.right = Bmp[DIRK_PLATE].rcDes.left + Bmp[DIRK_PLATE].Breite;
        Bmp[DIRK_PLATE].rcDes.bottom = Bmp[DIRK_PLATE].rcDes.top + Bmp[DIRK_PLATE].Hoehe;
        Bmp[DIRK_PLATE].Surface = lpDDSCredits;

        // MATTHIAS
        Bmp[MATTHIAS].Anzahl = 1;
        Bmp[MATTHIAS].rcSrc.left = 0;
        Bmp[MATTHIAS].rcSrc.top = 96;
        Bmp[MATTHIAS].rcSrc.right = Bmp[MATTHIAS].rcSrc.left + 379;
        Bmp[MATTHIAS].rcSrc.bottom = Bmp[MATTHIAS].rcSrc.top + 47;
        Bmp[MATTHIAS].Breite = static_cast<short>(Bmp[MATTHIAS].rcSrc.right - Bmp[MATTHIAS].rcSrc.left);
        Bmp[MATTHIAS].Hoehe = static_cast<short>(Bmp[MATTHIAS].rcSrc.bottom - Bmp[MATTHIAS].rcSrc.top);
        Bmp[MATTHIAS].rcDes.left = MAX_SCREEN_X / 2 - Bmp[MATTHIAS].Breite / 2;
        Bmp[MATTHIAS].rcDes.top = MAX_SCREEN_Y - Bmp[MATTHIAS].Hoehe / 2;
        Bmp[MATTHIAS].rcDes.right = Bmp[MATTHIAS].rcDes.left + Bmp[MATTHIAS].Breite;
        Bmp[MATTHIAS].rcDes.bottom = Bmp[MATTHIAS].rcDes.top + Bmp[MATTHIAS].Hoehe;
        Bmp[MATTHIAS].Surface = lpDDSCredits;

        // TESTSPIELER
        Bmp[TEST_PLAYER].Anzahl = 1;
        Bmp[TEST_PLAYER].rcSrc.left = 0;
        Bmp[TEST_PLAYER].rcSrc.top = 143;
        Bmp[TEST_PLAYER].rcSrc.right = Bmp[TEST_PLAYER].rcSrc.left + 210;
        Bmp[TEST_PLAYER].rcSrc.bottom = Bmp[TEST_PLAYER].rcSrc.top + 55;
        Bmp[TEST_PLAYER].Breite = static_cast<short>(Bmp[TEST_PLAYER].rcSrc.right - Bmp[TEST_PLAYER].rcSrc.left);
        Bmp[TEST_PLAYER].Hoehe = static_cast<short>(Bmp[TEST_PLAYER].rcSrc.bottom - Bmp[TEST_PLAYER].rcSrc.top);
        Bmp[TEST_PLAYER].rcDes.left = MAX_SCREEN_X / 2 - Bmp[TEST_PLAYER].Breite / 2;
        Bmp[TEST_PLAYER].rcDes.top = MAX_SCREEN_Y - Bmp[TEST_PLAYER].Hoehe / 2;
        Bmp[TEST_PLAYER].rcDes.right = Bmp[TEST_PLAYER].rcDes.left + Bmp[TEST_PLAYER].Breite;
        Bmp[TEST_PLAYER].rcDes.bottom = Bmp[TEST_PLAYER].rcDes.top + Bmp[TEST_PLAYER].Hoehe;
        Bmp[TEST_PLAYER].Surface = lpDDSCredits;

        // TOBIAS
        Bmp[TOBIAS].Anzahl = 1;
        Bmp[TOBIAS].rcSrc.left = 0;
        Bmp[TOBIAS].rcSrc.top = 198;
        Bmp[TOBIAS].rcSrc.right = Bmp[TOBIAS].rcSrc.left + 273;
        Bmp[TOBIAS].rcSrc.bottom = Bmp[TOBIAS].rcSrc.top + 56;
        Bmp[TOBIAS].Breite = static_cast<short>(Bmp[TOBIAS].rcSrc.right - Bmp[TOBIAS].rcSrc.left);
        Bmp[TOBIAS].Hoehe = static_cast<short>(Bmp[TOBIAS].rcSrc.bottom - Bmp[TOBIAS].rcSrc.top);
        Bmp[TOBIAS].rcDes.left = MAX_SCREEN_X / 2 - Bmp[TOBIAS].Breite / 2;
        Bmp[TOBIAS].rcDes.top = MAX_SCREEN_Y - Bmp[TOBIAS].Hoehe / 2;
        Bmp[TOBIAS].rcDes.right = Bmp[TOBIAS].rcDes.left + Bmp[TOBIAS].Breite;
        Bmp[TOBIAS].rcDes.bottom = Bmp[TOBIAS].rcDes.top + Bmp[TOBIAS].Hoehe;
        Bmp[TOBIAS].Surface = lpDDSCredits;

        // SIGRID
        Bmp[SIGRID].Anzahl = 1;
        Bmp[SIGRID].rcSrc.left = 0;
        Bmp[SIGRID].rcSrc.top = 254;
        Bmp[SIGRID].rcSrc.right = Bmp[SIGRID].rcSrc.left + 226;
        Bmp[SIGRID].rcSrc.bottom = Bmp[SIGRID].rcSrc.top + 56;
        Bmp[SIGRID].Breite = static_cast<short>(Bmp[SIGRID].rcSrc.right - Bmp[SIGRID].rcSrc.left);
        Bmp[SIGRID].Hoehe = static_cast<short>(Bmp[SIGRID].rcSrc.bottom - Bmp[SIGRID].rcSrc.top);
        Bmp[SIGRID].rcDes.left = MAX_SCREEN_X / 2 - Bmp[SIGRID].Breite / 2;
        Bmp[SIGRID].rcDes.top = MAX_SCREEN_Y - Bmp[SIGRID].Hoehe / 2;
        Bmp[SIGRID].rcDes.right = Bmp[SIGRID].rcDes.left + Bmp[SIGRID].Breite;
        Bmp[SIGRID].rcDes.bottom = Bmp[SIGRID].rcDes.top + Bmp[SIGRID].Hoehe;
        Bmp[SIGRID].Surface = lpDDSCredits;

        // PATHFINDING
        Bmp[PATHFINDING].Anzahl = 1;
        Bmp[PATHFINDING].rcSrc.left = 0;
        Bmp[PATHFINDING].rcSrc.top = 353;
        Bmp[PATHFINDING].rcSrc.right = Bmp[PATHFINDING].rcSrc.left + 233;
        Bmp[PATHFINDING].rcSrc.bottom = Bmp[PATHFINDING].rcSrc.top + 59;
        Bmp[PATHFINDING].Breite = static_cast<short>(Bmp[PATHFINDING].rcSrc.right - Bmp[PATHFINDING].rcSrc.left);
        Bmp[PATHFINDING].Hoehe = static_cast<short>(Bmp[PATHFINDING].rcSrc.bottom - Bmp[PATHFINDING].rcSrc.top);
        Bmp[PATHFINDING].rcDes.left = MAX_SCREEN_X / 2 - Bmp[PATHFINDING].Breite / 2;
        Bmp[PATHFINDING].rcDes.top = MAX_SCREEN_Y - Bmp[PATHFINDING].Hoehe / 2;
        Bmp[PATHFINDING].rcDes.right = Bmp[PATHFINDING].rcDes.left + Bmp[PATHFINDING].Breite;
        Bmp[PATHFINDING].rcDes.bottom = Bmp[PATHFINDING].rcDes.top + Bmp[PATHFINDING].Hoehe;
        Bmp[PATHFINDING].Surface = lpDDSCredits;

        // JOHN
        Bmp[JOHN].Anzahl = 1;
        Bmp[JOHN].rcSrc.left = 0;
        Bmp[JOHN].rcSrc.top = 412;
        Bmp[JOHN].rcSrc.right = Bmp[JOHN].rcSrc.left + 347;
        Bmp[JOHN].rcSrc.bottom = Bmp[JOHN].rcSrc.top + 56;
        Bmp[JOHN].Breite = static_cast<short>(Bmp[JOHN].rcSrc.right - Bmp[JOHN].rcSrc.left);
        Bmp[JOHN].Hoehe = static_cast<short>(Bmp[JOHN].rcSrc.bottom - Bmp[JOHN].rcSrc.top);
        Bmp[JOHN].rcDes.left = MAX_SCREEN_X / 2 - Bmp[JOHN].Breite / 2;
        Bmp[JOHN].rcDes.top = MAX_SCREEN_Y - Bmp[JOHN].Hoehe / 2;
        Bmp[JOHN].rcDes.right = Bmp[JOHN].rcDes.left + Bmp[JOHN].Breite;
        Bmp[JOHN].rcDes.bottom = Bmp[JOHN].rcDes.top + Bmp[JOHN].Hoehe;
        Bmp[JOHN].Surface = lpDDSCredits;

        // HEIKO
        Bmp[HEIKO].Anzahl = 1;
        Bmp[HEIKO].rcSrc.left = 0;
        Bmp[HEIKO].rcSrc.top = 468;
        Bmp[HEIKO].rcSrc.right = Bmp[HEIKO].rcSrc.left + 236;
        Bmp[HEIKO].rcSrc.bottom = Bmp[HEIKO].rcSrc.top + 47;
        Bmp[HEIKO].Breite = static_cast<short>(Bmp[HEIKO].rcSrc.right - Bmp[HEIKO].rcSrc.left);
        Bmp[HEIKO].Hoehe = static_cast<short>(Bmp[HEIKO].rcSrc.bottom - Bmp[HEIKO].rcSrc.top);
        Bmp[HEIKO].rcDes.left = MAX_SCREEN_X / 2 - Bmp[HEIKO].Breite / 2;
        Bmp[HEIKO].rcDes.top = MAX_SCREEN_Y - Bmp[HEIKO].Hoehe / 2;
        Bmp[HEIKO].rcDes.right = Bmp[HEIKO].rcDes.left + Bmp[HEIKO].Breite;
        Bmp[HEIKO].rcDes.bottom = Bmp[HEIKO].rcDes.top + Bmp[HEIKO].Hoehe;
        Bmp[HEIKO].Surface = lpDDSCredits;

        // GISELA
        Bmp[GISELA].Anzahl = 1;
        Bmp[GISELA].rcSrc.left = 0;
        Bmp[GISELA].rcSrc.top = 515;
        Bmp[GISELA].rcSrc.right = Bmp[GISELA].rcSrc.left + 232;
        Bmp[GISELA].rcSrc.bottom = Bmp[GISELA].rcSrc.top + 47;
        Bmp[GISELA].Breite = static_cast<short>(Bmp[GISELA].rcSrc.right - Bmp[GISELA].rcSrc.left);
        Bmp[GISELA].Hoehe = static_cast<short>(Bmp[GISELA].rcSrc.bottom - Bmp[GISELA].rcSrc.top);
        Bmp[GISELA].rcDes.left = MAX_SCREEN_X / 2 - Bmp[GISELA].Breite / 2;
        Bmp[GISELA].rcDes.top = MAX_SCREEN_Y - Bmp[GISELA].Hoehe / 2;
        Bmp[GISELA].rcDes.right = Bmp[GISELA].rcDes.left + Bmp[GISELA].Breite;
        Bmp[GISELA].rcDes.bottom = Bmp[GISELA].rcDes.top + Bmp[GISELA].Hoehe;
        Bmp[GISELA].Surface = lpDDSCredits;

        // WEITEREHILFE
        Bmp[FURTHER_HELP].Anzahl = 1;
        Bmp[FURTHER_HELP].rcSrc.left = 0;
        Bmp[FURTHER_HELP].rcSrc.top = 562;
        Bmp[FURTHER_HELP].rcSrc.right = Bmp[FURTHER_HELP].rcSrc.left + 258;
        Bmp[FURTHER_HELP].rcSrc.bottom = Bmp[FURTHER_HELP].rcSrc.top + 46;
        Bmp[FURTHER_HELP].Breite = static_cast<short>(Bmp[FURTHER_HELP].rcSrc.right - Bmp[FURTHER_HELP].rcSrc.left);
        Bmp[FURTHER_HELP].Hoehe = static_cast<short>(Bmp[FURTHER_HELP].rcSrc.bottom - Bmp[FURTHER_HELP].rcSrc.top);
        Bmp[FURTHER_HELP].rcDes.left = MAX_SCREEN_X / 2 - Bmp[FURTHER_HELP].Breite / 2;
        Bmp[FURTHER_HELP].rcDes.top = MAX_SCREEN_Y - Bmp[FURTHER_HELP].Hoehe / 2;
        Bmp[FURTHER_HELP].rcDes.right = Bmp[FURTHER_HELP].rcDes.left + Bmp[FURTHER_HELP].Breite;
        Bmp[FURTHER_HELP].rcDes.bottom = Bmp[FURTHER_HELP].rcDes.top + Bmp[FURTHER_HELP].Hoehe;
        Bmp[FURTHER_HELP].Surface = lpDDSCredits;

        // DPSOFTWARE
        Bmp[DPSOFTWARE].Anzahl = 1;
        Bmp[DPSOFTWARE].rcSrc.left = 0;
        Bmp[DPSOFTWARE].rcSrc.top = 608;
        Bmp[DPSOFTWARE].rcSrc.right = Bmp[DPSOFTWARE].rcSrc.left + 291;
        Bmp[DPSOFTWARE].rcSrc.bottom = Bmp[DPSOFTWARE].rcSrc.top + 99;
        Bmp[DPSOFTWARE].Breite = static_cast<short>(Bmp[DPSOFTWARE].rcSrc.right - Bmp[DPSOFTWARE].rcSrc.left);
        Bmp[DPSOFTWARE].Hoehe = static_cast<short>(Bmp[DPSOFTWARE].rcSrc.bottom - Bmp[DPSOFTWARE].rcSrc.top);
        Bmp[DPSOFTWARE].rcDes.left = MAX_SCREEN_X / 2 - Bmp[DPSOFTWARE].Breite / 2;
        Bmp[DPSOFTWARE].rcDes.top = MAX_SCREEN_Y - Bmp[DPSOFTWARE].Hoehe / 2;
        Bmp[DPSOFTWARE].rcDes.right = Bmp[DPSOFTWARE].rcDes.left + Bmp[DPSOFTWARE].Breite;
        Bmp[DPSOFTWARE].rcDes.bottom = Bmp[DPSOFTWARE].rcDes.top + Bmp[DPSOFTWARE].Hoehe;
        Bmp[DPSOFTWARE].Surface = lpDDSCredits;

        // SCHWARZ
        Bmp[SCHWARZ].Anzahl = 1;
        Bmp[SCHWARZ].rcSrc.left = 0;
        Bmp[SCHWARZ].rcSrc.top = 608;
        Bmp[SCHWARZ].rcSrc.right = Bmp[SCHWARZ].rcSrc.left + 1;
        Bmp[SCHWARZ].rcSrc.bottom = Bmp[SCHWARZ].rcSrc.top + 1;
        Bmp[SCHWARZ].Breite = static_cast<short>(Bmp[SCHWARZ].rcSrc.right - Bmp[SCHWARZ].rcSrc.left);
        Bmp[SCHWARZ].Hoehe = static_cast<short>(Bmp[SCHWARZ].rcSrc.bottom - Bmp[SCHWARZ].rcSrc.top);
        Bmp[SCHWARZ].rcDes.left = MAX_SCREEN_X / 2 - Bmp[SCHWARZ].Breite / 2;
        Bmp[SCHWARZ].rcDes.top = MAX_SCREEN_Y - Bmp[SCHWARZ].Hoehe / 2;
        Bmp[SCHWARZ].rcDes.right = Bmp[SCHWARZ].rcDes.left + Bmp[SCHWARZ].Breite;
        Bmp[SCHWARZ].rcDes.bottom = Bmp[SCHWARZ].rcDes.top + Bmp[SCHWARZ].Hoehe;
        Bmp[SCHWARZ].Surface = lpDDSCredits;

        // SOUNDS
        Bmp[SOUNDS].Anzahl = 1;
        Bmp[SOUNDS].rcSrc.left = 0;
        Bmp[SOUNDS].rcSrc.top = 310;
        Bmp[SOUNDS].rcSrc.right = Bmp[SOUNDS].rcSrc.left + 144;
        Bmp[SOUNDS].rcSrc.bottom = Bmp[SOUNDS].rcSrc.top + 43;
        Bmp[SOUNDS].Breite = static_cast<short>(Bmp[SOUNDS].rcSrc.right - Bmp[SOUNDS].rcSrc.left);
        Bmp[SOUNDS].Hoehe = static_cast<short>(Bmp[SOUNDS].rcSrc.bottom - Bmp[SOUNDS].rcSrc.top);
        Bmp[SOUNDS].rcDes.left = MAX_SCREEN_X / 2 - Bmp[SOUNDS].Breite / 2;
        Bmp[SOUNDS].rcDes.top = MAX_SCREEN_Y - Bmp[SOUNDS].Hoehe / 2;
        Bmp[SOUNDS].rcDes.right = Bmp[SOUNDS].rcDes.left + Bmp[SOUNDS].Breite;
        Bmp[SOUNDS].rcDes.bottom = Bmp[SOUNDS].rcDes.top + Bmp[SOUNDS].Hoehe;
        Bmp[SOUNDS].Surface = lpDDSCredits;

        // MUSIK
        Bmp[MUSIC].Anzahl = 1;
        Bmp[MUSIC].rcSrc.left = 160;
        Bmp[MUSIC].rcSrc.top = 310;
        Bmp[MUSIC].rcSrc.right = Bmp[MUSIC].rcSrc.left + 124;
        Bmp[MUSIC].rcSrc.bottom = Bmp[MUSIC].rcSrc.top + 39;
        Bmp[MUSIC].Breite = static_cast<short>(Bmp[MUSIC].rcSrc.right - Bmp[MUSIC].rcSrc.left);
        Bmp[MUSIC].Hoehe = static_cast<short>(Bmp[MUSIC].rcSrc.bottom - Bmp[MUSIC].rcSrc.top);
        Bmp[MUSIC].rcDes.left = MAX_SCREEN_X / 2 - Bmp[MUSIC].Breite / 2;
        Bmp[MUSIC].rcDes.top = MAX_SCREEN_Y - Bmp[MUSIC].Hoehe / 2;
        Bmp[MUSIC].rcDes.right = Bmp[MUSIC].rcDes.left + Bmp[MUSIC].Breite;
        Bmp[MUSIC].rcDes.bottom = Bmp[MUSIC].rcDes.top + Bmp[MUSIC].Hoehe;
        Bmp[MUSIC].Surface = lpDDSCredits;

        for (i = 0; i < 10; i++)
            for (k = 0; k < 10; k++)
            {
                AbspannListe[i][k].Aktiv = false;
                AbspannListe[i][k].Bild = -1;
            }
        AbspannListe[0][0].Bild = PROGRAMMING;
        AbspannListe[0][1].Aktiv = true; // nur den hier true setzen, löst dann alles andere aus
        AbspannListe[0][1].Bild = DIRK_PLATE;
        AbspannListe[1][0].Bild = MUSIC;
        AbspannListe[1][1].Bild = HEIKO;
        AbspannListe[2][0].Bild = SOUNDS;
        AbspannListe[2][1].Bild = DIRK_PLATE;
        AbspannListe[3][0].Bild = TEST_PLAYER;
        AbspannListe[3][1].Bild = MATTHIAS;
        AbspannListe[3][2].Bild = TOBIAS;
        AbspannListe[3][3].Bild = SIGRID;
        AbspannListe[4][0].Bild = PATHFINDING;
        AbspannListe[4][1].Bild = JOHN;
        AbspannListe[5][0].Bild = FURTHER_HELP;
        AbspannListe[5][1].Bild = HEIKO;
        AbspannListe[5][2].Bild = GISELA;
        AbspannListe[6][0].Bild = SCHWARZ;
        AbspannListe[6][1].Bild = DPSOFTWARE;

        // Textausgabe
        TextBereich[TXTTEXTFELD].Aktiv = false;
        TextBereich[TXTTEXTFELD].rcText.left = 9;
        TextBereich[TXTTEXTFELD].rcText.top = MAX_SCREEN_Y - 17;
        TextBereich[TXTTEXTFELD].rcText.right = MAX_SCREEN_X - 200;
        TextBereich[TXTTEXTFELD].rcText.bottom = MAX_SCREEN_Y - 2;

        TextBereich[TXTFPS].Aktiv = false;
        TextBereich[TXTFPS].rcText.left = MAX_SCREEN_X - 40;
        TextBereich[TXTFPS].rcText.top = 3;
        TextBereich[TXTFPS].rcText.right = TextBereich[TXTFPS].rcText.left + 2 * FONT1_LETTER_WIDTH;
        TextBereich[TXTFPS].rcText.bottom = TextBereich[TXTFPS].rcText.top + FONT1_LETTER_HEIGHT;

        TextBereich[TXTTAGESZEIT].Aktiv = true;
        TextBereich[TXTTAGESZEIT].rcText.left = MAX_SCREEN_X - 110;
        TextBereich[TXTTAGESZEIT].rcText.top = MAX_SCREEN_Y - 20;
        TextBereich[TXTTAGESZEIT].rcText.right = TextBereich[TXTTAGESZEIT].rcText.left + 5 * FONT2_LETTER_WIDTH;
        TextBereich[TXTTAGESZEIT].rcText.bottom = TextBereich[TXTTAGESZEIT].rcText.top + FONT2_LETTER_HEIGHT;

        TextBereich[TXTPAPIER].Aktiv = false;
        TextBereich[TXTPAPIER].rcText.left = 450;
        TextBereich[TXTPAPIER].rcText.top = 160;
        TextBereich[TXTPAPIER].rcText.right = 830;
        TextBereich[TXTPAPIER].rcText.bottom = 560;

        TextBereich[TXTCHANCE].Aktiv = false;
        TextBereich[TXTCHANCE].rcText.left = Bmp[RING].rcDes.left + 5;
        TextBereich[TXTCHANCE].rcText.top = Bmp[RING].rcDes.top + Bmp[RING].Hoehe + 10;
        TextBereich[TXTCHANCE].rcText.right = TextBereich[TXTCHANCE].rcText.left + 3 * FONT2_LETTER_WIDTH;
        TextBereich[TXTCHANCE].rcText.bottom = TextBereich[TXTCHANCE].rcText.top + FONT2_LETTER_HEIGHT;

        Guy.Resource[WASSER] = 100;
        Guy.Resource[NAHRUNG] = 100;
        Guy.Resource[GESUNDHEIT] = 100;

        for (i = RAW_TREE_BRANCH; i <= RAW_SLINGSHOT; i++)
        {
            Guy.Inventar[i] = 0;
        }
        CursorTyp = CURSOR_ARROW;
        Gitter = false;
        PapierText = -1;
        HauptMenue = 0;
        TwoClicks = -1;
        Nacht = false;
        Frage = -1;
        LastBild = 100;
        Bild = 0;
        MousePosition.x = MAX_SCREEN_X / 2;
        MousePosition.y = MAX_SCREEN_Y / 2;
        Direct::Reset();
        SchatzPos.x = -1;
        SchatzPos.y = -1;
        SchatzGef = false;
    }

    void InitWaves()
    {
        // Sounds
        for (short i = 0; i < Sound::COUNT; i++)
        {
            Wav[i].Dateiname = "sounds/klick.wav";
            Wav[i].Loop = false;
            Wav[i].Volume = 100;
        }

        Wav[Sound::STORM].Dateiname = "sounds/sturm.wav";
        Wav[Sound::STORM].Volume = 85;

        Wav[Sound::SWIM].Dateiname = "sounds/schwimmen.wav";
        Wav[Sound::SWIM].Volume = 90;

        Wav[Sound::SPLAT].Dateiname = "sounds/platsch.wav";
        Wav[Sound::SPLAT].Volume = 95;

        Wav[Sound::LOG].Dateiname = "sounds/faellen.wav";
        Wav[Sound::LOG].Volume = 100;

        Wav[Sound::HIT].Dateiname = "sounds/schlagen.wav";
        Wav[Sound::HIT].Volume = 100;

        Wav[Sound::SLINGSHOT].Dateiname = "sounds/schleuder.wav";
        Wav[Sound::SLINGSHOT].Volume = 100;

        Wav[Sound::DIG].Dateiname = "sounds/schaufeln.wav";
        Wav[Sound::DIG].Volume = 90;

        Wav[Sound::HAMMER].Dateiname = "sounds/hammer.wav";
        Wav[Sound::HAMMER].Volume = 100;

        Wav[Sound::CRASH].Dateiname = "sounds/crash.wav";
        Wav[Sound::CRASH].Volume = 100;

        Wav[Sound::SNORE].Dateiname = "sounds/schnarchen.wav";
        Wav[Sound::SNORE].Volume = 90;

        Wav[Sound::DRINK].Dateiname = "sounds/trinken.wav";
        Wav[Sound::DRINK].Volume = 95;

        Wav[Sound::CRACKLE].Dateiname = "sounds/knistern.wav";
        Wav[Sound::CRACKLE].Volume = 90;

        Wav[Sound::FISH].Dateiname = "sounds/angel.wav";
        Wav[Sound::FISH].Volume = 100;

        Wav[Sound::FOREST].Dateiname = "sounds/wald.wav";
        Wav[Sound::FOREST].Volume = 90;

        Wav[Sound::FIRE].Dateiname = "sounds/feuer.wav";
        Wav[Sound::FIRE].Volume = 100;

        Wav[Sound::SURF].Dateiname = "sounds/brandung.wav";
        Wav[Sound::SURF].Volume = 100;

        Wav[Sound::TIMBER].Dateiname = "sounds/baumfaellt.wav";
        Wav[Sound::TIMBER].Volume = 100;

        Wav[Sound::RIVER].Dateiname = "sounds/fluss.wav";
        Wav[Sound::RIVER].Volume = 85;

        Wav[Sound::CLICK].Dateiname = "sounds/klick.wav";
        Wav[Sound::CLICK].Volume = 95;

        Wav[Sound::CLICK2].Dateiname = "sounds/klick2.wav";
        Wav[Sound::CLICK2].Volume = 95;

        Wav[Sound::OUTRO].Dateiname = "sounds/abspann.wav";
        Wav[Sound::OUTRO].Volume = 100;
        Wav[Sound::OUTRO].Loop = true;

        Wav[Sound::LOGO].Dateiname = "sounds/logo.wav";
        Wav[Sound::LOGO].Volume = 100;
        Wav[Sound::LOGO].Loop = true;

        Wav[Sound::WOLF].Dateiname = "sounds/wolf.wav";
        Wav[Sound::WOLF].Volume = 90;

        Wav[Sound::INVENTION].Dateiname = "sounds/erfindung.wav";
        Wav[Sound::INVENTION].Volume = 95;

        // Testweise alle Sounds gleich in den Speicher laden
        for (short i = 1; i < Sound::COUNT; i++)
            Sound::LoadSound(i);
    }

    void NeuesSpiel(bool neu)
    {
        short x, y;
        bool LoadOK = false;

        InitStructs();
        InitWaves();

        if (!neu) LoadOK = LoadGame();

        if ((!LoadOK) || (neu))
        {
            // Für die Statusanzeige
            rcRectdes.left = 0;
            rcRectdes.top = 0;
            rcRectdes.right = MAX_SCREEN_X;
            rcRectdes.bottom = MAX_SCREEN_Y;
            ddbltfx.dwFillColor = Renderer::RGB2DWORD(70, 70, 100);
//            lpDDSPrimary->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);
            ddbltfx.dwFillColor = Renderer::RGB2DWORD(255, 0, 255);
//            lpDDSSchrift->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

            // Landschaft erzeugen

            Renderer::DrawString("Erschaffe Landschaft...", 5, 5, 2);
            rcRectdes.left = 0;
            rcRectdes.top = 0;
            rcRectdes.right = MAX_SCREEN_X;
            rcRectdes.bottom = MAX_SCREEN_Y;
//            lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
            World::Compute(200, 600);

            Renderer::DrawString("Ueberflute Land...", 5, 35, 2);
            rcRectdes.left = 0;
            rcRectdes.top = 0;
            rcRectdes.right = MAX_SCREEN_X;
            rcRectdes.bottom = MAX_SCREEN_Y;
//            lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
            World::Meer();

            Renderer::DrawString("Lege Fluss fest...", 5, 65, 2);
            rcRectdes.left = 0;
            rcRectdes.top = 0;
            rcRectdes.right = MAX_SCREEN_X;
            rcRectdes.bottom = MAX_SCREEN_Y;
//            lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
            World::Fluss();
            Math::CalcKoor();

            Renderer::DrawString("Pflanze Baeume...", 5, 95, 2);
            rcRectdes.left = 0;
            rcRectdes.top = 0;
            rcRectdes.right = MAX_SCREEN_X;
            rcRectdes.bottom = MAX_SCREEN_Y;
//            lpDDSPrimary->Blt(&rcRectdes, lpDDSSchrift, &rcRectdes, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
            World::Baeume(30);

            World::Piratenwrack();

            // Guy Position
            Guy.Pos.x = 1;
            Guy.Pos.y = MAX_TILESY / 2;
            Guy.PosScreen.x =
            (Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                Scape[Guy.Pos.x][Guy.Pos.y].xScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2;
            Guy.PosScreen.y =
            (Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                Scape[Guy.Pos.x][Guy.Pos.y].yScreen + EckKoor[Scape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2;

            Camera.x = Guy.PosScreen.x - static_cast<short>(rcGesamt.right / 2);
            Camera.y = Guy.PosScreen.y - static_cast<short>(rcGesamt.bottom / 2);

            Chance = 0;

            BootsFahrt = false;
            if (!BootsFahrt) World::ChangeBootsFahrt();

            Tag = 1;
            Stunden = 0;
            Minuten = 0;

            Spielzustand = State::INTRO;
            Guy.Aktiv = false;
            Guy.Zustand = GUY_SHIP;
            Guy.AkNummer = 0;
            Guy.Aktion = Action::INTRO;
        }

        // SchriftSurface löschen
        rcRectdes.left = 0;
        rcRectdes.top = 0;
        rcRectdes.right = MAX_SCREEN_X;
        rcRectdes.bottom = MAX_SCREEN_Y;
        ddbltfx.dwFillColor = Renderer::RGB2DWORD(255, 0, 255);
//        lpDDSSchrift->Blt(&rcRectdes, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);

        bool Anitmp = LAnimation;
        bool Entdeckttmp[MAX_TILES_X][MAX_TILESY];

        LAnimation = false;
        // Schatzvergraben und Schatzkarte malen
        for (y = 0; y < MAX_TILESY; y++)
            for (x = 0; x < MAX_TILES_X; x++)
            {
                Entdeckttmp[x][y] = Scape[x][y].Entdeckt;
                Scape[x][y].Entdeckt = true;
            }

        World::Generate(); // Einmal vor dem Schatz schon entdeckt malen
        World::Schatz();

        for (y = 0; y < MAX_TILESY; y++) for (x = 0; x < MAX_TILES_X; x++) Scape[x][y].Entdeckt = Entdeckttmp[x][y];
        World::Entdecken();
        LAnimation = Anitmp;
        World::Generate(); // Und nochmal ohne das die Gegend entdeckt ist
        Guy.PosAlt = Guy.PosScreen;
    }
} // namespace Game
