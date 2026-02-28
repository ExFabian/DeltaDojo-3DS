#include "Heart.h"
#include <math.h>

Heart::Heart(C2D_SpriteSheet &spriteSheet) {
    for(int i = 0; i <= 1; i++) {
        C2D_SpriteFromSheet(&sprite[i], spriteSheet, i);
        C2D_SpriteSetCenter(&sprite[i], 0.5f, 0.5f);
    }

    for(int i = 0; i < 4; i++) {
        C2D_SpriteFromSheet(&grazespr[i], spriteSheet, i + 17);
        C2D_SpriteSetCenter(&grazespr[i], 0.5f, 0.5f);
    }
}

void Heart::move(int xd, int yd) {
    if(hurtTimer > 0) {
        hurtTimer--;
        if(hurtTimer % 8 == 0)
            sprIndex = !sprIndex;
        if(hurtTimer == 0)
            sprIndex = 0;
    }

    if(grazeTimer > 0) {
        grazeTimer--;
        if(grazeTimer == 0)
            grazing = false;
    }

    int px = xd * wspeed;
    int py = yd * wspeed;

    // todo: slowdown button

    x += px;
    y += py;

    // yummy hardcoding... will have to replace with battlesolid at some point but this will do for now
    if(x < 141)
        x = 141;
    if(x > 259)
        x = 259;
    if(y < 61)
        y = 61;
    if(y > 179)
        y = 179;
    C2D_SpriteSetPos(&sprite[sprIndex], x, y);
    if(grazing)
        C2D_SpriteSetPos(&grazespr[grazesprIndex], x, y);
}

void Heart::draw() {
    C2D_DrawSprite(&sprite[sprIndex]);
    if(grazing)
        C2D_DrawSprite(&grazespr[grazesprIndex]);
    else
        grazeTimer = 0;
}

void Heart::hurt(bool hurted) {
    if(hurted) {
        hurtTimer = 40;
        sprIndex = 1;
    }
    else {
        hurtTimer = 0;
        sprIndex = 0;
    }
}

void Heart::setCoords(double x, double y) {
    this->x = x;
    this->y = y;
    C2D_SpriteSetPos(&sprite[sprIndex], x, y);
}

void Heart::setGrazing(bool grazing) {
    this->grazing = grazing;
    if(grazing)
        grazeTimer = grazeTime;
}

double Heart::getX() {
    return x;
}

double Heart::getY() {
    return y;
}

int Heart::getHitboxW() {
    return hitboxw;
}

int Heart::getHitboxH() {
    return hitboxh;
}

int Heart::getTensionHitboxW() {
    return tensionhitboxw;
}

int Heart::getTensionHitboxH() {
    return tensionhitboxh;
}

bool Heart::getHurt() {
    return hurtTimer > 0;
}
