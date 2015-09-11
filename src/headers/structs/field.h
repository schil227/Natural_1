/*
 * field.h
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_FIELD_H_
#define SRC_HEADERS_STRUCTS_FIELD_H_
#include<stdbool.h>
#include "../individual_pub_methods.h"

typedef struct {
	int numCharacters;

	bool isPassable;
//	character items[];
	character* background;
	individual* currentIndividual;
//	individual* tmpIndividual;
} space;

typedef struct {
	size_t size;
	int displayedWidth;
	int displayedHeight;

	int displayedX;
	int displayedY;

	int totalX;
	int totalY;

	space* grid[100][100];

} field;

typedef struct {
	int x;
	int y;
	int hasTraversed;
	struct moveNode * nextMoveNode;
} moveNode;


typedef struct {
	int pathLength;
	int x;
	int y;
	int isFinalPathNode;
	struct node * previousNode;
} node;

typedef struct {
	node ** nodeArray;
	int size;
} nodeArr;

typedef struct{
	int sum;
	int pathLength;
	character * shadowCharacter;
	moveNode * rootMoveNode;
} moveNodeMeta;


#endif /* SRC_HEADERS_STRUCTS_FIELD_H_ */
