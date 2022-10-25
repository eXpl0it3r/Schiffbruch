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
    ofs.write(reinterpret_cast<char *>(&s_GameState), sizeof(s_GameState));
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
    ifs.read(reinterpret_cast<char *>(&s_GameState), sizeof(s_GameState));
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
    for (i = Tiles::GUY_LEFT; i <= Tiles::GUY_BELOW; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].AnimationPhaseCount = 4;
        Bmp[i].Speed = 30;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSGuyAni;
        Bmp[i].sourceRect.left = 7 * (i - Tiles::GUY_LEFT);
        Bmp[i].sourceRect.right = 7 + 7 * (i - Tiles::GUY_LEFT);
        Bmp[i].sourceRect.top = 0;
        Bmp[i].sourceRect.bottom = 18;
        Bmp[i].Width = 7;
        Bmp[i].Height = 18;
    }

    for (i = Tiles::GUY_SEARCH; i <= Tiles::GUY_SLINGSHOT; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSGuyAni;
    }

    Bmp[Tiles::GUY_SEARCH].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_SEARCH].Speed = 4;
    Bmp[Tiles::GUY_SEARCH].sourceRect.left = 28;
    Bmp[Tiles::GUY_SEARCH].sourceRect.right = 39;
    Bmp[Tiles::GUY_SEARCH].sourceRect.top = 0;
    Bmp[Tiles::GUY_SEARCH].sourceRect.bottom = 14;
    Bmp[Tiles::GUY_SEARCH].Width = 11;
    Bmp[Tiles::GUY_SEARCH].Height = 14;
    Bmp[Tiles::GUY_SEARCH].Sound = Sound::CRACKLE;

    Bmp[Tiles::GUY_EAT].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_EAT].Speed = 4;
    Bmp[Tiles::GUY_EAT].sourceRect.left = 39;
    Bmp[Tiles::GUY_EAT].sourceRect.right = 39 + 7;
    Bmp[Tiles::GUY_EAT].sourceRect.top = 0;
    Bmp[Tiles::GUY_EAT].sourceRect.bottom = 17;
    Bmp[Tiles::GUY_EAT].Width = 7;
    Bmp[Tiles::GUY_EAT].Height = 17;
    Bmp[Tiles::GUY_EAT].Sound = Sound::CRACKLE;

    Bmp[Tiles::GUY_DRINK].AnimationPhaseCount = 5;
    Bmp[Tiles::GUY_DRINK].Speed = 4;
    Bmp[Tiles::GUY_DRINK].sourceRect.left = 46;
    Bmp[Tiles::GUY_DRINK].sourceRect.right = 46 + 9;
    Bmp[Tiles::GUY_DRINK].sourceRect.top = 0;
    Bmp[Tiles::GUY_DRINK].sourceRect.bottom = 13;
    Bmp[Tiles::GUY_DRINK].Width = 9;
    Bmp[Tiles::GUY_DRINK].Height = 13;
    Bmp[Tiles::GUY_DRINK].Sound = Sound::DRINK;

    Bmp[Tiles::GUY_CHOP].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_CHOP].Speed = 6;
    Bmp[Tiles::GUY_CHOP].sourceRect.left = 55;
    Bmp[Tiles::GUY_CHOP].sourceRect.right = 55 + 15;
    Bmp[Tiles::GUY_CHOP].sourceRect.top = 0;
    Bmp[Tiles::GUY_CHOP].sourceRect.bottom = 19;
    Bmp[Tiles::GUY_CHOP].Width = 15;
    Bmp[Tiles::GUY_CHOP].Height = 19;
    Bmp[Tiles::GUY_CHOP].Sound = Sound::LOG;

    Bmp[Tiles::GUY_WAITING].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_WAITING].Speed = 2;
    Bmp[Tiles::GUY_WAITING].sourceRect.left = 70;
    Bmp[Tiles::GUY_WAITING].sourceRect.right = 70 + 7;
    Bmp[Tiles::GUY_WAITING].sourceRect.top = 0;
    Bmp[Tiles::GUY_WAITING].sourceRect.bottom = 18;
    Bmp[Tiles::GUY_WAITING].Width = 7;
    Bmp[Tiles::GUY_WAITING].Height = 18;

    Bmp[Tiles::GUY_FARMING].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_FARMING].Speed = 4;
    Bmp[Tiles::GUY_FARMING].sourceRect.left = 78;
    Bmp[Tiles::GUY_FARMING].sourceRect.right = 78 + 19;
    Bmp[Tiles::GUY_FARMING].sourceRect.top = 0;
    Bmp[Tiles::GUY_FARMING].sourceRect.bottom = 18;
    Bmp[Tiles::GUY_FARMING].Width = 19;
    Bmp[Tiles::GUY_FARMING].Height = 18;

    Bmp[Tiles::GUY_TIE_UP].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_TIE_UP].Speed = 1;
    Bmp[Tiles::GUY_TIE_UP].sourceRect.left = 97;
    Bmp[Tiles::GUY_TIE_UP].sourceRect.right = 97 + 8;
    Bmp[Tiles::GUY_TIE_UP].sourceRect.top = 0;
    Bmp[Tiles::GUY_TIE_UP].sourceRect.bottom = 18;
    Bmp[Tiles::GUY_TIE_UP].Width = 8;
    Bmp[Tiles::GUY_TIE_UP].Height = 18;

    Bmp[Tiles::GUY_TIE_DOWN].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_TIE_DOWN].Speed = 1;
    Bmp[Tiles::GUY_TIE_DOWN].sourceRect.left = 98;
    Bmp[Tiles::GUY_TIE_DOWN].sourceRect.right = 98 + 7;
    Bmp[Tiles::GUY_TIE_DOWN].sourceRect.top = 36;
    Bmp[Tiles::GUY_TIE_DOWN].sourceRect.bottom = 36 + 18;
    Bmp[Tiles::GUY_TIE_DOWN].Width = 7;
    Bmp[Tiles::GUY_TIE_DOWN].Height = 18;

    Bmp[Tiles::GUY_SLEEPING_TENT].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_SLEEPING_TENT].Speed = 1;
    Bmp[Tiles::GUY_SLEEPING_TENT].sourceRect.left = 105;
    Bmp[Tiles::GUY_SLEEPING_TENT].sourceRect.right = 105 + 20;
    Bmp[Tiles::GUY_SLEEPING_TENT].sourceRect.top = 54;
    Bmp[Tiles::GUY_SLEEPING_TENT].sourceRect.bottom = 54 + 10;
    Bmp[Tiles::GUY_SLEEPING_TENT].Width = 20;
    Bmp[Tiles::GUY_SLEEPING_TENT].Height = 10;
    Bmp[Tiles::GUY_SLEEPING_TENT].Sound = Sound::SNORE;

    Bmp[Tiles::GUY_SLEEPING].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_SLEEPING].Speed = 1;
    Bmp[Tiles::GUY_SLEEPING].sourceRect.left = 125;
    Bmp[Tiles::GUY_SLEEPING].sourceRect.right = 125 + 17;
    Bmp[Tiles::GUY_SLEEPING].sourceRect.top = 36;
    Bmp[Tiles::GUY_SLEEPING].sourceRect.bottom = 36 + 18;
    Bmp[Tiles::GUY_SLEEPING].Width = 17;
    Bmp[Tiles::GUY_SLEEPING].Height = 18;
    Bmp[Tiles::GUY_SLEEPING].Sound = Sound::SNORE;

    Bmp[Tiles::GUY_ENTER_TENT].AnimationPhaseCount = 3;
    Bmp[Tiles::GUY_ENTER_TENT].Speed = 4;
    Bmp[Tiles::GUY_ENTER_TENT].sourceRect.left = 105;
    Bmp[Tiles::GUY_ENTER_TENT].sourceRect.right = 105 + 7;
    Bmp[Tiles::GUY_ENTER_TENT].sourceRect.top = 0;
    Bmp[Tiles::GUY_ENTER_TENT].sourceRect.bottom = 0 + 18;
    Bmp[Tiles::GUY_ENTER_TENT].Width = 7;
    Bmp[Tiles::GUY_ENTER_TENT].Height = 18;

    Bmp[Tiles::GUY_LAYING_TENT].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_LAYING_TENT].Speed = 2;
    Bmp[Tiles::GUY_LAYING_TENT].sourceRect.left = 125;
    Bmp[Tiles::GUY_LAYING_TENT].sourceRect.right = 125 + 17;
    Bmp[Tiles::GUY_LAYING_TENT].sourceRect.top = 0;
    Bmp[Tiles::GUY_LAYING_TENT].sourceRect.bottom = 0 + 18;
    Bmp[Tiles::GUY_LAYING_TENT].Width = 17;
    Bmp[Tiles::GUY_LAYING_TENT].Height = 18;

    Bmp[Tiles::GUY_STAND_UP].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_STAND_UP].Speed = 2;
    Bmp[Tiles::GUY_STAND_UP].sourceRect.left = 142;
    Bmp[Tiles::GUY_STAND_UP].sourceRect.right = 142 + 9;
    Bmp[Tiles::GUY_STAND_UP].sourceRect.top = 0;
    Bmp[Tiles::GUY_STAND_UP].sourceRect.bottom = 0 + 18;
    Bmp[Tiles::GUY_STAND_UP].Width = 9;
    Bmp[Tiles::GUY_STAND_UP].Height = 18;

    Bmp[Tiles::GUY_FISHING_LEFT_1].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_FISHING_LEFT_1].Speed = 6;
    Bmp[Tiles::GUY_FISHING_LEFT_1].sourceRect.left = 151;
    Bmp[Tiles::GUY_FISHING_LEFT_1].sourceRect.right = 151 + 16;
    Bmp[Tiles::GUY_FISHING_LEFT_1].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_LEFT_1].sourceRect.bottom = 0 + 17;
    Bmp[Tiles::GUY_FISHING_LEFT_1].Width = 16;
    Bmp[Tiles::GUY_FISHING_LEFT_1].Height = 17;

    Bmp[Tiles::GUY_FISHING_ABOVE_1].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_FISHING_ABOVE_1].Speed = 6;
    Bmp[Tiles::GUY_FISHING_ABOVE_1].sourceRect.left = 167;
    Bmp[Tiles::GUY_FISHING_ABOVE_1].sourceRect.right = 167 + 16;
    Bmp[Tiles::GUY_FISHING_ABOVE_1].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_ABOVE_1].sourceRect.bottom = 0 + 17;
    Bmp[Tiles::GUY_FISHING_ABOVE_1].Width = 16;
    Bmp[Tiles::GUY_FISHING_ABOVE_1].Height = 17;

    Bmp[Tiles::GUY_FISHING_RIGHT_1].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_FISHING_RIGHT_1].Speed = 6;
    Bmp[Tiles::GUY_FISHING_RIGHT_1].sourceRect.left = 183;
    Bmp[Tiles::GUY_FISHING_RIGHT_1].sourceRect.right = 183 + 14;
    Bmp[Tiles::GUY_FISHING_RIGHT_1].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_RIGHT_1].sourceRect.bottom = 0 + 17;
    Bmp[Tiles::GUY_FISHING_RIGHT_1].Width = 14;
    Bmp[Tiles::GUY_FISHING_RIGHT_1].Height = 17;

    Bmp[Tiles::GUY_FISHING_BELOW_1].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_FISHING_BELOW_1].Speed = 6;
    Bmp[Tiles::GUY_FISHING_BELOW_1].sourceRect.left = 197;
    Bmp[Tiles::GUY_FISHING_BELOW_1].sourceRect.right = 197 + 14;
    Bmp[Tiles::GUY_FISHING_BELOW_1].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_BELOW_1].sourceRect.bottom = 0 + 17;
    Bmp[Tiles::GUY_FISHING_BELOW_1].Width = 14;
    Bmp[Tiles::GUY_FISHING_BELOW_1].Height = 17;

    Bmp[Tiles::GUY_FISHING_LEFT_2].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_FISHING_LEFT_2].Speed = 3;
    Bmp[Tiles::GUY_FISHING_LEFT_2].sourceRect.left = 211;
    Bmp[Tiles::GUY_FISHING_LEFT_2].sourceRect.right = 211 + 16;
    Bmp[Tiles::GUY_FISHING_LEFT_2].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_LEFT_2].sourceRect.bottom = 0 + 16;
    Bmp[Tiles::GUY_FISHING_LEFT_2].Width = 16;
    Bmp[Tiles::GUY_FISHING_LEFT_2].Height = 16;

    Bmp[Tiles::GUY_FISHING_ABOVE_2].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_FISHING_ABOVE_2].Speed = 3;
    Bmp[Tiles::GUY_FISHING_ABOVE_2].sourceRect.left = 227;
    Bmp[Tiles::GUY_FISHING_ABOVE_2].sourceRect.right = 227 + 16;
    Bmp[Tiles::GUY_FISHING_ABOVE_2].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_ABOVE_2].sourceRect.bottom = 0 + 16;
    Bmp[Tiles::GUY_FISHING_ABOVE_2].Width = 16;
    Bmp[Tiles::GUY_FISHING_ABOVE_2].Height = 16;

    Bmp[Tiles::GUY_FISHING_RIGHT_2].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_FISHING_RIGHT_2].Speed = 3;
    Bmp[Tiles::GUY_FISHING_RIGHT_2].sourceRect.left = 243;
    Bmp[Tiles::GUY_FISHING_RIGHT_2].sourceRect.right = 243 + 14;
    Bmp[Tiles::GUY_FISHING_RIGHT_2].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_RIGHT_2].sourceRect.bottom = 0 + 15;
    Bmp[Tiles::GUY_FISHING_RIGHT_2].Width = 14;
    Bmp[Tiles::GUY_FISHING_RIGHT_2].Height = 15;

    Bmp[Tiles::GUY_FISHING_BELOW_2].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_FISHING_BELOW_2].Speed = 3;
    Bmp[Tiles::GUY_FISHING_BELOW_2].sourceRect.left = 257;
    Bmp[Tiles::GUY_FISHING_BELOW_2].sourceRect.right = 257 + 14;
    Bmp[Tiles::GUY_FISHING_BELOW_2].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_BELOW_2].sourceRect.bottom = 0 + 15;
    Bmp[Tiles::GUY_FISHING_BELOW_2].Width = 14;
    Bmp[Tiles::GUY_FISHING_BELOW_2].Height = 15;

    Bmp[Tiles::GUY_FISHING_LEFT_3].AnimationPhaseCount = 3;
    Bmp[Tiles::GUY_FISHING_LEFT_3].Speed = 2;
    Bmp[Tiles::GUY_FISHING_LEFT_3].sourceRect.left = 271;
    Bmp[Tiles::GUY_FISHING_LEFT_3].sourceRect.right = 271 + 16;
    Bmp[Tiles::GUY_FISHING_LEFT_3].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_LEFT_3].sourceRect.bottom = 0 + 16;
    Bmp[Tiles::GUY_FISHING_LEFT_3].Width = 16;
    Bmp[Tiles::GUY_FISHING_LEFT_3].Height = 16;

    Bmp[Tiles::GUY_FISHING_ABOVE_3].AnimationPhaseCount = 3;
    Bmp[Tiles::GUY_FISHING_ABOVE_3].Speed = 2;
    Bmp[Tiles::GUY_FISHING_ABOVE_3].sourceRect.left = 285;
    Bmp[Tiles::GUY_FISHING_ABOVE_3].sourceRect.right = 285 + 16;
    Bmp[Tiles::GUY_FISHING_ABOVE_3].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_ABOVE_3].sourceRect.bottom = 0 + 16;
    Bmp[Tiles::GUY_FISHING_ABOVE_3].Width = 16;
    Bmp[Tiles::GUY_FISHING_ABOVE_3].Height = 16;

    Bmp[Tiles::GUY_FISHING_RIGHT_3].AnimationPhaseCount = 3;
    Bmp[Tiles::GUY_FISHING_RIGHT_3].Speed = 2;
    Bmp[Tiles::GUY_FISHING_RIGHT_3].sourceRect.left = 299;
    Bmp[Tiles::GUY_FISHING_RIGHT_3].sourceRect.right = 299 + 14;
    Bmp[Tiles::GUY_FISHING_RIGHT_3].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_RIGHT_3].sourceRect.bottom = 0 + 15;
    Bmp[Tiles::GUY_FISHING_RIGHT_3].Width = 14;
    Bmp[Tiles::GUY_FISHING_RIGHT_3].Height = 15;

    Bmp[Tiles::GUY_FISHING_BELOW_3].AnimationPhaseCount = 3;
    Bmp[Tiles::GUY_FISHING_BELOW_3].Speed = 2;
    Bmp[Tiles::GUY_FISHING_BELOW_3].sourceRect.left = 313;
    Bmp[Tiles::GUY_FISHING_BELOW_3].sourceRect.right = 313 + 14;
    Bmp[Tiles::GUY_FISHING_BELOW_3].sourceRect.top = 0;
    Bmp[Tiles::GUY_FISHING_BELOW_3].sourceRect.bottom = 0 + 15;
    Bmp[Tiles::GUY_FISHING_BELOW_3].Width = 14;
    Bmp[Tiles::GUY_FISHING_BELOW_3].Height = 15;

    Bmp[Tiles::GUY_BEAT].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_BEAT].Speed = 7;
    Bmp[Tiles::GUY_BEAT].sourceRect.left = 327;
    Bmp[Tiles::GUY_BEAT].sourceRect.right = 327 + 12;
    Bmp[Tiles::GUY_BEAT].sourceRect.top = 0;
    Bmp[Tiles::GUY_BEAT].sourceRect.bottom = 0 + 24;
    Bmp[Tiles::GUY_BEAT].Width = 12;
    Bmp[Tiles::GUY_BEAT].Height = 24;
    Bmp[Tiles::GUY_BEAT].Sound = Sound::HIT;

    Bmp[Tiles::GUY_BOAT_LEFT].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_BOAT_LEFT].Speed = 10;
    Bmp[Tiles::GUY_BOAT_LEFT].sourceRect.left = 339;
    Bmp[Tiles::GUY_BOAT_LEFT].sourceRect.right = 339 + 26;
    Bmp[Tiles::GUY_BOAT_LEFT].sourceRect.top = 0;
    Bmp[Tiles::GUY_BOAT_LEFT].sourceRect.bottom = 0 + 21;
    Bmp[Tiles::GUY_BOAT_LEFT].Width = 26;
    Bmp[Tiles::GUY_BOAT_LEFT].Height = 21;

    Bmp[Tiles::GUY_BOAT_ABOVE].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_BOAT_ABOVE].Speed = 10;
    Bmp[Tiles::GUY_BOAT_ABOVE].sourceRect.left = 365;
    Bmp[Tiles::GUY_BOAT_ABOVE].sourceRect.right = 365 + 26;
    Bmp[Tiles::GUY_BOAT_ABOVE].sourceRect.top = 0;
    Bmp[Tiles::GUY_BOAT_ABOVE].sourceRect.bottom = 0 + 21;
    Bmp[Tiles::GUY_BOAT_ABOVE].Width = 26;
    Bmp[Tiles::GUY_BOAT_ABOVE].Height = 21;

    Bmp[Tiles::GUY_BOAT_RIGHT].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_BOAT_RIGHT].Speed = 10;
    Bmp[Tiles::GUY_BOAT_RIGHT].sourceRect.left = 391;
    Bmp[Tiles::GUY_BOAT_RIGHT].sourceRect.right = 391 + 26;
    Bmp[Tiles::GUY_BOAT_RIGHT].sourceRect.top = 0;
    Bmp[Tiles::GUY_BOAT_RIGHT].sourceRect.bottom = 0 + 21;
    Bmp[Tiles::GUY_BOAT_RIGHT].Width = 26;
    Bmp[Tiles::GUY_BOAT_RIGHT].Height = 21;

    Bmp[Tiles::GUY_BOAT_BELOW].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_BOAT_BELOW].Speed = 10;
    Bmp[Tiles::GUY_BOAT_BELOW].sourceRect.left = 417;
    Bmp[Tiles::GUY_BOAT_BELOW].sourceRect.right = 417 + 26;
    Bmp[Tiles::GUY_BOAT_BELOW].sourceRect.top = 0;
    Bmp[Tiles::GUY_BOAT_BELOW].sourceRect.bottom = 0 + 21;
    Bmp[Tiles::GUY_BOAT_BELOW].Width = 26;
    Bmp[Tiles::GUY_BOAT_BELOW].Height = 21;

    Bmp[Tiles::GUY_BOAT_FISHING_1].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_BOAT_FISHING_1].Speed = 6;
    Bmp[Tiles::GUY_BOAT_FISHING_1].sourceRect.left = 443;
    Bmp[Tiles::GUY_BOAT_FISHING_1].sourceRect.right = 443 + 26;
    Bmp[Tiles::GUY_BOAT_FISHING_1].sourceRect.top = 0;
    Bmp[Tiles::GUY_BOAT_FISHING_1].sourceRect.bottom = 0 + 25;
    Bmp[Tiles::GUY_BOAT_FISHING_1].Width = 26;
    Bmp[Tiles::GUY_BOAT_FISHING_1].Height = 25;

    Bmp[Tiles::GUY_BOAT_FISHING_2].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_BOAT_FISHING_2].Speed = 3;
    Bmp[Tiles::GUY_BOAT_FISHING_2].sourceRect.left = 469;
    Bmp[Tiles::GUY_BOAT_FISHING_2].sourceRect.right = 469 + 26;
    Bmp[Tiles::GUY_BOAT_FISHING_2].sourceRect.top = 0;
    Bmp[Tiles::GUY_BOAT_FISHING_2].sourceRect.bottom = 0 + 25;
    Bmp[Tiles::GUY_BOAT_FISHING_2].Width = 26;
    Bmp[Tiles::GUY_BOAT_FISHING_2].Height = 25;

    Bmp[Tiles::GUY_BOAT_FISHING_3].AnimationPhaseCount = 3;
    Bmp[Tiles::GUY_BOAT_FISHING_3].Speed = 2;
    Bmp[Tiles::GUY_BOAT_FISHING_3].sourceRect.left = 495;
    Bmp[Tiles::GUY_BOAT_FISHING_3].sourceRect.right = 495 + 26;
    Bmp[Tiles::GUY_BOAT_FISHING_3].sourceRect.top = 0;
    Bmp[Tiles::GUY_BOAT_FISHING_3].sourceRect.bottom = 0 + 25;
    Bmp[Tiles::GUY_BOAT_FISHING_3].Width = 26;
    Bmp[Tiles::GUY_BOAT_FISHING_3].Height = 25;

    Bmp[Tiles::GUY_DIVING_1].AnimationPhaseCount = 5;
    Bmp[Tiles::GUY_DIVING_1].Speed = 5;
    Bmp[Tiles::GUY_DIVING_1].sourceRect.left = 521;
    Bmp[Tiles::GUY_DIVING_1].sourceRect.right = 521 + 26;
    Bmp[Tiles::GUY_DIVING_1].sourceRect.top = 0;
    Bmp[Tiles::GUY_DIVING_1].sourceRect.bottom = 0 + 27;
    Bmp[Tiles::GUY_DIVING_1].Width = 26;
    Bmp[Tiles::GUY_DIVING_1].Height = 27;

    Bmp[Tiles::GUY_DIVING_2].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_DIVING_2].Speed = 3;
    Bmp[Tiles::GUY_DIVING_2].sourceRect.left = 547;
    Bmp[Tiles::GUY_DIVING_2].sourceRect.right = 547 + 26;
    Bmp[Tiles::GUY_DIVING_2].sourceRect.top = 0;
    Bmp[Tiles::GUY_DIVING_2].sourceRect.bottom = 0 + 17;
    Bmp[Tiles::GUY_DIVING_2].Width = 26;
    Bmp[Tiles::GUY_DIVING_2].Height = 17;

    Bmp[Tiles::GUY_DIVING_3].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_DIVING_3].Speed = 2;
    Bmp[Tiles::GUY_DIVING_3].sourceRect.left = 573;
    Bmp[Tiles::GUY_DIVING_3].sourceRect.right = 573 + 26;
    Bmp[Tiles::GUY_DIVING_3].sourceRect.top = 0;
    Bmp[Tiles::GUY_DIVING_3].sourceRect.bottom = 0 + 17;
    Bmp[Tiles::GUY_DIVING_3].Width = 26;
    Bmp[Tiles::GUY_DIVING_3].Height = 17;

    Bmp[Tiles::GUY_HAMMER_1].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_HAMMER_1].Speed = 4;
    Bmp[Tiles::GUY_HAMMER_1].sourceRect.left = 599;
    Bmp[Tiles::GUY_HAMMER_1].sourceRect.right = 599 + 9;
    Bmp[Tiles::GUY_HAMMER_1].sourceRect.top = 0;
    Bmp[Tiles::GUY_HAMMER_1].sourceRect.bottom = 0 + 18;
    Bmp[Tiles::GUY_HAMMER_1].Width = 9;
    Bmp[Tiles::GUY_HAMMER_1].Height = 18;
    Bmp[Tiles::GUY_HAMMER_1].Sound = Sound::HAMMER;

    Bmp[Tiles::GUY_CLIMBING_1].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_CLIMBING_1].Speed = 2;
    Bmp[Tiles::GUY_CLIMBING_1].sourceRect.left = 608;
    Bmp[Tiles::GUY_CLIMBING_1].sourceRect.right = 608 + 7;
    Bmp[Tiles::GUY_CLIMBING_1].sourceRect.top = 0;
    Bmp[Tiles::GUY_CLIMBING_1].sourceRect.bottom = 0 + 34;
    Bmp[Tiles::GUY_CLIMBING_1].Width = 7;
    Bmp[Tiles::GUY_CLIMBING_1].Height = 34;

    Bmp[Tiles::GUY_CLIMBING_2].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_CLIMBING_2].Speed = 2;
    Bmp[Tiles::GUY_CLIMBING_2].sourceRect.left = 615;
    Bmp[Tiles::GUY_CLIMBING_2].sourceRect.right = 615 + 7;
    Bmp[Tiles::GUY_CLIMBING_2].sourceRect.top = 0;
    Bmp[Tiles::GUY_CLIMBING_2].sourceRect.bottom = 0 + 34;
    Bmp[Tiles::GUY_CLIMBING_2].Width = 7;
    Bmp[Tiles::GUY_CLIMBING_2].Height = 34;

    Bmp[Tiles::GUY_HAMMER_2].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_HAMMER_2].Speed = 4;
    Bmp[Tiles::GUY_HAMMER_2].sourceRect.left = 622;
    Bmp[Tiles::GUY_HAMMER_2].sourceRect.right = 622 + 7;
    Bmp[Tiles::GUY_HAMMER_2].sourceRect.top = 0;
    Bmp[Tiles::GUY_HAMMER_2].sourceRect.bottom = 0 + 34;
    Bmp[Tiles::GUY_HAMMER_2].Width = 7;
    Bmp[Tiles::GUY_HAMMER_2].Height = 34;
    Bmp[Tiles::GUY_HAMMER_2].Sound = Sound::HAMMER;

    Bmp[Tiles::GUY_ENTER_HOUSE].AnimationPhaseCount = 3;
    Bmp[Tiles::GUY_ENTER_HOUSE].Speed = 2;
    Bmp[Tiles::GUY_ENTER_HOUSE].sourceRect.left = 631;
    Bmp[Tiles::GUY_ENTER_HOUSE].sourceRect.right = 631 + 9;
    Bmp[Tiles::GUY_ENTER_HOUSE].sourceRect.top = 0;
    Bmp[Tiles::GUY_ENTER_HOUSE].sourceRect.bottom = 0 + 34;
    Bmp[Tiles::GUY_ENTER_HOUSE].Width = 9;
    Bmp[Tiles::GUY_ENTER_HOUSE].Height = 34;

    Bmp[Tiles::GUY_SLEEP_HOUSE].AnimationPhaseCount = 2;
    Bmp[Tiles::GUY_SLEEP_HOUSE].Speed = 1;
    Bmp[Tiles::GUY_SLEEP_HOUSE].sourceRect.left = 640;
    Bmp[Tiles::GUY_SLEEP_HOUSE].sourceRect.right = 640 + 10;
    Bmp[Tiles::GUY_SLEEP_HOUSE].sourceRect.top = 0;
    Bmp[Tiles::GUY_SLEEP_HOUSE].sourceRect.bottom = 0 + 34;
    Bmp[Tiles::GUY_SLEEP_HOUSE].Width = 10;
    Bmp[Tiles::GUY_SLEEP_HOUSE].Height = 34;
    Bmp[Tiles::GUY_SLEEP_HOUSE].Sound = Sound::SNORE;

    Bmp[Tiles::GUY_EXIT_HOUSE].AnimationPhaseCount = 3;
    Bmp[Tiles::GUY_EXIT_HOUSE].Speed = 2;
    Bmp[Tiles::GUY_EXIT_HOUSE].sourceRect.left = 650;
    Bmp[Tiles::GUY_EXIT_HOUSE].sourceRect.right = 650 + 9;
    Bmp[Tiles::GUY_EXIT_HOUSE].sourceRect.top = 0;
    Bmp[Tiles::GUY_EXIT_HOUSE].sourceRect.bottom = 0 + 34;
    Bmp[Tiles::GUY_EXIT_HOUSE].Width = 9;
    Bmp[Tiles::GUY_EXIT_HOUSE].Height = 34;

    Bmp[Tiles::GUY_SET_FIRE].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_SET_FIRE].Speed = 5;
    Bmp[Tiles::GUY_SET_FIRE].sourceRect.left = 659;
    Bmp[Tiles::GUY_SET_FIRE].sourceRect.right = 659 + 19;
    Bmp[Tiles::GUY_SET_FIRE].sourceRect.top = 0;
    Bmp[Tiles::GUY_SET_FIRE].sourceRect.bottom = 0 + 18;
    Bmp[Tiles::GUY_SET_FIRE].Width = 19;
    Bmp[Tiles::GUY_SET_FIRE].Height = 18;

    Bmp[Tiles::GUY_LOOK_OUT].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_LOOK_OUT].Speed = 1;
    Bmp[Tiles::GUY_LOOK_OUT].sourceRect.left = 678;
    Bmp[Tiles::GUY_LOOK_OUT].sourceRect.right = 678 + 10;
    Bmp[Tiles::GUY_LOOK_OUT].sourceRect.top = 0;
    Bmp[Tiles::GUY_LOOK_OUT].sourceRect.bottom = 0 + 18;
    Bmp[Tiles::GUY_LOOK_OUT].Width = 10;
    Bmp[Tiles::GUY_LOOK_OUT].Height = 18;

    Bmp[Tiles::GUY_SHOVEL].AnimationPhaseCount = 10;
    Bmp[Tiles::GUY_SHOVEL].Speed = 3;
    Bmp[Tiles::GUY_SHOVEL].sourceRect.left = 688;
    Bmp[Tiles::GUY_SHOVEL].sourceRect.right = 688 + 17;
    Bmp[Tiles::GUY_SHOVEL].sourceRect.top = 0;
    Bmp[Tiles::GUY_SHOVEL].sourceRect.bottom = 0 + 19;
    Bmp[Tiles::GUY_SHOVEL].Width = 17;
    Bmp[Tiles::GUY_SHOVEL].Height = 19;
    Bmp[Tiles::GUY_SHOVEL].Sound = Sound::DIG;

    Bmp[Tiles::GUY_SHIP].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_SHIP].Speed = 10;
    Bmp[Tiles::GUY_SHIP].sourceRect.left = 297;
    Bmp[Tiles::GUY_SHIP].sourceRect.right = 297 + 48;
    Bmp[Tiles::GUY_SHIP].sourceRect.top = 0;
    Bmp[Tiles::GUY_SHIP].sourceRect.bottom = 0 + 38;
    Bmp[Tiles::GUY_SHIP].Width = 48;
    Bmp[Tiles::GUY_SHIP].Height = 38;
    Bmp[Tiles::GUY_SHIP].Surface = lpDDSBau;
    Bmp[Tiles::GUY_SHIP].Sound = Sound::STORM;

    Bmp[Tiles::GUY_SHIP_DOWN].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_SHIP_DOWN].Speed = 3;
    Bmp[Tiles::GUY_SHIP_DOWN].sourceRect.left = 345;
    Bmp[Tiles::GUY_SHIP_DOWN].sourceRect.right = 345 + 46;
    Bmp[Tiles::GUY_SHIP_DOWN].sourceRect.top = 0;
    Bmp[Tiles::GUY_SHIP_DOWN].sourceRect.bottom = 0 + 40;
    Bmp[Tiles::GUY_SHIP_DOWN].Width = 46;
    Bmp[Tiles::GUY_SHIP_DOWN].Height = 40;
    Bmp[Tiles::GUY_SHIP_DOWN].Surface = lpDDSBau;
    Bmp[Tiles::GUY_SHIP_DOWN].Sound = Sound::SPLAT;

    Bmp[Tiles::GUY_SWIM].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_SWIM].Speed = 10;
    Bmp[Tiles::GUY_SWIM].sourceRect.left = 705;
    Bmp[Tiles::GUY_SWIM].sourceRect.right = 705 + 15;
    Bmp[Tiles::GUY_SWIM].sourceRect.top = 0;
    Bmp[Tiles::GUY_SWIM].sourceRect.bottom = 0 + 9;
    Bmp[Tiles::GUY_SWIM].Width = 15;
    Bmp[Tiles::GUY_SWIM].Height = 9;
    Bmp[Tiles::GUY_SWIM].Sound = Sound::SWIM;

    Bmp[Tiles::GUY_DEAD].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_DEAD].Speed = 1;
    Bmp[Tiles::GUY_DEAD].sourceRect.left = 743;
    Bmp[Tiles::GUY_DEAD].sourceRect.right = 743 + 16;
    Bmp[Tiles::GUY_DEAD].sourceRect.top = 0;
    Bmp[Tiles::GUY_DEAD].sourceRect.bottom = 0 + 10;
    Bmp[Tiles::GUY_DEAD].Width = 16;
    Bmp[Tiles::GUY_DEAD].Height = 10;

    Bmp[Tiles::GUY_BOAT_DEAD].AnimationPhaseCount = 6;
    Bmp[Tiles::GUY_BOAT_DEAD].Speed = 1;
    Bmp[Tiles::GUY_BOAT_DEAD].sourceRect.left = 759;
    Bmp[Tiles::GUY_BOAT_DEAD].sourceRect.right = 759 + 26;
    Bmp[Tiles::GUY_BOAT_DEAD].sourceRect.top = 0;
    Bmp[Tiles::GUY_BOAT_DEAD].sourceRect.bottom = 0 + 18;
    Bmp[Tiles::GUY_BOAT_DEAD].Width = 26;
    Bmp[Tiles::GUY_BOAT_DEAD].Height = 18;

    Bmp[Tiles::GUY_BOAT_WAITING].AnimationPhaseCount = 4;
    Bmp[Tiles::GUY_BOAT_WAITING].Speed = 2;
    Bmp[Tiles::GUY_BOAT_WAITING].sourceRect.left = 0;
    Bmp[Tiles::GUY_BOAT_WAITING].sourceRect.right = 0 + 26;
    Bmp[Tiles::GUY_BOAT_WAITING].sourceRect.top = 72;
    Bmp[Tiles::GUY_BOAT_WAITING].sourceRect.bottom = 72 + 18;
    Bmp[Tiles::GUY_BOAT_WAITING].Width = 26;
    Bmp[Tiles::GUY_BOAT_WAITING].Height = 20;

    Bmp[Tiles::GUY_SLINGSHOT].AnimationPhaseCount = 5;
    Bmp[Tiles::GUY_SLINGSHOT].Speed = 4;
    Bmp[Tiles::GUY_SLINGSHOT].sourceRect.left = 720;
    Bmp[Tiles::GUY_SLINGSHOT].sourceRect.right = 720 + 23;
    Bmp[Tiles::GUY_SLINGSHOT].sourceRect.top = 0;
    Bmp[Tiles::GUY_SLINGSHOT].sourceRect.bottom = 0 + 20;
    Bmp[Tiles::GUY_SLINGSHOT].Width = 23;
    Bmp[Tiles::GUY_SLINGSHOT].Height = 20;

    // Cursor
    for (i = Tiles::CURSOR_ARROW; i <= Tiles::CURSOR_CLOCK; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].AnimationPhaseCount = 1;
        Bmp[i].Speed = 0;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSCursor;
        Bmp[i].sourceRect.left = (i - Tiles::CURSOR_ARROW) * 18;
        Bmp[i].sourceRect.top = 0;
        Bmp[i].sourceRect.right = Bmp[i].sourceRect.left + 18;
        Bmp[i].sourceRect.bottom = 18;
        Bmp[i].Width = 18;
        Bmp[i].Height = 18;
    }

    // Landschaftsanimationen
    for (i = Tiles::SEA_WAVES; i <= Tiles::FLOODGATE_6; i++) {
        Bmp[i].IsAnimationRunning = true;
        Bmp[i].AnimationPhaseCount = 7;
        Bmp[i].Speed = 7;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSAnimation;
        Bmp[i].Sound = Sound::RIVER;
    }

    Bmp[Tiles::SEA_WAVES].Speed = 3;
    Bmp[Tiles::SEA_WAVES].Width = 28;
    Bmp[Tiles::SEA_WAVES].Height = 13;
    Bmp[Tiles::SEA_WAVES].sourceRect.left = 0;
    Bmp[Tiles::SEA_WAVES].sourceRect.right = Bmp[Tiles::SEA_WAVES].sourceRect.left + Bmp[Tiles::SEA_WAVES].Width;
    Bmp[Tiles::SEA_WAVES].sourceRect.top = 0;
    Bmp[Tiles::SEA_WAVES].sourceRect.bottom = Bmp[Tiles::SEA_WAVES].sourceRect.top + Bmp[Tiles::SEA_WAVES].Height;
    Bmp[Tiles::SEA_WAVES].targetRect.left = 9;
    Bmp[Tiles::SEA_WAVES].targetRect.right = Bmp[Tiles::SEA_WAVES].targetRect.left + Bmp[Tiles::SEA_WAVES].Width;
    Bmp[Tiles::SEA_WAVES].targetRect.top = 23;
    Bmp[Tiles::SEA_WAVES].targetRect.bottom = Bmp[Tiles::SEA_WAVES].targetRect.top + Bmp[Tiles::SEA_WAVES].Height;
    Bmp[Tiles::SEA_WAVES].Sound = Sound::SURF;

    Bmp[Tiles::RIVER_1].Width = 34;
    Bmp[Tiles::RIVER_1].Height = 8;
    Bmp[Tiles::RIVER_1].sourceRect.left = 28;
    Bmp[Tiles::RIVER_1].sourceRect.right = Bmp[Tiles::RIVER_1].sourceRect.left + Bmp[Tiles::RIVER_1].Width;
    Bmp[Tiles::RIVER_1].sourceRect.top = 0;
    Bmp[Tiles::RIVER_1].sourceRect.bottom = Bmp[Tiles::RIVER_1].sourceRect.top + Bmp[Tiles::RIVER_1].Height;
    Bmp[Tiles::RIVER_1].targetRect.left = 10;
    Bmp[Tiles::RIVER_1].targetRect.right = Bmp[Tiles::RIVER_1].targetRect.left + Bmp[Tiles::RIVER_1].Width;
    Bmp[Tiles::RIVER_1].targetRect.top = 18;
    Bmp[Tiles::RIVER_1].targetRect.bottom = Bmp[Tiles::RIVER_1].targetRect.top + Bmp[Tiles::RIVER_1].Height;

    Bmp[Tiles::RIVER_2].Width = 34;
    Bmp[Tiles::RIVER_2].Height = 8;
    Bmp[Tiles::RIVER_2].sourceRect.left = 62;
    Bmp[Tiles::RIVER_2].sourceRect.right = Bmp[Tiles::RIVER_2].sourceRect.left + Bmp[Tiles::RIVER_2].Width;
    Bmp[Tiles::RIVER_2].sourceRect.top = 0;
    Bmp[Tiles::RIVER_2].sourceRect.bottom = Bmp[Tiles::RIVER_2].sourceRect.top + Bmp[Tiles::RIVER_2].Height;
    Bmp[Tiles::RIVER_2].targetRect.left = 10;
    Bmp[Tiles::RIVER_2].targetRect.right = Bmp[Tiles::RIVER_2].targetRect.left + Bmp[Tiles::RIVER_2].Width;
    Bmp[Tiles::RIVER_2].targetRect.top = 19;
    Bmp[Tiles::RIVER_2].targetRect.bottom = Bmp[Tiles::RIVER_2].targetRect.top + Bmp[Tiles::RIVER_2].Height;

    Bmp[Tiles::RIVER_3].Width = 34;
    Bmp[Tiles::RIVER_3].Height = 34;
    Bmp[Tiles::RIVER_3].sourceRect.left = 96;
    Bmp[Tiles::RIVER_3].sourceRect.right = Bmp[Tiles::RIVER_3].sourceRect.left + Bmp[Tiles::RIVER_3].Width;
    Bmp[Tiles::RIVER_3].sourceRect.top = 0;
    Bmp[Tiles::RIVER_3].sourceRect.bottom = Bmp[Tiles::RIVER_3].sourceRect.top + Bmp[Tiles::RIVER_3].Height;
    Bmp[Tiles::RIVER_3].targetRect.left = 10;
    Bmp[Tiles::RIVER_3].targetRect.right = Bmp[Tiles::RIVER_3].targetRect.left + Bmp[Tiles::RIVER_3].Width;
    Bmp[Tiles::RIVER_3].targetRect.top = 5;
    Bmp[Tiles::RIVER_3].targetRect.bottom = Bmp[Tiles::RIVER_3].targetRect.top + Bmp[Tiles::RIVER_3].Height;

    Bmp[Tiles::RIVER_4].Width = 34;
    Bmp[Tiles::RIVER_4].Height = 34;
    Bmp[Tiles::RIVER_4].sourceRect.left = 130;
    Bmp[Tiles::RIVER_4].sourceRect.right = Bmp[Tiles::RIVER_4].sourceRect.left + Bmp[Tiles::RIVER_4].Width;
    Bmp[Tiles::RIVER_4].sourceRect.top = 0;
    Bmp[Tiles::RIVER_4].sourceRect.bottom = Bmp[Tiles::RIVER_4].sourceRect.top + Bmp[Tiles::RIVER_4].Height;
    Bmp[Tiles::RIVER_4].targetRect.left = 10;
    Bmp[Tiles::RIVER_4].targetRect.right = Bmp[Tiles::RIVER_4].targetRect.left + Bmp[Tiles::RIVER_4].Width;
    Bmp[Tiles::RIVER_4].targetRect.top = 5;
    Bmp[Tiles::RIVER_4].targetRect.bottom = Bmp[Tiles::RIVER_4].targetRect.top + Bmp[Tiles::RIVER_4].Height;

    Bmp[Tiles::RIVER_5].Width = 34;
    Bmp[Tiles::RIVER_5].Height = 18;
    Bmp[Tiles::RIVER_5].sourceRect.left = 164;
    Bmp[Tiles::RIVER_5].sourceRect.right = Bmp[Tiles::RIVER_5].sourceRect.left + Bmp[Tiles::RIVER_5].Width;
    Bmp[Tiles::RIVER_5].sourceRect.top = 0;
    Bmp[Tiles::RIVER_5].sourceRect.bottom = Bmp[Tiles::RIVER_5].sourceRect.top + Bmp[Tiles::RIVER_5].Height;
    Bmp[Tiles::RIVER_5].targetRect.left = 10;
    Bmp[Tiles::RIVER_5].targetRect.right = Bmp[Tiles::RIVER_5].targetRect.left + Bmp[Tiles::RIVER_5].Width;
    Bmp[Tiles::RIVER_5].targetRect.top = 21;
    Bmp[Tiles::RIVER_5].targetRect.bottom = Bmp[Tiles::RIVER_5].targetRect.top + Bmp[Tiles::RIVER_5].Height;

    Bmp[Tiles::RIVER_6].Width = 34;
    Bmp[Tiles::RIVER_6].Height = 18;
    Bmp[Tiles::RIVER_6].sourceRect.left = 198;
    Bmp[Tiles::RIVER_6].sourceRect.right = Bmp[Tiles::RIVER_6].sourceRect.left + Bmp[Tiles::RIVER_6].Width;
    Bmp[Tiles::RIVER_6].sourceRect.top = 0;
    Bmp[Tiles::RIVER_6].sourceRect.bottom = Bmp[Tiles::RIVER_6].sourceRect.top + Bmp[Tiles::RIVER_6].Height;
    Bmp[Tiles::RIVER_6].targetRect.left = 10;
    Bmp[Tiles::RIVER_6].targetRect.right = Bmp[Tiles::RIVER_6].targetRect.left + Bmp[Tiles::RIVER_6].Width;
    Bmp[Tiles::RIVER_6].targetRect.top = 21;
    Bmp[Tiles::RIVER_6].targetRect.bottom = Bmp[Tiles::RIVER_6].targetRect.top + Bmp[Tiles::RIVER_6].Height;

    Bmp[Tiles::RIVER_7].Width = 18;
    Bmp[Tiles::RIVER_7].Height = 18;
    Bmp[Tiles::RIVER_7].sourceRect.left = 232;
    Bmp[Tiles::RIVER_7].sourceRect.right = Bmp[Tiles::RIVER_7].sourceRect.left + Bmp[Tiles::RIVER_7].Width;
    Bmp[Tiles::RIVER_7].sourceRect.top = 0;
    Bmp[Tiles::RIVER_7].sourceRect.bottom = Bmp[Tiles::RIVER_7].sourceRect.top + Bmp[Tiles::RIVER_7].Height;
    Bmp[Tiles::RIVER_7].targetRect.left = 10;
    Bmp[Tiles::RIVER_7].targetRect.right = Bmp[Tiles::RIVER_7].targetRect.left + Bmp[Tiles::RIVER_7].Width;
    Bmp[Tiles::RIVER_7].targetRect.top = 21;
    Bmp[Tiles::RIVER_7].targetRect.bottom = Bmp[Tiles::RIVER_7].targetRect.top + Bmp[Tiles::RIVER_7].Height;

    Bmp[Tiles::RIVER_8].Width = 18;
    Bmp[Tiles::RIVER_8].Height = 18;
    Bmp[Tiles::RIVER_8].sourceRect.left = 250;
    Bmp[Tiles::RIVER_8].sourceRect.right = Bmp[Tiles::RIVER_8].sourceRect.left + Bmp[Tiles::RIVER_8].Width;
    Bmp[Tiles::RIVER_8].sourceRect.top = 0;
    Bmp[Tiles::RIVER_8].sourceRect.bottom = Bmp[Tiles::RIVER_8].sourceRect.top + Bmp[Tiles::RIVER_8].Height;
    Bmp[Tiles::RIVER_8].targetRect.left = 26;
    Bmp[Tiles::RIVER_8].targetRect.right = Bmp[Tiles::RIVER_8].targetRect.left + Bmp[Tiles::RIVER_8].Width;
    Bmp[Tiles::RIVER_8].targetRect.top = 21;
    Bmp[Tiles::RIVER_8].targetRect.bottom = Bmp[Tiles::RIVER_8].targetRect.top + Bmp[Tiles::RIVER_8].Height;

    Bmp[Tiles::RIVER_9].Width = 34;
    Bmp[Tiles::RIVER_9].Height = 15;
    Bmp[Tiles::RIVER_9].sourceRect.left = 268;
    Bmp[Tiles::RIVER_9].sourceRect.right = Bmp[Tiles::RIVER_9].sourceRect.left + Bmp[Tiles::RIVER_9].Width;
    Bmp[Tiles::RIVER_9].sourceRect.top = 0;
    Bmp[Tiles::RIVER_9].sourceRect.bottom = Bmp[Tiles::RIVER_9].sourceRect.top + Bmp[Tiles::RIVER_9].Height;
    Bmp[Tiles::RIVER_9].targetRect.left = 10;
    Bmp[Tiles::RIVER_9].targetRect.right = Bmp[Tiles::RIVER_9].targetRect.left + Bmp[Tiles::RIVER_9].Width;
    Bmp[Tiles::RIVER_9].targetRect.top = 20;
    Bmp[Tiles::RIVER_9].targetRect.bottom = Bmp[Tiles::RIVER_9].targetRect.top + Bmp[Tiles::RIVER_9].Height;

    Bmp[Tiles::RIVER_10].Width = 34;
    Bmp[Tiles::RIVER_10].Height = 13;
    Bmp[Tiles::RIVER_10].sourceRect.left = 302;
    Bmp[Tiles::RIVER_10].sourceRect.right = Bmp[Tiles::RIVER_10].sourceRect.left + Bmp[Tiles::RIVER_10].Width;
    Bmp[Tiles::RIVER_10].sourceRect.top = 0;
    Bmp[Tiles::RIVER_10].sourceRect.bottom = Bmp[Tiles::RIVER_10].sourceRect.top + Bmp[Tiles::RIVER_10].Height;
    Bmp[Tiles::RIVER_10].targetRect.left = 10;
    Bmp[Tiles::RIVER_10].targetRect.right = Bmp[Tiles::RIVER_10].targetRect.left + Bmp[Tiles::RIVER_10].Width;
    Bmp[Tiles::RIVER_10].targetRect.top = 26;
    Bmp[Tiles::RIVER_10].targetRect.bottom = Bmp[Tiles::RIVER_10].targetRect.top + Bmp[Tiles::RIVER_10].Height;

    Bmp[Tiles::RIVER_END_1].Width = 42;
    Bmp[Tiles::RIVER_END_1].Height = 22;
    Bmp[Tiles::RIVER_END_1].sourceRect.left = 336;
    Bmp[Tiles::RIVER_END_1].sourceRect.right = Bmp[Tiles::RIVER_END_1].sourceRect.left + Bmp[Tiles::RIVER_END_1].Width;
    Bmp[Tiles::RIVER_END_1].sourceRect.top = 0;
    Bmp[Tiles::RIVER_END_1].sourceRect.bottom = Bmp[Tiles::RIVER_END_1].sourceRect.top + Bmp[Tiles::RIVER_END_1].Height;
    Bmp[Tiles::RIVER_END_1].targetRect.left = 2;
    Bmp[Tiles::RIVER_END_1].targetRect.right = Bmp[Tiles::RIVER_END_1].targetRect.left + Bmp[Tiles::RIVER_END_1].Width;
    Bmp[Tiles::RIVER_END_1].targetRect.top = 17;
    Bmp[Tiles::RIVER_END_1].targetRect.bottom = Bmp[Tiles::RIVER_END_1].targetRect.top + Bmp[Tiles::RIVER_END_1].Height;

    Bmp[Tiles::RIVER_END_2].Width = 40;
    Bmp[Tiles::RIVER_END_2].Height = 22;
    Bmp[Tiles::RIVER_END_2].sourceRect.left = 378;
    Bmp[Tiles::RIVER_END_2].sourceRect.right = Bmp[Tiles::RIVER_END_2].sourceRect.left + Bmp[Tiles::RIVER_END_2].Width;
    Bmp[Tiles::RIVER_END_2].sourceRect.top = 0;
    Bmp[Tiles::RIVER_END_2].sourceRect.bottom = Bmp[Tiles::RIVER_END_2].sourceRect.top + Bmp[Tiles::RIVER_END_2].Height;
    Bmp[Tiles::RIVER_END_2].targetRect.left = 10;
    Bmp[Tiles::RIVER_END_2].targetRect.right = Bmp[Tiles::RIVER_END_2].targetRect.left + Bmp[Tiles::RIVER_END_2].Width;
    Bmp[Tiles::RIVER_END_2].targetRect.top = 17;
    Bmp[Tiles::RIVER_END_2].targetRect.bottom = Bmp[Tiles::RIVER_END_2].targetRect.top + Bmp[Tiles::RIVER_END_2].Height;

    Bmp[Tiles::RIVER_END_3].Width = 40;
    Bmp[Tiles::RIVER_END_3].Height = 22;
    Bmp[Tiles::RIVER_END_3].sourceRect.left = 418;
    Bmp[Tiles::RIVER_END_3].sourceRect.right = Bmp[Tiles::RIVER_END_3].sourceRect.left + Bmp[Tiles::RIVER_END_3].Width;
    Bmp[Tiles::RIVER_END_3].sourceRect.top = 0;
    Bmp[Tiles::RIVER_END_3].sourceRect.bottom = Bmp[Tiles::RIVER_END_3].sourceRect.top + Bmp[Tiles::RIVER_END_3].Height;
    Bmp[Tiles::RIVER_END_3].targetRect.left = 10;
    Bmp[Tiles::RIVER_END_3].targetRect.right = Bmp[Tiles::RIVER_END_3].targetRect.left + Bmp[Tiles::RIVER_END_3].Width;
    Bmp[Tiles::RIVER_END_3].targetRect.top = 21;
    Bmp[Tiles::RIVER_END_3].targetRect.bottom = Bmp[Tiles::RIVER_END_3].targetRect.top + Bmp[Tiles::RIVER_END_3].Height;

    Bmp[Tiles::RIVER_END_4].Width = 42;
    Bmp[Tiles::RIVER_END_4].Height = 22;
    Bmp[Tiles::RIVER_END_4].sourceRect.left = 458;
    Bmp[Tiles::RIVER_END_4].sourceRect.right = Bmp[Tiles::RIVER_END_4].sourceRect.left + Bmp[Tiles::RIVER_END_4].Width;
    Bmp[Tiles::RIVER_END_4].sourceRect.top = 0;
    Bmp[Tiles::RIVER_END_4].sourceRect.bottom = Bmp[Tiles::RIVER_END_4].sourceRect.top + Bmp[Tiles::RIVER_END_4].Height;
    Bmp[Tiles::RIVER_END_4].targetRect.left = 2;
    Bmp[Tiles::RIVER_END_4].targetRect.right = Bmp[Tiles::RIVER_END_4].targetRect.left + Bmp[Tiles::RIVER_END_4].Width;
    Bmp[Tiles::RIVER_END_4].targetRect.top = 21;
    Bmp[Tiles::RIVER_END_4].targetRect.bottom = Bmp[Tiles::RIVER_END_4].targetRect.top + Bmp[Tiles::RIVER_END_4].Height;

    Bmp[Tiles::RIVER_START_1].Width = 30;
    Bmp[Tiles::RIVER_START_1].Height = 19;
    Bmp[Tiles::RIVER_START_1].sourceRect.left = 500;
    Bmp[Tiles::RIVER_START_1].sourceRect.right = Bmp[Tiles::RIVER_START_1].sourceRect.left + Bmp[Tiles::RIVER_START_1].Width;
    Bmp[Tiles::RIVER_START_1].sourceRect.top = 0;
    Bmp[Tiles::RIVER_START_1].sourceRect.bottom = Bmp[Tiles::RIVER_START_1].sourceRect.top + Bmp[Tiles::RIVER_START_1].Height;
    Bmp[Tiles::RIVER_START_1].targetRect.left = 16;
    Bmp[Tiles::RIVER_START_1].targetRect.right = Bmp[Tiles::RIVER_START_1].targetRect.left + Bmp[Tiles::RIVER_START_1].Width;
    Bmp[Tiles::RIVER_START_1].targetRect.top = 19;
    Bmp[Tiles::RIVER_START_1].targetRect.bottom = Bmp[Tiles::RIVER_START_1].targetRect.top + Bmp[Tiles::RIVER_START_1].Height;

    Bmp[Tiles::RIVER_START_2].Width = 26;
    Bmp[Tiles::RIVER_START_2].Height = 19;
    Bmp[Tiles::RIVER_START_2].sourceRect.left = 530;
    Bmp[Tiles::RIVER_START_2].sourceRect.right = Bmp[Tiles::RIVER_START_2].sourceRect.left + Bmp[Tiles::RIVER_START_2].Width;
    Bmp[Tiles::RIVER_START_2].sourceRect.top = 0;
    Bmp[Tiles::RIVER_START_2].sourceRect.bottom = Bmp[Tiles::RIVER_START_2].sourceRect.top + Bmp[Tiles::RIVER_START_2].Height;
    Bmp[Tiles::RIVER_START_2].targetRect.left = 9;
    Bmp[Tiles::RIVER_START_2].targetRect.right = Bmp[Tiles::RIVER_START_2].targetRect.left + Bmp[Tiles::RIVER_START_2].Width;
    Bmp[Tiles::RIVER_START_2].targetRect.top = 20;
    Bmp[Tiles::RIVER_START_2].targetRect.bottom = Bmp[Tiles::RIVER_START_2].targetRect.top + Bmp[Tiles::RIVER_START_2].Height;

    Bmp[Tiles::RIVER_START_3].Width = 25;
    Bmp[Tiles::RIVER_START_3].Height = 16;
    Bmp[Tiles::RIVER_START_3].sourceRect.left = 556;
    Bmp[Tiles::RIVER_START_3].sourceRect.right = Bmp[Tiles::RIVER_START_3].sourceRect.left + Bmp[Tiles::RIVER_START_3].Width;
    Bmp[Tiles::RIVER_START_3].sourceRect.top = 0;
    Bmp[Tiles::RIVER_START_3].sourceRect.bottom = Bmp[Tiles::RIVER_START_3].sourceRect.top + Bmp[Tiles::RIVER_START_3].Height;
    Bmp[Tiles::RIVER_START_3].targetRect.left = 19;
    Bmp[Tiles::RIVER_START_3].targetRect.right = Bmp[Tiles::RIVER_START_3].targetRect.left + Bmp[Tiles::RIVER_START_3].Width;
    Bmp[Tiles::RIVER_START_3].targetRect.top = 19;
    Bmp[Tiles::RIVER_START_3].targetRect.bottom = Bmp[Tiles::RIVER_START_3].targetRect.top + Bmp[Tiles::RIVER_START_3].Height;

    Bmp[Tiles::RIVER_START_4].Width = 25;
    Bmp[Tiles::RIVER_START_4].Height = 15;
    Bmp[Tiles::RIVER_START_4].sourceRect.left = 581;
    Bmp[Tiles::RIVER_START_4].sourceRect.right = Bmp[Tiles::RIVER_START_4].sourceRect.left + Bmp[Tiles::RIVER_START_4].Width;
    Bmp[Tiles::RIVER_START_4].sourceRect.top = 0;
    Bmp[Tiles::RIVER_START_4].sourceRect.bottom = Bmp[Tiles::RIVER_START_4].sourceRect.top + Bmp[Tiles::RIVER_START_4].Height;
    Bmp[Tiles::RIVER_START_4].targetRect.left = 8;
    Bmp[Tiles::RIVER_START_4].targetRect.right = Bmp[Tiles::RIVER_START_4].targetRect.left + Bmp[Tiles::RIVER_START_4].Width;
    Bmp[Tiles::RIVER_START_4].targetRect.top = 21;
    Bmp[Tiles::RIVER_START_4].targetRect.bottom = Bmp[Tiles::RIVER_START_4].targetRect.top + Bmp[Tiles::RIVER_START_4].Height;

    Bmp[Tiles::FLOODGATE_1].Width = 35;
    Bmp[Tiles::FLOODGATE_1].Height = 22;
    Bmp[Tiles::FLOODGATE_1].sourceRect.left = 164;
    Bmp[Tiles::FLOODGATE_1].sourceRect.right = Bmp[Tiles::FLOODGATE_1].sourceRect.left + Bmp[Tiles::FLOODGATE_1].Width;
    Bmp[Tiles::FLOODGATE_1].sourceRect.top = 126;
    Bmp[Tiles::FLOODGATE_1].sourceRect.bottom = Bmp[Tiles::FLOODGATE_1].sourceRect.top + Bmp[Tiles::FLOODGATE_1].Height;
    Bmp[Tiles::FLOODGATE_1].targetRect.left = 10;
    Bmp[Tiles::FLOODGATE_1].targetRect.right = Bmp[Tiles::FLOODGATE_1].targetRect.left + Bmp[Tiles::FLOODGATE_1].Width;
    Bmp[Tiles::FLOODGATE_1].targetRect.top = 17;
    Bmp[Tiles::FLOODGATE_1].targetRect.bottom = Bmp[Tiles::FLOODGATE_1].targetRect.top + Bmp[Tiles::FLOODGATE_1].Height;

    Bmp[Tiles::FLOODGATE_2].Width = 34;
    Bmp[Tiles::FLOODGATE_2].Height = 23;
    Bmp[Tiles::FLOODGATE_2].sourceRect.left = 199;
    Bmp[Tiles::FLOODGATE_2].sourceRect.right = Bmp[Tiles::FLOODGATE_2].sourceRect.left + Bmp[Tiles::FLOODGATE_2].Width;
    Bmp[Tiles::FLOODGATE_2].sourceRect.top = 126;
    Bmp[Tiles::FLOODGATE_2].sourceRect.bottom = Bmp[Tiles::FLOODGATE_2].sourceRect.top + Bmp[Tiles::FLOODGATE_2].Height;
    Bmp[Tiles::FLOODGATE_2].targetRect.left = 10;
    Bmp[Tiles::FLOODGATE_2].targetRect.right = Bmp[Tiles::FLOODGATE_2].targetRect.left + Bmp[Tiles::FLOODGATE_2].Width;
    Bmp[Tiles::FLOODGATE_2].targetRect.top = 16;
    Bmp[Tiles::FLOODGATE_2].targetRect.bottom = Bmp[Tiles::FLOODGATE_2].targetRect.top + Bmp[Tiles::FLOODGATE_2].Height;

    Bmp[Tiles::FLOODGATE_3].Width = 34;
    Bmp[Tiles::FLOODGATE_3].Height = 22;
    Bmp[Tiles::FLOODGATE_3].sourceRect.left = 233;
    Bmp[Tiles::FLOODGATE_3].sourceRect.right = Bmp[Tiles::FLOODGATE_3].sourceRect.left + Bmp[Tiles::FLOODGATE_3].Width;
    Bmp[Tiles::FLOODGATE_3].sourceRect.top = 126;
    Bmp[Tiles::FLOODGATE_3].sourceRect.bottom = Bmp[Tiles::FLOODGATE_3].sourceRect.top + Bmp[Tiles::FLOODGATE_3].Height;
    Bmp[Tiles::FLOODGATE_3].targetRect.left = 10;
    Bmp[Tiles::FLOODGATE_3].targetRect.right = Bmp[Tiles::FLOODGATE_3].targetRect.left + Bmp[Tiles::FLOODGATE_3].Width;
    Bmp[Tiles::FLOODGATE_3].targetRect.top = 17;
    Bmp[Tiles::FLOODGATE_3].targetRect.bottom = Bmp[Tiles::FLOODGATE_3].targetRect.top + Bmp[Tiles::FLOODGATE_3].Height;

    Bmp[Tiles::FLOODGATE_4].Width = 33;
    Bmp[Tiles::FLOODGATE_4].Height = 23;
    Bmp[Tiles::FLOODGATE_4].sourceRect.left = 268;
    Bmp[Tiles::FLOODGATE_4].sourceRect.right = Bmp[Tiles::FLOODGATE_4].sourceRect.left + Bmp[Tiles::FLOODGATE_4].Width;
    Bmp[Tiles::FLOODGATE_4].sourceRect.top = 105;
    Bmp[Tiles::FLOODGATE_4].sourceRect.bottom = Bmp[Tiles::FLOODGATE_4].sourceRect.top + Bmp[Tiles::FLOODGATE_4].Height;
    Bmp[Tiles::FLOODGATE_4].targetRect.left = 11;
    Bmp[Tiles::FLOODGATE_4].targetRect.right = Bmp[Tiles::FLOODGATE_4].targetRect.left + Bmp[Tiles::FLOODGATE_4].Width;
    Bmp[Tiles::FLOODGATE_4].targetRect.top = 16;
    Bmp[Tiles::FLOODGATE_4].targetRect.bottom = Bmp[Tiles::FLOODGATE_4].targetRect.top + Bmp[Tiles::FLOODGATE_4].Height;

    Bmp[Tiles::FLOODGATE_5].Width = 34;
    Bmp[Tiles::FLOODGATE_5].Height = 17;
    Bmp[Tiles::FLOODGATE_5].sourceRect.left = 302;
    Bmp[Tiles::FLOODGATE_5].sourceRect.right = Bmp[Tiles::FLOODGATE_5].sourceRect.left + Bmp[Tiles::FLOODGATE_5].Width;
    Bmp[Tiles::FLOODGATE_5].sourceRect.top = 91;
    Bmp[Tiles::FLOODGATE_5].sourceRect.bottom = Bmp[Tiles::FLOODGATE_5].sourceRect.top + Bmp[Tiles::FLOODGATE_5].Height;
    Bmp[Tiles::FLOODGATE_5].targetRect.left = 10;
    Bmp[Tiles::FLOODGATE_5].targetRect.right = Bmp[Tiles::FLOODGATE_5].targetRect.left + Bmp[Tiles::FLOODGATE_5].Width;
    Bmp[Tiles::FLOODGATE_5].targetRect.top = 20;
    Bmp[Tiles::FLOODGATE_5].targetRect.bottom = Bmp[Tiles::FLOODGATE_5].targetRect.top + Bmp[Tiles::FLOODGATE_5].Height;

    Bmp[Tiles::FLOODGATE_6].Width = 35;
    Bmp[Tiles::FLOODGATE_6].Height = 23;
    Bmp[Tiles::FLOODGATE_6].sourceRect.left = 336;
    Bmp[Tiles::FLOODGATE_6].sourceRect.right = Bmp[Tiles::FLOODGATE_6].sourceRect.left + Bmp[Tiles::FLOODGATE_6].Width;
    Bmp[Tiles::FLOODGATE_6].sourceRect.top = 154;
    Bmp[Tiles::FLOODGATE_6].sourceRect.bottom = Bmp[Tiles::FLOODGATE_6].sourceRect.top + Bmp[Tiles::FLOODGATE_6].Height;
    Bmp[Tiles::FLOODGATE_6].targetRect.left = 10;
    Bmp[Tiles::FLOODGATE_6].targetRect.right = Bmp[Tiles::FLOODGATE_6].targetRect.left + Bmp[Tiles::FLOODGATE_6].Width;
    Bmp[Tiles::FLOODGATE_6].targetRect.top = 16;
    Bmp[Tiles::FLOODGATE_6].targetRect.bottom = Bmp[Tiles::FLOODGATE_6].targetRect.top + Bmp[Tiles::FLOODGATE_6].Height;

    // Bauwerke
    for (i = Tiles::FIELD; i <= Tiles::BONFIRE; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].Speed = 0;
        Bmp[i].AnimationPhase = 0;
    }

    Bmp[Tiles::FIELD].AnimationPhaseCount = 3;
    Bmp[Tiles::FIELD].Surface = lpDDSBau;
    Bmp[Tiles::FIELD].Width = 42;
    Bmp[Tiles::FIELD].Height = 27;
    Bmp[Tiles::FIELD].sourceRect.left = 0;
    Bmp[Tiles::FIELD].sourceRect.right = 0 + Bmp[Tiles::FIELD].Width;
    Bmp[Tiles::FIELD].sourceRect.top = 0;
    Bmp[Tiles::FIELD].sourceRect.bottom = 0 + Bmp[Tiles::FIELD].Height;
    Bmp[Tiles::FIELD].targetRect.left = 5;
    Bmp[Tiles::FIELD].targetRect.right = Bmp[Tiles::FIELD].targetRect.left + Bmp[Tiles::FIELD].Width;
    Bmp[Tiles::FIELD].targetRect.top = 15;
    Bmp[Tiles::FIELD].targetRect.bottom = Bmp[Tiles::FIELD].targetRect.top + Bmp[Tiles::FIELD].Height;
    Bmp[Tiles::FIELD].RequiredActionCases = 20;

    Bmp[Tiles::TENT].AnimationPhaseCount = 1;
    Bmp[Tiles::TENT].Surface = lpDDSBau;
    Bmp[Tiles::TENT].Width = 23;
    Bmp[Tiles::TENT].Height = 20;
    Bmp[Tiles::TENT].sourceRect.left = 42;
    Bmp[Tiles::TENT].sourceRect.right = 42 + Bmp[Tiles::TENT].Width;
    Bmp[Tiles::TENT].sourceRect.top = 0;
    Bmp[Tiles::TENT].sourceRect.bottom = 0 + Bmp[Tiles::TENT].Height;
    Bmp[Tiles::TENT].targetRect.left = 14;
    Bmp[Tiles::TENT].targetRect.right = Bmp[Tiles::TENT].targetRect.left + Bmp[Tiles::TENT].Width;
    Bmp[Tiles::TENT].targetRect.top = 9;
    Bmp[Tiles::TENT].targetRect.bottom = Bmp[Tiles::TENT].targetRect.top + Bmp[Tiles::TENT].Height;
    Bmp[Tiles::TENT].RequiredRawMaterials[Tiles::RAW_TREE_BRANCH] = 5;
    Bmp[Tiles::TENT].RequiredRawMaterials[Tiles::RAW_LEAF] = 5;
    Bmp[Tiles::TENT].RequiredActionCases = 16;

    Bmp[Tiles::BOAT].AnimationPhaseCount = 2;
    Bmp[Tiles::BOAT].Surface = lpDDSBau;
    Bmp[Tiles::BOAT].Width = 26;
    Bmp[Tiles::BOAT].Height = 18;
    Bmp[Tiles::BOAT].sourceRect.left = 65;
    Bmp[Tiles::BOAT].sourceRect.right = 65 + Bmp[Tiles::BOAT].Width;
    Bmp[Tiles::BOAT].sourceRect.top = 0;
    Bmp[Tiles::BOAT].sourceRect.bottom = 0 + Bmp[Tiles::BOAT].Height;
    Bmp[Tiles::BOAT].targetRect.left = 14;
    Bmp[Tiles::BOAT].targetRect.right = Bmp[Tiles::BOAT].targetRect.left + Bmp[Tiles::BOAT].Width;
    Bmp[Tiles::BOAT].targetRect.top = 20;
    Bmp[Tiles::BOAT].targetRect.bottom = Bmp[Tiles::BOAT].targetRect.top + Bmp[Tiles::BOAT].Height;
    Bmp[Tiles::BOAT].RequiredRawMaterials[Tiles::RAW_TREE_BRANCH] = 2;
    Bmp[Tiles::BOAT].RequiredRawMaterials[Tiles::RAW_TREE_TRUNK] = 1;
    Bmp[Tiles::BOAT].RequiredActionCases = 16;

    Bmp[Tiles::PIPE].AnimationPhaseCount = 2;
    Bmp[Tiles::PIPE].Surface = lpDDSBau;
    Bmp[Tiles::PIPE].Width = 34;
    Bmp[Tiles::PIPE].Height = 21;
    Bmp[Tiles::PIPE].sourceRect.left = 91;
    Bmp[Tiles::PIPE].sourceRect.right = 91 + Bmp[Tiles::PIPE].Width;
    Bmp[Tiles::PIPE].sourceRect.top = 0;
    Bmp[Tiles::PIPE].sourceRect.bottom = 0 + Bmp[Tiles::PIPE].Height;
    Bmp[Tiles::PIPE].targetRect.left = 11;
    Bmp[Tiles::PIPE].targetRect.right = Bmp[Tiles::PIPE].targetRect.left + Bmp[Tiles::PIPE].Width;
    Bmp[Tiles::PIPE].targetRect.top = 16;
    Bmp[Tiles::PIPE].targetRect.bottom = Bmp[Tiles::PIPE].targetRect.top + Bmp[Tiles::PIPE].Height;
    Bmp[Tiles::PIPE].RequiredRawMaterials[Tiles::RAW_TREE_TRUNK] = 1;
    Bmp[Tiles::PIPE].RequiredActionCases = 18;

    Bmp[Tiles::SOS].AnimationPhaseCount = 1;
    Bmp[Tiles::SOS].Surface = lpDDSBau;
    Bmp[Tiles::SOS].Width = 36;
    Bmp[Tiles::SOS].Height = 19;
    Bmp[Tiles::SOS].sourceRect.left = 125;
    Bmp[Tiles::SOS].sourceRect.right = 125 + Bmp[Tiles::SOS].Width;
    Bmp[Tiles::SOS].sourceRect.top = 0;
    Bmp[Tiles::SOS].sourceRect.bottom = 0 + Bmp[Tiles::SOS].Height;
    Bmp[Tiles::SOS].targetRect.left = 11;
    Bmp[Tiles::SOS].targetRect.right = Bmp[Tiles::SOS].targetRect.left + Bmp[Tiles::SOS].Width;
    Bmp[Tiles::SOS].targetRect.top = 20;
    Bmp[Tiles::SOS].targetRect.bottom = Bmp[Tiles::SOS].targetRect.top + Bmp[Tiles::SOS].Height;
    Bmp[Tiles::SOS].RequiredRawMaterials[Tiles::RAW_STONE] = 10;
    Bmp[Tiles::SOS].RequiredActionCases = 20;

    Bmp[Tiles::HOUSE_1].AnimationPhaseCount = 1;
    Bmp[Tiles::HOUSE_1].Surface = lpDDSBau;
    Bmp[Tiles::HOUSE_1].Width = 26;
    Bmp[Tiles::HOUSE_1].Height = 41;
    Bmp[Tiles::HOUSE_1].sourceRect.left = 161;
    Bmp[Tiles::HOUSE_1].sourceRect.right = 161 + Bmp[Tiles::HOUSE_1].Width;
    Bmp[Tiles::HOUSE_1].sourceRect.top = 0;
    Bmp[Tiles::HOUSE_1].sourceRect.bottom = 0 + Bmp[Tiles::HOUSE_1].Height;
    Bmp[Tiles::HOUSE_1].targetRect.left = 0;
    Bmp[Tiles::HOUSE_1].targetRect.right = Bmp[Tiles::HOUSE_1].targetRect.left + Bmp[Tiles::HOUSE_1].Width;
    Bmp[Tiles::HOUSE_1].targetRect.top = 0;
    Bmp[Tiles::HOUSE_1].targetRect.bottom = Bmp[Tiles::HOUSE_1].targetRect.top + Bmp[Tiles::HOUSE_1].Height;
    Bmp[Tiles::HOUSE_1].RequiredRawMaterials[Tiles::RAW_TREE_BRANCH] = 4;
    Bmp[Tiles::HOUSE_1].RequiredActionCases = 19;
    Bmp[Tiles::HOUSE_1].Sound = Sound::FOREST;

    Bmp[Tiles::HOUSE_2].AnimationPhaseCount = 1;
    Bmp[Tiles::HOUSE_2].Surface = lpDDSBau;
    Bmp[Tiles::HOUSE_2].Width = 34;
    Bmp[Tiles::HOUSE_2].Height = 41;
    Bmp[Tiles::HOUSE_2].sourceRect.left = 187;
    Bmp[Tiles::HOUSE_2].sourceRect.right = 187 + Bmp[Tiles::HOUSE_2].Width;
    Bmp[Tiles::HOUSE_2].sourceRect.top = 0;
    Bmp[Tiles::HOUSE_2].sourceRect.bottom = 0 + Bmp[Tiles::HOUSE_2].Height;
    Bmp[Tiles::HOUSE_2].targetRect.left = 0;
    Bmp[Tiles::HOUSE_2].targetRect.right = Bmp[Tiles::HOUSE_2].targetRect.left + Bmp[Tiles::HOUSE_2].Width;
    Bmp[Tiles::HOUSE_2].targetRect.top = 0;
    Bmp[Tiles::HOUSE_2].targetRect.bottom = Bmp[Tiles::HOUSE_2].targetRect.top + Bmp[Tiles::HOUSE_2].Height;
    Bmp[Tiles::HOUSE_2].RequiredRawMaterials[Tiles::RAW_TREE_BRANCH] = 3;
    Bmp[Tiles::HOUSE_2].RequiredRawMaterials[Tiles::RAW_TREE_TRUNK] = 3;
    Bmp[Tiles::HOUSE_2].RequiredActionCases = 21;
    Bmp[Tiles::HOUSE_2].Sound = Sound::FOREST;

    Bmp[Tiles::HOUSE_3].AnimationPhaseCount = 1;
    Bmp[Tiles::HOUSE_3].Surface = lpDDSBau;
    Bmp[Tiles::HOUSE_3].Width = 34;
    Bmp[Tiles::HOUSE_3].Height = 41;
    Bmp[Tiles::HOUSE_3].sourceRect.left = 221;
    Bmp[Tiles::HOUSE_3].sourceRect.right = 221 + Bmp[Tiles::HOUSE_3].Width;
    Bmp[Tiles::HOUSE_3].sourceRect.top = 0;
    Bmp[Tiles::HOUSE_3].sourceRect.bottom = 0 + Bmp[Tiles::HOUSE_3].Height;
    Bmp[Tiles::HOUSE_3].targetRect.left = 0;
    Bmp[Tiles::HOUSE_3].targetRect.right = Bmp[Tiles::HOUSE_3].targetRect.left + Bmp[Tiles::HOUSE_3].Width;
    Bmp[Tiles::HOUSE_3].targetRect.top = 0;
    Bmp[Tiles::HOUSE_3].targetRect.bottom = Bmp[Tiles::HOUSE_3].targetRect.top + Bmp[Tiles::HOUSE_3].Height;
    Bmp[Tiles::HOUSE_3].RequiredRawMaterials[Tiles::RAW_TREE_BRANCH] = 4;
    Bmp[Tiles::HOUSE_3].RequiredRawMaterials[Tiles::RAW_TREE_TRUNK] = 4;
    Bmp[Tiles::HOUSE_3].RequiredRawMaterials[Tiles::RAW_LEAF] = 5;
    Bmp[Tiles::HOUSE_3].RequiredActionCases = 21;
    Bmp[Tiles::HOUSE_3].Sound = Sound::FOREST;

    Bmp[Tiles::BONFIRE].AnimationPhaseCount = 1;
    Bmp[Tiles::BONFIRE].Surface = lpDDSBau;
    Bmp[Tiles::BONFIRE].Width = 21;
    Bmp[Tiles::BONFIRE].Height = 19;
    Bmp[Tiles::BONFIRE].sourceRect.left = 255;
    Bmp[Tiles::BONFIRE].sourceRect.right = 255 + Bmp[Tiles::BONFIRE].Width;
    Bmp[Tiles::BONFIRE].sourceRect.top = 0;
    Bmp[Tiles::BONFIRE].sourceRect.bottom = 0 + Bmp[Tiles::BONFIRE].Height;
    Bmp[Tiles::BONFIRE].targetRect.left = 15;
    Bmp[Tiles::BONFIRE].targetRect.right = Bmp[Tiles::BONFIRE].targetRect.left + Bmp[Tiles::BONFIRE].Width;
    Bmp[Tiles::BONFIRE].targetRect.top = 10;
    Bmp[Tiles::BONFIRE].targetRect.bottom = Bmp[Tiles::BONFIRE].targetRect.top + Bmp[Tiles::BONFIRE].Height;
    Bmp[Tiles::BONFIRE].RequiredRawMaterials[Tiles::RAW_TREE_BRANCH] = 5;
    Bmp[Tiles::BONFIRE].RequiredRawMaterials[Tiles::RAW_TREE_TRUNK] = 4;
    Bmp[Tiles::BONFIRE].RequiredActionCases = 9;

    // Allgemein Bäume
    for (i = Tiles::TREE_1; i <= Tiles::TREE_4; i++) {
        Bmp[i].IsAnimationRunning = true;
        Bmp[i].AnimationPhaseCount = 4;
        Bmp[i].Speed = 4;
        Bmp[i].AnimationPhase = 0;
        Bmp[i].Surface = lpDDSBaum;
        Bmp[i].Sound = Sound::FOREST;
    }

    // Baum1
    Bmp[Tiles::TREE_1].sourceRect.left = 0;
    Bmp[Tiles::TREE_1].sourceRect.top = 0;
    Bmp[Tiles::TREE_1].sourceRect.right = 21;
    Bmp[Tiles::TREE_1].sourceRect.bottom = 26;
    Bmp[Tiles::TREE_1].Width = static_cast<short>(Bmp[Tiles::TREE_1].sourceRect.right - Bmp[Tiles::TREE_1].sourceRect.left);
    Bmp[Tiles::TREE_1].Height = static_cast<short>(Bmp[Tiles::TREE_1].sourceRect.bottom - Bmp[Tiles::TREE_1].sourceRect.top);

    // Baum2
    Bmp[Tiles::TREE_2].sourceRect.left = 21;
    Bmp[Tiles::TREE_2].sourceRect.top = 0;
    Bmp[Tiles::TREE_2].sourceRect.right = 42;
    Bmp[Tiles::TREE_2].sourceRect.bottom = 26;
    Bmp[Tiles::TREE_2].Width = static_cast<short>(Bmp[Tiles::TREE_2].sourceRect.right - Bmp[Tiles::TREE_2].sourceRect.left);
    Bmp[Tiles::TREE_2].Height = static_cast<short>(Bmp[Tiles::TREE_2].sourceRect.bottom - Bmp[Tiles::TREE_2].sourceRect.top);

    // Baum3
    Bmp[Tiles::TREE_3].sourceRect.left = 42;
    Bmp[Tiles::TREE_3].sourceRect.top = 0;
    Bmp[Tiles::TREE_3].sourceRect.right = 64;
    Bmp[Tiles::TREE_3].sourceRect.bottom = 27;
    Bmp[Tiles::TREE_3].Width = static_cast<short>(Bmp[Tiles::TREE_3].sourceRect.right - Bmp[Tiles::TREE_3].sourceRect.left);
    Bmp[Tiles::TREE_3].Height = static_cast<short>(Bmp[Tiles::TREE_3].sourceRect.bottom - Bmp[Tiles::TREE_3].sourceRect.top);

    // Baum4
    Bmp[Tiles::TREE_4].sourceRect.left = 64;
    Bmp[Tiles::TREE_4].sourceRect.top = 0;
    Bmp[Tiles::TREE_4].sourceRect.right = 81;
    Bmp[Tiles::TREE_4].sourceRect.bottom = 16;
    Bmp[Tiles::TREE_4].Width = static_cast<short>(Bmp[Tiles::TREE_4].sourceRect.right - Bmp[Tiles::TREE_4].sourceRect.left);
    Bmp[Tiles::TREE_4].Height = static_cast<short>(Bmp[Tiles::TREE_4].sourceRect.bottom - Bmp[Tiles::TREE_4].sourceRect.top);

    // Baumgroß
    Bmp[Tiles::TREE_BIG].sourceRect.left = 238;
    Bmp[Tiles::TREE_BIG].sourceRect.top = 0;
    Bmp[Tiles::TREE_BIG].sourceRect.right = 238 + 26;
    Bmp[Tiles::TREE_BIG].sourceRect.bottom = 41;
    Bmp[Tiles::TREE_BIG].Width = static_cast<short>(Bmp[Tiles::TREE_BIG].sourceRect.right - Bmp[Tiles::TREE_BIG].sourceRect.left);
    Bmp[Tiles::TREE_BIG].Height = static_cast<short>(Bmp[Tiles::TREE_BIG].sourceRect.bottom - Bmp[Tiles::TREE_BIG].sourceRect.top);
    Bmp[Tiles::TREE_BIG].IsAnimationRunning = false;
    Bmp[Tiles::TREE_BIG].AnimationPhaseCount = 1;
    Bmp[Tiles::TREE_BIG].Speed = 0;
    Bmp[Tiles::TREE_BIG].AnimationPhase = 0;
    Bmp[Tiles::TREE_BIG].Surface = lpDDSBaum;
    Bmp[Tiles::TREE_BIG].Sound = Sound::FOREST;

    // Feuer
    Bmp[Tiles::FIRE].AnimationPhaseCount = 3;
    Bmp[Tiles::FIRE].Surface = lpDDSBau;
    Bmp[Tiles::FIRE].Width = 21;
    Bmp[Tiles::FIRE].Height = 28;
    Bmp[Tiles::FIRE].sourceRect.left = 276;
    Bmp[Tiles::FIRE].sourceRect.right = 276 + Bmp[Tiles::FIRE].Width;
    Bmp[Tiles::FIRE].sourceRect.top = 0;
    Bmp[Tiles::FIRE].sourceRect.bottom = 0 + Bmp[Tiles::FIRE].Height;
    Bmp[Tiles::FIRE].targetRect.left = 15;
    Bmp[Tiles::FIRE].targetRect.right = Bmp[Tiles::FIRE].targetRect.left + Bmp[Tiles::FIRE].Width;
    Bmp[Tiles::FIRE].targetRect.top = 1;
    Bmp[Tiles::FIRE].targetRect.bottom = Bmp[Tiles::FIRE].targetRect.top + Bmp[Tiles::FIRE].Height;
    Bmp[Tiles::FIRE].IsAnimationRunning = true;
    Bmp[Tiles::FIRE].Speed = 6;
    Bmp[Tiles::FIRE].AnimationPhase = 0;
    Bmp[Tiles::FIRE].Sound = Sound::FIRE;

    // WRACK
    Bmp[Tiles::WRECK_1].AnimationPhaseCount = 3;
    Bmp[Tiles::WRECK_1].Surface = lpDDSBau;
    Bmp[Tiles::WRECK_1].Width = 24;
    Bmp[Tiles::WRECK_1].Height = 18;
    Bmp[Tiles::WRECK_1].sourceRect.left = 391;
    Bmp[Tiles::WRECK_1].sourceRect.right = 391 + Bmp[Tiles::WRECK_1].Width;
    Bmp[Tiles::WRECK_1].sourceRect.top = 0;
    Bmp[Tiles::WRECK_1].sourceRect.bottom = 0 + Bmp[Tiles::WRECK_1].Height;
    Bmp[Tiles::WRECK_1].targetRect.left = 15;
    Bmp[Tiles::WRECK_1].targetRect.right = Bmp[Tiles::WRECK_1].targetRect.left + Bmp[Tiles::WRECK_1].Width;
    Bmp[Tiles::WRECK_1].targetRect.top = 20;
    Bmp[Tiles::WRECK_1].targetRect.bottom = Bmp[Tiles::WRECK_1].targetRect.top + Bmp[Tiles::WRECK_1].Height;
    Bmp[Tiles::WRECK_1].IsAnimationRunning = true;
    Bmp[Tiles::WRECK_1].Speed = 5;
    Bmp[Tiles::WRECK_1].AnimationPhase = 0;

    // WRACK2
    Bmp[Tiles::WRECK_2].AnimationPhaseCount = 3;
    Bmp[Tiles::WRECK_2].Surface = lpDDSBau;
    Bmp[Tiles::WRECK_2].Width = 27;
    Bmp[Tiles::WRECK_2].Height = 26;
    Bmp[Tiles::WRECK_2].sourceRect.left = 415;
    Bmp[Tiles::WRECK_2].sourceRect.right = 415 + Bmp[Tiles::WRECK_2].Width;
    Bmp[Tiles::WRECK_2].sourceRect.top = 0;
    Bmp[Tiles::WRECK_2].sourceRect.bottom = 0 + Bmp[Tiles::WRECK_2].Height;
    Bmp[Tiles::WRECK_2].targetRect.left = 15;
    Bmp[Tiles::WRECK_2].targetRect.right = Bmp[Tiles::WRECK_2].targetRect.left + Bmp[Tiles::WRECK_2].Width;
    Bmp[Tiles::WRECK_2].targetRect.top = 16;
    Bmp[Tiles::WRECK_2].targetRect.bottom = Bmp[Tiles::WRECK_2].targetRect.top + Bmp[Tiles::WRECK_2].Height;
    Bmp[Tiles::WRECK_2].IsAnimationRunning = true;
    Bmp[Tiles::WRECK_2].Speed = 5;
    Bmp[Tiles::WRECK_2].AnimationPhase = 0;

    // Früchte
    // Busch
    Bmp[Tiles::BUSH].sourceRect.left = 81;
    Bmp[Tiles::BUSH].sourceRect.top = 0;
    Bmp[Tiles::BUSH].sourceRect.right = 81 + 20;
    Bmp[Tiles::BUSH].sourceRect.bottom = 13;
    Bmp[Tiles::BUSH].Width = static_cast<short>(Bmp[Tiles::BUSH].sourceRect.right - Bmp[Tiles::BUSH].sourceRect.left);
    Bmp[Tiles::BUSH].Height = static_cast<short>(Bmp[Tiles::BUSH].sourceRect.bottom - Bmp[Tiles::BUSH].sourceRect.top);
    Bmp[Tiles::BUSH].IsAnimationRunning = false;
    Bmp[Tiles::BUSH].AnimationPhaseCount = 3;
    Bmp[Tiles::BUSH].Speed = 0;
    Bmp[Tiles::BUSH].AnimationPhase = 0;
    Bmp[Tiles::BUSH].Surface = lpDDSBaum;

    // Buttons

    // StandardBmponsinitialisierung
    for (i = Tiles::BUTTON_GRID; i <= Tiles::BUTTON_DESTROY; i++) {
        Bmp[i].IsAnimationRunning = false;
        Bmp[i].Surface = lpDDSButtons;
        Bmp[i].AnimationPhaseCount = 1;
        Bmp[i].AnimationPhase = 0;
    }

    // ButtGitter
    Bmp[Tiles::BUTTON_GRID].sourceRect.left = 0;
    Bmp[Tiles::BUTTON_GRID].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_GRID].sourceRect.right = 28;
    Bmp[Tiles::BUTTON_GRID].sourceRect.bottom = 28;
    Bmp[Tiles::BUTTON_GRID].targetRect.left = rcPanel.left + 173;
    Bmp[Tiles::BUTTON_GRID].targetRect.top = rcPanel.top + 26;
    Bmp[Tiles::BUTTON_GRID].targetRect.right = Bmp[Tiles::BUTTON_GRID].targetRect.left + 28;
    Bmp[Tiles::BUTTON_GRID].targetRect.bottom = Bmp[Tiles::BUTTON_GRID].targetRect.top + 28;
    Bmp[Tiles::BUTTON_GRID].Width = static_cast<short>(Bmp[Tiles::BUTTON_GRID].sourceRect.right - Bmp[Tiles::BUTTON_GRID].sourceRect.left);
    Bmp[Tiles::BUTTON_GRID].Height = static_cast<short>(Bmp[Tiles::BUTTON_GRID].sourceRect.bottom - Bmp[Tiles::BUTTON_GRID].sourceRect.top);
    Bmp[Tiles::BUTTON_GRID].AnimationPhaseCount = 2;

    // BUTTANIMATION
    Bmp[Tiles::BUTTON_ANIMATION].sourceRect.left = 0;
    Bmp[Tiles::BUTTON_ANIMATION].sourceRect.top = 56;
    Bmp[Tiles::BUTTON_ANIMATION].sourceRect.right = 28;
    Bmp[Tiles::BUTTON_ANIMATION].sourceRect.bottom = 56 + 28;
    Bmp[Tiles::BUTTON_ANIMATION].targetRect.left = rcPanel.left + 173;
    Bmp[Tiles::BUTTON_ANIMATION].targetRect.top = rcPanel.top + 60;
    Bmp[Tiles::BUTTON_ANIMATION].targetRect.right = Bmp[Tiles::BUTTON_ANIMATION].targetRect.left + 28;
    Bmp[Tiles::BUTTON_ANIMATION].targetRect.bottom = Bmp[Tiles::BUTTON_ANIMATION].targetRect.top + 28;
    Bmp[Tiles::BUTTON_ANIMATION].Width = static_cast<short>(Bmp[Tiles::BUTTON_ANIMATION].sourceRect.right - Bmp[Tiles::BUTTON_ANIMATION].sourceRect.left);
    Bmp[Tiles::BUTTON_ANIMATION].Height = static_cast<short>(Bmp[Tiles::BUTTON_ANIMATION].sourceRect.bottom - Bmp[Tiles::BUTTON_ANIMATION].sourceRect.top);
    Bmp[Tiles::BUTTON_ANIMATION].AnimationPhaseCount = 2;

    // BUTTBEENDEN
    Bmp[Tiles::BUTTON_END].sourceRect.left = 0;
    Bmp[Tiles::BUTTON_END].sourceRect.top = 112;
    Bmp[Tiles::BUTTON_END].sourceRect.right = 20;
    Bmp[Tiles::BUTTON_END].sourceRect.bottom = 112 + 20;
    Bmp[Tiles::BUTTON_END].targetRect.left = rcPanel.left + 60;
    Bmp[Tiles::BUTTON_END].targetRect.top = rcPanel.top + 2;
    Bmp[Tiles::BUTTON_END].targetRect.right = Bmp[Tiles::BUTTON_END].targetRect.left + 20;
    Bmp[Tiles::BUTTON_END].targetRect.bottom = Bmp[Tiles::BUTTON_END].targetRect.top + 20;
    Bmp[Tiles::BUTTON_END].Width = static_cast<short>(Bmp[Tiles::BUTTON_END].sourceRect.right - Bmp[Tiles::BUTTON_END].sourceRect.left);
    Bmp[Tiles::BUTTON_END].Height = static_cast<short>(Bmp[Tiles::BUTTON_END].sourceRect.bottom - Bmp[Tiles::BUTTON_END].sourceRect.top);
    Bmp[Tiles::BUTTON_END].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_END].Speed = 4;

    // BUTTNEU
    Bmp[Tiles::BUTTON_NEW].sourceRect.left = 0;
    Bmp[Tiles::BUTTON_NEW].sourceRect.top = 192;
    Bmp[Tiles::BUTTON_NEW].sourceRect.right = 20;
    Bmp[Tiles::BUTTON_NEW].sourceRect.bottom = 192 + 20;
    Bmp[Tiles::BUTTON_NEW].targetRect.left = rcPanel.left + 100;
    Bmp[Tiles::BUTTON_NEW].targetRect.top = rcPanel.top + 2;
    Bmp[Tiles::BUTTON_NEW].targetRect.right = Bmp[Tiles::BUTTON_NEW].targetRect.left + 20;
    Bmp[Tiles::BUTTON_NEW].targetRect.bottom = Bmp[Tiles::BUTTON_NEW].targetRect.top + 20;
    Bmp[Tiles::BUTTON_NEW].Width = static_cast<short>(Bmp[Tiles::BUTTON_NEW].sourceRect.right - Bmp[Tiles::BUTTON_NEW].sourceRect.left);
    Bmp[Tiles::BUTTON_NEW].Height = static_cast<short>(Bmp[Tiles::BUTTON_NEW].sourceRect.bottom - Bmp[Tiles::BUTTON_NEW].sourceRect.top);
    Bmp[Tiles::BUTTON_NEW].AnimationPhaseCount = 2;
    Bmp[Tiles::BUTTON_NEW].Speed = 3;


    // BUTTTAGNEU
    Bmp[Tiles::BUTTON_NEW_DAY].sourceRect.left = 0;
    Bmp[Tiles::BUTTON_NEW_DAY].sourceRect.top = 232;
    Bmp[Tiles::BUTTON_NEW_DAY].sourceRect.right = 20;
    Bmp[Tiles::BUTTON_NEW_DAY].sourceRect.bottom = 232 + 20;
    Bmp[Tiles::BUTTON_NEW_DAY].targetRect.left = rcPanel.left + 140;
    Bmp[Tiles::BUTTON_NEW_DAY].targetRect.top = rcPanel.top + 2;
    Bmp[Tiles::BUTTON_NEW_DAY].targetRect.right = Bmp[Tiles::BUTTON_NEW_DAY].targetRect.left + 20;
    Bmp[Tiles::BUTTON_NEW_DAY].targetRect.bottom = Bmp[Tiles::BUTTON_NEW_DAY].targetRect.top + 20;
    Bmp[Tiles::BUTTON_NEW_DAY].Width = static_cast<short>(Bmp[Tiles::BUTTON_NEW_DAY].sourceRect.right - Bmp[Tiles::BUTTON_NEW_DAY].sourceRect.left);
    Bmp[Tiles::BUTTON_NEW_DAY].Height = static_cast<short>(Bmp[Tiles::BUTTON_NEW_DAY].sourceRect.bottom - Bmp[Tiles::BUTTON_NEW_DAY].sourceRect.top);
    Bmp[Tiles::BUTTON_NEW_DAY].AnimationPhaseCount = 2;
    Bmp[Tiles::BUTTON_NEW_DAY].Speed = 2;

    // BUTTSOUND
    Bmp[Tiles::BUTTON_SOUND].sourceRect.left = 0;
    Bmp[Tiles::BUTTON_SOUND].sourceRect.top = 272;
    Bmp[Tiles::BUTTON_SOUND].sourceRect.right = 28;
    Bmp[Tiles::BUTTON_SOUND].sourceRect.bottom = 272 + 28;
    Bmp[Tiles::BUTTON_SOUND].targetRect.left = rcPanel.left + 173;
    Bmp[Tiles::BUTTON_SOUND].targetRect.top = rcPanel.top + 94;
    Bmp[Tiles::BUTTON_SOUND].targetRect.right = Bmp[Tiles::BUTTON_SOUND].targetRect.left + 28;
    Bmp[Tiles::BUTTON_SOUND].targetRect.bottom = Bmp[Tiles::BUTTON_SOUND].targetRect.top + 28;
    Bmp[Tiles::BUTTON_SOUND].Width = static_cast<short>(Bmp[Tiles::BUTTON_SOUND].sourceRect.right - Bmp[Tiles::BUTTON_SOUND].sourceRect.left);
    Bmp[Tiles::BUTTON_SOUND].Height = static_cast<short>(Bmp[Tiles::BUTTON_SOUND].sourceRect.bottom - Bmp[Tiles::BUTTON_SOUND].sourceRect.top);
    Bmp[Tiles::BUTTON_SOUND].AnimationPhaseCount = 2;

    // ButtAktion
    Bmp[Tiles::BUTTON_ACTION].sourceRect.left = 28;
    Bmp[Tiles::BUTTON_ACTION].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_ACTION].sourceRect.right = 28 + 35;
    Bmp[Tiles::BUTTON_ACTION].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_ACTION].targetRect.left = rcPanel.left + 29;
    Bmp[Tiles::BUTTON_ACTION].targetRect.top = rcPanel.top + 191;
    Bmp[Tiles::BUTTON_ACTION].targetRect.right = Bmp[Tiles::BUTTON_ACTION].targetRect.left + 35;
    Bmp[Tiles::BUTTON_ACTION].targetRect.bottom = Bmp[Tiles::BUTTON_ACTION].targetRect.top + 35;
    Bmp[Tiles::BUTTON_ACTION].Width = static_cast<short>(Bmp[Tiles::BUTTON_ACTION].sourceRect.right - Bmp[Tiles::BUTTON_ACTION].sourceRect.left);
    Bmp[Tiles::BUTTON_ACTION].Height = static_cast<short>(Bmp[Tiles::BUTTON_ACTION].sourceRect.bottom - Bmp[Tiles::BUTTON_ACTION].sourceRect.top);
    Bmp[Tiles::BUTTON_ACTION].AnimationPhaseCount = 3;
    Bmp[Tiles::BUTTON_ACTION].Speed = 6;

    // BUTTBAUEN
    Bmp[Tiles::BUTTON_CONSTRUCT].sourceRect.left = 203;
    Bmp[Tiles::BUTTON_CONSTRUCT].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_CONSTRUCT].sourceRect.right = 203 + 35;
    Bmp[Tiles::BUTTON_CONSTRUCT].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_CONSTRUCT].targetRect.left = rcPanel.left + 70;
    Bmp[Tiles::BUTTON_CONSTRUCT].targetRect.top = rcPanel.top + 191;
    Bmp[Tiles::BUTTON_CONSTRUCT].targetRect.right = Bmp[Tiles::BUTTON_CONSTRUCT].targetRect.left + 35;
    Bmp[Tiles::BUTTON_CONSTRUCT].targetRect.bottom = Bmp[Tiles::BUTTON_CONSTRUCT].targetRect.top + 35;
    Bmp[Tiles::BUTTON_CONSTRUCT].Width = static_cast<short>(Bmp[Tiles::BUTTON_CONSTRUCT].sourceRect.right - Bmp[Tiles::BUTTON_CONSTRUCT].sourceRect.left);
    Bmp[Tiles::BUTTON_CONSTRUCT].Height = static_cast<short>(Bmp[Tiles::BUTTON_CONSTRUCT].sourceRect.bottom - Bmp[Tiles::BUTTON_CONSTRUCT].sourceRect.top);
    Bmp[Tiles::BUTTON_CONSTRUCT].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_CONSTRUCT].Speed = 5;


    // BUTTINVENTAR
    Bmp[Tiles::BUTTON_INVENTORY].sourceRect.left = 168;
    Bmp[Tiles::BUTTON_INVENTORY].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_INVENTORY].sourceRect.right = 168 + 35;
    Bmp[Tiles::BUTTON_INVENTORY].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_INVENTORY].targetRect.left = rcPanel.left + 152;
    Bmp[Tiles::BUTTON_INVENTORY].targetRect.top = rcPanel.top + 191;
    Bmp[Tiles::BUTTON_INVENTORY].targetRect.right = Bmp[Tiles::BUTTON_INVENTORY].targetRect.left + 35;
    Bmp[Tiles::BUTTON_INVENTORY].targetRect.bottom = Bmp[Tiles::BUTTON_INVENTORY].targetRect.top + 35;
    Bmp[Tiles::BUTTON_INVENTORY].Width = static_cast<short>(Bmp[Tiles::BUTTON_INVENTORY].sourceRect.right - Bmp[Tiles::BUTTON_INVENTORY].sourceRect.left);
    Bmp[Tiles::BUTTON_INVENTORY].Height = static_cast<short>(Bmp[Tiles::BUTTON_INVENTORY].sourceRect.bottom - Bmp[Tiles::BUTTON_INVENTORY].sourceRect.top);
    Bmp[Tiles::BUTTON_INVENTORY].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_INVENTORY].Speed = 4;

    // BUTTWEITER
    Bmp[Tiles::BUTTON_CONTINUE].sourceRect.left = 343;
    Bmp[Tiles::BUTTON_CONTINUE].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_CONTINUE].sourceRect.right = 343 + 35;
    Bmp[Tiles::BUTTON_CONTINUE].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_CONTINUE].targetRect.left = rcPanel.left + 111;
    Bmp[Tiles::BUTTON_CONTINUE].targetRect.top = rcPanel.top + 191;
    Bmp[Tiles::BUTTON_CONTINUE].targetRect.right = Bmp[Tiles::BUTTON_CONTINUE].targetRect.left + 35;
    Bmp[Tiles::BUTTON_CONTINUE].targetRect.bottom = Bmp[Tiles::BUTTON_CONTINUE].targetRect.top + 35;
    Bmp[Tiles::BUTTON_CONTINUE].Width = static_cast<short>(Bmp[Tiles::BUTTON_CONTINUE].sourceRect.right - Bmp[Tiles::BUTTON_CONTINUE].sourceRect.left);
    Bmp[Tiles::BUTTON_CONTINUE].Height = static_cast<short>(Bmp[Tiles::BUTTON_CONTINUE].sourceRect.bottom - Bmp[Tiles::BUTTON_CONTINUE].sourceRect.top);
    Bmp[Tiles::BUTTON_CONTINUE].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_CONTINUE].Speed = 4;
    Bmp[Tiles::BUTTON_CONTINUE].AnimationPhase = -1;

    // BUTTSTOP
    Bmp[Tiles::BUTTON_STOP].sourceRect.left = 378;
    Bmp[Tiles::BUTTON_STOP].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_STOP].sourceRect.right = 378 + 35;
    Bmp[Tiles::BUTTON_STOP].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_STOP].targetRect.left = rcPanel.left + 111;
    Bmp[Tiles::BUTTON_STOP].targetRect.top = rcPanel.top + 191;
    Bmp[Tiles::BUTTON_STOP].targetRect.right = Bmp[Tiles::BUTTON_STOP].targetRect.left + 35;
    Bmp[Tiles::BUTTON_STOP].targetRect.bottom = Bmp[Tiles::BUTTON_STOP].targetRect.top + 35;
    Bmp[Tiles::BUTTON_STOP].Width = static_cast<short>(Bmp[Tiles::BUTTON_STOP].sourceRect.right - Bmp[Tiles::BUTTON_STOP].sourceRect.left);
    Bmp[Tiles::BUTTON_STOP].Height = static_cast<short>(Bmp[Tiles::BUTTON_STOP].sourceRect.bottom - Bmp[Tiles::BUTTON_STOP].sourceRect.top);
    Bmp[Tiles::BUTTON_STOP].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_STOP].Speed = 4;
    Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

    // BUTTABLEGEN
    Bmp[Tiles::BUTTON_LAY_DOWN].sourceRect.left = 483;
    Bmp[Tiles::BUTTON_LAY_DOWN].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_LAY_DOWN].sourceRect.right = 483 + 35;
    Bmp[Tiles::BUTTON_LAY_DOWN].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_LAY_DOWN].targetRect.left = rcPanel.left + 111;
    Bmp[Tiles::BUTTON_LAY_DOWN].targetRect.top = rcPanel.top + 191;
    Bmp[Tiles::BUTTON_LAY_DOWN].targetRect.right = Bmp[Tiles::BUTTON_LAY_DOWN].targetRect.left + 35;
    Bmp[Tiles::BUTTON_LAY_DOWN].targetRect.bottom = Bmp[Tiles::BUTTON_LAY_DOWN].targetRect.top + 35;
    Bmp[Tiles::BUTTON_LAY_DOWN].Width = static_cast<short>(Bmp[Tiles::BUTTON_LAY_DOWN].sourceRect.right - Bmp[Tiles::BUTTON_LAY_DOWN].sourceRect.left);
    Bmp[Tiles::BUTTON_LAY_DOWN].Height = static_cast<short>(Bmp[Tiles::BUTTON_LAY_DOWN].sourceRect.bottom - Bmp[Tiles::BUTTON_LAY_DOWN].sourceRect.top);
    Bmp[Tiles::BUTTON_LAY_DOWN].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_LAY_DOWN].Speed = 3;
    Bmp[Tiles::BUTTON_STOP].AnimationPhase = -1;

    // BUTTSUCHEN
    Bmp[Tiles::BUTTON_SEARCH].sourceRect.left = 63;
    Bmp[Tiles::BUTTON_SEARCH].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_SEARCH].sourceRect.right = 63 + 35;
    Bmp[Tiles::BUTTON_SEARCH].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_SEARCH].targetRect.left = rcPanel.left + 29;
    Bmp[Tiles::BUTTON_SEARCH].targetRect.top = rcPanel.top + 270;
    Bmp[Tiles::BUTTON_SEARCH].targetRect.right = Bmp[Tiles::BUTTON_SEARCH].targetRect.left + 35;
    Bmp[Tiles::BUTTON_SEARCH].targetRect.bottom = Bmp[Tiles::BUTTON_SEARCH].targetRect.top + 35;
    Bmp[Tiles::BUTTON_SEARCH].Width = static_cast<short>(Bmp[Tiles::BUTTON_SEARCH].sourceRect.right - Bmp[Tiles::BUTTON_SEARCH].sourceRect.left);
    Bmp[Tiles::BUTTON_SEARCH].Height = static_cast<short>(Bmp[Tiles::BUTTON_SEARCH].sourceRect.bottom - Bmp[Tiles::BUTTON_SEARCH].sourceRect.top);
    Bmp[Tiles::BUTTON_SEARCH].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_SEARCH].Speed = 4;

    // BUTTESSEN
    Bmp[Tiles::BUTTON_EAT].sourceRect.left = 133;
    Bmp[Tiles::BUTTON_EAT].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_EAT].sourceRect.right = 133 + 35;
    Bmp[Tiles::BUTTON_EAT].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_EAT].targetRect.left = rcPanel.left + 70;
    Bmp[Tiles::BUTTON_EAT].targetRect.top = rcPanel.top + 270;
    Bmp[Tiles::BUTTON_EAT].targetRect.right = Bmp[Tiles::BUTTON_EAT].targetRect.left + 35;
    Bmp[Tiles::BUTTON_EAT].targetRect.bottom = Bmp[Tiles::BUTTON_EAT].targetRect.top + 35;
    Bmp[Tiles::BUTTON_EAT].Width = static_cast<short>(Bmp[Tiles::BUTTON_EAT].sourceRect.right - Bmp[Tiles::BUTTON_EAT].sourceRect.left);
    Bmp[Tiles::BUTTON_EAT].Height = static_cast<short>(Bmp[Tiles::BUTTON_EAT].sourceRect.bottom - Bmp[Tiles::BUTTON_EAT].sourceRect.top);
    Bmp[Tiles::BUTTON_EAT].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_EAT].Speed = 4;

    // BUTTSCHLAFEN
    Bmp[Tiles::BUTTON_SLEEP].sourceRect.left = 308;
    Bmp[Tiles::BUTTON_SLEEP].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_SLEEP].sourceRect.right = 308 + 35;
    Bmp[Tiles::BUTTON_SLEEP].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_SLEEP].targetRect.left = rcPanel.left + 111;
    Bmp[Tiles::BUTTON_SLEEP].targetRect.top = rcPanel.top + 270;
    Bmp[Tiles::BUTTON_SLEEP].targetRect.right = Bmp[Tiles::BUTTON_SLEEP].targetRect.left + 35;
    Bmp[Tiles::BUTTON_SLEEP].targetRect.bottom = Bmp[Tiles::BUTTON_SLEEP].targetRect.top + 35;
    Bmp[Tiles::BUTTON_SLEEP].Width = static_cast<short>(Bmp[Tiles::BUTTON_SLEEP].sourceRect.right - Bmp[Tiles::BUTTON_SLEEP].sourceRect.left);
    Bmp[Tiles::BUTTON_SLEEP].Height = static_cast<short>(Bmp[Tiles::BUTTON_SLEEP].sourceRect.bottom - Bmp[Tiles::BUTTON_SLEEP].sourceRect.top);
    Bmp[Tiles::BUTTON_SLEEP].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_SLEEP].Speed = 3;

    // BUTTFAELLEN
    Bmp[Tiles::BUTTON_CHOP].sourceRect.left = 98;
    Bmp[Tiles::BUTTON_CHOP].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_CHOP].sourceRect.right = 98 + 35;
    Bmp[Tiles::BUTTON_CHOP].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_CHOP].targetRect.left = rcPanel.left + 152;
    Bmp[Tiles::BUTTON_CHOP].targetRect.top = rcPanel.top + 270;
    Bmp[Tiles::BUTTON_CHOP].targetRect.right = Bmp[Tiles::BUTTON_CHOP].targetRect.left + 35;
    Bmp[Tiles::BUTTON_CHOP].targetRect.bottom = Bmp[Tiles::BUTTON_CHOP].targetRect.top + 35;
    Bmp[Tiles::BUTTON_CHOP].Width = static_cast<short>(Bmp[Tiles::BUTTON_CHOP].sourceRect.right - Bmp[Tiles::BUTTON_CHOP].sourceRect.left);
    Bmp[Tiles::BUTTON_CHOP].Height = static_cast<short>(Bmp[Tiles::BUTTON_CHOP].sourceRect.bottom - Bmp[Tiles::BUTTON_CHOP].sourceRect.top);
    Bmp[Tiles::BUTTON_CHOP].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_CHOP].Speed = 4;
    Bmp[Tiles::BUTTON_CHOP].AnimationPhase = -1;

    // BUTTANGELN
    Bmp[Tiles::BUTTON_FISH].sourceRect.left = 413;
    Bmp[Tiles::BUTTON_FISH].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_FISH].sourceRect.right = 413 + 35;
    Bmp[Tiles::BUTTON_FISH].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_FISH].targetRect.left = rcPanel.left + 29;
    Bmp[Tiles::BUTTON_FISH].targetRect.top = rcPanel.top + 325;
    Bmp[Tiles::BUTTON_FISH].targetRect.right = Bmp[Tiles::BUTTON_FISH].targetRect.left + 35;
    Bmp[Tiles::BUTTON_FISH].targetRect.bottom = Bmp[Tiles::BUTTON_FISH].targetRect.top + 35;
    Bmp[Tiles::BUTTON_FISH].Width = static_cast<short>(Bmp[Tiles::BUTTON_FISH].sourceRect.right - Bmp[Tiles::BUTTON_FISH].sourceRect.left);
    Bmp[Tiles::BUTTON_FISH].Height = static_cast<short>(Bmp[Tiles::BUTTON_FISH].sourceRect.bottom - Bmp[Tiles::BUTTON_FISH].sourceRect.top);
    Bmp[Tiles::BUTTON_FISH].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_FISH].Speed = 3;
    Bmp[Tiles::BUTTON_FISH].AnimationPhase = -1;

    // BUTTANZUENDEN
    Bmp[Tiles::BUTTON_IGNITE].sourceRect.left = 28;
    Bmp[Tiles::BUTTON_IGNITE].sourceRect.top = 175;
    Bmp[Tiles::BUTTON_IGNITE].sourceRect.right = 28 + 35;
    Bmp[Tiles::BUTTON_IGNITE].sourceRect.bottom = 175 + 35;
    Bmp[Tiles::BUTTON_IGNITE].targetRect.left = rcPanel.left + 70;
    Bmp[Tiles::BUTTON_IGNITE].targetRect.top = rcPanel.top + 325;
    Bmp[Tiles::BUTTON_IGNITE].targetRect.right = Bmp[Tiles::BUTTON_IGNITE].targetRect.left + 35;
    Bmp[Tiles::BUTTON_IGNITE].targetRect.bottom = Bmp[Tiles::BUTTON_IGNITE].targetRect.top + 35;
    Bmp[Tiles::BUTTON_IGNITE].Width = static_cast<short>(Bmp[Tiles::BUTTON_IGNITE].sourceRect.right - Bmp[Tiles::BUTTON_IGNITE].sourceRect.left);
    Bmp[Tiles::BUTTON_IGNITE].Height = static_cast<short>(Bmp[Tiles::BUTTON_IGNITE].sourceRect.bottom - Bmp[Tiles::BUTTON_IGNITE].sourceRect.top);
    Bmp[Tiles::BUTTON_IGNITE].AnimationPhaseCount = 3;
    Bmp[Tiles::BUTTON_IGNITE].Speed = 4;
    Bmp[Tiles::BUTTON_IGNITE].AnimationPhase = -1;

    // BUTTAUSSCHAU
    Bmp[Tiles::BUTTON_LOOK_OUT].sourceRect.left = 63;
    Bmp[Tiles::BUTTON_LOOK_OUT].sourceRect.top = 175;
    Bmp[Tiles::BUTTON_LOOK_OUT].sourceRect.right = 63 + 35;
    Bmp[Tiles::BUTTON_LOOK_OUT].sourceRect.bottom = 175 + 35;
    Bmp[Tiles::BUTTON_LOOK_OUT].targetRect.left = rcPanel.left + 111;
    Bmp[Tiles::BUTTON_LOOK_OUT].targetRect.top = rcPanel.top + 325;
    Bmp[Tiles::BUTTON_LOOK_OUT].targetRect.right = Bmp[Tiles::BUTTON_LOOK_OUT].targetRect.left + 35;
    Bmp[Tiles::BUTTON_LOOK_OUT].targetRect.bottom = Bmp[Tiles::BUTTON_LOOK_OUT].targetRect.top + 35;
    Bmp[Tiles::BUTTON_LOOK_OUT].Width = static_cast<short>(Bmp[Tiles::BUTTON_LOOK_OUT].sourceRect.right - Bmp[Tiles::BUTTON_LOOK_OUT].sourceRect.left);
    Bmp[Tiles::BUTTON_LOOK_OUT].Height = static_cast<short>(Bmp[Tiles::BUTTON_LOOK_OUT].sourceRect.bottom - Bmp[Tiles::BUTTON_LOOK_OUT].sourceRect.top);
    Bmp[Tiles::BUTTON_LOOK_OUT].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_LOOK_OUT].Speed = 3;
    Bmp[Tiles::BUTTON_LOOK_OUT].AnimationPhase = -1;

    // BUTTSCHATZKARTE
    Bmp[Tiles::BUTTON_TREASUREMAP].sourceRect.left = 98;
    Bmp[Tiles::BUTTON_TREASUREMAP].sourceRect.top = 175;
    Bmp[Tiles::BUTTON_TREASUREMAP].sourceRect.right = 98 + 35;
    Bmp[Tiles::BUTTON_TREASUREMAP].sourceRect.bottom = 175 + 35;
    Bmp[Tiles::BUTTON_TREASUREMAP].targetRect.left = rcPanel.left + 152;
    Bmp[Tiles::BUTTON_TREASUREMAP].targetRect.top = rcPanel.top + 325;
    Bmp[Tiles::BUTTON_TREASUREMAP].targetRect.right = Bmp[Tiles::BUTTON_TREASUREMAP].targetRect.left + 35;
    Bmp[Tiles::BUTTON_TREASUREMAP].targetRect.bottom = Bmp[Tiles::BUTTON_TREASUREMAP].targetRect.top + 35;
    Bmp[Tiles::BUTTON_TREASUREMAP].Width = static_cast<short>(Bmp[Tiles::BUTTON_TREASUREMAP].sourceRect.right - Bmp[Tiles::BUTTON_TREASUREMAP].sourceRect.left);
    Bmp[Tiles::BUTTON_TREASUREMAP].Height = static_cast<short>(Bmp[Tiles::BUTTON_TREASUREMAP].sourceRect.bottom - Bmp[Tiles::BUTTON_TREASUREMAP].sourceRect.top);
    Bmp[Tiles::BUTTON_TREASUREMAP].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_TREASUREMAP].Speed = 3;
    Bmp[Tiles::BUTTON_TREASUREMAP].AnimationPhase = -1;

    // BUTTSCHATZ
    Bmp[Tiles::BUTTON_TREASURE].sourceRect.left = 133;
    Bmp[Tiles::BUTTON_TREASURE].sourceRect.top = 175;
    Bmp[Tiles::BUTTON_TREASURE].sourceRect.right = 133 + 35;
    Bmp[Tiles::BUTTON_TREASURE].sourceRect.bottom = 175 + 35;
    Bmp[Tiles::BUTTON_TREASURE].targetRect.left = rcPanel.left + 29;
    Bmp[Tiles::BUTTON_TREASURE].targetRect.top = rcPanel.top + 380;
    Bmp[Tiles::BUTTON_TREASURE].targetRect.right = Bmp[Tiles::BUTTON_TREASURE].targetRect.left + 35;
    Bmp[Tiles::BUTTON_TREASURE].targetRect.bottom = Bmp[Tiles::BUTTON_TREASURE].targetRect.top + 35;
    Bmp[Tiles::BUTTON_TREASURE].Width = static_cast<short>(Bmp[Tiles::BUTTON_TREASURE].sourceRect.right - Bmp[Tiles::BUTTON_TREASURE].sourceRect.left);
    Bmp[Tiles::BUTTON_TREASURE].Height = static_cast<short>(Bmp[Tiles::BUTTON_TREASURE].sourceRect.bottom - Bmp[Tiles::BUTTON_TREASURE].sourceRect.top);
    Bmp[Tiles::BUTTON_TREASURE].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_TREASURE].Speed = 3;
    Bmp[Tiles::BUTTON_TREASURE].AnimationPhase = -1;

    // BUTTSCHLEUDER
    Bmp[Tiles::BUTTON_SLINGSHOT].sourceRect.left = 168;
    Bmp[Tiles::BUTTON_SLINGSHOT].sourceRect.top = 175;
    Bmp[Tiles::BUTTON_SLINGSHOT].sourceRect.right = 168 + 35;
    Bmp[Tiles::BUTTON_SLINGSHOT].sourceRect.bottom = 175 + 35;
    Bmp[Tiles::BUTTON_SLINGSHOT].targetRect.left = rcPanel.left + 70;
    Bmp[Tiles::BUTTON_SLINGSHOT].targetRect.top = rcPanel.top + 380;
    Bmp[Tiles::BUTTON_SLINGSHOT].targetRect.right = Bmp[Tiles::BUTTON_SLINGSHOT].targetRect.left + 35;
    Bmp[Tiles::BUTTON_SLINGSHOT].targetRect.bottom = Bmp[Tiles::BUTTON_SLINGSHOT].targetRect.top + 35;
    Bmp[Tiles::BUTTON_SLINGSHOT].Width = static_cast<short>(Bmp[Tiles::BUTTON_SLINGSHOT].sourceRect.right - Bmp[Tiles::BUTTON_SLINGSHOT].sourceRect.left);
    Bmp[Tiles::BUTTON_SLINGSHOT].Height = static_cast<short>(Bmp[Tiles::BUTTON_SLINGSHOT].sourceRect.bottom - Bmp[Tiles::BUTTON_SLINGSHOT].sourceRect.top);
    Bmp[Tiles::BUTTON_SLINGSHOT].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_SLINGSHOT].Speed = 3;
    Bmp[Tiles::BUTTON_SLINGSHOT].AnimationPhase = -1;

    // BUTTFELD
    Bmp[Tiles::BUTTON_FARM].sourceRect.left = 238;
    Bmp[Tiles::BUTTON_FARM].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_FARM].sourceRect.right = 238 + 35;
    Bmp[Tiles::BUTTON_FARM].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_FARM].targetRect.left = rcPanel.left + 70;
    Bmp[Tiles::BUTTON_FARM].targetRect.top = rcPanel.top + 270;
    Bmp[Tiles::BUTTON_FARM].targetRect.right = Bmp[Tiles::BUTTON_FARM].targetRect.left + 35;
    Bmp[Tiles::BUTTON_FARM].targetRect.bottom = Bmp[Tiles::BUTTON_FARM].targetRect.top + 35;
    Bmp[Tiles::BUTTON_FARM].Width = static_cast<short>(Bmp[Tiles::BUTTON_FARM].sourceRect.right - Bmp[Tiles::BUTTON_FARM].sourceRect.left);
    Bmp[Tiles::BUTTON_FARM].Height = static_cast<short>(Bmp[Tiles::BUTTON_FARM].sourceRect.bottom - Bmp[Tiles::BUTTON_FARM].sourceRect.top);
    Bmp[Tiles::BUTTON_FARM].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_FARM].Speed = 3;
    Bmp[Tiles::BUTTON_FARM].AnimationPhase = -1;

    // BUTTZELT
    Bmp[Tiles::BUTTON_TENT].sourceRect.left = 273;
    Bmp[Tiles::BUTTON_TENT].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_TENT].sourceRect.right = 273 + 35;
    Bmp[Tiles::BUTTON_TENT].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_TENT].targetRect.left = rcPanel.left + 29;
    Bmp[Tiles::BUTTON_TENT].targetRect.top = rcPanel.top + 270;
    Bmp[Tiles::BUTTON_TENT].targetRect.right = Bmp[Tiles::BUTTON_TENT].targetRect.left + 35;
    Bmp[Tiles::BUTTON_TENT].targetRect.bottom = Bmp[Tiles::BUTTON_TENT].targetRect.top + 35;
    Bmp[Tiles::BUTTON_TENT].Width = static_cast<short>(Bmp[Tiles::BUTTON_TENT].sourceRect.right - Bmp[Tiles::BUTTON_TENT].sourceRect.left);
    Bmp[Tiles::BUTTON_TENT].Height = static_cast<short>(Bmp[Tiles::BUTTON_TENT].sourceRect.bottom - Bmp[Tiles::BUTTON_TENT].sourceRect.top);
    Bmp[Tiles::BUTTON_TENT].AnimationPhaseCount = 3;
    Bmp[Tiles::BUTTON_TENT].Speed = 3;
    Bmp[Tiles::BUTTON_TENT].AnimationPhase = 0;

    // BUTTBOOT
    Bmp[Tiles::BUTTON_BOAT].sourceRect.left = 448;
    Bmp[Tiles::BUTTON_BOAT].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_BOAT].sourceRect.right = 448 + 35;
    Bmp[Tiles::BUTTON_BOAT].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_BOAT].targetRect.left = rcPanel.left + 111;
    Bmp[Tiles::BUTTON_BOAT].targetRect.top = rcPanel.top + 270;
    Bmp[Tiles::BUTTON_BOAT].targetRect.right = Bmp[Tiles::BUTTON_BOAT].targetRect.left + 35;
    Bmp[Tiles::BUTTON_BOAT].targetRect.bottom = Bmp[Tiles::BUTTON_BOAT].targetRect.top + 35;
    Bmp[Tiles::BUTTON_BOAT].Width = static_cast<short>(Bmp[Tiles::BUTTON_BOAT].sourceRect.right - Bmp[Tiles::BUTTON_BOAT].sourceRect.left);
    Bmp[Tiles::BUTTON_BOAT].Height = static_cast<short>(Bmp[Tiles::BUTTON_BOAT].sourceRect.bottom - Bmp[Tiles::BUTTON_BOAT].sourceRect.top);
    Bmp[Tiles::BUTTON_BOAT].AnimationPhaseCount = 3;
    Bmp[Tiles::BUTTON_BOAT].Speed = 3;
    Bmp[Tiles::BUTTON_BOAT].AnimationPhase = -1;

    // BUTTROHR
    Bmp[Tiles::BUTTON_PIPE].sourceRect.left = 518;
    Bmp[Tiles::BUTTON_PIPE].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_PIPE].sourceRect.right = 518 + 35;
    Bmp[Tiles::BUTTON_PIPE].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_PIPE].targetRect.left = rcPanel.left + 152;
    Bmp[Tiles::BUTTON_PIPE].targetRect.top = rcPanel.top + 270;
    Bmp[Tiles::BUTTON_PIPE].targetRect.right = Bmp[Tiles::BUTTON_PIPE].targetRect.left + 35;
    Bmp[Tiles::BUTTON_PIPE].targetRect.bottom = Bmp[Tiles::BUTTON_PIPE].targetRect.top + 35;
    Bmp[Tiles::BUTTON_PIPE].Width = static_cast<short>(Bmp[Tiles::BUTTON_PIPE].sourceRect.right - Bmp[Tiles::BUTTON_PIPE].sourceRect.left);
    Bmp[Tiles::BUTTON_PIPE].Height = static_cast<short>(Bmp[Tiles::BUTTON_PIPE].sourceRect.bottom - Bmp[Tiles::BUTTON_PIPE].sourceRect.top);
    Bmp[Tiles::BUTTON_PIPE].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_PIPE].Speed = 2;
    Bmp[Tiles::BUTTON_PIPE].AnimationPhase = -1;

    // BUTTSOS
    Bmp[Tiles::BUTTON_SOS].sourceRect.left = 588;
    Bmp[Tiles::BUTTON_SOS].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_SOS].sourceRect.right = 588 + 35;
    Bmp[Tiles::BUTTON_SOS].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_SOS].targetRect.left = rcPanel.left + 29;
    Bmp[Tiles::BUTTON_SOS].targetRect.top = rcPanel.top + 325;
    Bmp[Tiles::BUTTON_SOS].targetRect.right = Bmp[Tiles::BUTTON_SOS].targetRect.left + 35;
    Bmp[Tiles::BUTTON_SOS].targetRect.bottom = Bmp[Tiles::BUTTON_SOS].targetRect.top + 35;
    Bmp[Tiles::BUTTON_SOS].Width = static_cast<short>(Bmp[Tiles::BUTTON_SOS].sourceRect.right - Bmp[Tiles::BUTTON_SOS].sourceRect.left);
    Bmp[Tiles::BUTTON_SOS].Height = static_cast<short>(Bmp[Tiles::BUTTON_SOS].sourceRect.bottom - Bmp[Tiles::BUTTON_SOS].sourceRect.top);
    Bmp[Tiles::BUTTON_SOS].AnimationPhaseCount = 3;
    Bmp[Tiles::BUTTON_SOS].Speed = 2;

    // BUTTHAUS1
    Bmp[Tiles::BUTTON_HOUSE_1].sourceRect.left = 623;
    Bmp[Tiles::BUTTON_HOUSE_1].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_HOUSE_1].sourceRect.right = 623 + 35;
    Bmp[Tiles::BUTTON_HOUSE_1].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_HOUSE_1].targetRect.left = rcPanel.left + 70;
    Bmp[Tiles::BUTTON_HOUSE_1].targetRect.top = rcPanel.top + 325;
    Bmp[Tiles::BUTTON_HOUSE_1].targetRect.right = Bmp[Tiles::BUTTON_HOUSE_1].targetRect.left + 35;
    Bmp[Tiles::BUTTON_HOUSE_1].targetRect.bottom = Bmp[Tiles::BUTTON_HOUSE_1].targetRect.top + 35;
    Bmp[Tiles::BUTTON_HOUSE_1].Width = static_cast<short>(Bmp[Tiles::BUTTON_HOUSE_1].sourceRect.right - Bmp[Tiles::BUTTON_HOUSE_1].sourceRect.left);
    Bmp[Tiles::BUTTON_HOUSE_1].Height = static_cast<short>(Bmp[Tiles::BUTTON_HOUSE_1].sourceRect.bottom - Bmp[Tiles::BUTTON_HOUSE_1].sourceRect.top);
    Bmp[Tiles::BUTTON_HOUSE_1].AnimationPhaseCount = 5;
    Bmp[Tiles::BUTTON_HOUSE_1].Speed = 3;
    Bmp[Tiles::BUTTON_HOUSE_1].AnimationPhase = -1;

    // BUTTHAUS2
    Bmp[Tiles::BUTTON_HOUSE_2].sourceRect.left = 658;
    Bmp[Tiles::BUTTON_HOUSE_2].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_HOUSE_2].sourceRect.right = 658 + 35;
    Bmp[Tiles::BUTTON_HOUSE_2].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_HOUSE_2].targetRect.left = rcPanel.left + 111;
    Bmp[Tiles::BUTTON_HOUSE_2].targetRect.top = rcPanel.top + 325;
    Bmp[Tiles::BUTTON_HOUSE_2].targetRect.right = Bmp[Tiles::BUTTON_HOUSE_2].targetRect.left + 35;
    Bmp[Tiles::BUTTON_HOUSE_2].targetRect.bottom = Bmp[Tiles::BUTTON_HOUSE_2].targetRect.top + 35;
    Bmp[Tiles::BUTTON_HOUSE_2].Width = static_cast<short>(Bmp[Tiles::BUTTON_HOUSE_2].sourceRect.right - Bmp[Tiles::BUTTON_HOUSE_2].sourceRect.left);
    Bmp[Tiles::BUTTON_HOUSE_2].Height = static_cast<short>(Bmp[Tiles::BUTTON_HOUSE_2].sourceRect.bottom - Bmp[Tiles::BUTTON_HOUSE_2].sourceRect.top);
    Bmp[Tiles::BUTTON_HOUSE_2].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_HOUSE_2].Speed = 3;
    Bmp[Tiles::BUTTON_HOUSE_2].AnimationPhase = -1;

    // BUTTHAUS3
    Bmp[Tiles::BUTTON_HOUSE_3].sourceRect.left = 693;
    Bmp[Tiles::BUTTON_HOUSE_3].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_HOUSE_3].sourceRect.right = 693 + 35;
    Bmp[Tiles::BUTTON_HOUSE_3].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_HOUSE_3].targetRect.left = rcPanel.left + 152;
    Bmp[Tiles::BUTTON_HOUSE_3].targetRect.top = rcPanel.top + 325;
    Bmp[Tiles::BUTTON_HOUSE_3].targetRect.right = Bmp[Tiles::BUTTON_HOUSE_3].targetRect.left + 35;
    Bmp[Tiles::BUTTON_HOUSE_3].targetRect.bottom = Bmp[Tiles::BUTTON_HOUSE_3].targetRect.top + 35;
    Bmp[Tiles::BUTTON_HOUSE_3].Width = static_cast<short>(Bmp[Tiles::BUTTON_HOUSE_3].sourceRect.right - Bmp[Tiles::BUTTON_HOUSE_3].sourceRect.left);
    Bmp[Tiles::BUTTON_HOUSE_3].Height = static_cast<short>(Bmp[Tiles::BUTTON_HOUSE_3].sourceRect.bottom - Bmp[Tiles::BUTTON_HOUSE_3].sourceRect.top);
    Bmp[Tiles::BUTTON_HOUSE_3].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_HOUSE_3].Speed = 3;
    Bmp[Tiles::BUTTON_HOUSE_3].AnimationPhase = -1;

    // BUTTFEUERST
    Bmp[Tiles::BUTTON_FIRE].sourceRect.left = 728;
    Bmp[Tiles::BUTTON_FIRE].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_FIRE].sourceRect.right = 728 + 35;
    Bmp[Tiles::BUTTON_FIRE].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_FIRE].targetRect.left = rcPanel.left + 29;
    Bmp[Tiles::BUTTON_FIRE].targetRect.top = rcPanel.top + 380;
    Bmp[Tiles::BUTTON_FIRE].targetRect.right = Bmp[Tiles::BUTTON_FIRE].targetRect.left + 35;
    Bmp[Tiles::BUTTON_FIRE].targetRect.bottom = Bmp[Tiles::BUTTON_FIRE].targetRect.top + 35;
    Bmp[Tiles::BUTTON_FIRE].Width = static_cast<short>(Bmp[Tiles::BUTTON_FIRE].sourceRect.right - Bmp[Tiles::BUTTON_FIRE].sourceRect.left);
    Bmp[Tiles::BUTTON_FIRE].Height = static_cast<short>(Bmp[Tiles::BUTTON_FIRE].sourceRect.bottom - Bmp[Tiles::BUTTON_FIRE].sourceRect.top);
    Bmp[Tiles::BUTTON_FIRE].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_FIRE].Speed = 3;

    // BUTTFRAGEZ
    Bmp[Tiles::BUTTON_QUESTION].sourceRect.left = 203;
    Bmp[Tiles::BUTTON_QUESTION].sourceRect.top = 175;
    Bmp[Tiles::BUTTON_QUESTION].sourceRect.right = 203 + 35;
    Bmp[Tiles::BUTTON_QUESTION].sourceRect.bottom = 175 + 35;
    Bmp[Tiles::BUTTON_QUESTION].targetRect.left = 0;
    Bmp[Tiles::BUTTON_QUESTION].targetRect.top = 0;
    Bmp[Tiles::BUTTON_QUESTION].targetRect.right = 0 + 35;
    Bmp[Tiles::BUTTON_QUESTION].targetRect.bottom = 0 + 35;
    Bmp[Tiles::BUTTON_QUESTION].Width = static_cast<short>(Bmp[Tiles::BUTTON_QUESTION].sourceRect.right - Bmp[Tiles::BUTTON_QUESTION].sourceRect.left);
    Bmp[Tiles::BUTTON_QUESTION].Height = static_cast<short>(Bmp[Tiles::BUTTON_QUESTION].sourceRect.bottom - Bmp[Tiles::BUTTON_QUESTION].sourceRect.top);
    Bmp[Tiles::BUTTON_QUESTION].AnimationPhaseCount = 1;
    Bmp[Tiles::BUTTON_QUESTION].Speed = 0;

    // BUTTDESTROY
    Bmp[Tiles::BUTTON_DESTROY].sourceRect.left = 553;
    Bmp[Tiles::BUTTON_DESTROY].sourceRect.top = 0;
    Bmp[Tiles::BUTTON_DESTROY].sourceRect.right = 553 + 35;
    Bmp[Tiles::BUTTON_DESTROY].sourceRect.bottom = 35;
    Bmp[Tiles::BUTTON_DESTROY].targetRect.left = rcPanel.left + 152;
    Bmp[Tiles::BUTTON_DESTROY].targetRect.top = rcPanel.top + 380;
    Bmp[Tiles::BUTTON_DESTROY].targetRect.right = Bmp[Tiles::BUTTON_DESTROY].targetRect.left + 35;
    Bmp[Tiles::BUTTON_DESTROY].targetRect.bottom = Bmp[Tiles::BUTTON_DESTROY].targetRect.top + 35;
    Bmp[Tiles::BUTTON_DESTROY].Width = static_cast<short>(Bmp[Tiles::BUTTON_DESTROY].sourceRect.right - Bmp[Tiles::BUTTON_DESTROY].sourceRect.left);
    Bmp[Tiles::BUTTON_DESTROY].Height = static_cast<short>(Bmp[Tiles::BUTTON_DESTROY].sourceRect.bottom - Bmp[Tiles::BUTTON_DESTROY].sourceRect.top);
    Bmp[Tiles::BUTTON_DESTROY].AnimationPhaseCount = 4;
    Bmp[Tiles::BUTTON_DESTROY].Speed = 5;

    // SpzAni
    for (i = Tiles::TREE_DOWN_1; i <= Tiles::TREE_DOWN_4; i++) {
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
    Bmp[Tiles::TREE_DOWN_1].sourceRect.left = 101;
    Bmp[Tiles::TREE_DOWN_1].sourceRect.top = 0;
    Bmp[Tiles::TREE_DOWN_1].sourceRect.right = 101 + 35;
    Bmp[Tiles::TREE_DOWN_1].sourceRect.bottom = 27;
    Bmp[Tiles::TREE_DOWN_1].Width = static_cast<short>(Bmp[Tiles::TREE_DOWN_1].sourceRect.right - Bmp[Tiles::TREE_DOWN_1].sourceRect.left);
    Bmp[Tiles::TREE_DOWN_1].Height = static_cast<short>(Bmp[Tiles::TREE_DOWN_1].sourceRect.bottom - Bmp[Tiles::TREE_DOWN_1].sourceRect.top);

    // BAUM2DOWN
    Bmp[Tiles::TREE_DOWN_2].sourceRect.left = 136;
    Bmp[Tiles::TREE_DOWN_2].sourceRect.top = 0;
    Bmp[Tiles::TREE_DOWN_2].sourceRect.right = 136 + 36;
    Bmp[Tiles::TREE_DOWN_2].sourceRect.bottom = 27;
    Bmp[Tiles::TREE_DOWN_2].Width = static_cast<short>(Bmp[Tiles::TREE_DOWN_2].sourceRect.right - Bmp[Tiles::TREE_DOWN_2].sourceRect.left);
    Bmp[Tiles::TREE_DOWN_2].Height = static_cast<short>(Bmp[Tiles::TREE_DOWN_2].sourceRect.bottom - Bmp[Tiles::TREE_DOWN_2].sourceRect.top);

    // BAUM3DOWN
    Bmp[Tiles::TREE_DOWN_3].sourceRect.left = 172;
    Bmp[Tiles::TREE_DOWN_3].sourceRect.top = 0;
    Bmp[Tiles::TREE_DOWN_3].sourceRect.right = 172 + 34;
    Bmp[Tiles::TREE_DOWN_3].sourceRect.bottom = 28;
    Bmp[Tiles::TREE_DOWN_3].Width = static_cast<short>(Bmp[Tiles::TREE_DOWN_3].sourceRect.right - Bmp[Tiles::TREE_DOWN_3].sourceRect.left);
    Bmp[Tiles::TREE_DOWN_3].Height = static_cast<short>(Bmp[Tiles::TREE_DOWN_3].sourceRect.bottom - Bmp[Tiles::TREE_DOWN_3].sourceRect.top);

    // BAUM4DOWN
    Bmp[Tiles::TREE_DOWN_4].sourceRect.left = 206;
    Bmp[Tiles::TREE_DOWN_4].sourceRect.top = 0;
    Bmp[Tiles::TREE_DOWN_4].sourceRect.right = 206 + 32;
    Bmp[Tiles::TREE_DOWN_4].sourceRect.bottom = 17;
    Bmp[Tiles::TREE_DOWN_4].Width = static_cast<short>(Bmp[Tiles::TREE_DOWN_4].sourceRect.right - Bmp[Tiles::TREE_DOWN_4].sourceRect.left);
    Bmp[Tiles::TREE_DOWN_4].Height = static_cast<short>(Bmp[Tiles::TREE_DOWN_4].sourceRect.bottom - Bmp[Tiles::TREE_DOWN_4].sourceRect.top);

    // Sonstiges

    // Säule1
    Bmp[Tiles::COLUMN_1].AnimationPhaseCount = 1;
    Bmp[Tiles::COLUMN_1].sourceRect.left = 205;
    Bmp[Tiles::COLUMN_1].sourceRect.top = 115;
    Bmp[Tiles::COLUMN_1].sourceRect.right = Bmp[Tiles::COLUMN_1].sourceRect.left + 11;
    Bmp[Tiles::COLUMN_1].sourceRect.bottom = Bmp[Tiles::COLUMN_1].sourceRect.top + 115;
    Bmp[Tiles::COLUMN_1].targetRect.left = rcPanel.left + 52;
    Bmp[Tiles::COLUMN_1].targetRect.top = rcPanel.top + 471;
    Bmp[Tiles::COLUMN_1].targetRect.right = Bmp[Tiles::COLUMN_1].targetRect.left + 11;
    Bmp[Tiles::COLUMN_1].targetRect.bottom = Bmp[Tiles::COLUMN_1].targetRect.top + 115;
    Bmp[Tiles::COLUMN_1].Width = static_cast<short>(Bmp[Tiles::COLUMN_1].sourceRect.right - Bmp[Tiles::COLUMN_1].sourceRect.left);
    Bmp[Tiles::COLUMN_1].Height = static_cast<short>(Bmp[Tiles::COLUMN_1].sourceRect.bottom - Bmp[Tiles::COLUMN_1].sourceRect.top);
    Bmp[Tiles::COLUMN_1].Surface = lpDDSPanel;

    // Säule2
    Bmp[Tiles::COLUMN_2].AnimationPhaseCount = 1;
    Bmp[Tiles::COLUMN_2].sourceRect.left = 216;
    Bmp[Tiles::COLUMN_2].sourceRect.top = 115;
    Bmp[Tiles::COLUMN_2].sourceRect.right = Bmp[Tiles::COLUMN_2].sourceRect.left + 11;
    Bmp[Tiles::COLUMN_2].sourceRect.bottom = Bmp[Tiles::COLUMN_2].sourceRect.top + 115;
    Bmp[Tiles::COLUMN_2].targetRect.left = rcPanel.left + 114;
    Bmp[Tiles::COLUMN_2].targetRect.top = rcPanel.top + 471;
    Bmp[Tiles::COLUMN_2].targetRect.right = Bmp[Tiles::COLUMN_2].targetRect.left + 11;
    Bmp[Tiles::COLUMN_2].targetRect.bottom = Bmp[Tiles::COLUMN_2].targetRect.top + 115;
    Bmp[Tiles::COLUMN_2].Width = static_cast<short>(Bmp[Tiles::COLUMN_2].sourceRect.right - Bmp[Tiles::COLUMN_2].sourceRect.left);
    Bmp[Tiles::COLUMN_2].Height = static_cast<short>(Bmp[Tiles::COLUMN_2].sourceRect.bottom - Bmp[Tiles::COLUMN_2].sourceRect.top);
    Bmp[Tiles::COLUMN_2].Surface = lpDDSPanel;

    // Säule3
    Bmp[Tiles::COLUMN_3].AnimationPhaseCount = 1;
    Bmp[Tiles::COLUMN_3].sourceRect.left = 227;
    Bmp[Tiles::COLUMN_3].sourceRect.top = 115;
    Bmp[Tiles::COLUMN_3].sourceRect.right = Bmp[Tiles::COLUMN_3].sourceRect.left + 11;
    Bmp[Tiles::COLUMN_3].sourceRect.bottom = Bmp[Tiles::COLUMN_3].sourceRect.top + 115;
    Bmp[Tiles::COLUMN_3].targetRect.left = rcPanel.left + 175;
    Bmp[Tiles::COLUMN_3].targetRect.top = rcPanel.top + 471;
    Bmp[Tiles::COLUMN_3].targetRect.right = Bmp[Tiles::COLUMN_3].targetRect.left + 11;
    Bmp[Tiles::COLUMN_3].targetRect.bottom = Bmp[Tiles::COLUMN_3].targetRect.top + 115;
    Bmp[Tiles::COLUMN_3].Width = static_cast<short>(Bmp[Tiles::COLUMN_3].sourceRect.right - Bmp[Tiles::COLUMN_3].sourceRect.left);
    Bmp[Tiles::COLUMN_3].Height = static_cast<short>(Bmp[Tiles::COLUMN_3].sourceRect.bottom - Bmp[Tiles::COLUMN_3].sourceRect.top);
    Bmp[Tiles::COLUMN_3].Surface = lpDDSPanel;

    // Rohstoffe
    for (i = Tiles::RAW_TREE_BRANCH; i <= Tiles::RAW_SLINGSHOT; i++) {
        Bmp[i].AnimationPhaseCount = 1;
        Bmp[i].sourceRect.left = 18 + (i - Tiles::RAW_TREE_BRANCH) * 16;
        Bmp[i].sourceRect.top = 0;
        Bmp[i].sourceRect.right = Bmp[i].sourceRect.left + 16;
        Bmp[i].sourceRect.bottom = Bmp[i].sourceRect.top + 15;
        Bmp[i].Width = static_cast<short>(Bmp[i].sourceRect.right - Bmp[i].sourceRect.left);
        Bmp[i].Height = static_cast<short>(Bmp[i].sourceRect.bottom - Bmp[i].sourceRect.top);
        Bmp[i].Surface = lpDDSInventar;
    }

    // RohAst
    Bmp[Tiles::RAW_TREE_BRANCH].targetRect.left = rcPanel.left + 34;
    Bmp[Tiles::RAW_TREE_BRANCH].targetRect.top = rcPanel.top + 280;
    Bmp[Tiles::RAW_TREE_BRANCH].targetRect.right = Bmp[Tiles::RAW_TREE_BRANCH].targetRect.left + 16;
    Bmp[Tiles::RAW_TREE_BRANCH].targetRect.bottom = Bmp[Tiles::RAW_TREE_BRANCH].targetRect.top + 15;
    // ROHSTEIN
    Bmp[Tiles::RAW_STONE].targetRect.left = rcPanel.left + 34;
    Bmp[Tiles::RAW_STONE].targetRect.top = rcPanel.top + 300;
    Bmp[Tiles::RAW_STONE].targetRect.right = Bmp[Tiles::RAW_STONE].targetRect.left + 16;
    Bmp[Tiles::RAW_STONE].targetRect.bottom = Bmp[Tiles::RAW_STONE].targetRect.top + 15;
    // ROHSTAMM
    Bmp[Tiles::RAW_TREE_TRUNK].targetRect.left = rcPanel.left + 34;
    Bmp[Tiles::RAW_TREE_TRUNK].targetRect.top = rcPanel.top + 360;
    Bmp[Tiles::RAW_TREE_TRUNK].targetRect.right = Bmp[Tiles::RAW_TREE_TRUNK].targetRect.left + 16;
    Bmp[Tiles::RAW_TREE_TRUNK].targetRect.bottom = Bmp[Tiles::RAW_TREE_TRUNK].targetRect.top + 15;
    // ROHAXT
    Bmp[Tiles::RAW_AXE].targetRect.left = rcPanel.left + 150;
    Bmp[Tiles::RAW_AXE].targetRect.top = rcPanel.top + 280;
    Bmp[Tiles::RAW_AXE].targetRect.right = Bmp[Tiles::RAW_AXE].targetRect.left + 16;
    Bmp[Tiles::RAW_AXE].targetRect.bottom = Bmp[Tiles::RAW_AXE].targetRect.top + 15;
    // ROHBLATT
    Bmp[Tiles::RAW_LEAF].targetRect.left = rcPanel.left + 34;
    Bmp[Tiles::RAW_LEAF].targetRect.top = rcPanel.top + 320;
    Bmp[Tiles::RAW_LEAF].targetRect.right = Bmp[Tiles::RAW_LEAF].targetRect.left + 16;
    Bmp[Tiles::RAW_LEAF].targetRect.bottom = Bmp[Tiles::RAW_LEAF].targetRect.top + 15;
    // ROHEGGE
    Bmp[Tiles::RAW_HOE].targetRect.left = rcPanel.left + 150;
    Bmp[Tiles::RAW_HOE].targetRect.top = rcPanel.top + 300;
    Bmp[Tiles::RAW_HOE].targetRect.right = Bmp[Tiles::RAW_HOE].targetRect.left + 16;
    Bmp[Tiles::RAW_HOE].targetRect.bottom = Bmp[Tiles::RAW_HOE].targetRect.top + 15;
    // ROHLIANE
    Bmp[Tiles::RAW_LIANA].targetRect.left = rcPanel.left + 34;
    Bmp[Tiles::RAW_LIANA].targetRect.top = rcPanel.top + 340;
    Bmp[Tiles::RAW_LIANA].targetRect.right = Bmp[Tiles::RAW_LIANA].targetRect.left + 16;
    Bmp[Tiles::RAW_LIANA].targetRect.bottom = Bmp[Tiles::RAW_LIANA].targetRect.top + 15;
    // ROHANGEL
    Bmp[Tiles::RAW_FISHING_POLE].targetRect.left = rcPanel.left + 150;
    Bmp[Tiles::RAW_FISHING_POLE].targetRect.top = rcPanel.top + 320;
    Bmp[Tiles::RAW_FISHING_POLE].targetRect.right = Bmp[Tiles::RAW_FISHING_POLE].targetRect.left + 16;
    Bmp[Tiles::RAW_FISHING_POLE].targetRect.bottom = Bmp[Tiles::RAW_FISHING_POLE].targetRect.top + 15;
    // ROHHAMMER
    Bmp[Tiles::RAW_HAMMER].targetRect.left = rcPanel.left + 150;
    Bmp[Tiles::RAW_HAMMER].targetRect.top = rcPanel.top + 340;
    Bmp[Tiles::RAW_HAMMER].targetRect.right = Bmp[Tiles::RAW_HAMMER].targetRect.left + 16;
    Bmp[Tiles::RAW_HAMMER].targetRect.bottom = Bmp[Tiles::RAW_HAMMER].targetRect.top + 15;
    // ROHFERNROHR
    Bmp[Tiles::RAW_TELESCOPE].targetRect.left = rcPanel.left + 150;
    Bmp[Tiles::RAW_TELESCOPE].targetRect.top = rcPanel.top + 360;
    Bmp[Tiles::RAW_TELESCOPE].targetRect.right = Bmp[Tiles::RAW_TELESCOPE].targetRect.left + 16;
    Bmp[Tiles::RAW_TELESCOPE].targetRect.bottom = Bmp[Tiles::RAW_TELESCOPE].targetRect.top + 15;
    // ROHSTREICHHOLZ
    Bmp[Tiles::RAW_MATCH].targetRect.left = rcPanel.left + 105;
    Bmp[Tiles::RAW_MATCH].targetRect.top = rcPanel.top + 280;
    Bmp[Tiles::RAW_MATCH].targetRect.right = Bmp[Tiles::RAW_MATCH].targetRect.left + 16;
    Bmp[Tiles::RAW_MATCH].targetRect.bottom = Bmp[Tiles::RAW_MATCH].targetRect.top + 15;
    // ROHSCHAUFEL
    Bmp[Tiles::RAW_SHOVEL].targetRect.left = rcPanel.left + 105;
    Bmp[Tiles::RAW_SHOVEL].targetRect.top = rcPanel.top + 320;
    Bmp[Tiles::RAW_SHOVEL].targetRect.right = Bmp[Tiles::RAW_SHOVEL].targetRect.left + 16;
    Bmp[Tiles::RAW_SHOVEL].targetRect.bottom = Bmp[Tiles::RAW_SHOVEL].targetRect.top + 15;
    // ROHKARTE
    Bmp[Tiles::RAW_MAP].targetRect.left = rcPanel.left + 105;
    Bmp[Tiles::RAW_MAP].targetRect.top = rcPanel.top + 320;
    Bmp[Tiles::RAW_MAP].targetRect.right = Bmp[Tiles::RAW_MAP].targetRect.left + 16;
    Bmp[Tiles::RAW_MAP].targetRect.bottom = Bmp[Tiles::RAW_MAP].targetRect.top + 15;
    // ROHSCHLEUDER
    Bmp[Tiles::RAW_SLINGSHOT].targetRect.left = rcPanel.left + 105;
    Bmp[Tiles::RAW_SLINGSHOT].targetRect.top = rcPanel.top + 340;
    Bmp[Tiles::RAW_SLINGSHOT].targetRect.right = Bmp[Tiles::RAW_SLINGSHOT].targetRect.left + 16;
    Bmp[Tiles::RAW_SLINGSHOT].targetRect.bottom = Bmp[Tiles::RAW_SLINGSHOT].targetRect.top + 15;

    // ROEMISCH1
    Bmp[Tiles::ROEMISCH1].AnimationPhaseCount = 1;
    Bmp[Tiles::ROEMISCH1].sourceRect.left = 0;
    Bmp[Tiles::ROEMISCH1].sourceRect.top = 0;
    Bmp[Tiles::ROEMISCH1].sourceRect.right = Bmp[Tiles::ROEMISCH1].sourceRect.left + 3;
    Bmp[Tiles::ROEMISCH1].sourceRect.bottom = Bmp[Tiles::ROEMISCH1].sourceRect.top + 13;
    Bmp[Tiles::ROEMISCH1].targetRect.left = rcPanel.left + 50;
    Bmp[Tiles::ROEMISCH1].targetRect.top = rcPanel.top;
    Bmp[Tiles::ROEMISCH1].targetRect.right = Bmp[Tiles::ROEMISCH1].targetRect.left + 3;
    Bmp[Tiles::ROEMISCH1].targetRect.bottom = Bmp[Tiles::ROEMISCH1].targetRect.top + 13;
    Bmp[Tiles::ROEMISCH1].Width = static_cast<short>(Bmp[Tiles::ROEMISCH1].sourceRect.right - Bmp[Tiles::ROEMISCH1].sourceRect.left);
    Bmp[Tiles::ROEMISCH1].Height = static_cast<short>(Bmp[Tiles::ROEMISCH1].sourceRect.bottom - Bmp[Tiles::ROEMISCH1].sourceRect.top);
    Bmp[Tiles::ROEMISCH1].Surface = lpDDSInventar;

    // ROEMISCH2
    Bmp[Tiles::ROEMISCH2].AnimationPhaseCount = 1;
    Bmp[Tiles::ROEMISCH2].sourceRect.left = 3;
    Bmp[Tiles::ROEMISCH2].sourceRect.top = 0;
    Bmp[Tiles::ROEMISCH2].sourceRect.right = Bmp[Tiles::ROEMISCH2].sourceRect.left + 15;
    Bmp[Tiles::ROEMISCH2].sourceRect.bottom = Bmp[Tiles::ROEMISCH2].sourceRect.top + 10;
    Bmp[Tiles::ROEMISCH2].targetRect.left = rcPanel.left + 50;
    Bmp[Tiles::ROEMISCH2].targetRect.top = rcPanel.top;
    Bmp[Tiles::ROEMISCH2].targetRect.right = Bmp[Tiles::ROEMISCH2].targetRect.left + 15;
    Bmp[Tiles::ROEMISCH2].targetRect.bottom = Bmp[Tiles::ROEMISCH2].targetRect.top + 10;
    Bmp[Tiles::ROEMISCH2].Width = static_cast<short>(Bmp[Tiles::ROEMISCH2].sourceRect.right - Bmp[Tiles::ROEMISCH2].sourceRect.left);
    Bmp[Tiles::ROEMISCH2].Height = static_cast<short>(Bmp[Tiles::ROEMISCH2].sourceRect.bottom - Bmp[Tiles::ROEMISCH2].sourceRect.top);
    Bmp[Tiles::ROEMISCH2].Surface = lpDDSInventar;

    // INVPAPIER
    Bmp[Tiles::INVENTORY_PAPER].AnimationPhaseCount = 1;
    Bmp[Tiles::INVENTORY_PAPER].sourceRect.left = 0;
    Bmp[Tiles::INVENTORY_PAPER].sourceRect.top = 15;
    Bmp[Tiles::INVENTORY_PAPER].sourceRect.right = Bmp[Tiles::INVENTORY_PAPER].sourceRect.left + 178;
    Bmp[Tiles::INVENTORY_PAPER].sourceRect.bottom = Bmp[Tiles::INVENTORY_PAPER].sourceRect.top + 114;
    Bmp[Tiles::INVENTORY_PAPER].targetRect.left = rcPanel.left + 15;
    Bmp[Tiles::INVENTORY_PAPER].targetRect.top = rcPanel.top + 270;
    Bmp[Tiles::INVENTORY_PAPER].targetRect.right = Bmp[Tiles::INVENTORY_PAPER].targetRect.left + 178;
    Bmp[Tiles::INVENTORY_PAPER].targetRect.bottom = Bmp[Tiles::INVENTORY_PAPER].targetRect.top + 114;
    Bmp[Tiles::INVENTORY_PAPER].Width = static_cast<short>(Bmp[Tiles::INVENTORY_PAPER].sourceRect.right - Bmp[Tiles::INVENTORY_PAPER].sourceRect.left);
    Bmp[Tiles::INVENTORY_PAPER].Height = static_cast<short>(Bmp[Tiles::INVENTORY_PAPER].sourceRect.bottom - Bmp[Tiles::INVENTORY_PAPER].sourceRect.top);
    Bmp[Tiles::INVENTORY_PAPER].Surface = lpDDSInventar;

    // RING
    Bmp[Tiles::RING].AnimationPhaseCount = 1;
    Bmp[Tiles::RING].sourceRect.left = 205;
    Bmp[Tiles::RING].sourceRect.top = 230;
    Bmp[Tiles::RING].sourceRect.right = Bmp[Tiles::RING].sourceRect.left + 37;
    Bmp[Tiles::RING].sourceRect.bottom = Bmp[Tiles::RING].sourceRect.top + 150;
    Bmp[Tiles::RING].targetRect.left = rcPanel.left + 5;
    Bmp[Tiles::RING].targetRect.top = rcPanel.top - 113;
    Bmp[Tiles::RING].targetRect.right = Bmp[Tiles::RING].targetRect.left + 30;
    Bmp[Tiles::RING].targetRect.bottom = Bmp[Tiles::RING].targetRect.top;
    Bmp[Tiles::RING].Width = static_cast<short>(Bmp[Tiles::RING].sourceRect.right - Bmp[Tiles::RING].sourceRect.left);
    Bmp[Tiles::RING].Height = static_cast<short>(Bmp[Tiles::RING].sourceRect.bottom - Bmp[Tiles::RING].sourceRect.top);
    Bmp[Tiles::RING].Surface = lpDDSPanel;

    // KREUZ
    Bmp[Tiles::CROSS].AnimationPhaseCount = 1;
    Bmp[Tiles::CROSS].sourceRect.left = 205;
    Bmp[Tiles::CROSS].sourceRect.top = 380;
    Bmp[Tiles::CROSS].sourceRect.right = Bmp[Tiles::CROSS].sourceRect.left + 40;
    Bmp[Tiles::CROSS].sourceRect.bottom = Bmp[Tiles::CROSS].sourceRect.top + 22;
    Bmp[Tiles::CROSS].Width = static_cast<short>(Bmp[Tiles::CROSS].sourceRect.right - Bmp[Tiles::CROSS].sourceRect.left);
    Bmp[Tiles::CROSS].Height = static_cast<short>(Bmp[Tiles::CROSS].sourceRect.bottom - Bmp[Tiles::CROSS].sourceRect.top);
    Bmp[Tiles::CROSS].Surface = lpDDSPanel;

    // JA
    Bmp[Tiles::YES].AnimationPhaseCount = 1;
    Bmp[Tiles::YES].sourceRect.left = 0;
    Bmp[Tiles::YES].sourceRect.top = 154;
    Bmp[Tiles::YES].sourceRect.right = Bmp[Tiles::YES].sourceRect.left + 41;
    Bmp[Tiles::YES].sourceRect.bottom = Bmp[Tiles::YES].sourceRect.top + 42;
    Bmp[Tiles::YES].Width = static_cast<short>(Bmp[Tiles::YES].sourceRect.right - Bmp[Tiles::YES].sourceRect.left);
    Bmp[Tiles::YES].Height = static_cast<short>(Bmp[Tiles::YES].sourceRect.bottom - Bmp[Tiles::YES].sourceRect.top);
    Bmp[Tiles::YES].Surface = lpDDSPaper;

    // NEIN
    Bmp[Tiles::NO].AnimationPhaseCount = 1;
    Bmp[Tiles::NO].sourceRect.left = 41;
    Bmp[Tiles::NO].sourceRect.top = 154;
    Bmp[Tiles::NO].sourceRect.right = Bmp[Tiles::NO].sourceRect.left + 100;
    Bmp[Tiles::NO].sourceRect.bottom = Bmp[Tiles::NO].sourceRect.top + 39;
    Bmp[Tiles::NO].Width = static_cast<short>(Bmp[Tiles::NO].sourceRect.right - Bmp[Tiles::NO].sourceRect.left);
    Bmp[Tiles::NO].Height = static_cast<short>(Bmp[Tiles::NO].sourceRect.bottom - Bmp[Tiles::NO].sourceRect.top);
    Bmp[Tiles::NO].Surface = lpDDSPaper;

    // Sonne
    Bmp[Tiles::SUN].AnimationPhaseCount = 1;
    Bmp[Tiles::SUN].sourceRect.left = 205;
    Bmp[Tiles::SUN].sourceRect.top = 65;
    Bmp[Tiles::SUN].sourceRect.right = Bmp[Tiles::SUN].sourceRect.left + 51;
    Bmp[Tiles::SUN].sourceRect.bottom = Bmp[Tiles::SUN].sourceRect.top + 50;
    Bmp[Tiles::SUN].targetRect.left = rcPanel.left + 30;
    Bmp[Tiles::SUN].targetRect.top = rcPanel.top + 630;
    Bmp[Tiles::SUN].targetRect.right = Bmp[Tiles::SUN].targetRect.left + 152;
    Bmp[Tiles::SUN].targetRect.bottom = Bmp[Tiles::SUN].targetRect.top + 55;
    Bmp[Tiles::SUN].Width = static_cast<short>(Bmp[Tiles::SUN].sourceRect.right - Bmp[Tiles::SUN].sourceRect.left);
    Bmp[Tiles::SUN].Height = static_cast<short>(Bmp[Tiles::SUN].sourceRect.bottom - Bmp[Tiles::SUN].sourceRect.top);
    Bmp[Tiles::SUN].Surface = lpDDSPanel;

    // PROGRAMMIERUNG
    Bmp[Tiles::PROGRAMMING].AnimationPhaseCount = 1;
    Bmp[Tiles::PROGRAMMING].sourceRect.left = 0;
    Bmp[Tiles::PROGRAMMING].sourceRect.top = 0;
    Bmp[Tiles::PROGRAMMING].sourceRect.right = Bmp[Tiles::PROGRAMMING].sourceRect.left + 348;
    Bmp[Tiles::PROGRAMMING].sourceRect.bottom = Bmp[Tiles::PROGRAMMING].sourceRect.top + 49;
    Bmp[Tiles::PROGRAMMING].Width = static_cast<short>(Bmp[Tiles::PROGRAMMING].sourceRect.right - Bmp[Tiles::PROGRAMMING].sourceRect.left);
    Bmp[Tiles::PROGRAMMING].Height = static_cast<short>(Bmp[Tiles::PROGRAMMING].sourceRect.bottom - Bmp[Tiles::PROGRAMMING].sourceRect.top);
    Bmp[Tiles::PROGRAMMING].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::PROGRAMMING].Width / 2;
    Bmp[Tiles::PROGRAMMING].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::PROGRAMMING].Height / 2;
    Bmp[Tiles::PROGRAMMING].targetRect.right = Bmp[Tiles::PROGRAMMING].targetRect.left + Bmp[Tiles::PROGRAMMING].Width;
    Bmp[Tiles::PROGRAMMING].targetRect.bottom = Bmp[Tiles::PROGRAMMING].targetRect.top + Bmp[Tiles::PROGRAMMING].Height;
    Bmp[Tiles::PROGRAMMING].Surface = lpDDSCredits;

    // DIRKPLATE
    Bmp[Tiles::DIRK_PLATE].AnimationPhaseCount = 1;
    Bmp[Tiles::DIRK_PLATE].sourceRect.left = 0;
    Bmp[Tiles::DIRK_PLATE].sourceRect.top = 49;
    Bmp[Tiles::DIRK_PLATE].sourceRect.right = Bmp[Tiles::DIRK_PLATE].sourceRect.left + 196;
    Bmp[Tiles::DIRK_PLATE].sourceRect.bottom = Bmp[Tiles::DIRK_PLATE].sourceRect.top + 47;
    Bmp[Tiles::DIRK_PLATE].Width = static_cast<short>(Bmp[Tiles::DIRK_PLATE].sourceRect.right - Bmp[Tiles::DIRK_PLATE].sourceRect.left);
    Bmp[Tiles::DIRK_PLATE].Height = static_cast<short>(Bmp[Tiles::DIRK_PLATE].sourceRect.bottom - Bmp[Tiles::DIRK_PLATE].sourceRect.top);
    Bmp[Tiles::DIRK_PLATE].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::DIRK_PLATE].Width / 2;
    Bmp[Tiles::DIRK_PLATE].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::DIRK_PLATE].Height / 2;
    Bmp[Tiles::DIRK_PLATE].targetRect.right = Bmp[Tiles::DIRK_PLATE].targetRect.left + Bmp[Tiles::DIRK_PLATE].Width;
    Bmp[Tiles::DIRK_PLATE].targetRect.bottom = Bmp[Tiles::DIRK_PLATE].targetRect.top + Bmp[Tiles::DIRK_PLATE].Height;
    Bmp[Tiles::DIRK_PLATE].Surface = lpDDSCredits;

    // MATTHIAS
    Bmp[Tiles::MATTHIAS].AnimationPhaseCount = 1;
    Bmp[Tiles::MATTHIAS].sourceRect.left = 0;
    Bmp[Tiles::MATTHIAS].sourceRect.top = 96;
    Bmp[Tiles::MATTHIAS].sourceRect.right = Bmp[Tiles::MATTHIAS].sourceRect.left + 379;
    Bmp[Tiles::MATTHIAS].sourceRect.bottom = Bmp[Tiles::MATTHIAS].sourceRect.top + 47;
    Bmp[Tiles::MATTHIAS].Width = static_cast<short>(Bmp[Tiles::MATTHIAS].sourceRect.right - Bmp[Tiles::MATTHIAS].sourceRect.left);
    Bmp[Tiles::MATTHIAS].Height = static_cast<short>(Bmp[Tiles::MATTHIAS].sourceRect.bottom - Bmp[Tiles::MATTHIAS].sourceRect.top);
    Bmp[Tiles::MATTHIAS].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::MATTHIAS].Width / 2;
    Bmp[Tiles::MATTHIAS].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::MATTHIAS].Height / 2;
    Bmp[Tiles::MATTHIAS].targetRect.right = Bmp[Tiles::MATTHIAS].targetRect.left + Bmp[Tiles::MATTHIAS].Width;
    Bmp[Tiles::MATTHIAS].targetRect.bottom = Bmp[Tiles::MATTHIAS].targetRect.top + Bmp[Tiles::MATTHIAS].Height;
    Bmp[Tiles::MATTHIAS].Surface = lpDDSCredits;

    // TESTSPIELER
    Bmp[Tiles::TEST_PLAYER].AnimationPhaseCount = 1;
    Bmp[Tiles::TEST_PLAYER].sourceRect.left = 0;
    Bmp[Tiles::TEST_PLAYER].sourceRect.top = 143;
    Bmp[Tiles::TEST_PLAYER].sourceRect.right = Bmp[Tiles::TEST_PLAYER].sourceRect.left + 210;
    Bmp[Tiles::TEST_PLAYER].sourceRect.bottom = Bmp[Tiles::TEST_PLAYER].sourceRect.top + 55;
    Bmp[Tiles::TEST_PLAYER].Width = static_cast<short>(Bmp[Tiles::TEST_PLAYER].sourceRect.right - Bmp[Tiles::TEST_PLAYER].sourceRect.left);
    Bmp[Tiles::TEST_PLAYER].Height = static_cast<short>(Bmp[Tiles::TEST_PLAYER].sourceRect.bottom - Bmp[Tiles::TEST_PLAYER].sourceRect.top);
    Bmp[Tiles::TEST_PLAYER].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::TEST_PLAYER].Width / 2;
    Bmp[Tiles::TEST_PLAYER].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::TEST_PLAYER].Height / 2;
    Bmp[Tiles::TEST_PLAYER].targetRect.right = Bmp[Tiles::TEST_PLAYER].targetRect.left + Bmp[Tiles::TEST_PLAYER].Width;
    Bmp[Tiles::TEST_PLAYER].targetRect.bottom = Bmp[Tiles::TEST_PLAYER].targetRect.top + Bmp[Tiles::TEST_PLAYER].Height;
    Bmp[Tiles::TEST_PLAYER].Surface = lpDDSCredits;

    // TOBIAS
    Bmp[Tiles::TOBIAS].AnimationPhaseCount = 1;
    Bmp[Tiles::TOBIAS].sourceRect.left = 0;
    Bmp[Tiles::TOBIAS].sourceRect.top = 198;
    Bmp[Tiles::TOBIAS].sourceRect.right = Bmp[Tiles::TOBIAS].sourceRect.left + 273;
    Bmp[Tiles::TOBIAS].sourceRect.bottom = Bmp[Tiles::TOBIAS].sourceRect.top + 56;
    Bmp[Tiles::TOBIAS].Width = static_cast<short>(Bmp[Tiles::TOBIAS].sourceRect.right - Bmp[Tiles::TOBIAS].sourceRect.left);
    Bmp[Tiles::TOBIAS].Height = static_cast<short>(Bmp[Tiles::TOBIAS].sourceRect.bottom - Bmp[Tiles::TOBIAS].sourceRect.top);
    Bmp[Tiles::TOBIAS].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::TOBIAS].Width / 2;
    Bmp[Tiles::TOBIAS].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::TOBIAS].Height / 2;
    Bmp[Tiles::TOBIAS].targetRect.right = Bmp[Tiles::TOBIAS].targetRect.left + Bmp[Tiles::TOBIAS].Width;
    Bmp[Tiles::TOBIAS].targetRect.bottom = Bmp[Tiles::TOBIAS].targetRect.top + Bmp[Tiles::TOBIAS].Height;
    Bmp[Tiles::TOBIAS].Surface = lpDDSCredits;

    // SIGRID
    Bmp[Tiles::SIGRID].AnimationPhaseCount = 1;
    Bmp[Tiles::SIGRID].sourceRect.left = 0;
    Bmp[Tiles::SIGRID].sourceRect.top = 254;
    Bmp[Tiles::SIGRID].sourceRect.right = Bmp[Tiles::SIGRID].sourceRect.left + 226;
    Bmp[Tiles::SIGRID].sourceRect.bottom = Bmp[Tiles::SIGRID].sourceRect.top + 56;
    Bmp[Tiles::SIGRID].Width = static_cast<short>(Bmp[Tiles::SIGRID].sourceRect.right - Bmp[Tiles::SIGRID].sourceRect.left);
    Bmp[Tiles::SIGRID].Height = static_cast<short>(Bmp[Tiles::SIGRID].sourceRect.bottom - Bmp[Tiles::SIGRID].sourceRect.top);
    Bmp[Tiles::SIGRID].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::SIGRID].Width / 2;
    Bmp[Tiles::SIGRID].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::SIGRID].Height / 2;
    Bmp[Tiles::SIGRID].targetRect.right = Bmp[Tiles::SIGRID].targetRect.left + Bmp[Tiles::SIGRID].Width;
    Bmp[Tiles::SIGRID].targetRect.bottom = Bmp[Tiles::SIGRID].targetRect.top + Bmp[Tiles::SIGRID].Height;
    Bmp[Tiles::SIGRID].Surface = lpDDSCredits;

    // PATHFINDING
    Bmp[Tiles::PATHFINDING].AnimationPhaseCount = 1;
    Bmp[Tiles::PATHFINDING].sourceRect.left = 0;
    Bmp[Tiles::PATHFINDING].sourceRect.top = 353;
    Bmp[Tiles::PATHFINDING].sourceRect.right = Bmp[Tiles::PATHFINDING].sourceRect.left + 233;
    Bmp[Tiles::PATHFINDING].sourceRect.bottom = Bmp[Tiles::PATHFINDING].sourceRect.top + 59;
    Bmp[Tiles::PATHFINDING].Width = static_cast<short>(Bmp[Tiles::PATHFINDING].sourceRect.right - Bmp[Tiles::PATHFINDING].sourceRect.left);
    Bmp[Tiles::PATHFINDING].Height = static_cast<short>(Bmp[Tiles::PATHFINDING].sourceRect.bottom - Bmp[Tiles::PATHFINDING].sourceRect.top);
    Bmp[Tiles::PATHFINDING].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::PATHFINDING].Width / 2;
    Bmp[Tiles::PATHFINDING].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::PATHFINDING].Height / 2;
    Bmp[Tiles::PATHFINDING].targetRect.right = Bmp[Tiles::PATHFINDING].targetRect.left + Bmp[Tiles::PATHFINDING].Width;
    Bmp[Tiles::PATHFINDING].targetRect.bottom = Bmp[Tiles::PATHFINDING].targetRect.top + Bmp[Tiles::PATHFINDING].Height;
    Bmp[Tiles::PATHFINDING].Surface = lpDDSCredits;

    // JOHN
    Bmp[Tiles::JOHN].AnimationPhaseCount = 1;
    Bmp[Tiles::JOHN].sourceRect.left = 0;
    Bmp[Tiles::JOHN].sourceRect.top = 412;
    Bmp[Tiles::JOHN].sourceRect.right = Bmp[Tiles::JOHN].sourceRect.left + 347;
    Bmp[Tiles::JOHN].sourceRect.bottom = Bmp[Tiles::JOHN].sourceRect.top + 56;
    Bmp[Tiles::JOHN].Width = static_cast<short>(Bmp[Tiles::JOHN].sourceRect.right - Bmp[Tiles::JOHN].sourceRect.left);
    Bmp[Tiles::JOHN].Height = static_cast<short>(Bmp[Tiles::JOHN].sourceRect.bottom - Bmp[Tiles::JOHN].sourceRect.top);
    Bmp[Tiles::JOHN].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::JOHN].Width / 2;
    Bmp[Tiles::JOHN].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::JOHN].Height / 2;
    Bmp[Tiles::JOHN].targetRect.right = Bmp[Tiles::JOHN].targetRect.left + Bmp[Tiles::JOHN].Width;
    Bmp[Tiles::JOHN].targetRect.bottom = Bmp[Tiles::JOHN].targetRect.top + Bmp[Tiles::JOHN].Height;
    Bmp[Tiles::JOHN].Surface = lpDDSCredits;

    // HEIKO
    Bmp[Tiles::HEIKO].AnimationPhaseCount = 1;
    Bmp[Tiles::HEIKO].sourceRect.left = 0;
    Bmp[Tiles::HEIKO].sourceRect.top = 468;
    Bmp[Tiles::HEIKO].sourceRect.right = Bmp[Tiles::HEIKO].sourceRect.left + 236;
    Bmp[Tiles::HEIKO].sourceRect.bottom = Bmp[Tiles::HEIKO].sourceRect.top + 47;
    Bmp[Tiles::HEIKO].Width = static_cast<short>(Bmp[Tiles::HEIKO].sourceRect.right - Bmp[Tiles::HEIKO].sourceRect.left);
    Bmp[Tiles::HEIKO].Height = static_cast<short>(Bmp[Tiles::HEIKO].sourceRect.bottom - Bmp[Tiles::HEIKO].sourceRect.top);
    Bmp[Tiles::HEIKO].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::HEIKO].Width / 2;
    Bmp[Tiles::HEIKO].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::HEIKO].Height / 2;
    Bmp[Tiles::HEIKO].targetRect.right = Bmp[Tiles::HEIKO].targetRect.left + Bmp[Tiles::HEIKO].Width;
    Bmp[Tiles::HEIKO].targetRect.bottom = Bmp[Tiles::HEIKO].targetRect.top + Bmp[Tiles::HEIKO].Height;
    Bmp[Tiles::HEIKO].Surface = lpDDSCredits;

    // GISELA
    Bmp[Tiles::GISELA].AnimationPhaseCount = 1;
    Bmp[Tiles::GISELA].sourceRect.left = 0;
    Bmp[Tiles::GISELA].sourceRect.top = 515;
    Bmp[Tiles::GISELA].sourceRect.right = Bmp[Tiles::GISELA].sourceRect.left + 232;
    Bmp[Tiles::GISELA].sourceRect.bottom = Bmp[Tiles::GISELA].sourceRect.top + 47;
    Bmp[Tiles::GISELA].Width = static_cast<short>(Bmp[Tiles::GISELA].sourceRect.right - Bmp[Tiles::GISELA].sourceRect.left);
    Bmp[Tiles::GISELA].Height = static_cast<short>(Bmp[Tiles::GISELA].sourceRect.bottom - Bmp[Tiles::GISELA].sourceRect.top);
    Bmp[Tiles::GISELA].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::GISELA].Width / 2;
    Bmp[Tiles::GISELA].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::GISELA].Height / 2;
    Bmp[Tiles::GISELA].targetRect.right = Bmp[Tiles::GISELA].targetRect.left + Bmp[Tiles::GISELA].Width;
    Bmp[Tiles::GISELA].targetRect.bottom = Bmp[Tiles::GISELA].targetRect.top + Bmp[Tiles::GISELA].Height;
    Bmp[Tiles::GISELA].Surface = lpDDSCredits;

    // WEITEREHILFE
    Bmp[Tiles::FURTHER_HELP].AnimationPhaseCount = 1;
    Bmp[Tiles::FURTHER_HELP].sourceRect.left = 0;
    Bmp[Tiles::FURTHER_HELP].sourceRect.top = 562;
    Bmp[Tiles::FURTHER_HELP].sourceRect.right = Bmp[Tiles::FURTHER_HELP].sourceRect.left + 258;
    Bmp[Tiles::FURTHER_HELP].sourceRect.bottom = Bmp[Tiles::FURTHER_HELP].sourceRect.top + 46;
    Bmp[Tiles::FURTHER_HELP].Width = static_cast<short>(Bmp[Tiles::FURTHER_HELP].sourceRect.right - Bmp[Tiles::FURTHER_HELP].sourceRect.left);
    Bmp[Tiles::FURTHER_HELP].Height = static_cast<short>(Bmp[Tiles::FURTHER_HELP].sourceRect.bottom - Bmp[Tiles::FURTHER_HELP].sourceRect.top);
    Bmp[Tiles::FURTHER_HELP].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::FURTHER_HELP].Width / 2;
    Bmp[Tiles::FURTHER_HELP].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::FURTHER_HELP].Height / 2;
    Bmp[Tiles::FURTHER_HELP].targetRect.right = Bmp[Tiles::FURTHER_HELP].targetRect.left + Bmp[Tiles::FURTHER_HELP].Width;
    Bmp[Tiles::FURTHER_HELP].targetRect.bottom = Bmp[Tiles::FURTHER_HELP].targetRect.top + Bmp[Tiles::FURTHER_HELP].Height;
    Bmp[Tiles::FURTHER_HELP].Surface = lpDDSCredits;

    // DPSOFTWARE
    Bmp[Tiles::DPSOFTWARE].AnimationPhaseCount = 1;
    Bmp[Tiles::DPSOFTWARE].sourceRect.left = 0;
    Bmp[Tiles::DPSOFTWARE].sourceRect.top = 608;
    Bmp[Tiles::DPSOFTWARE].sourceRect.right = Bmp[Tiles::DPSOFTWARE].sourceRect.left + 291;
    Bmp[Tiles::DPSOFTWARE].sourceRect.bottom = Bmp[Tiles::DPSOFTWARE].sourceRect.top + 99;
    Bmp[Tiles::DPSOFTWARE].Width = static_cast<short>(Bmp[Tiles::DPSOFTWARE].sourceRect.right - Bmp[Tiles::DPSOFTWARE].sourceRect.left);
    Bmp[Tiles::DPSOFTWARE].Height = static_cast<short>(Bmp[Tiles::DPSOFTWARE].sourceRect.bottom - Bmp[Tiles::DPSOFTWARE].sourceRect.top);
    Bmp[Tiles::DPSOFTWARE].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::DPSOFTWARE].Width / 2;
    Bmp[Tiles::DPSOFTWARE].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::DPSOFTWARE].Height / 2;
    Bmp[Tiles::DPSOFTWARE].targetRect.right = Bmp[Tiles::DPSOFTWARE].targetRect.left + Bmp[Tiles::DPSOFTWARE].Width;
    Bmp[Tiles::DPSOFTWARE].targetRect.bottom = Bmp[Tiles::DPSOFTWARE].targetRect.top + Bmp[Tiles::DPSOFTWARE].Height;
    Bmp[Tiles::DPSOFTWARE].Surface = lpDDSCredits;

    // SCHWARZ
    Bmp[Tiles::SCHWARZ].AnimationPhaseCount = 1;
    Bmp[Tiles::SCHWARZ].sourceRect.left = 0;
    Bmp[Tiles::SCHWARZ].sourceRect.top = 608;
    Bmp[Tiles::SCHWARZ].sourceRect.right = Bmp[Tiles::SCHWARZ].sourceRect.left + 1;
    Bmp[Tiles::SCHWARZ].sourceRect.bottom = Bmp[Tiles::SCHWARZ].sourceRect.top + 1;
    Bmp[Tiles::SCHWARZ].Width = static_cast<short>(Bmp[Tiles::SCHWARZ].sourceRect.right - Bmp[Tiles::SCHWARZ].sourceRect.left);
    Bmp[Tiles::SCHWARZ].Height = static_cast<short>(Bmp[Tiles::SCHWARZ].sourceRect.bottom - Bmp[Tiles::SCHWARZ].sourceRect.top);
    Bmp[Tiles::SCHWARZ].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::SCHWARZ].Width / 2;
    Bmp[Tiles::SCHWARZ].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::SCHWARZ].Height / 2;
    Bmp[Tiles::SCHWARZ].targetRect.right = Bmp[Tiles::SCHWARZ].targetRect.left + Bmp[Tiles::SCHWARZ].Width;
    Bmp[Tiles::SCHWARZ].targetRect.bottom = Bmp[Tiles::SCHWARZ].targetRect.top + Bmp[Tiles::SCHWARZ].Height;
    Bmp[Tiles::SCHWARZ].Surface = lpDDSCredits;

    // SOUNDS
    Bmp[Tiles::SOUNDS].AnimationPhaseCount = 1;
    Bmp[Tiles::SOUNDS].sourceRect.left = 0;
    Bmp[Tiles::SOUNDS].sourceRect.top = 310;
    Bmp[Tiles::SOUNDS].sourceRect.right = Bmp[Tiles::SOUNDS].sourceRect.left + 144;
    Bmp[Tiles::SOUNDS].sourceRect.bottom = Bmp[Tiles::SOUNDS].sourceRect.top + 43;
    Bmp[Tiles::SOUNDS].Width = static_cast<short>(Bmp[Tiles::SOUNDS].sourceRect.right - Bmp[Tiles::SOUNDS].sourceRect.left);
    Bmp[Tiles::SOUNDS].Height = static_cast<short>(Bmp[Tiles::SOUNDS].sourceRect.bottom - Bmp[Tiles::SOUNDS].sourceRect.top);
    Bmp[Tiles::SOUNDS].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::SOUNDS].Width / 2;
    Bmp[Tiles::SOUNDS].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::SOUNDS].Height / 2;
    Bmp[Tiles::SOUNDS].targetRect.right = Bmp[Tiles::SOUNDS].targetRect.left + Bmp[Tiles::SOUNDS].Width;
    Bmp[Tiles::SOUNDS].targetRect.bottom = Bmp[Tiles::SOUNDS].targetRect.top + Bmp[Tiles::SOUNDS].Height;
    Bmp[Tiles::SOUNDS].Surface = lpDDSCredits;

    // MUSIK
    Bmp[Tiles::MUSIC].AnimationPhaseCount = 1;
    Bmp[Tiles::MUSIC].sourceRect.left = 160;
    Bmp[Tiles::MUSIC].sourceRect.top = 310;
    Bmp[Tiles::MUSIC].sourceRect.right = Bmp[Tiles::MUSIC].sourceRect.left + 124;
    Bmp[Tiles::MUSIC].sourceRect.bottom = Bmp[Tiles::MUSIC].sourceRect.top + 39;
    Bmp[Tiles::MUSIC].Width = static_cast<short>(Bmp[Tiles::MUSIC].sourceRect.right - Bmp[Tiles::MUSIC].sourceRect.left);
    Bmp[Tiles::MUSIC].Height = static_cast<short>(Bmp[Tiles::MUSIC].sourceRect.bottom - Bmp[Tiles::MUSIC].sourceRect.top);
    Bmp[Tiles::MUSIC].targetRect.left = MAX_SCREEN_X / 2 - Bmp[Tiles::MUSIC].Width / 2;
    Bmp[Tiles::MUSIC].targetRect.top = MAX_SCREEN_Y - Bmp[Tiles::MUSIC].Height / 2;
    Bmp[Tiles::MUSIC].targetRect.right = Bmp[Tiles::MUSIC].targetRect.left + Bmp[Tiles::MUSIC].Width;
    Bmp[Tiles::MUSIC].targetRect.bottom = Bmp[Tiles::MUSIC].targetRect.top + Bmp[Tiles::MUSIC].Height;
    Bmp[Tiles::MUSIC].Surface = lpDDSCredits;

    for (i = 0; i < 10; i++)
        for (k = 0; k < 10; k++) {
            CreditsList[i][k].IsRunning = false;
            CreditsList[i][k].Picture = -1;
        }

    CreditsList[0][0].Picture = Tiles::PROGRAMMING;
    CreditsList[0][1].IsRunning = true; // nur den hier true setzen, löst dann alles andere aus
    CreditsList[0][1].Picture = Tiles::DIRK_PLATE;
    CreditsList[1][0].Picture = Tiles::MUSIC;
    CreditsList[1][1].Picture = Tiles::HEIKO;
    CreditsList[2][0].Picture = Tiles::SOUNDS;
    CreditsList[2][1].Picture = Tiles::DIRK_PLATE;
    CreditsList[3][0].Picture = Tiles::TEST_PLAYER;
    CreditsList[3][1].Picture = Tiles::MATTHIAS;
    CreditsList[3][2].Picture = Tiles::TOBIAS;
    CreditsList[3][3].Picture = Tiles::SIGRID;
    CreditsList[4][0].Picture = Tiles::PATHFINDING;
    CreditsList[4][1].Picture = Tiles::JOHN;
    CreditsList[5][0].Picture = Tiles::FURTHER_HELP;
    CreditsList[5][1].Picture = Tiles::HEIKO;
    CreditsList[5][2].Picture = Tiles::GISELA;
    CreditsList[6][0].Picture = Tiles::SCHWARZ;
    CreditsList[6][1].Picture = Tiles::DPSOFTWARE;

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
    TextBereich[TXTCHANCE].textRect.left = Bmp[Tiles::RING].targetRect.left + 5;
    TextBereich[TXTCHANCE].textRect.top = Bmp[Tiles::RING].targetRect.top + Bmp[Tiles::RING].Height + 10;
    TextBereich[TXTCHANCE].textRect.right = TextBereich[TXTCHANCE].textRect.left + 3 * FONT2_LETTER_WIDTH;
    TextBereich[TXTCHANCE].textRect.bottom = TextBereich[TXTCHANCE].textRect.top + FONT2_LETTER_HEIGHT;

    Guy.ResourceAmount[Resources::Water] = 100;
    Guy.ResourceAmount[Resources::Food] = 100;
    Guy.ResourceAmount[Resources::Health] = 100;

    for (i = Tiles::RAW_TREE_BRANCH; i <= Tiles::RAW_SLINGSHOT; i++) {
        Guy.Inventory[i] = 0;
    }

    CursorTyp = Tiles::CURSOR_ARROW;
    Gitter = false;
    PapierText = -1;
    HauptMenue = 0;
    TwoClicks = -1;
    Night = false;
    Frage = -1;
    FPS = 100;
    CurrentFrame = 0;
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

        s_GameState = State::INTRO;
        Guy.IsActive = false;
        Guy.AnimationState = Tiles::GUY_SHIP;
        Guy.ActionStep = 0;
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
