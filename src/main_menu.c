/*
 * main_menu.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Adrian
 */

#include "./headers/main_menu_pub_methods.h"
#include <stdio.h>
static mainMenu * thisMainMenu;

void setUpAvatars(){
	thisMainMenu->newGame->numAvatars = 4;
	thisMainMenu->newGame->currentAvatar = rand() % 4;
	thisMainMenu->newGame->avatars[0] = malloc(sizeof(character));
	thisMainMenu->newGame->avatars[1] = malloc(sizeof(character));
	thisMainMenu->newGame->avatars[2] = malloc(sizeof(character));
	thisMainMenu->newGame->avatars[3] = malloc(sizeof(character));

	//human - male - white
	character * tmpCharacter = thisMainMenu->newGame->avatars[0];
	animationContainer * mainContainer = initAnimationContainer();
	animationContainer * secondaryContainer;

	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2001));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2002));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2003));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2004));

	secondaryContainer = cloneAnimationContainer(mainContainer);

	tmpCharacter->thisAnimationContainer = mainContainer;
	tmpCharacter->secondaryAnimationContainer = secondaryContainer;
	tmpCharacter->darkAnimationContainer = NULL;
	tmpCharacter->direction = 0;
	tmpCharacter->x = 1;
	tmpCharacter->y = 1;
	tmpCharacter->xOff = 0;
	tmpCharacter->yOff = 0;

	//human - male - tan
	tmpCharacter = thisMainMenu->newGame->avatars[1];
	mainContainer = initAnimationContainer();

	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2005));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2008));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2009));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2010));

	secondaryContainer = cloneAnimationContainer(mainContainer);

	tmpCharacter->thisAnimationContainer = mainContainer;
	tmpCharacter->secondaryAnimationContainer = secondaryContainer;
	tmpCharacter->darkAnimationContainer = NULL;
	tmpCharacter->direction = 0;
	tmpCharacter->x = 1;
	tmpCharacter->y = 1;
	tmpCharacter->xOff = 0;
	tmpCharacter->yOff = 0;

	//human - male - brown
	tmpCharacter = thisMainMenu->newGame->avatars[2];
	mainContainer = initAnimationContainer();

	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2006));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2011));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2012));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2013));

	secondaryContainer = cloneAnimationContainer(mainContainer);

	tmpCharacter->thisAnimationContainer = mainContainer;
	tmpCharacter->secondaryAnimationContainer = secondaryContainer;
	tmpCharacter->darkAnimationContainer = NULL;
	tmpCharacter->direction = 0;
	tmpCharacter->x = 1;
	tmpCharacter->y = 1;
	tmpCharacter->xOff = 0;
	tmpCharacter->yOff = 0;

	//human - male - black
	tmpCharacter = thisMainMenu->newGame->avatars[3];
	mainContainer = initAnimationContainer();

	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2007));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2014));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2015));
	addAnimationToContainer(mainContainer,getAnimationFromRegistry(2016));

	secondaryContainer = cloneAnimationContainer(mainContainer);

	tmpCharacter->thisAnimationContainer = mainContainer;
	tmpCharacter->secondaryAnimationContainer = secondaryContainer;
	tmpCharacter->darkAnimationContainer = NULL;
	tmpCharacter->direction = 0;
	tmpCharacter->x = 1;
	tmpCharacter->y = 1;
	tmpCharacter->xOff = 0;
	tmpCharacter->yOff = 0;
}

void cloneNewGameIndividual(){
	individual * newGameClone = initIndividual();
	individual * basePlayer = getIndividualFromRegistry(0);

	newGameClone->ID = 1;
	strcpy(newGameClone->name, basePlayer->name);
	newGameClone->isPlayer = 1;

	newGameClone->STR = basePlayer->STR;
	newGameClone->DEX = basePlayer->DEX;
	newGameClone->CON = basePlayer->CON;
	newGameClone->INT = basePlayer->INT;
	newGameClone->WIS = basePlayer->WIS;
	newGameClone->WILL = basePlayer->WILL;
	newGameClone->CHR = basePlayer->CHR;
	newGameClone->LUCK = basePlayer->LUCK;

	newGameClone->baseHP = basePlayer->baseHP;
	newGameClone->hp = basePlayer->hp;
	newGameClone->baseMana = basePlayer->baseMana;
	newGameClone->mana = basePlayer->mana;
	newGameClone->totalActions = basePlayer->totalActions;
	newGameClone->remainingActions = basePlayer->remainingActions;
	newGameClone->hasAttacked = basePlayer->hasAttacked;
	newGameClone->totalFood = basePlayer->totalFood;
	newGameClone->food = basePlayer->food;
	newGameClone->foodBonusActive = basePlayer->foodBonusActive;
	newGameClone->foodNegativeActive = basePlayer->foodNegativeActive;
	newGameClone->AC = basePlayer->AC;
	newGameClone->attack = basePlayer->attack;
	newGameClone->maxDam = basePlayer->maxDam;
	newGameClone->minDam = basePlayer->minDam;
	newGameClone->baseDam = basePlayer->baseDam;

	newGameClone->range = basePlayer->range;
	strcpy(newGameClone->critType, basePlayer->critType);
	newGameClone->mvmt = basePlayer->mvmt;
	newGameClone->LoS = basePlayer->LoS;
	newGameClone->darkLoS = basePlayer->darkLoS;
	newGameClone->isSneaking = basePlayer->isSneaking;
	newGameClone->bluntDR = basePlayer->bluntDR;
	newGameClone->chopDR = basePlayer->chopDR;
	newGameClone->slashDR = basePlayer->slashDR;
	newGameClone->pierceDR = basePlayer->pierceDR;
	newGameClone->earthDR = basePlayer->earthDR;
	newGameClone->fireDR = basePlayer->fireDR;
	newGameClone->waterDR = basePlayer->waterDR;
	newGameClone->lightningDR = basePlayer->lightningDR;
	newGameClone->dialogID = basePlayer->dialogID;
	newGameClone->dialogPortraitID = basePlayer->dialogPortraitID;
	newGameClone->fateTokens = basePlayer->fateTokens;
	newGameClone->gold = basePlayer->gold;
	newGameClone->jumpTarget = basePlayer->jumpTarget;
	newGameClone->faction = basePlayer->faction;
	newGameClone->targetedDuration = basePlayer->targetedDuration;

	newGameClone->specialDialog = malloc(sizeof(specialDialogs));
	newGameClone->specialDialog->sawPlayerCrime = basePlayer->specialDialog->sawPlayerCrime;
	newGameClone->specialDialog->attackedByPlayer = basePlayer->specialDialog->attackedByPlayer;
	newGameClone->specialDialog->stolenFromByPlayer = basePlayer->specialDialog->stolenFromByPlayer;
	newGameClone->specialDialog->afraidOfPlayer = basePlayer->specialDialog->afraidOfPlayer;
	newGameClone->specialDialog->playerIsMarkedForDeath = basePlayer->specialDialog->playerIsMarkedForDeath;

	newGameClone->desiredLocation->x = 0;
	newGameClone->desiredLocation->y = 0;

	newGameClone->thisBehavior->offensiveness = basePlayer->thisBehavior->offensiveness;
	newGameClone->thisBehavior->abilityAffinity = basePlayer->thisBehavior->abilityAffinity;
	newGameClone->thisBehavior->tacticalness = basePlayer->thisBehavior->tacticalness;
	newGameClone->thisBehavior->cowardness = basePlayer->thisBehavior->cowardness;
	newGameClone->thisBehavior->isHostileToPlayer = basePlayer->thisBehavior->isHostileToPlayer;
	newGameClone->thisBehavior->isFocusedOnPlayer = basePlayer->thisBehavior->isFocusedOnPlayer;
	newGameClone->thisBehavior->isSurrounded = basePlayer->thisBehavior->isSurrounded;
	newGameClone->thisBehavior->respawns = basePlayer->thisBehavior->respawns;

	thisMainMenu->newGame->newPlayer = newGameClone;
}

void setUpSaveLoadData(){
	int i;
	char * strtok_save_pointer;
	char saveDirectory[256];
	char line[64];

	for(i = 0; i < thisMainMenu->load->numSaveData; i++){
		sprintf(saveDirectory, "%ssaves\\save%d\\saveMetaData.txt", thisMainMenu->rootMapDirectory, i);

		FILE * FP = fopen(saveDirectory, "r");

		if(FP == NULL){
			thisMainMenu->load->saves[i] = NULL;
		}else{
			thisMainMenu->load->saves[i] = malloc(sizeof(loadSaveData));

			fgets(line, 64, FP);
			thisMainMenu->load->saves[i]->saveNumber = i;

			char * value = strtok_r(line, ";", &strtok_save_pointer);
			strcpy(thisMainMenu->load->saves[i]->name, value);

			value = strtok_r(NULL, ";", &strtok_save_pointer);
			thisMainMenu->load->saves[i]->level = atoi(line);
		}

		fclose(FP);
	}
}

void initMainMenu(int inMenuMode, char * mapDirectory){
	int i;

	thisMainMenu = malloc(sizeof(mainMenu));
	thisMainMenu->title = malloc(sizeof(titleMenu));
	thisMainMenu->newGame = malloc(sizeof(newGameMenu));
	thisMainMenu->load = malloc(sizeof(loadMenu));
	thisMainMenu->inMenuMode = inMenuMode;
	thisMainMenu->currentMenu = MENU_TITLE;
	thisMainMenu->waitingForNameMode = 0;
	thisMainMenu->reloadBaseGame = 0;
	thisMainMenu->selectedDecision = CONFIRMATION_NO;
	thisMainMenu->confirmationDialog = createCharacter(1421, RGB(255,0,255), 0, 0);
	thisMainMenu->leftSelectArrow = createCharacter(1402, RGB(255,0,255), 0, 0);
	thisMainMenu->rightSelectArrow = createCharacter(1504, RGB(255,0,255), 0, 0);
	thisMainMenu->confirmationDialogSelect = createCharacter(1411, RGB(255,0,255), 0, 0);

	thisMainMenu->title->titleScreen = createCharacter(1400, RGB(255,0,255), 0, 0);
	thisMainMenu->title->options = createCharacter(1403, RGB(255,0,255), 0, 0);
	thisMainMenu->title->titleOptionsDisplayed = 0;
	thisMainMenu->title->selectedOption = TITLE_NEW_GAME;
	thisMainMenu->rightSelectArrow->y = 13;

	thisMainMenu->newGame->currentForm = NEW_GAME_CREATE;
	thisMainMenu->newGame->currentField = CREATE_NAME;
	thisMainMenu->newGame->currentSpread = SPREAD_8_8;
	setUpAvatars();

	thisMainMenu->newGame->inEditMode = 0;
	thisMainMenu->newGame->inAbilityEditMode = 0;
	thisMainMenu->newGame->statPoints = 6;
	thisMainMenu->newGame->numAbilites = 2;
	cloneNewGameIndividual();
	thisMainMenu->newGame->newAbilities = malloc(sizeof(abilityList));
	thisMainMenu->newGame->newAbilities->numAbilities = 0;
	thisMainMenu->newGame->newAbilities->MAX_ABILITIES = 6;
	thisMainMenu->newGame->selectedAbility = 0;

	for(i = 0; i < thisMainMenu->newGame->newAbilities->MAX_ABILITIES; i++){
		thisMainMenu->newGame->newAbilities->abilitiesList[i] = NULL;
	}

	thisMainMenu->newGame->descriptionRect = malloc(sizeof(RECT));
	thisMainMenu->newGame->descriptionRect->top = 0;
	thisMainMenu->newGame->descriptionRect->left = 0;
	thisMainMenu->newGame->descriptionRect->bottom = 130;
	thisMainMenu->newGame->descriptionRect->right = 486;

	thisMainMenu->newGame->creationScreen = createCharacter(1401, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->abilityCreationScreen = createCharacter(1416, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry1Select = createCharacter(1404, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry1Edit = createCharacter(1409, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry2Select = createCharacter(1405, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry2Edit = createCharacter(1410, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry3Select = createCharacter(1406, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry3Edit = createCharacter(1411, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry4Select = createCharacter(1407, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry4Edit = createCharacter(1412, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry5Select = createCharacter(1408, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry5Edit = createCharacter(1413, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry6Select = createCharacter(1414, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entry6Edit = createCharacter(1415, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->entryAddAbility = createCharacter(1417, RGB(255,0,255), 0, 0);

	strcpy(thisMainMenu->newGame->descriptionName, "The name of your character.");
	strcpy(thisMainMenu->newGame->descriptionSpread, "The starting HP/Mana of your character. Every time your character levels up, they will recieve at least half their dice spread for HP/Mana.\n\nFighters may opt for a 12/4 spread, wizards a 4/12, and the wily rogue an 8/8.");
	strcpy(thisMainMenu->newGame->descriptionAvatar, "The avatar of your character.");
	strcpy(thisMainMenu->newGame->descriptionSTR, "How strong your character is. Determines how much physical damage they do.\n\nA fighter with 3 strength could easily break down a door, while a wizard with -2 strength could hardly manage to open a door.");
	strcpy(thisMainMenu->newGame->descriptionDEX, "How much dexterity your character has. Determines their AC defense and how far they can move in a turn.\n\nA rogue with 3 dexterity is nimble and difficult to hit, while a bumbling paladin with -2 dexterity would trip over their boots.");
	strcpy(thisMainMenu->newGame->descriptionCON, "How tough your character is. Determines their starting hit points and how long they can go without food.\n\nA monk with 3 constitution could handle walking through the desert, while a mage with -2 constitution might collapse after walking up a grassy hill.");
	strcpy(thisMainMenu->newGame->descriptionINT, "How intelligent your character is. Determines the cost of abilities and abilities gained each level.\n\nA wizard with +3 intelligence could summon powerful bolts of lightning, while a fighter with -2 intelligence could have trouble remembering their name.");
	strcpy(thisMainMenu->newGame->descriptionWIS, "How much wisdom your character has. Determines how well they use weapons and the number of abilities they start with.\n\nA weapons master with 3 wisdom will land many blows on their target, while a talentless barbarian will frequently 'wiff'.");
	strcpy(thisMainMenu->newGame->descriptionWILL, "How much willpower your character has. Determines their starting mana points and their spell resistance.\n\nA mage with +3 willpower will be able to cast abilities several times, while a fighter with -2 willpower couldn't cast themselves out of a sack.");
	strcpy(thisMainMenu->newGame->descriptionCHR, "How much charisma your character has. Determines how well they interact with other characters and their starting fate tokens.\n\nA bard with +3 charisma could smooth talk themselves out of a sticky situation, while a raider with -2 charisma could smooth talk themselves into a sticky situation.");
	strcpy(thisMainMenu->newGame->descriptionLUCK, "How lucky your character is. Determines how frequently they land critical hits and helps a little bit with everything they do.\n\nA rogue with +3 luck may find a valuable amulet on the ground, while a wizard with -2 luck may be missing their valuable amulet.");
	strcpy(thisMainMenu->newGame->descriptionDone, "Move on to creating your characters abilities.");
	strcpy(thisMainMenu->newGame->descriptionHP, "The character's hit points. If their hit points reach zero, they die.\n\nTotal HP = HP dice spread + CON x 2");
	strcpy(thisMainMenu->newGame->descriptionMana, "The character's mana points. Allows them to use abilities.\n\nTotal Mana = Mana dice spread + WILL x 2");
	strcpy(thisMainMenu->newGame->descriptionFood, "How full the character can be. If their food hits 0, they die.\n\nIf their food is greater than 300, they get a bonus to STR, DEX, and CON. If their food is less than 50, they get a penalty to STR, DEX, and CON.");
	strcpy(thisMainMenu->newGame->descriptionAttack, "The character's attack. The higher the attack, the more likely they will hit their target.\n\nWhen attacking, a d20 is rolled. If the roll plus attack is greater or equal to the targets AC, the attack hits. Rolling a 20 automatically hits, while rolling a 1 always misses.");
	strcpy(thisMainMenu->newGame->descriptionAC, "The character's armor class. The higher the AC, the more likely damage will be avoided entirely.\n\nAC = 10 + DEX");
	strcpy(thisMainMenu->newGame->descriptionDam, "The character's damage output. When an attack connects, the target will receive a dice roll + STR damage.\n\nWeapons will change this damage output, some of which do 0.5, 1.0, 1.5, 2.0 x STR.");
	strcpy(thisMainMenu->newGame->descriptionDR, "The character's physical and magical damage resistance. The damage taken is equal to the total damage minus DR. Physical DR includes the types blunt, chop, slash and pierce, while magical DR can be earth, fire, water, and lightning.\n\nPhysical DR = CON / 2, Magical DR = INT / 2 (Rounded down).");
	strcpy(thisMainMenu->newGame->descriptionCrit, "The character's critical strike.\n\nMAX always returns the highest dice roll, while DUB doubles the dice roll.");
	strcpy(thisMainMenu->newGame->descriptionMvmt, "The number of spaces the character can move in one turn.");
	strcpy(thisMainMenu->newGame->descriptionNumAbilities, "The number of abilities the character will start with.");
	strcpy(thisMainMenu->newGame->descriptionFateTokens, "The number of fate tokens the character starts with.\n\nFate tokens can be used in a variety of situations to change the state of the game, such as re-rolling a bad roll or making a door unlock without a key.");
	strcpy(thisMainMenu->newGame->descriptionBonusMana, "Bonus mana points can be added to abilities to make them more powerful or cheaper to cast. These can be spent in the ability creation menu.");
	strcpy(thisMainMenu->newGame->descriptionClassAbility, "This permanent ability will be the class of your character. Its effects will always be active.");
	strcpy(thisMainMenu->newGame->descriptionAbilityDone, "Finalize character creation.");
	strcpy(thisMainMenu->newGame->descriptionAbility, "Create or edit an ability. Abilities can be used for a variety of purposes, from slaying your enemies, healing yourself, and giving your party temporary stat boosts. Allabilities cost at least one mana.");

	thisMainMenu->load->mode = LOAD_MODE;
	thisMainMenu->load->numSaveData = 10;
	thisMainMenu->load->selectedData = 0;
	thisMainMenu->load->scrollCount = 0;
	thisMainMenu->load->savesPerScreen = 4;
	thisMainMenu->load->readyToLoad = 0;
	thisMainMenu->load->readyToSave = 0;
	thisMainMenu->load->inGameLoadMode = 0;
	thisMainMenu->load->inConfirmationMode = 0;

	thisMainMenu->load->loadView = createCharacter(1418, RGB(255,0,255), 0, 0);
	thisMainMenu->load->loadSelect = createCharacter(1419, RGB(255,0,255), 0, 0);
	thisMainMenu->load->scrollUpArrow = createCharacter(1505, RGB(255,0,255), 0, 0);
	thisMainMenu->load->scrollDownArrow = createCharacter(1507, RGB(255,0,255), 0, 0);

	strcpy(thisMainMenu->rootMapDirectory, mapDirectory);

	setUpSaveLoadData();
}

void reinitializeMainMenu(){
	int i;
	thisMainMenu->title->selectedOption = TITLE_NEW_GAME;
	thisMainMenu->rightSelectArrow->y = 13;

	thisMainMenu->newGame->currentForm = NEW_GAME_CREATE;
	thisMainMenu->newGame->currentField = CREATE_NAME;
	thisMainMenu->newGame->currentSpread = SPREAD_8_8;
	thisMainMenu->newGame->inEditMode = 0;
	thisMainMenu->newGame->inAbilityEditMode = 0;
	thisMainMenu->newGame->statPoints = 6;
	thisMainMenu->newGame->numAbilites = 2;
	cloneNewGameIndividual();
	thisMainMenu->newGame->newAbilities->numAbilities = 0;
	thisMainMenu->newGame->newAbilities->MAX_ABILITIES = 6;
	thisMainMenu->newGame->selectedAbility = 0;
	thisMainMenu->newGame->currentAvatar = rand() % 4;

	for(i = 0; i < thisMainMenu->newGame->newAbilities->MAX_ABILITIES; i++){
		thisMainMenu->newGame->newAbilities->abilitiesList[i] = NULL;
	}

	for(i = 0; i < 4; i++){
		free(thisMainMenu->newGame->avatars[i]);
	}

	setUpAvatars();

	thisMainMenu->load->selectedData = 0;
	thisMainMenu->load->scrollCount = 0;

	setUpSaveLoadData();
}

void disableMainMenuMode(){
	thisMainMenu->inMenuMode = 0;
	thisMainMenu->load->readyToLoad = 0;
	thisMainMenu->load->readyToSave = 0;
}

void enableMainMenuMode(int mode, int reloadBaseGame){
	switch(mode){
		case 0:
			thisMainMenu->currentMenu = MENU_TITLE;
			break;
		case 1:
			thisMainMenu->currentMenu = MENU_NEW_GAME;
			break;
		case 2:
			thisMainMenu->currentMenu = MENU_NEW_GAME_ABILITY;
			break;
		case 3:
			thisMainMenu->currentMenu = MENU_LOAD;
			break;
	}

	thisMainMenu->reloadBaseGame = reloadBaseGame;

	if(!reloadBaseGame){
		reinitializeMainMenu();
	}

	thisMainMenu->inMenuMode = 1;
}

int inMainMenuMode(){
	if(thisMainMenu != NULL){
		return thisMainMenu->inMenuMode;
	}

	return 0;
}

void disableMainMenuReloadBaseGame(){
	thisMainMenu->reloadBaseGame = 0;
}

int mainMenuReloadBaseGame(){
	return thisMainMenu->reloadBaseGame;
}

int mainMenuReadyToLoad(){
	return thisMainMenu->load->readyToLoad;
}

int mainMenuReadyToSave(){
	return thisMainMenu->load->readyToSave;
}

void disableMainMenuWaitForNameMode(){
	thisMainMenu->waitingForNameMode = 0;
}

void enableMainMenuWaitForNameMode(){
	thisMainMenu->waitingForNameMode = 1;
}

int inMainMenuWaitForNameMode(){
	if(thisMainMenu != NULL){
		return thisMainMenu->waitingForNameMode;
	}

	return 0;
}

void mainMenuTitleUp(){
	if(!thisMainMenu->title->titleOptionsDisplayed){
		return;
	}

	switch(thisMainMenu->title->selectedOption){
	case TITLE_NEW_GAME:
		thisMainMenu->title->selectedOption = TITLE_EXIT;
		thisMainMenu->rightSelectArrow->y = 91;
		break;
	case TITLE_LOAD:
		thisMainMenu->title->selectedOption = TITLE_NEW_GAME;
		thisMainMenu->rightSelectArrow->y = 13;
		break;
	case TITLE_EXIT:
		thisMainMenu->title->selectedOption = TITLE_LOAD;
		thisMainMenu->rightSelectArrow->y = 52;
		break;
	}
}

void mainMenuTitleDown(){
	if(!thisMainMenu->title->titleOptionsDisplayed){
		return;
	}

	switch(thisMainMenu->title->selectedOption){
	case TITLE_NEW_GAME:
		thisMainMenu->title->selectedOption = TITLE_LOAD;
		thisMainMenu->rightSelectArrow->y = 52;
		break;
	case TITLE_LOAD:
		thisMainMenu->title->selectedOption = TITLE_EXIT;
		thisMainMenu->rightSelectArrow->y = 91;
		break;
	case TITLE_EXIT:
		thisMainMenu->title->selectedOption = TITLE_NEW_GAME;
		thisMainMenu->rightSelectArrow->y = 13;
		break;
	}
}

void mainMenuTitleSelect(){
	if(thisMainMenu->title->titleOptionsDisplayed){
		switch(thisMainMenu->title->selectedOption){
		case TITLE_NEW_GAME:
			thisMainMenu->currentMenu = MENU_NEW_GAME;
			break;
		case TITLE_LOAD:
			thisMainMenu->currentMenu = MENU_LOAD;
			thisMainMenu->load->mode = LOAD_MODE;
			break;
		case TITLE_EXIT:
			PostQuitMessage(0);
			break;
		}
	}else{
		thisMainMenu->title->titleOptionsDisplayed = 1;
	}
}

void drawMainMenuTitle(HDC hdc, HDC hdcBuffer, RECT * rect){
	int xOff = 0, yOff = 0;

	xOff = max((rect->right / 2) - (thisMainMenu->title->titleScreen->fixedWidth / 2) , 0);
	yOff = max((rect->bottom / 2) - (thisMainMenu->title->titleScreen->fixedHeight / 2) , 0);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisMainMenu->title->titleScreen);

	if(thisMainMenu->title->titleOptionsDisplayed){
		xOff += 90;
		yOff += 375;

		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisMainMenu->title->options);

		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + thisMainMenu->rightSelectArrow->x - 9, yOff + thisMainMenu->rightSelectArrow->y, thisMainMenu->rightSelectArrow);
	}
}

void loadMenuMoveVertical(int goingUp){
	if(goingUp){
		if(thisMainMenu->load->selectedData == 0){
			return;
		}

		thisMainMenu->load->selectedData--;

		if(thisMainMenu->load->scrollCount > 0 && thisMainMenu->load->scrollCount > thisMainMenu->load->selectedData){
			thisMainMenu->load->scrollCount--;
		}
	}else{
		if(thisMainMenu->load->selectedData + 1 == thisMainMenu->load->numSaveData){
			return;
		}

		thisMainMenu->load->selectedData++;

		if(thisMainMenu->load->scrollCount < thisMainMenu->load->numSaveData - thisMainMenu->load->savesPerScreen
				&& thisMainMenu->load->scrollCount + thisMainMenu->load->savesPerScreen <= thisMainMenu->load->selectedData){
			thisMainMenu->load->scrollCount++;
		}
	}
}

void newGameAbilityMenuMoveVertical(int goingUp){
	if(thisMainMenu->newGame->inAbilityEditMode){
		return;
	}

	switch(thisMainMenu->newGame->currentField){
	case CREATE_ABILITY_SELECT:
		if(goingUp){
			if(thisMainMenu->newGame->selectedAbility - 1 >= 0){
				thisMainMenu->newGame->selectedAbility--;
			}
		}else{
			if(thisMainMenu->newGame->selectedAbility + 1 == thisMainMenu->newGame->newAbilities->MAX_ABILITIES){
				thisMainMenu->newGame->currentField = CREATE_NUM_ABILITIES;
			}else{
				thisMainMenu->newGame->selectedAbility++;
			}
		}
		break;
	case CREATE_NUM_ABILITIES:
		if(goingUp){
			thisMainMenu->newGame->selectedAbility = thisMainMenu->newGame->newAbilities->MAX_ABILITIES - 1;
			thisMainMenu->newGame->currentField = CREATE_ABILITY_SELECT;
		}else{
			thisMainMenu->newGame->currentField = CREATE_ABILITY_BONUS_MANA;
		}
		break;
	case CREATE_ABILITY_BONUS_MANA:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_NUM_ABILITIES : CREATE_DONE);
		break;
	case CREATE_DONE:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_ABILITY_BONUS_MANA : CREATE_DONE);
		break;
	}
}

void newGameCreateMenuMoveVertical(int goingUp){
	if(thisMainMenu->newGame->inEditMode){
		return;
	}

	switch(thisMainMenu->newGame->currentField){
	case CREATE_NAME:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_NAME : CREATE_SPREAD);
		break;
	case CREATE_AVATAR:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_AVATAR : CREATE_HP);
		break;
	case CREATE_SPREAD:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_NAME : CREATE_STR);
		break;
	case CREATE_STR:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_SPREAD : CREATE_DEX);
		break;
	case CREATE_HP:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_AVATAR : CREATE_MANA);
		break;
	case CREATE_DEX:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_STR : CREATE_CON);
		break;
	case CREATE_MANA:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_HP : CREATE_FOOD);
		break;
	case CREATE_CON:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_DEX : CREATE_INT);
		break;
	case CREATE_FOOD:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_MANA : CREATE_ATTACK);
		break;
	case CREATE_INT:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_CON : CREATE_WIS);
		break;
	case CREATE_ATTACK:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_FOOD : CREATE_AC);
		break;
	case CREATE_WIS:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_INT : CREATE_WILL);
		break;
	case CREATE_AC:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_ATTACK : CREATE_DAM);
		break;
	case CREATE_WILL:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_WIS : CREATE_CHR);
		break;
	case CREATE_DAM:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_AC : CREATE_DR);
		break;
	case CREATE_DR:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_DAM : CREATE_CRIT);
		break;
	case CREATE_CHR:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_WILL : CREATE_LUCK);
		break;
	case CREATE_CRIT:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_DR : CREATE_MVMT);
		break;
	case CREATE_LUCK:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_CHR : CREATE_DONE);
		break;
	case CREATE_MVMT:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_CRIT : CREATE_NUM_ABILITIES);
		break;
	case CREATE_NUM_ABILITIES:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_MVMT : CREATE_FATE_TOKENS);
		break;
	case CREATE_DONE:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_LUCK : CREATE_DONE);
		break;
	case CREATE_FATE_TOKENS:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_NUM_ABILITIES : CREATE_FATE_TOKENS);
		break;
	}
}

void newGameCreateMenuMoveHorizontal(int goingLeft){
	if(thisMainMenu->newGame->inEditMode){
		return;
	}

	switch(thisMainMenu->newGame->currentField){
	case CREATE_NAME:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_NAME : CREATE_AVATAR);
		break;
	case CREATE_AVATAR:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_NAME : CREATE_AVATAR);
		break;
	case CREATE_SPREAD:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_SPREAD : CREATE_AVATAR);
		break;
	case CREATE_STR:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_STR : CREATE_MANA);
		break;
	case CREATE_HP:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_STR : CREATE_HP);
		break;
	case CREATE_DEX:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_DEX : CREATE_FOOD);
		break;
	case CREATE_MANA:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_STR : CREATE_MANA);
		break;
	case CREATE_CON:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_DEX : CREATE_FOOD);
		break;
	case CREATE_FOOD:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_CON : CREATE_ATTACK);
		break;
	case CREATE_INT:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_INT : CREATE_AC);
		break;
	case CREATE_ATTACK:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_CON : CREATE_ATTACK);
		break;
	case CREATE_WIS:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_WIS : CREATE_DAM);
		break;
	case CREATE_AC:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_INT : CREATE_AC);
		break;
	case CREATE_WILL:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_WILL : CREATE_DR);
		break;
	case CREATE_DAM:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_WIS : CREATE_DAM);
		break;
	case CREATE_DR:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_WILL : CREATE_DR);
		break;
	case CREATE_CHR:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_CHR : CREATE_CRIT);
		break;
	case CREATE_CRIT:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_CHR : CREATE_CRIT);
		break;
	case CREATE_LUCK:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_LUCK : CREATE_MVMT);
		break;
	case CREATE_MVMT:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_LUCK : CREATE_MVMT);
		break;
	case CREATE_NUM_ABILITIES:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_DONE : CREATE_NUM_ABILITIES);
		break;
	case CREATE_DONE:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_DONE : CREATE_NUM_ABILITIES);
		break;
	case CREATE_FATE_TOKENS:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_DONE : CREATE_FATE_TOKENS);
		break;
	}
}

void newGameMenuIncreaseField(){
	if(thisMainMenu->newGame->statPoints <= 0){
		return;
	}

	individual * newPlayer = thisMainMenu->newGame->newPlayer;

	switch(thisMainMenu->newGame->currentField){
	case CREATE_SPREAD:
		switch(thisMainMenu->newGame->currentSpread){
		case SPREAD_12_4:
			thisMainMenu->newGame->currentSpread = SPREAD_10_6;
			newPlayer->baseHP = 10;
			newPlayer->baseMana = 6;
			break;
		case SPREAD_10_6:
			thisMainMenu->newGame->currentSpread = SPREAD_8_8;
			newPlayer->baseHP = 8;
			newPlayer->baseMana = 8;
			break;
		case SPREAD_8_8:
			thisMainMenu->newGame->currentSpread = SPREAD_6_10;
			newPlayer->baseHP = 6;
			newPlayer->baseMana = 10;
			break;
		case SPREAD_6_10:
			if(newPlayer->hp <= 2){
				break;
			}

			thisMainMenu->newGame->currentSpread = SPREAD_4_12;
			newPlayer->baseHP = 4;
			newPlayer->baseMana = 12;
			break;
		case SPREAD_4_12:
			break;
		}

		newPlayer->hp = newPlayer->baseHP + (newPlayer->CON * 2);
		newPlayer->mana = newPlayer->baseMana + (newPlayer->WILL * 2);
		break;
	case CREATE_AVATAR:
		if(thisMainMenu->newGame->currentAvatar + 1 == thisMainMenu->newGame->numAvatars){
			thisMainMenu->newGame->currentAvatar = 0;
		}else{
			thisMainMenu->newGame->currentAvatar++;
		}
		break;
	case CREATE_STR:
		if(newPlayer->STR < 3){
			newPlayer->STR++;
			thisMainMenu->newGame->statPoints--;
		}
		break;
	case CREATE_DEX:
		if(newPlayer->DEX < 3){
			newPlayer->DEX++;
			newPlayer->mvmt = 3 + (int) (newPlayer->DEX / 2);
			newPlayer->AC++;
			thisMainMenu->newGame->statPoints--;
		}
		break;
	case CREATE_CON:
		if(newPlayer->CON < 3){
			newPlayer->CON++;
			newPlayer->totalFood += 50;
			newPlayer->hp = newPlayer->baseHP + newPlayer->CON * 2;
			thisMainMenu->newGame->statPoints--;
		}
		break;
	case CREATE_WIS:
		if(newPlayer->WIS < 3){
			newPlayer->WIS++;
			newPlayer->attack++;
			thisMainMenu->newGame->numAbilites += 1;
			thisMainMenu->newGame->statPoints--;
		}
		break;
	case CREATE_WILL:
		if(newPlayer->WILL < 3){
			newPlayer->WILL++;
			newPlayer->mana = newPlayer->baseMana + newPlayer->WILL * 2;
			thisMainMenu->newGame->statPoints--;
		}
		break;
	case CREATE_INT:
		if(newPlayer->INT < 3){
			newPlayer->INT++;
			thisMainMenu->newGame->statPoints--;
		}
		break;
	case CREATE_CHR:
		if(newPlayer->CHR < 3){
			newPlayer->CHR++;
			newPlayer->fateTokens++;
			thisMainMenu->newGame->statPoints--;
		}
		break;
	case CREATE_LUCK:
		if(newPlayer->LUCK < 3){
			newPlayer->LUCK++;
			thisMainMenu->newGame->statPoints--;
		}
		break;
	}
}

void newGameCreateMenuDecreaseField(){
	individual * newPlayer = thisMainMenu->newGame->newPlayer;

	switch(thisMainMenu->newGame->currentField){
	case CREATE_SPREAD:
		switch(thisMainMenu->newGame->currentSpread){
		case SPREAD_12_4:
			break;
		case SPREAD_10_6:
			thisMainMenu->newGame->currentSpread = SPREAD_12_4;
			newPlayer->baseHP = 12;
			newPlayer->baseMana = 4;
			break;
		case SPREAD_8_8:
			thisMainMenu->newGame->currentSpread = SPREAD_10_6;
			newPlayer->baseHP = 10;
			newPlayer->baseMana = 6;
			break;
		case SPREAD_6_10:
			thisMainMenu->newGame->currentSpread = SPREAD_8_8;
			newPlayer->baseHP = 8;
			newPlayer->baseMana = 8;
			break;
		case SPREAD_4_12:
			thisMainMenu->newGame->currentSpread = SPREAD_6_10;
			newPlayer->baseHP = 6;
			newPlayer->baseMana = 10;
			break;
		}

		newPlayer->hp = newPlayer->baseHP + newPlayer->CON * 2;
		newPlayer->mana = newPlayer->baseMana + newPlayer->WILL * 2;
		break;
	case CREATE_AVATAR:
		if(thisMainMenu->newGame->currentAvatar - 1 < 0){
			thisMainMenu->newGame->currentAvatar = thisMainMenu->newGame->numAvatars - 1;
		}else{
			thisMainMenu->newGame->currentAvatar--;
		}
		break;
	case CREATE_STR:
		if(newPlayer->STR > -2){
			newPlayer->STR--;
			thisMainMenu->newGame->statPoints++;
		}
		break;
	case CREATE_DEX:
		if(newPlayer->DEX > -2){
			newPlayer->DEX--;
			newPlayer->mvmt = 3 + (int) (newPlayer->DEX / 2);
			newPlayer->AC--;
			thisMainMenu->newGame->statPoints++;
		}
		break;
	case CREATE_CON:
		if(newPlayer->CON > -2 && newPlayer->hp > 2){
			newPlayer->CON--;
			newPlayer->totalFood -= 50;
			newPlayer->hp = newPlayer->baseHP + newPlayer->CON * 2;
			thisMainMenu->newGame->statPoints++;
		}
		break;
	case CREATE_WIS:
		if(newPlayer->WIS > -2){
			newPlayer->WIS--;
			newPlayer->attack--;
			thisMainMenu->newGame->numAbilites -= 1;
			thisMainMenu->newGame->statPoints++;
		}
		break;
	case CREATE_WILL:
		if(newPlayer->WILL > -2){
			newPlayer->WILL--;
			newPlayer->mana = newPlayer->baseMana + newPlayer->WILL * 2;
			thisMainMenu->newGame->statPoints++;
		}
		break;
	case CREATE_INT:
		if(newPlayer->INT > -2){
			newPlayer->INT--;
			thisMainMenu->newGame->statPoints++;
		}
		break;
	case CREATE_CHR:
		if(newPlayer->CHR > -2){
			newPlayer->CHR--;
			newPlayer->fateTokens--;
			thisMainMenu->newGame->statPoints++;
		}
		break;
	case CREATE_LUCK:
		if(newPlayer->LUCK > -2){
			newPlayer->LUCK--;
			thisMainMenu->newGame->statPoints++;
		}
		break;
	}
}

void newGameCreateMenuInterpretLeft(){
	if(thisMainMenu->newGame->inEditMode){
		newGameCreateMenuDecreaseField();
	}else{
		newGameCreateMenuMoveHorizontal(1);
	}
}

void newGameCreateMenuInterpretRight(){
	if(thisMainMenu->newGame->inEditMode){
		newGameMenuIncreaseField();
	}else{
		newGameCreateMenuMoveHorizontal(0);
	}
}

void disableNewGameAbilityEditMode(){
	thisMainMenu->newGame->inAbilityEditMode = 0;
}

void addAbilityToNewGameAbilityMode(ability * newAbility){
	ability * tmpOldAbility = thisMainMenu->newGame->newAbilities->abilitiesList[thisMainMenu->newGame->selectedAbility];

	thisMainMenu->newGame->newAbilities->abilitiesList[thisMainMenu->newGame->selectedAbility] = newAbility;

	if(tmpOldAbility != NULL){
		free(tmpOldAbility);
	}

	thisMainMenu->newGame->bonusMana -= getSpentBonusMana();

	thisMainMenu->newGame->inAbilityEditMode = 0;
}

void loadMenuInterpretHorizontal(int goingRight){
	if(thisMainMenu->load->inConfirmationMode){
		if(thisMainMenu->selectedDecision == CONFIRMATION_YES){
			thisMainMenu->selectedDecision = CONFIRMATION_NO;
		}else{
			thisMainMenu->selectedDecision = CONFIRMATION_YES;
		}
	}
}

void loadMenuInterpretEnter(){
	if(thisMainMenu->load->mode == LOAD_MODE && thisMainMenu->load->saves[thisMainMenu->load->selectedData] != NULL){
		if(thisMainMenu->load->inGameLoadMode){
			if(thisMainMenu->load->inConfirmationMode){
				if(thisMainMenu->selectedDecision == CONFIRMATION_YES){
					thisMainMenu->load->readyToLoad = 1;
				}

				thisMainMenu->load->inConfirmationMode = 0;
			}else{
				thisMainMenu->selectedDecision = CONFIRMATION_NO;
				thisMainMenu->load->inConfirmationMode = 1;
			}
		}else{
			thisMainMenu->selectedDecision = CONFIRMATION_NO;
			thisMainMenu->load->readyToLoad = 1;
		}
	}else if(thisMainMenu->load->mode == SAVE_MODE){
		if(thisMainMenu->load->saves[thisMainMenu->load->selectedData] != NULL){
			if(thisMainMenu->load->inConfirmationMode){
				if(thisMainMenu->selectedDecision == CONFIRMATION_YES){
					thisMainMenu->load->readyToSave = 1;
				}

				thisMainMenu->load->inConfirmationMode = 0;
			}else{
				thisMainMenu->selectedDecision = CONFIRMATION_NO;
				thisMainMenu->load->inConfirmationMode = 1;
			}
		}else{
			thisMainMenu->selectedDecision = CONFIRMATION_NO;
			thisMainMenu->load->readyToSave = 1;
		}
	}

}

void loadMenuInterpretEscape(){
	if(thisMainMenu->load->mode == LOAD_MODE){
		if(thisMainMenu->load->inGameLoadMode){
			if(thisMainMenu->load->inConfirmationMode){
				thisMainMenu->load->inConfirmationMode = 0;
			}else{
				disableMainMenuMode();
			}
		}else{
			thisMainMenu->currentMenu = MENU_TITLE;
			thisMainMenu->load->selectedData = 0;
			thisMainMenu->load->scrollCount = 0;
		}
	}else if(thisMainMenu->load->mode == SAVE_MODE){
		if(thisMainMenu->load->inConfirmationMode){
			thisMainMenu->load->inConfirmationMode = 0;
		}else{
			disableMainMenuMode();
		}
	}
}

void newGameAbilityMenuInterpretEnter(){
	switch(thisMainMenu->newGame->currentField){
		case CREATE_ABILITY_SELECT:
			if(thisMainMenu->newGame->selectedAbility == 0){
				enableAbilityCreateMode(0, ABILITY_CREATE_PERMENANT_ONLY, DEFAULT_ABILITY);
			}else{
				enableAbilityCreateMode(thisMainMenu->newGame->bonusMana, ABILITY_CREATE_EXCEPT_PERMENANT, DEFAULT_ABILITY);
			}

			thisMainMenu->newGame->inAbilityEditMode = 1;
		break;
		case CREATE_DONE:{
			int i;

			thisMainMenu->newGame->newPlayer->playerCharacter->thisAnimationContainer = cloneAnimationContainer(thisMainMenu->newGame->avatars[thisMainMenu->newGame->currentAvatar]->thisAnimationContainer);
			thisMainMenu->newGame->newPlayer->playerCharacter->secondaryAnimationContainer = cloneAnimationContainer(thisMainMenu->newGame->avatars[thisMainMenu->newGame->currentAvatar]->secondaryAnimationContainer);
			thisMainMenu->newGame->newPlayer->playerCharacter->x = thisMainMenu->newGame->avatars[thisMainMenu->newGame->currentAvatar]->x;
			thisMainMenu->newGame->newPlayer->playerCharacter->y = thisMainMenu->newGame->avatars[thisMainMenu->newGame->currentAvatar]->y;
			thisMainMenu->newGame->newPlayer->playerCharacter->xOff = 0;
			thisMainMenu->newGame->newPlayer->playerCharacter->yOff = 0;

			for(i = 0; i < thisMainMenu->newGame->newAbilities->MAX_ABILITIES; i++){
				if(thisMainMenu->newGame->newAbilities->abilitiesList[i] != NULL){
					addAbilityToIndividual(thisMainMenu->newGame->newPlayer, thisMainMenu->newGame->newAbilities->abilitiesList[i]);
				}
			}

			thisMainMenu->newGame->newPlayer->ID = 1;
			setPlayer(thisMainMenu->newGame->newPlayer);
			//TODO requres lock if I actually want to destory the other individual
			replaceIndividualInRegistry(thisMainMenu->newGame->newPlayer, 1);
			disableMainMenuMode();
			break;
		}
	}
}

void newGameCreateMenuInterpretEnter(){
	switch(thisMainMenu->newGame->currentField){
	case CREATE_NAME:
		toggleNameMode();
		enableMainMenuWaitForNameMode();
		break;
	case CREATE_SPREAD:
	case CREATE_AVATAR:
	case CREATE_STR:
	case CREATE_DEX:
	case CREATE_CON:
	case CREATE_WIS:
	case CREATE_WILL:
	case CREATE_INT:
	case CREATE_CHR:
	case CREATE_LUCK:
		thisMainMenu->newGame->inEditMode = (thisMainMenu->newGame->inEditMode + 1) % 2;
		break;
	case CREATE_DONE:
		switch(thisMainMenu->newGame->currentForm){
			case NEW_GAME_CREATE:
				if(thisMainMenu->newGame->newPlayer->hp < 1){
					break;
				}

				thisMainMenu->newGame->currentForm = NEW_GAME_ABILITIES;
				thisMainMenu->newGame->currentField = CREATE_ABILITY_SELECT;
				thisMainMenu->newGame->selectedAbility = 0;
				thisMainMenu->newGame->newAbilities->MAX_ABILITIES = thisMainMenu->newGame->numAbilites + 1;
				thisMainMenu->newGame->bonusMana = thisMainMenu->newGame->newPlayer->INT;

				if(thisMainMenu->newGame->bonusMana < 0){
					thisMainMenu->newGame->bonusMana = 0;
				}

				break;
			case NEW_GAME_ABILITIES:
			case NEW_GAME_FINALIZE:{
				character * tmpCharacter;

				tmpCharacter = thisMainMenu->newGame->newPlayer->playerCharacter;
				thisMainMenu->newGame->newPlayer->playerCharacter = thisMainMenu->newGame->avatars[thisMainMenu->newGame->currentAvatar];
				destroyCharacter(tmpCharacter);

				thisMainMenu->newGame->newPlayer->ID = 1;
				setPlayer(thisMainMenu->newGame->newPlayer);
				//TODO requres lock if I actually want to destory the other individual
				replaceIndividualInRegistry(thisMainMenu->newGame->newPlayer, 1);
				disableMainMenuMode();
				break;
			}
		}

		break;
	}
}

void MainMenuSetName(char * newName){
	switch(thisMainMenu->currentMenu){
		case MENU_NEW_GAME:
		strcpy(thisMainMenu->newGame->newPlayer->name, newName);
	}
}

int getMainMenuLoadSlot(){
	return thisMainMenu->load->selectedData;
}

void showSaveMenu(){
	thisMainMenu->load->mode = SAVE_MODE;
	thisMainMenu->load->inGameLoadMode = 1;
	enableMainMenuMode(3, 0);
}

void setupLoadMode(int inGameMode){
	thisMainMenu->load->mode = LOAD_MODE;
	thisMainMenu->load->inGameLoadMode = inGameMode;
}

void newGameResetPlayer(){
	thisMainMenu->newGame->newPlayer->name[0] = '\0';
	thisMainMenu->newGame->currentSpread = SPREAD_8_8;
	thisMainMenu->newGame->newPlayer->baseHP = 8;
	thisMainMenu->newGame->newPlayer->baseMana = 8;
	thisMainMenu->newGame->newPlayer->hp = 8;
	thisMainMenu->newGame->newPlayer->mana = 8;
	thisMainMenu->newGame->newPlayer->STR = 0;
	thisMainMenu->newGame->newPlayer->DEX = 0;
	thisMainMenu->newGame->newPlayer->CON = 0;
	thisMainMenu->newGame->newPlayer->WIS = 0;
	thisMainMenu->newGame->newPlayer->WILL = 0;
	thisMainMenu->newGame->newPlayer->INT = 0;
	thisMainMenu->newGame->newPlayer->CHR = 0;
	thisMainMenu->newGame->newPlayer->LUCK = 0;
	strcpy(thisMainMenu->newGame->newPlayer->critType, "MAX");
}

void newGameCreateMenuInterpretEscape(){
	if(thisMainMenu->newGame->inEditMode){
		thisMainMenu->newGame->inEditMode = 0;
	}else{
		thisMainMenu->currentMenu = MENU_TITLE;
		thisMainMenu->newGame->currentField = CREATE_NAME;
		newGameResetPlayer();
		thisMainMenu->newGame->statPoints = 6;
	}
}

void newGameAbilitiesMenuInterpretEscape(){
	if(thisMainMenu->newGame->inAbilityEditMode){
		thisMainMenu->newGame->inAbilityEditMode = 0;
	}else{
		thisMainMenu->newGame->currentField = CREATE_NAME;
		thisMainMenu->newGame->currentForm = NEW_GAME_CREATE;
	}
}

void mainMenuInterpretRight(){
	switch(thisMainMenu->currentMenu){
	case MENU_NEW_GAME:
		newGameCreateMenuInterpretRight();
		break;
	case MENU_LOAD:
		loadMenuInterpretHorizontal(1);
		break;
	}
}

void mainMenuInterpretLeft(){
	switch(thisMainMenu->currentMenu){
	case MENU_NEW_GAME:
		newGameCreateMenuInterpretLeft();
		break;
	case MENU_LOAD:
		loadMenuInterpretHorizontal(0);
		break;
	}
}

void mainMenuInterpretUp(){
	switch(thisMainMenu->currentMenu){
	case MENU_TITLE:
		mainMenuTitleUp();
		break;
	case MENU_NEW_GAME:
		switch(thisMainMenu->newGame->currentForm){
			case NEW_GAME_CREATE:
				newGameCreateMenuMoveVertical(1);
				break;
			case NEW_GAME_ABILITIES:
				newGameAbilityMenuMoveVertical(1);
				break;
		}
		break;
	case MENU_LOAD:
		loadMenuMoveVertical(1);
		break;
	}
}

void mainMenuInterpretDown(){
	switch(thisMainMenu->currentMenu){
	case MENU_TITLE:
		mainMenuTitleDown();
		break;
	case MENU_NEW_GAME:
		switch(thisMainMenu->newGame->currentForm){
			case NEW_GAME_CREATE:
				newGameCreateMenuMoveVertical(0);
				break;
			case NEW_GAME_ABILITIES:
				newGameAbilityMenuMoveVertical(0);
				break;
		}
		break;
	case MENU_LOAD:
		loadMenuMoveVertical(0);
		break;
	}
}

void mainMenuInterpretEnter(){
	switch(thisMainMenu->currentMenu){
	case MENU_TITLE:
		mainMenuTitleSelect();
		break;
	case MENU_NEW_GAME:
		switch(thisMainMenu->newGame->currentForm){
		case NEW_GAME_CREATE:
			newGameCreateMenuInterpretEnter();
			break;
		case NEW_GAME_ABILITIES:
			newGameAbilityMenuInterpretEnter();
			break;
		}
		break;
	case MENU_LOAD:
		loadMenuInterpretEnter();
		break;
	}

}

void mainMenuInterpretEscape(){
	switch(thisMainMenu->currentMenu){
	case MENU_TITLE:
		thisMainMenu->title->titleOptionsDisplayed = 0;
		break;
	case MENU_NEW_GAME:
		switch(thisMainMenu->newGame->currentForm){
			case NEW_GAME_CREATE:
				newGameCreateMenuInterpretEscape();
				break;
			case NEW_GAME_ABILITIES:
				newGameAbilitiesMenuInterpretEscape();
				break;
		}
		break;
	case MENU_LOAD:
		loadMenuInterpretEscape();
		break;
	}
}

void drawNewGameFormNumber(HDC hdcBuffer, RECT * rect, int startX, int startY, int number){
	char outStr[16];
	SIZE size;

	sprintf(outStr, "%d", number);

	GetTextExtentPoint32(hdcBuffer, outStr, strlen(outStr), &size);

	rect->top = startY;
	rect->left = startX - (size.cx / 2);
	rect->bottom = rect->top + 30;
	rect->right = rect->left + 150;

	DrawText(hdcBuffer, outStr, -1, rect, DT_SINGLELINE);
}

void drawNewGameText(HDC hdcBuffer, RECT * rect, int startX, int startY, char * text){
	rect->top = startY;
	rect->left = startX;
	rect->bottom = rect->top + 30;
	rect->right = rect->left + 150;

	DrawText(hdcBuffer, text, -1, rect, DT_SINGLELINE);
}

void drawNewGameFormText(HDC hdcBuffer, RECT * rect, int startX, int startY, char * text){
	SIZE size;

	GetTextExtentPoint32(hdcBuffer, text, strlen(text), &size);

	rect->top = startY;
	rect->left = startX - (size.cx / 2);
	rect->bottom = rect->top + 30;
	rect->right = rect->left + 150;

	DrawText(hdcBuffer, text, -1, rect, DT_SINGLELINE);
}

void drawDiceSpread(HDC hdcBuffer, RECT * rect, int startX, int startY){
	switch(thisMainMenu->newGame->currentSpread){
		case SPREAD_12_4:
			drawNewGameFormText(hdcBuffer, rect, startX, startY, "12/4");
			break;
		case SPREAD_10_6:
			drawNewGameFormText(hdcBuffer, rect, startX, startY, "10/6");
			break;
		case SPREAD_8_8:
			drawNewGameFormText(hdcBuffer, rect, startX, startY, "8/8");
			break;
		case SPREAD_6_10:
			drawNewGameFormText(hdcBuffer, rect, startX, startY, "6/10");
			break;
		case SPREAD_4_12:
			drawNewGameFormText(hdcBuffer, rect, startX, startY, "4/12");
			break;
	}
}

void drawCreateField(HDC hdc, HDC hdcBuffer, int xCord, int yCord, int type){
	fixedCharacter * outline;

	if(thisMainMenu->newGame->inEditMode){
		switch(type){
			case 1:
				outline = thisMainMenu->newGame->entry1Edit;
				break;
			case 2:
				outline = thisMainMenu->newGame->entry2Edit;
				break;
			case 3:
				outline = thisMainMenu->newGame->entry3Edit;
				break;
			case 4:
				outline = thisMainMenu->newGame->entry4Edit;
				break;
			case 5:
				outline = thisMainMenu->newGame->entry5Edit;
				break;
			case 6:
				outline = thisMainMenu->newGame->entry6Edit;
				break;
		}
	}else{
		switch(type){
			case 1:
				outline = thisMainMenu->newGame->entry1Select;
				break;
			case 2:
				outline = thisMainMenu->newGame->entry2Select;
				break;
			case 3:
				outline = thisMainMenu->newGame->entry3Select;
				break;
			case 4:
				outline = thisMainMenu->newGame->entry4Select;
				break;
			case 5:
				outline = thisMainMenu->newGame->entry5Select;
				break;
			case 6:
				outline = thisMainMenu->newGame->entry6Select;
				break;
		}
	}

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xCord, yCord, outline);
}

void drawCreateDescription(HDC hdcBuffer, char * description){
	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);
	DrawText(hdcBuffer, description, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
}

void drawNewGameAbilitySelectedInfo(HDC hdc, HDC hdcBuffer, int xOff, int yOff){
	switch(thisMainMenu->newGame->currentField){
	case CREATE_ABILITY_SELECT:
		if(thisMainMenu->newGame->selectedAbility == 0){
			drawCreateField(hdc, hdcBuffer, xOff + 40, yOff + 62, 5);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionClassAbility);
		}else{
			drawCreateField(hdc, hdcBuffer, xOff + 40, yOff + 120  + 46 * thisMainMenu->newGame->selectedAbility, 5);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionAbility);
		}
		break;
	case CREATE_NUM_ABILITIES:
		drawCreateField(hdc, hdcBuffer, xOff + 202, yOff + 410, 1);
		drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionNumAbilities);
		break;
	case CREATE_ABILITY_BONUS_MANA:
		drawCreateField(hdc, hdcBuffer,  xOff + 193, yOff + 452, 1);
		drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionBonusMana);
		break;
	case CREATE_DONE:
		drawCreateField(hdc, hdcBuffer, xOff + 100, yOff + 498, 3);
		drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionAbilityDone);
		break;
	}
}

void drawNewGameCreateSelectedInfo(HDC hdc, HDC hdcBuffer, int xOff, int yOff){
	switch(thisMainMenu->newGame->currentField){
		case CREATE_NAME:
			drawCreateField(hdc, hdcBuffer, xOff + 102, yOff + 22, 5);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionName);
			break;
		case CREATE_SPREAD:
			drawCreateField(hdc, hdcBuffer, xOff + 180, yOff + 68, 3);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionSpread);
			break;
		case CREATE_AVATAR:
			drawCreateField(hdc, hdcBuffer, xOff + 296, yOff + 22, 6);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionAvatar);
			break;
		case CREATE_STR:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 172, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionSTR);
			if(thisMainMenu->newGame->inEditMode){
				if(thisMainMenu->newGame->newPlayer->STR > -2){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 134, yOff + 182, thisMainMenu->leftSelectArrow);
				}

				if(thisMainMenu->newGame->newPlayer->STR < 3){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 206, yOff + 182, thisMainMenu->rightSelectArrow);
				}
			}
			break;
		case CREATE_DEX:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 172 + 42, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionDEX);

			if(thisMainMenu->newGame->inEditMode){
				if(thisMainMenu->newGame->newPlayer->DEX > -2){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 134, yOff + 182 + 42, thisMainMenu->leftSelectArrow);
				}

				if(thisMainMenu->newGame->newPlayer->DEX < 3){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 206, yOff + 182 + 42, thisMainMenu->rightSelectArrow);
				}
			}
			break;
		case CREATE_CON:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 172 + 42 * 2, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionCON);

			if(thisMainMenu->newGame->inEditMode){
				if(thisMainMenu->newGame->newPlayer->CON > -2){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 134, yOff + 182 + 42 * 2, thisMainMenu->leftSelectArrow);
				}

				if(thisMainMenu->newGame->newPlayer->CON < 3){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 206, yOff + 182 + 42 * 2, thisMainMenu->rightSelectArrow);
				}
			}
			break;
		case CREATE_INT:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 172 + 42 * 3, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionINT);

			if(thisMainMenu->newGame->inEditMode){
				if(thisMainMenu->newGame->newPlayer->INT > -2){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 134, yOff + 182 + 42 * 3, thisMainMenu->leftSelectArrow);
				}

				if(thisMainMenu->newGame->newPlayer->INT < 3){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 206, yOff + 182 + 42 * 3, thisMainMenu->rightSelectArrow);
				}
			}
			break;
		case CREATE_WIS:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 172 + 42 * 4, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionWIS);

			if(thisMainMenu->newGame->inEditMode){
				if(thisMainMenu->newGame->newPlayer->WIS > -2){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 134, yOff + 182 + 42 * 4, thisMainMenu->leftSelectArrow);
				}

				if(thisMainMenu->newGame->newPlayer->WIS < 3){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 206, yOff + 182 + 42 * 4, thisMainMenu->rightSelectArrow);
				}
			}
			break;
		case CREATE_WILL:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 172 + 42 * 5, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionWILL);

			if(thisMainMenu->newGame->inEditMode){
				if(thisMainMenu->newGame->newPlayer->WILL > -2){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 134, yOff + 182 + 42 * 5, thisMainMenu->leftSelectArrow);
				}

				if(thisMainMenu->newGame->newPlayer->WILL < 3){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 206, yOff + 182 + 42 * 5, thisMainMenu->rightSelectArrow);
				}
			}
			break;
		case CREATE_CHR:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 172 + 42 * 6, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionCHR);

			if(thisMainMenu->newGame->inEditMode){
				if(thisMainMenu->newGame->newPlayer->CHR > -2){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 134, yOff + 182 + 42 * 6, thisMainMenu->leftSelectArrow);
				}

				if(thisMainMenu->newGame->newPlayer->CHR < 3){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 206, yOff + 182 + 42 * 6, thisMainMenu->rightSelectArrow);
				}
			}
			break;
		case CREATE_LUCK:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 172 + 42 * 7, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionLUCK);

			if(thisMainMenu->newGame->inEditMode){
				if(thisMainMenu->newGame->newPlayer->LUCK > -2){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 134, yOff + 182 + 42 * 7, thisMainMenu->leftSelectArrow);
				}

				if(thisMainMenu->newGame->newPlayer->LUCK < 3){
					drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 206, yOff + 182 + 42 * 7, thisMainMenu->rightSelectArrow);
				}
			}
			break;
		case CREATE_DONE:
			drawCreateField(hdc, hdcBuffer, xOff + 80, yOff + 520, 3);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionDone);
			break;
		case CREATE_HP:
			drawCreateField(hdc, hdcBuffer, xOff + 306, yOff + 130, 3);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionHP);
			break;
		case CREATE_MANA:
			drawCreateField(hdc, hdcBuffer, xOff + 338, yOff + 172, 3);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionMana);
			break;
		case CREATE_FOOD:
			drawCreateField(hdc, hdcBuffer, xOff + 404, yOff + 214, 2);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionFood);
			break;
		case CREATE_ATTACK:
			drawCreateField(hdc, hdcBuffer, xOff + 345, yOff + 256, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionAttack);
			break;
		case CREATE_AC:
			drawCreateField(hdc, hdcBuffer, xOff + 302, yOff + 298, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionAC);
			break;
		case CREATE_DAM:
			drawCreateField(hdc, hdcBuffer, xOff + 326, yOff + 340, 4);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionDam);
			break;
		case CREATE_DR:
			drawCreateField(hdc, hdcBuffer, xOff + 307, yOff + 382, 3);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionDR);
			break;
		case CREATE_CRIT:
			drawCreateField(hdc, hdcBuffer, xOff + 311, yOff + 424, 3);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionCrit);
			break;
		case CREATE_MVMT:
			drawCreateField(hdc, hdcBuffer, xOff + 341, yOff + 466, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionMvmt);
			break;
		case CREATE_NUM_ABILITIES:
			drawCreateField(hdc, hdcBuffer, xOff + 441, yOff + 508, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionNumAbilities);
			break;
		case CREATE_FATE_TOKENS:
			drawCreateField(hdc, hdcBuffer, xOff + 426, yOff + 550, 1);
			drawCreateDescription(hdcBuffer, thisMainMenu->newGame->descriptionFateTokens);
			break;
	}
}

void drawNewGameCreateMenu(HDC hdc, HDC hdcBuffer, RECT * rect){
	int xOff = 0, yOff = 0;
	char outStr[16];
	RECT textBoxRect;
	individual * newPlayer = thisMainMenu->newGame->newPlayer;

	xOff = max((rect->right / 2) - (thisMainMenu->newGame->creationScreen->fixedWidth / 2) , 0);
	yOff = max((rect->bottom / 2) - (thisMainMenu->newGame->creationScreen->fixedHeight / 2) , 0);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisMainMenu->newGame->creationScreen);

	drawNewGameCreateSelectedInfo(hdc, hdcBuffer, xOff, yOff);

	drawUnboundAnimationByPixelsStretch(hdc, hdcBuffer, thisMainMenu->newGame->avatars[thisMainMenu->newGame->currentAvatar], xOff + 246, yOff - 29, 200, 200, 0);

	thisMainMenu->newGame->descriptionRect->top = yOff + 602;
	thisMainMenu->newGame->descriptionRect->bottom = yOff + 552 + 180;
	thisMainMenu->newGame->descriptionRect->left = xOff + 13;
	thisMainMenu->newGame->descriptionRect->right = xOff + 10 + 484;

	textBoxRect.top = yOff + 27;
	textBoxRect.left = xOff + 110;
	textBoxRect.bottom = textBoxRect.top + 30;
	textBoxRect.right = textBoxRect.left + 150;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);

	DrawText(hdcBuffer, newPlayer->name, -1, &textBoxRect, DT_SINGLELINE);

	drawDiceSpread(hdcBuffer, &textBoxRect, xOff + 180 + 42, yOff + 68);

	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 185, yOff + 130, thisMainMenu->newGame->statPoints);

	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174, newPlayer->STR);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 42, newPlayer->DEX);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 42 * 2, newPlayer->CON);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 42 * 3, newPlayer->INT);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 42 * 4, newPlayer->WIS);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 42 * 5, newPlayer->WILL);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 42 * 6, newPlayer->CHR);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 42 * 7, newPlayer->LUCK);
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 80 + 42, yOff + 521, "DONE");

	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 404 + 30, yOff + 215, newPlayer->totalFood);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 344 + 22, yOff + 258, newPlayer->attack);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 302 + 22, yOff + 300, newPlayer->AC);

	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 311 + 42, yOff + 426, newPlayer->critType);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 340 + 22, yOff + 468, newPlayer->mvmt);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 440 + 22, yOff + 510, thisMainMenu->newGame->numAbilites);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 426 + 22, yOff + 552, newPlayer->fateTokens);

	sprintf(outStr, "%d/%d", newPlayer->baseHP + newPlayer->CON * 2, newPlayer->baseHP + newPlayer->CON * 2);
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 307 + 42, yOff + 131, outStr);

	sprintf(outStr, "%d/%d", newPlayer->baseMana + newPlayer->WILL * 2, newPlayer->baseMana + newPlayer->WILL * 2);
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 339 + 42, yOff + 173, outStr);

	sprintf(outStr, "%d/%d", (int) newPlayer->CON / 2, (int) newPlayer->INT / 2);
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 307 + 42, yOff + 383, outStr);

	sprintf(outStr, "[%d-%d]", newPlayer->minDam, newPlayer->maxDam);

	if(newPlayer->STR >= 0){
		sprintf(outStr + strlen(outStr), " + %d", newPlayer->STR);
	}else if(newPlayer->STR < 0){
		sprintf(outStr + strlen(outStr), " - %d", abs(newPlayer->STR));
	}

	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 326 + 57, yOff + 341, outStr);

	SelectObject(hdcBuffer, oldFont);
	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
}

void drawNewGameAbilities(HDC hdc, HDC hdcBuffer, RECT * rect){
	int i, xOff = 0, yOff = 0;
	char outStr[16];
	RECT textBoxRect;
	individual * newPlayer = thisMainMenu->newGame->newPlayer;

	xOff = max((rect->right / 2) - (thisMainMenu->newGame->abilityCreationScreen->fixedWidth / 2) , 0);
	yOff = max((rect->bottom / 2) - (thisMainMenu->newGame->abilityCreationScreen->fixedHeight / 2) , 0);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisMainMenu->newGame->abilityCreationScreen);

	drawNewGameAbilitySelectedInfo(hdc, hdcBuffer, xOff, yOff);

	textBoxRect.top = yOff;
	textBoxRect.left = xOff;
	textBoxRect.bottom = textBoxRect.top + 30;
	textBoxRect.right = textBoxRect.left + 150;

	thisMainMenu->newGame->descriptionRect->top = yOff + 558;
	thisMainMenu->newGame->descriptionRect->bottom = yOff + 558 + 95;
	thisMainMenu->newGame->descriptionRect->left = xOff + 13;
	thisMainMenu->newGame->descriptionRect->right = xOff + 10 + 260;

	SetBkMode(hdcBuffer, TRANSPARENT);

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);

	//class ability
	if(thisMainMenu->newGame->newAbilities->abilitiesList[0] == NULL){
		if(thisMainMenu->newGame->selectedAbility != 0){
			drawUnboundCharacterByPixels(hdc, hdcBuffer,  xOff + 40, yOff + 62, thisMainMenu->newGame->entryAddAbility);
		}

		SetTextColor(hdcBuffer, RGB(255, 200, 0));
		drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 40 + 81, yOff + 62, "[Add Ability]");
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
	}else{
		SetTextColor(hdcBuffer, RGB(255, 200, 0));
		drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 40 + 81, yOff + 62, thisMainMenu->newGame->newAbilities->abilitiesList[0]->name);
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
	}

	for(i = 1; i < thisMainMenu->newGame->newAbilities->MAX_ABILITIES; i++){
		if(thisMainMenu->newGame->newAbilities->abilitiesList[i] == NULL){
			if(thisMainMenu->newGame->selectedAbility != i || thisMainMenu->newGame->currentField != CREATE_ABILITY_SELECT){
				drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 40, yOff + 120 + 46 * i, thisMainMenu->newGame->entryAddAbility);
			}

			SetTextColor(hdcBuffer, RGB(255, 200, 0));
			drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 40 + 81, yOff + 120 + 46 * i, "[Add Ability]");
			SetTextColor(hdcBuffer, RGB(0, 0, 0));
		}else{
			SetTextColor(hdcBuffer, RGB(255, 200, 0));
			drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 40 + 81, yOff + 120 + 46 * i, thisMainMenu->newGame->newAbilities->abilitiesList[i]->name);
			SetTextColor(hdcBuffer, RGB(0, 0, 0));
		}
	}

	SetTextColor(hdcBuffer, RGB(255, 200, 0));

	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 202 + 22, yOff + 410, thisMainMenu->newGame->numAbilites);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 193 + 22, yOff + 452, thisMainMenu->newGame->bonusMana);
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 100 + 42, yOff + 500, "DONE");

	SelectObject(hdcBuffer, oldFont);
	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
}

void drawLoadMenu(HDC hdc, HDC hdcBuffer, RECT * rect){
	int i, xOff = 0, yOff = 0;
	int upperBound = min(thisMainMenu->load->savesPerScreen + thisMainMenu->load->scrollCount, thisMainMenu->load->numSaveData);
	char outStr[16];
	RECT textBoxRect;

	xOff = max((rect->right / 2) - (thisMainMenu->load->loadView->fixedWidth / 2) , 0);
	yOff = max((rect->bottom / 2) - (thisMainMenu->load->loadView->fixedHeight / 2) , 0);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisMainMenu->load->loadView);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 36,
			yOff + 56 + 135 * (thisMainMenu->load->selectedData - thisMainMenu->load->scrollCount), thisMainMenu->load->loadSelect);

	if(thisMainMenu->load->scrollCount > 0){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 253 - (thisMainMenu->load->scrollUpArrow->fixedWidth / 2), yOff + 45 -(thisMainMenu->load->scrollUpArrow->fixedWidth / 2), thisMainMenu->load->scrollUpArrow);
	}

	if(thisMainMenu->load->numSaveData > thisMainMenu->load->scrollCount + thisMainMenu->load->savesPerScreen){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 253 - (thisMainMenu->load->scrollDownArrow->fixedWidth / 2), yOff + 620 -(thisMainMenu->load->scrollDownArrow->fixedWidth / 2), thisMainMenu->load->scrollDownArrow);
	}

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);

	if(thisMainMenu->load->mode == LOAD_MODE){
		drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 10 + 36, yOff + 7, "Load");
	}else if(thisMainMenu->load->mode == SAVE_MODE){
		drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 10 + 36, yOff + 7, "Save");
	}

	for(i = thisMainMenu->load->scrollCount; i < upperBound; i++){
		loadSaveData * tmpLoadSaveData= thisMainMenu->load->saves[i];

		sprintf(outStr, "Save %d", i);

		drawNewGameText(hdcBuffer, &textBoxRect, xOff + 69, yOff + 63 + (i - thisMainMenu->load->scrollCount) * 135, outStr);

		if(tmpLoadSaveData == NULL){
			drawNewGameText(hdcBuffer, &textBoxRect, xOff + 197, yOff + 104 + (i - thisMainMenu->load->scrollCount) * 135, "EMPTY");
		}else{
			drawNewGameText(hdcBuffer, &textBoxRect, xOff + 69, yOff + 91 + (i - thisMainMenu->load->scrollCount) * 135, tmpLoadSaveData->name);

			outStr[0] = '\0';
			sprintf(outStr, "Level: %d", tmpLoadSaveData->level);
			drawNewGameText(hdcBuffer, &textBoxRect, xOff + 69, yOff + 120 + (i - thisMainMenu->load->scrollCount) * 135, outStr);
		}
	}

	if(thisMainMenu->load->inConfirmationMode){
		SetTextColor(hdcBuffer, RGB(0, 0, 0));

		int confirmXOff = xOff + (thisMainMenu->load->loadView->fixedWidth / 2) - (thisMainMenu->confirmationDialog->fixedWidth / 2);
		int confirmYOff = yOff + (thisMainMenu->load->loadView->fixedHeight / 2) - (thisMainMenu->confirmationDialog->fixedHeight / 2);

		drawUnboundCharacterByPixels(hdc, hdcBuffer, confirmXOff, confirmYOff, thisMainMenu->confirmationDialog);

		if(thisMainMenu->selectedDecision == CONFIRMATION_YES){
			drawUnboundCharacterByPixels(hdc, hdcBuffer, confirmXOff + 43, confirmYOff + 119, thisMainMenu->confirmationDialogSelect);
		}else{
			drawUnboundCharacterByPixels(hdc, hdcBuffer, confirmXOff + 162, confirmYOff + 119, thisMainMenu->confirmationDialogSelect);
		}

		SetTextColor(hdcBuffer, RGB(255, 200, 0));

		textBoxRect.top = confirmYOff + 46;
		textBoxRect.left = confirmXOff + 8;
		textBoxRect.right = textBoxRect.left + 271;
		textBoxRect.bottom = textBoxRect.top + 50;

		if(thisMainMenu->load->mode == LOAD_MODE){
			DrawText(hdcBuffer, "Quit and load?", -1, &textBoxRect, DT_CENTER);
		}else if(thisMainMenu->load->mode == SAVE_MODE){
			DrawText(hdcBuffer, "Overwrite save?", -1, &textBoxRect, DT_CENTER);
		}

		drawNewGameFormText(hdcBuffer, &textBoxRect, confirmXOff + 43 + 42, confirmYOff + 119, "YES");
		drawNewGameFormText(hdcBuffer, &textBoxRect, confirmXOff + 162 + 42, confirmYOff + 119, "NO");

	}

	SelectObject(hdcBuffer, oldFont);
	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
}

void drawMainMenu(HDC hdc, HDC hdcBuffer, RECT * rect){
	switch(thisMainMenu->currentMenu){
		case MENU_TITLE:
			drawMainMenuTitle(hdc, hdcBuffer, rect);
			break;
		case MENU_NEW_GAME:
			switch(thisMainMenu->newGame->currentForm){
				case NEW_GAME_CREATE:
					drawNewGameCreateMenu(hdc, hdcBuffer, rect);
					break;
				case NEW_GAME_ABILITIES:
					drawNewGameAbilities(hdc, hdcBuffer, rect);
					break;
			}
			break;
		case MENU_LOAD:
			drawLoadMenu(hdc, hdcBuffer, rect);
			break;
	}
}
