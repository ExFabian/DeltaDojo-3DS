#include "utils.h"

//used for random number generation
std::random_device rd;

double approach(double a, double b, double index) {
    if(a > b) {
        a -= index;
        if(a < b)
            a = b;
    }
    else if(a < b) {
        a += index;
        if(a > b)
            a = b;
    }
    return a;
}

bool collisionCheck(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2) { // im not making ts public
    bool xb = ((x1 - w1/2 < x2 - w2/2 && x2 - w2/2 < x1 + w1/2 && x1 + w1/2 < x2 + w2/2) || (x1 - w1/2 < x2 - w2/2 && x2 + w2/2 < x1 + w1/2) || (x2 - w2/2 < x1 - w1/2 && x1 - w1/2 < x2 + w2/2 && x2 + w2/2 < x1 + w1/2));
    bool yb = ((y1 - h1/2 < y2 - h2/2 && y2 - h2/2 < y1 + h1/2 && y1 + h1/2 < y2 + h2/2) || (y1 - h1/2 < y2 - h2/2 && y2 + h2/2 < y1 + h1/2) || (y2 - h2/2 < y1 - h1/2 && y1 - h1/2 < y2 + h2/2 && y2 + h2/2 < y1 + h1/2));

    return xb && yb;
}

void C2D_DrawRectHollow(float x1, float y1, float x2, float y2, float z, u32 c, float thickness) {
    C2D_DrawLine(x1, y1, c, x2 + thickness / 2, y1, c, thickness, z);
    C2D_DrawLine(x2, y1, c, x2, y2 + thickness / 2, c, thickness, z);
    C2D_DrawLine(x2, y2, c, x1 - thickness / 2, y2, c, thickness, z);
    C2D_DrawLine(x1, y2, c, x1, y1 - thickness / 2, c, thickness, z);
}

void drawHPBar(int hp, int maxhp, float x, float y, float z, float w, float h, u32 c1, u32 c2) {
    float hpRatio = ((float)hp)/maxhp;
    if(hp > 0) {
        C2D_DrawRectSolid(x, y, z, hpRatio * w, h, c1);
        C2D_DrawRectSolid(x + hpRatio * w, y, z, w - hpRatio * w, h, c2);
    }
    else
        C2D_DrawRectSolid(x, y, z, w, h, c2);
}

int randInt(int min, int max) {
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

void clearCString(char str[]) {
    for(size_t i = 0; str[i] != 0; i++)
        str[i] = 0;
}
