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

	thisCharacterInfoView->infoWindow = createCharacter(1525,RGB(255,0,255),0,0);
	thisCharacterInfoView->activeEffectsWindow = createCharacter(1526,RGB(255,0,255),0,0);
	thisCharacterInfoView->activeEffectsArrow = createCharacter(1528,RGB(255,0,255),0,0);
	thisCharacterInfoView->characterInfoArrow = createCharacter(1530,RGB(255,0,255),0,0);
	thisCharacterInfoView->levelUpArrow = createCharacter(1529,RGB(255,0,255),0,0);
	thisCharacterInfoView->selectArrow = createCharacter(1504,RGB(255,0,255),0,0);
	thisCharacterInfoView->scrollUpArrow = createCharacter(1505,RGB(255,0,255),0,0);
	thisCharacterInfoView->scrollDownArrow = createCharacter(1507,RGB(255,0,255),0,0);
	thisCharacterInfoView->drChart = createCharacter(1527,RGB(255,0,255),0,0);
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
	return thisCharacterInfoView->inCharacterInfoView;
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

void drawInfoMode(HDC hdc, HDC hdcBuffer, RECT * rect, int xOff, int yOff){
	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisCharacterInfoView->infoWindow);
	drawUnboundCharacterByPixels(hdc, hdcBuffer,
			xOff + thisCharacterInfoView->infoWindow->fixedWidth - (thisCharacterInfoView->activeEffectsArrow->fixedWidth + 5),
			yOff + thisCharacterInfoView->infoWindow->fixedHeight - (thisCharacterInfoView->activeEffectsArrow->fixedHeight + 5),
			thisCharacterInfoView->activeEffectsArrow);


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

	}else if(thisCharacterInfoView->thisIndividual->foodBonusActive){
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
	int i, effectsPassed = 0;
	RECT textBoxRect;
	RECT effectRect;

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisCharacterInfoView->activeEffectsWindow);
	drawUnboundCharacterByPixels(hdc, hdcBuffer,
			xOff + 5,
			yOff + thisCharacterInfoView->activeEffectsWindow->fixedHeight - (thisCharacterInfoView->characterInfoArrow->fixedHeight + 5),
			thisCharacterInfoView->characterInfoArrow);

	textBoxRect.top = yOff + 10;
		textBoxRect.bottom = textBoxRect.top + 30;
		textBoxRect.left = xOff + 10;
		textBoxRect.right = textBoxRect.left + 200;

	effectRect.top = yOff + 10;
		effectRect.bottom = effectRect.top + 30;
		effectRect.left = xOff + 140;
		effectRect.right = effectRect.left + 200;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);
	DrawText(hdcBuffer, "Active Abilities", -1, &textBoxRect, DT_SINGLELINE);

	textBoxRect.left = textBoxRect.left + 5 + thisCharacterInfoView->selectArrow->fixedWidth;
	textBoxRect.top = textBoxRect.top + 12;
	textBoxRect.bottom = textBoxRect.bottom + 12;

	for(i = 0; i < thisCharacterInfoView->thisIndividual->activeAbilities->MAX_ABILITIES; i++){
		activeAbility * tmpAbility = thisCharacterInfoView->thisIndividual->activeAbilities->abilitiesList[i];

		if(tmpAbility != NULL){
			DrawText(hdcBuffer, tmpAbility->thisAbility->name, -1, &textBoxRect, DT_SINGLELINE);

			if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_ABILITY && effectsPassed == thisCharacterInfoView->selectedEffectIndex){
				drawAbilityEffects(hdcBuffer, tmpAbility->thisAbility, &effectRect);
				drawUnboundCharacterByPixels(hdc, hdcBuffer,
						xOff + 7,
						textBoxRect.top,
						thisCharacterInfoView->selectArrow);
			}

			textBoxRect.top = textBoxRect.top + 12;
			textBoxRect.bottom = textBoxRect.bottom + 12;

			effectsPassed++;

			if(effectsPassed == thisCharacterInfoView->thisIndividual->activeAbilities->numAbilities){
				break;
			}
		}
	}

	effectsPassed = 0;

	textBoxRect.left = textBoxRect.left - (5 + thisCharacterInfoView->selectArrow->fixedWidth);
	DrawText(hdcBuffer, "Active Statuses", -1, &textBoxRect, DT_SINGLELINE);

	textBoxRect.left = textBoxRect.left + 5 + thisCharacterInfoView->selectArrow->fixedWidth;
	textBoxRect.top = textBoxRect.top + 12;
	textBoxRect.bottom = textBoxRect.bottom + 12;

	for(i = 0; i < thisCharacterInfoView->thisIndividual->activeStatuses->numStatuses; i++){
		status * tmpStatus = thisCharacterInfoView->thisIndividual->activeStatuses->statuses[i];

		if(tmpStatus != NULL){
			DrawText(hdcBuffer, lookUpStatusEffectName(tmpStatus->effect, 1), -1, &textBoxRect, DT_SINGLELINE);

			if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_STATUS && effectsPassed == thisCharacterInfoView->selectedEffectIndex){
				drawStatus(hdcBuffer, tmpStatus, &effectRect);
				drawUnboundCharacterByPixels(hdc, hdcBuffer,
										xOff + 7,
										textBoxRect.top,
										thisCharacterInfoView->selectArrow);
			}

			textBoxRect.top = textBoxRect.top + 12;
			textBoxRect.bottom = textBoxRect.bottom + 12;

			effectsPassed++;

			if(effectsPassed == thisCharacterInfoView->thisIndividual->activeStatuses->numStatuses){
				break;
			}
		}
	}

	effectsPassed = 0;

	drawFoodEffect(hdc, hdcBuffer, &textBoxRect, &effectRect, xOff);

	textBoxRect.left = textBoxRect.left - (5 + thisCharacterInfoView->selectArrow->fixedWidth);
	DrawText(hdcBuffer, "Active Items", -1, &textBoxRect, DT_SINGLELINE);

	textBoxRect.left = textBoxRect.left + 5 + thisCharacterInfoView->selectArrow->fixedWidth;
	textBoxRect.top = textBoxRect.top + 12;
	textBoxRect.bottom = textBoxRect.bottom + 12;

	for(i = 0; i < thisCharacterInfoView->numEquiptItems; i++){
		DrawText(hdcBuffer, thisCharacterInfoView->equippedItems[i]->name, -1, &textBoxRect, DT_SINGLELINE);

		if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_EQUIPT_ITEM && i == thisCharacterInfoView->selectedEffectIndex){
			drawItem(hdcBuffer, thisCharacterInfoView->equippedItems[i], 0, &effectRect);
			drawUnboundCharacterByPixels(hdc, hdcBuffer,
									xOff + 7,
									textBoxRect.top,
									thisCharacterInfoView->selectArrow);
		}

		textBoxRect.top = textBoxRect.top + 12;
		textBoxRect.bottom = textBoxRect.bottom + 12;
	}

	for(i = 0; i < thisCharacterInfoView->thisIndividual->activeItems->activeItemsTotal; i++){
		activeItem * tmpItem = thisCharacterInfoView->thisIndividual->activeItems->activeItemArr[i];

		if(tmpItem != NULL){
			DrawText(hdcBuffer, tmpItem->thisItem->name, -1, &textBoxRect, DT_SINGLELINE);

			if(thisCharacterInfoView->selectedEffectMode == ACTIVE_EFFECT_ITEM && effectsPassed == thisCharacterInfoView->selectedEffectIndex){
				drawItem(hdcBuffer, tmpItem->thisItem, tmpItem->remaningTurns, &effectRect);
				drawUnboundCharacterByPixels(hdc, hdcBuffer,
										xOff + 7,
										textBoxRect.top,
										thisCharacterInfoView->selectArrow);
			}

			textBoxRect.top = textBoxRect.top + 12;
			textBoxRect.bottom = textBoxRect.bottom + 12;

			effectsPassed++;

			if(effectsPassed == thisCharacterInfoView->thisIndividual->activeItems->activeItemsTotal){
				break;
			}
		}
	}

	effectsPassed = 0;

	SetTextColor(hdcBuffer, RGB(0, 0, 0));
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
