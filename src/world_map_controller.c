/*
 * world_map_controller.c
 *
 *  Created on: Jun 13, 2017
 *      Author: Adrian
 */

#include "./headers/world_map_controller_pub_methods.h"

static int AREA_NODE_IDLE_IMAGE_ID = 10004;
static int AREA_NODE_ACTIVE_IMAGE_ID = 10005;
static int STEPPING_STONE_IDLE_IMAGE_ID = 10006;
static int STEPPING_STONE_ACTIVE_IMAGE_ID = 10007;

worldMapController * thisWorldMapController;

void initWorldMapController(int worldMapImageID){
	thisWorldMapController = malloc(sizeof(worldMapController));
	thisWorldMapController->worldMap = createCharacter(worldMapImageID, RGB(255,0,255), 0,0);
	thisWorldMapController->MAX_AREA_NODES = 100;
	thisWorldMapController->inWorldMapMode = 0;
	thisWorldMapController->currentAreaNode = NULL;
	thisWorldMapController->numAreaNodes = getAllAreaNodesFromRegistry(thisWorldMapController->allNodes, thisWorldMapController->MAX_AREA_NODES);
}

void destroyWorldMapController(){
	destroyFixedCharacter(thisWorldMapController->worldMap);
	free(thisWorldMapController);
}

void enableWorldMapMode(int areaNodeID){
	thisWorldMapController->inWorldMapMode = 1;
	thisWorldMapController->currentAreaNode = getAreaNodeFromRegistry(areaNodeID);
	thisWorldMapController->currentAreaNode->playerOccupied = 1;
	thisWorldMapController->currentAreaNode->isHidden = 0;
	setCurrentNeighborNodeAnimations(thisWorldMapController->currentAreaNode, 1);
}

void disableWorldMapMode(){
	setCurrentNeighborNodeAnimations(thisWorldMapController->currentAreaNode, 0);
	thisWorldMapController->inWorldMapMode = 0;
}

int inWorldMapMode(){
	if(thisWorldMapController != NULL){
		return thisWorldMapController->inWorldMapMode;
	}

	return 0;
}

areaNode * getCurrentAreaNode(){
	return thisWorldMapController->currentAreaNode;
}

void drawWorldMapInstance(HDC hdc, HDC hdcBuffer, individual * player, shiftData * viewShift){
	int i, x = 0, y = 0;

	//TODO: update bounds for scrolling
	drawUnboundCharacterByPixels(hdc, hdcBuffer, 0 + viewShift->xShift, 0 + viewShift->yShift, thisWorldMapController->worldMap);

	for(i = 0; i < thisWorldMapController->numAreaNodes; i++){
		areaNode * tmpNode = thisWorldMapController->allNodes[i];

		if(tmpNode->isHidden){
			continue;
		}

		x = tmpNode->nodeCharacter->x;
		y = tmpNode->nodeCharacter->y;

		//TODO: check if in bounds before drawing

		drawUnboundAnimationByPixels(hdc, hdcBuffer, tmpNode->nodeCharacter, x, y, 0);
		updateAnimation(tmpNode->nodeCharacter);

		if(tmpNode->playerOccupied){
			int playerXOff = x - 37;
			int playerYOff = y - 58;
			drawIndividualDefaultByPixels(hdc, hdcBuffer, player, playerXOff, playerYOff, 0);
		}
	}
}

void tryMoveWorldMap(int directionIndex){
	int nextAreaNodeID;
	areaNode * newAreaNode;
	if(directionIndex == 5){
		return;
	}

	//adjust for the gap in the array
	if(directionIndex > 5){
		directionIndex -= 2;
	}else{
		directionIndex -= 1;
	}

	nextAreaNodeID = thisWorldMapController->currentAreaNode->LinkedNodeIDs[directionIndex];

	if(nextAreaNodeID == 0){
		return;
	}

	newAreaNode = getAreaNodeFromRegistry(nextAreaNodeID);

	thisWorldMapController->currentAreaNode->playerOccupied = 0;
	setCurrentNeighborNodeAnimations(thisWorldMapController->currentAreaNode, 0);
	thisWorldMapController->currentAreaNode = newAreaNode;
	thisWorldMapController->currentAreaNode->playerOccupied = 1;

	setCurrentNeighborNodeAnimations(thisWorldMapController->currentAreaNode, 1);
}

void setCurrentNeighborNodeAnimations(areaNode * newNode,  int animateSpace){
	int i;

	for(i = 0; i < thisWorldMapController->numAreaNodes; i++){
		areaNode * tmpNode = thisWorldMapController->allNodes[i];

		if(isCurrentNeighborNode(tmpNode)){
			if(animateSpace){
				setDefaultAnimation(tmpNode->nodeCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_1);
			}else{
				setDefaultAnimation(tmpNode->nodeCharacter->thisAnimationContainer, ANIMATION_IDLE);
			}
		}
	}
}

int isCurrentNeighborNode(areaNode * thisNode){
	int i;

	for(i = 0; i < thisWorldMapController->currentAreaNode->MAX_NODES; i++){
		if(thisNode->id == thisWorldMapController->currentAreaNode->LinkedNodeIDs[i]){
			return 1;
		}
	}

	return 0;
}

void unhideAreaNodes(char * areaNodeIDs){
	char * strtok_line_pointer;
	char * value;

	value = strtok_r(areaNodeIDs, ",", &strtok_line_pointer);
	while(value != NULL){
		unhideAreaNodeByID(atoi(value));
		value = strtok_r(NULL, ",", &strtok_line_pointer);
	}
}

void unhideAreaNodeByID(int id){
	int i;

	for(i = 0; i < thisWorldMapController->numAreaNodes; i++){
		if(thisWorldMapController->allNodes[i]->id == id){
			thisWorldMapController->allNodes[i]->isHidden = 0;
		}
	}
}

areaNode * getAreaNodeFromLine(char * line){
	areaNode * thisAreaNode = malloc(sizeof(areaNode));
	char * value;
	char * strtok_line_pointer;

	thisAreaNode->playerOccupied = 0;
	thisAreaNode->MAX_NODES = 8;
	thisAreaNode->MAX_STEPPING_STONES = 15;
	thisAreaNode->numSteppingStones = 0;

	thisAreaNode->nodeCharacter = malloc(sizeof(character));
	thisAreaNode->nodeCharacter->thisAnimationContainer = initAnimationContainer();
	addAnimationToContainer(thisAreaNode->nodeCharacter->thisAnimationContainer, cloneAnimationFromRegistry(AREA_NODE_IDLE_IMAGE_ID));
	addAnimationToContainer(thisAreaNode->nodeCharacter->thisAnimationContainer, cloneAnimationFromRegistry(AREA_NODE_ACTIVE_IMAGE_ID));

	thisAreaNode->nodeCharacter->secondaryAnimationContainer = cloneAnimationContainer(thisAreaNode->nodeCharacter->thisAnimationContainer);
	thisAreaNode->nodeCharacter->darkAnimationContainer = NULL;
	thisAreaNode->nodeCharacter->direction = 0;
	thisAreaNode->nodeCharacter->xOff = 0;
	thisAreaNode->nodeCharacter->yOff = 0;

	value = strtok_r(line, ";", &strtok_line_pointer);
	thisAreaNode->id = atoi(line);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->nodeCharacter->x = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->nodeCharacter->y = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->mapID = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->mapTransitID = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->isHidden = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->LinkedNodeIDs[0] = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->LinkedNodeIDs[1] = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->LinkedNodeIDs[2] = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->LinkedNodeIDs[3] = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->LinkedNodeIDs[4] = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->LinkedNodeIDs[5] = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->LinkedNodeIDs[6] = atoi(value);

	value = strtok_r(NULL, ";", &strtok_line_pointer);
	thisAreaNode->LinkedNodeIDs[7] = atoi(value);

	return thisAreaNode;
}

char * getAreaNodeAsLine(areaNode * thisAreaNode){
	char * toReturn = malloc(sizeof(char) * 128);
	int i = 0;

	i += sprintf(toReturn + i, "%d;", thisAreaNode->id);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->nodeCharacter->x);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->nodeCharacter->y);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->mapID);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->mapTransitID);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->isHidden);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->LinkedNodeIDs[0]);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->LinkedNodeIDs[1]);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->LinkedNodeIDs[2]);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->LinkedNodeIDs[3]);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->LinkedNodeIDs[4]);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->LinkedNodeIDs[5]);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->LinkedNodeIDs[6]);
	i += sprintf(toReturn + i, "%d;", thisAreaNode->LinkedNodeIDs[7]);

	return toReturn;
}
