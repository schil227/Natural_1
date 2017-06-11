/*
 * event.c
 *
 *  Created on: Jan 16, 2016
 *      Author: Adrian
 */
#include"./headers/event_pub_methods.h"
#include<stdio.h>

static eventQueue * theEventQueue;
static eventTriggerManager * theEventTriggerManager;

void initEventHandlers(){
	initTheEventQueue();
	initEventTriggerManager();
}

void initTheEventQueue(){
	theEventQueue = malloc(sizeof(eventQueue));
	theEventQueue->numEvents = 0;
	theEventQueue->MAX_EVENTS = 20;
}

void initEventTriggerManager(){
	theEventTriggerManager = malloc(sizeof(eventTriggerManager));

	theEventTriggerManager->onAttackTriggerMap = malloc(sizeof(individualEventMap));
	theEventTriggerManager->onHarmTriggerMap = malloc(sizeof(individualEventMap));
	theEventTriggerManager->onDeathTriggerMap = malloc(sizeof(individualEventMap));
	theEventTriggerManager->onPickupTriggerMap = malloc(sizeof(itemEventMap));

	theEventTriggerManager->onAttackTriggerMap->MAX_MAP_SIZE = 1000;
	theEventTriggerManager->onHarmTriggerMap->MAX_MAP_SIZE = 1000;
	theEventTriggerManager->onDeathTriggerMap->MAX_MAP_SIZE = 1000;
	theEventTriggerManager->onPickupTriggerMap->MAX_MAP_SIZE = 1000;

	theEventTriggerManager->onAttackTriggerMap->numIndividualEvents = 0;
	theEventTriggerManager->onHarmTriggerMap->numIndividualEvents = 0;
	theEventTriggerManager->onDeathTriggerMap->numIndividualEvents = 0;
	theEventTriggerManager->onPickupTriggerMap->numItemEvents = 0;
}

void destroyEventHandlers(){
	destroyEventQueue();
	destroyEventTriggerManager();
}

void destroyEventQueue(){
	free(theEventQueue);
}

void destroyEventTriggerManager(){
	int i;

	for(i = 0; i < theEventTriggerManager->onAttackTriggerMap->numIndividualEvents; i++){
		free(theEventTriggerManager->onAttackTriggerMap->map[i]);
	}

	for(i = 0; i < theEventTriggerManager->onHarmTriggerMap->numIndividualEvents; i++){
		free(theEventTriggerManager->onHarmTriggerMap->map[i]);
	}

	for(i = 0; i < theEventTriggerManager->onDeathTriggerMap->numIndividualEvents; i++){
		free(theEventTriggerManager->onDeathTriggerMap->map[i]);
	}

	free(theEventTriggerManager->onAttackTriggerMap);
	free(theEventTriggerManager->onHarmTriggerMap);
	free(theEventTriggerManager->onDeathTriggerMap);

	free(theEventTriggerManager);
}

event * triggerEventOnAttack(int thisIndividualID, int isPlayer){
	int i;

	for(i = 0; i < theEventTriggerManager->onAttackTriggerMap->numIndividualEvents; i++){
		if(theEventTriggerManager->onAttackTriggerMap->map[i]->individualID == thisIndividualID){
			int onlyTriggerableByPlayer = eventOnlyTriggerableByPlayer(theEventTriggerManager->onAttackTriggerMap->map[i]->eventID);

			if(!onlyTriggerableByPlayer || (onlyTriggerableByPlayer && isPlayer)){
				triggerEvent(theEventTriggerManager->onAttackTriggerMap->map[i]->eventID);
			}
		}
	}

	return 0;
}

event * triggerEventOnHarm(int thisIndividualID, int isPlayer){
	int i;

	for(i = 0; i < theEventTriggerManager->onHarmTriggerMap->numIndividualEvents; i++){
		if(theEventTriggerManager->onHarmTriggerMap->map[i]->individualID == thisIndividualID){
			int onlyTriggerableByPlayer = eventOnlyTriggerableByPlayer(theEventTriggerManager->onHarmTriggerMap->map[i]->eventID);

			if(!onlyTriggerableByPlayer || (onlyTriggerableByPlayer && isPlayer)){
				triggerEvent(theEventTriggerManager->onHarmTriggerMap->map[i]->eventID);
			}
		}
	}

	return 0;
}

event * triggerEventOnDeath(int thisIndividualID, int isPlayer){
	int i;

	for(i = 0; i < theEventTriggerManager->onDeathTriggerMap->numIndividualEvents; i++){
		if(theEventTriggerManager->onDeathTriggerMap->map[i]->individualID == thisIndividualID){
			int onlyTriggerableByPlayer = eventOnlyTriggerableByPlayer(theEventTriggerManager->onDeathTriggerMap->map[i]->eventID);

			if(!onlyTriggerableByPlayer || (onlyTriggerableByPlayer && isPlayer)){
				triggerEvent(theEventTriggerManager->onDeathTriggerMap->map[i]->eventID);
			}
		}
	}

	return 0;
}

event * triggerEventOnPickup(int thisItemID, int isPlayer){
	int i;

	for(i = 0; i < theEventTriggerManager->onPickupTriggerMap->numItemEvents; i++){
		if(theEventTriggerManager->onPickupTriggerMap->map[i]->itemID == thisItemID){
			int onlyTriggerableByPlayer = eventOnlyTriggerableByPlayer(theEventTriggerManager->onPickupTriggerMap->map[i]->eventID);

			if(!onlyTriggerableByPlayer || (onlyTriggerableByPlayer && isPlayer)){
				triggerEvent(theEventTriggerManager->onPickupTriggerMap->map[i]->eventID);
			}
		}
	}

	return 0;
}

event * createEventFromFile(char * line){
	event * newEvent = malloc(sizeof(event));

	char * value = strtok(line,";");
	newEvent->ID = atoi(value);

	value = strtok(NULL, ";");
	newEvent->eventType = atoi(value);

	value = strtok(NULL, ";");
	newEvent->nextEventID = atoi(value);

	value = strtok(NULL, ";");
	newEvent->individualID = atoi(value);

	value = strtok(NULL, ";");
	newEvent->itemID = atoi(value);

	value = strtok(NULL, ";");
	newEvent->intA = atoi(value);

	value = strtok(NULL, ";");
	newEvent->intB = atoi(value);

	value = strtok(NULL, ";");
	newEvent->dialogIDCritSuccess = atoi(value);

	value = strtok(NULL, ";");
	newEvent->dialogIDSuccess = atoi(value);

	value = strtok(NULL, ";");
	newEvent->dialogIDInconclusive = atoi(value);

	value = strtok(NULL, ";");
	newEvent->dialogIDFailure = atoi(value);

	value = strtok(NULL, ";");
	newEvent->dialogIDCritFailure = atoi(value);

	value = strtok(NULL, ";");
	strcpy(newEvent->message, value);

	if(newEvent->message[strlen(newEvent->message) - 1] == '\n' ){
		newEvent->message[strlen(newEvent->message) - 1] = '\0';
	}

	return newEvent;
}

void loadTriggerMaps(char* directory, char* onAttackTriggerFileName, char* onHarmTriggerFileName, char* onDeathTriggerFileName, char* onPickupTriggerFileName){
	loadIndividualEventsToTriggerManager(directory, onAttackTriggerFileName, theEventTriggerManager->onAttackTriggerMap);
	loadIndividualEventsToTriggerManager(directory, onHarmTriggerFileName, theEventTriggerManager->onHarmTriggerMap);
	loadIndividualEventsToTriggerManager(directory, onDeathTriggerFileName, theEventTriggerManager->onDeathTriggerMap);
	loadItemEventsToTriggerManager(directory, onPickupTriggerFileName, theEventTriggerManager->onPickupTriggerMap);
}

individualEvent * createIndividualEventFromLine(char * line){
	individualEvent * newIE = malloc(sizeof(individualEvent));

	char * value = strtok(line,";");
	newIE->individualID = atoi(value);

	value = strtok(NULL, ";");
	newIE->eventID = atoi(value);

	return newIE;
}

itemEvent * createItemEventFromLine(char * line){
	itemEvent * newItemEvent = malloc(sizeof(itemEvent));

	char * value = strtok(line, ";");
	newItemEvent->itemID = atoi(value);

	value = strtok(NULL, ";");
	newItemEvent->eventID = atoi(value);

	return newItemEvent;
}

void loadIndividualEventsToTriggerManager(char* directory, char* triggerFileName, individualEventMap * triggerMap){
	char * fullFileName = appendStrings(directory, triggerFileName);
	FILE * FP = fopen(fullFileName, "r");
	char line[32];

	while(fgets(line,32,FP) != NULL){
		if (line[0] != '#') {
			individualEvent * newIndividualEvent = createIndividualEventFromLine(line);
			addIndividualEventToMap(newIndividualEvent, triggerMap);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadItemEventsToTriggerManager(char * directory, char * triggerFileName, itemEventMap * triggerMap){
	char * fullFileName = appendStrings(directory, triggerFileName);
	FILE * FP = fopen(fullFileName, "r");
	char line[32];

	while(fgets(line,32,FP) != NULL){
		if (line[0] != '#') {
			itemEvent * newItemEvent = createItemEventFromLine(line);
			addItemEventToMap(newItemEvent, triggerMap);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void addIndividualEventToMap(individualEvent * newIndividualEvent, individualEventMap * triggerMap){
	if(triggerMap->numIndividualEvents +1 == triggerMap->MAX_MAP_SIZE){
		char * errLog[128];
		sprintf(errLog, "!! COULD NOT ADD TO EVENT TRIGGER MAP: INDIVIDUAL ID: %d, EVENT ID: %d", newIndividualEvent->individualID, newIndividualEvent->eventID);
		cwrite(errLog);
	}

	triggerMap->map[triggerMap->numIndividualEvents] = newIndividualEvent;
	triggerMap->numIndividualEvents++;
	triggerMap->map[triggerMap->numIndividualEvents] = NULL;
}

void addItemEventToMap(itemEvent * newItemEvent, itemEventMap * triggerMap){
	if(triggerMap->numItemEvents +1 == triggerMap->MAX_MAP_SIZE){
		char * errLog[128];
		sprintf(errLog, "!! COULD NOT ADD TO EVENT TRIGGER MAP: INDIVIDUAL ID: %d, EVENT ID: %d", newItemEvent->itemID, newItemEvent->eventID);
		cwrite(errLog);
	}

	triggerMap->map[triggerMap->numItemEvents] = newItemEvent;
	triggerMap->numItemEvents++;
	triggerMap->map[triggerMap->numItemEvents] = NULL;
}

void removeIndividualEventFromMap(individualEvent * thisIndividualEvent, individualEventMap * triggerMap){
	int i;

	for(i = 0; i < triggerMap->numIndividualEvents; i++){
		if(triggerMap->map[i] == thisIndividualEvent){
			free(triggerMap->map[i]);

			//rebalance array
			triggerMap->numIndividualEvents--;
			triggerMap->map[i] = triggerMap->map[triggerMap->numIndividualEvents];
			triggerMap->map[triggerMap->numIndividualEvents] = NULL;
		}
	}
}

int eventOnlyTriggerableByPlayer(int eventID){
	switch(eventID){
	case 5: //Crime
		return 1;
	default:
		return 0;
	}
}

int processCrimeEvent(crimeType crime, int bounty, int victimID, int itemID){
	int i;

	event * thisCrimeEvent = getEventFromRegistryByTypeAndIntA(5,crime);

	thisCrimeEvent->intB = bounty;
	thisCrimeEvent->individualID = victimID;
	thisCrimeEvent->itemID = itemID;

	return triggerEvent(thisCrimeEvent->ID);
}

///// Process Event Functions /////

int becomeNPC(individual * player, int individualID, int faction, groupContainer * thisGroupContainer){
	individual * thisIndividual = getIndividualFromRegistry(individualID);

	if(thisIndividual->currentGroupType != GROUP_NPCS){
		deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer,thisIndividual), thisIndividual);
		addIndividualToGroup(thisGroupContainer->npcs, thisIndividual);
		thisIndividual->currentGroupType = GROUP_NPCS;
		thisIndividual->thisBehavior->isFocusedOnPlayer = 0;
		thisIndividual->faction = faction;

		if(thisIndividual->targetedIndividual->isPlayer){
			thisIndividual->targetedIndividual = NULL;
			thisIndividual->targetedDuration = 0;
		}

		return 1;
	}

	return 0;
}

int becomeEnemy(int individualID, int faction, int focusedOnPlayer, groupContainer * thisGroupContainer){
	individual * thisIndividual = getIndividualFromRegistry(individualID);

	if(thisIndividual->currentGroupType != GROUP_ENEMIES){
		deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer,thisIndividual), thisIndividual);
		addIndividualToGroup(thisGroupContainer->enemies, thisIndividual);
		thisIndividual->currentGroupType = GROUP_ENEMIES;
		thisIndividual->thisBehavior->isFocusedOnPlayer = focusedOnPlayer;
		thisIndividual->faction = faction;
		return 1;
	}

	return 0;

}

int enterBuyMode(int individualID){
	individual * merchant = getIndividualFromRegistry(individualID);
	enableInventoryBuyMode();
	enableInventoryViewMode(merchant->backpack);

	return 1;
}

int statCheck(individual * player, event * thisEvent){
	int statValue = 0, d20, toReturn;

	statValue = getAttributeSum(player, thisEvent->message);
	d20 = rand() % 20 + 1;

	char statOut[128];
	sprintf(statOut, "%s Check: %d + 2*%d = %d", thisEvent->message, d20, statValue, (d20 + statValue*2));
	cwrite(statOut);

	if((d20 + statValue*2) >= thisEvent->intA){
		setNextMessageByID(thisEvent->dialogIDSuccess);
		toReturn = 1;
	}else{//failed
		setNextMessageByID(thisEvent->dialogIDFailure);

		toReturn = 0;
	}

	return toReturn;
}

int crimeCommittedInLoS(individual * player, individualGroup * guards, individualGroup * npcs, field * thisField, event * crimeEvent){
	int i, individuals_passed = 0, toReturn = 0, isInGuardLoS = 0;
	int crimeTypeID = crimeEvent->intA;
	int bounty = crimeEvent->intB;

	//Can a guard see the player
	if(guards->numIndividuals > 0){
		for(i = 0; i < guards->MAX_INDIVIDUALS; i++){
			if(guards->individuals[i] != NULL){

				if(isInLineOfSight(guards->individuals[i], player, thisField)){
					isInGuardLoS = 1;
					break;
				}

				individuals_passed++;
				if(individuals_passed == guards->numIndividuals){
					break;
				}
			}
		}
	}

	//If it's a violent crime, whether or not it's reported, make applicable groups hostile
	if(crimeAlertsVictim((crimeType) crimeTypeID)){
		if(player->targetedIndividual != NULL){
			if(player->targetedIndividual->currentGroupType == GROUP_GUARDS || isInGuardLoS){
				changePlayerDispositionForFriendlyGroups(guards, npcs, 0, 1);
			}

			player->targetedIndividual->thisBehavior->isHostileToPlayer = 1;
		}
	}

	if(crimeAlreadyReported(player, (crimeType) crimeTypeID, crimeEvent->individualID, crimeEvent->itemID)){
		return -1;
	}

	if(crimeAlertsVictim((crimeType) crimeTypeID)){
		if(player->targetedIndividual != NULL){
			if(player->targetedIndividual->currentGroupType == GROUP_GUARDS){
				reportActiveCrimes(player);
				clearCrimeSpecialDialogForGroup(npcs);
				setGroupSpecialDialog(guards, DIALOG_CRIME_WITNESS);

				addReportedCrime(player, (crimeType) crimeTypeID, bounty, crimeEvent->individualID, crimeEvent->itemID);
				changePlayerDispositionForFriendlyGroups(guards, npcs, 0, 1);
				return -1;
			}else if(player->targetedIndividual->currentGroupType == GROUP_NPCS){
				if(!activeCrimeAlreadyExists(player,(crimeType)  crimeTypeID, crimeEvent->individualID, crimeEvent->itemID, crimeEvent->individualID)){
					addActiveCrime(player, (crimeType) crimeTypeID, bounty, crimeEvent->individualID, crimeEvent->itemID, crimeEvent->individualID);
				}

				if(player->targetedIndividual->specialDialog->activeDialog != DIALOG_AFRAID_OF_PLAYER){
					setSpecialDialogId(player->targetedIndividual->ID, player->targetedIndividual->specialDialog->attackedByPlayer);
					player->targetedIndividual->specialDialog->activeDialog = DIALOG_ATTACKED_BY_PLAYER;
				}
			}
		}
	}

	if(isInGuardLoS){
		reportActiveCrimes(player);
		clearCrimeSpecialDialogForGroup(npcs);
		setGroupSpecialDialog(guards, DIALOG_CRIME_WITNESS);

		addReportedCrime(player, (crimeType) crimeTypeID, bounty, crimeEvent->individualID, crimeEvent->itemID);

		return -1;
	}

	individuals_passed = 0;
	if(npcs->numIndividuals > 0){
		for(i = 0; i < npcs->MAX_INDIVIDUALS; i++){
			if(npcs->individuals[i] != NULL){

				if(isInLineOfSight(npcs->individuals[i], player, thisField)){
					if(!activeCrimeAlreadyExists(player, crimeTypeID, crimeEvent->individualID, crimeEvent->itemID, npcs->individuals[i]->ID)){
						addActiveCrime(player, (crimeType) crimeTypeID, bounty, crimeEvent->individualID, crimeEvent->itemID, npcs->individuals[i]->ID);
					}

					if(npcs->individuals[i]->specialDialog->activeDialog == DIALOG_DEFAULT){
						setSpecialDialogId(npcs->individuals[i]->ID, npcs->individuals[i]->specialDialog->sawPlayerCrime);
						npcs->individuals[i]->specialDialog->activeDialog = DIALOG_CRIME_WITNESS;
					}
					toReturn++;
				}

				individuals_passed++;
				if(individuals_passed == npcs->numIndividuals){
					break;
				}
			}
		}
	}

	return toReturn;
}

int multiStatDialogCheck(individual * player, event * thisEvent){
	int toBeat = 12, statSum = 0, d20;
	int numStats = thisEvent->intA;

	if(thisEvent->intB > 0){
		toBeat = thisEvent->intB;
	}

	d20 = rand() % 20 + 1;

	if(d20 == 20){
		cwrite("Natural 20!");
		if(thisEvent->dialogIDCritSuccess > 0){
			return thisEvent->dialogIDCritSuccess;
		}else{
			return thisEvent->dialogIDSuccess;
		}
	}else if(d20 == 1){
		cwrite("Natural 1!");
		if(thisEvent->dialogIDCritFailure > 0){
			return thisEvent->dialogIDCritFailure;
		}else{
			return thisEvent->dialogIDFailure;
		}
	}

	char * value = strtok(thisEvent->message,",");

	while(value != NULL){
		int tmpTotal = getAttributeSum(player, value);

		//get magnitued of stat
		value = strtok(NULL,",");
		statSum += tmpTotal * atoi(value);

		value = strtok(NULL,",");
	}

	d20 += statSum;

	if(d20 == toBeat && thisEvent->dialogIDInconclusive > 0){
		return thisEvent->dialogIDInconclusive;
	}

	if(d20 > toBeat){
		return thisEvent->dialogIDSuccess;
	}else{
		return thisEvent->dialogIDFailure;
	}
}

int multiStatDialogCheckCritShift(individual * player, event * thisEvent){
	int toBeat = 10, statSum = 0, d20;
	char * value = strtok(thisEvent->message,",");
	int numStats = atoi(value);

	d20 = rand() % 20 + 1;

	if(d20 == 20){
		cwrite("Natural 20!");
		if(thisEvent->dialogIDCritSuccess > 0){
			return thisEvent->dialogIDCritSuccess;
		}else{
			return thisEvent->dialogIDSuccess;
		}
	}else if(d20 == 1){
		cwrite("Natural 1!");
		if(thisEvent->dialogIDCritFailure > 0){
			return thisEvent->dialogIDCritFailure;
		}else{
			return thisEvent->dialogIDFailure;
		}
	}

	value = strtok(NULL,",");

	while(value != NULL){
		int tmpTotal = getAttributeSum(player, value);

		//get magnitued of stat
		value = strtok(NULL,",");
		statSum += tmpTotal * atoi(value);

		value = strtok(NULL,",");
	}

	d20 += statSum;

	if(d20 == toBeat && thisEvent->dialogIDInconclusive > 0){
		return thisEvent->dialogIDInconclusive;
	}

	if(d20 > thisEvent->intA){
		return thisEvent->dialogIDCritSuccess;
	}else if(d20 > toBeat){
		return thisEvent->dialogIDSuccess;
	}else if(d20 > thisEvent->intB){
		return thisEvent->dialogIDFailure;
	}else{
		return thisEvent->dialogIDCritFailure;
	}
}

int statsAtLeastX(individual * player, event * thisEvent){
	int numStats = thisEvent->intA;
	int allMustSucceed = thisEvent->intB;
	int atLeastOneSucceeded = 0;
	char * strtok_save_pointer;
	char msg[128];

	strcpy(msg, thisEvent->message);

	char * value = strtok_r(msg,",", &strtok_save_pointer);

	while(value != NULL){
		int tmpTotal = getAttributeSum(player, value);

		//get magnitued of stat
		value = strtok_r(NULL,",", &strtok_save_pointer);

		if(tmpTotal < atoi(value) && allMustSucceed){
			return 0;
		}else if(tmpTotal >= atoi(value)){
			if(!allMustSucceed){
				return 1;
			}

			atLeastOneSucceeded = 1;
		}

		value = strtok_r(NULL,",", &strtok_save_pointer);
	}

	if(atLeastOneSucceeded){
		return 1;
	}else{
		return 0;
	}
}

int returnstatsAtLeastXReturnDialog(individual * player, event * thisEvent){
	if(statsAtLeastX(player, thisEvent)){
		return thisEvent->dialogIDSuccess;
	}else{
		return thisEvent->dialogIDFailure;
	}
}

int tryReturnStolenWitnessedItems(individual * player, int witnessID){
	return 0;
}

int makeIndividualHostileToPlayer(int individualID){
	individual * thisIndividual = getIndividualFromRegistry(individualID);
	if(thisIndividual != NULL){
		thisIndividual->thisBehavior->isHostileToPlayer = 1;
		return 1;
	}else{
		return 0;
	}
}

int makeIndividualFriendlyToPlayer(int individualID){
	individual * thisIndividual = getIndividualFromRegistry(individualID);

	if(thisIndividual != NULL){
		thisIndividual->thisBehavior->isHostileToPlayer = 0;
		return 1;
	}else{
		return 0;
	}
}

int makeSpeakingIndividualFriendlyToPlayer(int speakingIndividualID){
	individual * speakingIndividual = getIndividualFromRegistry(speakingIndividualID);

	if(speakingIndividual != NULL){
		speakingIndividual->thisBehavior->isHostileToPlayer = 0;
		return 1;
	}else{
		return 0;
	}
}

int resetSpecialDialogForSpeakingIndividual(dialogType specialDialogType, int speakingIndividualID){
	individual * speakingIndividual = getIndividualFromRegistry(speakingIndividualID);

	if(speakingIndividual->specialDialog->activeDialog == specialDialogType){
		removeSpecialDialog(speakingIndividualID);
	}

	return 0;
}

int resetSpecialDialogForSpeakingIndividualGroup(dialogType specialDialogType, int speakingIndividualID, groupContainer * thisGroupContainer){
	int i, individualsPassed = 0;

	individual * speakingIndividual = getIndividualFromRegistry(speakingIndividualID);
	individualGroup * thisGroup = getGroupFromIndividual(thisGroupContainer, speakingIndividual);

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i]  != NULL){
			if(thisGroup->individuals[i]->specialDialog->activeDialog == specialDialogType){
				removeSpecialDialog(thisGroup->individuals[i]->ID);
			}

			individualsPassed++;

			if(individualsPassed == thisGroup->numIndividuals){
				break;
			}
		}
	}

	return 0;
}

int clearCrimesSpecialDialogStolenItems(individual * player, individualGroup * guards, individualGroup * npcs){
	clearActiveCrimes(player);
	clearReportedCrimes(player);
	clearCrimeSpecialDialogForGroup(guards);
	clearCrimeSpecialDialogForGroup(npcs);
	removeStolenItems(player);

	return 1;
}

int changePlayerDispositionForFriendlyGroups(individualGroup * guards, individualGroup * npcs, int speakingIndividualID, int isHostile){
	int i, individualsPassed = 0;

	if(speakingIndividualID != 0 && !isHostile){
		individual * tmpindividual = getIndividualFromRegistry(speakingIndividualID);

		if(tmpindividual != NULL){
			tmpindividual->thisBehavior->hasAlreadyYieldedToPlayer = 1;
		}

	}

	if(guards->numIndividuals > 0){
		for(i = 0; i < guards->numIndividuals; i++){
			if(guards->individuals[i] != NULL){
				guards->individuals[i]->thisBehavior->isHostileToPlayer = isHostile;

				individualsPassed++;

				if(individualsPassed == guards->numIndividuals){
					individualsPassed = 0;
					break;
				}

			}
		}
	}

	if(npcs->numIndividuals > 0){
		for(i = 0; i < npcs->numIndividuals; i++){
			if(npcs->individuals[i] != NULL){
				npcs->individuals[i]->thisBehavior->isHostileToPlayer = isHostile;

				individualsPassed++;

				if(individualsPassed == npcs->numIndividuals){
					individualsPassed = 0;
					break;
				}

			}
		}
	}

	return 0;
}

int tryRobIndividual(individual * player, event * thisEvent, int individualID){
	individual * targetIndividual = getIndividualFromRegistry(individualID);

	robIndividual(player, targetIndividual);

	//disallow player from removing crime
	targetIndividual->thisBehavior->isHostileToPlayer = 0;
	targetIndividual->thisBehavior->hasAlreadyYieldedToPlayer = 1;
	targetIndividual->specialDialog->activeDialog = DIALOG_AFRAID_OF_PLAYER;
	setSpecialDialogId(individualID, targetIndividual->specialDialog->afraidOfPlayer);

	return 0;
}

int addIndividualToAllyGroup(groupContainer * thisGroupContainer, int individualID, int newDialogId){
	if(thisGroupContainer->allies->numIndividuals >= thisGroupContainer->allies->MAX_INDIVIDUALS){
		return 0;
	}

	individual * ally = getIndividualFromRegistry(individualID);

	if(ally == NULL){
		return 0;
	}

	individualGroup * formerGroup = getGroupFromIndividual(thisGroupContainer, ally);

	if(formerGroup == NULL){
		return 0;
	}

	deleteIndividiaulFromGroup(formerGroup, ally);
	addIndividualToGroup(thisGroupContainer->allies, ally);
	ally->currentGroupType = GROUP_ALLIES;

	if(newDialogId > 0){
		loadOrAddIndividualDialog(individualID, newDialogId, 1);
	}

	return 1;
}

int returnIndividualToDefaultGroup(groupContainer * thisGroupContainer, int individualID, int newDialogId){
	individual * thisIndividual = getIndividualFromRegistry(individualID);

	if(thisIndividual == NULL){
		return 0;
	}

	individualGroup * currentGroup = getGroupFromIndividual(thisGroupContainer, thisIndividual);
	individualGroup * defaultGroup = getDefaultGroupFromIndividual(thisGroupContainer, thisIndividual);

	if(defaultGroup == NULL || currentGroup == NULL || defaultGroup->numIndividuals >= defaultGroup->MAX_INDIVIDUALS){
		return 0;
	}

	thisIndividual->currentGroupType = thisIndividual->defaultGroupType;
	deleteIndividiaulFromGroup(currentGroup, thisIndividual);
	addIndividualToGroup(defaultGroup, thisIndividual);

	//Set their desired location to wherever they are
	thisIndividual->desiredLocation->x = thisIndividual->playerCharacter->x;
	thisIndividual->desiredLocation->y = thisIndividual->playerCharacter->y;

	if(newDialogId > 0){
		loadOrAddIndividualDialog(individualID, newDialogId, 1);
	}

	return 1;
}

int getNextEventID(int eventID){
	event * thisEvent = getEventFromRegistry(eventID);

	if(thisEvent->nextEventID){
		return thisEvent->nextEventID;
	}else{
		return 0;
	}
}

void completeAndDestroyCurrentInteractive(individual * player, event * thisEvent){
	interactable * thisInteractableObject = player->currentInteractableObject;
	animationState stateToClone;
	animation * interactAnimation;

	thisInteractableObject->isEnabled = 0;
	thisInteractableObject->isPassable = 1;
	thisInteractableObject->shouldDraw = 0;
	thisInteractableObject->inFinalMode = 1;

	switch(thisEvent->intA){
		case 0:
			return;
		case 1:
			stateToClone = ANIMATION_INTERACTABLE_ACTION_1;
			break;
		case 2:
			stateToClone = ANIMATION_INTERACTABLE_ACTION_2;
			break;
		case 3:
			stateToClone = ANIMATION_INTERACTABLE_ACTION_FINAL;
			break;
	}

	animation * tmpAnimation = getAnimationFromType(thisInteractableObject->thisCharacter->thisAnimationContainer, stateToClone);

	if(tmpAnimation == NULL){
		return;
	}

	interactAnimation = cloneAnimation(tmpAnimation);

	addCharacterToSpecialDrawWithCoords(createCharacterFromAnimation(interactAnimation), thisInteractableObject->thisCharacter->x, thisInteractableObject->thisCharacter->y);
	increaseSpecialDrawDurationIfGreater(interactAnimation->totalDuration);
	enableSpecialDrawMode();
}

void finalizeInteractable(individual * player, event * thisEvent){
	interactable * thisInteractableObject = player->currentInteractableObject;
	animation * interactAnimation;

	thisInteractableObject->inFinalMode = 1;

	setDefaultAnimation(thisInteractableObject->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_FINAL);

	if(thisEvent->intA != 0){
		switch(thisEvent->intA){
			case 1:
				setAnimation(thisInteractableObject->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_1);
				break;
			case 2:
				setAnimation(thisInteractableObject->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_2);
				break;
		}
	}
}

void decreaseAttribute(individual * player, event * thisEvent){
	if(strcmp(thisEvent->message, "hp") == 0){
		player->hp -= thisEvent->intA;
	}else if(strcmp(thisEvent->message, "mana") == 0){
		player->mana -= thisEvent->intA;
	}else if(strcmp(thisEvent->message, "food") == 0){
		decreaseFood(player, ((double)thisEvent->intA));
	}

	if(player->hp <= 0){
		removeFromExistance(player->ID);
		enableSpecialDrawMode();
		addSpecialIndividual(player);
		int delay = player->playerCharacter->thisAnimationContainer->animations[player->playerCharacter->thisAnimationContainer->currentAnimation]->totalDuration;
		setIndividualDelayAnimation(player, ANIMATION_DEATH, delay);
		int deathDelay = player->playerCharacter->thisAnimationContainer->animations[player->playerCharacter->thisAnimationContainer->nextAnimationAfterDelay]->totalDuration;
		increaseSpecialDrawDurationIfGreater(delay + deathDelay);
	}
}

int hasItemForSelectedInteractable(individual * player, event * thisEvent){
	int i;
	item * tmpItem;

	if(player->currentInteractableObject == NULL){
		return 0;
	}

	for(i = 0; i < player->backpack->inventorySize; i++){
		tmpItem = player->backpack->inventoryArr[i];

		if(tmpItem != NULL && tmpItem->interactableObjectType == player->currentInteractableObject->type){
			return 1;
		}
	}

	return 0;
}

int removeItemForSelectedInteractableReturnSuccessDialog(individual * player, event * thisEvent){
	int i;
	item * tmpItem;

	if(player->currentInteractableObject == NULL){
		return 0;
	}

	for(i = 0; i < player->backpack->inventorySize; i++){
		tmpItem = player->backpack->inventoryArr[i];

		if(tmpItem != NULL && tmpItem->interactableObjectType == player->currentInteractableObject->type){
			removeItemFromInventory(player->backpack, tmpItem);
		}
	}

	return thisEvent->dialogIDSuccess;
}

int cloneItemToPlayer(individual * player, event * thisEvent){
	item * tmpItem = getItemFromRegistry(thisEvent->intA);

	if(tmpItem != NULL){
		item * newItem = cloneItem(tmpItem, 0);

		addItemToInventory(player->backpack, newItem);
	}

	return 0;
}

void viewInteractableItems(individual * player, event * thisEvent){
	enableObjectGetMode();
	enableInventoryViewMode(player->currentInteractableObject->objectInventory);
}

void handleMimicActivation(individual * player, event * thisEvent, groupContainer * thisGroupContainer, field * thisField, int * inActionMode){
	individual * tmpIndividual = getIndividualFromRegistry(thisEvent->individualID);
	interactable * thisInteractable = player->currentInteractableObject;

	if(tmpIndividual == NULL){
		return;
	}

	spawnIndividualToField(thisGroupContainer, thisField, tmpIndividual->ID, thisInteractable->thisCharacter->x, thisInteractable->thisCharacter->y, thisInteractable->thisCharacter->x, thisInteractable->thisCharacter->y);
	addAnimationToContainer(tmpIndividual->playerCharacter->thisAnimationContainer, cloneAnimation(getAnimationFromType(thisInteractable->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_FINAL)));
	setAnimation(tmpIndividual->playerCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_FINAL);

	thisInteractable->isEnabled = 0;
	thisInteractable->shouldDraw = 0;
	thisInteractable->isPassable = 1;
	thisInteractable->inFinalMode = 1;

	decreaseTurns(player, thisGroupContainer, player->remainingActions, *inActionMode);
}

void attackDesguisedMimic(individual * player, event * thisEvent, groupContainer * thisGroupContainer, field * thisField, int * inActionMode){
	individual * tmpIndividual = getIndividualFromRegistry(thisEvent->individualID);
	interactable * thisInteractable = player->currentInteractableObject;

	if(tmpIndividual == NULL){
		return;
	}

	spawnIndividualToField(thisGroupContainer, thisField, tmpIndividual->ID, thisInteractable->thisCharacter->x, thisInteractable->thisCharacter->y, thisInteractable->thisCharacter->x, thisInteractable->thisCharacter->y);
	addAnimationToContainer(tmpIndividual->playerCharacter->thisAnimationContainer, cloneAnimation(getAnimationFromType(thisInteractable->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_FINAL)));
	setAnimation(tmpIndividual->playerCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_FINAL);

	thisInteractable->isEnabled = 0;
	thisInteractable->shouldDraw = 0;
	thisInteractable->isPassable = 1;
	thisInteractable->inFinalMode = 1;

	tmpIndividual->hp = (short int)(getTotalHP(tmpIndividual) * 0.8);
	decreaseTurns(tmpIndividual, thisGroupContainer, tmpIndividual->remainingActions, *inActionMode);
}

void openGateEvent(individual * player, event * thisEvent){
	interactable * gate = getInteractableObjectFromRegistry(thisEvent->intA);
	interactable * thisSwitch = player->currentInteractableObject;
	animation * gateOpenAnimation;

	if(gate == NULL){
		return;
	}

	//flip switch
	thisSwitch->inFinalMode = 1;
	setDefaultAnimation(thisSwitch->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_FINAL);
	setAnimation(thisSwitch->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_1);

	//raise gate
	gate->isEnabled = 0;
	gate->isPassable = 1;
	gate->shouldDraw = 0;
	gate->inFinalMode = 1;
	gate->canAttackThrough = 1;

	animation * tmpAnimation = getAnimationFromType(gate->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_FINAL);

	if(tmpAnimation == NULL){
		return;
	}

	gateOpenAnimation = cloneAnimation(tmpAnimation);

	addCharacterToSpecialDrawWithCoords(createCharacterFromAnimation(gateOpenAnimation), gate->thisCharacter->x, gate->thisCharacter->y);
	increaseSpecialDrawDurationIfGreater(gateOpenAnimation->totalDuration);
	enableSpecialDrawMode();
}

void closeGateEvent(individual * player, event * thisEvent, field * thisField){
	interactable * gate = getInteractableObjectFromRegistry(thisEvent->intA);
	interactable * thisSwitch = player->currentInteractableObject;
	animation * gateOpenAnimation;
	space * tmpSpace;

	if(gate == NULL){
		return;
	}

	tmpSpace = getSpaceFromField(thisField, gate->thisCharacter->x, gate->thisCharacter->y);

	//cant close gate if individual in the way
	if(tmpSpace == NULL || tmpSpace->currentIndividual != NULL){
		return;
	}

	//flip switch
	thisSwitch->inFinalMode = 0;
	setDefaultAnimation(thisSwitch->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_IDLE);
	setAnimation(thisSwitch->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_2);

	//lower gate
	gate->isEnabled = 1;
	gate->isPassable = 0;
	gate->shouldDraw = 1;
	gate->inFinalMode = 0;
	gate->canAttackThrough = 0;

	setAnimation(gate->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_1);

	increaseSpecialDrawDurationIfGreater(gate->thisCharacter->thisAnimationContainer->animations[gate->thisCharacter->thisAnimationContainer->currentAnimation]->totalDuration);
	enableSpecialDrawMode();
}

int processEvent(int eventID, individual * player, groupContainer * thisGroupContainer, field * thisField, int * inActionMode){
	event * thisEvent = getEventFromRegistry(eventID);

	switch(thisEvent->eventType){
		case 1://become npc
			return becomeNPC(player, thisEvent->individualID, thisEvent->intA, thisGroupContainer);
		case 2: // become enemy
			return becomeEnemy(thisEvent->individualID, thisEvent->intA, thisEvent->intB, thisGroupContainer);
		case 3: // enable buy mode
			return enterBuyMode(thisEvent->individualID);
		case 4: // stat check
			return statCheck(player, thisEvent);
		case 5:
			return crimeCommittedInLoS(player, thisGroupContainer->guards, thisGroupContainer->npcs, thisField, thisEvent);
		case 6: // remove active crimes from talking witness
			return removeActiveCrimesFromIndividual(player, getSpeakingIndividualID());
		case 7: // report all active crimes
			return reportActiveCrimes(player);
		case 8: // multi stat dialog check
			return multiStatDialogCheck(player, thisEvent);
		case 9: // stats at least X
			return statsAtLeastX(player, thisEvent);
		case 10:// try return stolen witnessed items
			return tryReturnStolenWitnessedItems(player, getSpeakingIndividualID());
		case 11:
			return makeIndividualHostileToPlayer(thisEvent->individualID);
		case 12:
			return makeIndividualFriendlyToPlayer(thisEvent->individualID);
		case 13: // reset special dialog for speaking individual
			return resetSpecialDialogForSpeakingIndividual(thisEvent->intA, getSpeakingIndividualID());
		case 14: // reset special dialog for speaking individual group
			return resetSpecialDialogForSpeakingIndividualGroup(thisEvent->intA, getSpeakingIndividualID(), thisGroupContainer);
		case 15: //clear crimes and special dialogs
			return clearCrimesSpecialDialogStolenItems(player, thisGroupContainer->guards, thisGroupContainer->npcs);
		case 16:
			return makeSpeakingIndividualFriendlyToPlayer(getSpeakingIndividualID());
		case 17://try rob individual
			return tryRobIndividual(player, thisEvent, getSpeakingIndividualID());
		case 18: //make player hostile to friendlies
			return changePlayerDispositionForFriendlyGroups(thisGroupContainer->guards, thisGroupContainer->npcs, getSpeakingIndividualID(), thisEvent->intA);
		case 19:
			return addIndividualToAllyGroup(thisGroupContainer, thisEvent->individualID, thisEvent->intA);
		case 20:
			return returnIndividualToDefaultGroup(thisGroupContainer, thisEvent->individualID, thisEvent->intA);
		case 21:
			return forcePlayerTransit(thisEvent->intA, thisEvent->intB);
		case 22:
			return getCurrentBounty(player) <= player->gold;
		case 23:
			player->gold = max(0, player->gold - getCurrentBounty(player));
			return 0;
		case 24://complete and destroy current event
			completeAndDestroyCurrentInteractive(player, thisEvent);
			return 0;
		case 25:
			decreaseAttribute(player, thisEvent);
			return 0;
		case 26://stat at least return dialog
			return returnstatsAtLeastXReturnDialog(player, thisEvent);
		case 27:
			return hasItemForSelectedInteractable(player, thisEvent);
		case 28:
			return removeItemForSelectedInteractableReturnSuccessDialog(player, thisEvent);
		case 29:
			return multiStatDialogCheckCritShift(player, thisEvent);
		case 30:
			return cloneItemToPlayer(player, thisEvent);
		case 31:
			finalizeInteractable(player, thisEvent);
			return 0;
		case 32:
			viewInteractableItems(player, thisEvent);
			return 0;
		case 33:
			handleMimicActivation(player, thisEvent, thisGroupContainer, thisField, inActionMode);
			return 0;
		case 34:
			attackDesguisedMimic(player, thisEvent, thisGroupContainer, thisField, inActionMode);
			return 0;
		case 35:
			openGateEvent(player, thisEvent);
			return 0;
		case 36:
			closeGateEvent(player, thisEvent, thisField);
			return 0;
		}
}

char * processContextKey(char * contextKey, individual * player, groupContainer * thisGroupContainer, field * thisField){
	if(strcmp(contextKey, "NAME") == 0){
		char * toReturn = malloc(sizeof(char) * 32);
		strcpy(toReturn, player->name);
		return toReturn;
	}

	if(strcmp(contextKey, "BOUNTY") == 0){
		int bounty = getCurrentBounty(player);
		char * toReturn = malloc(sizeof(char) * 32);
		itoa(bounty, toReturn, 10);
		return toReturn;
	}

	if(strcmp(contextKey, "WORST_CRIME") == 0){
		return getWorstCrime(player);
	}

	if(strcmp(contextKey, "TARGET_NAME") == 0){
		char * toReturn = malloc(sizeof(char) * 32);

		individual * speakingIndividual = getIndividualFromRegistry(getSpeakingIndividualID());
		if(speakingIndividual != NULL){
			strcpy(toReturn, speakingIndividual->name);
		}

		return toReturn;
	}

	char * toReturn = malloc(sizeof(char) * 10);
	strcpy(toReturn, "NOT_FOUND");
	return toReturn;
}
