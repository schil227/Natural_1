/*
 * cursor.h
 *
 *  Created on: Apr 26, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_CURSOR_H_
#define SRC_HEADERS_STRUCTS_CURSOR_H_

#include"../individual_pub_methods.h"

typedef enum {CURSOR_ATTACK, CURSOR_TALK, CURSOR_ABILITY, CURSOR_ABILITY_AOE_LINE, CURSOR_PICKPOCKET, CURSOR_LOOK, CURSOR_INTERACT} cursorModes;

typedef struct {
	int inCursorMode;
	cursorModes thisMode;

	cord * tmpCord;
	cordArr * aoeSpaceCoordinates;

	character * cursorCharacter;
	character * attackSpace;
} cursor;


#endif /* SRC_HEADERS_STRUCTS_CURSOR_H_ */
