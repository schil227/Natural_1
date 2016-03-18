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
	thisAbilityCreationInstance = malloc(sizeof(abilityCreationInstance));
	thisAbilityCreationInstance->inCreateMode = 0;
	thisAbilityCreationInstance->currentTemplateIndex = 0;
	thisAbilityCreationInstance->MAX_ABILITY_TEMPLATES = 3;
	thisAbilityCreationInstance->numAbilityTemplates = 0;
	thisAbilityCreationInstance->creationWindow = createCharacter(imageID, rgb, x, y);

	loadTemplateAbilities(directory, effectsFileNam);

	thisAbilityCreationInstance->abilityInsance = cloneEffectAndManaMapList(thisAbilityCreationInstance->abilityTemplates[0]) ;
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

void drawAbilityCreateWindow(HDC hdc, HDC hdcBuffer, RECT * prc){
	HDC hdcMem = CreateCompatibleDC(hdc);
	RECT textRect;
	textRect.top = thisAbilityCreationInstance->creationWindow->y + 40;
	textRect.left = thisAbilityCreationInstance->creationWindow->x + 10;
	textRect.bottom = textRect.top + 40;
	textRect.right = textRect.left + 100;

	//draw window
	SelectObject(hdcMem, thisAbilityCreationInstance->creationWindow->image);
	BitBlt(hdcBuffer, thisAbilityCreationInstance->creationWindow->x, thisAbilityCreationInstance->creationWindow->y, thisAbilityCreationInstance->creationWindow->width, thisAbilityCreationInstance->creationWindow->height, hdcMem, 0, 0, SRCCOPY);

	char tmpLine[128];
	sprintf(tmpLine,"Type: %c", thisAbilityCreationInstance->abilityInsance->type);

	DrawText(hdcBuffer, tmpLine, -1, &textRect, DT_SINGLELINE);


	DeleteDC(hdcMem);

}

int calculateManaCost(ability * thisAbility){
	int sum = 0;
	int dam = 0;
	int duration = 0;
	int DRSum = 0;

	if(thisAbility->rangeEnabled){
		sum += thisAbility->range->effectAndManaArray[thisAbility->range->selectedIndex]->manaCost;
	}

	//only counts if diceDamagage enanbled
	if(thisAbility->targetedEnabled && thisAbility->diceDamageEnabled){
		sum += thisAbility->targeted->effectAndManaArray[thisAbility->targeted->selectedIndex]->manaCost;
	}

	if(thisAbility->extraAttack){
		sum += thisAbility->extraAttack->effectAndManaArray[thisAbility->extraAttack->selectedIndex]->manaCost;
	}

	if(thisAbility->diceDamageEnabled){
		dam += thisAbility->diceDamage->effectAndManaArray[thisAbility->diceDamage->selectedIndex]->manaCost;
	}

	if(thisAbility->damageEnabled){
		dam += thisAbility->damage->effectAndManaArray[thisAbility->damage->selectedIndex]->manaCost;
	}

	if(thisAbility->diceDamageDurationEnabled){
		duration += thisAbility->diceDamageDuration->effectAndManaArray[thisAbility->diceDamageDuration->selectedIndex]->manaCost;
	}

	if(thisAbility->diceDamageDurationModEnabled){
		duration += thisAbility->diceDamageDurationMod->effectAndManaArray[thisAbility->diceDamageDurationMod->selectedIndex]->manaCost;
	}

	if(duration > 0){
	sum += dam*duration;
	}else{
		sum += dam;
	}

	if(thisAbility->STREnabled){
		sum += thisAbility->STR->effectAndManaArray[thisAbility->STR->selectedIndex]->manaCost;
	}

	if(thisAbility->DEXEnabled){
		sum += thisAbility->DEX->effectAndManaArray[thisAbility->DEX->selectedIndex]->manaCost;
	}

	if(thisAbility->CONEnabled){
		sum += thisAbility->CON->effectAndManaArray[thisAbility->CON->selectedIndex]->manaCost;
	}

	if(thisAbility->WILLEnabled){
		sum += thisAbility->WILL->effectAndManaArray[thisAbility->WILL->selectedIndex]->manaCost;
	}

	if(thisAbility->INTEnabled){
		sum += thisAbility->INT->effectAndManaArray[thisAbility->INT->selectedIndex]->manaCost;
	}

	if(thisAbility->WISEnabled){
		sum += thisAbility->WIS->effectAndManaArray[thisAbility->WIS->selectedIndex]->manaCost;
	}

	if(thisAbility->CHREnabled){
		sum += thisAbility->CHR->effectAndManaArray[thisAbility->CHR->selectedIndex]->manaCost;
	}

	if(thisAbility->LUCKEnabled){
		sum += thisAbility->LUCK->effectAndManaArray[thisAbility->LUCK->selectedIndex]->manaCost;
	}

	if(thisAbility->acEnabled){
		sum += thisAbility->ac->effectAndManaArray[thisAbility->ac->selectedIndex]->manaCost;
	}

	if(thisAbility->damageModEnabled){
		sum += thisAbility->damageMod->effectAndManaArray[thisAbility->damageMod->selectedIndex]->manaCost;
	}

	if(thisAbility->mvmtEnabled){
		sum += thisAbility->mvmt->effectAndManaArray[thisAbility->mvmt->selectedIndex]->manaCost;
	}

	if(thisAbility->hpEnabled){
		sum += thisAbility->hp->effectAndManaArray[thisAbility->hp->selectedIndex]->manaCost;
	}

	if(thisAbility->totalHPEnabled){
		sum += thisAbility->totalHP->effectAndManaArray[thisAbility->totalHP->selectedIndex]->manaCost;
	}

	if(thisAbility->totalManaEnabled){
		sum += thisAbility->totalMana->effectAndManaArray[thisAbility->totalMana->selectedIndex]->manaCost;
	}

	//DR: ceil( sum(<allDRMagnitudes>)/2 )
	if(thisAbility->bluntDREnabled){
		DRSum += thisAbility->bluntDR->effectAndManaArray[thisAbility->bluntDR->selectedIndex]->effectMagnitude;
	}

	if(thisAbility->chopDREnabled){
		DRSum += thisAbility->chopDR->effectAndManaArray[thisAbility->chopDR->selectedIndex]->effectMagnitude;
	}

	if(thisAbility->pierceDREnabled){
		DRSum += thisAbility->pierceDR->effectAndManaArray[thisAbility->pierceDR->selectedIndex]->effectMagnitude;
	}

	if(thisAbility->slashDREnabled){
		DRSum += thisAbility->slashDR->effectAndManaArray[thisAbility->slashDR->selectedIndex]->effectMagnitude;
	}

	if(thisAbility->earthDREnabled){
		DRSum += thisAbility->earthDR->effectAndManaArray[thisAbility->earthDR->selectedIndex]->effectMagnitude;
	}

	if(thisAbility->fireDREnabled){
		DRSum += thisAbility->fireDR->effectAndManaArray[thisAbility->fireDR->selectedIndex]->effectMagnitude;
	}

	if(thisAbility->waterDREnabled){
		DRSum += thisAbility->waterDR->effectAndManaArray[thisAbility->waterDR->selectedIndex]->effectMagnitude;
	}

	if(thisAbility->lightningDREnabled){
		DRSum += thisAbility->lightningDR->effectAndManaArray[thisAbility->lightningDR->selectedIndex]->effectMagnitude;
	}

	sum += ceil(DRSum/2);

	//#####effects that multiply below#####
	duration = 0;

	if(thisAbility->durationEnabled){
		duration = thisAbility->duration->effectAndManaArray[thisAbility->duration->selectedIndex]->manaCost;
	}

	if(thisAbility->durationModEnabled){
		duration = thisAbility->durationMod->effectAndManaArray[thisAbility->durationMod->selectedIndex]->manaCost;
	}

	if(duration > 1){
		sum  = sum * duration;
	}

	if(thisAbility->aoeEnabled){
		int tmp = thisAbility->aoe->effectAndManaArray[thisAbility->aoe->selectedIndex]->manaCost;
		if(tmp > 1){
			sum = sum * tmp;
		}
	}

	return sum;
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
	int i, effect, mana;
	char * number = strtok(line,",");
	effectAndManaMapList *  mapList = malloc(sizeof(effectAndManaMapList));

	mapList->defaultStartingIndex = 0;
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
		thisAbilityCreationInstance->numAbilityTemplates = i;
		i++;
	}

	fclose(FP);
	free(fullFileName);
}

ability * createAbilityFromLine(char line[2048]){
	char * strtok_save_pointer;
	int mapSize;
	ability * newAbility = malloc(sizeof(ability));

	char * value = strtok_r(line,";",&strtok_save_pointer);
	newAbility->ID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->type = *value;

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(newAbility->name , value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->damageTypeEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->damageTypeEnabled){
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
		char *tmpStr = strdup(value);
		newAbility->range = makeEffectManaMapList(tmpStr, mapSize, newAbility->name);
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
		newAbility->targeted = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->targeted = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->extraAttackEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->extraAttackEnabled){
		newAbility->extraAttack = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->extraAttack = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceDamageEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceDamageEnabled){
		newAbility->diceDamage = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->diceDamage = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->damageEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->damageEnabled){
		newAbility->damage = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->damage = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceDamageDurationEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceDamageDurationEnabled){
		newAbility->diceDamageDuration = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->diceDamageDuration = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->aoeEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->aoeEnabled){
		newAbility->aoe = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->aoe = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceDamageDurationModEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceDamageDurationModEnabled){
		newAbility->diceDamageDurationMod = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->diceDamageDurationMod = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->durationEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->durationEnabled){
		newAbility->duration = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->duration = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->durationModEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->durationModEnabled){
		newAbility->durationMod = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->durationMod = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->STREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->STREnabled){
		newAbility->STR = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->STR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->DEXEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->DEXEnabled){
		newAbility->DEX = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->DEX = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->CONEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->CONEnabled){
		newAbility->CON = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->CON = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->WILLEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->WILLEnabled){
		newAbility->WILL = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->WILL = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->INTEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->INTEnabled){
		newAbility->INT = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->INT = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->WISEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->WISEnabled){
		newAbility->WIS = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->WIS = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->CHREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->CHREnabled){
		newAbility->CHR = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->CHR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->LUCKEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->LUCKEnabled){
		newAbility->LUCK = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->LUCK = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->acEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->acEnabled){
		newAbility->ac = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->ac = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->damageModEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->damageModEnabled){
		newAbility->damageMod = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->damageMod = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->mvmtEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->mvmtEnabled){
		newAbility->mvmt = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->mvmt = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->hpEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->hpEnabled){
		newAbility->hp = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->hp = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->totalHPEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->totalHPEnabled){
		newAbility->totalHP = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->totalHP = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->totalManaEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->totalManaEnabled){
		newAbility->totalMana = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->totalMana = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->bluntDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->bluntDREnabled){
		newAbility->bluntDR = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->bluntDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->chopDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->chopDREnabled){
		newAbility->chopDR = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->chopDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->pierceDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->pierceDREnabled){
		newAbility->pierceDR = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->pierceDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->slashDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->slashDREnabled){
		newAbility->slashDR  = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->slashDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->earthDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->earthDREnabled){
		newAbility->earthDR = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->earthDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->fireDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->fireDREnabled){
		newAbility->fireDR = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->fireDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->waterDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->waterDREnabled){
		newAbility->waterDR = makeEffectManaMapList(value, mapSize, newAbility->name);
	}else{
		newAbility->waterDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->lightningDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mapSize = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->lightningDREnabled){
		newAbility->lightningDR = makeEffectManaMapList(value, mapSize, newAbility->name);
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
	effectAndManaMapList * newMap = malloc(sizeof(thisMap));

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
	strcpy(newAbility->description, thisAbility->description);
	newAbility->totalManaCost = thisAbility->totalManaCost;

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
