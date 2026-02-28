#ifndef MONSTERPARENT_H
#define MONSTERPARENT_H

#include <string>
#include <vector>

#include <3ds.h>
#include <citro2d.h>

struct Act {
    std::string name;
    std::string text;
    bool partyNeeded[4];
};

class MonsterParent {
protected:
    int maxhp;
    int hp;
    int at;
    int df;
    int sparepoint;
    int mercymod;
    int mercymax = 100;
    int bturn = 1;
    double x = 330.0, y = 120.0;
    int hurtTime = 50;
    int hurtTimer = 0;
    int hurtShake = 0;
    int shake_x = 0;
public:
    MonsterParent(int maxhp, int hp, int at, int df, int sparepoint, int mercymod, int mercymax);
    void setTurnNum(int bturn);
    bool isSpareable();
    int getHP();
    int getMaxHP();
    int getDF();
    double getX();
    double getY();
};

class TestEnemy : public MonsterParent {
private:
    std::string name = "Test Enemy";
    std::vector<std::string> battleText;
    std::vector<Act> acts;
    int idlesprNum = 4;
    C2D_Sprite idlespr[4];
    C2D_Sprite hurtspr;
    int sprIndex = 0;
    int sprtimer = 0;
    int image_speed = 12;
public:
    using MonsterParent::MonsterParent;
    void setSprites(C2D_SpriteSheet &spriteSheet);
    void setBattleText();
    void setActs();
    std::string getBattleText();
    std::string doAct(int actNum);
    void hurt(int dmg);
    void move();
    std::string getName();
    std::vector<std::string> getActs();
    C2D_Sprite* getSpr();
};

#endif
