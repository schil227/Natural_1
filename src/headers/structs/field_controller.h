/*
 * field_controller.h
 *
 *  Created on: Jul 23, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_FIELD_CONTROLLER_H_
#define SRC_HEADERS_STRUCTS_FIELD_CONTROLLER_H_
#include "../field_pub_methods.h"

typedef enum {TRANSITION_SLOW, TRANSITION_MEDIUM, TRANSITION_FAST} windowTransitionSpeed;

typedef struct{
	int inWindowTransition;
	int readyToTransitField;
	int greyscaleLevel;
	int fadingOut;
	int fieldUnloaded;

	int areaNodeId;
	char mapDirectory[256];
	char mapName[256];
	windowTransitionSpeed selectedTransition;

	fixedCharacter * blackCharacter;
} windowTransition;

typedef struct {
	size_t size;
	int MAX_INDIVIDUALS;
	int numIndividuals;
	int currentIndividualIndex;
	individual * individuals[50];
} individualGroup;

typedef struct{
	int initGroupActionMode;
	int groupActionMode;
	int groupMoveMode;
	int postGroupActionMode;

	groupType activeGroup;

	individualGroup * enemies;
	individualGroup * beasts;
	individualGroup * allies;
	individualGroup * npcs;
	individualGroup * guards;
	individualGroup * movingIndividuals;

	individualGroup * selectedGroup;

} groupContainer;

#endif /* SRC_HEADERS_STRUCTS_FIELD_CONTROLLER_H_ */
