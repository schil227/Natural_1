/*
 * main_menu.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Adrian
 */

#include "./headers/main_menu_pub_methods.h"

static mainMenu * thisMainMenu;

void initMainMenu(int inMenuMode){
	thisMainMenu = malloc(sizeof(mainMenu));
	thisMainMenu->title = malloc(sizeof(titleMenu));
	thisMainMenu->newGame = malloc(sizeof(newGameMenu));
	thisMainMenu->inMenuMode = inMenuMode;
	thisMainMenu->currentMenu = MENU_TITLE;
	thisMainMenu->waitingForNameMode = 0;

	thisMainMenu->leftSelectArrow = createCharacter(1402, RGB(255,0,255), 0, 0);
	thisMainMenu->rightSelectArrow = createCharacter(1504, RGB(255,0,255), 0, 0);

	thisMainMenu->title->titleScreen = createCharacter(1400, RGB(255,0,255), 0, 0);
	thisMainMenu->title->options = createCharacter(1403, RGB(255,0,255), 0, 0);
	thisMainMenu->title->titleOptionsDisplayed = 0;
	thisMainMenu->title->selectedOption = TITLE_NEW_GAME;
	thisMainMenu->rightSelectArrow->y = 13;

	thisMainMenu->newGame->currentField = CREATE_NAME;
	thisMainMenu->newGame->currentSpread = SPREAD_8_8;
	thisMainMenu->newGame->inEditMode = 0;
	thisMainMenu->newGame->statPoints = 6;
	thisMainMenu->newGame->numAbilites = 2;
	thisMainMenu->newGame->newPlayer = getIndividualFromRegistry(0);
	thisMainMenu->newGame->creationScreen = createCharacter(1401, RGB(255,0,255), 0, 0);
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

	thisMainMenu->newGame->descriptionRect = malloc(sizeof(RECT));
	thisMainMenu->newGame->descriptionRect->top = 0;
	thisMainMenu->newGame->descriptionRect->left = 0;
	thisMainMenu->newGame->descriptionRect->bottom = 130;
	thisMainMenu->newGame->descriptionRect->right = 486;

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
	strcpy(thisMainMenu->newGame->descriptionCrit, "The character's critical strike.\n\nMAX always returns the highest dice roll, while DUB doubles the dice roll.");
	strcpy(thisMainMenu->newGame->descriptionMvmt, "The number of spaces the character can move in one turn.");
	strcpy(thisMainMenu->newGame->descriptionNumAbilities, "The number of abilities the character will start with.");
	strcpy(thisMainMenu->newGame->descriptionFateTokens, "The number of fate tokens the character starts with.\n\nFate tokens can be used in a variety of situations to change the state of the game, such as re-rolling a bad roll or making a door unlock without a key.");

}

void disableMainMenuMode(){
	thisMainMenu->inMenuMode = 0;
}

void enableMainMenuMode(){
	thisMainMenu->inMenuMode = 1;
}

int inMainMenuMode(){
	if(thisMainMenu != NULL){
		return thisMainMenu->inMenuMode;
	}

	return 0;
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
			disableMainMenuMode();
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

void newGameMenuMoveVertical(int goingUp){
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
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_AC : CREATE_CRIT);
		break;
	case CREATE_CHR:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_WILL : CREATE_LUCK);
		break;
	case CREATE_CRIT:
		thisMainMenu->newGame->currentField = (goingUp ? CREATE_DAM : CREATE_MVMT);
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

void newGameMenuMoveHorizontal(int goingLeft){
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
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_STR : CREATE_HP);
		break;
	case CREATE_HP:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_STR : CREATE_HP);
		break;
	case CREATE_DEX:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_DEX : CREATE_MANA);
		break;
	case CREATE_MANA:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_DEX : CREATE_MANA);
		break;
	case CREATE_CON:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_CON : CREATE_FOOD);
		break;
	case CREATE_FOOD:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_CON : CREATE_FOOD);
		break;
	case CREATE_INT:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_INT : CREATE_ATTACK);
		break;
	case CREATE_ATTACK:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_INT : CREATE_ATTACK);
		break;
	case CREATE_WIS:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_WIS : CREATE_AC);
		break;
	case CREATE_AC:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_WIS : CREATE_AC);
		break;
	case CREATE_WILL:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_WILL : CREATE_DAM);
		break;
	case CREATE_DAM:
		thisMainMenu->newGame->currentField = (goingLeft ? CREATE_WILL : CREATE_DAM);
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

void newGameMenuDecreaseField(){
	if(thisMainMenu->newGame->statPoints <= 0){
		return;
	}

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
		if(newPlayer->CON > -2){
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

void newGameMenuInterpretLeft(){
	if(thisMainMenu->newGame->inEditMode){
		newGameMenuDecreaseField();
	}else{
		newGameMenuMoveHorizontal(1);
	}
}

void newGameMenuInterpretRight(){
	if(thisMainMenu->newGame->inEditMode){
		newGameMenuIncreaseField();
	}else{
		newGameMenuMoveHorizontal(0);
	}
}

void newGameMenuInterpretEnter(){
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
		thisMainMenu->newGame->newPlayer->ID = 1;
		setPlayer(thisMainMenu->newGame->newPlayer);
		replaceIndividualInRegistry(thisMainMenu->newGame->newPlayer, 1);
		disableMainMenuMode();
		break;
	}
}

void MainMenuSetName(char * newName){
	switch(thisMainMenu->currentMenu){
		case MENU_NEW_GAME:
		strcpy(thisMainMenu->newGame->newPlayer->name, newName);
	}
}

void newGameResetPlayer(){
	thisMainMenu->newGame->newPlayer->name[0] = '\0';
	thisMainMenu->newGame->currentSpread = SPREAD_8_8;
	thisMainMenu->newPlayer->baseHP = 8;
	thisMainMenu->newPlayer->baseMana = 8;
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

void newGameMenuInterpretEscape(){
	if(thisMainMenu->newGame->inEditMode){
		thisMainMenu->newGame->inEditMode = 0;
	}else{
		thisMainMenu->currentMenu = MENU_TITLE;
		newGameResetPlayer();
		thisMainMenu->newGame->statPoints = 6;
	}
}

void mainMenuInterpretRight(){
	switch(thisMainMenu->currentMenu){
	case MENU_NEW_GAME:
		newGameMenuInterpretRight();
		break;
	}
}

void mainMenuInterpretLeft(){
	switch(thisMainMenu->currentMenu){
	case MENU_NEW_GAME:
		newGameMenuInterpretLeft();
		break;
	}
}

void mainMenuInterpretUp(){
	switch(thisMainMenu->currentMenu){
	case MENU_TITLE:
		mainMenuTitleUp();
		break;
	case MENU_NEW_GAME:
		newGameMenuMoveVertical(1);
		break;
	}
}

void mainMenuInterpretDown(){
	switch(thisMainMenu->currentMenu){
	case MENU_TITLE:
		mainMenuTitleDown();
		break;
	case MENU_NEW_GAME:
		newGameMenuMoveVertical(0);
		break;
	}
}

void mainMenuInterpretEnter(){
	switch(thisMainMenu->currentMenu){
	case MENU_TITLE:
		mainMenuTitleSelect();
		break;
	case MENU_NEW_GAME:
		newGameMenuInterpretEnter();
		break;
	}
}

void mainMenuInterpretEscape(){
	switch(thisMainMenu->currentMenu){
	case MENU_TITLE:
		thisMainMenu->title->titleOptionsDisplayed = 0;
		break;
	case MENU_NEW_GAME:
		newGameMenuInterpretEscape();
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

void drawNewGameSelectedInfo(HDC hdc, HDC hdcBuffer, int xOff, int yOff){
	switch(thisMainMenu->newGame->currentField){
		case CREATE_NAME:
			drawCreateField(hdc, hdcBuffer, xOff + 102, yOff + 22, 5);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionName, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_SPREAD:
			drawCreateField(hdc, hdcBuffer, xOff + 180, yOff + 68, 3);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionSpread, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_AVATAR:
			drawCreateField(hdc, hdcBuffer, xOff + 296, yOff + 22, 6);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionAvatar, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_STR:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 174, 1);

			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionSTR, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_DEX:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 174 + 38, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionDEX, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_CON:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 174 + 38 * 2, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionCON, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_INT:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 174 + 38 * 3, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionINT, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_WIS:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 174 + 38 * 4, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionWIS, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_WILL:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 174 + 38 * 5, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionWILL, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_CHR:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 174 + 38 * 6, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionCHR, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_LUCK:
			drawCreateField(hdc, hdcBuffer, xOff + 156, yOff + 174 + 38 * 7, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionLUCK, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_DONE:
			drawCreateField(hdc, hdcBuffer, xOff + 80, yOff + 486, 3);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionDone, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_HP:
			drawCreateField(hdc, hdcBuffer, xOff + 306, yOff + 140, 3);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionHP, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_MANA:
			drawCreateField(hdc, hdcBuffer, xOff + 338, yOff + 182, 3);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionMana, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_FOOD:
			drawCreateField(hdc, hdcBuffer, xOff + 404, yOff + 224, 2);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionFood, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_ATTACK:
			drawCreateField(hdc, hdcBuffer, xOff + 344, yOff + 266, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionAttack, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_AC:
			drawCreateField(hdc, hdcBuffer, xOff + 302, yOff + 306, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionAC, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_DAM:
			drawCreateField(hdc, hdcBuffer, xOff + 326, yOff + 344, 4);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionDam, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_CRIT:
			drawCreateField(hdc, hdcBuffer, xOff + 310, yOff + 386, 3);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionCrit, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_MVMT:
			drawCreateField(hdc, hdcBuffer, xOff + 340, yOff + 428, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionMvmt, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_NUM_ABILITIES:
			drawCreateField(hdc, hdcBuffer, xOff + 440, yOff + 466, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionNumAbilities, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
		case CREATE_FATE_TOKENS:
			drawCreateField(hdc, hdcBuffer, xOff + 426, yOff + 504, 1);
			DrawText(hdcBuffer, thisMainMenu->newGame->descriptionFateTokens, -1, thisMainMenu->newGame->descriptionRect, DT_WORDBREAK);
			break;
	}
}

void drawNewGameMenu(HDC hdc, HDC hdcBuffer, RECT * rect){
	int xOff = 0, yOff = 0;
	char outStr[16];
	RECT textBoxRect;
	SIZE size;
	individual * newPlayer = thisMainMenu->newGame->newPlayer;

	xOff = max((rect->right / 2) - (thisMainMenu->newGame->creationScreen->fixedWidth / 2) , 0);
	yOff = max((rect->bottom / 2) - (thisMainMenu->newGame->creationScreen->fixedHeight / 2) , 0);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisMainMenu->newGame->creationScreen);

	drawUnboundAnimationByPixels(hdc, hdcBuffer, newPlayer->playerCharacter, xOff + 296, yOff + 22, 0);

	thisMainMenu->newGame->descriptionRect->top = yOff + 552;
	thisMainMenu->newGame->descriptionRect->bottom = yOff + 552 + 130;
	thisMainMenu->newGame->descriptionRect->left = xOff + 13;
	thisMainMenu->newGame->descriptionRect->right = xOff + 10 + 484;

	textBoxRect.top = yOff + 27;
	textBoxRect.left = xOff + 110;
	textBoxRect.bottom = textBoxRect.top + 30;
	textBoxRect.right = textBoxRect.left + 150;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	drawNewGameSelectedInfo(hdc, hdcBuffer, xOff, yOff);

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);

	DrawText(hdcBuffer, newPlayer->name, -1, &textBoxRect, DT_SINGLELINE);

	drawDiceSpread(hdcBuffer, &textBoxRect, xOff + 180 + 42, yOff + 68);

	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 178, yOff + 140, thisMainMenu->newGame->statPoints);

	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174, newPlayer->STR);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 38, newPlayer->DEX);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 38 * 2, newPlayer->CON);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 38 * 3, newPlayer->INT);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 38 * 4, newPlayer->WIS);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 38 * 5, newPlayer->WILL);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 38 * 6, newPlayer->CHR);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 156 + 22, yOff + 174 + 38 * 7, newPlayer->LUCK);

	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 404 + 30, yOff + 225, newPlayer->totalFood);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 344 + 22, yOff + 266, newPlayer->attack);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 302 + 22, yOff + 306, newPlayer->AC);
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 312 + 30, yOff + 387, newPlayer->critType);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 340 + 22, yOff + 428, newPlayer->mvmt);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 440 + 22, yOff + 466, thisMainMenu->newGame->numAbilites);
	drawNewGameFormNumber(hdcBuffer, &textBoxRect, xOff + 426 + 22, yOff + 504, newPlayer->fateTokens);

	sprintf(outStr, "%d/%d", newPlayer->baseHP + newPlayer->CON * 2, newPlayer->baseHP + newPlayer->CON * 2);
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 307 + 42, yOff + 141, outStr);

	sprintf(outStr, "%d/%d", newPlayer->baseMana + newPlayer->WILL * 2, newPlayer->baseMana + newPlayer->WILL * 2);
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 339 + 42, yOff + 183, outStr);

	sprintf(outStr, "[%d-%d]", newPlayer->minDam, newPlayer->maxDam);

	if(newPlayer->STR >= 0){
		sprintf(outStr + strlen(outStr), " + %d", newPlayer->STR);
	}else if(newPlayer->STR < 0){
		sprintf(outStr + strlen(outStr), " - %d", abs(newPlayer->STR));
	}

	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 326 + 57, yOff + 345, outStr);

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
			drawNewGameMenu(hdc, hdcBuffer, rect);
			break;
	}
}
