/*
 * abilities.c
 *
 *  Created on: Mar 2, 2016
 *      Author: Adrian
 */
#include"./headers/abilities_pub_methods.h"
#include<string.h>

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

effectAndManaMapList * makeEffectManaMapList(char * line, char * effectName){
	int i, effect, mana;
	char * number = strtok(line,",");
	effectAndManaMapList *  mapList = malloc(sizeof(effectAndManaMapList));

	mapList->defaultStartingIndex = 0;
	mapList->size = 0;
	mapList->selectedIndex = 0;
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

effect * createEffectFromLine(char line[1024]){
	char * strtok_save_pointer;
	effect * newEffect = malloc(sizeof(effect));

	char * value = strtok_r(line,";",&strtok_save_pointer);
	newEffect->ID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(newEffect->name , value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->damageTypeEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->damageTypeEnabled){
		newEffect->damageType = *value;
	}else{
		newEffect->damageType = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->rangeEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->rangeEnabled){
		char *tmpStr = strdup(value);
		newEffect->range = makeEffectManaMapList(tmpStr, newEffect->name);
		free(tmpStr);
	}else{
		newEffect->range = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->targetedEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->targetedEnabled){
		newEffect->targeted = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->targeted = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->diceDamageEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->diceDamageEnabled){
		newEffect->diceDamage = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->diceDamage = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->diceDamageDurationEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->diceDamageDurationEnabled){
		newEffect->diceDamageDuration = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->diceDamageDuration = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->STREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->STREnabled){
		newEffect->STR = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->STR = NULL;
	}


	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->DEXEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->DEXEnabled){
		newEffect->DEX = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->DEX = NULL;
	}


	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->CONEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->CONEnabled){
		newEffect->CON = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->CON = NULL;
	}


	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->WILLEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->WILLEnabled){
		newEffect->WILL = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->WILL = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->INTEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->INTEnabled){
		newEffect->INT = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->INT = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->WISEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->WISEnabled){
		newEffect->WIS = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->WIS = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->CHREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->CHREnabled){
		newEffect->CHR = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->CHR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->LUCKEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->LUCKEnabled){
		newEffect->LUCK = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->LUCK = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->acEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->acEnabled){
		newEffect->ac = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->ac = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->damageModEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->damageModEnabled){
		newEffect->damageMod = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->damageMod = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->hpEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->hpEnabled){
		newEffect->hp = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->hp = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->totalHPEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->totalHPEnabled){
		newEffect->totalHP = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->totalHP = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->totalManaEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->totalManaEnabled){
		newEffect->totalMana = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->totalMana = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->bluntDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->bluntDREnabled){
		newEffect->bluntDR = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->bluntDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->chopDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->chopDREnabled){
		newEffect->chopDR = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->chopDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->pierceDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->pierceDREnabled){
		newEffect->pierceDR = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->pierceDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->slashDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->slashDREnabled){
		newEffect->slashDR  = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->slashDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->earthDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->earthDREnabled){
		newEffect->earthDR = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->earthDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->fireDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->fireDREnabled){
		newEffect->fireDR = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->fireDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->waterDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->waterDREnabled){
		newEffect->waterDR = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->waterDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newEffect->lightningDREnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	if(newEffect->lightningDREnabled){
		newEffect->lightningDR = makeEffectManaMapList(value, newEffect->name);
	}else{
		newEffect->lightningDR = NULL;
	}

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(newEffect->description, value);

	return newEffect;
}
