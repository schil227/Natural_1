/*
 * mapGenerator.h
 *
 *  Created on: May 23, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_MAP_GENERATOR_H_
#define SRC_HEADERS_STRUCTS_MAP_GENERATOR_H_
#include "../field_pub_methods.h"

typedef struct{
	field * thisField;
	fixedCharacter * selectedCharacterBox;
	fixedCharacter * characterSelectBackground;
	character * cursor;
	character * selectedCharacter;
	character * characterSet[1000];
	cord * cursorCords;
	int numCharacters;
	int maxCharacters;
	int currentSelectedCharacterIndex;
	int inSelectionMode;
	int selectionStartingHeight;
	int selectedIndex;
	int numCharactersPerRow;
	int rotation;
} mapGenerator;


#endif /* SRC_HEADERS_STRUCTS_MAP_GENERATOR_H_ */
