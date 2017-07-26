/*
 * ability_view.c
 *
 *  Created on: Apr 1, 2016
 *      Author: Adrian
 */
#include"./headers/abilities_pub_methods.h"

static abilityView * thisAbilityView;

void initThisAbilityView(int imageID, COLORREF rgb, int x, int y){
	thisAbilityView = malloc(sizeof(abilityView));

	thisAbilityView->inAbiltyViewMode = 0;
	thisAbilityView->numAbilites = 0;
	thisAbilityView->MAX_ABILITIES = 64;

	thisAbilityView->currentAbilityIndex = 0;
	thisAbilityView->startingAbilityIndex = 0;
	thisAbilityView->endingAbilityIndex = 0;
	thisAbilityView->MAX_ABILITIES_ON_WINDOW =3;

	thisAbilityView->abilityViewWindow = createCharacter(imageID, rgb, x, y);
	thisAbilityView->selector = createCharacter(1504, rgb, x, y);
}

void toggleAbilityViewMode(){
	thisAbilityView->inAbiltyViewMode = (thisAbilityView->inAbiltyViewMode + 1) % 2;
}

int inAbilityViewMode(){
	if(thisAbilityView != NULL){
		return thisAbilityView->inAbiltyViewMode;
	}else{
		return 0;
	}
}

void clearAblitiesView(){
	int i;

	for(i = 0; i < thisAbilityView->numAbilites; i++){
		thisAbilityView->abilitiesList[i] = NULL;
	}

	thisAbilityView->numAbilites = 0;
}

void refreshAbilityView(int numAbilities, ability * abilitiesList[64]){
	int i;

	clearAblitiesView();

	for(i = 0; i < numAbilities; i++){
		if(i < thisAbilityView->MAX_ABILITIES){
			thisAbilityView->abilitiesList[i] = abilitiesList[i];
			thisAbilityView->numAbilites++;
		} else {
			cwrite("!! CANNOT ADD MORE ABILITES TO THE VIEW !!");
			break;
		}
	}

	thisAbilityView->endingAbilityIndex = min(thisAbilityView->numAbilites, thisAbilityView->MAX_ABILITIES_ON_WINDOW);
}

void resetAbilityView(){
	thisAbilityView->currentAbilityIndex = 0;
	thisAbilityView->startingAbilityIndex = 0;
	thisAbilityView->endingAbilityIndex = 0;
}

void drawThisAbilityView(HDC hdc, HDC hdcBuffer, RECT * prc){
	int i;
	HDC hdcMem = CreateCompatibleDC(hdc);

	RECT textRect;
	textRect.top = thisAbilityView->abilityViewWindow->y + 30;
	textRect.left = thisAbilityView->abilityViewWindow->x + 30;
	textRect.bottom = textRect.top + 40;
	textRect.right = textRect.left + 240;

	RECT effectRect;
	effectRect.top = thisAbilityView->abilityViewWindow->y + 5;
	effectRect.left = thisAbilityView->abilityViewWindow->x + 185;
	effectRect.bottom = effectRect.top + 40;
	effectRect.right = effectRect.left + 300;

	RECT manaRect;
	manaRect.top = thisAbilityView->abilityViewWindow->y + 10;
	manaRect.left = thisAbilityView->abilityViewWindow->x + 80;
	manaRect.bottom = manaRect.top + 20;
	manaRect.right = manaRect.left + 100;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));

	SelectObject(hdcMem, thisAbilityView->abilityViewWindow->fixedImage);
	BitBlt(hdcBuffer, thisAbilityView->abilityViewWindow->x, thisAbilityView->abilityViewWindow->y, thisAbilityView->abilityViewWindow->fixedWidth, thisAbilityView->abilityViewWindow->fixedHeight, hdcMem, 0, 0, SRCCOPY);



	for(i = thisAbilityView->startingAbilityIndex; i < thisAbilityView->endingAbilityIndex; i++){
		DrawText(hdcBuffer, thisAbilityView->abilitiesList[i]->name, strlen(thisAbilityView->abilitiesList[i]->name), &textRect, DT_SINGLELINE);
		if(thisAbilityView->currentAbilityIndex == i){
			drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect.left - 20,textRect.top,thisAbilityView->selector);
			drawAbilityEffects(hdcBuffer, thisAbilityView->abilitiesList[i], &effectRect);
			drawManaCost(hdcBuffer, thisAbilityView->abilitiesList[i], &manaRect);
		}
		moveRECTDown(&textRect, 17);
	}

	SetTextColor(hdcBuffer, RGB(0, 0, 0));

	DeleteDC(hdcMem);
}


void selectPreviousAbility(){
	if(thisAbilityView->currentAbilityIndex == thisAbilityView->startingAbilityIndex &&
			thisAbilityView->currentAbilityIndex > 0){
		shiftAbilityListDown();
	}else if(thisAbilityView->currentAbilityIndex > 0){
		thisAbilityView->currentAbilityIndex--;
	}

}

void selectNextAbility(){
	if(thisAbilityView->currentAbilityIndex + 1 == thisAbilityView->endingAbilityIndex &&
			thisAbilityView->endingAbilityIndex < thisAbilityView->numAbilites){
		shiftAbilityListUp();
	}else if (thisAbilityView->currentAbilityIndex + 1 < thisAbilityView->endingAbilityIndex){
		thisAbilityView->currentAbilityIndex++;
	}
}

void shiftAbilityListUp(){
	thisAbilityView->startingAbilityIndex++;
	thisAbilityView->endingAbilityIndex++;
	thisAbilityView->currentAbilityIndex++;
}

void shiftAbilityListDown(){
	thisAbilityView->startingAbilityIndex--;
	thisAbilityView->endingAbilityIndex--;
	thisAbilityView->currentAbilityIndex--;
}

ability * getAbilityToActivate(){
	return thisAbilityView->abilitiesList[thisAbilityView->currentAbilityIndex];
}

void drawAbilityEffects(HDC hdcBuffer, ability * thisAbility,RECT * effectRect){
	drawSpecificType(hdcBuffer, thisAbility->damageType, thisAbility->damageTypeEnabled, "damageType", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->range, thisAbility->rangeEnabled, "range", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->targeted, thisAbility->targetedEnabled, "targeted", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->extraAttack, thisAbility->extraAttackEnabled, "extraAttack", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->diceDamage, thisAbility->diceDamageEnabled, "diceDamage", effectRect);

	if(thisAbility->diceDamageEnabled && thisAbility->diceDamage->effectAndManaArray[thisAbility->diceDamage->selectedIndex]->effectMagnitude != 0){
		drawSpecificEffect(hdcBuffer, thisAbility->diceDamageMultiplier, thisAbility->diceDamageMultiplierEnabled, "diceDamageMultiplier", effectRect);
	}

	drawSpecificEffect(hdcBuffer, thisAbility->damage, thisAbility->damageEnabled, "damage", effectRect);
	drawSpecificType(hdcBuffer, thisAbility->status, thisAbility->statusEnabled, "status", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->statusDiceDamage, thisAbility->statusDiceDamageEnabled, "statusDiceDamage", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->statusDamage, thisAbility->statusDamageEnabled, "statusDamage", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->diceStatusDuration, thisAbility->diceStatusDurationEnabled, "statusDiceDuration", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->statusDuration, thisAbility->statusDurationEnabled, "statusDuration", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->aoe, thisAbility->aoeEnabled, "aoe", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->duration, thisAbility->durationEnabled, "duration", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->durationMod, thisAbility->durationModEnabled, "durationMod", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->actions, thisAbility->actionsEnabled, "Actions", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->ac, thisAbility->acEnabled, "ac", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->attack, thisAbility->attackEnabled, "attack", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->damageMod, thisAbility->damageModEnabled, "damageMod", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->mvmt, thisAbility->mvmtEnabled, "mvmt", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->diceHP, thisAbility->diceHPEnabled, "diceHP", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->hp, thisAbility->hpEnabled, "hp", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->baseHP, thisAbility->baseHPEnabled, "baseHP", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->baseMana, thisAbility->baseManaEnabled, "baseMana", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->bluntDR, thisAbility->bluntDREnabled, "bluntDR", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->chopDR, thisAbility->chopDREnabled, "chopDR", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->pierceDR, thisAbility->pierceDREnabled, "pierceDR", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->slashDR, thisAbility->slashDREnabled, "slashDR", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->earthDR, thisAbility->earthDREnabled, "earthDR", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->fireDR, thisAbility->fireDREnabled, "fireDR", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->waterDR, thisAbility->waterDREnabled, "waterDR", effectRect);
	drawSpecificEffect(hdcBuffer, thisAbility->lightningDR, thisAbility->lightningDREnabled, "lightningDR", effectRect);

}

void drawSpecificEffect(HDC hdcBuffer, effectAndManaMapList * mapList, int isEnabled, char * name , RECT * effectRect){
	if(isEnabled){
		int magnitude = mapList->effectAndManaArray[mapList->selectedIndex]->effectMagnitude;
		int cost = mapList->effectAndManaArray[mapList->selectedIndex]->manaCost;
		if (magnitude != 0) {

			char * outStr[64];

			if(cost > 0){
				SetTextColor(hdcBuffer, RGB(0, 162, 255));
			}else if (cost < 0){
				SetTextColor(hdcBuffer, RGB(255, 0, 0));
			}

			if(magnitude > 0){
				sprintf(outStr, "%s: +%d",name, magnitude);
			}else{
				sprintf(outStr, "%s: %d",name, magnitude);
			}

			DrawText(hdcBuffer, outStr, strlen(outStr), effectRect, DT_SINGLELINE);

			SetTextColor(hdcBuffer, RGB(255, 200, 0));

			moveRECTDown(effectRect, 14);
		}
	}
}

void drawSpecificType(HDC hdcBuffer, typeAndManaMapList * mapList, int isEnabled, char * name , RECT * effectRect){
	if(isEnabled){
		char * type[16];
		strcpy(type, mapList->typeAndManaArray[mapList->selectedIndex]->type);
		int cost = mapList->typeAndManaArray[mapList->selectedIndex]->manaCost;
		if (strcmp(type,"None") != 0) {

			char * outStr[64];

			if(cost > 0){
				SetTextColor(hdcBuffer, RGB(0, 162, 255));
			}else if (cost < 0){
				SetTextColor(hdcBuffer, RGB(255, 0, 0));
			}

			sprintf(outStr, "%s: %s",name, type);

			DrawText(hdcBuffer, outStr, strlen(outStr), effectRect, DT_SINGLELINE);

			SetTextColor(hdcBuffer, RGB(255, 200, 0));

			moveRECTDown(effectRect, 14);
		}
	}
}
