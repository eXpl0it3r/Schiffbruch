#include "Renderer.hpp"

#include "Action.hpp"
#include "Direct.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Menu.hpp"
#include "Sound.hpp"
#include "Routing.hpp"
#include "World.hpp"
#include "Application.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cassert>

#include <SFML/Window.hpp>

#define MB_LEFT sf::Mouse::Left;

namespace Renderer
{
sf::Texture* createEmptyTexture(const size_t width, const size_t height, const sf::Color& color)
{
    printf("create empty: %lu %lu\n", width, height);
    assert(width > 0 && height > 0);

    sf::Image image;
    image.create(width, height, color);
    printf("loda empty image: %d %d\n", image.getSize().x, image.getSize().y);

    sf::Texture* texture = new sf::Texture;
    if (!texture->loadFromImage(image))
    {
        fprintf(stderr, "Failed to load texture from empty image width size %dx%d\n", image.getSize().x,
                image.getSize().y);
        assert(false);
    }
    return texture;
}

sf::Texture* loadTexture(const void* data, const size_t size, const sf::Color& mask)
{
    printf("Loading %lu bytes from %p...\n", size, data);
    sf::Image image;
    if (!image.loadFromMemory(data, size) || ! image.getSize().x)
    {
        fprintf(stderr, "Failed to load bmp\n");
        return nullptr;
    }
    printf("loda image: %d %d\n", image.getSize().x, image.getSize().y);

    image.createMaskFromColor(mask);

    sf::Texture* texture = new sf::Texture;
    texture->loadFromImage(image);
    if (!texture->loadFromImage(image))
    {
        fprintf(stderr, "Failed to load texture from image width size %dx%d\n", image.getSize().x, image.getSize().y);
        assert(false);
    }
    assert(texture->getSize().x > 0 && texture->getSize().y > 0);
    return texture;
}

void Fade(short RP, short GP, short BP)
{
    s_darknessColor.r = 255 * RP / 100;
    s_darknessColor.g = 255 * GP / 100;
    s_darknessColor.b = 255 * BP / 100;
}

void LimitScroll()
{
    if (Camera.x < ScapeGrenze.left)
    {
        Camera.x = static_cast<short>(ScapeGrenze.left);
    }

    if (Camera.x + rcPlayingSurface.right > ScapeGrenze.right)
    {
        Camera.x = static_cast<short>(ScapeGrenze.right) - static_cast<short>(rcPlayingSurface.right);
    }

    if (Camera.y < ScapeGrenze.top)
    {
        Camera.y = static_cast<short>(ScapeGrenze.top);
    }

    if (Camera.y + rcPlayingSurface.bottom > ScapeGrenze.bottom)
    {
        Camera.y = static_cast<short>(ScapeGrenze.bottom) - static_cast<short>(rcPlayingSurface.bottom);
    }
}

Coordinate GetTile(short PosX, short PosY)
{
    for (short y = 0; y < MAX_TILESY; y++)
    {
        for (short x = 0; x < MAX_TILES_X; x++)
        {
            // Die in Betracht kommenden Kacheln rausfinden
            if ((PosX > Landscape[x][y].xScreen) && (PosX < Landscape[x][y].xScreen + TILE_SIZE_X) &&
                (PosY > Landscape[x][y].yScreen) && (PosY < Landscape[x][y].yScreen + TILE_SIZE_Y))
            {
                if ((Math::InTriangle(PosX, PosY,
                                      Landscape[x][y].xScreen + CornerCoord[Landscape[x][y].Type][0].x,
                                      Landscape[x][y].yScreen + CornerCoord[Landscape[x][y].Type][0].y,
                                      Landscape[x][y].xScreen + CornerCoord[Landscape[x][y].Type][1].x,
                                      Landscape[x][y].yScreen + CornerCoord[Landscape[x][y].Type][1].y,
                                      Landscape[x][y].xScreen + CornerCoord[Landscape[x][y].Type][3].x,
                                      Landscape[x][y].yScreen + CornerCoord[Landscape[x][y].Type][3].y)) ||
                    (Math::InTriangle(PosX, PosY,
                                      Landscape[x][y].xScreen + CornerCoord[Landscape[x][y].Type][2].x,
                                      Landscape[x][y].yScreen + CornerCoord[Landscape[x][y].Type][2].y,
                                      Landscape[x][y].xScreen + CornerCoord[Landscape[x][y].Type][1].x,
                                      Landscape[x][y].yScreen + CornerCoord[Landscape[x][y].Type][1].y,
                                      Landscape[x][y].xScreen + CornerCoord[Landscape[x][y].Type][3].x,
                                      Landscape[x][y].yScreen + CornerCoord[Landscape[x][y].Type][3].y)))
                {
                    return { x, y };
                }
            }
        }
    }
    return { -1, -1 };
}

void BlitToText(sf::Texture* from)
{
    sf::IntRect srcrect;
    srcrect.left = rcRectsrc.left;
    srcrect.top = rcRectsrc.top;
    srcrect.width = rcRectsrc.right - rcRectsrc.left;
    srcrect.height = rcRectsrc.bottom - rcRectsrc.top;

    int dstWidth = rcRectdes.right - rcRectdes.left;
    int dstHeight = rcRectdes.bottom - rcRectdes.top;
    srcrect.width = std::min(dstWidth, srcrect.width);
    srcrect.height = std::min(dstHeight, srcrect.height);

    assert(srcrect.width > 0 && srcrect.height > 0);
    if (srcrect.width <= 0 || srcrect.height <= 0)
    {
        return;
    }
    sf::Sprite sprite;
    sprite.setTexture(*from);
    sprite.setPosition(rcRectdes.left, rcRectdes.top);
    sprite.setTextureRect(srcrect);
    Application::drawToText(sprite);
}

void BlitToScreen(sf::Texture* from)
{
    sf::IntRect srcrect;
    srcrect.left = rcRectsrc.left;
    srcrect.top = rcRectsrc.top;
    srcrect.width = rcRectsrc.right - rcRectsrc.left;
    srcrect.height = rcRectsrc.bottom - rcRectsrc.top;

    int dstWidth = rcRectdes.right - rcRectdes.left;
    int dstHeight = rcRectdes.bottom - rcRectdes.top;

    if (dstWidth > 0 && dstHeight > 0)
    {
        srcrect.width = std::min(dstWidth, srcrect.width);
        srcrect.height = std::min(dstHeight, srcrect.height);
    }

    if (srcrect.width <= 0 || srcrect.height <= 0)
    {
        return;
    }
    sf::Sprite sprite;
    sprite.setTexture(*from);
    sprite.setPosition(rcRectdes.left, rcRectdes.top);
    sprite.setTextureRect(srcrect);
    Application::drawSprite(sprite);
}

void BlitToLandscape(sf::Texture* from)
{
    sf::IntRect srcrect(rcRectsrc.left, rcRectsrc.top, rcRectsrc.right - rcRectsrc.left,
                        rcRectsrc.bottom - rcRectsrc.top);
    assert(srcrect.width > 0 && srcrect.height > 0);
    if (srcrect.width <= 0 || srcrect.height <= 0)
    {
        return;
    }
    sf::Sprite sprite;
    sprite.setTexture(*from);
    sprite.setPosition(rcRectdes.left, rcRectdes.top);
    sprite.setTextureRect(srcrect);
    Application::drawToLandscape(sprite);
}

void PutPixel(short x, short y, uint8_t r, uint8_t g, uint8_t b, sf::Image* img)
{
    if (x < 0 || y < 0 || x >= static_cast<int>(img->getSize().x) || y >= static_cast<int>(img->getSize().y))
    {
        return;
    }
    if (!img)
    {
        return;
    }
    img->setPixel(x, y, { r, g, b });
    //    WORD *pixels = static_cast<WORD *>(ddsd->lpSurface);
    //    // DWORD pitch = ddsd->dwWidth+2;
    //    DWORD pitch = ddsd->lPitch >> 1;
    //    pixels[y * pitch + x * 2] = static_cast<WORD>(color);
}

void GetPixel(short x, short y, sf::Image* img)
{
    //    WORD *pixels = static_cast<WORD *>(ddsd->lpSurface);
    //    // DWORD pitch = ddsd->dwWidth;
    //    DWORD pitch = ddsd->lPitch >> 1;
    //    DWORD color = pixels[y * pitch + x * 2];

    // TODO: more efficient
    if (x < 0 || y < 0 || x >= static_cast<int>(img->getSize().x) || y >= static_cast<int>(img->getSize().y))
    {
        rgbStruct.r = 0;
        return;
    }
    if (!img)
    {
        return;
    }

    sf::Color c = img->getPixel(x, y);
    rgbStruct.r = c.r;
    rgbStruct.g = c.g;
    rgbStruct.b = c.b;
}

void DrawPicture(short x, short y, short i, RECT target, bool Reverse, short Fruit)
{
    short Phase;

    if (Fruit == -1)
    {
        Phase = Bmp[i].AnimationPhase;
    }
    else
    {
        Phase = Fruit;
    }

    rcRectsrc = Bmp[i].sourceRect;
    if (i == TENT)
    {
        printf("tent: %d, phase: %d, tent: %d\n", i, Phase, TENT);
    }

    if (!Reverse)
    {
        rcRectsrc.top += Phase * (Bmp[i].Height);
    }
    else
    {
        rcRectsrc.top = Bmp[i].sourceRect.top + (Bmp[i].AnimationPhaseCount - 1) * Bmp[i].Height - Phase * Bmp[i].
            Height;
    }

    rcRectsrc.bottom = rcRectsrc.top + (Bmp[i].Height);
    rcRectdes.left = x;
    rcRectdes.top = y;
    rcRectdes.right = x + (Bmp[i].Width);
    rcRectdes.bottom = y + (Bmp[i].Height);
    Math::CalcRect(target);
    BlitToScreen(Bmp[i].Surface);
}

void DrawObjects()
{
    for (short y = 0; y < MAX_TILESY; y++)
    {
        for (short x = 0; x < MAX_TILES_X; x++)
        {
            bool drawGuy = false;

            if ((Guy.Pos.x == x) && (Guy.Pos.y == y))
            {
                drawGuy = true;
            }

            // Filter out the invisible tiles (or unaffected)

            if (!((Landscape[x][y].xScreen > Camera.x + rcPlayingSurface.left - TILE_SIZE_X) &&
                (Landscape[x][y].xScreen < Camera.x + rcPlayingSurface.right + TILE_SIZE_X) &&
                (Landscape[x][y].yScreen > Camera.y + rcPlayingSurface.top - TILE_SIZE_Y) &&
                (Landscape[x][y].yScreen < Camera.y + rcPlayingSurface.bottom + TILE_SIZE_Y) &&
                (Landscape[x][y].Discovered) &&
                ((Landscape[x][y].Marked) || (Landscape[x][y].Object != -1) || (drawGuy))))
            {
                continue;
            }

            if (Landscape[x][y].Marked)
            { // Paint the frames around the marked tiles
                rcRectsrc.left = TILE_SIZE_X * Landscape[x][y].Type;
                rcRectsrc.right = TILE_SIZE_X * Landscape[x][y].Type + TILE_SIZE_X;
                rcRectsrc.top = 2 * TILE_SIZE_Y;
                rcRectsrc.bottom = 3 * TILE_SIZE_Y;
                rcRectdes.left = Landscape[x][y].xScreen - Camera.x;
                rcRectdes.right = rcRectdes.left + TILE_SIZE_X;
                rcRectdes.top = Landscape[x][y].yScreen - Camera.y;
                rcRectdes.bottom = rcRectdes.top + TILE_SIZE_Y;
                Math::CalcRect(rcPlayingSurface);
                BlitToScreen(lpDDSMisc);
            }

            // paint landscape animations (and field)
            if ((Landscape[x][y].Object != -1) && (LAnimation) &&
                (((Landscape[x][y].Object <= FLOODGATE_6))
                    || (Landscape[x][y].Object == FIELD) // Der Guy ist immer vor diesen Objecten
                    || (Landscape[x][y].Object == PIPE)
                    || (Landscape[x][y].Object == SOS)))
            {
                // Sound abspielen
                if (
                    ((Guy.Pos.x - 1 <= x) && (x <= Guy.Pos.x + 1)) &&
                    ((Guy.Pos.y - 1 <= y) && (y <= Guy.Pos.y + 1)))
                {
                    if ((x == Guy.Pos.x) && (y == Guy.Pos.y))
                    {
                        Sound::PlaySound(Bmp[Landscape[x][y].Object].Sound, 100);
                    }
                    else if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == -1 || Bmp[Landscape[x][y].Object].Sound != Bmp[
                        Landscape[Guy.Pos.x][Guy.Pos.y].Object].Sound)
                    {
                        Sound::PlaySound(Bmp[Landscape[x][y].Object].Sound, 90);
                    }
                }

                DrawPicture(Landscape[x][y].xScreen + Landscape[x][y].ObjectPosOffset.x - Camera.x,
                            Landscape[x][y].yScreen + Landscape[x][y].ObjectPosOffset.y - Camera.y,
                            Landscape[x][y].Object, rcPlayingSurface, Landscape[x][y].ReverseAnimation,
                            static_cast<short>(Landscape[x][y].AnimationPhase));
            }
            else
            {
                if ((((Landscape[x][y].Object >= TREE_1) && (Landscape[x][y].Object <= TREE_DOWN_4))) ||
                    (Landscape[x][y].Object == TREE_BIG) || (Landscape[x][y].Object == FIRE) ||
                    (Landscape[x][y].Object == WRECK_1) || (Landscape[x][y].Object == WRECK_2) ||
                    (Landscape[x][y].Object >= TENT))
                { // Bäume und Früchte (und alle anderen Objecte) malen
                    // Sound abspielen
                    if (Landscape[x][y].Object != -1 &&
                        ((Guy.Pos.x - 1 <= x) && (x <= Guy.Pos.x + 1)) &&
                        ((Guy.Pos.y - 1 <= y) && (y <= Guy.Pos.y + 1)))
                    {
                        if ((x == Guy.Pos.x) && (y == Guy.Pos.y))
                        {
                            Sound::PlaySound(Bmp[Landscape[x][y].Object].Sound, 100);
                        }
                        else if (Landscape[Guy.Pos.x][Guy.Pos.y].Object == -1 || Bmp[Landscape[x][y].Object].Sound !=
                            Bmp[Landscape[Guy.Pos.x][Guy.Pos.y].Object].Sound)
                        {
                            Sound::PlaySound(Bmp[Landscape[x][y].Object].Sound, 90);
                        }
                    }

                    if (drawGuy)
                    {
                        if ((Guy.ScreenPosition.y) < (Landscape[x][y].yScreen + Landscape[x][y].ObjectPosOffset.y
                            + Bmp[Landscape[x][y].Object].Height))
                        {
                            DrawGuy();
                            drawGuy = false;
                        }
                    }

                    DrawPicture(Landscape[x][y].xScreen + Landscape[x][y].ObjectPosOffset.x - Camera.x,
                                Landscape[x][y].yScreen + Landscape[x][y].ObjectPosOffset.y - Camera.y,
                                Landscape[x][y].Object, rcPlayingSurface, false,
                                static_cast<short>(Landscape[x][y].AnimationPhase));
                }
            }

            if (drawGuy)
            {
                DrawGuy();
            }
        }
    }
}

void DrawGuy()
{
    if (IsInBoat)
    {
        if (Guy.AnimationState == GUY_SHIP)
        {
            DrawPicture(Guy.ScreenPosition.x - 30 - Camera.x,
                        Guy.ScreenPosition.y - 28 - Camera.y,
                        Guy.AnimationState, rcPlayingSurface, false, -1);
        }
        else
        {
            DrawPicture(Guy.ScreenPosition.x - (Bmp[Guy.AnimationState].Width) / 2 - Camera.x,
                        Guy.ScreenPosition.y - (Bmp[Guy.AnimationState].Height) / 2 - Camera.y,
                        Guy.AnimationState, rcPlayingSurface, false, -1);
        }
    }
    else
        DrawPicture(Guy.ScreenPosition.x - (Bmp[Guy.AnimationState].Width) / 2 - Camera.x,
                    Guy.ScreenPosition.y - (Bmp[Guy.AnimationState].Height) - Camera.y,
                    Guy.AnimationState, rcPlayingSurface, false, -1);

    // Sound abspielen
    if (Guy.IsActive)
    {
        Sound::PlaySound(Bmp[Guy.AnimationState].Sound, 100);
    }
}

void DrawPaper()
{
    rcRectsrc.left = 0;
    rcRectsrc.top = 0;
    rcRectsrc.right = 464;
    rcRectsrc.bottom = 77;
    rcRectdes.left = TextBereich[TXTPAPIER].textRect.left - 60;
    rcRectdes.top = TextBereich[TXTPAPIER].textRect.top - 30;
    rcRectdes.right = rcRectdes.left + 464;
    rcRectdes.bottom = rcRectdes.top + 77;
    BlitToScreen(lpDDSPaper);

    rcRectdes.left = rcRectdes.left + 34;
    rcRectdes.top = rcRectdes.top + 77;
    rcRectdes.right = rcRectdes.right + 0;
    rcRectdes.bottom = TextBereich[TXTPAPIER].textRect.top + PapierText;

    // TODO: check that this works
    sf::RectangleShape rect(sf::Vector2f(rcRectdes.right - rcRectdes.left, rcRectdes.bottom - rcRectdes.top));
    rect.setFillColor(sf::Color(236, 215, 179));
    rect.setPosition(rcRectdes.left, rcRectdes.top);
    Application::drawToScreen(rect);

    rcRectsrc.left = 0;
    rcRectsrc.top = 77;
    rcRectsrc.right = 464;
    rcRectsrc.bottom = 154;
    rcRectdes.left = TextBereich[TXTPAPIER].textRect.left - 60;
    rcRectdes.top = rcRectdes.bottom - 47;
    rcRectdes.right = rcRectdes.left + 464;
    rcRectdes.bottom = rcRectdes.top + 77;
    BlitToScreen(lpDDSPaper);
}

void DrawPanel()
{
    // Karte
    rcRectsrc.left = 0;
    rcRectsrc.top = 0;
    rcRectsrc.right = 2 * MAX_TILES_X;
    rcRectsrc.bottom = 2 * MAX_TILESY;
    rcRectdes.left = rcKarte.left;
    rcRectdes.top = rcKarte.top;
    rcRectdes.right = rcKarte.right;
    rcRectdes.bottom = rcKarte.bottom;
    BlitToScreen(lpDDSKarte);
    //    Blit(lpDDSKarte, lpDDSBack, false);

    // Spielfigur
    sf::RectangleShape rect(sf::Vector2f(2, 2));
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(rcKarte.left + 2 * Guy.Pos.x, rcKarte.top + 2 * Guy.Pos.y);
    Application::drawToScreen(rect);

    // Position einmalen
    rcRectsrc.left = 205;
    rcRectsrc.top = 0;
    rcRectsrc.right = 205 + 65;
    rcRectsrc.bottom = 0 + 65;
    rcRectdes.left = rcKarte.left + (Camera.x + 2 * Camera.y) / (TILE_SIZE_X / 2) - MAX_TILES_X - 2;
    rcRectdes.top = rcKarte.top + (2 * Camera.y - Camera.x) / (TILE_SIZE_X / 2) + MAX_TILESY - 21 - 2;
    rcRectdes.right = rcRectdes.left + 65;
    rcRectdes.bottom = rcRectdes.top + 65;
    Math::CalcRect(rcKarte);
    BlitToScreen(lpDDSPanel);

    // Panel malen
    rcRectsrc.left = 0;
    rcRectsrc.top = 0;
    rcRectsrc.right = 205;
    rcRectsrc.bottom = 720;
    rcRectdes.left = rcPanel.left;
    rcRectdes.top = rcPanel.top;
    rcRectdes.right = rcPanel.right;
    rcRectdes.bottom = rcPanel.bottom;
    BlitToScreen(lpDDSPanel);

    // Gitternetzknopf
    if (Gitter)
    {
        Bmp[BUTTON_GRID].AnimationPhase = 1;
    }
    else
    {
        Bmp[BUTTON_GRID].AnimationPhase = 0;
    }

    DrawPicture(static_cast<short>(Bmp[BUTTON_GRID].targetRect.left),
                static_cast<short>(Bmp[BUTTON_GRID].targetRect.top),
                BUTTON_GRID, rcPanel, false, -1);

    // SOUNDknopf
    if ((s_SoundState == 0) || (s_SoundState == -1))
    {
        Bmp[BUTTON_SOUND].AnimationPhase = 1;
    }
    else
    {
        Bmp[BUTTON_SOUND].AnimationPhase = 0;
    }

    DrawPicture(static_cast<short>(Bmp[BUTTON_SOUND].targetRect.left),
                static_cast<short>(Bmp[BUTTON_SOUND].targetRect.top),
                BUTTON_SOUND, rcPanel, false, -1);

    // ANIMATIONknopf
    if (!LAnimation)
    {
        Bmp[BUTTON_ANIMATION].AnimationPhase = 1;
    }
    else
    {
        Bmp[BUTTON_ANIMATION].AnimationPhase = 0;
    }

    DrawPicture(static_cast<short>(Bmp[BUTTON_ANIMATION].targetRect.left),
                static_cast<short>(Bmp[BUTTON_ANIMATION].targetRect.top),
                BUTTON_ANIMATION, rcPanel, false, -1);

    // BEENDENknopf
    DrawPicture(static_cast<short>(Bmp[BUTTON_END].targetRect.left),
                static_cast<short>(Bmp[BUTTON_END].targetRect.top),
                BUTTON_END, rcPanel, false, -1);

    // NEUknopf
    DrawPicture(static_cast<short>(Bmp[BUTTON_NEW].targetRect.left),
                static_cast<short>(Bmp[BUTTON_NEW].targetRect.top),
                BUTTON_NEW, rcPanel, false, -1);

    // TAGNEUknopf
    DrawPicture(static_cast<short>(Bmp[BUTTON_NEW_DAY].targetRect.left),
                static_cast<short>(Bmp[BUTTON_NEW_DAY].targetRect.top),
                BUTTON_NEW_DAY, rcPanel, false, -1);

    // Aktionsknopf
    if (HauptMenue == Menu::ACTION)
    {
        Bmp[BUTTON_ACTION].AnimationPhase = Bmp[BUTTON_ACTION].AnimationPhaseCount;
    }
    else if (Bmp[BUTTON_ACTION].AnimationPhase == Bmp[BUTTON_ACTION].AnimationPhaseCount)
    {
        Bmp[BUTTON_ACTION].AnimationPhase = 0;
    }

    DrawPicture(static_cast<short>(Bmp[BUTTON_ACTION].targetRect.left),
                static_cast<short>(Bmp[BUTTON_ACTION].targetRect.top),
                BUTTON_ACTION, rcPanel, false, -1);

    // BauKnopf
    if (HauptMenue == Menu::BUILD)
    {
        Bmp[BUTTON_CONSTRUCT].AnimationPhase = Bmp[BUTTON_CONSTRUCT].AnimationPhaseCount;
    }
    else if (Bmp[BUTTON_CONSTRUCT].AnimationPhase == Bmp[BUTTON_CONSTRUCT].AnimationPhaseCount)
    {
        Bmp[BUTTON_CONSTRUCT].AnimationPhase = 0;
    }

    DrawPicture(static_cast<short>(Bmp[BUTTON_CONSTRUCT].targetRect.left),
                static_cast<short>(Bmp[BUTTON_CONSTRUCT].targetRect.top),
                BUTTON_CONSTRUCT, rcPanel, false, -1);

    // Inventoryknopf
    if (HauptMenue == Menu::INVENTORY)
    {
        Bmp[BUTTON_INVENTORY].AnimationPhase = Bmp[BUTTON_INVENTORY].AnimationPhaseCount;
    }
    else if (Bmp[BUTTON_INVENTORY].AnimationPhase == Bmp[BUTTON_INVENTORY].AnimationPhaseCount)
    {
        Bmp[BUTTON_INVENTORY].AnimationPhase = 0;
    }

    DrawPicture(static_cast<short>(Bmp[BUTTON_INVENTORY].targetRect.left),
                static_cast<short>(Bmp[BUTTON_INVENTORY].targetRect.top),
                BUTTON_INVENTORY, rcPanel, false, -1);

    // WEITERknopf
    if (Bmp[BUTTON_CONTINUE].AnimationPhase != -1)
        DrawPicture(static_cast<short>(Bmp[BUTTON_CONTINUE].targetRect.left),
                    static_cast<short>(Bmp[BUTTON_CONTINUE].targetRect.top),
                    BUTTON_CONTINUE, rcPanel, false, -1);

    // STOPknopf
    if (Bmp[BUTTON_STOP].AnimationPhase != -1)
        DrawPicture(static_cast<short>(Bmp[BUTTON_STOP].targetRect.left),
                    static_cast<short>(Bmp[BUTTON_STOP].targetRect.top),
                    BUTTON_STOP, rcPanel, false, -1);

    // ABLEGENknopf
    if (Bmp[BUTTON_LAY_DOWN].AnimationPhase != -1)
        DrawPicture(static_cast<short>(Bmp[BUTTON_LAY_DOWN].targetRect.left),
                    static_cast<short>(Bmp[BUTTON_LAY_DOWN].targetRect.top),
                    BUTTON_LAY_DOWN, rcPanel, false, -1);

    // Welches Menü zeichnen?
    switch (HauptMenue)
    {
    case Menu::ACTION:
        for (short i = BUTTON_SEARCH; i <= BUTTON_SLINGSHOT; i++)
        {
            if (Bmp[i].AnimationPhase == -1)
            {
                DrawPicture(static_cast<short>(Bmp[i].targetRect.left),
                            static_cast<short>(Bmp[i].targetRect.top),
                            BUTTON_QUESTION, rcPanel, false, -1);
                continue;
            }

            DrawPicture(static_cast<short>(Bmp[i].targetRect.left),
                        static_cast<short>(Bmp[i].targetRect.top),
                        i, rcPanel, false, -1);
        }

        break;

    case Menu::BUILD:
        for (short i = BUTTON_TENT; i <= BUTTON_DESTROY; i++)
        {
            if (Bmp[i].AnimationPhase == -1)
            {
                DrawPicture(static_cast<short>(Bmp[i].targetRect.left),
                            static_cast<short>(Bmp[i].targetRect.top),
                            BUTTON_QUESTION, rcPanel, false, -1);
                continue;
            }

            DrawPicture(static_cast<short>(Bmp[i].targetRect.left),
                        static_cast<short>(Bmp[i].targetRect.top),
                        i, rcPanel, false, -1);
        }

        break;

    case Menu::INVENTORY:
        DrawPicture(static_cast<short>(Bmp[INVENTORY_PAPER].targetRect.left),
                    static_cast<short>(Bmp[INVENTORY_PAPER].targetRect.top),
                    INVENTORY_PAPER, rcPanel, false, -1);

        for (short i = RAW_TREE_BRANCH; i <= RAW_SLINGSHOT; i++)
        {
            if (Guy.Inventory[i] <= 0)
            {
                continue;
            }

            DrawPicture(static_cast<short>(Bmp[i].targetRect.left),
                        static_cast<short>(Bmp[i].targetRect.top),
                        i, rcPanel, false, -1);
            Bmp[ROEMISCH1].targetRect.top = Bmp[i].targetRect.top;
            Bmp[ROEMISCH2].targetRect.top = Bmp[i].targetRect.top;

            for (short j = 1; j <= Guy.Inventory[i]; j++)
            {
                if (j < 5)
                {
                    DrawPicture(static_cast<short>(Bmp[i].targetRect.left) + 20 + j * 4,
                                static_cast<short>(Bmp[ROEMISCH1].targetRect.top),
                                ROEMISCH1, rcPanel, false, -1);
                }
                else if (j == 5)
                {
                    DrawPicture(static_cast<short>(Bmp[i].targetRect.left) + 23,
                                static_cast<short>(Bmp[ROEMISCH2].targetRect.top),
                                ROEMISCH2, rcPanel, false, -1);
                }
                else if ((j > 5) && (j < 10))
                {
                    DrawPicture(static_cast<short>(Bmp[i].targetRect.left) + 20 + j * 4,
                                static_cast<short>(Bmp[ROEMISCH1].targetRect.top),
                                ROEMISCH1, rcPanel, false, -1);
                }
                else if (j == 10)
                {
                    DrawPicture(static_cast<short>(Bmp[i].targetRect.left) + 43,
                                static_cast<short>(Bmp[ROEMISCH2].targetRect.top),
                                ROEMISCH2, rcPanel, false, -1);
                }
            }
        }

        break;
    }

    // Säule1
    short i = Bmp[COLUMN_1].Height - static_cast<short>(Guy.ResourceAmount[Water]) * Bmp[COLUMN_1].Height / 100;
    rcRectsrc = Bmp[COLUMN_1].sourceRect;
    rcRectsrc.top += i;
    rcRectdes = Bmp[COLUMN_1].targetRect;
    rcRectdes.top += i;
    BlitToScreen(Bmp[COLUMN_1].Surface);

    // Säule2
    i = Bmp[COLUMN_2].Height - static_cast<short>(Guy.ResourceAmount[Food]) * Bmp[COLUMN_2].Height / 100;
    rcRectsrc = Bmp[COLUMN_2].sourceRect;
    rcRectsrc.top += i;
    rcRectdes = Bmp[COLUMN_2].targetRect;
    rcRectdes.top += i;
    BlitToScreen(Bmp[COLUMN_2].Surface);

    // Säule3
    i = Bmp[COLUMN_3].Height - static_cast<short>(Guy.ResourceAmount[Health]) * Bmp[COLUMN_3].Height / 100;
    rcRectsrc = Bmp[COLUMN_3].sourceRect;
    rcRectsrc.top += i;
    rcRectdes = Bmp[COLUMN_3].targetRect;
    rcRectdes.top += i;
    BlitToScreen(Bmp[COLUMN_3].Surface);

    // Sonnenanzeige
    short diffx = (static_cast<short>(Bmp[SUN].targetRect.right) - static_cast<short>(Bmp[SUN].targetRect.left) - Bmp[SUN].Width) / 2;
    short diffy = static_cast<short>(Bmp[SUN].targetRect.bottom) - static_cast<short>(Bmp[SUN].targetRect.top) - Bmp[SUN].Height / 2;
    short TagesZeit = (Hours * 10 + Minutes * 10 / 60);

    DrawPicture(static_cast<short>(Bmp[SUN].targetRect.left + diffx * cos(M_PI - M_PI * TagesZeit / 120) + diffx),
                static_cast<short>(Bmp[SUN].targetRect.top + (-diffy * sin(M_PI - M_PI * TagesZeit / 120) + diffy)),
                SUN, Bmp[SUN].targetRect, false, -1);

    // Rettungsring
    short Ringtmp;

    if (Chance < 100)
    {
        Ringtmp = static_cast<short>(100 * sin(M_PI / 200 * Chance));
    }
    else
    {
        Ringtmp = 100;
    }

    if (Ringtmp > 100)
    {
        Ringtmp = 100;
    }

    DrawPicture(static_cast<short>(Bmp[RING].targetRect.left),
                static_cast<short>(Bmp[RING].targetRect.top + Ringtmp),
                RING, rcPanel, false, -1);

    // Die ChanceZahl ausgeben
    HideText(TXTCHANCE);
    TextBereich[TXTCHANCE].HasText = true;
    TextBereich[TXTCHANCE].textRect.top = Bmp[RING].targetRect.top + Ringtmp + Bmp[RING].Height;
    TextBereich[TXTCHANCE].textRect.bottom = TextBereich[TXTCHANCE].textRect.top + FONT2_LETTER_HEIGHT;
    std::sprintf(StdString, "%.1f", Chance);
    DrawString(StdString, static_cast<short>(TextBereich[TXTCHANCE].textRect.left),
               static_cast<short>(TextBereich[TXTCHANCE].textRect.top), 2);

    // TextFeld malen
    rcRectsrc.left = 0;
    rcRectsrc.top = 0;
    rcRectsrc.right = 605;
    rcRectsrc.bottom = 20;
    rcRectdes = { 0, MAX_SCREEN_Y - 20, MAX_SCREEN_X - 195, MAX_SCREEN_Y };
    BlitToScreen(lpDDSTextFeld);
}

void DrawString(const char* string, short x, short y, short Art)
{
    short Width = 0;
    short Height = 0;

    if (Art == 1)
    {
        Width = FONT1_LETTER_WIDTH;
        Height = FONT1_LETTER_HEIGHT;
    }

    if (Art == 2)
    {
        Width = FONT2_LETTER_WIDTH;
        Height = FONT2_LETTER_HEIGHT;
    }

    // Länge der Schrift ermitteln
    std::size_t length = strlen(string);

    // Alle Zeichen durchgehen
    for (std::size_t index = 0; index < length; index++)
    {
        // Korrekte indexNummer ermitteln
        short cindex = string[index] - ' ';

        if ((string[index] >= ' ') && (string[index] <= '/'))
        {
            rcRectsrc.left = cindex * Width;
            rcRectsrc.top = 0;
        }

        if ((string[index] >= '0') && (string[index] <= '?'))
        {
            rcRectsrc.left = (cindex - 16) * Width;
            rcRectsrc.top = Height;
        }

        if ((string[index] >= '@') && (string[index] <= 'O'))
        {
            rcRectsrc.left = (cindex - 16 * 2) * Width;
            rcRectsrc.top = 2 * Height;
        }

        if ((string[index] >= 'P') && (string[index] <= '_'))
        {
            rcRectsrc.left = (cindex - 16 * 3) * Width;
            rcRectsrc.top = 3 * Height;
        }

        if ((string[index] > '_') && (string[index] <= 'o'))
        {
            rcRectsrc.left = (cindex - 16 * 4) * Width;
            rcRectsrc.top = 4 * Height;
        }

        if ((string[index] >= 'p') && (string[index] <= '~'))
        {
            rcRectsrc.left = (cindex - 16 * 5) * Width;
            rcRectsrc.top = 5 * Height;
        }

        rcRectsrc.right = rcRectsrc.left + Width;
        rcRectsrc.bottom = rcRectsrc.top + Height;
        rcRectdes.left = x;
        rcRectdes.top = y;
        rcRectdes.right = x + Width;
        rcRectdes.bottom = y + Height;

        // Zeichen zeichnen
        if (Art == 1)
        {
            BlitToText(lpDDSSchrift1);
            //            Blit(lpDDSSchrift1, lpDDSSchrift, true);
            // x Position weiterschieben
            x += FONT1_LETTER_SPACING;
        }

        if (Art == 2)
        {
            BlitToText(lpDDSSchrift2);
            // x Position weiterschieben
            x += FONT2_LETTER_SPACING;
        }
    }
}

short DrawText(const int TEXT, short Bereich, short Art)
{
    short BWidth = 0;
    short BHeight = 0;
    const char* Text;
    int blank = ' ';
    int slash = '/';
    int strend = 0x0;
    char StdString2[10]; // Zur Variablenausgabe
    short Anzahl; // Zur Variablenausgabe

    HideText(Bereich);
    TextBereich[Bereich].HasText = true;

    if (Art == 1)
    {
        BWidth = FONT1_LETTER_SPACING;
        BHeight = FONT1_LETTER_HEIGHT;
    }

    if (Art == 2)
    {
        BWidth = FONT2_LETTER_SPACING;
        BHeight = FONT2_LETTER_HEIGHT;
    }

    Text = GetLanguageString(TEXT);
    short Posx = static_cast<short>(TextBereich[Bereich].textRect.left);
    short Posy = static_cast<short>(TextBereich[Bereich].textRect.top);
    const char* Posnext = Text;

    while (true)
    {
        strcpy(StdString, "");
        short Pos = Posnext - Text;
        Posnext = strchr(Text + Pos + 1, blank);
        const char* Posnext2 = strchr(Text + Pos + 1, slash);

        if ((Posnext != nullptr) && (Posnext2 != nullptr) && (Posnext2 <= Posnext))
        {
            char scratch = *(Posnext2 + 1);

            switch (scratch)
            {
            case 'a':
                Anzahl = std::sprintf(StdString2, " %d", Tag);
                DrawString(StdString2, Posx, Posy, Art);
                Posx += BWidth * (Anzahl);
                break;

            case 'b':
                Anzahl = std::sprintf(StdString2, " %d", static_cast<short>(Guy.ResourceAmount[Health]));
                DrawString(StdString2, Posx, Posy, Art);
                Posx += BWidth * (Anzahl);
                break;

            case 'c':
                Anzahl = std::sprintf(StdString2, " %.2f", Chance);
                DrawString(StdString2, Posx, Posy, Art);
                Posx += BWidth * (Anzahl);
                break;

            case 'd':
                Frage = 0;
                rcRectsrc = Bmp[YES].sourceRect;
                rcRectdes.left = static_cast<short>(TextBereich[Bereich].textRect.left) + 50;
                rcRectdes.top = Posy + 50;
                rcRectdes.right = rcRectdes.left + Bmp[YES].Width;
                rcRectdes.bottom = rcRectdes.top + Bmp[YES].Height;
                Bmp[YES].targetRect = rcRectdes;
                BlitToText(Bmp[YES].Surface);

                rcRectsrc = Bmp[NO].sourceRect;
                rcRectdes.left = static_cast<short>(TextBereich[Bereich].textRect.left) + 220;
                rcRectdes.top = Posy + 50;
                rcRectdes.right = rcRectdes.left + Bmp[NO].Width;
                rcRectdes.bottom = rcRectdes.top + Bmp[NO].Height;
                Bmp[NO].targetRect = rcRectdes;
                BlitToText(Bmp[NO].Surface);
                Posy += 115;
                break;

            case 'z':
                Posx = static_cast<short>(TextBereich[Bereich].textRect.left) - BWidth;
                Posy += BHeight + 3;
                break;
            }

            Pos = Pos + 3;
            Posnext = Posnext2 + 2;
        }

        if (Posnext == nullptr)
        {
            Posnext = strchr(Text + Pos + 1, strend);
        }

        strncpy(StdString, Text + Pos, (Posnext - Text) - Pos);

        if (Posx + BWidth * ((Posnext - Text) - Pos) > TextBereich[Bereich].textRect.right)
        {
            Posx = static_cast<short>(TextBereich[Bereich].textRect.left) - BWidth;
            Posy += BHeight + 3;
        }

        StdString[(Posnext - Text) - Pos] = static_cast<char>(0);
        DrawString(StdString, Posx, Posy, Art);

        if (Posnext[0] == static_cast<char>(0))
        {
            break;
        }

        Posx += BWidth * ((Posnext - Text) - Pos);
    }

    short Erg = static_cast<short>(Posy + BHeight - TextBereich[Bereich].textRect.top);

    if (Erg < 100)
    {
        Erg = 100;
    }

    return Erg;
}

void HideText(short Area)
{
    TextBereich[Area].HasText = false;
    Application::clearText(TextBereich[Area].textRect.left,
                           TextBereich[Area].textRect.top,
                           TextBereich[Area].textRect.right - TextBereich[Area].textRect.left,
                           TextBereich[Area].textRect.bottom - TextBereich[Area].textRect.top
    );
}

void DrawSchatzkarte()
{
    HideText(TXTPAPIER);
    TextBereich[TXTPAPIER].HasText = true;
    PapierText = TREASUREMAP_HEIGHT;

    rcRectsrc.left = 0;
    rcRectsrc.right = TREASUREMAP_WIDTH;
    rcRectsrc.top = 0;
    rcRectsrc.bottom = TREASUREMAP_HEIGHT;
    rcRectdes.left = TextBereich[TXTPAPIER].textRect.left;
    rcRectdes.top = TextBereich[TXTPAPIER].textRect.top;
    rcRectdes.right = rcRectdes.left + TREASUREMAP_WIDTH;
    rcRectdes.bottom = rcRectdes.top + TREASUREMAP_HEIGHT;

    BlitToText(lpDDSSchatzkarte);
}

void Show()
{
    char Stringsave1[128], Stringsave2[128]; // Für die Zeitausgabe

    rcRectsrc.left = Camera.x + rcPlayingSurface.left;
    rcRectsrc.top = Camera.y + rcPlayingSurface.top;
    rcRectsrc.right = Camera.x + rcPlayingSurface.right;
    rcRectsrc.bottom = Camera.y + rcPlayingSurface.bottom;
    rcRectdes.left = rcPlayingSurface.left;
    rcRectdes.top = rcPlayingSurface.top;
    rcRectdes.right = rcPlayingSurface.right;
    rcRectdes.bottom = rcPlayingSurface.bottom;
    Application::setLandscapeOffset(rcRectsrc.left, rcRectsrc.top);

    DrawObjects();

    DrawPanel();

    // Die TagesZeit ausgeben
    HideText(TXTTAGESZEIT);
    TextBereich[TXTTAGESZEIT].HasText = true;
    std::sprintf(Stringsave1, "%d", Hours + 6);
    std::sprintf(Stringsave2, "%d", Minutes);
    strcpy(StdString, "");

    if (Hours + 6 < 10)
    {
        strncat(StdString, "0", TXTTAGESZEIT);
    }

    strncat(StdString, Stringsave1, TXTTAGESZEIT);
    strncat(StdString, ":", TXTTAGESZEIT);

    if (Minutes < 10)
    {
        strncat(StdString, "0", TXTTAGESZEIT);
    }

    strncat(StdString, Stringsave2, TXTTAGESZEIT);
    DrawString(StdString, static_cast<short>(TextBereich[TXTTAGESZEIT].textRect.left),
               static_cast<short>(TextBereich[TXTTAGESZEIT].textRect.top), 2);

    if (PapierText != -1)
    {
        DrawPaper();
    }

    // Paint over everything black and only show the paper with text
    if (Night)
    {
        rcRectdes.left = 0;
        rcRectdes.top = 0;
        rcRectdes.right = MAX_SCREEN_X;
        rcRectdes.bottom = MAX_SCREEN_Y;
        //        s_gameScreenVisible = false;
        //        lpDDSBack->create(lpDDSBack->getSize().x, lpDDSBack->getSize().y, sf::Color(0, 0, 0));

        if (PapierText != -1)
        {
            DrawPaper();
            rcRectsrc = TextBereich[TXTPAPIER].textRect;
            rcRectdes = TextBereich[TXTPAPIER].textRect;
            //            Blit(lpDDSSchrift, lpDDSBack, true);
        }

        Fade(100, 100, 100);
    }

    // Cursor
    if (CursorTyp == CURSOR_ARROW)
        DrawPicture(MousePosition.x, MousePosition.y,
                    CursorTyp, rcGesamt, false, -1);
    else
        DrawPicture(MousePosition.x - Bmp[CursorTyp].Width / 2,
                    MousePosition.y - Bmp[CursorTyp].Height / 2,
                    CursorTyp, rcGesamt, false, -1);

    // Flippen
    // TODO: look up the directx api, see if this is still relevant
//    while (true) {
//        HRESULT ddrval = lpDDSPrimary->Flip(nullptr, 0);

    //        if (ddrval == DD_OK) {
    //            break;
    //        }

    //        if (ddrval == DDERR_SURFACELOST) {
    //            ddrval = lpDDSPrimary->Restore();

    //            if (ddrval != DD_OK) {
    //                break;
    //            }
    //        }

    //        if (ddrval != DDERR_WASSTILLDRAWING) {
    //            break;
    //        }
    //    }

    if (Night)
    {
        Fade(100, 100, 100);    // Das muß hier stehen, damit man die Textnachricht in der Nacht lesen kann
    }
}

void ShowIntro()
{
    rcRectdes.left = 0;
    rcRectdes.top = 0;
    rcRectdes.right = MAX_SCREEN_X;
    rcRectdes.bottom = MAX_SCREEN_Y;

    // TODO: more efficient way of filling with black
    Application::clearScreenContent();

    rcRectsrc.left = Camera.x + rcPlayingSurface.left;
    rcRectsrc.top = Camera.y + rcPlayingSurface.top;
    rcRectsrc.right = Camera.x + rcPlayingSurface.right;
    rcRectsrc.bottom = Camera.y + rcPlayingSurface.bottom;
    rcRectdes.left = rcPlayingSurface.left;
    rcRectdes.top = rcPlayingSurface.top;
    rcRectdes.right = rcPlayingSurface.right;
    rcRectdes.bottom = rcPlayingSurface.bottom;
    Application::setLandscapeOffset(rcRectsrc.left, rcRectsrc.top);

    DrawObjects();

    if (PapierText != -1)
    {
        DrawPaper();
    }
}

void ShowCredits()
{
    PlaySound(Sound::OUTRO, 100);

    rcRectdes.left = 0;
    rcRectdes.top = 0;
    rcRectdes.right = MAX_SCREEN_X;
    rcRectdes.bottom = MAX_SCREEN_Y;

    Application::clearScreenContent(sf::Color(0, 0, 0, 200));

    if (CreditsState == 0)
    {
        DrawPicture(MAX_SCREEN_X / 2 - Bmp[CreditsList[CreditsNum][0].Picture].Width / 2, 100,
                    CreditsList[CreditsNum][0].Picture, rcGesamt, false, -1);

        for (int z = 1; z < 10; z++)
        {
            if (CreditsList[CreditsNum][z].IsRunning)
            {
                CreditsBlt(CreditsList[CreditsNum][z].Picture,
                           static_cast<short>(100 * sin(
                               M_PI / MAX_SCREEN_Y * (Bmp[CreditsList[CreditsNum][z].Picture].targetRect.top +
                                   Bmp[CreditsList[CreditsNum][z].IsRunning].Height / 2))));
            }
        }
    }
    else if (CreditsState == 1)
    {
        puts("State 2");
        rcRectsrc = Bmp[CreditsNum].sourceRect;
        rcRectsrc.top += Bmp[CreditsNum].AnimationPhase * Bmp[CreditsNum].Height;
        rcRectsrc.bottom = rcRectsrc.top + Bmp[CreditsNum].Height;

        rcRectdes.left = 2;
        rcRectdes.top = 2;
        rcRectdes.right = Bmp[CreditsNum].Width + 2;
        rcRectdes.bottom = Bmp[CreditsNum].Height + 2;

        BlitToScreen(Bmp[CreditsNum].Surface);

        rcRectsrc.left = 0;
        rcRectsrc.top = 0;
        rcRectsrc.right = Bmp[CreditsNum].Width + 4;
        rcRectsrc.bottom = Bmp[CreditsNum].Height + 4;

        rcRectdes.left = static_cast<short>(MAX_SCREEN_X) / 2 - rcRectsrc.right * 10 / 2;
        rcRectdes.top = static_cast<short>(MAX_SCREEN_Y) / 2 - rcRectsrc.bottom * 10 / 2;
        rcRectdes.right = rcRectdes.left + rcRectsrc.right * 10;
        rcRectdes.bottom = rcRectdes.top + rcRectsrc.bottom * 10;

        BlitToScreen(lpDDSBack);

        rcRectsrc.left = 100;
        rcRectsrc.top = 2;
        rcRectsrc.right = 100 + Bmp[CreditsNum].Width + 2;
        rcRectsrc.bottom = Bmp[CreditsNum].Height + 2;

        rcRectdes.left = 2;
        rcRectdes.top = 2;
        rcRectdes.right = Bmp[CreditsNum].Width + 2;
        rcRectdes.bottom = Bmp[CreditsNum].Height + 2;

        BlitToScreen(lpDDSBack);
    }

    // Flippen
    // TODO: look up the directdraw API
//    while (true) {
//        HRESULT ddrval = lpDDSPrimary->Flip(nullptr, 0);

    //        if (ddrval == DD_OK) {
    //            break;
    //        }

    //        if (ddrval == DDERR_SURFACELOST) {
    //            ddrval = lpDDSPrimary->Restore();

    //            if (ddrval != DD_OK) {
    //                break;
    //            }
    //        }

    //        if (ddrval != DDERR_WASSTILLDRAWING) {
    //            break;
    //        }
    //    }
}

void ShowLogo()
{
    rcRectdes.left = 0;
    rcRectdes.top = 0;
    rcRectdes.right = MAX_SCREEN_X;
    rcRectdes.bottom = MAX_SCREEN_Y;

    // TODO: more efficient way of filling with black
    //    lpDDSBack->create(MAX_SCREEN_X, MAX_SCREEN_Y, sf::Color(0, 0, 0));
    Application::clearScreenContent();
    //    s_gameScreenVisible = false;

    rcRectsrc.left = 0;
    rcRectsrc.right = 500;
    rcRectsrc.top = 0;
    rcRectsrc.bottom = 500;
    rcRectdes.left = MAX_SCREEN_X / 2 - 250;
    rcRectdes.right = MAX_SCREEN_X / 2 + 250;
    rcRectdes.top = MAX_SCREEN_Y / 2 - 250;
    rcRectdes.bottom = MAX_SCREEN_Y / 2 + 250;

    BlitToScreen(lpDDSLogo);

    PlaySound(Sound::LOGO, 100);
}

void CreditsBlt(short Bild, short Prozent)
{
    sf::IntRect srcrect;
    srcrect.left = Bmp[Bild].sourceRect.left;
    srcrect.top = Bmp[Bild].sourceRect.top;
    srcrect.width = Bmp[Bild].sourceRect.right - Bmp[Bild].sourceRect.left;
    srcrect.height = Bmp[Bild].sourceRect.bottom - Bmp[Bild].sourceRect.top;

    //    RECT rcRectdes = Bmp[Bild].targetRect;
    //    int dstWidth = rcRectdes.right - rcRectdes.left;
    //    int dstHeight = rcRectdes.bottom - rcRectdes.top;
    //    srcrect.width = std::min(dstWidth, srcrect.width);
    //    srcrect.height = std::min(dstHeight, srcrect.height);

    //    assert(srcrect.width > 0 && srcrect.height > 0);
    //    if (srcrect.width <= 0 || srcrect.height <= 0) {
    //        return;
    //    }
    sf::Sprite sprite;
    sprite.setTexture(*Bmp[Bild].Surface);
    sprite.setPosition(sf::Vector2f(Bmp[Bild].targetRect.left, Bmp[Bild].targetRect.top));
    sprite.setColor(sf::Color(255, 255, 255, 255 * Prozent / 100));
    sprite.setTextureRect(srcrect);
    Application::drawSprite(sprite);
}

sf::Image landscapeImage()
{
    // TODO: get rid of
    return Application::landscapeImage();
}

void drawRect(const int x, const int y, const int width, const int height, const sf::Color& color)
{
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    rect.setFillColor(color);
    Application::drawToScreen(rect);
}
} // namespace Renderer
