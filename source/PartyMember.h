#ifndef PARTYMEMBER_H
#define PARTYMEMBER_H

#include <3ds.h>
#include <citro2d.h>
#include <vector>
#include <string>
#include "utils.h"

#define TYPE_KRIS 0
#define TYPE_SUSIE 1
#define TYPE_RALSEI 2

struct Spell {
    std::string name;
    std::string description;
    int cost;
};

struct Item {
    std::string name;
    std::string description;
    int heal;
};

class PartyMember {
private:
    int type; // 0 - kris, 1 - ralsei, 2 - susie
    int maxhp, hp;
    int at, df;
    std::vector<C2D_Sprite> idlespr;
    std::vector<C2D_Sprite> fightrspr;
    std::vector<C2D_Sprite> fightspr;
    std::vector<C2D_Sprite> actrspr;
    std::vector<C2D_Sprite> actspr;
    std::vector<C2D_Sprite> spellrspr;
    std::vector<C2D_Sprite> spellspr;
    std::vector<C2D_Sprite> itemrspr;
    std::vector<C2D_Sprite> itemspr;
    std::vector<C2D_Sprite> defendspr;
    std::vector<C2D_Sprite> atkspr;
    C2D_Sprite hurtspr;
    C2D_Sprite headspr[11];
    C2D_Sprite namespr;
    C2D_Sprite hpspr;
    C2D_Sprite pressspr;
    C2D_Sprite numspr[13];
    C2D_Sprite dmgnumspr[10];
    int sprIndex = 0;
    int sprTimer = 0;
    int atkSprIndex = 0;
    int atkSprTimer = 0;
    int atkHSpeed = 0, atkVSpeed = 0, atkVStartSpeed = 0, atkBounces = 0;
    int atkMoveTime = 3, atkMoveTimer;
    int atkDoneTime = 50, atkDoneTimer;
    int dmgNumTime = 100, dmgNumTimer = 0;
    int hurtTime = 50, hurtTimer = 0;
    int dmgNum = 0;
    int image_speed, idle_image_speed;
    int atk_image_speed = 6;
    int curAnim = -1;
    const int ACT_HOLD_SPR = 7;
    const int ATK_PARTICLE_DELAY = 25;
    double x, y;
    double target_x, target_y, target_ystart;
    int target_df;
    int pos;
    int choice = -1;
    std::vector<Spell> spells;
    double atkbar_pos;
    int points = 0;
    bool doingAction = false, actionDone = false;
    bool dealDamage = false;
    u32 main_color, atk_color, atkdark_color;

public:
    int setHeroSprites(C2D_SpriteSheet &spritesheet, std::vector<C2D_Sprite> &vecspr, int sprNum, int offset, float centerX, float centerY);
    PartyMember(C2D_SpriteSheet &spritesheet, C2D_SpriteSheet &numspritesheet, SpriteParams sprParam, int type, int maxhp, int at, int df, int idle_image_speed, int pos, u32 main_color, u32 atk_color, u32 atkdark_color);
    void hurt(int dmg);
    void doAction(bool pressed);
    void heal(int healp);
    void drawTop();
    void drawBot(int turn);

    void setChoice(int choice, int image_speed = -1, double target_x = -1.0, double target_y = -1.0, int target_df = -1);

    Spell getSpell(int spell);
    std::vector<Spell> getSpells();
    int getType();
    std::string getName();
    bool getDoingAction();
    bool getActionDone();
    bool getDealDamage();
    int getPoints();
    int getTrueAt();
    double getAtkbarPos();
    int getCurAnim();
};

#endif
