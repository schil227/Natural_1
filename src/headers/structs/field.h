/*
 * field.h
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_FIELD_H_
#define SRC_HEADERS_STRUCTS_FIELD_H_
#include "../character_pub_methods.h"

typedef struct {
	int numCharacters;
	character charArr[];
} characterArray;

typedef struct {
	size_t size;
	int width;
	int height;

	characterArray grid[25][25];

} field;

#endif /* SRC_HEADERS_STRUCTS_FIELD_H_ */
