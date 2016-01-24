/*
 * event.c
 *
 *  Created on: Jan 16, 2016
 *      Author: Adrian
 */
#include"./headers/event_pub_methods.h"

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
			triggerEvent(theEventTriggerManager->onHarmTriggerMap->map[i]->eventID);
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
			triggerEvent(theEventTriggerManager->onHarmTriggerMap->map[i]->eventID);
			return 1;
		}
	}

	return 0;
}

void processEvent(int eventID, individual * player, individualGroup * npcs, individualGroup * enemies, field * thisField){

}
