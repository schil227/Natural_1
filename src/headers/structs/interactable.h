/*
 * interactable.h
 *
 *  Created on: May 30, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_INTERACTABLE_H_
#define SRC_HEADERS_STRUCTS_INTERACTABLE_H_

#include "../character_pub_methods.h"

typedef enum {INTERACT_DEFAULT, INTERACT_DOOR, INTERACT_CHEST, INTERACT_FISHING, INTERACT_SWITCH, INTERACT_SIGN} interactType;

typedef struct{
	int ID;
	int isPassable;
	int canAttackThrough;
	int canSeeThrough;
	int triggerDialogID;
	int triggerEventID;
	int isEnabled;
	int shouldDraw;
	int isRespawning;
	int finalEvent;
	interactType type;

	character * thisCharacter;
} interactable;

#endif /* SRC_HEADERS_STRUCTS_INTERACTABLE_H_ */
