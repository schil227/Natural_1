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
	character background;
	bool isPassable;
	character currentCharacter;
	character items[];
} space;

typedef struct {
	size_t size;
	int width;
	int height;

	int currentX;
	int currentY;

	space grid[25][25];

} field;

#endif /* SRC_HEADERS_STRUCTS_FIELD_H_ */
