#include "PartyMember.h"
#include <cmath>

int PartyMember::setHeroSprites(C2D_SpriteSheet &spritesheet, std::vector<C2D_Sprite> &vecspr, int sprNum, int offset, float centerX, float centerY) {
    vecspr.resize(sprNum);
    for(int i = 0; i < sprNum; i++) {
        C2D_SpriteFromSheet(&vecspr[i], spritesheet, i + offset);
        C2D_SpriteSetCenterRaw(&vecspr[i], centerX, centerY);
        C2D_SpriteSetPos(&vecspr[i], x, y);
        C2D_SpriteSetScale(&vecspr[i], 2.0f, 2.0f);
    }
    return sprNum;
}

PartyMember::PartyMember(C2D_SpriteSheet &spritesheet, C2D_SpriteSheet &numspritesheet, SpriteParams sprParam, int type, int maxhp, int at, int df, int idle_image_speed, int pos, u32 main_color, u32 atk_color, u32 atkdark_color) {
    this->type = type;
    this->maxhp = maxhp;
    hp = maxhp;
    this->at = at;
    this->df = df;
    this->idle_image_speed = idle_image_speed;
    image_speed = idle_image_speed;
    this->pos = pos;
    this->main_color = main_color;
    this->atk_color = atk_color;
    this->atkdark_color = atkdark_color;

    x = 30;
    y = 70 * pos - 65;

    switch(type) {
        case TYPE_SUSIE:
            spells.push_back({"Rude Buster", "the buster\nthat rudes", 50});
            spells.push_back({"Placeholder 2", "the holder 2\nthat places", 1});
            spells.push_back({"gdgdgdgfvdf", "fefsrgdfbhgfhgf", 69});
            break;

        case TYPE_RALSEI:
            spells.push_back({"Heal Prayer", "the prayer\nthat heals", 32});
            spells.push_back({"Placeholder", "the holder\nthat places", 1});
            break;
    }

    int i = 0;

    i += setHeroSprites(spritesheet, idlespr, sprParam.idleSprNum, i, sprParam.idleCenterX, sprParam.idleCenterY);
    i += setHeroSprites(spritesheet, fightrspr, sprParam.fightReadySprNum, i, sprParam.fightReadyCenterX, sprParam.fightReadyCenterY);
    i += setHeroSprites(spritesheet, fightspr, sprParam.fightSprNum, i, sprParam.fightCenterX, sprParam.fightCenterY);
    i += setHeroSprites(spritesheet, actrspr, sprParam.actReadySprNum, i, sprParam.actReadyCenterX, sprParam.actReadyCenterY);
    i += setHeroSprites(spritesheet, actspr, sprParam.actSprNum, i, sprParam.actCenterX, sprParam.actCenterY);
    if(sprParam.spellSprNum > 0) {
        i += setHeroSprites(spritesheet, spellrspr, sprParam.spellReadySprNum, i, sprParam.spellReadyCenterX, sprParam.spellReadyCenterY);
        i += setHeroSprites(spritesheet, spellspr, sprParam.spellSprNum, i, sprParam.spellCenterX, sprParam.spellCenterY);
    }
    i += setHeroSprites(spritesheet, itemrspr, sprParam.itemReadySprNum, i, sprParam.itemReadyCenterX, sprParam.itemReadyCenterY);
    i += setHeroSprites(spritesheet, itemspr, sprParam.itemSprNum, i, sprParam.itemCenterX, sprParam.itemCenterY);
    i += setHeroSprites(spritesheet, defendspr, sprParam.defendSprNum, i, sprParam.defendCenterX, sprParam.defendCenterY);

    C2D_SpriteFromSheet(&hurtspr, spritesheet, i);
    C2D_SpriteSetCenterRaw(&hurtspr, sprParam.hurtCenterX, sprParam.hurtCenterY);
    C2D_SpriteSetPos(&hurtspr, x, y);
    C2D_SpriteSetScale(&hurtspr, 2.0f, 2.0f);
    i++;

    atkspr.resize(sprParam.atkSprNum);
    for(int j = 0; j < sprParam.atkSprNum; j++) {
        C2D_SpriteFromSheet(&atkspr[j], spritesheet, i);
        C2D_SpriteSetCenter(&atkspr[j], 0.5f, 0.5f);
        C2D_SpriteSetPos(&atkspr[j], 0, 0);
        C2D_SpriteSetScale(&atkspr[j], 2.0f, 2.0f);
        i++;
    }

    for(size_t j = 0; j < 11; j++) {
        C2D_SpriteFromSheet(&headspr[j], spritesheet, i);
        C2D_SpriteSetCenter(&headspr[j], 0.0f, 0.5f);
        C2D_SpriteSetPos(&headspr[j], 120, 187 - 38 * (pos - 1));
        C2D_SpriteSetScale(&headspr[j], 1.005f, 1.005f);
        i++;
    }

    C2D_SpriteFromSheet(&namespr, spritesheet, i);
    C2D_SpriteSetCenter(&namespr, 0.0f, 0.5f);
    C2D_SpriteSetPos(&namespr, 156, 185 - 38 * (pos - 1));
    C2D_SpriteSetScale(&namespr, 1.005f, 1.005f);

    C2D_SpriteFromSheet(&hpspr, spritesheet, i + 1);
    C2D_SpriteSetCenter(&hpspr, 0.0f, 0.5f);
    C2D_SpriteSetPos(&hpspr, 225, 195 - 38 * (pos - 1));
    C2D_SpriteSetScale(&hpspr, 1.005f, 1.005f);

    C2D_SpriteFromSheet(&pressspr, spritesheet, i + 2);
    C2D_SpriteSetCenter(&pressspr, 0.0f, 0.5f);
    C2D_SpriteSetPos(&pressspr, 115, 184 - 38 * (pos - 1));
    C2D_SpriteSetScale(&pressspr, 1.005f, 1.005f);

    i = 0;
    for(size_t j = 0; j < 13; j++) {
        C2D_SpriteFromSheet(&numspr[j], numspritesheet, i);
        C2D_SpriteSetCenter(&numspr[j], 1.0f, 0.5f);
        C2D_SpriteSetScale(&numspr[j], 1.005f, 1.005f);
        i++;
    }

    for(size_t j = 0; j < 10; j++) {
        C2D_SpriteFromSheet(&dmgnumspr[j], numspritesheet, i);
        C2D_SpriteSetCenter(&dmgnumspr[j], 1.0f, 0.5f);
        C2D_SpriteSetScale(&dmgnumspr[j], 1.005f, 1.005f);
        i++;
    }
}

void PartyMember::hurt(int dmg) {
    int tdmg = dmg - (df * 3);
    if(tdmg < 1)
        tdmg = 1;
    hp -= tdmg;

    dmgNum = tdmg;
    hurtTimer = hurtTime;
    atkMoveTimer = 0;
    dmgNumTimer = dmgNumTime;
    atkHSpeed = 10;
    atkVSpeed = -5 - randInt(0, 2);
    atkVStartSpeed = atkVSpeed;
    atkBounces = 0;
    atkMoveTimer = 0;
    atkDoneTimer = 0;

    target_x = x + 30;
    target_y = y + 50;
}

void PartyMember::doAction(bool pressed) {
    if(actionDone) return;

    switch(choice) {
        case 0: // attack
            if(!doingAction)
                atkbar_pos -= 2.2;
            if(!doingAction && (pressed || atkbar_pos < -40.0)) {
                doingAction = true;
                if(atkbar_pos < -40.0) points = 0;
                else if(atkbar_pos > -5.0 && atkbar_pos < 15.0) points = 150;
                else if(atkbar_pos > -10.0 && atkbar_pos < 20.0) points = 120;
                else if(atkbar_pos > -15.0 && atkbar_pos < 25.0) points = 110;
                else {
                    points = 110 - abs(floor(atkbar_pos));
                    if(points < 0) points = 0;
                }
            }
            break;

        case 1: // act/spell
            if(!doingAction)
                doingAction = true;
            if(pressed) {
                if(type == TYPE_KRIS && sprIndex == ACT_HOLD_SPR)
                    sprIndex++;
                actionDone = true;
            }
            break;

        case 2: // item
        case 3: // mercy
            if(!doingAction)
                doingAction = true;
            if(pressed)
                actionDone = true;
            break;

        case 4: // defend
            if(!doingAction)
                doingAction = true;
            if(!actionDone)
                actionDone = true;
            break;
    }
}

void PartyMember::heal(int healp) {
    hp += healp;
    if(hp > maxhp)
        hp = maxhp;
}

void PartyMember::drawTop() {
    if(image_speed > 0) {
        sprTimer++;
        if(sprTimer >= image_speed) {
            sprTimer = 0;
            if(type != TYPE_KRIS || curAnim != 1 || sprIndex != ACT_HOLD_SPR)
                sprIndex++;
        }
    }
    if(hurtTimer > 0) {
        int hurtx;
        if(hurtTime - hurtTimer <= 2)
            hurtx = x - 10 + (hurtTime - hurtTimer) * 5;
        else
            hurtx = x;

        if(curAnim != 4) {
            C2D_SpriteSetPos(&hurtspr, hurtx, y);
            C2D_DrawSprite(&hurtspr);
        }
        else {
            C2D_SpriteSetPos(&defendspr[sprIndex], hurtx, y);
            C2D_DrawSprite(&defendspr[sprIndex]);
        }

        hurtTimer--;
    }
    else
    switch(curAnim) {
        case -1: // idle
        default:
            if((unsigned int)sprIndex >= idlespr.size()) sprIndex = 0;
            C2D_DrawSprite(&idlespr[sprIndex]);
        break;

        case 0: // fight
            if(actionDone || doingAction) {
                if((unsigned int)sprIndex >= fightspr.size()) {
                    sprIndex--;
                    image_speed = 0;
                    // actionDone is set when the attack particle is done
                }
                C2D_DrawSprite(&fightspr[sprIndex]);
            }
            else {
                if((unsigned int)sprIndex >= fightrspr.size()) sprIndex = 0;
                C2D_DrawSprite(&fightrspr[sprIndex]);
            }
            break;

        case 1: // spell / act
            if(type != TYPE_KRIS) { // will have to replace this at some point
                if(doingAction) {
                    if((unsigned int)sprIndex >= spellspr.size()) {
                        sprIndex = 0;
                        curAnim = -1;
                        C2D_DrawSprite(&idlespr[sprIndex]);
                    }
                    else
                        C2D_DrawSprite(&spellspr[sprIndex]);
                }
                else {
                    if((unsigned int)sprIndex >= spellrspr.size()) sprIndex = 0;
                    C2D_DrawSprite(&spellrspr[sprIndex]);
                }
            }
            else {
                if(doingAction) {
                    if((unsigned int)sprIndex >= actspr.size()) {
                        sprIndex = 0;
                        curAnim = -1;
                        C2D_DrawSprite(&idlespr[sprIndex]);
                    }
                    else
                        C2D_DrawSprite(&actspr[sprIndex]);
                }
                else {
                    if((unsigned int)sprIndex >= actrspr.size()) sprIndex = 0;
                    C2D_DrawSprite(&actrspr[sprIndex]);
                }
            }
            break;

        case 2: // item
            if(doingAction) {
                if((unsigned int)sprIndex >= itemspr.size()) {
                    sprIndex = 0;
                    curAnim = -1;
                    C2D_DrawSprite(&idlespr[sprIndex]);
                }
                else
                    C2D_DrawSprite(&itemspr[sprIndex]);
            }
            else {
                if((unsigned int)sprIndex >= itemrspr.size()) sprIndex = 0;
                C2D_DrawSprite(&itemrspr[sprIndex]);
            }
            break;

        case 3: // spare
            if(type != TYPE_KRIS) {
                if(doingAction) {
                    if((unsigned int)sprIndex >= spellspr.size()) {
                        sprIndex = 0;
                        curAnim = -1;
                        C2D_DrawSprite(&idlespr[sprIndex]);
                    }
                    else
                        C2D_DrawSprite(&spellspr[sprIndex]);
                }
                else {
                    if((unsigned int)sprIndex >= idlespr.size()) sprIndex = 0;
                    C2D_DrawSprite(&idlespr[sprIndex]);
                }

            }
            else {
                if(doingAction) {
                    if((unsigned int)sprIndex >= actspr.size()) {
                        sprIndex = 0;
                        curAnim = -1;
                        C2D_DrawSprite(&idlespr[sprIndex]);
                    }
                    else
                        C2D_DrawSprite(&actspr[sprIndex]);
                }
                else {
                    if((unsigned int)sprIndex >= idlespr.size()) sprIndex = 0;
                    C2D_DrawSprite(&idlespr[sprIndex]);
                }
            }
            break;

        case 4: // defend
            if(actionDone || doingAction) {
                if((unsigned int)sprIndex >= defendspr.size()) {
                    sprIndex--;
                    image_speed = 0;
                }
                C2D_DrawSprite(&defendspr[sprIndex]);
            }
            break;
    }

    if(choice == 0 && (doingAction || actionDone) && atkSprIndex < (int)atkspr.size()) { // the attack particle that appears on enemy when you attack them
        if(dealDamage) dealDamage = false;
        atkSprTimer++;
        if(atkSprTimer >= atk_image_speed) {
            atkSprTimer = 0;
            atkSprIndex++;
            if(atkSprIndex == 0) dealDamage = true;
        }
        if(atkSprIndex > -1 && atkSprIndex < (int)atkspr.size())
            C2D_DrawSprite(&atkspr[atkSprIndex]);
    }

    if(choice == 0 && (doingAction || actionDone) && atkSprIndex > -1) { // enemy damage numbers and also counting for actionDone
        atkDoneTimer++;
        if(atkDoneTimer >= atkDoneTime) {
            actionDone = true;
            doingAction = false;
        }

        int trueAt = (at * points) / 20 - (target_df * 3);
        if(trueAt < 0) trueAt = 0;
        int npos = 0;
        while(trueAt > 0) {
            int d = trueAt % 10;

            atkMoveTimer++;
            if(atkMoveTimer >= atkMoveTime) {
                atkMoveTimer = 0;
                if (atkHSpeed > 0)
                    atkHSpeed -= 1;
                if (atkHSpeed < 0)
                    atkHSpeed += 1;
                if (abs(atkHSpeed) < 1)
                    atkHSpeed = 0;

                if (atkBounces < 2)
                    atkVSpeed += 1;
                if (target_y > target_ystart && atkBounces < 2) {
                    target_y = target_ystart;
                    atkVSpeed = atkVStartSpeed / 2;
                    atkBounces += 1;
                }
                if (atkBounces >= 2) {
                    atkVSpeed = 0;
                    target_y = target_ystart;
                }

                target_x += atkHSpeed;
                target_y += atkVSpeed;
            }

            C2D_SpriteSetPos(&dmgnumspr[d], target_x - npos * 21, target_y - 22 * (pos - 1));

            C2D_ImageTint tint;
            for(size_t i = 0; i < 4; i++)
                tint.corners[i] = {main_color, 0.3};
            C2D_DrawSpriteTinted(&dmgnumspr[d], &tint);

            trueAt /= 10;
            npos++;
        }
    }

    if(dmgNumTimer > 0) { // party damage numbers
        int nDmgNum = dmgNum;
        int truey = y + 50;
        int npos = 0;
        while(nDmgNum > 0) {
            int d = nDmgNum % 10;

            atkMoveTimer++;
            if(atkMoveTimer >= atkMoveTime) {
                atkMoveTimer = 0;
                if (atkHSpeed > 0)
                    atkHSpeed -= 1;
                if (atkHSpeed < 0)
                    atkHSpeed += 1;
                if (abs(atkHSpeed) < 1)
                    atkHSpeed = 0;

                if (atkBounces < 2)
                    atkVSpeed += 1;
                if (target_y > truey && atkBounces < 2) {
                    target_y = truey;
                    atkVSpeed = atkVStartSpeed / 2;
                    atkBounces += 1;
                }
                if (atkBounces >= 2) {
                    atkVSpeed = 0;
                    target_y = truey;
                }

                target_x += atkHSpeed;
                target_y += atkVSpeed;
            }

            C2D_SpriteSetPos(&dmgnumspr[d], target_x - npos * 21, target_y - 22 * (pos - 1));
            C2D_DrawSprite(&dmgnumspr[d]);

            nDmgNum /= 10;
            npos++;
        }

        dmgNumTimer--;
    }
}

void PartyMember::drawBot(int turn) {
    switch(choice) { // head icons
        case -1:
            C2D_DrawSprite(&headspr[0]);
            break;

        case 0:
            if(!doingAction) C2D_DrawSprite(&headspr[1]);
            else C2D_DrawSprite(&headspr[0]);
            break;

        case 1:
            if(type == TYPE_KRIS) C2D_DrawSprite(&headspr[6]);
            else C2D_DrawSprite(&headspr[2]);
            break;

        case 2:
            if(type == TYPE_KRIS) C2D_DrawSprite(&headspr[2]);
            else C2D_DrawSprite(&headspr[3]);
            break;

        case 3:
            C2D_DrawSprite(&headspr[10]);
            break;

        case 4:
            C2D_DrawSprite(&headspr[4]);
            break;
    }

    if(choice == 0 && turn >= 4) { // attacking
        int atkbox_posX = 195, atkbox_posY = 168;
        int atkbox_width = 110, atkbox_height = 36;
        int atksweet_width = 10;
        int atkbar_width = 8;
        int atkbar_offset = 220;
        int atkbox_spacebetween = 2;
        C2D_DrawSprite(&pressspr);
        C2D_DrawRectHollow(atkbox_posX, atkbox_posY - (atkbox_height + atkbox_spacebetween) * (pos - 1), atkbox_posX + atkbox_width, (atkbox_posY + atkbox_height) - (atkbox_height + atkbox_spacebetween) * (pos - 1), 0, atkdark_color, 1);
        C2D_DrawRectHollow(atkbox_posX, atkbox_posY - (atkbox_height + atkbox_spacebetween) * (pos - 1), atkbox_posX + atksweet_width, (atkbox_posY + atkbox_height) - (atkbox_height + atkbox_spacebetween) * (pos - 1), 0, atk_color, 2);
        if(!doingAction && !actionDone)
            C2D_DrawRectSolid(atkbar_offset + atkbar_pos, (atkbox_posY - 1) - (atkbox_height + atkbox_spacebetween) * (pos - 1), 0, atkbar_width, atkbox_height + 2, C_WHITE);
        return;
    }

    C2D_DrawSprite(&namespr);
    C2D_DrawSprite(&hpspr);
    drawHPBar(hp, maxhp, 243.0, 188.0 - 38 * (pos - 1), 0, 62, 10, main_color, C_DARKRED);

    //hp numbers
    int hp1 = maxhp;
    int npos = 0;
    while(hp1 > 0) {
        int d = hp1 % 10;
        C2D_SpriteSetPos(&numspr[d], 305 - npos * 8, 180 - 38 * (pos - 1));
        C2D_DrawSprite(&numspr[d]);
        hp1 /= 10;
        npos++;
    }
    C2D_SpriteSetPos(&numspr[12], 305 - npos * 8, 180 - 38 * (pos - 1));
    C2D_DrawSprite(&numspr[12]);
    hp1 = hp;
    if(hp1 < 0)
        hp1 *= -1;
    npos++;
    while(hp1 > 0) {
        int d = hp1 % 10;
        C2D_SpriteSetPos(&numspr[d], 298 - npos * 8, 180 - 38 * (pos - 1));
        C2D_DrawSprite(&numspr[d]);
        hp1 /= 10;
        npos++;
    }
    if(hp < 0) {
        C2D_SpriteSetPos(&numspr[10], 298 - npos * 8, 180 - 38 * (pos - 1));
        C2D_DrawSprite(&numspr[10]);
    }

    if(turn == pos)
        C2D_DrawRectHollow(115, 165 - 38 * (pos - 1), 310, 165 - 38 * (pos - 2), 0, main_color, 2);
}

void PartyMember::setChoice(int choice, int image_speed, double target_x, double target_y, int target_df) {
    this->choice = choice;
    curAnim = choice;
    sprIndex = 0;
    if(choice == -1) {
        doingAction = false;
        actionDone = false;
        dealDamage = false;
    }
    if(choice == 0) {
        atkbar_pos = 100.0 + 40.0 * randInt(0, 2);
        points = 0;
        atkSprIndex = -1;
        atkSprTimer = -1 * ATK_PARTICLE_DELAY;
        atkHSpeed = 10;
        atkVSpeed = -5 - randInt(0, 2);
        atkVStartSpeed = atkVSpeed;
        atkBounces = 0;
        atkMoveTimer = 0;
        atkDoneTimer = 0;

        this->target_x = target_x;
        this->target_y = target_y;
        target_ystart = target_y;
        this->target_df = target_df;
        for(size_t i = 0; i < atkspr.size(); i++)
            C2D_SpriteSetPos(&atkspr[i], target_x, target_y);
    }
    if(choice == 4) {
        doingAction = true;
        actionDone = true;
    }

    if(image_speed > -1)
        this->image_speed = image_speed;
    else
        this->image_speed = idle_image_speed;
}

Spell PartyMember::getSpell(int spell) {
    return spells[spell];
}

std::vector<Spell> PartyMember::getSpells() {
    return spells;
}

int PartyMember::getType() {
    return type;
}

std::string PartyMember::getName() {
    switch(type) {
        case TYPE_KRIS:
            return "Kris";
        case TYPE_SUSIE:
            return "Susie";
        case TYPE_RALSEI:
            return "Ralsei";
    }
    return "Wing Gaster";
}

bool PartyMember::getDoingAction() {
    return doingAction;
}

bool PartyMember::getActionDone() {
    return actionDone;
}

bool PartyMember::getDealDamage() {
    return dealDamage;
}

int PartyMember::getPoints() {
    return points;
}

int PartyMember::getTrueAt() {
    return (at * points) / 20;
}

double PartyMember::getAtkbarPos() {
    return atkbar_pos;
}

int PartyMember::getCurAnim() {
    return curAnim;
}
