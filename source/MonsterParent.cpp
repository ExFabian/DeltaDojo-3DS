#include "MonsterParent.h"

MonsterParent::MonsterParent(int maxhp, int hp, int at, int df, int sparepoint, int mercymod, int mercymax) {
    this->maxhp = maxhp;
    this->hp = hp;
    this->at = at;
    this->df = df;
    this->sparepoint = sparepoint;
    this->mercymod = mercymod;
    this->mercymax = mercymax;
}

void MonsterParent::setTurnNum(int bturn) {
    this->bturn = bturn;
}

bool MonsterParent::isSpareable() {
    return mercymod >= mercymax;
}

int MonsterParent::getHP() {
    return hp;
}

int MonsterParent::getMaxHP() {
    return maxhp;
}

int MonsterParent::getDF() {
    return df;
}

double MonsterParent::getX() {
    return x;
}

double MonsterParent::getY() {
    return y;
}

// TestEnemy
void TestEnemy::setSprites(C2D_SpriteSheet &spriteSheet) {
    int i = 0;
    for(; i < idlesprNum; i++) {
        C2D_SpriteFromSheet(&idlespr[i], spriteSheet, i);
        C2D_SpriteSetCenter(&idlespr[i], 0.5f, 0.5f);
        C2D_SpriteSetPos(&idlespr[i], x, y);
        C2D_SpriteSetScale(&idlespr[i], 2.0f, 2.0f);
    }

    C2D_SpriteFromSheet(&hurtspr, spriteSheet, i);
    C2D_SpriteSetCenter(&hurtspr, 0.5f, 0.5f);
    C2D_SpriteSetPos(&hurtspr, x, y);
    C2D_SpriteSetScale(&hurtspr, 2.0f, 2.0f);
}

void TestEnemy::setBattleText() {
    battleText.push_back("* Wow, this is cool!\ntest 2 XD :) -:()+\nHello, I am da test enemee"); // idle turn 1
    battleText.push_back("* TestEnemy starts rambling\nabout some tall skeleton\nbeing a knight."); // idle turn 2+
}

void TestEnemy::setActs() {
    acts.push_back({"Check", "Test Enemy - ?AT ?DF\nHe legally changed his\nname to that, for some\nreason.", {1, 0, 0, 0} });
    acts.push_back({"Do something", "Kris did something!", {1, 0, 0, 0} });
}

std::string TestEnemy::getBattleText() {
    if(bturn <= 1)
        return battleText[0];
    else
        return battleText[1];
}

std::string TestEnemy::doAct(int actNum) {
    switch(actNum) {
        case 0: // check
            return acts[0].text;
            break;

        case 1: // do something
            return acts[1].text;
            break;
    }

    return "This text shouldn't appear!\nIf it does, I'm an idiot...";
}

void TestEnemy::hurt(int dmg) {
    int tdmg = dmg - (df * 3);
    if(tdmg < 1)
        tdmg = 1;
    hp -= tdmg;

    hurtTimer = hurtTime;
    hurtShake = 0;
    shake_x = 9;
}

void TestEnemy::move() {
    if(hurtTimer <= 0) {
        sprtimer++;
        if(sprtimer > image_speed) {
            sprtimer = 0;
            sprIndex++;
            if(sprIndex >= idlesprNum)
                sprIndex = 0;
        }
        C2D_DrawSprite(&idlespr[sprIndex]);
    }
    else {
        hurtShake += 1;
        if (hurtShake > 1)
        {
            if (shake_x > 0)
                shake_x -= 1;

            if (shake_x < 0)
                shake_x += 1;

            shake_x = -shake_x;
            hurtShake = 0;
        }

        hurtTimer--;
        C2D_SpriteSetPos(&hurtspr, x + shake_x, y);
        C2D_DrawSprite(&hurtspr);
    }
}

std::string TestEnemy::getName() {
    return name;
}

std::vector<std::string> TestEnemy::getActs() {
    std::vector<std::string> actTexts;
    for(size_t i = 0; i < acts.size(); i++)
        actTexts.push_back(acts[i].name);

    return actTexts;
}

C2D_Sprite* TestEnemy::getSpr() {
    return &(idlespr[sprIndex]);
}
