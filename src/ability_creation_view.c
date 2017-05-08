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
	thisAbilityCreationInstance->MAX_ABILITY_TEMPLATES = 4;
	thisAbilityCreationInstance->numAbilityTemplates = 0;

	thisAbilityCreationInstance->effectCurrentIndex = 0;
	thisAbilityCreationInstance->effectStartingIndex = 0;
	thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW = 15;
	thisAbilityCreationInstance->effectEndingIndex = thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW;
	thisAbilityCreationInstance->mode = DEFAULT_ABILITY;

	thisAbilityCreationInstance->creationWindow = createCharacter(imageID, rgb, x, y);
	thisAbilityCreationInstance->selector = createCharacter(9501, rgb, x, y);
	thisAbilityCreationInstance->leftRightArrow = createCharacter(9502, rgb, x, y);
	thisAbilityCreationInstance->scrollUpArrow = createCharacter(1505, rgb, x, y);
	thisAbilityCreationInstance->scrollDownArrow = createCharacter(1507, rgb, x, y);

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

		ability * tmpAbility = createAbilityFromLine(line);
		addTemplateAbilityToRegistry(tmpAbility);
		thisAbilityCreationInstance->abilityTemplates[i] = cloneAbility(tmpAbility);
		thisAbilityCreationInstance->abilityTemplates[i]->ID = tmpAbility->ID;
		i++;
	}

	thisAbilityCreationInstance->numAbilityTemplates = i;

	fclose(FP);
	free(fullFileName);
}

ability * getTemplateAbility(int ID){
	switch(ID){
		case 0:
		case 1:
		case 2:
		case 3:
			return thisAbilityCreationInstance->abilityTemplates[ID];
	}

	char errlog[32];
	sprintf(errlog, "!! ABILITY TEMPLATE ID NOT FOUND: %d !!", ID);
	cwrite(errlog);

	return NULL;
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
	SetTextColor(hdcBuffer, RGB(255, 200, 0));

	//draw create window
	SelectObject(hdcMem, thisAbilityCreationInstance->creationWindow->fixedImage);
	BitBlt(hdcBuffer, thisAbilityCreationInstance->creationWindow->x, thisAbilityCreationInstance->creationWindow->y, thisAbilityCreationInstance->creationWindow->fixedWidth, thisAbilityCreationInstance->creationWindow->fixedHeight, hdcMem, 0, 0, SRCCOPY);

	char tmpLine[128];

	sprintf(tmpLine,"Type: %s", thisAbilityCreationInstance->abilityInsance->typeName);
	DrawText(hdcBuffer, tmpLine, strlen(tmpLine), &textRect, DT_SINGLELINE);
	tmpLine[0] = '\0';

	if(thisAbilityCreationInstance->effectCurrentIndex == -1){
		drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect.left - 25,textRect.top,thisAbilityCreationInstance->selector);
	}

	moveRECTRight(&textRect, 150);
	sprintf(tmpLine,"Mana Cost: %i", thisAbilityCreationInstance->abilityInsance->totalManaCost);
	DrawText(hdcBuffer, tmpLine, strlen(tmpLine), &textRect, DT_SINGLELINE);
	tmpLine[0] = '\0';
	moveRECTRight(&textRect, -150);

	//scroll up arrow
	if(thisAbilityCreationInstance->effectStartingIndex > 0){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,thisAbilityCreationInstance->creationWindow->x + 40,
				thisAbilityCreationInstance->creationWindow->y + 57, thisAbilityCreationInstance->scrollUpArrow);
	}

	//scroll down arrow
	if(thisAbilityCreationInstance->effectEndingIndex < thisAbilityCreationInstance->abilityInsance->numEnabledEffects){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,thisAbilityCreationInstance->creationWindow->x + 40,
				thisAbilityCreationInstance->creationWindow->y + 77 + 17 * thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW,
				thisAbilityCreationInstance->scrollDownArrow);
	}

	moveRECTDown(&textRect, 20);

	effectIndex = 0;

	processTypeMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageTypeEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DAMAGE_TYPE, "Damage Type", 0, thisAbilityCreationInstance->abilityInsance->damageType);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->rangeEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_RANGE, "Range", 0, thisAbilityCreationInstance->abilityInsance->range);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->targetedEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_TARGETED, "Target", 0, thisAbilityCreationInstance->abilityInsance->targeted);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->extraAttackEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_EXTRA_ATTACK, "Extra Attack", 0, thisAbilityCreationInstance->abilityInsance->extraAttack);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_DICE_DAMAGE, "Dice Damage", 0, thisAbilityCreationInstance->abilityInsance->diceDamage);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageMultiplierEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DICE_DAMAGE_MULTIPLIER, "Dice Damage Multiplier", 0, thisAbilityCreationInstance->abilityInsance->diceDamageMultiplier);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_DAMAGE, "damage", 0, thisAbilityCreationInstance->abilityInsance->damage);

	processTypeMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_STATUS, "Status", 0, thisAbilityCreationInstance->abilityInsance->status);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusDiceDamageEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_STATUS_DICE_DAMAGE, "Status Dice Damage", 0, thisAbilityCreationInstance->abilityInsance->statusDiceDamage);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusDamageEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_STATUS_DAMAGE, "Status Damage", 0, thisAbilityCreationInstance->abilityInsance->statusDamage);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceStatusDurationEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_STATUS_DICE_DURATION, "Status Dice Duration", 0, thisAbilityCreationInstance->abilityInsance->diceStatusDuration);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusDurationEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_STATUS_DURATION, "Status Duration", 0, thisAbilityCreationInstance->abilityInsance->statusDuration);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->aoeEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_AOE, "aoe", 0, thisAbilityCreationInstance->abilityInsance->aoe);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DURATION, "duration", 0, thisAbilityCreationInstance->abilityInsance->duration);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationModEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DURATION_MOD, "durationMod", 0, thisAbilityCreationInstance->abilityInsance->durationMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->actionsEnabled,
					 hdc, hdcBuffer, &textRect, ABILITY_ACTIONS, "Actions", 0, thisAbilityCreationInstance->abilityInsance->actions);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->STREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_STR, "STR", 0, thisAbilityCreationInstance->abilityInsance->STR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->DEXEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DEX, "DEX", 0, thisAbilityCreationInstance->abilityInsance->DEX);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->CONEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_CON, "CON", 0, thisAbilityCreationInstance->abilityInsance->CON);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->WILLEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_WILL, "WILL", 0, thisAbilityCreationInstance->abilityInsance->WILL);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->INTEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_INT, "INT", 0, thisAbilityCreationInstance->abilityInsance->INT);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->WISEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_WIS, "WIS", 0, thisAbilityCreationInstance->abilityInsance->WIS);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->CHREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_CHR, "CHR", 0, thisAbilityCreationInstance->abilityInsance->CHR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->LUCKEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_LUCK, "LUCK", 0, thisAbilityCreationInstance->abilityInsance->LUCK);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->acEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_AC, "ac", 0, thisAbilityCreationInstance->abilityInsance->ac);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->attackEnabled,
					 hdc, hdcBuffer, &textRect, ABILITY_ATTACK, "attack", 0, thisAbilityCreationInstance->abilityInsance->attack);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageModEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DAMAGE_MOD, "damageMod", 0, thisAbilityCreationInstance->abilityInsance->damageMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->mvmtEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_MVMT, "movement", 0, thisAbilityCreationInstance->abilityInsance->mvmt);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceHPEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DICE_HP, "diceHP", 0, thisAbilityCreationInstance->abilityInsance->diceHP);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->hpEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_HP, "hp", 0, thisAbilityCreationInstance->abilityInsance->hp);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->baseHPEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_BASE_HP, "baseHP", 0, thisAbilityCreationInstance->abilityInsance->baseHP);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->baseManaEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_BASE_MANA, "baseMana", 0, thisAbilityCreationInstance->abilityInsance->baseMana);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->bluntDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_BLUNT_DR, "bluntDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->bluntDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->chopDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_CHOP_DR, "chopDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->chopDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->pierceDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_PIERCE_DR, "pierceDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->pierceDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->slashDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_SLASH_DR, "slashDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->slashDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->earthDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_EARTH_DR, "earthDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->earthDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->fireDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_FIRE_DR, "fireDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->fireDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->waterDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_WATER_DR, "waterDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->waterDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->lightningDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_LIGHTNING_DR, "lightningDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->lightningDR);

	SetTextColor(hdcBuffer, RGB(0, 0, 0));

	DeleteDC(hdcMem);
}

void processEffectMapListRendering(int * effectIndex, int isEnabled, HDC hdc, HDC hdcBuffer, RECT * textRect, effect_types type, char * fieldName, int isDR, effectAndManaMapList * mapList){
	if(isEnabled){
		if(*effectIndex >= thisAbilityCreationInstance->effectStartingIndex && *effectIndex < thisAbilityCreationInstance->effectEndingIndex){
			drawEffectMapList(hdcBuffer, textRect, fieldName, isDR, mapList);
			if(thisAbilityCreationInstance->effectCurrentIndex == *effectIndex){
				setAbilityCreationSelectedType(type);
				drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect->left - 25,textRect->top-2,thisAbilityCreationInstance->selector);
				drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect->right,textRect->top-2,thisAbilityCreationInstance->leftRightArrow);
			}

		}
		(*effectIndex)++;
	}
}

void processTypeMapListRendering(int * effectIndex, int isEnabled, HDC hdc, HDC hdcBuffer, RECT * textRect, effect_types type, char * fieldName, int isDR, typeAndManaMapList * mapList){
	if(isEnabled){
		if(*effectIndex >= thisAbilityCreationInstance->effectStartingIndex && *effectIndex < thisAbilityCreationInstance->effectEndingIndex){
			drawTypeMapList(hdcBuffer, textRect, fieldName, isDR, mapList);
			if(thisAbilityCreationInstance->effectCurrentIndex == *effectIndex){
				setAbilityCreationSelectedType(type);
				drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect->left - 25,textRect->top-2,thisAbilityCreationInstance->selector);
				drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect->right,textRect->top-2,thisAbilityCreationInstance->leftRightArrow);
			}

		}
		(*effectIndex)++;
	}
}


void setAbilityCreationSelectedType(effect_types type){
	thisAbilityCreationInstance->selectedType = type;
}

void drawEffectMapList(HDC hdcBuffer, RECT * textRect, char * fieldName, int isDR, effectAndManaMapList * mapList){
		moveRECTDown(textRect, 17);
		char * tmpString = getEffectAndManaString(fieldName, mapList);
		int cost;
		if(isDR == 1){
			cost= mapList->effectAndManaArray[mapList->selectedIndex]->effectMagnitude;
		}else if(isDR == 2){
			cost= abs(mapList->effectAndManaArray[mapList->selectedIndex]->effectMagnitude);
		}
		else{
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

void drawTypeMapList(HDC hdcBuffer, RECT * textRect, char * fieldName, int isDR, typeAndManaMapList * mapList){
		moveRECTDown(textRect, 17);
		char * tmpString[64];

		sprintf(tmpString, "%s: %s", fieldName, mapList->typeAndManaArray[mapList->selectedIndex]->type);

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

typeAndManaMapList * getTypeMapListFromEffectType(){
	switch(thisAbilityCreationInstance->selectedType){
	case ABILITY_DAMAGE_TYPE:
		return thisAbilityCreationInstance->abilityInsance->damageType;
	case ABILITY_STATUS:
		return thisAbilityCreationInstance->abilityInsance->status;
	}
}

void interpretRightAbilityCreation(){
	if(thisAbilityCreationInstance->effectCurrentIndex >= 0){
		effectAndManaMapList * tmpMap = getMapListFromEffectType();

		if(tmpMap == NULL){
			typeAndManaMapList * tmpTypeMap = getTypeMapListFromEffectType();

			if(tmpTypeMap == NULL){
				cwrite("!! selectedType NOT FOUND !!");
			}else{
				selectNextType(tmpTypeMap);
			}

		}else{
			increaseEffect(tmpMap);
		}

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
		if(tmpMap == NULL){
			typeAndManaMapList * tmpTypeMap = getTypeMapListFromEffectType();
			selectPreviousType(tmpTypeMap);
		}else{
			decreaseEffect(tmpMap, range, mvmt, totalHP, totalMana);
		}
	}else if(thisAbilityCreationInstance->effectCurrentIndex == -1){
		changeAbilityTemplate(-1);
	}
}

effectAndManaMapList * getMapListFromEffectType(){
	switch(thisAbilityCreationInstance->selectedType){
	case ABILITY_RANGE:
		return thisAbilityCreationInstance->abilityInsance->range;
	case ABILITY_TARGETED:
		return thisAbilityCreationInstance->abilityInsance->targeted;
	case ABILITY_EXTRA_ATTACK:
		return thisAbilityCreationInstance->abilityInsance->extraAttack;
	case ABILITY_DICE_DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->diceDamage;
	case ABILITY_DICE_DAMAGE_MULTIPLIER:
		return thisAbilityCreationInstance->abilityInsance->diceDamageMultiplier;
	case ABILITY_DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->damage;
	case ABILITY_STATUS_DICE_DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->statusDiceDamage;
	case ABILITY_STATUS_DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->statusDamage;
	case ABILITY_STATUS_DICE_DURATION:
		return thisAbilityCreationInstance->abilityInsance->diceStatusDuration;
	case ABILITY_STATUS_DURATION:
		return thisAbilityCreationInstance->abilityInsance->statusDuration;
	case ABILITY_AOE:
		return thisAbilityCreationInstance->abilityInsance->aoe;
	case ABILITY_DURATION:
		return thisAbilityCreationInstance->abilityInsance->duration;
	case ABILITY_DURATION_MOD:
		return thisAbilityCreationInstance->abilityInsance->durationMod;
	case ABILITY_ACTIONS:
		return thisAbilityCreationInstance->abilityInsance->actions;
	case ABILITY_STR:
		return thisAbilityCreationInstance->abilityInsance->STR;
	case ABILITY_DEX:
		return thisAbilityCreationInstance->abilityInsance->DEX;
	case ABILITY_CON:
		return thisAbilityCreationInstance->abilityInsance->CON;
	case ABILITY_WILL:
		return thisAbilityCreationInstance->abilityInsance->WILL;
	case ABILITY_INT:
		return thisAbilityCreationInstance->abilityInsance->INT;
	case ABILITY_WIS:
		return thisAbilityCreationInstance->abilityInsance->WIS;
	case ABILITY_CHR:
		return thisAbilityCreationInstance->abilityInsance->CHR;
	case ABILITY_LUCK:
		return thisAbilityCreationInstance->abilityInsance->LUCK;
	case ABILITY_AC:
		return thisAbilityCreationInstance->abilityInsance->ac;
	case ABILITY_ATTACK:
		return thisAbilityCreationInstance->abilityInsance->attack;
	case ABILITY_DAMAGE_MOD:
		return thisAbilityCreationInstance->abilityInsance->damageMod;
	case ABILITY_MVMT:
		return thisAbilityCreationInstance->abilityInsance->mvmt;
	case ABILITY_HP:
		return thisAbilityCreationInstance->abilityInsance->hp;
	case ABILITY_DICE_HP:
		return thisAbilityCreationInstance->abilityInsance->diceHP;
	case ABILITY_BASE_HP:
		return thisAbilityCreationInstance->abilityInsance->baseHP;
	case ABILITY_BASE_MANA:
		return thisAbilityCreationInstance->abilityInsance->baseMana;
	case ABILITY_BLUNT_DR:
		return thisAbilityCreationInstance->abilityInsance->bluntDR;
	case ABILITY_CHOP_DR:
		return thisAbilityCreationInstance->abilityInsance->chopDR;
	case ABILITY_PIERCE_DR:
		return thisAbilityCreationInstance->abilityInsance->pierceDR;
	case ABILITY_SLASH_DR:
		return thisAbilityCreationInstance->abilityInsance->slashDR;
	case ABILITY_EARTH_DR:
		return thisAbilityCreationInstance->abilityInsance->earthDR;
	case ABILITY_FIRE_DR:
		return thisAbilityCreationInstance->abilityInsance->fireDR;
	case ABILITY_WATER_DR:
		return thisAbilityCreationInstance->abilityInsance->waterDR;
	case ABILITY_LIGHTNING_DR:
		return thisAbilityCreationInstance->abilityInsance->lightningDR;
	}

	return NULL;
}

int canDecreaseEffect(effectAndManaMapList * selectedMap, int range, int mvmt, int totalHP, int totalMana){
	switch(thisAbilityCreationInstance->selectedType){
		case ABILITY_RANGE:
			if(range + selectedMap->effectAndManaArray[selectedMap->selectedIndex-1]->effectMagnitude > 0){
				return 1;
			}else{
				return 0;
			}
		break;
		case ABILITY_MVMT:
			if(mvmt + selectedMap->effectAndManaArray[selectedMap->selectedIndex-1]->effectMagnitude > 0){
				return 1;
			}else{
				return 0;
			}
		break;
		case ABILITY_BASE_HP:
		case ABILITY_CON:{
			int abilityBaseHP = 0, abilityCON = 0;

			if(thisAbilityCreationInstance->abilityInsance->CONEnabled){
				abilityCON = thisAbilityCreationInstance->abilityInsance->CON->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->CON->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->abilityInsance->baseHPEnabled){
				abilityBaseHP = thisAbilityCreationInstance->abilityInsance->baseHP->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->baseHP->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->selectedType == ABILITY_CON){
				abilityCON -= 1;
			}

			if(thisAbilityCreationInstance->selectedType == ABILITY_BASE_HP){
				abilityBaseHP -= 1;
			}

			if(totalHP + abilityBaseHP + abilityCON * 2 > 0){
				return 1;
			}else{
				return 0;
			}
		}
		break;
		case ABILITY_BASE_MANA:
		case ABILITY_WILL:{
			int abilityBaseMana = 0, abilityWill = 0;

			if(thisAbilityCreationInstance->abilityInsance->WILLEnabled){
				abilityWill = 2 * thisAbilityCreationInstance->abilityInsance->WILL->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->WILL->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->abilityInsance->baseManaEnabled){
				abilityBaseMana = thisAbilityCreationInstance->abilityInsance->baseMana->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->baseMana->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->selectedType == ABILITY_WILL){
				abilityWill -= 1;
			}

			if(thisAbilityCreationInstance->selectedType == ABILITY_BASE_MANA){
				abilityBaseMana -= 1;
			}

			if(totalMana + abilityBaseMana + abilityWill * 2 > 0){
				return 1;
			}else{
				return 0;
			}
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


void selectNextType(typeAndManaMapList * thisMapList){
	if(thisMapList->selectedIndex +1 < thisMapList->size){
		thisMapList->selectedIndex++;
	}else{
		thisMapList->selectedIndex = 0;;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance);
}

void selectPreviousType(typeAndManaMapList * thisMapList){
	if(thisMapList->selectedIndex > 0){
		thisMapList->selectedIndex--;
	}else{
		thisMapList->selectedIndex = thisMapList->size - 1;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance);
}

int canCreateAbility(){
	if(thisAbilityCreationInstance->abilityInsance->type == 'p' && thisAbilityCreationInstance->abilityInsance->totalManaCost == 0){
		return 1;
	}else if((thisAbilityCreationInstance->abilityInsance->type == 'd' || thisAbilityCreationInstance->abilityInsance->type == 't') && thisAbilityCreationInstance->abilityInsance->totalManaCost > 0){
		return 1;
	}else if(thisAbilityCreationInstance->abilityInsance->type == 'i' && thisAbilityCreationInstance->abilityInsance->totalManaCost >= 0){
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
