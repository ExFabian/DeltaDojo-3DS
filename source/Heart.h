#ifndef HEART_H
#define HEART_H

#include <3ds.h>
#include <citro2d.h>
#include <vector>

class Heart {
private:
    double x = 200.0, y = 120.0;
    int hitboxw = 14, hitboxh = 14;
    int tensionhitboxw = 40, tensionhitboxh = 40;
    int wspeed = 2;
    bool disableslow = true;
    C2D_Sprite sprite[2];
    C2D_Sprite grazespr[4];
    int sprIndex = 0;
    int grazesprIndex = 0;
    int hurtTimer = 0;
    bool grazing = false;
    int grazeTime = 20, grazeTimer = 0;

public:
    Heart(C2D_SpriteSheet &spriteSheet);
    void move(int xd, int yd);
    void draw();
    void hurt(bool hurted = true);
    void setCoords(double x, double y);
    void setGrazing(bool grazing);
    double getX();
    double getY();
    int getHitboxW();
    int getHitboxH();
    int getTensionHitboxW();
    int getTensionHitboxH();
    bool getHurt();
};

#endif
