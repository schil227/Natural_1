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
	thisWorldMapController->numSteppingStoneCords = 0;
	thisWorldMapController->MAX_STEPPING_STONE_CORDS = 2000;
	thisWorldMapController->numActiveSteppingStones = 0;
	thisWorldMapController->MAX_ACTIVE_STEPPING_STONES = 100;

	thisWorldMapController->currentAreaNode = NULL;
	thisWorldMapController->numAreaNodes = getAllAreaNodesFromRegistry(thisWorldMapController->allNodes, thisWorldMapController->MAX_AREA_NODES);

	thisWorldMapController->steppingStoneIdle = createCharacterFromAnimation(getAnimationFromRegistry(STEPPING_STONE_IDLE_IMAGE_ID));
	thisWorldMapController->steppingStonesPerCords = 7;
}

void destroyWorldMapController(){
	while(!tryGetWorldMapReadLock()){}
	while(!tryGetWorldMapWriteLock()){}

	destroyFixedCharacter(thisWorldMapController->worldMap);
	destroyCharacter(thisWorldMapController->steppingStoneIdle);
	free(thisWorldMapController);

	releaseWorldMapWriteLock();
	releaseWorldMapReadLock();
}

void enableWorldMapMode(int areaNodeID){
	while(!tryGetWorldMapReadLock()){}
	while(!tryGetWorldMapWriteLock()){}

	thisWorldMapController->inWorldMapMode = 1;
	thisWorldMapController->currentAreaNode = getAreaNodeFromRegistry(areaNodeID);
	thisWorldMapController->currentAreaNode->playerOccupied = 1;
	thisWorldMapController->currentAreaNode->isHidden = 0;
	setCurrentNeighborNodeAnimations(thisWorldMapController->currentAreaNode, 1);
	updateSteppingStones();

	releaseWorldMapWriteLock();
	releaseWorldMapReadLock();
}

void disableWorldMapMode(){
	int i;

	while(!tryGetWorldMapReadLock()){}
	while(!tryGetWorldMapWriteLock()){}

	setCurrentNeighborNodeAnimations(thisWorldMapController->currentAreaNode, 0);
	thisWorldMapController->inWorldMapMode = 0;

	for(i = 0; i < thisWorldMapController->numSteppingStoneCords; i++){
		free(thisWorldMapController->steppingStoneCords[i]);
	}

	thisWorldMapController->numSteppingStoneCords = 0;

	releaseWorldMapWriteLock();
	releaseWorldMapReadLock();
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

	while(!tryGetWorldMapReadLock()){}

	//TODO: update bounds for scrolling
	drawUnboundCharacterByPixels(hdc, hdcBuffer, 0 + viewShift->xShift, 0 + viewShift->yShift, thisWorldMapController->worldMap);

	for(i = 0; i < thisWorldMapController->numSteppingStoneCords; i++){
		drawUnboundAnimationByPixels(hdc, hdcBuffer, thisWorldMapController->steppingStoneIdle, thisWorldMapController->steppingStoneCords[i]->x + 5, thisWorldMapController->steppingStoneCords[i]->y + 5, 0);
	}

	for(i = 0; i < thisWorldMapController->numActiveSteppingStones; i++){
		drawUnboundAnimationByPixels(hdc, hdcBuffer, thisWorldMapController->activeSteppingStones[i], thisWorldMapController->activeSteppingStones[i]->x + 5, thisWorldMapController->activeSteppingStones[i]->y + 5, 0);
		updateAnimation(thisWorldMapController->activeSteppingStones[i]);
	}

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

	releaseWorldMapReadLock();
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

	if(newAreaNode->isHidden){
		return;
	}

	while(!tryGetWorldMapReadLock()){}
	while(!tryGetWorldMapWriteLock()){}

	thisWorldMapController->currentAreaNode->playerOccupied = 0;
	setCurrentNeighborNodeAnimations(thisWorldMapController->currentAreaNode, 0);
	thisWorldMapController->currentAreaNode = newAreaNode;
	thisWorldMapController->currentAreaNode->playerOccupied = 1;

	setCurrentNeighborNodeAnimations(thisWorldMapController->currentAreaNode, 1);
	updateSteppingStones();

	releaseWorldMapWriteLock();
	releaseWorldMapReadLock();
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

int distanceBetweenTwoCordsInPixels(int x1, int x2, int y1, int y2){
	double width = abs(x1 - x2);
	double height = abs(y1 - y2);

	return (int)floor(sqrt(pow(width, 2) + pow(height, 2)));
}

cordArr * getCordsSpacedOverLine(int x1, int y1, int x2, int y2, int cordsPerLine){
	int i, xSum = 0, ySum = 0;
	int lineLength = distanceBetweenTwoCordsInPixels(x1, x2, y1, y2);
	int xStep = (x2 - x1) / cordsPerLine;
	int yStep = (y2 - y1) / cordsPerLine;

	cordArr * cordsOnLine = initCordArr();

	for(i = 0; i < cordsPerLine - 1; i++){
		cord * newCord = malloc(sizeof(cord));
		xSum += xStep;
		ySum += yStep;

		newCord->x = xSum + x1;
		newCord->y = ySum + y1;

		addCordIfUnique(cordsOnLine, newCord);
	}

	return cordsOnLine;
}

//void updateActiveSteppingStones(){
//	int i, j, id;
//
//	cordArr * nodeCombinations = initCordArr();
//
//	//destroy previous cords
//	for(i = 0; i < thisWorldMapController->numActiveSteppingStones; i++){
//		destroyCharacter(thisWorldMapController->activeSteppingStones[i]);
//	}
//
//	thisWorldMapController->numActiveSteppingStones = 0;
//
//	for(i = 0; i < thisWorldMapController->currentAreaNode->MAX_NODES; i++){
//		if(thisWorldMapController->numActiveSteppingStones == thisWorldMapController->MAX_ACTIVE_STEPPING_STONES){
//			break;
//		}
//
//		id = thisWorldMapController->currentAreaNode->LinkedNodeIDs[i];
//
//		if(id != 0){
//			areaNode * tmpNode = getAreaNodeFromRegistry(id);
//
//			if(!tmpNode->isHidden){
//				int distanceBetweenChords = distanceBetweenTwoCordsInPixels(thisWorldMapController->currentAreaNode->nodeCharacter->x, thisWorldMapController->currentAreaNode->nodeCharacter->y, tmpNode->nodeCharacter->x, tmpNode->nodeCharacter->y);
//
//				cordArr * steppingStoneCords = getCordsSpacedOverLine(thisWorldMapController->currentAreaNode->nodeCharacter->x,
//						thisWorldMapController->currentAreaNode->nodeCharacter->y,
//						tmpNode->nodeCharacter->x,
//						tmpNode->nodeCharacter->y,
//						thisWorldMapController->steppingStonesPerCords);
//
//				if(steppingStoneCords == NULL){
//					continue;
//				}
//
//				int currentAnimationIndex = 0;
//
//				for(j = 0; j < steppingStoneCords->numCords; j++){
//					if(thisWorldMapController->numActiveSteppingStones == thisWorldMapController->MAX_ACTIVE_STEPPING_STONES){
//						free(steppingStoneCords);
//						break;
//					}
//
//					character * steppingStoneCharacter = createCharacterFromAnimation(cloneAnimation(getAnimationFromRegistry(STEPPING_STONE_ACTIVE_IMAGE_ID)));
//					steppingStoneCharacter->x = steppingStoneCords->cords[j]->x;
//					steppingStoneCharacter->y = steppingStoneCords->cords[j]->y;
//
//					steppingStoneCharacter->thisAnimationContainer->animations[0]->currentFrame = currentAnimationIndex;
//					currentAnimationIndex = (currentAnimationIndex + 1) % steppingStoneCharacter->thisAnimationContainer->animations[0]->numFrames;
//
//					thisWorldMapController->activeSteppingStones[thisWorldMapController->numActiveSteppingStones] = steppingStoneCharacter;
//					thisWorldMapController->numActiveSteppingStones++;
//				}
//
//				free(steppingStoneCords);
//			}
//		}
//	}
//}

void updateSteppingStones(){
	int i, j, k, id, currentNodeID = thisWorldMapController->currentAreaNode->id;
	cordArr * cordPairs = initCordArr();

	//destroy previous cords
	for(i = 0; i < thisWorldMapController->numActiveSteppingStones; i++){
		destroyCharacter(thisWorldMapController->activeSteppingStones[i]);
	}

	thisWorldMapController->numActiveSteppingStones = 0;

	for(i = 0; i < thisWorldMapController->numSteppingStoneCords; i++){
		free(thisWorldMapController->steppingStoneCords[i]);
	}

	thisWorldMapController->numSteppingStoneCords = 0;

	cordArr * nodeCombinations = initCordArr();

	for(i = 0; i < thisWorldMapController->currentAreaNode->MAX_NODES; i++){
		if(thisWorldMapController->numActiveSteppingStones == thisWorldMapController->MAX_ACTIVE_STEPPING_STONES){
			break;
		}

		id = thisWorldMapController->currentAreaNode->LinkedNodeIDs[i];

		if(id != 0){
			areaNode * tmpNode = getAreaNodeFromRegistry(id);

			if(!tmpNode->isHidden){
				int distanceBetweenChords = distanceBetweenTwoCordsInPixels(thisWorldMapController->currentAreaNode->nodeCharacter->x, thisWorldMapController->currentAreaNode->nodeCharacter->y, tmpNode->nodeCharacter->x, tmpNode->nodeCharacter->y);

				cordArr * steppingStoneCords = getCordsSpacedOverLine(thisWorldMapController->currentAreaNode->nodeCharacter->x,
						thisWorldMapController->currentAreaNode->nodeCharacter->y,
						tmpNode->nodeCharacter->x,
						tmpNode->nodeCharacter->y,
						thisWorldMapController->steppingStonesPerCords);

				if(steppingStoneCords == NULL){
					continue;
				}

				int currentAnimationIndex = 0;

				for(j = 0; j < steppingStoneCords->numCords; j++){
					if(thisWorldMapController->numActiveSteppingStones == thisWorldMapController->MAX_ACTIVE_STEPPING_STONES){
						free(steppingStoneCords);
						break;
					}

					character * steppingStoneCharacter = createCharacterFromAnimation(cloneAnimation(getAnimationFromRegistry(STEPPING_STONE_ACTIVE_IMAGE_ID)));
					steppingStoneCharacter->x = steppingStoneCords->cords[j]->x;
					steppingStoneCharacter->y = steppingStoneCords->cords[j]->y;

					steppingStoneCharacter->thisAnimationContainer->animations[0]->currentFrame = currentAnimationIndex;
					currentAnimationIndex = (currentAnimationIndex + 1) % steppingStoneCharacter->thisAnimationContainer->animations[0]->numFrames;

					thisWorldMapController->activeSteppingStones[thisWorldMapController->numActiveSteppingStones] = steppingStoneCharacter;
					thisWorldMapController->numActiveSteppingStones++;
				}

				free(steppingStoneCords);
			}
		}
	}

	for(i = 0; i < thisWorldMapController->numAreaNodes; i++){
		if(thisWorldMapController->numSteppingStoneCords == thisWorldMapController->MAX_STEPPING_STONE_CORDS){
			break;
		}

		areaNode * thisAreaNode = thisWorldMapController->allNodes[i];

		if(thisAreaNode->isHidden || thisAreaNode->id == currentNodeID){
			continue;
		}

		for(j = 0; j < thisAreaNode->MAX_NODES; j++){
			if(thisWorldMapController->numSteppingStoneCords == thisWorldMapController->MAX_STEPPING_STONE_CORDS){
				break;
			}

			id = thisAreaNode->LinkedNodeIDs[j];

			if(id != 0 && id != currentNodeID){
				areaNode * tmpNode = getAreaNodeFromRegistry(id);

				if(!tmpNode->isHidden && !cordArrContainsCoordinates(cordPairs, thisAreaNode->id, tmpNode->id) && !cordArrContainsCoordinates(cordPairs, tmpNode->id, thisAreaNode->id)){
					cord * pair = malloc(sizeof(cord));
					pair->x = thisAreaNode->id;
					pair->y = tmpNode->id;

					addCordIfUnique(cordPairs, pair);

					int distanceBetweenChords = distanceBetweenTwoCordsInPixels(thisWorldMapController->currentAreaNode->nodeCharacter->x, thisWorldMapController->currentAreaNode->nodeCharacter->y, tmpNode->nodeCharacter->x, tmpNode->nodeCharacter->y);

					cordArr * steppingStoneCords = getCordsSpacedOverLine(thisAreaNode->nodeCharacter->x,
							thisAreaNode->nodeCharacter->y,
							tmpNode->nodeCharacter->x,
							tmpNode->nodeCharacter->y,
							thisWorldMapController->steppingStonesPerCords);

					if(steppingStoneCords == NULL){
						continue;
					}

					for(k = 0; k < steppingStoneCords->numCords; k++){
						if(thisWorldMapController->numSteppingStoneCords == thisWorldMapController->MAX_STEPPING_STONE_CORDS){
							free(steppingStoneCords);
							break;
						}

						thisWorldMapController->steppingStoneCords[thisWorldMapController->numSteppingStoneCords] = steppingStoneCords->cords[k];
						thisWorldMapController->numSteppingStoneCords++;
					}

					free(steppingStoneCords);
				}
			}
		}
	}

	destroyCordArr(cordPairs);
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
