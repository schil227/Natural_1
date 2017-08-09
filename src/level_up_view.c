/*
 * level_up_view.c
 *
 *  Created on: Aug 4, 2017
 *      Author: Adrian
 */
#include "./headers/level_up_view_pub_methods.h"

static levelUpView * thisLevelUpView;

void initLevelUpView(){
	thisLevelUpView = malloc(sizeof(levelUpView));
	thisLevelUpView->abilitiesView = malloc(sizeof(levelUpAbilitiesView));

	thisLevelUpView->currentMode = LEVELUP_MODE_DEFAULT;
	thisLevelUpView->currentField = LEVELUP_HP;
	thisLevelUpView->selectedStatField = LEVELUP_NONE;
	thisLevelUpView->bonusStatPoints = 1;
	thisLevelUpView->inLevelUpMode = 0;
	thisLevelUpView->bonusHP = -1;
	thisLevelUpView->bonusMana = -1;
	thisLevelUpView->rollsRemaining = 2;

	thisLevelUpView->levelUpView = createCharacter(1438, RGB(255,0,255), 0, 0);
	thisLevelUpView->spreadRollView = createCharacter(1439, RGB(255,0,255), 0, 0);
	thisLevelUpView->fieldSelect = createCharacter(1440, RGB(255,0,255), 0, 0);
	thisLevelUpView->fieldEnter = createCharacter(1441, RGB(255,0,255), 0, 0);
	thisLevelUpView->rollSelect = createCharacter(1407, RGB(255,0,255), 0, 0);
	thisLevelUpView->rollEnter = createCharacter(1412, RGB(255,0,255), 0, 0);
	thisLevelUpView->statFieldEdit = createCharacter(1409, RGB(255,0,255), 0, 0);
	thisLevelUpView->selectArrowRight = createCharacter(1504, RGB(255,0,255), 0, 0);
	thisLevelUpView->selectArrowLeft = createCharacter(1402, RGB(255,0,255), 0, 0);

	thisLevelUpView->abilitiesView->upgradePoints = 0;
	thisLevelUpView->abilitiesView->bonusMana = 0;
	thisLevelUpView->abilitiesView->abilitiesOffset = 0;
	thisLevelUpView->abilitiesView->numAbilities = 0;
	thisLevelUpView->abilitiesView->abilitiesPerScreen = 7;
	thisLevelUpView->abilitiesView->inCreateAbilityMode = 0;
	thisLevelUpView->abilitiesView->currentMode = UPGRADE_ABILITY_ABILITIES;
	thisLevelUpView->abilitiesView->upgradeAbilitiesView = createCharacter(1442, RGB(255,0,255), 0, 0);
	thisLevelUpView->abilitiesView->entryAddAbility = createCharacter(1417, RGB(255,0,255), 0, 0);
}

void destroyLevelUpView(){
	destroyFixedCharacter(thisLevelUpView->levelUpView);
	destroyFixedCharacter(thisLevelUpView->spreadRollView);
	destroyFixedCharacter(thisLevelUpView->fieldSelect);
	destroyFixedCharacter(thisLevelUpView->fieldEnter);
	destroyFixedCharacter(thisLevelUpView->rollSelect);
	destroyFixedCharacter(thisLevelUpView->rollEnter);

	destroyFixedCharacter(thisLevelUpView->abilitiesView->upgradeAbilitiesView);

	free(thisLevelUpView->abilitiesView);
	free(thisLevelUpView);
}

void enableLevelUpView(individual * player){
	thisLevelUpView->player = player;
	thisLevelUpView->inLevelUpMode = 1;
}

void disableLevelUpView(){
	thisLevelUpView->inLevelUpMode = 0;
}

int inLevelUpView(){
	if(thisLevelUpView == NULL){
		return 0;
	}

	return thisLevelUpView->inLevelUpMode;
}

int disableLevelUpCreateAbilityMode(){
	thisLevelUpView->abilitiesView->inCreateAbilityMode = 0;
}

int inLevelUpCreateAbilityMode(){
	return thisLevelUpView->abilitiesView->inCreateAbilityMode;
}

void levelUpReRoll(){
	if(thisLevelUpView->rollsRemaining <= 0){
		return;
	}

	thisLevelUpView->rollsRemaining--;

	if(thisLevelUpView->currentField == LEVELUP_HP){
		thisLevelUpView->bonusHP = rand() % getDiceSpread(thisLevelUpView->player, 'h') + 1;
	}else{
		thisLevelUpView->bonusMana = rand() % getDiceSpread(thisLevelUpView->player, 'm') + 1;
	}
}

void setUpLevelUpRoll(){
	thisLevelUpView->currentMode = LEVELUP_MODE_ROLL;
	thisLevelUpView->rollChoice = ROLLMODE_ROLL;

	if(thisLevelUpView->currentField == LEVELUP_HP){
		thisLevelUpView->bonusHP = rand() % getDiceSpread(thisLevelUpView->player, 'h') + 1;
	}else{
		thisLevelUpView->bonusMana = rand() % getDiceSpread(thisLevelUpView->player, 'm') + 1;
	}
}

void disableLevelUpRollMode(){
	thisLevelUpView->rollsRemaining = 2;
	thisLevelUpView->currentMode = LEVELUP_MODE_DEFAULT;
}

void reInitializeLevelUpView(){
	thisLevelUpView->bonusHP = -1;
	thisLevelUpView->bonusMana = -1;
	thisLevelUpView->currentField = LEVELUP_HP;
	thisLevelUpView->currentMode = LEVELUP_MODE_DEFAULT;
	thisLevelUpView->bonusStatPoints = 1;
	thisLevelUpView->rollsRemaining = 2;
}

void finalizeLevelUpView(){
	thisLevelUpView->player->level++;

	thisLevelUpView->player->baseHP += thisLevelUpView->bonusHP;
	thisLevelUpView->player->baseMana += thisLevelUpView->bonusMana;

	thisLevelUpView->player->hp = getTotalHP(thisLevelUpView->player);
	thisLevelUpView->player->mana = getTotalMana(thisLevelUpView->player);
	thisLevelUpView->player->food = getTotalFood(thisLevelUpView->player);
}

void initializeAbilityUpgradeMode(){
	thisLevelUpView->abilitiesView->upgradePoints = thisLevelUpView->player->INT + 1;
	thisLevelUpView->abilitiesView->bonusMana = thisLevelUpView->player->INT + thisLevelUpView->player->bonusMana;
	thisLevelUpView->abilitiesView->numAbilities = thisLevelUpView->player->abilities->numAbilities + 1;
	thisLevelUpView->abilitiesView->selectedAbilityIndex = 0;
	thisLevelUpView->currentMode = LEVELUP_MODE_ABLITIES;
}

void addAbilityToLevelUpUpgrade(ability * newAbility){
	//if adding the ability
	if(thisLevelUpView->abilitiesView->selectedAbilityIndex == thisLevelUpView->abilitiesView->numAbilities - 1){
		addAbilityToRegistryByType(newAbility);
		addAbilityToIndividual(thisLevelUpView->player, newAbility);
		thisLevelUpView->abilitiesView->upgradePoints--;
		thisLevelUpView->abilitiesView->numAbilities++;
	}

	thisLevelUpView->abilitiesView->bonusMana -= getSpentBonusMana();
}

void interpretLevelUpUpdateAbilitesVertical(int goingUp){
	switch(thisLevelUpView->abilitiesView->currentMode){
	case UPGRADE_ABILITY_ABILITIES:
		if(goingUp){
			if(thisLevelUpView->abilitiesView->selectedAbilityIndex > 0){
				thisLevelUpView->abilitiesView->selectedAbilityIndex--;
				thisLevelUpView->abilitiesView->abilitiesOffset = max(0, thisLevelUpView->abilitiesView->selectedAbilityIndex - thisLevelUpView->abilitiesView->abilitiesPerScreen);
			}
		}else{
			if(thisLevelUpView->abilitiesView->selectedAbilityIndex + 1 == thisLevelUpView->abilitiesView->numAbilities
					|| (thisLevelUpView->abilitiesView->selectedAbilityIndex + 1 == thisLevelUpView->abilitiesView->numAbilities - 1 && thisLevelUpView->abilitiesView->upgradePoints == 0)){
				thisLevelUpView->abilitiesView->currentMode = UPGRADE_ABILITY_POINTS;
			}else{
				thisLevelUpView->abilitiesView->selectedAbilityIndex++;
				thisLevelUpView->abilitiesView->abilitiesOffset = max(0, thisLevelUpView->abilitiesView->selectedAbilityIndex - thisLevelUpView->abilitiesView->abilitiesPerScreen);
			}
		}
		break;
	case UPGRADE_ABILITY_POINTS:
		if(!goingUp){
			thisLevelUpView->abilitiesView->currentMode = UPGRADE_ABILITY_BONUS;
		}else{
			if(thisLevelUpView->abilitiesView->upgradePoints == 0){
				thisLevelUpView->abilitiesView->selectedAbilityIndex = thisLevelUpView->player->abilities->numAbilities - 1;
			}else{
				thisLevelUpView->abilitiesView->selectedAbilityIndex = thisLevelUpView->abilitiesView->numAbilities - 1;
			}

			thisLevelUpView->abilitiesView->abilitiesOffset = max(0, thisLevelUpView->abilitiesView->selectedAbilityIndex - thisLevelUpView->abilitiesView->abilitiesPerScreen);
			thisLevelUpView->abilitiesView->currentMode = UPGRADE_ABILITY_ABILITIES;
		}
		break;
	case UPGRADE_ABILITY_BONUS:
		thisLevelUpView->abilitiesView->currentMode = (goingUp) ? UPGRADE_ABILITY_POINTS : UPGRADE_ABILITY_DONE;
		break;
	case UPGRADE_ABILITY_DONE:
		if(goingUp){
			thisLevelUpView->abilitiesView->currentMode = UPGRADE_ABILITY_BONUS;
		}
		break;
	}
}

void interpretLevelUpViewVertical(int goingUp){
	if(	thisLevelUpView->currentMode == LEVELUP_MODE_ROLL){
		return;
	}else if(thisLevelUpView->currentMode == LEVELUP_MODE_ABLITIES){
		interpretLevelUpUpdateAbilitesVertical(goingUp);
	}

	switch(thisLevelUpView->currentField){
	case LEVELUP_HP:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_HP : LEVELUP_MANA;
		break;
	case LEVELUP_MANA:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_HP : LEVELUP_STR;
		break;
	case LEVELUP_STR:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_MANA : LEVELUP_DEX;
		break;
	case LEVELUP_DEX:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_STR : LEVELUP_CON;
		break;
	case LEVELUP_CON:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_DEX : LEVELUP_INT;
		break;
	case LEVELUP_INT:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_CON : LEVELUP_WIS;
		break;
	case LEVELUP_WIS:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_INT : LEVELUP_WILL;
		break;
	case LEVELUP_WILL:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_WIS : LEVELUP_CHR;
		break;
	case LEVELUP_CHR:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_WILL : LEVELUP_LUCK;
		break;
	case LEVELUP_LUCK:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_CHR : LEVELUP_NEXT;
		break;
	case LEVELUP_NEXT:
		thisLevelUpView->currentField = (goingUp) ? LEVELUP_LUCK : LEVELUP_NEXT;
		break;
	}
}

void interpretLevelUpRollHorizontal(int goingLeft){
	switch(thisLevelUpView->rollChoice){
	case ROLLMODE_ACCEPT:
		thisLevelUpView->rollChoice = ROLLMODE_ROLL;
		break;
	case ROLLMODE_ROLL:
		thisLevelUpView->rollChoice = ROLLMODE_ACCEPT;
		break;
	}
}

int updateLevelUpStatField(levelUpField selectedStat, int goingLeft){
	if(!goingLeft){
		if(thisLevelUpView->bonusStatPoints == 1){
			thisLevelUpView->selectedStatField = selectedStat;
			thisLevelUpView->bonusStatPoints = 0;
			return 1;
		}
	}else{
		if(thisLevelUpView->bonusStatPoints == 0 && thisLevelUpView->selectedStatField == selectedStat){
			thisLevelUpView->selectedStatField = LEVELUP_NONE;
			thisLevelUpView->bonusStatPoints = 1;
			return -1;
		}
	}

	return 0;
}

void interpretLevelUpViewHorizontal(int goingLeft){
	if(thisLevelUpView->currentMode == LEVELUP_MODE_ROLL){
		interpretLevelUpRollHorizontal(goingLeft);
		return;
	}

	switch(thisLevelUpView->currentField){
	case LEVELUP_STR:
		thisLevelUpView->player->STR += updateLevelUpStatField(LEVELUP_STR, goingLeft);
		break;
	case LEVELUP_DEX:
		thisLevelUpView->player->DEX += updateLevelUpStatField(LEVELUP_DEX, goingLeft);
		break;
	case LEVELUP_CON:
		thisLevelUpView->player->CON += updateLevelUpStatField(LEVELUP_CON, goingLeft);
		break;
	case LEVELUP_INT:
		thisLevelUpView->player->INT += updateLevelUpStatField(LEVELUP_INT, goingLeft);
		break;
	case LEVELUP_WIS:
		thisLevelUpView->player->WIS += updateLevelUpStatField(LEVELUP_WIS, goingLeft);
		break;
	case LEVELUP_WILL:
		thisLevelUpView->player->WILL += updateLevelUpStatField(LEVELUP_WILL, goingLeft);
		break;
	case LEVELUP_CHR:
		thisLevelUpView->player->CHR += updateLevelUpStatField(LEVELUP_CHR, goingLeft);
		break;
	case LEVELUP_LUCK:
		thisLevelUpView->player->LUCK += updateLevelUpStatField(LEVELUP_LUCK, goingLeft);
		break;
	}
}

void interpretLevelUpViewEscape(){
	if(thisLevelUpView->currentMode == LEVELUP_MODE_ROLL){
		return;
	}
}

void interpretLevelUpUpgradeAbilitiesEnter(){
	switch(thisLevelUpView->abilitiesView->currentMode){
		case UPGRADE_ABILITY_ABILITIES:
			if(thisLevelUpView->abilitiesView->upgradePoints == 0){
				return;
			}

			if(thisLevelUpView->abilitiesView->selectedAbilityIndex == thisLevelUpView->abilitiesView->numAbilities - 1){
				thisLevelUpView->abilitiesView->inCreateAbilityMode = 1;

				enableAbilityCreateMode(thisLevelUpView->abilitiesView->bonusMana, ABILITY_CREATE_EXCEPT_PERMENANT, DEFAULT_ABILITY, NULL);
			}else{
				//level up effects take place immediately
				thisLevelUpView->player->abilities->abilitiesList[thisLevelUpView->abilitiesView->selectedAbilityIndex]->level++;
				thisLevelUpView->abilitiesView->upgradePoints--;
				enableAbilityCreateMode(thisLevelUpView->abilitiesView->bonusMana, ABILITY_CREATE_EXCEPT_PERMENANT, LEVELUP_ABILITY, thisLevelUpView->player->abilities->abilitiesList[thisLevelUpView->abilitiesView->selectedAbilityIndex]);
			}
			break;
		case UPGRADE_ABILITY_DONE:
			disableLevelUpView();
			reInitializeLevelUpView();
		break;
	}
}

void interpretLevelUpRollEnter(){
	switch(thisLevelUpView->rollChoice){
	case ROLLMODE_ACCEPT:
		disableLevelUpRollMode();
		break;
	case ROLLMODE_ROLL:
		if(thisLevelUpView->rollsRemaining > 0){
			levelUpReRoll();
		}
		break;
	}
}

void interpretLevelUpViewEnter(){
	if(thisLevelUpView->currentMode == LEVELUP_MODE_ROLL){
		interpretLevelUpRollEnter();
		return;
	}else if(thisLevelUpView->currentMode == LEVELUP_MODE_ABLITIES){
		interpretLevelUpUpgradeAbilitiesEnter();
		return;
	}

	switch(thisLevelUpView->currentField){
	case LEVELUP_HP:
		if(thisLevelUpView->bonusHP == -1){
			setUpLevelUpRoll();
		}
		break;
	case LEVELUP_MANA:
		if(thisLevelUpView->bonusMana == -1){
			setUpLevelUpRoll();
		}
		break;
	case LEVELUP_NEXT:
		if(thisLevelUpView->bonusHP != -1 && thisLevelUpView->bonusMana != -1 && thisLevelUpView->bonusStatPoints != 1){
			finalizeLevelUpView();
			initializeAbilityUpgradeMode();
		}
		break;
	}
}

void drawLevelUpUpgradeAbilitiesView(HDC hdc, HDC hdcBuffer, RECT * rect, int xOff, int yOff){
	RECT textRect;
	int i;
	int numAbilitiesToDraw = min(thisLevelUpView->abilitiesView->numAbilities, thisLevelUpView->abilitiesView->abilitiesPerScreen);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisLevelUpView->abilitiesView->upgradeAbilitiesView);

	switch(thisLevelUpView->abilitiesView->currentMode){
		case UPGRADE_ABILITY_POINTS:
			drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 226, yOff + 410, thisLevelUpView->statFieldEdit);
			break;
		case UPGRADE_ABILITY_BONUS:
			drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 189, yOff + 452, thisLevelUpView->statFieldEdit);
			break;
		case UPGRADE_ABILITY_DONE:
			drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 100, yOff + 498, thisLevelUpView->fieldEnter);
			break;
	}

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);
	SetTextColor(hdcBuffer, RGB(255, 200, 0));

	for(i = 0; i < numAbilitiesToDraw; i++){
		int index = i + thisLevelUpView->abilitiesView->abilitiesOffset;

		if(i + 1 == numAbilitiesToDraw && thisLevelUpView->abilitiesView->upgradePoints == 0){
			break;
		}

		if(thisLevelUpView->abilitiesView->selectedAbilityIndex == index && thisLevelUpView->abilitiesView->currentMode == UPGRADE_ABILITY_ABILITIES){
			drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 57, yOff + 61 + 46 * i, thisLevelUpView->abilitiesView->entryAddAbility);
		}

		//reserve last one for [Add Ability]
		if(i + 1 == numAbilitiesToDraw + thisLevelUpView->abilitiesView->abilitiesOffset){
			drawNewGameFormText(hdcBuffer, &textRect,  xOff + 57 + 81, yOff + 63 + 46 * i, "[Add Ability]");
		}else{
			drawNewGameFormText(hdcBuffer, &textRect,  xOff + 57 + 81, yOff + 63 + 46 * i, thisLevelUpView->player->abilities->abilitiesList[index]->name);
		}
	}

	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 226 + 22, yOff + 411, thisLevelUpView->abilitiesView->upgradePoints);
	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 189 + 22, yOff + 453, thisLevelUpView->abilitiesView->bonusMana);
	drawNewGameFormText(hdcBuffer, &textRect, xOff + 100 + 42, yOff + 498, "DONE");

	SelectObject(hdcBuffer, oldFont);
	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
}

void drawLevelUpRollView(HDC hdc, HDC hdcBuffer, RECT * rect, int xOff, int yOff){
	RECT textRect;
	char outText[128];

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisLevelUpView->spreadRollView);

	if(thisLevelUpView->rollChoice == ROLLMODE_ROLL){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 38, yOff + 123, thisLevelUpView->rollEnter);
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 173, yOff + 123, thisLevelUpView->rollSelect);
	}else{
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 38, yOff + 123, thisLevelUpView->rollSelect);
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 173, yOff + 123, thisLevelUpView->rollEnter);
	}

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);

	if(thisLevelUpView->currentField == LEVELUP_HP){
		sprintf(outText, "Bonus HP: %d", thisLevelUpView->bonusHP);
		drawNewGameFormText(hdcBuffer, &textRect, xOff + (thisLevelUpView->spreadRollView->fixedWidth / 2), yOff + 28, outText);
	}else{
		sprintf(outText, "Bonus Mana: %d", thisLevelUpView->bonusMana);
		drawNewGameFormText(hdcBuffer, &textRect, xOff + (thisLevelUpView->spreadRollView->fixedWidth / 2), yOff + 28, outText);
	}

	sprintf(outText, "Re-rolls remaining: %d", thisLevelUpView->rollsRemaining);
	drawNewGameFormText(hdcBuffer, &textRect, xOff + (thisLevelUpView->spreadRollView->fixedWidth / 2), yOff + 68, outText);

	drawNewGameFormText(hdcBuffer, &textRect, xOff + 38 + 57, yOff + 123, "Re-roll");
	drawNewGameFormText(hdcBuffer, &textRect, xOff + 173 + 57, yOff + 123, "Accept");

	SelectObject(hdcBuffer, oldFont);
	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
}

void drawLevelUpStatArrow(HDC hdc, HDC hdcBuffer, levelUpField thisStatField, int xOff, int yOff){
	if(thisStatField == thisLevelUpView->currentField){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisLevelUpView->statFieldEdit);
	}

	if(thisLevelUpView->bonusStatPoints > 0){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 50, yOff + 10, thisLevelUpView->selectArrowRight);
	}else if(thisStatField == thisLevelUpView->selectedStatField){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff - 20, yOff + 10, thisLevelUpView->selectArrowLeft);
	}
}

void drawLevelUpView(HDC hdc, HDC hdcBuffer, RECT * rect){
	int xOff = 40;
	int yOff = 40;

	if(thisLevelUpView->currentMode == LEVELUP_MODE_ABLITIES){
		drawLevelUpUpgradeAbilitiesView(hdc, hdcBuffer, rect, xOff, yOff);
		return;
	}

	RECT textRect;
	textRect.top = 20;
	textRect.left = 20;
	textRect.bottom = 100;
	textRect.right = 200;

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisLevelUpView->levelUpView);

	if(thisLevelUpView->bonusHP == -1){
		if(thisLevelUpView->currentField == LEVELUP_HP){
			drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 202, yOff + 54, thisLevelUpView->fieldEnter);
		}else{
			drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 202, yOff + 54, thisLevelUpView->fieldSelect);
		}
	}

	if(thisLevelUpView->bonusMana == -1){
		if(thisLevelUpView->currentField == LEVELUP_MANA){
			drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 202, yOff + 97, thisLevelUpView->fieldEnter);
		}else{
			drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 202, yOff + 97, thisLevelUpView->fieldSelect);
		}
	}

	if(thisLevelUpView->currentField == LEVELUP_NEXT){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 122, yOff + 529, thisLevelUpView->fieldEnter);
	}else{
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 122, yOff + 529, thisLevelUpView->fieldSelect);
	}

	drawLevelUpStatArrow(hdc, hdcBuffer, LEVELUP_STR, xOff + 199, yOff + 182);
	drawLevelUpStatArrow(hdc, hdcBuffer, LEVELUP_DEX, xOff + 199, yOff + 182 + 42);
	drawLevelUpStatArrow(hdc, hdcBuffer, LEVELUP_CON, xOff + 199, yOff + 182 + 42 * 2);
	drawLevelUpStatArrow(hdc, hdcBuffer, LEVELUP_INT, xOff + 199, yOff + 182 + 42 * 3);
	drawLevelUpStatArrow(hdc, hdcBuffer, LEVELUP_WIS, xOff + 199, yOff + 182 + 42 * 4);
	drawLevelUpStatArrow(hdc, hdcBuffer, LEVELUP_WILL, xOff + 199, yOff + 182 + 42 * 5);
	drawLevelUpStatArrow(hdc, hdcBuffer, LEVELUP_CHR, xOff + 200, yOff + 182 + 42 * 6);
	drawLevelUpStatArrow(hdc, hdcBuffer, LEVELUP_LUCK, xOff + 199, yOff + 182 + 42 * 7);

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);

	if(thisLevelUpView->bonusHP == -1){
		drawNewGameFormText(hdcBuffer, &textRect, xOff + 199 + 44, yOff + 55, "ROLL");
	}else{
		drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 202 + 44, yOff + 57, thisLevelUpView->bonusHP);
	}

	if(thisLevelUpView->bonusMana == -1){
		drawNewGameFormText(hdcBuffer, &textRect, xOff + 199 + 44, yOff + 98, "ROLL");
	}else{
		drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 202 + 44, yOff + 97, thisLevelUpView->bonusMana);
	}

	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 199 + 22, yOff + 182, thisLevelUpView->player->STR);
	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 199 + 22, yOff + 182 + 42, thisLevelUpView->player->DEX);
	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 199 + 22, yOff + 182 + 42 * 2, thisLevelUpView->player->CON);
	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 199 + 22, yOff + 182 + 42 * 3, thisLevelUpView->player->INT);
	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 199 + 22, yOff + 182 + 42 * 4, thisLevelUpView->player->WIS);
	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 199 + 22, yOff + 182 + 42 * 5, thisLevelUpView->player->WILL);
	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 199 + 22, yOff + 182 + 42 * 6, thisLevelUpView->player->CHR);
	drawNewGameFormNumber(hdcBuffer, &textRect, xOff + 199 + 22, yOff + 182 + 42 * 7, thisLevelUpView->player->LUCK);

//	if(thisLevelUpView->player->INT + 1 <= 0){
//		drawNewGameFormText(hdcBuffer, &textRect, xOff + 122 + 44, yOff + 529, "DONE");
//	}else{
	drawNewGameFormText(hdcBuffer, &textRect, xOff + 122 + 44, yOff + 529, "NEXT");
//	}


	SelectObject(hdcBuffer, oldFont);
	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));

	if(thisLevelUpView->currentMode == LEVELUP_MODE_ROLL){
		xOff = 80;
		yOff = 80;

		drawLevelUpRollView(hdc, hdcBuffer, rect, xOff, yOff);
	}
}
