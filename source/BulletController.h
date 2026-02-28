#ifndef BULLETCONTROLLER_H
#define BULLETCONTROLLER_H

#include <vector>
#include <3ds.h>
#include <citro2d.h>

#define SPRITES_NUM 2

class Bullet {
private:
    C2D_Sprite sprite;
    int type;
    double x, y;
    int hitboxw, hitboxh;
    int damage;
    int grazepoints, timepoints;
    bool grazed = false;
    double vspeed = 0.0;
    bool inactive = false;
public:
    Bullet(C2D_Sprite sprite, int type, int damage, int x, int y, int grazepoints, int timepoints);
    void move();
    void draw();

    void setGrazed(bool);

    bool getInactive();
    int getHitboxW();
    int getHitboxH();
    int getX();
    int getY();
    int getDamage();
    bool getGrazed();
    int getGrazePoints();
    int getTimePoints();
};

class BulletController {
private:
    C2D_SpriteSheet bulletSpriteSheet;
    C2D_Sprite sprite[SPRITES_NUM];
    std::vector<Bullet*> bullets;
    std::vector<Bullet*>::iterator it;
    int btimer = 0;
    float ratio = 1.0;
    int side = 1;

public:
    BulletController();
    void doAttack(int type, int damage, int heartX, int heartY);
    int doBulletCollision(int heartX, int heartY, int heartW, int heartH);

    int getBulletDamage(int index);
    int getBulletGrazePoints(int index);
    int getBulletTimePoints(int index);

    void deleteBullets();
    void draw();
};

#endif
