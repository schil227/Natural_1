/*
 * event.c
 *
 *  Created on: Jan 16, 2016
 *      Author: Adrian
 */
#include"./headers/event_pub_methods.h"

static eventQueue * theEventQueue;

void initTheEventQueue(){
	theEventQueue = malloc(sizeof(eventQueue));

	theEventQueue->eventArr[0] = NULL;
	theEventQueue->numEvents = 0;
}

int eventsToProcess(){
	if(theEventQueue != NULL){
		return theEventQueue->numEvents;
	}

	return 0;
}

int addEventToQueue(int eventID){

}

void processAllEvents(individual * player, individualGroup * npcs, individualGroup * enemies, field * thisField){
	int i;

	for(i = 0; i < theEventQueue->numEvents; i++){
		processEvent(theEventQueue->eventArr[i], player, npcs, enemies, thisField);
		theEventQueue->eventArr[i] = NULL;
	}

}

void processEvent(event theEvent, individual * player, individualGroup * npcs, individualGroup * enemies, field * thisField){

}
