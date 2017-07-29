/*
 * individual.c
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#include"./headers/individual_pub_methods.h"

static int HIT_IMAGE_ID = 10000;
static int MISS_IMAGE_ID = 10001;

static int SOUND_BLUNT_ID = 4;
static int SOUND_CHOP_ID = 5;
static int SOUND_PIERCE_ID = 6;
static int SOUND_SLASH_ID = 7;
static int SOUND_SLICE_ID = 8;

int isRandomized = 0;

individual *initIndividual(){
	int i;

	individual* toReturn = malloc(sizeof(individual));
	toReturn->playerCharacter = malloc(sizeof(character));
	toReturn->playerCharacter->darkAnimationContainer = NULL;

	toReturn->currentInteractableObject = NULL;

	toReturn->backpack = malloc(sizeof(inventory));
	toReturn->backpack->inventorySize = 0;
	toReturn->backpack->MAX_ITEMS = 40;

	toReturn->activeItems = malloc(sizeof(activeItemList));
	toReturn->activeItems->activeItemsTotal = 0;

	for (i = 0; i < 40; i++) {
		toReturn->backpack->inventoryArr[i] = NULL;
		toReturn->activeItems->activeItemArr[i] = NULL;
	}

	toReturn->abilities = malloc(sizeof(abilityList));
	toReturn->abilities->numAbilities = 0;
	toReturn->abilities->MAX_ABILITIES = 64;

	for(i = 0; i < toReturn->abilities->MAX_ABILITIES; i++){
		toReturn->abilities->abilitiesList[i] = NULL;
	}

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

	toReturn->targetedIndividual = NULL;
	toReturn->targetedDuration = 0;
	toReturn->allyIndividual = NULL;

	toReturn->thisBehavior = malloc(sizeof(behavior));
	toReturn->thisBehavior->isOffensive = 0;
	toReturn->thisBehavior->hasAbilityAffinity = 0;
	toReturn->thisBehavior->isTactical = 0;
	toReturn->thisBehavior->isCowardly = 0;
	toReturn->thisBehavior->cowardlyTurnsRemaining = 0;
	toReturn->thisBehavior->isHostileToPlayer = 0;
	toReturn->thisBehavior->isFocusedOnPlayer = 0;
	toReturn->thisBehavior->isThreatened = 0;
	toReturn->thisBehavior->isSurrounded = 0;
	toReturn->thisBehavior->respawns = 0;
	toReturn->thisBehavior->hasAlreadyYieldedToPlayer = 0;
	toReturn->thisBehavior->wasRecentlyAttacked = 0;
	toReturn->thisBehavior->alertDuration = 0;
	toReturn->thisBehavior->gotConfused = 0;

	toReturn->specialDialog = NULL;

	toReturn->thisActiveCrimes = malloc(sizeof(activeCrimes));
	toReturn->thisActiveCrimes->numActiveCrimes = 0;
	toReturn->thisActiveCrimes->MAX_ACTIVE_CRIMES = 200;

	for(i = 0; i < toReturn->thisActiveCrimes->MAX_ACTIVE_CRIMES; i++){
		toReturn->thisActiveCrimes->activeCrimeList[i] = NULL;
	}

	toReturn->thisReportedCrimes = malloc(sizeof(reportedCrimes));
	toReturn->thisReportedCrimes->numReportedCrimes = 0;
	toReturn->thisReportedCrimes->MAX_REPORTED_CRIMES = 200;

	for(i = 0; i < toReturn->thisReportedCrimes->MAX_REPORTED_CRIMES; i++){
		toReturn->thisReportedCrimes->reportedCrimeList[i] = NULL;
	}

	toReturn->desiredLocation = malloc(sizeof(cord));
	toReturn->thisMoveNodeMeta = malloc(sizeof(moveNodeMeta));

	toReturn->armorItem = NULL;
	toReturn->weaponItem = NULL;
	toReturn->shieldItem = NULL;

	return toReturn;
}

int defineIndividual(individual * thisIndividual, int ID, int isPlayer, COLORREF rgb, char * name, int direction, int x,
		int y, int STR, int DEX, int CON, int WILL, int INT, int WIS, int CHR, int LUCK, int hp, int mana, int food, int baseHP, int totalActions, int baseMana, int baseAC, int attack, int maxDam, int minDam, int baseDam,  char critType[3],
		int range, int mvmt, int LoS, int darkLoS, int isSneaking, int bluntDR, int chopDR, int slashDR, int pierceDR, int earthDR, int fireDR, int waterDR, int lightningDR,
		int dialogID, int dialogPortraitID, int fateTokens, int gold, int faction, groupType defaultType, groupType currentType,  int offensiveness, int abilityAffinity, int tacticalness, int cowardness,
		int isHostileToPlayer, int isFocusedOnPlayer, int isSurrounded, int respawns, int desiredLocationX, int desiredLocationY,
		specialDialogs * thisDialog, abilityList * loadedAbilities, animationContainer * thisAnimationContainer, animationContainer * secondaryAnimationContainer){
	int i;
	BITMAP bm;

	thisIndividual->ID = ID;
	thisIndividual->isPlayer = isPlayer;

	strcpy(thisIndividual->name, name);
	thisIndividual->playerCharacter->direction = direction;
	thisIndividual->playerCharacter->x = x;
	thisIndividual->playerCharacter->y = y;
	thisIndividual->playerCharacter->xOff = 0;
	thisIndividual->playerCharacter->yOff = 0;

	thisIndividual->playerCharacter->thisAnimationContainer = thisAnimationContainer;
	thisIndividual->playerCharacter->secondaryAnimationContainer = secondaryAnimationContainer;

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
	thisIndividual->LoS = LoS;
	thisIndividual->darkLoS = darkLoS;
	thisIndividual->isSneaking = isSneaking;

	thisIndividual->bluntDR = bluntDR;
	thisIndividual->chopDR = chopDR;
	thisIndividual->slashDR = slashDR;
	thisIndividual->pierceDR = pierceDR;

	thisIndividual->earthDR = earthDR;
	thisIndividual->fireDR = fireDR;
	thisIndividual->waterDR = waterDR;
	thisIndividual->lightningDR = lightningDR;

	thisIndividual->dialogID = dialogID;
	thisIndividual->dialogPortraitID = dialogPortraitID;

	thisIndividual->specialDialog = thisDialog;

	thisIndividual->fateTokens = fateTokens;
	thisIndividual->gold = gold;
	thisIndividual->faction = faction;

	thisIndividual->jumpTarget = 0;

	thisIndividual->baseHP = baseHP;

	if(hp == -1){
	thisIndividual->hp = baseHP + CON * 2;
	}else{
		thisIndividual->hp = hp;
	}

	thisIndividual->baseMana = baseMana;

	if(mana == -1){
		thisIndividual->mana = baseMana + WILL * 2;
	}else{
		thisIndividual->mana = mana;
	}

	thisIndividual->baseFood = 300 + CON * 50;

	if(food == -1){
		thisIndividual->food = thisIndividual->baseFood;
	}else if(food < 50){
		thisIndividual->food = 50;
	}else{
		thisIndividual->food = food;
	}

	if(thisIndividual->food > 300){
		thisIndividual->foodBonusActive = 1;
	}else{
		thisIndividual->foodBonusActive = 0;
	}

	if(thisIndividual->food < 50){
		thisIndividual->foodNegativeActive = 1;
	}else{
		thisIndividual->foodNegativeActive = 0;
	}

	thisIndividual->armorItem = NULL;
	thisIndividual->weaponItem = NULL;
	thisIndividual->shieldItem = NULL;

	thisIndividual->defaultGroupType = defaultType;
	thisIndividual->currentGroupType = currentType;

	thisIndividual->thisBehavior->offensiveness = offensiveness;
	thisIndividual->thisBehavior->abilityAffinity = abilityAffinity;
	thisIndividual->thisBehavior->tacticalness = tacticalness;
	thisIndividual->thisBehavior->cowardness = cowardness;
	thisIndividual->thisBehavior->isHostileToPlayer = isHostileToPlayer;
	thisIndividual->thisBehavior->isFocusedOnPlayer = isFocusedOnPlayer;
	thisIndividual->thisBehavior->isSurrounded = isSurrounded;
	thisIndividual->thisBehavior->respawns = respawns;

	if(loadedAbilities != NULL){
		for(i = 0; i < loadedAbilities->numAbilities; i++){
			addAbilityToIndividual(thisIndividual, loadedAbilities->abilitiesList[i]);
		}
	}

	thisIndividual->desiredLocation->x = desiredLocationX;
	thisIndividual->desiredLocation->y = desiredLocationY;

	character * shadowCharacter = malloc(sizeof(character));
	shadowCharacter->thisAnimationContainer = cloneAnimationContainer(thisIndividual->playerCharacter->thisAnimationContainer);
	shadowCharacter->secondaryAnimationContainer = cloneAnimationContainer(thisIndividual->playerCharacter->secondaryAnimationContainer);
	shadowCharacter->darkAnimationContainer = NULL;

	shadowCharacter->x = thisIndividual->playerCharacter->x;
	shadowCharacter->y = thisIndividual->playerCharacter->y;
	shadowCharacter->xOff = 0;
	shadowCharacter->yOff = 0;
	shadowCharacter->direction = thisIndividual->playerCharacter->direction;

	thisIndividual->thisMoveNodeMeta->sum = 0;
	thisIndividual->thisMoveNodeMeta->pathLength = 0;
	thisIndividual->thisMoveNodeMeta->shadowCharacter = shadowCharacter;
	thisIndividual->thisMoveNodeMeta->useDummyCords = 0;
	thisIndividual->thisMoveNodeMeta->dummyCord = malloc(sizeof(cord));

	return 0;
}

char * getIndividualAsLine(individual * thisIndividual){
	int i, j, dialogId, numPermAbilites = 0, numDurationAbilites = 0, numTargetedAbilities = 0, numInstantAbilities = 0;
	int permenantAbilities[64];
	int selfDurationAbilities[64];
	int targetAbilities[64];
	int instantAbilities[64];

	char * line = malloc(sizeof(char) * 2048);
	line[0] = '\0';

	animationContainer * thisAnimationContainer = thisIndividual->playerCharacter->thisAnimationContainer;

	for(j = 0; j < thisIndividual->abilities->numAbilities; j++){
		ability * tmpAbility = thisIndividual->abilities->abilitiesList [j];

		if(tmpAbility == NULL){
			continue;
		}

		switch(tmpAbility->type){
			case 'p':
				permenantAbilities[numPermAbilites] = tmpAbility->ID;
				numPermAbilites++;
				break;
			case 'd':
				selfDurationAbilities[numDurationAbilites] = tmpAbility->ID;
				numDurationAbilites++;
				break;
			case 't':
				targetAbilities[numTargetedAbilities] = tmpAbility->ID;
				numTargetedAbilities++;
				break;
			case 'i':
				instantAbilities[numInstantAbilities] = tmpAbility->ID;
				numInstantAbilities++;
				break;
		}
	}

	i = sprintf(line, "%d;", thisIndividual->ID);
	i += sprintf(line + i, "%d;", thisIndividual->isPlayer);
	i += sprintf(line + i, "%d;", 255); // r
	i += sprintf(line + i, "%d;", 0); // g
	i += sprintf(line + i, "%d;", 255); // b
	i += sprintf(line + i, "%s;", thisIndividual->name);
	i += sprintf(line + i, "%d;", thisIndividual->playerCharacter->direction);
	i += sprintf(line + i, "%d;", thisIndividual->playerCharacter->x);
	i += sprintf(line + i, "%d;", thisIndividual->playerCharacter->y);

	i += sprintf(line + i, "%d;", thisIndividual->STR);
	i += sprintf(line + i, "%d;", thisIndividual->DEX);
	i += sprintf(line + i, "%d;", thisIndividual->CON);
	i += sprintf(line + i, "%d;", thisIndividual->WILL);
	i += sprintf(line + i, "%d;", thisIndividual->INT);
	i += sprintf(line + i, "%d;", thisIndividual->WIS);
	i += sprintf(line + i, "%d;", thisIndividual->CHR);
	i += sprintf(line + i, "%d;", thisIndividual->LUCK);

	i += sprintf(line + i, "%d;", thisIndividual->hp);
	i += sprintf(line + i, "%d;", thisIndividual->mana);
	i += sprintf(line + i, "%d;", (int) thisIndividual->food);

	i += sprintf(line + i, "%d;", thisIndividual->baseHP);
	i += sprintf(line + i, "%d;", thisIndividual->totalActions);
	i += sprintf(line + i, "%d;", thisIndividual->baseMana);
	i += sprintf(line + i, "%d;", thisIndividual->AC);
	i += sprintf(line + i, "%d;", thisIndividual->attack);
	i += sprintf(line + i, "%d;", thisIndividual->maxDam);
	i += sprintf(line + i, "%d;", thisIndividual->minDam);
	i += sprintf(line + i, "%d;", thisIndividual->baseDam);
	i += sprintf(line + i, "%s;", thisIndividual->critType);
	i += sprintf(line + i, "%d;", thisIndividual->range);
	i += sprintf(line + i, "%d;", thisIndividual->mvmt);
	i += sprintf(line + i, "%d;", thisIndividual->LoS);
	i += sprintf(line + i, "%d;", thisIndividual->darkLoS);
	i += sprintf(line + i, "%d;", thisIndividual->isSneaking);

	i += sprintf(line + i, "%d;", thisIndividual->bluntDR);
	i += sprintf(line + i, "%d;", thisIndividual->chopDR);
	i += sprintf(line + i, "%d;", thisIndividual->slashDR);
	i += sprintf(line + i, "%d;", thisIndividual->pierceDR);
	i += sprintf(line + i, "%d;", thisIndividual->earthDR);
	i += sprintf(line + i, "%d;", thisIndividual->fireDR);
	i += sprintf(line + i, "%d;", thisIndividual->waterDR);
	i += sprintf(line + i, "%d;", thisIndividual->lightningDR);

	dialogId = getDefaultDialogIDFromIndividual(thisIndividual->ID);

	if(dialogId > -1){
		i += sprintf(line + i, "%d;", dialogId);
	}else{
		i += sprintf(line + i, "%d;", thisIndividual->dialogID);
	}

	i += sprintf(line + i, "%d;", thisIndividual->specialDialog->sawPlayerCrime);
	i += sprintf(line + i, "%d;", thisIndividual->specialDialog->attackedByPlayer);
	i += sprintf(line + i, "%d;", thisIndividual->specialDialog->stolenFromByPlayer);
	i += sprintf(line + i, "%d;", thisIndividual->specialDialog->afraidOfPlayer);
	i += sprintf(line + i, "%d;", thisIndividual->specialDialog->playerIsMarkedForDeath);
	i += sprintf(line + i, "%d;", thisIndividual->dialogPortraitID);
	i += sprintf(line + i, "%d;", thisIndividual->fateTokens);
	i += sprintf(line + i, "%d;", thisIndividual->gold);
	i += sprintf(line + i, "%d;", thisIndividual->faction);
	i += sprintf(line + i, "%d;", thisIndividual->defaultGroupType);
	i += sprintf(line + i, "%d;", thisIndividual->currentGroupType);
	i += sprintf(line + i, "%d;", thisIndividual->thisBehavior->offensiveness);
	i += sprintf(line + i, "%d;", thisIndividual->thisBehavior->abilityAffinity);
	i += sprintf(line + i, "%d;", thisIndividual->thisBehavior->tacticalness);
	i += sprintf(line + i, "%d;", thisIndividual->thisBehavior->cowardness);
	i += sprintf(line + i, "%d;", thisIndividual->thisBehavior->isHostileToPlayer);
	i += sprintf(line + i, "%d;", thisIndividual->thisBehavior->isFocusedOnPlayer);
	i += sprintf(line + i, "%d;", thisIndividual->thisBehavior->isSurrounded);
	i += sprintf(line + i, "%d;", thisIndividual->thisBehavior->respawns);
	i += sprintf(line + i, "%d;", thisIndividual->desiredLocation->x);
	i += sprintf(line + i, "%d;", thisIndividual->desiredLocation->y);

	if(numPermAbilites == 0){
		i += sprintf(line + i, "-1;");
	}else{
		i += sprintf(line + i, "%d,", numPermAbilites);

		for(j = 0; j < numPermAbilites; j++){
			if(j + 1 == numPermAbilites){
				i += sprintf(line + i, "%d;", permenantAbilities[j]);
			}else{
				i += sprintf(line + i, "%d,", permenantAbilities[j]);
			}
		}
	}

	if(numDurationAbilites == 0){
		i += sprintf(line + i, "-1;");
	}else{
		i += sprintf(line + i, "%d,", numDurationAbilites);

		for(j = 0; j < numDurationAbilites; j++){
			if(j + 1 == numDurationAbilites){
				i += sprintf(line + i, "%d;", selfDurationAbilities[j]);
			}else{
				i += sprintf(line + i, "%d,", selfDurationAbilities[j]);
			}

		}
	}

	if(numTargetedAbilities == 0){
		i += sprintf(line + i, "-1;");
	}else{
		i += sprintf(line + i, "%d,", numTargetedAbilities);

		for(j = 0; j < numTargetedAbilities; j++){
			if(j + 1 == numTargetedAbilities){
				i += sprintf(line + i, "%d;", targetAbilities[j]);
			}else{
				i += sprintf(line + i, "%d,", targetAbilities[j]);
			}
		}
	}

	if(numInstantAbilities == 0){
		i += sprintf(line + i, "-1;");
	}else{
		i += sprintf(line + i, "%d,", numInstantAbilities);

		for(j = 0; j < numInstantAbilities; j++){
			if(j + 1 == numInstantAbilities){
				i += sprintf(line + i, "%d;", instantAbilities[j]);
			}else{
				i += sprintf(line + i, "%d,", instantAbilities[j]);
			}
		}
	}

	i += sprintf(line + i, "%d;", thisAnimationContainer->animationsEnabled);
	i += sprintf(line + i, "%d;", thisAnimationContainer->defaultAnimation);
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_IDLE));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_SLASH));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_CHOP));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_BLUNT));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_PIERCE));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_BOW));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_HARM));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_DEATH));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_CAST));

	return line;
}

void destroyIndividual(individual* thisIndividual){
	int i, itemsPassed;
	if(thisIndividual->playerCharacter == NULL){
		destroyCharacter(thisIndividual->playerCharacter);
	}

	free(thisIndividual->activeItems);

	free(thisIndividual->backpack);

	free(thisIndividual->thisBehavior);

	free(thisIndividual);
}

item * getActiveWeapon(inventory * backpack){
	int i;
	item * tmpItem = NULL;

	for(i = 0; i < backpack->MAX_ITEMS; i++){
		if(backpack->inventoryArr[i] != NULL){
			tmpItem = backpack->inventoryArr[i];
			if(tmpItem->type == 'w' && tmpItem->isEquipt){
				return tmpItem;
			}
		}
	}

	return NULL;
}

void setAttackAnimation(individual * thisIndividual){
	item * weapon = getActiveWeapon(thisIndividual->backpack);

	if(weapon == NULL){
		//Use default animation, currently slash
		setIndividualAnimation(thisIndividual, ANIMATION_ATTACK_SLASH);
	}else{
		switch (weapon->weaponDamageType){
			case 's':
				setIndividualAnimation(thisIndividual, ANIMATION_ATTACK_SLASH);
				break;
			case 'b':
				setIndividualAnimation(thisIndividual, ANIMATION_ATTACK_BLUNT);
				break;
			case 'c':
				setIndividualAnimation(thisIndividual, ANIMATION_ATTACK_CHOP);
				break;
			case 'p':
				setIndividualAnimation(thisIndividual, ANIMATION_ATTACK_PIERCE);
				break;
			case 'a':
				setIndividualAnimation(thisIndividual, ANIMATION_ATTACK_BOW);
				break;
			default:
				setIndividualAnimation(thisIndividual, ANIMATION_ATTACK_SLASH);
				break;
		}
	}
}

void onAttackedChecks(individual *thisIndividual, individual *targetIndividual){
	targetIndividual->thisBehavior->wasRecentlyAttacked = 1;

	if(thisIndividual->isPlayer && !thisIndividual->thisBehavior->gotConfused){
		thisIndividual->targetedIndividual = targetIndividual;

		if((targetIndividual->currentGroupType == GROUP_NPCS || targetIndividual->currentGroupType == GROUP_GUARDS)){
			processCrimeEvent(CRIME_ASSULT, 40, targetIndividual->ID, 0);
		}
	}

	disableSleepStatusOnAttack(targetIndividual);
}

int attackIndividual(individual *thisIndividual, individual *targetIndividual){
	int d20 = rand() % 20 + 1;
	int totalAttack = d20 + getAttributeSum(thisIndividual, "attack");
	int totalAC = getAttributeSum(targetIndividual,"ac") + getAttributeSum(thisIndividual, "DEX");
	int i;
	item * tmpItem;

	triggerEventOnAttack(targetIndividual->ID, thisIndividual->isPlayer);
	onAttackedChecks(thisIndividual, targetIndividual);

	enableSpecialDrawMode();
	setDurationInTimerTicks(20);

	setAttackAnimation(thisIndividual);

	if(d20 == 20){
		addCharacterToSpecialDrawWithCoords(createCharacterFromAnimation(cloneAnimationFromRegistry(HIT_IMAGE_ID)), targetIndividual->playerCharacter->x, targetIndividual->playerCharacter->y);
		return damageIndividual(thisIndividual, targetIndividual, 1);
	} else if(d20 == 1){ //THE natural one.
		addCharacterToSpecialDrawWithCoords(createCharacterFromAnimation(cloneAnimationFromRegistry(MISS_IMAGE_ID)), targetIndividual->playerCharacter->x, targetIndividual->playerCharacter->y);
		cwrite("Where'd you learn to fight?\n");
		return 0;
	}

	if(totalAttack >= totalAC){ //Tie goes to attacker, of course.
		addCharacterToSpecialDrawWithCoords(createCharacterFromAnimation(cloneAnimationFromRegistry(HIT_IMAGE_ID)), targetIndividual->playerCharacter->x, targetIndividual->playerCharacter->y);
		return damageIndividual(thisIndividual, targetIndividual, 0);
	}else{ //miss
		addCharacterToSpecialDrawWithCoords(createCharacterFromAnimation(cloneAnimationFromRegistry(MISS_IMAGE_ID)), targetIndividual->playerCharacter->x, targetIndividual->playerCharacter->y);
		sendMissedDialog(thisIndividual->name,targetIndividual->name,d20,totalAC);
		return 0;
	}
}

void triggerAttackSound(char attackType){
	switch (attackType) {
	case 'b':
		triggerSoundEffect(SOUND_BLUNT_ID);
		break;
	case 'c':
		triggerSoundEffect(SOUND_CHOP_ID);
		break;
	case 's':
		triggerSoundEffect(SOUND_SLASH_ID);
		break;
	case 'p':
		triggerSoundEffect(SOUND_PIERCE_ID);
		break;
	case 'e':
		triggerSoundEffect(SOUND_SLASH_ID);
		break;
	case 'f':
		triggerSoundEffect(SOUND_SLASH_ID);
		break;
	case 'w':
		triggerSoundEffect(SOUND_SLASH_ID);
		break;
	case 'l':
		triggerSoundEffect(SOUND_SLASH_ID);
		break;
	}
}

int attackIndividualWithAbility(individual * thisIndividual, individual * target){
	ability * targetAbility = thisIndividual->activeAbilities->selectedAbility;
	int i, isTargeted = 0;

	enableSpecialDrawMode();
	setDurationInTimerTicks(20);

	if(targetAbility->targetedEnabled){
		isTargeted = targetAbility->targeted->effectAndManaArray[targetAbility->targeted->selectedIndex]->effectMagnitude;
	}

	if(isTargeted){
		addCharacterToSpecialDrawWithCoords(createCharacterFromAnimation(cloneAnimationFromRegistry(HIT_IMAGE_ID)), target->playerCharacter->x, target->playerCharacter->y);
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
			addCharacterToSpecialDrawWithCoords(createCharacterFromAnimation(cloneAnimationFromRegistry(HIT_IMAGE_ID)), target->playerCharacter->x, target->playerCharacter->y);
			return 	damageIndividualWithAbility(thisIndividual, target);
		}else{
			addCharacterToSpecialDrawWithCoords(createCharacterFromAnimation(cloneAnimationFromRegistry(MISS_IMAGE_ID)), target->playerCharacter->x, target->playerCharacter->y);
			sprintf(statOut, "%s's ability failed!", thisIndividual->name);
			cwrite(statOut);
			return 0;
		}

	}
}

void channelMana(individual * thisIndividual){
	int i;
	int manaRestore = 1;
	char logStr[128];
	sprintf(logStr, "%s is channeling:", thisIndividual->name);
	cwrite(logStr);

	for(i = 0; i < max(getAttributeSum(thisIndividual, "WILL") + 1,1); i++){
		manaRestore = max(manaRestore, (rand() % 4) + 1);
	}

	manaRestore = max(min(manaRestore + getAttributeSum(thisIndividual, "WILL"), 4) , 1);

	restoreMana(thisIndividual, manaRestore);

	sprintf(logStr, "Restored %d mana.", manaRestore);
	cwrite(logStr);
}

int abilityIsBuffing(ability * thisAbility){
	if(thisAbility->type == 'd'){
		if (thisAbility->rangeEnabled
				&& thisAbility->range->effectAndManaArray[thisAbility->range->selectedIndex]->effectMagnitude > 0) {
			return 1;
		}
	}

	if(thisAbility->type == 'i'){
		if (thisAbility->extraAttackEnabled
				&& thisAbility->extraAttack->effectAndManaArray[thisAbility->extraAttack->selectedIndex]->effectMagnitude > 0) {
			return 1;
		}
		if (thisAbility->attackEnabled
				&& thisAbility->attack->effectAndManaArray[thisAbility->attack->selectedIndex]->effectMagnitude > 0) {
			return 1;
		}
		if (thisAbility->damageModEnabled
				&& thisAbility->damageMod->effectAndManaArray[thisAbility->damageMod->selectedIndex]->effectMagnitude > 0) {
			return 1;
		}
	}

	if (thisAbility->acEnabled
			&& thisAbility->ac->effectAndManaArray[thisAbility->ac->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->attackEnabled
			&& thisAbility->attack->effectAndManaArray[thisAbility->attack->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->damageModEnabled
			&& thisAbility->damageMod->effectAndManaArray[thisAbility->damageMod->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->mvmtEnabled
			&& thisAbility->mvmt->effectAndManaArray[thisAbility->mvmt->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->baseHPEnabled
			&& thisAbility->baseHP->effectAndManaArray[thisAbility->baseHP->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->baseManaEnabled
			&& thisAbility->baseMana->effectAndManaArray[thisAbility->baseMana->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->bluntDREnabled
			&& thisAbility->bluntDR->effectAndManaArray[thisAbility->bluntDR->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->chopDREnabled
			&& thisAbility->chopDR->effectAndManaArray[thisAbility->chopDR->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->pierceDREnabled
			&& thisAbility->pierceDR->effectAndManaArray[thisAbility->pierceDR->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->slashDREnabled
			&& thisAbility->slashDR->effectAndManaArray[thisAbility->slashDR->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->earthDREnabled
			&& thisAbility->earthDR->effectAndManaArray[thisAbility->earthDR->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->fireDREnabled
			&& thisAbility->fireDR->effectAndManaArray[thisAbility->fireDR->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->waterDREnabled
			&& thisAbility->waterDR->effectAndManaArray[thisAbility->waterDR->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	if (thisAbility->lightningDREnabled
			&& thisAbility->lightningDR->effectAndManaArray[thisAbility->lightningDR->selectedIndex]->effectMagnitude > 0) {
		return 1;
	}

	return 0;
}

int abilityIsOffensive(ability * thisAbility){
	if(thisAbility->type == 'd' && abilityIsHarmful(thisAbility)){
		return 1;
	}

	if (thisAbility->type == 't') {
		if (thisAbility->diceDamageEnabled
				&& thisAbility->diceDamage->effectAndManaArray[thisAbility->diceDamage->selectedIndex]->effectMagnitude > 0) {
			return 1;
		}
		if (thisAbility->damageEnabled
				&& thisAbility->damage->effectAndManaArray[thisAbility->damage->selectedIndex]->effectMagnitude > 0) {
			return 1;
		}

		if (thisAbility->acEnabled
				&& thisAbility->ac->effectAndManaArray[thisAbility->ac->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->attackEnabled
				&& thisAbility->attack->effectAndManaArray[thisAbility->attack->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->damageModEnabled
				&& thisAbility->damageMod->effectAndManaArray[thisAbility->damageMod->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->mvmtEnabled
				&& thisAbility->mvmt->effectAndManaArray[thisAbility->mvmt->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->baseHPEnabled
				&& thisAbility->baseHP->effectAndManaArray[thisAbility->baseHP->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->baseManaEnabled
				&& thisAbility->baseMana->effectAndManaArray[thisAbility->baseMana->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->bluntDREnabled
				&& thisAbility->bluntDR->effectAndManaArray[thisAbility->bluntDR->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->chopDREnabled
				&& thisAbility->chopDR->effectAndManaArray[thisAbility->chopDR->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->pierceDREnabled
				&& thisAbility->pierceDR->effectAndManaArray[thisAbility->pierceDR->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->slashDREnabled
				&& thisAbility->slashDR->effectAndManaArray[thisAbility->slashDR->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->earthDREnabled
				&& thisAbility->earthDR->effectAndManaArray[thisAbility->earthDR->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->fireDREnabled
				&& thisAbility->fireDR->effectAndManaArray[thisAbility->fireDR->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->waterDREnabled
				&& thisAbility->waterDR->effectAndManaArray[thisAbility->waterDR->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if (thisAbility->lightningDREnabled
				&& thisAbility->lightningDR->effectAndManaArray[thisAbility->lightningDR->selectedIndex]->effectMagnitude < 0) {
			return 1;
		}

		if(abilityIsHarmful(thisAbility)){
			return 1;
		}
	}

	//Since 'i' abilities can only be used on the self, and not targeted at others, these are only buffs.
	//They can have the side affect of offensiveness, but that's no different than adding strength or dex, etc.
//	if(thisAbility->type == 'i'){
//		if (thisAbility->extraAttackEnabled
//				&& thisAbility->extraAttack->effectAndManaArray[thisAbility->extraAttack->selectedIndex]->effectMagnitude > 0) {
//			return 1;
//		}
//		if (thisAbility->attackEnabled
//				&& thisAbility->attack->effectAndManaArray[thisAbility->attack->selectedIndex]->effectMagnitude > 0) {
//			return 1;
//		}
//		if (thisAbility->damageModEnabled
//				&& thisAbility->damageMod->effectAndManaArray[thisAbility->damageMod->selectedIndex]->effectMagnitude > 0) {
//			return 1;
//		}
//	}

	return 0;
}

int abilityIsHarmful(ability * thisAbility){
	//Note: damage and diceDamage only affect the caster, and are not harmful to others in range.

	if(thisAbility->statusEnabled){
		return statusIsHarmful(thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type);
	}

	return 0;
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

int processCasterOnlyEffects(individual * thisIndividual, ability * thisAbility){
	if (thisAbility->type == 'd' || thisAbility->type == 'i') {
		int i, damage = 0, numDice = 0, totalDamage, targetDR;

		if(thisAbility->diceDamageMultiplierEnabled){
			numDice = thisAbility->diceDamageMultiplier->effectAndManaArray[thisAbility->diceDamageMultiplier->selectedIndex]->effectMagnitude;
		}

		if(thisAbility->diceDamageEnabled){
			int diceDamage = thisAbility->diceDamage->effectAndManaArray[thisAbility->diceDamage->selectedIndex]->effectMagnitude;

			for(i = 0; i < numDice; i++){
				if(diceDamage > 0){
					damage += (rand() % diceDamage) + 1;
				}
			}
		}

		if (thisAbility->damageEnabled) {
			damage += thisAbility->damage->effectAndManaArray[thisAbility->damage->selectedIndex]->effectMagnitude;
		}

		if(damage == 0){
			return 0;
		}

		targetDR = calcDR(thisIndividual, thisAbility->damageType->typeAndManaArray[thisAbility->damageType->selectedIndex]->type[0]);

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
	int i, damage = 0, numDice = 0, totalDamage, targetDR;
	ability * targetAbility = thisIndividual->activeAbilities->selectedAbility;

	triggerAttackSound(targetAbility->damageType->typeAndManaArray[targetAbility->damageType->selectedIndex]->type[0]);

	if(targetAbility->diceDamageMultiplierEnabled){
		numDice = targetAbility->diceDamageMultiplier->effectAndManaArray[targetAbility->diceDamageMultiplier->selectedIndex]->effectMagnitude;
	}

	if(targetAbility->diceDamageEnabled){
		int diceDamage = targetAbility->diceDamage->effectAndManaArray[targetAbility->diceDamage->selectedIndex]->effectMagnitude;

		for(i = 0; i < numDice; i++){
			if(diceDamage > 0){
				damage += (rand() % diceDamage) + 1;
			}
		}
	}

	if(targetAbility->damageEnabled){
		damage += targetAbility->damage->effectAndManaArray[targetAbility->damage->selectedIndex]->effectMagnitude;
	}

	targetDR = calcDR(targetIndividual, targetAbility->damageType->typeAndManaArray[targetAbility->damageType->selectedIndex]->type[0]);

	totalDamage = damage - targetDR;

	if(totalDamage < 1){
		totalDamage = 0;
	}

	if(totalDamage > 0){
		triggerEventOnHarm(targetIndividual->ID, thisIndividual->isPlayer);
	}

	sendHitDialog(thisIndividual->name, targetIndividual->name, 20, totalDamage);
	targetIndividual->hp = targetIndividual->hp - totalDamage;

	if(targetIndividual->hp <= 0){ //target is dead
		sendDeathDialog(targetIndividual->name, thisIndividual->name);
		triggerEventOnDeath(targetIndividual->ID, thisIndividual->isPlayer);

		if(thisIndividual->isPlayer && (targetIndividual->currentGroupType == GROUP_NPCS || targetIndividual->currentGroupType == GROUP_GUARDS)){
			processCrimeEvent(CRIME_MURDER, 300, targetIndividual->ID, 0);
		}

		removeIndividualFromExistance(targetIndividual->ID);
		addSpecialIndividual(targetIndividual);

		addSpecialIndividual(targetIndividual);
		int delay = thisIndividual->playerCharacter->thisAnimationContainer->animations[thisIndividual->playerCharacter->thisAnimationContainer->currentAnimation]->totalDuration;
		setIndividualDelayAnimation(targetIndividual, ANIMATION_DEATH, delay);
		int deathDelay = targetIndividual->playerCharacter->thisAnimationContainer->animations[targetIndividual->playerCharacter->thisAnimationContainer->nextAnimationAfterDelay]->totalDuration;
		increaseSpecialDrawDurationIfGreater(delay + deathDelay);
		return 1;
	}else{ //non-fatal blow
		return 0;
	}
}

int useDurationAbilityOnIndividual(individual * thisIndividual, ability * thisAbility, char * casterName){
	int duration = calcAbilityDuration(thisAbility);
	char * tmp[64];

	if(thisIndividual->activeAbilities->numAbilities + 1 < thisIndividual->activeAbilities->MAX_ABILITIES){
		if(strcmp(casterName, thisIndividual->name) == 0){
			sprintf(tmp, "%s used %s", thisIndividual->name, thisAbility->name);
		}
		else{
			sprintf(tmp, "%s used %s on %s", casterName, thisAbility->name, thisIndividual->name);
		}

		if(duration > 0){
			char * durationStr[32];
			sprintf(durationStr, " for %d turns.", duration);
			strcat(tmp, durationStr);
		}

		cwrite(tmp);

		addActiveAbilityToIndividual(thisIndividual, thisAbility, duration);
		useActiveAbility(thisIndividual, thisAbility);

		if(thisAbility->statusEnabled){
			statusEffect thisEffect = lookUpStatusType( thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type);

			if(thisEffect != STATUS_NONE){
				status * newStatus = createStatusFromAbility(thisAbility);

				tmp[0] = '\0';

				if(strcmp(casterName, thisIndividual->name) == 0){
					sprintf(tmp, "%s used %s for %d turns.", thisIndividual->name, thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type, newStatus->turnsRemaining);
				}
				else{
					sprintf(tmp, "%s used %s on %s for %d turns.", casterName, thisAbility->status->typeAndManaArray[thisAbility->status->selectedIndex]->type, thisIndividual->name, newStatus->turnsRemaining);
				}

				cwrite(tmp);

				addStatusToIndividual(thisIndividual, newStatus);
				processStatus(thisIndividual,newStatus);
				newStatus->turnsRemaining--;
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
	thisStatus->turnsRemaining = calcStatusDuration(thisAbility);

	return thisStatus;
}

int damageIndividual(individual *thisIndividual, individual *targetIndividual, int isCrit){
	int totalDamage = 0, i, totalDR, maxDamTotal, minDamTotal, baseDam;
	char attackType = 'b'; //for now, default is blunt (punching)

	item * weapon = getActiveWeapon(thisIndividual->backpack);
	attackType = getIndividualAttackType(thisIndividual, weapon);

//	triggerAttackSound(attackType);

	baseDam = getAttributeSum(thisIndividual,"baseDam"); //doesn't include STR mod

	baseDam += calcStrengthDamageMod(thisIndividual, weapon);

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
		if(maxDamTotal == minDamTotal){
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
		triggerEventOnHarm(targetIndividual->ID, thisIndividual->isPlayer);
	}

	sendHitDialog(thisIndividual->name, targetIndividual->name, thisIndividual->maxDam, totalDamage);
	targetIndividual->hp = targetIndividual->hp - totalDamage;

	if(targetIndividual->hp <= 0){ //target is dead
		sendDeathDialog(targetIndividual->name, thisIndividual->name);
		triggerEventOnDeath(targetIndividual->ID, thisIndividual->isPlayer);

		if(targetIndividual->currentGroupType == GROUP_NPCS || targetIndividual->currentGroupType == GROUP_GUARDS){
			processCrimeEvent(CRIME_MURDER, 300, targetIndividual->ID, 0);
		}

		removeIndividualFromExistance(targetIndividual->ID);
		addSpecialIndividual(targetIndividual);
		int delay = thisIndividual->playerCharacter->thisAnimationContainer->animations[thisIndividual->playerCharacter->thisAnimationContainer->currentAnimation]->totalDuration;
		setIndividualDelayAnimation(targetIndividual, ANIMATION_DEATH, delay);
		int deathDelay = targetIndividual->playerCharacter->thisAnimationContainer->animations[targetIndividual->playerCharacter->thisAnimationContainer->nextAnimationAfterDelay]->totalDuration;
		increaseSpecialDrawDurationIfGreater(delay + deathDelay);
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

				destroyItem(tmpActiveItem->thisItem);
				free(tmpActiveItem);
				thisIndividual->activeItems->activeItemArr[i] = NULL;
				thisIndividual->activeItems->activeItemsTotal--;
			}else{
				consumeDurationItem(thisIndividual,tmpActiveItem->thisItem);
				if(thisIndividual->hp <= 0){
					char * tmp[128];
					sprintf(tmp, "%s has perished from %s!", thisIndividual->name, tmpActiveItem->thisItem->name);
					cwrite(tmp);
					removeIndividualFromExistance(thisIndividual->ID);
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
						removeIndividualFromExistance(thisIndividual->ID);
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
					removeIndividualFromExistance(thisIndividual->ID);

					enableSpecialDrawMode();
					addSpecialIndividual(thisIndividual);
					int delay = thisIndividual->playerCharacter->thisAnimationContainer->animations[thisIndividual->playerCharacter->thisAnimationContainer->currentAnimation]->totalDuration;
					setIndividualDelayAnimation(thisIndividual, ANIMATION_DEATH, delay);
					int deathDelay = thisIndividual->playerCharacter->thisAnimationContainer->animations[thisIndividual->playerCharacter->thisAnimationContainer->nextAnimationAfterDelay]->totalDuration;
					increaseSpecialDrawDurationIfGreater(delay + deathDelay);
					return 1;
				}
			}else{
				char * tmp[64];
				sprintf(tmp, "%s recovered from %s.", thisIndividual->name, lookUpStatusEffectName(tmpStatus->effect, 0));
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
			break;
		}
		case STATUS_PARALYZED:{
			thisIndividual->remainingActions = 0;
			break;
		}
		case STATUS_SLEEPING:{
			if(thisStatus->turnsRemaining == 0 && (rand() % 3) != 0){
				thisStatus->turnsRemaining++;
			}

			thisIndividual->remainingActions = 0;

			break;
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
	}else if(strcmp(statusType, "Sleep") == 0){
		return STATUS_SLEEPING;
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

char * lookUpStatusEffectName(statusEffect effect, int uppercase){
	switch(effect){
	case(STATUS_POISONED):
		if(uppercase){
			return "Poison";
		}
		return "poison";
	case(STATUS_PARALYZED):
		if(uppercase){
			return "Paralysis";
		}
		return "paralysis";
	case(STATUS_CONFUSED):
		if(uppercase){
			return "Confusion";
		}
		return "confusion";
	case(STATUS_BURNING):
		if(uppercase){
			return "Burning";
		}
		return "burning";
	case(STATUS_BLEEDING):
		if(uppercase){
			return "Bleeding";
		}
		return "bleeding";
	case(STATUS_BERZERK):
		if(uppercase){
			return "Berzerk";
		}
		return "berzerk";
	case(STATUS_SILENCED):
		if(uppercase){
			return "Silence";
		}
		return "silence";
	case(STATUS_SLEEPING):
		if(uppercase){
			return "Sleep";
		}
		return "sleep";
	}

	return "!! STATUS NOT FOUND !!";
}

int hasActiveStatusEffect(individual * thisIndividual, statusEffect effect){
	int i, statusesPassed = 0;

	if(thisIndividual->activeStatuses->numStatuses > 0){
		for(i = 0; i < thisIndividual->activeStatuses->MAX_STATUSES; i++){
			status * tmpStatus = thisIndividual->activeStatuses->statuses[i];

			if(tmpStatus != NULL){
				if(tmpStatus->effect == effect){
					return 1;
				}

				if(statusesPassed == thisIndividual->activeStatuses->numStatuses){
					break;
				}
			}
		}
	}

	return 0;
}

void disableSleepStatusOnAttack(individual * attackedIndividual){
	int i, statusesPassed = 0;

	if(attackedIndividual->activeStatuses->numStatuses > 0){
		for(i = 0; i < attackedIndividual->activeStatuses->MAX_STATUSES; i++){
			status * tmpStatus = attackedIndividual->activeStatuses->statuses[i];

			if(tmpStatus != NULL){
				if(tmpStatus->effect == STATUS_SLEEPING){
					attackedIndividual->activeStatuses->statuses[i] = NULL;
					attackedIndividual->activeStatuses->numStatuses--;
					statusesPassed--;
				}

				if(statusesPassed == attackedIndividual->activeStatuses->numStatuses){
					break;
				}
			}
		}
	}
}

void useActiveAbility(individual * thisIndividual, ability * thisAbility){
	int amountHealed = 0;
	int food = 0;

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

	if(thisAbility->foodEnabled){
		food += thisAbility->food->effectAndManaArray[thisAbility->food->selectedIndex]->effectMagnitude;

		restoreFood(thisIndividual, food);

		if(food > 0){
			char * tmp[64];
			sprintf(tmp, "%s restored %d food from %s", thisIndividual->name, food, thisAbility->name);
			cwrite(tmp);
		}
	}
}


void endTurn(individual *thisIndividual){
	printf("player turn ended\n");
	thisIndividual->hasAttacked = 0;
	thisIndividual->remainingActions += thisIndividual->totalActions;
}

int decreaseFood(individual * thisIndividual, double food){
	if((int) thisIndividual->food <= 0){
		return 1;
	}

	thisIndividual->food -= food;

	if(thisIndividual->food <= 300 && thisIndividual->foodBonusActive){
		thisIndividual->foodBonusActive = 0;
	}

	if(thisIndividual->food <= 50 && !thisIndividual->foodNegativeActive){
		thisIndividual->foodNegativeActive = 1;
	}

	//Individual has died of starvation.
	if(thisIndividual->food <= 0){
		thisIndividual->food = 0.0;
		thisIndividual->hp = 0;

		char * starvationStr[64];
		sprintf(starvationStr, "%s has starved to death.", thisIndividual->name);
		cwrite(starvationStr);

		triggerEventOnDeath(thisIndividual->ID, thisIndividual->isPlayer);

		removeIndividualFromExistance(thisIndividual->ID);

		enableSpecialDrawMode();
		addSpecialIndividual(thisIndividual);
		int delay = thisIndividual->playerCharacter->thisAnimationContainer->animations[thisIndividual->playerCharacter->thisAnimationContainer->currentAnimation]->totalDuration;
		setIndividualDelayAnimation(thisIndividual, ANIMATION_DEATH, delay);
		int deathDelay = thisIndividual->playerCharacter->thisAnimationContainer->animations[thisIndividual->playerCharacter->thisAnimationContainer->nextAnimationAfterDelay]->totalDuration;
		increaseSpecialDrawDurationIfGreater(delay + deathDelay);
		return 1;
	}

	return 0;
}

int targetInRangeOfCord(individual * target, int range, int x, int y){
	if(abs(target->playerCharacter->x - x) <= range
	&& abs(target->playerCharacter->y - y) <= range){
		return 1;
	}
	return 0;
}

int cordWithinRange(individual * thisIndividual, int x, int y){
	int range = getAttributeSum(thisIndividual, "range");

	if(abs(thisIndividual->playerCharacter->x - x) <= range
	&& abs(thisIndividual->playerCharacter->y - y) <= range){
		return 1;
	}
	return 0;
}

int individualWithinTalkingRange(individual * thisIndividual, individual * target, int talkingRange){
	if(abs(thisIndividual->playerCharacter->x - target->playerCharacter->x) <= talkingRange
	&& abs(thisIndividual->playerCharacter->y - target->playerCharacter->y) <= talkingRange){
		return 1;
	}
	return 0;
}

int calcExtraTimesToAttack(individual * thisIndividual){
	if(thisIndividual->activeAbilities->selectedAbility != NULL && thisIndividual->activeAbilities->selectedAbility->type == 'i' ){

		if(thisIndividual->activeAbilities->selectedAbility->extraAttackEnabled){
			return thisIndividual->activeAbilities->selectedAbility->extraAttack->effectAndManaArray[thisIndividual->activeAbilities->selectedAbility->extraAttack->selectedIndex]->effectMagnitude;
		}

	}

	return 0;
}

char getIndividualAttackType(individual * thisIndividual, item * weapon){
	if(weapon != NULL){
		return weapon->weaponDamageType;
	}else{
		return 'b'; //TODO: individuals have default attack type (claws, fists, etc);
	}
}

int calcStrengthDamageMod(individual * thisIndividual, item * weapon){
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

setIndividualAnimation(individual * thisIndividual, animationState state){
	setAnimation(thisIndividual->playerCharacter->thisAnimationContainer, state);

	if(thisIndividual->armorItem != NULL){
		setAnimation(thisIndividual->armorItem->thisAnimationContainer, state);
	}

	if(thisIndividual->shieldItem != NULL){
		setAnimation(thisIndividual->shieldItem->thisAnimationContainer, state);
	}

	if(thisIndividual->weaponItem != NULL){
		setAnimation(thisIndividual->weaponItem->thisAnimationContainer, state);
	}
}

setIndividualDelayAnimation(individual * thisIndividual, animationState state, int delay){
	setDelayAnimation(thisIndividual->playerCharacter->thisAnimationContainer, state, delay);

	if(thisIndividual->armorItem != NULL){
		setDelayAnimation(thisIndividual->armorItem->thisAnimationContainer, state);
	}

	if(thisIndividual->shieldItem != NULL){
		setDelayAnimation(thisIndividual->shieldItem->thisAnimationContainer, state);
	}

	if(thisIndividual->weaponItem != NULL){
		setDelayAnimation(thisIndividual->weaponItem->thisAnimationContainer, state);
	}
}

int animationDelayUpdate(character * thisCharacter){
	if(thisCharacter->thisAnimationContainer->clockTickDelay > 0){
		if(!isPaused()){
			thisCharacter->thisAnimationContainer->clockTickDelay--;
		}

		if(thisCharacter->thisAnimationContainer->clockTickDelay == 0){
			thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->currentFrame = 0;
			thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->nextAnimationAfterDelay]->currentFrame = 0;

			thisCharacter->thisAnimationContainer->currentAnimation = thisCharacter->thisAnimationContainer->nextAnimationAfterDelay;
			thisCharacter->thisAnimationContainer->clockTickCount = 0;
			return 1;
		}
	}

	return 0;
}

int animationFrameTickUpdate(character * thisCharacter){
	int toReturn = 0;
	animation * thisAnimationSet = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation];

	if(thisAnimationSet->durationInTicks[thisAnimationSet->currentFrame] < thisCharacter->thisAnimationContainer->clockTickCount){
	  if(thisAnimationSet->currentFrame + 1 == thisAnimationSet->numFrames){
		thisAnimationSet->currentFrame = 0;
		thisCharacter->thisAnimationContainer->currentAnimation = thisCharacter->thisAnimationContainer->defaultAnimation; // Go back to default animation
		toReturn = 1;
	  }else{
		thisAnimationSet->currentFrame++;

		if(thisAnimationSet->soundFrame == thisAnimationSet->currentFrame){
			triggerSoundEffect(thisAnimationSet->soundID);
		}

	  }
	  thisCharacter->thisAnimationContainer->clockTickCount = 0;
	}

	return toReturn;
}

void returnEquiptItemsToIdle(individual * thisIndividual){
	if(thisIndividual->armorItem != NULL && thisIndividual->armorItem->thisAnimationContainer->animationsEnabled){
		thisIndividual->armorItem->thisAnimationContainer->currentAnimation = thisIndividual->armorItem->thisAnimationContainer->defaultAnimation;
	}

	if(thisIndividual->shieldItem != NULL && thisIndividual->shieldItem->thisAnimationContainer->animationsEnabled){
		thisIndividual->shieldItem->thisAnimationContainer->currentAnimation = thisIndividual->shieldItem->thisAnimationContainer->defaultAnimation;
	}

	if(thisIndividual->weaponItem != NULL && thisIndividual->weaponItem->thisAnimationContainer->animationsEnabled){
		thisIndividual->weaponItem->thisAnimationContainer->currentAnimation = thisIndividual->weaponItem->thisAnimationContainer->defaultAnimation;
	}
}

void updateEquiptItemsToDelayedAnimationState(individual * thisIndividual){
	if(thisIndividual->armorItem != NULL && thisIndividual->armorItem->thisAnimationContainer->animationsEnabled){
		thisIndividual->armorItem->thisAnimationContainer->currentAnimation = thisIndividual->armorItem->thisAnimationContainer->nextAnimationAfterDelay;
	}

	if(thisIndividual->shieldItem != NULL && thisIndividual->shieldItem->thisAnimationContainer->animationsEnabled){
		thisIndividual->shieldItem->thisAnimationContainer->currentAnimation = thisIndividual->shieldItem->thisAnimationContainer->nextAnimationAfterDelay;
	}

	if(thisIndividual->weaponItem != NULL && thisIndividual->weaponItem->thisAnimationContainer->animationsEnabled){
		thisIndividual->weaponItem->thisAnimationContainer->currentAnimation = thisIndividual->weaponItem->thisAnimationContainer->nextAnimationAfterDelay;
	}
}

void drawDefaultEquiptItemsByPixel(HDC hdc, HDC hdcBuffer, individual* thisIndividual, int x, int y){
	if(thisIndividual->armorItem != NULL && thisIndividual->armorItem->thisAnimationContainer->animationsEnabled){
		drawLayerFromBaseAnimationByPixel(hdc, hdcBuffer, thisIndividual->armorItem, thisIndividual->playerCharacter->thisAnimationContainer, x, y);
	}

	if(thisIndividual->shieldItem != NULL && thisIndividual->shieldItem->thisAnimationContainer->animationsEnabled){
		drawLayerFromBaseAnimationByPixel(hdc, hdcBuffer, thisIndividual->shieldItem, thisIndividual->playerCharacter->thisAnimationContainer, x, y);
	}

	if(thisIndividual->weaponItem != NULL && thisIndividual->weaponItem->thisAnimationContainer->animationsEnabled){
		drawLayerFromBaseAnimationByPixel(hdc, hdcBuffer, thisIndividual->weaponItem, thisIndividual->playerCharacter->thisAnimationContainer, x, y);
	}
}

void drawEquiptItems(HDC hdc, HDC hdcBuffer, individual* thisIndividual,  shiftData * viewShift){
	int x = thisIndividual->playerCharacter->x;
	int y = thisIndividual->playerCharacter->y;
	double xOffset = (double)thisIndividual->playerCharacter->xOff;
	double yOffset = (double)thisIndividual->playerCharacter->yOff;

	if(thisIndividual->armorItem != NULL && thisIndividual->armorItem->thisAnimationContainer->animationsEnabled){
		drawLayerFromBaseAnimation(hdc, hdcBuffer, thisIndividual->armorItem, thisIndividual->playerCharacter->thisAnimationContainer, x, y, xOffset, yOffset, viewShift);
	}

	if(thisIndividual->shieldItem != NULL && thisIndividual->shieldItem->thisAnimationContainer->animationsEnabled){
		drawLayerFromBaseAnimation(hdc, hdcBuffer, thisIndividual->shieldItem, thisIndividual->playerCharacter->thisAnimationContainer, x, y, xOffset, yOffset, viewShift);
	}

	if(thisIndividual->weaponItem != NULL && thisIndividual->weaponItem->thisAnimationContainer->animationsEnabled){
		drawLayerFromBaseAnimation(hdc, hdcBuffer, thisIndividual->weaponItem, thisIndividual->playerCharacter->thisAnimationContainer, x, y, xOffset, yOffset, viewShift);
	}
}

void drawIndividualDefaultByPixels(HDC hdc, HDC hdcBuffer, individual* thisIndividual, int x, int y, int useSecondary){
	if(thisIndividual == NULL){
		return;
	}

	drawUnboundAnimationByPixels(hdc, hdcBuffer, thisIndividual->playerCharacter, x, y, 0);

	drawDefaultEquiptItemsByPixel(hdc, hdcBuffer, thisIndividual, x, y);
}

void drawIndividual(HDC hdc, HDC hdcBuffer, individual* thisIndividual, shiftData * viewShift){
	if(thisIndividual == NULL){
		return;
	}

	if(!isPaused()){
		thisIndividual->playerCharacter->thisAnimationContainer->clockTickCount++;
	}


	if(animationDelayUpdate(thisIndividual->playerCharacter)){
		updateEquiptItemsToDelayedAnimationState(thisIndividual);
	}

	if(animationFrameTickUpdate(thisIndividual->playerCharacter)){
		returnEquiptItemsToIdle(thisIndividual);
	}

	drawCharacterAnimation(hdc, hdcBuffer, thisIndividual->playerCharacter, viewShift, 0);

	drawEquiptItems(hdc, hdcBuffer, thisIndividual, viewShift);
}

void drawLayerFromBaseAnimationByPixel(HDC hdc, HDC hdcBuffer, character * layer, animationContainer * baseAnimation, int x, int y){
	HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP image, imageMask;

		int shitfX;
		shitfX = baseAnimation->animations[baseAnimation->currentAnimation]->currentFrame*100;

		image = layer->thisAnimationContainer->animations[layer->thisAnimationContainer->currentAnimation]->image;
		imageMask = layer->thisAnimationContainer->animations[layer->thisAnimationContainer->currentAnimation]->imageMask;

		SelectObject(hdcMem, imageMask);

		BitBlt(hdcBuffer, x, y,
				100,100,
				hdcMem,
				shitfX,
				0,
				SRCAND);

		SelectObject(hdcMem, image);

		BitBlt(hdcBuffer, x,y,
				100,100,
				hdcMem,
				shitfX,
				0,
				SRCPAINT);

		DeleteDC(hdcMem);
}


item * removeItemFromInventory(inventory * backpack, item * thisItem){
	int i, j;
	item * removedItem;

	for(i = 0; i < backpack->MAX_ITEMS; i++){
		if(backpack->inventoryArr[i] == thisItem){
			removedItem = backpack->inventoryArr[i];
			removedItem->isEquipt = 0;

			//rebalance inventory
			for(j = i; j < backpack->inventorySize - 1; j++){
				backpack->inventoryArr[j] = backpack->inventoryArr[j+1];
			}

			backpack->inventorySize--;
			backpack->inventoryArr[backpack->inventorySize] = NULL;

			return removedItem;
		}
	}

	return NULL;
}

void modifyItem(item * theItem, individual * thisIndividual) {
	char type = theItem->type;
	switch (type) {
		case 'w': {
			if(theItem->isEquipt){
				theItem->isEquipt = 0;
				thisIndividual->weaponItem = NULL;
			}else{
				if(tryEquipItem(thisIndividual->backpack, theItem)){
//					setDefaultAnimation(theItem->itemCharacter->thisAnimationContainer, ANIMATION_IDLE_EQUIPT);
					thisIndividual->weaponItem = theItem->itemCharacter;
					setAnimation(thisIndividual->weaponItem->thisAnimationContainer, ANIMATION_IDLE_EQUIPT);
					setDefaultAnimation(thisIndividual->weaponItem->thisAnimationContainer, ANIMATION_IDLE_EQUIPT);
				}
			}
		}
		break;
		case 'a': {
			if(theItem->isEquipt){
				theItem->isEquipt = 0;
				thisIndividual->armorItem = NULL;
			}else{
				if(tryEquipItem(thisIndividual->backpack, theItem)){
//					setDefaultAnimation(theItem->itemCharacter->thisAnimationContainer, ANIMATION_IDLE_EQUIPT);
					thisIndividual->armorItem = theItem->itemCharacter;
					setAnimation(thisIndividual->armorItem->thisAnimationContainer, ANIMATION_IDLE_EQUIPT);
					setDefaultAnimation(thisIndividual->armorItem->thisAnimationContainer, ANIMATION_IDLE_EQUIPT);
				}
			}
		}
		break;
		case 's': {
			if(theItem->isEquipt){
				theItem->isEquipt = 0;
				thisIndividual->shieldItem = NULL;
			}else{
				if(tryEquipItem(thisIndividual->backpack, theItem)){
//					setDefaultAnimation(theItem->itemCharacter->thisAnimationContainer, ANIMATION_IDLE_EQUIPT);
					thisIndividual->shieldItem = theItem->itemCharacter;
					setAnimation(thisIndividual->shieldItem->thisAnimationContainer, ANIMATION_IDLE_EQUIPT);
					setDefaultAnimation(thisIndividual->shieldItem->thisAnimationContainer, ANIMATION_IDLE_EQUIPT);
				}
			}
		}
		break;
		case 'i': {
			if(theItem->itemType == 'c'){
				consumeItem(thisIndividual, theItem);
				deleteItemFromRegistry(theItem->ID);
				destroyItem(removeItemFromInventory(thisIndividual->backpack, theItem));
			}else if(theItem->itemType == 'd'){
				if(thisIndividual->activeItems->activeItemsTotal < 40){
					consumeItem(thisIndividual, theItem);
					addItemToActiveItemList(thisIndividual, theItem);
					deleteItemFromRegistry(theItem->ID);
					removeItemFromInventory(thisIndividual->backpack, theItem);
				}
			}
		}
	}
}

int tryEquipItem(inventory * backpack, item * thisItem){
	int i;
	item * tmpItem;

	for(i = 0; i < backpack->MAX_ITEMS; i++){
		tmpItem = backpack->inventoryArr[i];
		if(tmpItem != NULL && tmpItem != thisItem && tmpItem->type == thisItem->type && tmpItem->isEquipt){
			return 0;
		}
	}

	thisItem->isEquipt = 1;
	return 1;
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

int addItemToInventory(inventory * backpack, item * newItem){
	int i, availableSpot;
	if(backpack->inventorySize < backpack->MAX_ITEMS){
		for(i = 0; i < backpack->MAX_ITEMS; i++){
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

	if(theItem->food != 0){
		if(theItem->food > 0){
			restoreFood(thisIndividual, theItem->food);
		}
	}
}

void consumeDurationItem(individual * thisIndividual, item * theItem){
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

void restoreFood(individual * thisIndividual, int food){
	if(thisIndividual->food + food >= getAttributeSum(thisIndividual, "baseFood")){
		thisIndividual->food = getAttributeSum(thisIndividual, "baseFood");
	}else{
		thisIndividual->food += food;
	}

	if(thisIndividual->food > 300 && !thisIndividual->foodBonusActive){
		thisIndividual->foodBonusActive = 1;
	}

	if(thisIndividual->food > 50 && thisIndividual->foodNegativeActive){
		thisIndividual->foodNegativeActive = 0;
	}
}

int getTotalMana(individual * thisIndividual){
	return getAttributeSum(thisIndividual, "baseMana") + 2 * getAttributeSum(thisIndividual, "WILL");
}

int attemptToBuyItem(item * thisItem, individual * thisIndividual){
	if(thisItem->price <= thisIndividual->gold){
		item * newItem = cloneItem(thisItem, 0);

		if(newItem == NULL){
			char * errLog[128];
			sprintf(errLog, "!! ITEM %d COULD NOT BE CLONED!!", thisItem->ID);
			return 0;
		}

		addItemToRegistry(newItem);

		addItemToInventory(thisIndividual->backpack, newItem);
		newItem->npcID = thisIndividual->ID;

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

		if(processCasterOnlyEffects(thisIndividual, thisAbility)){
			char * tmp[128];
			sprintf(tmp, "%s perished from %s!", thisIndividual->name, thisAbility->name);
			cwrite(tmp);

			enableSpecialDrawMode();
			addSpecialIndividual(thisIndividual);
			int delay = thisIndividual->playerCharacter->thisAnimationContainer->animations[thisIndividual->playerCharacter->thisAnimationContainer->currentAnimation]->totalDuration;
			setIndividualDelayAnimation(thisIndividual, ANIMATION_DEATH, delay);
			int deathDelay = thisIndividual->playerCharacter->thisAnimationContainer->animations[thisIndividual->playerCharacter->thisAnimationContainer->nextAnimationAfterDelay]->totalDuration;
			increaseSpecialDrawDurationIfGreater(delay + deathDelay);
			return 1;
		}
	}

	return 0;
}

void decreaseMana(individual * thisIndividual, int mana){
	thisIndividual->mana -= mana;
}

int canUseAnyAbilities(individual * thisIndividual){
	int i;

	for(i = 0; i < thisIndividual->abilities->numAbilities; i++){
		if(thisIndividual->abilities->abilitiesList[i] != NULL && thisIndividual->abilities->abilitiesList[i]->type != 'p' && thisIndividual->mana - thisIndividual->abilities->abilitiesList[i]->totalManaCost >= 0 ){
			return 1;
		}
	}

	return 0;
}

int canUseAbility(individual * thisIndividual, ability * thisAbility){
	if(thisAbility->type != 'p' && thisIndividual->mana - thisAbility->totalManaCost >= 0 && thisIndividual->activeAbilities->selectedAbility == NULL){
		return 1;
	}else{
		return 0;
	}
}

cordArr* initCordArr(){
	cordArr * thisCordArr = malloc(sizeof(cordArr));
	thisCordArr->numCords = 0;
	thisCordArr->MAX_CORDS = 300;

	return thisCordArr;
}

cord * makeCord(int x, int y){
	cord * thisCord = malloc(sizeof(cord));
	thisCord->x = x;
	thisCord->y = y;

	return thisCord;
}

void destroyCordArr(cordArr * thisCordArr){
	int i;

	if(thisCordArr == NULL){
		return;
	}

	for(i = 0; i < thisCordArr->numCords; i++){
		free(thisCordArr->cords[i]);
	}

	free(thisCordArr);
}

double calcSlope(int x1, int y1, int x2, int y2){
	double slope;

	slope = (((y2 * 0.1) - (y1 * 0.1)) / ((x2 * 0.1) - (x1 * 0.1)));

	return slope;
}

int containsCordXY(cordArr * thisCordArr, int x, int y){
	int i;

	for(i = 0; i < thisCordArr->numCords; i++){
		if(thisCordArr->cords[i]->x == x && thisCordArr->cords[i]->y == y){
			return 1;
		}
	}

	return 0;
}

int containsCord(cordArr * thisCordArr, cord * thisCord){
	int i;

	for(i = 0; i < thisCordArr->numCords; i++){
		if(thisCordArr->cords[i]->x == thisCord->x && thisCordArr->cords[i]->y == thisCord->y){
			return 1;
		}
	}

	return 0;
}

int addNewCordIfUnique(cordArr * thisCordArr, int x, int y){
	if(thisCordArr->numCords < thisCordArr->MAX_CORDS && !containsCordXY(thisCordArr, x, y)){
		cord * newCord = malloc(sizeof(cord));
		newCord->x = x;
		newCord->y = y;

		thisCordArr->cords[thisCordArr->numCords] = newCord;
		thisCordArr->numCords++;
		return 1;
	}

	return 0;
}

int addCordIfUnique(cordArr * thisCordArr, cord * thisCord){
	if(thisCordArr->numCords < thisCordArr->MAX_CORDS && !containsCord(thisCordArr, thisCord)){
		thisCordArr->cords[thisCordArr->numCords] = thisCord;
		thisCordArr->numCords++;
		return 1;
	}

	return 0;
}

/*Taking into account that the Y access grows as it goes down (essentially flipping the access)
 * The abs() call is necessary because when calculating the slope the Y values are negated
 * (this allows the slope to be computed in the bottom-right quadrant of the Cartesian system)
 */
int isAboveOnField(int baseY, int aboveY){
	return abs(baseY) < abs(aboveY);
}

int getRetreatDirection(int thisX, int thisY, int targetX, int targetY){
	double slope;

	targetY = targetY*-1;
	thisY = thisY*-1;

	if(thisX == targetX && thisY == targetY){
			return 0;
	}else if(thisX == targetX){ // n/0; target is either above or below thisIndividual
		if(isAboveOnField(thisY, targetY)){ // target is above thisIndividual
			return 8;
		}else {
			return 2;
		}
	}else{
		slope = calcSlope(thisX, thisY, targetX, targetY);

		if(slope > 2.0 || slope < -2.0){
			if(isAboveOnField(thisY, targetY)){
				return 8;
			}else {
				return 2;
			}
		}

		if(slope > 0.5){
			if(isAboveOnField(thisY, targetY)){
				return 9;
			}else {
				return 1;
			}
		}

		if(slope > -0.5){
			if(thisX < targetX){
				return 4;
			}else {
				return 6;
			}
		}

		if(slope > -2){
			if(isAboveOnField(thisY, targetY)){
				return 7;
			}else {
				return 3;
			}
		}
	}

	return 0;
}

int * getOrderedRetreatDirections(int rootDirection){
	int * directionArray;

	if(rootDirection == 0){
		return NULL;
	}

	directionArray = malloc(5*sizeof(int));

	directionArray[0] = rootDirection;

	switch (rootDirection){
		case 1:
			if(rand()%2 == 0){
				directionArray[1] = 2;
				directionArray[2] = 4;
			}else{
				directionArray[1] = 4;
				directionArray[2] = 2;
			}
			if(rand()%2 == 0){
				directionArray[3] = 3;
				directionArray[4] = 7;
			}else{
				directionArray[3] = 7;
				directionArray[4] = 3;
			}

			return directionArray;
		case 2:
			if(rand()%2 == 0){
				directionArray[1] = 1;
				directionArray[2] = 3;
			}else{
				directionArray[1] = 3;
				directionArray[2] = 1;
			}
			if(rand()%2 == 0){
				directionArray[3] = 4;
				directionArray[4] = 6;
			}else{
				directionArray[3] = 6;
				directionArray[4] = 4;
			}

			return directionArray;
		case 3:
			if(rand()%2 == 0){
				directionArray[1] = 2;
				directionArray[2] = 6;
			}else{
				directionArray[1] = 6;
				directionArray[2] = 2;
			}
			if(rand()%2 == 0){
				directionArray[3] = 1;
				directionArray[4] = 9;
			}else{
				directionArray[3] = 9;
				directionArray[4] = 1;
			}

			return directionArray;
		case 4:
			if(rand()%2 == 0){
				directionArray[1] = 1;
				directionArray[2] = 7;
			}else{
				directionArray[1] = 7;
				directionArray[2] = 1;
			}
			if(rand()%2 == 0){
				directionArray[3] = 2;
				directionArray[4] = 8;
			}else{
				directionArray[3] = 8;
				directionArray[4] = 2;
			}

			return directionArray;
		case 6:
			if(rand()%2 == 0){
				directionArray[1] = 3;
				directionArray[2] = 9;
			}else{
				directionArray[1] = 9;
				directionArray[2] = 3;
			}
			if(rand()%2 == 0){
				directionArray[3] = 2;
				directionArray[4] = 8;
			}else{
				directionArray[3] = 8;
				directionArray[4] = 2;
			}

			return directionArray;
		case 7:
			if(rand()%2 == 0){
				directionArray[1] = 4;
				directionArray[2] = 8;
			}else{
				directionArray[1] = 8;
				directionArray[2] = 4;
			}
			if(rand()%2 == 0){
				directionArray[3] = 1;
				directionArray[4] = 9;
			}else{
				directionArray[3] = 9;
				directionArray[4] = 1;
			}

			return directionArray;
		case 8:
			if(rand()%2 == 0){
				directionArray[1] = 7;
				directionArray[2] = 9;
			}else{
				directionArray[1] = 9;
				directionArray[2] = 7;
			}
			if(rand()%2 == 0){
				directionArray[3] = 4;
				directionArray[4] = 6;
			}else{
				directionArray[3] = 6;
				directionArray[4] = 4;
			}

			return directionArray;
		case 9:
			if(rand()%2 == 0){
				directionArray[1] = 6;
				directionArray[2] = 8;
			}else{
				directionArray[1] = 8;
				directionArray[2] = 6;
			}
			if(rand()%2 == 0){
				directionArray[3] = 3;
				directionArray[4] = 7;
			}else{
				directionArray[3] = 7;
				directionArray[4] = 3;
			}

			return directionArray;
	}
}

cordArr * cordsBetweenTwoIndividuals(individual * thisIndividual, individual * targetIndividual, int maxDistance){
	int startingX = 0, startingY = 0, endingX, endingY;
	double slope;
	int wrtY = 1, startingGreaterThanEnding = 0, foundTargetSpace = 0;

	endingX = targetIndividual->playerCharacter->x - thisIndividual->playerCharacter->x;
	endingY = targetIndividual->playerCharacter->y - thisIndividual->playerCharacter->y;

	cordArr * thisCordArr = initCordArr();

	if(startingX == endingX && startingY == endingY){
		return NULL;
	}else if(startingX == endingX){ // n/0
		slope = 0;
		wrtY = 0;
	}else{
		slope = calcSlope(startingX, startingY, endingX, endingY);

		if(fabs(slope) > 1){ // calc with respect to
			slope = calcSlope(startingY, startingX, endingY, endingX);
			wrtY = 0;
		}
	}

	if(wrtY){
		if(startingX > endingX){
			startingGreaterThanEnding = 1;
		}
	}else{
		if(startingY > endingY){
			startingGreaterThanEnding = 1;
		}
	}

	double i, step = 0.25;

	if(startingGreaterThanEnding){
		step = step * -1;
	}

	for(i = 0; abs(i) < maxDistance; i+= step){
		cord * nextCord = malloc(sizeof(cord));

		if(wrtY){
			nextCord->x = floor(i + startingX + 0.5);
			nextCord->y = floor(slope*i + startingY + 0.5);
		}else{
			nextCord->x = floor(slope*i + startingX + 0.5);
			nextCord->y = floor(i + startingY + 0.5);
		}

		if(!addCordIfUnique(thisCordArr, nextCord)){
			free(nextCord);
		}else if(nextCord->x == endingX && nextCord->y == endingY){
			foundTargetSpace = 1;
			break;
		}
	}

	if(!foundTargetSpace){
		destroyCordArr(thisCordArr);
		return NULL;
	}

	return thisCordArr;
}

void addActiveCrimeFromEntry(individual * player, activeCrimeEntry * thisEntry){
	int i;

	if(player->thisActiveCrimes->numActiveCrimes == player->thisActiveCrimes->MAX_ACTIVE_CRIMES){
		cwrite("!! TOO MANY ACTIVE CRIMES !!");
		free(thisEntry);
		return;
	}

	for(i = 0; i < player->thisActiveCrimes->MAX_ACTIVE_CRIMES; i++){
		if(player->thisActiveCrimes->activeCrimeList[i] == NULL){
			player->thisActiveCrimes->activeCrimeList[i] = thisEntry;
			player->thisActiveCrimes->numActiveCrimes++;
			return;
		}
	}

	cwrite("!! TOO MANY ACTIVE CRIMES !!");
	free(thisEntry);
}

void addReportedCrimeFromEntry(individual * player, activeCrimeEntry * thisEntry){
	int i;

	if(player->thisReportedCrimes->numReportedCrimes == player->thisReportedCrimes->MAX_REPORTED_CRIMES){
		cwrite("!! TOO MANY REPORTED CRIMES !!");
		free(thisEntry);
		return;
	}

	if(crimeAlreadyReported(player, thisEntry->crime, thisEntry->victimID, thisEntry->stolenItemID)){
		return;
	}

	for(i = 0; i < player->thisReportedCrimes->MAX_REPORTED_CRIMES; i++){
		if(player->thisReportedCrimes->reportedCrimeList[i] == NULL){
			player->thisReportedCrimes->reportedCrimeList[i] = thisEntry;
			player->thisReportedCrimes->numReportedCrimes++;
			return;
		}
	}

	cwrite("!! TOO MANY REPORTED CRIMES !!");
	free(thisEntry);
}

int reportActiveCrimes(individual * player){
	int i, newReportedCrimes = 0;

	if(player->thisActiveCrimes->numActiveCrimes == 0){
		return 0;
	}

	if(player->thisActiveCrimes->numActiveCrimes > 0){
		for(i = 0; i < player->thisActiveCrimes->MAX_ACTIVE_CRIMES; i++){
			if(player->thisActiveCrimes->activeCrimeList[i] != NULL){
			
				//This check is in the event that an individual died (on their turn or otherwise) but was witness to crimes
				if(doesExist(player->thisActiveCrimes->activeCrimeList[i]->witnessID) != 0){
					player->thisActiveCrimes->activeCrimeList[i]->witnessID = 0;
					addReportedCrimeFromEntry(player, player->thisActiveCrimes->activeCrimeList[i]);
					newReportedCrimes++;
				}

				player->thisActiveCrimes->activeCrimeList[i] = NULL;
				player->thisActiveCrimes->numActiveCrimes--;

				if(player->thisActiveCrimes->numActiveCrimes == 0){
					break;
				}
			}
		}
	}

	if(newReportedCrimes){
		return 1;
	}else{
		return 0;
	}
}

void addReportedCrime(individual * player, crimeType crime, int bounty, int victimID, int stolenItemID){
	activeCrimeEntry * thisCrime = malloc(sizeof(activeCrimeEntry));
	thisCrime->crime = crime;
	thisCrime->crimeBounty = bounty;
	thisCrime->victimID = victimID;
	thisCrime->stolenItemID = stolenItemID;
	thisCrime->witnessID = 0;

	addReportedCrimeFromEntry(player, thisCrime);
}

void addActiveCrime(individual * player, crimeType crime, int bounty, int victimID, int stolenItemID, int witnessID){
	activeCrimeEntry * thisCrime = malloc(sizeof(activeCrimeEntry));
	thisCrime->crime = crime;
	thisCrime->crimeBounty = bounty;
	thisCrime->witnessID = witnessID;
	thisCrime->victimID = victimID;
	thisCrime->stolenItemID = stolenItemID;

	addActiveCrimeFromEntry(player, thisCrime);
}

void removeActiveCrimesFromIndividual(individual * player, int individualID){
	int i, crimesPassed = 0;

	individual * thisNPC = getIndividualFromRegistry(individualID);

	if(player->thisActiveCrimes->numActiveCrimes > 0){
		for(i = 0; i < player->thisActiveCrimes->MAX_ACTIVE_CRIMES; i++){
			if(player->thisActiveCrimes->activeCrimeList[i] != NULL){

				if(player->thisActiveCrimes->activeCrimeList[i]->witnessID == individualID || player->thisActiveCrimes->activeCrimeList[i]->victimID == individualID){
					free(player->thisActiveCrimes->activeCrimeList[i]);
					player->thisActiveCrimes->activeCrimeList[i] = NULL;
					player->thisActiveCrimes->numActiveCrimes--;
					crimesPassed--;
				}

				crimesPassed++;
				if(crimesPassed == player->thisActiveCrimes->numActiveCrimes){
					break;
				}
			}
		}
	}

	if(thisNPC->specialDialog->activeDialog == DIALOG_CRIME_WITNESS){
		resetSpecialDialogForSpeakingIndividual(DIALOG_CRIME_WITNESS, individualID);
	}else if(thisNPC->specialDialog->activeDialog == DIALOG_ATTACKED_BY_PLAYER){
		resetSpecialDialogForSpeakingIndividual(DIALOG_ATTACKED_BY_PLAYER, individualID);
	}else if(thisNPC->specialDialog->activeDialog == DIALOG_STOLEN_FROM_BY_PLAYER){
		resetSpecialDialogForSpeakingIndividual(DIALOG_STOLEN_FROM_BY_PLAYER, individualID);
	}

}

void clearActiveCrimes(individual * player){
	int i;

	if(player->thisActiveCrimes->numActiveCrimes > 0){
		for(i = 0; i < player->thisActiveCrimes->MAX_ACTIVE_CRIMES; i++){
			if(player->thisActiveCrimes->activeCrimeList[i] != NULL){
				free(player->thisActiveCrimes->activeCrimeList[i]);
				player->thisActiveCrimes->activeCrimeList[i] = NULL;

				if(player->thisActiveCrimes->numActiveCrimes == 0){
					break;
				}
			}
		}
	}
}

void clearReportedCrimes(individual * player){
	int i;

	if(player->thisReportedCrimes->numReportedCrimes == 0){
		return;
	}

	for(i = 0; i < player->thisReportedCrimes->MAX_REPORTED_CRIMES; i++){
		if(player->thisReportedCrimes->reportedCrimeList[i] != NULL){
			free(player->thisReportedCrimes->reportedCrimeList[i]);
			player->thisReportedCrimes->reportedCrimeList[i] = NULL;
			player->thisReportedCrimes->numReportedCrimes--;

			if(player->thisReportedCrimes->numReportedCrimes == 0){
				break;
			}
		}
	}
}

int stealingCrimeAlreadyExists(activeCrimeEntry * thisCrime, int victimID, int itemID, int witnessID){
	return (thisCrime->victimID == victimID && thisCrime->stolenItemID == itemID && witnessID == thisCrime->witnessID);
}

int pickpocketCrimeAlreadyExists(activeCrimeEntry * thisCrime, int victimID, int itemID, int witnessID){
	return (thisCrime->victimID == victimID && thisCrime->stolenItemID == itemID && witnessID == thisCrime->witnessID);
}

int assaultCrimeAlreadyExists(activeCrimeEntry * thisCrime, int victimID, int witnessID){
	return (thisCrime->victimID == victimID  && witnessID == thisCrime->witnessID);
}

int murderCrimeAlreadyExists(activeCrimeEntry * thisCrime, int victimID, int witnessID){
	return (thisCrime->victimID == victimID  && witnessID == thisCrime->witnessID);
}

int activeCrimeAlreadyExists(individual * player, crimeType crime, int victimID, int itemID, int witnessID){
	int i, numCrimesPassed = 0;

		if(player->thisActiveCrimes->numActiveCrimes== 0){
			return 0;
		}

		for(i = 0; i < player->thisReportedCrimes->MAX_REPORTED_CRIMES; i++){
			if(player->thisActiveCrimes->activeCrimeList[i] != NULL){
				numCrimesPassed++;

				if(player->thisActiveCrimes->activeCrimeList[i]->crime == crime){
					switch(crime){
						case CRIME_STEALING:
							if(stealingCrimeAlreadyExists(player->thisActiveCrimes->activeCrimeList[i], victimID, itemID, witnessID)){
								return 1;
							}
							break;
						case CRIME_PICKPOCKETING:
							if(pickpocketCrimeAlreadyExists(player->thisActiveCrimes->activeCrimeList[i], victimID, itemID, witnessID)){
								return 1;
							}
							break;
						case CRIME_ASSULT:
							if(assaultCrimeAlreadyExists(player->thisActiveCrimes->activeCrimeList[i], victimID, witnessID)){
								return 1;
							}
							break;
						case CRIME_MURDER:
							if(murderCrimeAlreadyExists(player->thisActiveCrimes->activeCrimeList[i], victimID, witnessID)){
								return 1;
							}
							break;
					}
				}

				if(numCrimesPassed == player->thisActiveCrimes->numActiveCrimes){
					break;
				}

			}
		}

		return 0;
}

int crimeAlreadyReported(individual * player, crimeType crime, int victimID, int itemID){
	int i, numCrimesPassed;

	if(player->thisReportedCrimes->numReportedCrimes == 0){
		return 0;
	}

	for(i = 0; i < player->thisReportedCrimes->MAX_REPORTED_CRIMES; i++){
		if(player->thisReportedCrimes->reportedCrimeList[i] != NULL){
			if(player->thisReportedCrimes->reportedCrimeList[i]->crime == crime){
				switch(crime){
					case CRIME_STEALING:
						if(stealingCrimeAlreadyExists(player->thisReportedCrimes->reportedCrimeList[i], victimID, itemID, 0)){
							return 1;
						}
						break;
					case CRIME_PICKPOCKETING:
						if(pickpocketCrimeAlreadyExists(player->thisReportedCrimes->reportedCrimeList[i], victimID, itemID, 0)){
							return 1;
						}
						break;
					case CRIME_ASSULT:
						if(assaultCrimeAlreadyExists(player->thisReportedCrimes->reportedCrimeList[i], victimID, 0)){
							return 1;
						}
						break;
					case CRIME_MURDER:
						if(murderCrimeAlreadyExists(player->thisReportedCrimes->reportedCrimeList[i], victimID, 0)){
							return 1;
						}
						break;
				}
			}

			numCrimesPassed++;

			if(numCrimesPassed == player->thisReportedCrimes->numReportedCrimes){
				break;
			}

		}
	}

	return 0;
}

int getCurrentBounty(individual * player){
	int i, sum = 0, crimesPassed = 0;

	if(player->thisReportedCrimes->numReportedCrimes > 0){
		for(i = 0; i < player->thisReportedCrimes->MAX_REPORTED_CRIMES; i++){
			if(player->thisReportedCrimes->reportedCrimeList[i] != NULL){
				sum += player->thisReportedCrimes->reportedCrimeList[i]->crimeBounty;
				crimesPassed++;

				if(crimesPassed == player->thisReportedCrimes->numReportedCrimes){
					break;
				}
			}
		}
	}

	return sum;
}

char * getCrimeString(crimeType thisCrime){
	char * crimeStr = malloc(sizeof(char) * 32);

	switch(thisCrime){
		case CRIME_STEALING:
			strcpy(crimeStr, "stealing");
			return crimeStr;
		case CRIME_PICKPOCKETING:
			strcpy(crimeStr, "pickpocketing");
			return crimeStr;
		case CRIME_ASSULT:
			strcpy(crimeStr, "assult");
			return crimeStr;
		case CRIME_MURDER:
			strcpy(crimeStr, "murder");
			return crimeStr;
		case CRIME_TREASON:
			strcpy(crimeStr, "treason");
			return crimeStr;
		default:
			free(crimeStr);
			return NULL;
	}
}

int crimeAlertsVictim(crimeType thisCrime){
	if(thisCrime == CRIME_ASSULT || thisCrime == CRIME_MURDER){
		return 1;
	}else{
		return 0;
	}
}

char * getWorstCrime(individual * player){
	int i, crimesPassed = 0;
	crimeType worstCrime = CRIME_NONE;

	if(player->thisReportedCrimes->numReportedCrimes > 0){
		for(i = 0; i < player->thisReportedCrimes->MAX_REPORTED_CRIMES; i++){
			if(player->thisReportedCrimes->reportedCrimeList[i] != NULL){
				if(worstCrime < player->thisReportedCrimes->reportedCrimeList[i]->crime){
					worstCrime = player->thisReportedCrimes->reportedCrimeList[i]->crime;
				}

				crimesPassed++;

				if(crimesPassed == player->thisReportedCrimes->numReportedCrimes){
					break;
				}
			}

		}

		return getCrimeString(worstCrime);

	}else{
		return NULL;
	}
}

void removeStolenItems(individual * player){
	int i;

	for(i = 0; i < player->backpack->inventorySize; i++){
		item * tmpItem = player->backpack->inventoryArr[i];

		if(tmpItem != NULL && tmpItem->isStolen){
			removeItemFromInventory(player->backpack, tmpItem);

			individual * owner = getIndividualFromRegistry(tmpItem->owner);

			if(owner != NULL){
				addItemToInventory(owner->backpack, tmpItem);
			}

			i--;
		}
	}
}

int robIndividual(individual * player, individual * targetIndividual){
	int gold = 0;
	int d20 = rand() %20 + 1;

	if(targetIndividual->gold <= 0 || d20 == 1){
		cwrite("They dont have any gold.");
		return 0;
	}

	if(d20 == 20){
		player->gold = targetIndividual->gold;
		targetIndividual->gold = 0;

		return gold;
	}

	d20 = min(d20 + (getAttributeSum(player,"LUCK") * 2),20);

	gold = (d20 * 10 * targetIndividual->gold)/200;
	targetIndividual->gold = targetIndividual->gold - gold;

	player->gold += gold;

	char outStr[128];
	sprintf(outStr, "Robbed %d gold.", gold);
	cwrite(outStr);

	return gold;
}

int attemptToPickPocketItem(item * thisItem, individual * player){
	individual * targetIndividual = player->targetedIndividual;

	int d20 = (rand() % 20) + 1;

	if(d20 == 20){
		removeItemFromInventory(targetIndividual->backpack, thisItem);
		addItemToInventory(player->backpack, thisItem);
		thisItem->npcID = player->ID;

		refreshInventory(targetIndividual->backpack);
		return 1;
	}else if(d20 == 1){
		if(targetIndividual->currentGroupType == GROUP_NPCS || targetIndividual->currentGroupType == GROUP_GUARDS){
			processCrimeEvent(CRIME_PICKPOCKETING, 10 + thisItem->price, targetIndividual->ID, thisItem->ID);
		}

		char outStr[128];
		sprintf(outStr, "%s has caught you pickpocketing!", targetIndividual->name);
		cwrite(outStr);

		return 0;
	}

	d20 += getAttributeSum(player, "DEX") * 2;

	if(d20 >= 10 + getAttributeSum(targetIndividual, "DEX")){
		removeItemFromInventory(targetIndividual->backpack, thisItem);
		addItemToInventory(player->backpack, thisItem);
		thisItem->npcID = player->ID;

		refreshInventory(targetIndividual->backpack);
		return 1;
	}else{
		if(targetIndividual->currentGroupType == GROUP_NPCS || targetIndividual->currentGroupType == GROUP_GUARDS){
			processCrimeEvent(CRIME_PICKPOCKETING, 10 + thisItem->price, targetIndividual->ID, thisItem->ID);
		}

		char outStr[128];
		sprintf(outStr, "%s has caught you pickpocketing!", targetIndividual->name);
		cwrite(outStr);

		return 0;
	}

}

int tryPickPocketIndividualShowView(individual * player, individual * targetIndividual, int isCrime){
	int d20 = (rand() % 20) + 1;

	if(d20 == 20){
		enableInventoryPickPocketMode();
		enableInventoryViewMode(targetIndividual->backpack);
		return 1;
	}else if(d20 == 1){
		if(isCrime){
			processCrimeEvent(CRIME_PICKPOCKETING, 10, targetIndividual->ID, 0);
		}

		char outStr[128];
		sprintf(outStr, "%s has caught you trying to pickpocket!", targetIndividual->name);
		cwrite(outStr);

		return 0;
	}

	d20 += getAttributeSum(player, "DEX") * 2;

	if(d20 >= 10 + getAttributeSum(targetIndividual, "DEX")){
		enableInventoryPickPocketMode();
		enableInventoryViewMode(targetIndividual->backpack);
		return 1;
	}else{
		if(isCrime){
			processCrimeEvent(CRIME_PICKPOCKETING, 10, targetIndividual->ID, 0);
		}

		char outStr[128];
		sprintf(outStr, "%s has caught you trying to pickpocket!", targetIndividual->name);
		cwrite(outStr);

		return 0;
	}
}

int getAttributeFromIndividual(individual * thisIndividual, char * attribute){
	int toReturn = 0;

	if(strcmp("STR",attribute) == 0 ){
		toReturn += thisIndividual->STR;

		if(thisIndividual->foodBonusActive){
			toReturn++;
		}else if(thisIndividual->foodNegativeActive){
			toReturn--;
		}

		return toReturn;
	}else if(strcmp("DEX",attribute) == 0){
		toReturn += thisIndividual->DEX;

		if(thisIndividual->foodBonusActive){
			toReturn++;
		}else if(thisIndividual->foodNegativeActive){
			toReturn--;
		}

		return toReturn;
	}else if(strcmp("CON",attribute) == 0){
		toReturn += thisIndividual->CON;

		if(thisIndividual->foodBonusActive){
			toReturn++;
		}else if(thisIndividual->foodNegativeActive){
			toReturn--;
		}

		return toReturn;
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
	} else if(strcmp("LoS", attribute) == 0){
		return thisIndividual->LoS;
	}else if(strcmp("darkLoS", attribute) == 0){
		return thisIndividual->darkLoS;
	}else if(strcmp("baseFood", attribute) == 0){
		return thisIndividual->baseFood;
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
	}else if(strcmp("darkLoS",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->darkLoSMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->darkLoSMod;
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
	}
	return toReturn;
}

int getAttributeFromActiveAbility(ability * activeAbility, char * attribute){
	if(strcmp("STR",attribute) == 0 ){
		return 0;
	}else if(strcmp("DEX",attribute) == 0){
		return 0;
	}else if(strcmp("CON",attribute) == 0){
		return 0;
	}else if(strcmp("WILL",attribute) == 0){
		return 0;
	}else if(strcmp("INT",attribute) == 0){
		return 0;
	}else if(strcmp("WIS",attribute) == 0){
		return 0;
	}else if(strcmp("CHR",attribute) == 0){
		return 0;
	}else if(strcmp("LUCK",attribute) == 0){
		return 0;
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
	} else if(strcmp("baseFood",attribute) == 0 ){
		if(activeAbility->baseFoodEnabled){
			return activeAbility->baseFood->effectAndManaArray[activeAbility->baseFood->selectedIndex]->effectMagnitude;
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
	} else if(strcmp("darkLoS",attribute) == 0 || strcmp("LoS",attribute) == 0 ){
		if(activeAbility->LoSEnabled){
			return activeAbility->LoS->effectAndManaArray[activeAbility->LoS->selectedIndex]->effectMagnitude;
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
	}

	return 0;
}

int getAttributeFromActiveStatus(status * thisStatus, char * attribute){
	if(thisStatus->effect == STATUS_BERZERK){
		if(strcmp("attack",attribute) == 0){
			return -2;
		}else if(strcmp("baseDam",attribute) == 0 ){
			return 2;
		}
	}

	return 0;
}

int getAttributeSum(individual * thisIndividual, char * attribute){
	int toReturn = 0, i, itemTotal = 0, activeItemTotal = 0, abilitiesPassed = 0, statusesPassed = 0;

	toReturn += getAttributeFromIndividual(thisIndividual, attribute);

	for(i = 0; i < thisIndividual->backpack->MAX_ITEMS; i++){
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

	if(thisIndividual->activeAbilities->numAbilities > 0){
		for(i = 0; i < thisIndividual->activeAbilities->MAX_ABILITIES; i++){
			if(thisIndividual->activeAbilities->abilitiesList[i] != NULL){
				abilitiesPassed++;
				toReturn += getAttributeFromActiveAbility(thisIndividual->activeAbilities->abilitiesList[i]->thisAbility, attribute);

				if(abilitiesPassed >= thisIndividual->activeAbilities->numAbilities){
					break;
				}
			}
		}
	}

	if(thisIndividual->activeStatuses->numStatuses){
		for(i = 0; i < thisIndividual->activeStatuses->MAX_STATUSES; i++){
			if(thisIndividual->activeStatuses->statuses[i] != NULL){
				statusesPassed++;

				toReturn += getAttributeFromActiveStatus(thisIndividual->activeStatuses->statuses[i], attribute);

				if(statusesPassed == thisIndividual->activeStatuses->numStatuses){
					break;
				}
			}
		}
	}

	return toReturn;
}
