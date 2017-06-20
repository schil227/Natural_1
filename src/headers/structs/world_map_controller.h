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
	int nodeAID;
	int nodeBID;
	cord * nodeACord;
	cord * nodeBCord;
} steppingStoneToken;

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
	character * steppingStoneIdle;
	areaNode * currentAreaNode;
	areaNode * allNodes[100];
	cord * steppingStoneCords[2000];
	character * activeSteppingStones[100];
	int inWorldMapMode;
	int steppingStonesPerCords;

	int numAreaNodes;
	int MAX_AREA_NODES;

	int numSteppingStoneCords;
	int MAX_STEPPING_STONE_CORDS;

	int numActiveSteppingStones;
	int MAX_ACTIVE_STEPPING_STONES;

} worldMapController;


#endif /* SRC_HEADERS_STRUCTS_WORLD_MAP_CONTROLLER_H_ */
