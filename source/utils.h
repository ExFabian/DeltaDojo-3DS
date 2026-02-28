#ifndef UTILS_H
#define UTILS_H

#include <3ds.h>
#include <citro2d.h>
#include <random>

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240
#define BOTTOM_SCREEN_WIDTH  320
#define BOTTOM_SCREEN_HEIGHT 240

double approach(double a, double b, double index);
bool collisionCheck(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2);
void C2D_DrawRectHollow(float x1, float y1, float x2, float y2, float z, u32 c, float thickness);
void drawHPBar(int hp, int maxhp, float x, float y, float z, float w, float h, u32 c1, u32 c2);
int randInt(int min, int max);
void clearCString(char str[]);

struct SpriteParams {
    int idleSprNum, fightReadySprNum, fightSprNum, actReadySprNum, actSprNum, spellReadySprNum, spellSprNum, itemReadySprNum, itemSprNum, defendSprNum, atkSprNum;
    float idleCenterX, idleCenterY, fightReadyCenterX, fightReadyCenterY, fightCenterX, fightCenterY, actReadyCenterX, actReadyCenterY, actCenterX, actCenterY, spellReadyCenterX, spellReadyCenterY, spellCenterX, spellCenterY, itemReadyCenterX, itemReadyCenterY, itemCenterX, itemCenterY, defendCenterX, defendCenterY, hurtCenterX, hurtCenterY;
};

// colors
const uint32_t C_DARKRED = C2D_Color32(0x80, 0x00, 0x00, 0xFF);
const uint32_t C_ORANGE = C2D_Color32(0xFF, 0xA0, 0x40, 0xFF);
const uint32_t C_BLACK = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
const uint32_t C_WHITE = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
const uint32_t C_GRAY = C2D_Color32(0x80, 0x80, 0x80, 0xFF);
const uint32_t C_BLUE = C2D_Color32(0x00, 0xFF, 0xFF, 0xFF);
const uint32_t C_GREEN = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
const uint32_t C_PINK = C2D_Color32(0xFF, 0x00, 0xFF, 0xFF);
const uint32_t C_KRISBLUE = C2D_Color32(0x00, 0xA2, 0xE8, 0xFF);
const uint32_t C_KRISDARKBLUE = C2D_Color32(0x03, 0x02, 0xEF, 0xFF);
const uint32_t C_SUSIEPINK = C2D_Color32(0xEA, 0x79, 0xC8, 0xFF);
const uint32_t C_SUSIEDARKPINK = C2D_Color32(0x85, 0x10, 0x86, 0xFF);
const uint32_t C_RALSEIGREEN = C2D_Color32(0xB5, 0xE6, 0x1D, 0xFF);
const uint32_t C_RALSEIDARKGREEN = C2D_Color32(0x0A, 0x61, 0x12, 0xFF);

#endif
