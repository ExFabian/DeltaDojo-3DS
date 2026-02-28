#include "BulletController.h"
#include "utils.h"

Bullet::Bullet(C2D_Sprite sprite, int type, int damage, int x, int y, int grazepoints, int timepoints) {
    this->type = type;
    this->damage = damage;
    this->x = x;
    this->y = y;
    this->grazepoints = grazepoints;
    this->timepoints = timepoints;
    this->sprite = sprite;
    C2D_SpriteSetPos(&sprite, x, y);
    switch(type) {
        case 0:
            hitboxw = 6;
            hitboxh = 8;
            break;
    }
}

void Bullet::move() {
    if(!inactive) {
        switch(type) {
            case 0: // rudinn vertical diamond
                vspeed = approach(vspeed, 3.0, 0.05);
                y -= vspeed;
                if(y < -20)
                    inactive = true;
            break;
        }
        C2D_SpriteSetPos(&sprite, x, y);
    }
}

void Bullet::draw() {
    C2D_DrawSprite(&sprite);
}

void Bullet::setGrazed(bool grazed) {
    this->grazed = grazed;
}

bool Bullet::getInactive() {
    return inactive;
}

int Bullet::getHitboxW() {
    return hitboxw;
}

int Bullet::getHitboxH() {
    return hitboxh;
}

int Bullet::getX() {
    return x;
}

int Bullet::getY() {
    return y;
}

int Bullet::getDamage() {
    return damage;
}

bool Bullet::getGrazed() {
    return grazed;
}

int Bullet::getGrazePoints() {
    return grazepoints;
}

int Bullet::getTimePoints() {
    return timepoints;
}

BulletController::BulletController() {
    bulletSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/bulletSprites.t3x");
    for(int i = 0; i < SPRITES_NUM; i++) {
        C2D_SpriteFromSheet(&sprite[i], bulletSpriteSheet, i);
        C2D_SpriteSetCenter(&sprite[i], 0.5f, 0.5f);
        C2D_SpriteSetScale(&sprite[i], 1.005f, 1.005f);
    }
}

void BulletController::doAttack(int type, int damage, int heartX, int heartY) {
    for(it = bullets.begin(); it != bullets.end(); it++) {// move active bullets and delete inactive bullets from memory
        if((*it)->getInactive()) {
            delete (*it);
            bullets.erase(it);
            it--;
        }
        else (*it)->move();
    }

    btimer++;
    switch(type) {
        case 0: // rudinn 1
        if (btimer >= (18 * ratio))
        {
            btimer = 0;
            int radius = 100 + randInt(0, 40);
            int yy = radius * side;
            int xx = -100 + randInt(0, 200);
            int num = randInt(0, 3);

            if (num == 3)
                xx = -10 + randInt(0, 20);

            xx = xx + heartX + 8;
            yy = yy + heartY + 8;

            if(yy > 230)
                yy = 230;

            bullets.push_back(new Bullet(sprite[0], 0, damage, xx, yy, 5, 5));
        }
        break;
    }
}

int BulletController::doBulletCollision(int heartX, int heartY, int heartW, int heartH) { // check collision with heart for every bullet, if collision happens return bullet index or -1 if no collision happens
    for(it = bullets.begin(); it != bullets.end(); it++)
        if(collisionCheck(heartX, heartY, heartW, heartH, (*it)->getX(), (*it)->getY(), (*it)->getHitboxW(), (*it)->getHitboxH()))
            return it - bullets.begin();

    return -1;
}

int BulletController::getBulletDamage(int index) {
    return bullets[index]->getDamage();
}

int BulletController::getBulletGrazePoints(int index) {
    if(bullets[index]->getGrazed())
        return 0;
    bullets[index]->setGrazed(true);
    return bullets[index]->getGrazePoints();
}

int BulletController::getBulletTimePoints(int index) {
    if(bullets[index]->getGrazed())
        return 0;
    return bullets[index]->getTimePoints();
}

void BulletController::deleteBullets() {
    for(it = bullets.begin(); it != bullets.end(); it++) {
        delete (*it);
        bullets.erase(it);
        it--;
    }
}

void BulletController::draw() { // draw every bullet sprite
    for(it = bullets.begin(); it != bullets.end(); it++)
        (*it)->draw();
}
