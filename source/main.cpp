#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <algorithm> // for std::count and std::find

#include <3ds.h> //main 3ds library
#include <citro2d.h> //graphics library

#include "Heart.h"
#include "PartyMember.h"
#include "MonsterParent.h"
#include "BulletController.h"
#include "utils.h"

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

#define MAX_SPRITES 64

Item items[] = {
{"Unused", "wing gaster\ndess 1225 asriel\nspamton knight\npapyrus 666", -8364724},
{"Dark Candy", "Heals\n40HP", 40},
{"test1", "Heal\nDowned\nAlly", 2},
{"test2", "Heal\nDowned\nAlly", 2},
{"test3", "Heal\nDowned\nAlly", 2},
{"test4", "Heal\nDowned\nAlly", 2},
{"test5", "Heal\nDowned\nAlly", 2}
};

C2D_TextBuf fontbuf;
C2D_Text text_binfo;
C2D_Text text_bdesc;
C2D_Text text_tp;
C2D_Font font_main; // credit the guy that I got the font from: https://www.dafont.com/big-shot.font

static C2D_SpriteSheet mainSpriteSheet;
static C2D_SpriteSheet enemySpriteSheet;
static C2D_SpriteSheet krisSpriteSheet;
static C2D_SpriteSheet ralseiSpriteSheet;
static C2D_SpriteSheet susieSpriteSheet;
static C2D_SpriteSheet numFontSpriteSheet;
static C2D_Sprite battlebg_spr[2];
static C2D_Sprite battlebt_spr[12];
static C2D_Sprite tplogo_spr;

BulletController* bulletController;
Heart* heart;
std::vector<PartyMember*> party;
std::vector<PartyMember*>::iterator p_it;

TestEnemy* dummy;

int menuOptionSel = 0;
int turn = 1;
int numTurns = 1;
bool mybattle = true; // false - enemy turn, true - player turn
bool changeBattle = false;
bool inMenu = false;
bool inMenu2 = false;
int tension = 0;
int defendTension = 16;
int tensionMax = 100;
int maxMenuRows = 4;

void load_libs() {
	romfsInit();
	cfguInit(); // Allow C2D_FontLoadSystem to work
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
}

int load_spritesheets() {
	mainSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/menuSprites.t3x");
	enemySpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/enemySprites.t3x");
	krisSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/krisSprites.t3x");
	ralseiSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/ralseiSprites.t3x");
	susieSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/susieSprites.t3x");
	numFontSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/numbersfontSprites.t3x");
	if (!mainSpriteSheet || !enemySpriteSheet || !krisSpriteSheet || !ralseiSpriteSheet || !susieSpriteSheet || !numFontSpriteSheet) return 1;

	return 0;
}

void exit_game() {
	delete bulletController;
	delete heart;
	delete dummy;
	for(p_it = party.begin(); p_it != party.end(); p_it++) // todo: also clean up graphics, this might lead to a memory leak (?)
		delete (*p_it);

	// Delete graphics
	C2D_SpriteSheetFree(mainSpriteSheet);
	C2D_SpriteSheetFree(enemySpriteSheet);
	C2D_SpriteSheetFree(krisSpriteSheet);
	C2D_SpriteSheetFree(ralseiSpriteSheet);
	C2D_SpriteSheetFree(susieSpriteSheet);
	C2D_SpriteSheetFree(numFontSpriteSheet);

	C2D_TextBufDelete(fontbuf);
	C2D_FontFree(font_main);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
}

void tension_heal(int heal) {
	tension += heal;
	if(tension > tensionMax)
		tension = tensionMax;
}

int main(int argc, char* argv[]) {
	// todo: do checks for unintended behaviour, its good practice
	load_libs();

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTargetSetOutput(top, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
	C3D_RenderTarget* bot = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(bot, GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	fontbuf = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer
	font_main = C2D_FontLoad("romfs:/big-shot.bcfnt");

	// Load graphics
	if(load_spritesheets() != 0) svcBreak(USERBREAK_PANIC);

	bulletController = new BulletController();
	heart = new Heart(mainSpriteSheet);
	heart->setCoords(TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2);
	SpriteParams krisParam = {6, 1, 7, 2, 11, 0, 0, 1, 7, 6, 5,
	3, 0, 3, 5, 3, 5, 4, 5, 4, 5, 0, 0, 0, 0, 6, 4, 6, 4, 3, 2, 3, 5};
	SpriteParams ralseiParam = {5, 1, 6, 1, 11, 4, 10, 1, 8, 8, 8,
	4, 0, 8, 0, 8, 0, 0, 0, 0, 0, 9, 0, 9, 0, 5, 8, 5, 8, 0, 0, 0, 0};
	SpriteParams susieParam = {4, 1, 6, 1, 11, 4, 9, 2, 5, 6, 8,
	19, 0, 21, 24, 21, 24, 21, 24, 21, 24, 19, 14, 19, 29, 19, 0, 19, 0, 17, 22, 20, 0};
	party.push_back(new PartyMember(krisSpriteSheet, numFontSpriteSheet, krisParam, TYPE_KRIS, 90, 10, 2, 9, 1, C_BLUE, C_KRISBLUE, C_KRISDARKBLUE));
	party.push_back(new PartyMember(susieSpriteSheet, numFontSpriteSheet, susieParam, TYPE_SUSIE, 110, 14, 2, 9, 2, C_PINK, C_SUSIEPINK, C_SUSIEDARKPINK));
	party.push_back(new PartyMember(ralseiSpriteSheet, numFontSpriteSheet, ralseiParam, TYPE_RALSEI, 70, 8, 2, 9, 3, C_GREEN, C_RALSEIGREEN, C_RALSEIDARKGREEN));

	dummy = new TestEnemy(180, 180, 7, 0, 10, 0, 100);
	dummy->setSprites(enemySpriteSheet);
	dummy->setBattleText();
	dummy->setActs();

	std::string battleText = dummy->getBattleText();
	std::string actionText;
	std::string descText;
	std::string descTpText;

	char text_writebuf[256] = "";
	int textindex = 0;
	float text_x = 0.0, text_y = 0.0;
	bool redrawText = false;

	int battleBoxAnim = 40;
	int battleBoxAnimTimer = battleBoxAnim;
	int battleTime = 700;
	int battleTimer = battleTime;
	for(int i = 0; i < 2; i++) {
		C2D_SpriteFromSheet(&battlebg_spr[i], mainSpriteSheet, i + 2); //fuckass code
		C2D_SpriteSetCenter(&battlebg_spr[i], 0.5f, 0.5f);
		C2D_SpriteSetPos(&battlebg_spr[i], TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2);
	}
	for(int i = 0; i < 12; i++) {
		C2D_SpriteFromSheet(&battlebt_spr[i], mainSpriteSheet, i + 4);
		C2D_SpriteSetCenter(&battlebt_spr[i], 0.5f, 0.5f);
		C2D_SpriteSetScale(&battlebt_spr[i], 1.005f, 1.005f);
		//C2D_SpriteSetPos(&battlebt_spr[i], 16 + 36 * i, BOTTOM_SCREEN_HEIGHT / 2);
	}
	C2D_SpriteFromSheet(&tplogo_spr, mainSpriteSheet, 16);
	C2D_SpriteSetCenter(&tplogo_spr, 0.5f, 0.5f);
	C2D_SpriteSetScale(&tplogo_spr, 1.005f, 1.005f);
	C2D_SpriteSetPos(&tplogo_spr, 40, 130);

	for(int i = 0; i < 4; i += 2) {
		C2D_SpriteSetPos(&battlebt_spr[i], BOTTOM_SCREEN_WIDTH - 36 * (5 - i/2), BOTTOM_SCREEN_HEIGHT - 19);
		C2D_SpriteSetPos(&battlebt_spr[i+1], BOTTOM_SCREEN_WIDTH - 36 * (5 - i/2), BOTTOM_SCREEN_HEIGHT - 19);
	}
	C2D_SpriteSetPos(&battlebt_spr[4], BOTTOM_SCREEN_WIDTH - 36 * 4, BOTTOM_SCREEN_HEIGHT - 19);
	C2D_SpriteSetPos(&battlebt_spr[5], BOTTOM_SCREEN_WIDTH - 36 * 4, BOTTOM_SCREEN_HEIGHT - 19);
	for(int i = 6; i < 12; i += 2) {
		C2D_SpriteSetPos(&battlebt_spr[i], BOTTOM_SCREEN_WIDTH - 36 * (6 - i/2), BOTTOM_SCREEN_HEIGHT - 19);
		C2D_SpriteSetPos(&battlebt_spr[i+1], BOTTOM_SCREEN_WIDTH - 36 * (6 - i/2), BOTTOM_SCREEN_HEIGHT - 19);
	}

	std::vector<int> inventory = {1, 1, 2, 3, 4, 5, 6};
	std::vector<int> usedItemsStack;

	int choice[4] = {0, -1, -1, -1}; // action of every party member
	int choiceMenu[4] = {0, -1, -1, -1}; // menu choice of every party member
	int enemyChoice[4] = {0, -1, -1, -1}; // enemy on which to use action on, when healing stored the party member being healed
	int heartChoice = 0; // menu choice
	int menuOffset = 0; // offset the menu choicec when having more than 4 choices available
	int itemChoice = -1; // what item to use
	size_t optionNum = 0; // number of options that can be chosen in a menu
	int prevCurAnim = 0; // used when doing healing logic because I am a horrible coder
	int healsDone = 0; // ditto
	int activeParty = -1; // stores the active party member when we start doing actions

	// Main loop
	while (aptMainLoop()) {
		hidScanInput(); //get player inputs

		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();

		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		if(!mybattle) {
			if(battleBoxAnimTimer > 0 && (battleBoxAnimTimer < battleBoxAnim || battleTimer > 0)) {
				if(battleTimer > 0)
					battleBoxAnimTimer--;
				float scale = 1.0 / battleBoxAnim * (battleBoxAnim - battleBoxAnimTimer);
				for(int i = 0; i < 2; i++) {
					C2D_SpriteSetScale(&battlebg_spr[i], scale, scale);
					C2D_SpriteSetRotationDegrees(&battlebg_spr[i], battleBoxAnimTimer * 4);
				}
				if(battleTimer <= 0)
					battleBoxAnimTimer++;
			}
			else {
				if(battleTimer <= 0) {
					changeBattle = true;
					bulletController->deleteBullets();
					heart->setGrazing(false);
					heart->hurt(false);
					for(p_it = party.begin(); p_it != party.end(); p_it++)
						(*p_it)->setChoice(-1);
					turn = 1;
					numTurns++;
					dummy->setTurnNum(numTurns);
					battleText = dummy->getBattleText();
					clearCString(text_writebuf);
					textindex = 0;
				}
				else {
					int xd = 0, yd = 0;
					if ((kHeld & KEY_DLEFT))
						xd = -1;
					if ((kHeld & KEY_DRIGHT))
						xd = 1;
					if ((kHeld & KEY_DUP))
						yd = -1;
					if ((kHeld & KEY_DDOWN))
						yd = 1;

					if(kDown & KEY_L) // for testing purposes
						battleTimer = 1;

					heart->move(xd, yd);
					int heartX = heart->getX(), heartY = heart->getY();
					int heartW = heart->getHitboxW(), heartH = heart->getHitboxH();
					int heartTensionW = heart->getTensionHitboxW(), heartTensionH = heart->getTensionHitboxH();

					bulletController->doAttack(0, 30, heartX, heartY);
					int index = bulletController->doBulletCollision(heartX, heartY, heartW, heartH);
					if(index > -1) {
						if(!heart->getHurt()) {
							heart->hurt();
							for(p_it = party.begin(); p_it != party.end(); p_it++)
								(*p_it)->hurt(bulletController->getBulletDamage(index));
						}
					}

					if(!heart->getHurt()) {
						index = bulletController->doBulletCollision(heartX, heartY, heartTensionW, heartTensionH);
						if(index > -1) {
							tension_heal(bulletController->getBulletGrazePoints(index));
							heart->setGrazing(true);
							battleTimer -= bulletController->getBulletTimePoints(index);
						}
					}

					battleTimer--;
					if(battleTimer <= 0)
						battleBoxAnimTimer = 1;
				}
			}
		}
		else {
			if(turn >= 4) { // all choices have been done, do actions now
				bool done = true;
				bool pressed = (kDown & KEY_A);

				for(p_it = party.begin(); p_it != party.end(); p_it++) { // does every non-attack choice in order, then does the attacks
					activeParty = std::distance(party.begin(), p_it);
					if((*p_it)->getActionDone() || choice[activeParty + 1] == 0) continue;
					done = false;
					if(choice[activeParty + 1] == 2) {
						if((*p_it)->getCurAnim() == -1 && prevCurAnim != -1) {
							party[enemyChoice[activeParty + 1]]->heal(items[inventory[choiceMenu[activeParty + 1]]].heal); // todo: particular cases like whole party heals
							healsDone++;
						}
						prevCurAnim = (*p_it)->getCurAnim();
					}
					if((*p_it)->getDoingAction()) break;
					switch(choice[activeParty + 1]) {
						case 1:
							if((*p_it)->getType() == TYPE_KRIS)
								battleText = dummy->doAct(choiceMenu[activeParty + 1]);
							else
								battleText = "* " + (*p_it)->getName() + " used " + ((*p_it)->getSpell(choiceMenu[activeParty + 1])).name + "!";
							clearCString(text_writebuf);
							textindex = 0;
							break;

						case 2:
							battleText = "* " + (*p_it)->getName() + " used the\n" + items[inventory[choiceMenu[activeParty + 1]]].name + "!";
							clearCString(text_writebuf);
							textindex = 0;
							break;

						case 3:
							battleText = dummy->isSpareable() ? "* " + (*p_it)->getName() + " spared " + dummy->getName() + "!" :
																"* " + (*p_it)->getName() + " spared " + dummy->getName() + "!\nBut its name wasn't YELLOW...";
							clearCString(text_writebuf);
							textindex = 0;
							break;
					}
					break;
				}
				if(!done)
					party[activeParty]->doAction(pressed);
				else {
					if(battleText.length() > 0) {
						battleText = "";
						clearCString(text_writebuf);
						textindex = 0;
					}
					double minBarPos = 300.0;
					for(p_it = party.begin(); p_it != party.end(); p_it++) {
						if(!(*p_it)->getDoingAction() && choice[std::distance(party.begin(), p_it) + 1] == 0) {
							double curBarPos = (*p_it)->getAtkbarPos();
							if(curBarPos <= minBarPos)
								minBarPos = curBarPos;
						}
					}
					for(p_it = party.begin(); p_it != party.end(); p_it++) {
						if(choice[std::distance(party.begin(), p_it) + 1] == 0) {
							if((*p_it)->getAtkbarPos() == minBarPos)
								(*p_it)->doAction(pressed);
							else
								(*p_it)->doAction(false);

							if((*p_it)->getDealDamage()) {
								dummy->hurt((*p_it)->getTrueAt());
								tension_heal((*p_it)->getPoints() / 10); // todo: is points/15 against jevil
							}
						}
						done = done && (*p_it)->getActionDone();
					}
					if(done) {
						changeBattle = true;
						battleTimer = battleTime;
						battleBoxAnimTimer = battleBoxAnim;
						prevCurAnim = 0;
						healsDone = 0;
						heart->setCoords(TOP_SCREEN_WIDTH / 2, TOP_SCREEN_HEIGHT / 2);
						for(p_it = party.begin(); p_it != party.end(); p_it++) // doing this so the idle anim plays after our turn is over, bit of a workaround
							if(choice[std::distance(party.begin(), p_it) + 1] == 0)
								(*p_it)->setChoice(-1);

						std::sort(usedItemsStack.begin(), usedItemsStack.end(), std::greater<int>()); // sort vector in descending order
						for(size_t i = 0; i < usedItemsStack.size(); i++)
							inventory.erase(inventory.begin() + usedItemsStack[i]);
						usedItemsStack.clear();
					}
				}
			}
			else {
				if ((kDown & KEY_DLEFT) && menuOptionSel > 0 && !inMenu)
					menuOptionSel--;
				if ((kDown & KEY_DRIGHT) && menuOptionSel < 4 && !inMenu)
					menuOptionSel++;
				if((kDown & KEY_DUP) && inMenu && heartChoice > 0) {
					heartChoice--;
					if(heartChoice < menuOffset)
						menuOffset--;
					else
						heart->setCoords(heart->getX(), heart->getY() - 20.0);

					switch(menuOptionSel) {
						case 0:
						case 3:
							enemyChoice[turn] = heartChoice;
							break;

						case 1:
							if(party[turn - 1]->getType() == TYPE_KRIS) break;
							if(!inMenu2)
								enemyChoice[turn] = heartChoice;
							descText = (party[turn - 1]->getSpell(heartChoice)).description;
							descTpText = std::to_string((party[turn - 1]->getSpell(heartChoice)).cost) + "% TP";
							break;

						case 2:
							if(inMenu && !inMenu2)
								itemChoice = heartChoice;
							descText = items[inventory[heartChoice]].description;
							break;
					}

					redrawText = true;
				}
				if((kDown & KEY_DDOWN) && inMenu && (size_t)heartChoice < optionNum - 1) {
					heartChoice++;
					if(heartChoice > maxMenuRows + menuOffset - 1)
						menuOffset++;
					else
						heart->setCoords(heart->getX(), heart->getY() + 20.0);

					switch(menuOptionSel) {
						case 0:
						case 3:
							enemyChoice[turn] = heartChoice;
							break;

						case 1:
							if(party[turn - 1]->getType() == TYPE_KRIS) break;
							if(!inMenu2)
								enemyChoice[turn] = heartChoice;
							descText = (party[turn - 1]->getSpell(heartChoice)).description;
							descTpText = std::to_string((party[turn - 1]->getSpell(heartChoice)).cost) + "% TP";
							break;

						case 2:
							if(inMenu && !inMenu2)
								itemChoice = heartChoice;
							descText = items[inventory[heartChoice]].description;
							break;
					}

					redrawText = true;
				}
				if ((kDown & KEY_A)) {
					if(!inMenu && menuOptionSel != 4) {
						if(menuOptionSel != 2 || inventory.size() > 0) {
							inMenu = true;
							heartChoice = 0;
							switch(menuOptionSel) {
								case 0:
								case 1:
								case 3:
									actionText = dummy->getName();
									heart->setCoords(10.0, 15.0);
									optionNum = 1; // change depending on num of enemies
									break;
								case 2:
									optionNum = inventory.size();
									itemChoice = 0;
									actionText = "";
									heart->setCoords(10.0, 15.0);
									for(size_t i = 0; i < optionNum; i++)
										actionText += items[inventory[i]].name + "\n";

									descText = items[inventory[heartChoice]].description;
									break;
							}
							redrawText = true;
						}
					}
					else if(!inMenu2 && menuOptionSel != 0 && menuOptionSel != 3 && menuOptionSel != 4) {
						// todo: get enemy id and get their acts
						if(menuOptionSel != 2 || std::find(usedItemsStack.begin(), usedItemsStack.end(), heartChoice) == usedItemsStack.end()) {
							inMenu2 = true;
							heartChoice = 0;
							switch(menuOptionSel) {
								case 1:
									if(party[turn - 1]->getType() == TYPE_KRIS) {
										std::vector<std::string> actArray = dummy->getActs();
										optionNum = actArray.size();
										actionText = "";
										for(size_t i = 0; i < optionNum; i++)
											actionText += actArray[i] + "\n";
									}
									else {
										std::vector<Spell> spellArray = party[turn - 1]->getSpells();
										optionNum = spellArray.size();
										actionText = "";
										for(size_t i = 0; i < optionNum; i++)
											actionText += spellArray[i].name + "\n";

										descText = spellArray[heartChoice].description;
										descTpText = std::to_string(spellArray[heartChoice].cost) + "% TP";
									}
									heart->setCoords(10.0, 15.0);
									break;

								case 2:
									optionNum = party.size();
									actionText = "";
									heart->setCoords(10.0, 15.0);
									for(size_t i = 0; i < optionNum; i++)
										actionText += party[i]->getName() + "\n";
									break;
							}
							redrawText = true;
						}
					}
					else {
						bool good = true;
						switch(menuOptionSel) {
							case 1: {
								if(party[turn - 1]->getType() == TYPE_KRIS) break;
								int spellCost = (party[turn - 1]->getSpell(heartChoice)).cost;
								if(spellCost > tension)
									good = false;
								else
									tension -= spellCost;
								break;
							}

							case 2:
								usedItemsStack.push_back(itemChoice);
								enemyChoice[turn] = heartChoice;
								break;

							case 4:
								tension_heal(defendTension);
								heartChoice = -1;
								break;
						}

						if(good) {
							inMenu = false;
							inMenu2 = false;
							redrawText = true;
							actionText = "";
							descText = "";
							descTpText = "";
							choice[turn] = menuOptionSel;
							menuOffset = 0;
							if(menuOptionSel == 2)
								choiceMenu[turn] = itemChoice;
							else
								choiceMenu[turn] = heartChoice;
							party[turn - 1]->setChoice(menuOptionSel, -1, dummy->getX(), dummy->getY(), dummy->getDF());
							turn++;
						}
					}
				}
				if (kDown & KEY_B) {
					if(inMenu2) {
						inMenu2 = false;
						redrawText = true;
						heartChoice = 0;
						menuOffset = 0;
						switch(menuOptionSel) {
							case 1:
								actionText = dummy->getName();
								heart->setCoords(10.0, 15.0);
								optionNum = 1;
								break;

							case 2:
								optionNum = inventory.size();
								actionText = "";
								heart->setCoords(10.0, 15.0);
								for(size_t i = 0; i < optionNum; i++)
									actionText += items[inventory[i]].name + "\n";
								break;
						}
					}
					else if(inMenu) {
						inMenu = false;
						redrawText = true;
						actionText = dummy->getName();
					}
					else if(turn > 1) {
						turn--;
						switch(choice[turn]) {
							case 1:
								if(party[turn - 1]->getType() == TYPE_KRIS) break;
								tension += (party[turn - 1]->getSpell(choiceMenu[turn])).cost;
								break;
							case 2:
								usedItemsStack.pop_back();
								break;
							case 4:
								tension -= defendTension;
								break;
						}

						choice[turn] = -1;
						party[turn - 1]->setChoice(-1);
					}
				}
			}
		}

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C_BLACK);
		C3D_FrameDrawOn(top);
		C2D_SceneBegin(top);

		if(!mybattle) {
			C2D_DrawSprite(&battlebg_spr[1]);
			C2D_DrawSprite(&battlebg_spr[0]);
			heart->draw();
		}

		dummy->move();
		if(!mybattle)
			bulletController->draw();

		for(p_it = party.begin(); p_it != party.end(); p_it++)
			(*p_it)->drawTop();
		//C2D_DrawRectangle(heartX - heartW/2, heartY - heartH/2, 0, heartW, heartH, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		//C2D_DrawSprite(dummy->getSpr());

		C2D_TargetClear(bot, C_BLACK);
		C3D_FrameDrawOn(bot);
		C2D_SceneBegin(bot);

		// heart
		if(inMenu)
			heart->draw();

		// battle text
		if(mybattle) {
			if(!inMenu) {
				C2D_TextBufClear(fontbuf);
				if(redrawText) {
					strcpy(text_writebuf, battleText.c_str());
					textindex = battleText.length();
				}
				else if((size_t)textindex < battleText.length()) {
					text_writebuf[textindex] = battleText[textindex];
					textindex++;
					if((size_t)textindex == battleText.length())
						text_writebuf[textindex] = 0;
				}
				text_x = 10.0;
				text_y = 20.0;
			}
			else if(redrawText) {
				C2D_TextBufClear(fontbuf);
				strcpy(text_writebuf, actionText.c_str());
				text_x = 30.0;
				text_y = 20.0;
			}

			if(inMenu && !inMenu2 && menuOptionSel == 2 && text_writebuf[0] != 0) { // item menu, draw items grayed out if they're currently being used by someone else
				char text_actualbuf[256];
				strcpy(text_actualbuf, text_writebuf);
				char *tok = strtok(text_actualbuf, "\n");
				float text_ytemp = text_y;
				for(int i = 0; i < std::count(actionText.begin(), actionText.end(), '\n'); i++) {
					if(i >= menuOffset && i < menuOffset + 4) {
						C2D_TextFontParse(&text_binfo, font_main, fontbuf, tok);
						C2D_TextOptimize(&text_binfo);
						C2D_DrawText(&text_binfo, C2D_AtBaseline | C2D_WithColor, text_x, text_ytemp, 0, 0.65f, 0.65f, std::find(usedItemsStack.begin(), usedItemsStack.end(), i) == usedItemsStack.end() ? C_WHITE : C_GRAY);

						text_ytemp += 20.0;
					}
					tok = strtok(NULL, "\n");
				}
			}
			else if(inMenu2 && menuOptionSel == 1 && party[turn - 1]->getType() != TYPE_KRIS && text_writebuf[0] != 0) { // draw the spell menu, grayed out text if you don't have enough TP
				char text_actualbuf[256];
				strcpy(text_actualbuf, text_writebuf);
				char *tok = strtok(text_actualbuf, "\n"); // i can't believe i'm doing this
				float text_ytemp = text_y;
				for(int i = 0; i < std::count(actionText.begin(), actionText.end(), '\n'); i++) {
					C2D_TextFontParse(&text_binfo, font_main, fontbuf, tok);
					C2D_TextOptimize(&text_binfo);
					C2D_DrawText(&text_binfo, C2D_AtBaseline | C2D_WithColor, text_x, text_ytemp, 0, 0.65f, 0.65f, (party[turn - 1]->getSpell(i)).cost <= tension ? C_WHITE : C_GRAY);

					text_ytemp += 20.0;
					tok = strtok(NULL, "\n");
				}
			}
			else {
				C2D_TextFontParse(&text_binfo, font_main, fontbuf, text_writebuf);
				C2D_TextOptimize(&text_binfo);
				C2D_DrawText(&text_binfo, C2D_AtBaseline | C2D_WithColor, text_x, text_y, 0, 0.65f, 0.65f, C_WHITE);
				if(inMenu && !inMenu2 && menuOptionSel == 0)
					drawHPBar(dummy->getHP(), dummy->getMaxHP(), 250, text_y - 10, 0, 60, 12, C_GREEN, C_DARKRED);
			}

			if(descText.length() != 0 && ((inMenu2 && menuOptionSel == 1 && party[turn - 1]->getType() != TYPE_KRIS) || (inMenu && !inMenu2 && menuOptionSel == 2))) {
				float textd_x = 190.0, textd_y = 20.0;
				C2D_TextFontParse(&text_bdesc, font_main, fontbuf, descText.c_str());
				C2D_TextOptimize(&text_bdesc);
				C2D_DrawText(&text_bdesc, C2D_AtBaseline | C2D_WithColor, textd_x, textd_y, 0, 0.65f, 0.65f, C_GRAY);

				textd_y += 20 * (std::count(descText.begin(), descText.end(), '\n') + 1);

				if(menuOptionSel == 1) {
					C2D_TextFontParse(&text_bdesc, font_main, fontbuf, descTpText.c_str());
					C2D_TextOptimize(&text_bdesc);
					C2D_DrawText(&text_bdesc, C2D_AtBaseline | C2D_WithColor, textd_x, textd_y, 0, 0.65f, 0.65f, C_ORANGE);
				}
			}

			redrawText = false;
		}

		//menu options
		if(turn > 0 && turn < 4) {
			for(int i = 0; i < 5; i++) {
				int sprPos;
				if(party[turn - 1]->getType() == TYPE_KRIS)  // if it's kris' turn, display the ACT button, otherwise display the MAGIC button
					sprPos = i*2 + 2*(i>1);
				else
					sprPos = i*2 + 2*(i>0);

				if(menuOptionSel == i)
					C2D_DrawSprite(&battlebt_spr[sprPos + 1]);
				else
					C2D_DrawSprite(&battlebt_spr[sprPos]);
			}
		}

		//party member specifics
		for(p_it = party.begin(); p_it != party.end(); p_it++)
			(*p_it)->drawBot(turn);

		//tp meter
		C2D_DrawSprite(&tplogo_spr);

		C2D_TextBufClear(fontbuf);
		char text_tpbuf[8];
		snprintf(text_tpbuf, 8, "%d\n%%", tension);
		C2D_TextFontParse(&text_tp, font_main, fontbuf, text_tpbuf);
		C2D_TextOptimize(&text_tp);

		C2D_DrawText(&text_tp, C2D_AlignCenter | C2D_WithColor, 40.0, 150.0, 0, 0.90f, 0.90f, C_WHITE);

		int bar_x = 70, bar_y = 100, bar_w = 20, bar_h = 130;
		float tpRatio = 1.0 - ((float)tension)/tensionMax;

		C2D_DrawRectSolid(bar_x, bar_y, 0, bar_w, bar_h * tpRatio, C_DARKRED);
		C2D_DrawRectSolid(bar_x, bar_y + bar_h * tpRatio, 0, bar_w, bar_h - bar_h * tpRatio, C_ORANGE);
		C2D_DrawTriangle(bar_x, bar_y + bar_h, C_BLACK, bar_x + bar_w, bar_y + bar_h, C_BLACK, bar_x + bar_w, bar_y + bar_h * 0.85, C_BLACK, 0);
		C2D_DrawTriangle(bar_x, bar_y, C_BLACK, bar_x, bar_y + bar_h * 0.15, C_BLACK, bar_x + bar_w, bar_y, C_BLACK, 0);

		C3D_FrameEnd(0);

		if(changeBattle) {
			mybattle = !mybattle;
			changeBattle = false;
		}
	}

	exit_game();

	return 0;
}
