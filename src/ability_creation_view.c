/*
 * ability_creation_view.c
 *
 *  Created on: Apr 1, 2016
 *      Author: Adrian
 */
#include"./headers/abilities_pub_methods.h"
#include<string.h>
#include<stdio.h>

static abilityCreationInstance * thisAbilityCreationInstance;

void initAbilityCreationInstance(int imageID, COLORREF rgb, int x, int y, char* directory, char* effectsFileNam){
	int i;

	thisAbilityCreationInstance = malloc(sizeof(abilityCreationInstance));
	thisAbilityCreationInstance->inCreateMode = 0;
	thisAbilityCreationInstance->inNameMode = 0;
	thisAbilityCreationInstance->templateIndex = 0;

	thisAbilityCreationInstance->waitingForName = 0;

	thisAbilityCreationInstance->currentTemplateIndex = 0;
	thisAbilityCreationInstance->MAX_ABILITY_TEMPLATES = 3;
	thisAbilityCreationInstance->numAbilityTemplates = 0;

	thisAbilityCreationInstance->effectCurrentIndex = 0;
	thisAbilityCreationInstance->effectStartingIndex = 0;
	thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW = 15;
	thisAbilityCreationInstance->effectEndingIndex = thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW;
	thisAbilityCreationInstance->mode = DEFAULT_ABILITY;

	thisAbilityCreationInstance->creationWindow = createCharacter(imageID, rgb, x, y);
	thisAbilityCreationInstance->selector = createCharacter(3501, rgb, x, y);
	thisAbilityCreationInstance->leftRightArrow = createCharacter(3502, rgb, x, y);
	thisAbilityCreationInstance->scrollUpArrow = createCharacter(3002, rgb, x, y);
	thisAbilityCreationInstance->scrollDownArrow = createCharacter(3004, rgb, x, y);

	loadTemplateAbilities(directory, effectsFileNam);

	thisAbilityCreationInstance->abilityInsance = cloneAbility(thisAbilityCreationInstance->abilityTemplates[thisAbilityCreationInstance->templateIndex]) ;
}

void loadTemplateAbilities(char* directory, char* effectsFileName){
	int i;
	char * fullFileName = appendStrings(directory, effectsFileName);
	FILE * FP = fopen(fullFileName, "r");
	char line[2048];

	i = 0;
	while(fgets(line,2048,FP)){
		if(i >= thisAbilityCreationInstance->MAX_ABILITY_TEMPLATES){
			cwrite("!! CANNOT ADD ABILITY TEMPLATES - MAXIMUM LEVEL MET !!");
			break;
		}

		thisAbilityCreationInstance->abilityTemplates[i] = createAbilityFromLine(line);
		i++;
	}
	thisAbilityCreationInstance->numAbilityTemplates = i;

	fclose(FP);
	free(fullFileName);
}

void toggleCreateMode(){
	thisAbilityCreationInstance->inCreateMode = (thisAbilityCreationInstance->inCreateMode + 1) % 2;
}


int inAbilityCreateMode(){
	if(thisAbilityCreationInstance != NULL){
		return thisAbilityCreationInstance->inCreateMode;
	}else{
		return 0;
	}
}

void changeAbilityTemplate(int shift){
	int newIndex = thisAbilityCreationInstance->templateIndex + shift;
	newIndex = newIndex < 0 ? thisAbilityCreationInstance->numAbilityTemplates -1 : newIndex % (thisAbilityCreationInstance->numAbilityTemplates);

	thisAbilityCreationInstance->templateIndex = newIndex;

	if(thisAbilityCreationInstance->mode == DEFAULT_ABILITY){
		free(thisAbilityCreationInstance->abilityInsance);
	}

	thisAbilityCreationInstance->abilityInsance = cloneAbility(thisAbilityCreationInstance->abilityTemplates[thisAbilityCreationInstance->templateIndex]) ;
}



void drawManaCost(HDC hdcBuffer, ability * thisAbility, RECT * manaRect){
	char * manaCost[16];

	sprintf(manaCost, "Mana: %d", thisAbility->totalManaCost);

	DrawText(hdcBuffer, manaCost, strlen(manaCost), manaRect, DT_SINGLELINE);
}

void drawAbilityCreateWindow(HDC hdc, HDC hdcBuffer, RECT * prc){
	int effectIndex;
	HDC hdcMem = CreateCompatibleDC(hdc);
	RECT textRect;
	textRect.top = thisAbilityCreationInstance->creationWindow->y + 40;
	textRect.left = thisAbilityCreationInstance->creationWindow->x + 30;
	textRect.bottom = textRect.top + 40;
	textRect.right = textRect.left + 240;

	//draw create window
	SelectObject(hdcMem, thisAbilityCreationInstance->creationWindow->image);
	BitBlt(hdcBuffer, thisAbilityCreationInstance->creationWindow->x, thisAbilityCreationInstance->creationWindow->y, thisAbilityCreationInstance->creationWindow->width, thisAbilityCreationInstance->creationWindow->height, hdcMem, 0, 0, SRCCOPY);

	char tmpLine[128];

	sprintf(tmpLine,"Type: %s", thisAbilityCreationInstance->abilityInsance->typeName);
	DrawText(hdcBuffer, tmpLine, strlen(tmpLine), &textRect, DT_SINGLELINE);
	tmpLine[0] = '\0';

	if(thisAbilityCreationInstance->effectCurrentIndex == -1){
		drawUnboundCharacterAbsolute(hdc,hdcBuffer,textRect.left - 25,textRect.top,thisAbilityCreationInstance->selector);
	}

	moveRECTRight(&textRect, 150);
	sprintf(tmpLine,"Mana Cost: %i", thisAbilityCreationInstance->abilityInsance->totalManaCost);
	DrawText(hdcBuffer, tmpLine, strlen(tmpLine), &textRect, DT_SINGLELINE);
	tmpLine[0] = '\0';
	moveRECTRight(&textRect, -150);

	//scroll up arrow
	if(thisAbilityCreationInstance->effectStartingIndex > 0){
		drawUnboundCharacterAbsolute(hdc, hdcBuffer,thisAbilityCreationInstance->creationWindow->x + 40,
				thisAbilityCreationInstance->creationWindow->y + 57, thisAbilityCreationInstance->scrollUpArrow);
	}

	//scroll down arrow
	if(thisAbilityCreationInstance->effectEndingIndex < thisAbilityCreationInstance->abilityInsance->numEnabledEffects){
		drawUnboundCharacterAbsolute(hdc, hdcBuffer,thisAbilityCreationInstance->creationWindow->x + 40,
				thisAbilityCreationInstance->creationWindow->y + 77 + 17 * thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW,
				thisAbilityCreationInstance->scrollDownArrow);
	}

	moveRECTDown(&textRect, 20);

	effectIndex = 0;

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->rangeEnabled,
			 hdc, hdcBuffer, &textRect, RANGE, "Range", 0, thisAbilityCreationInstance->abilityInsance->range);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->targetedEnabled,
			 hdc, hdcBuffer, &textRect, TARGETED, "Target", 0, thisAbilityCreationInstance->abilityInsance->targeted);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->extraAttackEnabled,
			 hdc, hdcBuffer, &textRect, EXTRA_ATTACK, "Extra Attack", 0, thisAbilityCreationInstance->abilityInsance->extraAttack);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageEnabled,
			 hdc, hdcBuffer, &textRect, DICE_DAMAGE, "Dice Damage", 0, thisAbilityCreationInstance->abilityInsance->diceDamage);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageEnabled,
			 hdc, hdcBuffer, &textRect, DAMAGE, "damage", 0, thisAbilityCreationInstance->abilityInsance->damage);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageDurationEnabled,
			 hdc, hdcBuffer, &textRect, DICE_DAMAGE_DURATION, "Dice Damage Duration", 0, thisAbilityCreationInstance->abilityInsance->diceDamageDuration);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageDurationModEnabled,
			 hdc, hdcBuffer, &textRect, DICE_DAMAGE_DURATION_MOD, "diceDamageDurationMod", 0, thisAbilityCreationInstance->abilityInsance->diceDamageDurationMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->aoeEnabled,
				 hdc, hdcBuffer, &textRect, AOE, "aoe", 0, thisAbilityCreationInstance->abilityInsance->aoe);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationEnabled,
				 hdc, hdcBuffer, &textRect, DURATION, "duration", 0, thisAbilityCreationInstance->abilityInsance->duration);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationModEnabled,
				 hdc, hdcBuffer, &textRect, DURATION_MOD, "durationMod", 0, thisAbilityCreationInstance->abilityInsance->durationMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->STREnabled,
				 hdc, hdcBuffer, &textRect, STR, "STR", 0, thisAbilityCreationInstance->abilityInsance->STR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->DEXEnabled,
				 hdc, hdcBuffer, &textRect, DEX, "DEX", 0, thisAbilityCreationInstance->abilityInsance->DEX);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->CONEnabled,
				 hdc, hdcBuffer, &textRect, CON, "CON", 0, thisAbilityCreationInstance->abilityInsance->CON);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->WILLEnabled,
				 hdc, hdcBuffer, &textRect, WILL, "WILL", 0, thisAbilityCreationInstance->abilityInsance->WILL);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->INTEnabled,
				 hdc, hdcBuffer, &textRect, INTEL, "INT", 0, thisAbilityCreationInstance->abilityInsance->INT);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->WISEnabled,
				 hdc, hdcBuffer, &textRect, WIS, "WIS", 0, thisAbilityCreationInstance->abilityInsance->WIS);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->CHREnabled,
				 hdc, hdcBuffer, &textRect, CHR, "CHR", 0, thisAbilityCreationInstance->abilityInsance->CHR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->LUCKEnabled,
				 hdc, hdcBuffer, &textRect, LUCK, "LUCK", 0, thisAbilityCreationInstance->abilityInsance->LUCK);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->acEnabled,
				 hdc, hdcBuffer, &textRect, AC, "ac", 0, thisAbilityCreationInstance->abilityInsance->ac);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->attackEnabled,
					 hdc, hdcBuffer, &textRect, ATTACK, "attack", 0, thisAbilityCreationInstance->abilityInsance->attack);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageModEnabled,
				 hdc, hdcBuffer, &textRect, DAMAGE_MOD, "damageMod", 0, thisAbilityCreationInstance->abilityInsance->damageMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->mvmtEnabled,
				 hdc, hdcBuffer, &textRect, MVMT, "movement", 0, thisAbilityCreationInstance->abilityInsance->mvmt);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->hpEnabled,
				 hdc, hdcBuffer, &textRect, HP, "hp", 0, thisAbilityCreationInstance->abilityInsance->hp);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->totalHPEnabled,
				 hdc, hdcBuffer, &textRect, TOTAL_HP, "totalHP", 0, thisAbilityCreationInstance->abilityInsance->totalHP);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->totalManaEnabled,
				 hdc, hdcBuffer, &textRect, TOTAL_MANA, "totalMana", 0, thisAbilityCreationInstance->abilityInsance->totalMana);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->bluntDREnabled,
				 hdc, hdcBuffer, &textRect, BLUNT_DR, "bluntDR", 1, thisAbilityCreationInstance->abilityInsance->bluntDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->chopDREnabled,
				 hdc, hdcBuffer, &textRect, CHOP_DR, "chopDR", 1, thisAbilityCreationInstance->abilityInsance->chopDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->pierceDREnabled,
				 hdc, hdcBuffer, &textRect, PIERCE_DR, "pierceDR", 1, thisAbilityCreationInstance->abilityInsance->pierceDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->slashDREnabled,
				 hdc, hdcBuffer, &textRect, SLASH_DR, "slashDR", 1, thisAbilityCreationInstance->abilityInsance->slashDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->earthDREnabled,
				 hdc, hdcBuffer, &textRect, EARTH_DR, "earthDR", 1, thisAbilityCreationInstance->abilityInsance->earthDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->fireDREnabled,
				 hdc, hdcBuffer, &textRect, FIRE_DR, "fireDR", 1, thisAbilityCreationInstance->abilityInsance->fireDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->waterDREnabled,
				 hdc, hdcBuffer, &textRect, WATER_DR, "waterDR", 1, thisAbilityCreationInstance->abilityInsance->waterDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->lightningDREnabled,
				 hdc, hdcBuffer, &textRect, LIGHTNING_DR, "lightningDR", 1, thisAbilityCreationInstance->abilityInsance->lightningDR);

	DeleteDC(hdcMem);

}

void processEffectMapListRendering(int * effectIndex, int isEnabled, HDC hdc, HDC hdcBuffer, RECT * textRect, effect_types type, char * fieldName, int isDR, effectAndManaMapList * mapList){
	if(isEnabled){
		if(*effectIndex >= thisAbilityCreationInstance->effectStartingIndex && *effectIndex < thisAbilityCreationInstance->effectEndingIndex){
			drawEffectMapList(hdcBuffer, textRect, fieldName, isDR, mapList);
			if(thisAbilityCreationInstance->effectCurrentIndex == *effectIndex){
				thisAbilityCreationInstance->selectedType = type;
				drawUnboundCharacterAbsolute(hdc,hdcBuffer,textRect->left - 25,textRect->top-2,thisAbilityCreationInstance->selector);
				drawUnboundCharacterAbsolute(hdc,hdcBuffer,textRect->right,textRect->top-2,thisAbilityCreationInstance->leftRightArrow);
			}

		}
		(*effectIndex)++;
	}
}

void drawEffectMapList(HDC hdcBuffer, RECT * textRect, char * fieldName, int isDR, effectAndManaMapList * mapList){
		moveRECTDown(textRect, 17);
		char * tmpString = getEffectAndManaString(fieldName, mapList);
		int cost;
		if(isDR){
			cost= mapList->effectAndManaArray[mapList->selectedIndex]->effectMagnitude;
		}else{
			cost= mapList->effectAndManaArray[mapList->selectedIndex]->manaCost;
		}
		if(cost > 0){
			SetTextColor(hdcBuffer, RGB(0, 162, 255));
		}else if (cost < 0){
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
		}

		DrawText(hdcBuffer, tmpString, strlen(tmpString), textRect, DT_SINGLELINE);

		SetTextColor(hdcBuffer, RGB(255, 200, 0));

		free(tmpString);
}

char * getEffectAndManaString(char * propertyName, effectAndManaMapList * map){
	char * toReturn = malloc(sizeof(char) * 64);
	int effect = map->effectAndManaArray[map->selectedIndex]->effectMagnitude;
	int mana = map->effectAndManaArray[map->selectedIndex]->manaCost;

	sprintf(toReturn, "%s: %d, cost: %d", propertyName, effect, mana);

	return toReturn;
}

void selectNextEffect(){
	if(thisAbilityCreationInstance->effectCurrentIndex + 1 == thisAbilityCreationInstance->effectEndingIndex &&
			thisAbilityCreationInstance->effectEndingIndex  < thisAbilityCreationInstance->abilityInsance->numEnabledEffects){
		shiftEffectListUp();
	} else if(thisAbilityCreationInstance->effectCurrentIndex + 1 < thisAbilityCreationInstance->effectEndingIndex){
		thisAbilityCreationInstance->effectCurrentIndex++;
	}
}

void selectPreviousEffect(){
	if(thisAbilityCreationInstance->effectCurrentIndex == thisAbilityCreationInstance->effectStartingIndex &&
			thisAbilityCreationInstance->effectStartingIndex  > 0){
		shiftEffectListDown();
	} else if(thisAbilityCreationInstance->effectCurrentIndex > -1){
		thisAbilityCreationInstance->effectCurrentIndex--;
	}
}

void shiftEffectListUp(){
	thisAbilityCreationInstance->effectStartingIndex++;
	thisAbilityCreationInstance->effectEndingIndex++;
	thisAbilityCreationInstance->effectCurrentIndex++;
}

void shiftEffectListDown(){
	thisAbilityCreationInstance->effectStartingIndex--;
	thisAbilityCreationInstance->effectEndingIndex--;
	thisAbilityCreationInstance->effectCurrentIndex--;
}

void interpretRightAbilityCreation(){
	if(thisAbilityCreationInstance->effectCurrentIndex >= 0){
		effectAndManaMapList * tmpMap = getMapListFromEffectType();
		increaseEffect(tmpMap);
	}else if(thisAbilityCreationInstance->effectCurrentIndex == -1){
		changeAbilityTemplate(1);
	}
}

void setAbilityName(char * newName){
	strcpy(thisAbilityCreationInstance->abilityInsance->name, newName);
}

void interpretLeftAbilityCreation(int range, int mvmt, int totalHP, int totalMana){
	if(thisAbilityCreationInstance->effectCurrentIndex >= 0){
		effectAndManaMapList * tmpMap = getMapListFromEffectType();
		decreaseEffect(tmpMap, range, mvmt, totalHP, totalMana);
	}else if(thisAbilityCreationInstance->effectCurrentIndex == -1){
		changeAbilityTemplate(-1);
	}
}

effectAndManaMapList * getMapListFromEffectType(){
	switch(thisAbilityCreationInstance->selectedType){
	case RANGE:
		return thisAbilityCreationInstance->abilityInsance->range;
	case TARGETED:
		return thisAbilityCreationInstance->abilityInsance->targeted;
	case EXTRA_ATTACK:
		return thisAbilityCreationInstance->abilityInsance->extraAttack;
	case DICE_DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->diceDamage;
	case DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->damage;
	case DICE_DAMAGE_DURATION:
		return thisAbilityCreationInstance->abilityInsance->diceDamageDuration;
	case DICE_DAMAGE_DURATION_MOD:
		return thisAbilityCreationInstance->abilityInsance->diceDamageDurationMod;
	case AOE:
		return thisAbilityCreationInstance->abilityInsance->aoe;
	case DURATION:
		return thisAbilityCreationInstance->abilityInsance->duration;
	case DURATION_MOD:
		return thisAbilityCreationInstance->abilityInsance->durationMod;
	case STR:
		return thisAbilityCreationInstance->abilityInsance->STR;
	case DEX:
		return thisAbilityCreationInstance->abilityInsance->DEX;
	case CON:
		return thisAbilityCreationInstance->abilityInsance->CON;
	case WILL:
		return thisAbilityCreationInstance->abilityInsance->WILL;
	case INTEL:
		return thisAbilityCreationInstance->abilityInsance->INT;
	case WIS:
		return thisAbilityCreationInstance->abilityInsance->WIS;
	case CHR:
		return thisAbilityCreationInstance->abilityInsance->CHR;
	case LUCK:
		return thisAbilityCreationInstance->abilityInsance->LUCK;
	case AC:
		return thisAbilityCreationInstance->abilityInsance->ac;
	case ATTACK:
		return thisAbilityCreationInstance->abilityInsance->attack;
	case DAMAGE_MOD:
		return thisAbilityCreationInstance->abilityInsance->damageMod;
	case MVMT:
		return thisAbilityCreationInstance->abilityInsance->mvmt;
	case HP:
		return thisAbilityCreationInstance->abilityInsance->hp;
	case TOTAL_HP:
		return thisAbilityCreationInstance->abilityInsance->totalHP;
	case TOTAL_MANA:
		return thisAbilityCreationInstance->abilityInsance->totalMana;
	case BLUNT_DR:
		return thisAbilityCreationInstance->abilityInsance->bluntDR;
	case CHOP_DR:
		return thisAbilityCreationInstance->abilityInsance->chopDR;
	case PIERCE_DR:
		return thisAbilityCreationInstance->abilityInsance->pierceDR;
	case SLASH_DR:
		return thisAbilityCreationInstance->abilityInsance->slashDR;
	case EARTH_DR:
		return thisAbilityCreationInstance->abilityInsance->earthDR;
	case FIRE_DR:
		return thisAbilityCreationInstance->abilityInsance->fireDR;
	case WATER_DR:
		return thisAbilityCreationInstance->abilityInsance->waterDR;
	case LIGHTNING_DR:
		return thisAbilityCreationInstance->abilityInsance->lightningDR;
	}

	return NULL;
}

int canDecreaseEffect(effectAndManaMapList * selectedMap, int range, int mvmt, int totalHP, int totalMana){
	switch(thisAbilityCreationInstance->selectedType){
		case RANGE:
			if(range + selectedMap->effectAndManaArray[selectedMap->selectedIndex-1]->effectMagnitude > 0){
				return 1;
			}else{
				return 0;
			}
		break;
		case MVMT:
			if(mvmt + selectedMap->effectAndManaArray[selectedMap->selectedIndex-1]->effectMagnitude > 0){
				return 1;
			}else{
				return 0;
			}
		break;
		case TOTAL_HP:
			if(totalHP + selectedMap->effectAndManaArray[selectedMap->selectedIndex-1]->effectMagnitude > 0){
				return 1;
			}else{
				return 0;
			}
		break;
		case TOTAL_MANA:
			if(totalMana + selectedMap->effectAndManaArray[selectedMap->selectedIndex-1]->effectMagnitude > 0){
				return 1;
			}else{
				return 0;
			}
		break;
	}

	return 1;
}

void increaseEffect(effectAndManaMapList * selectedMap){
	if(selectedMap->selectedIndex + 1 < selectedMap->size){
		selectedMap->selectedIndex++;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance);
}

void decreaseEffect(effectAndManaMapList * selectedMap, int range, int mvmt, int totalHP, int totalMana){
	if(selectedMap->selectedIndex > 0 && canDecreaseEffect(selectedMap, range, mvmt, totalHP, totalMana)){
		selectedMap->selectedIndex--;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance);
}

int canCreateAbility(){
	if(thisAbilityCreationInstance->abilityInsance->type == 'p' && thisAbilityCreationInstance->abilityInsance->totalManaCost == 0){
		return 1;
	}else if((thisAbilityCreationInstance->abilityInsance->type == 'd' || thisAbilityCreationInstance->abilityInsance->type == 't') && thisAbilityCreationInstance->abilityInsance->totalManaCost > 0){
		return 1;
	}

	return 0;
}

void toggleAbilityWaitForNameMode(){
	thisAbilityCreationInstance->waitingForName = (thisAbilityCreationInstance->waitingForName + 1) % 2;
}

int inAbilityWaitForNameMode(){
	return thisAbilityCreationInstance->waitingForName;
}

ability * getNewAbility(){
	return cloneAbility(thisAbilityCreationInstance->abilityInsance);
}
