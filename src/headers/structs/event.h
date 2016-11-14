/*
 * event.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_EVENT_H_
#define SRC_HEADERS_STRUCTS_EVENT_H_

#include"../field_pub_methods.h";

typedef struct{
	int ID;
	int eventType;
	int nextEventID;

	int	individualID;
	int itemID;

	int intA;
	int intB;

	int dialogIDA;
	int dialogIDB;

	char message[256];
} event;

typedef struct{
	event * queue[20];
	int numEvents;
	int MAX_EVENTS;
} eventQueue;

typedef struct{
	int individualID;
	int eventID;
} individualEvent;

typedef struct{
	int itemID;
	int eventID;
} itemEvent;

typedef struct{
	individualEvent * map[1000];
	int numIndividualEvents;
	int MAX_MAP_SIZE;
} individualEventMap;

typedef struct{
	itemEvent * map[1000];
	int numItemEvents;
	int MAX_MAP_SIZE;
} itemEventMap;

typedef struct{
	individualEventMap * onAttackTriggerMap;
	individualEventMap * onHarmTriggerMap;
	individualEventMap * onDeathTriggerMap;
	itemEventMap * onPickupTriggerMap;
} eventTriggerManager;

#endif /* SRC_HEADERS_STRUCTS_EVENT_H_ */
