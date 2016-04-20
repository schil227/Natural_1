/*
 * individual.c
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#include"./headers/individual_pub_methods.h"

int isRandomized = 0;

individual *initIndividual(){
	int i;

	individual* toReturn = malloc(sizeof(individual));
	toReturn->playerCharacter = malloc(sizeof(character));

	toReturn->backpack = malloc(sizeof(inventory));
	toReturn->backpack->inventorySize = 0;

	toReturn->activeItems = malloc(sizeof(activeItemList));
	toReturn->activeItems->activeItemsTotal = 0;

	for (i = 0; i < 40; i++) {
		toReturn->backpack->inventoryArr[i] = NULL;
		toReturn->activeItems->activeItemArr[i] = NULL;
	}

	toReturn->abilities = malloc(sizeof(abilityList));
	toReturn->abilities->numAbilities = 0;
	toReturn->abilities->MAX_ABILITIES = 64;

	toReturn->activeAbilities = malloc(sizeof(activeAbilityList));
	toReturn->activeAbilities->numAbilities = 0;
	toReturn->activeAbilities->MAX_ABILITIES = 64;
	toReturn->activeAbilities->selectedAbility = NULL;

	for(i = 0; i < 64; i++){
		toReturn->activeAbilities->abilitiesList[i] = NULL;
	}

	toReturn->activeStatuses = malloc(sizeof(statusList));
	toReturn->activeStatuses->numStatuses = 0;
	toReturn->activeStatuses->MAX_STATUSES = 16;

	for(i = 0; i < 16; i++){
		toReturn->activeStatuses->statuses[i] = NULL;
	}

	return toReturn;
}

int defineIndividual(individual * thisIndividual, int imageID, int ID, COLORREF rgb, char * name, int direction, int x,
		int y, int STR, int DEX, int CON, int WILL, int INT, int WIS, int CHR, int LUCK, int baseHP, int totalActions, int baseMana, int baseAC, int attack, int maxDam, int minDam, int baseDam,  char critType[3],
		int range, int mvmt, int bluntDR, int chopDR, int slashDR, int pierceDR, int earthDR, int fireDR,
		int waterDR, int lightningDR, int earthWeakness, int fireWeakness, int waterWeakness,
		int lightiningWeakness, int dialogID, int gold){

	BITMAP bm;

	thisIndividual->playerCharacter->imageID = imageID;
	thisIndividual->ID = ID;
	thisIndividual->playerCharacter->image = LoadBitmap(GetModuleHandle(NULL),
			MAKEINTRESOURCE(imageID));

	if(thisIndividual->playerCharacter->image == NULL) {
		return 1;
	}

	thisIndividual->playerCharacter->imageMask = CreateBitmapMask(
			thisIndividual->playerCharacter->image, rgb);

	GetObjectA(thisIndividual->playerCharacter->image, sizeof(bm), &bm);

	thisIndividual->playerCharacter->height = bm.bmHeight;
	thisIndividual->playerCharacter->width = bm.bmWidth;

	thisIndividual->playerCharacter->rgb = rgb;

	strcpy(thisIndividual->name, name);
	thisIndividual->playerCharacter->direction = direction;
	thisIndividual->playerCharacter->x = x;
	thisIndividual->playerCharacter->y = y;

	thisIndividual->STR = STR;
	thisIndividual->DEX = DEX;
	thisIndividual->CON = CON;
	thisIndividual->WILL = WILL;
	thisIndividual->INT = INT;
	thisIndividual->WIS = WIS;
	thisIndividual->CHR = CHR;
	thisIndividual->LUCK = LUCK;

	thisIndividual->baseHP = baseHP;
	thisIndividual->totalActions = totalActions;
	thisIndividual->remainingActions = totalActions;
	thisIndividual->baseMana = baseMana;
	thisIndividual->AC = baseAC;
	thisIndividual->attack = attack;
	thisIndividual->maxDam = maxDam;
	thisIndividual->minDam = minDam;
	thisIndividual->baseDam = baseDam;
	strcpy(thisIndividual->critType,critType);
	thisIndividual->range = range;
	thisIndividual->mvmt = mvmt;

	thisIndividual->bluntDR = bluntDR;
	thisIndividual->chopDR = chopDR;
	thisIndividual->slashDR = slashDR;
	thisIndividual->pierceDR = pierceDR;

	thisIndividual->earthDR = earthDR;
	thisIndividual->fireDR = fireDR;
	thisIndividual->waterDR = waterDR;
	thisIndividual->lightningDR = lightningDR;

	thisIndividual->earthWeakness = earthWeakness;
	thisIndividual->fireWeakness = fireWeakness;
	thisIndividual->waterWeakness = waterWeakness;
	thisIndividual->lightiningWeakness = lightiningWeakness;

	thisIndividual->dialogID = dialogID;

	thisIndividual->gold = gold;

	thisIndividual->jumpTarget = 0;

	thisIndividual->baseHP = baseHP;
	thisIndividual->hp = baseHP + CON * 2;

	thisIndividual->baseMana = baseMana;
	thisIndividual->mana = baseMana + WILL * 2;


	return 0;
}

void destroyIndividual(individual* thisIndividual){
	int i, itemsPassed;
	if(thisIndividual->playerCharacter){ //Null check
		destroyCharacter(thisIndividual->playerCharacter);
	}

//	if(thisIndividual->activeItems != NULL){
//		itemsPassed = 0;
//		for(i = 0; i < 40; i++){
//			if(thisIndividual->activeItems->activeItemArr[i] != NULL){
////				destroyItem(thisIndividual->activeItems->activeItemArr[i]->thisItem);
////				free(thisIndividual->activeItems->activeItemArr[i]);
//				itemsPassed++;
//			}
//			if(itemsPassed >= thisIndividual->activeItems->activeItemsTotal){
//				break;
//			}
//
//		}
//	}

	free(thisIndividual->activeItems);

//	if(thisIndividual->backpack != NULL){
//		itemsPassed = 0;
//		for(i = 0; i < 40; i++){
//			if(thisIndividual->backpack->inventoryArr[i] != NULL){
//				destroyItem(thisIndividual->backpack->inventoryArr[i]);
//				itemsPassed++;
//			}
//			if(itemsPassed >= thisIndividual->backpack->inventorySize){
//				break;
//			}
//
//		}
//	}

	free(thisIndividual->backpack);

	free(thisIndividual);

}

item * getActiveWeapon(inventory * backpack){
	int i;
	item * tmpItem = NULL;

	for(i = 0; i < 40; i++){
		if(backpack->inventoryArr[i] != NULL){
			tmpItem = backpack->inventoryArr[i];
			if(tmpItem->type == 'w' && tmpItem->isEquipt){
				return tmpItem;
			}
		}
	}

	return NULL;
}


int attackIndividual(individual *thisIndividual, individual *targetIndividual){
	int d20 = rand() % 20 + 1;
	int totalAttack = d20 + getAttributeSum(thisIndividual, "attack");
	int totalAC = getAttributeSum(targetIndividual,"ac") + getAttributeSum(thisIndividual, "DEX");
	int i;
	item * tmpItem;

	triggerEventOnAttack(targetIndividual->ID);

	if(d20 == 20){
		return damageIndividual(thisIndividual, targetIndividual, 1);

	} else if(d20 == 1){ //THE natural one.
		cwrite("Where'd you learn to fight?\n");
		return 0;
	}

	if(totalAttack >= totalAC){ //Tie goes to attacker, of course.
		return damageIndividual(thisIndividual, targetIndividual, 0);
	}else{ //miss
		sendMissedDialog(thisIndividual->name,targetIndividual->name,d20,totalAC);
		return 0;
	}
}

int attackIndividualWithAbility(individual * thisIndividual, individual * target){
	ability * targetAbility = thisIndividual->activeAbilities->selectedAbility;
	int i, isTargeted = 0;

	triggerEventOnAttack(target->ID);

	if(targetAbility->targetedEnabled){
		isTargeted = targetAbility->targeted->effectAndManaArray[targetAbility->targeted->selectedIndex]->effectMagnitude;
	}

	if(isTargeted){
		return damageIndividualWithAbility(thisIndividual, target);
	}else{
		int statValue = 0, d20;

		statValue = getAttributeSum(target, "WIS");
		d20 = rand() % 20 + 1;

		char statOut[128];
		sprintf(statOut, "Saving throw WIS Check: %d + 2*%d = %d", d20, statValue, (d20 + statValue*2));
		cwrite(statOut);

		if((d20 + statValue*2) < 13){
			cwrite("Saving throw failed!");
		return 	damageIndividualWithAbility(thisIndividual, target);
		}else{
			sprintf(statOut, "%s's ability failed!", thisIndividual->name);
			cwrite(statOut);
			return 0;
		}

	}
}

int abilityIsHarmful(ability * thisAbility){
	//Note: damage and diceDamage only affect the caster, and are not harmful to others in range.

	if(thisAbility->statusEnabled){
		return statusIsHarmful(thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type);
	}

}

int statusIsHarmful(char * type){
	statusEffect thisStatusEffect = lookUpStatusType(type);

	switch(thisStatusEffect){
		case STATUS_NONE:{
			return 0;
		}
	}

	return 1;
}

int processCasterOnlyAffects(individual * thisIndividual, ability * thisAbility){
	if (thisAbility->type == 'd' || thisAbility->type == 'i') {
		int damage = 0, totalDamage, targetDR;

		if (thisAbility->diceDamageEnabled) {
			int diceDamage = thisAbility->diceDamage->effectAndManaArray[thisAbility->diceDamage->selectedIndex]->effectMagnitude;

			if (diceDamage > 0) {
				damage = (rand() % diceDamage) + 1;
			}
		}

		if (thisAbility->damageEnabled) {
			damage += thisAbility->damage->effectAndManaArray[thisAbility->damage->selectedIndex]->effectMagnitude;
		}

		if(damage == 0){
			return 0;
		}

		targetDR = calcDR(thisIndividual, thisAbility->damageType);

		totalDamage = damage - targetDR;

		sendHitDialog(thisIndividual->name, thisIndividual->name, 20, totalDamage);
		thisIndividual->hp = thisIndividual->hp - totalDamage;

		if (thisIndividual->hp <= 0) {
			return 1;
		} else {
			return 0;
		}
	}
	return 0;
}


int damageIndividualWithAbility(individual *thisIndividual, individual *targetIndividual){
	int damage = 0, totalDamage, targetDR;
	ability * targetAbility = thisIndividual->activeAbilities->selectedAbility;

	if(targetAbility->diceDamageEnabled){
		int diceDamage = targetAbility->diceDamage->effectAndManaArray[targetAbility->diceDamage->selectedIndex]->effectMagnitude;
		if(diceDamage > 0){
			damage = (rand() % diceDamage) + 1;
		}
	}

	if(targetAbility->damageEnabled){
		damage += targetAbility->damage->effectAndManaArray[targetAbility->damage->selectedIndex]->effectMagnitude;
	}

	targetDR = calcDR(targetIndividual, targetAbility->damageType);

	totalDamage = damage - targetDR;

	if(totalDamage < 1){
		totalDamage = 0;
	}

	if(totalDamage > 0){
		triggerEventOnHarm(targetIndividual->ID);
	}

	sendHitDialog(thisIndividual->name, targetIndividual->name, 20, totalDamage);
	targetIndividual->hp = targetIndividual->hp - totalDamage;

	//Add status
	if(targetAbility->statusEnabled){
		int sum = 0, dummy = 0;
		calcStatusCost(&sum, &dummy, targetAbility);
		if(sum > 0){
			status * newStatus = createStatusFromAbility(targetAbility);
			addStatusToIndividual(targetIndividual, newStatus);
			processStatus(targetIndividual, newStatus);
		}

	}

	if(targetIndividual->hp <= 0){ //target is dead
		sendDeathDialog(targetIndividual->name, thisIndividual->name);
		triggerEventOnDeath(targetIndividual->ID);
		removeFromExistance(targetIndividual->ID);
		return 1;
	}else{ //non-fatal blow
		return 0;
	}

}

int useDurationAbilityOnIndividual(individual * thisIndividual, ability * thisAbility){
	int duration = calcAbilityDuration(thisAbility);
	char * tmp[64];

	if(thisIndividual->activeAbilities->numAbilities + 1 < thisIndividual->activeAbilities->MAX_ABILITIES){
		sprintf(tmp, "Used %s for %d turns.",thisAbility->name, duration);
		cwrite(tmp);

		addActiveAbilityToIndividual(thisIndividual, thisAbility, duration);
		useActiveAbility(thisIndividual, thisAbility);

		if(thisAbility->statusEnabled){
			statusEffect thisEffect = lookUpStatusType( thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type);

			if(thisEffect != STATUS_NONE){
				status * newStatus = createStatusFromAbility(thisAbility);
				addStatusToIndividual(thisIndividual, newStatus);
				processStatus(thisIndividual,newStatus);
			}

		}

	}else{
		cwrite("Cannot use another ability");
	}

	if(thisIndividual->hp <= 0){
		return 1;
	}else{
		return 0;
	}

}

void addStatusToIndividual(individual * thisIndividual, status * newStatus){
	int i;

	if(thisIndividual->activeStatuses->numStatuses < thisIndividual->activeStatuses->MAX_STATUSES){

		for(i = 0; i < thisIndividual->activeStatuses->MAX_STATUSES; i++){
			if(thisIndividual->activeStatuses->statuses[i] == NULL){
				thisIndividual->activeStatuses->statuses[i] = newStatus;
				thisIndividual->activeStatuses->numStatuses++;
				break;
			}
		}

	}

}

status * createStatusFromAbility(ability * thisAbility){
	int duration = 0, diceDuration = 0;

	status * thisStatus = malloc(sizeof(status));

	thisStatus->effect = lookUpStatusType(thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type);

	thisStatus->diceDamage =  thisAbility->statusDiceDamage->effectAndManaArray[thisAbility->statusDiceDamage->selectedIndex]->effectMagnitude;
	thisStatus->damage =  thisAbility->statusDamage->effectAndManaArray[thisAbility->statusDamage->selectedIndex]->effectMagnitude;
	diceDuration =  thisAbility->diceStatusDuration->effectAndManaArray[thisAbility->diceStatusDuration->selectedIndex]->effectMagnitude;
	duration =  thisAbility->statusDuration->effectAndManaArray[thisAbility->statusDuration->selectedIndex]->effectMagnitude;

	if(diceDuration > 0){
		thisStatus->turnsRemaining = (rand() % diceDuration) + 1 + duration;
	}else{
		thisStatus->turnsRemaining = duration;
	}

	return thisStatus;
}

int damageIndividual(individual *thisIndividual, individual *targetIndividual, int isCrit){
	int totalDamage = 0, i, totalDR, maxDamTotal, minDamTotal, baseDam;
	char attackType = 'b'; //for now, default is blunt (punching)

	baseDam = getAttributeSum(thisIndividual,"baseDam"); //doesn't include STR mod

	baseDam += calcStrengthDamageMod(thisIndividual);

	maxDamTotal = calcMaxDam(thisIndividual);
	minDamTotal = calcMinDam(thisIndividual);

	thisIndividual->hasAttacked = 1;

	if(minDamTotal < 0){
		minDamTotal = 0;
	}

	if(minDamTotal > maxDamTotal){
		maxDamTotal = minDamTotal;
	}

	if(isCrit){
		cwrite("CRITICAL HIT!!!\n");
		totalDamage = calcCrit(thisIndividual, maxDamTotal, minDamTotal, baseDam);
	}else{
		if(maxDamTotal-minDamTotal == 0){ //max/min are the same!
			totalDamage = maxDamTotal;
		}else{
			totalDamage = rand() % (maxDamTotal - minDamTotal);
		}
		totalDamage = totalDamage + minDamTotal + baseDam;
	}

	totalDamage = totalDamage - calcDR(targetIndividual, attackType);

	if(totalDamage < 1){
		totalDamage = 0;
	}

	if(totalDamage > 0){
		triggerEventOnHarm(targetIndividual->ID);
	}

	sendHitDialog(thisIndividual->name, targetIndividual->name, thisIndividual->maxDam, totalDamage);
	targetIndividual->hp = targetIndividual->hp - totalDamage;

	if(targetIndividual->hp <= 0){ //target is dead
		sendDeathDialog(targetIndividual->name, thisIndividual->name);
		triggerEventOnDeath(targetIndividual->ID);
		removeFromExistance(targetIndividual->ID);
		return 1;
	}else{ //non-fatal blow
		return 0;
	}
}

int calcDR(individual * targetIndividual, char attackType){
	int i, totalDR = 0;
	item * tmpItem;
	switch (attackType) {
	case 'b':
		totalDR += getAttributeSum(targetIndividual, "bluntDR");
		break;
	case 'c':
		totalDR += getAttributeSum(targetIndividual, "chopDR");
		break;
	case 's':
		totalDR += getAttributeSum(targetIndividual, "slashDR");
		break;
	case 'p':
		totalDR += getAttributeSum(targetIndividual, "pierceDR");
		break;
	case 'e':
		totalDR += getAttributeSum(targetIndividual, "earthDR");
		break;
	case 'f':
		totalDR += getAttributeSum(targetIndividual, "fireDR");
		break;
	case 'w':
		totalDR += getAttributeSum(targetIndividual, "waterDR");
		break;
	case 'l':
		totalDR += getAttributeSum(targetIndividual, "lightningDR");
		break;
	}

	return totalDR;
}

int calcCrit(individual * thisIndividual, int maxDamTotal, int minDamTotal, int baseDam){
	if(strcmp(thisIndividual->critType, "MAX") == 0){
		return maxDamTotal + baseDam;
	} else if(strcmp(thisIndividual->critType, "DUB") == 0){
		int attackDamage = 0;
		if(maxDamTotal - minDamTotal == 0){ //max/min are the same!
			attackDamage = maxDamTotal;
		}else{
			attackDamage = rand() % (maxDamTotal - minDamTotal);
		}
		return (attackDamage + minDamTotal + baseDam) * 2;
	}
}


int startTurn(individual * thisIndividual){
	if(processActiveItems(thisIndividual) || processActiveAbilities(thisIndividual) || processStatuses(thisIndividual)){
		return 1;
	}
	return 0;
}

int processActiveItems(individual * thisIndividual){
	int i, itemsPassed = 0;
	activeItem * tmpActiveItem;

	for(i = 0; i < 40; i++){
		tmpActiveItem = thisIndividual->activeItems->activeItemArr[i];

		if(tmpActiveItem != NULL){
			tmpActiveItem->remaningTurns--;

			if(tmpActiveItem->remaningTurns <= 0){
				char * tmp[64];
				sprintf(tmp, "%s has worn off.", tmpActiveItem->thisItem->name);
				cwrite(tmp);

				thisIndividual->activeItems->activeItemArr[i] = NULL;
				thisIndividual->activeItems->activeItemsTotal--;
			}else{
				consumeItem(thisIndividual,tmpActiveItem->thisItem);
				if(thisIndividual->hp <= 0){
					char * tmp[128];
					sprintf(tmp, "%s has perished from %s!", thisIndividual->name, tmpActiveItem->thisItem->name);
					cwrite(tmp);
					removeFromExistance(thisIndividual->ID);
					return 1;
				}
				itemsPassed++;
			}

			if(itemsPassed >= thisIndividual->activeItems->activeItemsTotal){
				break;
			}
		}

	}

	return 0;
}

int processActiveAbilities(individual * thisIndividual){
	int i, abilitiesPassed = 0;
	activeAbilityList * activeAbilities = thisIndividual->activeAbilities;

	for(i = 0; i < activeAbilities->MAX_ABILITIES; i++){
		activeAbility * thisActiveAbility = activeAbilities->abilitiesList[i];
		if (thisActiveAbility != NULL) {
			abilitiesPassed++;

			if (thisActiveAbility->thisAbility->type != 'p') {
				if (thisActiveAbility->turnsRemaining - 1 >= 0) {
					thisActiveAbility->turnsRemaining--;
					useActiveAbility(thisIndividual, thisActiveAbility->thisAbility);

					if (thisIndividual->hp <= 0) {
						char * tmp[128];
						sprintf(tmp, "%s has perished from %s!", thisIndividual->name, thisActiveAbility->thisAbility->name);
						cwrite(tmp);
						removeFromExistance(thisIndividual->ID);
						return 1;
					}
				} else {
					char * tmp[64];
					sprintf(tmp, "Ability %s has finished.",thisActiveAbility->thisAbility->name);
					cwrite(tmp);

					activeAbilities->abilitiesList[i] = NULL;
					activeAbilities->numAbilities--;

					postProcessAbility(thisIndividual, thisActiveAbility);
				}
			}

			if (abilitiesPassed >= activeAbilities->numAbilities) {
				break;
			}

		}
	}

	return 0;
}

void postProcessAbility(individual * thisIndividual, ability * endingAbility){
	int totalHP = getTotalHP(thisIndividual);
	int totalMana = getTotalMana(thisIndividual);

	if(thisIndividual->hp > totalHP){
		thisIndividual->hp = totalHP;
	}

	if(thisIndividual->mana > totalMana){
		thisIndividual->mana = totalMana;
	}

}

int processStatuses(individual * thisIndividual){
	int i, statusesPassed = 0;

	for(i = 0; i < thisIndividual->activeStatuses->MAX_STATUSES; i++){
		status * tmpStatus = thisIndividual->activeStatuses->statuses[i];
		if(tmpStatus != NULL){
			statusesPassed++;
			if(tmpStatus->turnsRemaining >= 0){
				processStatus(thisIndividual, tmpStatus);

				if(thisIndividual->hp <= 0){
					char * tmp[128];
					sprintf(tmp, "%s has perished from ailment!", thisIndividual->name);
					cwrite(tmp);
					removeFromExistance(thisIndividual->ID);
					return 1;
				}
			}else{
				char * tmp[64];
				sprintf(tmp, "%s recovered from %s.", thisIndividual->name, lookUpStatusEffectName(tmpStatus->effect));
				cwrite(tmp);

				thisIndividual->activeStatuses->statuses[i] = NULL;
				thisIndividual->activeStatuses->numStatuses--;

				free(tmpStatus);
			}

			if(statusesPassed >= thisIndividual->activeStatuses->numStatuses){
				break;
			}
		}
	}

	return 0;
}

void processStatus(individual * thisIndividual, status * thisStatus){
	thisStatus->turnsRemaining--;

	switch(thisStatus->effect){
		case STATUS_POISONED:
		case STATUS_BURNING:
		case STATUS_BLEEDING:{
			damageIndividualWithStatus(thisIndividual, thisStatus);
		}
	}
}

statusEffect lookUpStatusType(char * statusType[16]){
	if(strcmp(statusType, "None") == 0){
		return STATUS_NONE;
	}else if(strcmp(statusType, "Poison") == 0){
		return STATUS_POISONED;
	}else if(strcmp(statusType, "Paralysis") == 0){
		return STATUS_PARALYZED;
	}else if(strcmp(statusType, "Confusion") == 0){
		return STATUS_CONFUSED;
	}else if(strcmp(statusType, "Burn") == 0){
		return STATUS_BURNING;
	}else if(strcmp(statusType, "Bleed") == 0){
		return STATUS_BLEEDING;
	}else if(strcmp(statusType, "Berzerk") == 0){
		return STATUS_BERZERK;
	}else if(strcmp(statusType, "Silence") == 0){
		return STATUS_SILENCED;
	}

	char * tmpStr[64];
	sprintf(tmpStr, "!! STATUS TYPE NOT FOUND: %s", statusType);
	cwrite(tmpStr);

	return STATUS_NONE;
}

void damageIndividualWithStatus(individual * thisIndividual, status * thisStatus){
	int damage = 0;

	int diceDamage = thisStatus->diceDamage;
	if(diceDamage > 0){
		damage = (rand() % diceDamage) + 1;
	}

	damage += thisStatus->damage;

	if(damage < 0){
		damage = 0;
	}

	thisIndividual->hp -= damage;

	char * hitMessage[128];
	switch(thisStatus->effect){
		case STATUS_POISONED:{
			if(damage > 0){
				sprintf(hitMessage, "%s took %d damage from %s",thisIndividual->name, damage, "poison");
			}else{
				sprintf(hitMessage, "%s was unaffected by the %s",thisIndividual->name, "poison");
			}
		}
		break;
		case STATUS_BURNING:{
			if(damage > 0){
				sprintf(hitMessage, "%s took %d damage from %s",thisIndividual->name, damage, "burning");
			}else{
				sprintf(hitMessage, "%s was unaffected by the %s",thisIndividual->name, "burning");
			}
		}
		break;
		case STATUS_BLEEDING:{
			if(damage > 0){
				sprintf(hitMessage, "%s took %d damage from %s",thisIndividual->name, damage, "bleeding");
			}else{
				sprintf(hitMessage, "%s was unaffected by the %s",thisIndividual->name, "bleeding");
			}
		}
		break;

		strcpy(hitMessage, "!! damageIndividualWithStatus() COULD NOT FIND STATUS !!");
	}

	cwrite(hitMessage);
}

char * lookUpStatusEffectName(statusEffect effect){
	switch(effect){
	case(STATUS_POISONED):
		return "poison";
	case(STATUS_PARALYZED):
		return "paralysis";
	case(STATUS_CONFUSED):
		return "confusion";
	case(STATUS_BURNING):
		return "burning";
	case(STATUS_BLEEDING):
		return "bleeding";
	case(STATUS_BERZERK):
		return "berzerk";
	case(STATUS_SILENCED):
		return "silence";
	}

	return "!! STATUS NOT FOUND !!";
}

void useActiveAbility(individual * thisIndividual, ability * thisAbility){
	int amountHealed = 0;

	if(thisAbility->hpEnabled){
		amountHealed += thisAbility->hp->effectAndManaArray[thisAbility->hp->selectedIndex]->effectMagnitude;
		healIndividual(thisIndividual, thisAbility->hp->effectAndManaArray[thisAbility->hp->selectedIndex]->effectMagnitude);
	}

	if(thisAbility->diceHPEnabled){
		int diceHP = thisAbility->diceHP->effectAndManaArray[thisAbility->diceHP->selectedIndex]->effectMagnitude;

		if(diceHP != 0){
			int diceHeal = (rand() % diceHP) + 1;
			amountHealed += diceHeal;
			healIndividual(thisIndividual, diceHeal);
		}
	}

	if(amountHealed > 0){
		char * tmp[64];
		sprintf(tmp, "%s healed %d hp from %s", thisIndividual->name, amountHealed, thisAbility->name);
		cwrite(tmp);
	}
}

void decreaseTurns(individual * thisIndividual, int * enemyActionMode, int * initEnemyActionMode, int numTurns){

	if(thisIndividual->activeAbilities->selectedAbility != NULL && thisIndividual->activeAbilities->selectedAbility->type == 'i'){

		if(thisIndividual->activeAbilities->selectedAbility->actionsEnabled){
			numTurns += thisIndividual->activeAbilities->selectedAbility->actions->effectAndManaArray[thisIndividual->activeAbilities->selectedAbility->actions->selectedIndex]->effectMagnitude;
		 }

		removeActiveAbility(thisIndividual, thisIndividual->activeAbilities->selectedAbility);

		thisIndividual->activeAbilities->selectedAbility = NULL;
	}

	thisIndividual->remainingActions -= numTurns;

	if (thisIndividual->remainingActions <= 0) {
		endTurn(thisIndividual);
		*enemyActionMode = 1;
		*initEnemyActionMode = 1;
	}
}



void endTurn(individual *thisIndividual){
	printf("player turn ended\n");
	thisIndividual->hasAttacked = 0;
	thisIndividual->remainingActions += thisIndividual->totalActions;
}

int individualWithinRange(individual * thisIndividual, individual * target){
	int range = getAttributeSum(thisIndividual, "range");

	if(abs(thisIndividual->playerCharacter->x - target->playerCharacter->x) <= range
	&& abs(thisIndividual->playerCharacter->y - target->playerCharacter->y) <= range){
		return 1;
	}
	return 0;
}

int attackIfInRange(individual *thisIndividual, individual *targetIndividual){
	if(individualWithinRange(thisIndividual, targetIndividual)){
		attackIndividual(thisIndividual, targetIndividual);
		return 1;
	}else{
		return 0;
	}
}

int calcStrengthDamageMod(individual * thisIndividual){
	item * weapon = getActiveWeapon(thisIndividual->backpack);
	int str = getAttributeSum(thisIndividual, "STR");

	if(weapon != NULL){
		return floor(str * weapon->weaponStrMod);
	}else{
		return str;
	}

}

int calcMinDam(individual * thisIndividual){
	item * weapon = getActiveWeapon(thisIndividual->backpack);
	if (weapon == NULL) {
		return thisIndividual->minDam;
	} else {
		return weapon->minDamMod;
	}
}

int calcMaxDam(individual * thisIndividual){
	item * weapon = getActiveWeapon(thisIndividual->backpack);
	if (weapon == NULL) {
		return thisIndividual->maxDam;
	} else {
		return weapon->maxDamMod;
	}
}

void drawIndividual(HDC hdc, HDC hdcBuffer, individual* thisIndividual, shiftData * viewShift){
		HDC hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, thisIndividual->playerCharacter->imageMask);

		BitBlt(hdcBuffer, thisIndividual->playerCharacter->x*40 - (viewShift->xShift)*40, thisIndividual->playerCharacter->y*40 - (viewShift->yShift)*40, thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height, hdcMem, 0, 0, SRCAND);

		SelectObject(hdcMem, thisIndividual->playerCharacter->image);

		BitBlt(hdcBuffer, thisIndividual->playerCharacter->x*40 - (viewShift->xShift)*40, thisIndividual->playerCharacter->y*40 - (viewShift->yShift)*40, thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height, hdcMem, 0, 0, SRCPAINT);
		DeleteDC(hdcMem);

}

item * removeItemFromInventory(inventory * backpack, item * thisItem){
	int i;
	item * removedItem;

	for(i = 0; i < 40; i++){
		if(backpack->inventoryArr[i] == thisItem){
			removedItem = backpack->inventoryArr[i];
			removedItem->isEquipt = 0;

			backpack->inventoryArr[i] = NULL;
			backpack->inventorySize--;

			return removedItem;
		}
	}
}

void modifyItem(item * theItem, individual * thisIndividual) {
	char type = theItem->type;
	switch (type) {
		case 'w': {
			if(theItem->isEquipt){
				theItem->isEquipt = 0;
			}else{
				tryEquipItem(thisIndividual->backpack, theItem);
			}
		}
		break;
		case 'a': {
			if(theItem->isEquipt){
				theItem->isEquipt = 0;
			}else{
				tryEquipItem(thisIndividual, theItem);
			}
		}
		break;
		case 'i': {
			if(theItem->itemType == 'c'){
				consumeItem(thisIndividual, theItem);
				free(removeItemFromInventory(thisIndividual->backpack, theItem));
			}else if(theItem->itemType == 'd'){
				if(thisIndividual->activeItems->activeItemsTotal < 40){
					consumeItem(thisIndividual, theItem);
					addItemToActiveItemList(thisIndividual, theItem);
					removeItemFromInventory(thisIndividual->backpack, theItem);
				}
			}
		}
	}
}

void tryEquipItem(inventory * backpack, item * thisItem){
	int i;
	item * tmpItem;

	for(i = 0; i < 40; i++){
		tmpItem = backpack->inventoryArr[i];
		if(tmpItem != NULL && tmpItem != thisItem && tmpItem->type == thisItem->type && tmpItem->isEquipt){
			return;
		}
	}

	thisItem->isEquipt = 1;
}

void addItemToActiveItemList(individual * thisIndividual, item * theItem){
	int i, numTurns = 0;

		for(i = 0; i < 40; i++){

			if(thisIndividual->activeItems->activeItemArr[i] == NULL){
				activeItem * newActiveItem = malloc(sizeof(activeItem));
				newActiveItem->thisItem = theItem;

				if(theItem->maxTurns <= theItem->minTurns){
					numTurns = theItem->minTurns;
				}else{
					numTurns = (rand() % (theItem->maxTurns - theItem->minTurns)) + theItem->minTurns;
				}

				newActiveItem->remaningTurns = numTurns;

				thisIndividual->activeItems->activeItemArr[i] = newActiveItem;
				thisIndividual->activeItems->activeItemsTotal++;
				break;
			}
		}

}

int addItemToIndividual(inventory * backpack, item * newItem){
	int i, availableSpot;
	if(backpack->inventorySize < 40){
		for(i = 0; i < 40; i++){
			if(backpack->inventoryArr[i] == NULL){
				backpack->inventoryArr[i] = newItem;
				backpack->inventorySize++;
				return 1;
			}
		}
	}

	return 0;
}

void consumeItem(individual * thisIndividual, item * theItem){
	//note, +/- healthMod
	if(theItem->healthMod != 0){
		if(theItem->healthMod > 0){
			healIndividual(thisIndividual, theItem->healthMod);
		}
	}

	if(theItem->manaMod != 0){
		if(theItem->manaMod > 0){
			restoreMana(thisIndividual, theItem->manaMod);
		}
	}
}

void healIndividual(individual * thisIndividual, int hp){
	int totalHP = getTotalHP(thisIndividual);

	if(totalHP - thisIndividual->hp < hp){
		thisIndividual->hp = totalHP;
	}else{
		thisIndividual->hp += hp;
	}
}

int getTotalHP(individual * thisIndividual){
	return getAttributeSum(thisIndividual, "baseHP") + 2 * getAttributeSum(thisIndividual, "CON");
}

void restoreMana(individual * thisIndividual, int mana){
	int totalMana = getTotalMana(thisIndividual);

	if(totalMana - thisIndividual->mana < mana){
		thisIndividual->mana = totalMana;
	}else{
		thisIndividual->mana += mana;
	}
}


int getTotalMana(individual * thisIndividual){
	return getAttributeSum(thisIndividual, "baseMana") + 2 * getAttributeSum(thisIndividual, "WILL");
}

int attemptToBuyItem(item * thisItem, individual * thisIndividual){

	if(thisItem->price <= thisIndividual->gold){
		item * newItem = cloneItem(thisItem);

		if(newItem == NULL){
			char * errLog[128];
			sprintf(errLog, "!! ITEM %d COULD NOT BE CLONED!!", thisItem->ID);
			return 0;
		}

		addItemToRegistry(newItem);
		addItemToIndividual(thisIndividual->backpack, newItem);

		thisIndividual->gold -= thisItem->price;
		return 1;
	}
	return 0;

}

void addAbilityToIndividual(individual * thisIndividual, ability * newAbility){
	if (newAbility->type == 'p') {
		addActiveAbilityToIndividual(thisIndividual, newAbility, 0);
	}

	if (thisIndividual->abilities->numAbilities == thisIndividual->abilities->MAX_ABILITIES) {
		cwrite("!! CANNOT ADD MORE ABILITIES !!");
	}

	thisIndividual->abilities->abilitiesList[thisIndividual->abilities->numAbilities] = newAbility;
	thisIndividual->abilities->numAbilities++;
}

void addActiveAbilityToIndividual(individual * thisIndividual, ability * thisAbility, int duration){
	if(thisIndividual->activeAbilities->numAbilities == thisIndividual->activeAbilities->MAX_ABILITIES){
		cwrite("!! CANNOT ADD MORE ACTIVE ABILITIES !!");
	}

	activeAbility * newActiveAbility = malloc(sizeof(activeAbility));

	newActiveAbility->thisAbility = thisAbility;
	newActiveAbility->turnsRemaining = duration;

	thisIndividual->activeAbilities->abilitiesList[thisIndividual->activeAbilities->numAbilities] = newActiveAbility;
	thisIndividual->activeAbilities->numAbilities++;
}

void removeActiveAbility(individual * thisIndividual, ability * selectedAbility){
	int i;
	for(i = 0; i < thisIndividual->activeAbilities->MAX_ABILITIES; i++){
		if(thisIndividual->activeAbilities->abilitiesList[i] != NULL && thisIndividual->activeAbilities->abilitiesList[i]->thisAbility == selectedAbility){
			free(thisIndividual->activeAbilities->abilitiesList[i]);
			thisIndividual->activeAbilities->abilitiesList[i] = NULL;
			thisIndividual->activeAbilities->numAbilities--;
			return;
		}
	}
}

int useAbility(individual * thisIndividual, ability * thisAbility){
	//target cursor mode
	if(thisAbility->type == 't' || thisAbility->type == 'd'){ //duration
		thisIndividual->activeAbilities->selectedAbility = thisAbility;
	}else if(thisAbility->type == 'i'){
		thisIndividual->activeAbilities->selectedAbility = thisAbility;

		decreaseMana(thisIndividual, thisAbility->totalManaCost);
		addActiveAbilityToIndividual(thisIndividual, thisAbility, 0);

		useActiveAbility(thisIndividual, thisAbility);

		if(processCasterOnlyAffects(thisIndividual, thisAbility)){
			char * tmp[128];
			sprintf(tmp, "%s perished from %s!", thisIndividual->name, thisAbility->name);
			cwrite(tmp);
			return 1;
		}
	}

	return 0;
}

void decreaseMana(individual * thisIndividual, int mana){
	thisIndividual->mana -= mana;
}

int canUseAbility(individual * thisIndividual, ability * thisAbility){
	if(thisAbility->type != 'p' && thisIndividual->mana - thisAbility->totalManaCost >= 0){
		return 1;
	}else{
		return 0;
	}
}

int getAttributeFromIndividual(individual * thisIndividual, char * attribute){
	int toReturn = 0;

	if(strcmp("STR",attribute) == 0 ){
			return thisIndividual->STR;
	}else if(strcmp("DEX",attribute) == 0){
		return thisIndividual->DEX;
	}else if(strcmp("CON",attribute) == 0){
		return thisIndividual->CON;
	}else if(strcmp("WILL",attribute) == 0){
		return thisIndividual->WILL;
	}else if(strcmp("INT",attribute) == 0){
		return thisIndividual->INT;
	}else if(strcmp("WIS",attribute) == 0){
		return thisIndividual->WIS;
	}else if(strcmp("CHR",attribute) == 0){
		return thisIndividual->CHR;
	}else if(strcmp("LUCK",attribute) == 0){
		return thisIndividual->LUCK;
	}else if(strcmp("baseHP",attribute) == 0 ){
		return thisIndividual->baseHP;
	} else if(strcmp("health",attribute) == 0 ){
		return thisIndividual->hp;
	} else if(strcmp("baseMana",attribute) == 0 ){
		return thisIndividual->baseMana;
	} else if(strcmp("mana",attribute) == 0 ){
		return thisIndividual->mana;
	} else if(strcmp("minTurns",attribute) == 0 ){ //item only for now
		return 0;
	} else if(strcmp("maxTurns",attribute) == 0 ){ //item only for now
		return 0;
	} else if(strcmp("ac",attribute) == 0 ){
		return thisIndividual->AC;
	} else if(strcmp("attack",attribute) == 0 ){
		return thisIndividual->attack;
	} else if(strcmp("baseDam",attribute) == 0 ){
		return thisIndividual->baseDam;
	} else if(strcmp("maxDam",attribute) == 0 ){
		cwrite("!! UNSUPPORTED ATTRIBUTE TYPE: maxDam !!");
		return 0;
	} else if(strcmp("minDam",attribute) == 0 ){
		cwrite("!! UNSUPPORTED ATTRIBUTE TYPE: minDam !!");
		return 0;
	} else if(strcmp("mvmt",attribute) == 0 ){
		return thisIndividual->mvmt;
	} else if(strcmp("range",attribute) == 0 ){
		return thisIndividual->range;
	} else if(strcmp("bluntDR",attribute) == 0 ){
		return thisIndividual->bluntDR;
	} else if(strcmp("chopDR",attribute) == 0 ){
		return thisIndividual->chopDR;
	} else if(strcmp("slashDR",attribute) == 0 ){
		return thisIndividual->slashDR;
	} else if(strcmp("pierceDR",attribute) == 0 ){
		return thisIndividual->pierceDR;
	} else if(strcmp("earthDR",attribute) == 0 ){
		return thisIndividual->earthDR;
	} else if(strcmp("fireDR",attribute) == 0 ){
		return thisIndividual->fireDR;
	} else if(strcmp("waterDR",attribute) == 0 ){
		return thisIndividual->waterDR;
	} else if(strcmp("lightningDR",attribute) == 0 ){
		return thisIndividual->lightningDR;
	} else if(strcmp("earthWeakness",attribute) == 0 ){
		return thisIndividual->earthWeakness;
	} else if(strcmp("fireWeakness",attribute) == 0 ){
		return thisIndividual->fireWeakness;
	} else if(strcmp("waterWeakness",attribute) == 0 ){
		return thisIndividual->waterWeakness;
	} else if(strcmp("lightiningWeakness",attribute) == 0 ){
		return thisIndividual->lightiningWeakness;
	}

	return toReturn;
}

int getAttributeFromItem(item * thisItem, item * activeItem, char * attribute){
	int toReturn = 0;
	if(strcmp("STR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->strMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->strMod;
		 }
	}else if(strcmp("DEX",attribute) == 0){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->dexMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->dexMod;
		 }
	}else if(strcmp("CON",attribute) == 0){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->conMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->conMod;
		 }
	}else if(strcmp("WILL",attribute) == 0){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->willMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->willMod;
		 }
	}else if(strcmp("INT",attribute) == 0){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->intMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->intMod;
		 }
	}else if(strcmp("WIS",attribute) == 0){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->wisMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->wisMod;
		 }
	}else if(strcmp("CHR",attribute) == 0){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->chrMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->chrMod;
		 }
	}else if(strcmp("LUCK",attribute) == 0){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->luckMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->luckMod;
		 }
	}else if(strcmp("baseHP",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->totalHealthMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->totalHealthMod;
		 }
	} else if(strcmp("health",attribute) == 0 ){
		return 0;
	} else if(strcmp("baseMana",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->totalManaMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->totalManaMod;
		 }
	} else if(strcmp("mana",attribute) == 0 ){
			return 0;
	} else if(strcmp("minTurns",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->minTurns;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->minTurns;
		 }
	} else if(strcmp("maxTurns",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->maxTurns;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->maxTurns;
		 }
	} else if(strcmp("ac",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->acMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->acMod;
		 }
	} else if(strcmp("attack",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->attackMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->attackMod;
		 }
	} else if(strcmp("baseDam",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->damMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->damMod;
		 }
	} else if(strcmp("maxDam",attribute) == 0 ){
		cwrite("!! UNSUPPORTED ATTRIBUTE TYPE: maxDam !!");
		return 0;
	} else if(strcmp("minDam",attribute) == 0 ){
		cwrite("!! UNSUPPORTED ATTRIBUTE TYPE: minDam !!");
		return 0;
	} else if(strcmp("mvmt",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->mvmtMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->mvmtMod;
		 }
	} else if(strcmp("range",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->rangeMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->rangeMod;
		 }
	} else if(strcmp("bluntDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->bluntDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->bluntDRMod;
		 }
	} else if(strcmp("chopDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->chopDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->chopDRMod;
		 }
	} else if(strcmp("slashDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->slashDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->slashDRMod;
		 }
	} else if(strcmp("pierceDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->pierceDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->pierceDRMod;
		 }
	} else if(strcmp("earthDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->earthDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->earthDRMod;
		 }
	} else if(strcmp("fireDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->fireDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->fireDRMod;
		 }
	} else if(strcmp("waterDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->waterDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->waterDRMod;
		 }
	} else if(strcmp("lightningDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->lightningDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->lightningDRMod;
		 }
	} else if(strcmp("earthWeakness",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->earthWeaknessMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->earthWeaknessMod;
		 }
	} else if(strcmp("fireWeakness",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->fireWeaknessMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->fireWeaknessMod;
		 }
	} else if(strcmp("waterWeakness",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->waterWeaknessMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->waterWeaknessMod;
		 }
	} else if(strcmp("lightiningWeakness",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->lightiningWeaknessMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->lightiningWeaknessMod;
		 }
	}
	return toReturn;
}

int getAttributeFromActiveAbility(ability * activeAbility, char * attribute){
	if(strcmp("STR",attribute) == 0 ){
		if(activeAbility->STREnabled){
			return activeAbility->STR->effectAndManaArray[activeAbility->STR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	}else if(strcmp("DEX",attribute) == 0){
		if(activeAbility->DEXEnabled){
			return activeAbility->DEX->effectAndManaArray[activeAbility->DEX->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	}else if(strcmp("CON",attribute) == 0){
		if(activeAbility->CONEnabled){
			return activeAbility->CON->effectAndManaArray[activeAbility->CON->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	}else if(strcmp("WILL",attribute) == 0){
		if(activeAbility->WILLEnabled){
			return activeAbility->WILL->effectAndManaArray[activeAbility->WILL->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	}else if(strcmp("INT",attribute) == 0){
		if(activeAbility->INTEnabled){
			return activeAbility->INT->effectAndManaArray[activeAbility->INT->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	}else if(strcmp("WIS",attribute) == 0){
		if(activeAbility->WISEnabled){
			return activeAbility->WIS->effectAndManaArray[activeAbility->WIS->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	}else if(strcmp("CHR",attribute) == 0){
		if(activeAbility->CHREnabled){
			return activeAbility->CHR->effectAndManaArray[activeAbility->CHR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	}else if(strcmp("LUCK",attribute) == 0){
		if(activeAbility->LUCKEnabled){
			return activeAbility->LUCK->effectAndManaArray[activeAbility->LUCK->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	}else if(strcmp("baseHP",attribute) == 0 ){
		if(activeAbility->baseHPEnabled){
			return activeAbility->baseHP->effectAndManaArray[activeAbility->baseHP->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("health",attribute) == 0 ){
			return 0;
	} else if(strcmp("baseMana",attribute) == 0 ){
		if(activeAbility->baseManaEnabled){
			return activeAbility->baseMana->effectAndManaArray[activeAbility->baseMana->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("mana",attribute) == 0 ){
			return 0;
	} else if(strcmp("ac",attribute) == 0 ){
		if(activeAbility->acEnabled){
			return activeAbility->ac->effectAndManaArray[activeAbility->ac->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("attack",attribute) == 0 ){
		if(activeAbility->attackEnabled){
			return activeAbility->attack->effectAndManaArray[activeAbility->attack->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("baseDam",attribute) == 0 ){
		if(activeAbility->damageModEnabled){
			return activeAbility->damageMod->effectAndManaArray[activeAbility->damageMod->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("maxDam",attribute) == 0 ){
		cwrite("!! UNSUPPORTED ATTRIBUTE TYPE: maxDam !!");
		return 0;
	} else if(strcmp("minDam",attribute) == 0 ){
		cwrite("!! UNSUPPORTED ATTRIBUTE TYPE: minDam !!");
		return 0;
	} else if(strcmp("mvmt",attribute) == 0 ){
		if(activeAbility->mvmtEnabled){
			return activeAbility->mvmt->effectAndManaArray[activeAbility->mvmt->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("range",attribute) == 0 ){
		if(activeAbility->rangeEnabled && (activeAbility->type == 'p' || activeAbility->type == 'd')){ //only applicable to perminant abilities
			return activeAbility->range->effectAndManaArray[activeAbility->range->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("bluntDR",attribute) == 0 ){
		if(activeAbility->bluntDREnabled){
			return activeAbility->bluntDR->effectAndManaArray[activeAbility->bluntDR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("chopDR",attribute) == 0 ){
		if(activeAbility->chopDREnabled){
			return activeAbility->chopDR->effectAndManaArray[activeAbility->chopDR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("slashDR",attribute) == 0 ){
		if(activeAbility->slashDREnabled){
			return activeAbility->slashDR->effectAndManaArray[activeAbility->slashDR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("pierceDR",attribute) == 0 ){
		if(activeAbility->pierceDREnabled){
			return activeAbility->pierceDR->effectAndManaArray[activeAbility->pierceDR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("earthDR",attribute) == 0 ){
		if(activeAbility->earthDREnabled){
			return activeAbility->earthDR->effectAndManaArray[activeAbility->earthDR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("fireDR",attribute) == 0 ){
		if(activeAbility->fireDREnabled){
			return activeAbility->fireDR->effectAndManaArray[activeAbility->fireDR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("waterDR",attribute) == 0 ){
		if(activeAbility->waterDREnabled){
			return activeAbility->waterDR->effectAndManaArray[activeAbility->waterDR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("lightningDR",attribute) == 0 ){
		if(activeAbility->lightningDREnabled){
			return activeAbility->lightningDR->effectAndManaArray[activeAbility->lightningDR->selectedIndex]->effectMagnitude;
		}else{
			return 0;
		}
	} else if(strcmp("earthWeakness",attribute) == 0 ){
//		if(activeAbility->earthDR){
//			return activeAbility->XXXXX->effectAndManaArray[activeAbility->XXXXXX->selectedIndex]->effectMagnitude;
//		}else{
			return 0;
//		}
	} else if(strcmp("fireWeakness",attribute) == 0 ){
//		if(activeAbility->XXXXXX){
//			return activeAbility->XXXXX->effectAndManaArray[activeAbility->XXXXXX->selectedIndex]->effectMagnitude;
//		}else{
			return 0;
//		}
	} else if(strcmp("waterWeakness",attribute) == 0 ){
//		if(activeAbility->XXXXXX){
//			return activeAbility->XXXXX->effectAndManaArray[activeAbility->XXXXXX->selectedIndex]->effectMagnitude;
//		}else{
			return 0;
//		}
	} else if(strcmp("lightiningWeakness",attribute) == 0 ){
//		if(activeAbility->XXXXXX){
//			return activeAbility->XXXXX->effectAndManaArray[activeAbility->XXXXXX->selectedIndex]->effectMagnitude;
//		}else{
			return 0;
//		}
	}
}

int getAttributeSum(individual * thisIndividual, char * attribute){
	int toReturn = 0, i, itemTotal = 0, activeItemTotal = 0, abilitiesPassed = 0;

	toReturn += getAttributeFromIndividual(thisIndividual, attribute);

	for(i = 0; i < 40; i++){
		activeItem * tmpActiveItem;
		// have all items been used?
		if(itemTotal == thisIndividual->backpack->inventorySize &&
			activeItemTotal == thisIndividual->activeItems->activeItemsTotal){
			break;
		}
		if(thisIndividual->activeItems->activeItemArr[i] != NULL){
			toReturn += getAttributeFromItem(thisIndividual->backpack->inventoryArr[i], thisIndividual->activeItems->activeItemArr[i]->thisItem, attribute);
			activeItemTotal++;
		}else{
			toReturn += getAttributeFromItem(thisIndividual->backpack->inventoryArr[i], NULL, attribute);
		}

		if(thisIndividual->backpack->inventoryArr[i] != NULL){
			itemTotal++;
		}

	}

	for(i = 0; i < thisIndividual->activeAbilities->MAX_ABILITIES; i++){
		if(thisIndividual->activeAbilities->abilitiesList[i] != NULL){
			abilitiesPassed++;
			toReturn += getAttributeFromActiveAbility(thisIndividual->activeAbilities->abilitiesList[i]->thisAbility, attribute);

		if(abilitiesPassed >= thisIndividual->activeAbilities->numAbilities){
			break;
		}
		}
	}

	return toReturn;
}
