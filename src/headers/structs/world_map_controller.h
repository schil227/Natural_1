/*
 * world_map_controller.h
 *
 *  Created on: Jun 13, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_WORLD_MAP_CONTROLLER_H_
#define SRC_HEADERS_STRUCTS_WORLD_MAP_CONTROLLER_H_

#include"../individual_pub_methods.h"

typedef struct{
	int id;
	character * nodeCharacter;

	int * LinkedNodeIDs[8];
	int MAX_NODES;

	cord * steppingStones[15];
	int numSteppingStones;
	int MAX_STEPPING_STONES;

	int playerOccupied;
	int isHidden;
	int mapID;
	int mapTransitID;
} areaNode;

typedef struct{
	fixedCharacter * worldMap;
	areaNode * currentAreaNode;
	areaNode * allNodes[100];
	int numAreaNodes;
	int MAX_AREA_NODES;
	int inWorldMapMode;
} worldMapController;


#endif /* SRC_HEADERS_STRUCTS_WORLD_MAP_CONTROLLER_H_ */
