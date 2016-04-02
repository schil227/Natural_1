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

void moveRECTDown(RECT * thisRect, int distance){
	thisRect->top += distance;
	thisRect->bottom = thisRect->bottom + distance;
}

void moveRECTRight(RECT * thisRect, int distance){
	thisRect->left += distance;
	thisRect->right = thisRect->right + distance;
}

int calcAbilityDuration(ability * thisAbility){
	int duration = 0, durationMod = 0;

	if(thisAbility->durationEnabled){
		duration = thisAbility->duration->effectAndManaArray[thisAbility->duration->selectedIndex]->effectMagnitude;
	}

	if(thisAbility->durationModEnabled){
		durationMod = thisAbility->durationMod->effectAndManaArray[thisAbility->durationMod->selectedIndex]->effectMagnitude;
	}

	if(duration > 0){
		return (rand() % duration) + 1 + durationMod;
	}else{
		return durationMod;
	}


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

	updateElementSummation(&sum, &hasEffect, thisAbility->attackEnabled, thisAbility->attack);

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
