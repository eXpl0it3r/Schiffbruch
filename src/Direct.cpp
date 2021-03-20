#include "Direct.hpp"

#include "types.hpp"

#include "Action.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Sound.hpp"
#include "State.hpp"
#include "World.hpp"

#include "bin/images/Animation.BMP.h"
#include "bin/images/Bau.bmp.h"
#include "bin/images/Baum.bmp.h"
#include "bin/images/Buttons.bmp.h"
#include "bin/images/Credits.bmp.h"
#include "bin/images/Cursor.BMP.h"
#include "bin/images/GuyAni.bmp.h"
#include "bin/images/Inventar.bmp.h"
#include "bin/images/Logo.png.h"
#include "bin/images/Misc.BMP.h"
#include "bin/images/Panel.png.h"
#include "bin/images/Papier.bmp.h"
#include "bin/images/Schrift1.BMP.h"
#include "bin/images/Schrift2.bmp.h"
#include "bin/images/Textfeld.bmp.h"

#include <SFML/Window.hpp>

namespace Direct {
//LPDIRECTDRAW4 lpDD = nullptr; // DirectDraw object
bool Button0down; // linke Maustaste gedrückt gehalten
bool Button1down; // rechte Maustaste gedrückt gehalten

void finiObjects()
{
//    if (lpDD != nullptr) {
        if (screenTexture != nullptr) {
//            lpDDSPrimary->Release();
            delete screenTexture;
            screenTexture = nullptr;
        }
        delete minimapPlayerTexture;
        delete minimapPlayerSprite;

        delete s_creditsSprite;

//        if (lpDDPal != nullptr) {
//            lpDDPal->Release();
//            lpDDPal = nullptr;
//        }

//        lpDD->Release();
//        lpDD = nullptr;
//    }
}

bool InitDDraw()
{
//    DDSCAPS2 ddscaps;
//    LPDIRECTDRAW pDD;
    minimapPlayerTexture = Renderer::createEmptyTexture(2, 2, sf::Color::Red);
    minimapPlayerSprite = new sf::Sprite;
    minimapPlayerSprite->setTexture(*minimapPlayerTexture);

    s_creditsSprite = new sf::Sprite;

    s_darknessColor = sf::Color::White;

    lpDDSBack = Renderer::createEmptyTexture(MAX_SCREEN_X, MAX_SCREEN_Y, sf::Color(0, 0, 0));
#if 0
    // Create the main DirectDraw object
    HRESULT ddrval = dx_DirectDrawCreate(nullptr, &pDD, nullptr);

    if (ddrval != DD_OK) {
        goto error;
    }

//        ddrval = pDD->QueryInterface(IID_IDirectDraw, reinterpret_cast<LPVOID *>(& lpDD));
//        if (ddrval != DD_OK)
//            goto error;

    // Get exclusive mode
//        ddrval = lpDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
//        if (ddrval != DD_OK)
//            goto error;

    // Set the video mode to 800x600x16
    ddrval = lpDD->SetDisplayMode(MAX_SCREEN_X, MAX_SCREEN_Y, 32);

    if (ddrval != DD_OK) {
        switch (ddrval) {
        case DDERR_GENERIC:
        case DDERR_INVALIDMODE:
        case DDERR_INVALIDOBJECT:

//            case DDERR_INVALIDPARAMS:
        case DDERR_LOCKEDSURFACES:
        case DDERR_NOEXCLUSIVEMODE:
        case DDERR_SURFACEBUSY:
//            case DDERR_UNSUPPORTED:
//            case DDERR_UNSUPPORTEDMODE:
        {
//                    MessageBeep(MB_OK);
            break;
        }

        case DDERR_WASSTILLDRAWING:
            ;
        }

        goto error;
    }
#endif

//    ZeroMemory(&ddsd2, sizeof(ddsd2));
//    ddsd2.dwSize = sizeof(ddsd2);

//    // Create the primary surface with 1 back buffer
//    ZeroMemory(&ddsd, sizeof(ddsd));
//    ddsd.dwSize = sizeof(ddsd);
////        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
////        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
//    ddsd.dwBackBufferCount = 1; // Anzahl ??
//    screenTexture = new sf::Texture;
//    ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, nullptr);

//    if (ddrval != DD_OK) {
//        goto error;
//    }

    // für gamma-ablenden
//        lpDDSPrimary->QueryInterface(IID_IDirectDrawGammaControl, reinterpret_cast<void **>(&lpDDGammaControl));
//    lpDDGammaControl->GetGammaRamp(0, &DDGammaOld);
//    lpDDGammaControl->GetGammaRamp(0, &DDGammaRamp);

//        ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
//        ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);

//    if (ddrval != DD_OK) {
//        goto error;
//    }

//    ddsd.dwSize = sizeof(ddsd); // Tell DirectDraw which members are valid.
//        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
//        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    // In diese Surface sollen die Bausteine geladen werden
    lpDDSMisc = Renderer::loadTexture(resource_Misc_BMP_data, resource_Misc_BMP_size);
    if (!lpDDSMisc) {
        puts("Failed to load misc");
        return false;
    }

    // In diese Surface sollen die Animation der Figur gespeichert werden
    lpDDSGuyAni = Renderer::loadTexture(resource_GuyAni_bmp_data, resource_GuyAni_bmp_size);
    if (!lpDDSGuyAni) {
        puts("failed to load guyani.bmp");
        return false;
    }

    // In diese Surface sollen das Panel geladen werden
    lpDDSPanel = Renderer::loadTexture(resource_Panel_png_data, resource_Panel_png_size);
    if (!lpDDSPanel) {
        puts("failed to load panel.png");
        return false;
    }

    // In diese Surface sollen die Landschaftsanimationen gespeichert werden
    lpDDSAnimation = Renderer::loadTexture(resource_Animation_BMP_data, resource_Animation_BMP_size);
    if (!lpDDSAnimation) {
        puts("failed to load animation.bmp");
        return false;
    }

    // In diese Surface soll die Schrift1 gespeichert werden
    lpDDSSchrift1 = Renderer::loadTexture(resource_Schrift1_BMP_data, resource_Schrift1_BMP_size);
    if (!lpDDSSchrift1) {
        puts("failed to load schrift1");
        return false;
    }

    // In diese Surface soll die Schrift2 gespeichert werden
    lpDDSSchrift2 = Renderer::loadTexture(resource_Schrift2_bmp_data, resource_Schrift2_bmp_size);
    if (!lpDDSSchrift2) {
        puts("failed to load schrift2");
        return false;
    }

    // In diese Surface soll das Papier gespeichert werden
    lpDDSPaper = Renderer::loadTexture(resource_Papier_bmp_data, resource_Papier_bmp_size);
    if (!lpDDSPaper) {
        puts("failed to load papier.bmp");
        return false;
    }

    // In diese Surface solln die Bäume gespeichert werden
    lpDDSBaum = Renderer::loadTexture(resource_Baum_bmp_data, resource_Baum_bmp_size);
    if (!lpDDSBaum) {
        puts("failed to load baum.bmp");
        return false;
    }

    // In diese Surface solln die Cursor gespeichert werden
    lpDDSCursor = Renderer::loadTexture(resource_Cursor_BMP_data, resource_Cursor_BMP_size);
    if (!lpDDSCursor) {
        puts("failed to load texture.bmp");
        return false;
    }

    // In diese Surface solln die Buttons gespeichert werden
    lpDDSButtons = Renderer::loadTexture(resource_Buttons_bmp_data, resource_Buttons_bmp_size);
    if (!lpDDSButtons) {
        puts("failed to load buttons.bmp");
        return false;
    }

    // In diese Surface solln das TextFeld gespeichert werden
    lpDDSTextFeld = Renderer::loadTexture(resource_Textfeld_bmp_data, resource_Textfeld_bmp_size);
    if (!lpDDSTextFeld) {
        puts("failed to load textfeld.bmp");
        return false;
    }

    // In diese Surface solln das Inventar gespeichert werden
    lpDDSInventar = Renderer::loadTexture(resource_Inventar_bmp_data, resource_Inventar_bmp_size);
    if (!lpDDSInventar) {
        puts("failed to load inventar.bmp");
        return false;
    }

    // In diese Surface solln die Bauwerke gespeichert werden
    lpDDSBau = Renderer::loadTexture(resource_Bau_bmp_data, resource_Bau_bmp_size);
    if (!lpDDSBau) {
        puts("failed to load bau.bmp");
        return false;
    }

    // In diese Surface solln die Credits gespeichert werden
    lpDDSCredits = Renderer::loadTexture(resource_Credits_bmp_data, resource_Credits_bmp_size, sf::Color::Black);
    if (!lpDDSCredits) {
        puts("failed to load credits.bmp");
        return false;
    }

    // In diese Surface solln das Logo gespeichert werden
    lpDDSLogo = Renderer::loadTexture(resource_Logo_png_data, resource_Logo_png_size);
    if (!lpDDSLogo) {
        puts("failed to load logo.png");
        return false;
    }

    // In diese Surface soll die MiniMap gespeichert werden
    lpDDSKarte = Renderer::createEmptyTexture(2 * MAX_TILES_X, 2 * MAX_TILESY, sf::Color::Transparent);

    // The landscape should be saved in this surface
//    lpDDSScape = Renderer::createEmptyTexture(2 * MAX_SURFACE_X, 2 * MAX_SURFACE_Y, sf::Color::Transparent);

    // In diese Surface soll die Schrift gespeichert werden
//    lpDDSSchrift = Renderer::createEmptyTexture(2 * MAX_SCREEN_X, 2 * MAX_SCREEN_Y, sf::Color::Transparent);

    // In diese Surface soll die Schatzkarte gespeichert werden
    lpDDSSchatzkarte = Renderer::createEmptyTexture(2 * TREASUREMAP_WIDTH, 2 * TREASUREMAP_HEIGHT, sf::Color::Transparent);
//    s_treasureMapSprite = new sf::Sprite;
//    s_treasureMapSprite->setTexture(*lpDDSSchatzkarte);

    return true;
}

void Reset()
{
    Button0down = false;
    Button1down = false;
}

void CheckMouse(const Coordinate newMousePos)
{
    // Mausbewegung
    short xDiff = MousePosition.x - newMousePos.x; // Die Differenz zur vorherigen Position ((Für Scrollen)

    MousePosition.x = newMousePos.x;

    if (MousePosition.x < 0) {
        MousePosition.x = 0;
    }

    if (MousePosition.x > MAX_SCREEN_X - 2) {
        MousePosition.x = MAX_SCREEN_X - 2;
    }

    short yDiff = MousePosition.y - newMousePos.y; // Die Differenz zur vorherigen Position ((Für Scrollen)
    MousePosition.y = newMousePos.y;

    if (MousePosition.y < 0) {
        MousePosition.y = 0;
    }

    if (MousePosition.y > MAX_SCREEN_Y - 2) {
        MousePosition.y = MAX_SCREEN_Y - 2;
        if (yDiff < 0) {
            yDiff = 0;
        }
    }

    if (TwoClicks == -1) {
        if (Guy.IsActive) {
            if (Math::InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_STOP].targetRect) && (Bmp[Tiles::BUTTON_STOP].AnimationPhase != -1)) {
                CursorTyp = Tiles::CURSOR_ARROW;
            } else {
                CursorTyp = Tiles::CURSOR_CLOCK;
            }
        } else {
            CursorTyp = Tiles::CURSOR_ARROW;
        }
    }

    short Button = -1; // Welcher Knopf ist gedrückt worden
    short Push = 0; // Knopf gedrückt(1) oder losgelassen(-1) oder gedrückt(0) gehalten

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        Button = 0;

        if (Button0down) {
            Push = 0;
        } else {
            Push = 1;
            Button0down = true;
        }
    } else {
        if (Button0down) {
            Button = 0;
            Push = -1;
            Button0down = false;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        Button = 1;

        if (Button1down) {
            Push = 0;
        } else {
            Push = 1;
            Button1down = true;
        }
    } else {
        if (Button1down) {
            Button = 1;
            Push = -1;
            Button1down = false;
        }
    }

    // Wenn ein Text steht, dann bei Mausdruck Text weg
    if (PapierText != -1) {
        if (Frage == 0) {
            if (Math::InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::YES].targetRect)) {
                CursorTyp = Tiles::CURSOR_ARROW;

                if ((Button == 0) && (Push == 1)) {
                    Frage = 1;
                    Renderer::HideText(TXTPAPIER);
                    PapierText = -1;
                    Guy.IsActive = false;
                    PlaySound(Sound::CLICK2, 100);
                }
            } else if (Math::InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::NO].targetRect)) {
                CursorTyp = Tiles::CURSOR_ARROW;

                if ((Button == 0) && (Push == 1)) {
                    Frage = 2;
                    Renderer::HideText(TXTPAPIER);
                    PapierText = -1;
                    Guy.IsActive = false;
                    PlaySound(Sound::CLICK2, 100);
                }
            } else if ((Button == 0) && (Push == 1)) {
                PlaySound(Sound::CLICK, 100);
            }
        } else if ((Button != -1) && (Push == 1)) {
            Renderer::HideText(TXTPAPIER);
            PapierText = -1;
            Guy.IsActive = false;
        }

        return;
    }

    // Animationen und Text löschen (werden dann von den MouseIn.. Sachen neu angestellt
    Renderer::HideText(TXTTEXTFELD);
    Math::ButtAniAus();

    // Wenn der Guy aktiv dann linke Mouse-Buttons ignorieren
    if ((Guy.IsActive) && (Button == 0)) {
        if (!(Math::InRect(MousePosition.x, MousePosition.y, Bmp[Tiles::BUTTON_STOP].targetRect)) ||
                (Bmp[Tiles::BUTTON_STOP].AnimationPhase == -1)) {
            Button = -1;
        }
    }

    // die Maus ist in der Spielflaeche ->
    if (Math::InRect(MousePosition.x, MousePosition.y, rcPlayingSurface)) {
        Math::UpdateMousePosition(Button, Push, xDiff, yDiff);
    }

    // die Maus ist im Panel ->
    if (Math::InRect(MousePosition.x, MousePosition.y, rcPanel)) {
        Math::MouseInPanel(Button, Push);
    }
}

short CheckKey()
{
    if (s_GameState == State::LOGO) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // Logo Abbrechen
            StopSound(Sound::LOGO);
            Game::NewGame(false);
            return 2;
        }
    } else if (s_GameState == State::INTRO) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // Intro Abbrechen
            StopSound(Sound::STORM); // Sound hier sofort stoppen
            StopSound(Sound::SWIM); // Sound hier sofort stoppen
            Guy.IsActive = false;

            for (short x = Guy.Pos.x; x < MAX_TILES_X; x++) {
                Guy.Pos.x = x;
                World::UpdateDiscovered();

                if (Landscape[Guy.Pos.x][Guy.Pos.y].Terrain != 1) {
                    break;
                }
            }

            Landscape[Guy.Pos.x - 2][Guy.Pos.y].Object = Tiles::WRECK_1;
            Landscape[Guy.Pos.x - 2][Guy.Pos.y].ObjectPosOffset.x = static_cast<short>(Bmp[Tiles::WRECK_1].targetRect.left);
            Landscape[Guy.Pos.x - 2][Guy.Pos.y].ObjectPosOffset.y = static_cast<short>(Bmp[Tiles::WRECK_1].targetRect.top);

            Guy.ScreenPosition.x =
                (Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][0].x +
                 Landscape[Guy.Pos.x][Guy.Pos.y].xScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][2].x) / 2;
            Guy.ScreenPosition.y =
                (Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][1].y +
                 Landscape[Guy.Pos.x][Guy.Pos.y].yScreen + CornerCoord[Landscape[Guy.Pos.x][Guy.Pos.y].Type][3].y) / 2;
            RouteStart.x = -1;
            RouteStart.y = -1;
            RouteDestination.x = -1;
            RouteDestination.y = -1;
            Camera.x = Guy.ScreenPosition.x - static_cast<short>(rcPlayingSurface.right / 2);
            Camera.y = Guy.ScreenPosition.y - static_cast<short>(rcPlayingSurface.bottom / 2);

            if (IsInBoat) {
                World::ToggleIsInBoat();
            }

            Guy.AnimationState = Tiles::GUY_LEFT;
            Guy.CurrentAction = Action::NOTHING;
            s_GameState = State::GAME;
            Guy.OriginalPosition = Guy.ScreenPosition;
            Game::SaveGame();
            return 1;
        }
    } else if (s_GameState == State::RESCUED) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            s_GameState = State::OUTRO;
            return 1;
        }
    } else if (s_GameState == State::GAME) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            Camera.x += 10;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            Camera.x -= 10;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            Camera.y += 10;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            Camera.y -= 10;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            Guy.ActionStep = 0;
            Guy.IsActive = false;
            Guy.CurrentAction = Action::QUIT;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11)) {
            Guy.ActionStep = 0;
            Guy.IsActive = false;
            Guy.CurrentAction = Action::RESTART;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
            Gitter = !Gitter;
            World::Generate();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            LAnimation = !LAnimation;
            World::Generate();
        }

        // Development cheats
#ifndef NDEBUG
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            short x,y;
            for (y=0;y<MAX_TILESY;y++)
            for (x=0;x<MAX_TILES_X;x++)
            Landscape[x][y].Discovered = true;
            World::Generate();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            Guy.Inventory[Tiles::RAW_TREE_BRANCH] = 10;
            Guy.Inventory[Tiles::RAW_STONE] = 10;
            Guy.Inventory[Tiles::RAW_LEAF] = 10;
            Guy.Inventory[Tiles::RAW_LIANA] = 10;
            Guy.Inventory[Tiles::RAW_TREE_TRUNK] = 9;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            Guy.Inventory[Tiles::RAW_AXE]   = 1;
            Guy.Inventory[Tiles::RAW_HOE]  = 1;
            Guy.Inventory[Tiles::RAW_FISHING_POLE]  = 1;
            Guy.Inventory[Tiles::RAW_HAMMER]   = 1;
            Guy.Inventory[Tiles::RAW_TELESCOPE] = 1;
            Guy.Inventory[Tiles::RAW_MATCH] = 1;
            Guy.Inventory[Tiles::RAW_SHOVEL] = 1;
            Guy.Inventory[Tiles::RAW_MAP] = 1;
            Guy.Inventory[Tiles::RAW_SLINGSHOT] = 1;

            Bmp[Tiles::BUTTON_CHOP].AnimationPhase  = 0;
            Bmp[Tiles::BUTTON_FISH].AnimationPhase  = 0;
            Bmp[Tiles::BUTTON_IGNITE].AnimationPhase  = 0;
            Bmp[Tiles::BUTTON_LOOK_OUT].AnimationPhase = 0;
            Bmp[Tiles::BUTTON_TREASUREMAP].AnimationPhase = 0;
            Bmp[Tiles::BUTTON_TREASURE].AnimationPhase = 0;
            Bmp[Tiles::BUTTON_SLINGSHOT].AnimationPhase = 0;
            Bmp[Tiles::BUTTON_FARM].AnimationPhase  = 0;
            Bmp[Tiles::BUTTON_BOAT].AnimationPhase  = 0;
            Bmp[Tiles::BUTTON_PIPE].AnimationPhase  = 0;
            Bmp[Tiles::BUTTON_HOUSE_1].AnimationPhase = 0;
            Bmp[Tiles::BUTTON_HOUSE_2].AnimationPhase = 0;
            Bmp[Tiles::BUTTON_HOUSE_3].AnimationPhase = 0;
        }//

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
        {
            s_GameState = State::OUTRO;
        }
#endif

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (s_SoundState == 0) {
                s_SoundState = 1;
            } else if (s_SoundState == 1) {
                s_SoundState = 0;
            }
        }
    } else if (s_GameState == State::OUTRO) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            StopSound(Sound::OUTRO);
            return 0;
        }
    }

    return 1;
}
} // namespace Direct
