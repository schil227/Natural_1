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
	int MAX_ITEMS;
	int inventorySize;
	item * inventoryArr[1000];
} fieldInventory;

typedef struct {
	char transitMap[256];
	int transitID;
	int targetMapTransitID;
} transitInfo;

typedef struct {
	int numCharacters;
	int isPassable;
	int canSeeThrough;
	int canAttackThrough;
	character* background;
	individual* currentIndividual;
	transitInfo* thisTransitInfo;
} space;

typedef struct {
	size_t size;
	int id;
	int displayedWidth;
	int displayedHeight;

	int displayedX;
	int displayedY;

	int totalX;
	int totalY;

	space* grid[100][100];

	fieldInventory * thisFieldInventory;
	inventory * currentSpaceInventory;
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
	node * nodeArray[20];
	int size;
} nodeArr;

typedef struct{
	int sum;
	int pathLength;
	character * shadowCharacter;
	moveNode * rootMoveNode;
} moveNodeMeta;

typedef struct {
	int id;
	char mapName[64];
	int * individuals[250];
	int * items[500];

	int MAX_INDIVIDUALS;
	int numIndividuals;

	int MAX_ITEMS;
	int numItems;
} mapInfo;

#endif /* SRC_HEADERS_STRUCTS_FIELD_H_ */
