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
#include "../interactable_pub_methods.h"

typedef struct {
	character * transitSpaceCharacter;
	character * worldSpaceCharacter;
} fieldGraphicContainer;

typedef struct {
	int MAX_ITEMS;
	int inventorySize;
	item * inventoryArr[1000];
} fieldInventory;

typedef struct {
	char transitMap[256];
	int transitID;
	int targetMapTransitID;
	int areaNodeID;
	int transitEventID;
	int showOutline;
} transitInfo;

typedef struct {
	int isPassable;
	int canSeeThrough;
	int canAttackThrough;
	int spaceIsReserved;
	character* background;
	individual* currentIndividual;
	interactable* interactableObject;
	transitInfo* thisTransitInfo;
} space;

typedef struct {
	size_t size;
	int id;
	int isDark;
	int playerLoS;

	int totalX;
	int totalY;

	space* grid[100][100];

	fieldInventory * thisFieldInventory;
	inventory * currentSpaceInventory;
	cord * playerCords;
} field;

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

typedef struct {
	int id;
	char mapName[64];
	int * individuals[250];
	int * items[500];
	int * interactableObjects[500];
	int isDark;
	int isCurrentMap;

	int MAX_INDIVIDUALS;
	int numIndividuals;

	int MAX_ITEMS;
	int numItems;

	int MAX_INTERACTABLES;
	int numInteractables;
} mapInfo;

#endif /* SRC_HEADERS_STRUCTS_FIELD_H_ */
