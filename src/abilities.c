/*
 * abilities.c
 *
 *  Created on: Mar 2, 2016
 *      Author: Adrian
 */
#include"./headers/abilities_pub_methods.h"
#include<string.h>
#include<stdio.h>

void moveRECTDown(RECT * thisRect, int distance){
	thisRect->top += distance;
	thisRect->bottom = thisRect->bottom + distance;
}

void moveRECTRight(RECT * thisRect, int distance){
	thisRect->left += distance;
	thisRect->right = thisRect->right + distance;
}

int calcStatusDuration(ability * thisAbility){
	int duration = 0, durationMod = 0;

	if(thisAbility->statusDurationEnabled){
		durationMod = thisAbility->statusDuration->effectAndManaArray[thisAbility->statusDuration->selectedIndex]->effectMagnitude;
	}

	if(thisAbility->diceStatusDurationEnabled){
		duration = thisAbility->diceStatusDuration->effectAndManaArray[thisAbility->diceStatusDuration->selectedIndex]->effectMagnitude;
	}

	if(duration > 0){
		return (rand() % duration) + 1 + durationMod;
	}else{
		return durationMod;
	}
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
	int sum = 0; //Ability = -1
	int diceDam = 0;
	int duration = 0;
	int DRSum = 0;
	int aoeRange = 0;
	int hasEffect = 0;
	int dummyInt = 0;

	if(thisAbility->type == 't'){
		updateElementSummation(&sum, &dummyInt, thisAbility->rangeEnabled, thisAbility->range);
	}else{
		updateElementSummation(&sum, &hasEffect, thisAbility->rangeEnabled, thisAbility->range);
	}

	updateElementSummation(&sum, &dummyInt, (thisAbility->targetedEnabled * thisAbility->diceDamageEnabled), thisAbility->targeted);

	updateElementSummation(&sum, &hasEffect, thisAbility->extraAttackEnabled, thisAbility->extraAttack);

	updateElementSummation(&diceDam, &hasEffect, thisAbility->diceDamageEnabled, thisAbility->diceDamage);

	updateElementSummation(&sum, &hasEffect, thisAbility->damageEnabled, thisAbility->damage);

	if(thisAbility->diceDamageMultiplierEnabled){
		sum += diceDam * thisAbility->diceDamageMultiplier->effectAndManaArray[thisAbility->diceDamageMultiplier->selectedIndex]->manaCost;
	}

	if(thisAbility->statusEnabled){
		calcStatusCost(&sum, &hasEffect, thisAbility);
	}

	duration = 0;

	updateElementSummation(&duration, &dummyInt, thisAbility->durationEnabled, thisAbility->duration);

	updateElementSummation(&duration, &dummyInt, thisAbility->durationModEnabled, thisAbility->durationMod);

	updateElementSummation(&sum, &hasEffect, thisAbility->actionsEnabled, thisAbility->actions);

	if(!(thisAbility->type == 't' && duration == 0)){
		updateElementSummation(&sum, &hasEffect, thisAbility->acEnabled, thisAbility->ac);

		updateElementSummation(&sum, &hasEffect, thisAbility->attackEnabled, thisAbility->attack);

		updateElementSummation(&sum, &hasEffect, thisAbility->damageModEnabled, thisAbility->damageMod);

		updateElementSummation(&sum, &hasEffect, thisAbility->mvmtEnabled, thisAbility->mvmt);

		updateElementSummation(&sum, &hasEffect, thisAbility->baseHPEnabled, thisAbility->baseHP);

		updateElementSummation(&sum, &hasEffect, thisAbility->baseManaEnabled, thisAbility->baseMana);

		updateElementSummation(&sum, &hasEffect, thisAbility->foodEnabled, thisAbility->food);

		updateElementSummation(&sum, &hasEffect, thisAbility->baseFoodEnabled, thisAbility->baseFood);

		updateElementSummation(&sum, &hasEffect, thisAbility->LoSEnabled, thisAbility->LoS);

		//DR: ceil( sum(<allDRMagnitudes>)/2.0 )
		if(thisAbility->type == 't'){
			dummyInt = 0;
			updateElementDRSummation(&dummyInt, &hasEffect, thisAbility->bluntDREnabled, thisAbility->bluntDR);
			DRSum += abs(dummyInt);
			dummyInt = 0;
			updateElementDRSummation(&dummyInt, &hasEffect, thisAbility->chopDREnabled, thisAbility->chopDR);
			DRSum += abs(dummyInt);
			dummyInt = 0;
			updateElementDRSummation(&dummyInt, &hasEffect, thisAbility->pierceDREnabled, thisAbility->pierceDR);
			DRSum += abs(dummyInt);
			dummyInt = 0;
			updateElementDRSummation(&dummyInt, &hasEffect, thisAbility->slashDREnabled, thisAbility->slashDR);
			DRSum += abs(dummyInt);
			dummyInt = 0;
			updateElementDRSummation(&dummyInt, &hasEffect, thisAbility->earthDREnabled, thisAbility->earthDR);
			DRSum += abs(dummyInt);
			dummyInt = 0;
			updateElementDRSummation(&dummyInt, &hasEffect, thisAbility->fireDREnabled, thisAbility->fireDR);
			DRSum += abs(dummyInt);
			dummyInt = 0;
			updateElementDRSummation(&dummyInt, &hasEffect, thisAbility->waterDREnabled, thisAbility->waterDR);
			DRSum += abs(dummyInt);
			dummyInt = 0;
			updateElementDRSummation(&dummyInt, &hasEffect, thisAbility->lightningDREnabled, thisAbility->lightningDR);
			DRSum += abs(dummyInt);
			dummyInt = 0;
		}else{
			updateElementDRSummation(&DRSum, &hasEffect, thisAbility->bluntDREnabled, thisAbility->bluntDR);

			updateElementDRSummation(&DRSum, &hasEffect, thisAbility->chopDREnabled, thisAbility->chopDR);

			updateElementDRSummation(&DRSum, &hasEffect, thisAbility->pierceDREnabled, thisAbility->pierceDR);

			updateElementDRSummation(&DRSum, &hasEffect, thisAbility->slashDREnabled, thisAbility->slashDR);

			updateElementDRSummation(&DRSum, &hasEffect, thisAbility->earthDREnabled, thisAbility->earthDR);

			updateElementDRSummation(&DRSum, &hasEffect, thisAbility->fireDREnabled, thisAbility->fireDR);

			updateElementDRSummation(&DRSum, &hasEffect, thisAbility->waterDREnabled, thisAbility->waterDR);

			updateElementDRSummation(&DRSum, &hasEffect, thisAbility->lightningDREnabled, thisAbility->lightningDR);
		}

		sum += ceil(DRSum/2.0);
	}

	updateElementSummation(&sum, &hasEffect, thisAbility->diceHPEnabled, thisAbility->diceHP);

	updateElementSummation(&sum, &hasEffect, thisAbility->hpEnabled, thisAbility->hp);

	//if a meaningful effect has not be modified, return initial mana cost
//	if(!hasEffect){
//		return (-1 - thisAbility->level);
//	}

	if(duration > 1){
		sum  = sum * duration;
	}

	updateElementSummation(&aoeRange, &dummyInt, thisAbility->aoeNovaEnabled, thisAbility->aoeNova);
	updateElementSummation(&aoeRange, &dummyInt, thisAbility->aoeLineEnabled, thisAbility->aoeLine);

	if (aoeRange > 1) {
		sum = sum * aoeRange;
	}

	//Negatives (given)
	sum += -1*thisAbility->level;
	sum += -1; //ability
	sum += -1*thisAbility->bonusMana;

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

void calcStatusCost(int * sum, int * hasEffect, ability * thisAbility){
	int damageCost = 1, durationCost = 0;

	if(strcmp(thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type, "None") == 0){
		return;
	}

	if(strcmp(thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type, "Poison") == 0
			|| strcmp(thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type, "Burn") == 0){
		damageCost = thisAbility->statusDamage->effectAndManaArray[thisAbility->statusDamage->selectedIndex]->manaCost;
		damageCost += thisAbility->statusDiceDamage->effectAndManaArray[thisAbility->statusDiceDamage->selectedIndex]->manaCost;
	}

	durationCost += thisAbility->diceStatusDuration->effectAndManaArray[thisAbility->diceStatusDuration->selectedIndex]->manaCost;
	durationCost += thisAbility->statusDuration->effectAndManaArray[thisAbility->statusDuration->selectedIndex]->manaCost;

	if(damageCost * durationCost > 0){
		*sum += damageCost * durationCost;
		*hasEffect = 1;
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

void addTypeManaMaptoMapList(typeAndMana * map, typeAndManaMapList * mapList, char * effectName){
	if(mapList->size != mapList->MAX_SIZE){
		mapList->typeAndManaArray[mapList->size] = map;
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

typeAndManaMapList * makeTypeManaMapList(char * line, int startingIndex, char * effectName){
	short int mana;
	char * value = strtok(line,",");
	char * type[16];
	typeAndManaMapList *  mapList = malloc(sizeof(typeAndManaMapList));

	mapList->defaultStartingIndex = startingIndex;
	mapList->size = 0;
	mapList->selectedIndex = startingIndex;
	mapList->MAX_SIZE = 16;

	while(value != NULL){
		strcpy(type,value);
		value = strtok(NULL,",");

		if(value != NULL){
			mana = atoi(value);
			value = strtok(NULL,",");
		}else{
			mana = 999;
		}

		typeAndMana * map = malloc(sizeof(typeAndMana));
		strcpy(map->type,type);
		map->manaCost = mana;

		addTypeManaMaptoMapList(map, mapList, effectName);
	}

	return mapList;
}

ability * createAbilityFromLine(char line[2048]){
	char * strtok_save_pointer;
	int startingIndex;
	ability * newAbility = malloc(sizeof(ability));
	newAbility->numEnabledEffects = 0;
	newAbility->bonusMana = 0;

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
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->damageTypeEnabled){
		newAbility->numEnabledEffects++;
		newAbility->damageType = makeTypeManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->damageType = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->rangeEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->rangeEnabled){
		newAbility->numEnabledEffects++;
		char *tmpStr = strdup(value);
		newAbility->range = makeEffectManaMapList(tmpStr, startingIndex, newAbility->typeName);
		free(tmpStr);
	}else{
		newAbility->range = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->targetedEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->targetedEnabled){
		newAbility->numEnabledEffects++;
		newAbility->targeted = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->targeted = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->extraAttackEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->extraAttackEnabled){
		newAbility->numEnabledEffects++;
		newAbility->extraAttack = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->extraAttack = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceDamageEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceDamageEnabled){
		newAbility->numEnabledEffects++;
		newAbility->diceDamage = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->diceDamage = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceDamageMultiplierEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceDamageMultiplierEnabled){
		newAbility->numEnabledEffects++;
		newAbility->diceDamageMultiplier = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->diceDamageMultiplier = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->damageEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->damageEnabled){
		newAbility->numEnabledEffects++;
		newAbility->damage = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->damage = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->statusEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->statusEnabled){
		newAbility->numEnabledEffects++;
		newAbility->status = makeTypeManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->status = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->statusDiceDamageEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->statusDiceDamageEnabled){
		newAbility->numEnabledEffects++;
		newAbility->statusDiceDamage = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->statusDiceDamage = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->statusDamageEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->statusDamageEnabled){
		newAbility->numEnabledEffects++;
		newAbility->statusDamage = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->statusDamage = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceStatusDurationEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceStatusDurationEnabled){
		newAbility->numEnabledEffects++;
		newAbility->diceStatusDuration = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->diceStatusDuration = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->statusDurationEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->statusDurationEnabled){
		newAbility->numEnabledEffects++;
		newAbility->statusDuration = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->statusDuration = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->aoeNovaEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->aoeNovaEnabled){
		newAbility->numEnabledEffects++;
		newAbility->aoeNova = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->aoeNova = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->aoeLineEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->aoeLineEnabled){
		newAbility->numEnabledEffects++;
		newAbility->aoeLine = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->aoeLine = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->durationEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->durationEnabled){
		newAbility->numEnabledEffects++;
		newAbility->duration = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->duration = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->durationModEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->durationModEnabled){
		newAbility->numEnabledEffects++;
		newAbility->durationMod = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->durationMod = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->actionsEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->actionsEnabled){
		newAbility->numEnabledEffects++;
		newAbility->actions = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->actions = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->acEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->acEnabled){
		newAbility->numEnabledEffects++;
		newAbility->ac = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->ac = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->attackEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->attackEnabled){
		newAbility->numEnabledEffects++;
		newAbility->attack = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->attack = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->damageModEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->damageModEnabled){
		newAbility->numEnabledEffects++;
		newAbility->damageMod = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->damageMod = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->mvmtEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->mvmtEnabled){
		newAbility->numEnabledEffects++;
		newAbility->mvmt = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->mvmt = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->diceHPEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->diceHPEnabled){
		newAbility->numEnabledEffects++;
		newAbility->diceHP = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->diceHP = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->hpEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->hpEnabled){
		newAbility->numEnabledEffects++;
		newAbility->hp = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->hp = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->baseHPEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->baseHPEnabled){
		newAbility->numEnabledEffects++;
		newAbility->baseHP = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->baseHP = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->baseManaEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->baseManaEnabled){
		newAbility->numEnabledEffects++;
		newAbility->baseMana = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->baseMana = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->foodEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->foodEnabled){
		newAbility->numEnabledEffects++;
		newAbility->food = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->food = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->baseFoodEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->baseFoodEnabled){
		newAbility->numEnabledEffects++;
		newAbility->baseFood = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->baseFood = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->LoSEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->LoSEnabled){
		newAbility->numEnabledEffects++;
		newAbility->LoS = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->LoS = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->bluntDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->bluntDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->bluntDR = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->bluntDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->chopDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->chopDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->chopDR = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->chopDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->pierceDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->pierceDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->pierceDR = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->pierceDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->slashDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->slashDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->slashDR  = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->slashDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->earthDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->earthDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->earthDR = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->earthDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->fireDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->fireDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->fireDR = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->fireDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->waterDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->waterDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->waterDR = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
	}else{
		newAbility->waterDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newAbility->lightningDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	startingIndex = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newAbility->lightningDREnabled){
		newAbility->numEnabledEffects++;
		newAbility->lightningDR = makeEffectManaMapList(value, startingIndex, newAbility->typeName);
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

typeAndManaMapList * cloneTypeAndManaMapList(typeAndManaMapList * thisMap){
	if(thisMap == NULL){
		return NULL;
	}

	int i;
	typeAndManaMapList * newMap = malloc(sizeof(typeAndManaMapList));

	newMap->size  = thisMap->size;
	newMap->MAX_SIZE = thisMap->MAX_SIZE;
	newMap->selectedIndex = thisMap->selectedIndex;
	newMap->defaultStartingIndex = thisMap->defaultStartingIndex;

	for(i = 0; i < thisMap->size; i++){
		typeAndMana * newTypeAndMana = malloc(sizeof(typeAndMana));
		strcpy(newTypeAndMana->type,thisMap->typeAndManaArray[i]->type);
		newTypeAndMana->manaCost = thisMap->typeAndManaArray[i]->manaCost;

		newMap->typeAndManaArray[i] = newTypeAndMana;
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
	newAbility->bonusMana = thisAbility->bonusMana;
	newAbility->numEnabledEffects = thisAbility->numEnabledEffects;

	newAbility->damageTypeEnabled = thisAbility->damageTypeEnabled;
	newAbility->damageType = cloneTypeAndManaMapList(thisAbility->damageType);

	newAbility->rangeEnabled = thisAbility->rangeEnabled;
	newAbility->range = cloneEffectAndManaMapList(thisAbility->range);

	newAbility->targetedEnabled = thisAbility->targetedEnabled;
	newAbility->targeted = cloneEffectAndManaMapList(thisAbility->targeted);

	newAbility->extraAttackEnabled = thisAbility->extraAttackEnabled;
	newAbility->extraAttack = cloneEffectAndManaMapList(thisAbility->extraAttack);

	newAbility->diceDamageEnabled = thisAbility->diceDamageEnabled;
	newAbility->diceDamage = cloneEffectAndManaMapList(thisAbility->diceDamage);

	newAbility->diceDamageMultiplierEnabled = thisAbility->diceDamageMultiplierEnabled;
	newAbility->diceDamageMultiplier = cloneEffectAndManaMapList(thisAbility->diceDamageMultiplier);

	newAbility->damageEnabled = thisAbility->damageEnabled;
	newAbility->damage = cloneEffectAndManaMapList(thisAbility->damage);

	newAbility->statusEnabled = thisAbility->statusEnabled;
	newAbility->status = cloneTypeAndManaMapList(thisAbility->status);

	newAbility->statusDiceDamageEnabled = thisAbility->statusDiceDamageEnabled;
	newAbility->statusDiceDamage = cloneEffectAndManaMapList(thisAbility->statusDiceDamage);

	newAbility->statusDamageEnabled = thisAbility->statusDamageEnabled;
	newAbility->statusDamage = cloneEffectAndManaMapList(thisAbility->statusDamage);

	newAbility->diceStatusDurationEnabled = thisAbility->diceStatusDurationEnabled;
	newAbility->diceStatusDuration = cloneEffectAndManaMapList(thisAbility->diceStatusDuration);

	newAbility->statusDurationEnabled = thisAbility->statusDurationEnabled;
	newAbility->statusDuration = cloneEffectAndManaMapList(thisAbility->statusDuration);

	newAbility->aoeNovaEnabled = thisAbility->aoeNovaEnabled;
	newAbility->aoeNova = cloneEffectAndManaMapList(thisAbility->aoeNova);

	newAbility->aoeLineEnabled = thisAbility->aoeLineEnabled;
	newAbility->aoeLine = cloneEffectAndManaMapList(thisAbility->aoeLine);

	newAbility->durationEnabled = thisAbility->durationEnabled;
	newAbility->duration = cloneEffectAndManaMapList(thisAbility->duration);

	newAbility->durationModEnabled = thisAbility->durationModEnabled;
	newAbility->durationMod = cloneEffectAndManaMapList(thisAbility->durationMod);

	newAbility->actionsEnabled = thisAbility->actionsEnabled;
	newAbility->actions = cloneEffectAndManaMapList(thisAbility->actions);

	newAbility->acEnabled = thisAbility->acEnabled;
	newAbility->ac = cloneEffectAndManaMapList(thisAbility->ac);

	newAbility->attackEnabled = thisAbility->attackEnabled;
	newAbility->attack = cloneEffectAndManaMapList(thisAbility->attack);

	newAbility->damageModEnabled = thisAbility->damageModEnabled;
	newAbility->damageMod = cloneEffectAndManaMapList(thisAbility->damageMod);

	newAbility->mvmtEnabled = thisAbility->mvmtEnabled;
	newAbility->mvmt = cloneEffectAndManaMapList(thisAbility->mvmt);

	newAbility->diceHPEnabled = thisAbility->diceHPEnabled;
	newAbility->diceHP = cloneEffectAndManaMapList(thisAbility->diceHP);

	newAbility->hpEnabled = thisAbility->hpEnabled;
	newAbility->hp = cloneEffectAndManaMapList(thisAbility->hp);

	newAbility->baseHPEnabled = thisAbility->baseHPEnabled;
	newAbility->baseHP = cloneEffectAndManaMapList(thisAbility->baseHP);

	newAbility->baseManaEnabled = thisAbility->baseManaEnabled;
	newAbility->baseMana = cloneEffectAndManaMapList(thisAbility->baseMana);

	newAbility->foodEnabled = thisAbility->foodEnabled;
	newAbility->food = cloneEffectAndManaMapList(thisAbility->food);

	newAbility->baseFoodEnabled = thisAbility->baseFoodEnabled;
	newAbility->baseFood= cloneEffectAndManaMapList(thisAbility->baseFood);

	newAbility->LoSEnabled = thisAbility->LoSEnabled;
	newAbility->LoS = cloneEffectAndManaMapList(thisAbility->LoS);

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


int appendAbilityIndexToline(char * line, int lineIndex, int value, int defaultIndex){
	if(value != defaultIndex){
		return sprintf(line + lineIndex, "%d;", value);
	}else{
		return sprintf(line + lineIndex, "%c;", 'd');
	}
}

char * getPermenantAbilityAsLine(ability * thisAbility){
	int i;
	char * line = malloc(sizeof(char) * 512);

	i = sprintf(line, "%d;", thisAbility->ID);
	i += sprintf(line + i, "%s;", thisAbility->name);
	i += sprintf(line + i, "%d;", thisAbility->level);
	i += sprintf(line + i, "%d;", thisAbility->bonusMana);
	i += sprintf(line + i, "%d;", thisAbility->range->selectedIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->ac->selectedIndex, thisAbility->ac->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->attack->selectedIndex, thisAbility->attack->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->damageMod->selectedIndex, thisAbility->damageMod->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->mvmt->selectedIndex, thisAbility->mvmt->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->baseHP->selectedIndex, thisAbility->baseHP->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->baseMana->selectedIndex, thisAbility->baseMana->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->baseFood->selectedIndex, thisAbility->baseFood->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->LoS->selectedIndex, thisAbility->LoS->defaultStartingIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->bluntDR->selectedIndex, thisAbility->bluntDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->chopDR->selectedIndex, thisAbility->chopDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->pierceDR->selectedIndex, thisAbility->pierceDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->slashDR->selectedIndex, thisAbility->slashDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->earthDR->selectedIndex, thisAbility->earthDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->fireDR->selectedIndex, thisAbility->fireDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->waterDR->selectedIndex, thisAbility->waterDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->lightningDR->selectedIndex, thisAbility->lightningDR->defaultStartingIndex);

	return line;
}

ability * createPermenantAbilityFromLine(char * line){
	ability * permenantAbility = cloneAbility(getTemplateAbilityFromRegistry(0));
	char * value;

	value = strtok(line, ";");
	permenantAbility->ID = atoi(value);

	value = strtok(NULL, ";");
	strcpy(permenantAbility->name,value);

	value = strtok(NULL, ";");
	permenantAbility->level = atoi(value);

	value = strtok(NULL, ";");
	permenantAbility->bonusMana = atoi(value);

	value = strtok(NULL, ";");
	if(*value != 'd'){
		permenantAbility->range->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->ac->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->attack->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->damageMod->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->mvmt->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->baseHP->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->baseMana->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->baseFood->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->LoS->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->bluntDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->chopDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->pierceDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->slashDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->earthDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->fireDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->waterDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		permenantAbility->lightningDR->selectedIndex = atoi(value);
	}

	permenantAbility->totalManaCost = 0;

	return permenantAbility;
}

char * getDurationAbilityAsLine(ability * thisAbility){
	int i;
	char * line = malloc(sizeof(char) * 512);

	i = sprintf(line, "%d;", thisAbility->ID);
	i += sprintf(line + i, "%s;", thisAbility->name);
	i += sprintf(line + i, "%d;", thisAbility->level);
	i += sprintf(line + i, "%d;", thisAbility->bonusMana);

	i += appendAbilityIndexToline(line, i, thisAbility->range->selectedIndex, thisAbility->range->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceDamage->selectedIndex, thisAbility->diceDamage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceDamageMultiplier->selectedIndex, thisAbility->diceDamageMultiplier->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->damage->selectedIndex, thisAbility->damage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->status->selectedIndex, thisAbility->status->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->statusDiceDamage->selectedIndex, thisAbility->statusDiceDamage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->statusDamage->selectedIndex, thisAbility->statusDamage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceStatusDuration->selectedIndex, thisAbility->diceStatusDuration->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->statusDuration->selectedIndex, thisAbility->statusDuration->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->aoeNova->selectedIndex, thisAbility->aoeNova->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->aoeLine->selectedIndex, thisAbility->aoeLine->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->duration->selectedIndex, thisAbility->duration->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->durationMod->selectedIndex, thisAbility->durationMod->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->actions->selectedIndex, thisAbility->actions->defaultStartingIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->ac->selectedIndex, thisAbility->ac->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->attack->selectedIndex, thisAbility->attack->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->damageMod->selectedIndex, thisAbility->damageMod->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->mvmt->selectedIndex, thisAbility->mvmt->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceHP->selectedIndex, thisAbility->diceHP->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->hp->selectedIndex, thisAbility->hp->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->baseHP->selectedIndex, thisAbility->baseHP->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->baseMana->selectedIndex, thisAbility->baseMana->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->food->selectedIndex, thisAbility->food->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->LoS->selectedIndex, thisAbility->LoS->defaultStartingIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->bluntDR->selectedIndex, thisAbility->bluntDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->chopDR->selectedIndex, thisAbility->chopDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->pierceDR->selectedIndex, thisAbility->pierceDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->slashDR->selectedIndex, thisAbility->slashDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->earthDR->selectedIndex, thisAbility->earthDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->fireDR->selectedIndex, thisAbility->fireDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->waterDR->selectedIndex, thisAbility->waterDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->lightningDR->selectedIndex, thisAbility->lightningDR->defaultStartingIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->totalManaCost, 0);

	return line;
}

ability * createDurationAbilityFromLine(char * line){
	ability * durationAbility = cloneAbility(getTemplateAbilityFromRegistry(1));
	char * value;

	value = strtok(line,";");
	durationAbility->ID = atoi(value);

	value = strtok(NULL, ";");
	strcpy(durationAbility->name,value);

	value = strtok(NULL,";");
	durationAbility->level = atoi(value);

	value = strtok(NULL,";");
	durationAbility->bonusMana = atoi(value);

	value = strtok(NULL, ";");
	if(*value != 'd'){
		durationAbility->range->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->diceDamage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->diceDamageMultiplier->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->damage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->status->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->statusDiceDamage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->statusDamage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->diceStatusDuration->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->statusDuration->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->aoeNova->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->aoeLine->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->duration->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->durationMod->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->actions->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->ac->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->attack->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->damageMod->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->mvmt->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->diceHP->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->hp->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->baseHP->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->baseMana->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->food->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->LoS->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->bluntDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->chopDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->pierceDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->slashDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->earthDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->fireDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->waterDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->lightningDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		durationAbility->totalManaCost = atoi(value);
	}else{
		durationAbility->totalManaCost = calculateManaCost(durationAbility);
	}

	return durationAbility;
}

char * getTargetAbilityAsLine(ability * thisAbility){
	int i;
	char * line = malloc(sizeof(char) * 512);

	i = sprintf(line, "%d;", thisAbility->ID);
	i += sprintf(line + i, "%s;", thisAbility->name);
	i += sprintf(line + i, "%d;", thisAbility->level);
	i += sprintf(line + i, "%d;", thisAbility->bonusMana);
	i += sprintf(line + i, "%d;", thisAbility->damageType->selectedIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->range->selectedIndex, thisAbility->range->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->targeted->selectedIndex, thisAbility->targeted->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceDamage->selectedIndex, thisAbility->diceDamage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceDamageMultiplier->selectedIndex, thisAbility->diceDamageMultiplier->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->damage->selectedIndex, thisAbility->damage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->status->selectedIndex, thisAbility->status->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->statusDiceDamage->selectedIndex, thisAbility->statusDiceDamage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->statusDamage->selectedIndex, thisAbility->statusDamage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceStatusDuration->selectedIndex, thisAbility->diceStatusDuration->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->statusDuration->selectedIndex, thisAbility->statusDuration->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->aoeNova->selectedIndex, thisAbility->aoeNova->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->aoeLine->selectedIndex, thisAbility->aoeLine->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->duration->selectedIndex, thisAbility->duration->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->durationMod->selectedIndex, thisAbility->durationMod->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->actions->selectedIndex, thisAbility->actions->defaultStartingIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->ac->selectedIndex, thisAbility->ac->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->attack->selectedIndex, thisAbility->attack->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->damageMod->selectedIndex, thisAbility->damageMod->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->mvmt->selectedIndex, thisAbility->mvmt->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceHP->selectedIndex, thisAbility->diceHP->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->hp->selectedIndex, thisAbility->hp->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->baseHP->selectedIndex, thisAbility->baseHP->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->baseMana->selectedIndex, thisAbility->baseMana->defaultStartingIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->bluntDR->selectedIndex, thisAbility->bluntDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->chopDR->selectedIndex, thisAbility->chopDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->pierceDR->selectedIndex, thisAbility->pierceDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->slashDR->selectedIndex, thisAbility->slashDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->earthDR->selectedIndex, thisAbility->earthDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->fireDR->selectedIndex, thisAbility->fireDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->waterDR->selectedIndex, thisAbility->waterDR->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->lightningDR->selectedIndex, thisAbility->lightningDR->defaultStartingIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->totalManaCost, 0);

	return line;
}

ability * createTargetedAbilityFromLine(char * line){
	ability * targetedAbility = cloneAbility(getTemplateAbilityFromRegistry(2));
	char * value;

	value = strtok(line, ";");
	targetedAbility->ID = atoi(value);

	value = strtok(NULL, ";");
	strcpy(targetedAbility->name,value);

	value = strtok(NULL, ";");
	targetedAbility->level = atoi(value);

	value = strtok(NULL, ";");
	targetedAbility->bonusMana = atoi(value);

	value = strtok(NULL, ";");
	targetedAbility->damageType->selectedIndex = atoi(value);

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->range->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->targeted->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->diceDamage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->diceDamageMultiplier->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->damage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->status->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->statusDiceDamage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->statusDamage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->diceStatusDuration->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->statusDuration->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->aoeNova->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->aoeLine->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->duration->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->durationMod->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->actions->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->ac->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->attack->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->damageMod->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->mvmt->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->diceHP->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->hp->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->baseHP->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->baseMana->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->bluntDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->chopDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->pierceDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->slashDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->earthDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->fireDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->waterDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->lightningDR->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		targetedAbility->totalManaCost = atoi(value);
	}else{
		targetedAbility->totalManaCost = calculateManaCost(targetedAbility);
	}

	return targetedAbility;
}

char * getInstantAbilityAsLine(ability * thisAbility){
	int i;
	char * line = malloc(sizeof(char) * 512);

	i = sprintf(line, "%d;", thisAbility->ID);
	i += sprintf(line + i, "%s;", thisAbility->name);
	i += sprintf(line + i, "%d;", thisAbility->level);
	i += sprintf(line + i, "%d;", thisAbility->bonusMana);
	i += sprintf(line + i, "%d;", thisAbility->damageType->selectedIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->range->selectedIndex, thisAbility->range->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceDamage->selectedIndex, thisAbility->diceDamage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceDamageMultiplier->selectedIndex, thisAbility->diceDamageMultiplier->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->damage->selectedIndex, thisAbility->damage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->status->selectedIndex, thisAbility->status->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->statusDiceDamage->selectedIndex, thisAbility->statusDiceDamage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->statusDamage->selectedIndex, thisAbility->statusDamage->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->diceStatusDuration->selectedIndex, thisAbility->diceStatusDuration->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->statusDuration->selectedIndex, thisAbility->statusDuration->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->actions->selectedIndex, thisAbility->actions->defaultStartingIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->attack->selectedIndex, thisAbility->attack->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->damageMod->selectedIndex, thisAbility->damageMod->defaultStartingIndex);
	i += appendAbilityIndexToline(line, i, thisAbility->mvmt->selectedIndex, thisAbility->mvmt->defaultStartingIndex);

	i += appendAbilityIndexToline(line, i, thisAbility->totalManaCost, 0);

	return line;
}

ability * createInstantAbilityFromLine(char * line) {
	ability * instantAbility = cloneAbility(getTemplateAbilityFromRegistry(3));
	char * value;

	value = strtok(line, ";");
	instantAbility->ID = atoi(value);

	value = strtok(NULL, ";");
	strcpy(instantAbility->name, value);

	value = strtok(NULL, ";");
	instantAbility->level = atoi(value);

	value = strtok(NULL, ";");
	instantAbility->bonusMana = atoi(value);

	value = strtok(NULL, ";");
	instantAbility->damageType->selectedIndex = atoi(value);

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->range->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->diceDamage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->diceDamageMultiplier->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->damage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->status->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->statusDiceDamage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->statusDamage->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->diceStatusDuration->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->statusDuration->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if (*value != 'd') {
		instantAbility->actions->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		instantAbility->attack->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		instantAbility->damageMod->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		instantAbility->mvmt->selectedIndex = atoi(value);
	}

	value = strtok(NULL, ";");
	if(*value != 'd'){
		instantAbility->totalManaCost = atoi(value);
	}else{
		instantAbility->totalManaCost = calculateManaCost(instantAbility);
	}

	return instantAbility;
}
