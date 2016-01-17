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
	int eventType;

	int	individualID;
	int itemID;
} event;


typedef struct{
	event * eventArr[50];
	int numEvents;
} eventQueue;

#endif /* SRC_HEADERS_STRUCTS_EVENT_H_ */
