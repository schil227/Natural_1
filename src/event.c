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

///// Process Event Functions /////

int becomeNPC(int individualID, groupContainer * thisGroupContainer){
	individual * thisIndividual = getIndividualFromRegistry(individualID);

	if(thisIndividual->currentGroupType != GROUP_NPCS){
		deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer,thisIndividual), thisIndividual);
		addIndividualToGroup(thisGroupContainer->npcs, thisIndividual);
		thisIndividual->currentGroupType = GROUP_NPCS;

		return 1;
	}

	return 0;
}

int becomeEnemy(int individualID, groupContainer * thisGroupContainer){
	individual * thisIndividual = getIndividualFromRegistry(individualID);

	if(thisIndividual->currentGroupType != GROUP_ENEMIES){
		deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer,thisIndividual), thisIndividual);
		addIndividualToGroup(thisGroupContainer->enemies, thisIndividual);
		thisIndividual->currentGroupType = GROUP_ENEMIES;

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

int crimeCommittedInLoS(individual * player, individualGroup * guards, individualGroup * npcs, field * thisField, int crimeType, int bounty){
	int i, individuals_passed = 0, toReturn = 0;

	if(guards->numIndividuals > 0){
		for(i = 0; i < guards->MAX_INDIVIDUALS; i++){
			if(guards->individuals[i] != NULL){

				if(isInLineOfSight(guards->individuals[i], player, thisField)){

					reportActiveCrimes(player);
					addReportedCrime(crimeType, bounty);

					return -1;
				}

				individuals_passed++;
				if(individuals_passed == guards->numIndividuals){
					break;
				}
			}
		}
	}

	individuals_passed = 0;
	if(npcs->numIndividuals > 0){
		for(i = 0; i < npcs->MAX_INDIVIDUALS; i++){
			if(npcs->individuals[i] != NULL){

				if(isInLineOfSight(npcs->individuals[i], player, thisField)){
					addActiveCrime(crimeType, bounty, npcs->individuals[i]);
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

int statsAtLeastX(individual * player, event * thisEvent){
	int numStats = thisEvent->intA;
	int allMustSucceed = thisEvent->intB;
	int atLeastOneSucceeded = 0;

	char * value = strtok(thisEvent->message,",");

	while(value != NULL){
		int tmpTotal = getAttributeSum(player, value);

		//get magnitued of stat
		value = strtok(NULL,",");

		if(tmpTotal < atoi(value) && allMustSucceed){
			return 0;
		}else if(tmpTotal >= atoi(value)){
			if(!allMustSucceed){
				return 1;
			}

			atLeastOneSucceeded = 1;
		}

		value = strtok(NULL,",");
	}

	if(atLeastOneSucceeded){
		return 1;
	}else{
		return 0;
	}
}

int tryReturnStolenWitnessedItems(individual * player, int witnessID){
	return 0;
}

int processEvent(int eventID, individual * player, groupContainer * thisGroupContainer, field * thisField){

	event * thisEvent = getEventFromRegistry(eventID);

	switch(thisEvent->eventType){
		case 1://become npc
			return becomeNPC(thisEvent->individualID, thisGroupContainer);
		case 2: // become enemy
			return becomeEnemy(thisEvent->individualID, thisGroupContainer);
		case 3: // enable buy mode
			return enterBuyMode(thisEvent->individualID);
		case 4: // stat check
			return statCheck(player, thisEvent);
		case 5:
			return crimeCommittedInLoS(player, thisGroupContainer->guards, thisGroupContainer->npcs, thisField, thisEvent->intA, thisEvent->intB);
		case 6: // remove active crimes from talking witness
			return removeActiveCrimesFromTalkingWitness(player, getSpeakingIndividualID());
		case 7: // report all active crimes
			return reportActiveCrimes(player);
		case 8: // multi stat dialog check
			return multiStatDialogCheck(player, thisEvent);
		case 9: // stats at least X
			return statsAtLeastX(player, thisEvent);
		case 10:// try return stolen witnessed items
			return tryReturnStolenWitnessedItems(player, getSpeakingIndividualID());
	}

	return 0;
}

char * processContextKey(char * contextKey, individual * player, groupContainer * thisGroupContainer, field * thisField){
	if(strcmp(contextKey, "NAME") == 0){
		char * toReturn = malloc(sizeof(char) * 32);
		strcpy(toReturn, player->name);
		return toReturn;
	}

	if(strcmp(contextKey, "BOUNTY")){
		int bounty = getCurrentBounty(player);
		char * toReturn = malloc(sizeof(char) * 32);
		itoa(bounty, toReturn, 10);
		return toReturn;
	}

	if(strcmp(contextKey, "WORST_CRIME")){
		return getWorstCrime(player);
	}
}
