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

	theEventTriggerManager->onAttackTriggerMap->MAX_MAP_SIZE = 1000;
	theEventTriggerManager->onHarmTriggerMap->MAX_MAP_SIZE = 1000;
	theEventTriggerManager->onDeathTriggerMap->MAX_MAP_SIZE = 1000;

	theEventTriggerManager->onAttackTriggerMap->numIndividualEvents = 0;
	theEventTriggerManager->onHarmTriggerMap->numIndividualEvents = 0;
	theEventTriggerManager->onDeathTriggerMap->numIndividualEvents = 0;
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

event * triggerEventOnAttack(int thisIndividualID){
	int i;

	for(i = 0; i < theEventTriggerManager->onAttackTriggerMap->numIndividualEvents; i++){
		if(theEventTriggerManager->onAttackTriggerMap->map[i]->individualID == thisIndividualID){
			triggerEvent(theEventTriggerManager->onAttackTriggerMap->map[i]->eventID);
			return 1;
		}
	}

	return 0;
}

event * triggerEventOnHarm(int thisIndividualID){
	int i;

	for(i = 0; i < theEventTriggerManager->onHarmTriggerMap->numIndividualEvents; i++){
		if(theEventTriggerManager->onHarmTriggerMap->map[i]->individualID == thisIndividualID){
			triggerEvent(theEventTriggerManager->onHarmTriggerMap->map[i]->eventID);
			return 1;
		}
	}

	return 0;
}

event * triggerEventOnDeath(int thisIndividualID){
	int i;

	for(i = 0; i < theEventTriggerManager->onDeathTriggerMap->numIndividualEvents; i++){
		if(theEventTriggerManager->onDeathTriggerMap->map[i]->individualID == thisIndividualID){
			triggerEvent(theEventTriggerManager->onDeathTriggerMap->map[i]->eventID);
			return 1;
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
	newEvent->dialogIDA = atoi(value);

	value = strtok(NULL, ";");
	newEvent->dialogIDB = atoi(value);

	value = strtok(NULL, ";");
	strcpy(newEvent->message, value);

	return newEvent;
}

void loadTriggerMaps(char* directory, char* onAttackTriggerFileName, char* onHarmTriggerFileName, char* onDeathTriggerFileName){
	loadIndividualEventsToTriggerManager(directory, onAttackTriggerFileName, theEventTriggerManager->onAttackTriggerMap);
	loadIndividualEventsToTriggerManager(directory, onHarmTriggerFileName, theEventTriggerManager->onHarmTriggerMap);
	loadIndividualEventsToTriggerManager(directory, onDeathTriggerFileName, theEventTriggerManager->onDeathTriggerMap);
}

individualEvent * createIndividualEventFromLine(char * line){
	individualEvent * newIE = malloc(sizeof(individualEvent));

	char * value = strtok(line,";");
	newIE->individualID = atoi(value);

	value = strtok(NULL, ";");
	newIE->eventID = atoi(value);

	return newIE;
}

void loadIndividualEventsToTriggerManager(char* directory, char* triggerFileName, individualEventMap * triggerMap){
	char * fullFileName = appendStrings(directory, triggerFileName);
	//fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
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

///// Process Event Functions /////

void becomeNPC(int individualID, individualGroup * npcs, individualGroup * enemies){
	individual * thisIndividual = getIndividualFromRegistry(individualID);

	if(!individualInGroup(thisIndividual, npcs)){
		deleteIndividiaulFromGroup(enemies, thisIndividual);
		addIndividualToGroup(npcs, thisIndividual);
	}

}

void becomeEnemy(int individualID, individualGroup * npcs, individualGroup * enemies){
	individual * thisIndividual = getIndividualFromRegistry(individualID);

	if(!individualInGroup(thisIndividual, enemies)){
		deleteIndividiaulFromGroup(npcs, thisIndividual);
		addIndividualToGroup(enemies, thisIndividual);
	}

}

void enterBuyMode(int individualID){
	individual * merchant = getIndividualFromRegistry(individualID);
	enableInventoryBuyMode();
	enableInventoryViewMode(merchant->backpack);
}


void processEvent(int eventID, individual * player, individualGroup * npcs, individualGroup * enemies, field * thisField){

	event * thisEvent = getEventFromRegistry(eventID);

	switch(thisEvent->eventType){
		case 1://become npc
			becomeNPC(thisEvent->individualID, npcs, enemies);
			break;
		case 2: // become enemy
			becomeEnemy(thisEvent->individualID, npcs, enemies);
			break;
		case 3: // enable buy mode
			enterBuyMode(thisEvent->individualID);

	}

}
