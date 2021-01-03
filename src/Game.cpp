#include "Game.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

#include <fstream>

namespace Game {
void SaveGame()
{
    std::ofstream ofs("save.dat", std::ios::binary);

    if (!ofs) {
        return;
    }

    ofs.write(reinterpret_cast<char *>(Landscape), sizeof(Landscape));
    ofs.write(reinterpret_cast<char *>(&Guy), sizeof(Guy));
    ofs.write(reinterpret_cast<char *>(&IsInBoat), sizeof(IsInBoat));
    ofs.write(reinterpret_cast<char *>(&Camera), sizeof(Camera));
    ofs.write(reinterpret_cast<char *>(&Chance), sizeof(Chance));
    ofs.write(reinterpret_cast<char *>(&Gitter), sizeof(Gitter));
    ofs.write(reinterpret_cast<char *>(&HauptMenue), sizeof(HauptMenue));
    ofs.write(reinterpret_cast<char *>(&LAnimation), sizeof(LAnimation));
    ofs.write(reinterpret_cast<char *>(&Minutes), sizeof(Minutes));
    ofs.write(reinterpret_cast<char *>(&ScapeGrenze), sizeof(ScapeGrenze));
    ofs.write(reinterpret_cast<char *>(&SchatzPos), sizeof(SchatzPos));
    ofs.write(reinterpret_cast<char *>(&Spielzustand), sizeof(Spielzustand));
    ofs.write(reinterpret_cast<char *>(&Hours), sizeof(Hours));
    ofs.write(reinterpret_cast<char *>(&Tag), sizeof(Tag));
    ofs.write(reinterpret_cast<char *>(TextBereich), sizeof(TextBereich));
    ofs.write(reinterpret_cast<char *>(&SchatzGef), sizeof(SchatzGef));

    for (short i = 0; i < SPRITE_COUNT; i++) {
        ofs.write(reinterpret_cast<char *>(&Bmp[i].IsAnimationRunning), sizeof(Bmp[i].IsAnimationRunning));
        ofs.write(reinterpret_cast<char *>(&Bmp[i].AnimationPhase), sizeof(Bmp[i].AnimationPhase));
        ofs.write(reinterpret_cast<char *>(&Bmp[i].First), sizeof(Bmp[i].First));
    }
}

bool LoadGame()
{
    std::ifstream ifs("save.dat", std::ios::binary); // | ios::nocreate);

    if (!ifs) {
        return (false);
    }

    ifs.read(reinterpret_cast<char *>(Landscape), sizeof(Landscape));
    ifs.read(reinterpret_cast<char *>(&Guy), sizeof(Guy));
    ifs.read(reinterpret_cast<char *>(&IsInBoat), sizeof(IsInBoat));
    ifs.read(reinterpret_cast<char *>(&Camera), sizeof(Camera));
    ifs.read(reinterpret_cast<char *>(&Chance), sizeof(Chance));
    ifs.read(reinterpret_cast<char *>(&Gitter), sizeof(Gitter));
    ifs.read(reinterpret_cast<char *>(&HauptMenue), sizeof(HauptMenue));
    ifs.read(reinterpret_cast<char *>(&LAnimation), sizeof(LAnimation));
    ifs.read(reinterpret_cast<char *>(&Minutes), sizeof(Minutes));
    ifs.read(reinterpret_cast<char *>(&ScapeGrenze), sizeof(ScapeGrenze));
    ifs.read(reinterpret_cast<char *>(&SchatzPos), sizeof(SchatzPos));
    ifs.read(reinterpret_cast<char *>(&Spielzustand), sizeof(Spielzustand));
    ifs.read(reinterpret_cast<char *>(&Hours), sizeof(Hours));
    ifs.read(reinterpret_cast<char *>(&Tag), sizeof(Tag));
    ifs.read(reinterpret_cast<char *>(TextBereich), sizeof(TextBereich));
    ifs.read(reinterpret_cast<char *>(&SchatzGef), sizeof(SchatzGef));

    for (short i = 0; i < SPRITE_COUNT; i++) {
        ifs.read(reinterpret_cast<char *>(&Bmp[i].IsAnimationRunning), sizeof(Bmp[i].IsAnimationRunning));
        ifs.read(reinterpret_cast<char *>(&Bmp[i].AnimationPhase), sizeof(Bmp[i].AnimationPhase));
        ifs.read(reinterpret_cast<char *>(&Bmp[i].First), sizeof(Bmp[i].First));
    }

    return (true);
}

void InitStructs()
{
    short i, k;

    // BILD
    // Standardbildinitialisierung
    for (i = 0; i < SPRITE_COUNT; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].AnimationPhaseCount = 0;
        Bmp[i].Speed = 0;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = nullptr;
        Bmp[i].sourceRect.left = 0;
        Bmp[i].sourceRect.right = 0;
        Bmp[i].sourceRect.top = 0;
        Bmp[i].sourceRect.bottom = 0;
        Bmp[i].targetRect.left = 0;
        Bmp[i].targetRect.right = 0;
        Bmp[i].targetRect.top = 0;
        Bmp[i].targetRect.bottom = 0;
        Bmp[i].Width = 0;
        Bmp[i].Height = 0;
        Bmp[i].Sound = 0;

        for (k = 0; k < SPRITE_COUNT; k++) {
            Bmp[i].RequiredRawMaterials[k] = 0;
        }

        Bmp[i].RequiredActionCases = 0;
        Bmp[i].First = true;
    }

    // Guy
    for (i = GUY_LEFT; i <= GUY_BELOW; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].AnimationPhaseCount = 4;
        Bmp[i].Speed = 20;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSGuyAni;
        Bmp[i].sourceRect.left = 7 * (i - GUY_LEFT);
        Bmp[i].sourceRect.right = 7 + 7 * (i - GUY_LEFT);
        Bmp[i].sourceRect.top = 0;
        Bmp[i].sourceRect.bottom = 18;
        Bmp[i].Width = 7;
        Bmp[i].Height = 18;
    }

    for (i = GUY_SEARCH; i <= GUY_SLINGSHOT; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSGuyAni;
    }

    Bmp[GUY_SEARCH].AnimationPhaseCount = 4;
    Bmp[GUY_SEARCH].Speed = 4;
    Bmp[GUY_SEARCH].sourceRect.left = 28;
    Bmp[GUY_SEARCH].sourceRect.right = 39;
    Bmp[GUY_SEARCH].sourceRect.top = 0;
    Bmp[GUY_SEARCH].sourceRect.bottom = 14;
    Bmp[GUY_SEARCH].Width = 11;
    Bmp[GUY_SEARCH].Height = 14;
    Bmp[GUY_SEARCH].Sound = Sound::CRACKLE;

    Bmp[GUY_EAT].AnimationPhaseCount = 4;
    Bmp[GUY_EAT].Speed = 4;
    Bmp[GUY_EAT].sourceRect.left = 39;
    Bmp[GUY_EAT].sourceRect.right = 39 + 7;
    Bmp[GUY_EAT].sourceRect.top = 0;
    Bmp[GUY_EAT].sourceRect.bottom = 17;
    Bmp[GUY_EAT].Width = 7;
    Bmp[GUY_EAT].Height = 17;
    Bmp[GUY_EAT].Sound = Sound::CRACKLE;

    Bmp[GUY_DRINK].AnimationPhaseCount = 5;
    Bmp[GUY_DRINK].Speed = 4;
    Bmp[GUY_DRINK].sourceRect.left = 46;
    Bmp[GUY_DRINK].sourceRect.right = 46 + 9;
    Bmp[GUY_DRINK].sourceRect.top = 0;
    Bmp[GUY_DRINK].sourceRect.bottom = 13;
    Bmp[GUY_DRINK].Width = 9;
    Bmp[GUY_DRINK].Height = 13;
    Bmp[GUY_DRINK].Sound = Sound::DRINK;

    Bmp[GUY_CHOP].AnimationPhaseCount = 4;
    Bmp[GUY_CHOP].Speed = 6;
    Bmp[GUY_CHOP].sourceRect.left = 55;
    Bmp[GUY_CHOP].sourceRect.right = 55 + 15;
    Bmp[GUY_CHOP].sourceRect.top = 0;
    Bmp[GUY_CHOP].sourceRect.bottom = 19;
    Bmp[GUY_CHOP].Width = 15;
    Bmp[GUY_CHOP].Height = 19;
    Bmp[GUY_CHOP].Sound = Sound::LOG;

    Bmp[GUY_WAITING].AnimationPhaseCount = 4;
    Bmp[GUY_WAITING].Speed = 2;
    Bmp[GUY_WAITING].sourceRect.left = 70;
    Bmp[GUY_WAITING].sourceRect.right = 70 + 7;
    Bmp[GUY_WAITING].sourceRect.top = 0;
    Bmp[GUY_WAITING].sourceRect.bottom = 18;
    Bmp[GUY_WAITING].Width = 7;
    Bmp[GUY_WAITING].Height = 18;

    Bmp[GUY_FARMING].AnimationPhaseCount = 4;
    Bmp[GUY_FARMING].Speed = 4;
    Bmp[GUY_FARMING].sourceRect.left = 78;
    Bmp[GUY_FARMING].sourceRect.right = 78 + 19;
    Bmp[GUY_FARMING].sourceRect.top = 0;
    Bmp[GUY_FARMING].sourceRect.bottom = 18;
    Bmp[GUY_FARMING].Width = 19;
    Bmp[GUY_FARMING].Height = 18;

    Bmp[GUY_TIE_UP].AnimationPhaseCount = 2;
    Bmp[GUY_TIE_UP].Speed = 1;
    Bmp[GUY_TIE_UP].sourceRect.left = 97;
    Bmp[GUY_TIE_UP].sourceRect.right = 97 + 8;
    Bmp[GUY_TIE_UP].sourceRect.top = 0;
    Bmp[GUY_TIE_UP].sourceRect.bottom = 18;
    Bmp[GUY_TIE_UP].Width = 8;
    Bmp[GUY_TIE_UP].Height = 18;

    Bmp[GUY_TIE_DOWN].AnimationPhaseCount = 2;
    Bmp[GUY_TIE_DOWN].Speed = 1;
    Bmp[GUY_TIE_DOWN].sourceRect.left = 98;
    Bmp[GUY_TIE_DOWN].sourceRect.right = 98 + 7;
    Bmp[GUY_TIE_DOWN].sourceRect.top = 36;
    Bmp[GUY_TIE_DOWN].sourceRect.bottom = 36 + 18;
    Bmp[GUY_TIE_DOWN].Width = 7;
    Bmp[GUY_TIE_DOWN].Height = 18;

    Bmp[GUY_SLEEPING_TENT].AnimationPhaseCount = 2;
    Bmp[GUY_SLEEPING_TENT].Speed = 1;
    Bmp[GUY_SLEEPING_TENT].sourceRect.left = 105;
    Bmp[GUY_SLEEPING_TENT].sourceRect.right = 105 + 20;
    Bmp[GUY_SLEEPING_TENT].sourceRect.top = 54;
    Bmp[GUY_SLEEPING_TENT].sourceRect.bottom = 54 + 10;
    Bmp[GUY_SLEEPING_TENT].Width = 20;
    Bmp[GUY_SLEEPING_TENT].Height = 10;
    Bmp[GUY_SLEEPING_TENT].Sound = Sound::SNORE;

    Bmp[GUY_SLEEPING].AnimationPhaseCount = 2;
    Bmp[GUY_SLEEPING].Speed = 1;
    Bmp[GUY_SLEEPING].sourceRect.left = 125;
    Bmp[GUY_SLEEPING].sourceRect.right = 125 + 17;
    Bmp[GUY_SLEEPING].sourceRect.top = 36;
    Bmp[GUY_SLEEPING].sourceRect.bottom = 36 + 18;
    Bmp[GUY_SLEEPING].Width = 17;
    Bmp[GUY_SLEEPING].Height = 18;
    Bmp[GUY_SLEEPING].Sound = Sound::SNORE;

    Bmp[GUY_ENTER_TENT].AnimationPhaseCount = 3;
    Bmp[GUY_ENTER_TENT].Speed = 4;
    Bmp[GUY_ENTER_TENT].sourceRect.left = 105;
    Bmp[GUY_ENTER_TENT].sourceRect.right = 105 + 7;
    Bmp[GUY_ENTER_TENT].sourceRect.top = 0;
    Bmp[GUY_ENTER_TENT].sourceRect.bottom = 0 + 18;
    Bmp[GUY_ENTER_TENT].Width = 7;
    Bmp[GUY_ENTER_TENT].Height = 18;

    Bmp[GUY_LAYING_TENT].AnimationPhaseCount = 2;
    Bmp[GUY_LAYING_TENT].Speed = 2;
    Bmp[GUY_LAYING_TENT].sourceRect.left = 125;
    Bmp[GUY_LAYING_TENT].sourceRect.right = 125 + 17;
    Bmp[GUY_LAYING_TENT].sourceRect.top = 0;
    Bmp[GUY_LAYING_TENT].sourceRect.bottom = 0 + 18;
    Bmp[GUY_LAYING_TENT].Width = 17;
    Bmp[GUY_LAYING_TENT].Height = 18;

    Bmp[GUY_STAND_UP].AnimationPhaseCount = 2;
    Bmp[GUY_STAND_UP].Speed = 2;
    Bmp[GUY_STAND_UP].sourceRect.left = 142;
    Bmp[GUY_STAND_UP].sourceRect.right = 142 + 9;
    Bmp[GUY_STAND_UP].sourceRect.top = 0;
    Bmp[GUY_STAND_UP].sourceRect.bottom = 0 + 18;
    Bmp[GUY_STAND_UP].Width = 9;
    Bmp[GUY_STAND_UP].Height = 18;

    Bmp[GUY_FISHING_LEFT_1].AnimationPhaseCount = 6;
    Bmp[GUY_FISHING_LEFT_1].Speed = 6;
    Bmp[GUY_FISHING_LEFT_1].sourceRect.left = 151;
    Bmp[GUY_FISHING_LEFT_1].sourceRect.right = 151 + 16;
    Bmp[GUY_FISHING_LEFT_1].sourceRect.top = 0;
    Bmp[GUY_FISHING_LEFT_1].sourceRect.bottom = 0 + 17;
    Bmp[GUY_FISHING_LEFT_1].Width = 16;
    Bmp[GUY_FISHING_LEFT_1].Height = 17;

    Bmp[GUY_FISHING_ABOVE_1].AnimationPhaseCount = 6;
    Bmp[GUY_FISHING_ABOVE_1].Speed = 6;
    Bmp[GUY_FISHING_ABOVE_1].sourceRect.left = 167;
    Bmp[GUY_FISHING_ABOVE_1].sourceRect.right = 167 + 16;
    Bmp[GUY_FISHING_ABOVE_1].sourceRect.top = 0;
    Bmp[GUY_FISHING_ABOVE_1].sourceRect.bottom = 0 + 17;
    Bmp[GUY_FISHING_ABOVE_1].Width = 16;
    Bmp[GUY_FISHING_ABOVE_1].Height = 17;

    Bmp[GUY_FISHING_RIGHT_1].AnimationPhaseCount = 6;
    Bmp[GUY_FISHING_RIGHT_1].Speed = 6;
    Bmp[GUY_FISHING_RIGHT_1].sourceRect.left = 183;
    Bmp[GUY_FISHING_RIGHT_1].sourceRect.right = 183 + 14;
    Bmp[GUY_FISHING_RIGHT_1].sourceRect.top = 0;
    Bmp[GUY_FISHING_RIGHT_1].sourceRect.bottom = 0 + 17;
    Bmp[GUY_FISHING_RIGHT_1].Width = 14;
    Bmp[GUY_FISHING_RIGHT_1].Height = 17;

    Bmp[GUY_FISHING_BELOW_1].AnimationPhaseCount = 6;
    Bmp[GUY_FISHING_BELOW_1].Speed = 6;
    Bmp[GUY_FISHING_BELOW_1].sourceRect.left = 197;
    Bmp[GUY_FISHING_BELOW_1].sourceRect.right = 197 + 14;
    Bmp[GUY_FISHING_BELOW_1].sourceRect.top = 0;
    Bmp[GUY_FISHING_BELOW_1].sourceRect.bottom = 0 + 17;
    Bmp[GUY_FISHING_BELOW_1].Width = 14;
    Bmp[GUY_FISHING_BELOW_1].Height = 17;

    Bmp[GUY_FISHING_LEFT_2].AnimationPhaseCount = 4;
    Bmp[GUY_FISHING_LEFT_2].Speed = 3;
    Bmp[GUY_FISHING_LEFT_2].sourceRect.left = 211;
    Bmp[GUY_FISHING_LEFT_2].sourceRect.right = 211 + 16;
    Bmp[GUY_FISHING_LEFT_2].sourceRect.top = 0;
    Bmp[GUY_FISHING_LEFT_2].sourceRect.bottom = 0 + 16;
    Bmp[GUY_FISHING_LEFT_2].Width = 16;
    Bmp[GUY_FISHING_LEFT_2].Height = 16;

    Bmp[GUY_FISHING_ABOVE_2].AnimationPhaseCount = 4;
    Bmp[GUY_FISHING_ABOVE_2].Speed = 3;
    Bmp[GUY_FISHING_ABOVE_2].sourceRect.left = 227;
    Bmp[GUY_FISHING_ABOVE_2].sourceRect.right = 227 + 16;
    Bmp[GUY_FISHING_ABOVE_2].sourceRect.top = 0;
    Bmp[GUY_FISHING_ABOVE_2].sourceRect.bottom = 0 + 16;
    Bmp[GUY_FISHING_ABOVE_2].Width = 16;
    Bmp[GUY_FISHING_ABOVE_2].Height = 16;

    Bmp[GUY_FISHING_RIGHT_2].AnimationPhaseCount = 4;
    Bmp[GUY_FISHING_RIGHT_2].Speed = 3;
    Bmp[GUY_FISHING_RIGHT_2].sourceRect.left = 243;
    Bmp[GUY_FISHING_RIGHT_2].sourceRect.right = 243 + 14;
    Bmp[GUY_FISHING_RIGHT_2].sourceRect.top = 0;
    Bmp[GUY_FISHING_RIGHT_2].sourceRect.bottom = 0 + 15;
    Bmp[GUY_FISHING_RIGHT_2].Width = 14;
    Bmp[GUY_FISHING_RIGHT_2].Height = 15;

    Bmp[GUY_FISHING_BELOW_2].AnimationPhaseCount = 4;
    Bmp[GUY_FISHING_BELOW_2].Speed = 3;
    Bmp[GUY_FISHING_BELOW_2].sourceRect.left = 257;
    Bmp[GUY_FISHING_BELOW_2].sourceRect.right = 257 + 14;
    Bmp[GUY_FISHING_BELOW_2].sourceRect.top = 0;
    Bmp[GUY_FISHING_BELOW_2].sourceRect.bottom = 0 + 15;
    Bmp[GUY_FISHING_BELOW_2].Width = 14;
    Bmp[GUY_FISHING_BELOW_2].Height = 15;

    Bmp[GUY_FISHING_LEFT_3].AnimationPhaseCount = 3;
    Bmp[GUY_FISHING_LEFT_3].Speed = 2;
    Bmp[GUY_FISHING_LEFT_3].sourceRect.left = 271;
    Bmp[GUY_FISHING_LEFT_3].sourceRect.right = 271 + 16;
    Bmp[GUY_FISHING_LEFT_3].sourceRect.top = 0;
    Bmp[GUY_FISHING_LEFT_3].sourceRect.bottom = 0 + 16;
    Bmp[GUY_FISHING_LEFT_3].Width = 16;
    Bmp[GUY_FISHING_LEFT_3].Height = 16;

    Bmp[GUY_FISHING_ABOVE_3].AnimationPhaseCount = 3;
    Bmp[GUY_FISHING_ABOVE_3].Speed = 2;
    Bmp[GUY_FISHING_ABOVE_3].sourceRect.left = 285;
    Bmp[GUY_FISHING_ABOVE_3].sourceRect.right = 285 + 16;
    Bmp[GUY_FISHING_ABOVE_3].sourceRect.top = 0;
    Bmp[GUY_FISHING_ABOVE_3].sourceRect.bottom = 0 + 16;
    Bmp[GUY_FISHING_ABOVE_3].Width = 16;
    Bmp[GUY_FISHING_ABOVE_3].Height = 16;

    Bmp[GUY_FISHING_RIGHT_3].AnimationPhaseCount = 3;
    Bmp[GUY_FISHING_RIGHT_3].Speed = 2;
    Bmp[GUY_FISHING_RIGHT_3].sourceRect.left = 299;
    Bmp[GUY_FISHING_RIGHT_3].sourceRect.right = 299 + 14;
    Bmp[GUY_FISHING_RIGHT_3].sourceRect.top = 0;
    Bmp[GUY_FISHING_RIGHT_3].sourceRect.bottom = 0 + 15;
    Bmp[GUY_FISHING_RIGHT_3].Width = 14;
    Bmp[GUY_FISHING_RIGHT_3].Height = 15;

    Bmp[GUY_FISHING_BELOW_3].AnimationPhaseCount = 3;
    Bmp[GUY_FISHING_BELOW_3].Speed = 2;
    Bmp[GUY_FISHING_BELOW_3].sourceRect.left = 313;
    Bmp[GUY_FISHING_BELOW_3].sourceRect.right = 313 + 14;
    Bmp[GUY_FISHING_BELOW_3].sourceRect.top = 0;
    Bmp[GUY_FISHING_BELOW_3].sourceRect.bottom = 0 + 15;
    Bmp[GUY_FISHING_BELOW_3].Width = 14;
    Bmp[GUY_FISHING_BELOW_3].Height = 15;

    Bmp[GUY_BEAT].AnimationPhaseCount = 4;
    Bmp[GUY_BEAT].Speed = 7;
    Bmp[GUY_BEAT].sourceRect.left = 327;
    Bmp[GUY_BEAT].sourceRect.right = 327 + 12;
    Bmp[GUY_BEAT].sourceRect.top = 0;
    Bmp[GUY_BEAT].sourceRect.bottom = 0 + 24;
    Bmp[GUY_BEAT].Width = 12;
    Bmp[GUY_BEAT].Height = 24;
    Bmp[GUY_BEAT].Sound = Sound::HIT;

    Bmp[GUY_BOAT_LEFT].AnimationPhaseCount = 6;
    Bmp[GUY_BOAT_LEFT].Speed = 10;
    Bmp[GUY_BOAT_LEFT].sourceRect.left = 339;
    Bmp[GUY_BOAT_LEFT].sourceRect.right = 339 + 26;
    Bmp[GUY_BOAT_LEFT].sourceRect.top = 0;
    Bmp[GUY_BOAT_LEFT].sourceRect.bottom = 0 + 21;
    Bmp[GUY_BOAT_LEFT].Width = 26;
    Bmp[GUY_BOAT_LEFT].Height = 21;

    Bmp[GUY_BOAT_ABOVE].AnimationPhaseCount = 6;
    Bmp[GUY_BOAT_ABOVE].Speed = 10;
    Bmp[GUY_BOAT_ABOVE].sourceRect.left = 365;
    Bmp[GUY_BOAT_ABOVE].sourceRect.right = 365 + 26;
    Bmp[GUY_BOAT_ABOVE].sourceRect.top = 0;
    Bmp[GUY_BOAT_ABOVE].sourceRect.bottom = 0 + 21;
    Bmp[GUY_BOAT_ABOVE].Width = 26;
    Bmp[GUY_BOAT_ABOVE].Height = 21;

    Bmp[GUY_BOAT_RIGHT].AnimationPhaseCount = 6;
    Bmp[GUY_BOAT_RIGHT].Speed = 10;
    Bmp[GUY_BOAT_RIGHT].sourceRect.left = 391;
    Bmp[GUY_BOAT_RIGHT].sourceRect.right = 391 + 26;
    Bmp[GUY_BOAT_RIGHT].sourceRect.top = 0;
    Bmp[GUY_BOAT_RIGHT].sourceRect.bottom = 0 + 21;
    Bmp[GUY_BOAT_RIGHT].Width = 26;
    Bmp[GUY_BOAT_RIGHT].Height = 21;

    Bmp[GUY_BOAT_BELOW].AnimationPhaseCount = 6;
    Bmp[GUY_BOAT_BELOW].Speed = 10;
    Bmp[GUY_BOAT_BELOW].sourceRect.left = 417;
    Bmp[GUY_BOAT_BELOW].sourceRect.right = 417 + 26;
    Bmp[GUY_BOAT_BELOW].sourceRect.top = 0;
    Bmp[GUY_BOAT_BELOW].sourceRect.bottom = 0 + 21;
    Bmp[GUY_BOAT_BELOW].Width = 26;
    Bmp[GUY_BOAT_BELOW].Height = 21;

    Bmp[GUY_BOAT_FISHING_1].AnimationPhaseCount = 6;
    Bmp[GUY_BOAT_FISHING_1].Speed = 6;
    Bmp[GUY_BOAT_FISHING_1].sourceRect.left = 443;
    Bmp[GUY_BOAT_FISHING_1].sourceRect.right = 443 + 26;
    Bmp[GUY_BOAT_FISHING_1].sourceRect.top = 0;
    Bmp[GUY_BOAT_FISHING_1].sourceRect.bottom = 0 + 25;
    Bmp[GUY_BOAT_FISHING_1].Width = 26;
    Bmp[GUY_BOAT_FISHING_1].Height = 25;

    Bmp[GUY_BOAT_FISHING_2].AnimationPhaseCount = 4;
    Bmp[GUY_BOAT_FISHING_2].Speed = 3;
    Bmp[GUY_BOAT_FISHING_2].sourceRect.left = 469;
    Bmp[GUY_BOAT_FISHING_2].sourceRect.right = 469 + 26;
    Bmp[GUY_BOAT_FISHING_2].sourceRect.top = 0;
    Bmp[GUY_BOAT_FISHING_2].sourceRect.bottom = 0 + 25;
    Bmp[GUY_BOAT_FISHING_2].Width = 26;
    Bmp[GUY_BOAT_FISHING_2].Height = 25;

    Bmp[GUY_BOAT_FISHING_3].AnimationPhaseCount = 3;
    Bmp[GUY_BOAT_FISHING_3].Speed = 2;
    Bmp[GUY_BOAT_FISHING_3].sourceRect.left = 495;
    Bmp[GUY_BOAT_FISHING_3].sourceRect.right = 495 + 26;
    Bmp[GUY_BOAT_FISHING_3].sourceRect.top = 0;
    Bmp[GUY_BOAT_FISHING_3].sourceRect.bottom = 0 + 25;
    Bmp[GUY_BOAT_FISHING_3].Width = 26;
    Bmp[GUY_BOAT_FISHING_3].Height = 25;

    Bmp[GUY_DIVING_1].AnimationPhaseCount = 5;
    Bmp[GUY_DIVING_1].Speed = 5;
    Bmp[GUY_DIVING_1].sourceRect.left = 521;
    Bmp[GUY_DIVING_1].sourceRect.right = 521 + 26;
    Bmp[GUY_DIVING_1].sourceRect.top = 0;
    Bmp[GUY_DIVING_1].sourceRect.bottom = 0 + 27;
    Bmp[GUY_DIVING_1].Width = 26;
    Bmp[GUY_DIVING_1].Height = 27;

    Bmp[GUY_DIVING_2].AnimationPhaseCount = 4;
    Bmp[GUY_DIVING_2].Speed = 3;
    Bmp[GUY_DIVING_2].sourceRect.left = 547;
    Bmp[GUY_DIVING_2].sourceRect.right = 547 + 26;
    Bmp[GUY_DIVING_2].sourceRect.top = 0;
    Bmp[GUY_DIVING_2].sourceRect.bottom = 0 + 17;
    Bmp[GUY_DIVING_2].Width = 26;
    Bmp[GUY_DIVING_2].Height = 17;

    Bmp[GUY_DIVING_3].AnimationPhaseCount = 2;
    Bmp[GUY_DIVING_3].Speed = 2;
    Bmp[GUY_DIVING_3].sourceRect.left = 573;
    Bmp[GUY_DIVING_3].sourceRect.right = 573 + 26;
    Bmp[GUY_DIVING_3].sourceRect.top = 0;
    Bmp[GUY_DIVING_3].sourceRect.bottom = 0 + 17;
    Bmp[GUY_DIVING_3].Width = 26;
    Bmp[GUY_DIVING_3].Height = 17;

    Bmp[GUY_HAMMER_1].AnimationPhaseCount = 2;
    Bmp[GUY_HAMMER_1].Speed = 4;
    Bmp[GUY_HAMMER_1].sourceRect.left = 599;
    Bmp[GUY_HAMMER_1].sourceRect.right = 599 + 9;
    Bmp[GUY_HAMMER_1].sourceRect.top = 0;
    Bmp[GUY_HAMMER_1].sourceRect.bottom = 0 + 18;
    Bmp[GUY_HAMMER_1].Width = 9;
    Bmp[GUY_HAMMER_1].Height = 18;
    Bmp[GUY_HAMMER_1].Sound = Sound::HAMMER;

    Bmp[GUY_CLIMBING_1].AnimationPhaseCount = 6;
    Bmp[GUY_CLIMBING_1].Speed = 2;
    Bmp[GUY_CLIMBING_1].sourceRect.left = 608;
    Bmp[GUY_CLIMBING_1].sourceRect.right = 608 + 7;
    Bmp[GUY_CLIMBING_1].sourceRect.top = 0;
    Bmp[GUY_CLIMBING_1].sourceRect.bottom = 0 + 34;
    Bmp[GUY_CLIMBING_1].Width = 7;
    Bmp[GUY_CLIMBING_1].Height = 34;

    Bmp[GUY_CLIMBING_2].AnimationPhaseCount = 6;
    Bmp[GUY_CLIMBING_2].Speed = 2;
    Bmp[GUY_CLIMBING_2].sourceRect.left = 615;
    Bmp[GUY_CLIMBING_2].sourceRect.right = 615 + 7;
    Bmp[GUY_CLIMBING_2].sourceRect.top = 0;
    Bmp[GUY_CLIMBING_2].sourceRect.bottom = 0 + 34;
    Bmp[GUY_CLIMBING_2].Width = 7;
    Bmp[GUY_CLIMBING_2].Height = 34;

    Bmp[GUY_HAMMER_2].AnimationPhaseCount = 2;
    Bmp[GUY_HAMMER_2].Speed = 4;
    Bmp[GUY_HAMMER_2].sourceRect.left = 622;
    Bmp[GUY_HAMMER_2].sourceRect.right = 622 + 7;
    Bmp[GUY_HAMMER_2].sourceRect.top = 0;
    Bmp[GUY_HAMMER_2].sourceRect.bottom = 0 + 34;
    Bmp[GUY_HAMMER_2].Width = 7;
    Bmp[GUY_HAMMER_2].Height = 34;
    Bmp[GUY_HAMMER_2].Sound = Sound::HAMMER;

    Bmp[GUY_ENTER_HOUSE].AnimationPhaseCount = 3;
    Bmp[GUY_ENTER_HOUSE].Speed = 2;
    Bmp[GUY_ENTER_HOUSE].sourceRect.left = 631;
    Bmp[GUY_ENTER_HOUSE].sourceRect.right = 631 + 9;
    Bmp[GUY_ENTER_HOUSE].sourceRect.top = 0;
    Bmp[GUY_ENTER_HOUSE].sourceRect.bottom = 0 + 34;
    Bmp[GUY_ENTER_HOUSE].Width = 9;
    Bmp[GUY_ENTER_HOUSE].Height = 34;

    Bmp[GUY_SLEEP_HOUSE].AnimationPhaseCount = 2;
    Bmp[GUY_SLEEP_HOUSE].Speed = 1;
    Bmp[GUY_SLEEP_HOUSE].sourceRect.left = 640;
    Bmp[GUY_SLEEP_HOUSE].sourceRect.right = 640 + 10;
    Bmp[GUY_SLEEP_HOUSE].sourceRect.top = 0;
    Bmp[GUY_SLEEP_HOUSE].sourceRect.bottom = 0 + 34;
    Bmp[GUY_SLEEP_HOUSE].Width = 10;
    Bmp[GUY_SLEEP_HOUSE].Height = 34;
    Bmp[GUY_SLEEP_HOUSE].Sound = Sound::SNORE;

    Bmp[GUY_EXIT_HOUSE].AnimationPhaseCount = 3;
    Bmp[GUY_EXIT_HOUSE].Speed = 2;
    Bmp[GUY_EXIT_HOUSE].sourceRect.left = 650;
    Bmp[GUY_EXIT_HOUSE].sourceRect.right = 650 + 9;
    Bmp[GUY_EXIT_HOUSE].sourceRect.top = 0;
    Bmp[GUY_EXIT_HOUSE].sourceRect.bottom = 0 + 34;
    Bmp[GUY_EXIT_HOUSE].Width = 9;
    Bmp[GUY_EXIT_HOUSE].Height = 34;

    Bmp[GUY_SET_FIRE].AnimationPhaseCount = 6;
    Bmp[GUY_SET_FIRE].Speed = 5;
    Bmp[GUY_SET_FIRE].sourceRect.left = 659;
    Bmp[GUY_SET_FIRE].sourceRect.right = 659 + 19;
    Bmp[GUY_SET_FIRE].sourceRect.top = 0;
    Bmp[GUY_SET_FIRE].sourceRect.bottom = 0 + 18;
    Bmp[GUY_SET_FIRE].Width = 19;
    Bmp[GUY_SET_FIRE].Height = 18;

    Bmp[GUY_LOOK_OUT].AnimationPhaseCount = 4;
    Bmp[GUY_LOOK_OUT].Speed = 1;
    Bmp[GUY_LOOK_OUT].sourceRect.left = 678;
    Bmp[GUY_LOOK_OUT].sourceRect.right = 678 + 10;
    Bmp[GUY_LOOK_OUT].sourceRect.top = 0;
    Bmp[GUY_LOOK_OUT].sourceRect.bottom = 0 + 18;
    Bmp[GUY_LOOK_OUT].Width = 10;
    Bmp[GUY_LOOK_OUT].Height = 18;

    Bmp[GUY_SHOVEL].AnimationPhaseCount = 10;
    Bmp[GUY_SHOVEL].Speed = 3;
    Bmp[GUY_SHOVEL].sourceRect.left = 688;
    Bmp[GUY_SHOVEL].sourceRect.right = 688 + 17;
    Bmp[GUY_SHOVEL].sourceRect.top = 0;
    Bmp[GUY_SHOVEL].sourceRect.bottom = 0 + 19;
    Bmp[GUY_SHOVEL].Width = 17;
    Bmp[GUY_SHOVEL].Height = 19;
    Bmp[GUY_SHOVEL].Sound = Sound::DIG;

    Bmp[GUY_SHIP].AnimationPhaseCount = 4;
    Bmp[GUY_SHIP].Speed = 10;
    Bmp[GUY_SHIP].sourceRect.left = 297;
    Bmp[GUY_SHIP].sourceRect.right = 297 + 48;
    Bmp[GUY_SHIP].sourceRect.top = 0;
    Bmp[GUY_SHIP].sourceRect.bottom = 0 + 38;
    Bmp[GUY_SHIP].Width = 48;
    Bmp[GUY_SHIP].Height = 38;
    Bmp[GUY_SHIP].Surface = lpDDSBau;
    Bmp[GUY_SHIP].Sound = Sound::STORM;

    Bmp[GUY_SHIP_DOWN].AnimationPhaseCount = 6;
    Bmp[GUY_SHIP_DOWN].Speed = 3;
    Bmp[GUY_SHIP_DOWN].sourceRect.left = 345;
    Bmp[GUY_SHIP_DOWN].sourceRect.right = 345 + 46;
    Bmp[GUY_SHIP_DOWN].sourceRect.top = 0;
    Bmp[GUY_SHIP_DOWN].sourceRect.bottom = 0 + 40;
    Bmp[GUY_SHIP_DOWN].Width = 46;
    Bmp[GUY_SHIP_DOWN].Height = 40;
    Bmp[GUY_SHIP_DOWN].Surface = lpDDSBau;
    Bmp[GUY_SHIP_DOWN].Sound = Sound::SPLAT;

    Bmp[GUY_SWIM].AnimationPhaseCount = 4;
    Bmp[GUY_SWIM].Speed = 10;
    Bmp[GUY_SWIM].sourceRect.left = 705;
    Bmp[GUY_SWIM].sourceRect.right = 705 + 15;
    Bmp[GUY_SWIM].sourceRect.top = 0;
    Bmp[GUY_SWIM].sourceRect.bottom = 0 + 9;
    Bmp[GUY_SWIM].Width = 15;
    Bmp[GUY_SWIM].Height = 9;
    Bmp[GUY_SWIM].Sound = Sound::SWIM;

    Bmp[GUY_DEAD].AnimationPhaseCount = 6;
    Bmp[GUY_DEAD].Speed = 1;
    Bmp[GUY_DEAD].sourceRect.left = 743;
    Bmp[GUY_DEAD].sourceRect.right = 743 + 16;
    Bmp[GUY_DEAD].sourceRect.top = 0;
    Bmp[GUY_DEAD].sourceRect.bottom = 0 + 10;
    Bmp[GUY_DEAD].Width = 16;
    Bmp[GUY_DEAD].Height = 10;

    Bmp[GUY_BOAT_DEAD].AnimationPhaseCount = 6;
    Bmp[GUY_BOAT_DEAD].Speed = 1;
    Bmp[GUY_BOAT_DEAD].sourceRect.left = 759;
    Bmp[GUY_BOAT_DEAD].sourceRect.right = 759 + 26;
    Bmp[GUY_BOAT_DEAD].sourceRect.top = 0;
    Bmp[GUY_BOAT_DEAD].sourceRect.bottom = 0 + 18;
    Bmp[GUY_BOAT_DEAD].Width = 26;
    Bmp[GUY_BOAT_DEAD].Height = 18;

    Bmp[GUY_BOAT_WAITING].AnimationPhaseCount = 4;
    Bmp[GUY_BOAT_WAITING].Speed = 2;
    Bmp[GUY_BOAT_WAITING].sourceRect.left = 0;
    Bmp[GUY_BOAT_WAITING].sourceRect.right = 0 + 26;
    Bmp[GUY_BOAT_WAITING].sourceRect.top = 72;
    Bmp[GUY_BOAT_WAITING].sourceRect.bottom = 72 + 18;
    Bmp[GUY_BOAT_WAITING].Width = 26;
    Bmp[GUY_BOAT_WAITING].Height = 20;

    Bmp[GUY_SLINGSHOT].AnimationPhaseCount = 5;
    Bmp[GUY_SLINGSHOT].Speed = 4;
    Bmp[GUY_SLINGSHOT].sourceRect.left = 720;
    Bmp[GUY_SLINGSHOT].sourceRect.right = 720 + 23;
    Bmp[GUY_SLINGSHOT].sourceRect.top = 0;
    Bmp[GUY_SLINGSHOT].sourceRect.bottom = 0 + 20;
    Bmp[GUY_SLINGSHOT].Width = 23;
    Bmp[GUY_SLINGSHOT].Height = 20;

    // Cursor
    for (i = CURSOR_ARROW; i <= CURSOR_CLOCK; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].AnimationPhaseCount = 1;
        Bmp[i].Speed = 0;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSCursor;
        Bmp[i].sourceRect.left = (i - CURSOR_ARROW) * 18;
        Bmp[i].sourceRect.top = 0;
        Bmp[i].sourceRect.right = Bmp[i].sourceRect.left + 18;
        Bmp[i].sourceRect.bottom = 18;
        Bmp[i].Width = 18;
        Bmp[i].Height = 18;
    }

    // Landschaftsanimationen
    for (i = SEA_WAVES; i <= FLOODGATE_6; i++) {
        Bmp[i].IsAnimationRunning = true;
        Bmp[i].AnimationPhaseCount = 7;
        Bmp[i].Speed = 7;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSAnimation;
        Bmp[i].Sound = Sound::RIVER;
    }

    Bmp[SEA_WAVES].Speed = 3;
    Bmp[SEA_WAVES].Width = 28;
    Bmp[SEA_WAVES].Height = 13;
    Bmp[SEA_WAVES].sourceRect.left = 0;
    Bmp[SEA_WAVES].sourceRect.right = Bmp[SEA_WAVES].sourceRect.left + Bmp[SEA_WAVES].Width;
    Bmp[SEA_WAVES].sourceRect.top = 0;
    Bmp[SEA_WAVES].sourceRect.bottom = Bmp[SEA_WAVES].sourceRect.top + Bmp[SEA_WAVES].Height;
    Bmp[SEA_WAVES].targetRect.left = 9;
    Bmp[SEA_WAVES].targetRect.right = Bmp[SEA_WAVES].targetRect.left + Bmp[SEA_WAVES].Width;
    Bmp[SEA_WAVES].targetRect.top = 23;
    Bmp[SEA_WAVES].targetRect.bottom = Bmp[SEA_WAVES].targetRect.top + Bmp[SEA_WAVES].Height;
    Bmp[SEA_WAVES].Sound = Sound::SURF;

    Bmp[RIVER_1].Width = 34;
    Bmp[RIVER_1].Height = 8;
    Bmp[RIVER_1].sourceRect.left = 28;
    Bmp[RIVER_1].sourceRect.right = Bmp[RIVER_1].sourceRect.left + Bmp[RIVER_1].Width;
    Bmp[RIVER_1].sourceRect.top = 0;
    Bmp[RIVER_1].sourceRect.bottom = Bmp[RIVER_1].sourceRect.top + Bmp[RIVER_1].Height;
    Bmp[RIVER_1].targetRect.left = 10;
    Bmp[RIVER_1].targetRect.right = Bmp[RIVER_1].targetRect.left + Bmp[RIVER_1].Width;
    Bmp[RIVER_1].targetRect.top = 18;
    Bmp[RIVER_1].targetRect.bottom = Bmp[RIVER_1].targetRect.top + Bmp[RIVER_1].Height;

    Bmp[RIVER_2].Width = 34;
    Bmp[RIVER_2].Height = 8;
    Bmp[RIVER_2].sourceRect.left = 62;
    Bmp[RIVER_2].sourceRect.right = Bmp[RIVER_2].sourceRect.left + Bmp[RIVER_2].Width;
    Bmp[RIVER_2].sourceRect.top = 0;
    Bmp[RIVER_2].sourceRect.bottom = Bmp[RIVER_2].sourceRect.top + Bmp[RIVER_2].Height;
    Bmp[RIVER_2].targetRect.left = 10;
    Bmp[RIVER_2].targetRect.right = Bmp[RIVER_2].targetRect.left + Bmp[RIVER_2].Width;
    Bmp[RIVER_2].targetRect.top = 19;
    Bmp[RIVER_2].targetRect.bottom = Bmp[RIVER_2].targetRect.top + Bmp[RIVER_2].Height;

    Bmp[RIVER_3].Width = 34;
    Bmp[RIVER_3].Height = 34;
    Bmp[RIVER_3].sourceRect.left = 96;
    Bmp[RIVER_3].sourceRect.right = Bmp[RIVER_3].sourceRect.left + Bmp[RIVER_3].Width;
    Bmp[RIVER_3].sourceRect.top = 0;
    Bmp[RIVER_3].sourceRect.bottom = Bmp[RIVER_3].sourceRect.top + Bmp[RIVER_3].Height;
    Bmp[RIVER_3].targetRect.left = 10;
    Bmp[RIVER_3].targetRect.right = Bmp[RIVER_3].targetRect.left + Bmp[RIVER_3].Width;
    Bmp[RIVER_3].targetRect.top = 5;
    Bmp[RIVER_3].targetRect.bottom = Bmp[RIVER_3].targetRect.top + Bmp[RIVER_3].Height;

    Bmp[RIVER_4].Width = 34;
    Bmp[RIVER_4].Height = 34;
    Bmp[RIVER_4].sourceRect.left = 130;
    Bmp[RIVER_4].sourceRect.right = Bmp[RIVER_4].sourceRect.left + Bmp[RIVER_4].Width;
    Bmp[RIVER_4].sourceRect.top = 0;
    Bmp[RIVER_4].sourceRect.bottom = Bmp[RIVER_4].sourceRect.top + Bmp[RIVER_4].Height;
    Bmp[RIVER_4].targetRect.left = 10;
    Bmp[RIVER_4].targetRect.right = Bmp[RIVER_4].targetRect.left + Bmp[RIVER_4].Width;
    Bmp[RIVER_4].targetRect.top = 5;
    Bmp[RIVER_4].targetRect.bottom = Bmp[RIVER_4].targetRect.top + Bmp[RIVER_4].Height;

    Bmp[RIVER_5].Width = 34;
    Bmp[RIVER_5].Height = 18;
    Bmp[RIVER_5].sourceRect.left = 164;
    Bmp[RIVER_5].sourceRect.right = Bmp[RIVER_5].sourceRect.left + Bmp[RIVER_5].Width;
    Bmp[RIVER_5].sourceRect.top = 0;
    Bmp[RIVER_5].sourceRect.bottom = Bmp[RIVER_5].sourceRect.top + Bmp[RIVER_5].Height;
    Bmp[RIVER_5].targetRect.left = 10;
    Bmp[RIVER_5].targetRect.right = Bmp[RIVER_5].targetRect.left + Bmp[RIVER_5].Width;
    Bmp[RIVER_5].targetRect.top = 21;
    Bmp[RIVER_5].targetRect.bottom = Bmp[RIVER_5].targetRect.top + Bmp[RIVER_5].Height;

    Bmp[RIVER_6].Width = 34;
    Bmp[RIVER_6].Height = 18;
    Bmp[RIVER_6].sourceRect.left = 198;
    Bmp[RIVER_6].sourceRect.right = Bmp[RIVER_6].sourceRect.left + Bmp[RIVER_6].Width;
    Bmp[RIVER_6].sourceRect.top = 0;
    Bmp[RIVER_6].sourceRect.bottom = Bmp[RIVER_6].sourceRect.top + Bmp[RIVER_6].Height;
    Bmp[RIVER_6].targetRect.left = 10;
    Bmp[RIVER_6].targetRect.right = Bmp[RIVER_6].targetRect.left + Bmp[RIVER_6].Width;
    Bmp[RIVER_6].targetRect.top = 21;
    Bmp[RIVER_6].targetRect.bottom = Bmp[RIVER_6].targetRect.top + Bmp[RIVER_6].Height;

    Bmp[RIVER_7].Width = 18;
    Bmp[RIVER_7].Height = 18;
    Bmp[RIVER_7].sourceRect.left = 232;
    Bmp[RIVER_7].sourceRect.right = Bmp[RIVER_7].sourceRect.left + Bmp[RIVER_7].Width;
    Bmp[RIVER_7].sourceRect.top = 0;
    Bmp[RIVER_7].sourceRect.bottom = Bmp[RIVER_7].sourceRect.top + Bmp[RIVER_7].Height;
    Bmp[RIVER_7].targetRect.left = 10;
    Bmp[RIVER_7].targetRect.right = Bmp[RIVER_7].targetRect.left + Bmp[RIVER_7].Width;
    Bmp[RIVER_7].targetRect.top = 21;
    Bmp[RIVER_7].targetRect.bottom = Bmp[RIVER_7].targetRect.top + Bmp[RIVER_7].Height;

    Bmp[RIVER_8].Width = 18;
    Bmp[RIVER_8].Height = 18;
    Bmp[RIVER_8].sourceRect.left = 250;
    Bmp[RIVER_8].sourceRect.right = Bmp[RIVER_8].sourceRect.left + Bmp[RIVER_8].Width;
    Bmp[RIVER_8].sourceRect.top = 0;
    Bmp[RIVER_8].sourceRect.bottom = Bmp[RIVER_8].sourceRect.top + Bmp[RIVER_8].Height;
    Bmp[RIVER_8].targetRect.left = 26;
    Bmp[RIVER_8].targetRect.right = Bmp[RIVER_8].targetRect.left + Bmp[RIVER_8].Width;
    Bmp[RIVER_8].targetRect.top = 21;
    Bmp[RIVER_8].targetRect.bottom = Bmp[RIVER_8].targetRect.top + Bmp[RIVER_8].Height;

    Bmp[RIVER_9].Width = 34;
    Bmp[RIVER_9].Height = 15;
    Bmp[RIVER_9].sourceRect.left = 268;
    Bmp[RIVER_9].sourceRect.right = Bmp[RIVER_9].sourceRect.left + Bmp[RIVER_9].Width;
    Bmp[RIVER_9].sourceRect.top = 0;
    Bmp[RIVER_9].sourceRect.bottom = Bmp[RIVER_9].sourceRect.top + Bmp[RIVER_9].Height;
    Bmp[RIVER_9].targetRect.left = 10;
    Bmp[RIVER_9].targetRect.right = Bmp[RIVER_9].targetRect.left + Bmp[RIVER_9].Width;
    Bmp[RIVER_9].targetRect.top = 20;
    Bmp[RIVER_9].targetRect.bottom = Bmp[RIVER_9].targetRect.top + Bmp[RIVER_9].Height;

    Bmp[RIVER_10].Width = 34;
    Bmp[RIVER_10].Height = 13;
    Bmp[RIVER_10].sourceRect.left = 302;
    Bmp[RIVER_10].sourceRect.right = Bmp[RIVER_10].sourceRect.left + Bmp[RIVER_10].Width;
    Bmp[RIVER_10].sourceRect.top = 0;
    Bmp[RIVER_10].sourceRect.bottom = Bmp[RIVER_10].sourceRect.top + Bmp[RIVER_10].Height;
    Bmp[RIVER_10].targetRect.left = 10;
    Bmp[RIVER_10].targetRect.right = Bmp[RIVER_10].targetRect.left + Bmp[RIVER_10].Width;
    Bmp[RIVER_10].targetRect.top = 26;
    Bmp[RIVER_10].targetRect.bottom = Bmp[RIVER_10].targetRect.top + Bmp[RIVER_10].Height;

    Bmp[RIVER_END_1].Width = 42;
    Bmp[RIVER_END_1].Height = 22;
    Bmp[RIVER_END_1].sourceRect.left = 336;
    Bmp[RIVER_END_1].sourceRect.right = Bmp[RIVER_END_1].sourceRect.left + Bmp[RIVER_END_1].Width;
    Bmp[RIVER_END_1].sourceRect.top = 0;
    Bmp[RIVER_END_1].sourceRect.bottom = Bmp[RIVER_END_1].sourceRect.top + Bmp[RIVER_END_1].Height;
    Bmp[RIVER_END_1].targetRect.left = 2;
    Bmp[RIVER_END_1].targetRect.right = Bmp[RIVER_END_1].targetRect.left + Bmp[RIVER_END_1].Width;
    Bmp[RIVER_END_1].targetRect.top = 17;
    Bmp[RIVER_END_1].targetRect.bottom = Bmp[RIVER_END_1].targetRect.top + Bmp[RIVER_END_1].Height;

    Bmp[RIVER_END_2].Width = 40;
    Bmp[RIVER_END_2].Height = 22;
    Bmp[RIVER_END_2].sourceRect.left = 378;
    Bmp[RIVER_END_2].sourceRect.right = Bmp[RIVER_END_2].sourceRect.left + Bmp[RIVER_END_2].Width;
    Bmp[RIVER_END_2].sourceRect.top = 0;
    Bmp[RIVER_END_2].sourceRect.bottom = Bmp[RIVER_END_2].sourceRect.top + Bmp[RIVER_END_2].Height;
    Bmp[RIVER_END_2].targetRect.left = 10;
    Bmp[RIVER_END_2].targetRect.right = Bmp[RIVER_END_2].targetRect.left + Bmp[RIVER_END_2].Width;
    Bmp[RIVER_END_2].targetRect.top = 17;
    Bmp[RIVER_END_2].targetRect.bottom = Bmp[RIVER_END_2].targetRect.top + Bmp[RIVER_END_2].Height;

    Bmp[RIVER_END_3].Width = 40;
    Bmp[RIVER_END_3].Height = 22;
    Bmp[RIVER_END_3].sourceRect.left = 418;
    Bmp[RIVER_END_3].sourceRect.right = Bmp[RIVER_END_3].sourceRect.left + Bmp[RIVER_END_3].Width;
    Bmp[RIVER_END_3].sourceRect.top = 0;
    Bmp[RIVER_END_3].sourceRect.bottom = Bmp[RIVER_END_3].sourceRect.top + Bmp[RIVER_END_3].Height;
    Bmp[RIVER_END_3].targetRect.left = 10;
    Bmp[RIVER_END_3].targetRect.right = Bmp[RIVER_END_3].targetRect.left + Bmp[RIVER_END_3].Width;
    Bmp[RIVER_END_3].targetRect.top = 21;
    Bmp[RIVER_END_3].targetRect.bottom = Bmp[RIVER_END_3].targetRect.top + Bmp[RIVER_END_3].Height;

    Bmp[RIVER_END_4].Width = 42;
    Bmp[RIVER_END_4].Height = 22;
    Bmp[RIVER_END_4].sourceRect.left = 458;
    Bmp[RIVER_END_4].sourceRect.right = Bmp[RIVER_END_4].sourceRect.left + Bmp[RIVER_END_4].Width;
    Bmp[RIVER_END_4].sourceRect.top = 0;
    Bmp[RIVER_END_4].sourceRect.bottom = Bmp[RIVER_END_4].sourceRect.top + Bmp[RIVER_END_4].Height;
    Bmp[RIVER_END_4].targetRect.left = 2;
    Bmp[RIVER_END_4].targetRect.right = Bmp[RIVER_END_4].targetRect.left + Bmp[RIVER_END_4].Width;
    Bmp[RIVER_END_4].targetRect.top = 21;
    Bmp[RIVER_END_4].targetRect.bottom = Bmp[RIVER_END_4].targetRect.top + Bmp[RIVER_END_4].Height;

    Bmp[RIVER_START_1].Width = 30;
    Bmp[RIVER_START_1].Height = 19;
    Bmp[RIVER_START_1].sourceRect.left = 500;
    Bmp[RIVER_START_1].sourceRect.right = Bmp[RIVER_START_1].sourceRect.left + Bmp[RIVER_START_1].Width;
    Bmp[RIVER_START_1].sourceRect.top = 0;
    Bmp[RIVER_START_1].sourceRect.bottom = Bmp[RIVER_START_1].sourceRect.top + Bmp[RIVER_START_1].Height;
    Bmp[RIVER_START_1].targetRect.left = 16;
    Bmp[RIVER_START_1].targetRect.right = Bmp[RIVER_START_1].targetRect.left + Bmp[RIVER_START_1].Width;
    Bmp[RIVER_START_1].targetRect.top = 19;
    Bmp[RIVER_START_1].targetRect.bottom = Bmp[RIVER_START_1].targetRect.top + Bmp[RIVER_START_1].Height;

    Bmp[RIVER_START_2].Width = 26;
    Bmp[RIVER_START_2].Height = 19;
    Bmp[RIVER_START_2].sourceRect.left = 530;
    Bmp[RIVER_START_2].sourceRect.right = Bmp[RIVER_START_2].sourceRect.left + Bmp[RIVER_START_2].Width;
    Bmp[RIVER_START_2].sourceRect.top = 0;
    Bmp[RIVER_START_2].sourceRect.bottom = Bmp[RIVER_START_2].sourceRect.top + Bmp[RIVER_START_2].Height;
    Bmp[RIVER_START_2].targetRect.left = 9;
    Bmp[RIVER_START_2].targetRect.right = Bmp[RIVER_START_2].targetRect.left + Bmp[RIVER_START_2].Width;
    Bmp[RIVER_START_2].targetRect.top = 20;
    Bmp[RIVER_START_2].targetRect.bottom = Bmp[RIVER_START_2].targetRect.top + Bmp[RIVER_START_2].Height;

    Bmp[RIVER_START_3].Width = 25;
    Bmp[RIVER_START_3].Height = 16;
    Bmp[RIVER_START_3].sourceRect.left = 556;
    Bmp[RIVER_START_3].sourceRect.right = Bmp[RIVER_START_3].sourceRect.left + Bmp[RIVER_START_3].Width;
    Bmp[RIVER_START_3].sourceRect.top = 0;
    Bmp[RIVER_START_3].sourceRect.bottom = Bmp[RIVER_START_3].sourceRect.top + Bmp[RIVER_START_3].Height;
    Bmp[RIVER_START_3].targetRect.left = 19;
    Bmp[RIVER_START_3].targetRect.right = Bmp[RIVER_START_3].targetRect.left + Bmp[RIVER_START_3].Width;
    Bmp[RIVER_START_3].targetRect.top = 19;
    Bmp[RIVER_START_3].targetRect.bottom = Bmp[RIVER_START_3].targetRect.top + Bmp[RIVER_START_3].Height;

    Bmp[RIVER_START_4].Width = 25;
    Bmp[RIVER_START_4].Height = 15;
    Bmp[RIVER_START_4].sourceRect.left = 581;
    Bmp[RIVER_START_4].sourceRect.right = Bmp[RIVER_START_4].sourceRect.left + Bmp[RIVER_START_4].Width;
    Bmp[RIVER_START_4].sourceRect.top = 0;
    Bmp[RIVER_START_4].sourceRect.bottom = Bmp[RIVER_START_4].sourceRect.top + Bmp[RIVER_START_4].Height;
    Bmp[RIVER_START_4].targetRect.left = 8;
    Bmp[RIVER_START_4].targetRect.right = Bmp[RIVER_START_4].targetRect.left + Bmp[RIVER_START_4].Width;
    Bmp[RIVER_START_4].targetRect.top = 21;
    Bmp[RIVER_START_4].targetRect.bottom = Bmp[RIVER_START_4].targetRect.top + Bmp[RIVER_START_4].Height;

    Bmp[FLOODGATE_1].Width = 35;
    Bmp[FLOODGATE_1].Height = 22;
    Bmp[FLOODGATE_1].sourceRect.left = 164;
    Bmp[FLOODGATE_1].sourceRect.right = Bmp[FLOODGATE_1].sourceRect.left + Bmp[FLOODGATE_1].Width;
    Bmp[FLOODGATE_1].sourceRect.top = 126;
    Bmp[FLOODGATE_1].sourceRect.bottom = Bmp[FLOODGATE_1].sourceRect.top + Bmp[FLOODGATE_1].Height;
    Bmp[FLOODGATE_1].targetRect.left = 10;
    Bmp[FLOODGATE_1].targetRect.right = Bmp[FLOODGATE_1].targetRect.left + Bmp[FLOODGATE_1].Width;
    Bmp[FLOODGATE_1].targetRect.top = 17;
    Bmp[FLOODGATE_1].targetRect.bottom = Bmp[FLOODGATE_1].targetRect.top + Bmp[FLOODGATE_1].Height;

    Bmp[FLOODGATE_2].Width = 34;
    Bmp[FLOODGATE_2].Height = 23;
    Bmp[FLOODGATE_2].sourceRect.left = 199;
    Bmp[FLOODGATE_2].sourceRect.right = Bmp[FLOODGATE_2].sourceRect.left + Bmp[FLOODGATE_2].Width;
    Bmp[FLOODGATE_2].sourceRect.top = 126;
    Bmp[FLOODGATE_2].sourceRect.bottom = Bmp[FLOODGATE_2].sourceRect.top + Bmp[FLOODGATE_2].Height;
    Bmp[FLOODGATE_2].targetRect.left = 10;
    Bmp[FLOODGATE_2].targetRect.right = Bmp[FLOODGATE_2].targetRect.left + Bmp[FLOODGATE_2].Width;
    Bmp[FLOODGATE_2].targetRect.top = 16;
    Bmp[FLOODGATE_2].targetRect.bottom = Bmp[FLOODGATE_2].targetRect.top + Bmp[FLOODGATE_2].Height;

    Bmp[FLOODGATE_3].Width = 34;
    Bmp[FLOODGATE_3].Height = 22;
    Bmp[FLOODGATE_3].sourceRect.left = 233;
    Bmp[FLOODGATE_3].sourceRect.right = Bmp[FLOODGATE_3].sourceRect.left + Bmp[FLOODGATE_3].Width;
    Bmp[FLOODGATE_3].sourceRect.top = 126;
    Bmp[FLOODGATE_3].sourceRect.bottom = Bmp[FLOODGATE_3].sourceRect.top + Bmp[FLOODGATE_3].Height;
    Bmp[FLOODGATE_3].targetRect.left = 10;
    Bmp[FLOODGATE_3].targetRect.right = Bmp[FLOODGATE_3].targetRect.left + Bmp[FLOODGATE_3].Width;
    Bmp[FLOODGATE_3].targetRect.top = 17;
    Bmp[FLOODGATE_3].targetRect.bottom = Bmp[FLOODGATE_3].targetRect.top + Bmp[FLOODGATE_3].Height;

    Bmp[FLOODGATE_4].Width = 33;
    Bmp[FLOODGATE_4].Height = 23;
    Bmp[FLOODGATE_4].sourceRect.left = 268;
    Bmp[FLOODGATE_4].sourceRect.right = Bmp[FLOODGATE_4].sourceRect.left + Bmp[FLOODGATE_4].Width;
    Bmp[FLOODGATE_4].sourceRect.top = 105;
    Bmp[FLOODGATE_4].sourceRect.bottom = Bmp[FLOODGATE_4].sourceRect.top + Bmp[FLOODGATE_4].Height;
    Bmp[FLOODGATE_4].targetRect.left = 11;
    Bmp[FLOODGATE_4].targetRect.right = Bmp[FLOODGATE_4].targetRect.left + Bmp[FLOODGATE_4].Width;
    Bmp[FLOODGATE_4].targetRect.top = 16;
    Bmp[FLOODGATE_4].targetRect.bottom = Bmp[FLOODGATE_4].targetRect.top + Bmp[FLOODGATE_4].Height;

    Bmp[FLOODGATE_5].Width = 34;
    Bmp[FLOODGATE_5].Height = 17;
    Bmp[FLOODGATE_5].sourceRect.left = 302;
    Bmp[FLOODGATE_5].sourceRect.right = Bmp[FLOODGATE_5].sourceRect.left + Bmp[FLOODGATE_5].Width;
    Bmp[FLOODGATE_5].sourceRect.top = 91;
    Bmp[FLOODGATE_5].sourceRect.bottom = Bmp[FLOODGATE_5].sourceRect.top + Bmp[FLOODGATE_5].Height;
    Bmp[FLOODGATE_5].targetRect.left = 10;
    Bmp[FLOODGATE_5].targetRect.right = Bmp[FLOODGATE_5].targetRect.left + Bmp[FLOODGATE_5].Width;
    Bmp[FLOODGATE_5].targetRect.top = 20;
    Bmp[FLOODGATE_5].targetRect.bottom = Bmp[FLOODGATE_5].targetRect.top + Bmp[FLOODGATE_5].Height;

    Bmp[FLOODGATE_6].Width = 35;
    Bmp[FLOODGATE_6].Height = 23;
    Bmp[FLOODGATE_6].sourceRect.left = 336;
    Bmp[FLOODGATE_6].sourceRect.right = Bmp[FLOODGATE_6].sourceRect.left + Bmp[FLOODGATE_6].Width;
    Bmp[FLOODGATE_6].sourceRect.top = 154;
    Bmp[FLOODGATE_6].sourceRect.bottom = Bmp[FLOODGATE_6].sourceRect.top + Bmp[FLOODGATE_6].Height;
    Bmp[FLOODGATE_6].targetRect.left = 10;
    Bmp[FLOODGATE_6].targetRect.right = Bmp[FLOODGATE_6].targetRect.left + Bmp[FLOODGATE_6].Width;
    Bmp[FLOODGATE_6].targetRect.top = 16;
    Bmp[FLOODGATE_6].targetRect.bottom = Bmp[FLOODGATE_6].targetRect.top + Bmp[FLOODGATE_6].Height;

    // Bauwerke
    for (i = FIELD; i <= BONFIRE; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].Speed = 0;
        Bmp[i].AnimationPhase = 0;
    }

    Bmp[FIELD].AnimationPhaseCount = 3;
    Bmp[FIELD].Surface = lpDDSBau;
    Bmp[FIELD].Width = 42;
    Bmp[FIELD].Height = 27;
    Bmp[FIELD].sourceRect.left = 0;
    Bmp[FIELD].sourceRect.right = 0 + Bmp[FIELD].Width;
    Bmp[FIELD].sourceRect.top = 0;
    Bmp[FIELD].sourceRect.bottom = 0 + Bmp[FIELD].Height;
    Bmp[FIELD].targetRect.left = 5;
    Bmp[FIELD].targetRect.right = Bmp[FIELD].targetRect.left + Bmp[FIELD].Width;
    Bmp[FIELD].targetRect.top = 15;
    Bmp[FIELD].targetRect.bottom = Bmp[FIELD].targetRect.top + Bmp[FIELD].Height;
    Bmp[FIELD].RequiredActionCases = 20;

    Bmp[TENT].AnimationPhaseCount = 1;
    Bmp[TENT].Surface = lpDDSBau;
    Bmp[TENT].Width = 23;
    Bmp[TENT].Height = 20;
    Bmp[TENT].sourceRect.left = 42;
    Bmp[TENT].sourceRect.right = 42 + Bmp[TENT].Width;
    Bmp[TENT].sourceRect.top = 0;
    Bmp[TENT].sourceRect.bottom = 0 + Bmp[TENT].Height;
    Bmp[TENT].targetRect.left = 14;
    Bmp[TENT].targetRect.right = Bmp[TENT].targetRect.left + Bmp[TENT].Width;
    Bmp[TENT].targetRect.top = 9;
    Bmp[TENT].targetRect.bottom = Bmp[TENT].targetRect.top + Bmp[TENT].Height;
    Bmp[TENT].RequiredRawMaterials[RAW_TREE_BRANCH] = 5;
    Bmp[TENT].RequiredRawMaterials[RAW_LEAF] = 5;
    Bmp[TENT].RequiredActionCases = 16;

    Bmp[BOAT].AnimationPhaseCount = 2;
    Bmp[BOAT].Surface = lpDDSBau;
    Bmp[BOAT].Width = 26;
    Bmp[BOAT].Height = 18;
    Bmp[BOAT].sourceRect.left = 65;
    Bmp[BOAT].sourceRect.right = 65 + Bmp[BOAT].Width;
    Bmp[BOAT].sourceRect.top = 0;
    Bmp[BOAT].sourceRect.bottom = 0 + Bmp[BOAT].Height;
    Bmp[BOAT].targetRect.left = 14;
    Bmp[BOAT].targetRect.right = Bmp[BOAT].targetRect.left + Bmp[BOAT].Width;
    Bmp[BOAT].targetRect.top = 20;
    Bmp[BOAT].targetRect.bottom = Bmp[BOAT].targetRect.top + Bmp[BOAT].Height;
    Bmp[BOAT].RequiredRawMaterials[RAW_TREE_BRANCH] = 2;
    Bmp[BOAT].RequiredRawMaterials[RAW_TREE_TRUNK] = 1;
    Bmp[BOAT].RequiredActionCases = 16;

    Bmp[PIPE].AnimationPhaseCount = 2;
    Bmp[PIPE].Surface = lpDDSBau;
    Bmp[PIPE].Width = 34;
    Bmp[PIPE].Height = 21;
    Bmp[PIPE].sourceRect.left = 91;
    Bmp[PIPE].sourceRect.right = 91 + Bmp[PIPE].Width;
    Bmp[PIPE].sourceRect.top = 0;
    Bmp[PIPE].sourceRect.bottom = 0 + Bmp[PIPE].Height;
    Bmp[PIPE].targetRect.left = 11;
    Bmp[PIPE].targetRect.right = Bmp[PIPE].targetRect.left + Bmp[PIPE].Width;
    Bmp[PIPE].targetRect.top = 16;
    Bmp[PIPE].targetRect.bottom = Bmp[PIPE].targetRect.top + Bmp[PIPE].Height;
    Bmp[PIPE].RequiredRawMaterials[RAW_TREE_TRUNK] = 1;
    Bmp[PIPE].RequiredActionCases = 18;

    Bmp[SOS].AnimationPhaseCount = 1;
    Bmp[SOS].Surface = lpDDSBau;
    Bmp[SOS].Width = 36;
    Bmp[SOS].Height = 19;
    Bmp[SOS].sourceRect.left = 125;
    Bmp[SOS].sourceRect.right = 125 + Bmp[SOS].Width;
    Bmp[SOS].sourceRect.top = 0;
    Bmp[SOS].sourceRect.bottom = 0 + Bmp[SOS].Height;
    Bmp[SOS].targetRect.left = 11;
    Bmp[SOS].targetRect.right = Bmp[SOS].targetRect.left + Bmp[SOS].Width;
    Bmp[SOS].targetRect.top = 20;
    Bmp[SOS].targetRect.bottom = Bmp[SOS].targetRect.top + Bmp[SOS].Height;
    Bmp[SOS].RequiredRawMaterials[RAW_STONE] = 10;
    Bmp[SOS].RequiredActionCases = 20;

    Bmp[HOUSE_1].AnimationPhaseCount = 1;
    Bmp[HOUSE_1].Surface = lpDDSBau;
    Bmp[HOUSE_1].Width = 26;
    Bmp[HOUSE_1].Height = 41;
    Bmp[HOUSE_1].sourceRect.left = 161;
    Bmp[HOUSE_1].sourceRect.right = 161 + Bmp[HOUSE_1].Width;
    Bmp[HOUSE_1].sourceRect.top = 0;
    Bmp[HOUSE_1].sourceRect.bottom = 0 + Bmp[HOUSE_1].Height;
    Bmp[HOUSE_1].targetRect.left = 0;
    Bmp[HOUSE_1].targetRect.right = Bmp[HOUSE_1].targetRect.left + Bmp[HOUSE_1].Width;
    Bmp[HOUSE_1].targetRect.top = 0;
    Bmp[HOUSE_1].targetRect.bottom = Bmp[HOUSE_1].targetRect.top + Bmp[HOUSE_1].Height;
    Bmp[HOUSE_1].RequiredRawMaterials[RAW_TREE_BRANCH] = 4;
    Bmp[HOUSE_1].RequiredActionCases = 19;
    Bmp[HOUSE_1].Sound = Sound::FOREST;

    Bmp[HOUSE_2].AnimationPhaseCount = 1;
    Bmp[HOUSE_2].Surface = lpDDSBau;
    Bmp[HOUSE_2].Width = 34;
    Bmp[HOUSE_2].Height = 41;
    Bmp[HOUSE_2].sourceRect.left = 187;
    Bmp[HOUSE_2].sourceRect.right = 187 + Bmp[HOUSE_2].Width;
    Bmp[HOUSE_2].sourceRect.top = 0;
    Bmp[HOUSE_2].sourceRect.bottom = 0 + Bmp[HOUSE_2].Height;
    Bmp[HOUSE_2].targetRect.left = 0;
    Bmp[HOUSE_2].targetRect.right = Bmp[HOUSE_2].targetRect.left + Bmp[HOUSE_2].Width;
    Bmp[HOUSE_2].targetRect.top = 0;
    Bmp[HOUSE_2].targetRect.bottom = Bmp[HOUSE_2].targetRect.top + Bmp[HOUSE_2].Height;
    Bmp[HOUSE_2].RequiredRawMaterials[RAW_TREE_BRANCH] = 3;
    Bmp[HOUSE_2].RequiredRawMaterials[RAW_TREE_TRUNK] = 3;
    Bmp[HOUSE_2].RequiredActionCases = 21;
    Bmp[HOUSE_2].Sound = Sound::FOREST;

    Bmp[HOUSE_3].AnimationPhaseCount = 1;
    Bmp[HOUSE_3].Surface = lpDDSBau;
    Bmp[HOUSE_3].Width = 34;
    Bmp[HOUSE_3].Height = 41;
    Bmp[HOUSE_3].sourceRect.left = 221;
    Bmp[HOUSE_3].sourceRect.right = 221 + Bmp[HOUSE_3].Width;
    Bmp[HOUSE_3].sourceRect.top = 0;
    Bmp[HOUSE_3].sourceRect.bottom = 0 + Bmp[HOUSE_3].Height;
    Bmp[HOUSE_3].targetRect.left = 0;
    Bmp[HOUSE_3].targetRect.right = Bmp[HOUSE_3].targetRect.left + Bmp[HOUSE_3].Width;
    Bmp[HOUSE_3].targetRect.top = 0;
    Bmp[HOUSE_3].targetRect.bottom = Bmp[HOUSE_3].targetRect.top + Bmp[HOUSE_3].Height;
    Bmp[HOUSE_3].RequiredRawMaterials[RAW_TREE_BRANCH] = 4;
    Bmp[HOUSE_3].RequiredRawMaterials[RAW_TREE_TRUNK] = 4;
    Bmp[HOUSE_3].RequiredRawMaterials[RAW_LEAF] = 5;
    Bmp[HOUSE_3].RequiredActionCases = 21;
    Bmp[HOUSE_3].Sound = Sound::FOREST;

    Bmp[BONFIRE].AnimationPhaseCount = 1;
    Bmp[BONFIRE].Surface = lpDDSBau;
    Bmp[BONFIRE].Width = 21;
    Bmp[BONFIRE].Height = 19;
    Bmp[BONFIRE].sourceRect.left = 255;
    Bmp[BONFIRE].sourceRect.right = 255 + Bmp[BONFIRE].Width;
    Bmp[BONFIRE].sourceRect.top = 0;
    Bmp[BONFIRE].sourceRect.bottom = 0 + Bmp[BONFIRE].Height;
    Bmp[BONFIRE].targetRect.left = 15;
    Bmp[BONFIRE].targetRect.right = Bmp[BONFIRE].targetRect.left + Bmp[BONFIRE].Width;
    Bmp[BONFIRE].targetRect.top = 10;
    Bmp[BONFIRE].targetRect.bottom = Bmp[BONFIRE].targetRect.top + Bmp[BONFIRE].Height;
    Bmp[BONFIRE].RequiredRawMaterials[RAW_TREE_BRANCH] = 5;
    Bmp[BONFIRE].RequiredRawMaterials[RAW_TREE_TRUNK] = 4;
    Bmp[BONFIRE].RequiredActionCases = 9;

    // Allgemein Bäume
    for (i = TREE_1; i <= TREE_4; i++) {
        Bmp[i].IsAnimationRunning = true;
        Bmp[i].AnimationPhaseCount = 4;
        Bmp[i].Speed = 4;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSBaum;
        Bmp[i].Sound = Sound::FOREST;
    }

    // Baum1
    Bmp[TREE_1].sourceRect.left = 0;
    Bmp[TREE_1].sourceRect.top = 0;
    Bmp[TREE_1].sourceRect.right = 21;
    Bmp[TREE_1].sourceRect.bottom = 26;
    Bmp[TREE_1].Width = static_cast<short>(Bmp[TREE_1].sourceRect.right - Bmp[TREE_1].sourceRect.left);
    Bmp[TREE_1].Height = static_cast<short>(Bmp[TREE_1].sourceRect.bottom - Bmp[TREE_1].sourceRect.top);

    // Baum2
    Bmp[TREE_2].sourceRect.left = 21;
    Bmp[TREE_2].sourceRect.top = 0;
    Bmp[TREE_2].sourceRect.right = 42;
    Bmp[TREE_2].sourceRect.bottom = 26;
    Bmp[TREE_2].Width = static_cast<short>(Bmp[TREE_2].sourceRect.right - Bmp[TREE_2].sourceRect.left);
    Bmp[TREE_2].Height = static_cast<short>(Bmp[TREE_2].sourceRect.bottom - Bmp[TREE_2].sourceRect.top);

    // Baum3
    Bmp[TREE_3].sourceRect.left = 42;
    Bmp[TREE_3].sourceRect.top = 0;
    Bmp[TREE_3].sourceRect.right = 64;
    Bmp[TREE_3].sourceRect.bottom = 27;
    Bmp[TREE_3].Width = static_cast<short>(Bmp[TREE_3].sourceRect.right - Bmp[TREE_3].sourceRect.left);
    Bmp[TREE_3].Height = static_cast<short>(Bmp[TREE_3].sourceRect.bottom - Bmp[TREE_3].sourceRect.top);

    // Baum4
    Bmp[TREE_4].sourceRect.left = 64;
    Bmp[TREE_4].sourceRect.top = 0;
    Bmp[TREE_4].sourceRect.right = 81;
    Bmp[TREE_4].sourceRect.bottom = 16;
    Bmp[TREE_4].Width = static_cast<short>(Bmp[TREE_4].sourceRect.right - Bmp[TREE_4].sourceRect.left);
    Bmp[TREE_4].Height = static_cast<short>(Bmp[TREE_4].sourceRect.bottom - Bmp[TREE_4].sourceRect.top);

    // Baumgroß
    Bmp[TREE_BIG].sourceRect.left = 238;
    Bmp[TREE_BIG].sourceRect.top = 0;
    Bmp[TREE_BIG].sourceRect.right = 238 + 26;
    Bmp[TREE_BIG].sourceRect.bottom = 41;
    Bmp[TREE_BIG].Width = static_cast<short>(Bmp[TREE_BIG].sourceRect.right - Bmp[TREE_BIG].sourceRect.left);
    Bmp[TREE_BIG].Height = static_cast<short>(Bmp[TREE_BIG].sourceRect.bottom - Bmp[TREE_BIG].sourceRect.top);
    Bmp[TREE_BIG].IsAnimationRunning = false;
    Bmp[TREE_BIG].AnimationPhaseCount = 1;
    Bmp[TREE_BIG].Speed = 0;
    Bmp[TREE_BIG].AnimationPhase = 0;
    Bmp[TREE_BIG].Surface = lpDDSBaum;
    Bmp[TREE_BIG].Sound = Sound::FOREST;

    // Feuer
    Bmp[FIRE].AnimationPhaseCount = 3;
    Bmp[FIRE].Surface = lpDDSBau;
    Bmp[FIRE].Width = 21;
    Bmp[FIRE].Height = 28;
    Bmp[FIRE].sourceRect.left = 276;
    Bmp[FIRE].sourceRect.right = 276 + Bmp[FIRE].Width;
    Bmp[FIRE].sourceRect.top = 0;
    Bmp[FIRE].sourceRect.bottom = 0 + Bmp[FIRE].Height;
    Bmp[FIRE].targetRect.left = 15;
    Bmp[FIRE].targetRect.right = Bmp[FIRE].targetRect.left + Bmp[FIRE].Width;
    Bmp[FIRE].targetRect.top = 1;
    Bmp[FIRE].targetRect.bottom = Bmp[FIRE].targetRect.top + Bmp[FIRE].Height;
    Bmp[FIRE].IsAnimationRunning = true;
    Bmp[FIRE].Speed = 6;
    Bmp[FIRE].AnimationPhase = 0;
    Bmp[FIRE].Sound = Sound::FIRE;

    // WRACK
    Bmp[WRECK_1].AnimationPhaseCount = 3;
    Bmp[WRECK_1].Surface = lpDDSBau;
    Bmp[WRECK_1].Width = 24;
    Bmp[WRECK_1].Height = 18;
    Bmp[WRECK_1].sourceRect.left = 391;
    Bmp[WRECK_1].sourceRect.right = 391 + Bmp[WRECK_1].Width;
    Bmp[WRECK_1].sourceRect.top = 0;
    Bmp[WRECK_1].sourceRect.bottom = 0 + Bmp[WRECK_1].Height;
    Bmp[WRECK_1].targetRect.left = 15;
    Bmp[WRECK_1].targetRect.right = Bmp[WRECK_1].targetRect.left + Bmp[WRECK_1].Width;
    Bmp[WRECK_1].targetRect.top = 20;
    Bmp[WRECK_1].targetRect.bottom = Bmp[WRECK_1].targetRect.top + Bmp[WRECK_1].Height;
    Bmp[WRECK_1].IsAnimationRunning = true;
    Bmp[WRECK_1].Speed = 5;
    Bmp[WRECK_1].AnimationPhase = 0;

    // WRACK2
    Bmp[WRECK_2].AnimationPhaseCount = 3;
    Bmp[WRECK_2].Surface = lpDDSBau;
    Bmp[WRECK_2].Width = 27;
    Bmp[WRECK_2].Height = 26;
    Bmp[WRECK_2].sourceRect.left = 415;
    Bmp[WRECK_2].sourceRect.right = 415 + Bmp[WRECK_2].Width;
    Bmp[WRECK_2].sourceRect.top = 0;
    Bmp[WRECK_2].sourceRect.bottom = 0 + Bmp[WRECK_2].Height;
    Bmp[WRECK_2].targetRect.left = 15;
    Bmp[WRECK_2].targetRect.right = Bmp[WRECK_2].targetRect.left + Bmp[WRECK_2].Width;
    Bmp[WRECK_2].targetRect.top = 16;
    Bmp[WRECK_2].targetRect.bottom = Bmp[WRECK_2].targetRect.top + Bmp[WRECK_2].Height;
    Bmp[WRECK_2].IsAnimationRunning = true;
    Bmp[WRECK_2].Speed = 5;
    Bmp[WRECK_2].AnimationPhase = 0;

    // Früchte
    // Busch
    Bmp[BUSH].sourceRect.left = 81;
    Bmp[BUSH].sourceRect.top = 0;
    Bmp[BUSH].sourceRect.right = 81 + 20;
    Bmp[BUSH].sourceRect.bottom = 13;
    Bmp[BUSH].Width = static_cast<short>(Bmp[BUSH].sourceRect.right - Bmp[BUSH].sourceRect.left);
    Bmp[BUSH].Height = static_cast<short>(Bmp[BUSH].sourceRect.bottom - Bmp[BUSH].sourceRect.top);
    Bmp[BUSH].IsAnimationRunning = false;
    Bmp[BUSH].AnimationPhaseCount = 3;
    Bmp[BUSH].Speed = 0;
    Bmp[BUSH].AnimationPhase = 0;
    Bmp[BUSH].Surface = lpDDSBaum;

    // Buttons

    // StandardBmponsinitialisierung
    for (i = BUTTON_GRID; i <= BUTTON_DESTROY; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].Surface = lpDDSButtons;
        Bmp[i].AnimationPhaseCount = 1;
        Bmp[i].AnimationPhase = 0;
    }

    // ButtGitter
    Bmp[BUTTON_GRID].sourceRect.left = 0;
    Bmp[BUTTON_GRID].sourceRect.top = 0;
    Bmp[BUTTON_GRID].sourceRect.right = 28;
    Bmp[BUTTON_GRID].sourceRect.bottom = 28;
    Bmp[BUTTON_GRID].targetRect.left = rcPanel.left + 173;
    Bmp[BUTTON_GRID].targetRect.top = rcPanel.top + 26;
    Bmp[BUTTON_GRID].targetRect.right = Bmp[BUTTON_GRID].targetRect.left + 28;
    Bmp[BUTTON_GRID].targetRect.bottom = Bmp[BUTTON_GRID].targetRect.top + 28;
    Bmp[BUTTON_GRID].Width = static_cast<short>(Bmp[BUTTON_GRID].sourceRect.right - Bmp[BUTTON_GRID].sourceRect.left);
    Bmp[BUTTON_GRID].Height = static_cast<short>(Bmp[BUTTON_GRID].sourceRect.bottom - Bmp[BUTTON_GRID].sourceRect.top);
    Bmp[BUTTON_GRID].AnimationPhaseCount = 2;

    // BUTTANIMATION
    Bmp[BUTTON_ANIMATION].sourceRect.left = 0;
    Bmp[BUTTON_ANIMATION].sourceRect.top = 56;
    Bmp[BUTTON_ANIMATION].sourceRect.right = 28;
    Bmp[BUTTON_ANIMATION].sourceRect.bottom = 56 + 28;
    Bmp[BUTTON_ANIMATION].targetRect.left = rcPanel.left + 173;
    Bmp[BUTTON_ANIMATION].targetRect.top = rcPanel.top + 60;
    Bmp[BUTTON_ANIMATION].targetRect.right = Bmp[BUTTON_ANIMATION].targetRect.left + 28;
    Bmp[BUTTON_ANIMATION].targetRect.bottom = Bmp[BUTTON_ANIMATION].targetRect.top + 28;
    Bmp[BUTTON_ANIMATION].Width = static_cast<short>(Bmp[BUTTON_ANIMATION].sourceRect.right - Bmp[BUTTON_ANIMATION].sourceRect.left);
    Bmp[BUTTON_ANIMATION].Height = static_cast<short>(Bmp[BUTTON_ANIMATION].sourceRect.bottom - Bmp[BUTTON_ANIMATION].sourceRect.top);
    Bmp[BUTTON_ANIMATION].AnimationPhaseCount = 2;

    // BUTTBEENDEN
    Bmp[BUTTON_END].sourceRect.left = 0;
    Bmp[BUTTON_END].sourceRect.top = 112;
    Bmp[BUTTON_END].sourceRect.right = 20;
    Bmp[BUTTON_END].sourceRect.bottom = 112 + 20;
    Bmp[BUTTON_END].targetRect.left = rcPanel.left + 60;
    Bmp[BUTTON_END].targetRect.top = rcPanel.top + 2;
    Bmp[BUTTON_END].targetRect.right = Bmp[BUTTON_END].targetRect.left + 20;
    Bmp[BUTTON_END].targetRect.bottom = Bmp[BUTTON_END].targetRect.top + 20;
    Bmp[BUTTON_END].Width = static_cast<short>(Bmp[BUTTON_END].sourceRect.right - Bmp[BUTTON_END].sourceRect.left);
    Bmp[BUTTON_END].Height = static_cast<short>(Bmp[BUTTON_END].sourceRect.bottom - Bmp[BUTTON_END].sourceRect.top);
    Bmp[BUTTON_END].AnimationPhaseCount = 4;
    Bmp[BUTTON_END].Speed = 4;

    // BUTTNEU
    Bmp[BUTTON_NEW].sourceRect.left = 0;
    Bmp[BUTTON_NEW].sourceRect.top = 192;
    Bmp[BUTTON_NEW].sourceRect.right = 20;
    Bmp[BUTTON_NEW].sourceRect.bottom = 192 + 20;
    Bmp[BUTTON_NEW].targetRect.left = rcPanel.left + 100;
    Bmp[BUTTON_NEW].targetRect.top = rcPanel.top + 2;
    Bmp[BUTTON_NEW].targetRect.right = Bmp[BUTTON_NEW].targetRect.left + 20;
    Bmp[BUTTON_NEW].targetRect.bottom = Bmp[BUTTON_NEW].targetRect.top + 20;
    Bmp[BUTTON_NEW].Width = static_cast<short>(Bmp[BUTTON_NEW].sourceRect.right - Bmp[BUTTON_NEW].sourceRect.left);
    Bmp[BUTTON_NEW].Height = static_cast<short>(Bmp[BUTTON_NEW].sourceRect.bottom - Bmp[BUTTON_NEW].sourceRect.top);
    Bmp[BUTTON_NEW].AnimationPhaseCount = 2;
    Bmp[BUTTON_NEW].Speed = 3;


    // BUTTTAGNEU
    Bmp[BUTTON_NEW_DAY].sourceRect.left = 0;
    Bmp[BUTTON_NEW_DAY].sourceRect.top = 232;
    Bmp[BUTTON_NEW_DAY].sourceRect.right = 20;
    Bmp[BUTTON_NEW_DAY].sourceRect.bottom = 232 + 20;
    Bmp[BUTTON_NEW_DAY].targetRect.left = rcPanel.left + 140;
    Bmp[BUTTON_NEW_DAY].targetRect.top = rcPanel.top + 2;
    Bmp[BUTTON_NEW_DAY].targetRect.right = Bmp[BUTTON_NEW_DAY].targetRect.left + 20;
    Bmp[BUTTON_NEW_DAY].targetRect.bottom = Bmp[BUTTON_NEW_DAY].targetRect.top + 20;
    Bmp[BUTTON_NEW_DAY].Width = static_cast<short>(Bmp[BUTTON_NEW_DAY].sourceRect.right - Bmp[BUTTON_NEW_DAY].sourceRect.left);
    Bmp[BUTTON_NEW_DAY].Height = static_cast<short>(Bmp[BUTTON_NEW_DAY].sourceRect.bottom - Bmp[BUTTON_NEW_DAY].sourceRect.top);
    Bmp[BUTTON_NEW_DAY].AnimationPhaseCount = 2;
    Bmp[BUTTON_NEW_DAY].Speed = 2;

    // BUTTSOUND
    Bmp[BUTTON_SOUND].sourceRect.left = 0;
    Bmp[BUTTON_SOUND].sourceRect.top = 272;
    Bmp[BUTTON_SOUND].sourceRect.right = 28;
    Bmp[BUTTON_SOUND].sourceRect.bottom = 272 + 28;
    Bmp[BUTTON_SOUND].targetRect.left = rcPanel.left + 173;
    Bmp[BUTTON_SOUND].targetRect.top = rcPanel.top + 94;
    Bmp[BUTTON_SOUND].targetRect.right = Bmp[BUTTON_SOUND].targetRect.left + 28;
    Bmp[BUTTON_SOUND].targetRect.bottom = Bmp[BUTTON_SOUND].targetRect.top + 28;
    Bmp[BUTTON_SOUND].Width = static_cast<short>(Bmp[BUTTON_SOUND].sourceRect.right - Bmp[BUTTON_SOUND].sourceRect.left);
    Bmp[BUTTON_SOUND].Height = static_cast<short>(Bmp[BUTTON_SOUND].sourceRect.bottom - Bmp[BUTTON_SOUND].sourceRect.top);
    Bmp[BUTTON_SOUND].AnimationPhaseCount = 2;

    // ButtAktion
    Bmp[BUTTON_ACTION].sourceRect.left = 28;
    Bmp[BUTTON_ACTION].sourceRect.top = 0;
    Bmp[BUTTON_ACTION].sourceRect.right = 28 + 35;
    Bmp[BUTTON_ACTION].sourceRect.bottom = 35;
    Bmp[BUTTON_ACTION].targetRect.left = rcPanel.left + 29;
    Bmp[BUTTON_ACTION].targetRect.top = rcPanel.top + 191;
    Bmp[BUTTON_ACTION].targetRect.right = Bmp[BUTTON_ACTION].targetRect.left + 35;
    Bmp[BUTTON_ACTION].targetRect.bottom = Bmp[BUTTON_ACTION].targetRect.top + 35;
    Bmp[BUTTON_ACTION].Width = static_cast<short>(Bmp[BUTTON_ACTION].sourceRect.right - Bmp[BUTTON_ACTION].sourceRect.left);
    Bmp[BUTTON_ACTION].Height = static_cast<short>(Bmp[BUTTON_ACTION].sourceRect.bottom - Bmp[BUTTON_ACTION].sourceRect.top);
    Bmp[BUTTON_ACTION].AnimationPhaseCount = 3;
    Bmp[BUTTON_ACTION].Speed = 6;

    // BUTTBAUEN
    Bmp[BUTTON_CONSTRUCT].sourceRect.left = 203;
    Bmp[BUTTON_CONSTRUCT].sourceRect.top = 0;
    Bmp[BUTTON_CONSTRUCT].sourceRect.right = 203 + 35;
    Bmp[BUTTON_CONSTRUCT].sourceRect.bottom = 35;
    Bmp[BUTTON_CONSTRUCT].targetRect.left = rcPanel.left + 70;
    Bmp[BUTTON_CONSTRUCT].targetRect.top = rcPanel.top + 191;
    Bmp[BUTTON_CONSTRUCT].targetRect.right = Bmp[BUTTON_CONSTRUCT].targetRect.left + 35;
    Bmp[BUTTON_CONSTRUCT].targetRect.bottom = Bmp[BUTTON_CONSTRUCT].targetRect.top + 35;
    Bmp[BUTTON_CONSTRUCT].Width = static_cast<short>(Bmp[BUTTON_CONSTRUCT].sourceRect.right - Bmp[BUTTON_CONSTRUCT].sourceRect.left);
    Bmp[BUTTON_CONSTRUCT].Height = static_cast<short>(Bmp[BUTTON_CONSTRUCT].sourceRect.bottom - Bmp[BUTTON_CONSTRUCT].sourceRect.top);
    Bmp[BUTTON_CONSTRUCT].AnimationPhaseCount = 4;
    Bmp[BUTTON_CONSTRUCT].Speed = 5;


    // BUTTINVENTAR
    Bmp[BUTTON_INVENTORY].sourceRect.left = 168;
    Bmp[BUTTON_INVENTORY].sourceRect.top = 0;
    Bmp[BUTTON_INVENTORY].sourceRect.right = 168 + 35;
    Bmp[BUTTON_INVENTORY].sourceRect.bottom = 35;
    Bmp[BUTTON_INVENTORY].targetRect.left = rcPanel.left + 152;
    Bmp[BUTTON_INVENTORY].targetRect.top = rcPanel.top + 191;
    Bmp[BUTTON_INVENTORY].targetRect.right = Bmp[BUTTON_INVENTORY].targetRect.left + 35;
    Bmp[BUTTON_INVENTORY].targetRect.bottom = Bmp[BUTTON_INVENTORY].targetRect.top + 35;
    Bmp[BUTTON_INVENTORY].Width = static_cast<short>(Bmp[BUTTON_INVENTORY].sourceRect.right - Bmp[BUTTON_INVENTORY].sourceRect.left);
    Bmp[BUTTON_INVENTORY].Height = static_cast<short>(Bmp[BUTTON_INVENTORY].sourceRect.bottom - Bmp[BUTTON_INVENTORY].sourceRect.top);
    Bmp[BUTTON_INVENTORY].AnimationPhaseCount = 4;
    Bmp[BUTTON_INVENTORY].Speed = 4;

    // BUTTWEITER
    Bmp[BUTTON_CONTINUE].sourceRect.left = 343;
    Bmp[BUTTON_CONTINUE].sourceRect.top = 0;
    Bmp[BUTTON_CONTINUE].sourceRect.right = 343 + 35;
    Bmp[BUTTON_CONTINUE].sourceRect.bottom = 35;
    Bmp[BUTTON_CONTINUE].targetRect.left = rcPanel.left + 111;
    Bmp[BUTTON_CONTINUE].targetRect.top = rcPanel.top + 191;
    Bmp[BUTTON_CONTINUE].targetRect.right = Bmp[BUTTON_CONTINUE].targetRect.left + 35;
    Bmp[BUTTON_CONTINUE].targetRect.bottom = Bmp[BUTTON_CONTINUE].targetRect.top + 35;
    Bmp[BUTTON_CONTINUE].Width = static_cast<short>(Bmp[BUTTON_CONTINUE].sourceRect.right - Bmp[BUTTON_CONTINUE].sourceRect.left);
    Bmp[BUTTON_CONTINUE].Height = static_cast<short>(Bmp[BUTTON_CONTINUE].sourceRect.bottom - Bmp[BUTTON_CONTINUE].sourceRect.top);
    Bmp[BUTTON_CONTINUE].AnimationPhaseCount = 4;
    Bmp[BUTTON_CONTINUE].Speed = 4;
    Bmp[BUTTON_CONTINUE].AnimationPhase = -1;

    // BUTTSTOP
    Bmp[BUTTON_STOP].sourceRect.left = 378;
    Bmp[BUTTON_STOP].sourceRect.top = 0;
    Bmp[BUTTON_STOP].sourceRect.right = 378 + 35;
    Bmp[BUTTON_STOP].sourceRect.bottom = 35;
    Bmp[BUTTON_STOP].targetRect.left = rcPanel.left + 111;
    Bmp[BUTTON_STOP].targetRect.top = rcPanel.top + 191;
    Bmp[BUTTON_STOP].targetRect.right = Bmp[BUTTON_STOP].targetRect.left + 35;
    Bmp[BUTTON_STOP].targetRect.bottom = Bmp[BUTTON_STOP].targetRect.top + 35;
    Bmp[BUTTON_STOP].Width = static_cast<short>(Bmp[BUTTON_STOP].sourceRect.right - Bmp[BUTTON_STOP].sourceRect.left);
    Bmp[BUTTON_STOP].Height = static_cast<short>(Bmp[BUTTON_STOP].sourceRect.bottom - Bmp[BUTTON_STOP].sourceRect.top);
    Bmp[BUTTON_STOP].AnimationPhaseCount = 4;
    Bmp[BUTTON_STOP].Speed = 4;
    Bmp[BUTTON_STOP].AnimationPhase = -1;

    // BUTTABLEGEN
    Bmp[BUTTON_LAY_DOWN].sourceRect.left = 483;
    Bmp[BUTTON_LAY_DOWN].sourceRect.top = 0;
    Bmp[BUTTON_LAY_DOWN].sourceRect.right = 483 + 35;
    Bmp[BUTTON_LAY_DOWN].sourceRect.bottom = 35;
    Bmp[BUTTON_LAY_DOWN].targetRect.left = rcPanel.left + 111;
    Bmp[BUTTON_LAY_DOWN].targetRect.top = rcPanel.top + 191;
    Bmp[BUTTON_LAY_DOWN].targetRect.right = Bmp[BUTTON_LAY_DOWN].targetRect.left + 35;
    Bmp[BUTTON_LAY_DOWN].targetRect.bottom = Bmp[BUTTON_LAY_DOWN].targetRect.top + 35;
    Bmp[BUTTON_LAY_DOWN].Width = static_cast<short>(Bmp[BUTTON_LAY_DOWN].sourceRect.right - Bmp[BUTTON_LAY_DOWN].sourceRect.left);
    Bmp[BUTTON_LAY_DOWN].Height = static_cast<short>(Bmp[BUTTON_LAY_DOWN].sourceRect.bottom - Bmp[BUTTON_LAY_DOWN].sourceRect.top);
    Bmp[BUTTON_LAY_DOWN].AnimationPhaseCount = 4;
    Bmp[BUTTON_LAY_DOWN].Speed = 3;
    Bmp[BUTTON_STOP].AnimationPhase = -1;

    // BUTTSUCHEN
    Bmp[BUTTON_SEARCH].sourceRect.left = 63;
    Bmp[BUTTON_SEARCH].sourceRect.top = 0;
    Bmp[BUTTON_SEARCH].sourceRect.right = 63 + 35;
    Bmp[BUTTON_SEARCH].sourceRect.bottom = 35;
    Bmp[BUTTON_SEARCH].targetRect.left = rcPanel.left + 29;
    Bmp[BUTTON_SEARCH].targetRect.top = rcPanel.top + 270;
    Bmp[BUTTON_SEARCH].targetRect.right = Bmp[BUTTON_SEARCH].targetRect.left + 35;
    Bmp[BUTTON_SEARCH].targetRect.bottom = Bmp[BUTTON_SEARCH].targetRect.top + 35;
    Bmp[BUTTON_SEARCH].Width = static_cast<short>(Bmp[BUTTON_SEARCH].sourceRect.right - Bmp[BUTTON_SEARCH].sourceRect.left);
    Bmp[BUTTON_SEARCH].Height = static_cast<short>(Bmp[BUTTON_SEARCH].sourceRect.bottom - Bmp[BUTTON_SEARCH].sourceRect.top);
    Bmp[BUTTON_SEARCH].AnimationPhaseCount = 4;
    Bmp[BUTTON_SEARCH].Speed = 4;

    // BUTTESSEN
    Bmp[BUTTON_EAT].sourceRect.left = 133;
    Bmp[BUTTON_EAT].sourceRect.top = 0;
    Bmp[BUTTON_EAT].sourceRect.right = 133 + 35;
    Bmp[BUTTON_EAT].sourceRect.bottom = 35;
    Bmp[BUTTON_EAT].targetRect.left = rcPanel.left + 70;
    Bmp[BUTTON_EAT].targetRect.top = rcPanel.top + 270;
    Bmp[BUTTON_EAT].targetRect.right = Bmp[BUTTON_EAT].targetRect.left + 35;
    Bmp[BUTTON_EAT].targetRect.bottom = Bmp[BUTTON_EAT].targetRect.top + 35;
    Bmp[BUTTON_EAT].Width = static_cast<short>(Bmp[BUTTON_EAT].sourceRect.right - Bmp[BUTTON_EAT].sourceRect.left);
    Bmp[BUTTON_EAT].Height = static_cast<short>(Bmp[BUTTON_EAT].sourceRect.bottom - Bmp[BUTTON_EAT].sourceRect.top);
    Bmp[BUTTON_EAT].AnimationPhaseCount = 4;
    Bmp[BUTTON_EAT].Speed = 4;

    // BUTTSCHLAFEN
    Bmp[BUTTON_SLEEP].sourceRect.left = 308;
    Bmp[BUTTON_SLEEP].sourceRect.top = 0;
    Bmp[BUTTON_SLEEP].sourceRect.right = 308 + 35;
    Bmp[BUTTON_SLEEP].sourceRect.bottom = 35;
    Bmp[BUTTON_SLEEP].targetRect.left = rcPanel.left + 111;
    Bmp[BUTTON_SLEEP].targetRect.top = rcPanel.top + 270;
    Bmp[BUTTON_SLEEP].targetRect.right = Bmp[BUTTON_SLEEP].targetRect.left + 35;
    Bmp[BUTTON_SLEEP].targetRect.bottom = Bmp[BUTTON_SLEEP].targetRect.top + 35;
    Bmp[BUTTON_SLEEP].Width = static_cast<short>(Bmp[BUTTON_SLEEP].sourceRect.right - Bmp[BUTTON_SLEEP].sourceRect.left);
    Bmp[BUTTON_SLEEP].Height = static_cast<short>(Bmp[BUTTON_SLEEP].sourceRect.bottom - Bmp[BUTTON_SLEEP].sourceRect.top);
    Bmp[BUTTON_SLEEP].AnimationPhaseCount = 4;
    Bmp[BUTTON_SLEEP].Speed = 3;

    // BUTTFAELLEN
    Bmp[BUTTON_CHOP].sourceRect.left = 98;
    Bmp[BUTTON_CHOP].sourceRect.top = 0;
    Bmp[BUTTON_CHOP].sourceRect.right = 98 + 35;
    Bmp[BUTTON_CHOP].sourceRect.bottom = 35;
    Bmp[BUTTON_CHOP].targetRect.left = rcPanel.left + 152;
    Bmp[BUTTON_CHOP].targetRect.top = rcPanel.top + 270;
    Bmp[BUTTON_CHOP].targetRect.right = Bmp[BUTTON_CHOP].targetRect.left + 35;
    Bmp[BUTTON_CHOP].targetRect.bottom = Bmp[BUTTON_CHOP].targetRect.top + 35;
    Bmp[BUTTON_CHOP].Width = static_cast<short>(Bmp[BUTTON_CHOP].sourceRect.right - Bmp[BUTTON_CHOP].sourceRect.left);
    Bmp[BUTTON_CHOP].Height = static_cast<short>(Bmp[BUTTON_CHOP].sourceRect.bottom - Bmp[BUTTON_CHOP].sourceRect.top);
    Bmp[BUTTON_CHOP].AnimationPhaseCount = 4;
    Bmp[BUTTON_CHOP].Speed = 4;
    Bmp[BUTTON_CHOP].AnimationPhase = -1;

    // BUTTANGELN
    Bmp[BUTTON_FISH].sourceRect.left = 413;
    Bmp[BUTTON_FISH].sourceRect.top = 0;
    Bmp[BUTTON_FISH].sourceRect.right = 413 + 35;
    Bmp[BUTTON_FISH].sourceRect.bottom = 35;
    Bmp[BUTTON_FISH].targetRect.left = rcPanel.left + 29;
    Bmp[BUTTON_FISH].targetRect.top = rcPanel.top + 325;
    Bmp[BUTTON_FISH].targetRect.right = Bmp[BUTTON_FISH].targetRect.left + 35;
    Bmp[BUTTON_FISH].targetRect.bottom = Bmp[BUTTON_FISH].targetRect.top + 35;
    Bmp[BUTTON_FISH].Width = static_cast<short>(Bmp[BUTTON_FISH].sourceRect.right - Bmp[BUTTON_FISH].sourceRect.left);
    Bmp[BUTTON_FISH].Height = static_cast<short>(Bmp[BUTTON_FISH].sourceRect.bottom - Bmp[BUTTON_FISH].sourceRect.top);
    Bmp[BUTTON_FISH].AnimationPhaseCount = 4;
    Bmp[BUTTON_FISH].Speed = 3;
    Bmp[BUTTON_FISH].AnimationPhase = -1;

    // BUTTANZUENDEN
    Bmp[BUTTON_IGNITE].sourceRect.left = 28;
    Bmp[BUTTON_IGNITE].sourceRect.top = 175;
    Bmp[BUTTON_IGNITE].sourceRect.right = 28 + 35;
    Bmp[BUTTON_IGNITE].sourceRect.bottom = 175 + 35;
    Bmp[BUTTON_IGNITE].targetRect.left = rcPanel.left + 70;
    Bmp[BUTTON_IGNITE].targetRect.top = rcPanel.top + 325;
    Bmp[BUTTON_IGNITE].targetRect.right = Bmp[BUTTON_IGNITE].targetRect.left + 35;
    Bmp[BUTTON_IGNITE].targetRect.bottom = Bmp[BUTTON_IGNITE].targetRect.top + 35;
    Bmp[BUTTON_IGNITE].Width = static_cast<short>(Bmp[BUTTON_IGNITE].sourceRect.right - Bmp[BUTTON_IGNITE].sourceRect.left);
    Bmp[BUTTON_IGNITE].Height = static_cast<short>(Bmp[BUTTON_IGNITE].sourceRect.bottom - Bmp[BUTTON_IGNITE].sourceRect.top);
    Bmp[BUTTON_IGNITE].AnimationPhaseCount = 3;
    Bmp[BUTTON_IGNITE].Speed = 4;
    Bmp[BUTTON_IGNITE].AnimationPhase = -1;

    // BUTTAUSSCHAU
    Bmp[BUTTON_LOOK_OUT].sourceRect.left = 63;
    Bmp[BUTTON_LOOK_OUT].sourceRect.top = 175;
    Bmp[BUTTON_LOOK_OUT].sourceRect.right = 63 + 35;
    Bmp[BUTTON_LOOK_OUT].sourceRect.bottom = 175 + 35;
    Bmp[BUTTON_LOOK_OUT].targetRect.left = rcPanel.left + 111;
    Bmp[BUTTON_LOOK_OUT].targetRect.top = rcPanel.top + 325;
    Bmp[BUTTON_LOOK_OUT].targetRect.right = Bmp[BUTTON_LOOK_OUT].targetRect.left + 35;
    Bmp[BUTTON_LOOK_OUT].targetRect.bottom = Bmp[BUTTON_LOOK_OUT].targetRect.top + 35;
    Bmp[BUTTON_LOOK_OUT].Width = static_cast<short>(Bmp[BUTTON_LOOK_OUT].sourceRect.right - Bmp[BUTTON_LOOK_OUT].sourceRect.left);
    Bmp[BUTTON_LOOK_OUT].Height = static_cast<short>(Bmp[BUTTON_LOOK_OUT].sourceRect.bottom - Bmp[BUTTON_LOOK_OUT].sourceRect.top);
    Bmp[BUTTON_LOOK_OUT].AnimationPhaseCount = 4;
    Bmp[BUTTON_LOOK_OUT].Speed = 3;
    Bmp[BUTTON_LOOK_OUT].AnimationPhase = -1;

    // BUTTSCHATZKARTE
    Bmp[BUTTON_TREASUREMAP].sourceRect.left = 98;
    Bmp[BUTTON_TREASUREMAP].sourceRect.top = 175;
    Bmp[BUTTON_TREASUREMAP].sourceRect.right = 98 + 35;
    Bmp[BUTTON_TREASUREMAP].sourceRect.bottom = 175 + 35;
    Bmp[BUTTON_TREASUREMAP].targetRect.left = rcPanel.left + 152;
    Bmp[BUTTON_TREASUREMAP].targetRect.top = rcPanel.top + 325;
    Bmp[BUTTON_TREASUREMAP].targetRect.right = Bmp[BUTTON_TREASUREMAP].targetRect.left + 35;
    Bmp[BUTTON_TREASUREMAP].targetRect.bottom = Bmp[BUTTON_TREASUREMAP].targetRect.top + 35;
    Bmp[BUTTON_TREASUREMAP].Width = static_cast<short>(Bmp[BUTTON_TREASUREMAP].sourceRect.right - Bmp[BUTTON_TREASUREMAP].sourceRect.left);
    Bmp[BUTTON_TREASUREMAP].Height = static_cast<short>(Bmp[BUTTON_TREASUREMAP].sourceRect.bottom - Bmp[BUTTON_TREASUREMAP].sourceRect.top);
    Bmp[BUTTON_TREASUREMAP].AnimationPhaseCount = 4;
    Bmp[BUTTON_TREASUREMAP].Speed = 3;
    Bmp[BUTTON_TREASUREMAP].AnimationPhase = -1;

    // BUTTSCHATZ
    Bmp[BUTTON_TREASURE].sourceRect.left = 133;
    Bmp[BUTTON_TREASURE].sourceRect.top = 175;
    Bmp[BUTTON_TREASURE].sourceRect.right = 133 + 35;
    Bmp[BUTTON_TREASURE].sourceRect.bottom = 175 + 35;
    Bmp[BUTTON_TREASURE].targetRect.left = rcPanel.left + 29;
    Bmp[BUTTON_TREASURE].targetRect.top = rcPanel.top + 380;
    Bmp[BUTTON_TREASURE].targetRect.right = Bmp[BUTTON_TREASURE].targetRect.left + 35;
    Bmp[BUTTON_TREASURE].targetRect.bottom = Bmp[BUTTON_TREASURE].targetRect.top + 35;
    Bmp[BUTTON_TREASURE].Width = static_cast<short>(Bmp[BUTTON_TREASURE].sourceRect.right - Bmp[BUTTON_TREASURE].sourceRect.left);
    Bmp[BUTTON_TREASURE].Height = static_cast<short>(Bmp[BUTTON_TREASURE].sourceRect.bottom - Bmp[BUTTON_TREASURE].sourceRect.top);
    Bmp[BUTTON_TREASURE].AnimationPhaseCount = 4;
    Bmp[BUTTON_TREASURE].Speed = 3;
    Bmp[BUTTON_TREASURE].AnimationPhase = -1;

    // BUTTSCHLEUDER
    Bmp[BUTTON_SLINGSHOT].sourceRect.left = 168;
    Bmp[BUTTON_SLINGSHOT].sourceRect.top = 175;
    Bmp[BUTTON_SLINGSHOT].sourceRect.right = 168 + 35;
    Bmp[BUTTON_SLINGSHOT].sourceRect.bottom = 175 + 35;
    Bmp[BUTTON_SLINGSHOT].targetRect.left = rcPanel.left + 70;
    Bmp[BUTTON_SLINGSHOT].targetRect.top = rcPanel.top + 380;
    Bmp[BUTTON_SLINGSHOT].targetRect.right = Bmp[BUTTON_SLINGSHOT].targetRect.left + 35;
    Bmp[BUTTON_SLINGSHOT].targetRect.bottom = Bmp[BUTTON_SLINGSHOT].targetRect.top + 35;
    Bmp[BUTTON_SLINGSHOT].Width = static_cast<short>(Bmp[BUTTON_SLINGSHOT].sourceRect.right - Bmp[BUTTON_SLINGSHOT].sourceRect.left);
    Bmp[BUTTON_SLINGSHOT].Height = static_cast<short>(Bmp[BUTTON_SLINGSHOT].sourceRect.bottom - Bmp[BUTTON_SLINGSHOT].sourceRect.top);
    Bmp[BUTTON_SLINGSHOT].AnimationPhaseCount = 4;
    Bmp[BUTTON_SLINGSHOT].Speed = 3;
    Bmp[BUTTON_SLINGSHOT].AnimationPhase = -1;

    // BUTTFELD
    Bmp[BUTTON_FARM].sourceRect.left = 238;
    Bmp[BUTTON_FARM].sourceRect.top = 0;
    Bmp[BUTTON_FARM].sourceRect.right = 238 + 35;
    Bmp[BUTTON_FARM].sourceRect.bottom = 35;
    Bmp[BUTTON_FARM].targetRect.left = rcPanel.left + 70;
    Bmp[BUTTON_FARM].targetRect.top = rcPanel.top + 270;
    Bmp[BUTTON_FARM].targetRect.right = Bmp[BUTTON_FARM].targetRect.left + 35;
    Bmp[BUTTON_FARM].targetRect.bottom = Bmp[BUTTON_FARM].targetRect.top + 35;
    Bmp[BUTTON_FARM].Width = static_cast<short>(Bmp[BUTTON_FARM].sourceRect.right - Bmp[BUTTON_FARM].sourceRect.left);
    Bmp[BUTTON_FARM].Height = static_cast<short>(Bmp[BUTTON_FARM].sourceRect.bottom - Bmp[BUTTON_FARM].sourceRect.top);
    Bmp[BUTTON_FARM].AnimationPhaseCount = 4;
    Bmp[BUTTON_FARM].Speed = 3;
    Bmp[BUTTON_FARM].AnimationPhase = -1;

    // BUTTZELT
    Bmp[BUTTON_TENT].sourceRect.left = 273;
    Bmp[BUTTON_TENT].sourceRect.top = 0;
    Bmp[BUTTON_TENT].sourceRect.right = 273 + 35;
    Bmp[BUTTON_TENT].sourceRect.bottom = 35;
    Bmp[BUTTON_TENT].targetRect.left = rcPanel.left + 29;
    Bmp[BUTTON_TENT].targetRect.top = rcPanel.top + 270;
    Bmp[BUTTON_TENT].targetRect.right = Bmp[BUTTON_TENT].targetRect.left + 35;
    Bmp[BUTTON_TENT].targetRect.bottom = Bmp[BUTTON_TENT].targetRect.top + 35;
    Bmp[BUTTON_TENT].Width = static_cast<short>(Bmp[BUTTON_TENT].sourceRect.right - Bmp[BUTTON_TENT].sourceRect.left);
    Bmp[BUTTON_TENT].Height = static_cast<short>(Bmp[BUTTON_TENT].sourceRect.bottom - Bmp[BUTTON_TENT].sourceRect.top);
    Bmp[BUTTON_TENT].AnimationPhaseCount = 3;
    Bmp[BUTTON_TENT].Speed = 3;
    Bmp[BUTTON_TENT].AnimationPhase = 0;

    // BUTTBOOT
    Bmp[BUTTON_BOAT].sourceRect.left = 448;
    Bmp[BUTTON_BOAT].sourceRect.top = 0;
    Bmp[BUTTON_BOAT].sourceRect.right = 448 + 35;
    Bmp[BUTTON_BOAT].sourceRect.bottom = 35;
    Bmp[BUTTON_BOAT].targetRect.left = rcPanel.left + 111;
    Bmp[BUTTON_BOAT].targetRect.top = rcPanel.top + 270;
    Bmp[BUTTON_BOAT].targetRect.right = Bmp[BUTTON_BOAT].targetRect.left + 35;
    Bmp[BUTTON_BOAT].targetRect.bottom = Bmp[BUTTON_BOAT].targetRect.top + 35;
    Bmp[BUTTON_BOAT].Width = static_cast<short>(Bmp[BUTTON_BOAT].sourceRect.right - Bmp[BUTTON_BOAT].sourceRect.left);
    Bmp[BUTTON_BOAT].Height = static_cast<short>(Bmp[BUTTON_BOAT].sourceRect.bottom - Bmp[BUTTON_BOAT].sourceRect.top);
    Bmp[BUTTON_BOAT].AnimationPhaseCount = 3;
    Bmp[BUTTON_BOAT].Speed = 3;
    Bmp[BUTTON_BOAT].AnimationPhase = -1;

    // BUTTROHR
    Bmp[BUTTON_PIPE].sourceRect.left = 518;
    Bmp[BUTTON_PIPE].sourceRect.top = 0;
    Bmp[BUTTON_PIPE].sourceRect.right = 518 + 35;
    Bmp[BUTTON_PIPE].sourceRect.bottom = 35;
    Bmp[BUTTON_PIPE].targetRect.left = rcPanel.left + 152;
    Bmp[BUTTON_PIPE].targetRect.top = rcPanel.top + 270;
    Bmp[BUTTON_PIPE].targetRect.right = Bmp[BUTTON_PIPE].targetRect.left + 35;
    Bmp[BUTTON_PIPE].targetRect.bottom = Bmp[BUTTON_PIPE].targetRect.top + 35;
    Bmp[BUTTON_PIPE].Width = static_cast<short>(Bmp[BUTTON_PIPE].sourceRect.right - Bmp[BUTTON_PIPE].sourceRect.left);
    Bmp[BUTTON_PIPE].Height = static_cast<short>(Bmp[BUTTON_PIPE].sourceRect.bottom - Bmp[BUTTON_PIPE].sourceRect.top);
    Bmp[BUTTON_PIPE].AnimationPhaseCount = 4;
    Bmp[BUTTON_PIPE].Speed = 2;
    Bmp[BUTTON_PIPE].AnimationPhase = -1;

    // BUTTSOS
    Bmp[BUTTON_SOS].sourceRect.left = 588;
    Bmp[BUTTON_SOS].sourceRect.top = 0;
    Bmp[BUTTON_SOS].sourceRect.right = 588 + 35;
    Bmp[BUTTON_SOS].sourceRect.bottom = 35;
    Bmp[BUTTON_SOS].targetRect.left = rcPanel.left + 29;
    Bmp[BUTTON_SOS].targetRect.top = rcPanel.top + 325;
    Bmp[BUTTON_SOS].targetRect.right = Bmp[BUTTON_SOS].targetRect.left + 35;
    Bmp[BUTTON_SOS].targetRect.bottom = Bmp[BUTTON_SOS].targetRect.top + 35;
    Bmp[BUTTON_SOS].Width = static_cast<short>(Bmp[BUTTON_SOS].sourceRect.right - Bmp[BUTTON_SOS].sourceRect.left);
    Bmp[BUTTON_SOS].Height = static_cast<short>(Bmp[BUTTON_SOS].sourceRect.bottom - Bmp[BUTTON_SOS].sourceRect.top);
    Bmp[BUTTON_SOS].AnimationPhaseCount = 3;
    Bmp[BUTTON_SOS].Speed = 2;

    // BUTTHAUS1
    Bmp[BUTTON_HOUSE_1].sourceRect.left = 623;
    Bmp[BUTTON_HOUSE_1].sourceRect.top = 0;
    Bmp[BUTTON_HOUSE_1].sourceRect.right = 623 + 35;
    Bmp[BUTTON_HOUSE_1].sourceRect.bottom = 35;
    Bmp[BUTTON_HOUSE_1].targetRect.left = rcPanel.left + 70;
    Bmp[BUTTON_HOUSE_1].targetRect.top = rcPanel.top + 325;
    Bmp[BUTTON_HOUSE_1].targetRect.right = Bmp[BUTTON_HOUSE_1].targetRect.left + 35;
    Bmp[BUTTON_HOUSE_1].targetRect.bottom = Bmp[BUTTON_HOUSE_1].targetRect.top + 35;
    Bmp[BUTTON_HOUSE_1].Width = static_cast<short>(Bmp[BUTTON_HOUSE_1].sourceRect.right - Bmp[BUTTON_HOUSE_1].sourceRect.left);
    Bmp[BUTTON_HOUSE_1].Height = static_cast<short>(Bmp[BUTTON_HOUSE_1].sourceRect.bottom - Bmp[BUTTON_HOUSE_1].sourceRect.top);
    Bmp[BUTTON_HOUSE_1].AnimationPhaseCount = 5;
    Bmp[BUTTON_HOUSE_1].Speed = 3;
    Bmp[BUTTON_HOUSE_1].AnimationPhase = -1;

    // BUTTHAUS2
    Bmp[BUTTON_HOUSE_2].sourceRect.left = 658;
    Bmp[BUTTON_HOUSE_2].sourceRect.top = 0;
    Bmp[BUTTON_HOUSE_2].sourceRect.right = 658 + 35;
    Bmp[BUTTON_HOUSE_2].sourceRect.bottom = 35;
    Bmp[BUTTON_HOUSE_2].targetRect.left = rcPanel.left + 111;
    Bmp[BUTTON_HOUSE_2].targetRect.top = rcPanel.top + 325;
    Bmp[BUTTON_HOUSE_2].targetRect.right = Bmp[BUTTON_HOUSE_2].targetRect.left + 35;
    Bmp[BUTTON_HOUSE_2].targetRect.bottom = Bmp[BUTTON_HOUSE_2].targetRect.top + 35;
    Bmp[BUTTON_HOUSE_2].Width = static_cast<short>(Bmp[BUTTON_HOUSE_2].sourceRect.right - Bmp[BUTTON_HOUSE_2].sourceRect.left);
    Bmp[BUTTON_HOUSE_2].Height = static_cast<short>(Bmp[BUTTON_HOUSE_2].sourceRect.bottom - Bmp[BUTTON_HOUSE_2].sourceRect.top);
    Bmp[BUTTON_HOUSE_2].AnimationPhaseCount = 4;
    Bmp[BUTTON_HOUSE_2].Speed = 3;
    Bmp[BUTTON_HOUSE_2].AnimationPhase = -1;

    // BUTTHAUS3
    Bmp[BUTTON_HOUSE_3].sourceRect.left = 693;
    Bmp[BUTTON_HOUSE_3].sourceRect.top = 0;
    Bmp[BUTTON_HOUSE_3].sourceRect.right = 693 + 35;
    Bmp[BUTTON_HOUSE_3].sourceRect.bottom = 35;
    Bmp[BUTTON_HOUSE_3].targetRect.left = rcPanel.left + 152;
    Bmp[BUTTON_HOUSE_3].targetRect.top = rcPanel.top + 325;
    Bmp[BUTTON_HOUSE_3].targetRect.right = Bmp[BUTTON_HOUSE_3].targetRect.left + 35;
    Bmp[BUTTON_HOUSE_3].targetRect.bottom = Bmp[BUTTON_HOUSE_3].targetRect.top + 35;
    Bmp[BUTTON_HOUSE_3].Width = static_cast<short>(Bmp[BUTTON_HOUSE_3].sourceRect.right - Bmp[BUTTON_HOUSE_3].sourceRect.left);
    Bmp[BUTTON_HOUSE_3].Height = static_cast<short>(Bmp[BUTTON_HOUSE_3].sourceRect.bottom - Bmp[BUTTON_HOUSE_3].sourceRect.top);
    Bmp[BUTTON_HOUSE_3].AnimationPhaseCount = 4;
    Bmp[BUTTON_HOUSE_3].Speed = 3;
    Bmp[BUTTON_HOUSE_3].AnimationPhase = -1;

    // BUTTFEUERST
    Bmp[BUTTON_FIRE].sourceRect.left = 728;
    Bmp[BUTTON_FIRE].sourceRect.top = 0;
    Bmp[BUTTON_FIRE].sourceRect.right = 728 + 35;
    Bmp[BUTTON_FIRE].sourceRect.bottom = 35;
    Bmp[BUTTON_FIRE].targetRect.left = rcPanel.left + 29;
    Bmp[BUTTON_FIRE].targetRect.top = rcPanel.top + 380;
    Bmp[BUTTON_FIRE].targetRect.right = Bmp[BUTTON_FIRE].targetRect.left + 35;
    Bmp[BUTTON_FIRE].targetRect.bottom = Bmp[BUTTON_FIRE].targetRect.top + 35;
    Bmp[BUTTON_FIRE].Width = static_cast<short>(Bmp[BUTTON_FIRE].sourceRect.right - Bmp[BUTTON_FIRE].sourceRect.left);
    Bmp[BUTTON_FIRE].Height = static_cast<short>(Bmp[BUTTON_FIRE].sourceRect.bottom - Bmp[BUTTON_FIRE].sourceRect.top);
    Bmp[BUTTON_FIRE].AnimationPhaseCount = 4;
    Bmp[BUTTON_FIRE].Speed = 3;

    // BUTTFRAGEZ
    Bmp[BUTTON_QUESTION].sourceRect.left = 203;
    Bmp[BUTTON_QUESTION].sourceRect.top = 175;
    Bmp[BUTTON_QUESTION].sourceRect.right = 203 + 35;
    Bmp[BUTTON_QUESTION].sourceRect.bottom = 175 + 35;
    Bmp[BUTTON_QUESTION].targetRect.left = 0;
    Bmp[BUTTON_QUESTION].targetRect.top = 0;
    Bmp[BUTTON_QUESTION].targetRect.right = 0 + 35;
    Bmp[BUTTON_QUESTION].targetRect.bottom = 0 + 35;
    Bmp[BUTTON_QUESTION].Width = static_cast<short>(Bmp[BUTTON_QUESTION].sourceRect.right - Bmp[BUTTON_QUESTION].sourceRect.left);
    Bmp[BUTTON_QUESTION].Height = static_cast<short>(Bmp[BUTTON_QUESTION].sourceRect.bottom - Bmp[BUTTON_QUESTION].sourceRect.top);
    Bmp[BUTTON_QUESTION].AnimationPhaseCount = 1;
    Bmp[BUTTON_QUESTION].Speed = 0;

    // BUTTDESTROY
    Bmp[BUTTON_DESTROY].sourceRect.left = 553;
    Bmp[BUTTON_DESTROY].sourceRect.top = 0;
    Bmp[BUTTON_DESTROY].sourceRect.right = 553 + 35;
    Bmp[BUTTON_DESTROY].sourceRect.bottom = 35;
    Bmp[BUTTON_DESTROY].targetRect.left = rcPanel.left + 152;
    Bmp[BUTTON_DESTROY].targetRect.top = rcPanel.top + 380;
    Bmp[BUTTON_DESTROY].targetRect.right = Bmp[BUTTON_DESTROY].targetRect.left + 35;
    Bmp[BUTTON_DESTROY].targetRect.bottom = Bmp[BUTTON_DESTROY].targetRect.top + 35;
    Bmp[BUTTON_DESTROY].Width = static_cast<short>(Bmp[BUTTON_DESTROY].sourceRect.right - Bmp[BUTTON_DESTROY].sourceRect.left);
    Bmp[BUTTON_DESTROY].Height = static_cast<short>(Bmp[BUTTON_DESTROY].sourceRect.bottom - Bmp[BUTTON_DESTROY].sourceRect.top);
    Bmp[BUTTON_DESTROY].AnimationPhaseCount = 4;
    Bmp[BUTTON_DESTROY].Speed = 5;

    // SpzAni
    for (i = TREE_DOWN_1; i <= TREE_DOWN_4; i++) {
        Bmp[i].IsAnimationRunning = true;
        Bmp[i].Surface = lpDDSBaum;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].targetRect.left = 0;
        Bmp[i].targetRect.top = 0;
        Bmp[i].targetRect.right = 0;
        Bmp[i].targetRect.bottom = 0;
        Bmp[i].AnimationPhaseCount = 3;
        Bmp[i].Speed = 4;
    }

    // BAUM1DOWN
    Bmp[TREE_DOWN_1].sourceRect.left = 101;
    Bmp[TREE_DOWN_1].sourceRect.top = 0;
    Bmp[TREE_DOWN_1].sourceRect.right = 101 + 35;
    Bmp[TREE_DOWN_1].sourceRect.bottom = 27;
    Bmp[TREE_DOWN_1].Width = static_cast<short>(Bmp[TREE_DOWN_1].sourceRect.right - Bmp[TREE_DOWN_1].sourceRect.left);
    Bmp[TREE_DOWN_1].Height = static_cast<short>(Bmp[TREE_DOWN_1].sourceRect.bottom - Bmp[TREE_DOWN_1].sourceRect.top);

    // BAUM2DOWN
    Bmp[TREE_DOWN_2].sourceRect.left = 136;
    Bmp[TREE_DOWN_2].sourceRect.top = 0;
    Bmp[TREE_DOWN_2].sourceRect.right = 136 + 36;
    Bmp[TREE_DOWN_2].sourceRect.bottom = 27;
    Bmp[TREE_DOWN_2].Width = static_cast<short>(Bmp[TREE_DOWN_2].sourceRect.right - Bmp[TREE_DOWN_2].sourceRect.left);
    Bmp[TREE_DOWN_2].Height = static_cast<short>(Bmp[TREE_DOWN_2].sourceRect.bottom - Bmp[TREE_DOWN_2].sourceRect.top);

    // BAUM3DOWN
    Bmp[TREE_DOWN_3].sourceRect.left = 172;
    Bmp[TREE_DOWN_3].sourceRect.top = 0;
    Bmp[TREE_DOWN_3].sourceRect.right = 172 + 34;
    Bmp[TREE_DOWN_3].sourceRect.bottom = 28;
    Bmp[TREE_DOWN_3].Width = static_cast<short>(Bmp[TREE_DOWN_3].sourceRect.right - Bmp[TREE_DOWN_3].sourceRect.left);
    Bmp[TREE_DOWN_3].Height = static_cast<short>(Bmp[TREE_DOWN_3].sourceRect.bottom - Bmp[TREE_DOWN_3].sourceRect.top);

    // BAUM4DOWN
    Bmp[TREE_DOWN_4].sourceRect.left = 206;
    Bmp[TREE_DOWN_4].sourceRect.top = 0;
    Bmp[TREE_DOWN_4].sourceRect.right = 206 + 32;
    Bmp[TREE_DOWN_4].sourceRect.bottom = 17;
    Bmp[TREE_DOWN_4].Width = static_cast<short>(Bmp[TREE_DOWN_4].sourceRect.right - Bmp[TREE_DOWN_4].sourceRect.left);
    Bmp[TREE_DOWN_4].Height = static_cast<short>(Bmp[TREE_DOWN_4].sourceRect.bottom - Bmp[TREE_DOWN_4].sourceRect.top);

    // Sonstiges

    // Säule1
    Bmp[COLUMN_1].AnimationPhaseCount = 1;
    Bmp[COLUMN_1].sourceRect.left = 205;
    Bmp[COLUMN_1].sourceRect.top = 115;
    Bmp[COLUMN_1].sourceRect.right = Bmp[COLUMN_1].sourceRect.left + 11;
    Bmp[COLUMN_1].sourceRect.bottom = Bmp[COLUMN_1].sourceRect.top + 115;
    Bmp[COLUMN_1].targetRect.left = rcPanel.left + 52;
    Bmp[COLUMN_1].targetRect.top = rcPanel.top + 471;
    Bmp[COLUMN_1].targetRect.right = Bmp[COLUMN_1].targetRect.left + 11;
    Bmp[COLUMN_1].targetRect.bottom = Bmp[COLUMN_1].targetRect.top + 115;
    Bmp[COLUMN_1].Width = static_cast<short>(Bmp[COLUMN_1].sourceRect.right - Bmp[COLUMN_1].sourceRect.left);
    Bmp[COLUMN_1].Height = static_cast<short>(Bmp[COLUMN_1].sourceRect.bottom - Bmp[COLUMN_1].sourceRect.top);
    Bmp[COLUMN_1].Surface = lpDDSPanel;

    // Säule2
    Bmp[COLUMN_2].AnimationPhaseCount = 1;
    Bmp[COLUMN_2].sourceRect.left = 216;
    Bmp[COLUMN_2].sourceRect.top = 115;
    Bmp[COLUMN_2].sourceRect.right = Bmp[COLUMN_2].sourceRect.left + 11;
    Bmp[COLUMN_2].sourceRect.bottom = Bmp[COLUMN_2].sourceRect.top + 115;
    Bmp[COLUMN_2].targetRect.left = rcPanel.left + 114;
    Bmp[COLUMN_2].targetRect.top = rcPanel.top + 471;
    Bmp[COLUMN_2].targetRect.right = Bmp[COLUMN_2].targetRect.left + 11;
    Bmp[COLUMN_2].targetRect.bottom = Bmp[COLUMN_2].targetRect.top + 115;
    Bmp[COLUMN_2].Width = static_cast<short>(Bmp[COLUMN_2].sourceRect.right - Bmp[COLUMN_2].sourceRect.left);
    Bmp[COLUMN_2].Height = static_cast<short>(Bmp[COLUMN_2].sourceRect.bottom - Bmp[COLUMN_2].sourceRect.top);
    Bmp[COLUMN_2].Surface = lpDDSPanel;

    // Säule3
    Bmp[COLUMN_3].AnimationPhaseCount = 1;
    Bmp[COLUMN_3].sourceRect.left = 227;
    Bmp[COLUMN_3].sourceRect.top = 115;
    Bmp[COLUMN_3].sourceRect.right = Bmp[COLUMN_3].sourceRect.left + 11;
    Bmp[COLUMN_3].sourceRect.bottom = Bmp[COLUMN_3].sourceRect.top + 115;
    Bmp[COLUMN_3].targetRect.left = rcPanel.left + 175;
    Bmp[COLUMN_3].targetRect.top = rcPanel.top + 471;
    Bmp[COLUMN_3].targetRect.right = Bmp[COLUMN_3].targetRect.left + 11;
    Bmp[COLUMN_3].targetRect.bottom = Bmp[COLUMN_3].targetRect.top + 115;
    Bmp[COLUMN_3].Width = static_cast<short>(Bmp[COLUMN_3].sourceRect.right - Bmp[COLUMN_3].sourceRect.left);
    Bmp[COLUMN_3].Height = static_cast<short>(Bmp[COLUMN_3].sourceRect.bottom - Bmp[COLUMN_3].sourceRect.top);
    Bmp[COLUMN_3].Surface = lpDDSPanel;

    // Rohstoffe
    for (i = RAW_TREE_BRANCH; i <= RAW_SLINGSHOT; i++) {
        Bmp[i].AnimationPhaseCount = 1;
        Bmp[i].sourceRect.left = 18 + (i - RAW_TREE_BRANCH) * 16;
        Bmp[i].sourceRect.top = 0;
        Bmp[i].sourceRect.right = Bmp[i].sourceRect.left + 16;
        Bmp[i].sourceRect.bottom = Bmp[i].sourceRect.top + 15;
        Bmp[i].Width = static_cast<short>(Bmp[i].sourceRect.right - Bmp[i].sourceRect.left);
        Bmp[i].Height = static_cast<short>(Bmp[i].sourceRect.bottom - Bmp[i].sourceRect.top);
        Bmp[i].Surface = lpDDSInventar;
    }

    // RohAst
    Bmp[RAW_TREE_BRANCH].targetRect.left = rcPanel.left + 34;
    Bmp[RAW_TREE_BRANCH].targetRect.top = rcPanel.top + 280;
    Bmp[RAW_TREE_BRANCH].targetRect.right = Bmp[RAW_TREE_BRANCH].targetRect.left + 16;
    Bmp[RAW_TREE_BRANCH].targetRect.bottom = Bmp[RAW_TREE_BRANCH].targetRect.top + 15;
    // ROHSTEIN
    Bmp[RAW_STONE].targetRect.left = rcPanel.left + 34;
    Bmp[RAW_STONE].targetRect.top = rcPanel.top + 300;
    Bmp[RAW_STONE].targetRect.right = Bmp[RAW_STONE].targetRect.left + 16;
    Bmp[RAW_STONE].targetRect.bottom = Bmp[RAW_STONE].targetRect.top + 15;
    // ROHSTAMM
    Bmp[RAW_TREE_TRUNK].targetRect.left = rcPanel.left + 34;
    Bmp[RAW_TREE_TRUNK].targetRect.top = rcPanel.top + 360;
    Bmp[RAW_TREE_TRUNK].targetRect.right = Bmp[RAW_TREE_TRUNK].targetRect.left + 16;
    Bmp[RAW_TREE_TRUNK].targetRect.bottom = Bmp[RAW_TREE_TRUNK].targetRect.top + 15;
    // ROHAXT
    Bmp[RAW_AXE].targetRect.left = rcPanel.left + 150;
    Bmp[RAW_AXE].targetRect.top = rcPanel.top + 280;
    Bmp[RAW_AXE].targetRect.right = Bmp[RAW_AXE].targetRect.left + 16;
    Bmp[RAW_AXE].targetRect.bottom = Bmp[RAW_AXE].targetRect.top + 15;
    // ROHBLATT
    Bmp[RAW_LEAF].targetRect.left = rcPanel.left + 34;
    Bmp[RAW_LEAF].targetRect.top = rcPanel.top + 320;
    Bmp[RAW_LEAF].targetRect.right = Bmp[RAW_LEAF].targetRect.left + 16;
    Bmp[RAW_LEAF].targetRect.bottom = Bmp[RAW_LEAF].targetRect.top + 15;
    // ROHEGGE
    Bmp[RAW_HOE].targetRect.left = rcPanel.left + 150;
    Bmp[RAW_HOE].targetRect.top = rcPanel.top + 300;
    Bmp[RAW_HOE].targetRect.right = Bmp[RAW_HOE].targetRect.left + 16;
    Bmp[RAW_HOE].targetRect.bottom = Bmp[RAW_HOE].targetRect.top + 15;
    // ROHLIANE
    Bmp[RAW_LIANA].targetRect.left = rcPanel.left + 34;
    Bmp[RAW_LIANA].targetRect.top = rcPanel.top + 340;
    Bmp[RAW_LIANA].targetRect.right = Bmp[RAW_LIANA].targetRect.left + 16;
    Bmp[RAW_LIANA].targetRect.bottom = Bmp[RAW_LIANA].targetRect.top + 15;
    // ROHANGEL
    Bmp[RAW_FISHING_POLE].targetRect.left = rcPanel.left + 150;
    Bmp[RAW_FISHING_POLE].targetRect.top = rcPanel.top + 320;
    Bmp[RAW_FISHING_POLE].targetRect.right = Bmp[RAW_FISHING_POLE].targetRect.left + 16;
    Bmp[RAW_FISHING_POLE].targetRect.bottom = Bmp[RAW_FISHING_POLE].targetRect.top + 15;
    // ROHHAMMER
    Bmp[RAW_HAMMER].targetRect.left = rcPanel.left + 150;
    Bmp[RAW_HAMMER].targetRect.top = rcPanel.top + 340;
    Bmp[RAW_HAMMER].targetRect.right = Bmp[RAW_HAMMER].targetRect.left + 16;
    Bmp[RAW_HAMMER].targetRect.bottom = Bmp[RAW_HAMMER].targetRect.top + 15;
    // ROHFERNROHR
    Bmp[RAW_TELESCOPE].targetRect.left = rcPanel.left + 150;
    Bmp[RAW_TELESCOPE].targetRect.top = rcPanel.top + 360;
    Bmp[RAW_TELESCOPE].targetRect.right = Bmp[RAW_TELESCOPE].targetRect.left + 16;
    Bmp[RAW_TELESCOPE].targetRect.bottom = Bmp[RAW_TELESCOPE].targetRect.top + 15;
    // ROHSTREICHHOLZ
    Bmp[RAW_MATCH].targetRect.left = rcPanel.left + 105;
    Bmp[RAW_MATCH].targetRect.top = rcPanel.top + 280;
    Bmp[RAW_MATCH].targetRect.right = Bmp[RAW_MATCH].targetRect.left + 16;
    Bmp[RAW_MATCH].targetRect.bottom = Bmp[RAW_MATCH].targetRect.top + 15;
    // ROHSCHAUFEL
    Bmp[RAW_SHOVEL].targetRect.left = rcPanel.left + 105;
    Bmp[RAW_SHOVEL].targetRect.top = rcPanel.top + 320;
    Bmp[RAW_SHOVEL].targetRect.right = Bmp[RAW_SHOVEL].targetRect.left + 16;
    Bmp[RAW_SHOVEL].targetRect.bottom = Bmp[RAW_SHOVEL].targetRect.top + 15;
    // ROHKARTE
    Bmp[RAW_MAP].targetRect.left = rcPanel.left + 105;
    Bmp[RAW_MAP].targetRect.top = rcPanel.top + 320;
    Bmp[RAW_MAP].targetRect.right = Bmp[RAW_MAP].targetRect.left + 16;
    Bmp[RAW_MAP].targetRect.bottom = Bmp[RAW_MAP].targetRect.top + 15;
    // ROHSCHLEUDER
    Bmp[RAW_SLINGSHOT].targetRect.left = rcPanel.left + 105;
    Bmp[RAW_SLINGSHOT].targetRect.top = rcPanel.top + 340;
    Bmp[RAW_SLINGSHOT].targetRect.right = Bmp[RAW_SLINGSHOT].targetRect.left + 16;
    Bmp[RAW_SLINGSHOT].targetRect.bottom = Bmp[RAW_SLINGSHOT].targetRect.top + 15;

    // ROEMISCH1
    Bmp[ROEMISCH1].AnimationPhaseCount = 1;
    Bmp[ROEMISCH1].sourceRect.left = 0;
    Bmp[ROEMISCH1].sourceRect.top = 0;
    Bmp[ROEMISCH1].sourceRect.right = Bmp[ROEMISCH1].sourceRect.left + 3;
    Bmp[ROEMISCH1].sourceRect.bottom = Bmp[ROEMISCH1].sourceRect.top + 13;
    Bmp[ROEMISCH1].targetRect.left = rcPanel.left + 50;
    Bmp[ROEMISCH1].targetRect.top = rcPanel.top;
    Bmp[ROEMISCH1].targetRect.right = Bmp[ROEMISCH1].targetRect.left + 3;
    Bmp[ROEMISCH1].targetRect.bottom = Bmp[ROEMISCH1].targetRect.top + 13;
    Bmp[ROEMISCH1].Width = static_cast<short>(Bmp[ROEMISCH1].sourceRect.right - Bmp[ROEMISCH1].sourceRect.left);
    Bmp[ROEMISCH1].Height = static_cast<short>(Bmp[ROEMISCH1].sourceRect.bottom - Bmp[ROEMISCH1].sourceRect.top);
    Bmp[ROEMISCH1].Surface = lpDDSInventar;

    // ROEMISCH2
    Bmp[ROEMISCH2].AnimationPhaseCount = 1;
    Bmp[ROEMISCH2].sourceRect.left = 3;
    Bmp[ROEMISCH2].sourceRect.top = 0;
    Bmp[ROEMISCH2].sourceRect.right = Bmp[ROEMISCH2].sourceRect.left + 15;
    Bmp[ROEMISCH2].sourceRect.bottom = Bmp[ROEMISCH2].sourceRect.top + 10;
    Bmp[ROEMISCH2].targetRect.left = rcPanel.left + 50;
    Bmp[ROEMISCH2].targetRect.top = rcPanel.top;
    Bmp[ROEMISCH2].targetRect.right = Bmp[ROEMISCH2].targetRect.left + 15;
    Bmp[ROEMISCH2].targetRect.bottom = Bmp[ROEMISCH2].targetRect.top + 10;
    Bmp[ROEMISCH2].Width = static_cast<short>(Bmp[ROEMISCH2].sourceRect.right - Bmp[ROEMISCH2].sourceRect.left);
    Bmp[ROEMISCH2].Height = static_cast<short>(Bmp[ROEMISCH2].sourceRect.bottom - Bmp[ROEMISCH2].sourceRect.top);
    Bmp[ROEMISCH2].Surface = lpDDSInventar;

    // INVPAPIER
    Bmp[INVENTORY_PAPER].AnimationPhaseCount = 1;
    Bmp[INVENTORY_PAPER].sourceRect.left = 0;
    Bmp[INVENTORY_PAPER].sourceRect.top = 15;
    Bmp[INVENTORY_PAPER].sourceRect.right = Bmp[INVENTORY_PAPER].sourceRect.left + 178;
    Bmp[INVENTORY_PAPER].sourceRect.bottom = Bmp[INVENTORY_PAPER].sourceRect.top + 114;
    Bmp[INVENTORY_PAPER].targetRect.left = rcPanel.left + 15;
    Bmp[INVENTORY_PAPER].targetRect.top = rcPanel.top + 270;
    Bmp[INVENTORY_PAPER].targetRect.right = Bmp[INVENTORY_PAPER].targetRect.left + 178;
    Bmp[INVENTORY_PAPER].targetRect.bottom = Bmp[INVENTORY_PAPER].targetRect.top + 114;
    Bmp[INVENTORY_PAPER].Width = static_cast<short>(Bmp[INVENTORY_PAPER].sourceRect.right - Bmp[INVENTORY_PAPER].sourceRect.left);
    Bmp[INVENTORY_PAPER].Height = static_cast<short>(Bmp[INVENTORY_PAPER].sourceRect.bottom - Bmp[INVENTORY_PAPER].sourceRect.top);
    Bmp[INVENTORY_PAPER].Surface = lpDDSInventar;

    // RING
    Bmp[RING].AnimationPhaseCount = 1;
    Bmp[RING].sourceRect.left = 205;
    Bmp[RING].sourceRect.top = 230;
    Bmp[RING].sourceRect.right = Bmp[RING].sourceRect.left + 37;
    Bmp[RING].sourceRect.bottom = Bmp[RING].sourceRect.top + 150;
    Bmp[RING].targetRect.left = rcPanel.left + 5;
    Bmp[RING].targetRect.top = rcPanel.top - 113;
    Bmp[RING].targetRect.right = Bmp[RING].targetRect.left + 30;
    Bmp[RING].targetRect.bottom = Bmp[RING].targetRect.top;
    Bmp[RING].Width = static_cast<short>(Bmp[RING].sourceRect.right - Bmp[RING].sourceRect.left);
    Bmp[RING].Height = static_cast<short>(Bmp[RING].sourceRect.bottom - Bmp[RING].sourceRect.top);
    Bmp[RING].Surface = lpDDSPanel;

    // KREUZ
    Bmp[CROSS].AnimationPhaseCount = 1;
    Bmp[CROSS].sourceRect.left = 205;
    Bmp[CROSS].sourceRect.top = 380;
    Bmp[CROSS].sourceRect.right = Bmp[CROSS].sourceRect.left + 40;
    Bmp[CROSS].sourceRect.bottom = Bmp[CROSS].sourceRect.top + 22;
    Bmp[CROSS].Width = static_cast<short>(Bmp[CROSS].sourceRect.right - Bmp[CROSS].sourceRect.left);
    Bmp[CROSS].Height = static_cast<short>(Bmp[CROSS].sourceRect.bottom - Bmp[CROSS].sourceRect.top);
    Bmp[CROSS].Surface = lpDDSPanel;

    // JA
    Bmp[YES].AnimationPhaseCount = 1;
    Bmp[YES].sourceRect.left = 0;
    Bmp[YES].sourceRect.top = 154;
    Bmp[YES].sourceRect.right = Bmp[YES].sourceRect.left + 41;
    Bmp[YES].sourceRect.bottom = Bmp[YES].sourceRect.top + 42;
    Bmp[YES].Width = static_cast<short>(Bmp[YES].sourceRect.right - Bmp[YES].sourceRect.left);
    Bmp[YES].Height = static_cast<short>(Bmp[YES].sourceRect.bottom - Bmp[YES].sourceRect.top);
    Bmp[YES].Surface = lpDDSPaper;

    // NEIN
    Bmp[NO].AnimationPhaseCount = 1;
    Bmp[NO].sourceRect.left = 41;
    Bmp[NO].sourceRect.top = 154;
    Bmp[NO].sourceRect.right = Bmp[NO].sourceRect.left + 100;
    Bmp[NO].sourceRect.bottom = Bmp[NO].sourceRect.top + 39;
    Bmp[NO].Width = static_cast<short>(Bmp[NO].sourceRect.right - Bmp[NO].sourceRect.left);
    Bmp[NO].Height = static_cast<short>(Bmp[NO].sourceRect.bottom - Bmp[NO].sourceRect.top);
    Bmp[NO].Surface = lpDDSPaper;

    // Sonne
    Bmp[SUN].AnimationPhaseCount = 1;
    Bmp[SUN].sourceRect.left = 205;
    Bmp[SUN].sourceRect.top = 65;
    Bmp[SUN].sourceRect.right = Bmp[SUN].sourceRect.left + 51;
    Bmp[SUN].sourceRect.bottom = Bmp[SUN].sourceRect.top + 50;
    Bmp[SUN].targetRect.left = rcPanel.left + 30;
    Bmp[SUN].targetRect.top = rcPanel.top + 630;
    Bmp[SUN].targetRect.right = Bmp[SUN].targetRect.left + 152;
    Bmp[SUN].targetRect.bottom = Bmp[SUN].targetRect.top + 55;
    Bmp[SUN].Width = static_cast<short>(Bmp[SUN].sourceRect.right - Bmp[SUN].sourceRect.left);
    Bmp[SUN].Height = static_cast<short>(Bmp[SUN].sourceRect.bottom - Bmp[SUN].sourceRect.top);
    Bmp[SUN].Surface = lpDDSPanel;

    // PROGRAMMIERUNG
    Bmp[PROGRAMMING].AnimationPhaseCount = 1;
    Bmp[PROGRAMMING].sourceRect.left = 0;
    Bmp[PROGRAMMING].sourceRect.top = 0;
    Bmp[PROGRAMMING].sourceRect.right = Bmp[PROGRAMMING].sourceRect.left + 348;
    Bmp[PROGRAMMING].sourceRect.bottom = Bmp[PROGRAMMING].sourceRect.top + 49;
    Bmp[PROGRAMMING].Width = static_cast<short>(Bmp[PROGRAMMING].sourceRect.right - Bmp[PROGRAMMING].sourceRect.left);
    Bmp[PROGRAMMING].Height = static_cast<short>(Bmp[PROGRAMMING].sourceRect.bottom - Bmp[PROGRAMMING].sourceRect.top);
    Bmp[PROGRAMMING].targetRect.left = MAX_SCREEN_X / 2 - Bmp[PROGRAMMING].Width / 2;
    Bmp[PROGRAMMING].targetRect.top = MAX_SCREEN_Y - Bmp[PROGRAMMING].Height / 2;
    Bmp[PROGRAMMING].targetRect.right = Bmp[PROGRAMMING].targetRect.left + Bmp[PROGRAMMING].Width;
    Bmp[PROGRAMMING].targetRect.bottom = Bmp[PROGRAMMING].targetRect.top + Bmp[PROGRAMMING].Height;
    Bmp[PROGRAMMING].Surface = lpDDSCredits;

    // DIRKPLATE
    Bmp[DIRK_PLATE].AnimationPhaseCount = 1;
    Bmp[DIRK_PLATE].sourceRect.left = 0;
    Bmp[DIRK_PLATE].sourceRect.top = 49;
    Bmp[DIRK_PLATE].sourceRect.right = Bmp[DIRK_PLATE].sourceRect.left + 196;
    Bmp[DIRK_PLATE].sourceRect.bottom = Bmp[DIRK_PLATE].sourceRect.top + 47;
    Bmp[DIRK_PLATE].Width = static_cast<short>(Bmp[DIRK_PLATE].sourceRect.right - Bmp[DIRK_PLATE].sourceRect.left);
    Bmp[DIRK_PLATE].Height = static_cast<short>(Bmp[DIRK_PLATE].sourceRect.bottom - Bmp[DIRK_PLATE].sourceRect.top);
    Bmp[DIRK_PLATE].targetRect.left = MAX_SCREEN_X / 2 - Bmp[DIRK_PLATE].Width / 2;
    Bmp[DIRK_PLATE].targetRect.top = MAX_SCREEN_Y - Bmp[DIRK_PLATE].Height / 2;
    Bmp[DIRK_PLATE].targetRect.right = Bmp[DIRK_PLATE].targetRect.left + Bmp[DIRK_PLATE].Width;
    Bmp[DIRK_PLATE].targetRect.bottom = Bmp[DIRK_PLATE].targetRect.top + Bmp[DIRK_PLATE].Height;
    Bmp[DIRK_PLATE].Surface = lpDDSCredits;

    // MATTHIAS
    Bmp[MATTHIAS].AnimationPhaseCount = 1;
    Bmp[MATTHIAS].sourceRect.left = 0;
    Bmp[MATTHIAS].sourceRect.top = 96;
    Bmp[MATTHIAS].sourceRect.right = Bmp[MATTHIAS].sourceRect.left + 379;
    Bmp[MATTHIAS].sourceRect.bottom = Bmp[MATTHIAS].sourceRect.top + 47;
    Bmp[MATTHIAS].Width = static_cast<short>(Bmp[MATTHIAS].sourceRect.right - Bmp[MATTHIAS].sourceRect.left);
    Bmp[MATTHIAS].Height = static_cast<short>(Bmp[MATTHIAS].sourceRect.bottom - Bmp[MATTHIAS].sourceRect.top);
    Bmp[MATTHIAS].targetRect.left = MAX_SCREEN_X / 2 - Bmp[MATTHIAS].Width / 2;
    Bmp[MATTHIAS].targetRect.top = MAX_SCREEN_Y - Bmp[MATTHIAS].Height / 2;
    Bmp[MATTHIAS].targetRect.right = Bmp[MATTHIAS].targetRect.left + Bmp[MATTHIAS].Width;
    Bmp[MATTHIAS].targetRect.bottom = Bmp[MATTHIAS].targetRect.top + Bmp[MATTHIAS].Height;
    Bmp[MATTHIAS].Surface = lpDDSCredits;

    // TESTSPIELER
    Bmp[TEST_PLAYER].AnimationPhaseCount = 1;
    Bmp[TEST_PLAYER].sourceRect.left = 0;
    Bmp[TEST_PLAYER].sourceRect.top = 143;
    Bmp[TEST_PLAYER].sourceRect.right = Bmp[TEST_PLAYER].sourceRect.left + 210;
    Bmp[TEST_PLAYER].sourceRect.bottom = Bmp[TEST_PLAYER].sourceRect.top + 55;
    Bmp[TEST_PLAYER].Width = static_cast<short>(Bmp[TEST_PLAYER].sourceRect.right - Bmp[TEST_PLAYER].sourceRect.left);
    Bmp[TEST_PLAYER].Height = static_cast<short>(Bmp[TEST_PLAYER].sourceRect.bottom - Bmp[TEST_PLAYER].sourceRect.top);
    Bmp[TEST_PLAYER].targetRect.left = MAX_SCREEN_X / 2 - Bmp[TEST_PLAYER].Width / 2;
    Bmp[TEST_PLAYER].targetRect.top = MAX_SCREEN_Y - Bmp[TEST_PLAYER].Height / 2;
    Bmp[TEST_PLAYER].targetRect.right = Bmp[TEST_PLAYER].targetRect.left + Bmp[TEST_PLAYER].Width;
    Bmp[TEST_PLAYER].targetRect.bottom = Bmp[TEST_PLAYER].targetRect.top + Bmp[TEST_PLAYER].Height;
    Bmp[TEST_PLAYER].Surface = lpDDSCredits;

    // TOBIAS
    Bmp[TOBIAS].AnimationPhaseCount = 1;
    Bmp[TOBIAS].sourceRect.left = 0;
    Bmp[TOBIAS].sourceRect.top = 198;
    Bmp[TOBIAS].sourceRect.right = Bmp[TOBIAS].sourceRect.left + 273;
    Bmp[TOBIAS].sourceRect.bottom = Bmp[TOBIAS].sourceRect.top + 56;
    Bmp[TOBIAS].Width = static_cast<short>(Bmp[TOBIAS].sourceRect.right - Bmp[TOBIAS].sourceRect.left);
    Bmp[TOBIAS].Height = static_cast<short>(Bmp[TOBIAS].sourceRect.bottom - Bmp[TOBIAS].sourceRect.top);
    Bmp[TOBIAS].targetRect.left = MAX_SCREEN_X / 2 - Bmp[TOBIAS].Width / 2;
    Bmp[TOBIAS].targetRect.top = MAX_SCREEN_Y - Bmp[TOBIAS].Height / 2;
    Bmp[TOBIAS].targetRect.right = Bmp[TOBIAS].targetRect.left + Bmp[TOBIAS].Width;
    Bmp[TOBIAS].targetRect.bottom = Bmp[TOBIAS].targetRect.top + Bmp[TOBIAS].Height;
    Bmp[TOBIAS].Surface = lpDDSCredits;

    // SIGRID
    Bmp[SIGRID].AnimationPhaseCount = 1;
    Bmp[SIGRID].sourceRect.left = 0;
    Bmp[SIGRID].sourceRect.top = 254;
    Bmp[SIGRID].sourceRect.right = Bmp[SIGRID].sourceRect.left + 226;
    Bmp[SIGRID].sourceRect.bottom = Bmp[SIGRID].sourceRect.top + 56;
    Bmp[SIGRID].Width = static_cast<short>(Bmp[SIGRID].sourceRect.right - Bmp[SIGRID].sourceRect.left);
    Bmp[SIGRID].Height = static_cast<short>(Bmp[SIGRID].sourceRect.bottom - Bmp[SIGRID].sourceRect.top);
    Bmp[SIGRID].targetRect.left = MAX_SCREEN_X / 2 - Bmp[SIGRID].Width / 2;
    Bmp[SIGRID].targetRect.top = MAX_SCREEN_Y - Bmp[SIGRID].Height / 2;
    Bmp[SIGRID].targetRect.right = Bmp[SIGRID].targetRect.left + Bmp[SIGRID].Width;
    Bmp[SIGRID].targetRect.bottom = Bmp[SIGRID].targetRect.top + Bmp[SIGRID].Height;
    Bmp[SIGRID].Surface = lpDDSCredits;

    // PATHFINDING
    Bmp[PATHFINDING].AnimationPhaseCount = 1;
    Bmp[PATHFINDING].sourceRect.left = 0;
    Bmp[PATHFINDING].sourceRect.top = 353;
    Bmp[PATHFINDING].sourceRect.right = Bmp[PATHFINDING].sourceRect.left + 233;
    Bmp[PATHFINDING].sourceRect.bottom = Bmp[PATHFINDING].sourceRect.top + 59;
    Bmp[PATHFINDING].Width = static_cast<short>(Bmp[PATHFINDING].sourceRect.right - Bmp[PATHFINDING].sourceRect.left);
    Bmp[PATHFINDING].Height = static_cast<short>(Bmp[PATHFINDING].sourceRect.bottom - Bmp[PATHFINDING].sourceRect.top);
    Bmp[PATHFINDING].targetRect.left = MAX_SCREEN_X / 2 - Bmp[PATHFINDING].Width / 2;
    Bmp[PATHFINDING].targetRect.top = MAX_SCREEN_Y - Bmp[PATHFINDING].Height / 2;
    Bmp[PATHFINDING].targetRect.right = Bmp[PATHFINDING].targetRect.left + Bmp[PATHFINDING].Width;
    Bmp[PATHFINDING].targetRect.bottom = Bmp[PATHFINDING].targetRect.top + Bmp[PATHFINDING].Height;
    Bmp[PATHFINDING].Surface = lpDDSCredits;

    // JOHN
    Bmp[JOHN].AnimationPhaseCount = 1;
    Bmp[JOHN].sourceRect.left = 0;
    Bmp[JOHN].sourceRect.top = 412;
    Bmp[JOHN].sourceRect.right = Bmp[JOHN].sourceRect.left + 347;
    Bmp[JOHN].sourceRect.bottom = Bmp[JOHN].sourceRect.top + 56;
    Bmp[JOHN].Width = static_cast<short>(Bmp[JOHN].sourceRect.right - Bmp[JOHN].sourceRect.left);
    Bmp[JOHN].Height = static_cast<short>(Bmp[JOHN].sourceRect.bottom - Bmp[JOHN].sourceRect.top);
    Bmp[JOHN].targetRect.left = MAX_SCREEN_X / 2 - Bmp[JOHN].Width / 2;
    Bmp[JOHN].targetRect.top = MAX_SCREEN_Y - Bmp[JOHN].Height / 2;
    Bmp[JOHN].targetRect.right = Bmp[JOHN].targetRect.left + Bmp[JOHN].Width;
    Bmp[JOHN].targetRect.bottom = Bmp[JOHN].targetRect.top + Bmp[JOHN].Height;
    Bmp[JOHN].Surface = lpDDSCredits;

    // HEIKO
    Bmp[HEIKO].AnimationPhaseCount = 1;
    Bmp[HEIKO].sourceRect.left = 0;
    Bmp[HEIKO].sourceRect.top = 468;
    Bmp[HEIKO].sourceRect.right = Bmp[HEIKO].sourceRect.left + 236;
    Bmp[HEIKO].sourceRect.bottom = Bmp[HEIKO].sourceRect.top + 47;
    Bmp[HEIKO].Width = static_cast<short>(Bmp[HEIKO].sourceRect.right - Bmp[HEIKO].sourceRect.left);
    Bmp[HEIKO].Height = static_cast<short>(Bmp[HEIKO].sourceRect.bottom - Bmp[HEIKO].sourceRect.top);
    Bmp[HEIKO].targetRect.left = MAX_SCREEN_X / 2 - Bmp[HEIKO].Width / 2;
    Bmp[HEIKO].targetRect.top = MAX_SCREEN_Y - Bmp[HEIKO].Height / 2;
    Bmp[HEIKO].targetRect.right = Bmp[HEIKO].targetRect.left + Bmp[HEIKO].Width;
    Bmp[HEIKO].targetRect.bottom = Bmp[HEIKO].targetRect.top + Bmp[HEIKO].Height;
    Bmp[HEIKO].Surface = lpDDSCredits;

    // GISELA
    Bmp[GISELA].AnimationPhaseCount = 1;
    Bmp[GISELA].sourceRect.left = 0;
    Bmp[GISELA].sourceRect.top = 515;
    Bmp[GISELA].sourceRect.right = Bmp[GISELA].sourceRect.left + 232;
    Bmp[GISELA].sourceRect.bottom = Bmp[GISELA].sourceRect.top + 47;
    Bmp[GISELA].Width = static_cast<short>(Bmp[GISELA].sourceRect.right - Bmp[GISELA].sourceRect.left);
    Bmp[GISELA].Height = static_cast<short>(Bmp[GISELA].sourceRect.bottom - Bmp[GISELA].sourceRect.top);
    Bmp[GISELA].targetRect.left = MAX_SCREEN_X / 2 - Bmp[GISELA].Width / 2;
    Bmp[GISELA].targetRect.top = MAX_SCREEN_Y - Bmp[GISELA].Height / 2;
    Bmp[GISELA].targetRect.right = Bmp[GISELA].targetRect.left + Bmp[GISELA].Width;
    Bmp[GISELA].targetRect.bottom = Bmp[GISELA].targetRect.top + Bmp[GISELA].Height;
    Bmp[GISELA].Surface = lpDDSCredits;

    // WEITEREHILFE
    Bmp[FURTHER_HELP].AnimationPhaseCount = 1;
    Bmp[FURTHER_HELP].sourceRect.left = 0;
    Bmp[FURTHER_HELP].sourceRect.top = 562;
    Bmp[FURTHER_HELP].sourceRect.right = Bmp[FURTHER_HELP].sourceRect.left + 258;
    Bmp[FURTHER_HELP].sourceRect.bottom = Bmp[FURTHER_HELP].sourceRect.top + 46;
    Bmp[FURTHER_HELP].Width = static_cast<short>(Bmp[FURTHER_HELP].sourceRect.right - Bmp[FURTHER_HELP].sourceRect.left);
    Bmp[FURTHER_HELP].Height = static_cast<short>(Bmp[FURTHER_HELP].sourceRect.bottom - Bmp[FURTHER_HELP].sourceRect.top);
    Bmp[FURTHER_HELP].targetRect.left = MAX_SCREEN_X / 2 - Bmp[FURTHER_HELP].Width / 2;
    Bmp[FURTHER_HELP].targetRect.top = MAX_SCREEN_Y - Bmp[FURTHER_HELP].Height / 2;
    Bmp[FURTHER_HELP].targetRect.right = Bmp[FURTHER_HELP].targetRect.left + Bmp[FURTHER_HELP].Width;
    Bmp[FURTHER_HELP].targetRect.bottom = Bmp[FURTHER_HELP].targetRect.top + Bmp[FURTHER_HELP].Height;
    Bmp[FURTHER_HELP].Surface = lpDDSCredits;

    // DPSOFTWARE
    Bmp[DPSOFTWARE].AnimationPhaseCount = 1;
    Bmp[DPSOFTWARE].sourceRect.left = 0;
    Bmp[DPSOFTWARE].sourceRect.top = 608;
    Bmp[DPSOFTWARE].sourceRect.right = Bmp[DPSOFTWARE].sourceRect.left + 291;
    Bmp[DPSOFTWARE].sourceRect.bottom = Bmp[DPSOFTWARE].sourceRect.top + 99;
    Bmp[DPSOFTWARE].Width = static_cast<short>(Bmp[DPSOFTWARE].sourceRect.right - Bmp[DPSOFTWARE].sourceRect.left);
    Bmp[DPSOFTWARE].Height = static_cast<short>(Bmp[DPSOFTWARE].sourceRect.bottom - Bmp[DPSOFTWARE].sourceRect.top);
    Bmp[DPSOFTWARE].targetRect.left = MAX_SCREEN_X / 2 - Bmp[DPSOFTWARE].Width / 2;
    Bmp[DPSOFTWARE].targetRect.top = MAX_SCREEN_Y - Bmp[DPSOFTWARE].Height / 2;
    Bmp[DPSOFTWARE].targetRect.right = Bmp[DPSOFTWARE].targetRect.left + Bmp[DPSOFTWARE].Width;
    Bmp[DPSOFTWARE].targetRect.bottom = Bmp[DPSOFTWARE].targetRect.top + Bmp[DPSOFTWARE].Height;
    Bmp[DPSOFTWARE].Surface = lpDDSCredits;

    // SCHWARZ
    Bmp[SCHWARZ].AnimationPhaseCount = 1;
    Bmp[SCHWARZ].sourceRect.left = 0;
    Bmp[SCHWARZ].sourceRect.top = 608;
    Bmp[SCHWARZ].sourceRect.right = Bmp[SCHWARZ].sourceRect.left + 1;
    Bmp[SCHWARZ].sourceRect.bottom = Bmp[SCHWARZ].sourceRect.top + 1;
    Bmp[SCHWARZ].Width = static_cast<short>(Bmp[SCHWARZ].sourceRect.right - Bmp[SCHWARZ].sourceRect.left);
    Bmp[SCHWARZ].Height = static_cast<short>(Bmp[SCHWARZ].sourceRect.bottom - Bmp[SCHWARZ].sourceRect.top);
    Bmp[SCHWARZ].targetRect.left = MAX_SCREEN_X / 2 - Bmp[SCHWARZ].Width / 2;
    Bmp[SCHWARZ].targetRect.top = MAX_SCREEN_Y - Bmp[SCHWARZ].Height / 2;
    Bmp[SCHWARZ].targetRect.right = Bmp[SCHWARZ].targetRect.left + Bmp[SCHWARZ].Width;
    Bmp[SCHWARZ].targetRect.bottom = Bmp[SCHWARZ].targetRect.top + Bmp[SCHWARZ].Height;
    Bmp[SCHWARZ].Surface = lpDDSCredits;

    // SOUNDS
    Bmp[SOUNDS].AnimationPhaseCount = 1;
    Bmp[SOUNDS].sourceRect.left = 0;
    Bmp[SOUNDS].sourceRect.top = 310;
    Bmp[SOUNDS].sourceRect.right = Bmp[SOUNDS].sourceRect.left + 144;
    Bmp[SOUNDS].sourceRect.bottom = Bmp[SOUNDS].sourceRect.top + 43;
    Bmp[SOUNDS].Width = static_cast<short>(Bmp[SOUNDS].sourceRect.right - Bmp[SOUNDS].sourceRect.left);
    Bmp[SOUNDS].Height = static_cast<short>(Bmp[SOUNDS].sourceRect.bottom - Bmp[SOUNDS].sourceRect.top);
    Bmp[SOUNDS].targetRect.left = MAX_SCREEN_X / 2 - Bmp[SOUNDS].Width / 2;
    Bmp[SOUNDS].targetRect.top = MAX_SCREEN_Y - Bmp[SOUNDS].Height / 2;
    Bmp[SOUNDS].targetRect.right = Bmp[SOUNDS].targetRect.left + Bmp[SOUNDS].Width;
    Bmp[SOUNDS].targetRect.bottom = Bmp[SOUNDS].targetRect.top + Bmp[SOUNDS].Height;
    Bmp[SOUNDS].Surface = lpDDSCredits;

    // MUSIK
    Bmp[MUSIC].AnimationPhaseCount = 1;
    Bmp[MUSIC].sourceRect.left = 160;
    Bmp[MUSIC].sourceRect.top = 310;
    Bmp[MUSIC].sourceRect.right = Bmp[MUSIC].sourceRect.left + 124;
    Bmp[MUSIC].sourceRect.bottom = Bmp[MUSIC].sourceRect.top + 39;
    Bmp[MUSIC].Width = static_cast<short>(Bmp[MUSIC].sourceRect.right - Bmp[MUSIC].sourceRect.left);
    Bmp[MUSIC].Height = static_cast<short>(Bmp[MUSIC].sourceRect.bottom - Bmp[MUSIC].sourceRect.top);
    Bmp[MUSIC].targetRect.left = MAX_SCREEN_X / 2 - Bmp[MUSIC].Width / 2;
    Bmp[MUSIC].targetRect.top = MAX_SCREEN_Y - Bmp[MUSIC].Height / 2;
    Bmp[MUSIC].targetRect.right = Bmp[MUSIC].targetRect.left + Bmp[MUSIC].Width;
    Bmp[MUSIC].targetRect.bottom = Bmp[MUSIC].targetRect.top + Bmp[MUSIC].Height;
    Bmp[MUSIC].Surface = lpDDSCredits;

    for (i = 0; i < 10; i++)
        for (k = 0; k < 10; k++) {
            CreditsList[i][k].IsRunning = false;
            CreditsList[i][k].Picture = -1;
        }

    CreditsList[0][0].Picture = PROGRAMMING;
    CreditsList[0][1].IsRunning = true; // nur den hier true setzen, löst dann alles andere aus
    CreditsList[0][1].Picture = DIRK_PLATE;
    CreditsList[1][0].Picture = MUSIC;
    CreditsList[1][1].Picture = HEIKO;
    CreditsList[2][0].Picture = SOUNDS;
    CreditsList[2][1].Picture = DIRK_PLATE;
    CreditsList[3][0].Picture = TEST_PLAYER;
    CreditsList[3][1].Picture = MATTHIAS;
    CreditsList[3][2].Picture = TOBIAS;
    CreditsList[3][3].Picture = SIGRID;
    CreditsList[4][0].Picture = PATHFINDING;
    CreditsList[4][1].Picture = JOHN;
    CreditsList[5][0].Picture = FURTHER_HELP;
    CreditsList[5][1].Picture = HEIKO;
    CreditsList[5][2].Picture = GISELA;
    CreditsList[6][0].Picture = SCHWARZ;
    CreditsList[6][1].Picture = DPSOFTWARE;

    // Textausgabe
    TextBereich[TXTTEXTFELD].HasText = false;
    TextBereich[TXTTEXTFELD].textRect.left = 9;
    TextBereich[TXTTEXTFELD].textRect.top = MAX_SCREEN_Y - 17;
    TextBereich[TXTTEXTFELD].textRect.right = MAX_SCREEN_X - 200;
    TextBereich[TXTTEXTFELD].textRect.bottom = MAX_SCREEN_Y - 2;

    TextBereich[TXTFPS].HasText = false;
    TextBereich[TXTFPS].textRect.left = MAX_SCREEN_X - 40;
    TextBereich[TXTFPS].textRect.top = 3;
    TextBereich[TXTFPS].textRect.right = TextBereich[TXTFPS].textRect.left + 2 * FONT1_LETTER_WIDTH;
    TextBereich[TXTFPS].textRect.bottom = TextBereich[TXTFPS].textRect.top + FONT1_LETTER_HEIGHT;

    TextBereich[TXTTAGESZEIT].HasText = true;
    TextBereich[TXTTAGESZEIT].textRect.left = MAX_SCREEN_X - 110;
    TextBereich[TXTTAGESZEIT].textRect.top = MAX_SCREEN_Y - 20;
    TextBereich[TXTTAGESZEIT].textRect.right = TextBereich[TXTTAGESZEIT].textRect.left + 5 * FONT2_LETTER_WIDTH;
    TextBereich[TXTTAGESZEIT].textRect.bottom = TextBereich[TXTTAGESZEIT].textRect.top + FONT2_LETTER_HEIGHT;

    TextBereich[TXTPAPIER].HasText = false;
    TextBereich[TXTPAPIER].textRect.left = 450;
    TextBereich[TXTPAPIER].textRect.top = 160;
    TextBereich[TXTPAPIER].textRect.right = 830;
    TextBereich[TXTPAPIER].textRect.bottom = 560;

    TextBereich[TXTCHANCE].HasText = false;
    TextBereich[TXTCHANCE].textRect.left = Bmp[RING].targetRect.left + 5;
    TextBereich[TXTCHANCE].textRect.top = Bmp[RING].targetRect.top + Bmp[RING].Height + 10;
    TextBereich[TXTCHANCE].textRect.right = TextBereich[TXTCHANCE].textRect.left + 3 * FONT2_LETTER_WIDTH;
    TextBereich[TXTCHANCE].textRect.bottom = TextBereich[TXTCHANCE].textRect.top + FONT2_LETTER_HEIGHT;

    Guy.ResourceAmount[WASSER] = 100;
    Guy.ResourceAmount[NAHRUNG] = 100;
    Guy.ResourceAmount[GESUNDHEIT] = 100;

    for (i = RAW_TREE_BRANCH; i <= RAW_SLINGSHOT; i++) {
        Guy.Inventory[i] = 0;
    }

    CursorTyp = CURSOR_ARROW;
    Gitter = false;
    PapierText = -1;
    HauptMenue = 0;
    TwoClicks = -1;
    Night = false;
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
    for (short i = 0; i < Sound::COUNT; i++) {
        Wav[i].Filename = "sounds/klick.wav";
        Wav[i].Loop = false;
        Wav[i].Volume = 100;
    }

    Wav[Sound::STORM].Filename = "sounds/sturm.wav";
    Wav[Sound::STORM].Volume = 85;

    Wav[Sound::SWIM].Filename = "sounds/schwimmen.wav";
    Wav[Sound::SWIM].Volume = 90;

    Wav[Sound::SPLAT].Filename = "sounds/platsch.wav";
    Wav[Sound::SPLAT].Volume = 95;

    Wav[Sound::LOG].Filename = "sounds/faellen.wav";
    Wav[Sound::LOG].Volume = 100;

    Wav[Sound::HIT].Filename = "sounds/schlagen.wav";
    Wav[Sound::HIT].Volume = 100;

    Wav[Sound::SLINGSHOT].Filename = "sounds/schleuder.wav";
    Wav[Sound::SLINGSHOT].Volume = 100;

    Wav[Sound::DIG].Filename = "sounds/schaufeln.wav";
    Wav[Sound::DIG].Volume = 90;

    Wav[Sound::HAMMER].Filename = "sounds/hammer.wav";
    Wav[Sound::HAMMER].Volume = 100;

    Wav[Sound::CRASH].Filename = "sounds/crash.wav";
    Wav[Sound::CRASH].Volume = 100;

    Wav[Sound::SNORE].Filename = "sounds/schnarchen.wav";
    Wav[Sound::SNORE].Volume = 90;

    Wav[Sound::DRINK].Filename = "sounds/trinken.wav";
    Wav[Sound::DRINK].Volume = 95;

    Wav[Sound::CRACKLE].Filename = "sounds/knistern.wav";
    Wav[Sound::CRACKLE].Volume = 90;

    Wav[Sound::FISH].Filename = "sounds/angel.wav";
    Wav[Sound::FISH].Volume = 100;

    Wav[Sound::FOREST].Filename = "sounds/wald.wav";
    Wav[Sound::FOREST].Volume = 90;

    Wav[Sound::FIRE].Filename = "sounds/feuer.wav";
    Wav[Sound::FIRE].Volume = 100;

    Wav[Sound::SURF].Filename = "sounds/brandung.wav";
    Wav[Sound::SURF].Volume = 100;

    Wav[Sound::TIMBER].Filename = "sounds/baumfaellt.wav";
    Wav[Sound::TIMBER].Volume = 100;

    Wav[Sound::RIVER].Filename = "sounds/fluss.wav";
    Wav[Sound::RIVER].Volume = 85;

    Wav[Sound::CLICK].Filename = "sounds/klick.wav";
    Wav[Sound::CLICK].Volume = 95;

    Wav[Sound::CLICK2].Filename = "sounds/klick2.wav";
    Wav[Sound::CLICK2].Volume = 95;

    Wav[Sound::OUTRO].Filename = "sounds/abspann.wav";
    Wav[Sound::OUTRO].Volume = 100;
    Wav[Sound::OUTRO].Loop = true;

    Wav[Sound::LOGO].Filename = "sounds/logo.wav";
    Wav[Sound::LOGO].Volume = 100;
    Wav[Sound::LOGO].Loop = true;

    Wav[Sound::WOLF].Filename = "sounds/wolf.wav";
    Wav[Sound::WOLF].Volume = 90;

    Wav[Sound::INVENTION].Filename = "sounds/erfindung.wav";
    Wav[Sound::INVENTION].Volume = 95;

    // Testweise alle Sounds gleich in den Speicher laden
    for (short i = 1; i < Sound::COUNT; i++) {
        Sound::LoadSound(i);
    }
}

void NewGame(bool neu)
{
    short x, y;
    bool LoadOK = false;

    InitStructs();
    InitWaves();

    if (!neu) {
        LoadOK = LoadGame();
    }

    if ((!LoadOK) || (neu)) {
        // For the status display
        rcRectdes.left = 0;
        rcRectdes.top = 0;
        rcRectdes.right = MAX_SCREEN_X;
        rcRectdes.bottom = MAX_SCREEN_Y;

        screenTexture = Renderer::createEmptyTexture(MAX_SCREEN_X, MAX_SCREEN_Y, sf::Color(70, 70, 100));

        // Landschaft erzeugen

        Renderer::DrawString("Create landscape...", 5, 5, 2);
        rcRectdes.left = 0;
        rcRectdes.top = 0;
        rcRectdes.right = MAX_SCREEN_X;
        rcRectdes.bottom = MAX_SCREEN_Y;
        World::Compute(200, 600);

        Renderer::DrawString("Create sea...", 5, 35, 2);
        rcRectdes.left = 0;
        rcRectdes.top = 0;
        rcRectdes.right = MAX_SCREEN_X;
        rcRectdes.bottom = MAX_SCREEN_Y;
        World::CreateSea();

        Renderer::DrawString("Establish river flow...", 5, 65, 2);
        rcRectdes.left = 0;
        rcRectdes.top = 0;
        rcRectdes.right = MAX_SCREEN_X;
        rcRectdes.bottom = MAX_SCREEN_Y;
        World::CreateRiver();
        Math::CalcKoor();

        Renderer::DrawString("Plant trees...", 5, 95, 2);
        rcRectdes.left = 0;
        rcRectdes.top = 0;
        rcRectdes.right = MAX_SCREEN_X;
        rcRectdes.bottom = MAX_SCREEN_Y;
        World::CreateTrees(30);

        World::CreatePirateWreck();

        // Guy Position
        Guy.Pos.x = 1;
        Guy.Pos.y = MAX_TILESY / 2;
        Guy.ScreenPosition.x =
            (Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
             Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2;
        Guy.ScreenPosition.y =
            (Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
             Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2;

        Camera.x = Guy.ScreenPosition.x - static_cast<short>(rcGesamt.right / 2);
        Camera.y = Guy.ScreenPosition.y - static_cast<short>(rcGesamt.bottom / 2);

        Chance = 0;

        IsInBoat = false;

        if (!IsInBoat) {
            World::ToggleIsInBoat();
        }

        Tag = 1;
        Hours = 0;
        Minutes = 0;

        Spielzustand = State::INTRO;
        Guy.IsActive = false;
        Guy.AnimationState = GUY_SHIP;
        Guy.ActionNumber = 0;
        Guy.CurrentAction = Action::INTRO;
    }

    // Delete font surface
    rcRectdes.left = 0;
    rcRectdes.top = 0;
    rcRectdes.right = MAX_SCREEN_X;
    rcRectdes.bottom = MAX_SCREEN_Y;

    bool Anitmp = LAnimation;
    bool Entdeckttmp[MAX_TILES_X][MAX_TILESY];

    LAnimation = false;

    // Schatzvergraben und Schatzkarte malen
    for (y = 0; y < MAX_TILESY; y++)
        for (x = 0; x < MAX_TILES_X; x++) {
            Entdeckttmp[x][y] = Landscape[x][y].Discovered;
            Landscape[x][y].Discovered = true;
        }

    World::Generate(); // Einmal vor dem Schatz schon entdeckt malen
    World::Treasure();

    for (y = 0; y < MAX_TILESY; y++) for (x = 0; x < MAX_TILES_X; x++) {
            Landscape[x][y].Discovered = Entdeckttmp[x][y];
        }

    World::UpdateDiscovered();
    LAnimation = Anitmp;
    World::Generate(); // Und nochmal ohne das die Gegend entdeckt ist
    Guy.OriginalPosition = Guy.ScreenPosition;
}
} // namespace Game
