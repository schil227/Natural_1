/*
 * character_info_view.c
 *
 *  Created on: May 11, 2017
 *      Author: Adrian
 */

#include "./headers/character_info_view_pub_methods.h"

static characterInfoView * thisCharacterInfoView;

void initCharacterInfoView(){
	thisCharacterInfoView = malloc(sizeof(characterInfoView));
	thisCharacterInfoView->thisIndividual = NULL;

	thisCharacterInfoView->inCharacterInfoView = 0;
	thisCharacterInfoView->inInfoView = 1;
	thisCharacterInfoView->canLevelUp = 0;
	thisCharacterInfoView->selectedEffectMode = ACTIVE_EFFECT_ABILITY;
	thisCharacterInfoView->selectedEffectIndex = -1;
	thisCharacterInfoView->numEquiptItems = 0;
	thisCharacterInfoView->MAX_EQUIPPED_ITEMS = 5;
	thisCharacterInfoView->effectDrawSkipCount = 0;
	thisCharacterInfoView->MAX_EFFECTS = 13;

	thisCharacterInfoView->infoWindow = createCharacter(1525,RGB(255,0,255),0,0);
	thisCharacterInfoView->activeEffectsWindow = createCharacter(1526,RGB(255,0,255),0,0);
	thisCharacterInfoView->activeEffectsArrow = createCharacter(1528,RGB(255,0,255),0,0);
	thisCharacterInfoView->characterInfoArrow = createCharacter(1530,RGB(255,0,255),0,0);
	thisCharacterInfoView->levelUpArrow = createCharacter(1529,RGB(255,0,255),0,0);
	thisCharacterInfoView->selectArrow = createCharacter(1504,RGB(255,0,255),0,0);
	thisCharacterInfoView->scrollUpArrow = createCharacter(1505,RGB(255,0,255),0,0);
	thisCharacterInfoView->scrollDownArrow = createCharacter(1507,RGB(255,0,255),0,0);
	thisCharacterInfoView->drChart = createCharacter(1527,RGB(255,0,255),0,0);
	thisCharacterInfoView->frame = createCharacter(1506,RGB(255,0,255),0,0);
}

void destroyCharacterInfoView(){
	destroyFixedCharacter(thisCharacterInfoView->infoWindow);
	destroyFixedCharacter(thisCharacterInfoView->activeEffectsWindow);
	destroyFixedCharacter(thisCharacterInfoView->activeEffectsArrow);
	destroyFixedCharacter(thisCharacterInfoView->characterInfoArrow);
	destroyFixedCharacter(thisCharacterInfoView->levelUpArrow);
	destroyFixedCharacter(thisCharacterInfoView->selectArrow);
	destroyFixedCharacter(thisCharacterInfoView->scrollUpArrow);
	destroyFixedCharacter(thisCharacterInfoView->scrollDownArrow);
	destroyFixedCharacter(thisCharacterInfoView->drChart);
}


void populateCharacterInfoViewEquiptItems(individual * thisIndividual){
	int i, itemsPassed = 0;

	for(i = 0; i < thisIndividual->backpack->MAX_ITEMS; i++){
		item * tmpItem = thisIndividual->backpack->inventoryArr[i];

		if(tmpItem != NULL){
			if(tmpItem->isEquipt){
				thisCharacterInfoView->equippedItems[thisCharacterInfoView->numEquiptItems] = tmpItem;
				thisCharacterInfoView->numEquiptItems++;

				if(thisCharacterInfoView->numEquiptItems >= thisCharacterInfoView->MAX_EQUIPPED_ITEMS){
					break;
				}
			}

			itemsPassed++;

			if(itemsPassed == thisIndividual->backpack->inventorySize){
				break;
			}
		}
	}
}

void enableCharacterInfoView(individual * thisIndividual){
	thisCharacterInfoView->inCharacterInfoView = 1;
	thisCharacterInfoView->thisIndividual = thisIndividual;
	populateCharacterInfoViewEquiptItems(thisIndividual);
}

void disableCharacterInfoView(){
	thisCharacterInfoView->inCharacterInfoView = 0;
	thisCharacterInfoView->selectedEffectIndex = -1;
	thisCharacterInfoView->numEquiptItems = 0;
}

int inCharacterInfoView(){
	if(thisCharacterInfoView != NULL){
		return thisCharacterInfoView->inCharacterInfoView;
	}

	return 0;
}

void showInfoView(){
	thisCharacterInfoView->inInfoView = 1;
}

void showEffectsView(){
	thisCharacterInfoView->inInfoView = 0;

	if(thisCharacterInfoView->selectedEffectIndex == -1){
		findStartingCharacterInfoIndex();
	}
}

int inInfoView(){
	return thisCharacterInfoView->inInfoView;
}

void findStartingCharacterInfoIndex(){
	if(thisCharacterInfoView->thisIndividual->activeAbilities->numAbilities > 0){
		thisCharacterInfoView->selectedEffectMode = ACTIVE_EFFECT_ABILITY;
		thisCharacterInfoView->selectedEffectIndex = 0;
	}else if(thisCharacterInfoView->thisIndividual->activeStatuses->numStatuses > 0){
		thisCharacterInfoView->selectedEffectMode = ACTIVE_EFFECT_STATUS;
		thisCharacterInfoView->selectedEffectIndex = 0;
	}else if(thisCharacterInfoView->thisIndividual->foodBonusActive || thisCharacterInfoView->thisIndividual->foodNegativeActive){
		thisCharacterInfoView->selectedEffectMode = ACTIVE_EFFECT_FOOD;
		thisCharacterInfoView->selectedEffectIndex = 0;
	}else if(thisCharacterInfoView->numEquiptItems){
		thisCharacterInfoView->selectedEffectMode = ACTIVE_EFFECT_EQUIPT_ITEM;
		thisCharacterInfoView->selectedEffectIndex = 0;
	}else if(thisCharacterInfoView->thisIndividual->activeItems->activeItemsTotal > 0){
		thisCharacterInfoView->selectedEffectMode = ACTIVE_EFFECT_ITEM;
		thisCharacterInfoView->selectedEffectIndex = 0;
	}

	thisCharacterInfoView->effectDrawSkipCount = 0;
}

void selectNextActiveEffectUp(){
	activeEffectType traverseEffect = thisCharacterInfoView->selectedEffectMode;
	int index = thisCharacterInfoView->selectedEffectIndex;

	//None to select, initialization couldn't find anything
	if(thisCharacterInfoView->selectedEffectIndex == -1){
		return;
	}

	if(traverseEffect == ACTIVE_EFFECT_ITEM){
		if(index - 1 >= 0 && thisCharacterInfoView->thisIndividual->activeItems->activeItemsTotal > 0){
			thisCharacterInfoView->selectedEffectIndex = index - 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;
			return;
		}else{
			traverseEffect = ACTIVE_EFFECT_EQUIPT_ITEM;
			index = thisCharacterInfoView->numEquiptItems;
		}
	}

	if(traverseEffect == ACTIVE_EFFECT_EQUIPT_ITEM){
		if(index - 1 >= 0 && thisCharacterInfoView->numEquiptItems > 0){
			thisCharacterInfoView->selectedEffectIndex = index - 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;
			return;
		}else{
			traverseEffect = ACTIVE_EFFECT_FOOD;
			index = -1;
		}
	}

	if(traverseEffect == ACTIVE_EFFECT_FOOD){
		if(index == -1 && (thisCharacterInfoView->thisIndividual->foodBonusActive || thisCharacterInfoView->thisIndividual->foodBonusActive)){
			thisCharacterInfoView->selectedEffectIndex = index + 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;
			return;
		}else{
			traverseEffect = ACTIVE_EFFECT_STATUS;
			index = thisCharacterInfoView->thisIndividual->activeStatuses->numStatuses;
		}
	}

	if(traverseEffect == ACTIVE_EFFECT_STATUS){
		if(index - 1 >= 0 && thisCharacterInfoView->thisIndividual->activeStatuses->numStatuses > 0){
			thisCharacterInfoView->selectedEffectIndex = index - 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;
			return;
		}else{
			traverseEffect = ACTIVE_EFFECT_ABILITY;
			index = thisCharacterInfoView->thisIndividual->activeAbilities->numAbilities;
		}
	}

	if(traverseEffect == ACTIVE_EFFECT_ABILITY){
		if(index - 1 >= 0 && thisCharacterInfoView->thisIndividual->activeAbilities->numAbilities > 0){
			thisCharacterInfoView->selectedEffectIndex = index - 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;
			return;
		}
	}
}

void calculateSkipIndex(){
	activeEffectType traverseEffect = thisCharacterInfoView->selectedEffectMode;
	int index = thisCharacterInfoView->selectedEffectIndex;
	int skipTotal = 0;

	if(thisCharacterInfoView->selectedEffectIndex == -1){
		return;
	}

	if(traverseEffect == ACTIVE_EFFECT_ABILITY){
			thisCharacterInfoView->effectDrawSkipCount = max(0, (skipTotal + index + 2) - thisCharacterInfoView->MAX_EFFECTS);
			return;
	}

	skipTotal += thisCharacterInfoView->thisIndividual->activeAbilities->numAbilities + 1; // plus 1 for "Active Animations"

	if(traverseEffect == ACTIVE_EFFECT_STATUS){
		thisCharacterInfoView->effectDrawSkipCount = max(0, (skipTotal + index + 2) - thisCharacterInfoView->MAX_EFFECTS);
		return;
	}

	skipTotal += thisCharacterInfoView->thisIndividual->activeStatuses->numStatuses + 1;

	if(traverseEffect == ACTIVE_EFFECT_FOOD){
		thisCharacterInfoView->effectDrawSkipCount = max(0, (skipTotal + index + 1) - thisCharacterInfoView->MAX_EFFECTS);
		return;
	}

	if(thisCharacterInfoView->thisIndividual->foodBonusActive || thisCharacterInfoView->thisIndividual->foodNegativeActive){
		skipTotal += 1;
	}

	if(traverseEffect == ACTIVE_EFFECT_EQUIPT_ITEM){
		thisCharacterInfoView->effectDrawSkipCount = max(0, (skipTotal + index + 2) - thisCharacterInfoView->MAX_EFFECTS);
		return;
	}

	skipTotal += thisCharacterInfoView->numEquiptItems + 1;

	if(traverseEffect == ACTIVE_EFFECT_ITEM){
		thisCharacterInfoView->effectDrawSkipCount = max(0, (skipTotal + index + 1) - thisCharacterInfoView->MAX_EFFECTS);
		return;
	}
}

void selectNextActiveEffectDown(){
	activeEffectType traverseEffect = thisCharacterInfoView->selectedEffectMode;
	int index = thisCharacterInfoView->selectedEffectIndex;

	if(thisCharacterInfoView->selectedEffectIndex == -1){
		return;
	}

	if(traverseEffect == ACTIVE_EFFECT_ABILITY){
		if(index + 1 < thisCharacterInfoView->thisIndividual->activeAbilities->numAbilities){
			thisCharacterInfoView->selectedEffectIndex = index + 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;

			return;
		}else{
			traverseEffect = ACTIVE_EFFECT_STATUS;
			index = -1;
		}
	}

	if(traverseEffect == ACTIVE_EFFECT_STATUS){
		if(index + 1 < thisCharacterInfoView->thisIndividual->activeStatuses->numStatuses){
			thisCharacterInfoView->selectedEffectIndex = index + 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;
			return;
		}else{
			traverseEffect = ACTIVE_EFFECT_FOOD;
			index = -1;
		}
	}

	if(traverseEffect == ACTIVE_EFFECT_FOOD){
		if(index == -1 && (thisCharacterInfoView->thisIndividual->foodBonusActive || thisCharacterInfoView->thisIndividual->foodBonusActive)){
			thisCharacterInfoView->selectedEffectIndex = index + 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;
			return;
		}else{
			traverseEffect = ACTIVE_EFFECT_EQUIPT_ITEM;
			index = -1;
		}
	}

	if(traverseEffect == ACTIVE_EFFECT_EQUIPT_ITEM){
		if(index + 1 < thisCharacterInfoView->numEquiptItems){
			thisCharacterInfoView->selectedEffectIndex = index + 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;
			return;
		}else{
			traverseEffect = ACTIVE_EFFECT_ITEM;
			index = -1;
		}
	}

	if(traverseEffect == ACTIVE_EFFECT_ITEM){
		if(index + 1 < thisCharacterInfoView->thisIndividual->activeItems->activeItemsTotal){
			thisCharacterInfoView->selectedEffectIndex = index + 1;
			thisCharacterInfoView->selectedEffectMode = traverseEffect;
			return;
		}
	}
}

void handleDownOnCharacterInfoView(){
	if(!inInfoView()){
		selectNextActiveEffectDown();
		calculateSkipIndex();
	}
}

void handleUpOnCharacterInfoView(){
	if(!inInfoView()){
		selectNextActiveEffectUp();
		calculateSkipIndex();
	}
}

void setCursorColor(HDC hdcBuffer, int value, int isBuff){
	if((value > 0 && isBuff) || (value < 0 && !isBuff)){
		SetTextColor(hdcBuffer, RGB(0, 162, 255));
	}else if((value < 0 && isBuff) || (value > 0 && !isBuff)){
		SetTextColor(hdcBuffer, RGB(255, 0, 0));
	}
}

void drawStatChangeCharacterInfo(HDC hdcBuffer, int magnitude, RECT * textBox){
	char tmpStr[32];

	setCursorColor(hdcBuffer, magnitude, 1);

	if(magnitude > 0){
		sprintf(tmpStr, "+%d", magnitude);
	}else{
		sprintf(tmpStr, "%d", magnitude);
	}

	DrawText(hdcBuffer, tmpStr, -1, textBox, DT_SINGLELINE);

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
}

void drawStatCharacterInfo(HDC hdcBuffer, int stat, int magnitude, char * outText, RECT * textBox){
	char value[32];
	int boxShift, magnitudeShift;
	textBox->top = textBox->top + 16;
	textBox->bottom = textBox->top + 16;

	DrawText(hdcBuffer, outText, -1, textBox, DT_SINGLELINE);

	if(stat >= 0){
		boxShift = 46;
	}else{
		boxShift = 42;
	}

	if(magnitude >= 0){
		magnitudeShift = 54;
	}else{
		magnitudeShift = 56;
	}


	sprintf(value, "%d", stat);

	textBox->left += boxShift;
	textBox->right += boxShift;

	DrawText(hdcBuffer, value, -1, textBox, DT_SINGLELINE);

	textBox->left -= boxShift;
	textBox->right -= boxShift;

	if(magnitude != 0){
		textBox->left += magnitudeShift;
		textBox->right += magnitudeShift;

		drawStatChangeCharacterInfo(hdcBuffer, magnitude, textBox);

		textBox->left -= magnitudeShift;
		textBox->right -= magnitudeShift;
	}
}

void drawDR(HDC hdcBuffer, int value, int index, RECT * textBox){
	SIZE size;
	int startingLocation = textBox->left;
	char drString[4];

	sprintf(drString, "%d", value);
	GetTextExtentPoint32(hdcBuffer, drString, strlen(drString), &size);

	textBox->left = textBox->left + 10 + index * 20 - (size.cx/2);
	textBox->right = textBox->left + 400;

	DrawText(hdcBuffer, drString, -1, textBox, DT_SINGLELINE);

	textBox->left = startingLocation;
	textBox->right = textBox->left + 400;

}

void drawInfoMode(HDC hdc, HDC hdcBuffer, RECT * rect, int xOff, int yOff){
	char value[32];
	int tmpNum = 0;
	SIZE size;

	individual * tmpIndividual = thisCharacterInfoView->thisIndividual;

	RECT textBox;
	textBox.top = yOff + 8;
	textBox.left = xOff + 10;
	textBox.bottom = textBox.top + 15;
	textBox.right = textBox.left + 256;

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisCharacterInfoView->infoWindow);
	drawUnboundCharacterByPixels(hdc, hdcBuffer,
			xOff + thisCharacterInfoView->infoWindow->fixedWidth - (thisCharacterInfoView->activeEffectsArrow->fixedWidth + 5),
			yOff + thisCharacterInfoView->infoWindow->fixedHeight - (thisCharacterInfoView->activeEffectsArrow->fixedHeight + 5),
			thisCharacterInfoView->activeEffectsArrow);

	drawUnboundCharacterByPixels(hdc, hdcBuffer,
						xOff + 25,
						yOff + 30,
						thisCharacterInfoView->frame);

	drawIndividualDefaultByPixels(hdc, hdcBuffer, tmpIndividual, xOff, yOff + 5, 0);

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	textBox.left = xOff + 20;

	DrawText(hdcBuffer, tmpIndividual->name, -1, &textBox, DT_SINGLELINE);

	textBox.top = textBox.top + 75;
	textBox.bottom = textBox.top + 15;

	DrawText(hdcBuffer, "Level: X", -1, &textBox, DT_SINGLELINE);

	textBox.top = textBox.top + 20;
	textBox.bottom = textBox.top + 15;
	textBox.left = xOff + 10;
	DrawText(hdcBuffer, "STATS:", -1, &textBox, DT_SINGLELINE);

	textBox.left = xOff + 20;

	drawStatCharacterInfo(hdcBuffer, tmpIndividual->STR, getAttributeSum(tmpIndividual, "STR") - tmpIndividual->STR, "STR:", &textBox);
	drawStatCharacterInfo(hdcBuffer, tmpIndividual->DEX, getAttributeSum(tmpIndividual, "DEX") - tmpIndividual->DEX, "DEX:", &textBox);
	drawStatCharacterInfo(hdcBuffer, tmpIndividual->CON, getAttributeSum(tmpIndividual, "CON") - tmpIndividual->CON, "CON:", &textBox);
	drawStatCharacterInfo(hdcBuffer, tmpIndividual->INT, getAttributeSum(tmpIndividual, "INT") - tmpIndividual->INT, "INT:", &textBox);
	drawStatCharacterInfo(hdcBuffer, tmpIndividual->WIS, getAttributeSum(tmpIndividual, "WIS") - tmpIndividual->WIS, "WIS:", &textBox);
	drawStatCharacterInfo(hdcBuffer, tmpIndividual->WILL, getAttributeSum(tmpIndividual, "WILL") - tmpIndividual->WILL, "WILL:", &textBox);
	drawStatCharacterInfo(hdcBuffer, tmpIndividual->CHR, getAttributeSum(tmpIndividual, "CHR") - tmpIndividual->CHR, "CHR:", &textBox);
	drawStatCharacterInfo(hdcBuffer, tmpIndividual->LUCK, getAttributeSum(tmpIndividual, "LUCK") - tmpIndividual->LUCK, "LUCK:", &textBox);

	//begin drawing right column
	textBox.top = yOff + 30;
	textBox.bottom = textBox.top + 16;
	textBox.left = xOff + 100;
	textBox.right = textBox.left + 400;

	sprintf(value, "HP: %d/%d", tmpIndividual->hp, (getAttributeSum(tmpIndividual, "baseHP") + getAttributeSum(tmpIndividual, "CON") * 2));
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);

	textBox.left += 75;

	sprintf(value, "Mana: %d/%d", tmpIndividual->mana, (getAttributeSum(tmpIndividual, "baseMana") + getAttributeSum(tmpIndividual, "WILL") * 2));
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);

	textBox.left -= 75;
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 16;

	sprintf(value, "Actions: %d/%d", tmpIndividual->remainingActions, tmpIndividual->totalActions);
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 16;

	sprintf(value, "Attack: %d", getAttributeSum(tmpIndividual, "attack"));
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 16;

	sprintf(value, "AC: %d", getAttributeSum(tmpIndividual, "ac"));
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 16;

	sprintf(value, "Dam: [%d-%d]", calcMinDam(tmpIndividual), calcMaxDam(tmpIndividual));

	if(tmpIndividual->baseDam != 0){
		sprintf(value + strlen(value), " + %d", tmpIndividual->baseDam);
	}

	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 16;

	sprintf(value, "Crit: %s", tmpIndividual->critType);
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 16;

	sprintf(value, "Food: %d/%d", ((int)tmpIndividual->food), getAttributeSum(tmpIndividual, "baseFood"));
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 16;

	sprintf(value, "Mvmt: %d", getAttributeSum(tmpIndividual, "mvmt"));
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 20;

	sprintf(value, "Dark Sight: %d", getAttributeSum(tmpIndividual, "darkLoS"));
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 17;

	//DR
	DrawText(hdcBuffer, "Damage Reduction", -1, &textBox, DT_SINGLELINE);
	textBox.top = textBox.top + 17;
	textBox.bottom = textBox.top + 17;

	drawUnboundCharacterByPixels(hdc, hdcBuffer, textBox.left, textBox.top, thisCharacterInfoView->drChart);
	textBox.top = textBox.top + 16;
	textBox.bottom = textBox.top + 16;

	drawDR(hdcBuffer, getAttributeSum(tmpIndividual, "bluntDR"), 0, &textBox);
	drawDR(hdcBuffer, getAttributeSum(tmpIndividual, "chopDR"), 1, &textBox);
	drawDR(hdcBuffer, getAttributeSum(tmpIndividual, "pierceDR"), 2, &textBox);
	drawDR(hdcBuffer, getAttributeSum(tmpIndividual, "slashDR"), 3, &textBox);

	drawDR(hdcBuffer, getAttributeSum(tmpIndividual, "fireDR"), 4, &textBox);
	drawDR(hdcBuffer, getAttributeSum(tmpIndividual, "waterDR"), 5, &textBox);
	drawDR(hdcBuffer, getAttributeSum(tmpIndividual, "lightningDR"), 6, &textBox);
	drawDR(hdcBuffer, getAttributeSum(tmpIndividual, "earthDR"), 7, &textBox);

	textBox.top = textBox.top + 17;
	textBox.bottom = textBox.top + 17;
	sprintf(value, "Gold: %d", tmpIndividual->gold);
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);

	textBox.left += 80;
	textBox.right += 80;
	sprintf(value, "Bounty: %d", getCurrentBounty(tmpIndividual));
	DrawText(hdcBuffer, value, -1, &textBox, DT_SINGLELINE);


	SetTextColor(hdcBuffer, RGB(0, 0, 0));
}

void drawStatus(HDC hdcBuffer, status * thisStatus, RECT * textBox){
	char value[32];
	SetTextColor(hdcBuffer, RGB(255, 0, 0));

	sprintf(value, "Turns: %d", thisStatus->turnsRemaining);
	DrawText(hdcBuffer, value, -1, textBox, DT_SINGLELINE);
				textBox->top = textBox->top + 12;
				textBox->bottom = textBox->bottom + 12;

	value[0] = '\0';

	if(thisStatus->diceDamage > 0 || thisStatus->damage > 0){
		strcpy(value, "Dam: ");
		if(thisStatus->diceDamage > 0){
			sprintf(value + strlen(value), "[1-%d]", thisStatus->diceDamage);
		}

		if(thisStatus->diceDamage > 0 && thisStatus->damage > 0){
			sprintf(value + strlen(value), " + ");
		}

		if(thisStatus->damage > 0){
			sprintf(value + strlen(value), "%d", thisStatus->damage);
		}

		DrawText(hdcBuffer, value, -1, textBox, DT_SINGLELINE);
			textBox->top = textBox->top + 12;
			textBox->bottom = textBox->bottom + 12;
	}

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
}

void drawFoodEffect(HDC hdc, HDC hdcBuffer, RECT * textBoxRect, RECT * effectRect, int xOff){
	char foodStr[16];

	if(thisCharacterInfoView->thisIndividual->foodBonusActive){
		strcpy(foodStr, "Well Fed");

		if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_FOOD){
			SetTextColor(hdcBuffer, RGB(0, 162, 255));
			DrawText(hdcBuffer, "STR: +1", -1, effectRect, DT_SINGLELINE);
				effectRect->top = effectRect->top + 12;
				effectRect->bottom = effectRect->bottom + 12;
			DrawText(hdcBuffer, "CON: +1", -1, effectRect, DT_SINGLELINE);
				effectRect->top = effectRect->top + 12;
				effectRect->bottom = effectRect->bottom + 12;
			DrawText(hdcBuffer, "DEX: +1", -1, effectRect, DT_SINGLELINE);
				effectRect->top = effectRect->top + 12;
				effectRect->bottom = effectRect->bottom + 12;
		}

	}else if(thisCharacterInfoView->thisIndividual->foodNegativeActive){
		strcpy(foodStr, "Starving");

		if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_FOOD){
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			DrawText(hdcBuffer, "STR: -1", -1, effectRect, DT_SINGLELINE);
				effectRect->top = effectRect->top + 12;
				effectRect->bottom = effectRect->bottom + 12;
			DrawText(hdcBuffer, "CON: -1", -1, effectRect, DT_SINGLELINE);
				effectRect->top = effectRect->top + 12;
				effectRect->bottom = effectRect->bottom + 12;
			DrawText(hdcBuffer, "DEX: -1", -1, effectRect, DT_SINGLELINE);
				effectRect->top = effectRect->top + 12;
				effectRect->bottom = effectRect->bottom + 12;
		}
	}

	SetTextColor(hdcBuffer, RGB(255, 200, 0));

	DrawText(hdcBuffer, foodStr, -1, textBoxRect, DT_SINGLELINE);

	if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_FOOD){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,
				xOff + 7,
				textBoxRect->top,
				thisCharacterInfoView->selectArrow);
	}

	textBoxRect->top = textBoxRect->top + 12;
	textBoxRect->bottom = textBoxRect->bottom + 12;
}

void drawEffectsMode(HDC hdc, HDC hdcBuffer, RECT * rect, int xOff, int yOff){
	int i, effectsPassed = 0, titleEffectsDrawn = 0, hitEnd = 0;
	RECT textBoxRect;
	RECT effectRect;

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisCharacterInfoView->activeEffectsWindow);
	drawUnboundCharacterByPixels(hdc, hdcBuffer,
			xOff + 5,
			yOff + thisCharacterInfoView->activeEffectsWindow->fixedHeight - (thisCharacterInfoView->characterInfoArrow->fixedHeight + 5),
			thisCharacterInfoView->characterInfoArrow);

	if(thisCharacterInfoView->effectDrawSkipCount > 0){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,
				xOff + 68 - (thisCharacterInfoView->scrollUpArrow->fixedWidth / 2),
				yOff + 9,
				thisCharacterInfoView->scrollUpArrow);
	}

	textBoxRect.top = yOff + 10 + thisCharacterInfoView->scrollUpArrow->fixedHeight;
		textBoxRect.bottom = textBoxRect.top + 30;
		textBoxRect.left = xOff + 10;
		textBoxRect.right = textBoxRect.left + 200;

	effectRect.top = yOff + 10;
		effectRect.bottom = effectRect.top + 30;
		effectRect.left = xOff + 140;
		effectRect.right = effectRect.left + 200;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount >= 0 && titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount < thisCharacterInfoView->MAX_EFFECTS){
		DrawText(hdcBuffer, "Active Abilities", -1, &textBoxRect, DT_SINGLELINE);
		textBoxRect.top = textBoxRect.top + 15;
		textBoxRect.bottom = textBoxRect.bottom + 15;
	}

	titleEffectsDrawn++;

	textBoxRect.left = textBoxRect.left + 5 + thisCharacterInfoView->selectArrow->fixedWidth;

	for(i = 0; i < thisCharacterInfoView->thisIndividual->activeAbilities->MAX_ABILITIES; i++){
		activeAbility * tmpAbility = thisCharacterInfoView->thisIndividual->activeAbilities->abilitiesList[i];

		if(tmpAbility != NULL){
			if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount >= 0 && titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount < thisCharacterInfoView->MAX_EFFECTS){
				DrawText(hdcBuffer, tmpAbility->thisAbility->name, -1, &textBoxRect, DT_SINGLELINE);

				if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_ABILITY && effectsPassed == thisCharacterInfoView->selectedEffectIndex){
					drawAbilityEffects(hdcBuffer, tmpAbility->thisAbility, &effectRect);
					drawUnboundCharacterByPixels(hdc, hdcBuffer,
							xOff + 7,
							textBoxRect.top,
							thisCharacterInfoView->selectArrow);
				}

				textBoxRect.top = textBoxRect.top + 15;
				textBoxRect.bottom = textBoxRect.bottom + 15;
			}else if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount == thisCharacterInfoView->MAX_EFFECTS){
				hitEnd = 1;
			}

			titleEffectsDrawn++;
			effectsPassed++;

			if(effectsPassed == thisCharacterInfoView->thisIndividual->activeAbilities->numAbilities){
				break;
			}
		}
	}

	effectsPassed = 0;

	if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount >= 0 && titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount < thisCharacterInfoView->MAX_EFFECTS){
		textBoxRect.left = textBoxRect.left - (5 + thisCharacterInfoView->selectArrow->fixedWidth);
		DrawText(hdcBuffer, "Active Statuses", -1, &textBoxRect, DT_SINGLELINE);

		textBoxRect.left = textBoxRect.left + 5 + thisCharacterInfoView->selectArrow->fixedWidth;
		textBoxRect.top = textBoxRect.top + 15;
		textBoxRect.bottom = textBoxRect.bottom + 15;
	}else if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount == thisCharacterInfoView->MAX_EFFECTS){
		hitEnd = 1;
	}

	titleEffectsDrawn++;

	for(i = 0; i < thisCharacterInfoView->thisIndividual->activeStatuses->numStatuses; i++){
		status * tmpStatus = thisCharacterInfoView->thisIndividual->activeStatuses->statuses[i];

		if(tmpStatus != NULL){
			if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount >= 0 && titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount < thisCharacterInfoView->MAX_EFFECTS){
				DrawText(hdcBuffer, lookUpStatusEffectName(tmpStatus->effect, 1), -1, &textBoxRect, DT_SINGLELINE);

				if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_STATUS && effectsPassed == thisCharacterInfoView->selectedEffectIndex){
					drawStatus(hdcBuffer, tmpStatus, &effectRect);
					drawUnboundCharacterByPixels(hdc, hdcBuffer,
											xOff + 7,
											textBoxRect.top,
											thisCharacterInfoView->selectArrow);
				}

				textBoxRect.top = textBoxRect.top + 15;
				textBoxRect.bottom = textBoxRect.bottom + 15;
			}else if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount == thisCharacterInfoView->MAX_EFFECTS){
				hitEnd = 1;
			}

			titleEffectsDrawn++;
			effectsPassed++;

			if(effectsPassed == thisCharacterInfoView->thisIndividual->activeStatuses->numStatuses){
				break;
			}
		}
	}

	effectsPassed = 0;

	if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount >= 0 && titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount < thisCharacterInfoView->MAX_EFFECTS){
		drawFoodEffect(hdc, hdcBuffer, &textBoxRect, &effectRect, xOff);
	}else if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount == thisCharacterInfoView->MAX_EFFECTS){
		hitEnd = 1;
	}

	titleEffectsDrawn++;

	if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount >= 0 && titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount < thisCharacterInfoView->MAX_EFFECTS){
		textBoxRect.left = textBoxRect.left - (5 + thisCharacterInfoView->selectArrow->fixedWidth);
		DrawText(hdcBuffer, "Active Items", -1, &textBoxRect, DT_SINGLELINE);

		textBoxRect.left = textBoxRect.left + 5 + thisCharacterInfoView->selectArrow->fixedWidth;
		textBoxRect.top = textBoxRect.top + 15;
		textBoxRect.bottom = textBoxRect.bottom + 15;
	}else if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount == thisCharacterInfoView->MAX_EFFECTS){
		hitEnd = 1;
	}

	titleEffectsDrawn++;

	for(i = 0; i < thisCharacterInfoView->numEquiptItems; i++){
		if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount >= 0 && titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount < thisCharacterInfoView->MAX_EFFECTS){
			DrawText(hdcBuffer, thisCharacterInfoView->equippedItems[i]->name, -1, &textBoxRect, DT_SINGLELINE);

			if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_EQUIPT_ITEM && i == thisCharacterInfoView->selectedEffectIndex){
				drawItem(hdcBuffer, thisCharacterInfoView->equippedItems[i], 0, &effectRect);
				drawUnboundCharacterByPixels(hdc, hdcBuffer,
										xOff + 7,
										textBoxRect.top,
										thisCharacterInfoView->selectArrow);
			}

			textBoxRect.top = textBoxRect.top + 15;
			textBoxRect.bottom = textBoxRect.bottom + 15;
		}else if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount == thisCharacterInfoView->MAX_EFFECTS){
			hitEnd = 1;
		}

		titleEffectsDrawn++;
	}

	for(i = 0; i < thisCharacterInfoView->thisIndividual->activeItems->activeItemsTotal; i++){
		activeItem * tmpItem = thisCharacterInfoView->thisIndividual->activeItems->activeItemArr[i];

		if(tmpItem != NULL){
			if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount >= 0 && titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount < thisCharacterInfoView->MAX_EFFECTS){
				DrawText(hdcBuffer, tmpItem->thisItem->name, -1, &textBoxRect, DT_SINGLELINE);

				if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_ITEM && effectsPassed == thisCharacterInfoView->selectedEffectIndex){
					drawItem(hdcBuffer, tmpItem->thisItem, tmpItem->remaningTurns, &effectRect);
					drawUnboundCharacterByPixels(hdc, hdcBuffer,
											xOff + 7,
											textBoxRect.top,
											thisCharacterInfoView->selectArrow);
				}

				textBoxRect.top = textBoxRect.top + 15;
				textBoxRect.bottom = textBoxRect.bottom + 15;
			}else if(titleEffectsDrawn - thisCharacterInfoView->effectDrawSkipCount == thisCharacterInfoView->MAX_EFFECTS){
				hitEnd = 1;
			}

			titleEffectsDrawn++;
			effectsPassed++;

			if(effectsPassed == thisCharacterInfoView->thisIndividual->activeItems->activeItemsTotal){
				break;
			}
		}
	}

	effectsPassed = 0;

	SetTextColor(hdcBuffer, RGB(0, 0, 0));

	if(hitEnd){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,
			xOff + 68 - (thisCharacterInfoView->scrollDownArrow->fixedWidth / 2),
			yOff + thisCharacterInfoView->activeEffectsWindow->fixedHeight - (15 + thisCharacterInfoView->scrollDownArrow->fixedHeight * 2),
			thisCharacterInfoView->scrollDownArrow);
	}
}

void drawCharacterInfoView(HDC hdc, HDC hdcBuffer, RECT * rect){
	int xOff = rect->right * 0.25;
	int yOff = rect->bottom * 0.25;

	if(thisCharacterInfoView->inInfoView){
		drawInfoMode(hdc, hdcBuffer, rect, xOff, yOff);
	}else{
		drawEffectsMode(hdc, hdcBuffer, rect, xOff, yOff);
	}

}
