/*
 * field.h
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_FIELD_H_
#define SRC_HEADERS_STRUCTS_FIELD_H_
#include<stdbool.h>
#include "../character_pub_methods.h"

typedef struct {
	int numCharacters;

	bool isPassable;
//	character items[];
	character* background;
	character* currentCharacter;
} space;

typedef struct {
	size_t size;
	int displayedWidth;
	int displayedHeight;

	int displayedX;
	int displayedY;

	int totalX;
	int totalY;

	space* grid[25][25];

} field;

#endif /* SRC_HEADERS_STRUCTS_FIELD_H_ */
