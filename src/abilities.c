/*
 * abilities.c
 *
 *  Created on: Mar 2, 2016
 *      Author: Adrian
 */
#include"./headers/abilities_pub_methods.h"
#include<string.h>
#include<stdio.h>

/*
 * public domain strtok_r() by Charlie Gordon
 *
 *   from comp.lang.c  9/14/2007
 *
 *      http://groups.google.com/group/comp.lang.c/msg/2ab1ecbb86646684
 *
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */

char* strtok_r(
    char *str,
    const char *delim,
    char **nextp)
{
    char *ret;

    if (str == NULL)
    {
        str = *nextp;
    }

    str += strspn(str, delim);

    if (*str == '\0')
    {
        return NULL;
    }

    ret = str;

    str += strcspn(str, delim);

    if (*str)
    {
        *str++ = '\0';
    }

    *nextp = str;

    return ret;
}

static abilityCreationInstance * thisAbilityCreationInstance;

void initAbilityCreationInstance(int imageID, COLORREF rgb, int x, int y, char* directory, char* effectsFileNam){
	int i;

	thisAbilityCreationInstance = malloc(sizeof(abilityCreationInstance));
	thisAbilityCreationInstance->inCreateMode = 0;
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
			 hdc, hdcBuffer, &textRect, RANGE, "Range", thisAbilityCreationInstance->abilityInsance->range);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->targetedEnabled,
			 hdc, hdcBuffer, &textRect, TARGETED, "Target", thisAbilityCreationInstance->abilityInsance->targeted);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->extraAttackEnabled,
			 hdc, hdcBuffer, &textRect, EXTRA_ATTACK, "Extra Attack", thisAbilityCreationInstance->abilityInsance->extraAttack);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageEnabled,
			 hdc, hdcBuffer, &textRect, DICE_DAMAGE, "Dice Damage", thisAbilityCreationInstance->abilityInsance->diceDamage);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageEnabled,
			 hdc, hdcBuffer, &textRect, DAMAGE, "damage", thisAbilityCreationInstance->abilityInsance->damage);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageDurationEnabled,
			 hdc, hdcBuffer, &textRect, DICE_DAMAGE_DURATION, "Dice Damage Duration", thisAbilityCreationInstance->abilityInsance->diceDamageDuration);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageDurationModEnabled,
			 hdc, hdcBuffer, &textRect, DICE_DAMAGE_DURATION_MOD, "diceDamageDurationMod", thisAbilityCreationInstance->abilityInsance->diceDamageDurationMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->aoeEnabled,
				 hdc, hdcBuffer, &textRect, AOE, "aoe", thisAbilityCreationInstance->abilityInsance->aoe);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationEnabled,
				 hdc, hdcBuffer, &textRect, DURATION, "duration", thisAbilityCreationInstance->abilityInsance->duration);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationModEnabled,
				 hdc, hdcBuffer, &textRect, DURATION_MOD, "durationMod", thisAbilityCreationInstance->abilityInsance->durationMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->STREnabled,
				 hdc, hdcBuffer, &textRect, STR, "STR", thisAbilityCreationInstance->abilityInsance->STR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->DEXEnabled,
				 hdc, hdcBuffer, &textRect, DEX, "DEX", thisAbilityCreationInstance->abilityInsance->DEX);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->CONEnabled,
				 hdc, hdcBuffer, &textRect, CON, "CON", thisAbilityCreationInstance->abilityInsance->CON);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->WILLEnabled,
				 hdc, hdcBuffer, &textRect, WILL, "WILL", thisAbilityCreationInstance->abilityInsance->WILL);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->INTEnabled,
				 hdc, hdcBuffer, &textRect, INTEL, "INT", thisAbilityCreationInstance->abilityInsance->INT);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->WISEnabled,
				 hdc, hdcBuffer, &textRect, WIS, "WIS", thisAbilityCreationInstance->abilityInsance->WIS);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->CHREnabled,
				 hdc, hdcBuffer, &textRect, CHR, "CHR", thisAbilityCreationInstance->abilityInsance->CHR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->LUCKEnabled,
				 hdc, hdcBuffer, &textRect, LUCK, "LUCK", thisAbilityCreationInstance->abilityInsance->LUCK);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->acEnabled,
				 hdc, hdcBuffer, &textRect, AC, "ac", thisAbilityCreationInstance->abilityInsance->ac);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->attackEnabled,
					 hdc, hdcBuffer, &textRect, ATTACK, "attack", thisAbilityCreationInstance->abilityInsance->attack);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageModEnabled,
				 hdc, hdcBuffer, &textRect, DAMAGE_MOD, "damageMod", thisAbilityCreationInstance->abilityInsance->damageMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->mvmtEnabled,
				 hdc, hdcBuffer, &textRect, MVMT, "movement", thisAbilityCreationInstance->abilityInsance->mvmt);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->hpEnabled,
				 hdc, hdcBuffer, &textRect, HP, "hp", thisAbilityCreationInstance->abilityInsance->hp);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->totalHPEnabled,
				 hdc, hdcBuffer, &textRect, TOTAL_HP, "totalHP", thisAbilityCreationInstance->abilityInsance->totalHP);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->totalManaEnabled,
				 hdc, hdcBuffer, &textRect, TOTAL_MANA, "totalMana", thisAbilityCreationInstance->abilityInsance->totalMana);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->bluntDREnabled,
				 hdc, hdcBuffer, &textRect, BLUNT_DR, "bluntDR", thisAbilityCreationInstance->abilityInsance->bluntDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->chopDREnabled,
				 hdc, hdcBuffer, &textRect, CHOP_DR, "chopDR", thisAbilityCreationInstance->abilityInsance->chopDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->pierceDREnabled,
				 hdc, hdcBuffer, &textRect, PIERCE_DR, "pierceDR", thisAbilityCreationInstance->abilityInsance->pierceDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->slashDREnabled,
				 hdc, hdcBuffer, &textRect, SLASH_DR, "slashDR", thisAbilityCreationInstance->abilityInsance->slashDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->earthDREnabled,
				 hdc, hdcBuffer, &textRect, EARTH_DR, "earthDR", thisAbilityCreationInstance->abilityInsance->earthDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->fireDREnabled,
				 hdc, hdcBuffer, &textRect, FIRE_DR, "fireDR", thisAbilityCreationInstance->abilityInsance->fireDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->waterDREnabled,
				 hdc, hdcBuffer, &textRect, WATER_DR, "waterDR", thisAbilityCreationInstance->abilityInsance->waterDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->lightningDREnabled,
				 hdc, hdcBuffer, &textRect, LIGHTNING_DR, "lightningDR", thisAbilityCreationInstance->abilityInsance->lightningDR);

	DeleteDC(hdcMem);

}

void processEffectMapListRendering(int * effectIndex, int isEnabled, HDC hdc, HDC hdcBuffer, RECT * textRect, effect_types type, char * fieldName, effectAndManaMapList * mapList){
	if(isEnabled){
		if(*effectIndex >= thisAbilityCreationInstance->effectStartingIndex && *effectIndex < thisAbilityCreationInstance->effectEndingIndex){
			drawEffectMapList(hdcBuffer, textRect, fieldName, mapList);
			if(thisAbilityCreationInstance->effectCurrentIndex == *effectIndex){
				thisAbilityCreationInstance->selectedType = type;
				drawUnboundCharacterAbsolute(hdc,hdcBuffer,textRect->left - 25,textRect->top-2,thisAbilityCreationInstance->selector);
				drawUnboundCharacterAbsolute(hdc,hdcBuffer,textRect->right,textRect->top-2,thisAbilityCreationInstance->leftRightArrow);
			}

		}
		(*effectIndex)++;
	}
}

void drawEffectMapList(HDC hdcBuffer, RECT * textRect, char * fieldName, effectAndManaMapList * mapList){
		moveRECTDown(textRect, 17);
		char * tmpString = getEffectAndManaString(fieldName, mapList);
		int manaCost = mapList->effectAndManaArray[mapList->selectedIndex]->manaCost;
		if(manaCost > 0){
			SetTextColor(hdcBuffer, RGB(0, 162, 255));
		}else if (manaCost < 0){
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
		}

		DrawText(hdcBuffer, tmpString, strlen(tmpString), textRect, DT_SINGLELINE);

		SetTextColor(hdcBuffer, RGB(255, 200, 0));

		free(tmpString);
}

char * getEffectAndManaString(char * propertyName, effectAndManaMapList * map){
	char * toReturn = malloc(sizeof(char) * 32);
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
	return thisAbilityCreationInstance->abilityInsance;
}

void moveRECTDown(RECT * thisRect, int distance){
	thisRect->top += distance;
	thisRect->bottom = thisRect->bottom + distance;
}

void moveRECTRight(RECT * thisRect, int distance){
	thisRect->left += distance;
	thisRect->right = thisRect->right + distance;
}

void setAbilityName(char * newName){
	strcpy(thisAbilityCreationInstance->abilityInsance->name, newName);
}

int calculateManaCost(ability * thisAbility){
	int sum = -1; //Ability = -1
	int dam = 0;
	int duration = 0;
	int DRSum = 0;
	int aoeRange = 0;
	int hasEffect = 0;
	int dummyInt = 0;
	sum += -1*thisAbility->level;

	updateElementSummation(&sum, &dummyInt, thisAbility->rangeEnabled, thisAbility->range);

	//only counts if diceDamagage enanbled
	updateElementSummation(&sum, &dummyInt, (thisAbility->targetedEnabled * thisAbility->diceDamageEnabled), thisAbility->targeted);

	updateElementSummation(&sum, &hasEffect, thisAbility->extraAttackEnabled, thisAbility->extraAttack);

	updateElementSummation(&dam, &hasEffect, thisAbility->diceDamageEnabled, thisAbility->diceDamage);

	updateElementSummation(&dam, &hasEffect, thisAbility->damageEnabled, thisAbility->damage);

	updateElementSummation(&duration, &dummyInt, thisAbility->diceDamageDurationEnabled, thisAbility->diceDamageDuration);

	updateElementSummation(&duration, &dummyInt, thisAbility->diceDamageDurationModEnabled, thisAbility->diceDamageDurationMod);

	if(duration > 0){
		if(duration < 0 && dam < 0){
			sum += dam*duration*-1;
		}else{
			sum += dam*duration;
		}
	}else{
		sum += dam;
	}

	updateElementSummation(&sum, &hasEffect, thisAbility->STREnabled, thisAbility->STR);

	updateElementSummation(&sum, &hasEffect, thisAbility->DEXEnabled, thisAbility->DEX);

	updateElementSummation(&sum, &hasEffect, thisAbility->CONEnabled, thisAbility->CON);

	updateElementSummation(&sum, &hasEffect, thisAbility->WILLEnabled, thisAbility->WILL);

	updateElementSummation(&sum, &hasEffect, thisAbility->INTEnabled, thisAbility->INT);

	updateElementSummation(&sum, &hasEffect, thisAbility->WISEnabled, thisAbility->WIS);

	updateElementSummation(&sum, &hasEffect, thisAbility->CHREnabled, thisAbility->CHR);

	updateElementSummation(&sum, &hasEffect, thisAbility->LUCKEnabled, thisAbility->LUCK);

	updateElementSummation(&sum, &hasEffect, thisAbility->acEnabled, thisAbility->ac);

	updateElementDRSummation(&sum, &hasEffect, thisAbility->attackEnabled, thisAbility->attack);

	updateElementSummation(&sum, &hasEffect, thisAbility->damageModEnabled, thisAbility->damageMod);

	updateElementSummation(&sum, &hasEffect, thisAbility->mvmtEnabled, thisAbility->mvmt);

	updateElementSummation(&sum, &hasEffect, thisAbility->hpEnabled, thisAbility->hp);

	updateElementSummation(&sum, &hasEffect, thisAbility->totalHPEnabled, thisAbility->totalHP);

	updateElementSummation(&sum, &hasEffect, thisAbility->totalManaEnabled, thisAbility->totalMana);

	//DR: ceil( sum(<allDRMagnitudes>)/2.0 )
	updateElementDRSummation(&DRSum, &hasEffect, thisAbility->bluntDREnabled, thisAbility->bluntDR);

	updateElementDRSummation(&DRSum, &hasEffect, thisAbility->chopDREnabled, thisAbility->chopDR);

	updateElementDRSummation(&DRSum, &hasEffect, thisAbility->pierceDREnabled, thisAbility->pierceDR);

	updateElementDRSummation(&DRSum, &hasEffect, thisAbility->slashDREnabled, thisAbility->slashDR);

	updateElementDRSummation(&DRSum, &hasEffect, thisAbility->earthDREnabled, thisAbility->earthDR);

	updateElementDRSummation(&DRSum, &hasEffect, thisAbility->fireDREnabled, thisAbility->fireDR);

	updateElementDRSummation(&DRSum, &hasEffect, thisAbility->waterDREnabled, thisAbility->waterDR);

	updateElementDRSummation(&DRSum, &hasEffect, thisAbility->lightningDREnabled, thisAbility->lightningDR);

	sum += ceil(DRSum/2.0);

	//if a meaningful effect has not be modified, return initial mana cost
//	if(!hasEffect){
//		return (-1 - thisAbility->level);
//	}

	//#####effects that multiply below#####
	duration = 0;

	updateElementSummation(&duration, &dummyInt, thisAbility->durationEnabled, thisAbility->duration);

	updateElementSummation(&duration, &dummyInt, thisAbility->durationModEnabled, thisAbility->durationMod);

	if(duration > 1){
		sum  = sum * duration;
	}

	updateElementSummation(&aoeRange, &dummyInt, thisAbility->aoeEnabled, thisAbility->aoe);

	if (aoeRange > 1) {
		sum = sum * aoeRange;
	}

	//if no meaningful effect has been made, return initial cost
	if(thisAbility->type != 'p' && !hasEffect){
		return -1 - thisAbility->level;
	}

	return sum;
}

void updateElementSummation(int * sum, int * hasEffect, int isEnabled, effectAndManaMapList * mapList){
	if(isEnabled){
		int value = mapList->effectAndManaArray[mapList->selectedIndex]->manaCost;
		if(value != 0){
			*hasEffect = 1;
		}

		*(sum) += value;
	}
}

void updateElementDRSummation(int * sum, int * hasEffect, int isEnabled, effectAndManaMapList * mapList){
	if(isEnabled){
		int value = mapList->effectAndManaArray[mapList->selectedIndex]->effectMagnitude;
		if(value != 0){
			*hasEffect = 1;
		}

		*(sum) += value;
	}
}

void addEffectManaMaptoMapList(effectAndMana * map, effectAndManaMapList * mapList, char * effectName){

	if(mapList->size != mapList->MAX_SIZE){
		mapList->effectAndManaArray[mapList->size] = map;
		mapList->size++;
	}else{
		char * errStr[128];
		sprintf(errStr, "!! EFFECT_AND_MANA_MAP_LIST MAXED OUT FOR %s !!", effectName);
		cwrite(errStr);
	}
}

effectAndManaMapList * makeEffectManaMapList(char * line, int startingIndex, char * effectName){
	short int effect, mana;
	char * number = strtok(line,",");
	effectAndManaMapList *  mapList = malloc(sizeof(effectAndManaMapList));

	mapList->defaultStartingIndex = startingIndex;
	mapList->size = 0;
	mapList->selectedIndex = startingIndex;
	mapList->MAX_SIZE = 32;

	while(number != NULL){
		effect = atoi(number);
		number = strtok(NULL,",");

		if(number != NULL){
			mana = atoi(number);
			number = strtok(NULL,",");
		}else{
			mana = 999;
		}

		effectAndMana * map = malloc(sizeof(effectAndMana));
		map->effectMagnitude = effect;
		map->manaCost = mana;

		addEffectManaMaptoMapList(map, mapList, effectName);
	}

	return mapList;
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

ability * createAbilityFromLine(char line[2048]){
	char * strtok_save_pointer;
	int mapSize;
	ability * newAbility = malloc(sizeof(ability));
	newAbility->numEnabledEffects = 0;

	char * value = strtok_r(line,";",&strtok_save_pointer);
	newAbility->ID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->type = *value;

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(newAbility->typeName , value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->level = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->damageTypeEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->damageTypeEnabled){
		newAbility->numEnabledEffects++;
		newAbility->damageType = *value;
	}else{
		newAbility->damageType = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->rangeEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->rangeEnabled){
		newAbility->numEnabledEffects++;
		char *tmpStr = strdup(value);
		newAbility->range = makeEffectManaMapList(tmpStr, mapSize, newAbility->typeName);
		free(tmpStr);
	}else{
		newAbility->range = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->targetedEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->targetedEnabled){
		newAbility->numEnabledEffects++;
		newAbility->targeted = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->targeted = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->extraAttackEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->extraAttackEnabled){
		newAbility->numEnabledEffects++;
		newAbility->extraAttack = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->extraAttack = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceDamageEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceDamageEnabled){
		newAbility->numEnabledEffects++;
		newAbility->diceDamage = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->diceDamage = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->damageEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->damageEnabled){
		newAbility->numEnabledEffects++;
		newAbility->damage = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->damage = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceDamageDurationEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceDamageDurationEnabled){
		newAbility->numEnabledEffects++;
		newAbility->diceDamageDuration = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->diceDamageDuration = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->aoeEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->aoeEnabled){
		newAbility->numEnabledEffects++;
		newAbility->aoe = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->aoe = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceDamageDurationModEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceDamageDurationModEnabled){
		newAbility->numEnabledEffects++;
		newAbility->diceDamageDurationMod = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->diceDamageDurationMod = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->durationEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->durationEnabled){
		newAbility->numEnabledEffects++;
		newAbility->duration = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->duration = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->durationModEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->durationModEnabled){
		newAbility->numEnabledEffects++;
		newAbility->durationMod = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->durationMod = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->STREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->STREnabled){
		newAbility->numEnabledEffects++;
		newAbility->STR = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->STR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->DEXEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->DEXEnabled){
		newAbility->numEnabledEffects++;
		newAbility->DEX = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->DEX = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->CONEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->CONEnabled){
		newAbility->numEnabledEffects++;
		newAbility->CON = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->CON = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->WILLEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->WILLEnabled){
		newAbility->numEnabledEffects++;
		newAbility->WILL = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->WILL = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->INTEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->INTEnabled){
		newAbility->numEnabledEffects++;
		newAbility->INT = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->INT = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->WISEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->WISEnabled){
		newAbility->numEnabledEffects++;
		newAbility->WIS = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->WIS = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->CHREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->CHREnabled){
		newAbility->numEnabledEffects++;
		newAbility->CHR = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->CHR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->LUCKEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->LUCKEnabled){
		newAbility->numEnabledEffects++;
		newAbility->LUCK = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->LUCK = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->acEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->acEnabled){
		newAbility->numEnabledEffects++;
		newAbility->ac = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->ac = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->attackEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->attackEnabled){
		newAbility->numEnabledEffects++;
		newAbility->attack = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->attack = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->damageModEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->damageModEnabled){
		newAbility->numEnabledEffects++;
		newAbility->damageMod = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->damageMod = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->mvmtEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->mvmtEnabled){
		newAbility->numEnabledEffects++;
		newAbility->mvmt = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->mvmt = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->hpEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->hpEnabled){
		newAbility->numEnabledEffects++;
		newAbility->hp = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->hp = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->totalHPEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->totalHPEnabled){
		newAbility->numEnabledEffects++;
		newAbility->totalHP = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->totalHP = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->totalManaEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->totalManaEnabled){
		newAbility->numEnabledEffects++;
		newAbility->totalMana = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->totalMana = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->bluntDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->bluntDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->bluntDR = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->bluntDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->chopDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->chopDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->chopDR = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->chopDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->pierceDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->pierceDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->pierceDR = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->pierceDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->slashDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->slashDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->slashDR  = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->slashDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->earthDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->earthDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->earthDR = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->earthDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->fireDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->fireDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->fireDR = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->fireDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->waterDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->waterDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->waterDR = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->waterDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->lightningDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->lightningDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->lightningDR = makeEffectManaMapList(value, mapSize, newAbility->typeName);
	}else{
		newAbility->lightningDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(newAbility->description, value);

	newAbility->totalManaCost = calculateManaCost(newAbility);

	return newAbility;
}

effectAndManaMapList * cloneEffectAndManaMapList(effectAndManaMapList * thisMap){
	if(thisMap == NULL){
		return NULL;
	}

	int i;
	effectAndManaMapList * newMap = malloc(sizeof(effectAndManaMapList));

	newMap->size  = thisMap->size;
	newMap->MAX_SIZE = thisMap->MAX_SIZE;
	newMap->selectedIndex = thisMap->selectedIndex;
	newMap->defaultStartingIndex = thisMap->defaultStartingIndex;

	for(i = 0; i < thisMap->size; i++){
		effectAndMana * newEffectAndMana = malloc(sizeof(effectAndMana));
		newEffectAndMana->effectMagnitude = thisMap->effectAndManaArray[i]->effectMagnitude;
		newEffectAndMana->manaCost = thisMap->effectAndManaArray[i]->manaCost;

		newMap->effectAndManaArray[i] = newEffectAndMana;
	}

	return newMap;
}

ability * cloneAbility(ability * thisAbility){
	ability * newAbility = malloc(sizeof(ability));

	newAbility->type = thisAbility->type;
	strcpy(newAbility->name, thisAbility->name);
	strcpy(newAbility->typeName, thisAbility->typeName);
	strcpy(newAbility->description, thisAbility->description);
	newAbility->totalManaCost = thisAbility->totalManaCost;
	newAbility->level = thisAbility->level;
	newAbility->numEnabledEffects = thisAbility->numEnabledEffects;

	newAbility->damageTypeEnabled = thisAbility->damageTypeEnabled;
	newAbility->damageType = thisAbility->damageType;

	newAbility->rangeEnabled = thisAbility->rangeEnabled;
	newAbility->range = cloneEffectAndManaMapList(thisAbility->range);

	newAbility->targetedEnabled = thisAbility->targetedEnabled;
	newAbility->targeted = cloneEffectAndManaMapList(thisAbility->targeted);

	newAbility->extraAttackEnabled = thisAbility->extraAttackEnabled;
	newAbility->extraAttack = cloneEffectAndManaMapList(thisAbility->extraAttack);

	newAbility->diceDamageEnabled = thisAbility->diceDamageEnabled;
	newAbility->diceDamage = cloneEffectAndManaMapList(thisAbility->diceDamage);

	newAbility->damageEnabled = thisAbility->damageEnabled;
	newAbility->damage = cloneEffectAndManaMapList(thisAbility->damage);

	newAbility->diceDamageDurationEnabled = thisAbility->diceDamageDurationEnabled;
	newAbility->diceDamageDuration = cloneEffectAndManaMapList(thisAbility->diceDamageDuration);

	newAbility->diceDamageDurationModEnabled = thisAbility->diceDamageDurationModEnabled;
	newAbility->diceDamageDurationMod = cloneEffectAndManaMapList(thisAbility->diceDamageDurationMod);

	newAbility->aoeEnabled = thisAbility->aoeEnabled;
	newAbility->aoe = cloneEffectAndManaMapList(thisAbility->aoe);

	newAbility->durationEnabled = thisAbility->durationEnabled;
	newAbility->duration = cloneEffectAndManaMapList(thisAbility->duration);

	newAbility->durationModEnabled = thisAbility->durationModEnabled;
	newAbility->durationMod = cloneEffectAndManaMapList(thisAbility->durationMod);

	newAbility->STREnabled = thisAbility->STREnabled;
	newAbility->STR = cloneEffectAndManaMapList(thisAbility->STR);

	newAbility->DEXEnabled = thisAbility->DEXEnabled;
	newAbility->DEX = cloneEffectAndManaMapList(thisAbility->DEX);

	newAbility->CONEnabled = thisAbility->CONEnabled;
	newAbility->CON = cloneEffectAndManaMapList(thisAbility->CON);

	newAbility->WILLEnabled = thisAbility->WILLEnabled;
	newAbility->WILL = cloneEffectAndManaMapList(thisAbility->WILL);

	newAbility->INTEnabled = thisAbility->INTEnabled;
	newAbility->INT = cloneEffectAndManaMapList(thisAbility->INT);

	newAbility->WISEnabled = thisAbility->WISEnabled;
	newAbility->WIS = cloneEffectAndManaMapList(thisAbility->WIS);

	newAbility->CHREnabled = thisAbility->CHREnabled;
	newAbility->CHR = cloneEffectAndManaMapList(thisAbility->CHR);

	newAbility->LUCKEnabled = thisAbility->LUCKEnabled;
	newAbility->LUCK = cloneEffectAndManaMapList(thisAbility->LUCK);

	newAbility->acEnabled = thisAbility->acEnabled;
	newAbility->ac = cloneEffectAndManaMapList(thisAbility->ac);

	newAbility->attackEnabled = thisAbility->attackEnabled;
	newAbility->attack = cloneEffectAndManaMapList(thisAbility->attack);

	newAbility->damageModEnabled = thisAbility->damageModEnabled;
	newAbility->damageMod = cloneEffectAndManaMapList(thisAbility->damageMod);

	newAbility->mvmtEnabled = thisAbility->mvmtEnabled;
	newAbility->mvmt = cloneEffectAndManaMapList(thisAbility->mvmt);

	newAbility->hpEnabled = thisAbility->hpEnabled;
	newAbility->hp = cloneEffectAndManaMapList(thisAbility->hp);

	newAbility->totalHPEnabled = thisAbility->totalHPEnabled;
	newAbility->totalHP = cloneEffectAndManaMapList(thisAbility->totalHP);

	newAbility->totalManaEnabled = thisAbility->totalManaEnabled;
	newAbility->totalMana = cloneEffectAndManaMapList(thisAbility->totalMana);

	newAbility->bluntDREnabled = thisAbility->bluntDREnabled;
	newAbility->bluntDR = cloneEffectAndManaMapList(thisAbility->bluntDR);

	newAbility->chopDREnabled = thisAbility->chopDREnabled;
	newAbility->chopDR = cloneEffectAndManaMapList(thisAbility->chopDR);

	newAbility->pierceDREnabled = thisAbility->pierceDREnabled;
	newAbility->pierceDR = cloneEffectAndManaMapList(thisAbility->pierceDR);

	newAbility->slashDREnabled = thisAbility->slashDREnabled;
	newAbility->slashDR = cloneEffectAndManaMapList(thisAbility->slashDR);

	newAbility->earthDREnabled = thisAbility->earthDREnabled;
	newAbility->earthDR = cloneEffectAndManaMapList(thisAbility->earthDR);

	newAbility->fireDREnabled = thisAbility->fireDREnabled;
	newAbility->fireDR = cloneEffectAndManaMapList(thisAbility->fireDR);

	newAbility->waterDREnabled = thisAbility->waterDREnabled;
	newAbility->waterDR = cloneEffectAndManaMapList(thisAbility->waterDR);

	newAbility->lightningDREnabled = thisAbility->lightningDREnabled;
	newAbility->lightningDR = cloneEffectAndManaMapList(thisAbility->lightningDR);

	return newAbility;
}
