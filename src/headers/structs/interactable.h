/*
 * interactable.h
 *
 *  Created on: May 30, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_INTERACTABLE_H_
#define SRC_HEADERS_STRUCTS_INTERACTABLE_H_

#include"../item_pub_methods.h"

typedef enum {INTERACT_DEFAULT = 0, INTERACT_DOOR_FOREST_DUNGEON, INTERACT_CHEST, INTERACT_FISHING, INTERACT_SWITCH, INTERACT_SIGN} interactType;

typedef struct{
	int ID;
	int isPassable;
	int canAttackThrough;
	int canSeeThrough;
	int triggerDialogID;
	int triggerEventID;
	int onAttackEventID;
	int isEnabled;
	int shouldDraw;
	int isRespawning;
	int finalEvent;
	int inFinalMode;
	interactType type;

	inventory * objectInventory;
	character * thisCharacter;
} interactable;

#endif /* SRC_HEADERS_STRUCTS_INTERACTABLE_H_ */
